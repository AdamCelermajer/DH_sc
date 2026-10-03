
# _GLOBAL__I_.._.._src_Common_b2Math.cpp
007e3b88: ldr      ip, [pc, #0x5c]
007e3b8c: ldr      r2, [pc, #0x5c]
007e3b90: ldr      r3, [pc, #0x5c]
007e3b94: push     {r4, r5, r6}
007e3b98: add      ip, pc, ip
007e3b9c: ldr      r3, [ip, r3]
007e3ba0: ldr      r6, [ip, r2]
007e3ba4: mov      r4, #0
007e3ba8: mov      r2, #0x3f800000
007e3bac: str      r4, [r3, #8]
007e3bb0: str      r2, [r3, #0xc]
007e3bb4: str      r2, [r3]
007e3bb8: str      r4, [r3, #4]
007e3bbc: add      r5, r6, #8
007e3bc0: ldm      r3, {r0, r1, r2, r3}
007e3bc4: stm      r5, {r0, r1, r2, r3}
007e3bc8: ldr      r3, [pc, #0x28]
007e3bcc: mov      r2, #0
007e3bd0: str      r2, [r6]
007e3bd4: ldr      r3, [ip, r3]
007e3bd8: str      r2, [r6, #4]
007e3bdc: str      r4, [r3, #4]
007e3be0: str      r4, [r3]
007e3be4: pop      {r4, r5, r6}
007e3be8: bx       lr
007e3bec: ldrsheq  r0, [fp], -r8
007e3bf0: andeq    r2, r0, ip, lsl r2
007e3bf4: andeq    r0, r0, r0, ror #28
007e3bf8: andeq    r0, r0, r0, asr #18

# _ZL15ComputeCentroidPK6b2Vec2i
007e44f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e44f8: cmp      r2, #0
007e44fc: mov      r7, #0
007e4500: sub      sp, sp, #0x14
007e4504: str      r1, [sp, #0xc]
007e4508: mov      r4, r0
007e450c: str      r7, [r0]
007e4510: str      r7, [r0, #4]
007e4514: strle    r7, [sp, #8]
007e4518: ble      #0x7e4638
007e451c: ldr      r3, [sp, #0xc]
007e4520: mov      r6, #0
007e4524: str      r7, [sp, #8]
007e4528: add      r5, r3, #8
007e452c: ldr      r3, [sp, #0xc]
007e4530: add      r6, r6, #1
007e4534: cmp      r2, r6
007e4538: movgt    r3, r5
007e453c: ldr      sl, [r3, #4]
007e4540: ldr      fp, [r5, #-8]
007e4544: ldr      sb, [r3]
007e4548: mov      r1, sl
007e454c: mov      r0, fp
007e4550: str      r2, [sp]
007e4554: bl       #0x30ed6c
007e4558: mov      r1, sb
007e455c: mov      r8, r0
007e4560: ldr      r0, [r5, #-4]
007e4564: bl       #0x30ed6c
007e4568: mov      r1, r0
007e456c: mov      r0, r8
007e4570: bl       #0x30e3ac
007e4574: mov      r1, #0x3f000000
007e4578: bl       #0x30ed6c
007e457c: mov      r8, r0
007e4580: mov      r1, r8
007e4584: ldr      r0, [sp, #8]
007e4588: bl       #0x30eba4
007e458c: movw     r1, #0xaaab
007e4590: str      r0, [sp, #8]
007e4594: movt     r1, #0x3eaa
007e4598: mov      r0, r8
007e459c: bl       #0x30ed6c
007e45a0: mov      r1, #0
007e45a4: mov      r8, r0
007e45a8: mov      r0, fp
007e45ac: bl       #0x30eba4
007e45b0: mov      r1, #0
007e45b4: mov      fp, r0
007e45b8: ldr      r0, [r5, #-4]
007e45bc: bl       #0x30eba4
007e45c0: mov      r1, sb
007e45c4: mov      r3, r0
007e45c8: mov      r0, fp
007e45cc: str      r3, [sp, #4]
007e45d0: bl       #0x30eba4
007e45d4: ldr      r3, [sp, #4]
007e45d8: mov      r1, sl
007e45dc: mov      sb, r0
007e45e0: mov      r0, r3
007e45e4: bl       #0x30eba4
007e45e8: mov      r1, sb
007e45ec: mov      sl, r0
007e45f0: mov      r0, r8
007e45f4: bl       #0x30ed6c
007e45f8: mov      r1, r0
007e45fc: mov      r0, r7
007e4600: bl       #0x30eba4
007e4604: mov      r1, sl
007e4608: str      r0, [r4]
007e460c: mov      r7, r0
007e4610: mov      r0, r8
007e4614: bl       #0x30ed6c
007e4618: mov      r1, r0
007e461c: ldr      r0, [r4, #4]
007e4620: bl       #0x30eba4
007e4624: ldr      r2, [sp]
007e4628: add      r5, r5, #8
007e462c: str      r0, [r4, #4]
007e4630: cmp      r6, r2
007e4634: bne      #0x7e452c
007e4638: ldr      r1, [sp, #8]
007e463c: mov      r0, #0x3f800000
007e4640: bl       #0x30ec94
007e4644: mov      r1, r7
007e4648: mov      r5, r0
007e464c: bl       #0x30ed6c
007e4650: mov      r1, r5
007e4654: str      r0, [r4]
007e4658: ldr      r0, [r4, #4]
007e465c: bl       #0x30ed6c
007e4660: str      r0, [r4, #4]
007e4664: mov      r0, r4
007e4668: add      sp, sp, #0x14
007e466c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZL10ComputeOBBP5b2OBBPK6b2Vec2i
007e55ac: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e55b0: cmp      r2, #0
007e55b4: sub      sp, sp, #0x8c
007e55b8: str      r2, [sp, #0x30]
007e55bc: str      r0, [sp, #0x24]
007e55c0: ble      #0x7e5920
007e55c4: lsl      r2, r2, #3
007e55c8: add      r3, sp, #0x38
007e55cc: str      r2, [sp, #0x1c]
007e55d0: mov      r0, #0
007e55d4: str      r3, [sp, #0x18]
007e55d8: mov      lr, r3
007e55dc: mov      r4, r2
007e55e0: mov      r2, r1
007e55e4: ldr      ip, [r2, r0]!
007e55e8: mov      r3, lr
007e55ec: str      ip, [r3, r0]!
007e55f0: ldr      r2, [r2, #4]
007e55f4: add      r0, r0, #8
007e55f8: cmp      r0, r4
007e55fc: str      r2, [r3, #4]
007e5600: bne      #0x7e55e0
007e5604: ldr      r1, [sp, #0x1c]
007e5608: add      r2, sp, #0x88
007e560c: add      r3, r2, r1
007e5610: ldr      r2, [sp, #0x38]
007e5614: mvn      r1, #0x80000000
007e5618: sub      r1, r1, #0x800000
007e561c: str      r2, [r3, #-0x50]
007e5620: ldr      r2, [sp, #0x18]
007e5624: str      r1, [sp, #0x2c]
007e5628: mov      r1, #1
007e562c: add      r2, r2, #4
007e5630: str      r2, [sp, #0x20]
007e5634: ldr      r2, [sp, #0x3c]
007e5638: str      r1, [sp, #0x28]
007e563c: str      r2, [r3, #-0x4c]
007e5640: add      r2, sp, #0x80
007e5644: str      r2, [sp, #0x34]
007e5648: ldr      r3, [sp, #0x20]
007e564c: ldr      r1, [sp, #0x20]
007e5650: ldr      r2, [sp, #0x20]
007e5654: ldr      r3, [r3]
007e5658: mvn      sl, #0x80000000
007e565c: sub      sl, sl, #0x800000
007e5660: str      r3, [sp, #0xc]
007e5664: ldr      r1, [r1, #-4]
007e5668: mvn      sb, #0x800000
007e566c: mov      r5, #0
007e5670: str      r1, [sp, #8]
007e5674: ldr      r0, [r2, #8]
007e5678: mov      r1, r3
007e567c: bl       #0x30e3ac
007e5680: ldr      r3, [sp, #0x20]
007e5684: ldr      r1, [sp, #8]
007e5688: mov      r4, r0
007e568c: ldr      r0, [r3, #4]
007e5690: bl       #0x30e3ac
007e5694: str      r0, [sp, #0x80]
007e5698: ldr      r0, [sp, #0x34]
007e569c: str      r4, [sp, #0x84]
007e56a0: bl       #0x7e5524
007e56a4: ldr      r1, [sp, #0x84]
007e56a8: ldr      r8, [sp, #0x80]
007e56ac: str      sb, [sp, #4]
007e56b0: str      r1, [sp, #0x14]
007e56b4: add      r1, r1, #0x80000000
007e56b8: str      r1, [sp, #0x10]
007e56bc: mov      fp, sl
007e56c0: ldr      r4, [sp, #0x18]
007e56c4: ldr      r1, [sp, #8]
007e56c8: ldr      r0, [r4, r5]!
007e56cc: bl       #0x30e3ac
007e56d0: ldr      r1, [sp, #0xc]
007e56d4: mov      r7, r0
007e56d8: ldr      r0, [r4, #4]
007e56dc: bl       #0x30e3ac
007e56e0: mov      r1, r7
007e56e4: mov      r6, r0
007e56e8: mov      r0, r8
007e56ec: bl       #0x30ed6c
007e56f0: mov      r1, r6
007e56f4: mov      r4, r0
007e56f8: ldr      r0, [sp, #0x14]
007e56fc: bl       #0x30ed6c
007e5700: mov      r1, r0
007e5704: mov      r0, r4
007e5708: bl       #0x30eba4
007e570c: mov      r1, r7
007e5710: mov      r4, r0
007e5714: ldr      r0, [sp, #0x10]
007e5718: bl       #0x30ed6c
007e571c: mov      r1, r6
007e5720: mov      r7, r0
007e5724: mov      r0, r8
007e5728: bl       #0x30ed6c
007e572c: mov      r1, r0
007e5730: mov      r0, r7
007e5734: bl       #0x30eba4
007e5738: mov      r1, r4
007e573c: mov      r6, r0
007e5740: mov      r0, sl
007e5744: bl       #0x30e70c
007e5748: mov      r1, r6
007e574c: cmp      r0, #0
007e5750: mov      r0, fp
007e5754: moveq    sl, r4
007e5758: bl       #0x30e70c
007e575c: mov      r1, r4
007e5760: cmp      r0, #0
007e5764: mov      r0, sb
007e5768: moveq    fp, r6
007e576c: bl       #0x30e2f8
007e5770: mov      r1, r6
007e5774: cmp      r0, #0
007e5778: ldr      r0, [sp, #4]
007e577c: moveq    sb, r4
007e5780: bl       #0x30e2f8
007e5784: ldr      r2, [sp, #0x1c]
007e5788: cmp      r0, #0
007e578c: add      r5, r5, #8
007e5790: streq    r6, [sp, #4]
007e5794: cmp      r5, r2
007e5798: bne      #0x7e56c0
007e579c: mov      r1, sl
007e57a0: mov      r0, sb
007e57a4: bl       #0x30e3ac
007e57a8: mov      r1, fp
007e57ac: mov      r5, r0
007e57b0: ldr      r0, [sp, #4]
007e57b4: bl       #0x30e3ac
007e57b8: mov      r4, r0
007e57bc: mov      r1, r4
007e57c0: mov      r0, r5
007e57c4: bl       #0x30ed6c
007e57c8: movw     r1, #0x3333
007e57cc: mov      r6, r0
007e57d0: movt     r1, #0x3f73
007e57d4: ldr      r0, [sp, #0x2c]
007e57d8: bl       #0x30ed6c
007e57dc: mov      r1, r6
007e57e0: bl       #0x30e2f8
007e57e4: cmp      r0, #0
007e57e8: beq      #0x7e58fc
007e57ec: ldr      r3, [sp, #0x80]
007e57f0: ldr      r2, [sp, #0x84]
007e57f4: ldr      r1, [sp, #0x24]
007e57f8: mov      r0, sb
007e57fc: str      r3, [r1]
007e5800: str      r8, [r1, #0xc]
007e5804: str      r2, [r1, #4]
007e5808: ldr      r2, [sp, #0x10]
007e580c: str      r2, [r1, #8]
007e5810: mov      r1, sl
007e5814: bl       #0x30eba4
007e5818: ldr      r1, [sp, #4]
007e581c: mov      r7, r0
007e5820: mov      r0, fp
007e5824: bl       #0x30eba4
007e5828: mov      r1, #0x3f000000
007e582c: mov      sl, r0
007e5830: mov      r0, r7
007e5834: bl       #0x30ed6c
007e5838: mov      r1, #0x3f000000
007e583c: mov      r7, r0
007e5840: mov      r0, sl
007e5844: bl       #0x30ed6c
007e5848: ldr      r3, [sp, #0x24]
007e584c: mov      sl, r0
007e5850: mov      r0, r7
007e5854: ldr      r1, [r3]
007e5858: bl       #0x30ed6c
007e585c: mov      r1, sl
007e5860: mov      sb, r0
007e5864: ldr      r0, [sp, #0x10]
007e5868: bl       #0x30ed6c
007e586c: mov      r1, r0
007e5870: mov      r0, sb
007e5874: bl       #0x30eba4
007e5878: ldr      r2, [sp, #0x24]
007e587c: mov      sb, r0
007e5880: mov      r0, r7
007e5884: ldr      r1, [r2, #4]
007e5888: bl       #0x30ed6c
007e588c: mov      r1, sl
007e5890: mov      r7, r0
007e5894: mov      r0, r8
007e5898: bl       #0x30ed6c
007e589c: mov      r1, r0
007e58a0: mov      r0, r7
007e58a4: bl       #0x30eba4
007e58a8: mov      r1, r0
007e58ac: ldr      r0, [sp, #0xc]
007e58b0: bl       #0x30eba4
007e58b4: ldr      r3, [sp, #0x24]
007e58b8: mov      r1, sb
007e58bc: str      r0, [r3, #0x14]
007e58c0: ldr      r0, [sp, #8]
007e58c4: bl       #0x30eba4
007e58c8: ldr      r1, [sp, #0x24]
007e58cc: str      r0, [r1, #0x10]
007e58d0: mov      r1, #0x3f000000
007e58d4: mov      r0, r4
007e58d8: bl       #0x30ed6c
007e58dc: ldr      r2, [sp, #0x24]
007e58e0: mov      r1, #0x3f000000
007e58e4: str      r0, [r2, #0x1c]
007e58e8: mov      r0, r5
007e58ec: bl       #0x30ed6c
007e58f0: ldr      r3, [sp, #0x24]
007e58f4: str      r0, [r3, #0x18]
007e58f8: str      r6, [sp, #0x2c]
007e58fc: ldr      r1, [sp, #0x28]
007e5900: ldr      r3, [sp, #0x20]
007e5904: ldr      r2, [sp, #0x30]
007e5908: add      r1, r1, #1
007e590c: add      r3, r3, #8
007e5910: cmp      r2, r1
007e5914: str      r1, [sp, #0x28]
007e5918: str      r3, [sp, #0x20]
007e591c: bge      #0x7e5648
007e5920: add      sp, sp, #0x8c
007e5924: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _GLOBAL__I_.._.._src_Dynamics_b2WorldCallbacks.cpp
007e8d4c: ldr      r3, [pc, #0x38]
007e8d50: ldr      r2, [pc, #0x38]
007e8d54: str      r4, [sp, #-4]!
007e8d58: add      r3, pc, r3
007e8d5c: ldr      r4, [pc, #0x30]
007e8d60: ldr      ip, [r3, r2]
007e8d64: ldr      r1, [pc, #0x2c]
007e8d68: ldr      r2, [pc, #0x2c]
007e8d6c: ldr      r4, [r3, r4]
007e8d70: ldr      r1, [r3, r1]
007e8d74: ldr      r2, [r3, r2]
007e8d78: add      r4, r4, #8
007e8d7c: mov      r0, ip
007e8d80: str      r4, [ip]
007e8d84: ldm      sp!, {r4}
007e8d88: b        #0x30e304
007e8d8c: andseq   fp, sl, r8, lsr sp
007e8d90: andeq    r1, r0, r0, asr #1
007e8d94: andeq    r2, r0, r8, ror #1
007e8d98: andeq    r4, r0, r0, asr #21
007e8d9c: muleq    r0, r0, r8

# _ZN15b2CircleContact12GetManifoldsEv
007f3a94: add      r0, r0, #0x48
007f3a98: bx       lr

# _ZN13b2NullContact12GetManifoldsEv
007e65c8: mov      r0, #0
007e65cc: bx       lr

# _ZNK16b2PrismaticJoint16GetReactionForceEv
007ee940: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ee944: ldr      r7, [r1, #0x30]
007ee948: ldr      fp, [r1, #0x54]
007ee94c: sub      sp, sp, #0xc
007ee950: ldr      r6, [r7, #0xc]
007ee954: ldr      r8, [r1, #0x58]
007ee958: mov      r4, r1
007ee95c: mov      r5, r0
007ee960: mov      r1, fp
007ee964: mov      r0, r6
007ee968: bl       #0x30ed6c
007ee96c: ldr      sb, [r7, #0x14]
007ee970: mov      sl, r0
007ee974: mov      r1, r8
007ee978: mov      r0, sb
007ee97c: bl       #0x30ed6c
007ee980: mov      r1, r0
007ee984: mov      r0, sl
007ee988: bl       #0x30eba4
007ee98c: ldr      sl, [r7, #0x10]
007ee990: mov      r2, r0
007ee994: mov      r0, fp
007ee998: mov      r1, sl
007ee99c: ldr      r7, [r7, #0x18]
007ee9a0: str      r2, [sp]
007ee9a4: bl       #0x30ed6c
007ee9a8: mov      r1, r7
007ee9ac: mov      fp, r0
007ee9b0: mov      r0, r8
007ee9b4: bl       #0x30ed6c
007ee9b8: mov      r1, r0
007ee9bc: mov      r0, fp
007ee9c0: bl       #0x30eba4
007ee9c4: ldr      r8, [r4, #0x5c]
007ee9c8: mov      r3, r0
007ee9cc: mov      r0, r6
007ee9d0: mov      r1, r8
007ee9d4: ldr      r6, [r4, #0x60]
007ee9d8: str      r3, [sp, #4]
007ee9dc: bl       #0x30ed6c
007ee9e0: mov      r1, r6
007ee9e4: mov      fp, r0
007ee9e8: mov      r0, sb
007ee9ec: bl       #0x30ed6c
007ee9f0: mov      r1, r0
007ee9f4: mov      r0, fp
007ee9f8: bl       #0x30eba4
007ee9fc: mov      r1, r8
007eea00: mov      sb, r0
007eea04: mov      r0, sl
007eea08: bl       #0x30ed6c
007eea0c: mov      r1, r6
007eea10: mov      r8, r0
007eea14: mov      r0, r7
007eea18: bl       #0x30ed6c
007eea1c: mov      r1, r0
007eea20: mov      r0, r8
007eea24: bl       #0x30eba4
007eea28: ldr      r2, [sp]
007eea2c: ldr      r6, [r4, #0xb0]
007eea30: mov      r7, r0
007eea34: mov      r1, r2
007eea38: mov      r0, r6
007eea3c: bl       #0x30ed6c
007eea40: ldr      r3, [sp, #4]
007eea44: mov      r8, r0
007eea48: mov      r0, r6
007eea4c: mov      r1, r3
007eea50: bl       #0x30ed6c
007eea54: ldr      r4, [r4, #0x84]
007eea58: mov      r6, r0
007eea5c: mov      r1, sb
007eea60: mov      r0, r4
007eea64: bl       #0x30ed6c
007eea68: mov      r1, r7
007eea6c: mov      sl, r0
007eea70: mov      r0, r4
007eea74: bl       #0x30ed6c
007eea78: mov      r1, sl
007eea7c: mov      r7, r0
007eea80: mov      r0, r8
007eea84: bl       #0x30eba4
007eea88: mov      r1, r7
007eea8c: mov      r4, r0
007eea90: mov      r0, r6
007eea94: bl       #0x30eba4
007eea98: str      r4, [r5]
007eea9c: str      r0, [r5, #4]
007eeaa0: mov      r0, r5
007eeaa4: add      sp, sp, #0xc
007eeaa8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN16b2PrismaticJoint24SolveVelocityConstraintsERK10b2TimeStep
007ee080: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ee084: ldr      r3, [r1, #4]
007ee088: mov      r4, r0
007ee08c: sub      sp, sp, #0x24
007ee090: ldr      r5, [r0, #0x30]
007ee094: mov      r7, r1
007ee098: add      r0, r3, #0x80000000
007ee09c: ldr      r1, [r4, #0x80]
007ee0a0: bl       #0x30ed6c
007ee0a4: ldr      r1, [r5, #0x40]
007ee0a8: mov      r8, r0
007ee0ac: ldr      r0, [r4, #0x68]
007ee0b0: bl       #0x30ed6c
007ee0b4: ldr      r1, [r5, #0x44]
007ee0b8: mov      sl, r0
007ee0bc: ldr      r0, [r4, #0x6c]
007ee0c0: bl       #0x30ed6c
007ee0c4: mov      r1, r0
007ee0c8: mov      r0, sl
007ee0cc: bl       #0x30eba4
007ee0d0: ldr      r1, [r4, #0x70]
007ee0d4: mov      sl, r0
007ee0d8: ldr      r0, [r5, #0x48]
007ee0dc: bl       #0x30ed6c
007ee0e0: mov      r1, r0
007ee0e4: mov      r0, sl
007ee0e8: bl       #0x30eba4
007ee0ec: ldr      r6, [r4, #0x34]
007ee0f0: mov      sl, r0
007ee0f4: ldr      r0, [r4, #0x74]
007ee0f8: ldr      r1, [r6, #0x40]
007ee0fc: bl       #0x30ed6c
007ee100: ldr      r1, [r6, #0x44]
007ee104: mov      sb, r0
007ee108: ldr      r0, [r4, #0x78]
007ee10c: bl       #0x30ed6c
007ee110: mov      r1, r0
007ee114: mov      r0, sb
007ee118: bl       #0x30eba4
007ee11c: mov      r1, r0
007ee120: mov      r0, sl
007ee124: bl       #0x30eba4
007ee128: ldr      r1, [r4, #0x7c]
007ee12c: mov      sl, r0
007ee130: ldr      r0, [r6, #0x48]
007ee134: bl       #0x30ed6c
007ee138: mov      r1, r0
007ee13c: mov      r0, sl
007ee140: bl       #0x30eba4
007ee144: mov      r1, r0
007ee148: mov      r0, r8
007ee14c: bl       #0x30ed6c
007ee150: mov      r8, r0
007ee154: mov      r1, r0
007ee158: ldr      r0, [r4, #0x84]
007ee15c: bl       #0x30eba4
007ee160: ldr      r2, [r6, #0x78]
007ee164: ldr      sb, [r5, #0x80]
007ee168: ldr      sl, [r6, #0x80]
007ee16c: ldr      fp, [r5, #0x78]
007ee170: str      r2, [sp, #0xc]
007ee174: str      r0, [r4, #0x84]
007ee178: ldr      r1, [r7]
007ee17c: mov      r0, r8
007ee180: bl       #0x30ed6c
007ee184: mov      r8, r0
007ee188: mov      r1, r8
007ee18c: mov      r0, fp
007ee190: bl       #0x30ed6c
007ee194: ldr      r1, [r4, #0x6c]
007ee198: str      r0, [sp, #4]
007ee19c: bl       #0x30ed6c
007ee1a0: ldr      r3, [sp, #4]
007ee1a4: ldr      r1, [r4, #0x68]
007ee1a8: mov      r2, r0
007ee1ac: mov      r0, r3
007ee1b0: str      r2, [sp, #8]
007ee1b4: bl       #0x30ed6c
007ee1b8: mov      r1, r0
007ee1bc: ldr      r0, [r5, #0x40]
007ee1c0: bl       #0x30eba4
007ee1c4: str      r0, [r5, #0x40]
007ee1c8: ldr      r2, [sp, #8]
007ee1cc: ldr      r0, [r5, #0x44]
007ee1d0: mov      r1, r2
007ee1d4: bl       #0x30eba4
007ee1d8: mov      r1, r8
007ee1dc: str      r0, [r5, #0x44]
007ee1e0: mov      r0, sb
007ee1e4: bl       #0x30ed6c
007ee1e8: ldr      r1, [r4, #0x70]
007ee1ec: bl       #0x30ed6c
007ee1f0: mov      r1, r0
007ee1f4: ldr      r0, [r5, #0x48]
007ee1f8: bl       #0x30eba4
007ee1fc: str      r0, [r5, #0x48]
007ee200: ldr      r0, [sp, #0xc]
007ee204: mov      r1, r8
007ee208: bl       #0x30ed6c
007ee20c: ldr      r1, [r4, #0x78]
007ee210: str      r0, [sp, #4]
007ee214: bl       #0x30ed6c
007ee218: ldr      r3, [sp, #4]
007ee21c: ldr      r1, [r4, #0x74]
007ee220: mov      r2, r0
007ee224: mov      r0, r3
007ee228: str      r2, [sp, #8]
007ee22c: bl       #0x30ed6c
007ee230: mov      r1, r0
007ee234: ldr      r0, [r6, #0x40]
007ee238: bl       #0x30eba4
007ee23c: str      r0, [r6, #0x40]
007ee240: ldr      r2, [sp, #8]
007ee244: ldr      r0, [r6, #0x44]
007ee248: mov      r1, r2
007ee24c: bl       #0x30eba4
007ee250: mov      r1, r8
007ee254: str      r0, [r6, #0x44]
007ee258: mov      r0, sl
007ee25c: bl       #0x30ed6c
007ee260: ldr      r1, [r4, #0x7c]
007ee264: bl       #0x30ed6c
007ee268: ldr      r1, [r6, #0x48]
007ee26c: bl       #0x30eba4
007ee270: mov      r8, r0
007ee274: str      r0, [r6, #0x48]
007ee278: ldr      r0, [r7, #4]
007ee27c: ldr      r1, [r4, #0x88]
007ee280: add      r0, r0, #0x80000000
007ee284: bl       #0x30ed6c
007ee288: ldr      r1, [r5, #0x48]
007ee28c: mov      r3, r0
007ee290: mov      r0, r8
007ee294: str      r3, [sp, #4]
007ee298: bl       #0x30e3ac
007ee29c: ldr      r3, [sp, #4]
007ee2a0: mov      r1, r0
007ee2a4: mov      r0, r3
007ee2a8: bl       #0x30ed6c
007ee2ac: mov      r8, r0
007ee2b0: mov      r1, r0
007ee2b4: ldr      r0, [r4, #0x8c]
007ee2b8: bl       #0x30eba4
007ee2bc: str      r0, [r4, #0x8c]
007ee2c0: ldr      r1, [r7]
007ee2c4: mov      r0, r8
007ee2c8: bl       #0x30ed6c
007ee2cc: mov      r8, r0
007ee2d0: mov      r1, r8
007ee2d4: mov      r0, sb
007ee2d8: bl       #0x30ed6c
007ee2dc: mov      r1, r0
007ee2e0: ldr      r0, [r5, #0x48]
007ee2e4: bl       #0x30e3ac
007ee2e8: mov      r1, r8
007ee2ec: str      r0, [r5, #0x48]
007ee2f0: mov      r0, sl
007ee2f4: bl       #0x30ed6c
007ee2f8: ldr      r1, [r6, #0x48]
007ee2fc: bl       #0x30eba4
007ee300: str      r0, [r6, #0x48]
007ee304: ldrb     r3, [r4, #0xc9]
007ee308: mov      r8, r0
007ee30c: cmp      r3, #0
007ee310: beq      #0x7ee568
007ee314: ldr      r3, [r4, #0xcc]
007ee318: cmp      r3, #3
007ee31c: beq      #0x7ee568
007ee320: ldr      r0, [r7, #4]
007ee324: ldr      r3, [r4, #0x90]
007ee328: ldr      r1, [r4, #0xa8]
007ee32c: add      r0, r0, #0x80000000
007ee330: str      r3, [sp, #0x18]
007ee334: bl       #0x30ed6c
007ee338: ldr      r2, [r4, #0x94]
007ee33c: mov      ip, r0
007ee340: ldr      r1, [r5, #0x40]
007ee344: ldr      r0, [sp, #0x18]
007ee348: str      ip, [sp]
007ee34c: str      r2, [sp, #0x14]
007ee350: bl       #0x30ed6c
007ee354: ldr      r1, [r5, #0x44]
007ee358: mov      r3, r0
007ee35c: ldr      r0, [sp, #0x14]
007ee360: str      r3, [sp, #4]
007ee364: bl       #0x30ed6c
007ee368: ldr      r3, [sp, #4]
007ee36c: mov      r1, r0
007ee370: mov      r0, r3
007ee374: bl       #0x30eba4
007ee378: ldr      r1, [r4, #0x98]
007ee37c: mov      r3, r0
007ee380: ldr      r0, [r5, #0x48]
007ee384: str      r3, [sp, #4]
007ee388: bl       #0x30ed6c
007ee38c: ldr      r3, [sp, #4]
007ee390: mov      r1, r0
007ee394: mov      r0, r3
007ee398: bl       #0x30eba4
007ee39c: ldr      r1, [r6, #0x40]
007ee3a0: mov      r2, r0
007ee3a4: ldr      r0, [r4, #0x9c]
007ee3a8: str      r2, [sp, #8]
007ee3ac: bl       #0x30ed6c
007ee3b0: ldr      r1, [r6, #0x44]
007ee3b4: mov      r3, r0
007ee3b8: ldr      r0, [r4, #0xa0]
007ee3bc: str      r3, [sp, #4]
007ee3c0: bl       #0x30ed6c
007ee3c4: ldr      r3, [sp, #4]
007ee3c8: mov      r1, r0
007ee3cc: mov      r0, r3
007ee3d0: bl       #0x30eba4
007ee3d4: ldr      r2, [sp, #8]
007ee3d8: mov      r1, r0
007ee3dc: mov      r0, r2
007ee3e0: bl       #0x30eba4
007ee3e4: ldr      r1, [r4, #0xa4]
007ee3e8: mov      r3, r0
007ee3ec: mov      r0, r8
007ee3f0: str      r3, [sp, #4]
007ee3f4: bl       #0x30ed6c
007ee3f8: ldr      r3, [sp, #4]
007ee3fc: mov      r1, r0
007ee400: mov      r0, r3
007ee404: bl       #0x30eba4
007ee408: ldr      r1, [r4, #0xc4]
007ee40c: bl       #0x30e3ac
007ee410: ldr      ip, [sp]
007ee414: ldr      r3, [r4, #0xac]
007ee418: mov      r1, r0
007ee41c: mov      r0, ip
007ee420: str      r3, [sp, #0x1c]
007ee424: bl       #0x30ed6c
007ee428: ldr      r1, [sp, #0x1c]
007ee42c: bl       #0x30eba4
007ee430: str      r0, [sp, #0x10]
007ee434: ldr      r3, [r4, #0xc0]
007ee438: mov      r1, r3
007ee43c: str      r3, [sp, #4]
007ee440: bl       #0x30e70c
007ee444: ldr      r3, [sp, #4]
007ee448: cmp      r0, #0
007ee44c: add      r8, r3, #0x80000000
007ee450: streq    r3, [sp, #0x10]
007ee454: ldr      r1, [sp, #0x10]
007ee458: mov      r0, r8
007ee45c: bl       #0x30e2f8
007ee460: cmp      r0, #0
007ee464: ldreq    r8, [sp, #0x10]
007ee468: ldr      r1, [sp, #0x1c]
007ee46c: mov      r0, r8
007ee470: str      r8, [r4, #0xac]
007ee474: bl       #0x30e3ac
007ee478: ldr      r1, [r7]
007ee47c: bl       #0x30ed6c
007ee480: mov      r8, r0
007ee484: mov      r1, r8
007ee488: mov      r0, fp
007ee48c: bl       #0x30ed6c
007ee490: ldr      r1, [sp, #0x18]
007ee494: str      r0, [sp, #4]
007ee498: bl       #0x30ed6c
007ee49c: mov      r1, r0
007ee4a0: ldr      r0, [r5, #0x40]
007ee4a4: bl       #0x30eba4
007ee4a8: str      r0, [r5, #0x40]
007ee4ac: ldr      r3, [sp, #4]
007ee4b0: ldr      r1, [sp, #0x14]
007ee4b4: mov      r0, r3
007ee4b8: bl       #0x30ed6c
007ee4bc: mov      r1, r0
007ee4c0: ldr      r0, [r5, #0x44]
007ee4c4: bl       #0x30eba4
007ee4c8: mov      r1, r8
007ee4cc: str      r0, [r5, #0x44]
007ee4d0: mov      r0, sb
007ee4d4: bl       #0x30ed6c
007ee4d8: ldr      r1, [r4, #0x98]
007ee4dc: bl       #0x30ed6c
007ee4e0: mov      r1, r0
007ee4e4: ldr      r0, [r5, #0x48]
007ee4e8: bl       #0x30eba4
007ee4ec: str      r0, [r5, #0x48]
007ee4f0: mov      r1, r8
007ee4f4: ldr      r0, [sp, #0xc]
007ee4f8: bl       #0x30ed6c
007ee4fc: ldr      r1, [r4, #0xa0]
007ee500: str      r0, [sp, #4]
007ee504: bl       #0x30ed6c
007ee508: ldr      r3, [sp, #4]
007ee50c: mov      r2, r0
007ee510: ldr      r1, [r4, #0x9c]
007ee514: mov      r0, r3
007ee518: str      r2, [sp, #8]
007ee51c: bl       #0x30ed6c
007ee520: mov      r1, r0
007ee524: ldr      r0, [r6, #0x40]
007ee528: bl       #0x30eba4
007ee52c: str      r0, [r6, #0x40]
007ee530: ldr      r2, [sp, #8]
007ee534: ldr      r0, [r6, #0x44]
007ee538: mov      r1, r2
007ee53c: bl       #0x30eba4
007ee540: mov      r1, r8
007ee544: str      r0, [r6, #0x44]
007ee548: mov      r0, sl
007ee54c: bl       #0x30ed6c
007ee550: ldr      r1, [r4, #0xa4]
007ee554: bl       #0x30ed6c
007ee558: mov      r1, r0
007ee55c: ldr      r0, [r6, #0x48]
007ee560: bl       #0x30eba4
007ee564: str      r0, [r6, #0x48]
007ee568: ldrb     r3, [r4, #0xc8]
007ee56c: cmp      r3, #0
007ee570: beq      #0x7ee77c
007ee574: ldr      r8, [r4, #0xcc]
007ee578: cmp      r8, #0
007ee57c: beq      #0x7ee77c
007ee580: ldr      r2, [r4, #0x90]
007ee584: ldr      r0, [r7, #4]
007ee588: str      r2, [sp, #0x14]
007ee58c: ldr      r3, [r5, #0x40]
007ee590: add      r0, r0, #0x80000000
007ee594: str      r3, [sp, #0x10]
007ee598: ldr      r1, [r4, #0xa8]
007ee59c: bl       #0x30ed6c
007ee5a0: ldr      r2, [r4, #0x94]
007ee5a4: mov      ip, r0
007ee5a8: ldr      r1, [sp, #0x10]
007ee5ac: ldr      r0, [sp, #0x14]
007ee5b0: str      ip, [sp]
007ee5b4: str      r2, [sp, #0x18]
007ee5b8: bl       #0x30ed6c
007ee5bc: ldr      r1, [r5, #0x44]
007ee5c0: mov      r3, r0
007ee5c4: ldr      r0, [sp, #0x18]
007ee5c8: str      r3, [sp, #4]
007ee5cc: bl       #0x30ed6c
007ee5d0: ldr      r3, [sp, #4]
007ee5d4: mov      r1, r0
007ee5d8: mov      r0, r3
007ee5dc: bl       #0x30eba4
007ee5e0: ldr      r1, [r4, #0x98]
007ee5e4: mov      r3, r0
007ee5e8: ldr      r0, [r5, #0x48]
007ee5ec: str      r3, [sp, #4]
007ee5f0: bl       #0x30ed6c
007ee5f4: ldr      r3, [sp, #4]
007ee5f8: mov      r1, r0
007ee5fc: mov      r0, r3
007ee600: bl       #0x30eba4
007ee604: ldr      r1, [r6, #0x40]
007ee608: mov      r2, r0
007ee60c: ldr      r0, [r4, #0x9c]
007ee610: str      r2, [sp, #8]
007ee614: bl       #0x30ed6c
007ee618: ldr      r1, [r6, #0x44]
007ee61c: mov      r3, r0
007ee620: ldr      r0, [r4, #0xa0]
007ee624: str      r3, [sp, #4]
007ee628: bl       #0x30ed6c
007ee62c: ldr      r3, [sp, #4]
007ee630: mov      r1, r0
007ee634: mov      r0, r3
007ee638: bl       #0x30eba4
007ee63c: ldr      r2, [sp, #8]
007ee640: mov      r1, r0
007ee644: mov      r0, r2
007ee648: bl       #0x30eba4
007ee64c: ldr      r1, [r4, #0xa4]
007ee650: mov      r3, r0
007ee654: ldr      r0, [r6, #0x48]
007ee658: str      r3, [sp, #4]
007ee65c: bl       #0x30ed6c
007ee660: ldr      r3, [sp, #4]
007ee664: mov      r1, r0
007ee668: mov      r0, r3
007ee66c: bl       #0x30eba4
007ee670: ldr      ip, [sp]
007ee674: mov      r1, r0
007ee678: mov      r0, ip
007ee67c: bl       #0x30ed6c
007ee680: cmp      r8, #3
007ee684: mov      r3, r0
007ee688: beq      #0x7ee7fc
007ee68c: cmp      r8, #1
007ee690: beq      #0x7ee784
007ee694: cmp      r8, #2
007ee698: beq      #0x7ee7cc
007ee69c: mov      r0, r3
007ee6a0: ldr      r1, [r7]
007ee6a4: bl       #0x30ed6c
007ee6a8: mov      r7, r0
007ee6ac: mov      r1, r7
007ee6b0: mov      r0, fp
007ee6b4: bl       #0x30ed6c
007ee6b8: ldr      r1, [sp, #0x14]
007ee6bc: mov      r8, r0
007ee6c0: bl       #0x30ed6c
007ee6c4: ldr      r1, [sp, #0x10]
007ee6c8: bl       #0x30eba4
007ee6cc: str      r0, [r5, #0x40]
007ee6d0: ldr      r1, [sp, #0x18]
007ee6d4: mov      r0, r8
007ee6d8: bl       #0x30ed6c
007ee6dc: mov      r1, r0
007ee6e0: ldr      r0, [r5, #0x44]
007ee6e4: bl       #0x30eba4
007ee6e8: mov      r1, r7
007ee6ec: str      r0, [r5, #0x44]
007ee6f0: mov      r0, sb
007ee6f4: bl       #0x30ed6c
007ee6f8: ldr      r1, [r4, #0x98]
007ee6fc: bl       #0x30ed6c
007ee700: mov      r1, r0
007ee704: ldr      r0, [r5, #0x48]
007ee708: bl       #0x30eba4
007ee70c: str      r0, [r5, #0x48]
007ee710: ldr      r0, [sp, #0xc]
007ee714: mov      r1, r7
007ee718: bl       #0x30ed6c
007ee71c: ldr      r1, [r4, #0xa0]
007ee720: mov      r8, r0
007ee724: bl       #0x30ed6c
007ee728: ldr      r1, [r4, #0x9c]
007ee72c: mov      r5, r0
007ee730: mov      r0, r8
007ee734: bl       #0x30ed6c
007ee738: mov      r1, r0
007ee73c: ldr      r0, [r6, #0x40]
007ee740: bl       #0x30eba4
007ee744: mov      r1, r5
007ee748: str      r0, [r6, #0x40]
007ee74c: ldr      r0, [r6, #0x44]
007ee750: bl       #0x30eba4
007ee754: mov      r1, r7
007ee758: str      r0, [r6, #0x44]
007ee75c: mov      r0, sl
007ee760: bl       #0x30ed6c
007ee764: ldr      r1, [r4, #0xa4]
007ee768: bl       #0x30ed6c
007ee76c: mov      r1, r0
007ee770: ldr      r0, [r6, #0x48]
007ee774: bl       #0x30eba4
007ee778: str      r0, [r6, #0x48]
007ee77c: add      sp, sp, #0x24
007ee780: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ee784: ldr      r3, [r4, #0xb0]
007ee788: mov      r1, r3
007ee78c: str      r3, [sp, #4]
007ee790: bl       #0x30eba4
007ee794: mov      r1, #0
007ee798: mov      r8, r0
007ee79c: bl       #0x30e2f8
007ee7a0: cmp      r0, #0
007ee7a4: ldr      r3, [sp, #4]
007ee7a8: beq      #0x7ee7f4
007ee7ac: mov      r1, r3
007ee7b0: str      r8, [r4, #0xb0]
007ee7b4: mov      r0, r8
007ee7b8: bl       #0x30e3ac
007ee7bc: ldr      r2, [r5, #0x40]
007ee7c0: mov      r3, r0
007ee7c4: str      r2, [sp, #0x10]
007ee7c8: b        #0x7ee69c
007ee7cc: ldr      r3, [r4, #0xb0]
007ee7d0: mov      r1, r3
007ee7d4: str      r3, [sp, #4]
007ee7d8: bl       #0x30eba4
007ee7dc: mov      r1, #0
007ee7e0: mov      r8, r0
007ee7e4: bl       #0x30e70c
007ee7e8: cmp      r0, #0
007ee7ec: ldr      r3, [sp, #4]
007ee7f0: bne      #0x7ee7ac
007ee7f4: mov      r8, #0
007ee7f8: b        #0x7ee7ac
007ee7fc: ldr      r0, [r4, #0xb0]
007ee800: mov      r1, r3
007ee804: str      r3, [sp, #4]
007ee808: bl       #0x30eba4
007ee80c: str      r0, [r4, #0xb0]
007ee810: ldr      r2, [r5, #0x40]
007ee814: ldr      r3, [sp, #4]
007ee818: str      r2, [sp, #0x10]
007ee81c: b        #0x7ee69c

# _ZN7b2World8ValidateEv
007e69a0: mov      r3, #0x19000
007e69a4: add      r3, r3, #0x1d8
007e69a8: ldr      r0, [r0, r3]
007e69ac: b        #0x7e2ac4

# _ZN13b2PairManager6CommitEv
007e42e0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e42e4: mov      sl, #0x40000
007e42e8: add      sl, sl, #0x10
007e42ec: ldr      r3, [r0, sl]
007e42f0: ldr      r1, [pc, #0x1b8]
007e42f4: sub      sp, sp, #0xc
007e42f8: cmp      r3, #0
007e42fc: add      r1, pc, r1
007e4300: ldr      fp, [r0]
007e4304: mov      r6, r0
007e4308: str      r1, [sp]
007e430c: ble      #0x7e4430
007e4310: add      r3, r0, #0x30000
007e4314: add      r3, r3, #0x10
007e4318: mov      r8, #0
007e431c: str      r3, [sp, #4]
007e4320: mov      r5, r3
007e4324: mov      r7, r3
007e4328: mov      sb, r8
007e432c: b        #0x7e436c
007e4330: tst      r1, #4
007e4334: bne      #0x7e4460
007e4338: add      r3, sb, #0xc000
007e433c: add      r3, r3, #4
007e4340: lsl      r3, r3, #2
007e4344: strh     r0, [r6, r3]
007e4348: ldrh     r4, [r4, #6]
007e434c: add      r3, r6, r3
007e4350: add      sb, sb, #1
007e4354: strh     r4, [r3, #2]
007e4358: ldr      r3, [r6, sl]
007e435c: add      r8, r8, #1
007e4360: add      r7, r7, #4
007e4364: cmp      r3, r8
007e4368: ble      #0x7e4404
007e436c: ldrh     r1, [r7]
007e4370: ldrh     r2, [r7, #2]
007e4374: mov      r0, r6
007e4378: bl       #0x7e3f68
007e437c: ldrh     r1, [r0, #0xa]
007e4380: mov      r4, r0
007e4384: ldrh     r2, [r4, #6]
007e4388: bic      r3, r1, #1
007e438c: lsl      r3, r3, #0x10
007e4390: tst      r1, #2
007e4394: lsr      r3, r3, #0x10
007e4398: ldrh     r0, [r0, #4]
007e439c: strh     r3, [r4, #0xa]
007e43a0: bne      #0x7e4330
007e43a4: tst      r1, #4
007e43a8: bne      #0x7e4358
007e43ac: ldr      r3, [r6, #4]
007e43b0: add      r0, fp, r0, lsl #4
007e43b4: add      r2, fp, r2, lsl #4
007e43b8: add      r1, r0, #0x48000
007e43bc: add      r2, r2, #0x48000
007e43c0: add      r1, r1, #0x20
007e43c4: add      r2, r2, #0x20
007e43c8: mov      r0, r3
007e43cc: ldr      r1, [r1]
007e43d0: ldr      r3, [r3]
007e43d4: ldr      r2, [r2]
007e43d8: mov      lr, pc
007e43dc: ldr      pc, [r3, #8]
007e43e0: ldrh     r3, [r4, #0xa]
007e43e4: str      r0, [r4]
007e43e8: add      r8, r8, #1
007e43ec: orr      r3, r3, #4
007e43f0: strh     r3, [r4, #0xa]
007e43f4: ldr      r3, [r6, sl]
007e43f8: add      r7, r7, #4
007e43fc: cmp      r3, r8
007e4400: bgt      #0x7e436c
007e4404: cmp      sb, #0
007e4408: beq      #0x7e4430
007e440c: ldr      r2, [sp, #4]
007e4410: add      sb, r2, sb, lsl #2
007e4414: ldrh     r1, [r5]
007e4418: ldrh     r2, [r5, #2]
007e441c: mov      r0, r6
007e4420: add      r5, r5, #4
007e4424: bl       #0x7e4084
007e4428: cmp      r5, sb
007e442c: bne      #0x7e4414
007e4430: ldr      r1, [sp]
007e4434: ldr      r2, [pc, #0x78]
007e4438: mov      r3, #0x40000
007e443c: add      r3, r3, #0x10
007e4440: ldr      r2, [r1, r2]
007e4444: mov      r1, #0
007e4448: str      r1, [r6, r3]
007e444c: ldrb     r3, [r2]
007e4450: cmp      r3, r1
007e4454: bne      #0x7e44a0
007e4458: add      sp, sp, #0xc
007e445c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e4460: ldr      r3, [r6, #4]
007e4464: add      r0, fp, r0, lsl #4
007e4468: add      r2, fp, r2, lsl #4
007e446c: add      r1, r0, #0x48000
007e4470: add      r2, r2, #0x48000
007e4474: add      r1, r1, #0x20
007e4478: add      r2, r2, #0x20
007e447c: mov      r0, r3
007e4480: ldr      ip, [r3]
007e4484: ldr      r1, [r1]
007e4488: ldr      r2, [r2]
007e448c: ldr      r3, [r4]
007e4490: mov      lr, pc
007e4494: ldr      pc, [ip, #0xc]
007e4498: ldrh     r0, [r4, #4]
007e449c: b        #0x7e4338
007e44a0: mov      r0, r6
007e44a4: add      sp, sp, #0xc
007e44a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e44ac: b        #0x7e42dc
007e44b0: mulseq   fp, r4, r7
007e44b4: andeq    r2, r0, r8, ror #26

# _ZN16b2PrismaticJointD1Ev
007eeff8: bx       lr

# _ZN16b2PrismaticJoint11EnableLimitEb
007eefac: strb     r1, [r0, #0xc8]
007eefb0: bx       lr

# _ZN7b2Joint6CreateEPK10b2JointDefP16b2BlockAllocator
007eb37c: push     {r4, r5, r6, lr}
007eb380: ldr      r3, [r0]
007eb384: mov      r4, r0
007eb388: sub      r3, r3, #1
007eb38c: cmp      r3, #5
007eb390: addls    pc, pc, r3, lsl #2
007eb394: b        #0x7eb3d0
007eb398: b        #0x7eb3d8
007eb39c: b        #0x7eb3f8
007eb3a0: b        #0x7eb418
007eb3a4: b        #0x7eb438
007eb3a8: b        #0x7eb458
007eb3ac: b        #0x7eb3b0
007eb3b0: mov      r0, r1
007eb3b4: mov      r1, #0xa4
007eb3b8: bl       #0x7e90bc
007eb3bc: mov      r1, r4
007eb3c0: mov      r5, r0
007eb3c4: bl       #0x7fb5ec
007eb3c8: mov      r0, r5
007eb3cc: pop      {r4, r5, r6, pc}
007eb3d0: mov      r0, #0
007eb3d4: pop      {r4, r5, r6, pc}
007eb3d8: mov      r0, r1
007eb3dc: mov      r1, #0x9c
007eb3e0: bl       #0x7e90bc
007eb3e4: mov      r1, r4
007eb3e8: mov      r5, r0
007eb3ec: bl       #0x7f336c
007eb3f0: mov      r0, r5
007eb3f4: pop      {r4, r5, r6, pc}
007eb3f8: mov      r0, r1
007eb3fc: mov      r1, #0xd0
007eb400: bl       #0x7e90bc
007eb404: mov      r1, r4
007eb408: mov      r5, r0
007eb40c: bl       #0x7efa08
007eb410: mov      r0, r5
007eb414: pop      {r4, r5, r6, pc}
007eb418: mov      r0, r1
007eb41c: mov      r1, #0x78
007eb420: bl       #0x7e90bc
007eb424: mov      r1, r4
007eb428: mov      r5, r0
007eb42c: bl       #0x7faa90
007eb430: mov      r0, r5
007eb434: pop      {r4, r5, r6, pc}
007eb438: mov      r0, r1
007eb43c: mov      r1, #0xb8
007eb440: bl       #0x7e90bc
007eb444: mov      r1, r4
007eb448: mov      r5, r0
007eb44c: bl       #0x7f0510
007eb450: mov      r0, r5
007eb454: pop      {r4, r5, r6, pc}
007eb458: mov      r0, r1
007eb45c: mov      r1, #0x80
007eb460: bl       #0x7e90bc
007eb464: mov      r1, r4
007eb468: mov      r5, r0
007eb46c: bl       #0x7ebbe0
007eb470: mov      r0, r5
007eb474: pop      {r4, r5, r6, pc}

# _ZN22b2PolyAndCircleContact6CreateEP7b2ShapeS1_P16b2BlockAllocator
007ebf8c: push     {r4, r5, r6, lr}
007ebf90: mov      r6, r0
007ebf94: mov      r5, r1
007ebf98: mov      r0, r2
007ebf9c: mov      r1, #0x94
007ebfa0: bl       #0x7e90bc
007ebfa4: mov      r1, r6
007ebfa8: mov      r4, r0
007ebfac: mov      r2, r5
007ebfb0: bl       #0x7ebed0
007ebfb4: mov      r0, r4
007ebfb8: pop      {r4, r5, r6, pc}

# _ZN7b2WorldD1Ev
007e7f7c: push     {r4, r5, r6, lr}
007e7f80: mov      r5, #0x19000
007e7f84: add      r3, r5, #0x254
007e7f88: mov      r4, r0
007e7f8c: ldr      r1, [r0, r3]
007e7f90: add      r6, r5, #0x1d8
007e7f94: bl       #0x7e7db0
007e7f98: ldr      r0, [r4, r6]
007e7f9c: bl       #0x7e247c
007e7fa0: ldr      r0, [r4, r6]
007e7fa4: ldr      r6, [pc, #0x44]
007e7fa8: bl       #0x7f34bc
007e7fac: ldr      r2, [pc, #0x40]
007e7fb0: ldr      r3, [pc, #0x40]
007e7fb4: add      r6, pc, r6
007e7fb8: ldr      r2, [r6, r2]
007e7fbc: ldr      r3, [r6, r3]
007e7fc0: add      r1, r5, #0x1e4
007e7fc4: add      r2, r2, #8
007e7fc8: add      r3, r3, #8
007e7fcc: add      r5, r5, #0x1dc
007e7fd0: str      r2, [r4, r1]
007e7fd4: add      r0, r4, #0x44
007e7fd8: str      r3, [r4, r5]
007e7fdc: bl       #0x7f3594
007e7fe0: mov      r0, r4
007e7fe4: bl       #0x7e8ddc
007e7fe8: mov      r0, r4
007e7fec: pop      {r4, r5, r6, pc}
007e7ff0: ldrsbeq  ip, [sl], -ip
007e7ff4: andeq    r2, r0, r4, lsr r3
007e7ff8: muleq    r0, r4, r8

# _ZN16b2PolygonContactC2EP7b2ShapeS1_
007ecaec: push     {r4, r5, r6, lr}
007ecaf0: ldr      r4, [pc, #0x28]
007ecaf4: mov      r5, r0
007ecaf8: bl       #0x7e9eb8
007ecafc: ldr      r3, [pc, #0x20]
007ecb00: add      r4, pc, r4
007ecb04: mov      r2, #0
007ecb08: ldr      r3, [r4, r3]
007ecb0c: str      r2, [r5, #0x90]
007ecb10: mov      r0, r5
007ecb14: add      r3, r3, #8
007ecb18: str      r3, [r5]
007ecb1c: pop      {r4, r5, r6, pc}
007ecb20: mulseq   sl, r0, pc
007ecb24: andeq    r2, r0, r8, ror fp

# _ZN16b2StackAllocatorC2Ev
007f3530: mov      r3, #0x19000
007f3534: mov      r1, #0
007f3538: push     {r4, r5}
007f353c: add      ip, r3, #8
007f3540: add      r5, r3, #0x18c
007f3544: add      r4, r3, #4
007f3548: str      r1, [r0, r5]
007f354c: str      r1, [r0, r4]
007f3550: str      r1, [r0, ip]
007f3554: str      r1, [r0, r3]
007f3558: pop      {r4, r5}
007f355c: bx       lr

# _ZN16b2PolygonContact8EvaluateEP17b2ContactListener
007ecb84: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ecb88: sub      sp, sp, #0xac
007ecb8c: ldr      ip, [r0, #0x34]
007ecb90: ldr      r3, [r0, #0x38]
007ecb94: add      r2, sp, #0x28
007ecb98: str      r2, [sp, #0x18]
007ecb9c: ldr      r4, [ip, #0xc]
007ecba0: ldr      r5, [r3, #0xc]
007ecba4: add      r8, r0, #0x48
007ecba8: mov      r6, r0
007ecbac: mov      r2, #0x4c
007ecbb0: mov      r7, r1
007ecbb4: ldr      r0, [sp, #0x18]
007ecbb8: mov      r1, r8
007ecbbc: bl       #0x30e868
007ecbc0: ldr      r1, [r6, #0x34]
007ecbc4: ldr      r3, [r6, #0x38]
007ecbc8: add      ip, r5, #4
007ecbcc: mov      r0, r8
007ecbd0: add      r2, r4, #4
007ecbd4: str      ip, [sp]
007ecbd8: bl       #0x7f5920
007ecbdc: ldr      ip, [r6, #0x90]
007ecbe0: ldr      r0, [r6, #0x34]
007ecbe4: ldr      r1, [r6, #0x38]
007ecbe8: ldr      r2, [r6, #0x3c]
007ecbec: ldr      r3, [r6, #0x40]
007ecbf0: mov      fp, #0
007ecbf4: cmp      ip, #0
007ecbf8: str      r0, [sp, #0x74]
007ecbfc: str      r1, [sp, #0x78]
007ecc00: str      r2, [sp, #0x98]
007ecc04: str      r3, [sp, #0x9c]
007ecc08: strb     fp, [sp, #0xa4]
007ecc0c: strb     fp, [sp, #0xa5]
007ecc10: strle    fp, [r6, #8]
007ecc14: ble      #0x7ecfe8
007ecc18: add      r1, sp, #0x74
007ecc1c: mov      sl, r6
007ecc20: str      r1, [sp, #0x24]
007ecc24: add      r8, sp, #0xa4
007ecc28: mov      r2, #0
007ecc2c: str      r2, [sl, #0x5c]
007ecc30: str      r2, [sl, #0x60]
007ecc34: ldr      r0, [sp, #0x70]
007ecc38: ldr      sb, [sl, #0x64]
007ecc3c: cmp      r0, #0
007ecc40: ble      #0x7ecc74
007ecc44: ldr      r2, [sp, #0x18]
007ecc48: mov      r3, #0
007ecc4c: ldrb     r1, [r8, r3]
007ecc50: cmp      r1, #0
007ecc54: bne      #0x7ecc64
007ecc58: ldr      r1, [r2, #0x1c]
007ecc5c: cmp      r1, sb
007ecc60: beq      #0x7ed2dc
007ecc64: add      r3, r3, #1
007ecc68: cmp      r3, r0
007ecc6c: add      r2, r2, #0x20
007ecc70: bne      #0x7ecc4c
007ecc74: cmp      r7, #0
007ecc78: beq      #0x7ecfcc
007ecc7c: ldr      r2, [sl, #0x48]
007ecc80: ldr      r1, [r4, #0xc]
007ecc84: mov      r0, r2
007ecc88: str      r2, [sp, #0x10]
007ecc8c: bl       #0x30ed6c
007ecc90: ldr      r1, [r4, #0x14]
007ecc94: mov      r3, r0
007ecc98: ldr      r0, [sl, #0x4c]
007ecc9c: str      r3, [sp, #0x14]
007ecca0: bl       #0x30ed6c
007ecca4: ldr      r3, [sp, #0x14]
007ecca8: mov      r1, r0
007eccac: mov      r0, r3
007eccb0: bl       #0x30eba4
007eccb4: ldr      r2, [sp, #0x10]
007eccb8: ldr      r1, [r4, #0x10]
007eccbc: mov      ip, r0
007eccc0: mov      r0, r2
007eccc4: str      ip, [sp, #0xc]
007eccc8: bl       #0x30ed6c
007ecccc: ldr      r1, [r4, #0x18]
007eccd0: mov      r3, r0
007eccd4: ldr      r0, [sl, #0x4c]
007eccd8: str      r3, [sp, #0x14]
007eccdc: bl       #0x30ed6c
007ecce0: ldr      r3, [sp, #0x14]
007ecce4: mov      r1, r0
007ecce8: mov      r0, r3
007eccec: bl       #0x30eba4
007eccf0: ldr      ip, [sp, #0xc]
007eccf4: ldr      r1, [r4, #4]
007eccf8: mov      r3, r0
007eccfc: mov      r0, ip
007ecd00: str      r3, [sp, #0x14]
007ecd04: bl       #0x30eba4
007ecd08: ldr      r3, [sp, #0x14]
007ecd0c: ldr      r1, [r4, #8]
007ecd10: mov      r2, r0
007ecd14: mov      r0, r3
007ecd18: str      r2, [sp, #0x10]
007ecd1c: bl       #0x30eba4
007ecd20: ldr      r2, [sp, #0x10]
007ecd24: str      r0, [sp, #0x80]
007ecd28: str      r2, [sp, #0x7c]
007ecd2c: ldr      r2, [sl, #0x48]
007ecd30: ldr      r1, [r4, #0xc]
007ecd34: mov      r0, r2
007ecd38: str      r2, [sp, #0x10]
007ecd3c: bl       #0x30ed6c
007ecd40: ldr      r1, [r4, #0x14]
007ecd44: mov      r3, r0
007ecd48: ldr      r0, [sl, #0x4c]
007ecd4c: str      r3, [sp, #0x14]
007ecd50: bl       #0x30ed6c
007ecd54: ldr      r3, [sp, #0x14]
007ecd58: mov      r1, r0
007ecd5c: mov      r0, r3
007ecd60: bl       #0x30eba4
007ecd64: ldr      r2, [sp, #0x10]
007ecd68: mov      ip, r0
007ecd6c: ldr      r1, [r4, #0x10]
007ecd70: mov      r0, r2
007ecd74: str      ip, [sp, #0xc]
007ecd78: bl       #0x30ed6c
007ecd7c: ldr      r1, [r4, #0x18]
007ecd80: mov      r3, r0
007ecd84: ldr      r0, [sl, #0x4c]
007ecd88: str      r3, [sp, #0x14]
007ecd8c: bl       #0x30ed6c
007ecd90: ldr      r3, [sp, #0x14]
007ecd94: mov      r1, r0
007ecd98: mov      r0, r3
007ecd9c: bl       #0x30eba4
007ecda0: ldr      ip, [sp, #0xc]
007ecda4: ldr      r1, [r4, #4]
007ecda8: mov      r3, r0
007ecdac: mov      r0, ip
007ecdb0: str      r3, [sp, #0x14]
007ecdb4: bl       #0x30eba4
007ecdb8: ldr      r3, [sp, #0x14]
007ecdbc: ldr      r1, [r4, #8]
007ecdc0: mov      r2, r0
007ecdc4: mov      r0, r3
007ecdc8: str      r2, [sp, #0x10]
007ecdcc: bl       #0x30eba4
007ecdd0: ldr      r2, [sp, #0x10]
007ecdd4: ldr      r1, [r4, #0x2c]
007ecdd8: mov      ip, r0
007ecddc: mov      r0, r2
007ecde0: str      ip, [sp, #0xc]
007ecde4: bl       #0x30e3ac
007ecde8: ldr      ip, [sp, #0xc]
007ecdec: ldr      r1, [r4, #0x30]
007ecdf0: mov      r3, r0
007ecdf4: mov      r0, ip
007ecdf8: str      r3, [sp, #0x14]
007ecdfc: bl       #0x30e3ac
007ece00: ldr      r2, [r4, #0x48]
007ece04: add      r1, r2, #0x80000000
007ece08: bl       #0x30ed6c
007ece0c: ldr      r3, [sp, #0x14]
007ece10: mov      r2, r0
007ece14: ldr      r0, [r4, #0x48]
007ece18: mov      r1, r3
007ece1c: str      r2, [sp, #0x10]
007ece20: bl       #0x30ed6c
007ece24: ldr      r2, [sp, #0x10]
007ece28: ldr      r1, [r4, #0x40]
007ece2c: mov      r3, r0
007ece30: mov      r0, r2
007ece34: str      r3, [sp, #0x14]
007ece38: bl       #0x30eba4
007ece3c: ldr      r3, [sp, #0x14]
007ece40: str      r0, [sp, #0x1c]
007ece44: ldr      r1, [r4, #0x44]
007ece48: mov      r0, r3
007ece4c: bl       #0x30eba4
007ece50: str      r0, [sp, #0x20]
007ece54: ldr      r2, [sl, #0x50]
007ece58: ldr      r1, [r5, #0xc]
007ece5c: mov      r0, r2
007ece60: str      r2, [sp, #0x10]
007ece64: bl       #0x30ed6c
007ece68: ldr      r1, [r5, #0x14]
007ece6c: mov      r3, r0
007ece70: ldr      r0, [sl, #0x54]
007ece74: str      r3, [sp, #0x14]
007ece78: bl       #0x30ed6c
007ece7c: ldr      r3, [sp, #0x14]
007ece80: mov      r1, r0
007ece84: mov      r0, r3
007ece88: bl       #0x30eba4
007ece8c: ldr      r2, [sp, #0x10]
007ece90: ldr      r1, [r5, #0x10]
007ece94: mov      ip, r0
007ece98: mov      r0, r2
007ece9c: str      ip, [sp, #0xc]
007ecea0: bl       #0x30ed6c
007ecea4: ldr      r1, [r5, #0x18]
007ecea8: mov      r3, r0
007eceac: ldr      r0, [sl, #0x54]
007eceb0: str      r3, [sp, #0x14]
007eceb4: bl       #0x30ed6c
007eceb8: ldr      r3, [sp, #0x14]
007ecebc: mov      r1, r0
007ecec0: mov      r0, r3
007ecec4: bl       #0x30eba4
007ecec8: ldr      ip, [sp, #0xc]
007ececc: ldr      r1, [r5, #4]
007eced0: mov      r3, r0
007eced4: mov      r0, ip
007eced8: str      r3, [sp, #0x14]
007ecedc: bl       #0x30eba4
007ecee0: ldr      r3, [sp, #0x14]
007ecee4: ldr      r1, [r5, #8]
007ecee8: mov      r2, r0
007eceec: mov      r0, r3
007ecef0: str      r2, [sp, #0x10]
007ecef4: bl       #0x30eba4
007ecef8: ldr      r2, [sp, #0x10]
007ecefc: ldr      r1, [r5, #0x2c]
007ecf00: mov      r3, r0
007ecf04: mov      r0, r2
007ecf08: str      r3, [sp, #0x14]
007ecf0c: bl       #0x30e3ac
007ecf10: ldr      r3, [sp, #0x14]
007ecf14: ldr      r1, [r5, #0x30]
007ecf18: mov      r2, r0
007ecf1c: mov      r0, r3
007ecf20: str      r2, [sp, #0x10]
007ecf24: bl       #0x30e3ac
007ecf28: ldr      r3, [r5, #0x48]
007ecf2c: add      r1, r3, #0x80000000
007ecf30: bl       #0x30ed6c
007ecf34: ldr      r2, [sp, #0x10]
007ecf38: mov      r3, r0
007ecf3c: ldr      r0, [r5, #0x48]
007ecf40: mov      r1, r2
007ecf44: str      r3, [sp, #0x14]
007ecf48: bl       #0x30ed6c
007ecf4c: ldr      r3, [sp, #0x14]
007ecf50: mov      r2, r0
007ecf54: ldr      r1, [r5, #0x40]
007ecf58: mov      r0, r3
007ecf5c: str      r2, [sp, #0x10]
007ecf60: bl       #0x30eba4
007ecf64: ldr      r2, [sp, #0x10]
007ecf68: mov      r3, r0
007ecf6c: ldr      r1, [r5, #0x44]
007ecf70: mov      r0, r2
007ecf74: str      r3, [sp, #0x14]
007ecf78: bl       #0x30eba4
007ecf7c: ldr      r1, [sp, #0x20]
007ecf80: bl       #0x30e3ac
007ecf84: ldr      r3, [sp, #0x14]
007ecf88: str      r0, [sp, #0x88]
007ecf8c: ldr      r1, [sp, #0x1c]
007ecf90: mov      r0, r3
007ecf94: bl       #0x30e3ac
007ecf98: ldr      r2, [r6, #0x88]
007ecf9c: ldr      r3, [r6, #0x8c]
007ecfa0: str      r0, [sp, #0x84]
007ecfa4: str      r2, [sp, #0x8c]
007ecfa8: str      r3, [sp, #0x90]
007ecfac: ldr      r2, [sl, #0x58]
007ecfb0: str      sb, [sp, #0xa0]
007ecfb4: ldr      r3, [r7]
007ecfb8: mov      r0, r7
007ecfbc: str      r2, [sp, #0x94]
007ecfc0: ldr      r1, [sp, #0x24]
007ecfc4: mov      lr, pc
007ecfc8: ldr      pc, [r3, #8]
007ecfcc: ldr      r3, [r6, #0x90]
007ecfd0: add      fp, fp, #1
007ecfd4: add      sl, sl, #0x20
007ecfd8: cmp      r3, fp
007ecfdc: bgt      #0x7ecc28
007ecfe0: mov      r3, #1
007ecfe4: str      r3, [r6, #8]
007ecfe8: cmp      r7, #0
007ecfec: beq      #0x7ed2d4
007ecff0: ldr      r3, [sp, #0x70]
007ecff4: cmp      r3, #0
007ecff8: ble      #0x7ed2d4
007ecffc: ldr      r1, [sp, #0x18]
007ed000: add      r2, sp, #0x74
007ed004: mov      r8, #0
007ed008: add      r6, r1, #0x10
007ed00c: add      ip, sp, #0xa4
007ed010: str      r2, [sp, #0x18]
007ed014: ldrb     r2, [ip, r8]
007ed018: cmp      r2, #0
007ed01c: bne      #0x7ed2c4
007ed020: ldr      sb, [r6, #-0x10]
007ed024: ldr      r1, [r4, #0xc]
007ed028: ldr      sl, [r6, #-0xc]
007ed02c: mov      r0, sb
007ed030: str      ip, [sp, #0xc]
007ed034: bl       #0x30ed6c
007ed038: ldr      r1, [r4, #0x14]
007ed03c: mov      fp, r0
007ed040: mov      r0, sl
007ed044: bl       #0x30ed6c
007ed048: mov      r1, r0
007ed04c: mov      r0, fp
007ed050: bl       #0x30eba4
007ed054: ldr      r1, [r4, #0x10]
007ed058: mov      fp, r0
007ed05c: mov      r0, sb
007ed060: bl       #0x30ed6c
007ed064: ldr      r1, [r4, #0x18]
007ed068: mov      sb, r0
007ed06c: mov      r0, sl
007ed070: bl       #0x30ed6c
007ed074: mov      r1, r0
007ed078: mov      r0, sb
007ed07c: bl       #0x30eba4
007ed080: ldr      r1, [r4, #4]
007ed084: mov      sl, r0
007ed088: mov      r0, fp
007ed08c: bl       #0x30eba4
007ed090: ldr      r1, [r4, #8]
007ed094: mov      sb, r0
007ed098: mov      r0, sl
007ed09c: bl       #0x30eba4
007ed0a0: str      sb, [sp, #0x7c]
007ed0a4: str      r0, [sp, #0x80]
007ed0a8: ldr      sb, [r6, #-0x10]
007ed0ac: ldr      r1, [r4, #0xc]
007ed0b0: ldr      sl, [r6, #-0xc]
007ed0b4: mov      r0, sb
007ed0b8: bl       #0x30ed6c
007ed0bc: ldr      r1, [r4, #0x14]
007ed0c0: mov      fp, r0
007ed0c4: mov      r0, sl
007ed0c8: bl       #0x30ed6c
007ed0cc: mov      r1, r0
007ed0d0: mov      r0, fp
007ed0d4: bl       #0x30eba4
007ed0d8: ldr      r1, [r4, #0x10]
007ed0dc: mov      fp, r0
007ed0e0: mov      r0, sb
007ed0e4: bl       #0x30ed6c
007ed0e8: ldr      r1, [r4, #0x18]
007ed0ec: mov      sb, r0
007ed0f0: mov      r0, sl
007ed0f4: bl       #0x30ed6c
007ed0f8: mov      r1, r0
007ed0fc: mov      r0, sb
007ed100: bl       #0x30eba4
007ed104: ldr      r1, [r4, #4]
007ed108: mov      sl, r0
007ed10c: mov      r0, fp
007ed110: bl       #0x30eba4
007ed114: ldr      r1, [r4, #8]
007ed118: mov      fp, r0
007ed11c: mov      r0, sl
007ed120: bl       #0x30eba4
007ed124: ldr      r1, [r4, #0x2c]
007ed128: mov      sb, r0
007ed12c: mov      r0, fp
007ed130: bl       #0x30e3ac
007ed134: ldr      sl, [r4, #0x48]
007ed138: mov      fp, r0
007ed13c: ldr      r1, [r4, #0x30]
007ed140: mov      r0, sb
007ed144: bl       #0x30e3ac
007ed148: add      r1, sl, #0x80000000
007ed14c: bl       #0x30ed6c
007ed150: mov      r1, fp
007ed154: mov      sb, r0
007ed158: mov      r0, sl
007ed15c: bl       #0x30ed6c
007ed160: ldr      r1, [r4, #0x40]
007ed164: mov      sl, r0
007ed168: mov      r0, sb
007ed16c: bl       #0x30eba4
007ed170: ldr      r1, [r4, #0x44]
007ed174: mov      r2, r0
007ed178: mov      r0, sl
007ed17c: str      r2, [sp, #0x10]
007ed180: bl       #0x30eba4
007ed184: ldr      sb, [r6, #-8]
007ed188: mov      r3, r0
007ed18c: ldr      r1, [r5, #0xc]
007ed190: mov      r0, sb
007ed194: ldr      sl, [r6, #-4]
007ed198: str      r3, [sp, #0x14]
007ed19c: bl       #0x30ed6c
007ed1a0: ldr      r1, [r5, #0x14]
007ed1a4: mov      fp, r0
007ed1a8: mov      r0, sl
007ed1ac: bl       #0x30ed6c
007ed1b0: mov      r1, r0
007ed1b4: mov      r0, fp
007ed1b8: bl       #0x30eba4
007ed1bc: ldr      r1, [r5, #0x10]
007ed1c0: mov      fp, r0
007ed1c4: mov      r0, sb
007ed1c8: bl       #0x30ed6c
007ed1cc: ldr      r1, [r5, #0x18]
007ed1d0: mov      sb, r0
007ed1d4: mov      r0, sl
007ed1d8: bl       #0x30ed6c
007ed1dc: mov      r1, r0
007ed1e0: mov      r0, sb
007ed1e4: bl       #0x30eba4
007ed1e8: ldr      r1, [r5, #4]
007ed1ec: mov      sl, r0
007ed1f0: mov      r0, fp
007ed1f4: bl       #0x30eba4
007ed1f8: ldr      r1, [r5, #8]
007ed1fc: mov      fp, r0
007ed200: mov      r0, sl
007ed204: bl       #0x30eba4
007ed208: ldr      r1, [r5, #0x2c]
007ed20c: mov      sb, r0
007ed210: mov      r0, fp
007ed214: bl       #0x30e3ac
007ed218: ldr      sl, [r5, #0x48]
007ed21c: ldr      r1, [r5, #0x30]
007ed220: mov      fp, r0
007ed224: mov      r0, sb
007ed228: bl       #0x30e3ac
007ed22c: add      r1, sl, #0x80000000
007ed230: bl       #0x30ed6c
007ed234: mov      r1, fp
007ed238: mov      sb, r0
007ed23c: mov      r0, sl
007ed240: bl       #0x30ed6c
007ed244: ldr      r1, [r5, #0x40]
007ed248: mov      sl, r0
007ed24c: mov      r0, sb
007ed250: bl       #0x30eba4
007ed254: ldr      r1, [r5, #0x44]
007ed258: mov      sb, r0
007ed25c: mov      r0, sl
007ed260: bl       #0x30eba4
007ed264: ldr      r3, [sp, #0x14]
007ed268: mov      r1, r3
007ed26c: bl       #0x30e3ac
007ed270: ldr      r2, [sp, #0x10]
007ed274: str      r0, [sp, #0x88]
007ed278: mov      r0, sb
007ed27c: mov      r1, r2
007ed280: bl       #0x30e3ac
007ed284: ldr      r3, [sp, #0x68]
007ed288: str      r0, [sp, #0x84]
007ed28c: mov      r0, r7
007ed290: str      r3, [sp, #0x8c]
007ed294: ldr      r3, [sp, #0x6c]
007ed298: ldr      r1, [sp, #0x18]
007ed29c: str      r3, [sp, #0x90]
007ed2a0: ldr      r2, [r6]
007ed2a4: ldr      r3, [r7]
007ed2a8: str      r2, [sp, #0x94]
007ed2ac: ldr      r2, [r6, #0xc]
007ed2b0: str      r2, [sp, #0xa0]
007ed2b4: mov      lr, pc
007ed2b8: ldr      pc, [r3, #0x10]
007ed2bc: ldr      r3, [sp, #0x70]
007ed2c0: ldr      ip, [sp, #0xc]
007ed2c4: add      r8, r8, #1
007ed2c8: cmp      r3, r8
007ed2cc: add      r6, r6, #0x20
007ed2d0: bgt      #0x7ed014
007ed2d4: add      sp, sp, #0xac
007ed2d8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ed2dc: add      r1, sp, #0xa8
007ed2e0: add      r3, r1, r3
007ed2e4: mov      r1, #1
007ed2e8: strb     r1, [r3, #-4]
007ed2ec: ldr      r3, [r2, #0x14]
007ed2f0: cmp      r7, #0
007ed2f4: str      r3, [sl, #0x5c]
007ed2f8: ldr      r3, [r2, #0x18]
007ed2fc: str      r3, [sl, #0x60]
007ed300: beq      #0x7ecfcc
007ed304: ldr      r2, [sl, #0x48]
007ed308: ldr      r1, [r4, #0xc]
007ed30c: mov      r0, r2
007ed310: str      r2, [sp, #0x10]
007ed314: bl       #0x30ed6c
007ed318: ldr      r1, [r4, #0x14]
007ed31c: mov      r3, r0
007ed320: ldr      r0, [sl, #0x4c]
007ed324: str      r3, [sp, #0x14]
007ed328: bl       #0x30ed6c
007ed32c: ldr      r3, [sp, #0x14]
007ed330: mov      r1, r0
007ed334: mov      r0, r3
007ed338: bl       #0x30eba4
007ed33c: ldr      r2, [sp, #0x10]
007ed340: ldr      r1, [r4, #0x10]
007ed344: mov      ip, r0
007ed348: mov      r0, r2
007ed34c: str      ip, [sp, #0xc]
007ed350: bl       #0x30ed6c
007ed354: ldr      r1, [r4, #0x18]
007ed358: mov      r3, r0
007ed35c: ldr      r0, [sl, #0x4c]
007ed360: str      r3, [sp, #0x14]
007ed364: bl       #0x30ed6c
007ed368: ldr      r3, [sp, #0x14]
007ed36c: mov      r1, r0
007ed370: mov      r0, r3
007ed374: bl       #0x30eba4
007ed378: ldr      ip, [sp, #0xc]
007ed37c: ldr      r1, [r4, #4]
007ed380: mov      r3, r0
007ed384: mov      r0, ip
007ed388: str      r3, [sp, #0x14]
007ed38c: bl       #0x30eba4
007ed390: ldr      r3, [sp, #0x14]
007ed394: ldr      r1, [r4, #8]
007ed398: mov      r2, r0
007ed39c: mov      r0, r3
007ed3a0: str      r2, [sp, #0x10]
007ed3a4: bl       #0x30eba4
007ed3a8: ldr      r2, [sp, #0x10]
007ed3ac: str      r0, [sp, #0x80]
007ed3b0: str      r2, [sp, #0x7c]
007ed3b4: ldr      r2, [sl, #0x48]
007ed3b8: ldr      r1, [r4, #0xc]
007ed3bc: mov      r0, r2
007ed3c0: str      r2, [sp, #0x10]
007ed3c4: bl       #0x30ed6c
007ed3c8: ldr      r1, [r4, #0x14]
007ed3cc: mov      r3, r0
007ed3d0: ldr      r0, [sl, #0x4c]
007ed3d4: str      r3, [sp, #0x14]
007ed3d8: bl       #0x30ed6c
007ed3dc: ldr      r3, [sp, #0x14]
007ed3e0: mov      r1, r0
007ed3e4: mov      r0, r3
007ed3e8: bl       #0x30eba4
007ed3ec: ldr      r2, [sp, #0x10]
007ed3f0: mov      ip, r0
007ed3f4: ldr      r1, [r4, #0x10]
007ed3f8: mov      r0, r2
007ed3fc: str      ip, [sp, #0xc]
007ed400: bl       #0x30ed6c
007ed404: ldr      r1, [r4, #0x18]
007ed408: mov      r3, r0
007ed40c: ldr      r0, [sl, #0x4c]
007ed410: str      r3, [sp, #0x14]
007ed414: bl       #0x30ed6c
007ed418: ldr      r3, [sp, #0x14]
007ed41c: mov      r1, r0
007ed420: mov      r0, r3
007ed424: bl       #0x30eba4
007ed428: ldr      ip, [sp, #0xc]
007ed42c: ldr      r1, [r4, #4]
007ed430: mov      r3, r0
007ed434: mov      r0, ip
007ed438: str      r3, [sp, #0x14]
007ed43c: bl       #0x30eba4
007ed440: ldr      r3, [sp, #0x14]
007ed444: ldr      r1, [r4, #8]
007ed448: mov      r2, r0
007ed44c: mov      r0, r3
007ed450: str      r2, [sp, #0x10]
007ed454: bl       #0x30eba4
007ed458: ldr      r2, [sp, #0x10]
007ed45c: ldr      r1, [r4, #0x2c]
007ed460: mov      ip, r0
007ed464: mov      r0, r2
007ed468: str      ip, [sp, #0xc]
007ed46c: bl       #0x30e3ac
007ed470: ldr      ip, [sp, #0xc]
007ed474: ldr      r1, [r4, #0x30]
007ed478: mov      r3, r0
007ed47c: mov      r0, ip
007ed480: str      r3, [sp, #0x14]
007ed484: bl       #0x30e3ac
007ed488: ldr      r2, [r4, #0x48]
007ed48c: add      r1, r2, #0x80000000
007ed490: bl       #0x30ed6c
007ed494: ldr      r3, [sp, #0x14]
007ed498: mov      r2, r0
007ed49c: ldr      r0, [r4, #0x48]
007ed4a0: mov      r1, r3
007ed4a4: str      r2, [sp, #0x10]
007ed4a8: bl       #0x30ed6c
007ed4ac: ldr      r2, [sp, #0x10]
007ed4b0: ldr      r1, [r4, #0x40]
007ed4b4: mov      r3, r0
007ed4b8: mov      r0, r2
007ed4bc: str      r3, [sp, #0x14]
007ed4c0: bl       #0x30eba4
007ed4c4: ldr      r3, [sp, #0x14]
007ed4c8: str      r0, [sp, #0x1c]
007ed4cc: ldr      r1, [r4, #0x44]
007ed4d0: mov      r0, r3
007ed4d4: bl       #0x30eba4
007ed4d8: str      r0, [sp, #0x20]
007ed4dc: ldr      r2, [sl, #0x50]
007ed4e0: ldr      r1, [r5, #0xc]
007ed4e4: mov      r0, r2
007ed4e8: str      r2, [sp, #0x10]
007ed4ec: bl       #0x30ed6c
007ed4f0: ldr      r1, [r5, #0x14]
007ed4f4: mov      r3, r0
007ed4f8: ldr      r0, [sl, #0x54]
007ed4fc: str      r3, [sp, #0x14]
007ed500: bl       #0x30ed6c
007ed504: ldr      r3, [sp, #0x14]
007ed508: mov      r1, r0
007ed50c: mov      r0, r3
007ed510: bl       #0x30eba4
007ed514: ldr      r2, [sp, #0x10]
007ed518: ldr      r1, [r5, #0x10]
007ed51c: mov      ip, r0
007ed520: mov      r0, r2
007ed524: str      ip, [sp, #0xc]
007ed528: bl       #0x30ed6c
007ed52c: ldr      r1, [r5, #0x18]
007ed530: mov      r3, r0
007ed534: ldr      r0, [sl, #0x54]
007ed538: str      r3, [sp, #0x14]
007ed53c: bl       #0x30ed6c
007ed540: ldr      r3, [sp, #0x14]
007ed544: mov      r1, r0
007ed548: mov      r0, r3
007ed54c: bl       #0x30eba4
007ed550: ldr      ip, [sp, #0xc]
007ed554: ldr      r1, [r5, #4]
007ed558: mov      r3, r0
007ed55c: mov      r0, ip
007ed560: str      r3, [sp, #0x14]
007ed564: bl       #0x30eba4
007ed568: ldr      r3, [sp, #0x14]
007ed56c: ldr      r1, [r5, #8]
007ed570: mov      r2, r0
007ed574: mov      r0, r3
007ed578: str      r2, [sp, #0x10]
007ed57c: bl       #0x30eba4
007ed580: ldr      r2, [sp, #0x10]
007ed584: ldr      r1, [r5, #0x2c]
007ed588: mov      r3, r0
007ed58c: mov      r0, r2
007ed590: str      r3, [sp, #0x14]
007ed594: bl       #0x30e3ac
007ed598: ldr      r3, [sp, #0x14]
007ed59c: ldr      r1, [r5, #0x30]
007ed5a0: mov      r2, r0
007ed5a4: mov      r0, r3
007ed5a8: str      r2, [sp, #0x10]
007ed5ac: bl       #0x30e3ac
007ed5b0: ldr      r3, [r5, #0x48]
007ed5b4: add      r1, r3, #0x80000000
007ed5b8: bl       #0x30ed6c
007ed5bc: ldr      r2, [sp, #0x10]
007ed5c0: mov      r3, r0
007ed5c4: ldr      r0, [r5, #0x48]
007ed5c8: mov      r1, r2
007ed5cc: str      r3, [sp, #0x14]
007ed5d0: bl       #0x30ed6c
007ed5d4: ldr      r3, [sp, #0x14]
007ed5d8: ldr      r1, [r5, #0x40]
007ed5dc: mov      r2, r0
007ed5e0: mov      r0, r3
007ed5e4: str      r2, [sp, #0x10]
007ed5e8: bl       #0x30eba4
007ed5ec: ldr      r2, [sp, #0x10]
007ed5f0: mov      r3, r0
007ed5f4: ldr      r1, [r5, #0x44]
007ed5f8: mov      r0, r2
007ed5fc: str      r3, [sp, #0x14]
007ed600: bl       #0x30eba4
007ed604: ldr      r1, [sp, #0x20]
007ed608: bl       #0x30e3ac
007ed60c: ldr      r3, [sp, #0x14]
007ed610: str      r0, [sp, #0x88]
007ed614: ldr      r1, [sp, #0x1c]
007ed618: mov      r0, r3
007ed61c: bl       #0x30e3ac
007ed620: ldr      r2, [r6, #0x88]
007ed624: ldr      r3, [r6, #0x8c]
007ed628: str      r0, [sp, #0x84]
007ed62c: str      r2, [sp, #0x8c]
007ed630: str      r3, [sp, #0x90]
007ed634: ldr      r2, [sl, #0x58]
007ed638: str      sb, [sp, #0xa0]
007ed63c: ldr      r3, [r7]
007ed640: mov      r0, r7
007ed644: str      r2, [sp, #0x94]
007ed648: ldr      r1, [sp, #0x24]
007ed64c: mov      lr, pc
007ed650: ldr      pc, [r3, #0xc]
007ed654: b        #0x7ecfcc

# _ZN11b2DebugDrawD1Ev
007e8c44: bx       lr

# _ZN15b2RevoluteJoint24SolveVelocityConstraintsERK10b2TimeStep
007f2478: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f247c: ldr      r4, [r0, #0x30]
007f2480: sub      sp, sp, #0x14
007f2484: mov      r5, r0
007f2488: mov      r7, r1
007f248c: ldr      r0, [r0, #0x44]
007f2490: ldr      r1, [r4, #0x1c]
007f2494: bl       #0x30e3ac
007f2498: ldr      r1, [r4, #0x20]
007f249c: mov      sb, r0
007f24a0: ldr      r0, [r5, #0x48]
007f24a4: bl       #0x30e3ac
007f24a8: ldr      r1, [r4, #0xc]
007f24ac: mov      sl, r0
007f24b0: mov      r0, sb
007f24b4: bl       #0x30ed6c
007f24b8: ldr      r1, [r4, #0x14]
007f24bc: mov      r6, r0
007f24c0: mov      r0, sl
007f24c4: bl       #0x30ed6c
007f24c8: mov      r1, r0
007f24cc: mov      r0, r6
007f24d0: bl       #0x30eba4
007f24d4: ldr      r1, [r4, #0x10]
007f24d8: mov      r8, r0
007f24dc: mov      r0, sb
007f24e0: bl       #0x30ed6c
007f24e4: ldr      r1, [r4, #0x18]
007f24e8: mov      sb, r0
007f24ec: mov      r0, sl
007f24f0: bl       #0x30ed6c
007f24f4: mov      r1, r0
007f24f8: mov      r0, sb
007f24fc: bl       #0x30eba4
007f2500: ldr      r6, [r5, #0x34]
007f2504: str      r0, [sp, #8]
007f2508: ldr      r0, [r5, #0x4c]
007f250c: ldr      r1, [r6, #0x1c]
007f2510: bl       #0x30e3ac
007f2514: ldr      r1, [r6, #0x20]
007f2518: mov      sb, r0
007f251c: ldr      r0, [r5, #0x50]
007f2520: bl       #0x30e3ac
007f2524: ldr      r1, [r6, #0xc]
007f2528: mov      sl, r0
007f252c: mov      r0, sb
007f2530: bl       #0x30ed6c
007f2534: ldr      r1, [r6, #0x14]
007f2538: mov      fp, r0
007f253c: mov      r0, sl
007f2540: bl       #0x30ed6c
007f2544: mov      r1, r0
007f2548: mov      r0, fp
007f254c: bl       #0x30eba4
007f2550: str      r0, [sp, #0xc]
007f2554: ldr      r1, [r6, #0x10]
007f2558: mov      r0, sb
007f255c: bl       #0x30ed6c
007f2560: ldr      r1, [r6, #0x18]
007f2564: mov      sb, r0
007f2568: mov      r0, sl
007f256c: bl       #0x30ed6c
007f2570: mov      r1, r0
007f2574: mov      r0, sb
007f2578: bl       #0x30eba4
007f257c: ldr      sl, [r6, #0x48]
007f2580: str      r0, [sp]
007f2584: add      r1, sl, #0x80000000
007f2588: bl       #0x30ed6c
007f258c: ldr      r1, [sp, #0xc]
007f2590: mov      sb, r0
007f2594: mov      r0, sl
007f2598: bl       #0x30ed6c
007f259c: ldr      r1, [r6, #0x40]
007f25a0: mov      sl, r0
007f25a4: mov      r0, sb
007f25a8: bl       #0x30eba4
007f25ac: ldr      r1, [r6, #0x44]
007f25b0: mov      sb, r0
007f25b4: mov      r0, sl
007f25b8: bl       #0x30eba4
007f25bc: ldr      r1, [r4, #0x40]
007f25c0: mov      sl, r0
007f25c4: mov      r0, sb
007f25c8: bl       #0x30e3ac
007f25cc: ldr      r1, [r4, #0x44]
007f25d0: mov      fp, r0
007f25d4: mov      r0, sl
007f25d8: bl       #0x30e3ac
007f25dc: ldr      sl, [r4, #0x48]
007f25e0: mov      r3, r0
007f25e4: ldr      r0, [sp, #8]
007f25e8: add      r1, sl, #0x80000000
007f25ec: str      r3, [sp, #4]
007f25f0: bl       #0x30ed6c
007f25f4: mov      r1, r8
007f25f8: mov      sb, r0
007f25fc: mov      r0, sl
007f2600: bl       #0x30ed6c
007f2604: mov      r1, sb
007f2608: mov      sl, r0
007f260c: mov      r0, fp
007f2610: bl       #0x30e3ac
007f2614: ldr      r3, [sp, #4]
007f2618: mov      fp, r0
007f261c: mov      r1, sl
007f2620: mov      r0, r3
007f2624: bl       #0x30e3ac
007f2628: ldr      r1, [r5, #0x68]
007f262c: mov      sb, r0
007f2630: mov      r0, fp
007f2634: bl       #0x30ed6c
007f2638: ldr      r1, [r5, #0x70]
007f263c: mov      r3, r0
007f2640: mov      r0, sb
007f2644: ldr      sl, [r7, #4]
007f2648: str      r3, [sp, #4]
007f264c: bl       #0x30ed6c
007f2650: ldr      r3, [sp, #4]
007f2654: mov      r1, r0
007f2658: add      sl, sl, #0x80000000
007f265c: mov      r0, r3
007f2660: bl       #0x30eba4
007f2664: ldr      r1, [r5, #0x6c]
007f2668: mov      r3, r0
007f266c: mov      r0, fp
007f2670: str      r3, [sp, #4]
007f2674: bl       #0x30ed6c
007f2678: ldr      r1, [r5, #0x74]
007f267c: mov      fp, r0
007f2680: mov      r0, sb
007f2684: bl       #0x30ed6c
007f2688: mov      r1, r0
007f268c: mov      r0, fp
007f2690: bl       #0x30eba4
007f2694: ldr      r3, [sp, #4]
007f2698: mov      sb, r0
007f269c: mov      r0, sl
007f26a0: mov      r1, r3
007f26a4: bl       #0x30ed6c
007f26a8: mov      r1, sb
007f26ac: mov      fp, r0
007f26b0: mov      r0, sl
007f26b4: bl       #0x30ed6c
007f26b8: mov      r1, fp
007f26bc: mov      sl, r0
007f26c0: ldr      r0, [r5, #0x54]
007f26c4: bl       #0x30eba4
007f26c8: mov      r1, sl
007f26cc: str      r0, [r5, #0x54]
007f26d0: ldr      r0, [r5, #0x58]
007f26d4: bl       #0x30eba4
007f26d8: str      r0, [r5, #0x58]
007f26dc: ldr      sb, [r7]
007f26e0: mov      r1, fp
007f26e4: mov      r0, sb
007f26e8: bl       #0x30ed6c
007f26ec: mov      r1, sl
007f26f0: mov      fp, r0
007f26f4: mov      r0, sb
007f26f8: bl       #0x30ed6c
007f26fc: ldr      sb, [r4, #0x78]
007f2700: mov      sl, r0
007f2704: mov      r1, fp
007f2708: mov      r0, sb
007f270c: bl       #0x30ed6c
007f2710: mov      r1, r0
007f2714: ldr      r0, [r4, #0x40]
007f2718: bl       #0x30e3ac
007f271c: mov      r1, sl
007f2720: str      r0, [r4, #0x40]
007f2724: mov      r0, sb
007f2728: bl       #0x30ed6c
007f272c: mov      r1, r0
007f2730: ldr      r0, [r4, #0x44]
007f2734: bl       #0x30e3ac
007f2738: mov      r1, sl
007f273c: str      r0, [r4, #0x44]
007f2740: mov      r0, r8
007f2744: bl       #0x30ed6c
007f2748: mov      r1, fp
007f274c: mov      r8, r0
007f2750: ldr      r0, [sp, #8]
007f2754: bl       #0x30ed6c
007f2758: mov      r1, r0
007f275c: mov      r0, r8
007f2760: bl       #0x30e3ac
007f2764: ldr      r1, [r4, #0x80]
007f2768: bl       #0x30ed6c
007f276c: mov      r1, r0
007f2770: ldr      r0, [r4, #0x48]
007f2774: bl       #0x30e3ac
007f2778: str      r0, [r4, #0x48]
007f277c: ldr      r8, [r6, #0x78]
007f2780: mov      r1, fp
007f2784: mov      r0, r8
007f2788: bl       #0x30ed6c
007f278c: mov      r1, r0
007f2790: ldr      r0, [r6, #0x40]
007f2794: bl       #0x30eba4
007f2798: mov      r1, sl
007f279c: str      r0, [r6, #0x40]
007f27a0: mov      r0, r8
007f27a4: bl       #0x30ed6c
007f27a8: mov      r1, r0
007f27ac: ldr      r0, [r6, #0x44]
007f27b0: bl       #0x30eba4
007f27b4: str      r0, [r6, #0x44]
007f27b8: mov      r1, sl
007f27bc: ldr      r0, [sp, #0xc]
007f27c0: bl       #0x30ed6c
007f27c4: ldr      r2, [sp]
007f27c8: mov      r8, r0
007f27cc: mov      r1, fp
007f27d0: mov      r0, r2
007f27d4: bl       #0x30ed6c
007f27d8: mov      r1, r0
007f27dc: mov      r0, r8
007f27e0: bl       #0x30e3ac
007f27e4: ldr      r1, [r6, #0x80]
007f27e8: bl       #0x30ed6c
007f27ec: ldr      r1, [r6, #0x48]
007f27f0: bl       #0x30eba4
007f27f4: str      r0, [r6, #0x48]
007f27f8: ldrb     r3, [r5, #0x7c]
007f27fc: mov      r8, r0
007f2800: cmp      r3, #0
007f2804: beq      #0x7f28d4
007f2808: ldr      r3, [r5, #0x98]
007f280c: cmp      r3, #3
007f2810: beq      #0x7f28d4
007f2814: ldr      r0, [r7, #4]
007f2818: ldr      r1, [r5, #0x78]
007f281c: ldr      sb, [r5, #0x5c]
007f2820: add      r0, r0, #0x80000000
007f2824: bl       #0x30ed6c
007f2828: ldr      r1, [r4, #0x48]
007f282c: mov      sl, r0
007f2830: mov      r0, r8
007f2834: bl       #0x30e3ac
007f2838: ldr      r1, [r5, #0x84]
007f283c: bl       #0x30e3ac
007f2840: mov      r1, r0
007f2844: mov      r0, sl
007f2848: bl       #0x30ed6c
007f284c: mov      r1, sb
007f2850: bl       #0x30eba4
007f2854: ldr      sl, [r5, #0x80]
007f2858: mov      fp, r0
007f285c: mov      r1, sl
007f2860: bl       #0x30e70c
007f2864: cmp      r0, #0
007f2868: add      r8, sl, #0x80000000
007f286c: moveq    fp, sl
007f2870: mov      r0, r8
007f2874: mov      r1, fp
007f2878: bl       #0x30e2f8
007f287c: cmp      r0, #0
007f2880: moveq    r8, fp
007f2884: mov      r1, sb
007f2888: str      r8, [r5, #0x5c]
007f288c: mov      r0, r8
007f2890: bl       #0x30e3ac
007f2894: ldr      r1, [r7]
007f2898: bl       #0x30ed6c
007f289c: ldr      r1, [r4, #0x80]
007f28a0: mov      r8, r0
007f28a4: bl       #0x30ed6c
007f28a8: mov      r1, r0
007f28ac: ldr      r0, [r4, #0x48]
007f28b0: bl       #0x30e3ac
007f28b4: str      r0, [r4, #0x48]
007f28b8: ldr      r1, [r6, #0x80]
007f28bc: mov      r0, r8
007f28c0: bl       #0x30ed6c
007f28c4: mov      r1, r0
007f28c8: ldr      r0, [r6, #0x48]
007f28cc: bl       #0x30eba4
007f28d0: str      r0, [r6, #0x48]
007f28d4: ldrb     r3, [r5, #0x88]
007f28d8: cmp      r3, #0
007f28dc: beq      #0x7f297c
007f28e0: ldr      r8, [r5, #0x98]
007f28e4: cmp      r8, #0
007f28e8: beq      #0x7f297c
007f28ec: ldr      r0, [r7, #4]
007f28f0: ldr      r1, [r5, #0x78]
007f28f4: ldr      sl, [r4, #0x48]
007f28f8: add      r0, r0, #0x80000000
007f28fc: bl       #0x30ed6c
007f2900: mov      r1, sl
007f2904: mov      sb, r0
007f2908: ldr      r0, [r6, #0x48]
007f290c: bl       #0x30e3ac
007f2910: mov      r1, r0
007f2914: mov      r0, sb
007f2918: bl       #0x30ed6c
007f291c: cmp      r8, #3
007f2920: mov      sb, r0
007f2924: beq      #0x7f29e8
007f2928: cmp      r8, #1
007f292c: beq      #0x7f2984
007f2930: cmp      r8, #2
007f2934: beq      #0x7f29c0
007f2938: ldr      r1, [r7]
007f293c: mov      r0, sb
007f2940: bl       #0x30ed6c
007f2944: ldr      r1, [r4, #0x80]
007f2948: mov      r5, r0
007f294c: bl       #0x30ed6c
007f2950: mov      r1, r0
007f2954: mov      r0, sl
007f2958: bl       #0x30e3ac
007f295c: str      r0, [r4, #0x48]
007f2960: ldr      r1, [r6, #0x80]
007f2964: mov      r0, r5
007f2968: bl       #0x30ed6c
007f296c: mov      r1, r0
007f2970: ldr      r0, [r6, #0x48]
007f2974: bl       #0x30eba4
007f2978: str      r0, [r6, #0x48]
007f297c: add      sp, sp, #0x14
007f2980: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f2984: ldr      sl, [r5, #0x60]
007f2988: mov      r1, sl
007f298c: bl       #0x30eba4
007f2990: mov      r1, #0
007f2994: mov      r8, r0
007f2998: bl       #0x30e2f8
007f299c: cmp      r0, #0
007f29a0: beq      #0x7f29e0
007f29a4: mov      r1, sl
007f29a8: str      r8, [r5, #0x60]
007f29ac: mov      r0, r8
007f29b0: bl       #0x30e3ac
007f29b4: ldr      sl, [r4, #0x48]
007f29b8: mov      sb, r0
007f29bc: b        #0x7f2938
007f29c0: ldr      sl, [r5, #0x60]
007f29c4: mov      r1, sl
007f29c8: bl       #0x30eba4
007f29cc: mov      r1, #0
007f29d0: mov      r8, r0
007f29d4: bl       #0x30e70c
007f29d8: cmp      r0, #0
007f29dc: bne      #0x7f29a4
007f29e0: mov      r8, #0
007f29e4: b        #0x7f29a4
007f29e8: ldr      r0, [r5, #0x60]
007f29ec: mov      r1, sb
007f29f0: bl       #0x30eba4
007f29f4: str      r0, [r5, #0x60]
007f29f8: ldr      sl, [r4, #0x48]
007f29fc: b        #0x7f2938

# _ZN15b2RevoluteJoint11EnableLimitEb
007f2bb0: strb     r1, [r0, #0x88]
007f2bb4: bx       lr

# _ZN15b2CircleContact8EvaluateEP17b2ContactListener
007f3b9c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f3ba0: ldr      sb, [r0, #0x38]
007f3ba4: ldr      r8, [r0, #0x34]
007f3ba8: sub      sp, sp, #0xa4
007f3bac: ldr      r6, [sb, #0xc]
007f3bb0: add      sl, r0, #0x48
007f3bb4: mov      r4, r0
007f3bb8: mov      r2, #0x4c
007f3bbc: mov      r7, r1
007f3bc0: add      r0, sp, #0x24
007f3bc4: mov      r1, sl
007f3bc8: ldr      r5, [r8, #0xc]
007f3bcc: bl       #0x30e868
007f3bd0: add      ip, r6, #4
007f3bd4: str      ip, [sp]
007f3bd8: ldr      ip, [sp, #0x68]
007f3bdc: mov      r0, sl
007f3be0: mov      r1, r8
007f3be4: str      ip, [sp, #0x14]
007f3be8: ldr      ip, [sp, #0x64]
007f3bec: mov      r3, sb
007f3bf0: add      r2, r5, #4
007f3bf4: str      ip, [sp, #0x18]
007f3bf8: ldr      ip, [sp, #0x34]
007f3bfc: ldr      sb, [sp, #0x28]
007f3c00: ldr      sl, [sp, #0x24]
007f3c04: str      ip, [sp, #0x1c]
007f3c08: ldr      ip, [sp, #0x30]
007f3c0c: ldr      fp, [sp, #0x38]
007f3c10: ldr      r8, [sp, #0x6c]
007f3c14: str      ip, [sp, #0x10]
007f3c18: ldr      ip, [sp, #0x2c]
007f3c1c: str      ip, [sp, #0xc]
007f3c20: ldr      ip, [sp, #0x3c]
007f3c24: str      ip, [sp, #8]
007f3c28: bl       #0x7f4c4c
007f3c2c: ldr      r0, [r4, #0x90]
007f3c30: ldr      ip, [r4, #0x34]
007f3c34: ldr      r1, [r4, #0x38]
007f3c38: ldr      r2, [r4, #0x3c]
007f3c3c: ldr      r3, [r4, #0x40]
007f3c40: cmp      r0, #0
007f3c44: str      ip, [sp, #0x70]
007f3c48: str      r1, [sp, #0x74]
007f3c4c: str      r2, [sp, #0x94]
007f3c50: str      r3, [sp, #0x98]
007f3c54: ble      #0x7f419c
007f3c58: mov      r3, #1
007f3c5c: cmp      r8, #0
007f3c60: str      r3, [r4, #8]
007f3c64: beq      #0x7f3f04
007f3c68: str      fp, [r4, #0x5c]
007f3c6c: ldr      r3, [sp, #8]
007f3c70: cmp      r7, #0
007f3c74: str      r3, [r4, #0x60]
007f3c78: beq      #0x7f3efc
007f3c7c: ldr      sl, [r4, #0x48]
007f3c80: ldr      r1, [r5, #0xc]
007f3c84: ldr      r8, [r4, #0x4c]
007f3c88: mov      r0, sl
007f3c8c: bl       #0x30ed6c
007f3c90: ldr      r1, [r5, #0x14]
007f3c94: mov      sb, r0
007f3c98: mov      r0, r8
007f3c9c: bl       #0x30ed6c
007f3ca0: mov      r1, r0
007f3ca4: mov      r0, sb
007f3ca8: bl       #0x30eba4
007f3cac: ldr      r1, [r5, #0x10]
007f3cb0: mov      sb, r0
007f3cb4: mov      r0, sl
007f3cb8: bl       #0x30ed6c
007f3cbc: ldr      r1, [r5, #0x18]
007f3cc0: mov      fp, r0
007f3cc4: mov      r0, r8
007f3cc8: bl       #0x30ed6c
007f3ccc: mov      r1, r0
007f3cd0: mov      r0, fp
007f3cd4: bl       #0x30eba4
007f3cd8: ldr      r1, [r5, #4]
007f3cdc: mov      fp, r0
007f3ce0: mov      r0, sb
007f3ce4: bl       #0x30eba4
007f3ce8: ldr      r1, [r5, #8]
007f3cec: mov      sb, r0
007f3cf0: mov      r0, fp
007f3cf4: bl       #0x30eba4
007f3cf8: str      sb, [sp, #0x78]
007f3cfc: str      r0, [sp, #0x7c]
007f3d00: ldr      r1, [r5, #0xc]
007f3d04: mov      r0, sl
007f3d08: bl       #0x30ed6c
007f3d0c: ldr      r1, [r5, #0x14]
007f3d10: mov      sb, r0
007f3d14: mov      r0, r8
007f3d18: bl       #0x30ed6c
007f3d1c: mov      r1, r0
007f3d20: mov      r0, sb
007f3d24: bl       #0x30eba4
007f3d28: ldr      r1, [r5, #0x10]
007f3d2c: mov      sb, r0
007f3d30: mov      r0, sl
007f3d34: bl       #0x30ed6c
007f3d38: ldr      r1, [r5, #0x18]
007f3d3c: mov      sl, r0
007f3d40: mov      r0, r8
007f3d44: bl       #0x30ed6c
007f3d48: mov      r1, r0
007f3d4c: mov      r0, sl
007f3d50: bl       #0x30eba4
007f3d54: ldr      r1, [r5, #4]
007f3d58: mov      r8, r0
007f3d5c: mov      r0, sb
007f3d60: bl       #0x30eba4
007f3d64: ldr      r1, [r5, #8]
007f3d68: mov      sb, r0
007f3d6c: mov      r0, r8
007f3d70: bl       #0x30eba4
007f3d74: ldr      r1, [r5, #0x2c]
007f3d78: mov      sl, r0
007f3d7c: mov      r0, sb
007f3d80: bl       #0x30e3ac
007f3d84: ldr      r8, [r5, #0x48]
007f3d88: ldr      r1, [r5, #0x30]
007f3d8c: mov      sb, r0
007f3d90: mov      r0, sl
007f3d94: bl       #0x30e3ac
007f3d98: add      r1, r8, #0x80000000
007f3d9c: bl       #0x30ed6c
007f3da0: mov      r1, sb
007f3da4: mov      sl, r0
007f3da8: mov      r0, r8
007f3dac: bl       #0x30ed6c
007f3db0: ldr      r1, [r5, #0x40]
007f3db4: mov      r8, r0
007f3db8: mov      r0, sl
007f3dbc: bl       #0x30eba4
007f3dc0: ldr      r1, [r5, #0x44]
007f3dc4: mov      sl, r0
007f3dc8: mov      r0, r8
007f3dcc: bl       #0x30eba4
007f3dd0: ldr      r8, [r4, #0x50]
007f3dd4: ldr      r1, [r6, #0xc]
007f3dd8: mov      sb, r0
007f3ddc: mov      r0, r8
007f3de0: bl       #0x30ed6c
007f3de4: ldr      r5, [r4, #0x54]
007f3de8: mov      fp, r0
007f3dec: ldr      r1, [r6, #0x14]
007f3df0: mov      r0, r5
007f3df4: bl       #0x30ed6c
007f3df8: mov      r1, r0
007f3dfc: mov      r0, fp
007f3e00: bl       #0x30eba4
007f3e04: ldr      r1, [r6, #0x10]
007f3e08: mov      fp, r0
007f3e0c: mov      r0, r8
007f3e10: bl       #0x30ed6c
007f3e14: ldr      r1, [r6, #0x18]
007f3e18: mov      r8, r0
007f3e1c: mov      r0, r5
007f3e20: bl       #0x30ed6c
007f3e24: mov      r1, r0
007f3e28: mov      r0, r8
007f3e2c: bl       #0x30eba4
007f3e30: ldr      r1, [r6, #4]
007f3e34: mov      r5, r0
007f3e38: mov      r0, fp
007f3e3c: bl       #0x30eba4
007f3e40: ldr      r1, [r6, #8]
007f3e44: mov      r8, r0
007f3e48: mov      r0, r5
007f3e4c: bl       #0x30eba4
007f3e50: ldr      r1, [r6, #0x2c]
007f3e54: mov      fp, r0
007f3e58: mov      r0, r8
007f3e5c: bl       #0x30e3ac
007f3e60: ldr      r5, [r6, #0x48]
007f3e64: mov      r8, r0
007f3e68: ldr      r1, [r6, #0x30]
007f3e6c: mov      r0, fp
007f3e70: bl       #0x30e3ac
007f3e74: add      r1, r5, #0x80000000
007f3e78: bl       #0x30ed6c
007f3e7c: mov      r1, r8
007f3e80: mov      fp, r0
007f3e84: mov      r0, r5
007f3e88: bl       #0x30ed6c
007f3e8c: ldr      r1, [r6, #0x40]
007f3e90: mov      r8, r0
007f3e94: mov      r0, fp
007f3e98: bl       #0x30eba4
007f3e9c: ldr      r1, [r6, #0x44]
007f3ea0: mov      r5, r0
007f3ea4: mov      r0, r8
007f3ea8: bl       #0x30eba4
007f3eac: mov      r1, sb
007f3eb0: bl       #0x30e3ac
007f3eb4: mov      r1, sl
007f3eb8: str      r0, [sp, #0x84]
007f3ebc: mov      r0, r5
007f3ec0: bl       #0x30e3ac
007f3ec4: ldr      r3, [r4, #0x64]
007f3ec8: ldr      r1, [r4, #0x8c]
007f3ecc: ldr      ip, [r4, #0x88]
007f3ed0: ldr      r2, [r4, #0x58]
007f3ed4: str      r0, [sp, #0x80]
007f3ed8: str      r1, [sp, #0x8c]
007f3edc: str      ip, [sp, #0x88]
007f3ee0: str      r2, [sp, #0x90]
007f3ee4: str      r3, [sp, #0x9c]
007f3ee8: mov      r0, r7
007f3eec: ldr      r3, [r7]
007f3ef0: add      r1, sp, #0x70
007f3ef4: mov      lr, pc
007f3ef8: ldr      pc, [r3, #0xc]
007f3efc: add      sp, sp, #0xa4
007f3f00: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f3f04: mov      r3, #0
007f3f08: cmp      r7, #0
007f3f0c: str      r3, [r4, #0x60]
007f3f10: str      r3, [r4, #0x5c]
007f3f14: beq      #0x7f3efc
007f3f18: ldr      sl, [r4, #0x48]
007f3f1c: ldr      r1, [r5, #0xc]
007f3f20: ldr      r8, [r4, #0x4c]
007f3f24: mov      r0, sl
007f3f28: bl       #0x30ed6c
007f3f2c: ldr      r1, [r5, #0x14]
007f3f30: mov      sb, r0
007f3f34: mov      r0, r8
007f3f38: bl       #0x30ed6c
007f3f3c: mov      r1, r0
007f3f40: mov      r0, sb
007f3f44: bl       #0x30eba4
007f3f48: ldr      r1, [r5, #0x10]
007f3f4c: mov      sb, r0
007f3f50: mov      r0, sl
007f3f54: bl       #0x30ed6c
007f3f58: ldr      r1, [r5, #0x18]
007f3f5c: mov      fp, r0
007f3f60: mov      r0, r8
007f3f64: bl       #0x30ed6c
007f3f68: mov      r1, r0
007f3f6c: mov      r0, fp
007f3f70: bl       #0x30eba4
007f3f74: ldr      r1, [r5, #4]
007f3f78: mov      fp, r0
007f3f7c: mov      r0, sb
007f3f80: bl       #0x30eba4
007f3f84: ldr      r1, [r5, #8]
007f3f88: mov      sb, r0
007f3f8c: mov      r0, fp
007f3f90: bl       #0x30eba4
007f3f94: str      sb, [sp, #0x78]
007f3f98: str      r0, [sp, #0x7c]
007f3f9c: ldr      r1, [r5, #0xc]
007f3fa0: mov      r0, sl
007f3fa4: bl       #0x30ed6c
007f3fa8: ldr      r1, [r5, #0x14]
007f3fac: mov      sb, r0
007f3fb0: mov      r0, r8
007f3fb4: bl       #0x30ed6c
007f3fb8: mov      r1, r0
007f3fbc: mov      r0, sb
007f3fc0: bl       #0x30eba4
007f3fc4: ldr      r1, [r5, #0x10]
007f3fc8: mov      sb, r0
007f3fcc: mov      r0, sl
007f3fd0: bl       #0x30ed6c
007f3fd4: ldr      r1, [r5, #0x18]
007f3fd8: mov      sl, r0
007f3fdc: mov      r0, r8
007f3fe0: bl       #0x30ed6c
007f3fe4: mov      r1, r0
007f3fe8: mov      r0, sl
007f3fec: bl       #0x30eba4
007f3ff0: ldr      r1, [r5, #4]
007f3ff4: mov      r8, r0
007f3ff8: mov      r0, sb
007f3ffc: bl       #0x30eba4
007f4000: ldr      r1, [r5, #8]
007f4004: mov      sb, r0
007f4008: mov      r0, r8
007f400c: bl       #0x30eba4
007f4010: ldr      r1, [r5, #0x2c]
007f4014: mov      sl, r0
007f4018: mov      r0, sb
007f401c: bl       #0x30e3ac
007f4020: ldr      r8, [r5, #0x48]
007f4024: ldr      r1, [r5, #0x30]
007f4028: mov      sb, r0
007f402c: mov      r0, sl
007f4030: bl       #0x30e3ac
007f4034: add      r1, r8, #0x80000000
007f4038: bl       #0x30ed6c
007f403c: mov      r1, sb
007f4040: mov      sl, r0
007f4044: mov      r0, r8
007f4048: bl       #0x30ed6c
007f404c: ldr      r1, [r5, #0x40]
007f4050: mov      r8, r0
007f4054: mov      r0, sl
007f4058: bl       #0x30eba4
007f405c: ldr      r1, [r5, #0x44]
007f4060: mov      sl, r0
007f4064: mov      r0, r8
007f4068: bl       #0x30eba4
007f406c: ldr      r8, [r4, #0x50]
007f4070: ldr      r1, [r6, #0xc]
007f4074: mov      sb, r0
007f4078: mov      r0, r8
007f407c: bl       #0x30ed6c
007f4080: ldr      r5, [r4, #0x54]
007f4084: ldr      r1, [r6, #0x14]
007f4088: mov      fp, r0
007f408c: mov      r0, r5
007f4090: bl       #0x30ed6c
007f4094: mov      r1, r0
007f4098: mov      r0, fp
007f409c: bl       #0x30eba4
007f40a0: ldr      r1, [r6, #0x10]
007f40a4: mov      fp, r0
007f40a8: mov      r0, r8
007f40ac: bl       #0x30ed6c
007f40b0: ldr      r1, [r6, #0x18]
007f40b4: mov      r8, r0
007f40b8: mov      r0, r5
007f40bc: bl       #0x30ed6c
007f40c0: mov      r1, r0
007f40c4: mov      r0, r8
007f40c8: bl       #0x30eba4
007f40cc: ldr      r1, [r6, #4]
007f40d0: mov      r5, r0
007f40d4: mov      r0, fp
007f40d8: bl       #0x30eba4
007f40dc: ldr      r1, [r6, #8]
007f40e0: mov      r8, r0
007f40e4: mov      r0, r5
007f40e8: bl       #0x30eba4
007f40ec: ldr      r1, [r6, #0x2c]
007f40f0: mov      fp, r0
007f40f4: mov      r0, r8
007f40f8: bl       #0x30e3ac
007f40fc: ldr      r5, [r6, #0x48]
007f4100: ldr      r1, [r6, #0x30]
007f4104: mov      r8, r0
007f4108: mov      r0, fp
007f410c: bl       #0x30e3ac
007f4110: add      r1, r5, #0x80000000
007f4114: bl       #0x30ed6c
007f4118: mov      r1, r8
007f411c: mov      fp, r0
007f4120: mov      r0, r5
007f4124: bl       #0x30ed6c
007f4128: ldr      r1, [r6, #0x40]
007f412c: mov      r8, r0
007f4130: mov      r0, fp
007f4134: bl       #0x30eba4
007f4138: ldr      r1, [r6, #0x44]
007f413c: mov      r5, r0
007f4140: mov      r0, r8
007f4144: bl       #0x30eba4
007f4148: mov      r1, sb
007f414c: bl       #0x30e3ac
007f4150: mov      r1, sl
007f4154: str      r0, [sp, #0x84]
007f4158: mov      r0, r5
007f415c: bl       #0x30e3ac
007f4160: ldr      r3, [r4, #0x64]
007f4164: ldr      r1, [r4, #0x8c]
007f4168: ldr      ip, [r4, #0x88]
007f416c: ldr      r2, [r4, #0x58]
007f4170: str      r0, [sp, #0x80]
007f4174: str      r1, [sp, #0x8c]
007f4178: str      ip, [sp, #0x88]
007f417c: str      r2, [sp, #0x90]
007f4180: str      r3, [sp, #0x9c]
007f4184: mov      r0, r7
007f4188: ldr      r3, [r7]
007f418c: add      r1, sp, #0x70
007f4190: mov      lr, pc
007f4194: ldr      pc, [r3, #8]
007f4198: b        #0x7f3efc
007f419c: mov      r3, #0
007f41a0: cmp      r7, #0
007f41a4: cmpne    r8, #0
007f41a8: str      r3, [r4, #8]
007f41ac: ble      #0x7f3efc
007f41b0: ldr      r1, [r5, #0xc]
007f41b4: mov      r0, sl
007f41b8: bl       #0x30ed6c
007f41bc: ldr      r1, [r5, #0x14]
007f41c0: mov      r4, r0
007f41c4: mov      r0, sb
007f41c8: bl       #0x30ed6c
007f41cc: mov      r1, r0
007f41d0: mov      r0, r4
007f41d4: bl       #0x30eba4
007f41d8: ldr      r1, [r5, #0x10]
007f41dc: mov      r8, r0
007f41e0: mov      r0, sl
007f41e4: bl       #0x30ed6c
007f41e8: ldr      r1, [r5, #0x18]
007f41ec: mov      r4, r0
007f41f0: mov      r0, sb
007f41f4: bl       #0x30ed6c
007f41f8: mov      r1, r0
007f41fc: mov      r0, r4
007f4200: bl       #0x30eba4
007f4204: ldr      r1, [r5, #4]
007f4208: mov      r4, r0
007f420c: mov      r0, r8
007f4210: bl       #0x30eba4
007f4214: ldr      r1, [r5, #8]
007f4218: mov      r8, r0
007f421c: mov      r0, r4
007f4220: bl       #0x30eba4
007f4224: str      r8, [sp, #0x78]
007f4228: str      r0, [sp, #0x7c]
007f422c: ldr      r1, [r5, #0xc]
007f4230: mov      r0, sl
007f4234: bl       #0x30ed6c
007f4238: ldr      r1, [r5, #0x14]
007f423c: mov      r4, r0
007f4240: mov      r0, sb
007f4244: bl       #0x30ed6c
007f4248: mov      r1, r0
007f424c: mov      r0, r4
007f4250: bl       #0x30eba4
007f4254: ldr      r1, [r5, #0x10]
007f4258: mov      r8, r0
007f425c: mov      r0, sl
007f4260: bl       #0x30ed6c
007f4264: ldr      r1, [r5, #0x18]
007f4268: mov      r4, r0
007f426c: mov      r0, sb
007f4270: bl       #0x30ed6c
007f4274: mov      r1, r0
007f4278: mov      r0, r4
007f427c: bl       #0x30eba4
007f4280: ldr      r1, [r5, #4]
007f4284: mov      r4, r0
007f4288: mov      r0, r8
007f428c: bl       #0x30eba4
007f4290: ldr      r1, [r5, #8]
007f4294: mov      r8, r0
007f4298: mov      r0, r4
007f429c: bl       #0x30eba4
007f42a0: ldr      r1, [r5, #0x2c]
007f42a4: mov      sl, r0
007f42a8: mov      r0, r8
007f42ac: bl       #0x30e3ac
007f42b0: ldr      sb, [r5, #0x48]
007f42b4: ldr      r1, [r5, #0x30]
007f42b8: mov      r4, r0
007f42bc: mov      r0, sl
007f42c0: bl       #0x30e3ac
007f42c4: add      r1, sb, #0x80000000
007f42c8: bl       #0x30ed6c
007f42cc: mov      r1, r4
007f42d0: mov      r8, r0
007f42d4: mov      r0, sb
007f42d8: bl       #0x30ed6c
007f42dc: ldr      r1, [r5, #0x40]
007f42e0: mov      r4, r0
007f42e4: mov      r0, r8
007f42e8: bl       #0x30eba4
007f42ec: ldr      r1, [r5, #0x44]
007f42f0: mov      sb, r0
007f42f4: mov      r0, r4
007f42f8: bl       #0x30eba4
007f42fc: ldr      r1, [r6, #0xc]
007f4300: mov      sl, r0
007f4304: ldr      r0, [sp, #0xc]
007f4308: bl       #0x30ed6c
007f430c: ldr      r1, [r6, #0x14]
007f4310: mov      r4, r0
007f4314: ldr      r0, [sp, #0x10]
007f4318: bl       #0x30ed6c
007f431c: mov      r1, r0
007f4320: mov      r0, r4
007f4324: bl       #0x30eba4
007f4328: ldr      r1, [r6, #0x10]
007f432c: mov      r4, r0
007f4330: ldr      r0, [sp, #0xc]
007f4334: bl       #0x30ed6c
007f4338: ldr      r1, [r6, #0x18]
007f433c: mov      r5, r0
007f4340: ldr      r0, [sp, #0x10]
007f4344: bl       #0x30ed6c
007f4348: mov      r1, r0
007f434c: mov      r0, r5
007f4350: bl       #0x30eba4
007f4354: ldr      r1, [r6, #4]
007f4358: mov      r5, r0
007f435c: mov      r0, r4
007f4360: bl       #0x30eba4
007f4364: ldr      r1, [r6, #8]
007f4368: mov      r8, r0
007f436c: mov      r0, r5
007f4370: bl       #0x30eba4
007f4374: ldr      r1, [r6, #0x2c]
007f4378: mov      r5, r0
007f437c: mov      r0, r8
007f4380: bl       #0x30e3ac
007f4384: ldr      r4, [r6, #0x48]
007f4388: ldr      r1, [r6, #0x30]
007f438c: mov      r8, r0
007f4390: mov      r0, r5
007f4394: bl       #0x30e3ac
007f4398: add      r1, r4, #0x80000000
007f439c: bl       #0x30ed6c
007f43a0: mov      r1, r8
007f43a4: mov      r5, r0
007f43a8: mov      r0, r4
007f43ac: bl       #0x30ed6c
007f43b0: ldr      r1, [r6, #0x40]
007f43b4: mov      r4, r0
007f43b8: mov      r0, r5
007f43bc: bl       #0x30eba4
007f43c0: ldr      r1, [r6, #0x44]
007f43c4: mov      r5, r0
007f43c8: mov      r0, r4
007f43cc: bl       #0x30eba4
007f43d0: mov      r1, sl
007f43d4: bl       #0x30e3ac
007f43d8: mov      r1, sb
007f43dc: str      r0, [sp, #0x84]
007f43e0: mov      r0, r5
007f43e4: bl       #0x30e3ac
007f43e8: ldr      ip, [sp, #0x14]
007f43ec: ldr      r3, [sp, #0x18]
007f43f0: str      r0, [sp, #0x80]
007f43f4: str      ip, [sp, #0x8c]
007f43f8: str      r3, [sp, #0x88]
007f43fc: ldr      ip, [sp, #0x1c]
007f4400: ldr      r3, [sp, #0x40]
007f4404: mov      r0, r7
007f4408: str      ip, [sp, #0x90]
007f440c: str      r3, [sp, #0x9c]
007f4410: ldr      r3, [r7]
007f4414: add      r1, sp, #0x70
007f4418: mov      lr, pc
007f441c: ldr      pc, [r3, #0x10]
007f4420: b        #0x7f3efc

# _ZN12b2MouseJointD0Ev
007eb83c: push     {r4, lr}
007eb840: mov      r4, r0
007eb844: bl       #0x30e2b0
007eb848: mov      r0, r4
007eb84c: pop      {r4, pc}

# _ZN22b2PolyAndCircleContact8EvaluateEP17b2ContactListener
007ebfbc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ebfc0: sub      sp, sp, #0xac
007ebfc4: ldr      ip, [r0, #0x34]
007ebfc8: ldr      r3, [r0, #0x38]
007ebfcc: add      r2, sp, #0x28
007ebfd0: str      r2, [sp, #0x18]
007ebfd4: ldr      r4, [ip, #0xc]
007ebfd8: ldr      r5, [r3, #0xc]
007ebfdc: add      r8, r0, #0x48
007ebfe0: mov      r6, r0
007ebfe4: mov      r2, #0x4c
007ebfe8: mov      r7, r1
007ebfec: ldr      r0, [sp, #0x18]
007ebff0: mov      r1, r8
007ebff4: bl       #0x30e868
007ebff8: ldr      r1, [r6, #0x34]
007ebffc: ldr      r3, [r6, #0x38]
007ec000: add      ip, r5, #4
007ec004: mov      r0, r8
007ec008: add      r2, r4, #4
007ec00c: str      ip, [sp]
007ec010: bl       #0x7f4424
007ec014: ldr      ip, [r6, #0x90]
007ec018: ldr      r0, [r6, #0x34]
007ec01c: ldr      r1, [r6, #0x38]
007ec020: ldr      r2, [r6, #0x3c]
007ec024: ldr      r3, [r6, #0x40]
007ec028: mov      fp, #0
007ec02c: cmp      ip, #0
007ec030: str      r0, [sp, #0x74]
007ec034: str      r1, [sp, #0x78]
007ec038: str      r2, [sp, #0x98]
007ec03c: str      r3, [sp, #0x9c]
007ec040: strb     fp, [sp, #0xa4]
007ec044: strb     fp, [sp, #0xa5]
007ec048: strle    fp, [r6, #8]
007ec04c: ble      #0x7ec420
007ec050: add      r1, sp, #0x74
007ec054: mov      sl, r6
007ec058: str      r1, [sp, #0x24]
007ec05c: add      r8, sp, #0xa4
007ec060: mov      r2, #0
007ec064: str      r2, [sl, #0x5c]
007ec068: str      r2, [sl, #0x60]
007ec06c: ldr      r0, [sp, #0x70]
007ec070: ldr      sb, [sl, #0x64]
007ec074: cmp      r0, #0
007ec078: ble      #0x7ec0ac
007ec07c: ldr      r2, [sp, #0x18]
007ec080: mov      r3, #0
007ec084: ldrb     r1, [r8, r3]
007ec088: cmp      r1, #0
007ec08c: bne      #0x7ec09c
007ec090: ldr      r1, [r2, #0x1c]
007ec094: cmp      r1, sb
007ec098: beq      #0x7ec714
007ec09c: add      r3, r3, #1
007ec0a0: cmp      r3, r0
007ec0a4: add      r2, r2, #0x20
007ec0a8: bne      #0x7ec084
007ec0ac: cmp      r7, #0
007ec0b0: beq      #0x7ec404
007ec0b4: ldr      r2, [sl, #0x48]
007ec0b8: ldr      r1, [r4, #0xc]
007ec0bc: mov      r0, r2
007ec0c0: str      r2, [sp, #0x10]
007ec0c4: bl       #0x30ed6c
007ec0c8: ldr      r1, [r4, #0x14]
007ec0cc: mov      r3, r0
007ec0d0: ldr      r0, [sl, #0x4c]
007ec0d4: str      r3, [sp, #0x14]
007ec0d8: bl       #0x30ed6c
007ec0dc: ldr      r3, [sp, #0x14]
007ec0e0: mov      r1, r0
007ec0e4: mov      r0, r3
007ec0e8: bl       #0x30eba4
007ec0ec: ldr      r2, [sp, #0x10]
007ec0f0: ldr      r1, [r4, #0x10]
007ec0f4: mov      ip, r0
007ec0f8: mov      r0, r2
007ec0fc: str      ip, [sp, #0xc]
007ec100: bl       #0x30ed6c
007ec104: ldr      r1, [r4, #0x18]
007ec108: mov      r3, r0
007ec10c: ldr      r0, [sl, #0x4c]
007ec110: str      r3, [sp, #0x14]
007ec114: bl       #0x30ed6c
007ec118: ldr      r3, [sp, #0x14]
007ec11c: mov      r1, r0
007ec120: mov      r0, r3
007ec124: bl       #0x30eba4
007ec128: ldr      ip, [sp, #0xc]
007ec12c: ldr      r1, [r4, #4]
007ec130: mov      r3, r0
007ec134: mov      r0, ip
007ec138: str      r3, [sp, #0x14]
007ec13c: bl       #0x30eba4
007ec140: ldr      r3, [sp, #0x14]
007ec144: ldr      r1, [r4, #8]
007ec148: mov      r2, r0
007ec14c: mov      r0, r3
007ec150: str      r2, [sp, #0x10]
007ec154: bl       #0x30eba4
007ec158: ldr      r2, [sp, #0x10]
007ec15c: str      r0, [sp, #0x80]
007ec160: str      r2, [sp, #0x7c]
007ec164: ldr      r2, [sl, #0x48]
007ec168: ldr      r1, [r4, #0xc]
007ec16c: mov      r0, r2
007ec170: str      r2, [sp, #0x10]
007ec174: bl       #0x30ed6c
007ec178: ldr      r1, [r4, #0x14]
007ec17c: mov      r3, r0
007ec180: ldr      r0, [sl, #0x4c]
007ec184: str      r3, [sp, #0x14]
007ec188: bl       #0x30ed6c
007ec18c: ldr      r3, [sp, #0x14]
007ec190: mov      r1, r0
007ec194: mov      r0, r3
007ec198: bl       #0x30eba4
007ec19c: ldr      r2, [sp, #0x10]
007ec1a0: mov      ip, r0
007ec1a4: ldr      r1, [r4, #0x10]
007ec1a8: mov      r0, r2
007ec1ac: str      ip, [sp, #0xc]
007ec1b0: bl       #0x30ed6c
007ec1b4: ldr      r1, [r4, #0x18]
007ec1b8: mov      r3, r0
007ec1bc: ldr      r0, [sl, #0x4c]
007ec1c0: str      r3, [sp, #0x14]
007ec1c4: bl       #0x30ed6c
007ec1c8: ldr      r3, [sp, #0x14]
007ec1cc: mov      r1, r0
007ec1d0: mov      r0, r3
007ec1d4: bl       #0x30eba4
007ec1d8: ldr      ip, [sp, #0xc]
007ec1dc: ldr      r1, [r4, #4]
007ec1e0: mov      r3, r0
007ec1e4: mov      r0, ip
007ec1e8: str      r3, [sp, #0x14]
007ec1ec: bl       #0x30eba4
007ec1f0: ldr      r3, [sp, #0x14]
007ec1f4: ldr      r1, [r4, #8]
007ec1f8: mov      r2, r0
007ec1fc: mov      r0, r3
007ec200: str      r2, [sp, #0x10]
007ec204: bl       #0x30eba4
007ec208: ldr      r2, [sp, #0x10]
007ec20c: ldr      r1, [r4, #0x2c]
007ec210: mov      ip, r0
007ec214: mov      r0, r2
007ec218: str      ip, [sp, #0xc]
007ec21c: bl       #0x30e3ac
007ec220: ldr      ip, [sp, #0xc]
007ec224: ldr      r1, [r4, #0x30]
007ec228: mov      r3, r0
007ec22c: mov      r0, ip
007ec230: str      r3, [sp, #0x14]
007ec234: bl       #0x30e3ac
007ec238: ldr      r2, [r4, #0x48]
007ec23c: add      r1, r2, #0x80000000
007ec240: bl       #0x30ed6c
007ec244: ldr      r3, [sp, #0x14]
007ec248: mov      r2, r0
007ec24c: ldr      r0, [r4, #0x48]
007ec250: mov      r1, r3
007ec254: str      r2, [sp, #0x10]
007ec258: bl       #0x30ed6c
007ec25c: ldr      r2, [sp, #0x10]
007ec260: ldr      r1, [r4, #0x40]
007ec264: mov      r3, r0
007ec268: mov      r0, r2
007ec26c: str      r3, [sp, #0x14]
007ec270: bl       #0x30eba4
007ec274: ldr      r3, [sp, #0x14]
007ec278: str      r0, [sp, #0x1c]
007ec27c: ldr      r1, [r4, #0x44]
007ec280: mov      r0, r3
007ec284: bl       #0x30eba4
007ec288: str      r0, [sp, #0x20]
007ec28c: ldr      r2, [sl, #0x50]
007ec290: ldr      r1, [r5, #0xc]
007ec294: mov      r0, r2
007ec298: str      r2, [sp, #0x10]
007ec29c: bl       #0x30ed6c
007ec2a0: ldr      r1, [r5, #0x14]
007ec2a4: mov      r3, r0
007ec2a8: ldr      r0, [sl, #0x54]
007ec2ac: str      r3, [sp, #0x14]
007ec2b0: bl       #0x30ed6c
007ec2b4: ldr      r3, [sp, #0x14]
007ec2b8: mov      r1, r0
007ec2bc: mov      r0, r3
007ec2c0: bl       #0x30eba4
007ec2c4: ldr      r2, [sp, #0x10]
007ec2c8: ldr      r1, [r5, #0x10]
007ec2cc: mov      ip, r0
007ec2d0: mov      r0, r2
007ec2d4: str      ip, [sp, #0xc]
007ec2d8: bl       #0x30ed6c
007ec2dc: ldr      r1, [r5, #0x18]
007ec2e0: mov      r3, r0
007ec2e4: ldr      r0, [sl, #0x54]
007ec2e8: str      r3, [sp, #0x14]
007ec2ec: bl       #0x30ed6c
007ec2f0: ldr      r3, [sp, #0x14]
007ec2f4: mov      r1, r0
007ec2f8: mov      r0, r3
007ec2fc: bl       #0x30eba4
007ec300: ldr      ip, [sp, #0xc]
007ec304: ldr      r1, [r5, #4]
007ec308: mov      r3, r0
007ec30c: mov      r0, ip
007ec310: str      r3, [sp, #0x14]
007ec314: bl       #0x30eba4
007ec318: ldr      r3, [sp, #0x14]
007ec31c: ldr      r1, [r5, #8]
007ec320: mov      r2, r0
007ec324: mov      r0, r3
007ec328: str      r2, [sp, #0x10]
007ec32c: bl       #0x30eba4
007ec330: ldr      r2, [sp, #0x10]
007ec334: ldr      r1, [r5, #0x2c]
007ec338: mov      r3, r0
007ec33c: mov      r0, r2
007ec340: str      r3, [sp, #0x14]
007ec344: bl       #0x30e3ac
007ec348: ldr      r3, [sp, #0x14]
007ec34c: ldr      r1, [r5, #0x30]
007ec350: mov      r2, r0
007ec354: mov      r0, r3
007ec358: str      r2, [sp, #0x10]
007ec35c: bl       #0x30e3ac
007ec360: ldr      r3, [r5, #0x48]
007ec364: add      r1, r3, #0x80000000
007ec368: bl       #0x30ed6c
007ec36c: ldr      r2, [sp, #0x10]
007ec370: mov      r3, r0
007ec374: ldr      r0, [r5, #0x48]
007ec378: mov      r1, r2
007ec37c: str      r3, [sp, #0x14]
007ec380: bl       #0x30ed6c
007ec384: ldr      r3, [sp, #0x14]
007ec388: mov      r2, r0
007ec38c: ldr      r1, [r5, #0x40]
007ec390: mov      r0, r3
007ec394: str      r2, [sp, #0x10]
007ec398: bl       #0x30eba4
007ec39c: ldr      r2, [sp, #0x10]
007ec3a0: mov      r3, r0
007ec3a4: ldr      r1, [r5, #0x44]
007ec3a8: mov      r0, r2
007ec3ac: str      r3, [sp, #0x14]
007ec3b0: bl       #0x30eba4
007ec3b4: ldr      r1, [sp, #0x20]
007ec3b8: bl       #0x30e3ac
007ec3bc: ldr      r3, [sp, #0x14]
007ec3c0: str      r0, [sp, #0x88]
007ec3c4: ldr      r1, [sp, #0x1c]
007ec3c8: mov      r0, r3
007ec3cc: bl       #0x30e3ac
007ec3d0: ldr      r2, [r6, #0x88]
007ec3d4: ldr      r3, [r6, #0x8c]
007ec3d8: str      r0, [sp, #0x84]
007ec3dc: str      r2, [sp, #0x8c]
007ec3e0: str      r3, [sp, #0x90]
007ec3e4: ldr      r2, [sl, #0x58]
007ec3e8: str      sb, [sp, #0xa0]
007ec3ec: ldr      r3, [r7]
007ec3f0: mov      r0, r7
007ec3f4: str      r2, [sp, #0x94]
007ec3f8: ldr      r1, [sp, #0x24]
007ec3fc: mov      lr, pc
007ec400: ldr      pc, [r3, #8]
007ec404: ldr      r3, [r6, #0x90]
007ec408: add      fp, fp, #1
007ec40c: add      sl, sl, #0x20
007ec410: cmp      r3, fp
007ec414: bgt      #0x7ec060
007ec418: mov      r3, #1
007ec41c: str      r3, [r6, #8]
007ec420: cmp      r7, #0
007ec424: beq      #0x7ec70c
007ec428: ldr      r3, [sp, #0x70]
007ec42c: cmp      r3, #0
007ec430: ble      #0x7ec70c
007ec434: ldr      r1, [sp, #0x18]
007ec438: add      r2, sp, #0x74
007ec43c: mov      r8, #0
007ec440: add      r6, r1, #0x10
007ec444: add      ip, sp, #0xa4
007ec448: str      r2, [sp, #0x18]
007ec44c: ldrb     r2, [ip, r8]
007ec450: cmp      r2, #0
007ec454: bne      #0x7ec6fc
007ec458: ldr      sb, [r6, #-0x10]
007ec45c: ldr      r1, [r4, #0xc]
007ec460: ldr      sl, [r6, #-0xc]
007ec464: mov      r0, sb
007ec468: str      ip, [sp, #0xc]
007ec46c: bl       #0x30ed6c
007ec470: ldr      r1, [r4, #0x14]
007ec474: mov      fp, r0
007ec478: mov      r0, sl
007ec47c: bl       #0x30ed6c
007ec480: mov      r1, r0
007ec484: mov      r0, fp
007ec488: bl       #0x30eba4
007ec48c: ldr      r1, [r4, #0x10]
007ec490: mov      fp, r0
007ec494: mov      r0, sb
007ec498: bl       #0x30ed6c
007ec49c: ldr      r1, [r4, #0x18]
007ec4a0: mov      sb, r0
007ec4a4: mov      r0, sl
007ec4a8: bl       #0x30ed6c
007ec4ac: mov      r1, r0
007ec4b0: mov      r0, sb
007ec4b4: bl       #0x30eba4
007ec4b8: ldr      r1, [r4, #4]
007ec4bc: mov      sl, r0
007ec4c0: mov      r0, fp
007ec4c4: bl       #0x30eba4
007ec4c8: ldr      r1, [r4, #8]
007ec4cc: mov      sb, r0
007ec4d0: mov      r0, sl
007ec4d4: bl       #0x30eba4
007ec4d8: str      sb, [sp, #0x7c]
007ec4dc: str      r0, [sp, #0x80]
007ec4e0: ldr      sb, [r6, #-0x10]
007ec4e4: ldr      r1, [r4, #0xc]
007ec4e8: ldr      sl, [r6, #-0xc]
007ec4ec: mov      r0, sb
007ec4f0: bl       #0x30ed6c
007ec4f4: ldr      r1, [r4, #0x14]
007ec4f8: mov      fp, r0
007ec4fc: mov      r0, sl
007ec500: bl       #0x30ed6c
007ec504: mov      r1, r0
007ec508: mov      r0, fp
007ec50c: bl       #0x30eba4
007ec510: ldr      r1, [r4, #0x10]
007ec514: mov      fp, r0
007ec518: mov      r0, sb
007ec51c: bl       #0x30ed6c
007ec520: ldr      r1, [r4, #0x18]
007ec524: mov      sb, r0
007ec528: mov      r0, sl
007ec52c: bl       #0x30ed6c
007ec530: mov      r1, r0
007ec534: mov      r0, sb
007ec538: bl       #0x30eba4
007ec53c: ldr      r1, [r4, #4]
007ec540: mov      sl, r0
007ec544: mov      r0, fp
007ec548: bl       #0x30eba4
007ec54c: ldr      r1, [r4, #8]
007ec550: mov      fp, r0
007ec554: mov      r0, sl
007ec558: bl       #0x30eba4
007ec55c: ldr      r1, [r4, #0x2c]
007ec560: mov      sb, r0
007ec564: mov      r0, fp
007ec568: bl       #0x30e3ac
007ec56c: ldr      sl, [r4, #0x48]
007ec570: mov      fp, r0
007ec574: ldr      r1, [r4, #0x30]
007ec578: mov      r0, sb
007ec57c: bl       #0x30e3ac
007ec580: add      r1, sl, #0x80000000
007ec584: bl       #0x30ed6c
007ec588: mov      r1, fp
007ec58c: mov      sb, r0
007ec590: mov      r0, sl
007ec594: bl       #0x30ed6c
007ec598: ldr      r1, [r4, #0x40]
007ec59c: mov      sl, r0
007ec5a0: mov      r0, sb
007ec5a4: bl       #0x30eba4
007ec5a8: ldr      r1, [r4, #0x44]
007ec5ac: mov      r2, r0
007ec5b0: mov      r0, sl
007ec5b4: str      r2, [sp, #0x10]
007ec5b8: bl       #0x30eba4
007ec5bc: ldr      sb, [r6, #-8]
007ec5c0: mov      r3, r0
007ec5c4: ldr      r1, [r5, #0xc]
007ec5c8: mov      r0, sb
007ec5cc: ldr      sl, [r6, #-4]
007ec5d0: str      r3, [sp, #0x14]
007ec5d4: bl       #0x30ed6c
007ec5d8: ldr      r1, [r5, #0x14]
007ec5dc: mov      fp, r0
007ec5e0: mov      r0, sl
007ec5e4: bl       #0x30ed6c
007ec5e8: mov      r1, r0
007ec5ec: mov      r0, fp
007ec5f0: bl       #0x30eba4
007ec5f4: ldr      r1, [r5, #0x10]
007ec5f8: mov      fp, r0
007ec5fc: mov      r0, sb
007ec600: bl       #0x30ed6c
007ec604: ldr      r1, [r5, #0x18]
007ec608: mov      sb, r0
007ec60c: mov      r0, sl
007ec610: bl       #0x30ed6c
007ec614: mov      r1, r0
007ec618: mov      r0, sb
007ec61c: bl       #0x30eba4
007ec620: ldr      r1, [r5, #4]
007ec624: mov      sl, r0
007ec628: mov      r0, fp
007ec62c: bl       #0x30eba4
007ec630: ldr      r1, [r5, #8]
007ec634: mov      fp, r0
007ec638: mov      r0, sl
007ec63c: bl       #0x30eba4
007ec640: ldr      r1, [r5, #0x2c]
007ec644: mov      sb, r0
007ec648: mov      r0, fp
007ec64c: bl       #0x30e3ac
007ec650: ldr      sl, [r5, #0x48]
007ec654: ldr      r1, [r5, #0x30]
007ec658: mov      fp, r0
007ec65c: mov      r0, sb
007ec660: bl       #0x30e3ac
007ec664: add      r1, sl, #0x80000000
007ec668: bl       #0x30ed6c
007ec66c: mov      r1, fp
007ec670: mov      sb, r0
007ec674: mov      r0, sl
007ec678: bl       #0x30ed6c
007ec67c: ldr      r1, [r5, #0x40]
007ec680: mov      sl, r0
007ec684: mov      r0, sb
007ec688: bl       #0x30eba4
007ec68c: ldr      r1, [r5, #0x44]
007ec690: mov      sb, r0
007ec694: mov      r0, sl
007ec698: bl       #0x30eba4
007ec69c: ldr      r3, [sp, #0x14]
007ec6a0: mov      r1, r3
007ec6a4: bl       #0x30e3ac
007ec6a8: ldr      r2, [sp, #0x10]
007ec6ac: str      r0, [sp, #0x88]
007ec6b0: mov      r0, sb
007ec6b4: mov      r1, r2
007ec6b8: bl       #0x30e3ac
007ec6bc: ldr      r3, [sp, #0x68]
007ec6c0: str      r0, [sp, #0x84]
007ec6c4: mov      r0, r7
007ec6c8: str      r3, [sp, #0x8c]
007ec6cc: ldr      r3, [sp, #0x6c]
007ec6d0: ldr      r1, [sp, #0x18]
007ec6d4: str      r3, [sp, #0x90]
007ec6d8: ldr      r2, [r6]
007ec6dc: ldr      r3, [r7]
007ec6e0: str      r2, [sp, #0x94]
007ec6e4: ldr      r2, [r6, #0xc]
007ec6e8: str      r2, [sp, #0xa0]
007ec6ec: mov      lr, pc
007ec6f0: ldr      pc, [r3, #0x10]
007ec6f4: ldr      r3, [sp, #0x70]
007ec6f8: ldr      ip, [sp, #0xc]
007ec6fc: add      r8, r8, #1
007ec700: cmp      r3, r8
007ec704: add      r6, r6, #0x20
007ec708: bgt      #0x7ec44c
007ec70c: add      sp, sp, #0xac
007ec710: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ec714: add      r1, sp, #0xa8
007ec718: add      r3, r1, r3
007ec71c: mov      r1, #1
007ec720: strb     r1, [r3, #-4]
007ec724: ldr      r3, [r2, #0x14]
007ec728: cmp      r7, #0
007ec72c: str      r3, [sl, #0x5c]
007ec730: ldr      r3, [r2, #0x18]
007ec734: str      r3, [sl, #0x60]
007ec738: beq      #0x7ec404
007ec73c: ldr      r2, [sl, #0x48]
007ec740: ldr      r1, [r4, #0xc]
007ec744: mov      r0, r2
007ec748: str      r2, [sp, #0x10]
007ec74c: bl       #0x30ed6c
007ec750: ldr      r1, [r4, #0x14]
007ec754: mov      r3, r0
007ec758: ldr      r0, [sl, #0x4c]
007ec75c: str      r3, [sp, #0x14]
007ec760: bl       #0x30ed6c
007ec764: ldr      r3, [sp, #0x14]
007ec768: mov      r1, r0
007ec76c: mov      r0, r3
007ec770: bl       #0x30eba4
007ec774: ldr      r2, [sp, #0x10]
007ec778: ldr      r1, [r4, #0x10]
007ec77c: mov      ip, r0
007ec780: mov      r0, r2
007ec784: str      ip, [sp, #0xc]
007ec788: bl       #0x30ed6c
007ec78c: ldr      r1, [r4, #0x18]
007ec790: mov      r3, r0
007ec794: ldr      r0, [sl, #0x4c]
007ec798: str      r3, [sp, #0x14]
007ec79c: bl       #0x30ed6c
007ec7a0: ldr      r3, [sp, #0x14]
007ec7a4: mov      r1, r0
007ec7a8: mov      r0, r3
007ec7ac: bl       #0x30eba4
007ec7b0: ldr      ip, [sp, #0xc]
007ec7b4: ldr      r1, [r4, #4]
007ec7b8: mov      r3, r0
007ec7bc: mov      r0, ip
007ec7c0: str      r3, [sp, #0x14]
007ec7c4: bl       #0x30eba4
007ec7c8: ldr      r3, [sp, #0x14]
007ec7cc: ldr      r1, [r4, #8]
007ec7d0: mov      r2, r0
007ec7d4: mov      r0, r3
007ec7d8: str      r2, [sp, #0x10]
007ec7dc: bl       #0x30eba4
007ec7e0: ldr      r2, [sp, #0x10]
007ec7e4: str      r0, [sp, #0x80]
007ec7e8: str      r2, [sp, #0x7c]
007ec7ec: ldr      r2, [sl, #0x48]
007ec7f0: ldr      r1, [r4, #0xc]
007ec7f4: mov      r0, r2
007ec7f8: str      r2, [sp, #0x10]
007ec7fc: bl       #0x30ed6c
007ec800: ldr      r1, [r4, #0x14]
007ec804: mov      r3, r0
007ec808: ldr      r0, [sl, #0x4c]
007ec80c: str      r3, [sp, #0x14]
007ec810: bl       #0x30ed6c
007ec814: ldr      r3, [sp, #0x14]
007ec818: mov      r1, r0
007ec81c: mov      r0, r3
007ec820: bl       #0x30eba4
007ec824: ldr      r2, [sp, #0x10]
007ec828: mov      ip, r0
007ec82c: ldr      r1, [r4, #0x10]
007ec830: mov      r0, r2
007ec834: str      ip, [sp, #0xc]
007ec838: bl       #0x30ed6c
007ec83c: ldr      r1, [r4, #0x18]
007ec840: mov      r3, r0
007ec844: ldr      r0, [sl, #0x4c]
007ec848: str      r3, [sp, #0x14]
007ec84c: bl       #0x30ed6c
007ec850: ldr      r3, [sp, #0x14]
007ec854: mov      r1, r0
007ec858: mov      r0, r3
007ec85c: bl       #0x30eba4
007ec860: ldr      ip, [sp, #0xc]
007ec864: ldr      r1, [r4, #4]
007ec868: mov      r3, r0
007ec86c: mov      r0, ip
007ec870: str      r3, [sp, #0x14]
007ec874: bl       #0x30eba4
007ec878: ldr      r3, [sp, #0x14]
007ec87c: ldr      r1, [r4, #8]
007ec880: mov      r2, r0
007ec884: mov      r0, r3
007ec888: str      r2, [sp, #0x10]
007ec88c: bl       #0x30eba4
007ec890: ldr      r2, [sp, #0x10]
007ec894: ldr      r1, [r4, #0x2c]
007ec898: mov      ip, r0
007ec89c: mov      r0, r2
007ec8a0: str      ip, [sp, #0xc]
007ec8a4: bl       #0x30e3ac
007ec8a8: ldr      ip, [sp, #0xc]
007ec8ac: ldr      r1, [r4, #0x30]
007ec8b0: mov      r3, r0
007ec8b4: mov      r0, ip
007ec8b8: str      r3, [sp, #0x14]
007ec8bc: bl       #0x30e3ac
007ec8c0: ldr      r2, [r4, #0x48]
007ec8c4: add      r1, r2, #0x80000000
007ec8c8: bl       #0x30ed6c
007ec8cc: ldr      r3, [sp, #0x14]
007ec8d0: mov      r2, r0
007ec8d4: ldr      r0, [r4, #0x48]
007ec8d8: mov      r1, r3
007ec8dc: str      r2, [sp, #0x10]
007ec8e0: bl       #0x30ed6c
007ec8e4: ldr      r2, [sp, #0x10]
007ec8e8: ldr      r1, [r4, #0x40]
007ec8ec: mov      r3, r0
007ec8f0: mov      r0, r2
007ec8f4: str      r3, [sp, #0x14]
007ec8f8: bl       #0x30eba4
007ec8fc: ldr      r3, [sp, #0x14]
007ec900: str      r0, [sp, #0x1c]
007ec904: ldr      r1, [r4, #0x44]
007ec908: mov      r0, r3
007ec90c: bl       #0x30eba4
007ec910: str      r0, [sp, #0x20]
007ec914: ldr      r2, [sl, #0x50]
007ec918: ldr      r1, [r5, #0xc]
007ec91c: mov      r0, r2
007ec920: str      r2, [sp, #0x10]
007ec924: bl       #0x30ed6c
007ec928: ldr      r1, [r5, #0x14]
007ec92c: mov      r3, r0
007ec930: ldr      r0, [sl, #0x54]
007ec934: str      r3, [sp, #0x14]
007ec938: bl       #0x30ed6c
007ec93c: ldr      r3, [sp, #0x14]
007ec940: mov      r1, r0
007ec944: mov      r0, r3
007ec948: bl       #0x30eba4
007ec94c: ldr      r2, [sp, #0x10]
007ec950: ldr      r1, [r5, #0x10]
007ec954: mov      ip, r0
007ec958: mov      r0, r2
007ec95c: str      ip, [sp, #0xc]
007ec960: bl       #0x30ed6c
007ec964: ldr      r1, [r5, #0x18]
007ec968: mov      r3, r0
007ec96c: ldr      r0, [sl, #0x54]
007ec970: str      r3, [sp, #0x14]
007ec974: bl       #0x30ed6c
007ec978: ldr      r3, [sp, #0x14]
007ec97c: mov      r1, r0
007ec980: mov      r0, r3
007ec984: bl       #0x30eba4
007ec988: ldr      ip, [sp, #0xc]
007ec98c: ldr      r1, [r5, #4]
007ec990: mov      r3, r0
007ec994: mov      r0, ip
007ec998: str      r3, [sp, #0x14]
007ec99c: bl       #0x30eba4
007ec9a0: ldr      r3, [sp, #0x14]
007ec9a4: ldr      r1, [r5, #8]
007ec9a8: mov      r2, r0
007ec9ac: mov      r0, r3
007ec9b0: str      r2, [sp, #0x10]
007ec9b4: bl       #0x30eba4
007ec9b8: ldr      r2, [sp, #0x10]
007ec9bc: ldr      r1, [r5, #0x2c]
007ec9c0: mov      r3, r0
007ec9c4: mov      r0, r2
007ec9c8: str      r3, [sp, #0x14]
007ec9cc: bl       #0x30e3ac
007ec9d0: ldr      r3, [sp, #0x14]
007ec9d4: ldr      r1, [r5, #0x30]
007ec9d8: mov      r2, r0
007ec9dc: mov      r0, r3
007ec9e0: str      r2, [sp, #0x10]
007ec9e4: bl       #0x30e3ac
007ec9e8: ldr      r3, [r5, #0x48]
007ec9ec: add      r1, r3, #0x80000000
007ec9f0: bl       #0x30ed6c
007ec9f4: ldr      r2, [sp, #0x10]
007ec9f8: mov      r3, r0
007ec9fc: ldr      r0, [r5, #0x48]
007eca00: mov      r1, r2
007eca04: str      r3, [sp, #0x14]
007eca08: bl       #0x30ed6c
007eca0c: ldr      r3, [sp, #0x14]
007eca10: ldr      r1, [r5, #0x40]
007eca14: mov      r2, r0
007eca18: mov      r0, r3
007eca1c: str      r2, [sp, #0x10]
007eca20: bl       #0x30eba4
007eca24: ldr      r2, [sp, #0x10]
007eca28: mov      r3, r0
007eca2c: ldr      r1, [r5, #0x44]
007eca30: mov      r0, r2
007eca34: str      r3, [sp, #0x14]
007eca38: bl       #0x30eba4
007eca3c: ldr      r1, [sp, #0x20]
007eca40: bl       #0x30e3ac
007eca44: ldr      r3, [sp, #0x14]
007eca48: str      r0, [sp, #0x88]
007eca4c: ldr      r1, [sp, #0x1c]
007eca50: mov      r0, r3
007eca54: bl       #0x30e3ac
007eca58: ldr      r2, [r6, #0x88]
007eca5c: ldr      r3, [r6, #0x8c]
007eca60: str      r0, [sp, #0x84]
007eca64: str      r2, [sp, #0x8c]
007eca68: str      r3, [sp, #0x90]
007eca6c: ldr      r2, [sl, #0x58]
007eca70: str      sb, [sp, #0xa0]
007eca74: ldr      r3, [r7]
007eca78: mov      r0, r7
007eca7c: str      r2, [sp, #0x94]
007eca80: ldr      r1, [sp, #0x24]
007eca84: mov      lr, pc
007eca88: ldr      pc, [r3, #0xc]
007eca8c: b        #0x7ec404

# _ZNK13b2CircleShape11ComputeMassEP10b2MassData
007e95ac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e95b0: ldr      r6, [r0, #0x38]
007e95b4: mov      r5, r1
007e95b8: movw     r1, #0xfdb
007e95bc: mov      r4, r0
007e95c0: movt     r1, #0x4049
007e95c4: ldr      r0, [r0, #0x14]
007e95c8: bl       #0x30ed6c
007e95cc: mov      r1, r6
007e95d0: bl       #0x30ed6c
007e95d4: mov      r1, r0
007e95d8: mov      r0, r6
007e95dc: bl       #0x30ed6c
007e95e0: str      r0, [r5]
007e95e4: ldr      r3, [r4, #0x30]
007e95e8: mov      r6, r0
007e95ec: mov      r1, #0x3f000000
007e95f0: str      r3, [r5, #4]
007e95f4: ldr      r3, [r4, #0x34]
007e95f8: str      r3, [r5, #8]
007e95fc: ldr      r7, [r4, #0x38]
007e9600: ldr      sl, [r4, #0x30]
007e9604: ldr      r8, [r4, #0x34]
007e9608: mov      r0, r7
007e960c: bl       #0x30ed6c
007e9610: mov      r1, r0
007e9614: mov      r0, r7
007e9618: bl       #0x30ed6c
007e961c: mov      r1, sl
007e9620: mov      r4, r0
007e9624: mov      r0, sl
007e9628: bl       #0x30ed6c
007e962c: mov      r1, r8
007e9630: mov      r7, r0
007e9634: mov      r0, r8
007e9638: bl       #0x30ed6c
007e963c: mov      r1, r0
007e9640: mov      r0, r7
007e9644: bl       #0x30eba4
007e9648: mov      r1, r0
007e964c: mov      r0, r4
007e9650: bl       #0x30eba4
007e9654: mov      r1, r0
007e9658: mov      r0, r6
007e965c: bl       #0x30ed6c
007e9660: str      r0, [r5, #0xc]
007e9664: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN14b2PolygonShapeC1EPK10b2ShapeDef
007e5928: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e592c: ldr      r6, [pc, #0x2f8]
007e5930: sub      sp, sp, #0x2c
007e5934: mov      r4, r0
007e5938: mov      r5, r1
007e593c: bl       #0x7e6068
007e5940: ldr      r3, [pc, #0x2e8]
007e5944: add      r6, pc, r6
007e5948: mov      r2, #1
007e594c: ldr      r3, [r6, r3]
007e5950: str      r2, [r4, #4]
007e5954: add      r3, r3, #8
007e5958: str      r3, [r4]
007e595c: ldr      r3, [r5, #0x60]
007e5960: cmp      r3, #0
007e5964: str      r3, [r4, #0x118]
007e5968: ble      #0x7e5a10
007e596c: mov      r2, r5
007e5970: mov      r6, r4
007e5974: mov      r3, r4
007e5978: mov      r1, #0
007e597c: ldr      r0, [r2, #0x20]
007e5980: add      r1, r1, #1
007e5984: str      r0, [r3, #0x58]
007e5988: ldr      r0, [r2, #0x24]
007e598c: add      r2, r2, #8
007e5990: str      r0, [r3, #0x5c]
007e5994: ldr      sl, [r4, #0x118]
007e5998: add      r3, r3, #8
007e599c: cmp      sl, r1
007e59a0: bgt      #0x7e597c
007e59a4: cmp      sl, #0
007e59a8: ble      #0x7e5a10
007e59ac: mov      r8, #0
007e59b0: add      r7, r8, #1
007e59b4: cmp      r7, sl
007e59b8: movlt    sl, r7
007e59bc: movge    sl, #0
007e59c0: add      sl, sl, #0xb
007e59c4: ldr      r0, [r4, sl, lsl #3]
007e59c8: ldr      r1, [r6, #0x58]
007e59cc: bl       #0x30e3ac
007e59d0: add      sl, r4, sl, lsl #3
007e59d4: ldr      r1, [r6, #0x5c]
007e59d8: mov      sb, r0
007e59dc: ldr      r0, [sl, #4]
007e59e0: bl       #0x30e3ac
007e59e4: add      r8, r8, #0x13
007e59e8: add      sb, sb, #0x80000000
007e59ec: str      r0, [r6, #0x98]
007e59f0: str      sb, [r6, #0x9c]
007e59f4: add      r0, r4, r8, lsl #3
007e59f8: bl       #0x7e5524
007e59fc: ldr      sl, [r4, #0x118]
007e5a00: add      r6, r6, #8
007e5a04: mov      r8, r7
007e5a08: cmp      sl, r7
007e5a0c: bgt      #0x7e59b0
007e5a10: ldr      r2, [r5, #0x60]
007e5a14: add      r0, sp, #0x20
007e5a18: add      r1, r5, #0x20
007e5a1c: bl       #0x7e44f4
007e5a20: ldr      r3, [sp, #0x24]
007e5a24: ldr      r2, [sp, #0x20]
007e5a28: add      r0, r4, #0x38
007e5a2c: str      r3, [r4, #0x34]
007e5a30: str      r2, [r4, #0x30]
007e5a34: add      r1, r4, #0x58
007e5a38: ldr      r2, [r4, #0x118]
007e5a3c: bl       #0x7e55ac
007e5a40: ldr      r3, [r4, #0x118]
007e5a44: cmp      r3, #0
007e5a48: str      r3, [sp, #0x18]
007e5a4c: ble      #0x7e5c20
007e5a50: ldr      r3, [r4, #0x30]
007e5a54: mov      r5, r4
007e5a58: mov      r6, #0
007e5a5c: str      r3, [sp, #0x10]
007e5a60: ldr      r3, [r4, #0x34]
007e5a64: mov      r2, r4
007e5a68: str      r3, [sp, #0x14]
007e5a6c: ldr      r3, [sp, #0x18]
007e5a70: sub      r3, r3, #1
007e5a74: str      r3, [sp, #0x1c]
007e5a78: cmp      r6, #0
007e5a7c: ldreq    r3, [sp, #0x1c]
007e5a80: subne    r3, r6, #1
007e5a84: ldr      r0, [r5, #0x58]
007e5a88: add      r3, r3, #0x13
007e5a8c: add      ip, r2, r3, lsl #3
007e5a90: ldr      r1, [sp, #0x10]
007e5a94: ldr      r7, [r2, r3, lsl #3]
007e5a98: ldr      r8, [ip, #4]
007e5a9c: str      r2, [sp, #4]
007e5aa0: bl       #0x30e3ac
007e5aa4: ldr      r1, [sp, #0x14]
007e5aa8: mov      sl, r0
007e5aac: ldr      r0, [r5, #0x5c]
007e5ab0: bl       #0x30e3ac
007e5ab4: mov      r1, sl
007e5ab8: mov      fp, r0
007e5abc: mov      r0, r7
007e5ac0: bl       #0x30ed6c
007e5ac4: mov      r1, fp
007e5ac8: mov      r4, r0
007e5acc: mov      r0, r8
007e5ad0: bl       #0x30ed6c
007e5ad4: mov      r1, r0
007e5ad8: mov      r0, r4
007e5adc: bl       #0x30eba4
007e5ae0: movw     r1, #0xd70a
007e5ae4: movt     r1, #0x3d23
007e5ae8: bl       #0x30e3ac
007e5aec: ldr      r4, [r5, #0x98]
007e5af0: mov      sb, r0
007e5af4: mov      r1, sl
007e5af8: mov      r0, r4
007e5afc: bl       #0x30ed6c
007e5b00: ldr      sl, [r5, #0x9c]
007e5b04: mov      r3, r0
007e5b08: mov      r1, fp
007e5b0c: mov      r0, sl
007e5b10: str      r3, [sp, #8]
007e5b14: bl       #0x30ed6c
007e5b18: ldr      r3, [sp, #8]
007e5b1c: mov      r1, r0
007e5b20: add      r6, r6, #1
007e5b24: mov      r0, r3
007e5b28: bl       #0x30eba4
007e5b2c: movw     r1, #0xd70a
007e5b30: movt     r1, #0x3d23
007e5b34: bl       #0x30e3ac
007e5b38: mov      r1, r7
007e5b3c: mov      fp, r0
007e5b40: mov      r0, sl
007e5b44: bl       #0x30ed6c
007e5b48: mov      r1, r8
007e5b4c: mov      r3, r0
007e5b50: mov      r0, r4
007e5b54: str      r3, [sp, #8]
007e5b58: bl       #0x30ed6c
007e5b5c: ldr      r3, [sp, #8]
007e5b60: mov      r1, r0
007e5b64: mov      r0, r3
007e5b68: bl       #0x30e3ac
007e5b6c: mov      r1, r0
007e5b70: mov      r0, #0x3f800000
007e5b74: bl       #0x30ec94
007e5b78: mov      r1, sb
007e5b7c: str      r0, [sp, #0xc]
007e5b80: mov      r0, sl
007e5b84: bl       #0x30ed6c
007e5b88: mov      r1, fp
007e5b8c: mov      sl, r0
007e5b90: mov      r0, r8
007e5b94: bl       #0x30ed6c
007e5b98: mov      r1, r0
007e5b9c: mov      r0, sl
007e5ba0: bl       #0x30e3ac
007e5ba4: mov      r1, r0
007e5ba8: ldr      r0, [sp, #0xc]
007e5bac: bl       #0x30ed6c
007e5bb0: mov      r1, fp
007e5bb4: mov      r8, r0
007e5bb8: mov      r0, r7
007e5bbc: bl       #0x30ed6c
007e5bc0: mov      r1, sb
007e5bc4: mov      r7, r0
007e5bc8: mov      r0, r4
007e5bcc: bl       #0x30ed6c
007e5bd0: mov      r1, r0
007e5bd4: mov      r0, r7
007e5bd8: bl       #0x30e3ac
007e5bdc: mov      r1, r0
007e5be0: ldr      r0, [sp, #0xc]
007e5be4: bl       #0x30ed6c
007e5be8: mov      r1, r0
007e5bec: ldr      r0, [sp, #0x14]
007e5bf0: bl       #0x30eba4
007e5bf4: mov      r1, r8
007e5bf8: str      r0, [r5, #0xdc]
007e5bfc: ldr      r0, [sp, #0x10]
007e5c00: bl       #0x30eba4
007e5c04: ldr      r3, [sp, #0x18]
007e5c08: str      r0, [r5, #0xd8]
007e5c0c: ldr      r2, [sp, #4]
007e5c10: cmp      r6, r3
007e5c14: add      r5, r5, #8
007e5c18: blt      #0x7e5a78
007e5c1c: mov      r4, r2
007e5c20: mov      r0, r4
007e5c24: add      sp, sp, #0x2c
007e5c28: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e5c2c: andseq   pc, sl, ip, asr #2
007e5c30: andeq    r1, r0, r8, lsl #17

# _ZN14b2PolygonShape17UpdateSweepRadiusERK6b2Vec2
007e5478: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e547c: ldr      r3, [r0, #0x118]
007e5480: mov      r7, #0
007e5484: mov      r8, r0
007e5488: cmp      r3, #0
007e548c: mov      sl, r1
007e5490: str      r7, [r0, #0x10]
007e5494: ble      #0x7e5520
007e5498: mov      r4, r0
007e549c: mov      r6, #0
007e54a0: b        #0x7e54a8
007e54a4: mov      r7, r5
007e54a8: ldr      r1, [sl]
007e54ac: ldr      r0, [r4, #0xd8]
007e54b0: bl       #0x30e3ac
007e54b4: ldr      r1, [sl, #4]
007e54b8: mov      r5, r0
007e54bc: ldr      r0, [r4, #0xdc]
007e54c0: bl       #0x30e3ac
007e54c4: mov      r1, r5
007e54c8: mov      sb, r0
007e54cc: mov      r0, r5
007e54d0: bl       #0x30ed6c
007e54d4: mov      r1, sb
007e54d8: mov      r5, r0
007e54dc: mov      r0, sb
007e54e0: bl       #0x30ed6c
007e54e4: mov      r1, r0
007e54e8: mov      r0, r5
007e54ec: bl       #0x30eba4
007e54f0: bl       #0x30e124
007e54f4: mov      r1, r7
007e54f8: mov      r5, r0
007e54fc: bl       #0x30e70c
007e5500: ldr      r3, [r8, #0x118]
007e5504: cmp      r0, #0
007e5508: add      r6, r6, #1
007e550c: movne    r5, r7
007e5510: cmp      r3, r6
007e5514: str      r5, [r8, #0x10]
007e5518: add      r4, r4, #8
007e551c: bgt      #0x7e54a4
007e5520: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN15b2CircleContactC2EP7b2ShapeS1_
007f3af8: push     {r4, r5, r6, lr}
007f3afc: ldr      r5, [pc, #0x34]
007f3b00: mov      r4, r0
007f3b04: bl       #0x7e9eb8
007f3b08: ldr      r3, [pc, #0x2c]
007f3b0c: add      r5, pc, r5
007f3b10: mov      r2, #0
007f3b14: ldr      r3, [r5, r3]
007f3b18: mov      r1, #0
007f3b1c: str      r1, [r4, #0x90]
007f3b20: add      r3, r3, #8
007f3b24: str      r3, [r4]
007f3b28: str      r2, [r4, #0x60]
007f3b2c: str      r2, [r4, #0x5c]
007f3b30: mov      r0, r4
007f3b34: pop      {r4, r5, r6, pc}
007f3b38: andseq   r0, sl, r4, lsl #31
007f3b3c: andeq    r3, r0, r4, lsl #2

# _ZNK11b2DebugDraw8GetFlagsEv
007e8cfc: ldr      r0, [r0, #4]
007e8d00: bx       lr

# _ZNK13b2PulleyJoint16GetGroundAnchor1Ev
007f0478: push     {r4, r5, r6, r7, r8, lr}
007f047c: ldr      r6, [r1, #0x44]
007f0480: mov      r4, r0
007f0484: mov      r5, r1
007f0488: ldr      r0, [r6, #8]
007f048c: ldr      r1, [r1, #0x4c]
007f0490: bl       #0x30eba4
007f0494: ldr      r1, [r5, #0x48]
007f0498: mov      r7, r0
007f049c: ldr      r0, [r6, #4]
007f04a0: bl       #0x30eba4
007f04a4: str      r7, [r4, #4]
007f04a8: str      r0, [r4]
007f04ac: mov      r0, r4
007f04b0: pop      {r4, r5, r6, r7, r8, pc}

# _ZN8b2Island8SolveTOIERK10b2TimeStep
007ea878: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ea87c: sub      sp, sp, #0x2c
007ea880: ldr      ip, [r0]
007ea884: add      r5, sp, #8
007ea888: ldr      r3, [r0, #0x1c]
007ea88c: ldr      r2, [r0, #0xc]
007ea890: mov      r4, r1
007ea894: mov      r6, r0
007ea898: mov      r0, r5
007ea89c: str      ip, [sp]
007ea8a0: bl       #0x7f7058
007ea8a4: ldr      r3, [r4, #0xc]
007ea8a8: cmp      r3, #0
007ea8ac: ble      #0x7ea8cc
007ea8b0: mov      r7, #0
007ea8b4: mov      r0, r5
007ea8b8: bl       #0x7f64a8
007ea8bc: ldr      r3, [r4, #0xc]
007ea8c0: add      r7, r7, #1
007ea8c4: cmp      r3, r7
007ea8c8: bgt      #0x7ea8b4
007ea8cc: ldr      r2, [r6, #0x14]
007ea8d0: cmp      r2, #0
007ea8d4: ble      #0x7ea980
007ea8d8: mov      r8, #0
007ea8dc: ldr      r3, [r6, #8]
007ea8e0: ldr      r7, [r3, r8, lsl #2]
007ea8e4: add      r8, r8, #1
007ea8e8: ldrsh    r3, [r7, #2]
007ea8ec: cmp      r3, #0
007ea8f0: beq      #0x7ea974
007ea8f4: ldr      r2, [r7, #0x2c]
007ea8f8: ldr      r3, [r7, #0x30]
007ea8fc: ldr      sl, [r7, #0x38]
007ea900: str      r2, [r7, #0x24]
007ea904: str      r3, [r7, #0x28]
007ea908: str      sl, [r7, #0x34]
007ea90c: ldr      sb, [r4]
007ea910: ldr      r1, [r7, #0x44]
007ea914: mov      r0, sb
007ea918: bl       #0x30ed6c
007ea91c: ldr      r1, [r7, #0x40]
007ea920: mov      fp, r0
007ea924: mov      r0, sb
007ea928: bl       #0x30ed6c
007ea92c: mov      r1, r0
007ea930: ldr      r0, [r7, #0x2c]
007ea934: bl       #0x30eba4
007ea938: mov      r1, fp
007ea93c: str      r0, [r7, #0x2c]
007ea940: ldr      r0, [r7, #0x30]
007ea944: bl       #0x30eba4
007ea948: str      r0, [r7, #0x30]
007ea94c: ldr      r0, [r4]
007ea950: ldr      r1, [r7, #0x48]
007ea954: bl       #0x30ed6c
007ea958: mov      r1, r0
007ea95c: mov      r0, sl
007ea960: bl       #0x30eba4
007ea964: str      r0, [r7, #0x38]
007ea968: mov      r0, r7
007ea96c: bl       #0x7e761c
007ea970: ldr      r2, [r6, #0x14]
007ea974: cmp      r2, r8
007ea978: bgt      #0x7ea8dc
007ea97c: ldr      r3, [r4, #0xc]
007ea980: cmp      r3, #0
007ea984: ble      #0x7ea9b4
007ea988: mov      r7, #0
007ea98c: b        #0x7ea99c
007ea990: ldr      r3, [r4, #0xc]
007ea994: cmp      r3, r7
007ea998: ble      #0x7ea9b4
007ea99c: mov      r0, r5
007ea9a0: mov      r1, #0x3f400000
007ea9a4: bl       #0x7f6b9c
007ea9a8: cmp      r0, #0
007ea9ac: add      r7, r7, #1
007ea9b0: beq      #0x7ea990
007ea9b4: mov      r0, r6
007ea9b8: ldr      r1, [sp, #0x20]
007ea9bc: bl       #0x7ea6a8
007ea9c0: mov      r0, r5
007ea9c4: bl       #0x7f7020
007ea9c8: add      sp, sp, #0x2c
007ea9cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6b2Body20SynchronizeTransformEv
007e761c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e7620: ldr      r6, [r0, #0x38]
007e7624: mov      r4, r0
007e7628: mov      r0, r6
007e762c: bl       #0x30e754
007e7630: mov      r5, r0
007e7634: mov      r0, r6
007e7638: bl       #0x30eb08
007e763c: ldr      r8, [r4, #0x1c]
007e7640: add      sl, r0, #0x80000000
007e7644: mov      r6, r0
007e7648: str      r5, [r4, #0xc]
007e764c: str      sl, [r4, #0x14]
007e7650: str      r0, [r4, #0x10]
007e7654: str      r5, [r4, #0x18]
007e7658: mov      r0, r5
007e765c: mov      r1, r8
007e7660: bl       #0x30ed6c
007e7664: ldr      r7, [r4, #0x20]
007e7668: mov      sb, r0
007e766c: mov      r0, sl
007e7670: mov      r1, r7
007e7674: bl       #0x30ed6c
007e7678: mov      r1, r0
007e767c: mov      r0, sb
007e7680: bl       #0x30eba4
007e7684: mov      r1, r8
007e7688: mov      sl, r0
007e768c: mov      r0, r6
007e7690: bl       #0x30ed6c
007e7694: mov      r1, r7
007e7698: mov      r6, r0
007e769c: mov      r0, r5
007e76a0: bl       #0x30ed6c
007e76a4: mov      r1, r0
007e76a8: mov      r0, r6
007e76ac: bl       #0x30eba4
007e76b0: mov      r1, sl
007e76b4: mov      r6, r0
007e76b8: ldr      r0, [r4, #0x2c]
007e76bc: bl       #0x30e3ac
007e76c0: mov      r1, r6
007e76c4: mov      r5, r0
007e76c8: ldr      r0, [r4, #0x30]
007e76cc: bl       #0x30e3ac
007e76d0: str      r5, [r4, #4]
007e76d4: str      r0, [r4, #8]
007e76d8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN12b2BroadPhase6CommitEv
007e2ac8: b        #0x7e42e0

# _ZN16b2ContactManagerD0Ev
007e9fc8: ldr      r3, [pc, #0x34]
007e9fcc: ldr      r1, [pc, #0x34]
007e9fd0: ldr      r2, [pc, #0x34]
007e9fd4: add      r3, pc, r3
007e9fd8: ldr      r1, [r3, r1]
007e9fdc: ldr      r2, [r3, r2]
007e9fe0: push     {r4, lr}
007e9fe4: add      r1, r1, #8
007e9fe8: add      r2, r2, #8
007e9fec: mov      r4, r0
007e9ff0: str      r1, [r0]
007e9ff4: str      r2, [r0, #8]
007e9ff8: bl       #0x30e2b0
007e9ffc: mov      r0, r4
007ea000: pop      {r4, pc}
007ea004: ldrheq   sl, [sl], -ip
007ea008: andeq    r2, r0, r8, lsl #1
007ea00c: andeq    r2, r0, r4, lsr r3

# _ZN13b2PairManager4FindEiij
007e3ef4: push     {r4, r5, r6, r7, r8}
007e3ef8: add      r3, r3, #0x20000
007e3efc: add      r3, r0, r3, lsl #1
007e3f00: ldrh     r3, [r3, #0x14]
007e3f04: movw     ip, #0xffff
007e3f08: cmp      r3, ip
007e3f0c: beq      #0x7e3f38
007e3f10: mov      r4, #0xc
007e3f14: mla      r8, r4, r3, r0
007e3f18: mul      r6, r4, r3
007e3f1c: add      r5, r8, #8
007e3f20: ldrh     r7, [r5, #4]
007e3f24: cmp      r1, r7
007e3f28: beq      #0x7e3f44
007e3f2c: ldrh     r3, [r8, #0x10]
007e3f30: cmp      r3, ip
007e3f34: bne      #0x7e3f14
007e3f38: mov      r0, #0
007e3f3c: pop      {r4, r5, r6, r7, r8}
007e3f40: bx       lr
007e3f44: ldrh     r5, [r5, #6]
007e3f48: cmp      r2, r5
007e3f4c: bne      #0x7e3f2c
007e3f50: movw     r2, #0xffff
007e3f54: cmp      r3, r2
007e3f58: addne    r6, r6, #8
007e3f5c: addne    r0, r0, r6
007e3f60: bne      #0x7e3f3c
007e3f64: b        #0x7e3f38

# _ZN13b2PairManager18RemoveBufferedPairEii
007e4224: push     {r4, r5, r6, lr}
007e4228: mov      r4, r0
007e422c: bl       #0x7e3f68
007e4230: ldr      r3, [pc, #0x9c]
007e4234: cmp      r0, #0
007e4238: add      r3, pc, r3
007e423c: beq      #0x7e4268
007e4240: ldrh     r2, [r0, #0xa]
007e4244: tst      r2, #1
007e4248: beq      #0x7e426c
007e424c: orr      r2, r2, #2
007e4250: strh     r2, [r0, #0xa]
007e4254: ldr      r2, [pc, #0x7c]
007e4258: ldr      r3, [r3, r2]
007e425c: ldrb     r3, [r3]
007e4260: cmp      r3, #0
007e4264: bne      #0x7e42c8
007e4268: pop      {r4, r5, r6, pc}
007e426c: orr      r2, r2, #1
007e4270: mov      r1, #0x40000
007e4274: strh     r2, [r0, #0xa]
007e4278: add      r1, r1, #0x10
007e427c: ldr      ip, [r4, r1]
007e4280: ldrh     r5, [r0, #4]
007e4284: add      r2, ip, #0xc000
007e4288: add      r2, r2, #4
007e428c: lsl      r2, r2, #2
007e4290: strh     r5, [r4, r2]
007e4294: ldrh     r5, [r0, #6]
007e4298: add      r2, r4, r2
007e429c: add      ip, ip, #1
007e42a0: strh     r5, [r2, #2]
007e42a4: str      ip, [r4, r1]
007e42a8: ldrh     r2, [r0, #0xa]
007e42ac: orr      r2, r2, #2
007e42b0: strh     r2, [r0, #0xa]
007e42b4: ldr      r2, [pc, #0x1c]
007e42b8: ldr      r3, [r3, r2]
007e42bc: ldrb     r3, [r3]
007e42c0: cmp      r3, #0
007e42c4: beq      #0x7e4268
007e42c8: mov      r0, r4
007e42cc: pop      {r4, r5, r6, lr}
007e42d0: b        #0x7e418c
007e42d4: andseq   r0, fp, r8, asr r8
007e42d8: andeq    r2, r0, r8, ror #26

# _ZN16b2PrismaticJoint24SolvePositionConstraintsEv
007ef010: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ef014: ldr      r5, [r0, #0x30]
007ef018: sub      sp, sp, #0x2c
007ef01c: mov      r6, r0
007ef020: ldr      r1, [r5, #0x1c]
007ef024: ldr      r0, [r0, #0x44]
007ef028: bl       #0x30e3ac
007ef02c: ldr      r1, [r5, #0x20]
007ef030: mov      r8, r0
007ef034: ldr      r0, [r6, #0x48]
007ef038: bl       #0x30e3ac
007ef03c: ldr      r2, [r5, #0xc]
007ef040: mov      r7, r0
007ef044: mov      r0, r8
007ef048: mov      r1, r2
007ef04c: ldr      r4, [r6, #0x34]
007ef050: str      r2, [sp, #8]
007ef054: bl       #0x30ed6c
007ef058: ldr      r1, [r5, #0x14]
007ef05c: mov      sl, r0
007ef060: mov      r0, r7
007ef064: bl       #0x30ed6c
007ef068: mov      r1, r0
007ef06c: mov      r0, sl
007ef070: bl       #0x30eba4
007ef074: ldr      ip, [r5, #0x10]
007ef078: mov      sb, r0
007ef07c: mov      r0, r8
007ef080: mov      r1, ip
007ef084: str      ip, [sp, #4]
007ef088: bl       #0x30ed6c
007ef08c: ldr      r1, [r5, #0x18]
007ef090: mov      r8, r0
007ef094: mov      r0, r7
007ef098: bl       #0x30ed6c
007ef09c: mov      r1, r0
007ef0a0: mov      r0, r8
007ef0a4: bl       #0x30eba4
007ef0a8: ldr      r1, [r4, #0x1c]
007ef0ac: mov      r8, r0
007ef0b0: ldr      r0, [r6, #0x4c]
007ef0b4: bl       #0x30e3ac
007ef0b8: ldr      r1, [r4, #0x20]
007ef0bc: mov      sl, r0
007ef0c0: ldr      r0, [r6, #0x50]
007ef0c4: bl       #0x30e3ac
007ef0c8: ldr      r1, [r4, #0xc]
007ef0cc: mov      r7, r0
007ef0d0: mov      r0, sl
007ef0d4: bl       #0x30ed6c
007ef0d8: ldr      r1, [r4, #0x14]
007ef0dc: mov      fp, r0
007ef0e0: mov      r0, r7
007ef0e4: bl       #0x30ed6c
007ef0e8: mov      r1, r0
007ef0ec: mov      r0, fp
007ef0f0: bl       #0x30eba4
007ef0f4: ldr      r1, [r4, #0x10]
007ef0f8: mov      fp, r0
007ef0fc: mov      r0, sl
007ef100: bl       #0x30ed6c
007ef104: ldr      r1, [r4, #0x18]
007ef108: mov      sl, r0
007ef10c: mov      r0, r7
007ef110: bl       #0x30ed6c
007ef114: mov      r1, r0
007ef118: mov      r0, sl
007ef11c: bl       #0x30eba4
007ef120: ldr      r7, [r5, #0x2c]
007ef124: ldr      r1, [r5, #0x78]
007ef128: mov      sl, r0
007ef12c: mov      r0, sb
007ef130: str      r1, [sp, #0x20]
007ef134: mov      r1, r7
007ef138: bl       #0x30eba4
007ef13c: ldr      r1, [r5, #0x30]
007ef140: mov      sb, r0
007ef144: mov      r0, r8
007ef148: bl       #0x30eba4
007ef14c: ldr      r1, [r4, #0x2c]
007ef150: mov      r8, r0
007ef154: mov      r0, fp
007ef158: bl       #0x30eba4
007ef15c: ldr      r1, [r4, #0x30]
007ef160: mov      fp, r0
007ef164: mov      r0, sl
007ef168: bl       #0x30eba4
007ef16c: mov      r1, sb
007ef170: mov      sl, r0
007ef174: mov      r0, fp
007ef178: bl       #0x30e3ac
007ef17c: mov      r1, r8
007ef180: mov      sb, r0
007ef184: mov      r0, sl
007ef188: bl       #0x30e3ac
007ef18c: ldr      r2, [sp, #8]
007ef190: ldr      sl, [r6, #0x5c]
007ef194: mov      r3, r0
007ef198: mov      r0, r2
007ef19c: mov      r1, sl
007ef1a0: ldr      r8, [r6, #0x60]
007ef1a4: str      r3, [sp]
007ef1a8: bl       #0x30ed6c
007ef1ac: mov      r1, r8
007ef1b0: mov      fp, r0
007ef1b4: ldr      r0, [r5, #0x14]
007ef1b8: bl       #0x30ed6c
007ef1bc: mov      r1, r0
007ef1c0: mov      r0, fp
007ef1c4: bl       #0x30eba4
007ef1c8: mov      r1, r0
007ef1cc: mov      r0, sb
007ef1d0: bl       #0x30ed6c
007ef1d4: ldr      ip, [sp, #4]
007ef1d8: mov      r1, sl
007ef1dc: mov      sb, r0
007ef1e0: mov      r0, ip
007ef1e4: bl       #0x30ed6c
007ef1e8: mov      r1, r8
007ef1ec: mov      sl, r0
007ef1f0: ldr      r0, [r5, #0x18]
007ef1f4: bl       #0x30ed6c
007ef1f8: mov      r1, r0
007ef1fc: mov      r0, sl
007ef200: bl       #0x30eba4
007ef204: ldr      r3, [sp]
007ef208: mov      r1, r0
007ef20c: mov      r0, r3
007ef210: bl       #0x30ed6c
007ef214: mov      r1, r0
007ef218: mov      r0, sb
007ef21c: bl       #0x30eba4
007ef220: movw     r1, #0xcccd
007ef224: movt     r1, #0x3e4c
007ef228: mov      r8, r0
007ef22c: bl       #0x30e70c
007ef230: ldr      r3, [r4, #0x78]
007ef234: cmp      r0, #0
007ef238: movweq   r8, #0xcccd
007ef23c: str      r3, [sp, #0x1c]
007ef240: ldr      r1, [r5, #0x80]
007ef244: movteq   r8, #0x3e4c
007ef248: str      r1, [sp, #0x10]
007ef24c: ldr      r3, [r4, #0x80]
007ef250: str      r3, [sp, #0xc]
007ef254: beq      #0x7ef4ac
007ef258: movw     r1, #0xcccd
007ef25c: mov      r0, r8
007ef260: movt     r1, #0xbe4c
007ef264: bl       #0x30e70c
007ef268: cmp      r0, #0
007ef26c: movwne   r8, #0xcccd
007ef270: movne    r3, #0
007ef274: movtne   r8, #0xbe4c
007ef278: beq      #0x7ef4ac
007ef27c: ldr      r0, [r6, #0x80]
007ef280: mov      r1, r8
007ef284: str      r3, [sp]
007ef288: add      r0, r0, #0x80000000
007ef28c: bl       #0x30ed6c
007ef290: mov      sl, r0
007ef294: mov      r1, sl
007ef298: ldr      r0, [sp, #0x20]
007ef29c: bl       #0x30ed6c
007ef2a0: ldr      r1, [r6, #0x6c]
007ef2a4: mov      fp, r0
007ef2a8: bl       #0x30ed6c
007ef2ac: ldr      r1, [r6, #0x68]
007ef2b0: mov      sb, r0
007ef2b4: mov      r0, fp
007ef2b8: bl       #0x30ed6c
007ef2bc: mov      r1, r0
007ef2c0: mov      r0, r7
007ef2c4: bl       #0x30eba4
007ef2c8: mov      r1, sb
007ef2cc: str      r0, [r5, #0x2c]
007ef2d0: ldr      r0, [r5, #0x30]
007ef2d4: bl       #0x30eba4
007ef2d8: str      r0, [r5, #0x30]
007ef2dc: ldr      r0, [sp, #0x10]
007ef2e0: mov      r1, sl
007ef2e4: bl       #0x30ed6c
007ef2e8: ldr      r1, [r6, #0x70]
007ef2ec: bl       #0x30ed6c
007ef2f0: mov      r1, r0
007ef2f4: ldr      r0, [r5, #0x38]
007ef2f8: bl       #0x30eba4
007ef2fc: str      r0, [r5, #0x38]
007ef300: ldr      r0, [sp, #0x1c]
007ef304: mov      r1, sl
007ef308: bl       #0x30ed6c
007ef30c: ldr      r1, [r6, #0x78]
007ef310: mov      sb, r0
007ef314: bl       #0x30ed6c
007ef318: ldr      r1, [r6, #0x74]
007ef31c: mov      r7, r0
007ef320: mov      r0, sb
007ef324: bl       #0x30ed6c
007ef328: mov      r1, r0
007ef32c: ldr      r0, [r4, #0x2c]
007ef330: bl       #0x30eba4
007ef334: mov      r1, r7
007ef338: str      r0, [r4, #0x2c]
007ef33c: ldr      r0, [r4, #0x30]
007ef340: bl       #0x30eba4
007ef344: str      r0, [r4, #0x30]
007ef348: mov      r1, sl
007ef34c: ldr      r0, [sp, #0xc]
007ef350: bl       #0x30ed6c
007ef354: ldr      r1, [r6, #0x7c]
007ef358: bl       #0x30ed6c
007ef35c: ldr      r1, [r4, #0x38]
007ef360: bl       #0x30eba4
007ef364: ldr      r3, [sp]
007ef368: str      r0, [r4, #0x38]
007ef36c: ldr      sl, [r5, #0x38]
007ef370: cmp      r3, #0
007ef374: addeq    r8, r8, #0x80000000
007ef378: mov      r1, sl
007ef37c: bl       #0x30e3ac
007ef380: ldr      r1, [r6, #0x64]
007ef384: bl       #0x30e3ac
007ef388: movw     r1, #0xfa36
007ef38c: movt     r1, #0x3e0e
007ef390: mov      r7, r0
007ef394: bl       #0x30e70c
007ef398: cmp      r0, #0
007ef39c: movweq   r7, #0xfa36
007ef3a0: movteq   r7, #0x3e0e
007ef3a4: beq      #0x7ef48c
007ef3a8: movw     r1, #0xfa36
007ef3ac: mov      r0, r7
007ef3b0: movt     r1, #0xbe0e
007ef3b4: bl       #0x30e70c
007ef3b8: cmp      r0, #0
007ef3bc: movwne   r7, #0xfa36
007ef3c0: movne    sb, #0
007ef3c4: movtne   r7, #0xbe0e
007ef3c8: beq      #0x7ef48c
007ef3cc: ldr      r0, [r6, #0x88]
007ef3d0: mov      r1, r7
007ef3d4: add      r0, r0, #0x80000000
007ef3d8: bl       #0x30ed6c
007ef3dc: ldr      r1, [r5, #0x80]
007ef3e0: mov      fp, r0
007ef3e4: bl       #0x30ed6c
007ef3e8: mov      r1, r0
007ef3ec: mov      r0, sl
007ef3f0: bl       #0x30e3ac
007ef3f4: str      r0, [r5, #0x38]
007ef3f8: ldr      r1, [r4, #0x80]
007ef3fc: mov      r0, fp
007ef400: bl       #0x30ed6c
007ef404: mov      r1, r0
007ef408: ldr      r0, [r4, #0x38]
007ef40c: bl       #0x30eba4
007ef410: str      r0, [r4, #0x38]
007ef414: mov      r0, r5
007ef418: bl       #0x7e761c
007ef41c: mov      r0, r4
007ef420: bl       #0x7e761c
007ef424: ldrb     r3, [r6, #0xc8]
007ef428: cmp      sb, #0
007ef42c: addeq    r1, r7, #0x80000000
007ef430: movne    fp, r7
007ef434: moveq    fp, r1
007ef438: cmp      r3, #0
007ef43c: beq      #0x7ef44c
007ef440: ldr      sl, [r6, #0xcc]
007ef444: cmp      sl, #0
007ef448: bne      #0x7ef4cc
007ef44c: movw     r1, #0xd70a
007ef450: mov      r0, r8
007ef454: movt     r1, #0x3ba3
007ef458: bl       #0x30e9ac
007ef45c: cmp      r0, #0
007ef460: beq      #0x7ef484
007ef464: movw     r1, #0xfa36
007ef468: mov      r0, fp
007ef46c: movt     r1, #0x3d0e
007ef470: bl       #0x30e9ac
007ef474: cmp      r0, #0
007ef478: mov      r0, #0
007ef47c: movne    r0, #1
007ef480: uxtb     r0, r0
007ef484: add      sp, sp, #0x2c
007ef488: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ef48c: mov      r0, r7
007ef490: mov      r1, #0
007ef494: bl       #0x30e2f8
007ef498: cmp      r0, #0
007ef49c: mov      sb, #0
007ef4a0: movne    sb, #1
007ef4a4: uxtb     sb, sb
007ef4a8: b        #0x7ef3cc
007ef4ac: mov      r0, r8
007ef4b0: mov      r1, #0
007ef4b4: bl       #0x30e2f8
007ef4b8: cmp      r0, #0
007ef4bc: mov      r3, #0
007ef4c0: movne    r3, #1
007ef4c4: uxtb     r3, r3
007ef4c8: b        #0x7ef27c
007ef4cc: ldr      r1, [r5, #0x1c]
007ef4d0: ldr      r0, [r6, #0x44]
007ef4d4: bl       #0x30e3ac
007ef4d8: str      r0, [sp, #0x18]
007ef4dc: ldr      r1, [r5, #0x20]
007ef4e0: ldr      r0, [r6, #0x48]
007ef4e4: bl       #0x30e3ac
007ef4e8: str      r0, [sp, #0x14]
007ef4ec: ldr      r1, [r5, #0xc]
007ef4f0: ldr      r0, [sp, #0x18]
007ef4f4: bl       #0x30ed6c
007ef4f8: ldr      r1, [r5, #0x14]
007ef4fc: mov      r3, r0
007ef500: ldr      r0, [sp, #0x14]
007ef504: str      r3, [sp]
007ef508: bl       #0x30ed6c
007ef50c: ldr      r3, [sp]
007ef510: mov      r1, r0
007ef514: mov      r0, r3
007ef518: bl       #0x30eba4
007ef51c: ldr      r1, [r5, #0x10]
007ef520: mov      ip, r0
007ef524: ldr      r0, [sp, #0x18]
007ef528: str      ip, [sp, #4]
007ef52c: bl       #0x30ed6c
007ef530: ldr      r1, [r5, #0x18]
007ef534: mov      r3, r0
007ef538: ldr      r0, [sp, #0x14]
007ef53c: str      r3, [sp]
007ef540: bl       #0x30ed6c
007ef544: ldr      r3, [sp]
007ef548: mov      r1, r0
007ef54c: mov      r0, r3
007ef550: bl       #0x30eba4
007ef554: str      r0, [sp, #0x24]
007ef558: ldr      r1, [r4, #0x1c]
007ef55c: ldr      r0, [r6, #0x4c]
007ef560: bl       #0x30e3ac
007ef564: str      r0, [sp, #0x18]
007ef568: ldr      r1, [r4, #0x20]
007ef56c: ldr      r0, [r6, #0x50]
007ef570: bl       #0x30e3ac
007ef574: str      r0, [sp, #0x14]
007ef578: ldr      r1, [r4, #0xc]
007ef57c: ldr      r0, [sp, #0x18]
007ef580: bl       #0x30ed6c
007ef584: ldr      r1, [r4, #0x14]
007ef588: mov      r3, r0
007ef58c: ldr      r0, [sp, #0x14]
007ef590: str      r3, [sp]
007ef594: bl       #0x30ed6c
007ef598: ldr      r3, [sp]
007ef59c: mov      r1, r0
007ef5a0: mov      r0, r3
007ef5a4: bl       #0x30eba4
007ef5a8: ldr      r1, [r4, #0x10]
007ef5ac: mov      r2, r0
007ef5b0: ldr      r0, [sp, #0x18]
007ef5b4: str      r2, [sp, #8]
007ef5b8: bl       #0x30ed6c
007ef5bc: ldr      r1, [r4, #0x18]
007ef5c0: mov      r3, r0
007ef5c4: ldr      r0, [sp, #0x14]
007ef5c8: str      r3, [sp]
007ef5cc: bl       #0x30ed6c
007ef5d0: ldr      r3, [sp]
007ef5d4: mov      r1, r0
007ef5d8: mov      r0, r3
007ef5dc: bl       #0x30eba4
007ef5e0: ldr      ip, [sp, #4]
007ef5e4: ldr      r1, [r5, #0x2c]
007ef5e8: mov      r3, r0
007ef5ec: mov      r0, ip
007ef5f0: str      r3, [sp]
007ef5f4: str      r1, [sp, #0x18]
007ef5f8: bl       #0x30eba4
007ef5fc: ldr      r1, [r5, #0x30]
007ef600: mov      ip, r0
007ef604: ldr      r0, [sp, #0x24]
007ef608: str      ip, [sp, #4]
007ef60c: bl       #0x30eba4
007ef610: ldr      r2, [sp, #8]
007ef614: str      r0, [sp, #0x14]
007ef618: ldr      r1, [r4, #0x2c]
007ef61c: mov      r0, r2
007ef620: bl       #0x30eba4
007ef624: ldr      r3, [sp]
007ef628: ldr      r1, [r4, #0x30]
007ef62c: mov      r2, r0
007ef630: mov      r0, r3
007ef634: str      r2, [sp, #8]
007ef638: bl       #0x30eba4
007ef63c: ldr      r2, [sp, #8]
007ef640: ldr      ip, [sp, #4]
007ef644: mov      r3, r0
007ef648: mov      r0, r2
007ef64c: mov      r1, ip
007ef650: str      r3, [sp]
007ef654: bl       #0x30e3ac
007ef658: ldr      r3, [sp]
007ef65c: mov      ip, r0
007ef660: ldr      r1, [sp, #0x14]
007ef664: mov      r0, r3
007ef668: str      ip, [sp, #4]
007ef66c: bl       #0x30e3ac
007ef670: str      r0, [sp, #0x14]
007ef674: ldr      r2, [r6, #0x54]
007ef678: ldr      r0, [r5, #0xc]
007ef67c: mov      r1, r2
007ef680: str      r2, [sp, #8]
007ef684: bl       #0x30ed6c
007ef688: ldr      r1, [r6, #0x58]
007ef68c: mov      r3, r0
007ef690: ldr      r0, [r5, #0x14]
007ef694: str      r3, [sp]
007ef698: bl       #0x30ed6c
007ef69c: ldr      r3, [sp]
007ef6a0: mov      r1, r0
007ef6a4: mov      r0, r3
007ef6a8: bl       #0x30eba4
007ef6ac: ldr      ip, [sp, #4]
007ef6b0: mov      r1, r0
007ef6b4: mov      r0, ip
007ef6b8: bl       #0x30ed6c
007ef6bc: ldr      r2, [sp, #8]
007ef6c0: mov      ip, r0
007ef6c4: ldr      r0, [r5, #0x10]
007ef6c8: mov      r1, r2
007ef6cc: str      ip, [sp, #4]
007ef6d0: bl       #0x30ed6c
007ef6d4: ldr      r1, [r6, #0x58]
007ef6d8: mov      r3, r0
007ef6dc: ldr      r0, [r5, #0x18]
007ef6e0: str      r3, [sp]
007ef6e4: bl       #0x30ed6c
007ef6e8: ldr      r3, [sp]
007ef6ec: mov      r1, r0
007ef6f0: mov      r0, r3
007ef6f4: bl       #0x30eba4
007ef6f8: mov      r1, r0
007ef6fc: ldr      r0, [sp, #0x14]
007ef700: bl       #0x30ed6c
007ef704: ldr      ip, [sp, #4]
007ef708: mov      r1, r0
007ef70c: mov      r0, ip
007ef710: bl       #0x30eba4
007ef714: cmp      sl, #3
007ef718: beq      #0x7ef818
007ef71c: cmp      sl, #1
007ef720: beq      #0x7ef894
007ef724: cmp      sl, #2
007ef728: movne    sl, #0
007ef72c: beq      #0x7ef958
007ef730: ldr      r0, [sp, #0x20]
007ef734: mov      r1, sl
007ef738: bl       #0x30ed6c
007ef73c: ldr      r1, [r6, #0x94]
007ef740: mov      r7, r0
007ef744: bl       #0x30ed6c
007ef748: ldr      r1, [r6, #0x90]
007ef74c: mov      sb, r0
007ef750: mov      r0, r7
007ef754: bl       #0x30ed6c
007ef758: ldr      r1, [sp, #0x18]
007ef75c: bl       #0x30eba4
007ef760: mov      r1, sb
007ef764: str      r0, [r5, #0x2c]
007ef768: ldr      r0, [r5, #0x30]
007ef76c: bl       #0x30eba4
007ef770: str      r0, [r5, #0x30]
007ef774: ldr      r0, [sp, #0x10]
007ef778: mov      r1, sl
007ef77c: bl       #0x30ed6c
007ef780: ldr      r1, [r6, #0x98]
007ef784: bl       #0x30ed6c
007ef788: mov      r1, r0
007ef78c: ldr      r0, [r5, #0x38]
007ef790: bl       #0x30eba4
007ef794: str      r0, [r5, #0x38]
007ef798: ldr      r0, [sp, #0x1c]
007ef79c: mov      r1, sl
007ef7a0: bl       #0x30ed6c
007ef7a4: ldr      r1, [r6, #0xa0]
007ef7a8: mov      r7, r0
007ef7ac: bl       #0x30ed6c
007ef7b0: ldr      r1, [r6, #0x9c]
007ef7b4: mov      sb, r0
007ef7b8: mov      r0, r7
007ef7bc: bl       #0x30ed6c
007ef7c0: mov      r1, r0
007ef7c4: ldr      r0, [r4, #0x2c]
007ef7c8: bl       #0x30eba4
007ef7cc: mov      r1, sb
007ef7d0: str      r0, [r4, #0x2c]
007ef7d4: ldr      r0, [r4, #0x30]
007ef7d8: bl       #0x30eba4
007ef7dc: str      r0, [r4, #0x30]
007ef7e0: ldr      r0, [sp, #0xc]
007ef7e4: mov      r1, sl
007ef7e8: bl       #0x30ed6c
007ef7ec: ldr      r1, [r6, #0xa4]
007ef7f0: bl       #0x30ed6c
007ef7f4: mov      r1, r0
007ef7f8: ldr      r0, [r4, #0x38]
007ef7fc: bl       #0x30eba4
007ef800: str      r0, [r4, #0x38]
007ef804: mov      r0, r5
007ef808: bl       #0x7e761c
007ef80c: mov      r0, r4
007ef810: bl       #0x7e761c
007ef814: b        #0x7ef44c
007ef818: movw     r1, #0xcccd
007ef81c: movt     r1, #0x3e4c
007ef820: str      r0, [sp]
007ef824: bl       #0x30e70c
007ef828: ldr      r3, [sp]
007ef82c: cmp      r0, #0
007ef830: movweq   r3, #0xcccd
007ef834: movteq   r3, #0x3e4c
007ef838: beq      #0x7ef930
007ef83c: movw     r1, #0xcccd
007ef840: movt     r1, #0xbe4c
007ef844: mov      r0, r3
007ef848: str      r3, [sp]
007ef84c: bl       #0x30e70c
007ef850: cmp      r0, #0
007ef854: movwne   r1, #0xcccd
007ef858: ldr      r3, [sp]
007ef85c: movtne   r1, #0xbe4c
007ef860: beq      #0x7ef930
007ef864: ldr      r0, [r6, #0xa8]
007ef868: add      r0, r0, #0x80000000
007ef86c: bl       #0x30ed6c
007ef870: cmp      sb, #0
007ef874: addeq    r7, r7, #0x80000000
007ef878: mov      sl, r0
007ef87c: mov      r1, r7
007ef880: mov      r0, r8
007ef884: bl       #0x30e2f8
007ef888: cmp      r0, #0
007ef88c: moveq    r8, r7
007ef890: b        #0x7ef730
007ef894: ldr      r1, [r6, #0xb8]
007ef898: bl       #0x30e3ac
007ef89c: add      r7, r0, #0x80000000
007ef8a0: mov      r1, r8
007ef8a4: mov      sl, r0
007ef8a8: mov      r0, r7
007ef8ac: bl       #0x30e70c
007ef8b0: movw     r1, #0xd70a
007ef8b4: cmp      r0, #0
007ef8b8: movt     r1, #0x3ba3
007ef8bc: mov      r0, sl
007ef8c0: moveq    r8, r7
007ef8c4: bl       #0x30eba4
007ef8c8: mov      r1, #0
007ef8cc: mov      r7, r0
007ef8d0: bl       #0x30e70c
007ef8d4: cmp      r0, #0
007ef8d8: moveq    r7, #0
007ef8dc: bne      #0x7ef938
007ef8e0: ldr      r0, [r6, #0xa8]
007ef8e4: ldr      sl, [r6, #0xb4]
007ef8e8: mov      r1, r7
007ef8ec: add      r0, r0, #0x80000000
007ef8f0: bl       #0x30ed6c
007ef8f4: mov      r1, sl
007ef8f8: bl       #0x30eba4
007ef8fc: mov      r1, #0
007ef900: mov      r7, r0
007ef904: bl       #0x30e2f8
007ef908: cmp      r0, #0
007ef90c: moveq    r7, #0
007ef910: mov      r1, sl
007ef914: str      r7, [r6, #0xb4]
007ef918: mov      r0, r7
007ef91c: bl       #0x30e3ac
007ef920: ldr      r3, [r5, #0x2c]
007ef924: mov      sl, r0
007ef928: str      r3, [sp, #0x18]
007ef92c: b        #0x7ef730
007ef930: mov      r1, r3
007ef934: b        #0x7ef864
007ef938: movw     r1, #0xcccd
007ef93c: mov      r0, r7
007ef940: movt     r1, #0xbe4c
007ef944: bl       #0x30e70c
007ef948: cmp      r0, #0
007ef94c: movwne   r7, #0xcccd
007ef950: movtne   r7, #0xbe4c
007ef954: b        #0x7ef8e0
007ef958: ldr      r1, [r6, #0xbc]
007ef95c: bl       #0x30e3ac
007ef960: mov      r1, r8
007ef964: mov      r7, r0
007ef968: bl       #0x30e70c
007ef96c: movw     r1, #0xd70a
007ef970: cmp      r0, #0
007ef974: movt     r1, #0x3ba3
007ef978: mov      r0, r7
007ef97c: moveq    r8, r7
007ef980: bl       #0x30e3ac
007ef984: movw     r1, #0xcccd
007ef988: movt     r1, #0x3e4c
007ef98c: mov      r7, r0
007ef990: bl       #0x30e70c
007ef994: cmp      r0, #0
007ef998: movweq   r7, #0xcccd
007ef99c: movteq   r7, #0x3e4c
007ef9a0: beq      #0x7ef9b8
007ef9a4: mov      r0, r7
007ef9a8: mov      r1, #0
007ef9ac: bl       #0x30e70c
007ef9b0: cmp      r0, #0
007ef9b4: movne    r7, #0
007ef9b8: ldr      r0, [r6, #0xa8]
007ef9bc: ldr      sl, [r6, #0xb4]
007ef9c0: mov      r1, r7
007ef9c4: add      r0, r0, #0x80000000
007ef9c8: bl       #0x30ed6c
007ef9cc: mov      r1, sl
007ef9d0: bl       #0x30eba4
007ef9d4: mov      r1, #0
007ef9d8: mov      r7, r0
007ef9dc: bl       #0x30e70c
007ef9e0: cmp      r0, #0
007ef9e4: moveq    r7, #0
007ef9e8: mov      r1, sl
007ef9ec: str      r7, [r6, #0xb4]
007ef9f0: mov      r0, r7
007ef9f4: bl       #0x30e3ac
007ef9f8: ldr      r1, [r5, #0x2c]
007ef9fc: mov      sl, r0
007efa00: str      r1, [sp, #0x18]
007efa04: b        #0x7ef730

# _ZN11b2DebugDrawC1Ev
007e8cc8: ldr      r3, [pc, #0x1c]
007e8ccc: ldr      r2, [pc, #0x1c]
007e8cd0: mov      ip, #0
007e8cd4: add      r3, pc, r3
007e8cd8: ldr      r2, [r3, r2]
007e8cdc: str      ip, [r0, #4]
007e8ce0: add      r2, r2, #8
007e8ce4: str      r2, [r0]
007e8ce8: bx       lr
007e8cec: ldrheq   fp, [sl], -ip
007e8cf0: strdeq   r1, r2, [r0], -r8

# _ZN7b2World22SetDestructionListenerEP21b2DestructionListener
007e65d4: mov      r3, #0x19000
007e65d8: add      r3, r3, #0x258
007e65dc: str      r1, [r0, r3]
007e65e0: bx       lr

# _ZN14b2PolygonShapeD1Ev
007e5444: ldr      r3, [pc, #0x24]
007e5448: ldr      r2, [pc, #0x24]
007e544c: push     {r4, lr}
007e5450: add      r3, pc, r3
007e5454: ldr      r2, [r3, r2]
007e5458: mov      r4, r0
007e545c: add      r2, r2, #8
007e5460: str      r2, [r0]
007e5464: bl       #0x7e6178
007e5468: mov      r0, r4
007e546c: pop      {r4, pc}
007e5470: andseq   pc, sl, r0, asr #12
007e5474: andeq    r1, r0, r8, lsl #17

# _ZN6b2Vec29NormalizeEv
007e5524: push     {r4, r5, r6, lr}
007e5528: mov      r4, r0
007e552c: ldr      r0, [r0]
007e5530: ldr      r6, [r4, #4]
007e5534: mov      r1, r0
007e5538: bl       #0x30ed6c
007e553c: mov      r1, r6
007e5540: mov      r5, r0
007e5544: mov      r0, r6
007e5548: bl       #0x30ed6c
007e554c: mov      r1, r0
007e5550: mov      r0, r5
007e5554: bl       #0x30eba4
007e5558: bl       #0x30e124
007e555c: mov      r1, #0x34000000
007e5560: mov      r5, r0
007e5564: bl       #0x30e70c
007e5568: cmp      r0, #0
007e556c: movne    r5, #0
007e5570: bne      #0x7e55a4
007e5574: mov      r1, r5
007e5578: mov      r0, #0x3f800000
007e557c: bl       #0x30ec94
007e5580: mov      r1, r0
007e5584: mov      r6, r0
007e5588: ldr      r0, [r4]
007e558c: bl       #0x30ed6c
007e5590: mov      r1, r6
007e5594: str      r0, [r4]
007e5598: ldr      r0, [r4, #4]
007e559c: bl       #0x30ed6c
007e55a0: str      r0, [r4, #4]
007e55a4: mov      r0, r5
007e55a8: pop      {r4, r5, r6, pc}

# _ZN13b2PulleyJoint24SolvePositionConstraintsEv
007f129c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f12a0: ldr      r7, [r0, #0x44]
007f12a4: sub      sp, sp, #0x2c
007f12a8: mov      r4, r0
007f12ac: ldr      sb, [r7, #4]
007f12b0: ldr      r1, [r0, #0x48]
007f12b4: ldr      r6, [r0, #0x30]
007f12b8: ldr      r5, [r0, #0x34]
007f12bc: mov      r0, sb
007f12c0: bl       #0x30eba4
007f12c4: ldr      r7, [r7, #8]
007f12c8: ldr      r1, [r4, #0x4c]
007f12cc: mov      sl, r0
007f12d0: mov      r0, r7
007f12d4: bl       #0x30eba4
007f12d8: ldr      r1, [r4, #0x50]
007f12dc: mov      r8, r0
007f12e0: mov      r0, sb
007f12e4: bl       #0x30eba4
007f12e8: str      r0, [sp, #0xc]
007f12ec: ldr      r1, [r4, #0x54]
007f12f0: mov      r0, r7
007f12f4: bl       #0x30eba4
007f12f8: str      r0, [sp, #0x10]
007f12fc: ldr      r3, [r4, #0xac]
007f1300: cmp      r3, #2
007f1304: movne    r7, #0
007f1308: beq      #0x7f1854
007f130c: ldr      r3, [r4, #0xb0]
007f1310: cmp      r3, #2
007f1314: beq      #0x7f15c8
007f1318: ldr      r3, [r4, #0xb4]
007f131c: cmp      r3, #2
007f1320: beq      #0x7f134c
007f1324: movw     r1, #0xd70a
007f1328: mov      r0, r7
007f132c: movt     r1, #0x3ba3
007f1330: bl       #0x30e70c
007f1334: cmp      r0, #0
007f1338: mov      r0, #0
007f133c: movne    r0, #1
007f1340: and      r0, r0, #1
007f1344: add      sp, sp, #0x2c
007f1348: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f134c: ldr      r1, [r5, #0x1c]
007f1350: ldr      r0, [r4, #0x60]
007f1354: bl       #0x30e3ac
007f1358: ldr      r1, [r5, #0x20]
007f135c: mov      sl, r0
007f1360: ldr      r0, [r4, #0x64]
007f1364: bl       #0x30e3ac
007f1368: ldr      r1, [r5, #0xc]
007f136c: mov      r6, r0
007f1370: mov      r0, sl
007f1374: bl       #0x30ed6c
007f1378: ldr      r1, [r5, #0x14]
007f137c: mov      r8, r0
007f1380: mov      r0, r6
007f1384: bl       #0x30ed6c
007f1388: mov      r1, r0
007f138c: mov      r0, r8
007f1390: bl       #0x30eba4
007f1394: ldr      r1, [r5, #0x10]
007f1398: mov      r8, r0
007f139c: mov      r0, sl
007f13a0: bl       #0x30ed6c
007f13a4: ldr      r1, [r5, #0x18]
007f13a8: mov      sl, r0
007f13ac: mov      r0, r6
007f13b0: bl       #0x30ed6c
007f13b4: mov      r1, r0
007f13b8: mov      r0, sl
007f13bc: bl       #0x30eba4
007f13c0: ldr      r1, [r5, #0x2c]
007f13c4: mov      r6, r0
007f13c8: mov      r0, r8
007f13cc: bl       #0x30eba4
007f13d0: ldr      r1, [r5, #0x30]
007f13d4: mov      sb, r0
007f13d8: mov      r0, r6
007f13dc: bl       #0x30eba4
007f13e0: ldr      r1, [sp, #0xc]
007f13e4: mov      sl, r0
007f13e8: mov      r0, sb
007f13ec: bl       #0x30e3ac
007f13f0: ldr      r1, [sp, #0x10]
007f13f4: mov      sb, r0
007f13f8: mov      r0, sl
007f13fc: bl       #0x30e3ac
007f1400: mov      sl, r0
007f1404: mov      r1, sb
007f1408: mov      r0, sb
007f140c: str      sl, [r4, #0x74]
007f1410: str      sb, [r4, #0x70]
007f1414: bl       #0x30ed6c
007f1418: mov      r1, sl
007f141c: mov      sb, r0
007f1420: mov      r0, sl
007f1424: bl       #0x30ed6c
007f1428: mov      r1, r0
007f142c: mov      r0, sb
007f1430: bl       #0x30eba4
007f1434: bl       #0x30e124
007f1438: movw     r1, #0xd70a
007f143c: movt     r1, #0x3ba3
007f1440: mov      sl, r0
007f1444: bl       #0x30e2f8
007f1448: cmp      r0, #0
007f144c: moveq    r3, #0
007f1450: streq    r3, [r4, #0x74]
007f1454: streq    r3, [r4, #0x70]
007f1458: beq      #0x7f148c
007f145c: mov      r1, sl
007f1460: mov      r0, #0x3f800000
007f1464: bl       #0x30ec94
007f1468: mov      r1, r0
007f146c: mov      sb, r0
007f1470: ldr      r0, [r4, #0x70]
007f1474: bl       #0x30ed6c
007f1478: mov      r1, sb
007f147c: str      r0, [r4, #0x70]
007f1480: ldr      r0, [r4, #0x74]
007f1484: bl       #0x30ed6c
007f1488: str      r0, [r4, #0x74]
007f148c: mov      r1, sl
007f1490: ldr      r0, [r4, #0x84]
007f1494: bl       #0x30e3ac
007f1498: add      sb, r0, #0x80000000
007f149c: mov      sl, r0
007f14a0: mov      r1, sb
007f14a4: mov      r0, r7
007f14a8: bl       #0x30e2f8
007f14ac: movw     r1, #0xd70a
007f14b0: cmp      r0, #0
007f14b4: movt     r1, #0x3ba3
007f14b8: mov      r0, sl
007f14bc: moveq    r7, sb
007f14c0: bl       #0x30eba4
007f14c4: mov      r1, #0
007f14c8: mov      sl, r0
007f14cc: bl       #0x30e70c
007f14d0: cmp      r0, #0
007f14d4: moveq    sl, #0
007f14d8: bne      #0x7f1cfc
007f14dc: ldr      r0, [r4, #0x90]
007f14e0: ldr      sb, [r4, #0xa8]
007f14e4: mov      r1, sl
007f14e8: add      r0, r0, #0x80000000
007f14ec: bl       #0x30ed6c
007f14f0: mov      r1, sb
007f14f4: bl       #0x30eba4
007f14f8: mov      r1, #0
007f14fc: mov      sl, r0
007f1500: bl       #0x30e70c
007f1504: cmp      r0, #0
007f1508: movne    sl, #0
007f150c: mov      r1, sb
007f1510: str      sl, [r4, #0xa8]
007f1514: mov      r0, sl
007f1518: bl       #0x30e3ac
007f151c: add      sb, r0, #0x80000000
007f1520: ldr      r1, [r4, #0x70]
007f1524: mov      r0, sb
007f1528: bl       #0x30ed6c
007f152c: ldr      r1, [r4, #0x74]
007f1530: mov      sl, r0
007f1534: mov      r0, sb
007f1538: bl       #0x30ed6c
007f153c: ldr      sb, [r5, #0x78]
007f1540: mov      r4, r0
007f1544: mov      r1, sl
007f1548: mov      r0, sb
007f154c: bl       #0x30ed6c
007f1550: mov      r1, r0
007f1554: ldr      r0, [r5, #0x2c]
007f1558: bl       #0x30eba4
007f155c: mov      r1, r4
007f1560: str      r0, [r5, #0x2c]
007f1564: mov      r0, sb
007f1568: bl       #0x30ed6c
007f156c: mov      r1, r0
007f1570: ldr      r0, [r5, #0x30]
007f1574: bl       #0x30eba4
007f1578: mov      r1, r4
007f157c: str      r0, [r5, #0x30]
007f1580: mov      r0, r8
007f1584: bl       #0x30ed6c
007f1588: mov      r1, sl
007f158c: mov      r4, r0
007f1590: mov      r0, r6
007f1594: bl       #0x30ed6c
007f1598: mov      r1, r0
007f159c: mov      r0, r4
007f15a0: bl       #0x30e3ac
007f15a4: ldr      r1, [r5, #0x80]
007f15a8: bl       #0x30ed6c
007f15ac: mov      r1, r0
007f15b0: ldr      r0, [r5, #0x38]
007f15b4: bl       #0x30eba4
007f15b8: str      r0, [r5, #0x38]
007f15bc: mov      r0, r5
007f15c0: bl       #0x7e761c
007f15c4: b        #0x7f1324
007f15c8: ldr      r1, [r6, #0x1c]
007f15cc: ldr      r0, [r4, #0x58]
007f15d0: bl       #0x30e3ac
007f15d4: ldr      r1, [r6, #0x20]
007f15d8: mov      fp, r0
007f15dc: ldr      r0, [r4, #0x5c]
007f15e0: bl       #0x30e3ac
007f15e4: ldr      r1, [r6, #0xc]
007f15e8: mov      sb, r0
007f15ec: mov      r0, fp
007f15f0: bl       #0x30ed6c
007f15f4: ldr      r1, [r6, #0x14]
007f15f8: mov      r3, r0
007f15fc: mov      r0, sb
007f1600: str      r3, [sp, #4]
007f1604: bl       #0x30ed6c
007f1608: ldr      r3, [sp, #4]
007f160c: mov      r1, r0
007f1610: mov      r0, r3
007f1614: bl       #0x30eba4
007f1618: str      r0, [sp, #8]
007f161c: ldr      r1, [r6, #0x10]
007f1620: mov      r0, fp
007f1624: bl       #0x30ed6c
007f1628: ldr      r1, [r6, #0x18]
007f162c: mov      fp, r0
007f1630: mov      r0, sb
007f1634: bl       #0x30ed6c
007f1638: mov      r1, r0
007f163c: mov      r0, fp
007f1640: bl       #0x30eba4
007f1644: ldr      r1, [r6, #0x2c]
007f1648: mov      sb, r0
007f164c: ldr      r0, [sp, #8]
007f1650: bl       #0x30eba4
007f1654: ldr      r1, [r6, #0x30]
007f1658: mov      fp, r0
007f165c: mov      r0, sb
007f1660: bl       #0x30eba4
007f1664: mov      r1, sl
007f1668: mov      r3, r0
007f166c: mov      r0, fp
007f1670: str      r3, [sp, #4]
007f1674: bl       #0x30e3ac
007f1678: ldr      r3, [sp, #4]
007f167c: mov      sl, r0
007f1680: mov      r1, r8
007f1684: mov      r0, r3
007f1688: bl       #0x30e3ac
007f168c: mov      r8, r0
007f1690: mov      r1, sl
007f1694: mov      r0, sl
007f1698: str      r8, [r4, #0x6c]
007f169c: str      sl, [r4, #0x68]
007f16a0: bl       #0x30ed6c
007f16a4: mov      r1, r8
007f16a8: mov      sl, r0
007f16ac: mov      r0, r8
007f16b0: bl       #0x30ed6c
007f16b4: mov      r1, r0
007f16b8: mov      r0, sl
007f16bc: bl       #0x30eba4
007f16c0: bl       #0x30e124
007f16c4: movw     r1, #0xd70a
007f16c8: movt     r1, #0x3ba3
007f16cc: mov      r8, r0
007f16d0: bl       #0x30e2f8
007f16d4: cmp      r0, #0
007f16d8: moveq    r3, #0
007f16dc: streq    r3, [r4, #0x6c]
007f16e0: streq    r3, [r4, #0x68]
007f16e4: beq      #0x7f1718
007f16e8: mov      r1, r8
007f16ec: mov      r0, #0x3f800000
007f16f0: bl       #0x30ec94
007f16f4: mov      r1, r0
007f16f8: mov      sl, r0
007f16fc: ldr      r0, [r4, #0x68]
007f1700: bl       #0x30ed6c
007f1704: mov      r1, sl
007f1708: str      r0, [r4, #0x68]
007f170c: ldr      r0, [r4, #0x6c]
007f1710: bl       #0x30ed6c
007f1714: str      r0, [r4, #0x6c]
007f1718: mov      r1, r8
007f171c: ldr      r0, [r4, #0x80]
007f1720: bl       #0x30e3ac
007f1724: add      sl, r0, #0x80000000
007f1728: mov      r8, r0
007f172c: mov      r1, sl
007f1730: mov      r0, r7
007f1734: bl       #0x30e2f8
007f1738: movw     r1, #0xd70a
007f173c: cmp      r0, #0
007f1740: movt     r1, #0x3ba3
007f1744: mov      r0, r8
007f1748: moveq    r7, sl
007f174c: bl       #0x30eba4
007f1750: mov      r1, #0
007f1754: mov      r8, r0
007f1758: bl       #0x30e70c
007f175c: cmp      r0, #0
007f1760: moveq    r8, #0
007f1764: bne      #0x7f1d3c
007f1768: ldr      r0, [r4, #0x8c]
007f176c: ldr      sl, [r4, #0xa4]
007f1770: mov      r1, r8
007f1774: add      r0, r0, #0x80000000
007f1778: bl       #0x30ed6c
007f177c: mov      r1, sl
007f1780: bl       #0x30eba4
007f1784: mov      r1, #0
007f1788: mov      r8, r0
007f178c: bl       #0x30e70c
007f1790: cmp      r0, #0
007f1794: movne    r8, #0
007f1798: mov      r1, sl
007f179c: str      r8, [r4, #0xa4]
007f17a0: mov      r0, r8
007f17a4: bl       #0x30e3ac
007f17a8: add      sl, r0, #0x80000000
007f17ac: ldr      r1, [r4, #0x68]
007f17b0: mov      r0, sl
007f17b4: bl       #0x30ed6c
007f17b8: ldr      r1, [r4, #0x6c]
007f17bc: mov      r8, r0
007f17c0: mov      r0, sl
007f17c4: bl       #0x30ed6c
007f17c8: ldr      fp, [r6, #0x78]
007f17cc: mov      sl, r0
007f17d0: mov      r1, r8
007f17d4: mov      r0, fp
007f17d8: bl       #0x30ed6c
007f17dc: mov      r1, r0
007f17e0: ldr      r0, [r6, #0x2c]
007f17e4: bl       #0x30eba4
007f17e8: mov      r1, sl
007f17ec: str      r0, [r6, #0x2c]
007f17f0: mov      r0, fp
007f17f4: bl       #0x30ed6c
007f17f8: mov      r1, r0
007f17fc: ldr      r0, [r6, #0x30]
007f1800: bl       #0x30eba4
007f1804: str      r0, [r6, #0x30]
007f1808: mov      r1, sl
007f180c: ldr      r0, [sp, #8]
007f1810: bl       #0x30ed6c
007f1814: mov      r1, r8
007f1818: mov      sl, r0
007f181c: mov      r0, sb
007f1820: bl       #0x30ed6c
007f1824: mov      r1, r0
007f1828: mov      r0, sl
007f182c: bl       #0x30e3ac
007f1830: ldr      r1, [r6, #0x80]
007f1834: bl       #0x30ed6c
007f1838: mov      r1, r0
007f183c: ldr      r0, [r6, #0x38]
007f1840: bl       #0x30eba4
007f1844: str      r0, [r6, #0x38]
007f1848: mov      r0, r6
007f184c: bl       #0x7e761c
007f1850: b        #0x7f1318
007f1854: ldr      r1, [r6, #0x1c]
007f1858: ldr      r0, [r4, #0x58]
007f185c: bl       #0x30e3ac
007f1860: ldr      r1, [r6, #0x20]
007f1864: mov      sb, r0
007f1868: ldr      r0, [r4, #0x5c]
007f186c: bl       #0x30e3ac
007f1870: ldr      r1, [r6, #0xc]
007f1874: mov      r7, r0
007f1878: mov      r0, sb
007f187c: bl       #0x30ed6c
007f1880: ldr      r1, [r6, #0x14]
007f1884: mov      fp, r0
007f1888: mov      r0, r7
007f188c: bl       #0x30ed6c
007f1890: mov      r1, r0
007f1894: mov      r0, fp
007f1898: bl       #0x30eba4
007f189c: str      r0, [sp, #0x18]
007f18a0: ldr      r1, [r6, #0x10]
007f18a4: mov      r0, sb
007f18a8: bl       #0x30ed6c
007f18ac: ldr      r1, [r6, #0x18]
007f18b0: mov      sb, r0
007f18b4: mov      r0, r7
007f18b8: bl       #0x30ed6c
007f18bc: mov      r1, r0
007f18c0: mov      r0, sb
007f18c4: bl       #0x30eba4
007f18c8: str      r0, [sp, #0x1c]
007f18cc: ldr      r1, [r5, #0x1c]
007f18d0: ldr      r0, [r4, #0x60]
007f18d4: bl       #0x30e3ac
007f18d8: ldr      r1, [r5, #0x20]
007f18dc: mov      sb, r0
007f18e0: ldr      r0, [r4, #0x64]
007f18e4: bl       #0x30e3ac
007f18e8: ldr      r1, [r5, #0xc]
007f18ec: mov      r7, r0
007f18f0: mov      r0, sb
007f18f4: bl       #0x30ed6c
007f18f8: ldr      r1, [r5, #0x14]
007f18fc: mov      fp, r0
007f1900: mov      r0, r7
007f1904: bl       #0x30ed6c
007f1908: mov      r1, r0
007f190c: mov      r0, fp
007f1910: bl       #0x30eba4
007f1914: str      r0, [sp, #8]
007f1918: ldr      r1, [r5, #0x10]
007f191c: mov      r0, sb
007f1920: bl       #0x30ed6c
007f1924: ldr      r1, [r5, #0x18]
007f1928: mov      sb, r0
007f192c: mov      r0, r7
007f1930: bl       #0x30ed6c
007f1934: mov      r1, r0
007f1938: mov      r0, sb
007f193c: bl       #0x30eba4
007f1940: str      r0, [sp, #0x14]
007f1944: ldr      r1, [r6, #0x2c]
007f1948: ldr      r0, [sp, #0x18]
007f194c: bl       #0x30eba4
007f1950: ldr      r1, [r6, #0x30]
007f1954: mov      sb, r0
007f1958: ldr      r0, [sp, #0x1c]
007f195c: bl       #0x30eba4
007f1960: ldr      r1, [r5, #0x2c]
007f1964: mov      r7, r0
007f1968: ldr      r0, [sp, #8]
007f196c: bl       #0x30eba4
007f1970: ldr      r1, [r5, #0x30]
007f1974: mov      r3, r0
007f1978: ldr      r0, [sp, #0x14]
007f197c: str      r3, [sp, #4]
007f1980: bl       #0x30eba4
007f1984: mov      r1, sl
007f1988: mov      fp, r0
007f198c: mov      r0, sb
007f1990: bl       #0x30e3ac
007f1994: mov      r1, r8
007f1998: mov      sb, r0
007f199c: mov      r0, r7
007f19a0: bl       #0x30e3ac
007f19a4: str      sb, [r4, #0x68]
007f19a8: str      r0, [r4, #0x6c]
007f19ac: ldr      r1, [sp, #0x10]
007f19b0: mov      r7, r0
007f19b4: mov      r0, fp
007f19b8: bl       #0x30e3ac
007f19bc: str      r0, [r4, #0x74]
007f19c0: ldr      r3, [sp, #4]
007f19c4: ldr      r1, [sp, #0xc]
007f19c8: mov      r0, r3
007f19cc: bl       #0x30e3ac
007f19d0: mov      r1, sb
007f19d4: str      r0, [r4, #0x70]
007f19d8: mov      r0, sb
007f19dc: bl       #0x30ed6c
007f19e0: mov      r1, r7
007f19e4: mov      sb, r0
007f19e8: mov      r0, r7
007f19ec: bl       #0x30ed6c
007f19f0: mov      r1, r0
007f19f4: mov      r0, sb
007f19f8: bl       #0x30eba4
007f19fc: bl       #0x30e124
007f1a00: mov      sb, r0
007f1a04: ldr      r0, [r4, #0x70]
007f1a08: ldr      fp, [r4, #0x74]
007f1a0c: mov      r1, r0
007f1a10: bl       #0x30ed6c
007f1a14: mov      r1, fp
007f1a18: mov      r7, r0
007f1a1c: mov      r0, fp
007f1a20: bl       #0x30ed6c
007f1a24: mov      r1, r0
007f1a28: mov      r0, r7
007f1a2c: bl       #0x30eba4
007f1a30: bl       #0x30e124
007f1a34: movw     r1, #0xd70a
007f1a38: mov      r7, r0
007f1a3c: movt     r1, #0x3ba3
007f1a40: mov      r0, sb
007f1a44: bl       #0x30e2f8
007f1a48: cmp      r0, #0
007f1a4c: moveq    r3, #0
007f1a50: streq    r3, [r4, #0x6c]
007f1a54: streq    r3, [r4, #0x68]
007f1a58: beq      #0x7f1a8c
007f1a5c: mov      r1, sb
007f1a60: mov      r0, #0x3f800000
007f1a64: bl       #0x30ec94
007f1a68: mov      r1, r0
007f1a6c: mov      fp, r0
007f1a70: ldr      r0, [r4, #0x68]
007f1a74: bl       #0x30ed6c
007f1a78: mov      r1, fp
007f1a7c: str      r0, [r4, #0x68]
007f1a80: ldr      r0, [r4, #0x6c]
007f1a84: bl       #0x30ed6c
007f1a88: str      r0, [r4, #0x6c]
007f1a8c: movw     r1, #0xd70a
007f1a90: mov      r0, r7
007f1a94: movt     r1, #0x3ba3
007f1a98: bl       #0x30e2f8
007f1a9c: cmp      r0, #0
007f1aa0: moveq    r3, #0
007f1aa4: streq    r3, [r4, #0x74]
007f1aa8: streq    r3, [r4, #0x70]
007f1aac: beq      #0x7f1ae0
007f1ab0: mov      r1, r7
007f1ab4: mov      r0, #0x3f800000
007f1ab8: bl       #0x30ec94
007f1abc: mov      r1, r0
007f1ac0: mov      fp, r0
007f1ac4: ldr      r0, [r4, #0x70]
007f1ac8: bl       #0x30ed6c
007f1acc: mov      r1, fp
007f1ad0: str      r0, [r4, #0x70]
007f1ad4: ldr      r0, [r4, #0x74]
007f1ad8: bl       #0x30ed6c
007f1adc: str      r0, [r4, #0x74]
007f1ae0: mov      r1, sb
007f1ae4: ldr      r0, [r4, #0x78]
007f1ae8: bl       #0x30e3ac
007f1aec: ldr      r1, [r4, #0x7c]
007f1af0: mov      sb, r0
007f1af4: mov      r0, r7
007f1af8: bl       #0x30ed6c
007f1afc: mov      r1, r0
007f1b00: mov      r0, sb
007f1b04: bl       #0x30e3ac
007f1b08: add      r7, r0, #0x80000000
007f1b0c: mov      sb, r0
007f1b10: mov      r1, #0
007f1b14: mov      r0, r7
007f1b18: bl       #0x30e70c
007f1b1c: movw     r1, #0xd70a
007f1b20: cmp      r0, #0
007f1b24: movt     r1, #0x3ba3
007f1b28: mov      r0, sb
007f1b2c: movne    r7, #0
007f1b30: bl       #0x30eba4
007f1b34: mov      r1, #0
007f1b38: mov      sb, r0
007f1b3c: bl       #0x30e70c
007f1b40: cmp      r0, #0
007f1b44: moveq    sb, #0
007f1b48: bne      #0x7f1d1c
007f1b4c: ldr      r0, [r4, #0x88]
007f1b50: ldr      fp, [r4, #0xa0]
007f1b54: mov      r1, sb
007f1b58: add      r0, r0, #0x80000000
007f1b5c: bl       #0x30ed6c
007f1b60: mov      r1, fp
007f1b64: bl       #0x30eba4
007f1b68: mov      r1, #0
007f1b6c: mov      sb, r0
007f1b70: bl       #0x30e70c
007f1b74: cmp      r0, #0
007f1b78: movne    sb, #0
007f1b7c: mov      r1, fp
007f1b80: str      sb, [r4, #0xa0]
007f1b84: mov      r0, sb
007f1b88: bl       #0x30e3ac
007f1b8c: add      r3, r0, #0x80000000
007f1b90: ldr      r1, [r4, #0x68]
007f1b94: mov      sb, r0
007f1b98: mov      r0, r3
007f1b9c: mov      fp, r3
007f1ba0: bl       #0x30ed6c
007f1ba4: str      r0, [sp, #0x20]
007f1ba8: ldr      r1, [r4, #0x6c]
007f1bac: mov      r0, fp
007f1bb0: bl       #0x30ed6c
007f1bb4: mov      fp, r0
007f1bb8: ldr      r0, [r4, #0x7c]
007f1bbc: mov      r1, sb
007f1bc0: add      r0, r0, #0x80000000
007f1bc4: bl       #0x30ed6c
007f1bc8: ldr      r1, [r4, #0x70]
007f1bcc: mov      sb, r0
007f1bd0: bl       #0x30ed6c
007f1bd4: str      r0, [sp, #0x24]
007f1bd8: ldr      r1, [r4, #0x74]
007f1bdc: mov      r0, sb
007f1be0: bl       #0x30ed6c
007f1be4: ldr      r3, [r6, #0x78]
007f1be8: ldr      r1, [sp, #0x20]
007f1bec: mov      sb, r0
007f1bf0: mov      r0, r3
007f1bf4: str      r3, [sp, #4]
007f1bf8: bl       #0x30ed6c
007f1bfc: mov      r1, r0
007f1c00: ldr      r0, [r6, #0x2c]
007f1c04: bl       #0x30eba4
007f1c08: str      r0, [r6, #0x2c]
007f1c0c: ldr      r3, [sp, #4]
007f1c10: mov      r1, fp
007f1c14: mov      r0, r3
007f1c18: bl       #0x30ed6c
007f1c1c: mov      r1, r0
007f1c20: ldr      r0, [r6, #0x30]
007f1c24: bl       #0x30eba4
007f1c28: str      r0, [r6, #0x30]
007f1c2c: mov      r1, fp
007f1c30: ldr      r0, [sp, #0x18]
007f1c34: bl       #0x30ed6c
007f1c38: ldr      r1, [sp, #0x20]
007f1c3c: mov      fp, r0
007f1c40: ldr      r0, [sp, #0x1c]
007f1c44: bl       #0x30ed6c
007f1c48: mov      r1, r0
007f1c4c: mov      r0, fp
007f1c50: bl       #0x30e3ac
007f1c54: ldr      r1, [r6, #0x80]
007f1c58: bl       #0x30ed6c
007f1c5c: mov      r1, r0
007f1c60: ldr      r0, [r6, #0x38]
007f1c64: bl       #0x30eba4
007f1c68: str      r0, [r6, #0x38]
007f1c6c: ldr      fp, [r5, #0x78]
007f1c70: ldr      r1, [sp, #0x24]
007f1c74: mov      r0, fp
007f1c78: bl       #0x30ed6c
007f1c7c: mov      r1, r0
007f1c80: ldr      r0, [r5, #0x2c]
007f1c84: bl       #0x30eba4
007f1c88: mov      r1, sb
007f1c8c: str      r0, [r5, #0x2c]
007f1c90: mov      r0, fp
007f1c94: bl       #0x30ed6c
007f1c98: mov      r1, r0
007f1c9c: ldr      r0, [r5, #0x30]
007f1ca0: bl       #0x30eba4
007f1ca4: str      r0, [r5, #0x30]
007f1ca8: ldr      r0, [sp, #8]
007f1cac: mov      r1, sb
007f1cb0: bl       #0x30ed6c
007f1cb4: ldr      r1, [sp, #0x24]
007f1cb8: mov      sb, r0
007f1cbc: ldr      r0, [sp, #0x14]
007f1cc0: bl       #0x30ed6c
007f1cc4: mov      r1, r0
007f1cc8: mov      r0, sb
007f1ccc: bl       #0x30e3ac
007f1cd0: ldr      r1, [r5, #0x80]
007f1cd4: bl       #0x30ed6c
007f1cd8: mov      r1, r0
007f1cdc: ldr      r0, [r5, #0x38]
007f1ce0: bl       #0x30eba4
007f1ce4: str      r0, [r5, #0x38]
007f1ce8: mov      r0, r6
007f1cec: bl       #0x7e761c
007f1cf0: mov      r0, r5
007f1cf4: bl       #0x7e761c
007f1cf8: b        #0x7f130c
007f1cfc: movw     r1, #0xcccd
007f1d00: mov      r0, sl
007f1d04: movt     r1, #0xbe4c
007f1d08: bl       #0x30e70c
007f1d0c: cmp      r0, #0
007f1d10: movwne   sl, #0xcccd
007f1d14: movtne   sl, #0xbe4c
007f1d18: b        #0x7f14dc
007f1d1c: movw     r1, #0xcccd
007f1d20: mov      r0, sb
007f1d24: movt     r1, #0xbe4c
007f1d28: bl       #0x30e70c
007f1d2c: cmp      r0, #0
007f1d30: movwne   sb, #0xcccd
007f1d34: movtne   sb, #0xbe4c
007f1d38: b        #0x7f1b4c
007f1d3c: movw     r1, #0xcccd
007f1d40: mov      r0, r8
007f1d44: movt     r1, #0xbe4c
007f1d48: bl       #0x30e70c
007f1d4c: cmp      r0, #0
007f1d50: movwne   r8, #0xcccd
007f1d54: movtne   r8, #0xbe4c
007f1d58: b        #0x7f1768

# _ZN9b2ContactD1Ev
007e65c0: bx       lr

# _ZN6b2BodyD2Ev
007e1644: bx       lr

# _ZN15b2ContactFilterD0Ev
007e8d24: push     {r4, lr}
007e8d28: mov      r4, r0
007e8d2c: bl       #0x30e2b0
007e8d30: mov      r0, r4
007e8d34: pop      {r4, pc}

# _ZN13b2PairManager10InitializeEP12b2BroadPhaseP14b2PairCallback
007e3eec: stm      r0, {r1, r2}
007e3ef0: bx       lr

# _ZN12b2BroadPhase11TestOverlapERK13b2BoundValuesP7b2Proxy
007e252c: push     {r4, r5}
007e2530: ldrh     ip, [r2, #4]
007e2534: mov      r3, #6
007e2538: add      r4, r0, #0x50000
007e253c: mul      ip, r3, ip
007e2540: add      r4, r4, #0x16
007e2544: ldrh     ip, [r4, ip]
007e2548: ldrh     r5, [r1]
007e254c: cmp      r5, ip
007e2550: bhi      #0x7e25b0
007e2554: ldrh     ip, [r2]
007e2558: ldrh     r5, [r1, #4]
007e255c: mul      ip, r3, ip
007e2560: ldrh     ip, [r4, ip]
007e2564: cmp      r5, ip
007e2568: blo      #0x7e25b0
007e256c: ldrh     ip, [r2, #6]
007e2570: add      r0, r0, #0x56000
007e2574: add      r0, r0, #0x16
007e2578: mul      ip, r3, ip
007e257c: ldrh     r4, [r1, #2]
007e2580: ldrh     ip, [r0, ip]
007e2584: cmp      r4, ip
007e2588: bhi      #0x7e25b0
007e258c: ldrh     ip, [r2, #2]
007e2590: ldrh     r2, [r1, #6]
007e2594: mul      r3, r3, ip
007e2598: ldrh     r3, [r0, r3]
007e259c: cmp      r2, r3
007e25a0: movlo    r0, #0
007e25a4: movhs    r0, #1
007e25a8: pop      {r4, r5}
007e25ac: bx       lr
007e25b0: mov      r0, #0
007e25b4: b        #0x7e25a8

# _ZN16b2ContactManager7DestroyEP9b2Contact
007ea084: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ea088: mov      ip, r1
007ea08c: ldr      r1, [r1, #8]
007ea090: sub      sp, sp, #0x5c
007ea094: str      r0, [sp, #0x10]
007ea098: str      r1, [sp, #0x18]
007ea09c: ldr      r2, [ip, #0x34]
007ea0a0: cmp      r1, #0
007ea0a4: str      r2, [sp, #0x20]
007ea0a8: ldr      r3, [ip, #0x38]
007ea0ac: str      r3, [sp, #0x1c]
007ea0b0: ble      #0x7ea41c
007ea0b4: ldr      r3, [r0, #4]
007ea0b8: mov      r6, #0x19000
007ea0bc: add      r6, r6, #0x264
007ea0c0: ldr      r3, [r3, r6]
007ea0c4: cmp      r3, #0
007ea0c8: beq      #0x7ea41c
007ea0cc: ldr      r1, [sp, #0x1c]
007ea0d0: ldr      r3, [ip]
007ea0d4: ldr      r4, [r2, #0xc]
007ea0d8: ldr      r5, [r1, #0xc]
007ea0dc: mov      r0, ip
007ea0e0: str      ip, [sp, #8]
007ea0e4: mov      lr, pc
007ea0e8: ldr      pc, [r3]
007ea0ec: ldr      ip, [sp, #8]
007ea0f0: mov      r7, r0
007ea0f4: ldr      r2, [ip, #0x3c]
007ea0f8: ldr      r3, [ip, #0x40]
007ea0fc: ldr      lr, [ip, #0x34]
007ea100: ldr      r1, [ip, #0x38]
007ea104: str      r2, [sp, #0x4c]
007ea108: str      r3, [sp, #0x50]
007ea10c: mov      r2, #0
007ea110: add      r3, sp, #0x28
007ea114: str      lr, [sp, #0x28]
007ea118: str      r1, [sp, #0x2c]
007ea11c: str      r2, [sp, #0xc]
007ea120: str      r3, [sp, #0x14]
007ea124: str      ip, [sp, #0x24]
007ea128: ldr      r3, [r7, #0x40]
007ea12c: str      r3, [sp, #0x40]
007ea130: ldr      r3, [r7, #0x44]
007ea134: str      r3, [sp, #0x44]
007ea138: ldr      r3, [r7, #0x48]
007ea13c: cmp      r3, #0
007ea140: ble      #0x7ea3fc
007ea144: mov      r6, r7
007ea148: mov      r8, #0
007ea14c: ldr      sb, [r6]
007ea150: ldr      r1, [r4, #0xc]
007ea154: ldr      sl, [r6, #4]
007ea158: mov      r0, sb
007ea15c: bl       #0x30ed6c
007ea160: ldr      r1, [r4, #0x14]
007ea164: mov      fp, r0
007ea168: mov      r0, sl
007ea16c: bl       #0x30ed6c
007ea170: mov      r1, r0
007ea174: mov      r0, fp
007ea178: bl       #0x30eba4
007ea17c: ldr      r1, [r4, #0x10]
007ea180: mov      fp, r0
007ea184: mov      r0, sb
007ea188: bl       #0x30ed6c
007ea18c: ldr      r1, [r4, #0x18]
007ea190: mov      sb, r0
007ea194: mov      r0, sl
007ea198: bl       #0x30ed6c
007ea19c: mov      r1, r0
007ea1a0: mov      r0, sb
007ea1a4: bl       #0x30eba4
007ea1a8: ldr      r1, [r4, #4]
007ea1ac: mov      sl, r0
007ea1b0: mov      r0, fp
007ea1b4: bl       #0x30eba4
007ea1b8: ldr      r1, [r4, #8]
007ea1bc: mov      sb, r0
007ea1c0: mov      r0, sl
007ea1c4: bl       #0x30eba4
007ea1c8: str      sb, [sp, #0x30]
007ea1cc: str      r0, [sp, #0x34]
007ea1d0: ldr      sb, [r6]
007ea1d4: ldr      r1, [r4, #0xc]
007ea1d8: ldr      sl, [r6, #4]
007ea1dc: mov      r0, sb
007ea1e0: bl       #0x30ed6c
007ea1e4: ldr      r1, [r4, #0x14]
007ea1e8: mov      fp, r0
007ea1ec: mov      r0, sl
007ea1f0: bl       #0x30ed6c
007ea1f4: mov      r1, r0
007ea1f8: mov      r0, fp
007ea1fc: bl       #0x30eba4
007ea200: ldr      r1, [r4, #0x10]
007ea204: mov      fp, r0
007ea208: mov      r0, sb
007ea20c: bl       #0x30ed6c
007ea210: ldr      r1, [r4, #0x18]
007ea214: mov      sb, r0
007ea218: mov      r0, sl
007ea21c: bl       #0x30ed6c
007ea220: mov      r1, r0
007ea224: mov      r0, sb
007ea228: bl       #0x30eba4
007ea22c: ldr      r1, [r4, #4]
007ea230: mov      sl, r0
007ea234: mov      r0, fp
007ea238: bl       #0x30eba4
007ea23c: ldr      r1, [r4, #8]
007ea240: mov      fp, r0
007ea244: mov      r0, sl
007ea248: bl       #0x30eba4
007ea24c: ldr      r1, [r4, #0x2c]
007ea250: mov      sb, r0
007ea254: mov      r0, fp
007ea258: bl       #0x30e3ac
007ea25c: ldr      sl, [r4, #0x48]
007ea260: mov      fp, r0
007ea264: ldr      r1, [r4, #0x30]
007ea268: mov      r0, sb
007ea26c: bl       #0x30e3ac
007ea270: add      r1, sl, #0x80000000
007ea274: bl       #0x30ed6c
007ea278: mov      r1, fp
007ea27c: mov      sb, r0
007ea280: mov      r0, sl
007ea284: bl       #0x30ed6c
007ea288: ldr      r1, [r4, #0x40]
007ea28c: mov      sl, r0
007ea290: mov      r0, sb
007ea294: bl       #0x30eba4
007ea298: ldr      r1, [r4, #0x44]
007ea29c: mov      r2, r0
007ea2a0: mov      r0, sl
007ea2a4: str      r2, [sp, #4]
007ea2a8: bl       #0x30eba4
007ea2ac: ldr      sb, [r6, #8]
007ea2b0: mov      r3, r0
007ea2b4: ldr      r1, [r5, #0xc]
007ea2b8: mov      r0, sb
007ea2bc: ldr      sl, [r6, #0xc]
007ea2c0: str      r3, [sp, #8]
007ea2c4: bl       #0x30ed6c
007ea2c8: ldr      r1, [r5, #0x14]
007ea2cc: mov      fp, r0
007ea2d0: mov      r0, sl
007ea2d4: bl       #0x30ed6c
007ea2d8: mov      r1, r0
007ea2dc: mov      r0, fp
007ea2e0: bl       #0x30eba4
007ea2e4: ldr      r1, [r5, #0x10]
007ea2e8: mov      fp, r0
007ea2ec: mov      r0, sb
007ea2f0: bl       #0x30ed6c
007ea2f4: ldr      r1, [r5, #0x18]
007ea2f8: mov      sb, r0
007ea2fc: mov      r0, sl
007ea300: bl       #0x30ed6c
007ea304: mov      r1, r0
007ea308: mov      r0, sb
007ea30c: bl       #0x30eba4
007ea310: ldr      r1, [r5, #4]
007ea314: mov      sl, r0
007ea318: mov      r0, fp
007ea31c: bl       #0x30eba4
007ea320: ldr      r1, [r5, #8]
007ea324: mov      fp, r0
007ea328: mov      r0, sl
007ea32c: bl       #0x30eba4
007ea330: ldr      r1, [r5, #0x2c]
007ea334: mov      sb, r0
007ea338: mov      r0, fp
007ea33c: bl       #0x30e3ac
007ea340: ldr      sl, [r5, #0x48]
007ea344: ldr      r1, [r5, #0x30]
007ea348: mov      fp, r0
007ea34c: mov      r0, sb
007ea350: bl       #0x30e3ac
007ea354: add      r1, sl, #0x80000000
007ea358: bl       #0x30ed6c
007ea35c: mov      r1, fp
007ea360: mov      sb, r0
007ea364: mov      r0, sl
007ea368: bl       #0x30ed6c
007ea36c: ldr      r1, [r5, #0x40]
007ea370: mov      sl, r0
007ea374: mov      r0, sb
007ea378: bl       #0x30eba4
007ea37c: ldr      r1, [r5, #0x44]
007ea380: mov      sb, r0
007ea384: mov      r0, sl
007ea388: bl       #0x30eba4
007ea38c: ldr      r3, [sp, #8]
007ea390: add      r8, r8, #1
007ea394: mov      r1, r3
007ea398: bl       #0x30e3ac
007ea39c: ldr      r2, [sp, #4]
007ea3a0: str      r0, [sp, #0x3c]
007ea3a4: mov      r0, sb
007ea3a8: mov      r1, r2
007ea3ac: bl       #0x30e3ac
007ea3b0: str      r0, [sp, #0x38]
007ea3b4: ldr      r2, [r6, #0x10]
007ea3b8: ldr      r0, [sp, #0x10]
007ea3bc: ldr      r1, [sp, #0x14]
007ea3c0: ldr      r3, [r0, #4]
007ea3c4: str      r2, [sp, #0x48]
007ea3c8: ldr      r2, [r6, #0x1c]
007ea3cc: add      r6, r6, #0x20
007ea3d0: str      r2, [sp, #0x54]
007ea3d4: mov      r2, #0x19000
007ea3d8: add      r2, r2, #0x264
007ea3dc: ldr      r3, [r3, r2]
007ea3e0: mov      r0, r3
007ea3e4: ldr      r3, [r3]
007ea3e8: mov      lr, pc
007ea3ec: ldr      pc, [r3, #0x10]
007ea3f0: ldr      r3, [r7, #0x48]
007ea3f4: cmp      r3, r8
007ea3f8: bgt      #0x7ea14c
007ea3fc: ldr      r3, [sp, #0xc]
007ea400: ldr      r0, [sp, #0x18]
007ea404: add      r7, r7, #0x4c
007ea408: add      r3, r3, #1
007ea40c: cmp      r3, r0
007ea410: str      r3, [sp, #0xc]
007ea414: bne      #0x7ea128
007ea418: ldr      ip, [sp, #0x24]
007ea41c: ldr      r3, [ip, #0xc]
007ea420: cmp      r3, #0
007ea424: ldrne    r2, [ip, #0x10]
007ea428: strne    r2, [r3, #0x10]
007ea42c: ldr      r3, [ip, #0x10]
007ea430: cmp      r3, #0
007ea434: ldrne    r2, [ip, #0xc]
007ea438: strne    r2, [r3, #0xc]
007ea43c: ldr      r1, [sp, #0x10]
007ea440: mov      r3, #0x19000
007ea444: add      r3, r3, #0x238
007ea448: ldr      r2, [r1, #4]
007ea44c: ldr      r1, [r2, r3]
007ea450: cmp      r1, ip
007ea454: ldreq    r1, [ip, #0x10]
007ea458: streq    r1, [r2, r3]
007ea45c: ldr      r3, [ip, #0x1c]
007ea460: ldr      r0, [sp, #0x1c]
007ea464: ldr      r2, [sp, #0x20]
007ea468: cmp      r3, #0
007ea46c: ldr      r1, [r2, #0xc]
007ea470: ldr      r2, [r0, #0xc]
007ea474: ldrne    r0, [ip, #0x20]
007ea478: strne    r0, [r3, #0xc]
007ea47c: ldr      r3, [ip, #0x20]
007ea480: cmp      r3, #0
007ea484: ldrne    r0, [ip, #0x1c]
007ea488: strne    r0, [r3, #8]
007ea48c: ldr      r0, [r1, #0x70]
007ea490: add      r3, ip, #0x14
007ea494: cmp      r0, r3
007ea498: ldreq    r3, [ip, #0x20]
007ea49c: mov      r0, ip
007ea4a0: streq    r3, [r1, #0x70]
007ea4a4: ldr      r3, [ip, #0x2c]
007ea4a8: cmp      r3, #0
007ea4ac: ldrne    r1, [ip, #0x30]
007ea4b0: strne    r1, [r3, #0xc]
007ea4b4: ldr      r3, [ip, #0x30]
007ea4b8: cmp      r3, #0
007ea4bc: ldrne    r1, [ip, #0x2c]
007ea4c0: strne    r1, [r3, #8]
007ea4c4: ldr      r1, [r2, #0x70]
007ea4c8: add      r3, ip, #0x24
007ea4cc: cmp      r1, r3
007ea4d0: ldreq    r3, [ip, #0x30]
007ea4d4: streq    r3, [r2, #0x70]
007ea4d8: ldr      r2, [sp, #0x10]
007ea4dc: ldr      r1, [r2, #4]
007ea4e0: bl       #0x7e9c98
007ea4e4: ldr      r3, [sp, #0x10]
007ea4e8: ldr      r2, [r3, #4]
007ea4ec: mov      r3, #0x19000
007ea4f0: add      r3, r3, #0x240
007ea4f4: ldr      r1, [r2, r3]
007ea4f8: sub      r1, r1, #1
007ea4fc: str      r1, [r2, r3]
007ea500: add      sp, sp, #0x5c
007ea504: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN13b2PairManager13ValidateTableEv
007e42dc: bx       lr

# _ZN7b2ShapeD2Ev
007e6178: bx       lr

# _ZN14b2PairCallbackD0Ev
007e80a4: push     {r4, lr}
007e80a8: mov      r4, r0
007e80ac: bl       #0x30e2b0
007e80b0: mov      r0, r4
007e80b4: pop      {r4, pc}

# _ZNK16b2PrismaticJoint13GetMotorForceEv
007eeff0: ldr      r0, [r0, #0xac]
007eeff4: bx       lr

# _ZNK13b2CircleShape9TestPointERK7b2XFormRK6b2Vec2
007e9218: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e921c: ldr      r7, [r0, #0x30]
007e9220: mov      r4, r1
007e9224: ldr      r6, [r0, #0x34]
007e9228: mov      r5, r0
007e922c: ldr      r1, [r1, #8]
007e9230: mov      r0, r7
007e9234: mov      r8, r2
007e9238: bl       #0x30ed6c
007e923c: ldr      r1, [r4, #0x10]
007e9240: mov      sl, r0
007e9244: mov      r0, r6
007e9248: bl       #0x30ed6c
007e924c: mov      r1, r0
007e9250: mov      r0, sl
007e9254: bl       #0x30eba4
007e9258: ldr      r1, [r4, #0xc]
007e925c: mov      sl, r0
007e9260: mov      r0, r7
007e9264: bl       #0x30ed6c
007e9268: ldr      r1, [r4, #0x14]
007e926c: mov      r7, r0
007e9270: mov      r0, r6
007e9274: bl       #0x30ed6c
007e9278: mov      r1, r0
007e927c: mov      r0, r7
007e9280: bl       #0x30eba4
007e9284: ldr      r1, [r4]
007e9288: mov      r7, r0
007e928c: mov      r0, sl
007e9290: bl       #0x30eba4
007e9294: ldr      r1, [r4, #4]
007e9298: mov      r6, r0
007e929c: mov      r0, r7
007e92a0: bl       #0x30eba4
007e92a4: mov      r1, r6
007e92a8: mov      r4, r0
007e92ac: ldr      r0, [r8]
007e92b0: bl       #0x30e3ac
007e92b4: mov      r1, r4
007e92b8: mov      r6, r0
007e92bc: ldr      r0, [r8, #4]
007e92c0: bl       #0x30e3ac
007e92c4: mov      r7, r0
007e92c8: ldr      r0, [r5, #0x38]
007e92cc: mov      r4, #0
007e92d0: mov      r1, r0
007e92d4: bl       #0x30ed6c
007e92d8: mov      r1, r6
007e92dc: mov      r5, r0
007e92e0: mov      r0, r6
007e92e4: bl       #0x30ed6c
007e92e8: mov      r1, r7
007e92ec: mov      r6, r0
007e92f0: mov      r0, r7
007e92f4: bl       #0x30ed6c
007e92f8: mov      r1, r0
007e92fc: mov      r0, r6
007e9300: bl       #0x30eba4
007e9304: mov      r1, r0
007e9308: mov      r0, r5
007e930c: bl       #0x30e4b4
007e9310: cmp      r0, #0
007e9314: movne    r4, #1
007e9318: and      r0, r4, #1
007e931c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK13b2PulleyJoint16GetReactionForceEv
007f0434: push     {r4, r5, r6, r7, r8, lr}
007f0438: ldr      r6, [r1, #0x94]
007f043c: mov      r4, r0
007f0440: mov      r5, r1
007f0444: mov      r0, r6
007f0448: ldr      r1, [r1, #0x74]
007f044c: bl       #0x30ed6c
007f0450: ldr      r1, [r5, #0x70]
007f0454: mov      r7, r0
007f0458: mov      r0, r6
007f045c: bl       #0x30ed6c
007f0460: str      r7, [r4, #4]
007f0464: str      r0, [r4]
007f0468: mov      r0, r4
007f046c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12b2BroadPhaseC1ERK6b2AABBP14b2PairCallback
007e3048: push     {r4, r5, r6, r7, r8, lr}
007e304c: mov      r4, r0
007e3050: mov      r6, r2
007e3054: mov      r5, r1
007e3058: bl       #0x7e3e54
007e305c: mov      r2, r6
007e3060: mov      r0, r4
007e3064: mov      r1, r4
007e3068: bl       #0x7e3eec
007e306c: ldm      r5, {r0, r1, r2, r3}
007e3070: add      ip, r4, #0x5d000
007e3074: add      ip, ip, #0x1c
007e3078: stm      ip, {r0, r1, r2, r3}
007e307c: mov      r3, #0x5d000
007e3080: add      r3, r3, #0x34
007e3084: mov      r6, #0
007e3088: str      r6, [r4, r3]
007e308c: ldr      r1, [r5, #4]
007e3090: ldr      r0, [r5, #0xc]
007e3094: bl       #0x30e3ac
007e3098: ldr      r1, [r5]
007e309c: mov      r7, r0
007e30a0: ldr      r0, [r5, #8]
007e30a4: bl       #0x30e3ac
007e30a8: mov      r1, r0
007e30ac: movw     r0, #0xff00
007e30b0: movt     r0, #0x477f
007e30b4: bl       #0x30ec94
007e30b8: mov      r3, #0x5d000
007e30bc: add      r3, r3, #0x2c
007e30c0: str      r0, [r4, r3]
007e30c4: movw     r0, #0xff00
007e30c8: mov      r1, r7
007e30cc: movt     r0, #0x477f
007e30d0: bl       #0x30ec94
007e30d4: mov      r3, #0x5d000
007e30d8: add      r3, r3, #0x30
007e30dc: add      r2, r4, #0x48000
007e30e0: str      r0, [r4, r3]
007e30e4: add      r2, r2, #0x14
007e30e8: mov      r3, r6
007e30ec: movw     r1, #0x7ff
007e30f0: add      r6, r6, #1
007e30f4: uxth     r6, r6
007e30f8: mov      r0, #0
007e30fc: mvn      r8, #0
007e3100: cmp      r6, r1
007e3104: strh     r6, [r2]
007e3108: strh     r0, [r2, #0xa]
007e310c: strh     r8, [r2, #8]
007e3110: str      r3, [r2, #0xc]
007e3114: add      r2, r2, #0x10
007e3118: bne      #0x7e30f0
007e311c: mov      r7, #0x50000
007e3120: mov      r6, r7
007e3124: mov      r5, r7
007e3128: mov      ip, r7
007e312c: mov      r0, r7
007e3130: mov      r1, #0x5d000
007e3134: mov      r2, r1
007e3138: add      r7, r7, #4
007e313c: add      r6, r6, #0xe
007e3140: add      r5, r5, #0xc
007e3144: add      ip, ip, #0x10
007e3148: add      r0, r0, #0x14
007e314c: strh     r8, [r4, r7]
007e3150: add      r1, r1, #0x38
007e3154: strh     r3, [r4, r6]
007e3158: add      r2, r2, #0x18
007e315c: strh     r8, [r4, r5]
007e3160: str      r3, [r4, ip]
007e3164: strh     r3, [r4, r0]
007e3168: mov      r0, #1
007e316c: strh     r0, [r4, r1]
007e3170: str      r3, [r4, r2]
007e3174: mov      r0, r4
007e3178: pop      {r4, r5, r6, r7, r8, pc}

# _ZN13b2CircleShapeC2EPK10b2ShapeDef
007e9a08: push     {r4, r5, r6, lr}
007e9a0c: ldr      r5, [pc, #0x44]
007e9a10: mov      r4, r0
007e9a14: mov      r6, r1
007e9a18: bl       #0x7e6068
007e9a1c: ldr      r3, [pc, #0x38]
007e9a20: add      r5, pc, r5
007e9a24: mov      r2, #0
007e9a28: ldr      r3, [r5, r3]
007e9a2c: str      r2, [r4, #4]
007e9a30: mov      r0, r4
007e9a34: add      r3, r3, #8
007e9a38: str      r3, [r4]
007e9a3c: ldr      r3, [r6, #0x20]
007e9a40: str      r3, [r4, #0x30]
007e9a44: ldr      r3, [r6, #0x24]
007e9a48: str      r3, [r4, #0x34]
007e9a4c: ldr      r3, [r6, #0x28]
007e9a50: str      r3, [r4, #0x38]
007e9a54: pop      {r4, r5, r6, pc}
007e9a58: andseq   fp, sl, r0, ror r0
007e9a5c: andeq    r0, r0, ip, ror #16

# _ZN12b2MouseJointC2EPK15b2MouseJointDef
007ebd48: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007ebd4c: ldr      r7, [pc, #0x154]
007ebd50: mov      r4, r0
007ebd54: mov      r6, r1
007ebd58: bl       #0x7eb1e0
007ebd5c: ldr      r3, [pc, #0x148]
007ebd60: add      r7, pc, r7
007ebd64: ldr      r5, [r4, #0x34]
007ebd68: ldr      r3, [r7, r3]
007ebd6c: add      r3, r3, #8
007ebd70: str      r3, [r4]
007ebd74: ldr      r3, [r6, #0x14]
007ebd78: str      r3, [r4, #0x4c]
007ebd7c: ldr      r3, [r6, #0x18]
007ebd80: ldr      r0, [r4, #0x4c]
007ebd84: str      r3, [r4, #0x50]
007ebd88: ldr      r1, [r5, #4]
007ebd8c: bl       #0x30e3ac
007ebd90: ldr      r1, [r5, #8]
007ebd94: mov      r8, r0
007ebd98: ldr      r0, [r4, #0x50]
007ebd9c: bl       #0x30e3ac
007ebda0: ldr      r1, [r5, #0xc]
007ebda4: mov      r7, r0
007ebda8: mov      r0, r8
007ebdac: bl       #0x30ed6c
007ebdb0: ldr      r1, [r5, #0x10]
007ebdb4: mov      sl, r0
007ebdb8: mov      r0, r7
007ebdbc: bl       #0x30ed6c
007ebdc0: mov      r1, r0
007ebdc4: mov      r0, sl
007ebdc8: bl       #0x30eba4
007ebdcc: ldr      r1, [r5, #0x14]
007ebdd0: mov      sl, r0
007ebdd4: mov      r0, r8
007ebdd8: bl       #0x30ed6c
007ebddc: ldr      r1, [r5, #0x18]
007ebde0: mov      r8, r0
007ebde4: mov      r0, r7
007ebde8: bl       #0x30ed6c
007ebdec: mov      r1, r0
007ebdf0: mov      r0, r8
007ebdf4: bl       #0x30eba4
007ebdf8: str      sl, [r4, #0x44]
007ebdfc: str      r0, [r4, #0x48]
007ebe00: ldr      r2, [r6, #0x1c]
007ebe04: mov      r3, #0
007ebe08: str      r3, [r4, #0x58]
007ebe0c: str      r2, [r4, #0x74]
007ebe10: str      r3, [r4, #0x54]
007ebe14: movw     r1, #0xfdb
007ebe18: ldr      r0, [r6, #0x20]
007ebe1c: movt     r1, #0x40c9
007ebe20: bl       #0x30ed6c
007ebe24: ldr      r8, [r5, #0x74]
007ebe28: ldr      r1, [r6, #0x28]
007ebe2c: mov      r5, r0
007ebe30: mov      r0, r8
007ebe34: bl       #0x30ed6c
007ebe38: mov      r1, r5
007ebe3c: mov      r7, r0
007ebe40: mov      r0, r5
007ebe44: bl       #0x30ed6c
007ebe48: mov      r1, r0
007ebe4c: mov      r0, r7
007ebe50: bl       #0x30ed6c
007ebe54: mov      r1, r8
007ebe58: mov      r7, r0
007ebe5c: mov      r0, r8
007ebe60: bl       #0x30eba4
007ebe64: ldr      r1, [r6, #0x24]
007ebe68: bl       #0x30ed6c
007ebe6c: mov      r1, r5
007ebe70: bl       #0x30ed6c
007ebe74: mov      r1, r7
007ebe78: bl       #0x30eba4
007ebe7c: mov      r5, r0
007ebe80: mov      r1, r0
007ebe84: mov      r0, #0x3f800000
007ebe88: bl       #0x30ec94
007ebe8c: mov      r1, r5
007ebe90: str      r0, [r4, #0x7c]
007ebe94: mov      r0, r7
007ebe98: bl       #0x30ec94
007ebe9c: str      r0, [r4, #0x78]
007ebea0: mov      r0, r4
007ebea4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007ebea8: andseq   r8, sl, r0, lsr sp
007ebeac: ldrdeq   r1, r2, [r0], -ip

# _ZN7b2World10CreateBodyEPK9b2BodyDef
007e7ef8: push     {r4, r5, r6, r7, r8, lr}
007e7efc: mov      r3, #0x19000
007e7f00: add      r3, r3, #0x1d4
007e7f04: ldrb     r6, [r0, r3]
007e7f08: mov      r4, r0
007e7f0c: mov      r7, r1
007e7f10: cmp      r6, #0
007e7f14: movne    r5, #0
007e7f18: bne      #0x7e7f74
007e7f1c: mov      r1, #0x94
007e7f20: bl       #0x7e90bc
007e7f24: mov      r2, r4
007e7f28: mov      r1, r7
007e7f2c: mov      r5, r0
007e7f30: bl       #0x7e2218
007e7f34: mov      r3, #0x19000
007e7f38: str      r6, [r5, #0x5c]
007e7f3c: add      r3, r3, #0x230
007e7f40: ldr      r2, [r4, r3]
007e7f44: str      r2, [r5, #0x60]
007e7f48: ldr      r3, [r4, r3]
007e7f4c: mov      r2, #0x19000
007e7f50: add      r2, r2, #0x230
007e7f54: cmp      r3, #0
007e7f58: strne    r5, [r3, #0x5c]
007e7f5c: mov      r3, #0x19000
007e7f60: str      r5, [r4, r2]
007e7f64: add      r3, r3, #0x23c
007e7f68: ldr      r2, [r4, r3]
007e7f6c: add      r2, r2, #1
007e7f70: str      r2, [r4, r3]
007e7f74: mov      r0, r5
007e7f78: pop      {r4, r5, r6, r7, r8, pc}

# _ZN9b2ContactC2EP7b2ShapeS1_
007e9eb8: ldr      r3, [pc, #0xc8]
007e9ebc: ldr      ip, [pc, #0xc8]
007e9ec0: push     {r4, r5, r6, lr}
007e9ec4: add      r3, pc, r3
007e9ec8: ldr      ip, [r3, ip]
007e9ecc: mov      r4, r0
007e9ed0: mov      r0, #0
007e9ed4: add      ip, ip, #8
007e9ed8: str      r0, [r4, #4]
007e9edc: str      ip, [r4]
007e9ee0: mov      r0, r1
007e9ee4: ldrb     r1, [r1, #0x28]
007e9ee8: cmp      r1, #0
007e9eec: beq      #0x7e9f78
007e9ef0: mov      r3, #1
007e9ef4: str      r3, [r4, #4]
007e9ef8: mov      r3, #0
007e9efc: str      r3, [r4, #8]
007e9f00: str      r2, [r4, #0x38]
007e9f04: str      r0, [r4, #0x34]
007e9f08: ldr      r1, [r2, #0x18]
007e9f0c: ldr      r0, [r0, #0x18]
007e9f10: bl       #0x30ed6c
007e9f14: bl       #0x30e124
007e9f18: ldr      r3, [r4, #0x38]
007e9f1c: ldr      r2, [r4, #0x34]
007e9f20: str      r0, [r4, #0x3c]
007e9f24: ldr      r5, [r3, #0x1c]
007e9f28: ldr      r6, [r2, #0x1c]
007e9f2c: mov      r1, r5
007e9f30: mov      r0, r6
007e9f34: bl       #0x30e2f8
007e9f38: cmp      r0, #0
007e9f3c: mov      r3, #0
007e9f40: movne    r5, r6
007e9f44: str      r5, [r4, #0x40]
007e9f48: str      r3, [r4, #0x24]
007e9f4c: str      r3, [r4, #0xc]
007e9f50: str      r3, [r4, #0x10]
007e9f54: str      r3, [r4, #0x18]
007e9f58: str      r3, [r4, #0x1c]
007e9f5c: str      r3, [r4, #0x20]
007e9f60: str      r3, [r4, #0x14]
007e9f64: str      r3, [r4, #0x28]
007e9f68: str      r3, [r4, #0x2c]
007e9f6c: str      r3, [r4, #0x30]
007e9f70: mov      r0, r4
007e9f74: pop      {r4, r5, r6, pc}
007e9f78: ldrb     r3, [r2, #0x28]
007e9f7c: cmp      r3, #0
007e9f80: beq      #0x7e9ef8
007e9f84: b        #0x7e9ef0
007e9f88: andseq   sl, sl, ip, asr #23
007e9f8c: andeq    r2, r0, r4, lsr r3

# _ZN12b2BroadPhase11CreateProxyERK6b2AABBPv
007e2d54: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e2d58: mov      r3, #0x50000
007e2d5c: add      r3, r3, #0x14
007e2d60: ldrh     r5, [r0, r3]
007e2d64: mov      lr, #0x48000
007e2d68: add      lr, lr, #0x20
007e2d6c: add      ip, r5, #0x4800
007e2d70: add      ip, ip, #1
007e2d74: add      r8, r0, ip, lsl #4
007e2d78: ldrh     r8, [r8, #4]
007e2d7c: add      r7, r0, r5, lsl #4
007e2d80: add      r6, r7, #0x48000
007e2d84: strh     r8, [r0, r3]
007e2d88: add      r6, r6, #0x18
007e2d8c: mov      r3, #0
007e2d90: mov      ip, #0x5d000
007e2d94: strh     r3, [r6, #4]
007e2d98: add      ip, ip, #0x34
007e2d9c: str      r2, [r7, lr]
007e2da0: ldr      lr, [r0, ip]
007e2da4: sub      sp, sp, #0x3c
007e2da8: add      ip, sp, #0x30
007e2dac: add      fp, sp, #0x34
007e2db0: mov      r3, r1
007e2db4: mov      r2, ip
007e2db8: lsl      lr, lr, #1
007e2dbc: mov      r1, fp
007e2dc0: str      lr, [sp, #0x18]
007e2dc4: str      ip, [sp, #0x14]
007e2dc8: mov      r4, r0
007e2dcc: bl       #0x7e25b8
007e2dd0: ldr      lr, [pc, #0x268]
007e2dd4: ldr      r2, [sp, #0x18]
007e2dd8: ldr      ip, [sp, #0x14]
007e2ddc: str      lr, [sp, #0x1c]
007e2de0: add      sl, r2, #1
007e2de4: ldr      r2, [sp, #0x1c]
007e2de8: mov      sb, #0
007e2dec: add      r3, sp, #0x2c
007e2df0: add      lr, sp, #0x28
007e2df4: add      r2, pc, r2
007e2df8: mov      r7, sb
007e2dfc: str      r3, [sp, #0x24]
007e2e00: str      lr, [sp, #0x20]
007e2e04: mov      r6, #6
007e2e08: mov      r8, r5
007e2e0c: str      r2, [sp, #0x1c]
007e2e10: mov      r3, #0x6000
007e2e14: ldrh     lr, [ip, sb]
007e2e18: mul      r5, r3, r7
007e2e1c: ldrh     r3, [fp, sb]
007e2e20: add      r5, r5, #0x50000
007e2e24: str      lr, [sp]
007e2e28: ldr      lr, [sp, #0x18]
007e2e2c: add      r5, r4, r5
007e2e30: add      r5, r5, #0x16
007e2e34: mov      r0, r4
007e2e38: ldr      r1, [sp, #0x24]
007e2e3c: ldr      r2, [sp, #0x20]
007e2e40: str      lr, [sp, #8]
007e2e44: str      ip, [sp, #0x14]
007e2e48: str      r5, [sp, #4]
007e2e4c: str      r7, [sp, #0xc]
007e2e50: bl       #0x7e2820
007e2e54: ldr      r2, [sp, #0x28]
007e2e58: ldr      r3, [sp, #0x18]
007e2e5c: mul      r1, r6, r2
007e2e60: rsb      r2, r2, r3
007e2e64: add      r0, r1, #0xc
007e2e68: mul      r2, r6, r2
007e2e6c: add      r1, r5, r1
007e2e70: add      r0, r5, r0
007e2e74: bl       #0x30df38
007e2e78: ldr      r1, [sp, #0x2c]
007e2e7c: ldr      r2, [sp, #0x28]
007e2e80: mla      r0, r1, r6, r6
007e2e84: rsb      r2, r1, r2
007e2e88: mul      r2, r6, r2
007e2e8c: add      r0, r5, r0
007e2e90: mla      r1, r6, r1, r5
007e2e94: bl       #0x30df38
007e2e98: ldr      r3, [sp, #0x2c]
007e2e9c: ldr      r2, [sp, #0x28]
007e2ea0: ldrh     lr, [fp, sb]
007e2ea4: mul      r3, r6, r3
007e2ea8: add      r2, r2, #1
007e2eac: str      r2, [sp, #0x28]
007e2eb0: strh     lr, [r5, r3]
007e2eb4: ldr      r3, [sp, #0x2c]
007e2eb8: mla      r3, r6, r3, r5
007e2ebc: strh     r8, [r3, #2]
007e2ec0: ldr      r3, [sp, #0x28]
007e2ec4: ldr      ip, [sp, #0x14]
007e2ec8: mul      r3, r6, r3
007e2ecc: ldrh     r2, [ip, sb]
007e2ed0: strh     r2, [r5, r3]
007e2ed4: ldr      r3, [sp, #0x28]
007e2ed8: mla      r3, r6, r3, r5
007e2edc: strh     r8, [r3, #2]
007e2ee0: ldr      r3, [sp, #0x2c]
007e2ee4: cmp      r3, #0
007e2ee8: mla      r2, r6, r3, r5
007e2eec: subne    r3, r3, #1
007e2ef0: mlane    r3, r6, r3, r5
007e2ef4: ldrhne   r3, [r3, #4]
007e2ef8: strh     r3, [r2, #4]
007e2efc: ldr      r3, [sp, #0x28]
007e2f00: sub      r2, r3, #1
007e2f04: mla      r2, r6, r2, r5
007e2f08: mla      r3, r6, r3, r5
007e2f0c: ldrh     r2, [r2, #4]
007e2f10: strh     r2, [r3, #4]
007e2f14: ldr      r3, [sp, #0x2c]
007e2f18: ldr      r2, [sp, #0x28]
007e2f1c: cmp      r3, r2
007e2f20: bge      #0x7e2f4c
007e2f24: mla      r2, r6, r3, r5
007e2f28: add      r2, r2, #4
007e2f2c: ldrh     r1, [r2]
007e2f30: add      r3, r3, #1
007e2f34: add      r1, r1, #1
007e2f38: strh     r1, [r2], #6
007e2f3c: ldr      r1, [sp, #0x28]
007e2f40: cmp      r1, r3
007e2f44: bgt      #0x7e2f2c
007e2f48: ldr      r3, [sp, #0x2c]
007e2f4c: cmp      sl, r3
007e2f50: blt      #0x7e2f8c
007e2f54: mla      r5, r6, r3, r5
007e2f58: add      r5, r5, #2
007e2f5c: ldrh     r2, [r5]
007e2f60: ldrh     r1, [r5, #-2]
007e2f64: add      r5, r5, #6
007e2f68: add      r2, r7, r2, lsl #3
007e2f6c: add      r2, r2, #0x24000
007e2f70: tst      r1, #1
007e2f74: add      r2, r4, r2, lsl #1
007e2f78: strheq   r3, [r2, #0x14]
007e2f7c: strhne   r3, [r2, #0x18]
007e2f80: add      r3, r3, #1
007e2f84: cmp      r3, sl
007e2f88: ble      #0x7e2f5c
007e2f8c: add      r7, r7, #1
007e2f90: cmp      r7, #2
007e2f94: add      sb, sb, #2
007e2f98: bne      #0x7e2e10
007e2f9c: mov      r3, #0x5d000
007e2fa0: add      r3, r3, #0x34
007e2fa4: ldr      r2, [r4, r3]
007e2fa8: mov      r5, r8
007e2fac: mov      r8, #0x5d000
007e2fb0: add      r2, r2, #1
007e2fb4: add      r8, r8, #0x18
007e2fb8: str      r2, [r4, r3]
007e2fbc: ldr      r3, [r4, r8]
007e2fc0: cmp      r3, #0
007e2fc4: ble      #0x7e2ff4
007e2fc8: add      r7, r4, #0x5c000
007e2fcc: add      r7, r7, #0x16
007e2fd0: mov      r6, #0
007e2fd4: mov      r0, r4
007e2fd8: mov      r1, r5
007e2fdc: ldrh     r2, [r7], #2
007e2fe0: bl       #0x7e4190
007e2fe4: ldr      r3, [r4, r8]
007e2fe8: add      r6, r6, #1
007e2fec: cmp      r3, r6
007e2ff0: bgt      #0x7e2fd4
007e2ff4: mov      r0, r4
007e2ff8: bl       #0x7e42e0
007e2ffc: ldr      r3, [pc, #0x40]
007e3000: ldr      lr, [sp, #0x1c]
007e3004: ldr      r3, [lr, r3]
007e3008: ldrb     r3, [r3]
007e300c: cmp      r3, #0
007e3010: beq      #0x7e301c
007e3014: mov      r0, r4
007e3018: bl       #0x7e2ac4
007e301c: mov      r3, #0x5d000
007e3020: add      r3, r3, #0x18
007e3024: mov      r2, #0
007e3028: mov      r0, r4
007e302c: str      r2, [r4, r3]
007e3030: bl       #0x7e276c
007e3034: mov      r0, r5
007e3038: add      sp, sp, #0x3c
007e303c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e3040: mulseq   fp, ip, ip
007e3044: andeq    r2, r0, r8, ror #26

# _ZNK14b2PolygonShape7SupportERK7b2XFormRK6b2Vec2
007e526c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e5270: sub      sp, sp, #0x24
007e5274: str      r1, [sp, #4]
007e5278: ldr      r4, [r1, #0x118]
007e527c: str      r0, [sp, #0xc]
007e5280: ldr      r1, [r2, #8]
007e5284: ldr      r8, [r3, #4]
007e5288: ldr      sl, [r3]
007e528c: str      r1, [sp, #0x14]
007e5290: mov      r5, r2
007e5294: ldr      r2, [r2, #0xc]
007e5298: cmp      r4, #1
007e529c: str      r2, [sp, #0x10]
007e52a0: ldr      r3, [r5, #0x10]
007e52a4: ldr      r2, [sp, #4]
007e52a8: str      r3, [sp, #0x1c]
007e52ac: ldr      r1, [r5, #0x14]
007e52b0: str      r1, [sp, #0x18]
007e52b4: ldr      r7, [r2, #0xd8]
007e52b8: ldr      r6, [r2, #0xdc]
007e52bc: ble      #0x7e53c0
007e52c0: mov      r0, sl
007e52c4: ldr      r1, [sp, #0x14]
007e52c8: bl       #0x30ed6c
007e52cc: ldr      r1, [sp, #0x10]
007e52d0: mov      sb, r0
007e52d4: mov      r0, r8
007e52d8: bl       #0x30ed6c
007e52dc: mov      r1, r0
007e52e0: mov      r0, sb
007e52e4: bl       #0x30eba4
007e52e8: ldr      r1, [sp, #0x1c]
007e52ec: mov      fp, r0
007e52f0: mov      r0, sl
007e52f4: bl       #0x30ed6c
007e52f8: ldr      r1, [sp, #0x18]
007e52fc: mov      sl, r0
007e5300: mov      r0, r8
007e5304: bl       #0x30ed6c
007e5308: mov      r1, r0
007e530c: mov      r0, sl
007e5310: bl       #0x30eba4
007e5314: mov      r1, r7
007e5318: mov      sb, r0
007e531c: mov      r0, fp
007e5320: bl       #0x30ed6c
007e5324: mov      r1, r6
007e5328: mov      r7, r0
007e532c: mov      r0, sb
007e5330: bl       #0x30ed6c
007e5334: mov      r1, r0
007e5338: mov      r0, r7
007e533c: bl       #0x30eba4
007e5340: ldr      r6, [sp, #4]
007e5344: mov      r3, #0
007e5348: mov      sl, r0
007e534c: mov      r7, #1
007e5350: str      r3, [sp, #8]
007e5354: ldr      r1, [r6, #0xe0]
007e5358: mov      r0, fp
007e535c: bl       #0x30ed6c
007e5360: ldr      r1, [r6, #0xe4]
007e5364: mov      r8, r0
007e5368: mov      r0, sb
007e536c: bl       #0x30ed6c
007e5370: mov      r1, r0
007e5374: mov      r0, r8
007e5378: bl       #0x30eba4
007e537c: mov      r8, r0
007e5380: mov      r1, r8
007e5384: mov      r0, sl
007e5388: bl       #0x30e70c
007e538c: cmp      r0, #0
007e5390: strne    r7, [sp, #8]
007e5394: add      r7, r7, #1
007e5398: movne    sl, r8
007e539c: cmp      r7, r4
007e53a0: add      r6, r6, #8
007e53a4: bne      #0x7e5354
007e53a8: ldr      r1, [sp, #8]
007e53ac: add      r3, r1, #0x1b
007e53b0: ldr      r1, [sp, #4]
007e53b4: add      r2, r1, r3, lsl #3
007e53b8: ldr      r6, [r2, #4]
007e53bc: ldr      r7, [r1, r3, lsl #3]
007e53c0: ldr      r0, [sp, #0x14]
007e53c4: mov      r1, r7
007e53c8: bl       #0x30ed6c
007e53cc: mov      r1, r6
007e53d0: mov      r4, r0
007e53d4: ldr      r0, [sp, #0x1c]
007e53d8: bl       #0x30ed6c
007e53dc: mov      r1, r0
007e53e0: mov      r0, r4
007e53e4: bl       #0x30eba4
007e53e8: mov      r1, r7
007e53ec: mov      r8, r0
007e53f0: ldr      r0, [sp, #0x10]
007e53f4: bl       #0x30ed6c
007e53f8: mov      r1, r6
007e53fc: mov      r4, r0
007e5400: ldr      r0, [sp, #0x18]
007e5404: bl       #0x30ed6c
007e5408: mov      r1, r0
007e540c: mov      r0, r4
007e5410: bl       #0x30eba4
007e5414: ldr      r1, [r5, #4]
007e5418: bl       #0x30eba4
007e541c: ldr      r1, [r5]
007e5420: mov      r4, r0
007e5424: mov      r0, r8
007e5428: bl       #0x30eba4
007e542c: ldr      r2, [sp, #0xc]
007e5430: str      r4, [r2, #4]
007e5434: str      r0, [r2]
007e5438: ldr      r0, [sp, #0xc]
007e543c: add      sp, sp, #0x24
007e5440: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN7b2Shape12DestroyProxyEP12b2BroadPhase
007e6180: push     {r4, lr}
007e6184: ldrh     r3, [r0, #0x20]
007e6188: movw     r2, #0xffff
007e618c: mov      r4, r0
007e6190: cmp      r3, r2
007e6194: beq      #0x7e61ac
007e6198: mov      r0, r1
007e619c: mov      r1, r3
007e61a0: bl       #0x7e2acc
007e61a4: mvn      r3, #0
007e61a8: strh     r3, [r4, #0x20]
007e61ac: pop      {r4, pc}

# _ZN16b2ContactManager9PairAddedEPvS0_
007ea520: push     {r4, r5, r6, lr}
007ea524: ldr      r3, [r1, #0xc]
007ea528: mov      r6, r1
007ea52c: mov      r5, r2
007ea530: ldrsh    r1, [r3, #2]
007ea534: mov      r4, r0
007ea538: ldr      r2, [r2, #0xc]
007ea53c: cmp      r1, #0
007ea540: bne      #0x7ea550
007ea544: ldrsh    r1, [r2, #2]
007ea548: cmp      r1, #0
007ea54c: beq      #0x7ea590
007ea550: cmp      r3, r2
007ea554: beq      #0x7ea590
007ea558: ldr      r2, [r2, #0x6c]
007ea55c: cmp      r2, #0
007ea560: bne      #0x7ea574
007ea564: b        #0x7ea598
007ea568: ldr      r2, [r2, #0xc]
007ea56c: cmp      r2, #0
007ea570: beq      #0x7ea598
007ea574: ldr      r1, [r2]
007ea578: cmp      r3, r1
007ea57c: bne      #0x7ea568
007ea580: ldr      r3, [r2, #4]
007ea584: ldrb     r3, [r3, #0x3d]
007ea588: cmp      r3, #1
007ea58c: beq      #0x7ea598
007ea590: add      r0, r4, #8
007ea594: pop      {r4, r5, r6, pc}
007ea598: ldr      r2, [r4, #4]
007ea59c: mov      r3, #0x19000
007ea5a0: add      r3, r3, #0x260
007ea5a4: ldr      r3, [r2, r3]
007ea5a8: cmp      r3, #0
007ea5ac: beq      #0x7ea5d4
007ea5b0: mov      r0, r3
007ea5b4: mov      r2, r5
007ea5b8: ldr      r3, [r3]
007ea5bc: mov      r1, r6
007ea5c0: mov      lr, pc
007ea5c4: ldr      pc, [r3, #8]
007ea5c8: cmp      r0, #0
007ea5cc: ldrne    r2, [r4, #4]
007ea5d0: beq      #0x7ea590
007ea5d4: mov      r0, r6
007ea5d8: mov      r1, r5
007ea5dc: bl       #0x7e9b88
007ea5e0: cmp      r0, #0
007ea5e4: beq      #0x7ea590
007ea5e8: ldr      r2, [r0, #0x34]
007ea5ec: ldr      r3, [r0, #0x38]
007ea5f0: mov      r1, #0
007ea5f4: ldr      r2, [r2, #0xc]
007ea5f8: ldr      r3, [r3, #0xc]
007ea5fc: str      r1, [r0, #0xc]
007ea600: ldr      ip, [r4, #4]
007ea604: mov      r1, #0x19000
007ea608: add      r1, r1, #0x238
007ea60c: ldr      ip, [ip, r1]
007ea610: str      ip, [r0, #0x10]
007ea614: ldr      ip, [r4, #4]
007ea618: ldr      r1, [ip, r1]
007ea61c: cmp      r1, #0
007ea620: strne    r0, [r1, #0xc]
007ea624: ldrne    ip, [r4, #4]
007ea628: mov      r1, #0x19000
007ea62c: add      r1, r1, #0x238
007ea630: str      r0, [ip, r1]
007ea634: mov      r1, #0
007ea638: str      r3, [r0, #0x14]
007ea63c: str      r1, [r0, #0x1c]
007ea640: str      r0, [r0, #0x18]
007ea644: ldr      r1, [r2, #0x70]
007ea648: str      r1, [r0, #0x20]
007ea64c: ldr      ip, [r2, #0x70]
007ea650: add      r1, r0, #0x14
007ea654: cmp      ip, #0
007ea658: strne    r1, [ip, #8]
007ea65c: str      r1, [r2, #0x70]
007ea660: str      r2, [r0, #0x24]
007ea664: mov      r2, #0
007ea668: str      r2, [r0, #0x2c]
007ea66c: str      r0, [r0, #0x28]
007ea670: ldr      r2, [r3, #0x70]
007ea674: str      r2, [r0, #0x30]
007ea678: ldr      r1, [r3, #0x70]
007ea67c: add      r2, r0, #0x24
007ea680: cmp      r1, #0
007ea684: strne    r2, [r1, #8]
007ea688: str      r2, [r3, #0x70]
007ea68c: ldr      r2, [r4, #4]
007ea690: mov      r3, #0x19000
007ea694: add      r3, r3, #0x240
007ea698: ldr      r1, [r2, r3]
007ea69c: add      r1, r1, #1
007ea6a0: str      r1, [r2, r3]
007ea6a4: pop      {r4, r5, r6, pc}

# _ZNK12b2MouseJoint17GetReactionTorqueEv
007eb830: mov      r0, #0
007eb834: bx       lr

# _ZNK14b2PolygonShape9TestPointERK7b2XFormRK6b2Vec2
007e4670: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e4674: ldr      r4, [r0, #0x118]
007e4678: mov      r5, r0
007e467c: ldr      fp, [r2, #4]
007e4680: cmp      r4, #0
007e4684: ldr      r0, [r2]
007e4688: sub      sp, sp, #0xc
007e468c: ldr      r3, [r1, #0x14]
007e4690: ldr      r2, [r1]
007e4694: ldr      sb, [r1, #4]
007e4698: ldr      sl, [r1, #8]
007e469c: ldr      r8, [r1, #0xc]
007e46a0: ldr      r7, [r1, #0x10]
007e46a4: ble      #0x7e47d4
007e46a8: mov      r1, r2
007e46ac: str      r3, [sp, #4]
007e46b0: bl       #0x30e3ac
007e46b4: mov      r1, sb
007e46b8: mov      r6, r0
007e46bc: mov      r0, fp
007e46c0: bl       #0x30e3ac
007e46c4: mov      r1, sl
007e46c8: mov      sb, r0
007e46cc: mov      r0, r6
007e46d0: bl       #0x30ed6c
007e46d4: mov      r1, r8
007e46d8: mov      sl, r0
007e46dc: mov      r0, sb
007e46e0: bl       #0x30ed6c
007e46e4: mov      r1, r0
007e46e8: mov      r0, sl
007e46ec: bl       #0x30eba4
007e46f0: mov      r1, r7
007e46f4: mov      r8, r0
007e46f8: mov      r0, r6
007e46fc: bl       #0x30ed6c
007e4700: ldr      r3, [sp, #4]
007e4704: mov      r6, r0
007e4708: mov      r0, sb
007e470c: mov      r1, r3
007e4710: bl       #0x30ed6c
007e4714: mov      r1, r0
007e4718: mov      r0, r6
007e471c: bl       #0x30eba4
007e4720: ldr      r1, [r5, #0x58]
007e4724: mov      sl, r0
007e4728: mov      r0, r8
007e472c: bl       #0x30e3ac
007e4730: ldr      r1, [r5, #0x98]
007e4734: bl       #0x30ed6c
007e4738: ldr      r1, [r5, #0x5c]
007e473c: mov      r6, r0
007e4740: mov      r0, sl
007e4744: bl       #0x30e3ac
007e4748: ldr      r1, [r5, #0x9c]
007e474c: bl       #0x30ed6c
007e4750: mov      r1, r0
007e4754: mov      r0, r6
007e4758: bl       #0x30eba4
007e475c: mov      r1, #0
007e4760: bl       #0x30e2f8
007e4764: subs     r6, r0, #0
007e4768: beq      #0x7e47c4
007e476c: b        #0x7e47e0
007e4770: ldr      r1, [r5, #0x60]
007e4774: bl       #0x30e3ac
007e4778: ldr      r1, [r5, #0xa0]
007e477c: bl       #0x30ed6c
007e4780: ldr      r1, [r5, #0x64]
007e4784: mov      r7, r0
007e4788: mov      r0, sl
007e478c: bl       #0x30e3ac
007e4790: ldr      r1, [r5, #0xa4]
007e4794: bl       #0x30ed6c
007e4798: mov      r1, r0
007e479c: mov      r0, r7
007e47a0: bl       #0x30eba4
007e47a4: mov      r1, #0
007e47a8: bl       #0x30e2f8
007e47ac: cmp      r0, #0
007e47b0: mov      r3, #0
007e47b4: movne    r3, #1
007e47b8: tst      r3, #0xff
007e47bc: add      r5, r5, #8
007e47c0: bne      #0x7e47e0
007e47c4: add      r6, r6, #1
007e47c8: cmp      r6, r4
007e47cc: mov      r0, r8
007e47d0: bne      #0x7e4770
007e47d4: mov      r0, #1
007e47d8: add      sp, sp, #0xc
007e47dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e47e0: mov      r0, #0
007e47e4: b        #0x7e47d8

# _ZNK16b2PrismaticJoint17GetReactionTorqueEv
007eeaac: ldr      r0, [r0, #0x8c]
007eeab0: bx       lr

# _ZN12b2BroadPhase5QueryEPiS0_ttP7b2Boundii
007e2820: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e2824: sub      sp, sp, #0xc
007e2828: ldr      ip, [sp, #0x38]
007e282c: str      r1, [sp, #4]
007e2830: mov      r4, r0
007e2834: subs     r1, ip, #1
007e2838: movmi    r6, #0
007e283c: mov      r5, r2
007e2840: ldr      sl, [sp, #0x34]
007e2844: ldr      sb, [sp, #0x3c]
007e2848: ldrh     r8, [sp, #0x30]
007e284c: movmi    r7, r6
007e2850: bmi      #0x7e297c
007e2854: mov      r0, r1
007e2858: mov      r7, #0
007e285c: mov      r6, #6
007e2860: add      r2, r0, r7
007e2864: asr      r2, r2, #1
007e2868: mul      ip, r6, r2
007e286c: ldrh     ip, [sl, ip]
007e2870: cmp      r3, ip
007e2874: sublo    r0, r2, #1
007e2878: blo      #0x7e2884
007e287c: bls      #0x7e2990
007e2880: add      r7, r2, #1
007e2884: cmp      r7, r0
007e2888: ble      #0x7e2860
007e288c: mov      r6, #0
007e2890: mov      r0, #6
007e2894: add      r3, r1, r6
007e2898: asr      r3, r3, #1
007e289c: mul      r2, r0, r3
007e28a0: ldrh     r2, [sl, r2]
007e28a4: cmp      r8, r2
007e28a8: sublo    r1, r3, #1
007e28ac: blo      #0x7e28b8
007e28b0: bls      #0x7e2998
007e28b4: add      r6, r3, #1
007e28b8: cmp      r1, r6
007e28bc: bge      #0x7e2894
007e28c0: cmp      r7, r6
007e28c4: bge      #0x7e290c
007e28c8: mov      r8, #6
007e28cc: mla      r8, r8, r7, sl
007e28d0: mov      fp, r7
007e28d4: b        #0x7e28e4
007e28d8: cmp      fp, r6
007e28dc: add      r8, r8, #6
007e28e0: beq      #0x7e290c
007e28e4: ldrh     r3, [r8]
007e28e8: add      fp, fp, #1
007e28ec: tst      r3, #1
007e28f0: bne      #0x7e28d8
007e28f4: ldrh     r1, [r8, #2]
007e28f8: mov      r0, r4
007e28fc: bl       #0x7e27c0
007e2900: cmp      fp, r6
007e2904: add      r8, r8, #6
007e2908: bne      #0x7e28e4
007e290c: cmp      r7, #0
007e2910: ble      #0x7e297c
007e2914: sub      r3, r7, #1
007e2918: mov      r2, #6
007e291c: mla      sl, r2, r3, sl
007e2920: ldrh     r8, [sl, #4]
007e2924: cmp      r8, #0
007e2928: bne      #0x7e293c
007e292c: b        #0x7e297c
007e2930: cmp      r8, #0
007e2934: sub      sl, sl, #6
007e2938: beq      #0x7e297c
007e293c: ldrh     r3, [sl]
007e2940: tst      r3, #1
007e2944: bne      #0x7e2930
007e2948: ldrh     r1, [sl, #2]
007e294c: add      r3, sb, r1, lsl #3
007e2950: add      r3, r3, #0x24000
007e2954: add      r3, r4, r3, lsl #1
007e2958: ldrh     r3, [r3, #0x18]
007e295c: cmp      r3, r7
007e2960: blt      #0x7e2930
007e2964: mov      r0, r4
007e2968: sub      r8, r8, #1
007e296c: bl       #0x7e27c0
007e2970: cmp      r8, #0
007e2974: sub      sl, sl, #6
007e2978: bne      #0x7e293c
007e297c: ldr      r3, [sp, #4]
007e2980: str      r7, [r3]
007e2984: str      r6, [r5]
007e2988: add      sp, sp, #0xc
007e298c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e2990: uxth     r7, r2
007e2994: b        #0x7e288c
007e2998: uxth     r6, r3
007e299c: b        #0x7e28c0

# _ZN22b2PolyAndCircleContact12GetManifoldsEv
007ebeb4: add      r0, r0, #0x48
007ebeb8: bx       lr

# _ZN12b2BroadPhase21IncrementOverlapCountEi
007e27c0: add      r3, r0, r1, lsl #4
007e27c4: add      r3, r3, #0x48000
007e27c8: mov      r2, #0x5d000
007e27cc: add      r3, r3, #0x18
007e27d0: add      r2, r2, #0x38
007e27d4: ldrh     r2, [r0, r2]
007e27d8: ldrh     ip, [r3, #6]
007e27dc: cmp      ip, r2
007e27e0: strhlo   r2, [r3, #6]
007e27e4: movlo    r2, #1
007e27e8: strhlo   r2, [r3, #4]
007e27ec: bxlo     lr
007e27f0: mov      r2, #0x5d000
007e27f4: mov      ip, #2
007e27f8: strh     ip, [r3, #4]
007e27fc: add      r2, r2, #0x18
007e2800: ldr      r3, [r0, r2]
007e2804: add      r3, r3, #0x2e000
007e2808: add      r3, r0, r3, lsl #1
007e280c: strh     r1, [r3, #0x16]
007e2810: ldr      r3, [r0, r2]
007e2814: add      r3, r3, #1
007e2818: str      r3, [r0, r2]
007e281c: bx       lr

# _ZNK15b2RevoluteJoint14GetMotorTorqueEv
007f2b90: ldr      r0, [r0, #0x5c]
007f2b94: bx       lr

# _ZN15b2RevoluteJointC1EPK18b2RevoluteJointDef
007f336c: push     {r4, r5, r6, lr}
007f3370: ldr      r6, [pc, #0x94]
007f3374: mov      r4, r0
007f3378: mov      r5, r1
007f337c: bl       #0x7eb1e0
007f3380: ldr      r2, [pc, #0x88]
007f3384: add      r6, pc, r6
007f3388: mov      r3, #0
007f338c: ldr      r2, [r6, r2]
007f3390: mov      r0, r4
007f3394: add      r2, r2, #8
007f3398: str      r2, [r4]
007f339c: ldr      r2, [r5, #0x14]
007f33a0: str      r2, [r4, #0x44]
007f33a4: ldr      r2, [r5, #0x18]
007f33a8: str      r2, [r4, #0x48]
007f33ac: ldr      r2, [r5, #0x1c]
007f33b0: str      r2, [r4, #0x4c]
007f33b4: ldr      r2, [r5, #0x20]
007f33b8: str      r2, [r4, #0x50]
007f33bc: ldr      r2, [r5, #0x24]
007f33c0: str      r3, [r4, #0x64]
007f33c4: str      r3, [r4, #0x54]
007f33c8: str      r2, [r4, #0x8c]
007f33cc: str      r3, [r4, #0x58]
007f33d0: str      r3, [r4, #0x5c]
007f33d4: str      r3, [r4, #0x60]
007f33d8: ldr      r3, [r5, #0x2c]
007f33dc: str      r3, [r4, #0x90]
007f33e0: ldr      r3, [r5, #0x30]
007f33e4: str      r3, [r4, #0x94]
007f33e8: ldr      r3, [r5, #0x3c]
007f33ec: str      r3, [r4, #0x80]
007f33f0: ldr      r3, [r5, #0x38]
007f33f4: str      r3, [r4, #0x84]
007f33f8: ldrb     r3, [r5, #0x28]
007f33fc: strb     r3, [r4, #0x88]
007f3400: ldrb     r3, [r5, #0x34]
007f3404: strb     r3, [r4, #0x7c]
007f3408: pop      {r4, r5, r6, pc}
007f340c: andseq   r1, sl, ip, lsl #14
007f3410: strdeq   r3, r4, [r0], -r4

# _ZN7b2World16SetContactFilterEP15b2ContactFilter
007e65f4: mov      r3, #0x19000
007e65f8: add      r3, r3, #0x260
007e65fc: str      r1, [r0, r3]
007e6600: bx       lr

# _ZNK13b2PulleyJoint10GetLength2Ev
007f07c0: push     {r4, r5, r6, r7, r8, lr}
007f07c4: ldr      r5, [r0, #0x34]
007f07c8: ldr      r7, [r0, #0x60]
007f07cc: mov      r4, r0
007f07d0: ldr      r6, [r0, #0x64]
007f07d4: ldr      r1, [r5, #0xc]
007f07d8: mov      r0, r7
007f07dc: bl       #0x30ed6c
007f07e0: ldr      r1, [r5, #0x14]
007f07e4: mov      r8, r0
007f07e8: mov      r0, r6
007f07ec: bl       #0x30ed6c
007f07f0: mov      r1, r0
007f07f4: mov      r0, r8
007f07f8: bl       #0x30eba4
007f07fc: ldr      r1, [r5, #0x10]
007f0800: mov      r8, r0
007f0804: mov      r0, r7
007f0808: bl       #0x30ed6c
007f080c: ldr      r1, [r5, #0x18]
007f0810: mov      r7, r0
007f0814: mov      r0, r6
007f0818: bl       #0x30ed6c
007f081c: mov      r1, r0
007f0820: mov      r0, r7
007f0824: bl       #0x30eba4
007f0828: ldr      r1, [r5, #4]
007f082c: mov      r6, r0
007f0830: mov      r0, r8
007f0834: bl       #0x30eba4
007f0838: ldr      r1, [r5, #8]
007f083c: mov      r8, r0
007f0840: mov      r0, r6
007f0844: bl       #0x30eba4
007f0848: ldr      r5, [r4, #0x44]
007f084c: ldr      r1, [r4, #0x50]
007f0850: mov      r6, r0
007f0854: ldr      r0, [r5, #4]
007f0858: bl       #0x30eba4
007f085c: ldr      r1, [r4, #0x54]
007f0860: mov      r7, r0
007f0864: ldr      r0, [r5, #8]
007f0868: bl       #0x30eba4
007f086c: mov      r1, r7
007f0870: mov      r5, r0
007f0874: mov      r0, r8
007f0878: bl       #0x30e3ac
007f087c: mov      r1, r5
007f0880: mov      r4, r0
007f0884: mov      r0, r6
007f0888: bl       #0x30e3ac
007f088c: mov      r1, r4
007f0890: mov      r5, r0
007f0894: mov      r0, r4
007f0898: bl       #0x30ed6c
007f089c: mov      r1, r5
007f08a0: mov      r4, r0
007f08a4: mov      r0, r5
007f08a8: bl       #0x30ed6c
007f08ac: mov      r1, r0
007f08b0: mov      r0, r4
007f08b4: bl       #0x30eba4
007f08b8: pop      {r4, r5, r6, r7, r8, lr}
007f08bc: b        #0x30e124

# _ZN6b2Body17SynchronizeShapesEv
007e20ac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e20b0: ldr      r6, [r0, #0x34]
007e20b4: sub      sp, sp, #0x18
007e20b8: mov      r5, r0
007e20bc: mov      r0, r6
007e20c0: bl       #0x30e754
007e20c4: mov      r4, r0
007e20c8: mov      r0, r6
007e20cc: bl       #0x30eb08
007e20d0: ldr      r8, [r5, #0x1c]
007e20d4: mov      r6, r0
007e20d8: add      sl, r0, #0x80000000
007e20dc: mov      r1, r4
007e20e0: mov      r0, r8
007e20e4: ldr      r7, [r5, #0x20]
007e20e8: str      r4, [sp, #8]
007e20ec: str      sl, [sp, #0x10]
007e20f0: str      r6, [sp, #0xc]
007e20f4: str      r4, [sp, #0x14]
007e20f8: bl       #0x30ed6c
007e20fc: mov      r1, sl
007e2100: mov      sb, r0
007e2104: mov      r0, r7
007e2108: bl       #0x30ed6c
007e210c: mov      r1, r0
007e2110: mov      r0, sb
007e2114: bl       #0x30eba4
007e2118: mov      r1, r6
007e211c: mov      sl, r0
007e2120: mov      r0, r8
007e2124: bl       #0x30ed6c
007e2128: mov      r1, r4
007e212c: mov      r6, r0
007e2130: mov      r0, r7
007e2134: bl       #0x30ed6c
007e2138: mov      r1, r0
007e213c: mov      r0, r6
007e2140: bl       #0x30eba4
007e2144: mov      r1, sl
007e2148: mov      r4, r0
007e214c: ldr      r0, [r5, #0x24]
007e2150: bl       #0x30e3ac
007e2154: mov      r1, r4
007e2158: mov      r6, r0
007e215c: ldr      r0, [r5, #0x28]
007e2160: bl       #0x30e3ac
007e2164: ldr      r4, [r5, #0x64]
007e2168: str      r0, [sp, #4]
007e216c: str      r6, [sp]
007e2170: cmp      r4, #0
007e2174: beq      #0x7e220c
007e2178: mov      r6, #0x19000
007e217c: add      r6, r6, #0x1d8
007e2180: add      r7, r5, #4
007e2184: mov      r8, sp
007e2188: b        #0x7e2198
007e218c: ldr      r4, [r4, #8]
007e2190: cmp      r4, #0
007e2194: beq      #0x7e220c
007e2198: ldr      r3, [r5, #0x58]
007e219c: mov      r0, r4
007e21a0: mov      r2, sp
007e21a4: ldr      r1, [r3, r6]
007e21a8: mov      r3, r7
007e21ac: bl       #0x7e63d0
007e21b0: cmp      r0, #0
007e21b4: bne      #0x7e218c
007e21b8: ldr      r4, [r5, #0x64]
007e21bc: ldrh     r2, [r5]
007e21c0: mov      r3, #0
007e21c4: cmp      r4, #0
007e21c8: orr      r2, r2, #2
007e21cc: movne    r6, #0x19000
007e21d0: strh     r2, [r5]
007e21d4: str      r3, [r5, #0x48]
007e21d8: str      r3, [r5, #0x40]
007e21dc: str      r3, [r5, #0x44]
007e21e0: addne    r6, r6, #0x1d8
007e21e4: beq      #0x7e2204
007e21e8: ldr      r3, [r5, #0x58]
007e21ec: mov      r0, r4
007e21f0: ldr      r1, [r3, r6]
007e21f4: bl       #0x7e6180
007e21f8: ldr      r4, [r4, #8]
007e21fc: cmp      r4, #0
007e2200: bne      #0x7e21e8
007e2204: mov      r0, #0
007e2208: b        #0x7e2210
007e220c: mov      r0, #1
007e2210: add      sp, sp, #0x18
007e2214: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN9b2Contact6UpdateEP17b2ContactListener
007e9d24: push     {r4, r5, r6, lr}
007e9d28: mov      r4, r0
007e9d2c: ldr      r3, [r0]
007e9d30: ldr      r5, [r0, #8]
007e9d34: mov      lr, pc
007e9d38: ldr      pc, [r3, #0xc]
007e9d3c: ldr      r0, [r4, #8]
007e9d40: cmp      r5, #0
007e9d44: movle    r1, #0
007e9d48: movgt    r1, #1
007e9d4c: ldr      r3, [r4, #0x34]
007e9d50: cmp      r0, #0
007e9d54: ldr      r2, [r4, #0x38]
007e9d58: movne    r1, #0
007e9d5c: cmp      r1, #0
007e9d60: ldr      r3, [r3, #0xc]
007e9d64: ldr      r2, [r2, #0xc]
007e9d68: beq      #0x7e9d90
007e9d6c: ldrh     r0, [r3]
007e9d70: mov      r1, #0
007e9d74: str      r1, [r3, #0x8c]
007e9d78: bic      r0, r0, #8
007e9d7c: strh     r0, [r3]
007e9d80: ldrh     r0, [r2]
007e9d84: str      r1, [r2, #0x8c]
007e9d88: bic      r1, r0, #8
007e9d8c: strh     r1, [r2]
007e9d90: ldrsh    r1, [r3, #2]
007e9d94: cmp      r1, #0
007e9d98: bne      #0x7e9dac
007e9d9c: ldr      r3, [r4, #4]
007e9da0: bic      r3, r3, #2
007e9da4: str      r3, [r4, #4]
007e9da8: pop      {r4, r5, r6, pc}
007e9dac: ldrh     r3, [r3]
007e9db0: tst      r3, #0x20
007e9db4: bne      #0x7e9d9c
007e9db8: ldrsh    r3, [r2, #2]
007e9dbc: cmp      r3, #0
007e9dc0: beq      #0x7e9d9c
007e9dc4: ldrh     r3, [r2]
007e9dc8: tst      r3, #0x20
007e9dcc: bne      #0x7e9d9c
007e9dd0: ldr      r3, [r4, #4]
007e9dd4: orr      r3, r3, #2
007e9dd8: str      r3, [r4, #4]
007e9ddc: pop      {r4, r5, r6, pc}

# _ZN16b2ContactManager7CollideEv
007ea010: push     {r4, r5, r6, lr}
007ea014: ldr      r2, [r0, #4]
007ea018: mov      r3, #0x19000
007ea01c: add      r3, r3, #0x238
007ea020: ldr      r4, [r2, r3]
007ea024: mov      r6, r0
007ea028: cmp      r4, #0
007ea02c: beq      #0x7ea080
007ea030: mov      r5, #0x19000
007ea034: add      r5, r5, #0x264
007ea038: ldr      r3, [r4, #0x34]
007ea03c: ldr      r2, [r4, #0x38]
007ea040: mov      r0, r4
007ea044: ldr      r3, [r3, #0xc]
007ea048: ldr      r2, [r2, #0xc]
007ea04c: ldrh     r3, [r3]
007ea050: tst      r3, #8
007ea054: beq      #0x7ea064
007ea058: ldrh     r3, [r2]
007ea05c: tst      r3, #8
007ea060: bne      #0x7ea070
007ea064: ldr      r3, [r6, #4]
007ea068: ldr      r1, [r3, r5]
007ea06c: bl       #0x7e9d24
007ea070: ldr      r4, [r4, #0x10]
007ea074: cmp      r4, #0
007ea078: bne      #0x7ea038
007ea07c: pop      {r4, r5, r6, pc}
007ea080: pop      {r4, r5, r6, pc}

# _ZN13b2PairManager10RemovePairEii
007e4084: cmp      r1, r2
007e4088: movgt    r3, r1
007e408c: movgt    r1, r2
007e4090: movgt    r2, r3
007e4094: orr      r3, r1, r2, lsl #16
007e4098: mvn      ip, r3
007e409c: add      r3, ip, r3, lsl #15
007e40a0: movw     ip, #0x809
007e40a4: eor      r3, r3, r3, lsr #12
007e40a8: push     {r4, r5, r6, r7, r8, sb, sl}
007e40ac: add      r3, r3, r3, lsl #2
007e40b0: movw     sb, #0xffff
007e40b4: eor      r3, r3, r3, lsr #4
007e40b8: mul      r3, ip, r3
007e40bc: eor      r3, r3, r3, lsr #16
007e40c0: lsl      r3, r3, #0x12
007e40c4: lsr      r3, r3, #0x12
007e40c8: add      sl, r3, #0x20000
007e40cc: add      sl, sl, #8
007e40d0: add      sl, r0, sl, lsl #1
007e40d4: ldrh     r3, [sl, #4]
007e40d8: cmp      r3, sb
007e40dc: beq      #0x7e417c
007e40e0: add      sl, sl, #4
007e40e4: mov      r6, #0xc
007e40e8: b        #0x7e40fc
007e40ec: ldrh     r3, [r8, #0x10]
007e40f0: add      sl, r0, ip
007e40f4: cmp      r3, sb
007e40f8: beq      #0x7e417c
007e40fc: mla      r5, r6, r3, r0
007e4100: mul      ip, r6, r3
007e4104: add      r4, r5, #8
007e4108: ldrh     r7, [r4, #4]
007e410c: mov      r8, r5
007e4110: add      ip, ip, #0x10
007e4114: cmp      r1, r7
007e4118: bne      #0x7e40ec
007e411c: ldrh     r7, [r4, #6]
007e4120: cmp      r2, r7
007e4124: bne      #0x7e40ec
007e4128: ldrh     r2, [r5, #0x10]
007e412c: mov      r1, #0x30000
007e4130: add      r1, r1, #8
007e4134: strh     r2, [sl]
007e4138: ldrh     r6, [r0, r1]
007e413c: ldr      ip, [r4]
007e4140: mov      r2, #0x30000
007e4144: strh     r6, [r5, #0x10]
007e4148: mov      r6, #0
007e414c: str      r6, [r4]
007e4150: mvn      r6, #0
007e4154: strh     r6, [r4, #4]
007e4158: strh     r6, [r4, #6]
007e415c: mov      r4, #0
007e4160: strh     r4, [r5, #0x12]
007e4164: add      r2, r2, #0xc
007e4168: strh     r3, [r0, r1]
007e416c: ldr      r3, [r0, r2]
007e4170: sub      r3, r3, #1
007e4174: str      r3, [r0, r2]
007e4178: b        #0x7e4180
007e417c: mov      ip, #0
007e4180: mov      r0, ip
007e4184: pop      {r4, r5, r6, r7, r8, sb, sl}
007e4188: bx       lr

# _ZN7b2WorldC2ERK6b2AABBRK6b2Vec2b
007e8288: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e828c: mov      r4, r0
007e8290: sub      sp, sp, #0x44
007e8294: mov      r7, r2
007e8298: stmib    sp, {r1, r3}
007e829c: ldr      r8, [pc, #0x1a4]
007e82a0: bl       #0x7e8efc
007e82a4: add      r0, r4, #0x44
007e82a8: bl       #0x7f3560
007e82ac: ldr      fp, [pc, #0x198]
007e82b0: ldr      lr, [pc, #0x198]
007e82b4: ldr      r3, [pc, #0x198]
007e82b8: add      r8, pc, r8
007e82bc: ldr      fp, [r8, fp]
007e82c0: ldr      lr, [r8, lr]
007e82c4: ldr      r3, [r8, r3]
007e82c8: mov      r5, #0x19000
007e82cc: add      sb, r5, #0x1dc
007e82d0: add      ip, r5, #0x1e4
007e82d4: add      fp, fp, #8
007e82d8: add      lr, lr, #8
007e82dc: str      r3, [sp, #0xc]
007e82e0: str      fp, [r4, sb]
007e82e4: str      lr, [r4, ip]
007e82e8: ldr      lr, [sp, #0xc]
007e82ec: add      ip, ip, #0x7c
007e82f0: mov      r6, #0
007e82f4: str      lr, [r4, ip]
007e82f8: add      lr, r5, #0x21c
007e82fc: sub      ip, ip, #0x48
007e8300: str      r6, [r4, ip]
007e8304: str      r6, [r4, lr]
007e8308: add      ip, ip, #0x14
007e830c: add      lr, lr, #0x3c
007e8310: strb     r6, [r4, ip]
007e8314: str      r6, [r4, lr]
007e8318: add      ip, ip, #0x30
007e831c: add      lr, lr, #0xc
007e8320: str      r6, [r4, ip]
007e8324: str      r6, [r4, lr]
007e8328: add      ip, ip, #0xc
007e832c: sub      lr, lr, #0x34
007e8330: str      r6, [r4, ip]
007e8334: str      r6, [r4, lr]
007e8338: sub      ip, ip, #0x30
007e833c: add      lr, lr, #4
007e8340: str      r6, [r4, ip]
007e8344: movw     r1, #0x9275
007e8348: str      r6, [r4, lr]
007e834c: add      ip, ip, #4
007e8350: add      lr, lr, #0xc
007e8354: movw     r2, #0x9276
007e8358: str      r6, [r4, ip]
007e835c: add      r3, r5, #0x1e0
007e8360: str      r6, [r4, lr]
007e8364: mov      sl, #1
007e8368: movt     r1, #1
007e836c: movt     r2, #1
007e8370: add      ip, ip, #8
007e8374: add      lr, lr, #0x34
007e8378: str      r6, [r4, ip]
007e837c: strb     sl, [r4, lr]
007e8380: str      r6, [r4, r3]
007e8384: strb     sl, [r4, r1]
007e8388: strb     sl, [r4, r2]
007e838c: ldr      r2, [sp, #8]
007e8390: add      r0, r5, #0x250
007e8394: add      ip, r5, #0x1d4
007e8398: strb     r2, [r4, r0]
007e839c: ldr      r1, [r7]
007e83a0: mov      r2, r4
007e83a4: add      r0, r5, #0x248
007e83a8: str      r1, [r2, r0]!
007e83ac: ldr      lr, [r7, #4]
007e83b0: mov      r0, #0x5d000
007e83b4: mov      r7, #0
007e83b8: add      r1, r5, #0x26c
007e83bc: add      r0, r0, #0x3c
007e83c0: str      lr, [r2, #4]
007e83c4: strb     r6, [r4, ip]
007e83c8: str      r7, [r4, r1]
007e83cc: str      r4, [r4, r3]
007e83d0: bl       #0x7f34f4
007e83d4: add      r2, r4, r5
007e83d8: ldr      r1, [sp, #4]
007e83dc: add      r2, r2, #0x1dc
007e83e0: mov      sb, r0
007e83e4: bl       #0x7e3048
007e83e8: add      r3, r5, #0x1d8
007e83ec: str      sb, [r4, r3]
007e83f0: mov      r0, r4
007e83f4: add      r1, sp, #0x14
007e83f8: str      r7, [sp, #0x38]
007e83fc: strb     sl, [sp, #0x3c]
007e8400: strb     r6, [sp, #0x3f]
007e8404: str      r7, [sp, #0x18]
007e8408: str      r7, [sp, #0x1c]
007e840c: str      r7, [sp, #0x14]
007e8410: str      r7, [sp, #0x20]
007e8414: str      r6, [sp, #0x24]
007e8418: str      r7, [sp, #0x28]
007e841c: str      r7, [sp, #0x2c]
007e8420: str      r7, [sp, #0x30]
007e8424: str      r7, [sp, #0x34]
007e8428: strb     r6, [sp, #0x3d]
007e842c: strb     r6, [sp, #0x3e]
007e8430: bl       #0x7e7ef8
007e8434: add      r5, r5, #0x254
007e8438: str      r0, [r4, r5]
007e843c: mov      r0, r4
007e8440: add      sp, sp, #0x44
007e8444: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e8448: ldrsbeq  ip, [sl], -r8
007e844c: andeq    r2, r0, r8, lsl #1
007e8450: andeq    r2, r0, r8, lsl #19
007e8454: andeq    r1, r0, r0, asr #1

# _ZN9b2Contact19InitializeRegistersEv
007e9b08: push     {r4, lr}
007e9b0c: ldr      r4, [pc, #0x58]
007e9b10: ldr      r3, [pc, #0x58]
007e9b14: mov      r2, #0
007e9b18: add      r4, pc, r4
007e9b1c: ldr      r0, [r4, r3]
007e9b20: ldr      r3, [pc, #0x4c]
007e9b24: ldr      r1, [r4, r3]
007e9b28: mov      r3, r2
007e9b2c: bl       #0x7e9a9c
007e9b30: ldr      r3, [pc, #0x40]
007e9b34: mov      r2, #1
007e9b38: ldr      r0, [r4, r3]
007e9b3c: ldr      r3, [pc, #0x38]
007e9b40: ldr      r1, [r4, r3]
007e9b44: mov      r3, #0
007e9b48: bl       #0x7e9a9c
007e9b4c: ldr      r3, [pc, #0x2c]
007e9b50: mov      r2, #1
007e9b54: ldr      r0, [r4, r3]
007e9b58: ldr      r3, [pc, #0x24]
007e9b5c: ldr      r1, [r4, r3]
007e9b60: mov      r3, r2
007e9b64: pop      {r4, lr}
007e9b68: b        #0x7e9a9c
007e9b6c: andseq   sl, sl, r8, ror pc
007e9b70: strdeq   r3, r4, [r0], -ip
007e9b74: andeq    r2, r0, ip, asr r4
007e9b78: andeq    r0, r0, r0, lsl #17
007e9b7c: andeq    r3, r0, ip, lsr #19
007e9b80: ldrdeq   r3, r4, [r0], -r4
007e9b84: andeq    r1, r0, r8, asr ip

# _ZN12b2BroadPhase5QueryERK6b2AABBPPvi
007e29a0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e29a4: mov      r7, #0x5d000
007e29a8: sub      sp, sp, #0x24
007e29ac: mov      r4, r0
007e29b0: add      r7, r7, #0x34
007e29b4: mov      r5, r3
007e29b8: mov      r6, r2
007e29bc: mov      r3, r1
007e29c0: add      r2, sp, #0x18
007e29c4: add      r1, sp, #0x1c
007e29c8: bl       #0x7e25b8
007e29cc: ldr      fp, [r4, r7]
007e29d0: ldrh     ip, [sp, #0x18]
007e29d4: add      r8, sp, #0x14
007e29d8: add      sl, sp, #0x10
007e29dc: add      lr, r4, #0x50000
007e29e0: add      lr, lr, #0x16
007e29e4: mov      sb, #0
007e29e8: ldrh     r3, [sp, #0x1c]
007e29ec: mov      r0, r4
007e29f0: mov      r1, r8
007e29f4: mov      r2, sl
007e29f8: lsl      fp, fp, #1
007e29fc: stm      sp, {ip, lr}
007e2a00: str      sb, [sp, #0xc]
007e2a04: str      fp, [sp, #8]
007e2a08: bl       #0x7e2820
007e2a0c: ldr      ip, [r4, r7]
007e2a10: ldrh     lr, [sp, #0x1a]
007e2a14: add      r7, r4, #0x56000
007e2a18: lsl      ip, ip, #1
007e2a1c: ldrh     r3, [sp, #0x1e]
007e2a20: mov      r1, r8
007e2a24: str      ip, [sp, #8]
007e2a28: add      r7, r7, #0x16
007e2a2c: mov      ip, #1
007e2a30: mov      r2, sl
007e2a34: mov      r0, r4
007e2a38: str      lr, [sp]
007e2a3c: str      r7, [sp, #4]
007e2a40: str      ip, [sp, #0xc]
007e2a44: bl       #0x7e2820
007e2a48: mov      r1, #0x5d000
007e2a4c: add      r1, r1, #0x18
007e2a50: ldr      r3, [r4, r1]
007e2a54: cmp      r5, sb
007e2a58: cmpgt    r3, sb
007e2a5c: movle    r3, #0
007e2a60: movgt    r3, #1
007e2a64: movle    sb, r3
007e2a68: ble      #0x7e2aa0
007e2a6c: add      r2, r4, #0x5c000
007e2a70: add      r2, r2, #0x16
007e2a74: ldrh     r3, [r2], #2
007e2a78: add      r3, r4, r3, lsl #4
007e2a7c: add      r3, r3, #0x48000
007e2a80: add      r3, r3, #0x20
007e2a84: ldr      r3, [r3]
007e2a88: str      r3, [r6, sb, lsl #2]
007e2a8c: ldr      r3, [r4, r1]
007e2a90: add      sb, sb, #1
007e2a94: cmp      r5, sb
007e2a98: cmpgt    r3, sb
007e2a9c: bgt      #0x7e2a74
007e2aa0: mov      r3, #0x5d000
007e2aa4: add      r3, r3, #0x18
007e2aa8: mov      r2, #0
007e2aac: mov      r0, r4
007e2ab0: str      r2, [r4, r3]
007e2ab4: bl       #0x7e276c
007e2ab8: mov      r0, sb
007e2abc: add      sp, sp, #0x24
007e2ac0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN9b2Contact6CreateEP7b2ShapeS1_P16b2BlockAllocator
007e9b88: push     {r4, r5, r6, r7, lr}
007e9b8c: ldr      r4, [pc, #0xf8]
007e9b90: ldr      r3, [pc, #0xf8]
007e9b94: sub      sp, sp, #0xc
007e9b98: add      r4, pc, r4
007e9b9c: ldr      r7, [r4, r3]
007e9ba0: mov      r5, r0
007e9ba4: mov      r6, r1
007e9ba8: ldrb     r3, [r7]
007e9bac: cmp      r3, #0
007e9bb0: beq      #0x7e9c74
007e9bb4: ldr      r3, [r5, #4]
007e9bb8: mov      r1, #0x18
007e9bbc: ldr      r0, [r6, #4]
007e9bc0: mul      r3, r1, r3
007e9bc4: mov      r1, #0xc
007e9bc8: mla      r1, r1, r0, r3
007e9bcc: ldr      r3, [pc, #0xc0]
007e9bd0: ldr      r0, [r4, r3]
007e9bd4: ldr      r3, [r0, r1]
007e9bd8: add      r1, r0, r1
007e9bdc: cmp      r3, #0
007e9be0: moveq    r5, r3
007e9be4: beq      #0x7e9c54
007e9be8: ldrb     r4, [r1, #8]
007e9bec: cmp      r4, #0
007e9bf0: bne      #0x7e9c60
007e9bf4: mov      r1, r5
007e9bf8: mov      r0, r6
007e9bfc: blx      r3
007e9c00: ldr      r3, [r0, #8]
007e9c04: mov      r5, r0
007e9c08: cmp      r3, #0
007e9c0c: ble      #0x7e9c54
007e9c10: mov      r6, r4
007e9c14: ldr      r3, [r5]
007e9c18: mov      r0, r5
007e9c1c: mov      lr, pc
007e9c20: ldr      pc, [r3]
007e9c24: add      r0, r0, r4
007e9c28: ldr      r2, [r0, #0x40]
007e9c2c: ldr      r3, [r0, #0x44]
007e9c30: add      r6, r6, #1
007e9c34: add      r2, r2, #0x80000000
007e9c38: add      r3, r3, #0x80000000
007e9c3c: str      r2, [r0, #0x40]
007e9c40: str      r3, [r0, #0x44]
007e9c44: ldr      r3, [r5, #8]
007e9c48: add      r4, r4, #0x4c
007e9c4c: cmp      r6, r3
007e9c50: blt      #0x7e9c14
007e9c54: mov      r0, r5
007e9c58: add      sp, sp, #0xc
007e9c5c: pop      {r4, r5, r6, r7, pc}
007e9c60: mov      r0, r5
007e9c64: mov      r1, r6
007e9c68: blx      r3
007e9c6c: mov      r5, r0
007e9c70: b        #0x7e9c54
007e9c74: str      r2, [sp, #4]
007e9c78: bl       #0x7e9b08
007e9c7c: mov      r3, #1
007e9c80: strb     r3, [r7]
007e9c84: ldr      r2, [sp, #4]
007e9c88: b        #0x7e9bb4
007e9c8c: ldrsheq  sl, [sl], -r8
007e9c90: andeq    r0, r0, r8, lsr #14
007e9c94: strdeq   r1, r2, [r0], -r4

# _ZN12b2BroadPhaseD2Ev
007e2478: bx       lr

# _ZNK16b2PrismaticJoint13GetJointSpeedEv
007eec94: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007eec98: ldr      r4, [r0, #0x30]
007eec9c: sub      sp, sp, #0x14
007eeca0: mov      r6, r0
007eeca4: ldr      r1, [r4, #0x1c]
007eeca8: ldr      r0, [r0, #0x44]
007eecac: bl       #0x30e3ac
007eecb0: ldr      r1, [r4, #0x20]
007eecb4: mov      r8, r0
007eecb8: ldr      r0, [r6, #0x48]
007eecbc: bl       #0x30e3ac
007eecc0: ldr      r2, [r4, #0xc]
007eecc4: mov      r7, r0
007eecc8: mov      r0, r8
007eeccc: mov      r1, r2
007eecd0: ldr      r5, [r6, #0x34]
007eecd4: str      r2, [sp, #4]
007eecd8: bl       #0x30ed6c
007eecdc: ldr      r1, [r4, #0x14]
007eece0: mov      sl, r0
007eece4: mov      r0, r7
007eece8: bl       #0x30ed6c
007eecec: mov      r1, r0
007eecf0: mov      r0, sl
007eecf4: bl       #0x30eba4
007eecf8: str      r0, [sp, #8]
007eecfc: ldr      r1, [r4, #0x10]
007eed00: mov      r0, r8
007eed04: bl       #0x30ed6c
007eed08: ldr      r1, [r4, #0x18]
007eed0c: mov      r8, r0
007eed10: mov      r0, r7
007eed14: bl       #0x30ed6c
007eed18: mov      r1, r0
007eed1c: mov      r0, r8
007eed20: bl       #0x30eba4
007eed24: str      r0, [sp, #0xc]
007eed28: ldr      r1, [r5, #0x1c]
007eed2c: ldr      r0, [r6, #0x4c]
007eed30: bl       #0x30e3ac
007eed34: ldr      r1, [r5, #0x20]
007eed38: mov      sl, r0
007eed3c: ldr      r0, [r6, #0x50]
007eed40: bl       #0x30e3ac
007eed44: ldr      r1, [r5, #0xc]
007eed48: mov      r8, r0
007eed4c: mov      r0, sl
007eed50: bl       #0x30ed6c
007eed54: ldr      r1, [r5, #0x14]
007eed58: mov      r7, r0
007eed5c: mov      r0, r8
007eed60: bl       #0x30ed6c
007eed64: mov      r1, r0
007eed68: mov      r0, r7
007eed6c: bl       #0x30eba4
007eed70: ldr      r1, [r5, #0x10]
007eed74: mov      r7, r0
007eed78: mov      r0, sl
007eed7c: bl       #0x30ed6c
007eed80: ldr      r1, [r5, #0x18]
007eed84: mov      sl, r0
007eed88: mov      r0, r8
007eed8c: bl       #0x30ed6c
007eed90: mov      r1, r0
007eed94: mov      r0, sl
007eed98: bl       #0x30eba4
007eed9c: ldr      r1, [r4, #0x2c]
007eeda0: mov      r8, r0
007eeda4: ldr      r0, [sp, #8]
007eeda8: bl       #0x30eba4
007eedac: ldr      r1, [r4, #0x30]
007eedb0: mov      sb, r0
007eedb4: ldr      r0, [sp, #0xc]
007eedb8: bl       #0x30eba4
007eedbc: ldr      r1, [r5, #0x2c]
007eedc0: mov      r3, r0
007eedc4: mov      r0, r7
007eedc8: str      r3, [sp]
007eedcc: bl       #0x30eba4
007eedd0: ldr      r1, [r5, #0x30]
007eedd4: mov      fp, r0
007eedd8: mov      r0, r8
007eeddc: bl       #0x30eba4
007eede0: mov      r1, sb
007eede4: mov      sl, r0
007eede8: mov      r0, fp
007eedec: bl       #0x30e3ac
007eedf0: ldr      r3, [sp]
007eedf4: mov      fp, r0
007eedf8: mov      r0, sl
007eedfc: mov      r1, r3
007eee00: bl       #0x30e3ac
007eee04: ldr      r2, [sp, #4]
007eee08: ldr      sb, [r6, #0x54]
007eee0c: mov      ip, r0
007eee10: mov      r0, r2
007eee14: mov      r1, sb
007eee18: ldr      r6, [r6, #0x58]
007eee1c: str      ip, [sp, #4]
007eee20: bl       #0x30ed6c
007eee24: mov      r1, r6
007eee28: mov      sl, r0
007eee2c: ldr      r0, [r4, #0x14]
007eee30: bl       #0x30ed6c
007eee34: mov      r1, r0
007eee38: mov      r0, sl
007eee3c: bl       #0x30eba4
007eee40: mov      r1, sb
007eee44: mov      sl, r0
007eee48: ldr      r0, [r4, #0x10]
007eee4c: bl       #0x30ed6c
007eee50: mov      r1, r6
007eee54: mov      sb, r0
007eee58: ldr      r0, [r4, #0x18]
007eee5c: bl       #0x30ed6c
007eee60: mov      r1, r0
007eee64: mov      r0, sb
007eee68: bl       #0x30eba4
007eee6c: ldr      r6, [r4, #0x48]
007eee70: str      r0, [sp]
007eee74: add      sb, r6, #0x80000000
007eee78: mov      r1, sb
007eee7c: bl       #0x30ed6c
007eee80: mov      r1, r0
007eee84: mov      r0, fp
007eee88: bl       #0x30ed6c
007eee8c: mov      r1, sl
007eee90: mov      fp, r0
007eee94: mov      r0, r6
007eee98: bl       #0x30ed6c
007eee9c: ldr      ip, [sp, #4]
007eeea0: mov      r1, r0
007eeea4: mov      r0, ip
007eeea8: bl       #0x30ed6c
007eeeac: mov      r1, r0
007eeeb0: mov      r0, fp
007eeeb4: bl       #0x30eba4
007eeeb8: ldr      r2, [r5, #0x48]
007eeebc: mov      fp, r0
007eeec0: mov      r0, r8
007eeec4: add      r1, r2, #0x80000000
007eeec8: bl       #0x30ed6c
007eeecc: mov      r1, r7
007eeed0: mov      r8, r0
007eeed4: ldr      r0, [r5, #0x48]
007eeed8: bl       #0x30ed6c
007eeedc: ldr      r1, [r5, #0x40]
007eeee0: mov      r7, r0
007eeee4: mov      r0, r8
007eeee8: bl       #0x30eba4
007eeeec: ldr      r8, [r5, #0x44]
007eeef0: mov      r1, r7
007eeef4: mov      r5, r0
007eeef8: mov      r0, r8
007eeefc: bl       #0x30eba4
007eef00: ldr      r8, [r4, #0x40]
007eef04: mov      r7, r0
007eef08: mov      r0, r5
007eef0c: mov      r1, r8
007eef10: bl       #0x30e3ac
007eef14: ldr      r5, [r4, #0x44]
007eef18: mov      r4, r0
007eef1c: mov      r0, r7
007eef20: mov      r1, r5
007eef24: bl       #0x30e3ac
007eef28: mov      r1, sb
007eef2c: mov      r5, r0
007eef30: ldr      r0, [sp, #0xc]
007eef34: bl       #0x30ed6c
007eef38: ldr      r1, [sp, #8]
007eef3c: mov      r7, r0
007eef40: mov      r0, r6
007eef44: bl       #0x30ed6c
007eef48: mov      r1, r7
007eef4c: mov      r6, r0
007eef50: mov      r0, r4
007eef54: bl       #0x30e3ac
007eef58: mov      r1, r0
007eef5c: mov      r0, sl
007eef60: bl       #0x30ed6c
007eef64: mov      r1, r6
007eef68: mov      r4, r0
007eef6c: mov      r0, r5
007eef70: bl       #0x30e3ac
007eef74: ldr      r3, [sp]
007eef78: mov      r1, r0
007eef7c: mov      r0, r3
007eef80: bl       #0x30ed6c
007eef84: mov      r1, r0
007eef88: mov      r0, r4
007eef8c: bl       #0x30eba4
007eef90: mov      r1, r0
007eef94: mov      r0, fp
007eef98: bl       #0x30eba4
007eef9c: add      sp, sp, #0x14
007eefa0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN13b2PairManagerC2Ev
007e3dbc: str      r4, [sp, #-4]!
007e3dc0: mov      r3, #0
007e3dc4: add      r2, r0, r3
007e3dc8: add      r3, r3, #2
007e3dcc: add      r2, r2, #0x40000
007e3dd0: mvn      r1, #0
007e3dd4: cmp      r3, #0x8000
007e3dd8: strh     r1, [r2, #0x14]
007e3ddc: bne      #0x7e3dc4
007e3de0: mov      r3, #0x30000
007e3de4: add      r3, r3, #8
007e3de8: mov      r2, #0
007e3dec: strh     r2, [r0, r3]
007e3df0: mov      r1, #0
007e3df4: mov      r3, r0
007e3df8: mov      r2, #1
007e3dfc: movw     ip, #0x4001
007e3e00: strh     r2, [r3, #0x10]
007e3e04: add      r2, r2, #1
007e3e08: mvn      r4, #0
007e3e0c: cmp      r2, ip
007e3e10: strh     r4, [r3, #0xc]
007e3e14: strh     r4, [r3, #0xe]
007e3e18: str      r1, [r3, #8]
007e3e1c: strh     r1, [r3, #0x12]
007e3e20: add      r3, r3, #0xc
007e3e24: bne      #0x7e3e00
007e3e28: mov      ip, #0x30000
007e3e2c: mov      r2, ip
007e3e30: mov      r3, #0x40000
007e3e34: add      ip, ip, #4
007e3e38: add      r2, r2, #0xc
007e3e3c: add      r3, r3, #0x10
007e3e40: strh     r4, [r0, ip]
007e3e44: str      r1, [r0, r2]
007e3e48: str      r1, [r0, r3]
007e3e4c: ldm      sp!, {r4}
007e3e50: bx       lr

# _ZN12b2MouseJoint24SolveVelocityConstraintsERK10b2TimeStep
007eb850: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007eb854: ldr      r5, [r0, #0x34]
007eb858: sub      sp, sp, #0x14
007eb85c: mov      r4, r0
007eb860: mov      r6, r1
007eb864: ldr      r0, [r0, #0x44]
007eb868: ldr      r1, [r5, #0x1c]
007eb86c: bl       #0x30e3ac
007eb870: ldr      r1, [r5, #0x20]
007eb874: mov      r8, r0
007eb878: ldr      r0, [r4, #0x48]
007eb87c: bl       #0x30e3ac
007eb880: ldr      r1, [r5, #0xc]
007eb884: mov      r7, r0
007eb888: mov      r0, r8
007eb88c: bl       #0x30ed6c
007eb890: ldr      r1, [r5, #0x14]
007eb894: mov      sl, r0
007eb898: mov      r0, r7
007eb89c: bl       #0x30ed6c
007eb8a0: mov      r1, r0
007eb8a4: mov      r0, sl
007eb8a8: bl       #0x30eba4
007eb8ac: str      r0, [sp, #8]
007eb8b0: ldr      r1, [r5, #0x10]
007eb8b4: mov      r0, r8
007eb8b8: bl       #0x30ed6c
007eb8bc: ldr      r1, [r5, #0x18]
007eb8c0: mov      r8, r0
007eb8c4: mov      r0, r7
007eb8c8: bl       #0x30ed6c
007eb8cc: mov      r1, r0
007eb8d0: mov      r0, r8
007eb8d4: bl       #0x30eba4
007eb8d8: str      r0, [sp, #0xc]
007eb8dc: ldr      r7, [r5, #0x48]
007eb8e0: add      r1, r7, #0x80000000
007eb8e4: bl       #0x30ed6c
007eb8e8: ldr      r1, [sp, #8]
007eb8ec: mov      r8, r0
007eb8f0: mov      r0, r7
007eb8f4: bl       #0x30ed6c
007eb8f8: ldr      r1, [r5, #0x40]
007eb8fc: mov      r7, r0
007eb900: mov      r0, r8
007eb904: bl       #0x30eba4
007eb908: ldr      r1, [r5, #0x44]
007eb90c: mov      fp, r0
007eb910: mov      r0, r7
007eb914: bl       #0x30eba4
007eb918: ldr      sl, [r6, #4]
007eb91c: mov      r8, r0
007eb920: ldr      r1, [r4, #0x78]
007eb924: mov      r0, sl
007eb928: bl       #0x30ed6c
007eb92c: ldr      r1, [r4, #0x6c]
007eb930: mov      r7, r0
007eb934: bl       #0x30ed6c
007eb938: ldr      r1, [r4, #0x70]
007eb93c: mov      sb, r0
007eb940: mov      r0, r7
007eb944: bl       #0x30ed6c
007eb948: mov      r1, sb
007eb94c: mov      r7, r0
007eb950: mov      r0, fp
007eb954: bl       #0x30eba4
007eb958: mov      r1, r7
007eb95c: mov      r2, r0
007eb960: mov      r0, r8
007eb964: str      r2, [sp, #4]
007eb968: bl       #0x30eba4
007eb96c: ldr      sb, [r4, #0x7c]
007eb970: ldr      r8, [r4, #0x54]
007eb974: mov      r3, r0
007eb978: mov      r0, sb
007eb97c: mov      r1, r8
007eb980: str      r3, [sp]
007eb984: bl       #0x30ed6c
007eb988: ldr      r7, [r4, #0x58]
007eb98c: mov      fp, r0
007eb990: mov      r0, sb
007eb994: mov      r1, r7
007eb998: bl       #0x30ed6c
007eb99c: mov      r1, fp
007eb9a0: mov      sb, r0
007eb9a4: ldr      r0, [r6]
007eb9a8: bl       #0x30ed6c
007eb9ac: mov      r1, sb
007eb9b0: mov      fp, r0
007eb9b4: ldr      r0, [r6]
007eb9b8: bl       #0x30ed6c
007eb9bc: ldr      r2, [sp, #4]
007eb9c0: mov      sb, r0
007eb9c4: mov      r1, fp
007eb9c8: mov      r0, r2
007eb9cc: bl       #0x30eba4
007eb9d0: ldr      r3, [sp]
007eb9d4: mov      fp, r0
007eb9d8: mov      r1, sb
007eb9dc: mov      r0, r3
007eb9e0: bl       #0x30eba4
007eb9e4: ldr      r1, [r4, #0x5c]
007eb9e8: mov      sb, r0
007eb9ec: mov      r0, fp
007eb9f0: bl       #0x30ed6c
007eb9f4: ldr      r1, [r4, #0x64]
007eb9f8: mov      r3, r0
007eb9fc: mov      r0, sb
007eba00: str      r3, [sp]
007eba04: bl       #0x30ed6c
007eba08: ldr      r3, [sp]
007eba0c: mov      r1, r0
007eba10: add      sl, sl, #0x80000000
007eba14: mov      r0, r3
007eba18: bl       #0x30eba4
007eba1c: ldr      r1, [r4, #0x60]
007eba20: mov      r3, r0
007eba24: mov      r0, fp
007eba28: str      r3, [sp]
007eba2c: bl       #0x30ed6c
007eba30: ldr      r1, [r4, #0x68]
007eba34: mov      fp, r0
007eba38: mov      r0, sb
007eba3c: bl       #0x30ed6c
007eba40: mov      r1, r0
007eba44: mov      r0, fp
007eba48: bl       #0x30eba4
007eba4c: ldr      r3, [sp]
007eba50: mov      sb, r0
007eba54: mov      r0, sl
007eba58: mov      r1, r3
007eba5c: bl       #0x30ed6c
007eba60: mov      r1, r0
007eba64: mov      r0, r8
007eba68: bl       #0x30eba4
007eba6c: mov      fp, r0
007eba70: mov      r1, sb
007eba74: mov      r0, sl
007eba78: str      fp, [r4, #0x54]
007eba7c: bl       #0x30ed6c
007eba80: mov      r1, r7
007eba84: bl       #0x30eba4
007eba88: mov      sl, r0
007eba8c: str      sl, [r4, #0x58]
007eba90: mov      r1, fp
007eba94: mov      r0, fp
007eba98: bl       #0x30ed6c
007eba9c: mov      r1, sl
007ebaa0: mov      sb, r0
007ebaa4: mov      r0, sl
007ebaa8: bl       #0x30ed6c
007ebaac: mov      r1, r0
007ebab0: mov      r0, sb
007ebab4: bl       #0x30eba4
007ebab8: bl       #0x30e124
007ebabc: ldr      sl, [r4, #0x74]
007ebac0: mov      r1, r0
007ebac4: mov      sb, r0
007ebac8: mov      r0, sl
007ebacc: bl       #0x30e70c
007ebad0: cmp      r0, #0
007ebad4: bne      #0x7ebba8
007ebad8: ldr      sl, [r4, #0x58]
007ebadc: ldr      sb, [r4, #0x54]
007ebae0: mov      r1, r8
007ebae4: mov      r0, sb
007ebae8: bl       #0x30e3ac
007ebaec: mov      r1, r7
007ebaf0: mov      r4, r0
007ebaf4: mov      r0, sl
007ebaf8: bl       #0x30e3ac
007ebafc: ldr      r6, [r6]
007ebb00: mov      r7, r0
007ebb04: mov      r1, r4
007ebb08: mov      r0, r6
007ebb0c: bl       #0x30ed6c
007ebb10: mov      r1, r7
007ebb14: mov      r4, r0
007ebb18: mov      r0, r6
007ebb1c: bl       #0x30ed6c
007ebb20: ldr      r7, [r5, #0x78]
007ebb24: mov      r6, r0
007ebb28: mov      r1, r4
007ebb2c: mov      r0, r7
007ebb30: bl       #0x30ed6c
007ebb34: mov      r1, r0
007ebb38: ldr      r0, [r5, #0x40]
007ebb3c: bl       #0x30eba4
007ebb40: mov      r1, r6
007ebb44: str      r0, [r5, #0x40]
007ebb48: mov      r0, r7
007ebb4c: bl       #0x30ed6c
007ebb50: mov      r1, r0
007ebb54: ldr      r0, [r5, #0x44]
007ebb58: bl       #0x30eba4
007ebb5c: str      r0, [r5, #0x44]
007ebb60: mov      r1, r6
007ebb64: ldr      r0, [sp, #8]
007ebb68: bl       #0x30ed6c
007ebb6c: mov      r1, r4
007ebb70: mov      r6, r0
007ebb74: ldr      r0, [sp, #0xc]
007ebb78: bl       #0x30ed6c
007ebb7c: mov      r1, r0
007ebb80: mov      r0, r6
007ebb84: bl       #0x30e3ac
007ebb88: ldr      r1, [r5, #0x80]
007ebb8c: bl       #0x30ed6c
007ebb90: mov      r1, r0
007ebb94: ldr      r0, [r5, #0x48]
007ebb98: bl       #0x30eba4
007ebb9c: str      r0, [r5, #0x48]
007ebba0: add      sp, sp, #0x14
007ebba4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ebba8: mov      r1, sb
007ebbac: mov      r0, sl
007ebbb0: bl       #0x30ec94
007ebbb4: ldr      r1, [r4, #0x54]
007ebbb8: mov      sl, r0
007ebbbc: bl       #0x30ed6c
007ebbc0: ldr      r1, [r4, #0x58]
007ebbc4: str      r0, [r4, #0x54]
007ebbc8: mov      sb, r0
007ebbcc: mov      r0, sl
007ebbd0: bl       #0x30ed6c
007ebbd4: mov      sl, r0
007ebbd8: str      r0, [r4, #0x58]
007ebbdc: b        #0x7ebae0

# _ZN16b2BlockAllocatorD1Ev
007e8ddc: push     {r4, r5, r6, lr}
007e8de0: ldr      r3, [r0, #4]
007e8de4: mov      r5, r0
007e8de8: cmp      r3, #0
007e8dec: ble      #0x7e8e14
007e8df0: mov      r4, #0
007e8df4: ldr      r3, [r5]
007e8df8: add      r3, r3, r4, lsl #3
007e8dfc: ldr      r0, [r3, #4]
007e8e00: bl       #0x7f34bc
007e8e04: ldr      r3, [r5, #4]
007e8e08: add      r4, r4, #1
007e8e0c: cmp      r3, r4
007e8e10: bgt      #0x7e8df4
007e8e14: ldr      r0, [r5]
007e8e18: bl       #0x7f34bc
007e8e1c: mov      r0, r5
007e8e20: pop      {r4, r5, r6, pc}

# _ZN7b2Shape6CreateEPK10b2ShapeDefP16b2BlockAllocator
007e6558: push     {r4, r5, r6, lr}
007e655c: ldr      r3, [r0, #4]
007e6560: mov      r4, r0
007e6564: cmp      r3, #0
007e6568: bne      #0x7e658c
007e656c: mov      r0, r1
007e6570: mov      r1, #0x3c
007e6574: bl       #0x7e90bc
007e6578: mov      r1, r4
007e657c: mov      r5, r0
007e6580: bl       #0x7e99b0
007e6584: mov      r0, r5
007e6588: pop      {r4, r5, r6, pc}
007e658c: cmp      r3, #1
007e6590: beq      #0x7e659c
007e6594: mov      r0, #0
007e6598: pop      {r4, r5, r6, pc}
007e659c: mov      r0, r1
007e65a0: mov      r1, #0x11c
007e65a4: bl       #0x7e90bc
007e65a8: mov      r1, r4
007e65ac: mov      r5, r0
007e65b0: bl       #0x7e5928
007e65b4: mov      r0, r5
007e65b8: pop      {r4, r5, r6, pc}

# _ZN7b2ShapeC1EPK10b2ShapeDef
007e60f0: ldr      r2, [pc, #0x78]
007e60f4: ldr      ip, [pc, #0x78]
007e60f8: str      r4, [sp, #-4]!
007e60fc: add      r2, pc, r2
007e6100: ldr      ip, [r2, ip]
007e6104: add      ip, ip, #8
007e6108: str      ip, [r0]
007e610c: ldr      r4, [r1, #8]
007e6110: mov      ip, #0
007e6114: str      r4, [r0, #0x2c]
007e6118: ldr      r2, [r1, #0xc]
007e611c: mov      r4, #0
007e6120: str      r2, [r0, #0x18]
007e6124: ldr      r2, [r1, #0x10]
007e6128: str      r2, [r0, #0x1c]
007e612c: ldr      r2, [r1, #0x14]
007e6130: str      r4, [r0, #0x10]
007e6134: str      ip, [r0, #8]
007e6138: str      r2, [r0, #0x14]
007e613c: mvn      r2, #0
007e6140: str      ip, [r0, #0xc]
007e6144: strh     r2, [r0, #0x20]
007e6148: ldrh     r2, [r1, #0x1a]
007e614c: strh     r2, [r0, #0x22]
007e6150: ldrh     r2, [r1, #0x1c]
007e6154: strh     r2, [r0, #0x24]
007e6158: ldrh     r2, [r1, #0x1e]
007e615c: strh     r2, [r0, #0x26]
007e6160: ldrb     r2, [r1, #0x18]
007e6164: strb     r2, [r0, #0x28]
007e6168: ldm      sp!, {r4}
007e616c: bx       lr
007e6170: mulseq   sl, r4, sb
007e6174: andeq    r3, r0, ip, lsr #32

# _ZNK15b2RevoluteJoint14IsLimitEnabledEv
007f2ba8: ldrb     r0, [r0, #0x88]
007f2bac: bx       lr

# _ZN13b2NullContactD1Ev
007e65d0: bx       lr

# _ZN6b2Body11CreateShapeEP10b2ShapeDef
007e1dc8: push     {r4, r5, r6, lr}
007e1dcc: ldr      r2, [r0, #0x58]
007e1dd0: mov      r3, #0x19000
007e1dd4: add      r3, r3, #0x1d4
007e1dd8: ldrb     r3, [r2, r3]
007e1ddc: mov      r4, r0
007e1de0: mov      r0, r1
007e1de4: cmp      r3, #0
007e1de8: movne    r5, #0
007e1dec: bne      #0x7e1e44
007e1df0: mov      r1, r2
007e1df4: bl       #0x7e6558
007e1df8: ldr      r2, [r4, #0x64]
007e1dfc: mov      r3, #0x19000
007e1e00: add      r3, r3, #0x1d8
007e1e04: str      r2, [r0, #8]
007e1e08: ldr      r1, [r4, #0x68]
007e1e0c: str      r0, [r4, #0x64]
007e1e10: mov      r5, r0
007e1e14: add      r1, r1, #1
007e1e18: str      r1, [r4, #0x68]
007e1e1c: str      r4, [r0, #0xc]
007e1e20: ldr      r1, [r4, #0x58]
007e1e24: add      r2, r4, #4
007e1e28: ldr      r1, [r1, r3]
007e1e2c: bl       #0x7e62d4
007e1e30: add      r1, r4, #0x1c
007e1e34: ldr      r3, [r5]
007e1e38: mov      r0, r5
007e1e3c: mov      lr, pc
007e1e40: ldr      pc, [r3, #0x1c]
007e1e44: mov      r0, r5
007e1e48: pop      {r4, r5, r6, pc}

# _ZN16b2PolygonContact7DestroyEP9b2ContactP16b2BlockAllocator
007ecb28: push     {r4, r5, r6, lr}
007ecb2c: ldr      r3, [r0]
007ecb30: mov      r5, r1
007ecb34: mov      r4, r0
007ecb38: mov      lr, pc
007ecb3c: ldr      pc, [r3, #4]
007ecb40: mov      r0, r5
007ecb44: mov      r1, r4
007ecb48: mov      r2, #0x94
007ecb4c: pop      {r4, r5, r6, lr}
007ecb50: b        #0x7e8da0

# _ZN15b2RevoluteJoint24SolvePositionConstraintsEv
007f2bec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f2bf0: ldr      r5, [r0, #0x30]
007f2bf4: sub      sp, sp, #0x2c
007f2bf8: mov      r6, r0
007f2bfc: ldr      r1, [r5, #0x1c]
007f2c00: ldr      r0, [r0, #0x44]
007f2c04: bl       #0x30e3ac
007f2c08: ldr      r1, [r5, #0x20]
007f2c0c: mov      r8, r0
007f2c10: ldr      r0, [r6, #0x48]
007f2c14: bl       #0x30e3ac
007f2c18: ldr      r1, [r5, #0xc]
007f2c1c: mov      r7, r0
007f2c20: mov      r0, r8
007f2c24: bl       #0x30ed6c
007f2c28: ldr      r1, [r5, #0x14]
007f2c2c: mov      sl, r0
007f2c30: mov      r0, r7
007f2c34: bl       #0x30ed6c
007f2c38: mov      r1, r0
007f2c3c: mov      r0, sl
007f2c40: bl       #0x30eba4
007f2c44: ldr      r1, [r5, #0x10]
007f2c48: mov      sb, r0
007f2c4c: mov      r0, r8
007f2c50: bl       #0x30ed6c
007f2c54: ldr      r1, [r5, #0x18]
007f2c58: mov      r8, r0
007f2c5c: mov      r0, r7
007f2c60: bl       #0x30ed6c
007f2c64: mov      r1, r0
007f2c68: mov      r0, r8
007f2c6c: bl       #0x30eba4
007f2c70: ldr      r4, [r6, #0x34]
007f2c74: mov      sl, r0
007f2c78: ldr      r0, [r6, #0x4c]
007f2c7c: ldr      r1, [r4, #0x1c]
007f2c80: bl       #0x30e3ac
007f2c84: ldr      r1, [r4, #0x20]
007f2c88: mov      fp, r0
007f2c8c: ldr      r0, [r6, #0x50]
007f2c90: bl       #0x30e3ac
007f2c94: ldr      r1, [r4, #0xc]
007f2c98: mov      r7, r0
007f2c9c: mov      r0, fp
007f2ca0: bl       #0x30ed6c
007f2ca4: ldr      r1, [r4, #0x14]
007f2ca8: mov      r8, r0
007f2cac: mov      r0, r7
007f2cb0: bl       #0x30ed6c
007f2cb4: mov      r1, r0
007f2cb8: mov      r0, r8
007f2cbc: bl       #0x30eba4
007f2cc0: ldr      r1, [r4, #0x10]
007f2cc4: mov      r8, r0
007f2cc8: mov      r0, fp
007f2ccc: bl       #0x30ed6c
007f2cd0: ldr      r1, [r4, #0x18]
007f2cd4: mov      fp, r0
007f2cd8: mov      r0, r7
007f2cdc: bl       #0x30ed6c
007f2ce0: mov      r1, r0
007f2ce4: mov      r0, fp
007f2ce8: bl       #0x30eba4
007f2cec: ldr      r1, [r5, #0x2c]
007f2cf0: mov      r7, r0
007f2cf4: mov      r0, sb
007f2cf8: bl       #0x30eba4
007f2cfc: ldr      r1, [r5, #0x30]
007f2d00: mov      r3, r0
007f2d04: mov      r0, sl
007f2d08: str      r3, [sp]
007f2d0c: bl       #0x30eba4
007f2d10: ldr      r1, [r4, #0x2c]
007f2d14: mov      ip, r0
007f2d18: mov      r0, r8
007f2d1c: str      ip, [sp, #8]
007f2d20: bl       #0x30eba4
007f2d24: ldr      r1, [r4, #0x30]
007f2d28: mov      fp, r0
007f2d2c: mov      r0, r7
007f2d30: bl       #0x30eba4
007f2d34: ldr      r3, [sp]
007f2d38: mov      r2, r0
007f2d3c: mov      r0, fp
007f2d40: mov      r1, r3
007f2d44: str      r2, [sp, #4]
007f2d48: bl       #0x30e3ac
007f2d4c: ldmib    sp, {r2, ip}
007f2d50: mov      r3, r0
007f2d54: mov      r1, ip
007f2d58: mov      r0, r2
007f2d5c: str      r3, [sp]
007f2d60: bl       #0x30e3ac
007f2d64: ldr      r3, [sp]
007f2d68: mov      r2, r0
007f2d6c: str      r2, [sp, #4]
007f2d70: mov      r1, r3
007f2d74: mov      r0, r3
007f2d78: bl       #0x30ed6c
007f2d7c: ldr      r2, [sp, #4]
007f2d80: mov      fp, r0
007f2d84: mov      r1, r2
007f2d88: mov      r0, r2
007f2d8c: bl       #0x30ed6c
007f2d90: mov      r1, r0
007f2d94: mov      r0, fp
007f2d98: bl       #0x30eba4
007f2d9c: bl       #0x30e124
007f2da0: str      r0, [sp, #0x20]
007f2da4: ldr      r1, [r4, #0x78]
007f2da8: ldr      r0, [r5, #0x78]
007f2dac: bl       #0x30eba4
007f2db0: str      r0, [sp, #0xc]
007f2db4: ldr      r0, [r5, #0x80]
007f2db8: mov      r1, sl
007f2dbc: bl       #0x30ed6c
007f2dc0: mov      r1, sl
007f2dc4: bl       #0x30ed6c
007f2dc8: ldr      r1, [r5, #0x80]
007f2dcc: mov      ip, r0
007f2dd0: str      ip, [sp, #8]
007f2dd4: add      r0, r1, #0x80000000
007f2dd8: mov      r1, sb
007f2ddc: bl       #0x30ed6c
007f2de0: mov      r1, sl
007f2de4: bl       #0x30ed6c
007f2de8: str      r0, [sp, #0x10]
007f2dec: ldr      r0, [r5, #0x80]
007f2df0: mov      r1, sb
007f2df4: bl       #0x30ed6c
007f2df8: mov      r1, sb
007f2dfc: bl       #0x30ed6c
007f2e00: ldr      fp, [r4, #0x80]
007f2e04: mov      r1, r7
007f2e08: str      r0, [sp, #0x14]
007f2e0c: mov      r0, fp
007f2e10: bl       #0x30ed6c
007f2e14: mov      r1, r7
007f2e18: bl       #0x30ed6c
007f2e1c: mov      r1, r8
007f2e20: str      r0, [sp, #0x18]
007f2e24: add      r0, fp, #0x80000000
007f2e28: bl       #0x30ed6c
007f2e2c: mov      r1, r7
007f2e30: bl       #0x30ed6c
007f2e34: mov      r1, r8
007f2e38: str      r0, [sp, #0x1c]
007f2e3c: mov      r0, fp
007f2e40: bl       #0x30ed6c
007f2e44: mov      r1, r8
007f2e48: bl       #0x30ed6c
007f2e4c: ldr      ip, [sp, #8]
007f2e50: str      r0, [sp, #0x24]
007f2e54: ldr      r0, [sp, #0xc]
007f2e58: mov      r1, ip
007f2e5c: bl       #0x30eba4
007f2e60: mov      r1, #0
007f2e64: mov      fp, r0
007f2e68: ldr      r0, [sp, #0x10]
007f2e6c: bl       #0x30eba4
007f2e70: ldr      r1, [sp, #0x14]
007f2e74: mov      ip, r0
007f2e78: ldr      r0, [sp, #0xc]
007f2e7c: str      ip, [sp, #8]
007f2e80: bl       #0x30eba4
007f2e84: mov      r1, fp
007f2e88: str      r0, [sp, #0xc]
007f2e8c: ldr      r0, [sp, #0x18]
007f2e90: bl       #0x30eba4
007f2e94: ldr      ip, [sp, #8]
007f2e98: str      r0, [sp, #0x18]
007f2e9c: ldr      r0, [sp, #0x1c]
007f2ea0: mov      r1, ip
007f2ea4: bl       #0x30eba4
007f2ea8: ldr      r1, [sp, #0xc]
007f2eac: mov      fp, r0
007f2eb0: ldr      r0, [sp, #0x24]
007f2eb4: bl       #0x30eba4
007f2eb8: mov      ip, r0
007f2ebc: mov      r1, r0
007f2ec0: ldr      r0, [sp, #0x18]
007f2ec4: str      ip, [sp, #8]
007f2ec8: bl       #0x30ed6c
007f2ecc: mov      r1, fp
007f2ed0: str      r0, [sp, #0xc]
007f2ed4: mov      r0, fp
007f2ed8: bl       #0x30ed6c
007f2edc: mov      r1, r0
007f2ee0: ldr      r0, [sp, #0xc]
007f2ee4: bl       #0x30e3ac
007f2ee8: ldr      r3, [sp]
007f2eec: ldr      r2, [sp, #4]
007f2ef0: mov      r1, r0
007f2ef4: add      r3, r3, #0x80000000
007f2ef8: add      r2, r2, #0x80000000
007f2efc: mov      r0, #0x3f800000
007f2f00: str      r2, [sp, #0x14]
007f2f04: str      r3, [sp, #0x10]
007f2f08: bl       #0x30ec94
007f2f0c: ldr      ip, [sp, #8]
007f2f10: ldr      r1, [sp, #0x10]
007f2f14: str      r0, [sp, #0x1c]
007f2f18: mov      r0, ip
007f2f1c: bl       #0x30ed6c
007f2f20: ldr      r1, [sp, #0x14]
007f2f24: mov      r3, r0
007f2f28: mov      r0, fp
007f2f2c: str      r3, [sp]
007f2f30: bl       #0x30ed6c
007f2f34: ldr      r3, [sp]
007f2f38: mov      r1, r0
007f2f3c: mov      r0, r3
007f2f40: bl       #0x30e3ac
007f2f44: mov      r1, r0
007f2f48: ldr      r0, [sp, #0x1c]
007f2f4c: bl       #0x30ed6c
007f2f50: ldr      r1, [sp, #0x14]
007f2f54: str      r0, [sp, #0xc]
007f2f58: ldr      r0, [sp, #0x18]
007f2f5c: bl       #0x30ed6c
007f2f60: ldr      r1, [sp, #0x10]
007f2f64: mov      r3, r0
007f2f68: mov      r0, fp
007f2f6c: str      r3, [sp]
007f2f70: bl       #0x30ed6c
007f2f74: ldr      r3, [sp]
007f2f78: mov      r1, r0
007f2f7c: mov      r0, r3
007f2f80: bl       #0x30e3ac
007f2f84: mov      r1, r0
007f2f88: ldr      r0, [sp, #0x1c]
007f2f8c: bl       #0x30ed6c
007f2f90: ldr      r1, [sp, #0xc]
007f2f94: mov      fp, r0
007f2f98: ldr      r0, [r5, #0x78]
007f2f9c: bl       #0x30ed6c
007f2fa0: mov      r1, r0
007f2fa4: ldr      r0, [r5, #0x2c]
007f2fa8: bl       #0x30e3ac
007f2fac: mov      r1, fp
007f2fb0: str      r0, [r5, #0x2c]
007f2fb4: ldr      r0, [r5, #0x78]
007f2fb8: bl       #0x30ed6c
007f2fbc: mov      r1, r0
007f2fc0: ldr      r0, [r5, #0x30]
007f2fc4: bl       #0x30e3ac
007f2fc8: mov      r1, fp
007f2fcc: str      r0, [r5, #0x30]
007f2fd0: mov      r0, sb
007f2fd4: bl       #0x30ed6c
007f2fd8: ldr      r1, [sp, #0xc]
007f2fdc: mov      sb, r0
007f2fe0: mov      r0, sl
007f2fe4: bl       #0x30ed6c
007f2fe8: mov      r1, r0
007f2fec: mov      r0, sb
007f2ff0: bl       #0x30e3ac
007f2ff4: ldr      r1, [r5, #0x80]
007f2ff8: bl       #0x30ed6c
007f2ffc: mov      r1, r0
007f3000: ldr      r0, [r5, #0x38]
007f3004: bl       #0x30e3ac
007f3008: str      r0, [r5, #0x38]
007f300c: ldr      sl, [r4, #0x78]
007f3010: ldr      r1, [sp, #0xc]
007f3014: mov      r0, sl
007f3018: bl       #0x30ed6c
007f301c: mov      r1, r0
007f3020: ldr      r0, [r4, #0x2c]
007f3024: bl       #0x30eba4
007f3028: mov      r1, fp
007f302c: str      r0, [r4, #0x2c]
007f3030: mov      r0, sl
007f3034: bl       #0x30ed6c
007f3038: mov      r1, r0
007f303c: ldr      r0, [r4, #0x30]
007f3040: bl       #0x30eba4
007f3044: mov      r1, fp
007f3048: str      r0, [r4, #0x30]
007f304c: mov      r0, r8
007f3050: bl       #0x30ed6c
007f3054: ldr      r1, [sp, #0xc]
007f3058: mov      r8, r0
007f305c: mov      r0, r7
007f3060: bl       #0x30ed6c
007f3064: mov      r1, r0
007f3068: mov      r0, r8
007f306c: bl       #0x30e3ac
007f3070: ldr      r1, [r4, #0x80]
007f3074: bl       #0x30ed6c
007f3078: mov      r1, r0
007f307c: ldr      r0, [r4, #0x38]
007f3080: bl       #0x30eba4
007f3084: str      r0, [r4, #0x38]
007f3088: mov      r0, r5
007f308c: bl       #0x7e761c
007f3090: mov      r0, r4
007f3094: bl       #0x7e761c
007f3098: ldrb     r3, [r6, #0x88]
007f309c: cmp      r3, #0
007f30a0: beq      #0x7f30b0
007f30a4: ldr      r8, [r6, #0x98]
007f30a8: cmp      r8, #0
007f30ac: bne      #0x7f30f4
007f30b0: mov      r7, #0
007f30b4: movw     r1, #0xd70a
007f30b8: ldr      r0, [sp, #0x20]
007f30bc: movt     r1, #0x3ba3
007f30c0: bl       #0x30e9ac
007f30c4: cmp      r0, #0
007f30c8: beq      #0x7f30ec
007f30cc: movw     r1, #0xfa36
007f30d0: mov      r0, r7
007f30d4: movt     r1, #0x3d0e
007f30d8: bl       #0x30e9ac
007f30dc: cmp      r0, #0
007f30e0: mov      r0, #0
007f30e4: movne    r0, #1
007f30e8: uxtb     r0, r0
007f30ec: add      sp, sp, #0x2c
007f30f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f30f4: ldr      sl, [r5, #0x38]
007f30f8: ldr      r0, [r4, #0x38]
007f30fc: mov      r1, sl
007f3100: bl       #0x30e3ac
007f3104: ldr      r1, [r6, #0x8c]
007f3108: bl       #0x30e3ac
007f310c: cmp      r8, #3
007f3110: mov      r7, r0
007f3114: beq      #0x7f317c
007f3118: cmp      r8, #1
007f311c: beq      #0x7f31cc
007f3120: cmp      r8, #2
007f3124: movne    r6, #0
007f3128: movne    r7, r6
007f312c: beq      #0x7f3284
007f3130: ldr      r1, [r5, #0x80]
007f3134: mov      r0, r6
007f3138: bl       #0x30ed6c
007f313c: mov      r1, r0
007f3140: mov      r0, sl
007f3144: bl       #0x30e3ac
007f3148: str      r0, [r5, #0x38]
007f314c: ldr      r1, [r4, #0x80]
007f3150: mov      r0, r6
007f3154: bl       #0x30ed6c
007f3158: mov      r1, r0
007f315c: ldr      r0, [r4, #0x38]
007f3160: bl       #0x30eba4
007f3164: str      r0, [r4, #0x38]
007f3168: mov      r0, r5
007f316c: bl       #0x7e761c
007f3170: mov      r0, r4
007f3174: bl       #0x7e761c
007f3178: b        #0x7f30b4
007f317c: movw     r1, #0xfa36
007f3180: movt     r1, #0x3e0e
007f3184: bl       #0x30e70c
007f3188: cmp      r0, #0
007f318c: beq      #0x7f3264
007f3190: movw     r1, #0xfa36
007f3194: mov      r0, r7
007f3198: movt     r1, #0xbe0e
007f319c: bl       #0x30e70c
007f31a0: cmp      r0, #0
007f31a4: beq      #0x7f3340
007f31a8: ldr      r0, [r6, #0x78]
007f31ac: movw     r1, #0xfa36
007f31b0: movt     r1, #0xbe0e
007f31b4: add      r0, r0, #0x80000000
007f31b8: bl       #0x30ed6c
007f31bc: movw     r7, #0xfa36
007f31c0: movt     r7, #0x3e0e
007f31c4: mov      r6, r0
007f31c8: b        #0x7f3130
007f31cc: ldr      r1, [r6, #0x90]
007f31d0: bl       #0x30e3ac
007f31d4: add      r7, r0, #0x80000000
007f31d8: mov      r8, r0
007f31dc: mov      r1, #0
007f31e0: mov      r0, r7
007f31e4: bl       #0x30e70c
007f31e8: movw     r1, #0xfa36
007f31ec: cmp      r0, #0
007f31f0: movt     r1, #0x3d0e
007f31f4: mov      r0, r8
007f31f8: movne    r7, #0
007f31fc: bl       #0x30eba4
007f3200: mov      r1, #0
007f3204: mov      r8, r0
007f3208: bl       #0x30e70c
007f320c: cmp      r0, #0
007f3210: moveq    r8, #0
007f3214: bne      #0x7f3320
007f3218: ldr      r0, [r6, #0x78]
007f321c: ldr      sl, [r6, #0x64]
007f3220: mov      r1, r8
007f3224: add      r0, r0, #0x80000000
007f3228: bl       #0x30ed6c
007f322c: mov      r1, sl
007f3230: bl       #0x30eba4
007f3234: mov      r1, #0
007f3238: mov      r8, r0
007f323c: bl       #0x30e2f8
007f3240: cmp      r0, #0
007f3244: beq      #0x7f3318
007f3248: str      r8, [r6, #0x64]
007f324c: mov      r1, sl
007f3250: mov      r0, r8
007f3254: bl       #0x30e3ac
007f3258: ldr      sl, [r5, #0x38]
007f325c: mov      r6, r0
007f3260: b        #0x7f3130
007f3264: ldr      r0, [r6, #0x78]
007f3268: movw     r1, #0xfa36
007f326c: movt     r1, #0x3e0e
007f3270: add      r0, r0, #0x80000000
007f3274: mov      r7, r1
007f3278: bl       #0x30ed6c
007f327c: mov      r6, r0
007f3280: b        #0x7f3130
007f3284: ldr      r1, [r6, #0x94]
007f3288: bl       #0x30e3ac
007f328c: mov      r1, #0
007f3290: mov      r8, r0
007f3294: bl       #0x30e70c
007f3298: movw     r1, #0xfa36
007f329c: cmp      r0, #0
007f32a0: movt     r1, #0x3d0e
007f32a4: mov      r0, r8
007f32a8: moveq    r7, r8
007f32ac: movne    r7, #0
007f32b0: bl       #0x30e3ac
007f32b4: movw     r1, #0xfa36
007f32b8: movt     r1, #0x3e0e
007f32bc: mov      r8, r0
007f32c0: bl       #0x30e70c
007f32c4: cmp      r0, #0
007f32c8: movweq   r8, #0xfa36
007f32cc: movteq   r8, #0x3e0e
007f32d0: beq      #0x7f32e8
007f32d4: mov      r0, r8
007f32d8: mov      r1, #0
007f32dc: bl       #0x30e70c
007f32e0: cmp      r0, #0
007f32e4: movne    r8, #0
007f32e8: ldr      r0, [r6, #0x78]
007f32ec: ldr      sl, [r6, #0x64]
007f32f0: mov      r1, r8
007f32f4: add      r0, r0, #0x80000000
007f32f8: bl       #0x30ed6c
007f32fc: mov      r1, sl
007f3300: bl       #0x30eba4
007f3304: mov      r1, #0
007f3308: mov      r8, r0
007f330c: bl       #0x30e70c
007f3310: cmp      r0, #0
007f3314: bne      #0x7f3248
007f3318: mov      r8, #0
007f331c: b        #0x7f3248
007f3320: movw     r1, #0xfa36
007f3324: mov      r0, r8
007f3328: movt     r1, #0xbe0e
007f332c: bl       #0x30e70c
007f3330: cmp      r0, #0
007f3334: movwne   r8, #0xfa36
007f3338: movtne   r8, #0xbe0e
007f333c: b        #0x7f3218
007f3340: ldr      r1, [r6, #0x78]
007f3344: mov      r0, r7
007f3348: add      r1, r1, #0x80000000
007f334c: bl       #0x30ed6c
007f3350: mov      r1, #0
007f3354: mov      r6, r0
007f3358: mov      r0, r7
007f335c: bl       #0x30e2f8
007f3360: cmp      r0, #0
007f3364: addeq    r7, r7, #0x80000000
007f3368: b        #0x7f3130

# _ZN16b2ContactManager11PairRemovedEPvS0_S0_
007ea508: subs     r1, r3, #0
007ea50c: bxeq     lr
007ea510: add      r3, r0, #8
007ea514: cmp      r1, r3
007ea518: bxeq     lr
007ea51c: b        #0x7ea084

# _ZN15b2RevoluteJointD1Ev
007f2bd4: bx       lr

# _ZN16b2PrismaticJoint13SetMotorSpeedEf
007eefe0: str      r1, [r0, #0xc4]
007eefe4: bx       lr

# _ZN12b2BroadPhaseC2ERK6b2AABBP14b2PairCallback
007e317c: push     {r4, r5, r6, r7, r8, lr}
007e3180: mov      r4, r0
007e3184: mov      r6, r2
007e3188: mov      r5, r1
007e318c: bl       #0x7e3e54
007e3190: mov      r2, r6
007e3194: mov      r0, r4
007e3198: mov      r1, r4
007e319c: bl       #0x7e3eec
007e31a0: ldm      r5, {r0, r1, r2, r3}
007e31a4: add      ip, r4, #0x5d000
007e31a8: add      ip, ip, #0x1c
007e31ac: stm      ip, {r0, r1, r2, r3}
007e31b0: mov      r3, #0x5d000
007e31b4: add      r3, r3, #0x34
007e31b8: mov      r6, #0
007e31bc: str      r6, [r4, r3]
007e31c0: ldr      r1, [r5, #4]
007e31c4: ldr      r0, [r5, #0xc]
007e31c8: bl       #0x30e3ac
007e31cc: ldr      r1, [r5]
007e31d0: mov      r7, r0
007e31d4: ldr      r0, [r5, #8]
007e31d8: bl       #0x30e3ac
007e31dc: mov      r1, r0
007e31e0: movw     r0, #0xff00
007e31e4: movt     r0, #0x477f
007e31e8: bl       #0x30ec94
007e31ec: mov      r3, #0x5d000
007e31f0: add      r3, r3, #0x2c
007e31f4: str      r0, [r4, r3]
007e31f8: movw     r0, #0xff00
007e31fc: mov      r1, r7
007e3200: movt     r0, #0x477f
007e3204: bl       #0x30ec94
007e3208: mov      r3, #0x5d000
007e320c: add      r3, r3, #0x30
007e3210: add      r2, r4, #0x48000
007e3214: str      r0, [r4, r3]
007e3218: add      r2, r2, #0x14
007e321c: mov      r3, r6
007e3220: movw     r1, #0x7ff
007e3224: add      r6, r6, #1
007e3228: uxth     r6, r6
007e322c: mov      r0, #0
007e3230: mvn      r8, #0
007e3234: cmp      r6, r1
007e3238: strh     r6, [r2]
007e323c: strh     r0, [r2, #0xa]
007e3240: strh     r8, [r2, #8]
007e3244: str      r3, [r2, #0xc]
007e3248: add      r2, r2, #0x10
007e324c: bne      #0x7e3224
007e3250: mov      r7, #0x50000
007e3254: mov      r6, r7
007e3258: mov      r5, r7
007e325c: mov      ip, r7
007e3260: mov      r0, r7
007e3264: mov      r1, #0x5d000
007e3268: mov      r2, r1
007e326c: add      r7, r7, #4
007e3270: add      r6, r6, #0xe
007e3274: add      r5, r5, #0xc
007e3278: add      ip, ip, #0x10
007e327c: add      r0, r0, #0x14
007e3280: strh     r8, [r4, r7]
007e3284: add      r1, r1, #0x38
007e3288: strh     r3, [r4, r6]
007e328c: add      r2, r2, #0x18
007e3290: strh     r8, [r4, r5]
007e3294: str      r3, [r4, ip]
007e3298: strh     r3, [r4, r0]
007e329c: mov      r0, #1
007e32a0: strh     r0, [r4, r1]
007e32a4: str      r3, [r4, r2]
007e32a8: mov      r0, r4
007e32ac: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12b2BroadPhase9MoveProxyEiRK6b2AABB
007e3344: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e3348: ldr      r3, [pc, #0x73c]
007e334c: sub      sp, sp, #0x44
007e3350: cmp      r1, #0x800
007e3354: add      r3, pc, r3
007e3358: str      r3, [sp]
007e335c: str      r1, [sp, #8]
007e3360: mov      r4, r0
007e3364: mov      r5, r2
007e3368: blt      #0x7e3374
007e336c: add      sp, sp, #0x44
007e3370: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e3374: mov      r0, r2
007e3378: bl       #0x7e32b0
007e337c: cmp      r0, #0
007e3380: beq      #0x7e336c
007e3384: mov      r3, #0x5d000
007e3388: add      ip, sp, #0x38
007e338c: add      r3, r3, #0x34
007e3390: ldr      sb, [r4, r3]
007e3394: mov      r1, ip
007e3398: add      r2, ip, #4
007e339c: mov      r3, r5
007e33a0: mov      r0, r4
007e33a4: str      ip, [sp, #0x14]
007e33a8: bl       #0x7e25b8
007e33ac: ldr      r0, [sp, #8]
007e33b0: mov      r3, #6
007e33b4: mov      ip, #0
007e33b8: add      r7, r0, #0x4800
007e33bc: add      r7, r7, #1
007e33c0: add      r7, r4, r7, lsl #4
007e33c4: ldrh     r1, [r7, #4]
007e33c8: add      r2, r4, r0, lsl #4
007e33cc: add      r2, r2, #0x48000
007e33d0: mla      r1, r3, r1, r4
007e33d4: add      r2, r2, #0x12
007e33d8: add      r1, r1, #0x50000
007e33dc: add      r1, r1, #0x10
007e33e0: ldrh     r1, [r1, #6]
007e33e4: add      r0, sp, #0x30
007e33e8: lsl      sb, sb, #1
007e33ec: strh     r1, [sp, #0x30]
007e33f0: ldrh     r1, [r7, #8]
007e33f4: str      ip, [sp, #0x28]
007e33f8: add      r7, r7, #4
007e33fc: mla      r1, r3, r1, r4
007e3400: sub      sb, sb, #1
007e3404: add      r1, r1, #0x50000
007e3408: add      r1, r1, #0x10
007e340c: ldrh     r1, [r1, #6]
007e3410: mov      r8, ip
007e3414: strh     r1, [sp, #0x34]
007e3418: ldrh     r1, [r2, #4]
007e341c: mla      r1, r3, r1, r4
007e3420: add      r1, r1, #0x56000
007e3424: add      r1, r1, #0x10
007e3428: ldrh     r1, [r1, #6]
007e342c: strh     r1, [sp, #0x32]
007e3430: ldrh     r2, [r2, #8]
007e3434: str      r0, [sp, #0x20]
007e3438: mla      r3, r3, r2, r4
007e343c: add      r3, r3, #0x56000
007e3440: add      r3, r3, #0x10
007e3444: ldrh     r3, [r3, #6]
007e3448: strh     r3, [sp, #0x36]
007e344c: ldrh     r2, [r7]
007e3450: mov      r1, #0x6000
007e3454: ldr      r0, [sp, #0x14]
007e3458: str      r2, [sp, #0x1c]
007e345c: ldrh     r3, [r7, #4]
007e3460: mul      fp, r1, r8
007e3464: ldr      r1, [sp, #0x28]
007e3468: mov      ip, #6
007e346c: str      r3, [sp, #0x18]
007e3470: mul      r3, ip, r2
007e3474: ldrh     r2, [r0, r1]!
007e3478: add      fp, fp, #0x50000
007e347c: ldr      r1, [sp, #0x18]
007e3480: add      fp, fp, #0x10
007e3484: ldrh     r0, [r0, #4]
007e3488: add      fp, r4, fp
007e348c: str      r2, [sp, #4]
007e3490: mul      r2, ip, r1
007e3494: add      ip, fp, ip
007e3498: str      ip, [sp, #0x10]
007e349c: ldrh     r1, [ip, r3]
007e34a0: str      r0, [sp, #0xc]
007e34a4: ldrh     r0, [ip, r2]
007e34a8: ldr      ip, [sp, #4]
007e34ac: subs     r1, ip, r1
007e34b0: str      r1, [sp, #0x2c]
007e34b4: ldr      r1, [sp, #0x10]
007e34b8: strh     ip, [r1, r3]
007e34bc: ldr      ip, [sp, #0xc]
007e34c0: rsb      r0, r0, ip
007e34c4: strh     ip, [r1, r2]
007e34c8: str      r0, [sp, #0x24]
007e34cc: bmi      #0x7e37fc
007e34d0: ldr      r1, [sp, #0x24]
007e34d4: cmp      r1, #0
007e34d8: ble      #0x7e35ac
007e34dc: ldr      r2, [sp, #0x18]
007e34e0: cmp      r2, sb
007e34e4: bge      #0x7e35ac
007e34e8: mov      r3, #6
007e34ec: mla      r6, r2, r3, r3
007e34f0: ldr      ip, [sp, #0x10]
007e34f4: ldr      r0, [sp, #0xc]
007e34f8: ldrh     r3, [ip, r6]
007e34fc: add      r6, ip, r6
007e3500: cmp      r3, r0
007e3504: bhi      #0x7e35ac
007e3508: add      r6, r6, #2
007e350c: mov      r1, #6
007e3510: mul      r5, r1, r2
007e3514: ldrh     r3, [r6, #2]
007e3518: mov      sl, r2
007e351c: ldrh     r2, [r6, #-2]
007e3520: add      r5, r5, #0xc
007e3524: add      r3, r3, #1
007e3528: tst      r2, #1
007e352c: add      r5, ip, r5
007e3530: ldrh     fp, [r6]
007e3534: strh     r3, [r6, #2]
007e3538: beq      #0x7e3704
007e353c: add      fp, r8, fp, lsl #3
007e3540: add      fp, fp, #0x24000
007e3544: add      fp, fp, #8
007e3548: add      fp, r4, fp, lsl #1
007e354c: ldrh     r3, [fp, #8]
007e3550: sub      r3, r3, #1
007e3554: strh     r3, [fp, #8]
007e3558: ldrh     r3, [r5, #-8]
007e355c: sub      r3, r3, #1
007e3560: strh     r3, [r5, #-8]
007e3564: ldrh     r3, [r7, #4]
007e3568: add      sl, sl, #1
007e356c: cmp      sl, sb
007e3570: add      r3, r3, #1
007e3574: strh     r3, [r7, #4]
007e3578: ldrh     ip, [r6, #-2]
007e357c: ldrh     r3, [r5, #-0xc]
007e3580: ldrh     r2, [r5, #-0xa]
007e3584: strh     ip, [r5, #-0xc]
007e3588: ldrh     r0, [r6]
007e358c: ldrh     r1, [r5, #-8]
007e3590: strh     r0, [r5, #-0xa]
007e3594: ldrh     ip, [r6, #2]
007e3598: strh     ip, [r5, #-8]
007e359c: strh     r1, [r6, #2]
007e35a0: strh     r2, [r6]
007e35a4: strh     r3, [r6, #-2]
007e35a8: bne      #0x7e36d4
007e35ac: ldr      r1, [sp, #0x2c]
007e35b0: cmp      r1, #0
007e35b4: ble      #0x7e3688
007e35b8: ldr      r2, [sp, #0x1c]
007e35bc: cmp      r2, sb
007e35c0: bge      #0x7e3688
007e35c4: mov      r3, #6
007e35c8: mla      r6, r2, r3, r3
007e35cc: ldr      ip, [sp, #0x10]
007e35d0: ldr      r0, [sp, #4]
007e35d4: ldrh     r3, [ip, r6]
007e35d8: add      r6, ip, r6
007e35dc: cmp      r3, r0
007e35e0: bhi      #0x7e3688
007e35e4: add      r6, r6, #2
007e35e8: mov      r1, #6
007e35ec: mul      r5, r1, r2
007e35f0: ldrh     r3, [r6, #2]
007e35f4: mov      sl, r2
007e35f8: ldrh     r2, [r6, #-2]
007e35fc: add      r5, r5, #0xc
007e3600: sub      r3, r3, #1
007e3604: tst      r2, #1
007e3608: add      r5, ip, r5
007e360c: ldrh     fp, [r6]
007e3610: strh     r3, [r6, #2]
007e3614: bne      #0x7e3784
007e3618: add      fp, r8, fp, lsl #3
007e361c: add      fp, fp, #0x24000
007e3620: add      fp, fp, #8
007e3624: add      fp, r4, fp, lsl #1
007e3628: ldrh     r3, [fp, #4]
007e362c: sub      r3, r3, #1
007e3630: strh     r3, [fp, #4]
007e3634: ldrh     r3, [r5, #-8]
007e3638: add      r3, r3, #1
007e363c: strh     r3, [r5, #-8]
007e3640: ldrh     r3, [r7]
007e3644: add      sl, sl, #1
007e3648: cmp      sl, sb
007e364c: add      r3, r3, #1
007e3650: strh     r3, [r7]
007e3654: ldrh     ip, [r6, #-2]
007e3658: ldrh     r3, [r5, #-0xc]
007e365c: ldrh     r2, [r5, #-0xa]
007e3660: strh     ip, [r5, #-0xc]
007e3664: ldrh     r0, [r6]
007e3668: ldrh     r1, [r5, #-8]
007e366c: strh     r0, [r5, #-0xa]
007e3670: ldrh     ip, [r6, #2]
007e3674: strh     ip, [r5, #-8]
007e3678: strh     r1, [r6, #2]
007e367c: strh     r2, [r6]
007e3680: strh     r3, [r6, #-2]
007e3684: bne      #0x7e3754
007e3688: ldr      r1, [sp, #0x24]
007e368c: cmp      r1, #0
007e3690: blt      #0x7e392c
007e3694: ldr      r1, [sp, #0x28]
007e3698: add      r8, r8, #1
007e369c: cmp      r8, #2
007e36a0: add      r1, r1, #2
007e36a4: add      r7, r7, #2
007e36a8: str      r1, [sp, #0x28]
007e36ac: bne      #0x7e344c
007e36b0: ldr      r3, [pc, #0x3d8]
007e36b4: ldr      r2, [sp]
007e36b8: ldr      r3, [r2, r3]
007e36bc: ldrb     r3, [r3]
007e36c0: cmp      r3, #0
007e36c4: beq      #0x7e336c
007e36c8: mov      r0, r4
007e36cc: bl       #0x7e2ac4
007e36d0: b        #0x7e336c
007e36d4: ldrh     r3, [r5], #6
007e36d8: ldr      r0, [sp, #0xc]
007e36dc: add      r6, r6, #6
007e36e0: cmp      r0, r3
007e36e4: blo      #0x7e35ac
007e36e8: ldrh     r3, [r6, #2]
007e36ec: ldrh     r2, [r6, #-2]
007e36f0: ldrh     fp, [r6]
007e36f4: add      r3, r3, #1
007e36f8: tst      r2, #1
007e36fc: strh     r3, [r6, #2]
007e3700: bne      #0x7e353c
007e3704: add      r3, fp, #0x4800
007e3708: add      r3, r3, #1
007e370c: add      r3, r4, r3, lsl #4
007e3710: add      r2, r3, #4
007e3714: ldr      r1, [sp, #0x14]
007e3718: mov      r0, r4
007e371c: bl       #0x7e252c
007e3720: cmp      r0, #0
007e3724: bne      #0x7e37e8
007e3728: add      fp, r8, fp, lsl #3
007e372c: add      fp, fp, #0x24000
007e3730: add      fp, fp, #8
007e3734: add      fp, r4, fp, lsl #1
007e3738: ldrh     r3, [fp, #4]
007e373c: sub      r3, r3, #1
007e3740: strh     r3, [fp, #4]
007e3744: ldrh     r3, [r5, #-8]
007e3748: add      r3, r3, #1
007e374c: strh     r3, [r5, #-8]
007e3750: b        #0x7e3564
007e3754: ldrh     r3, [r5], #6
007e3758: ldr      r0, [sp, #4]
007e375c: add      r6, r6, #6
007e3760: cmp      r0, r3
007e3764: blo      #0x7e3688
007e3768: ldrh     r3, [r6, #2]
007e376c: ldrh     r2, [r6, #-2]
007e3770: ldrh     fp, [r6]
007e3774: sub      r3, r3, #1
007e3778: tst      r2, #1
007e377c: strh     r3, [r6, #2]
007e3780: beq      #0x7e3618
007e3784: add      r3, fp, #0x4800
007e3788: add      r3, r3, #1
007e378c: add      r3, r4, r3, lsl #4
007e3790: add      r2, r3, #4
007e3794: ldr      r1, [sp, #0x20]
007e3798: mov      r0, r4
007e379c: bl       #0x7e252c
007e37a0: cmp      r0, #0
007e37a4: bne      #0x7e37d4
007e37a8: add      fp, r8, fp, lsl #3
007e37ac: add      fp, fp, #0x24000
007e37b0: add      fp, fp, #8
007e37b4: add      fp, r4, fp, lsl #1
007e37b8: ldrh     r3, [fp, #8]
007e37bc: sub      r3, r3, #1
007e37c0: strh     r3, [fp, #8]
007e37c4: ldrh     r3, [r5, #-8]
007e37c8: sub      r3, r3, #1
007e37cc: strh     r3, [r5, #-8]
007e37d0: b        #0x7e3640
007e37d4: mov      r0, r4
007e37d8: ldr      r1, [sp, #8]
007e37dc: mov      r2, fp
007e37e0: bl       #0x7e4224
007e37e4: b        #0x7e37a8
007e37e8: mov      r0, r4
007e37ec: ldr      r1, [sp, #8]
007e37f0: mov      r2, fp
007e37f4: bl       #0x7e4190
007e37f8: b        #0x7e3728
007e37fc: ldr      r0, [sp, #0x1c]
007e3800: cmp      r0, #0
007e3804: beq      #0x7e34d0
007e3808: sub      r5, r0, #1
007e380c: mov      r1, #6
007e3810: mul      r5, r1, r5
007e3814: ldr      ip, [sp, #0x10]
007e3818: ldr      r0, [sp, #4]
007e381c: ldrh     r2, [ip, r5]
007e3820: add      r5, ip, r5
007e3824: cmp      r2, r0
007e3828: bls      #0x7e34d0
007e382c: sub      r3, r3, #0xc
007e3830: add      r5, r5, #2
007e3834: add      r6, ip, r3
007e3838: add      fp, fp, #8
007e383c: b        #0x7e38c0
007e3840: add      r3, r8, sl, lsl #3
007e3844: add      r3, r3, #0x24000
007e3848: add      r3, r3, #8
007e384c: add      r3, r4, r3, lsl #1
007e3850: ldrh     r2, [r3, #4]
007e3854: add      r2, r2, #1
007e3858: strh     r2, [r3, #4]
007e385c: ldrh     r3, [r6, #0x10]
007e3860: sub      r3, r3, #1
007e3864: strh     r3, [r6, #0x10]
007e3868: ldrh     r3, [r7]
007e386c: cmp      r5, fp
007e3870: sub      r3, r3, #1
007e3874: strh     r3, [r7]
007e3878: ldrh     ip, [r5, #-2]
007e387c: ldrh     r3, [r6, #0xc]
007e3880: ldrh     r2, [r6, #0xe]
007e3884: strh     ip, [r6, #0xc]
007e3888: ldrh     r0, [r5]
007e388c: ldrh     r1, [r6, #0x10]
007e3890: strh     r0, [r6, #0xe]
007e3894: ldrh     ip, [r5, #2]
007e3898: strh     ip, [r6, #0x10]
007e389c: strh     r1, [r5, #2]
007e38a0: strh     r2, [r5]
007e38a4: strh     r3, [r5, #-2]
007e38a8: beq      #0x7e34d0
007e38ac: ldrh     r3, [r6], #-6
007e38b0: ldr      r0, [sp, #4]
007e38b4: sub      r5, r5, #6
007e38b8: cmp      r0, r3
007e38bc: bhs      #0x7e34d0
007e38c0: ldrh     r3, [r5, #2]
007e38c4: ldrh     r2, [r5, #-2]
007e38c8: ldrh     sl, [r5]
007e38cc: add      r3, r3, #1
007e38d0: tst      r2, #1
007e38d4: strh     r3, [r5, #2]
007e38d8: beq      #0x7e3840
007e38dc: add      r3, sl, #0x4800
007e38e0: add      r3, r3, #1
007e38e4: add      r3, r4, r3, lsl #4
007e38e8: add      r2, r3, #4
007e38ec: ldr      r1, [sp, #0x14]
007e38f0: mov      r0, r4
007e38f4: bl       #0x7e252c
007e38f8: cmp      r0, #0
007e38fc: bne      #0x7e3a78
007e3900: add      r3, r8, sl, lsl #3
007e3904: add      r3, r3, #0x24000
007e3908: add      r3, r3, #8
007e390c: add      r3, r4, r3, lsl #1
007e3910: ldrh     r2, [r3, #8]
007e3914: add      r2, r2, #1
007e3918: strh     r2, [r3, #8]
007e391c: ldrh     r3, [r6, #0x10]
007e3920: add      r3, r3, #1
007e3924: strh     r3, [r6, #0x10]
007e3928: b        #0x7e3868
007e392c: ldr      r2, [sp, #0x18]
007e3930: cmp      r2, #0
007e3934: beq      #0x7e3694
007e3938: mov      r3, #6
007e393c: sub      r5, r2, #1
007e3940: mul      r5, r3, r5
007e3944: ldr      ip, [sp, #0x10]
007e3948: ldr      r0, [sp, #0xc]
007e394c: ldrh     r3, [ip, r5]
007e3950: add      r5, ip, r5
007e3954: cmp      r3, r0
007e3958: bls      #0x7e3694
007e395c: mov      r1, #6
007e3960: mul      r3, r1, r2
007e3964: add      r5, r5, #2
007e3968: sub      r3, r3, #0xc
007e396c: add      r6, ip, r3
007e3970: add      fp, ip, #2
007e3974: b        #0x7e39f8
007e3978: add      r3, r8, sl, lsl #3
007e397c: add      r3, r3, #0x24000
007e3980: add      r3, r3, #8
007e3984: add      r3, r4, r3, lsl #1
007e3988: ldrh     r2, [r3, #8]
007e398c: add      r2, r2, #1
007e3990: strh     r2, [r3, #8]
007e3994: ldrh     r3, [r6, #0x10]
007e3998: add      r3, r3, #1
007e399c: strh     r3, [r6, #0x10]
007e39a0: ldrh     r3, [r7, #4]
007e39a4: cmp      r5, fp
007e39a8: sub      r3, r3, #1
007e39ac: strh     r3, [r7, #4]
007e39b0: ldrh     ip, [r5, #-2]
007e39b4: ldrh     r3, [r6, #0xc]
007e39b8: ldrh     r2, [r6, #0xe]
007e39bc: strh     ip, [r6, #0xc]
007e39c0: ldrh     r0, [r5]
007e39c4: ldrh     r1, [r6, #0x10]
007e39c8: strh     r0, [r6, #0xe]
007e39cc: ldrh     ip, [r5, #2]
007e39d0: strh     ip, [r6, #0x10]
007e39d4: strh     r1, [r5, #2]
007e39d8: strh     r2, [r5]
007e39dc: strh     r3, [r5, #-2]
007e39e0: beq      #0x7e3694
007e39e4: ldrh     r3, [r6], #-6
007e39e8: ldr      r0, [sp, #0xc]
007e39ec: sub      r5, r5, #6
007e39f0: cmp      r0, r3
007e39f4: bhs      #0x7e3694
007e39f8: ldrh     r3, [r5, #2]
007e39fc: ldrh     r2, [r5, #-2]
007e3a00: ldrh     sl, [r5]
007e3a04: sub      r3, r3, #1
007e3a08: tst      r2, #1
007e3a0c: strh     r3, [r5, #2]
007e3a10: bne      #0x7e3978
007e3a14: add      r3, sl, #0x4800
007e3a18: add      r3, r3, #1
007e3a1c: add      r3, r4, r3, lsl #4
007e3a20: add      r2, r3, #4
007e3a24: ldr      r1, [sp, #0x20]
007e3a28: mov      r0, r4
007e3a2c: bl       #0x7e252c
007e3a30: cmp      r0, #0
007e3a34: bne      #0x7e3a64
007e3a38: add      r3, r8, sl, lsl #3
007e3a3c: add      r3, r3, #0x24000
007e3a40: add      r3, r3, #8
007e3a44: add      r3, r4, r3, lsl #1
007e3a48: ldrh     r2, [r3, #4]
007e3a4c: add      r2, r2, #1
007e3a50: strh     r2, [r3, #4]
007e3a54: ldrh     r3, [r6, #0x10]
007e3a58: sub      r3, r3, #1
007e3a5c: strh     r3, [r6, #0x10]
007e3a60: b        #0x7e39a0
007e3a64: mov      r0, r4
007e3a68: ldr      r1, [sp, #8]
007e3a6c: mov      r2, sl
007e3a70: bl       #0x7e4224
007e3a74: b        #0x7e3a38
007e3a78: mov      r0, r4
007e3a7c: ldr      r1, [sp, #8]
007e3a80: mov      r2, sl
007e3a84: bl       #0x7e4190
007e3a88: b        #0x7e3900
007e3a8c: andseq   r1, fp, ip, lsr r7
007e3a90: andeq    r2, r0, r8, ror #26

# _ZN11b2DebugDraw10ClearFlagsEj
007e8d14: ldr      r3, [r0, #4]
007e8d18: bic      r3, r3, r1
007e8d1c: str      r3, [r0, #4]
007e8d20: bx       lr

# _Z6b2FreePv
007f34bc: ldr      r3, [pc, #0x28]
007f34c0: cmp      r0, #0
007f34c4: add      r3, pc, r3
007f34c8: bxeq     lr
007f34cc: ldr      r1, [pc, #0x1c]
007f34d0: ldr      r2, [r0, #-4]
007f34d4: sub      r0, r0, #4
007f34d8: ldr      r3, [r3, r1]
007f34dc: ldr      r1, [r3]
007f34e0: rsb      r2, r2, r1
007f34e4: str      r2, [r3]
007f34e8: b        #0x30def0
007f34ec: andseq   r1, sl, ip, asr #11
007f34f0: andeq    r0, r0, r4, lsl #16

# _ZN15b2RevoluteJoint23InitVelocityConstraintsERK10b2TimeStep
007f1e84: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f1e88: ldr      r6, [r0, #0x30]
007f1e8c: sub      sp, sp, #0x34
007f1e90: str      r1, [sp, #0x14]
007f1e94: mov      r4, r0
007f1e98: ldr      r1, [r6, #0x1c]
007f1e9c: ldr      r0, [r0, #0x44]
007f1ea0: bl       #0x30e3ac
007f1ea4: ldr      r1, [r6, #0x20]
007f1ea8: mov      r8, r0
007f1eac: ldr      r0, [r4, #0x48]
007f1eb0: bl       #0x30e3ac
007f1eb4: ldr      r1, [r6, #0xc]
007f1eb8: mov      r7, r0
007f1ebc: mov      r0, r8
007f1ec0: bl       #0x30ed6c
007f1ec4: ldr      r1, [r6, #0x14]
007f1ec8: mov      r5, r0
007f1ecc: mov      r0, r7
007f1ed0: bl       #0x30ed6c
007f1ed4: mov      r1, r0
007f1ed8: mov      r0, r5
007f1edc: bl       #0x30eba4
007f1ee0: ldr      r1, [r6, #0x10]
007f1ee4: mov      fp, r0
007f1ee8: mov      r0, r8
007f1eec: bl       #0x30ed6c
007f1ef0: ldr      r1, [r6, #0x18]
007f1ef4: mov      r8, r0
007f1ef8: mov      r0, r7
007f1efc: bl       #0x30ed6c
007f1f00: mov      r1, r0
007f1f04: mov      r0, r8
007f1f08: bl       #0x30eba4
007f1f0c: ldr      r5, [r4, #0x34]
007f1f10: mov      sb, r0
007f1f14: ldr      r0, [r4, #0x4c]
007f1f18: ldr      r1, [r5, #0x1c]
007f1f1c: bl       #0x30e3ac
007f1f20: ldr      r1, [r5, #0x20]
007f1f24: mov      r8, r0
007f1f28: ldr      r0, [r4, #0x50]
007f1f2c: bl       #0x30e3ac
007f1f30: ldr      r1, [r5, #0xc]
007f1f34: mov      r7, r0
007f1f38: mov      r0, r8
007f1f3c: bl       #0x30ed6c
007f1f40: ldr      r1, [r5, #0x14]
007f1f44: mov      sl, r0
007f1f48: mov      r0, r7
007f1f4c: bl       #0x30ed6c
007f1f50: mov      r1, r0
007f1f54: mov      r0, sl
007f1f58: bl       #0x30eba4
007f1f5c: ldr      r1, [r5, #0x10]
007f1f60: mov      sl, r0
007f1f64: mov      r0, r8
007f1f68: bl       #0x30ed6c
007f1f6c: ldr      r1, [r5, #0x18]
007f1f70: mov      r8, r0
007f1f74: mov      r0, r7
007f1f78: bl       #0x30ed6c
007f1f7c: mov      r1, r0
007f1f80: mov      r0, r8
007f1f84: bl       #0x30eba4
007f1f88: str      r0, [sp, #0x10]
007f1f8c: ldr      r2, [r6, #0x78]
007f1f90: str      r2, [sp, #0x28]
007f1f94: ldr      r3, [r5, #0x78]
007f1f98: mov      r0, r2
007f1f9c: mov      r1, r3
007f1fa0: str      r3, [sp, #0x24]
007f1fa4: bl       #0x30eba4
007f1fa8: ldr      r8, [r6, #0x80]
007f1fac: mov      r1, sb
007f1fb0: str      r0, [sp, #0xc]
007f1fb4: mov      r0, r8
007f1fb8: bl       #0x30ed6c
007f1fbc: mov      r1, sb
007f1fc0: bl       #0x30ed6c
007f1fc4: mov      r1, fp
007f1fc8: mov      r3, r0
007f1fcc: add      r0, r8, #0x80000000
007f1fd0: str      r3, [sp]
007f1fd4: bl       #0x30ed6c
007f1fd8: mov      r1, sb
007f1fdc: bl       #0x30ed6c
007f1fe0: mov      r1, fp
007f1fe4: mov      r2, r0
007f1fe8: mov      r0, r8
007f1fec: ldr      r7, [r5, #0x80]
007f1ff0: str      r2, [sp, #4]
007f1ff4: bl       #0x30ed6c
007f1ff8: mov      r1, fp
007f1ffc: bl       #0x30ed6c
007f2000: ldr      r1, [sp, #0x10]
007f2004: mov      ip, r0
007f2008: mov      r0, r7
007f200c: str      ip, [sp, #8]
007f2010: bl       #0x30ed6c
007f2014: ldr      r1, [sp, #0x10]
007f2018: bl       #0x30ed6c
007f201c: mov      r1, sl
007f2020: str      r0, [sp, #0x18]
007f2024: add      r0, r7, #0x80000000
007f2028: bl       #0x30ed6c
007f202c: ldr      r1, [sp, #0x10]
007f2030: bl       #0x30ed6c
007f2034: mov      r1, sl
007f2038: str      r0, [sp, #0x1c]
007f203c: mov      r0, r7
007f2040: bl       #0x30ed6c
007f2044: mov      r1, sl
007f2048: bl       #0x30ed6c
007f204c: ldr      r3, [sp]
007f2050: str      r0, [sp, #0x20]
007f2054: ldr      r0, [sp, #0xc]
007f2058: mov      r1, r3
007f205c: bl       #0x30eba4
007f2060: ldr      r2, [sp, #4]
007f2064: mov      r3, r0
007f2068: mov      r1, #0
007f206c: mov      r0, r2
007f2070: str      r3, [sp]
007f2074: bl       #0x30eba4
007f2078: ldr      ip, [sp, #8]
007f207c: mov      r2, r0
007f2080: ldr      r0, [sp, #0xc]
007f2084: mov      r1, ip
007f2088: str      r2, [sp, #4]
007f208c: bl       #0x30eba4
007f2090: ldr      r3, [sp]
007f2094: mov      ip, r0
007f2098: ldr      r0, [sp, #0x18]
007f209c: mov      r1, r3
007f20a0: str      ip, [sp, #8]
007f20a4: bl       #0x30eba4
007f20a8: ldr      r2, [sp, #4]
007f20ac: str      r0, [sp, #0xc]
007f20b0: ldr      r0, [sp, #0x1c]
007f20b4: mov      r1, r2
007f20b8: bl       #0x30eba4
007f20bc: ldr      ip, [sp, #8]
007f20c0: mov      r3, r0
007f20c4: ldr      r0, [sp, #0x20]
007f20c8: mov      r1, ip
007f20cc: str      r3, [sp]
007f20d0: bl       #0x30eba4
007f20d4: mov      ip, r0
007f20d8: mov      r1, ip
007f20dc: ldr      r0, [sp, #0xc]
007f20e0: str      ip, [sp, #8]
007f20e4: bl       #0x30ed6c
007f20e8: ldr      r3, [sp]
007f20ec: mov      r2, r0
007f20f0: str      r2, [sp, #4]
007f20f4: mov      r1, r3
007f20f8: mov      r0, r3
007f20fc: bl       #0x30ed6c
007f2100: ldr      r2, [sp, #4]
007f2104: mov      r1, r0
007f2108: mov      r0, r2
007f210c: bl       #0x30e3ac
007f2110: mov      r1, r0
007f2114: mov      r0, #0x3f800000
007f2118: bl       #0x30ec94
007f211c: ldr      r3, [sp]
007f2120: mov      r2, r0
007f2124: add      r1, r0, #0x80000000
007f2128: mov      r0, r3
007f212c: str      r2, [sp, #4]
007f2130: bl       #0x30ed6c
007f2134: ldr      r2, [sp, #4]
007f2138: mov      r3, r0
007f213c: ldr      r0, [sp, #0xc]
007f2140: mov      r1, r2
007f2144: str      r3, [sp]
007f2148: bl       #0x30ed6c
007f214c: str      r0, [r4, #0x74]
007f2150: ldr      r3, [sp]
007f2154: str      r3, [r4, #0x6c]
007f2158: ldr      r2, [sp, #4]
007f215c: str      r3, [r4, #0x70]
007f2160: ldr      ip, [sp, #8]
007f2164: mov      r1, r2
007f2168: mov      r0, ip
007f216c: bl       #0x30ed6c
007f2170: mov      r1, r7
007f2174: str      r0, [r4, #0x68]
007f2178: mov      r0, r8
007f217c: bl       #0x30eba4
007f2180: mov      r1, r0
007f2184: mov      r0, #0x3f800000
007f2188: bl       #0x30ec94
007f218c: ldrb     r3, [r4, #0x7c]
007f2190: str      r0, [r4, #0x78]
007f2194: cmp      r3, #0
007f2198: moveq    r3, #0
007f219c: streq    r3, [r4, #0x5c]
007f21a0: ldrb     r3, [r4, #0x88]
007f21a4: cmp      r3, #0
007f21a8: moveq    r3, #0
007f21ac: streq    r3, [r4, #0x60]
007f21b0: beq      #0x7f2220
007f21b4: ldr      r2, [r4, #0x94]
007f21b8: str      r2, [sp, #0x18]
007f21bc: ldr      r3, [r4, #0x90]
007f21c0: ldr      r0, [sp, #0x18]
007f21c4: str      r3, [sp, #0xc]
007f21c8: ldr      r2, [r5, #0x38]
007f21cc: mov      r1, r3
007f21d0: str      r2, [sp, #0x2c]
007f21d4: bl       #0x30e3ac
007f21d8: mov      r1, #0
007f21dc: str      r0, [sp]
007f21e0: bl       #0x30e2f8
007f21e4: ldr      r2, [r6, #0x38]
007f21e8: ldr      r3, [sp]
007f21ec: cmp      r0, #0
007f21f0: str      r2, [sp, #0x20]
007f21f4: ldr      r2, [r4, #0x8c]
007f21f8: addeq    r3, r3, #0x80000000
007f21fc: movw     r1, #0xfa36
007f2200: mov      r0, r3
007f2204: movt     r1, #0x3d8e
007f2208: str      r2, [sp, #0x1c]
007f220c: bl       #0x30e70c
007f2210: cmp      r0, #0
007f2214: movne    r3, #3
007f2218: strne    r3, [r4, #0x98]
007f221c: beq      #0x7f23f0
007f2220: ldr      r2, [sp, #0x14]
007f2224: ldrb     r3, [r2, #0x10]
007f2228: cmp      r3, #0
007f222c: beq      #0x7f23d8
007f2230: ldr      r1, [r2]
007f2234: ldr      r0, [sp, #0x28]
007f2238: bl       #0x30ed6c
007f223c: ldr      r1, [r4, #0x58]
007f2240: str      r0, [sp]
007f2244: bl       #0x30ed6c
007f2248: ldr      r3, [sp]
007f224c: ldr      r1, [r4, #0x54]
007f2250: mov      r2, r0
007f2254: mov      r0, r3
007f2258: str      r2, [sp, #4]
007f225c: bl       #0x30ed6c
007f2260: mov      r1, r0
007f2264: ldr      r0, [r6, #0x40]
007f2268: bl       #0x30e3ac
007f226c: str      r0, [r6, #0x40]
007f2270: ldr      r2, [sp, #4]
007f2274: ldr      r0, [r6, #0x44]
007f2278: mov      r1, r2
007f227c: bl       #0x30e3ac
007f2280: str      r0, [r6, #0x44]
007f2284: ldr      r3, [sp, #0x14]
007f2288: mov      r0, r8
007f228c: ldr      r1, [r3]
007f2290: bl       #0x30ed6c
007f2294: ldr      r1, [r4, #0x60]
007f2298: mov      r2, r0
007f229c: ldr      r0, [r4, #0x5c]
007f22a0: str      r2, [sp, #4]
007f22a4: bl       #0x30eba4
007f22a8: ldr      r1, [r4, #0x58]
007f22ac: mov      r3, r0
007f22b0: mov      r0, fp
007f22b4: str      r3, [sp]
007f22b8: bl       #0x30ed6c
007f22bc: ldr      r1, [r4, #0x54]
007f22c0: mov      r8, r0
007f22c4: mov      r0, sb
007f22c8: bl       #0x30ed6c
007f22cc: mov      r1, r0
007f22d0: mov      r0, r8
007f22d4: bl       #0x30e3ac
007f22d8: ldr      r3, [sp]
007f22dc: mov      r1, r0
007f22e0: mov      r0, r3
007f22e4: bl       #0x30eba4
007f22e8: ldr      r2, [sp, #4]
007f22ec: mov      r1, r0
007f22f0: mov      r0, r2
007f22f4: bl       #0x30ed6c
007f22f8: mov      r1, r0
007f22fc: ldr      r0, [r6, #0x48]
007f2300: bl       #0x30e3ac
007f2304: str      r0, [r6, #0x48]
007f2308: ldr      r2, [sp, #0x14]
007f230c: ldr      r0, [sp, #0x24]
007f2310: ldr      r1, [r2]
007f2314: bl       #0x30ed6c
007f2318: ldr      r1, [r4, #0x58]
007f231c: mov      r8, r0
007f2320: bl       #0x30ed6c
007f2324: ldr      r1, [r4, #0x54]
007f2328: mov      r6, r0
007f232c: mov      r0, r8
007f2330: bl       #0x30ed6c
007f2334: mov      r1, r0
007f2338: ldr      r0, [r5, #0x40]
007f233c: bl       #0x30eba4
007f2340: mov      r1, r6
007f2344: str      r0, [r5, #0x40]
007f2348: ldr      r0, [r5, #0x44]
007f234c: bl       #0x30eba4
007f2350: str      r0, [r5, #0x44]
007f2354: ldr      r3, [sp, #0x14]
007f2358: mov      r0, r7
007f235c: ldr      r1, [r3]
007f2360: bl       #0x30ed6c
007f2364: ldr      r1, [r4, #0x60]
007f2368: mov      r6, r0
007f236c: ldr      r0, [r4, #0x5c]
007f2370: bl       #0x30eba4
007f2374: ldr      r1, [r4, #0x58]
007f2378: mov      r7, r0
007f237c: mov      r0, sl
007f2380: bl       #0x30ed6c
007f2384: ldr      r1, [r4, #0x54]
007f2388: mov      r8, r0
007f238c: ldr      r0, [sp, #0x10]
007f2390: bl       #0x30ed6c
007f2394: mov      r1, r0
007f2398: mov      r0, r8
007f239c: bl       #0x30e3ac
007f23a0: mov      r1, r0
007f23a4: mov      r0, r7
007f23a8: bl       #0x30eba4
007f23ac: mov      r1, r0
007f23b0: mov      r0, r6
007f23b4: bl       #0x30ed6c
007f23b8: mov      r1, r0
007f23bc: ldr      r0, [r5, #0x48]
007f23c0: bl       #0x30eba4
007f23c4: str      r0, [r5, #0x48]
007f23c8: mov      r3, #0
007f23cc: str      r3, [r4, #0x64]
007f23d0: add      sp, sp, #0x34
007f23d4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f23d8: mov      r3, #0
007f23dc: str      r3, [r4, #0x60]
007f23e0: str      r3, [r4, #0x54]
007f23e4: str      r3, [r4, #0x58]
007f23e8: str      r3, [r4, #0x5c]
007f23ec: b        #0x7f23c8
007f23f0: ldr      r1, [sp, #0x20]
007f23f4: ldr      r0, [sp, #0x2c]
007f23f8: bl       #0x30e3ac
007f23fc: ldr      r1, [sp, #0x1c]
007f2400: bl       #0x30e3ac
007f2404: ldr      r1, [sp, #0xc]
007f2408: str      r0, [sp, #0x1c]
007f240c: bl       #0x30e9ac
007f2410: cmp      r0, #0
007f2414: bne      #0x7f2448
007f2418: ldr      r0, [sp, #0x1c]
007f241c: ldr      r1, [sp, #0x18]
007f2420: bl       #0x30e4b4
007f2424: cmp      r0, #0
007f2428: beq      #0x7f2464
007f242c: ldr      r3, [r4, #0x98]
007f2430: cmp      r3, #2
007f2434: movne    r3, #0
007f2438: strne    r3, [r4, #0x60]
007f243c: mov      r3, #2
007f2440: str      r3, [r4, #0x98]
007f2444: b        #0x7f2220
007f2448: ldr      r3, [r4, #0x98]
007f244c: cmp      r3, #1
007f2450: movne    r3, #0
007f2454: strne    r3, [r4, #0x60]
007f2458: mov      r3, #1
007f245c: str      r3, [r4, #0x98]
007f2460: b        #0x7f2220
007f2464: mov      r3, #0
007f2468: str      r3, [r4, #0x98]
007f246c: mov      r3, #0
007f2470: str      r3, [r4, #0x60]
007f2474: b        #0x7f2220

# _ZN12b2BroadPhase11TestOverlapEP7b2ProxyS1_
007e2480: push     {r4, r5}
007e2484: ldrh     r4, [r1]
007e2488: ldrh     ip, [r2, #4]
007e248c: mov      r3, #6
007e2490: mul      r4, r3, r4
007e2494: mul      ip, r3, ip
007e2498: add      r5, r0, #0x50000
007e249c: add      r5, r5, #0x16
007e24a0: ldrh     r4, [r5, r4]
007e24a4: ldrh     ip, [r5, ip]
007e24a8: cmp      r4, ip
007e24ac: bhi      #0x7e2524
007e24b0: ldrh     r4, [r1, #4]
007e24b4: ldrh     ip, [r2]
007e24b8: mul      r4, r3, r4
007e24bc: mul      ip, r3, ip
007e24c0: ldrh     r4, [r5, r4]
007e24c4: ldrh     ip, [r5, ip]
007e24c8: cmp      r4, ip
007e24cc: blo      #0x7e2524
007e24d0: ldrh     r4, [r1, #2]
007e24d4: ldrh     r5, [r2, #6]
007e24d8: add      ip, r0, #0x56000
007e24dc: mul      r4, r3, r4
007e24e0: mul      r5, r3, r5
007e24e4: add      ip, ip, #0x16
007e24e8: ldrh     r4, [ip, r4]
007e24ec: ldrh     r0, [ip, r5]
007e24f0: cmp      r4, r0
007e24f4: bhi      #0x7e2524
007e24f8: ldrh     r0, [r1, #6]
007e24fc: ldrh     r1, [r2, #2]
007e2500: mul      r2, r3, r0
007e2504: mul      r3, r3, r1
007e2508: ldrh     r0, [ip, r2]
007e250c: ldrh     r3, [ip, r3]
007e2510: cmp      r0, r3
007e2514: movlo    r0, #0
007e2518: movhs    r0, #1
007e251c: pop      {r4, r5}
007e2520: bx       lr
007e2524: mov      r0, #0
007e2528: b        #0x7e251c

# _ZNK13b2PulleyJoint8GetRatioEv
007f04f0: ldr      r0, [r0, #0x7c]
007f04f4: bx       lr

# _ZN16b2BlockAllocator8AllocateEi
007e90bc: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e90c0: ldr      r4, [pc, #0x144]
007e90c4: cmp      r1, #0
007e90c8: mov      r5, r0
007e90cc: add      r4, pc, r4
007e90d0: beq      #0x7e9104
007e90d4: ldr      r3, [pc, #0x134]
007e90d8: ldr      r3, [r4, r3]
007e90dc: ldrb     r8, [r3, r1]
007e90e0: add      r7, r8, #2
007e90e4: add      r3, r0, r7, lsl #2
007e90e8: ldr      r6, [r3, #4]
007e90ec: cmp      r6, #0
007e90f0: beq      #0x7e910c
007e90f4: ldr      r2, [r6]
007e90f8: mov      r0, r6
007e90fc: str      r2, [r3, #4]
007e9100: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e9104: mov      r0, r1
007e9108: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e910c: ldr      sb, [r0, #4]
007e9110: ldr      r3, [r0, #8]
007e9114: cmp      sb, r3
007e9118: beq      #0x7e91bc
007e911c: mov      r0, #0x1000
007e9120: ldr      fp, [r5]
007e9124: bl       #0x7f34f4
007e9128: ldr      r3, [pc, #0xe4]
007e912c: add      sl, fp, sb, lsl #3
007e9130: str      r0, [sl, #4]
007e9134: ldr      r3, [r4, r3]
007e9138: mov      r6, r0
007e913c: mov      r0, #0x1000
007e9140: ldr      r4, [r3, r8, lsl #2]
007e9144: str      r4, [fp, sb, lsl #3]
007e9148: mov      r1, r4
007e914c: bl       #0x30e2a4
007e9150: sub      ip, r0, #1
007e9154: cmp      ip, #0
007e9158: ble      #0x7e918c
007e915c: mov      r3, #0
007e9160: mov      r2, #1
007e9164: b        #0x7e916c
007e9168: ldr      r6, [sl, #4]
007e916c: add      r1, r6, r3
007e9170: add      r2, r2, #1
007e9174: add      r3, r3, r4
007e9178: add      r6, r6, r3
007e917c: cmp      r2, r0
007e9180: str      r6, [r1]
007e9184: bne      #0x7e9168
007e9188: ldr      r6, [sl, #4]
007e918c: mul      r4, r4, ip
007e9190: mov      r3, #0
007e9194: str      r3, [r6, r4]
007e9198: ldr      r3, [sl, #4]
007e919c: add      r7, r5, r7, lsl #2
007e91a0: ldr      r3, [r3]
007e91a4: str      r3, [r7, #4]
007e91a8: ldr      r3, [r5, #4]
007e91ac: add      r3, r3, #1
007e91b0: str      r3, [r5, #4]
007e91b4: ldr      r0, [sl, #4]
007e91b8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e91bc: add      r0, sb, #0x80
007e91c0: str      r0, [r5, #8]
007e91c4: lsl      r0, r0, #3
007e91c8: ldr      sl, [r5]
007e91cc: bl       #0x7f34f4
007e91d0: ldr      r2, [r5, #4]
007e91d4: mov      r1, sl
007e91d8: str      r0, [r5]
007e91dc: lsl      r2, r2, #3
007e91e0: bl       #0x30e868
007e91e4: ldr      r3, [r5]
007e91e8: ldr      r0, [r5, #4]
007e91ec: mov      r1, r6
007e91f0: mov      r2, #0x400
007e91f4: add      r0, r3, r0, lsl #3
007e91f8: bl       #0x30e460
007e91fc: mov      r0, sl
007e9200: bl       #0x7f34bc
007e9204: ldr      sb, [r5, #4]
007e9208: b        #0x7e911c
007e920c: andseq   fp, sl, r4, asr #19
007e9210: strdeq   r1, r2, [r0], -ip
007e9214: strheq   r2, [r0], -r8

# _ZN13b2PairManager14ValidateBufferEv
007e418c: bx       lr

# _ZN7b2World9DrawJointEP7b2Joint
007e69b0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e69b4: ldr      ip, [r1, #0x30]
007e69b8: sub      sp, sp, #0x40
007e69bc: ldr      r2, [r1, #0x34]
007e69c0: ldr      lr, [ip, #4]
007e69c4: ldr      r3, [r1]
007e69c8: add      r5, sp, #0x28
007e69cc: str      lr, [sp, #0x38]
007e69d0: ldr      ip, [ip, #8]
007e69d4: mov      sb, r1
007e69d8: mov      r6, r0
007e69dc: str      ip, [sp, #0x3c]
007e69e0: ldr      ip, [r2, #4]
007e69e4: mov      r0, r5
007e69e8: add      r4, sp, #0x20
007e69ec: str      ip, [sp, #0x30]
007e69f0: ldr      r2, [r2, #8]
007e69f4: str      r2, [sp, #0x34]
007e69f8: mov      lr, pc
007e69fc: ldr      pc, [r3]
007e6a00: ldr      r3, [sb]
007e6a04: mov      r1, sb
007e6a08: mov      r0, r4
007e6a0c: mov      lr, pc
007e6a10: ldr      pc, [r3, #4]
007e6a14: ldr      r3, [sb, #4]
007e6a18: movw     r2, #0xcccd
007e6a1c: movt     r2, #0x3f4c
007e6a20: mov      r1, #0x3f000000
007e6a24: cmp      r3, #4
007e6a28: str      r2, [sp, #0xc]
007e6a2c: str      r1, [sp, #4]
007e6a30: str      r2, [sp, #8]
007e6a34: beq      #0x7e6ae8
007e6a38: cmp      r3, #5
007e6a3c: beq      #0x7e6ab4
007e6a40: cmp      r3, #3
007e6a44: beq      #0x7e6abc
007e6a48: mov      r7, #0x19000
007e6a4c: add      r7, r7, #0x268
007e6a50: ldr      ip, [r6, r7]
007e6a54: add      r8, sp, #4
007e6a58: add      r1, sp, #0x38
007e6a5c: mov      r0, ip
007e6a60: mov      r2, r5
007e6a64: mov      r3, r8
007e6a68: ldr      ip, [ip]
007e6a6c: mov      lr, pc
007e6a70: ldr      pc, [ip, #0x18]
007e6a74: ldr      ip, [r6, r7]
007e6a78: mov      r1, r5
007e6a7c: mov      r2, r4
007e6a80: mov      r0, ip
007e6a84: mov      r3, r8
007e6a88: ldr      ip, [ip]
007e6a8c: mov      lr, pc
007e6a90: ldr      pc, [ip, #0x18]
007e6a94: ldr      r1, [r6, r7]
007e6a98: mov      r2, r4
007e6a9c: mov      r3, r8
007e6aa0: mov      r0, r1
007e6aa4: ldr      ip, [r1]
007e6aa8: add      r1, sp, #0x30
007e6aac: mov      lr, pc
007e6ab0: ldr      pc, [ip, #0x18]
007e6ab4: add      sp, sp, #0x40
007e6ab8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e6abc: mov      r3, #0x19000
007e6ac0: add      r3, r3, #0x268
007e6ac4: ldr      r3, [r6, r3]
007e6ac8: mov      r1, r5
007e6acc: mov      r2, r4
007e6ad0: mov      r0, r3
007e6ad4: ldr      ip, [r3]
007e6ad8: add      r3, sp, #4
007e6adc: mov      lr, pc
007e6ae0: ldr      pc, [ip, #0x18]
007e6ae4: b        #0x7e6ab4
007e6ae8: add      r8, sp, #0x18
007e6aec: add      sl, sp, #0x10
007e6af0: mov      r0, r8
007e6af4: mov      r1, sb
007e6af8: mov      r7, #0x19000
007e6afc: bl       #0x7f0478
007e6b00: add      r7, r7, #0x268
007e6b04: mov      r1, sb
007e6b08: mov      r0, sl
007e6b0c: bl       #0x7f04b4
007e6b10: ldr      ip, [r6, r7]
007e6b14: add      sb, sp, #4
007e6b18: mov      r2, r5
007e6b1c: mov      r0, ip
007e6b20: mov      r1, r8
007e6b24: mov      r3, sb
007e6b28: ldr      ip, [ip]
007e6b2c: mov      lr, pc
007e6b30: ldr      pc, [ip, #0x18]
007e6b34: ldr      ip, [r6, r7]
007e6b38: mov      r2, r4
007e6b3c: mov      r1, sl
007e6b40: mov      r0, ip
007e6b44: mov      r3, sb
007e6b48: ldr      ip, [ip]
007e6b4c: mov      lr, pc
007e6b50: ldr      pc, [ip, #0x18]
007e6b54: ldr      ip, [r6, r7]
007e6b58: mov      r1, r8
007e6b5c: mov      r2, sl
007e6b60: mov      r0, ip
007e6b64: mov      r3, sb
007e6b68: ldr      ip, [ip]
007e6b6c: mov      lr, pc
007e6b70: ldr      pc, [ip, #0x18]
007e6b74: b        #0x7e6ab4

# _ZN16b2PrismaticJoint16SetMaxMotorForceEf
007eefe8: str      r1, [r0, #0xc0]
007eefec: bx       lr

# _ZN12b2MouseJoint24SolvePositionConstraintsEv
007eb478: mov      r0, #1
007eb47c: bx       lr

# _ZN7b2Sweep7AdvanceEf
007e3a94: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e3a98: ldr      r5, [r0, #0x20]
007e3a9c: mov      r4, r0
007e3aa0: mov      r6, r1
007e3aa4: mov      r0, r5
007e3aa8: bl       #0x30e70c
007e3aac: cmp      r0, #0
007e3ab0: beq      #0x7e3b84
007e3ab4: mov      r1, r5
007e3ab8: mov      r0, #0x3f800000
007e3abc: bl       #0x30e3ac
007e3ac0: mov      r1, #0x34000000
007e3ac4: mov      r7, r0
007e3ac8: bl       #0x30e2f8
007e3acc: cmp      r0, #0
007e3ad0: beq      #0x7e3b84
007e3ad4: mov      r1, r5
007e3ad8: mov      r0, r6
007e3adc: bl       #0x30e3ac
007e3ae0: mov      r1, r7
007e3ae4: bl       #0x30ec94
007e3ae8: mov      r5, r0
007e3aec: mov      r1, r0
007e3af0: mov      r0, #0x3f800000
007e3af4: bl       #0x30e3ac
007e3af8: ldr      r1, [r4, #8]
007e3afc: mov      r7, r0
007e3b00: bl       #0x30ed6c
007e3b04: ldr      r1, [r4, #0xc]
007e3b08: mov      r8, r0
007e3b0c: mov      r0, r7
007e3b10: bl       #0x30ed6c
007e3b14: ldr      r1, [r4, #0x10]
007e3b18: mov      sb, r0
007e3b1c: mov      r0, r5
007e3b20: bl       #0x30ed6c
007e3b24: ldr      r1, [r4, #0x14]
007e3b28: mov      sl, r0
007e3b2c: mov      r0, r5
007e3b30: bl       #0x30ed6c
007e3b34: mov      r1, r0
007e3b38: mov      r0, sb
007e3b3c: bl       #0x30eba4
007e3b40: mov      r1, sl
007e3b44: str      r0, [r4, #0xc]
007e3b48: mov      r0, r8
007e3b4c: bl       #0x30eba4
007e3b50: ldr      r1, [r4, #0x18]
007e3b54: str      r0, [r4, #8]
007e3b58: mov      r0, r7
007e3b5c: bl       #0x30ed6c
007e3b60: ldr      r1, [r4, #0x1c]
007e3b64: mov      r7, r0
007e3b68: mov      r0, r5
007e3b6c: bl       #0x30ed6c
007e3b70: mov      r1, r0
007e3b74: mov      r0, r7
007e3b78: bl       #0x30eba4
007e3b7c: str      r6, [r4, #0x20]
007e3b80: str      r0, [r4, #0x18]
007e3b84: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK14b2PolygonShape11ComputeMassEP10b2MassData
007e4e8c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e4e90: sub      sp, sp, #0x2c
007e4e94: str      r0, [sp, #0x1c]
007e4e98: ldr      r2, [r0, #0x118]
007e4e9c: str      r1, [sp, #0x20]
007e4ea0: cmp      r2, #0
007e4ea4: str      r2, [sp, #0xc]
007e4ea8: ble      #0x7e51c8
007e4eac: ldr      r3, [sp, #0x1c]
007e4eb0: ldr      sl, [sp, #0x1c]
007e4eb4: mov      fp, #0
007e4eb8: add      r3, r3, #0x58
007e4ebc: str      r3, [sp, #0x24]
007e4ec0: str      fp, [sp, #0x10]
007e4ec4: mov      r8, #0
007e4ec8: str      fp, [sp, #0x14]
007e4ecc: str      fp, [sp, #8]
007e4ed0: ldr      r2, [sp, #0xc]
007e4ed4: add      r8, r8, #1
007e4ed8: ldr      r7, [sl, #0x58]
007e4edc: cmp      r8, r2
007e4ee0: ldrlt    r2, [sp, #0x1c]
007e4ee4: ldrge    r3, [sp, #0x24]
007e4ee8: addlt    r3, r8, #0xb
007e4eec: addlt    r3, r2, r3, lsl #3
007e4ef0: ldr      r4, [r3, #4]
007e4ef4: mov      r0, r7
007e4ef8: ldr      r6, [r3]
007e4efc: mov      r1, r4
007e4f00: bl       #0x30ed6c
007e4f04: ldr      r5, [sl, #0x5c]
007e4f08: mov      sb, r0
007e4f0c: mov      r1, r6
007e4f10: mov      r0, r5
007e4f14: bl       #0x30ed6c
007e4f18: mov      r1, r0
007e4f1c: mov      r0, sb
007e4f20: bl       #0x30e3ac
007e4f24: mov      r1, #0x3f000000
007e4f28: str      r0, [sp, #0x18]
007e4f2c: bl       #0x30ed6c
007e4f30: mov      sb, r0
007e4f34: mov      r1, sb
007e4f38: ldr      r0, [sp, #8]
007e4f3c: bl       #0x30eba4
007e4f40: movw     r1, #0xaaab
007e4f44: str      r0, [sp, #8]
007e4f48: movt     r1, #0x3eaa
007e4f4c: mov      r0, sb
007e4f50: bl       #0x30ed6c
007e4f54: mov      r1, #0
007e4f58: mov      sb, r0
007e4f5c: mov      r0, r7
007e4f60: bl       #0x30eba4
007e4f64: mov      r1, #0
007e4f68: mov      r3, r0
007e4f6c: mov      r0, r5
007e4f70: str      r3, [sp, #4]
007e4f74: bl       #0x30eba4
007e4f78: ldr      r3, [sp, #4]
007e4f7c: mov      r2, r0
007e4f80: mov      r1, r6
007e4f84: mov      r0, r3
007e4f88: str      r2, [sp, #4]
007e4f8c: bl       #0x30eba4
007e4f90: ldr      r2, [sp, #4]
007e4f94: mov      ip, r0
007e4f98: mov      r1, r4
007e4f9c: mov      r0, r2
007e4fa0: str      ip, [sp, #4]
007e4fa4: bl       #0x30eba4
007e4fa8: ldr      ip, [sp, #4]
007e4fac: mov      r3, r0
007e4fb0: mov      r0, sb
007e4fb4: mov      r1, ip
007e4fb8: str      r3, [sp, #4]
007e4fbc: bl       #0x30ed6c
007e4fc0: mov      r1, r0
007e4fc4: mov      r0, fp
007e4fc8: bl       #0x30eba4
007e4fcc: ldr      r3, [sp, #4]
007e4fd0: mov      fp, r0
007e4fd4: mov      r0, sb
007e4fd8: mov      r1, r3
007e4fdc: bl       #0x30ed6c
007e4fe0: mov      r1, r0
007e4fe4: ldr      r0, [sp, #0x10]
007e4fe8: bl       #0x30eba4
007e4fec: mov      r1, r7
007e4ff0: str      r0, [sp, #0x10]
007e4ff4: mov      r0, r7
007e4ff8: bl       #0x30ed6c
007e4ffc: mov      r1, r6
007e5000: mov      sb, r0
007e5004: mov      r0, r7
007e5008: bl       #0x30ed6c
007e500c: mov      r1, r0
007e5010: mov      r0, sb
007e5014: bl       #0x30eba4
007e5018: mov      r1, r6
007e501c: mov      sb, r0
007e5020: mov      r0, r6
007e5024: bl       #0x30ed6c
007e5028: mov      r1, r0
007e502c: mov      r0, sb
007e5030: bl       #0x30eba4
007e5034: mov      r1, #0x3e800000
007e5038: bl       #0x30ed6c
007e503c: mov      r1, #0
007e5040: mov      sb, r0
007e5044: mov      r0, r7
007e5048: bl       #0x30ed6c
007e504c: mov      r1, #0
007e5050: mov      r7, r0
007e5054: mov      r0, r6
007e5058: bl       #0x30ed6c
007e505c: mov      r1, r0
007e5060: mov      r0, r7
007e5064: bl       #0x30eba4
007e5068: mov      r1, r0
007e506c: mov      r0, sb
007e5070: bl       #0x30eba4
007e5074: movw     r1, #0xaaab
007e5078: movt     r1, #0x3eaa
007e507c: bl       #0x30ed6c
007e5080: mov      r1, #0
007e5084: bl       #0x30eba4
007e5088: mov      r1, r5
007e508c: mov      r6, r0
007e5090: mov      r0, r5
007e5094: bl       #0x30ed6c
007e5098: mov      r1, r4
007e509c: mov      r7, r0
007e50a0: mov      r0, r5
007e50a4: bl       #0x30ed6c
007e50a8: mov      r1, r0
007e50ac: mov      r0, r7
007e50b0: bl       #0x30eba4
007e50b4: mov      r1, r4
007e50b8: mov      r7, r0
007e50bc: mov      r0, r4
007e50c0: bl       #0x30ed6c
007e50c4: mov      r1, r0
007e50c8: mov      r0, r7
007e50cc: bl       #0x30eba4
007e50d0: mov      r1, #0x3e800000
007e50d4: bl       #0x30ed6c
007e50d8: mov      r1, #0
007e50dc: mov      r7, r0
007e50e0: mov      r0, r5
007e50e4: bl       #0x30ed6c
007e50e8: mov      r1, #0
007e50ec: mov      r5, r0
007e50f0: mov      r0, r4
007e50f4: bl       #0x30ed6c
007e50f8: mov      r1, r0
007e50fc: mov      r0, r5
007e5100: bl       #0x30eba4
007e5104: mov      r1, r0
007e5108: mov      r0, r7
007e510c: bl       #0x30eba4
007e5110: movw     r1, #0xaaab
007e5114: movt     r1, #0x3eaa
007e5118: bl       #0x30ed6c
007e511c: mov      r1, #0
007e5120: bl       #0x30eba4
007e5124: mov      r1, r0
007e5128: mov      r0, r6
007e512c: bl       #0x30eba4
007e5130: ldr      r1, [sp, #0x18]
007e5134: bl       #0x30ed6c
007e5138: mov      r1, r0
007e513c: ldr      r0, [sp, #0x14]
007e5140: bl       #0x30eba4
007e5144: str      r0, [sp, #0x14]
007e5148: ldr      r3, [sp, #0xc]
007e514c: add      sl, sl, #8
007e5150: cmp      r8, r3
007e5154: bne      #0x7e4ed0
007e5158: ldr      r2, [sp, #0x1c]
007e515c: ldr      r1, [sp, #8]
007e5160: ldr      r0, [r2, #0x14]
007e5164: bl       #0x30ed6c
007e5168: ldr      r3, [sp, #0x20]
007e516c: str      r0, [r3]
007e5170: ldr      r1, [sp, #8]
007e5174: mov      r0, #0x3f800000
007e5178: bl       #0x30ec94
007e517c: mov      r4, r0
007e5180: mov      r1, r4
007e5184: ldr      r0, [sp, #0x10]
007e5188: bl       #0x30ed6c
007e518c: ldr      r2, [sp, #0x20]
007e5190: mov      r1, fp
007e5194: str      r0, [r2, #8]
007e5198: mov      r0, r4
007e519c: bl       #0x30ed6c
007e51a0: ldr      r3, [sp, #0x20]
007e51a4: str      r0, [r3, #4]
007e51a8: ldr      r2, [sp, #0x1c]
007e51ac: ldr      r1, [sp, #0x14]
007e51b0: ldr      r0, [r2, #0x14]
007e51b4: bl       #0x30ed6c
007e51b8: ldr      r3, [sp, #0x20]
007e51bc: str      r0, [r3, #0xc]
007e51c0: add      sp, sp, #0x2c
007e51c4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e51c8: mov      fp, #0
007e51cc: str      fp, [sp, #0x10]
007e51d0: str      fp, [sp, #0x14]
007e51d4: str      fp, [sp, #8]
007e51d8: b        #0x7e5158

# _ZN13b2NullContactD0Ev
007e807c: push     {r4, lr}
007e8080: mov      r4, r0
007e8084: bl       #0x30e2b0
007e8088: mov      r0, r4
007e808c: pop      {r4, pc}

# _ZN22b2PolyAndCircleContactC1EP7b2ShapeS1_
007ebed0: push     {r4, r5, r6, lr}
007ebed4: ldr      r5, [pc, #0x34]
007ebed8: mov      r4, r0
007ebedc: bl       #0x7e9eb8
007ebee0: ldr      r3, [pc, #0x2c]
007ebee4: add      r5, pc, r5
007ebee8: mov      r2, #0
007ebeec: ldr      r3, [r5, r3]
007ebef0: mov      r1, #0
007ebef4: str      r1, [r4, #0x90]
007ebef8: add      r3, r3, #8
007ebefc: str      r3, [r4]
007ebf00: str      r2, [r4, #0x60]
007ebf04: str      r2, [r4, #0x5c]
007ebf08: mov      r0, r4
007ebf0c: pop      {r4, r5, r6, pc}
007ebf10: andseq   r8, sl, ip, lsr #23
007ebf14: strheq   r3, [r0], -r8

# _ZN7b2World5QueryERK6b2AABBPP7b2Shapei
007e759c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e75a0: add      r6, r0, #0x44
007e75a4: mov      r7, r0
007e75a8: mov      sl, r1
007e75ac: mov      r0, r6
007e75b0: lsl      r1, r3, #2
007e75b4: mov      r8, r3
007e75b8: mov      r5, r2
007e75bc: bl       #0x7f3644
007e75c0: mov      r3, #0x19000
007e75c4: mov      r4, r0
007e75c8: add      r3, r3, #0x1d8
007e75cc: ldr      r0, [r7, r3]
007e75d0: mov      r1, sl
007e75d4: mov      r3, r8
007e75d8: mov      r2, r4
007e75dc: bl       #0x7e29a0
007e75e0: subs     r7, r0, #0
007e75e4: ble      #0x7e7608
007e75e8: mov      r3, #0
007e75ec: mov      r2, r3
007e75f0: ldr      r1, [r4, r3]
007e75f4: add      r2, r2, #1
007e75f8: cmp      r2, r7
007e75fc: str      r1, [r5, r3]
007e7600: add      r3, r3, #4
007e7604: bne      #0x7e75f0
007e7608: mov      r0, r6
007e760c: mov      r1, r4
007e7610: bl       #0x7f35a8
007e7614: mov      r0, r7
007e7618: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN8b2Island5SolveERK10b2TimeStepRK6b2Vec2bb
007ea9d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ea9d4: mov      r4, r0
007ea9d8: sub      sp, sp, #0x44
007ea9dc: ldr      r6, [r0, #0x14]
007ea9e0: ldr      r0, [pc, #0x688]
007ea9e4: mov      r5, r1
007ea9e8: ldrb     r1, [sp, #0x68]
007ea9ec: add      r0, pc, r0
007ea9f0: cmp      r6, #0
007ea9f4: str      r0, [sp, #0x14]
007ea9f8: str      r2, [sp, #0x10]
007ea9fc: str      r3, [sp, #0x18]
007eaa00: str      r1, [sp, #0x1c]
007eaa04: ble      #0x7eac4c
007eaa08: mov      sl, #0
007eaa0c: mov      r8, #0
007eaa10: b        #0x7eaa50
007eaa14: mov      r1, #0
007eaa18: mov      r0, r6
007eaa1c: bl       #0x30e70c
007eaa20: cmp      r0, #0
007eaa24: moveq    r1, #0x43000000
007eaa28: addeq    r1, r1, #0x7a0000
007eaa2c: streq    r1, [r7, #0x48]
007eaa30: beq      #0x7eac3c
007eaa34: mov      r0, #0xc3000000
007eaa38: add      r0, r0, #0x7a0000
007eaa3c: str      r0, [r7, #0x48]
007eaa40: ldr      r6, [r4, #0x14]
007eaa44: add      r8, r8, #1
007eaa48: cmp      r6, r8
007eaa4c: ble      #0x7eac4c
007eaa50: ldr      r3, [r4, #8]
007eaa54: ldr      r7, [r3, r8, lsl #2]
007eaa58: ldrsh    r3, [r7, #2]
007eaa5c: cmp      r3, #0
007eaa60: beq      #0x7eaa44
007eaa64: ldr      fp, [r7, #0x78]
007eaa68: ldr      r1, [r7, #0x4c]
007eaa6c: ldr      r6, [r5]
007eaa70: mov      r0, fp
007eaa74: bl       #0x30ed6c
007eaa78: ldr      r1, [r7, #0x50]
007eaa7c: mov      sb, r0
007eaa80: mov      r0, fp
007eaa84: bl       #0x30ed6c
007eaa88: ldr      r3, [sp, #0x10]
007eaa8c: mov      fp, r0
007eaa90: mov      r0, sb
007eaa94: ldr      r1, [r3]
007eaa98: bl       #0x30eba4
007eaa9c: ldr      r3, [sp, #0x10]
007eaaa0: mov      sb, r0
007eaaa4: mov      r0, fp
007eaaa8: ldr      r1, [r3, #4]
007eaaac: bl       #0x30eba4
007eaab0: mov      r1, sb
007eaab4: mov      fp, r0
007eaab8: mov      r0, r6
007eaabc: bl       #0x30ed6c
007eaac0: ldr      r1, [r7, #0x40]
007eaac4: bl       #0x30eba4
007eaac8: mov      r1, fp
007eaacc: str      r0, [r7, #0x40]
007eaad0: mov      sb, r0
007eaad4: mov      r0, r6
007eaad8: bl       #0x30ed6c
007eaadc: ldr      r1, [r7, #0x44]
007eaae0: bl       #0x30eba4
007eaae4: str      r0, [r7, #0x44]
007eaae8: ldr      r1, [r7, #0x80]
007eaaec: mov      fp, r0
007eaaf0: ldr      r0, [r5]
007eaaf4: bl       #0x30ed6c
007eaaf8: ldr      r1, [r7, #0x54]
007eaafc: bl       #0x30ed6c
007eab00: ldr      r1, [r7, #0x48]
007eab04: bl       #0x30eba4
007eab08: str      r0, [sp, #0xc]
007eab0c: str      r0, [r7, #0x48]
007eab10: str      sl, [r7, #0x4c]
007eab14: str      sl, [r7, #0x50]
007eab18: str      sl, [r7, #0x54]
007eab1c: ldr      r1, [r7, #0x84]
007eab20: ldr      r0, [r5]
007eab24: bl       #0x30ed6c
007eab28: mov      r1, r0
007eab2c: mov      r0, #0x3f800000
007eab30: bl       #0x30e3ac
007eab34: mov      r1, #0x3f800000
007eab38: mov      r6, r0
007eab3c: bl       #0x30e70c
007eab40: cmp      r0, #0
007eab44: moveq    r6, #0x3f800000
007eab48: beq      #0x7eab60
007eab4c: mov      r0, r6
007eab50: mov      r1, sl
007eab54: bl       #0x30e70c
007eab58: cmp      r0, #0
007eab5c: movne    r6, sl
007eab60: mov      r1, r6
007eab64: mov      r0, sb
007eab68: bl       #0x30ed6c
007eab6c: mov      r1, r6
007eab70: str      r0, [r7, #0x40]
007eab74: mov      sb, r0
007eab78: mov      r0, fp
007eab7c: bl       #0x30ed6c
007eab80: str      r0, [r7, #0x44]
007eab84: ldr      r1, [r7, #0x88]
007eab88: mov      fp, r0
007eab8c: ldr      r0, [r5]
007eab90: bl       #0x30ed6c
007eab94: mov      r1, r0
007eab98: mov      r0, #0x3f800000
007eab9c: bl       #0x30e3ac
007eaba0: mov      r1, #0x3f800000
007eaba4: mov      r6, r0
007eaba8: bl       #0x30e70c
007eabac: cmp      r0, #0
007eabb0: moveq    r6, #0x3f800000
007eabb4: beq      #0x7eabcc
007eabb8: mov      r0, r6
007eabbc: mov      r1, #0
007eabc0: bl       #0x30e70c
007eabc4: cmp      r0, #0
007eabc8: movne    r6, #0
007eabcc: mov      r1, r6
007eabd0: ldr      r0, [sp, #0xc]
007eabd4: bl       #0x30ed6c
007eabd8: mov      r6, r0
007eabdc: mov      r1, sb
007eabe0: mov      r0, sb
007eabe4: str      r6, [r7, #0x48]
007eabe8: bl       #0x30ed6c
007eabec: mov      r1, fp
007eabf0: mov      sb, r0
007eabf4: mov      r0, fp
007eabf8: bl       #0x30ed6c
007eabfc: mov      r1, r0
007eac00: mov      r0, sb
007eac04: bl       #0x30eba4
007eac08: mov      r1, #0x47000000
007eac0c: add      r1, r1, #0x1c4000
007eac10: bl       #0x30e2f8
007eac14: cmp      r0, #0
007eac18: bne      #0x7eb038
007eac1c: mov      r1, r6
007eac20: mov      r0, r6
007eac24: bl       #0x30ed6c
007eac28: movw     r1, #0x2400
007eac2c: movt     r1, #0x4774
007eac30: bl       #0x30e2f8
007eac34: cmp      r0, #0
007eac38: bne      #0x7eaa14
007eac3c: ldr      r6, [r4, #0x14]
007eac40: add      r8, r8, #1
007eac44: cmp      r6, r8
007eac48: bgt      #0x7eaa50
007eac4c: ldr      ip, [r4]
007eac50: add      r7, sp, #0x20
007eac54: ldr      r3, [r4, #0x1c]
007eac58: ldr      r2, [r4, #0xc]
007eac5c: mov      r1, r5
007eac60: mov      r0, r7
007eac64: str      ip, [sp]
007eac68: bl       #0x7f7058
007eac6c: mov      r0, r7
007eac70: mov      r1, r5
007eac74: bl       #0x7f6210
007eac78: ldr      r3, [r4, #0x18]
007eac7c: cmp      r3, #0
007eac80: ble      #0x7eacb4
007eac84: mov      r6, #0
007eac88: ldr      r3, [r4, #0x10]
007eac8c: mov      r1, r5
007eac90: ldr      r3, [r3, r6, lsl #2]
007eac94: add      r6, r6, #1
007eac98: mov      r0, r3
007eac9c: ldr      r3, [r3]
007eaca0: mov      lr, pc
007eaca4: ldr      pc, [r3, #0x18]
007eaca8: ldr      r3, [r4, #0x18]
007eacac: cmp      r3, r6
007eacb0: bgt      #0x7eac88
007eacb4: ldr      r3, [r5, #0xc]
007eacb8: cmp      r3, #0
007eacbc: ble      #0x7ead18
007eacc0: mov      r8, #0
007eacc4: mov      r0, r7
007eacc8: bl       #0x7f64a8
007eaccc: ldr      r3, [r4, #0x18]
007eacd0: cmp      r3, #0
007eacd4: ble      #0x7ead08
007eacd8: mov      r6, #0
007eacdc: ldr      r3, [r4, #0x10]
007eace0: mov      r1, r5
007eace4: ldr      r3, [r3, r6, lsl #2]
007eace8: add      r6, r6, #1
007eacec: mov      r0, r3
007eacf0: ldr      r3, [r3]
007eacf4: mov      lr, pc
007eacf8: ldr      pc, [r3, #0x1c]
007eacfc: ldr      r3, [r4, #0x18]
007ead00: cmp      r3, r6
007ead04: bgt      #0x7eacdc
007ead08: ldr      r3, [r5, #0xc]
007ead0c: add      r8, r8, #1
007ead10: cmp      r3, r8
007ead14: bgt      #0x7eacc4
007ead18: mov      r0, r7
007ead1c: bl       #0x7f6b20
007ead20: ldr      r6, [r4, #0x14]
007ead24: cmp      r6, #0
007ead28: ble      #0x7eadd0
007ead2c: mov      sl, #0
007ead30: ldr      r3, [r4, #8]
007ead34: ldr      r8, [r3, sl, lsl #2]
007ead38: add      sl, sl, #1
007ead3c: ldrsh    r3, [r8, #2]
007ead40: cmp      r3, #0
007ead44: beq      #0x7eadc8
007ead48: ldr      r2, [r8, #0x2c]
007ead4c: ldr      r3, [r8, #0x30]
007ead50: ldr      r6, [r8, #0x38]
007ead54: str      r2, [r8, #0x24]
007ead58: str      r3, [r8, #0x28]
007ead5c: str      r6, [r8, #0x34]
007ead60: ldr      sb, [r5]
007ead64: ldr      r1, [r8, #0x44]
007ead68: mov      r0, sb
007ead6c: bl       #0x30ed6c
007ead70: ldr      r1, [r8, #0x40]
007ead74: mov      fp, r0
007ead78: mov      r0, sb
007ead7c: bl       #0x30ed6c
007ead80: mov      r1, r0
007ead84: ldr      r0, [r8, #0x2c]
007ead88: bl       #0x30eba4
007ead8c: mov      r1, fp
007ead90: str      r0, [r8, #0x2c]
007ead94: ldr      r0, [r8, #0x30]
007ead98: bl       #0x30eba4
007ead9c: str      r0, [r8, #0x30]
007eada0: ldr      r0, [r5]
007eada4: ldr      r1, [r8, #0x48]
007eada8: bl       #0x30ed6c
007eadac: mov      r1, r0
007eadb0: mov      r0, r6
007eadb4: bl       #0x30eba4
007eadb8: str      r0, [r8, #0x38]
007eadbc: mov      r0, r8
007eadc0: bl       #0x7e761c
007eadc4: ldr      r6, [r4, #0x14]
007eadc8: cmp      r6, sl
007eadcc: bgt      #0x7ead30
007eadd0: ldr      r3, [sp, #0x18]
007eadd4: cmp      r3, #0
007eadd8: beq      #0x7eaeb0
007eaddc: ldr      r3, [r4, #0x18]
007eade0: cmp      r3, #0
007eade4: ble      #0x7eae14
007eade8: mov      r6, #0
007eadec: ldr      r3, [r4, #0x10]
007eadf0: ldr      r3, [r3, r6, lsl #2]
007eadf4: add      r6, r6, #1
007eadf8: mov      r0, r3
007eadfc: ldr      r3, [r3]
007eae00: mov      lr, pc
007eae04: ldr      pc, [r3, #0x20]
007eae08: ldr      r3, [r4, #0x18]
007eae0c: cmp      r3, r6
007eae10: bgt      #0x7eadec
007eae14: mov      r3, #0
007eae18: str      r3, [r4, #0x2c]
007eae1c: ldr      r3, [r5, #0xc]
007eae20: cmp      r3, #0
007eae24: ble      #0x7eaeb0
007eae28: movw     r1, #0xcccd
007eae2c: mov      r0, r7
007eae30: movt     r1, #0x3e4c
007eae34: bl       #0x7f6b9c
007eae38: ldr      r3, [r4, #0x18]
007eae3c: mov      sl, r0
007eae40: cmp      r3, #0
007eae44: movle    r8, #1
007eae48: ble      #0x7eae88
007eae4c: mov      r6, #0
007eae50: mov      r8, #1
007eae54: ldr      r3, [r4, #0x10]
007eae58: ldr      r3, [r3, r6, lsl #2]
007eae5c: add      r6, r6, #1
007eae60: mov      r0, r3
007eae64: ldr      r3, [r3]
007eae68: mov      lr, pc
007eae6c: ldr      pc, [r3, #0x24]
007eae70: ldr      r3, [r4, #0x18]
007eae74: cmp      r8, #0
007eae78: movne    r8, r0
007eae7c: moveq    r8, #0
007eae80: cmp      r3, r6
007eae84: bgt      #0x7eae54
007eae88: cmp      sl, #0
007eae8c: beq      #0x7eae98
007eae90: cmp      r8, #0
007eae94: bne      #0x7eaeb0
007eae98: ldr      r3, [r4, #0x2c]
007eae9c: add      r3, r3, #1
007eaea0: str      r3, [r4, #0x2c]
007eaea4: ldr      r2, [r5, #0xc]
007eaea8: cmp      r2, r3
007eaeac: bgt      #0x7eae28
007eaeb0: mov      r0, r4
007eaeb4: ldr      r1, [sp, #0x38]
007eaeb8: bl       #0x7ea6a8
007eaebc: ldr      r0, [sp, #0x1c]
007eaec0: cmp      r0, #0
007eaec4: beq      #0x7eb028
007eaec8: ldr      r6, [r4, #0x14]
007eaecc: cmp      r6, #0
007eaed0: ble      #0x7eb028
007eaed4: mvn      fp, #0x80000000
007eaed8: sub      fp, fp, #0x800000
007eaedc: mov      sb, #0
007eaee0: mov      r8, #0
007eaee4: b        #0x7eaf00
007eaee8: str      sb, [sl, #0x8c]
007eaeec: ldr      r6, [r4, #0x14]
007eaef0: mov      fp, sb
007eaef4: add      r8, r8, #1
007eaef8: cmp      r6, r8
007eaefc: ble      #0x7eafc0
007eaf00: ldr      r3, [r4, #8]
007eaf04: mov      r1, #0
007eaf08: ldr      sl, [r3, r8, lsl #2]
007eaf0c: ldr      r0, [sl, #0x78]
007eaf10: bl       #0x30df8c
007eaf14: cmp      r0, #0
007eaf18: bne      #0x7eaef4
007eaf1c: ldrh     r3, [sl]
007eaf20: tst      r3, #0x10
007eaf24: streq    sb, [sl, #0x8c]
007eaf28: beq      #0x7eaee8
007eaf2c: ldr      r0, [sl, #0x48]
007eaf30: mov      r1, r0
007eaf34: bl       #0x30ed6c
007eaf38: movw     r1, #0x742e
007eaf3c: movt     r1, #0x3901
007eaf40: bl       #0x30e2f8
007eaf44: cmp      r0, #0
007eaf48: bne      #0x7eaee8
007eaf4c: ldr      r0, [sl, #0x40]
007eaf50: mov      r1, r0
007eaf54: bl       #0x30ed6c
007eaf58: mov      r6, r0
007eaf5c: ldr      r0, [sl, #0x44]
007eaf60: mov      r1, r0
007eaf64: bl       #0x30ed6c
007eaf68: mov      r1, r0
007eaf6c: mov      r0, r6
007eaf70: bl       #0x30eba4
007eaf74: movw     r1, #0xb717
007eaf78: movt     r1, #0x38d1
007eaf7c: bl       #0x30e2f8
007eaf80: cmp      r0, #0
007eaf84: bne      #0x7eaee8
007eaf88: ldr      r1, [r5]
007eaf8c: ldr      r0, [sl, #0x8c]
007eaf90: bl       #0x30eba4
007eaf94: mov      r6, r0
007eaf98: str      r0, [sl, #0x8c]
007eaf9c: mov      r1, r6
007eafa0: mov      r0, fp
007eafa4: bl       #0x30e70c
007eafa8: cmp      r0, #0
007eafac: moveq    fp, r6
007eafb0: ldr      r6, [r4, #0x14]
007eafb4: add      r8, r8, #1
007eafb8: cmp      r6, r8
007eafbc: bgt      #0x7eaf00
007eafc0: mov      r0, fp
007eafc4: mov      r1, #0x3f000000
007eafc8: bl       #0x30e4b4
007eafcc: cmp      r0, #0
007eafd0: beq      #0x7eb028
007eafd4: cmp      r6, #0
007eafd8: ble      #0x7eb028
007eafdc: ldr      r3, [pc, #0x90]
007eafe0: ldr      r0, [sp, #0x14]
007eafe4: mov      ip, #0
007eafe8: mov      r2, #0
007eafec: ldr      r1, [r0, r3]
007eaff0: ldr      r3, [r4, #8]
007eaff4: ldr      r3, [r3, r2, lsl #2]
007eaff8: add      r2, r2, #1
007eaffc: ldrh     r0, [r3]
007eb000: orr      r0, r0, #8
007eb004: strh     r0, [r3]
007eb008: ldr      r0, [r1]
007eb00c: str      r0, [r3, #0x40]
007eb010: ldr      r0, [r1, #4]
007eb014: str      ip, [r3, #0x48]
007eb018: str      r0, [r3, #0x44]
007eb01c: ldr      r3, [r4, #0x14]
007eb020: cmp      r3, r2
007eb024: bgt      #0x7eaff0
007eb028: mov      r0, r7
007eb02c: bl       #0x7f7020
007eb030: add      sp, sp, #0x44
007eb034: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007eb038: add      r0, r7, #0x40
007eb03c: bl       #0x7e5524
007eb040: mov      r1, #0x43000000
007eb044: ldr      r0, [r7, #0x40]
007eb048: add      r1, r1, #0x480000
007eb04c: bl       #0x30ed6c
007eb050: mov      r1, #0x43000000
007eb054: str      r0, [r7, #0x40]
007eb058: add      r1, r1, #0x480000
007eb05c: ldr      r0, [r7, #0x44]
007eb060: bl       #0x30ed6c
007eb064: ldr      r6, [r7, #0x48]
007eb068: str      r0, [r7, #0x44]
007eb06c: b        #0x7eac1c
007eb070: andseq   sl, sl, r4, lsr #1
007eb074: andeq    r0, r0, r0, asr #18

# _ZN8b2IslandD2Ev
007eb0ac: push     {r4, lr}
007eb0b0: mov      r4, r0
007eb0b4: ldr      r1, [r4, #0x10]
007eb0b8: ldr      r0, [r0]
007eb0bc: bl       #0x7f35a8
007eb0c0: ldr      r0, [r4]
007eb0c4: ldr      r1, [r4, #0xc]
007eb0c8: bl       #0x7f35a8
007eb0cc: ldr      r0, [r4]
007eb0d0: ldr      r1, [r4, #8]
007eb0d4: bl       #0x7f35a8
007eb0d8: mov      r0, r4
007eb0dc: pop      {r4, pc}

# _ZNK16b2PrismaticJoint10GetAnchor1Ev
007ee820: push     {r4, r5, r6, r7, r8, lr}
007ee824: ldr      r4, [r1, #0x30]
007ee828: ldr      r7, [r1, #0x44]
007ee82c: ldr      r6, [r1, #0x48]
007ee830: mov      r5, r0
007ee834: ldr      r1, [r4, #0xc]
007ee838: mov      r0, r7
007ee83c: bl       #0x30ed6c
007ee840: ldr      r1, [r4, #0x14]
007ee844: mov      r8, r0
007ee848: mov      r0, r6
007ee84c: bl       #0x30ed6c
007ee850: mov      r1, r0
007ee854: mov      r0, r8
007ee858: bl       #0x30eba4
007ee85c: ldr      r1, [r4, #0x10]
007ee860: mov      r8, r0
007ee864: mov      r0, r7
007ee868: bl       #0x30ed6c
007ee86c: ldr      r1, [r4, #0x18]
007ee870: mov      r7, r0
007ee874: mov      r0, r6
007ee878: bl       #0x30ed6c
007ee87c: mov      r1, r0
007ee880: mov      r0, r7
007ee884: bl       #0x30eba4
007ee888: ldr      r1, [r4, #8]
007ee88c: bl       #0x30eba4
007ee890: ldr      r1, [r4, #4]
007ee894: mov      r6, r0
007ee898: mov      r0, r8
007ee89c: bl       #0x30eba4
007ee8a0: str      r6, [r5, #4]
007ee8a4: str      r0, [r5]
007ee8a8: mov      r0, r5
007ee8ac: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12b2BroadPhase18IncrementTimeStampEv
007e276c: mov      r3, #0x5d000
007e2770: add      r3, r3, #0x38
007e2774: ldrh     r2, [r0, r3]
007e2778: movw     r1, #0xffff
007e277c: cmp      r2, r1
007e2780: addne    r2, r2, #1
007e2784: strhne   r2, [r0, r3]
007e2788: bxne     lr
007e278c: mov      r3, #0
007e2790: add      r2, r0, r3
007e2794: add      r3, r3, #0x10
007e2798: add      r2, r2, #0x48000
007e279c: mov      r1, #0
007e27a0: cmp      r3, #0x8000
007e27a4: strh     r1, [r2, #0x1e]
007e27a8: bne      #0x7e2790
007e27ac: mov      r3, #0x5d000
007e27b0: add      r3, r3, #0x38
007e27b4: mov      r2, #1
007e27b8: strh     r2, [r0, r3]
007e27bc: bx       lr

# _ZN13b2PulleyJointC1EPK16b2PulleyJointDef
007f0510: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007f0514: ldr      r6, [pc, #0x144]
007f0518: mov      r4, r0
007f051c: mov      r5, r1
007f0520: bl       #0x7eb1e0
007f0524: ldr      r2, [pc, #0x138]
007f0528: add      r6, pc, r6
007f052c: ldr      r1, [r4, #0x30]
007f0530: ldr      r2, [r6, r2]
007f0534: mov      r3, #0x19000
007f0538: add      r3, r3, #0x254
007f053c: add      r2, r2, #8
007f0540: str      r2, [r4]
007f0544: ldr      r2, [r1, #0x58]
007f0548: ldr      r6, [r2, r3]
007f054c: str      r6, [r4, #0x44]
007f0550: ldr      r1, [r6, #4]
007f0554: ldr      r0, [r5, #0x14]
007f0558: bl       #0x30e3ac
007f055c: ldr      r1, [r6, #8]
007f0560: mov      r7, r0
007f0564: ldr      r0, [r5, #0x18]
007f0568: bl       #0x30e3ac
007f056c: str      r7, [r4, #0x48]
007f0570: str      r0, [r4, #0x4c]
007f0574: ldr      r1, [r6, #4]
007f0578: ldr      r0, [r5, #0x1c]
007f057c: bl       #0x30e3ac
007f0580: ldr      r1, [r6, #8]
007f0584: mov      r7, r0
007f0588: ldr      r0, [r5, #0x20]
007f058c: bl       #0x30e3ac
007f0590: str      r7, [r4, #0x50]
007f0594: str      r0, [r4, #0x54]
007f0598: ldr      r3, [r5, #0x24]
007f059c: str      r3, [r4, #0x58]
007f05a0: ldr      r3, [r5, #0x28]
007f05a4: str      r3, [r4, #0x5c]
007f05a8: ldr      r3, [r5, #0x2c]
007f05ac: str      r3, [r4, #0x60]
007f05b0: ldr      r3, [r5, #0x30]
007f05b4: str      r3, [r4, #0x64]
007f05b8: ldr      r6, [r5, #0x44]
007f05bc: str      r6, [r4, #0x7c]
007f05c0: ldr      r1, [r5, #0x3c]
007f05c4: mov      r0, r6
007f05c8: bl       #0x30ed6c
007f05cc: ldr      r1, [r5, #0x34]
007f05d0: bl       #0x30eba4
007f05d4: mov      r1, #0xc0000000
007f05d8: mov      r7, r0
007f05dc: str      r0, [r4, #0x78]
007f05e0: mov      r0, r6
007f05e4: bl       #0x30ed6c
007f05e8: mov      r1, r0
007f05ec: mov      r0, r7
007f05f0: bl       #0x30eba4
007f05f4: ldr      r8, [r5, #0x38]
007f05f8: mov      sl, r0
007f05fc: mov      r1, sl
007f0600: mov      r0, r8
007f0604: bl       #0x30e70c
007f0608: cmp      r0, #0
007f060c: moveq    r8, sl
007f0610: str      r8, [r4, #0x80]
007f0614: mov      r0, r7
007f0618: mov      r1, #0x40000000
007f061c: bl       #0x30e3ac
007f0620: mov      r1, r6
007f0624: bl       #0x30ec94
007f0628: ldr      r5, [r5, #0x40]
007f062c: mov      r6, r0
007f0630: mov      r1, r6
007f0634: mov      r0, r5
007f0638: bl       #0x30e70c
007f063c: cmp      r0, #0
007f0640: mov      r3, #0
007f0644: moveq    r5, r6
007f0648: str      r5, [r4, #0x84]
007f064c: mov      r0, r4
007f0650: str      r3, [r4, #0x9c]
007f0654: str      r3, [r4, #0x94]
007f0658: str      r3, [r4, #0x98]
007f065c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007f0660: andseq   r4, sl, r8, ror #10
007f0664: strdeq   r3, r4, [r0], -r8

# _ZNK7b2Sweep8GetXFormEP7b2XFormf
007e3bfc: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e3c00: ldr      r6, [r0, #0x20]
007e3c04: mov      r5, r0
007e3c08: mov      r4, r1
007e3c0c: mov      r0, #0x3f800000
007e3c10: mov      r1, r6
007e3c14: mov      r8, r2
007e3c18: bl       #0x30e3ac
007e3c1c: mov      r1, #0x34000000
007e3c20: mov      r7, r0
007e3c24: bl       #0x30e2f8
007e3c28: cmp      r0, #0
007e3c2c: beq      #0x7e3d88
007e3c30: mov      r1, r6
007e3c34: mov      r0, r8
007e3c38: bl       #0x30e3ac
007e3c3c: mov      r1, r7
007e3c40: bl       #0x30ec94
007e3c44: mov      r6, r0
007e3c48: mov      r1, r0
007e3c4c: mov      r0, #0x3f800000
007e3c50: bl       #0x30e3ac
007e3c54: ldr      r1, [r5, #8]
007e3c58: mov      r7, r0
007e3c5c: bl       #0x30ed6c
007e3c60: ldr      r1, [r5, #0xc]
007e3c64: mov      fp, r0
007e3c68: mov      r0, r7
007e3c6c: bl       #0x30ed6c
007e3c70: ldr      r1, [r5, #0x10]
007e3c74: mov      sb, r0
007e3c78: mov      r0, r6
007e3c7c: bl       #0x30ed6c
007e3c80: ldr      r1, [r5, #0x14]
007e3c84: mov      sl, r0
007e3c88: mov      r0, r6
007e3c8c: bl       #0x30ed6c
007e3c90: mov      r1, sl
007e3c94: mov      r8, r0
007e3c98: mov      r0, fp
007e3c9c: bl       #0x30eba4
007e3ca0: mov      r1, r8
007e3ca4: mov      sl, r0
007e3ca8: mov      r0, sb
007e3cac: bl       #0x30eba4
007e3cb0: str      sl, [r4]
007e3cb4: str      r0, [r4, #4]
007e3cb8: ldr      r1, [r5, #0x18]
007e3cbc: mov      r0, r7
007e3cc0: bl       #0x30ed6c
007e3cc4: ldr      r1, [r5, #0x1c]
007e3cc8: mov      r7, r0
007e3ccc: mov      r0, r6
007e3cd0: bl       #0x30ed6c
007e3cd4: mov      r1, r0
007e3cd8: mov      r0, r7
007e3cdc: bl       #0x30eba4
007e3ce0: mov      r7, r0
007e3ce4: bl       #0x30e754
007e3ce8: mov      r6, r0
007e3cec: mov      r0, r7
007e3cf0: bl       #0x30eb08
007e3cf4: mov      r7, r0
007e3cf8: add      sb, r7, #0x80000000
007e3cfc: str      r7, [r4, #0xc]
007e3d00: str      r6, [r4, #8]
007e3d04: str      sb, [r4, #0x10]
007e3d08: str      r6, [r4, #0x14]
007e3d0c: ldr      r8, [r5]
007e3d10: mov      r1, r6
007e3d14: ldr      r5, [r5, #4]
007e3d18: mov      r0, r8
007e3d1c: bl       #0x30ed6c
007e3d20: mov      r1, sb
007e3d24: mov      fp, r0
007e3d28: mov      r0, r5
007e3d2c: bl       #0x30ed6c
007e3d30: mov      r1, r0
007e3d34: mov      r0, fp
007e3d38: bl       #0x30eba4
007e3d3c: mov      r1, r0
007e3d40: mov      r0, sl
007e3d44: bl       #0x30e3ac
007e3d48: mov      r1, r7
007e3d4c: str      r0, [r4]
007e3d50: mov      r0, r8
007e3d54: bl       #0x30ed6c
007e3d58: mov      r1, r6
007e3d5c: mov      r7, r0
007e3d60: mov      r0, r5
007e3d64: bl       #0x30ed6c
007e3d68: mov      r1, r0
007e3d6c: mov      r0, r7
007e3d70: bl       #0x30eba4
007e3d74: mov      r1, r0
007e3d78: ldr      r0, [r4, #4]
007e3d7c: bl       #0x30e3ac
007e3d80: str      r0, [r4, #4]
007e3d84: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e3d88: ldr      r3, [r5, #0x10]
007e3d8c: str      r3, [r4]
007e3d90: ldr      r3, [r5, #0x14]
007e3d94: str      r3, [r4, #4]
007e3d98: ldr      r7, [r5, #0x1c]
007e3d9c: mov      r0, r7
007e3da0: bl       #0x30e754
007e3da4: mov      r6, r0
007e3da8: mov      r0, r7
007e3dac: bl       #0x30eb08
007e3db0: ldr      sl, [r4]
007e3db4: mov      r7, r0
007e3db8: b        #0x7e3cf8

# _ZN7b2ShapeC2EPK10b2ShapeDef
007e6068: ldr      r2, [pc, #0x78]
007e606c: ldr      ip, [pc, #0x78]
007e6070: str      r4, [sp, #-4]!
007e6074: add      r2, pc, r2
007e6078: ldr      ip, [r2, ip]
007e607c: add      ip, ip, #8
007e6080: str      ip, [r0]
007e6084: ldr      r4, [r1, #8]
007e6088: mov      ip, #0
007e608c: str      r4, [r0, #0x2c]
007e6090: ldr      r2, [r1, #0xc]
007e6094: mov      r4, #0
007e6098: str      r2, [r0, #0x18]
007e609c: ldr      r2, [r1, #0x10]
007e60a0: str      r2, [r0, #0x1c]
007e60a4: ldr      r2, [r1, #0x14]
007e60a8: str      r4, [r0, #0x10]
007e60ac: str      ip, [r0, #8]
007e60b0: str      r2, [r0, #0x14]
007e60b4: mvn      r2, #0
007e60b8: str      ip, [r0, #0xc]
007e60bc: strh     r2, [r0, #0x20]
007e60c0: ldrh     r2, [r1, #0x1a]
007e60c4: strh     r2, [r0, #0x22]
007e60c8: ldrh     r2, [r1, #0x1c]
007e60cc: strh     r2, [r0, #0x24]
007e60d0: ldrh     r2, [r1, #0x1e]
007e60d4: strh     r2, [r0, #0x26]
007e60d8: ldrb     r2, [r1, #0x18]
007e60dc: strb     r2, [r0, #0x28]
007e60e0: ldm      sp!, {r4}
007e60e4: bx       lr
007e60e8: andseq   lr, sl, ip, lsl sl
007e60ec: andeq    r3, r0, ip, lsr #32

# _ZN7b2Shape13RefilterProxyEP12b2BroadPhaseRK7b2XForm
007e61b0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e61b4: ldrh     r3, [r0, #0x20]
007e61b8: mov      r4, r1
007e61bc: movw     r1, #0xffff
007e61c0: cmp      r3, r1
007e61c4: sub      sp, sp, #0x10
007e61c8: mov      r5, r0
007e61cc: mov      r7, r2
007e61d0: beq      #0x7e62b4
007e61d4: mov      r1, r3
007e61d8: mov      r0, r4
007e61dc: bl       #0x7e2acc
007e61e0: mov      r2, r7
007e61e4: mov      r0, r5
007e61e8: mov      r1, sp
007e61ec: ldr      r3, [r5]
007e61f0: mov      lr, pc
007e61f4: ldr      pc, [r3, #8]
007e61f8: mov      r3, #0x5d000
007e61fc: add      r3, r3, #0x24
007e6200: ldr      r1, [r4, r3]
007e6204: ldr      r0, [sp]
007e6208: bl       #0x30e3ac
007e620c: mov      r3, #0x5d000
007e6210: add      r3, r3, #0x28
007e6214: mov      r7, r0
007e6218: ldr      r1, [r4, r3]
007e621c: ldr      r0, [sp, #4]
007e6220: bl       #0x30e3ac
007e6224: mov      r3, #0x5d000
007e6228: add      r3, r3, #0x1c
007e622c: mov      r8, r0
007e6230: ldr      r1, [sp, #8]
007e6234: ldr      r0, [r4, r3]
007e6238: bl       #0x30e3ac
007e623c: mov      r3, #0x5d000
007e6240: add      r3, r3, #0x20
007e6244: mov      sb, r0
007e6248: ldr      r1, [sp, #0xc]
007e624c: ldr      r0, [r4, r3]
007e6250: bl       #0x30e3ac
007e6254: mov      r1, sb
007e6258: mov      sl, r0
007e625c: mov      r0, r7
007e6260: bl       #0x30e2f8
007e6264: mov      r1, sl
007e6268: cmp      r0, #0
007e626c: mov      r0, r8
007e6270: moveq    r7, sb
007e6274: bl       #0x30e2f8
007e6278: cmp      r0, #0
007e627c: moveq    r8, sl
007e6280: mov      r0, r7
007e6284: mov      r1, r8
007e6288: bl       #0x30e2f8
007e628c: cmp      r0, #0
007e6290: moveq    r7, r8
007e6294: mov      r0, r7
007e6298: mov      r1, #0
007e629c: bl       #0x30e70c
007e62a0: cmp      r0, #0
007e62a4: mvneq    r3, #0
007e62a8: mov      r6, sp
007e62ac: strheq   r3, [r5, #0x20]
007e62b0: bne      #0x7e62bc
007e62b4: add      sp, sp, #0x10
007e62b8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e62bc: mov      r0, r4
007e62c0: mov      r1, sp
007e62c4: mov      r2, r5
007e62c8: bl       #0x7e2d54
007e62cc: strh     r0, [r5, #0x20]
007e62d0: b        #0x7e62b4

# _ZN12b2BroadPhase13ComputeBoundsEPtS0_RK6b2AABB
007e25b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e25bc: mov      ip, #0x5d000
007e25c0: add      ip, ip, #0x24
007e25c4: ldr      ip, [r0, ip]
007e25c8: sub      sp, sp, #0x14
007e25cc: mov      r4, r0
007e25d0: str      ip, [sp, #4]
007e25d4: ldr      r5, [r3]
007e25d8: str      r1, [sp, #8]
007e25dc: mov      r1, ip
007e25e0: mov      r0, r5
007e25e4: str      r2, [sp, #0xc]
007e25e8: mov      r6, r3
007e25ec: bl       #0x30e70c
007e25f0: mov      r3, #0x5d000
007e25f4: add      r3, r3, #0x28
007e25f8: ldr      fp, [r4, r3]
007e25fc: ldr      r8, [r6, #4]
007e2600: cmp      r0, #0
007e2604: mov      r1, fp
007e2608: mov      r0, r8
007e260c: ldreq    r5, [sp, #4]
007e2610: bl       #0x30e70c
007e2614: mov      r3, #0x5d000
007e2618: add      r3, r3, #0x1c
007e261c: ldr      sb, [r4, r3]
007e2620: cmp      r0, #0
007e2624: mov      r1, r5
007e2628: mov      r0, sb
007e262c: moveq    r8, fp
007e2630: bl       #0x30e2f8
007e2634: mov      r3, #0x5d000
007e2638: add      r3, r3, #0x20
007e263c: ldr      r7, [r4, r3]
007e2640: cmp      r0, #0
007e2644: mov      r1, r8
007e2648: mov      r0, r7
007e264c: movne    r5, sb
007e2650: bl       #0x30e2f8
007e2654: ldr      sl, [r6, #8]
007e2658: cmp      r0, #0
007e265c: ldr      r0, [sp, #4]
007e2660: mov      r1, sl
007e2664: movne    r8, r7
007e2668: bl       #0x30e2f8
007e266c: ldr      r6, [r6, #0xc]
007e2670: cmp      r0, #0
007e2674: mov      r0, fp
007e2678: mov      r1, r6
007e267c: ldreq    sl, [sp, #4]
007e2680: bl       #0x30e2f8
007e2684: mov      r1, sl
007e2688: cmp      r0, #0
007e268c: mov      r0, sb
007e2690: moveq    r6, fp
007e2694: bl       #0x30e2f8
007e2698: mov      r1, r6
007e269c: cmp      r0, #0
007e26a0: mov      r0, r7
007e26a4: movne    sl, sb
007e26a8: bl       #0x30e2f8
007e26ac: cmp      r0, #0
007e26b0: moveq    r7, r6
007e26b4: mov      r6, #0x5d000
007e26b8: add      r6, r6, #0x2c
007e26bc: mov      r1, sb
007e26c0: mov      r0, r5
007e26c4: bl       #0x30e3ac
007e26c8: ldr      r1, [r4, r6]
007e26cc: bl       #0x30ed6c
007e26d0: bl       #0x8be2a0
007e26d4: ldr      r2, [sp, #8]
007e26d8: bic      r0, r0, #1
007e26dc: mov      r3, #0x5d000
007e26e0: strh     r0, [r2]
007e26e4: add      r3, r3, #0x1c
007e26e8: ldr      r1, [r4, r3]
007e26ec: mov      r0, sl
007e26f0: bl       #0x30e3ac
007e26f4: ldr      r1, [r4, r6]
007e26f8: bl       #0x30ed6c
007e26fc: bl       #0x8be2a0
007e2700: ldr      r3, [sp, #0xc]
007e2704: orr      r0, r0, #1
007e2708: mov      r6, #0x5d000
007e270c: strh     r0, [r3]
007e2710: add      r6, r6, #0x20
007e2714: mov      r5, #0x5d000
007e2718: ldr      r1, [r4, r6]
007e271c: add      r5, r5, #0x30
007e2720: mov      r0, r8
007e2724: bl       #0x30e3ac
007e2728: ldr      r1, [r4, r5]
007e272c: bl       #0x30ed6c
007e2730: bl       #0x8be2a0
007e2734: ldr      r2, [sp, #8]
007e2738: bic      r0, r0, #1
007e273c: strh     r0, [r2, #2]
007e2740: ldr      r1, [r4, r6]
007e2744: mov      r0, r7
007e2748: bl       #0x30e3ac
007e274c: ldr      r1, [r4, r5]
007e2750: bl       #0x30ed6c
007e2754: bl       #0x8be2a0
007e2758: ldr      r3, [sp, #0xc]
007e275c: orr      r0, r0, #1
007e2760: strh     r0, [r3, #2]
007e2764: add      sp, sp, #0x14
007e2768: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK7b2World13GetProxyCountEv
007e6968: mov      r3, #0x19000
007e696c: add      r3, r3, #0x1d8
007e6970: ldr      r2, [r0, r3]
007e6974: mov      r3, #0x5d000
007e6978: add      r3, r3, #0x34
007e697c: ldr      r0, [r2, r3]
007e6980: bx       lr

# _ZN12b2MouseJoint23InitVelocityConstraintsERK10b2TimeStep
007eb4b0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007eb4b4: ldr      r4, [r0, #0x34]
007eb4b8: sub      sp, sp, #0x14
007eb4bc: mov      ip, r1
007eb4c0: mov      r5, r0
007eb4c4: ldr      r1, [r4, #0x1c]
007eb4c8: ldr      r0, [r0, #0x44]
007eb4cc: str      ip, [sp]
007eb4d0: bl       #0x30e3ac
007eb4d4: ldr      r1, [r4, #0x20]
007eb4d8: mov      r8, r0
007eb4dc: ldr      r0, [r5, #0x48]
007eb4e0: bl       #0x30e3ac
007eb4e4: ldr      r1, [r4, #0xc]
007eb4e8: mov      r6, r0
007eb4ec: mov      r0, r8
007eb4f0: bl       #0x30ed6c
007eb4f4: ldr      r1, [r4, #0x14]
007eb4f8: mov      r7, r0
007eb4fc: mov      r0, r6
007eb500: bl       #0x30ed6c
007eb504: mov      r1, r0
007eb508: mov      r0, r7
007eb50c: bl       #0x30eba4
007eb510: ldr      r1, [r4, #0x10]
007eb514: mov      r7, r0
007eb518: mov      r0, r8
007eb51c: bl       #0x30ed6c
007eb520: ldr      r1, [r4, #0x18]
007eb524: mov      r8, r0
007eb528: mov      r0, r6
007eb52c: bl       #0x30ed6c
007eb530: mov      r1, r0
007eb534: mov      r0, r8
007eb538: bl       #0x30eba4
007eb53c: ldr      r8, [r4, #0x80]
007eb540: mov      r6, r0
007eb544: mov      r1, r0
007eb548: mov      r0, r8
007eb54c: bl       #0x30ed6c
007eb550: mov      r1, r6
007eb554: bl       #0x30ed6c
007eb558: mov      r1, r7
007eb55c: mov      sb, r0
007eb560: add      r0, r8, #0x80000000
007eb564: bl       #0x30ed6c
007eb568: mov      r1, r6
007eb56c: bl       #0x30ed6c
007eb570: mov      r1, r7
007eb574: mov      fp, r0
007eb578: mov      r0, r8
007eb57c: bl       #0x30ed6c
007eb580: mov      r1, r7
007eb584: bl       #0x30ed6c
007eb588: ldr      sl, [r4, #0x78]
007eb58c: mov      r2, r0
007eb590: mov      r1, sb
007eb594: mov      r0, sl
007eb598: str      r2, [sp, #8]
007eb59c: bl       #0x30eba4
007eb5a0: mov      r1, #0
007eb5a4: mov      sb, r0
007eb5a8: mov      r0, fp
007eb5ac: bl       #0x30eba4
007eb5b0: ldr      r3, [r5, #0x7c]
007eb5b4: mov      fp, r0
007eb5b8: mov      r1, sb
007eb5bc: mov      r0, r3
007eb5c0: str      r3, [sp, #4]
007eb5c4: bl       #0x30eba4
007eb5c8: ldr      r2, [sp, #8]
007eb5cc: str      r0, [sp, #0xc]
007eb5d0: mov      r0, sl
007eb5d4: mov      r1, r2
007eb5d8: bl       #0x30eba4
007eb5dc: ldr      r3, [sp, #4]
007eb5e0: mov      r1, r0
007eb5e4: mov      r0, r3
007eb5e8: bl       #0x30eba4
007eb5ec: mov      r3, r0
007eb5f0: mov      r1, r3
007eb5f4: ldr      r0, [sp, #0xc]
007eb5f8: str      r3, [sp, #4]
007eb5fc: bl       #0x30ed6c
007eb600: mov      r1, fp
007eb604: mov      sb, r0
007eb608: mov      r0, fp
007eb60c: bl       #0x30ed6c
007eb610: mov      r1, r0
007eb614: mov      r0, sb
007eb618: bl       #0x30e3ac
007eb61c: mov      r1, r0
007eb620: mov      r0, #0x3f800000
007eb624: bl       #0x30ec94
007eb628: mov      sb, r0
007eb62c: add      r1, r0, #0x80000000
007eb630: mov      r0, fp
007eb634: bl       #0x30ed6c
007eb638: mov      r1, sb
007eb63c: mov      fp, r0
007eb640: ldr      r0, [sp, #0xc]
007eb644: bl       #0x30ed6c
007eb648: str      fp, [r5, #0x60]
007eb64c: str      r0, [r5, #0x68]
007eb650: str      fp, [r5, #0x64]
007eb654: ldr      r3, [sp, #4]
007eb658: mov      r1, sb
007eb65c: mov      r0, r3
007eb660: bl       #0x30ed6c
007eb664: str      r0, [r5, #0x5c]
007eb668: ldr      r1, [r4, #0x2c]
007eb66c: mov      r0, r7
007eb670: bl       #0x30eba4
007eb674: ldr      r1, [r4, #0x30]
007eb678: mov      sb, r0
007eb67c: mov      r0, r6
007eb680: bl       #0x30eba4
007eb684: ldr      r1, [r5, #0x4c]
007eb688: mov      fp, r0
007eb68c: mov      r0, sb
007eb690: bl       #0x30e3ac
007eb694: ldr      r1, [r5, #0x50]
007eb698: mov      sb, r0
007eb69c: mov      r0, fp
007eb6a0: bl       #0x30e3ac
007eb6a4: str      sb, [r5, #0x6c]
007eb6a8: str      r0, [r5, #0x70]
007eb6ac: movw     r1, #0xe148
007eb6b0: ldr      r0, [r4, #0x48]
007eb6b4: movt     r1, #0x3f7a
007eb6b8: bl       #0x30ed6c
007eb6bc: str      r0, [r4, #0x48]
007eb6c0: ldr      ip, [sp]
007eb6c4: mov      r3, r0
007eb6c8: ldr      r1, [r5, #0x54]
007eb6cc: ldr      fp, [ip]
007eb6d0: str      r3, [sp, #4]
007eb6d4: mov      r0, fp
007eb6d8: bl       #0x30ed6c
007eb6dc: ldr      r1, [r5, #0x58]
007eb6e0: mov      sb, r0
007eb6e4: mov      r0, fp
007eb6e8: bl       #0x30ed6c
007eb6ec: mov      r1, sb
007eb6f0: mov      r5, r0
007eb6f4: mov      r0, sl
007eb6f8: bl       #0x30ed6c
007eb6fc: mov      r1, r0
007eb700: ldr      r0, [r4, #0x40]
007eb704: bl       #0x30eba4
007eb708: mov      r1, r5
007eb70c: str      r0, [r4, #0x40]
007eb710: mov      r0, sl
007eb714: bl       #0x30ed6c
007eb718: mov      r1, r0
007eb71c: ldr      r0, [r4, #0x44]
007eb720: bl       #0x30eba4
007eb724: mov      r1, r5
007eb728: str      r0, [r4, #0x44]
007eb72c: mov      r0, r7
007eb730: bl       #0x30ed6c
007eb734: mov      r1, sb
007eb738: mov      r5, r0
007eb73c: mov      r0, r6
007eb740: bl       #0x30ed6c
007eb744: mov      r1, r0
007eb748: mov      r0, r5
007eb74c: bl       #0x30e3ac
007eb750: mov      r1, r0
007eb754: mov      r0, r8
007eb758: bl       #0x30ed6c
007eb75c: ldr      r3, [sp, #4]
007eb760: mov      r1, r0
007eb764: mov      r0, r3
007eb768: bl       #0x30eba4
007eb76c: str      r0, [r4, #0x48]
007eb770: add      sp, sp, #0x14
007eb774: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK15b2RevoluteJoint16GetReactionForceEv
007f2b20: ldr      ip, [r1, #0x54]
007f2b24: ldr      r2, [r1, #0x58]
007f2b28: str      ip, [r0]
007f2b2c: str      r2, [r0, #4]
007f2b30: bx       lr

# _ZN16b2ContactManagerD1Ev
007e9f90: ldr      r3, [pc, #0x24]
007e9f94: ldr      r1, [pc, #0x24]
007e9f98: ldr      r2, [pc, #0x24]
007e9f9c: add      r3, pc, r3
007e9fa0: ldr      r1, [r3, r1]
007e9fa4: ldr      r2, [r3, r2]
007e9fa8: add      r1, r1, #8
007e9fac: add      r2, r2, #8
007e9fb0: str      r2, [r0, #8]
007e9fb4: str      r1, [r0]
007e9fb8: bx       lr
007e9fbc: ldrsheq  sl, [sl], -r4
007e9fc0: andeq    r2, r0, r8, lsl #1
007e9fc4: andeq    r2, r0, r4, lsr r3

# _ZNK16b2PrismaticJoint19GetJointTranslationEv
007eeab4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007eeab8: ldr      r5, [r0, #0x30]
007eeabc: ldr      r6, [r0, #0x44]
007eeac0: sub      sp, sp, #0xc
007eeac4: ldr      sl, [r5, #0xc]
007eeac8: ldr      sb, [r0, #0x48]
007eeacc: mov      r4, r0
007eead0: mov      r1, r6
007eead4: mov      r0, sl
007eead8: bl       #0x30ed6c
007eeadc: ldr      r8, [r5, #0x14]
007eeae0: mov      r7, r0
007eeae4: mov      r1, sb
007eeae8: mov      r0, r8
007eeaec: bl       #0x30ed6c
007eeaf0: mov      r1, r0
007eeaf4: mov      r0, r7
007eeaf8: bl       #0x30eba4
007eeafc: ldr      r7, [r5, #0x10]
007eeb00: mov      r3, r0
007eeb04: mov      r0, r6
007eeb08: mov      r1, r7
007eeb0c: ldr      r6, [r5, #0x18]
007eeb10: str      r3, [sp]
007eeb14: bl       #0x30ed6c
007eeb18: mov      r1, r6
007eeb1c: mov      fp, r0
007eeb20: mov      r0, sb
007eeb24: bl       #0x30ed6c
007eeb28: mov      r1, r0
007eeb2c: mov      r0, fp
007eeb30: bl       #0x30eba4
007eeb34: ldr      r3, [sp]
007eeb38: ldr      r1, [r5, #4]
007eeb3c: mov      sb, r0
007eeb40: mov      r0, r3
007eeb44: bl       #0x30eba4
007eeb48: ldr      r1, [r5, #8]
007eeb4c: mov      r2, r0
007eeb50: mov      r0, sb
007eeb54: ldr      r5, [r4, #0x34]
007eeb58: str      r2, [sp, #4]
007eeb5c: bl       #0x30eba4
007eeb60: ldr      sb, [r4, #0x4c]
007eeb64: mov      r3, r0
007eeb68: ldr      r1, [r5, #0xc]
007eeb6c: mov      r0, sb
007eeb70: str      r3, [sp]
007eeb74: bl       #0x30ed6c
007eeb78: ldr      r1, [r5, #0x14]
007eeb7c: mov      fp, r0
007eeb80: ldr      r0, [r4, #0x50]
007eeb84: bl       #0x30ed6c
007eeb88: mov      r1, r0
007eeb8c: mov      r0, fp
007eeb90: bl       #0x30eba4
007eeb94: ldr      r1, [r5, #0x10]
007eeb98: mov      fp, r0
007eeb9c: mov      r0, sb
007eeba0: bl       #0x30ed6c
007eeba4: ldr      r1, [r5, #0x18]
007eeba8: mov      sb, r0
007eebac: ldr      r0, [r4, #0x50]
007eebb0: bl       #0x30ed6c
007eebb4: mov      r1, r0
007eebb8: mov      r0, sb
007eebbc: bl       #0x30eba4
007eebc0: ldr      r1, [r5, #4]
007eebc4: mov      sb, r0
007eebc8: mov      r0, fp
007eebcc: bl       #0x30eba4
007eebd0: ldr      r1, [r5, #8]
007eebd4: mov      fp, r0
007eebd8: mov      r0, sb
007eebdc: bl       #0x30eba4
007eebe0: ldr      r2, [sp, #4]
007eebe4: mov      r5, r0
007eebe8: mov      r0, fp
007eebec: mov      r1, r2
007eebf0: bl       #0x30e3ac
007eebf4: ldr      r3, [sp]
007eebf8: mov      sb, r0
007eebfc: mov      r0, r5
007eec00: mov      r1, r3
007eec04: bl       #0x30e3ac
007eec08: ldr      r5, [r4, #0x54]
007eec0c: mov      fp, r0
007eec10: mov      r0, sl
007eec14: mov      r1, r5
007eec18: bl       #0x30ed6c
007eec1c: ldr      r4, [r4, #0x58]
007eec20: mov      sl, r0
007eec24: mov      r0, r8
007eec28: mov      r1, r4
007eec2c: bl       #0x30ed6c
007eec30: mov      r1, r0
007eec34: mov      r0, sl
007eec38: bl       #0x30eba4
007eec3c: mov      r1, r0
007eec40: mov      r0, sb
007eec44: bl       #0x30ed6c
007eec48: mov      r1, r5
007eec4c: mov      r8, r0
007eec50: mov      r0, r7
007eec54: bl       #0x30ed6c
007eec58: mov      r1, r4
007eec5c: mov      r5, r0
007eec60: mov      r0, r6
007eec64: bl       #0x30ed6c
007eec68: mov      r1, r0
007eec6c: mov      r0, r5
007eec70: bl       #0x30eba4
007eec74: mov      r1, r0
007eec78: mov      r0, fp
007eec7c: bl       #0x30ed6c
007eec80: mov      r1, r0
007eec84: mov      r0, r8
007eec88: bl       #0x30eba4
007eec8c: add      sp, sp, #0xc
007eec90: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN15b2CircleContact7DestroyEP9b2ContactP16b2BlockAllocator
007f3b40: push     {r4, r5, r6, lr}
007f3b44: ldr      r3, [r0]
007f3b48: mov      r5, r1
007f3b4c: mov      r4, r0
007f3b50: mov      lr, pc
007f3b54: ldr      pc, [r3, #4]
007f3b58: mov      r0, r5
007f3b5c: mov      r1, r4
007f3b60: mov      r2, #0x94
007f3b64: pop      {r4, r5, r6, lr}
007f3b68: b        #0x7e8da0

# _ZNK15b2RevoluteJoint13GetJointSpeedEv
007f2b64: push     {r4, lr}
007f2b68: ldr      r2, [r0, #0x30]
007f2b6c: ldr      r3, [r0, #0x34]
007f2b70: ldr      r1, [r2, #0x48]
007f2b74: ldr      r0, [r3, #0x48]
007f2b78: bl       #0x30e3ac
007f2b7c: pop      {r4, pc}

# _ZN15b2RevoluteJointC2EPK18b2RevoluteJointDef
007f3414: push     {r4, r5, r6, lr}
007f3418: ldr      r6, [pc, #0x94]
007f341c: mov      r4, r0
007f3420: mov      r5, r1
007f3424: bl       #0x7eb1e0
007f3428: ldr      r2, [pc, #0x88]
007f342c: add      r6, pc, r6
007f3430: mov      r3, #0
007f3434: ldr      r2, [r6, r2]
007f3438: mov      r0, r4
007f343c: add      r2, r2, #8
007f3440: str      r2, [r4]
007f3444: ldr      r2, [r5, #0x14]
007f3448: str      r2, [r4, #0x44]
007f344c: ldr      r2, [r5, #0x18]
007f3450: str      r2, [r4, #0x48]
007f3454: ldr      r2, [r5, #0x1c]
007f3458: str      r2, [r4, #0x4c]
007f345c: ldr      r2, [r5, #0x20]
007f3460: str      r2, [r4, #0x50]
007f3464: ldr      r2, [r5, #0x24]
007f3468: str      r3, [r4, #0x64]
007f346c: str      r3, [r4, #0x54]
007f3470: str      r2, [r4, #0x8c]
007f3474: str      r3, [r4, #0x58]
007f3478: str      r3, [r4, #0x5c]
007f347c: str      r3, [r4, #0x60]
007f3480: ldr      r3, [r5, #0x2c]
007f3484: str      r3, [r4, #0x90]
007f3488: ldr      r3, [r5, #0x30]
007f348c: str      r3, [r4, #0x94]
007f3490: ldr      r3, [r5, #0x3c]
007f3494: str      r3, [r4, #0x80]
007f3498: ldr      r3, [r5, #0x38]
007f349c: str      r3, [r4, #0x84]
007f34a0: ldrb     r3, [r5, #0x28]
007f34a4: strb     r3, [r4, #0x88]
007f34a8: ldrb     r3, [r5, #0x34]
007f34ac: strb     r3, [r4, #0x7c]
007f34b0: pop      {r4, r5, r6, pc}
007f34b4: andseq   r1, sl, r4, ror #12
007f34b8: strdeq   r3, r4, [r0], -r4

# _ZN16b2PrismaticJoint23InitVelocityConstraintsERK10b2TimeStep
007ed7e8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ed7ec: ldr      r6, [r0, #0x34]
007ed7f0: sub      sp, sp, #0x54
007ed7f4: str      r1, [sp, #0x28]
007ed7f8: mov      r4, r0
007ed7fc: ldr      r1, [r6, #0x1c]
007ed800: ldr      r0, [r0, #0x4c]
007ed804: bl       #0x30e3ac
007ed808: ldr      r1, [r6, #0x20]
007ed80c: mov      r8, r0
007ed810: ldr      r0, [r4, #0x50]
007ed814: bl       #0x30e3ac
007ed818: ldr      r1, [r6, #0xc]
007ed81c: mov      r7, r0
007ed820: mov      r0, r8
007ed824: bl       #0x30ed6c
007ed828: ldr      r1, [r6, #0x14]
007ed82c: mov      r5, r0
007ed830: mov      r0, r7
007ed834: bl       #0x30ed6c
007ed838: mov      r1, r0
007ed83c: mov      r0, r5
007ed840: bl       #0x30eba4
007ed844: ldr      r5, [r4, #0x30]
007ed848: str      r0, [sp, #8]
007ed84c: ldr      r1, [r6, #0x10]
007ed850: mov      r0, r8
007ed854: bl       #0x30ed6c
007ed858: ldr      r1, [r6, #0x18]
007ed85c: mov      r8, r0
007ed860: mov      r0, r7
007ed864: bl       #0x30ed6c
007ed868: mov      r1, r0
007ed86c: mov      r0, r8
007ed870: bl       #0x30eba4
007ed874: str      r0, [sp, #0xc]
007ed878: ldr      r1, [r5, #0xc]
007ed87c: ldr      sl, [r4, #0x5c]
007ed880: str      r1, [sp, #0x30]
007ed884: ldr      r2, [r5, #0x14]
007ed888: mov      r0, r1
007ed88c: mov      r1, sl
007ed890: ldr      r8, [r4, #0x60]
007ed894: str      r2, [sp, #0x34]
007ed898: bl       #0x30ed6c
007ed89c: mov      r1, r8
007ed8a0: mov      r7, r0
007ed8a4: ldr      r0, [sp, #0x34]
007ed8a8: bl       #0x30ed6c
007ed8ac: ldr      r3, [r5, #0x10]
007ed8b0: mov      r1, r0
007ed8b4: mov      r0, r7
007ed8b8: str      r3, [sp, #0x38]
007ed8bc: bl       #0x30eba4
007ed8c0: ldr      r2, [r5, #0x18]
007ed8c4: mov      r7, r0
007ed8c8: mov      r1, sl
007ed8cc: ldr      r0, [sp, #0x38]
007ed8d0: str      r2, [sp, #0x3c]
007ed8d4: bl       #0x30ed6c
007ed8d8: mov      r1, r8
007ed8dc: mov      sl, r0
007ed8e0: ldr      r0, [sp, #0x3c]
007ed8e4: bl       #0x30ed6c
007ed8e8: mov      r1, r0
007ed8ec: mov      r0, sl
007ed8f0: bl       #0x30eba4
007ed8f4: ldr      r1, [r6, #0x2c]
007ed8f8: mov      r8, r0
007ed8fc: ldr      r0, [sp, #8]
007ed900: bl       #0x30eba4
007ed904: ldr      r1, [r6, #0x30]
007ed908: mov      sl, r0
007ed90c: ldr      r0, [sp, #0xc]
007ed910: bl       #0x30eba4
007ed914: ldr      r1, [r5, #0x2c]
007ed918: mov      sb, r0
007ed91c: mov      r0, sl
007ed920: bl       #0x30e3ac
007ed924: str      r0, [sp, #0x14]
007ed928: ldr      r1, [r5, #0x30]
007ed92c: mov      r0, sb
007ed930: bl       #0x30e3ac
007ed934: ldr      r1, [sp, #0x14]
007ed938: str      r0, [sp, #0x20]
007ed93c: mov      r0, r8
007ed940: bl       #0x30ed6c
007ed944: ldr      r1, [sp, #0x20]
007ed948: mov      sl, r0
007ed94c: mov      r0, r7
007ed950: bl       #0x30ed6c
007ed954: mov      r1, r0
007ed958: mov      r0, sl
007ed95c: bl       #0x30e3ac
007ed960: add      r0, r0, #0x80000000
007ed964: mov      fp, r0
007ed968: mov      r1, r8
007ed96c: ldr      r0, [sp, #8]
007ed970: bl       #0x30ed6c
007ed974: mov      r1, r7
007ed978: mov      sb, r0
007ed97c: ldr      r0, [sp, #0xc]
007ed980: bl       #0x30ed6c
007ed984: mov      r1, r0
007ed988: mov      r0, sb
007ed98c: bl       #0x30e3ac
007ed990: ldr      r1, [r6, #0x80]
007ed994: ldr      sl, [r5, #0x80]
007ed998: add      r3, r7, #0x80000000
007ed99c: str      r1, [sp, #0x10]
007ed9a0: ldr      r1, [r5, #0x78]
007ed9a4: add      r2, r8, #0x80000000
007ed9a8: mov      sb, r0
007ed9ac: str      r1, [sp, #0x1c]
007ed9b0: ldr      r1, [r6, #0x78]
007ed9b4: str      r1, [sp, #0x18]
007ed9b8: ldr      r1, [r4, #0x44]
007ed9bc: str      r1, [sp, #0x40]
007ed9c0: ldr      r1, [r5, #0x1c]
007ed9c4: str      r1, [sp, #0x44]
007ed9c8: ldr      r1, [r4, #0x48]
007ed9cc: str      r1, [sp, #0x48]
007ed9d0: ldr      r1, [r5, #0x20]
007ed9d4: str      r1, [sp, #0x4c]
007ed9d8: str      r2, [r4, #0x6c]
007ed9dc: str      r3, [r4, #0x68]
007ed9e0: str      r7, [r4, #0x74]
007ed9e4: str      r0, [r4, #0x7c]
007ed9e8: mov      r1, fp
007ed9ec: str      r8, [r4, #0x78]
007ed9f0: str      fp, [r4, #0x70]
007ed9f4: mov      r0, sl
007ed9f8: bl       #0x30ed6c
007ed9fc: mov      r1, r0
007eda00: mov      r0, fp
007eda04: bl       #0x30ed6c
007eda08: ldr      r1, [sp, #0x1c]
007eda0c: bl       #0x30eba4
007eda10: ldr      r1, [sp, #0x18]
007eda14: bl       #0x30eba4
007eda18: mov      r1, sb
007eda1c: mov      r7, r0
007eda20: ldr      r0, [sp, #0x10]
007eda24: bl       #0x30ed6c
007eda28: mov      r1, sb
007eda2c: bl       #0x30ed6c
007eda30: mov      r1, r0
007eda34: mov      r0, r7
007eda38: bl       #0x30eba4
007eda3c: mov      r1, r0
007eda40: mov      r0, #0x3f800000
007eda44: bl       #0x30ec94
007eda48: str      r0, [r4, #0x80]
007eda4c: ldr      r1, [sp, #0x10]
007eda50: mov      r0, sl
007eda54: bl       #0x30eba4
007eda58: mov      r1, #0x34000000
007eda5c: mov      r7, r0
007eda60: str      r0, [r4, #0x88]
007eda64: bl       #0x30e2f8
007eda68: cmp      r0, #0
007eda6c: beq      #0x7eda80
007eda70: mov      r1, r7
007eda74: mov      r0, #0x3f800000
007eda78: bl       #0x30ec94
007eda7c: str      r0, [r4, #0x88]
007eda80: ldrb     sb, [r4, #0xc8]
007eda84: cmp      sb, #0
007eda88: beq      #0x7edea4
007eda8c: ldrb     r2, [r4, #0xc9]
007eda90: str      r2, [sp, #0x2c]
007eda94: ldr      fp, [r4, #0x54]
007eda98: ldr      r1, [r5, #0xc]
007eda9c: mov      r0, fp
007edaa0: bl       #0x30ed6c
007edaa4: ldr      r1, [r5, #0x14]
007edaa8: mov      r7, r0
007edaac: ldr      r0, [r4, #0x58]
007edab0: bl       #0x30ed6c
007edab4: mov      r1, r0
007edab8: mov      r0, r7
007edabc: bl       #0x30eba4
007edac0: ldr      r1, [r5, #0x10]
007edac4: mov      r8, r0
007edac8: mov      r0, fp
007edacc: bl       #0x30ed6c
007edad0: ldr      r1, [r5, #0x18]
007edad4: mov      r7, r0
007edad8: ldr      r0, [r4, #0x58]
007edadc: bl       #0x30ed6c
007edae0: mov      r1, r0
007edae4: mov      r0, r7
007edae8: bl       #0x30eba4
007edaec: mov      r7, r0
007edaf0: mov      r1, r7
007edaf4: ldr      r0, [sp, #0x14]
007edaf8: bl       #0x30ed6c
007edafc: mov      r1, r8
007edb00: mov      fp, r0
007edb04: ldr      r0, [sp, #0x20]
007edb08: bl       #0x30ed6c
007edb0c: mov      r1, r0
007edb10: mov      r0, fp
007edb14: bl       #0x30e3ac
007edb18: mov      r1, r7
007edb1c: add      r3, r0, #0x80000000
007edb20: ldr      r0, [sp, #8]
007edb24: str      r3, [sp, #4]
007edb28: bl       #0x30ed6c
007edb2c: mov      r1, r8
007edb30: mov      fp, r0
007edb34: ldr      r0, [sp, #0xc]
007edb38: bl       #0x30ed6c
007edb3c: mov      r1, r0
007edb40: mov      r0, fp
007edb44: bl       #0x30e3ac
007edb48: add      r2, r8, #0x80000000
007edb4c: add      r1, r7, #0x80000000
007edb50: str      r2, [r4, #0x90]
007edb54: str      r1, [r4, #0x94]
007edb58: ldr      r3, [sp, #4]
007edb5c: mov      fp, r0
007edb60: str      r0, [r4, #0xa4]
007edb64: str      r3, [r4, #0x98]
007edb68: mov      r1, r3
007edb6c: str      r7, [r4, #0xa0]
007edb70: str      r8, [r4, #0x9c]
007edb74: mov      r0, sl
007edb78: str      r3, [sp, #4]
007edb7c: bl       #0x30ed6c
007edb80: ldr      r3, [sp, #4]
007edb84: mov      r1, r0
007edb88: mov      r0, r3
007edb8c: bl       #0x30ed6c
007edb90: ldr      r1, [sp, #0x1c]
007edb94: bl       #0x30eba4
007edb98: ldr      r1, [sp, #0x18]
007edb9c: bl       #0x30eba4
007edba0: mov      r1, fp
007edba4: mov      r3, r0
007edba8: ldr      r0, [sp, #0x10]
007edbac: str      r3, [sp, #4]
007edbb0: bl       #0x30ed6c
007edbb4: mov      r1, fp
007edbb8: bl       #0x30ed6c
007edbbc: ldr      r3, [sp, #4]
007edbc0: mov      r1, r0
007edbc4: mov      r0, r3
007edbc8: bl       #0x30eba4
007edbcc: mov      r1, r0
007edbd0: mov      r0, #0x3f800000
007edbd4: bl       #0x30ec94
007edbd8: cmp      sb, #0
007edbdc: str      r0, [r4, #0xa8]
007edbe0: bne      #0x7edef0
007edbe4: ldr      r1, [sp, #0x2c]
007edbe8: cmp      r1, #0
007edbec: beq      #0x7edeb4
007edbf0: cmp      sb, #0
007edbf4: moveq    r3, #0
007edbf8: streq    r3, [r4, #0xb0]
007edbfc: ldr      r2, [sp, #0x28]
007edc00: ldrb     r3, [r2, #0x10]
007edc04: cmp      r3, #0
007edc08: beq      #0x7eded8
007edc0c: ldr      r8, [r4, #0x84]
007edc10: ldr      r1, [r4, #0x68]
007edc14: ldr      r7, [r2]
007edc18: mov      r0, r8
007edc1c: bl       #0x30ed6c
007edc20: ldr      r1, [r4, #0x6c]
007edc24: mov      fp, r0
007edc28: mov      r0, r8
007edc2c: bl       #0x30ed6c
007edc30: ldr      r1, [r4, #0xb0]
007edc34: mov      r2, r0
007edc38: ldr      r0, [r4, #0xac]
007edc3c: str      r2, [sp]
007edc40: bl       #0x30eba4
007edc44: ldr      r1, [r4, #0x90]
007edc48: mov      sb, r0
007edc4c: bl       #0x30ed6c
007edc50: ldr      r1, [r4, #0x94]
007edc54: mov      r3, r0
007edc58: mov      r0, sb
007edc5c: str      r3, [sp, #4]
007edc60: bl       #0x30ed6c
007edc64: ldr      r3, [sp, #4]
007edc68: mov      ip, r0
007edc6c: mov      r0, fp
007edc70: mov      r1, r3
007edc74: str      ip, [sp, #4]
007edc78: bl       #0x30eba4
007edc7c: ldm      sp, {r2, ip}
007edc80: mov      fp, r0
007edc84: mov      r1, ip
007edc88: mov      r0, r2
007edc8c: bl       #0x30eba4
007edc90: mov      r1, fp
007edc94: mov      r3, r0
007edc98: mov      r0, r7
007edc9c: str      r3, [sp, #4]
007edca0: bl       #0x30ed6c
007edca4: ldr      r3, [sp, #4]
007edca8: str      r0, [sp, #0xc]
007edcac: mov      r0, r7
007edcb0: mov      r1, r3
007edcb4: bl       #0x30ed6c
007edcb8: str      r0, [sp, #0x14]
007edcbc: ldr      r1, [r4, #0x74]
007edcc0: mov      r0, r8
007edcc4: bl       #0x30ed6c
007edcc8: ldr      r1, [r4, #0x78]
007edccc: mov      fp, r0
007edcd0: mov      r0, r8
007edcd4: bl       #0x30ed6c
007edcd8: ldr      r1, [r4, #0x9c]
007edcdc: mov      r2, r0
007edce0: mov      r0, sb
007edce4: str      r2, [sp]
007edce8: bl       #0x30ed6c
007edcec: ldr      r1, [r4, #0xa0]
007edcf0: mov      r3, r0
007edcf4: mov      r0, sb
007edcf8: str      r3, [sp, #4]
007edcfc: bl       #0x30ed6c
007edd00: ldr      r3, [sp, #4]
007edd04: mov      ip, r0
007edd08: mov      r0, fp
007edd0c: mov      r1, r3
007edd10: str      ip, [sp, #4]
007edd14: bl       #0x30eba4
007edd18: ldm      sp, {r2, ip}
007edd1c: mov      fp, r0
007edd20: mov      r1, ip
007edd24: mov      r0, r2
007edd28: bl       #0x30eba4
007edd2c: mov      r1, fp
007edd30: mov      r3, r0
007edd34: mov      r0, r7
007edd38: str      r3, [sp, #4]
007edd3c: bl       #0x30ed6c
007edd40: ldr      r3, [sp, #4]
007edd44: mov      r2, r0
007edd48: mov      r0, r7
007edd4c: mov      r1, r3
007edd50: str      r2, [sp]
007edd54: bl       #0x30ed6c
007edd58: ldr      r1, [r4, #0x70]
007edd5c: mov      r3, r0
007edd60: mov      r0, r8
007edd64: str      r3, [sp, #4]
007edd68: bl       #0x30ed6c
007edd6c: ldr      r1, [r4, #0x8c]
007edd70: bl       #0x30e3ac
007edd74: ldr      r1, [r4, #0x98]
007edd78: mov      fp, r0
007edd7c: mov      r0, sb
007edd80: bl       #0x30ed6c
007edd84: mov      r1, r0
007edd88: mov      r0, fp
007edd8c: bl       #0x30eba4
007edd90: mov      r1, r0
007edd94: mov      r0, r7
007edd98: bl       #0x30ed6c
007edd9c: ldr      r1, [r4, #0x7c]
007edda0: mov      fp, r0
007edda4: mov      r0, r8
007edda8: bl       #0x30ed6c
007eddac: mov      r1, r0
007eddb0: ldr      r0, [r4, #0x8c]
007eddb4: bl       #0x30eba4
007eddb8: ldr      r1, [r4, #0xa4]
007eddbc: mov      r8, r0
007eddc0: mov      r0, sb
007eddc4: bl       #0x30ed6c
007eddc8: mov      r1, r0
007eddcc: mov      r0, r8
007eddd0: bl       #0x30eba4
007eddd4: mov      r1, r0
007eddd8: mov      r0, r7
007edddc: bl       #0x30ed6c
007edde0: ldr      r1, [sp, #0xc]
007edde4: mov      r7, r0
007edde8: ldr      r0, [sp, #0x1c]
007eddec: bl       #0x30ed6c
007eddf0: mov      r1, r0
007eddf4: ldr      r0, [r5, #0x40]
007eddf8: bl       #0x30eba4
007eddfc: str      r0, [r5, #0x40]
007ede00: ldr      r1, [sp, #0x14]
007ede04: ldr      r0, [sp, #0x1c]
007ede08: bl       #0x30ed6c
007ede0c: mov      r1, r0
007ede10: ldr      r0, [r5, #0x44]
007ede14: bl       #0x30eba4
007ede18: mov      r1, fp
007ede1c: str      r0, [r5, #0x44]
007ede20: mov      r0, sl
007ede24: bl       #0x30ed6c
007ede28: mov      r1, r0
007ede2c: ldr      r0, [r5, #0x48]
007ede30: bl       #0x30eba4
007ede34: str      r0, [r5, #0x48]
007ede38: ldr      r2, [sp]
007ede3c: ldr      r0, [sp, #0x18]
007ede40: mov      r1, r2
007ede44: bl       #0x30ed6c
007ede48: mov      r1, r0
007ede4c: ldr      r0, [r6, #0x40]
007ede50: bl       #0x30eba4
007ede54: str      r0, [r6, #0x40]
007ede58: ldr      r3, [sp, #4]
007ede5c: ldr      r0, [sp, #0x18]
007ede60: mov      r1, r3
007ede64: bl       #0x30ed6c
007ede68: mov      r1, r0
007ede6c: ldr      r0, [r6, #0x44]
007ede70: bl       #0x30eba4
007ede74: str      r0, [r6, #0x44]
007ede78: mov      r1, r7
007ede7c: ldr      r0, [sp, #0x10]
007ede80: bl       #0x30ed6c
007ede84: mov      r1, r0
007ede88: ldr      r0, [r6, #0x48]
007ede8c: bl       #0x30eba4
007ede90: str      r0, [r6, #0x48]
007ede94: mov      r3, #0
007ede98: str      r3, [r4, #0xb4]
007ede9c: add      sp, sp, #0x54
007edea0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007edea4: ldrb     r3, [r4, #0xc9]
007edea8: cmp      r3, #0
007edeac: str      r3, [sp, #0x2c]
007edeb0: bne      #0x7eda94
007edeb4: mov      r3, #0
007edeb8: cmp      sb, #0
007edebc: str      r3, [r4, #0xac]
007edec0: moveq    r3, #0
007edec4: streq    r3, [r4, #0xb0]
007edec8: ldr      r2, [sp, #0x28]
007edecc: ldrb     r3, [r2, #0x10]
007eded0: cmp      r3, #0
007eded4: bne      #0x7edc0c
007eded8: mov      r3, #0
007ededc: str      r3, [r4, #0xac]
007edee0: str      r3, [r4, #0x84]
007edee4: str      r3, [r4, #0x8c]
007edee8: str      r3, [r4, #0xb0]
007edeec: b        #0x7ede94
007edef0: ldr      r1, [r4, #0xbc]
007edef4: str      r1, [sp, #0xc]
007edef8: ldr      r2, [r4, #0xb8]
007edefc: mov      r0, r1
007edf00: mov      r1, r2
007edf04: str      r2, [sp, #8]
007edf08: bl       #0x30e3ac
007edf0c: mov      r1, #0
007edf10: mov      fp, r0
007edf14: bl       #0x30e2f8
007edf18: cmp      r0, #0
007edf1c: moveq    r3, fp
007edf20: addeq    r3, r3, #0x80000000
007edf24: moveq    fp, r3
007edf28: movw     r1, #0xd70a
007edf2c: mov      r0, fp
007edf30: movt     r1, #0x3c23
007edf34: bl       #0x30e70c
007edf38: cmp      r0, #0
007edf3c: movne    r3, #3
007edf40: strne    r3, [r4, #0xcc]
007edf44: bne      #0x7edbe4
007edf48: ldr      r1, [sp, #0x44]
007edf4c: ldr      r0, [sp, #0x40]
007edf50: bl       #0x30e3ac
007edf54: ldr      r1, [sp, #0x4c]
007edf58: str      r0, [sp, #0x24]
007edf5c: ldr      r0, [sp, #0x48]
007edf60: bl       #0x30e3ac
007edf64: ldr      r1, [sp, #0x30]
007edf68: mov      fp, r0
007edf6c: ldr      r0, [sp, #0x24]
007edf70: bl       #0x30ed6c
007edf74: ldr      r1, [sp, #0x34]
007edf78: mov      r3, r0
007edf7c: mov      r0, fp
007edf80: str      r3, [sp, #4]
007edf84: bl       #0x30ed6c
007edf88: ldr      r3, [sp, #4]
007edf8c: mov      r1, r0
007edf90: mov      r0, r3
007edf94: bl       #0x30eba4
007edf98: mov      r1, r0
007edf9c: ldr      r0, [sp, #0x14]
007edfa0: bl       #0x30e3ac
007edfa4: mov      r1, r0
007edfa8: mov      r0, r8
007edfac: bl       #0x30ed6c
007edfb0: ldr      r1, [sp, #0x38]
007edfb4: mov      r3, r0
007edfb8: ldr      r0, [sp, #0x24]
007edfbc: str      r3, [sp, #4]
007edfc0: bl       #0x30ed6c
007edfc4: ldr      r1, [sp, #0x3c]
007edfc8: mov      r8, r0
007edfcc: mov      r0, fp
007edfd0: bl       #0x30ed6c
007edfd4: mov      r1, r0
007edfd8: mov      r0, r8
007edfdc: bl       #0x30eba4
007edfe0: mov      r1, r0
007edfe4: ldr      r0, [sp, #0x20]
007edfe8: bl       #0x30e3ac
007edfec: mov      r1, r0
007edff0: mov      r0, r7
007edff4: bl       #0x30ed6c
007edff8: ldr      r3, [sp, #4]
007edffc: mov      r1, r0
007ee000: mov      r0, r3
007ee004: bl       #0x30eba4
007ee008: mov      r7, r0
007ee00c: mov      r1, r7
007ee010: ldr      r0, [sp, #8]
007ee014: bl       #0x30e4b4
007ee018: cmp      r0, #0
007ee01c: beq      #0x7ee03c
007ee020: ldr      r3, [r4, #0xcc]
007ee024: cmp      r3, #1
007ee028: movne    r3, #0
007ee02c: strne    r3, [r4, #0xb0]
007ee030: mov      r3, #1
007ee034: str      r3, [r4, #0xcc]
007ee038: b        #0x7edbe4
007ee03c: ldr      r0, [sp, #0xc]
007ee040: mov      r1, r7
007ee044: bl       #0x30e9ac
007ee048: cmp      r0, #0
007ee04c: beq      #0x7ee06c
007ee050: ldr      r3, [r4, #0xcc]
007ee054: cmp      r3, #2
007ee058: movne    r3, #0
007ee05c: strne    r3, [r4, #0xb0]
007ee060: mov      r3, #2
007ee064: str      r3, [r4, #0xcc]
007ee068: b        #0x7edbe4
007ee06c: mov      r3, #0
007ee070: str      r3, [r4, #0xcc]
007ee074: mov      r3, #0
007ee078: str      r3, [r4, #0xb0]
007ee07c: b        #0x7edbe4

# _ZNK15b2RevoluteJoint13GetLowerLimitEv
007f2bb8: ldr      r0, [r0, #0x90]
007f2bbc: bx       lr

# _ZN16b2StackAllocator4FreeEPv
007f35a8: push     {r4, r5, r6, lr}
007f35ac: mov      r3, #0x19000
007f35b0: add      r3, r3, #0x18c
007f35b4: ldr      r5, [r0, r3]
007f35b8: mov      r2, #0xc
007f35bc: mov      r3, #0x19000
007f35c0: sub      r5, r5, #1
007f35c4: mla      r2, r2, r5, r0
007f35c8: mov      r4, r0
007f35cc: add      ip, r2, r3
007f35d0: add      ip, ip, #0x10
007f35d4: ldrb     r6, [ip, #4]
007f35d8: add      r3, r3, #0x10
007f35dc: mov      ip, #0x19000
007f35e0: cmp      r6, #0
007f35e4: bne      #0x7f3638
007f35e8: ldr      r3, [r2, r3]
007f35ec: ldr      r2, [r0, ip]
007f35f0: rsb      r3, r3, r2
007f35f4: str      r3, [r0, ip]
007f35f8: mov      r3, #0xc
007f35fc: mla      r5, r3, r5, r4
007f3600: mov      r2, #0x19000
007f3604: mov      r3, r2
007f3608: add      r3, r3, #0x10
007f360c: add      r2, r2, #4
007f3610: ldr      r1, [r5, r3]
007f3614: ldr      r0, [r4, r2]
007f3618: mov      r3, #0x19000
007f361c: add      r3, r3, #0x18c
007f3620: rsb      r1, r1, r0
007f3624: str      r1, [r4, r2]
007f3628: ldr      r2, [r4, r3]
007f362c: sub      r2, r2, #1
007f3630: str      r2, [r4, r3]
007f3634: pop      {r4, r5, r6, pc}
007f3638: mov      r0, r1
007f363c: bl       #0x7f34bc
007f3640: b        #0x7f35f8

# _ZN12b2BroadPhaseD1Ev
007e247c: bx       lr

# _ZNK14b2PolygonShape11TestSegmentERK7b2XFormPfP6b2Vec2RK9b2Segmentf
007e47e8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e47ec: sub      sp, sp, #0x34
007e47f0: str      r1, [sp, #0x1c]
007e47f4: ldr      ip, [r1]
007e47f8: ldr      r4, [sp, #0x58]
007e47fc: str      r0, [sp, #0x24]
007e4800: mov      r1, ip
007e4804: ldr      r0, [r4]
007e4808: str      ip, [sp, #8]
007e480c: str      r3, [sp, #0x28]
007e4810: str      r2, [sp, #0x2c]
007e4814: bl       #0x30e3ac
007e4818: ldr      r1, [sp, #0x1c]
007e481c: mov      sl, r0
007e4820: ldr      r0, [r4, #4]
007e4824: ldr      r8, [r1, #4]
007e4828: mov      r1, r8
007e482c: bl       #0x30e3ac
007e4830: ldr      r2, [sp, #0x1c]
007e4834: mov      r5, r0
007e4838: mov      r0, sl
007e483c: ldr      r7, [r2, #8]
007e4840: ldr      r6, [r2, #0xc]
007e4844: mov      r1, r7
007e4848: bl       #0x30ed6c
007e484c: mov      r1, r6
007e4850: mov      sb, r0
007e4854: mov      r0, r5
007e4858: bl       #0x30ed6c
007e485c: mov      r1, r0
007e4860: mov      r0, sb
007e4864: bl       #0x30eba4
007e4868: ldr      r1, [sp, #0x1c]
007e486c: str      r0, [sp, #0xc]
007e4870: mov      r0, sl
007e4874: ldr      r3, [r1, #0x10]
007e4878: ldr      sb, [r1, #0x14]
007e487c: mov      r1, r3
007e4880: str      r3, [sp, #4]
007e4884: bl       #0x30ed6c
007e4888: mov      r1, sb
007e488c: mov      sl, r0
007e4890: mov      r0, r5
007e4894: bl       #0x30ed6c
007e4898: mov      r1, r0
007e489c: mov      r0, sl
007e48a0: bl       #0x30eba4
007e48a4: ldr      ip, [sp, #8]
007e48a8: mov      fp, r0
007e48ac: ldr      r0, [r4, #8]
007e48b0: mov      r1, ip
007e48b4: bl       #0x30e3ac
007e48b8: mov      r1, r8
007e48bc: mov      r5, r0
007e48c0: ldr      r0, [r4, #0xc]
007e48c4: bl       #0x30e3ac
007e48c8: mov      r1, r5
007e48cc: mov      r4, r0
007e48d0: mov      r0, r7
007e48d4: bl       #0x30ed6c
007e48d8: mov      r1, r4
007e48dc: mov      r7, r0
007e48e0: mov      r0, r6
007e48e4: bl       #0x30ed6c
007e48e8: mov      r1, r0
007e48ec: mov      r0, r7
007e48f0: bl       #0x30eba4
007e48f4: ldr      r3, [sp, #4]
007e48f8: mov      r1, r5
007e48fc: mov      r6, r0
007e4900: mov      r0, r3
007e4904: bl       #0x30ed6c
007e4908: mov      r1, r4
007e490c: mov      r5, r0
007e4910: mov      r0, sb
007e4914: bl       #0x30ed6c
007e4918: mov      r1, r0
007e491c: mov      r0, r5
007e4920: bl       #0x30eba4
007e4924: ldr      r1, [sp, #0xc]
007e4928: mov      r4, r0
007e492c: mov      r0, r6
007e4930: bl       #0x30e3ac
007e4934: mov      r1, fp
007e4938: str      r0, [sp, #0x14]
007e493c: mov      r0, r4
007e4940: bl       #0x30e3ac
007e4944: ldr      r2, [sp, #0x24]
007e4948: str      r0, [sp, #0x18]
007e494c: ldr      r2, [r2, #0x118]
007e4950: cmp      r2, #0
007e4954: str      r2, [sp, #0x10]
007e4958: ble      #0x7e4aa8
007e495c: mvn      r3, #0
007e4960: mov      r8, #0
007e4964: ldr      sb, [sp, #0x5c]
007e4968: ldr      r4, [sp, #0x24]
007e496c: mov      r5, #0
007e4970: str      r3, [sp, #0x20]
007e4974: b        #0x7e49c8
007e4978: bl       #0x30ed6c
007e497c: mov      r1, sl
007e4980: bl       #0x30e2f8
007e4984: cmp      r0, #0
007e4988: beq      #0x7e4a50
007e498c: mov      r0, sl
007e4990: mov      r1, r6
007e4994: bl       #0x30ec94
007e4998: str      r5, [sp, #0x20]
007e499c: mov      r8, r0
007e49a0: mov      r0, sb
007e49a4: mov      r1, r8
007e49a8: bl       #0x30e70c
007e49ac: cmp      r0, #0
007e49b0: add      r5, r5, #1
007e49b4: bne      #0x7e4aa8
007e49b8: ldr      r1, [sp, #0x10]
007e49bc: add      r4, r4, #8
007e49c0: cmp      r5, r1
007e49c4: beq      #0x7e4ab4
007e49c8: ldr      r7, [r4, #0x98]
007e49cc: ldr      r1, [sp, #0xc]
007e49d0: ldr      r0, [r4, #0x58]
007e49d4: bl       #0x30e3ac
007e49d8: mov      r1, r7
007e49dc: bl       #0x30ed6c
007e49e0: ldr      r6, [r4, #0x9c]
007e49e4: mov      sl, r0
007e49e8: mov      r1, fp
007e49ec: ldr      r0, [r4, #0x5c]
007e49f0: bl       #0x30e3ac
007e49f4: mov      r1, r6
007e49f8: bl       #0x30ed6c
007e49fc: mov      r1, r0
007e4a00: mov      r0, sl
007e4a04: bl       #0x30eba4
007e4a08: mov      r1, r7
007e4a0c: mov      sl, r0
007e4a10: ldr      r0, [sp, #0x14]
007e4a14: bl       #0x30ed6c
007e4a18: mov      r1, r6
007e4a1c: mov      r7, r0
007e4a20: ldr      r0, [sp, #0x18]
007e4a24: bl       #0x30ed6c
007e4a28: mov      r1, r0
007e4a2c: mov      r0, r7
007e4a30: bl       #0x30eba4
007e4a34: mov      r1, #0
007e4a38: mov      r6, r0
007e4a3c: bl       #0x30e70c
007e4a40: cmp      r0, #0
007e4a44: mov      r1, r6
007e4a48: mov      r0, r8
007e4a4c: bne      #0x7e4978
007e4a50: mov      r1, #0
007e4a54: mov      r0, r6
007e4a58: bl       #0x30e2f8
007e4a5c: cmp      r0, #0
007e4a60: mov      r1, r6
007e4a64: mov      r0, sb
007e4a68: beq      #0x7e49a0
007e4a6c: bl       #0x30ed6c
007e4a70: mov      r1, sl
007e4a74: bl       #0x30e2f8
007e4a78: cmp      r0, #0
007e4a7c: beq      #0x7e49a0
007e4a80: mov      r1, r6
007e4a84: mov      r0, sl
007e4a88: bl       #0x30ec94
007e4a8c: mov      sb, r0
007e4a90: mov      r0, sb
007e4a94: mov      r1, r8
007e4a98: bl       #0x30e70c
007e4a9c: cmp      r0, #0
007e4aa0: add      r5, r5, #1
007e4aa4: beq      #0x7e49b8
007e4aa8: mov      r0, #0
007e4aac: add      sp, sp, #0x34
007e4ab0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e4ab4: ldr      r2, [sp, #0x20]
007e4ab8: cmn      r2, #1
007e4abc: beq      #0x7e4aa8
007e4ac0: ldr      r3, [sp, #0x2c]
007e4ac4: str      r8, [r3]
007e4ac8: ldr      r1, [sp, #0x24]
007e4acc: add      r3, r2, #0x13
007e4ad0: ldr      r2, [sp, #0x1c]
007e4ad4: ldr      r5, [r1, r3, lsl #3]
007e4ad8: add      r3, r1, r3, lsl #3
007e4adc: ldr      r1, [r2, #8]
007e4ae0: mov      r0, r5
007e4ae4: ldr      r4, [r3, #4]
007e4ae8: bl       #0x30ed6c
007e4aec: ldr      r3, [sp, #0x1c]
007e4af0: mov      r6, r0
007e4af4: mov      r0, r4
007e4af8: ldr      r1, [r3, #0x10]
007e4afc: bl       #0x30ed6c
007e4b00: mov      r1, r0
007e4b04: mov      r0, r6
007e4b08: bl       #0x30eba4
007e4b0c: ldr      r2, [sp, #0x1c]
007e4b10: mov      r6, r0
007e4b14: mov      r0, r5
007e4b18: ldr      r1, [r2, #0xc]
007e4b1c: bl       #0x30ed6c
007e4b20: ldr      r3, [sp, #0x1c]
007e4b24: mov      r5, r0
007e4b28: mov      r0, r4
007e4b2c: ldr      r1, [r3, #0x14]
007e4b30: bl       #0x30ed6c
007e4b34: mov      r1, r0
007e4b38: mov      r0, r5
007e4b3c: bl       #0x30eba4
007e4b40: ldr      r1, [sp, #0x28]
007e4b44: str      r0, [r1, #4]
007e4b48: str      r6, [r1]
007e4b4c: mov      r0, #1
007e4b50: b        #0x7e4aac

# _ZN15b2RevoluteJoint9SetLimitsEff
007f2bc8: str      r2, [r0, #0x94]
007f2bcc: str      r1, [r0, #0x90]
007f2bd0: bx       lr

# _ZN16b2PrismaticJointC2EPK19b2PrismaticJointDef
007efb0c: push     {r4, r5, r6, lr}
007efb10: ldr      r6, [pc, #0xf0]
007efb14: mov      r4, r0
007efb18: mov      r5, r1
007efb1c: bl       #0x7eb1e0
007efb20: ldr      r2, [pc, #0xe4]
007efb24: add      r6, pc, r6
007efb28: mov      r3, #0
007efb2c: ldr      r2, [r6, r2]
007efb30: mov      r0, r4
007efb34: add      r2, r2, #8
007efb38: str      r2, [r4]
007efb3c: ldr      r2, [r5, #0x14]
007efb40: str      r2, [r4, #0x44]
007efb44: ldr      r2, [r5, #0x18]
007efb48: str      r2, [r4, #0x48]
007efb4c: ldr      r2, [r5, #0x1c]
007efb50: str      r2, [r4, #0x4c]
007efb54: ldr      r2, [r5, #0x20]
007efb58: str      r2, [r4, #0x50]
007efb5c: ldr      r2, [r5, #0x24]
007efb60: str      r2, [r4, #0x54]
007efb64: ldr      r2, [r5, #0x28]
007efb68: ldr      ip, [r4, #0x54]
007efb6c: add      r1, r2, #0x80000000
007efb70: str      ip, [r4, #0x60]
007efb74: str      r1, [r4, #0x5c]
007efb78: str      r2, [r4, #0x58]
007efb7c: ldr      r2, [r5, #0x2c]
007efb80: str      r3, [r4, #0x68]
007efb84: str      r3, [r4, #0x6c]
007efb88: str      r3, [r4, #0x70]
007efb8c: str      r3, [r4, #0x74]
007efb90: str      r3, [r4, #0x78]
007efb94: str      r3, [r4, #0x7c]
007efb98: str      r3, [r4, #0x80]
007efb9c: str      r3, [r4, #0x84]
007efba0: str      r3, [r4, #0x88]
007efba4: str      r3, [r4, #0x8c]
007efba8: str      r3, [r4, #0x90]
007efbac: str      r3, [r4, #0x94]
007efbb0: str      r3, [r4, #0x98]
007efbb4: str      r3, [r4, #0x9c]
007efbb8: str      r2, [r4, #0x64]
007efbbc: str      r3, [r4, #0xa0]
007efbc0: str      r3, [r4, #0xb4]
007efbc4: str      r3, [r4, #0xa4]
007efbc8: str      r3, [r4, #0xa8]
007efbcc: str      r3, [r4, #0xac]
007efbd0: str      r3, [r4, #0xb0]
007efbd4: ldr      r3, [r5, #0x34]
007efbd8: str      r3, [r4, #0xb8]
007efbdc: ldr      r3, [r5, #0x38]
007efbe0: str      r3, [r4, #0xbc]
007efbe4: ldr      r3, [r5, #0x40]
007efbe8: str      r3, [r4, #0xc0]
007efbec: ldr      r3, [r5, #0x44]
007efbf0: str      r3, [r4, #0xc4]
007efbf4: ldrb     r3, [r5, #0x30]
007efbf8: strb     r3, [r4, #0xc8]
007efbfc: ldrb     r3, [r5, #0x3c]
007efc00: strb     r3, [r4, #0xc9]
007efc04: pop      {r4, r5, r6, pc}
007efc08: andseq   r4, sl, ip, ror #30
007efc0c: andeq    r0, r0, ip, lsr #13

# _ZN7b2World12DestroyJointEP7b2Joint
007e7b1c: push     {r4, r5, r6, r7, r8, lr}
007e7b20: ldr      r3, [r1, #8]
007e7b24: ldrb     r7, [r1, #0x3d]
007e7b28: mov      r5, r0
007e7b2c: cmp      r3, #0
007e7b30: ldrne    r2, [r1, #0xc]
007e7b34: strne    r2, [r3, #0xc]
007e7b38: ldr      r3, [r1, #0xc]
007e7b3c: cmp      r3, #0
007e7b40: ldrne    r2, [r1, #8]
007e7b44: strne    r2, [r3, #8]
007e7b48: mov      r3, #0x19000
007e7b4c: add      r3, r3, #0x234
007e7b50: ldr      r2, [r0, r3]
007e7b54: cmp      r2, r1
007e7b58: ldreq    r2, [r1, #0xc]
007e7b5c: streq    r2, [r0, r3]
007e7b60: ldr      r6, [r1, #0x30]
007e7b64: ldr      r4, [r1, #0x34]
007e7b68: mov      r3, #0
007e7b6c: ldrh     r2, [r6]
007e7b70: str      r3, [r6, #0x8c]
007e7b74: mov      r0, r1
007e7b78: bic      r2, r2, #8
007e7b7c: strh     r2, [r6]
007e7b80: ldrh     r2, [r4]
007e7b84: str      r3, [r4, #0x8c]
007e7b88: bic      r3, r2, #8
007e7b8c: strh     r3, [r4]
007e7b90: ldr      r3, [r1, #0x18]
007e7b94: cmp      r3, #0
007e7b98: ldrne    r2, [r1, #0x1c]
007e7b9c: strne    r2, [r3, #0xc]
007e7ba0: ldr      r3, [r1, #0x1c]
007e7ba4: cmp      r3, #0
007e7ba8: ldrne    r2, [r1, #0x18]
007e7bac: strne    r2, [r3, #8]
007e7bb0: ldr      r2, [r6, #0x6c]
007e7bb4: add      r3, r1, #0x10
007e7bb8: cmp      r2, r3
007e7bbc: ldreq    r3, [r1, #0x1c]
007e7bc0: streq    r3, [r6, #0x6c]
007e7bc4: ldr      r2, [r1, #0x28]
007e7bc8: mov      r3, #0
007e7bcc: str      r3, [r1, #0x1c]
007e7bd0: cmp      r2, r3
007e7bd4: str      r3, [r1, #0x18]
007e7bd8: ldrne    r3, [r1, #0x2c]
007e7bdc: strne    r3, [r2, #0xc]
007e7be0: ldr      r3, [r1, #0x2c]
007e7be4: cmp      r3, #0
007e7be8: ldrne    r2, [r1, #0x28]
007e7bec: strne    r2, [r3, #8]
007e7bf0: ldr      r2, [r4, #0x6c]
007e7bf4: add      r3, r1, #0x20
007e7bf8: cmp      r2, r3
007e7bfc: ldreq    r3, [r1, #0x2c]
007e7c00: streq    r3, [r4, #0x6c]
007e7c04: mov      r3, #0
007e7c08: str      r3, [r1, #0x2c]
007e7c0c: str      r3, [r1, #0x28]
007e7c10: mov      r1, r5
007e7c14: bl       #0x7eb2bc
007e7c18: mov      r3, #0x19000
007e7c1c: add      r3, r3, #0x244
007e7c20: ldr      r2, [r5, r3]
007e7c24: cmp      r7, #0
007e7c28: sub      r2, r2, #1
007e7c2c: str      r2, [r5, r3]
007e7c30: bne      #0x7e7c7c
007e7c34: ldr      r7, [r6, #0x68]
007e7c38: ldr      r3, [r4, #0x68]
007e7c3c: cmp      r7, r3
007e7c40: movlt    r7, r6
007e7c44: movge    r7, r4
007e7c48: ldr      r4, [r7, #0x64]
007e7c4c: cmp      r4, #0
007e7c50: beq      #0x7e7c7c
007e7c54: mov      r6, #0x19000
007e7c58: add      r7, r7, #4
007e7c5c: add      r6, r6, #0x1d8
007e7c60: mov      r0, r4
007e7c64: ldr      r1, [r5, r6]
007e7c68: mov      r2, r7
007e7c6c: bl       #0x7e61b0
007e7c70: ldr      r4, [r4, #8]
007e7c74: cmp      r4, #0
007e7c78: bne      #0x7e7c60
007e7c7c: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK14b2PolygonShape8CentroidERK7b2XForm
007e51dc: push     {r4, r5, r6, r7, r8, lr}
007e51e0: ldr      r4, [r1, #0x30]
007e51e4: ldr      r7, [r1, #0x34]
007e51e8: mov      r6, r0
007e51ec: ldr      r1, [r2, #8]
007e51f0: mov      r0, r4
007e51f4: mov      r5, r2
007e51f8: bl       #0x30ed6c
007e51fc: ldr      r1, [r5, #0x10]
007e5200: mov      r8, r0
007e5204: mov      r0, r7
007e5208: bl       #0x30ed6c
007e520c: mov      r1, r0
007e5210: mov      r0, r8
007e5214: bl       #0x30eba4
007e5218: ldr      r1, [r5, #0xc]
007e521c: mov      r8, r0
007e5220: mov      r0, r4
007e5224: bl       #0x30ed6c
007e5228: ldr      r1, [r5, #0x14]
007e522c: mov      r4, r0
007e5230: mov      r0, r7
007e5234: bl       #0x30ed6c
007e5238: mov      r1, r0
007e523c: mov      r0, r4
007e5240: bl       #0x30eba4
007e5244: ldr      r1, [r5, #4]
007e5248: bl       #0x30eba4
007e524c: ldr      r1, [r5]
007e5250: mov      r4, r0
007e5254: mov      r0, r8
007e5258: bl       #0x30eba4
007e525c: str      r4, [r6, #4]
007e5260: str      r0, [r6]
007e5264: mov      r0, r6
007e5268: pop      {r4, r5, r6, r7, r8, pc}

# _ZN16b2PulleyJointDef10InitializeEP6b2BodyS1_RK6b2Vec2S4_S4_S4_f
007f1038: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f103c: mov      r4, r0
007f1040: str      r2, [r4, #0xc]
007f1044: str      r1, [r4, #8]
007f1048: mov      fp, r3
007f104c: ldr      r3, [r3]
007f1050: sub      sp, sp, #0x14
007f1054: ldr      sl, [sp, #0x38]
007f1058: ldr      sb, [sp, #0x3c]
007f105c: str      r3, [r0, #0x14]
007f1060: ldr      r3, [fp, #4]
007f1064: ldr      r8, [sp, #0x40]
007f1068: ldr      r7, [sp, #0x44]
007f106c: str      r3, [r0, #0x18]
007f1070: ldr      r3, [sl]
007f1074: mov      r6, r1
007f1078: mov      r5, r2
007f107c: str      r3, [r0, #0x1c]
007f1080: ldr      r3, [sl, #4]
007f1084: str      r3, [r0, #0x20]
007f1088: ldr      r1, [r1, #4]
007f108c: ldr      r0, [sb]
007f1090: bl       #0x30e3ac
007f1094: str      r0, [sp, #8]
007f1098: ldr      r1, [r6, #8]
007f109c: ldr      r0, [sb, #4]
007f10a0: bl       #0x30e3ac
007f10a4: str      r0, [sp, #0xc]
007f10a8: ldr      r1, [r6, #0xc]
007f10ac: ldr      r0, [sp, #8]
007f10b0: bl       #0x30ed6c
007f10b4: ldr      r1, [r6, #0x10]
007f10b8: mov      r3, r0
007f10bc: ldr      r0, [sp, #0xc]
007f10c0: str      r3, [sp]
007f10c4: bl       #0x30ed6c
007f10c8: ldr      r3, [sp]
007f10cc: mov      r1, r0
007f10d0: mov      r0, r3
007f10d4: bl       #0x30eba4
007f10d8: ldr      r1, [r6, #0x14]
007f10dc: mov      r2, r0
007f10e0: ldr      r0, [sp, #8]
007f10e4: str      r2, [sp, #4]
007f10e8: bl       #0x30ed6c
007f10ec: ldr      r1, [r6, #0x18]
007f10f0: mov      r3, r0
007f10f4: ldr      r0, [sp, #0xc]
007f10f8: str      r3, [sp]
007f10fc: bl       #0x30ed6c
007f1100: ldr      r3, [sp]
007f1104: mov      r1, r0
007f1108: mov      r0, r3
007f110c: bl       #0x30eba4
007f1110: ldr      r2, [sp, #4]
007f1114: str      r0, [r4, #0x28]
007f1118: str      r2, [r4, #0x24]
007f111c: ldr      r1, [r5, #4]
007f1120: ldr      r0, [r8]
007f1124: bl       #0x30e3ac
007f1128: ldr      r1, [r5, #8]
007f112c: mov      r6, r0
007f1130: ldr      r0, [r8, #4]
007f1134: bl       #0x30e3ac
007f1138: str      r0, [sp, #8]
007f113c: ldr      r1, [r5, #0xc]
007f1140: mov      r0, r6
007f1144: bl       #0x30ed6c
007f1148: ldr      r1, [r5, #0x10]
007f114c: mov      r3, r0
007f1150: ldr      r0, [sp, #8]
007f1154: str      r3, [sp]
007f1158: bl       #0x30ed6c
007f115c: ldr      r3, [sp]
007f1160: mov      r1, r0
007f1164: mov      r0, r3
007f1168: bl       #0x30eba4
007f116c: ldr      r1, [r5, #0x14]
007f1170: mov      r3, r0
007f1174: mov      r0, r6
007f1178: str      r3, [sp]
007f117c: bl       #0x30ed6c
007f1180: ldr      r1, [r5, #0x18]
007f1184: mov      r6, r0
007f1188: ldr      r0, [sp, #8]
007f118c: bl       #0x30ed6c
007f1190: mov      r1, r0
007f1194: mov      r0, r6
007f1198: bl       #0x30eba4
007f119c: str      r0, [r4, #0x30]
007f11a0: ldr      r3, [sp]
007f11a4: str      r3, [r4, #0x2c]
007f11a8: ldr      r1, [fp]
007f11ac: ldr      r0, [sb]
007f11b0: bl       #0x30e3ac
007f11b4: ldr      r1, [fp, #4]
007f11b8: mov      r5, r0
007f11bc: ldr      r0, [sb, #4]
007f11c0: bl       #0x30e3ac
007f11c4: mov      r1, r5
007f11c8: mov      r6, r0
007f11cc: mov      r0, r5
007f11d0: bl       #0x30ed6c
007f11d4: mov      r1, r6
007f11d8: mov      r5, r0
007f11dc: mov      r0, r6
007f11e0: bl       #0x30ed6c
007f11e4: mov      r1, r0
007f11e8: mov      r0, r5
007f11ec: bl       #0x30eba4
007f11f0: bl       #0x30e124
007f11f4: str      r0, [r4, #0x34]
007f11f8: ldr      r1, [sl]
007f11fc: ldr      r0, [r8]
007f1200: bl       #0x30e3ac
007f1204: ldr      r1, [sl, #4]
007f1208: mov      r5, r0
007f120c: ldr      r0, [r8, #4]
007f1210: bl       #0x30e3ac
007f1214: mov      r1, r5
007f1218: mov      r6, r0
007f121c: mov      r0, r5
007f1220: bl       #0x30ed6c
007f1224: mov      r1, r6
007f1228: mov      r5, r0
007f122c: mov      r0, r6
007f1230: bl       #0x30ed6c
007f1234: mov      r1, r0
007f1238: mov      r0, r5
007f123c: bl       #0x30eba4
007f1240: bl       #0x30e124
007f1244: str      r7, [r4, #0x44]
007f1248: mov      r1, r0
007f124c: str      r0, [r4, #0x3c]
007f1250: mov      r0, r7
007f1254: bl       #0x30ed6c
007f1258: ldr      r1, [r4, #0x34]
007f125c: bl       #0x30eba4
007f1260: mov      r1, #0xc0000000
007f1264: mov      r5, r0
007f1268: mov      r0, r7
007f126c: bl       #0x30ed6c
007f1270: mov      r1, r5
007f1274: bl       #0x30eba4
007f1278: mov      r1, #0x40000000
007f127c: str      r0, [r4, #0x38]
007f1280: mov      r0, r5
007f1284: bl       #0x30e3ac
007f1288: mov      r1, r7
007f128c: bl       #0x30ec94
007f1290: str      r0, [r4, #0x40]
007f1294: add      sp, sp, #0x14
007f1298: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK7b2World12GetPairCountEv
007e6984: mov      r3, #0x19000
007e6988: add      r3, r3, #0x1d8
007e698c: ldr      r2, [r0, r3]
007e6990: mov      r3, #0x30000
007e6994: add      r3, r3, #0xc
007e6998: ldr      r0, [r2, r3]
007e699c: bx       lr

# _ZN7b2World18SetContactListenerEP17b2ContactListener
007e6604: mov      r3, #0x19000
007e6608: add      r3, r3, #0x264
007e660c: str      r1, [r0, r3]
007e6610: bx       lr

# _ZN9b2Contact7AddTypeEPFPS_P7b2ShapeS2_P16b2BlockAllocatorEPFvS0_S4_E11b2ShapeTypeS9_
007e9a9c: push     {r4, r5, r6, r7}
007e9aa0: ldr      ip, [pc, #0x58]
007e9aa4: mov      r4, #0x18
007e9aa8: mul      r5, r4, r2
007e9aac: ldr      r6, [pc, #0x50]
007e9ab0: add      ip, pc, ip
007e9ab4: mov      r7, #0xc
007e9ab8: mla      r5, r7, r3, r5
007e9abc: ldr      r6, [ip, r6]
007e9ac0: cmp      r2, r3
007e9ac4: add      ip, r6, r5
007e9ac8: str      r0, [r6, r5]
007e9acc: mov      r5, #1
007e9ad0: strb     r5, [ip, #8]
007e9ad4: str      r1, [ip, #4]
007e9ad8: beq      #0x7e9af8
007e9adc: mul      r4, r4, r3
007e9ae0: mov      r3, #0
007e9ae4: mla      r2, r7, r2, r4
007e9ae8: add      ip, r6, r2
007e9aec: str      r0, [r6, r2]
007e9af0: strb     r3, [ip, #8]
007e9af4: str      r1, [ip, #4]
007e9af8: pop      {r4, r5, r6, r7}
007e9afc: bx       lr
007e9b00: andseq   sl, sl, r0, ror #31
007e9b04: strdeq   r1, r2, [r0], -r4

# _ZNK16b2PrismaticJoint14IsMotorEnabledEv
007eefd0: ldrb     r0, [r0, #0xc9]
007eefd4: bx       lr

# _ZN7b2World4StepEfi
007e8b1c: push     {r4, r5, r6, lr}
007e8b20: mov      r3, #0x19000
007e8b24: mov      r5, r1
007e8b28: mov      r6, #0
007e8b2c: add      r3, r3, #0x1d4
007e8b30: mov      r1, #1
007e8b34: strb     r1, [r0, r3]
007e8b38: sub      sp, sp, #0x18
007e8b3c: mov      r4, r0
007e8b40: mov      r1, r6
007e8b44: mov      r0, r5
007e8b48: str      r2, [sp, #0x10]
007e8b4c: str      r5, [sp, #4]
007e8b50: bl       #0x30e2f8
007e8b54: cmp      r0, #0
007e8b58: streq    r6, [sp, #8]
007e8b5c: beq      #0x7e8b70
007e8b60: mov      r0, #0x3f800000
007e8b64: mov      r1, r5
007e8b68: bl       #0x30ec94
007e8b6c: str      r0, [sp, #8]
007e8b70: mov      r3, #0x19000
007e8b74: add      r3, r3, #0x26c
007e8b78: ldr      r0, [r4, r3]
007e8b7c: mov      r1, r5
007e8b80: bl       #0x30ed6c
007e8b84: mov      r2, #0x19000
007e8b88: movw     r3, #0x9275
007e8b8c: add      r2, r2, #0x274
007e8b90: movt     r3, #1
007e8b94: ldrb     r2, [r4, r2]
007e8b98: ldrb     r3, [r4, r3]
007e8b9c: add      r1, r4, #0x19000
007e8ba0: str      r0, [sp, #0xc]
007e8ba4: add      r0, r1, #0x1dc
007e8ba8: strb     r2, [sp, #0x15]
007e8bac: strb     r3, [sp, #0x14]
007e8bb0: bl       #0x7ea010
007e8bb4: ldr      r0, [sp, #4]
007e8bb8: mov      r1, #0
007e8bbc: bl       #0x30e2f8
007e8bc0: cmp      r0, #0
007e8bc4: bne      #0x7e8c30
007e8bc8: movw     r3, #0x9276
007e8bcc: movt     r3, #1
007e8bd0: ldrb     r3, [r4, r3]
007e8bd4: cmp      r3, #0
007e8bd8: beq      #0x7e8bf0
007e8bdc: ldr      r0, [sp, #4]
007e8be0: mov      r1, #0
007e8be4: bl       #0x30e2f8
007e8be8: cmp      r0, #0
007e8bec: bne      #0x7e8c20
007e8bf0: mov      r0, r4
007e8bf4: bl       #0x7e6b78
007e8bf8: ldr      r1, [sp, #8]
007e8bfc: mov      r2, #0x19000
007e8c00: mov      r3, r2
007e8c04: add      r2, r2, #0x26c
007e8c08: str      r1, [r4, r2]
007e8c0c: add      r3, r3, #0x1d4
007e8c10: mov      r2, #0
007e8c14: strb     r2, [r4, r3]
007e8c18: add      sp, sp, #0x18
007e8c1c: pop      {r4, r5, r6, pc}
007e8c20: mov      r0, r4
007e8c24: add      r1, sp, #4
007e8c28: bl       #0x7e8458
007e8c2c: b        #0x7e8bf0
007e8c30: mov      r0, r4
007e8c34: add      r1, sp, #4
007e8c38: bl       #0x7e76dc
007e8c3c: b        #0x7e8bc8

# _ZN22b2PolyAndCircleContactC2EP7b2ShapeS1_
007ebf18: push     {r4, r5, r6, lr}
007ebf1c: ldr      r5, [pc, #0x34]
007ebf20: mov      r4, r0
007ebf24: bl       #0x7e9eb8
007ebf28: ldr      r3, [pc, #0x2c]
007ebf2c: add      r5, pc, r5
007ebf30: mov      r2, #0
007ebf34: ldr      r3, [r5, r3]
007ebf38: mov      r1, #0
007ebf3c: str      r1, [r4, #0x90]
007ebf40: add      r3, r3, #8
007ebf44: str      r3, [r4]
007ebf48: str      r2, [r4, #0x60]
007ebf4c: str      r2, [r4, #0x5c]
007ebf50: mov      r0, r4
007ebf54: pop      {r4, r5, r6, pc}
007ebf58: andseq   r8, sl, r4, ror #22
007ebf5c: strheq   r3, [r0], -r8

# _ZN12b2MouseJoint9SetTargetERK6b2Vec2
007eb480: ldr      r3, [r0, #0x34]
007eb484: ldrh     r2, [r3]
007eb488: tst      r2, #8
007eb48c: bicne    r2, r2, #8
007eb490: strhne   r2, [r3]
007eb494: movne    r2, #0
007eb498: strne    r2, [r3, #0x8c]
007eb49c: ldr      r3, [r1]
007eb4a0: str      r3, [r0, #0x4c]
007eb4a4: ldr      r3, [r1, #4]
007eb4a8: str      r3, [r0, #0x50]
007eb4ac: bx       lr

# _ZN16b2StackAllocator8AllocateEi
007f3644: push     {r4, r5, r6, r7, r8, lr}
007f3648: mov      r3, #0x19000
007f364c: add      r3, r3, #0x18c
007f3650: ldr      r6, [r0, r3]
007f3654: mov      r5, #0xc
007f3658: mov      r3, #0x19000
007f365c: mla      r5, r5, r6, r0
007f3660: add      r3, r3, #0x10
007f3664: str      r1, [r5, r3]
007f3668: mov      r3, #0x19000
007f366c: mov      r7, r1
007f3670: ldr      r1, [r0, r3]
007f3674: add      r8, r5, r3
007f3678: mov      r4, r0
007f367c: add      r2, r7, r1
007f3680: cmp      r2, r3
007f3684: add      r8, r8, #0x10
007f3688: bgt      #0x7f3708
007f368c: mov      r2, #0x19000
007f3690: add      r2, r2, #0xc
007f3694: add      r1, r0, r1
007f3698: str      r1, [r5, r2]
007f369c: mov      r2, #0
007f36a0: strb     r2, [r8, #4]
007f36a4: ldr      r2, [r0, r3]
007f36a8: add      r2, r7, r2
007f36ac: str      r2, [r0, r3]
007f36b0: mov      r3, #0x19000
007f36b4: add      r3, r3, #4
007f36b8: ldr      r1, [r4, r3]
007f36bc: mov      r2, #0x19000
007f36c0: add      r2, r2, #8
007f36c4: add      r7, r7, r1
007f36c8: str      r7, [r4, r3]
007f36cc: ldr      r1, [r4, r2]
007f36d0: mov      r3, #0x19000
007f36d4: add      r3, r3, #0x18c
007f36d8: cmp      r7, r1
007f36dc: strge    r7, [r4, r2]
007f36e0: strlt    r1, [r4, r2]
007f36e4: ldr      r1, [r4, r3]
007f36e8: mov      r2, #0xc
007f36ec: mla      r6, r2, r6, r4
007f36f0: add      r1, r1, #1
007f36f4: mov      r2, #0x19000
007f36f8: str      r1, [r4, r3]
007f36fc: add      r2, r2, #0xc
007f3700: ldr      r0, [r6, r2]
007f3704: pop      {r4, r5, r6, r7, r8, pc}
007f3708: mov      r0, r7
007f370c: bl       #0x7f34f4
007f3710: mov      r3, #0x19000
007f3714: add      r3, r3, #0xc
007f3718: str      r0, [r5, r3]
007f371c: mov      r3, #1
007f3720: strb     r3, [r8, #4]
007f3724: b        #0x7f36b0

# _ZN14b2PairCallbackD1Ev
007e65bc: bx       lr

# _ZN8b2IslandD1Ev
007eb078: push     {r4, lr}
007eb07c: mov      r4, r0
007eb080: ldr      r1, [r4, #0x10]
007eb084: ldr      r0, [r0]
007eb088: bl       #0x7f35a8
007eb08c: ldr      r0, [r4]
007eb090: ldr      r1, [r4, #0xc]
007eb094: bl       #0x7f35a8
007eb098: ldr      r0, [r4]
007eb09c: ldr      r1, [r4, #8]
007eb0a0: bl       #0x7f35a8
007eb0a4: mov      r0, r4
007eb0a8: pop      {r4, pc}

# _ZN6b2BodyC1EPK9b2BodyDefP7b2World
007e2218: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e221c: mov      r3, #0
007e2220: strh     r3, [r0]
007e2224: ldrb     r3, [r1, #0x2b]
007e2228: mov      r4, r0
007e222c: mov      r5, r1
007e2230: cmp      r3, #0
007e2234: movne    r3, #0x20
007e2238: strhne   r3, [r0]
007e223c: ldrb     r3, [r1, #0x2a]
007e2240: mov      sb, #0x3f800000
007e2244: cmp      r3, #0
007e2248: ldrhne   r3, [r0]
007e224c: orrne    r3, r3, #0x40
007e2250: strhne   r3, [r0]
007e2254: ldrb     r3, [r1, #0x28]
007e2258: cmp      r3, #0
007e225c: ldrhne   r3, [r0]
007e2260: orrne    r3, r3, #0x10
007e2264: strhne   r3, [r0]
007e2268: ldrb     r3, [r1, #0x29]
007e226c: str      r2, [r0, #0x58]
007e2270: cmp      r3, #0
007e2274: ldrhne   r3, [r0]
007e2278: orrne    r3, r3, #8
007e227c: strhne   r3, [r0]
007e2280: ldr      r3, [r1, #0x14]
007e2284: str      r3, [r0, #4]
007e2288: ldr      r3, [r1, #0x18]
007e228c: str      r3, [r0, #8]
007e2290: ldr      r7, [r1, #0x1c]
007e2294: mov      r0, r7
007e2298: bl       #0x30e754
007e229c: mov      r6, r0
007e22a0: mov      r0, r7
007e22a4: bl       #0x30eb08
007e22a8: add      sl, r0, #0x80000000
007e22ac: str      r6, [r4, #0xc]
007e22b0: str      sl, [r4, #0x14]
007e22b4: str      r0, [r4, #0x10]
007e22b8: str      r6, [r4, #0x18]
007e22bc: ldr      r3, [r5, #4]
007e22c0: mov      r7, r0
007e22c4: mov      r1, r6
007e22c8: str      r3, [r4, #0x1c]
007e22cc: ldr      r3, [r5, #8]
007e22d0: str      sb, [r4, #0x3c]
007e22d4: ldr      r8, [r4, #0x1c]
007e22d8: str      r3, [r4, #0x20]
007e22dc: ldr      r3, [r5, #0x1c]
007e22e0: mov      r0, r8
007e22e4: str      r3, [r4, #0x34]
007e22e8: str      r3, [r4, #0x38]
007e22ec: bl       #0x30ed6c
007e22f0: mov      r1, sl
007e22f4: mov      fp, r0
007e22f8: ldr      r0, [r4, #0x20]
007e22fc: bl       #0x30ed6c
007e2300: mov      r1, r0
007e2304: mov      r0, fp
007e2308: bl       #0x30eba4
007e230c: mov      r1, r7
007e2310: mov      sl, r0
007e2314: mov      r0, r8
007e2318: bl       #0x30ed6c
007e231c: mov      r1, r6
007e2320: mov      r7, r0
007e2324: ldr      r0, [r4, #0x20]
007e2328: bl       #0x30ed6c
007e232c: mov      r1, r0
007e2330: mov      r0, r7
007e2334: bl       #0x30eba4
007e2338: ldr      r1, [r4, #4]
007e233c: mov      r7, r0
007e2340: mov      r0, sl
007e2344: bl       #0x30eba4
007e2348: ldr      r1, [r4, #8]
007e234c: mov      r6, r0
007e2350: mov      r0, r7
007e2354: bl       #0x30eba4
007e2358: str      r6, [r4, #0x2c]
007e235c: str      r0, [r4, #0x30]
007e2360: ldr      r1, [r4, #0x2c]
007e2364: ldr      r2, [r4, #0x30]
007e2368: mov      r3, #0
007e236c: str      r1, [r4, #0x24]
007e2370: str      r2, [r4, #0x28]
007e2374: str      r3, [r4, #0x60]
007e2378: str      r3, [r4, #0x6c]
007e237c: str      r3, [r4, #0x70]
007e2380: str      r3, [r4, #0x5c]
007e2384: ldr      r2, [r5, #0x20]
007e2388: mov      r3, #0
007e238c: mov      r1, r3
007e2390: str      r2, [r4, #0x84]
007e2394: ldr      r2, [r5, #0x24]
007e2398: str      r3, [r4, #0x4c]
007e239c: str      r3, [r4, #0x50]
007e23a0: str      r2, [r4, #0x88]
007e23a4: str      r3, [r4, #0x54]
007e23a8: str      r3, [r4, #0x40]
007e23ac: str      r3, [r4, #0x44]
007e23b0: str      r3, [r4, #0x48]
007e23b4: str      r3, [r4, #0x8c]
007e23b8: str      r3, [r4, #0x78]
007e23bc: str      r3, [r4, #0x7c]
007e23c0: str      r3, [r4, #0x80]
007e23c4: ldr      r6, [r5]
007e23c8: str      r6, [r4, #0x74]
007e23cc: mov      r0, r6
007e23d0: bl       #0x30e2f8
007e23d4: cmp      r0, #0
007e23d8: beq      #0x7e23ec
007e23dc: mov      r0, sb
007e23e0: mov      r1, r6
007e23e4: bl       #0x30ec94
007e23e8: str      r0, [r4, #0x78]
007e23ec: ldrh     r3, [r4]
007e23f0: mov      r1, #0
007e23f4: tst      r3, #0x40
007e23f8: ldreq    r6, [r5, #0xc]
007e23fc: ldrne    r6, [r4, #0x7c]
007e2400: streq    r6, [r4, #0x7c]
007e2404: mov      r0, r6
007e2408: bl       #0x30e2f8
007e240c: cmp      r0, #0
007e2410: beq      #0x7e2424
007e2414: mov      r1, r6
007e2418: mov      r0, #0x3f800000
007e241c: bl       #0x30ec94
007e2420: str      r0, [r4, #0x80]
007e2424: ldr      r0, [r4, #0x78]
007e2428: mov      r1, #0
007e242c: bl       #0x30df8c
007e2430: cmp      r0, #0
007e2434: beq      #0x7e2454
007e2438: ldr      r0, [r4, #0x80]
007e243c: mov      r1, #0
007e2440: bl       #0x30df8c
007e2444: cmp      r0, #0
007e2448: movne    r2, #0
007e244c: strhne   r2, [r4, #2]
007e2450: bne      #0x7e245c
007e2454: mov      r3, #1
007e2458: strh     r3, [r4, #2]
007e245c: ldr      r2, [r5, #0x10]
007e2460: mov      r3, #0
007e2464: str      r3, [r4, #0x68]
007e2468: str      r2, [r4, #0x90]
007e246c: str      r3, [r4, #0x64]
007e2470: mov      r0, r4
007e2474: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN16b2StackAllocatorD1Ev
007f3594: bx       lr

# _ZN13b2PulleyJointC2EPK16b2PulleyJointDef
007f0668: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007f066c: ldr      r6, [pc, #0x144]
007f0670: mov      r4, r0
007f0674: mov      r5, r1
007f0678: bl       #0x7eb1e0
007f067c: ldr      r2, [pc, #0x138]
007f0680: add      r6, pc, r6
007f0684: ldr      r1, [r4, #0x30]
007f0688: ldr      r2, [r6, r2]
007f068c: mov      r3, #0x19000
007f0690: add      r3, r3, #0x254
007f0694: add      r2, r2, #8
007f0698: str      r2, [r4]
007f069c: ldr      r2, [r1, #0x58]
007f06a0: ldr      r6, [r2, r3]
007f06a4: str      r6, [r4, #0x44]
007f06a8: ldr      r1, [r6, #4]
007f06ac: ldr      r0, [r5, #0x14]
007f06b0: bl       #0x30e3ac
007f06b4: ldr      r1, [r6, #8]
007f06b8: mov      r7, r0
007f06bc: ldr      r0, [r5, #0x18]
007f06c0: bl       #0x30e3ac
007f06c4: str      r7, [r4, #0x48]
007f06c8: str      r0, [r4, #0x4c]
007f06cc: ldr      r1, [r6, #4]
007f06d0: ldr      r0, [r5, #0x1c]
007f06d4: bl       #0x30e3ac
007f06d8: ldr      r1, [r6, #8]
007f06dc: mov      r7, r0
007f06e0: ldr      r0, [r5, #0x20]
007f06e4: bl       #0x30e3ac
007f06e8: str      r7, [r4, #0x50]
007f06ec: str      r0, [r4, #0x54]
007f06f0: ldr      r3, [r5, #0x24]
007f06f4: str      r3, [r4, #0x58]
007f06f8: ldr      r3, [r5, #0x28]
007f06fc: str      r3, [r4, #0x5c]
007f0700: ldr      r3, [r5, #0x2c]
007f0704: str      r3, [r4, #0x60]
007f0708: ldr      r3, [r5, #0x30]
007f070c: str      r3, [r4, #0x64]
007f0710: ldr      r6, [r5, #0x44]
007f0714: str      r6, [r4, #0x7c]
007f0718: ldr      r1, [r5, #0x3c]
007f071c: mov      r0, r6
007f0720: bl       #0x30ed6c
007f0724: ldr      r1, [r5, #0x34]
007f0728: bl       #0x30eba4
007f072c: mov      r1, #0xc0000000
007f0730: mov      r7, r0
007f0734: str      r0, [r4, #0x78]
007f0738: mov      r0, r6
007f073c: bl       #0x30ed6c
007f0740: mov      r1, r0
007f0744: mov      r0, r7
007f0748: bl       #0x30eba4
007f074c: ldr      r8, [r5, #0x38]
007f0750: mov      sl, r0
007f0754: mov      r1, sl
007f0758: mov      r0, r8
007f075c: bl       #0x30e70c
007f0760: cmp      r0, #0
007f0764: moveq    r8, sl
007f0768: str      r8, [r4, #0x80]
007f076c: mov      r0, r7
007f0770: mov      r1, #0x40000000
007f0774: bl       #0x30e3ac
007f0778: mov      r1, r6
007f077c: bl       #0x30ec94
007f0780: ldr      r5, [r5, #0x40]
007f0784: mov      r6, r0
007f0788: mov      r1, r6
007f078c: mov      r0, r5
007f0790: bl       #0x30e70c
007f0794: cmp      r0, #0
007f0798: mov      r3, #0
007f079c: moveq    r5, r6
007f07a0: str      r5, [r4, #0x84]
007f07a4: mov      r0, r4
007f07a8: str      r3, [r4, #0x9c]
007f07ac: str      r3, [r4, #0x94]
007f07b0: str      r3, [r4, #0x98]
007f07b4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007f07b8: andseq   r4, sl, r0, lsl r4
007f07bc: strdeq   r3, r4, [r0], -r8

# _ZN16b2BlockAllocator4FreeEPvi
007e8da0: ldr      r3, [pc, #0x2c]
007e8da4: cmp      r2, #0
007e8da8: add      r3, pc, r3
007e8dac: bxeq     lr
007e8db0: ldr      ip, [pc, #0x20]
007e8db4: ldr      r3, [r3, ip]
007e8db8: ldrb     r3, [r3, r2]
007e8dbc: add      r3, r3, #2
007e8dc0: add      r0, r0, r3, lsl #2
007e8dc4: ldr      r3, [r0, #4]
007e8dc8: str      r3, [r1]
007e8dcc: str      r1, [r0, #4]
007e8dd0: bx       lr
007e8dd4: andseq   fp, sl, r8, ror #25
007e8dd8: strdeq   r1, r2, [r0], -ip

# _ZN22b2PolyAndCircleContactD0Ev
007ebebc: push     {r4, lr}
007ebec0: mov      r4, r0
007ebec4: bl       #0x30e2b0
007ebec8: mov      r0, r4
007ebecc: pop      {r4, pc}

# _Z14b2TimeOfImpactPK7b2ShapeRK7b2SweepS1_S4_
007f3728: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f372c: sub      sp, sp, #0x94
007f3730: str      r1, [sp, #0x10]
007f3734: ldr      ip, [sp, #0x10]
007f3738: str      r0, [sp, #0x18]
007f373c: ldr      r1, [r1, #8]
007f3740: ldr      r0, [ip, #0x10]
007f3744: str      r3, [sp, #0x14]
007f3748: str      r2, [sp, #0x1c]
007f374c: bl       #0x30e3ac
007f3750: str      r0, [sp, #0x34]
007f3754: ldr      r0, [sp, #0x10]
007f3758: ldr      sl, [pc, #0x328]
007f375c: mov      r5, #0
007f3760: ldr      r1, [r0, #0xc]
007f3764: ldr      r0, [r0, #0x14]
007f3768: bl       #0x30e3ac
007f376c: ldr      r2, [sp, #0x14]
007f3770: str      r0, [sp, #0x38]
007f3774: add      sl, pc, sl
007f3778: ldr      r1, [r2, #8]
007f377c: ldr      r0, [r2, #0x10]
007f3780: bl       #0x30e3ac
007f3784: ldr      r3, [sp, #0x14]
007f3788: str      r0, [sp, #0x2c]
007f378c: add      r7, sp, #0x60
007f3790: ldr      r1, [r3, #0xc]
007f3794: ldr      r0, [r3, #0x14]
007f3798: bl       #0x30e3ac
007f379c: ldr      ip, [sp, #0x10]
007f37a0: str      r0, [sp, #0x30]
007f37a4: add      r6, sp, #0x48
007f37a8: ldr      r1, [ip, #0x18]
007f37ac: ldr      r0, [ip, #0x1c]
007f37b0: bl       #0x30e3ac
007f37b4: ldr      r1, [sp, #0x14]
007f37b8: mov      r8, r0
007f37bc: ldr      r0, [r1, #0x1c]
007f37c0: ldr      r1, [r1, #0x18]
007f37c4: bl       #0x30e3ac
007f37c8: ldr      ip, [sp, #0x18]
007f37cc: mov      r2, #0
007f37d0: str      r2, [sp, #0xc]
007f37d4: ldr      ip, [ip, #0x10]
007f37d8: mov      sb, r0
007f37dc: ldr      r0, [sp, #0x1c]
007f37e0: ldr      r3, [pc, #0x2a4]
007f37e4: str      ip, [sp, #0x40]
007f37e8: ldr      r0, [r0, #0x10]
007f37ec: ldr      r3, [sl, r3]
007f37f0: ldr      r1, [sp, #0x10]
007f37f4: str      r0, [sp, #0x3c]
007f37f8: ldr      r2, [r3, #4]
007f37fc: ldr      r1, [r1, #0x20]
007f3800: ldr      r3, [r3]
007f3804: add      ip, sp, #0x78
007f3808: str      r1, [sp, #0x20]
007f380c: str      r2, [sp, #0x7c]
007f3810: str      r3, [sp, #0x78]
007f3814: add      r2, sp, #0x88
007f3818: add      r3, sp, #0x80
007f381c: ldr      r4, [sp, #0xc]
007f3820: str      r2, [sp, #0x28]
007f3824: str      r3, [sp, #0x24]
007f3828: str      ip, [sp, #0x44]
007f382c: mov      r1, r4
007f3830: mov      r0, #0x3f800000
007f3834: bl       #0x30e3ac
007f3838: ldr      r1, [sp, #0x20]
007f383c: bl       #0x30ed6c
007f3840: mov      r1, r4
007f3844: bl       #0x30eba4
007f3848: mov      sl, r0
007f384c: mov      r2, sl
007f3850: ldr      r0, [sp, #0x10]
007f3854: mov      r1, r7
007f3858: bl       #0x7e3bfc
007f385c: mov      r2, sl
007f3860: ldr      r0, [sp, #0x14]
007f3864: mov      r1, r6
007f3868: bl       #0x7e3bfc
007f386c: ldr      ip, [sp, #0x1c]
007f3870: ldr      r0, [sp, #0x28]
007f3874: ldr      r1, [sp, #0x24]
007f3878: ldr      r2, [sp, #0x18]
007f387c: mov      r3, r7
007f3880: str      ip, [sp]
007f3884: str      r6, [sp, #4]
007f3888: bl       #0x7f9848
007f388c: cmp      r5, #0
007f3890: mov      sl, r0
007f3894: bne      #0x7f38e8
007f3898: movw     r1, #0xd70a
007f389c: movt     r1, #0x3da3
007f38a0: bl       #0x30e2f8
007f38a4: cmp      r0, #0
007f38a8: movwne   r0, #0xc28f
007f38ac: movtne   r0, #0x3d75
007f38b0: strne    r0, [sp, #0xc]
007f38b4: bne      #0x7f38e8
007f38b8: movw     r1, #0xd70a
007f38bc: movt     r1, #0x3ca3
007f38c0: mov      r0, sl
007f38c4: bl       #0x30e3ac
007f38c8: movw     r1, #0x126f
007f38cc: movt     r1, #0x3b03
007f38d0: str      r0, [sp, #0xc]
007f38d4: bl       #0x30e70c
007f38d8: cmp      r0, #0
007f38dc: movwne   r1, #0x126f
007f38e0: movtne   r1, #0x3b03
007f38e4: strne    r1, [sp, #0xc]
007f38e8: ldr      r1, [sp, #0xc]
007f38ec: mov      r0, sl
007f38f0: bl       #0x30e3ac
007f38f4: movw     r1, #0x126f
007f38f8: movt     r1, #0x3b03
007f38fc: mov      fp, r0
007f3900: bl       #0x30e70c
007f3904: cmp      r0, #0
007f3908: mov      r3, #0
007f390c: movne    r3, #1
007f3910: cmp      r5, #0x14
007f3914: orreq    r3, r3, #1
007f3918: tst      r3, #0xff
007f391c: beq      #0x7f392c
007f3920: mov      r0, r4
007f3924: add      sp, sp, #0x94
007f3928: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f392c: ldr      r1, [sp, #0x88]
007f3930: ldr      r0, [sp, #0x80]
007f3934: bl       #0x30e3ac
007f3938: ldr      r1, [sp, #0x8c]
007f393c: mov      sl, r0
007f3940: ldr      r0, [sp, #0x84]
007f3944: bl       #0x30e3ac
007f3948: str      r0, [sp, #0x7c]
007f394c: ldr      r0, [sp, #0x44]
007f3950: str      sl, [sp, #0x78]
007f3954: bl       #0x7e5524
007f3958: ldr      r1, [sp, #0x2c]
007f395c: ldr      r0, [sp, #0x34]
007f3960: bl       #0x30e3ac
007f3964: ldr      r1, [sp, #0x78]
007f3968: bl       #0x30ed6c
007f396c: ldr      r1, [sp, #0x30]
007f3970: mov      sl, r0
007f3974: ldr      r0, [sp, #0x38]
007f3978: bl       #0x30e3ac
007f397c: ldr      r1, [sp, #0x7c]
007f3980: bl       #0x30ed6c
007f3984: mov      r1, r0
007f3988: mov      r0, sl
007f398c: bl       #0x30eba4
007f3990: mov      r1, #0
007f3994: mov      sl, r0
007f3998: mov      r0, r8
007f399c: bl       #0x30e2f8
007f39a0: cmp      r0, #0
007f39a4: movne    r1, r8
007f39a8: addeq    r1, r8, #0x80000000
007f39ac: ldr      r0, [sp, #0x40]
007f39b0: bl       #0x30ed6c
007f39b4: mov      r1, sl
007f39b8: bl       #0x30eba4
007f39bc: mov      r1, #0
007f39c0: mov      sl, r0
007f39c4: mov      r0, sb
007f39c8: bl       #0x30e2f8
007f39cc: cmp      r0, #0
007f39d0: movne    r1, sb
007f39d4: addeq    r1, sb, #0x80000000
007f39d8: ldr      r0, [sp, #0x3c]
007f39dc: bl       #0x30ed6c
007f39e0: mov      r1, r0
007f39e4: mov      r0, sl
007f39e8: bl       #0x30eba4
007f39ec: mov      r1, #0
007f39f0: mov      sl, r0
007f39f4: bl       #0x30e2f8
007f39f8: cmp      r0, #0
007f39fc: movne    r0, sl
007f3a00: addeq    r0, sl, #0x80000000
007f3a04: mov      r1, #0x34000000
007f3a08: bl       #0x30e70c
007f3a0c: cmp      r0, #0
007f3a10: bne      #0x7f3a80
007f3a14: mov      r1, sl
007f3a18: mov      r0, fp
007f3a1c: bl       #0x30ec94
007f3a20: mov      r1, r0
007f3a24: mov      r0, r4
007f3a28: bl       #0x30eba4
007f3a2c: mov      r1, #0
007f3a30: mov      sl, r0
007f3a34: bl       #0x30e70c
007f3a38: cmp      r0, #0
007f3a3c: bne      #0x7f3a80
007f3a40: mov      r0, sl
007f3a44: mov      r1, #0x3f800000
007f3a48: bl       #0x30e2f8
007f3a4c: cmp      r0, #0
007f3a50: bne      #0x7f3a80
007f3a54: mov      r1, #0x3f800000
007f3a58: add      r1, r1, #0x64
007f3a5c: mov      r0, r4
007f3a60: bl       #0x30ed6c
007f3a64: mov      r1, sl
007f3a68: bl       #0x30e2f8
007f3a6c: cmp      r0, #0
007f3a70: bne      #0x7f3920
007f3a74: add      r5, r5, #1
007f3a78: mov      r4, sl
007f3a7c: b        #0x7f382c
007f3a80: mov      r4, #0x3f800000
007f3a84: b        #0x7f3920
007f3a88: andseq   r1, sl, ip, lsl r3
007f3a8c: andeq    r0, r0, r0, asr #18

# _ZN13b2PairManager15AddBufferedPairEii
007e4190: push     {r4, r5, r6, lr}
007e4194: mov      r4, r0
007e4198: bl       #0x7e3fa8
007e419c: ldrh     r2, [r0, #0xa]
007e41a0: ldr      r3, [pc, #0x74]
007e41a4: tst      r2, #1
007e41a8: add      r3, pc, r3
007e41ac: bne      #0x7e41f0
007e41b0: orr      r2, r2, #1
007e41b4: mov      r1, #0x40000
007e41b8: strh     r2, [r0, #0xa]
007e41bc: add      r1, r1, #0x10
007e41c0: ldr      ip, [r4, r1]
007e41c4: ldrh     r5, [r0, #4]
007e41c8: add      r2, ip, #0xc000
007e41cc: add      r2, r2, #4
007e41d0: lsl      r2, r2, #2
007e41d4: strh     r5, [r4, r2]
007e41d8: ldrh     r5, [r0, #6]
007e41dc: add      r2, r4, r2
007e41e0: add      ip, ip, #1
007e41e4: strh     r5, [r2, #2]
007e41e8: str      ip, [r4, r1]
007e41ec: ldrh     r2, [r0, #0xa]
007e41f0: bic      r2, r2, #2
007e41f4: strh     r2, [r0, #0xa]
007e41f8: ldr      r2, [pc, #0x20]
007e41fc: ldr      r3, [r3, r2]
007e4200: ldrb     r3, [r3]
007e4204: cmp      r3, #0
007e4208: bne      #0x7e4210
007e420c: pop      {r4, r5, r6, pc}
007e4210: mov      r0, r4
007e4214: pop      {r4, r5, r6, lr}
007e4218: b        #0x7e418c
007e421c: andseq   r0, fp, r8, ror #17
007e4220: andeq    r2, r0, r8, ror #26

# _ZN12b2BroadPhase8ValidateEv
007e2ac4: bx       lr

# _ZN7b2World13DrawDebugDataEv
007e6b78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e6b7c: mov      r3, #0x19000
007e6b80: sub      sp, sp, #0xdc
007e6b84: str      r0, [sp, #0x2c]
007e6b88: add      r3, r3, #0x268
007e6b8c: ldr      r0, [r0, r3]
007e6b90: cmp      r0, #0
007e6b94: beq      #0x7e7568
007e6b98: bl       #0x7e8cfc
007e6b9c: tst      r0, #1
007e6ba0: str      r0, [sp, #0x4c]
007e6ba4: beq      #0x7e6c94
007e6ba8: ldr      r0, [sp, #0x2c]
007e6bac: mov      r3, #0x19000
007e6bb0: add      r3, r3, #0x230
007e6bb4: ldr      r5, [r0, r3]
007e6bb8: ldr      r1, [sp, #0x4c]
007e6bbc: cmp      r5, #0
007e6bc0: ubfx     sl, r1, #2, #1
007e6bc4: beq      #0x7e6c94
007e6bc8: add      sb, sp, #0xcc
007e6bcc: movw     r6, #0x6666
007e6bd0: add      r2, sp, #0xc0
007e6bd4: str      sb, [sp, #0x14]
007e6bd8: mov      r7, #0x3f000000
007e6bdc: movt     r6, #0x3f66
007e6be0: add      fp, sp, #0xb4
007e6be4: str      r2, [sp, #0x18]
007e6be8: mov      r8, r0
007e6bec: mov      sb, sl
007e6bf0: ldr      r4, [r5, #0x64]
007e6bf4: add      sl, r5, #4
007e6bf8: cmp      r4, #0
007e6bfc: bne      #0x7e6c2c
007e6c00: b        #0x7e6c88
007e6c04: mov      r2, sl
007e6c08: ldr      r3, [sp, #0x14]
007e6c0c: str      r7, [sp, #0xcc]
007e6c10: str      r6, [sp, #0xd0]
007e6c14: str      r7, [sp, #0xd4]
007e6c18: str      sb, [sp]
007e6c1c: bl       #0x7e6624
007e6c20: ldr      r4, [r4, #8]
007e6c24: cmp      r4, #0
007e6c28: beq      #0x7e6c88
007e6c2c: ldrsh    r3, [r5, #2]
007e6c30: mov      r0, r8
007e6c34: mov      r1, r4
007e6c38: cmp      r3, #0
007e6c3c: beq      #0x7e6c04
007e6c40: ldrh     r3, [r5]
007e6c44: mov      r1, r4
007e6c48: mov      r0, r8
007e6c4c: tst      r3, #8
007e6c50: mov      r2, sl
007e6c54: mov      r3, fp
007e6c58: ldrne    r3, [sp, #0x18]
007e6c5c: strne    r7, [sp, #0xc0]
007e6c60: strne    r7, [sp, #0xc4]
007e6c64: strne    r6, [sp, #0xc8]
007e6c68: streq    r6, [sp, #0xb4]
007e6c6c: streq    r6, [sp, #0xb8]
007e6c70: streq    r6, [sp, #0xbc]
007e6c74: str      sb, [sp]
007e6c78: bl       #0x7e6624
007e6c7c: ldr      r4, [r4, #8]
007e6c80: cmp      r4, #0
007e6c84: bne      #0x7e6c2c
007e6c88: ldr      r5, [r5, #0x60]
007e6c8c: cmp      r5, #0
007e6c90: bne      #0x7e6bf0
007e6c94: ldr      r3, [sp, #0x4c]
007e6c98: tst      r3, #2
007e6c9c: beq      #0x7e6ce0
007e6ca0: ldr      r0, [sp, #0x2c]
007e6ca4: mov      r3, #0x19000
007e6ca8: add      r3, r3, #0x234
007e6cac: ldr      r4, [r0, r3]
007e6cb0: cmp      r4, #0
007e6cb4: beq      #0x7e6ce0
007e6cb8: mov      r5, r0
007e6cbc: ldr      r3, [r4, #4]
007e6cc0: mov      r1, r4
007e6cc4: mov      r0, r5
007e6cc8: cmp      r3, #5
007e6ccc: beq      #0x7e6cd4
007e6cd0: bl       #0x7e69b0
007e6cd4: ldr      r4, [r4, #0xc]
007e6cd8: cmp      r4, #0
007e6cdc: bne      #0x7e6cbc
007e6ce0: ldr      r1, [sp, #0x4c]
007e6ce4: tst      r1, #0x20
007e6ce8: beq      #0x7e70a4
007e6cec: ldr      r2, [sp, #0x2c]
007e6cf0: mov      r3, #0x19000
007e6cf4: add      r3, r3, #0x1d8
007e6cf8: ldr      r4, [r2, r3]
007e6cfc: mov      r3, #0x5d000
007e6d00: add      r3, r3, #0x2c
007e6d04: ldr      r1, [r4, r3]
007e6d08: mov      r0, #0x3f800000
007e6d0c: bl       #0x30ec94
007e6d10: mov      r3, #0x5d000
007e6d14: add      r3, r3, #0x30
007e6d18: ldr      r1, [r4, r3]
007e6d1c: mov      r6, r0
007e6d20: mov      r0, #0x3f800000
007e6d24: bl       #0x30ec94
007e6d28: movw     r3, #0x6666
007e6d2c: add      r1, r4, #0x40000
007e6d30: add      r2, r4, #0x48000
007e6d34: movt     r3, #0x3f66
007e6d38: add      r1, r1, #0x14
007e6d3c: add      r2, r2, #0x14
007e6d40: mov      r7, r0
007e6d44: str      r3, [sp, #0x94]
007e6d48: movw     r0, #0x999a
007e6d4c: str      r3, [sp, #0x90]
007e6d50: str      r1, [sp, #0x44]
007e6d54: str      r2, [sp, #0x48]
007e6d58: mov      r1, #0x5d000
007e6d5c: mov      r2, #0x5d000
007e6d60: mov      r3, #0x19000
007e6d64: movt     r0, #0x3e99
007e6d68: add      r1, r1, #0x1c
007e6d6c: add      r2, r2, #0x20
007e6d70: add      r3, r3, #0x268
007e6d74: str      r0, [sp, #0x98]
007e6d78: str      r1, [sp, #0x38]
007e6d7c: str      r2, [sp, #0x3c]
007e6d80: str      r3, [sp, #0x40]
007e6d84: ldr      r0, [sp, #0x44]
007e6d88: movw     r2, #0xffff
007e6d8c: ldrh     r3, [r0]
007e6d90: cmp      r3, r2
007e6d94: beq      #0x7e708c
007e6d98: add      r1, sp, #0x50
007e6d9c: add      r2, sp, #0x70
007e6da0: add      r0, sp, #0x90
007e6da4: str      r1, [sp, #0x28]
007e6da8: str      r2, [sp, #0x30]
007e6dac: str      r0, [sp, #0x34]
007e6db0: mov      r5, #6
007e6db4: mov      r1, #0xc
007e6db8: mla      r3, r1, r3, r4
007e6dbc: ldr      r2, [sp, #0x38]
007e6dc0: str      r3, [sp, #0x18]
007e6dc4: add      r3, r3, #8
007e6dc8: ldrh     sb, [r3, #4]
007e6dcc: ldrh     r3, [r3, #6]
007e6dd0: ldr      sl, [r4, r2]
007e6dd4: add      fp, sb, #0x4800
007e6dd8: add      fp, fp, #1
007e6ddc: str      r3, [sp, #0x14]
007e6de0: add      fp, r4, fp, lsl #4
007e6de4: ldrh     r3, [fp, #4]
007e6de8: add      sb, r4, sb, lsl #4
007e6dec: add      sb, sb, #0x48000
007e6df0: mla      r3, r5, r3, r4
007e6df4: add      sb, sb, #0x12
007e6df8: add      r3, r3, #0x50000
007e6dfc: add      r3, r3, #0x10
007e6e00: ldrh     r0, [r3, #6]
007e6e04: bl       #0x30e964
007e6e08: mov      r1, r0
007e6e0c: mov      r0, r6
007e6e10: bl       #0x30ed6c
007e6e14: mov      r1, r0
007e6e18: mov      r0, sl
007e6e1c: bl       #0x30eba4
007e6e20: str      r0, [sp, #0x1c]
007e6e24: ldrh     r3, [sb, #4]
007e6e28: ldr      r0, [sp, #0x3c]
007e6e2c: mla      r3, r5, r3, r4
007e6e30: ldr      r8, [r4, r0]
007e6e34: add      r3, r3, #0x56000
007e6e38: add      r3, r3, #0x10
007e6e3c: ldrh     r0, [r3, #6]
007e6e40: bl       #0x30e964
007e6e44: mov      r1, r0
007e6e48: mov      r0, r7
007e6e4c: bl       #0x30ed6c
007e6e50: mov      r1, r0
007e6e54: mov      r0, r8
007e6e58: bl       #0x30eba4
007e6e5c: str      r0, [sp, #0x20]
007e6e60: ldrh     r3, [fp, #8]
007e6e64: mla      r3, r5, r3, r4
007e6e68: add      r3, r3, #0x50000
007e6e6c: add      r3, r3, #0x10
007e6e70: ldrh     r0, [r3, #6]
007e6e74: bl       #0x30e964
007e6e78: mov      r1, r0
007e6e7c: mov      r0, r6
007e6e80: bl       #0x30ed6c
007e6e84: mov      r1, r0
007e6e88: mov      r0, sl
007e6e8c: bl       #0x30eba4
007e6e90: ldrh     r2, [sb, #8]
007e6e94: mov      r3, r0
007e6e98: mla      r2, r5, r2, r4
007e6e9c: add      r2, r2, #0x56000
007e6ea0: add      r2, r2, #0x10
007e6ea4: ldrh     r0, [r2, #6]
007e6ea8: str      r3, [sp, #8]
007e6eac: bl       #0x30e964
007e6eb0: mov      r1, r0
007e6eb4: mov      r0, r7
007e6eb8: bl       #0x30ed6c
007e6ebc: mov      r1, r0
007e6ec0: mov      r0, r8
007e6ec4: bl       #0x30eba4
007e6ec8: ldr      r1, [sp, #0x14]
007e6ecc: mov      r2, r0
007e6ed0: add      sb, r1, #0x4800
007e6ed4: add      sb, sb, #1
007e6ed8: add      sb, r4, sb, lsl #4
007e6edc: ldrh     r1, [sb, #4]
007e6ee0: mla      r1, r5, r1, r4
007e6ee4: add      r1, r1, #0x50000
007e6ee8: add      r1, r1, #0x10
007e6eec: ldrh     r0, [r1, #6]
007e6ef0: str      r2, [sp, #0xc]
007e6ef4: bl       #0x30e964
007e6ef8: mov      r1, r0
007e6efc: mov      r0, r6
007e6f00: bl       #0x30ed6c
007e6f04: mov      r1, r0
007e6f08: mov      r0, sl
007e6f0c: bl       #0x30eba4
007e6f10: str      r0, [sp, #0x24]
007e6f14: ldr      r0, [sp, #0x14]
007e6f18: add      fp, r4, r0, lsl #4
007e6f1c: add      fp, fp, #0x48000
007e6f20: add      fp, fp, #0x12
007e6f24: ldrh     r1, [fp, #4]
007e6f28: mla      r1, r5, r1, r4
007e6f2c: add      r1, r1, #0x56000
007e6f30: add      r1, r1, #0x10
007e6f34: ldrh     r0, [r1, #6]
007e6f38: bl       #0x30e964
007e6f3c: mov      r1, r0
007e6f40: mov      r0, r7
007e6f44: bl       #0x30ed6c
007e6f48: mov      r1, r0
007e6f4c: mov      r0, r8
007e6f50: bl       #0x30eba4
007e6f54: ldrh     r1, [sb, #8]
007e6f58: mov      ip, r0
007e6f5c: mla      r1, r5, r1, r4
007e6f60: add      r1, r1, #0x50000
007e6f64: add      r1, r1, #0x10
007e6f68: ldrh     r0, [r1, #6]
007e6f6c: str      ip, [sp, #0x10]
007e6f70: bl       #0x30e964
007e6f74: mov      r1, r0
007e6f78: mov      r0, r6
007e6f7c: bl       #0x30ed6c
007e6f80: mov      r1, r0
007e6f84: mov      r0, sl
007e6f88: bl       #0x30eba4
007e6f8c: ldrh     r1, [fp, #8]
007e6f90: mov      sl, r0
007e6f94: mla      r1, r5, r1, r4
007e6f98: add      r1, r1, #0x56000
007e6f9c: add      r1, r1, #0x10
007e6fa0: ldrh     r0, [r1, #6]
007e6fa4: bl       #0x30e964
007e6fa8: mov      r1, r0
007e6fac: mov      r0, r7
007e6fb0: bl       #0x30ed6c
007e6fb4: mov      r1, r0
007e6fb8: mov      r0, r8
007e6fbc: bl       #0x30eba4
007e6fc0: ldr      r3, [sp, #8]
007e6fc4: mov      r8, r0
007e6fc8: ldr      r0, [sp, #0x1c]
007e6fcc: mov      r1, r3
007e6fd0: bl       #0x30eba4
007e6fd4: ldr      r2, [sp, #0xc]
007e6fd8: mov      fp, r0
007e6fdc: ldr      r0, [sp, #0x20]
007e6fe0: mov      r1, r2
007e6fe4: bl       #0x30eba4
007e6fe8: mov      r1, #0x3f000000
007e6fec: mov      sb, r0
007e6ff0: mov      r0, fp
007e6ff4: bl       #0x30ed6c
007e6ff8: mov      r1, #0x3f000000
007e6ffc: str      r0, [sp, #0x50]
007e7000: mov      r0, sb
007e7004: bl       #0x30ed6c
007e7008: mov      r1, sl
007e700c: str      r0, [sp, #0x54]
007e7010: ldr      r0, [sp, #0x24]
007e7014: bl       #0x30eba4
007e7018: ldr      ip, [sp, #0x10]
007e701c: mov      r1, r8
007e7020: mov      sl, r0
007e7024: mov      r0, ip
007e7028: bl       #0x30eba4
007e702c: mov      r1, #0x3f000000
007e7030: mov      r8, r0
007e7034: mov      r0, sl
007e7038: bl       #0x30ed6c
007e703c: mov      r1, #0x3f000000
007e7040: str      r0, [sp, #0x70]
007e7044: mov      r0, r8
007e7048: bl       #0x30ed6c
007e704c: ldr      r2, [sp, #0x2c]
007e7050: ldr      r1, [sp, #0x40]
007e7054: ldr      r3, [r2, r1]
007e7058: str      r0, [sp, #0x74]
007e705c: ldr      r1, [sp, #0x28]
007e7060: mov      r0, r3
007e7064: ldr      ip, [r3]
007e7068: ldr      r2, [sp, #0x30]
007e706c: ldr      r3, [sp, #0x34]
007e7070: mov      lr, pc
007e7074: ldr      pc, [ip, #0x18]
007e7078: ldr      r0, [sp, #0x18]
007e707c: movw     r1, #0xffff
007e7080: ldrh     r3, [r0, #0x10]
007e7084: cmp      r3, r1
007e7088: bne      #0x7e6db4
007e708c: ldr      r2, [sp, #0x44]
007e7090: ldr      r3, [sp, #0x48]
007e7094: add      r2, r2, #2
007e7098: cmp      r2, r3
007e709c: str      r2, [sp, #0x44]
007e70a0: bne      #0x7e6d84
007e70a4: ldr      r0, [sp, #0x4c]
007e70a8: tst      r0, #8
007e70ac: beq      #0x7e7308
007e70b0: ldr      r1, [sp, #0x2c]
007e70b4: mov      r3, #0x19000
007e70b8: add      r3, r3, #0x1d8
007e70bc: ldr      r5, [r1, r3]
007e70c0: mov      lr, #0x5d000
007e70c4: mov      r2, lr
007e70c8: add      r2, r2, #0x28
007e70cc: ldr      r2, [r5, r2]
007e70d0: mov      ip, lr
007e70d4: mov      r1, #0x5d000
007e70d8: add      ip, ip, #0x1c
007e70dc: mov      r3, lr
007e70e0: add      r1, r1, #0x2c
007e70e4: add      lr, lr, #0x20
007e70e8: ldr      r1, [r5, r1]
007e70ec: ldr      fp, [r5, lr]
007e70f0: ldr      r7, [r5, ip]
007e70f4: add      r3, r3, #0x24
007e70f8: str      r2, [sp, #0x34]
007e70fc: ldr      r3, [r5, r3]
007e7100: mov      r0, #0x3f800000
007e7104: add      r4, r5, #0x48000
007e7108: str      r3, [sp, #0x30]
007e710c: bl       #0x30ec94
007e7110: mov      r3, #0x5d000
007e7114: str      r0, [sp, #0x18]
007e7118: add      r3, r3, #0x30
007e711c: ldr      r1, [r5, r3]
007e7120: mov      r0, #0x3f800000
007e7124: bl       #0x30ec94
007e7128: add      r2, r5, #0x50000
007e712c: movw     r1, #0x999a
007e7130: movt     r1, #0x3e99
007e7134: add      r2, r2, #0x1c
007e7138: str      r0, [sp, #0x14]
007e713c: movw     r3, #0x6666
007e7140: mov      r0, #0x19000
007e7144: movt     r3, #0x3f66
007e7148: str      r1, [sp, #0x94]
007e714c: str      r2, [sp, #0x1c]
007e7150: add      r0, r0, #0x268
007e7154: add      r1, sp, #0x70
007e7158: add      r2, sp, #0x90
007e715c: str      r3, [sp, #0x98]
007e7160: str      r3, [sp, #0x90]
007e7164: add      r4, r4, #0x1c
007e7168: str      r0, [sp, #0x20]
007e716c: mov      r6, #6
007e7170: str      r1, [sp, #0x24]
007e7174: str      r2, [sp, #0x28]
007e7178: ldrh     r3, [r4]
007e717c: movw     r0, #0xffff
007e7180: cmp      r3, r0
007e7184: beq      #0x7e728c
007e7188: ldrh     r3, [r4, #-8]
007e718c: mla      r3, r6, r3, r5
007e7190: add      r3, r3, #0x50000
007e7194: add      r3, r3, #0x10
007e7198: ldrh     r0, [r3, #6]
007e719c: bl       #0x30e964
007e71a0: mov      r1, r0
007e71a4: ldr      r0, [sp, #0x18]
007e71a8: bl       #0x30ed6c
007e71ac: mov      r1, r7
007e71b0: bl       #0x30eba4
007e71b4: ldrh     r3, [r4, #-6]
007e71b8: mov      sb, r0
007e71bc: mla      r3, r6, r3, r5
007e71c0: add      r3, r3, #0x56000
007e71c4: add      r3, r3, #0x10
007e71c8: ldrh     r0, [r3, #6]
007e71cc: bl       #0x30e964
007e71d0: mov      r1, r0
007e71d4: ldr      r0, [sp, #0x14]
007e71d8: bl       #0x30ed6c
007e71dc: mov      r1, fp
007e71e0: bl       #0x30eba4
007e71e4: ldrh     r3, [r4, #-4]
007e71e8: mov      sl, r0
007e71ec: mla      r3, r6, r3, r5
007e71f0: add      r3, r3, #0x50000
007e71f4: add      r3, r3, #0x10
007e71f8: ldrh     r0, [r3, #6]
007e71fc: bl       #0x30e964
007e7200: mov      r1, r0
007e7204: ldr      r0, [sp, #0x18]
007e7208: bl       #0x30ed6c
007e720c: mov      r1, r7
007e7210: bl       #0x30eba4
007e7214: ldrh     r3, [r4, #-2]
007e7218: mov      r8, r0
007e721c: mla      r3, r6, r3, r5
007e7220: add      r3, r3, #0x56000
007e7224: add      r3, r3, #0x10
007e7228: ldrh     r0, [r3, #6]
007e722c: bl       #0x30e964
007e7230: mov      r1, r0
007e7234: ldr      r0, [sp, #0x14]
007e7238: bl       #0x30ed6c
007e723c: mov      r1, fp
007e7240: bl       #0x30eba4
007e7244: ldr      r2, [sp, #0x2c]
007e7248: ldr      r1, [sp, #0x20]
007e724c: ldr      r3, [r2, r1]
007e7250: str      r0, [sp, #0x8c]
007e7254: str      sl, [sp, #0x7c]
007e7258: str      r8, [sp, #0x80]
007e725c: str      sb, [sp, #0x88]
007e7260: str      sb, [sp, #0x70]
007e7264: str      sl, [sp, #0x74]
007e7268: str      r8, [sp, #0x78]
007e726c: str      r0, [sp, #0x84]
007e7270: ldr      ip, [r3]
007e7274: mov      r0, r3
007e7278: ldr      r1, [sp, #0x24]
007e727c: mov      r2, #4
007e7280: ldr      r3, [sp, #0x28]
007e7284: mov      lr, pc
007e7288: ldr      pc, [ip, #8]
007e728c: ldr      r3, [sp, #0x1c]
007e7290: add      r4, r4, #0x10
007e7294: cmp      r4, r3
007e7298: bne      #0x7e7178
007e729c: ldr      r1, [sp, #0x2c]
007e72a0: mov      r3, #0x19000
007e72a4: add      r3, r3, #0x268
007e72a8: ldr      r0, [r1, r3]
007e72ac: ldr      r2, [sp, #0x30]
007e72b0: ldr      r3, [sp, #0x34]
007e72b4: str      fp, [sp, #0x5c]
007e72b8: str      r2, [sp, #0x60]
007e72bc: str      r3, [sp, #0x6c]
007e72c0: str      r3, [sp, #0x64]
007e72c4: str      r7, [sp, #0x68]
007e72c8: str      r7, [sp, #0x50]
007e72cc: str      fp, [sp, #0x54]
007e72d0: str      r2, [sp, #0x58]
007e72d4: ldr      r2, [r0]
007e72d8: movw     r3, #0x6666
007e72dc: movt     r3, #0x3f66
007e72e0: ldr      ip, [r2, #8]
007e72e4: movw     r2, #0x999a
007e72e8: movt     r2, #0x3e99
007e72ec: str      r3, [sp, #0xb0]
007e72f0: str      r2, [sp, #0xa8]
007e72f4: str      r3, [sp, #0xac]
007e72f8: add      r1, sp, #0x50
007e72fc: mov      r2, #4
007e7300: add      r3, sp, #0xa8
007e7304: blx      ip
007e7308: ldr      r0, [sp, #0x4c]
007e730c: tst      r0, #0x10
007e7310: beq      #0x7e74e0
007e7314: ldr      r1, [sp, #0x2c]
007e7318: mov      r3, #0x19000
007e731c: add      r3, r3, #0x230
007e7320: ldr      r5, [r1, r3]
007e7324: movw     r2, #0x999a
007e7328: mov      r3, #0x3f000000
007e732c: movt     r2, #0x3e99
007e7330: cmp      r5, #0
007e7334: str      r3, [sp, #0x98]
007e7338: str      r2, [sp, #0x94]
007e733c: str      r3, [sp, #0x90]
007e7340: beq      #0x7e74e0
007e7344: mov      r2, #0x19000
007e7348: add      r2, r2, #0x268
007e734c: add      r3, sp, #0x90
007e7350: str      r2, [sp, #0x14]
007e7354: add      r6, sp, #0x50
007e7358: str      r3, [sp, #0x18]
007e735c: ldr      r4, [r5, #0x64]
007e7360: cmp      r4, #0
007e7364: bne      #0x7e7378
007e7368: b        #0x7e74d4
007e736c: ldr      r4, [r4, #8]
007e7370: cmp      r4, #0
007e7374: beq      #0x7e74d4
007e7378: ldr      r3, [r4, #4]
007e737c: cmp      r3, #1
007e7380: bne      #0x7e736c
007e7384: ldr      r3, [r4, #0x54]
007e7388: ldr      r2, [r4, #0x50]
007e738c: mov      r7, #0
007e7390: add      sl, r3, #0x80000000
007e7394: add      r8, r2, #0x80000000
007e7398: str      r2, [sp, #0x60]
007e739c: str      r3, [sp, #0x6c]
007e73a0: str      r8, [sp, #0x50]
007e73a4: str      sl, [sp, #0x54]
007e73a8: str      r2, [sp, #0x58]
007e73ac: str      sl, [sp, #0x5c]
007e73b0: str      r3, [sp, #0x64]
007e73b4: str      r8, [sp, #0x68]
007e73b8: ldr      r1, [r4, #0x38]
007e73bc: mov      r0, r8
007e73c0: bl       #0x30ed6c
007e73c4: ldr      r1, [r4, #0x40]
007e73c8: mov      sb, r0
007e73cc: mov      r0, sl
007e73d0: bl       #0x30ed6c
007e73d4: mov      r1, r0
007e73d8: mov      r0, sb
007e73dc: bl       #0x30eba4
007e73e0: ldr      r1, [r4, #0x3c]
007e73e4: mov      sb, r0
007e73e8: mov      r0, r8
007e73ec: bl       #0x30ed6c
007e73f0: ldr      r1, [r4, #0x44]
007e73f4: mov      r8, r0
007e73f8: mov      r0, sl
007e73fc: bl       #0x30ed6c
007e7400: mov      r1, r0
007e7404: mov      r0, r8
007e7408: bl       #0x30eba4
007e740c: ldr      r1, [r4, #0x48]
007e7410: mov      sl, r0
007e7414: mov      r0, sb
007e7418: bl       #0x30eba4
007e741c: ldr      r1, [r4, #0x4c]
007e7420: mov      r8, r0
007e7424: mov      r0, sl
007e7428: bl       #0x30eba4
007e742c: add      sl, r6, r7
007e7430: str      r0, [sl, #4]
007e7434: str      r8, [r6, r7]
007e7438: mov      sb, r0
007e743c: ldr      r1, [r5, #0xc]
007e7440: mov      r0, r8
007e7444: bl       #0x30ed6c
007e7448: ldr      r1, [r5, #0x14]
007e744c: mov      fp, r0
007e7450: mov      r0, sb
007e7454: bl       #0x30ed6c
007e7458: mov      r1, r0
007e745c: mov      r0, fp
007e7460: bl       #0x30eba4
007e7464: ldr      r1, [r5, #0x10]
007e7468: mov      fp, r0
007e746c: mov      r0, r8
007e7470: bl       #0x30ed6c
007e7474: ldr      r1, [r5, #0x18]
007e7478: mov      r8, r0
007e747c: mov      r0, sb
007e7480: bl       #0x30ed6c
007e7484: mov      r1, r0
007e7488: mov      r0, r8
007e748c: bl       #0x30eba4
007e7490: ldr      r1, [r5, #4]
007e7494: mov      r8, r0
007e7498: mov      r0, fp
007e749c: bl       #0x30eba4
007e74a0: ldr      r1, [r5, #8]
007e74a4: mov      sb, r0
007e74a8: mov      r0, r8
007e74ac: bl       #0x30eba4
007e74b0: str      r0, [sl, #4]
007e74b4: str      sb, [r6, r7]
007e74b8: add      r7, r7, #8
007e74bc: cmp      r7, #0x20
007e74c0: beq      #0x7e7570
007e74c4: mov      r3, r6
007e74c8: ldr      r8, [r3, r7]!
007e74cc: ldr      sl, [r3, #4]
007e74d0: b        #0x7e73b8
007e74d4: ldr      r5, [r5, #0x60]
007e74d8: cmp      r5, #0
007e74dc: bne      #0x7e735c
007e74e0: ldr      r2, [sp, #0x4c]
007e74e4: tst      r2, #0x40
007e74e8: beq      #0x7e7568
007e74ec: ldr      r0, [sp, #0x2c]
007e74f0: mov      r3, #0x19000
007e74f4: add      r3, r3, #0x230
007e74f8: ldr      r5, [r0, r3]
007e74fc: cmp      r5, #0
007e7500: beq      #0x7e7568
007e7504: add      r4, sp, #0x90
007e7508: mov      r7, #0x19000
007e750c: add      r7, r7, #0x268
007e7510: mov      r6, r4
007e7514: mov      r8, r0
007e7518: add      ip, r5, #4
007e751c: ldm      ip!, {r0, r1, r2, r3}
007e7520: stm      r4!, {r0, r1, r2, r3}
007e7524: ldm      ip, {r0, r1}
007e7528: mov      r3, r4
007e752c: stm      r3, {r0, r1}
007e7530: ldr      r2, [r5, #0x2c]
007e7534: ldr      r3, [r8, r7]
007e7538: mov      r1, r6
007e753c: str      r2, [sp, #0x90]
007e7540: ldr      r2, [r5, #0x30]
007e7544: mov      r0, r3
007e7548: mov      r4, r6
007e754c: str      r2, [sp, #0x94]
007e7550: ldr      r3, [r3]
007e7554: mov      lr, pc
007e7558: ldr      pc, [r3, #0x1c]
007e755c: ldr      r5, [r5, #0x60]
007e7560: cmp      r5, #0
007e7564: bne      #0x7e7518
007e7568: add      sp, sp, #0xdc
007e756c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e7570: ldr      r1, [sp, #0x2c]
007e7574: ldr      r0, [sp, #0x14]
007e7578: mov      r2, #4
007e757c: ldr      r3, [r1, r0]
007e7580: mov      r1, r6
007e7584: mov      r0, r3
007e7588: ldr      ip, [r3]
007e758c: ldr      r3, [sp, #0x18]
007e7590: mov      lr, pc
007e7594: ldr      pc, [ip, #8]
007e7598: b        #0x7e736c

# _ZN7b2World9DrawShapeEP7b2ShapeRK7b2XFormRK7b2Colorb
007e6624: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e6628: movw     r4, #0x6666
007e662c: sub      sp, sp, #0x7c
007e6630: movt     r4, #0x3f66
007e6634: mov      r6, r1
007e6638: ldr      r1, [r1, #4]
007e663c: str      r4, [sp, #0x64]
007e6640: mov      r4, r2
007e6644: ldrb     r2, [sp, #0xa0]
007e6648: movw     ip, #0x999a
007e664c: movt     ip, #0x3f19
007e6650: cmp      r1, #0
007e6654: str      ip, [sp, #0x6c]
007e6658: str      r0, [sp, #0xc]
007e665c: str      r3, [sp, #0x14]
007e6660: str      ip, [sp, #0x68]
007e6664: str      r2, [sp, #0x18]
007e6668: bne      #0x7e6748
007e666c: ldr      r7, [r6, #0x30]
007e6670: ldr      r1, [r4, #8]
007e6674: ldr      r5, [r6, #0x34]
007e6678: mov      r0, r7
007e667c: bl       #0x30ed6c
007e6680: ldr      r1, [r4, #0x10]
007e6684: mov      r8, r0
007e6688: mov      r0, r5
007e668c: bl       #0x30ed6c
007e6690: mov      r1, r0
007e6694: mov      r0, r8
007e6698: bl       #0x30eba4
007e669c: ldr      r1, [r4, #0xc]
007e66a0: mov      r8, r0
007e66a4: mov      r0, r7
007e66a8: bl       #0x30ed6c
007e66ac: ldr      r1, [r4, #0x14]
007e66b0: mov      r7, r0
007e66b4: mov      r0, r5
007e66b8: bl       #0x30ed6c
007e66bc: mov      r1, r0
007e66c0: mov      r0, r7
007e66c4: bl       #0x30eba4
007e66c8: ldr      r1, [r4, #4]
007e66cc: bl       #0x30eba4
007e66d0: ldr      r1, [r4]
007e66d4: mov      r7, r0
007e66d8: mov      r0, r8
007e66dc: bl       #0x30eba4
007e66e0: ldr      r1, [r4, #8]
007e66e4: ldr      ip, [sp, #0xc]
007e66e8: mov      r5, #0x19000
007e66ec: ldr      r2, [r4, #0xc]
007e66f0: add      r5, r5, #0x268
007e66f4: ldr      r3, [ip, r5]
007e66f8: ldr      r4, [r6, #0x38]
007e66fc: str      r1, [sp, #0x24]
007e6700: ldr      r1, [sp, #0x14]
007e6704: str      r0, [sp, #0x70]
007e6708: str      r2, [sp, #0x28]
007e670c: str      r7, [sp, #0x74]
007e6710: add      r6, sp, #0x70
007e6714: ldr      ip, [r3]
007e6718: mov      r0, r3
007e671c: str      r1, [sp]
007e6720: mov      r2, r4
007e6724: mov      r1, r6
007e6728: add      r3, sp, #0x24
007e672c: mov      lr, pc
007e6730: ldr      pc, [ip, #0x14]
007e6734: ldr      r2, [sp, #0x18]
007e6738: cmp      r2, #0
007e673c: bne      #0x7e6930
007e6740: add      sp, sp, #0x7c
007e6744: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e6748: cmp      r1, #1
007e674c: bne      #0x7e6740
007e6750: ldr      ip, [r6, #0x118]
007e6754: cmp      ip, #0
007e6758: str      ip, [sp, #0x10]
007e675c: addle    r5, sp, #0x24
007e6760: ble      #0x7e681c
007e6764: ldr      r1, [sp, #0x10]
007e6768: mov      r8, r6
007e676c: mov      r7, #0
007e6770: lsl      r3, r1, #3
007e6774: add      r5, sp, #0x24
007e6778: mov      fp, r3
007e677c: str      r6, [sp, #0x1c]
007e6780: ldr      sl, [r8, #0x58]
007e6784: ldr      r1, [r4, #8]
007e6788: ldr      r6, [r8, #0x5c]
007e678c: mov      r0, sl
007e6790: bl       #0x30ed6c
007e6794: ldr      r1, [r4, #0x10]
007e6798: mov      sb, r0
007e679c: mov      r0, r6
007e67a0: bl       #0x30ed6c
007e67a4: mov      r1, r0
007e67a8: mov      r0, sb
007e67ac: bl       #0x30eba4
007e67b0: ldr      r1, [r4, #0xc]
007e67b4: mov      sb, r0
007e67b8: mov      r0, sl
007e67bc: bl       #0x30ed6c
007e67c0: ldr      r1, [r4, #0x14]
007e67c4: mov      sl, r0
007e67c8: mov      r0, r6
007e67cc: bl       #0x30ed6c
007e67d0: mov      r1, r0
007e67d4: mov      r0, sl
007e67d8: bl       #0x30eba4
007e67dc: ldr      r1, [r4]
007e67e0: mov      r6, r0
007e67e4: mov      r0, sb
007e67e8: bl       #0x30eba4
007e67ec: ldr      r1, [r4, #4]
007e67f0: mov      sl, r0
007e67f4: mov      r0, r6
007e67f8: bl       #0x30eba4
007e67fc: add      r3, r5, r7
007e6800: str      r0, [r3, #4]
007e6804: str      sl, [r5, r7]
007e6808: add      r7, r7, #8
007e680c: cmp      r7, fp
007e6810: add      r8, r8, #8
007e6814: bne      #0x7e6780
007e6818: ldr      r6, [sp, #0x1c]
007e681c: ldr      ip, [sp, #0xc]
007e6820: mov      r3, #0x19000
007e6824: add      r3, r3, #0x268
007e6828: ldr      r2, [ip, r3]
007e682c: mov      r1, r5
007e6830: ldr      r3, [sp, #0x14]
007e6834: mov      r0, r2
007e6838: ldr      ip, [r2]
007e683c: ldr      r2, [sp, #0x10]
007e6840: mov      lr, pc
007e6844: ldr      pc, [ip, #0xc]
007e6848: ldr      r1, [sp, #0x18]
007e684c: cmp      r1, #0
007e6850: beq      #0x7e6740
007e6854: ldr      r2, [sp, #0x10]
007e6858: cmp      r2, #0
007e685c: ble      #0x7e6900
007e6860: lsl      fp, r2, #3
007e6864: mov      r7, #0
007e6868: ldr      sl, [r6, #0xd8]
007e686c: ldr      r1, [r4, #8]
007e6870: ldr      r8, [r6, #0xdc]
007e6874: mov      r0, sl
007e6878: bl       #0x30ed6c
007e687c: ldr      r1, [r4, #0x10]
007e6880: mov      sb, r0
007e6884: mov      r0, r8
007e6888: bl       #0x30ed6c
007e688c: mov      r1, r0
007e6890: mov      r0, sb
007e6894: bl       #0x30eba4
007e6898: ldr      r1, [r4, #0xc]
007e689c: mov      sb, r0
007e68a0: mov      r0, sl
007e68a4: bl       #0x30ed6c
007e68a8: ldr      r1, [r4, #0x14]
007e68ac: mov      sl, r0
007e68b0: mov      r0, r8
007e68b4: bl       #0x30ed6c
007e68b8: mov      r1, r0
007e68bc: mov      r0, sl
007e68c0: bl       #0x30eba4
007e68c4: ldr      r1, [r4]
007e68c8: mov      r8, r0
007e68cc: mov      r0, sb
007e68d0: bl       #0x30eba4
007e68d4: ldr      r1, [r4, #4]
007e68d8: mov      sl, r0
007e68dc: mov      r0, r8
007e68e0: bl       #0x30eba4
007e68e4: add      r3, r5, r7
007e68e8: str      r0, [r3, #4]
007e68ec: str      sl, [r5, r7]
007e68f0: add      r7, r7, #8
007e68f4: cmp      r7, fp
007e68f8: add      r6, r6, #8
007e68fc: bne      #0x7e6868
007e6900: ldr      ip, [sp, #0xc]
007e6904: mov      r3, #0x19000
007e6908: add      r3, r3, #0x268
007e690c: ldr      r3, [ip, r3]
007e6910: mov      r1, r5
007e6914: ldr      r2, [sp, #0x10]
007e6918: mov      r0, r3
007e691c: ldr      ip, [r3]
007e6920: add      r3, sp, #0x64
007e6924: mov      lr, pc
007e6928: ldr      pc, [ip, #8]
007e692c: b        #0x7e6740
007e6930: ldr      r3, [sp, #0xc]
007e6934: movw     r1, #0xd70a
007e6938: mov      r0, r4
007e693c: movt     r1, #0x3d23
007e6940: ldr      r5, [r3, r5]
007e6944: bl       #0x30e3ac
007e6948: mov      r1, r6
007e694c: mov      r2, r0
007e6950: ldr      ip, [r5]
007e6954: mov      r0, r5
007e6958: add      r3, sp, #0x64
007e695c: mov      lr, pc
007e6960: ldr      pc, [ip, #0x10]
007e6964: b        #0x7e6740

# _ZNK13b2CircleShape11TestSegmentERK7b2XFormPfP6b2Vec2RK9b2Segmentf
007e969c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e96a0: ldr      r7, [r0, #0x30]
007e96a4: sub      sp, sp, #0xc
007e96a8: mov      r4, r1
007e96ac: ldr      r6, [r0, #0x34]
007e96b0: ldr      r1, [r1, #8]
007e96b4: mov      r5, r0
007e96b8: mov      r0, r7
007e96bc: str      r2, [sp, #4]
007e96c0: str      r3, [sp]
007e96c4: bl       #0x30ed6c
007e96c8: ldr      r1, [r4, #0x10]
007e96cc: mov      r8, r0
007e96d0: mov      r0, r6
007e96d4: bl       #0x30ed6c
007e96d8: mov      r1, r0
007e96dc: mov      r0, r8
007e96e0: bl       #0x30eba4
007e96e4: ldr      r1, [r4, #0xc]
007e96e8: mov      sl, r0
007e96ec: mov      r0, r7
007e96f0: bl       #0x30ed6c
007e96f4: ldr      r1, [r4, #0x14]
007e96f8: mov      r7, r0
007e96fc: mov      r0, r6
007e9700: bl       #0x30ed6c
007e9704: mov      r1, r0
007e9708: mov      r0, r7
007e970c: bl       #0x30eba4
007e9710: ldr      r1, [r4]
007e9714: mov      r6, r0
007e9718: mov      r0, sl
007e971c: bl       #0x30eba4
007e9720: ldr      r1, [r4, #4]
007e9724: mov      sl, r0
007e9728: mov      r0, r6
007e972c: bl       #0x30eba4
007e9730: ldr      r8, [sp, #0x30]
007e9734: mov      r6, r0
007e9738: mov      r1, sl
007e973c: ldr      r7, [r8]
007e9740: mov      r0, r7
007e9744: bl       #0x30e3ac
007e9748: ldr      sl, [r8, #4]
007e974c: mov      r4, r0
007e9750: mov      r1, r6
007e9754: mov      r0, sl
007e9758: bl       #0x30e3ac
007e975c: mov      r1, r4
007e9760: mov      r6, r0
007e9764: mov      r0, r4
007e9768: bl       #0x30ed6c
007e976c: mov      r1, r6
007e9770: mov      sb, r0
007e9774: mov      r0, r6
007e9778: bl       #0x30ed6c
007e977c: mov      r1, r0
007e9780: mov      r0, sb
007e9784: bl       #0x30eba4
007e9788: ldr      r5, [r5, #0x38]
007e978c: mov      sb, r0
007e9790: mov      r1, r5
007e9794: mov      r0, r5
007e9798: bl       #0x30ed6c
007e979c: mov      r1, r0
007e97a0: mov      r0, sb
007e97a4: bl       #0x30e3ac
007e97a8: mov      r1, #0
007e97ac: mov      sb, r0
007e97b0: bl       #0x30e70c
007e97b4: cmp      r0, #0
007e97b8: bne      #0x7e9884
007e97bc: mov      r1, r7
007e97c0: ldr      r0, [r8, #8]
007e97c4: bl       #0x30e3ac
007e97c8: mov      r1, sl
007e97cc: mov      r7, r0
007e97d0: ldr      r0, [r8, #0xc]
007e97d4: bl       #0x30e3ac
007e97d8: mov      r1, r7
007e97dc: mov      r5, r0
007e97e0: mov      r0, r4
007e97e4: bl       #0x30ed6c
007e97e8: mov      r1, r5
007e97ec: mov      r8, r0
007e97f0: mov      r0, r6
007e97f4: bl       #0x30ed6c
007e97f8: mov      r1, r0
007e97fc: mov      r0, r8
007e9800: bl       #0x30eba4
007e9804: mov      r1, r7
007e9808: mov      r8, r0
007e980c: mov      r0, r7
007e9810: bl       #0x30ed6c
007e9814: mov      r1, r5
007e9818: mov      sl, r0
007e981c: mov      r0, r5
007e9820: bl       #0x30ed6c
007e9824: mov      r1, r0
007e9828: mov      r0, sl
007e982c: bl       #0x30eba4
007e9830: mov      r1, r8
007e9834: mov      sl, r0
007e9838: mov      r0, r8
007e983c: bl       #0x30ed6c
007e9840: mov      r1, sl
007e9844: mov      fp, r0
007e9848: mov      r0, sb
007e984c: bl       #0x30ed6c
007e9850: mov      r1, r0
007e9854: mov      r0, fp
007e9858: bl       #0x30e3ac
007e985c: mov      r1, #0
007e9860: mov      sb, r0
007e9864: bl       #0x30e70c
007e9868: cmp      r0, #0
007e986c: bne      #0x7e9884
007e9870: mov      r0, sl
007e9874: mov      r1, #0x34000000
007e9878: bl       #0x30e70c
007e987c: cmp      r0, #0
007e9880: beq      #0x7e9890
007e9884: mov      r0, #0
007e9888: add      sp, sp, #0xc
007e988c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e9890: mov      r0, sb
007e9894: bl       #0x30e124
007e9898: mov      r1, r8
007e989c: bl       #0x30eba4
007e98a0: add      r8, r0, #0x80000000
007e98a4: mov      r0, r8
007e98a8: mov      r1, #0
007e98ac: bl       #0x30e4b4
007e98b0: cmp      r0, #0
007e98b4: beq      #0x7e9884
007e98b8: mov      r1, sl
007e98bc: ldr      r0, [sp, #0x34]
007e98c0: bl       #0x30ed6c
007e98c4: mov      r1, r8
007e98c8: bl       #0x30e4b4
007e98cc: cmp      r0, #0
007e98d0: beq      #0x7e9884
007e98d4: mov      r1, sl
007e98d8: mov      r0, r8
007e98dc: bl       #0x30ec94
007e98e0: ldr      r3, [sp, #4]
007e98e4: mov      r1, r7
007e98e8: mov      r8, r0
007e98ec: str      r0, [r3]
007e98f0: bl       #0x30ed6c
007e98f4: mov      r1, r5
007e98f8: mov      r7, r0
007e98fc: mov      r0, r8
007e9900: bl       #0x30ed6c
007e9904: mov      r1, r0
007e9908: mov      r0, r6
007e990c: bl       #0x30eba4
007e9910: ldr      r3, [sp]
007e9914: mov      r1, r7
007e9918: str      r0, [r3, #4]
007e991c: mov      r0, r4
007e9920: bl       #0x30eba4
007e9924: ldr      r3, [sp]
007e9928: str      r0, [r3]
007e992c: ldr      r0, [sp]
007e9930: bl       #0x7e5524
007e9934: mov      r0, #1
007e9938: b        #0x7e9888

# _ZN12b2MouseJointC1EPK15b2MouseJointDef
007ebbe0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007ebbe4: ldr      r7, [pc, #0x154]
007ebbe8: mov      r4, r0
007ebbec: mov      r6, r1
007ebbf0: bl       #0x7eb1e0
007ebbf4: ldr      r3, [pc, #0x148]
007ebbf8: add      r7, pc, r7
007ebbfc: ldr      r5, [r4, #0x34]
007ebc00: ldr      r3, [r7, r3]
007ebc04: add      r3, r3, #8
007ebc08: str      r3, [r4]
007ebc0c: ldr      r3, [r6, #0x14]
007ebc10: str      r3, [r4, #0x4c]
007ebc14: ldr      r3, [r6, #0x18]
007ebc18: ldr      r0, [r4, #0x4c]
007ebc1c: str      r3, [r4, #0x50]
007ebc20: ldr      r1, [r5, #4]
007ebc24: bl       #0x30e3ac
007ebc28: ldr      r1, [r5, #8]
007ebc2c: mov      r8, r0
007ebc30: ldr      r0, [r4, #0x50]
007ebc34: bl       #0x30e3ac
007ebc38: ldr      r1, [r5, #0xc]
007ebc3c: mov      r7, r0
007ebc40: mov      r0, r8
007ebc44: bl       #0x30ed6c
007ebc48: ldr      r1, [r5, #0x10]
007ebc4c: mov      sl, r0
007ebc50: mov      r0, r7
007ebc54: bl       #0x30ed6c
007ebc58: mov      r1, r0
007ebc5c: mov      r0, sl
007ebc60: bl       #0x30eba4
007ebc64: ldr      r1, [r5, #0x14]
007ebc68: mov      sl, r0
007ebc6c: mov      r0, r8
007ebc70: bl       #0x30ed6c
007ebc74: ldr      r1, [r5, #0x18]
007ebc78: mov      r8, r0
007ebc7c: mov      r0, r7
007ebc80: bl       #0x30ed6c
007ebc84: mov      r1, r0
007ebc88: mov      r0, r8
007ebc8c: bl       #0x30eba4
007ebc90: str      sl, [r4, #0x44]
007ebc94: str      r0, [r4, #0x48]
007ebc98: ldr      r2, [r6, #0x1c]
007ebc9c: mov      r3, #0
007ebca0: str      r3, [r4, #0x58]
007ebca4: str      r2, [r4, #0x74]
007ebca8: str      r3, [r4, #0x54]
007ebcac: movw     r1, #0xfdb
007ebcb0: ldr      r0, [r6, #0x20]
007ebcb4: movt     r1, #0x40c9
007ebcb8: bl       #0x30ed6c
007ebcbc: ldr      r8, [r5, #0x74]
007ebcc0: ldr      r1, [r6, #0x28]
007ebcc4: mov      r5, r0
007ebcc8: mov      r0, r8
007ebccc: bl       #0x30ed6c
007ebcd0: mov      r1, r5
007ebcd4: mov      r7, r0
007ebcd8: mov      r0, r5
007ebcdc: bl       #0x30ed6c
007ebce0: mov      r1, r0
007ebce4: mov      r0, r7
007ebce8: bl       #0x30ed6c
007ebcec: mov      r1, r8
007ebcf0: mov      r7, r0
007ebcf4: mov      r0, r8
007ebcf8: bl       #0x30eba4
007ebcfc: ldr      r1, [r6, #0x24]
007ebd00: bl       #0x30ed6c
007ebd04: mov      r1, r5
007ebd08: bl       #0x30ed6c
007ebd0c: mov      r1, r7
007ebd10: bl       #0x30eba4
007ebd14: mov      r5, r0
007ebd18: mov      r1, r0
007ebd1c: mov      r0, #0x3f800000
007ebd20: bl       #0x30ec94
007ebd24: mov      r1, r5
007ebd28: str      r0, [r4, #0x7c]
007ebd2c: mov      r0, r7
007ebd30: bl       #0x30ec94
007ebd34: str      r0, [r4, #0x78]
007ebd38: mov      r0, r4
007ebd3c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007ebd40: mulseq   sl, r8, lr
007ebd44: ldrdeq   r1, r2, [r0], -ip

# _ZN8b2IslandC1EiiiP16b2StackAllocatorP17b2ContactListener
007eb0e0: push     {r4, r5, r6, r7, r8, lr}
007eb0e4: ldr      ip, [sp, #0x18]
007eb0e8: mov      r5, r2
007eb0ec: ldr      r2, [sp, #0x1c]
007eb0f0: mov      r6, #0
007eb0f4: mov      r7, r3
007eb0f8: mov      r3, r1
007eb0fc: mov      r4, r0
007eb100: str      r2, [r0, #4]
007eb104: str      r3, [r0, #0x20]
007eb108: str      ip, [r0]
007eb10c: str      r5, [r0, #0x24]
007eb110: str      r7, [r0, #0x28]
007eb114: str      r6, [r0, #0x14]
007eb118: str      r6, [r0, #0x1c]
007eb11c: str      r6, [r0, #0x18]
007eb120: lsl      r1, r1, #2
007eb124: mov      r0, ip
007eb128: bl       #0x7f3644
007eb12c: lsl      r1, r5, #2
007eb130: str      r0, [r4, #8]
007eb134: ldr      r0, [r4]
007eb138: bl       #0x7f3644
007eb13c: lsl      r1, r7, #2
007eb140: str      r0, [r4, #0xc]
007eb144: ldr      r0, [r4]
007eb148: bl       #0x7f3644
007eb14c: str      r6, [r4, #0x2c]
007eb150: str      r0, [r4, #0x10]
007eb154: mov      r0, r4
007eb158: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK13b2PulleyJoint10GetAnchor2Ev
007f03a4: push     {r4, r5, r6, r7, r8, lr}
007f03a8: ldr      r4, [r1, #0x34]
007f03ac: ldr      r7, [r1, #0x60]
007f03b0: ldr      r6, [r1, #0x64]
007f03b4: mov      r5, r0
007f03b8: ldr      r1, [r4, #0xc]
007f03bc: mov      r0, r7
007f03c0: bl       #0x30ed6c
007f03c4: ldr      r1, [r4, #0x14]
007f03c8: mov      r8, r0
007f03cc: mov      r0, r6
007f03d0: bl       #0x30ed6c
007f03d4: mov      r1, r0
007f03d8: mov      r0, r8
007f03dc: bl       #0x30eba4
007f03e0: ldr      r1, [r4, #0x10]
007f03e4: mov      r8, r0
007f03e8: mov      r0, r7
007f03ec: bl       #0x30ed6c
007f03f0: ldr      r1, [r4, #0x18]
007f03f4: mov      r7, r0
007f03f8: mov      r0, r6
007f03fc: bl       #0x30ed6c
007f0400: mov      r1, r0
007f0404: mov      r0, r7
007f0408: bl       #0x30eba4
007f040c: ldr      r1, [r4, #8]
007f0410: bl       #0x30eba4
007f0414: ldr      r1, [r4, #4]
007f0418: mov      r6, r0
007f041c: mov      r0, r8
007f0420: bl       #0x30eba4
007f0424: str      r6, [r5, #4]
007f0428: str      r0, [r5]
007f042c: mov      r0, r5
007f0430: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK15b2RevoluteJoint13GetJointAngleEv
007f2b3c: push     {r4, lr}
007f2b40: ldr      r3, [r0, #0x30]
007f2b44: ldr      r2, [r0, #0x34]
007f2b48: mov      r4, r0
007f2b4c: ldr      r1, [r3, #0x38]
007f2b50: ldr      r0, [r2, #0x38]
007f2b54: bl       #0x30e3ac
007f2b58: ldr      r1, [r4, #0x8c]
007f2b5c: bl       #0x30e3ac
007f2b60: pop      {r4, pc}

# _ZNK16b2PrismaticJoint10GetAnchor2Ev
007ee8b0: push     {r4, r5, r6, r7, r8, lr}
007ee8b4: ldr      r4, [r1, #0x34]
007ee8b8: ldr      r7, [r1, #0x4c]
007ee8bc: ldr      r6, [r1, #0x50]
007ee8c0: mov      r5, r0
007ee8c4: ldr      r1, [r4, #0xc]
007ee8c8: mov      r0, r7
007ee8cc: bl       #0x30ed6c
007ee8d0: ldr      r1, [r4, #0x14]
007ee8d4: mov      r8, r0
007ee8d8: mov      r0, r6
007ee8dc: bl       #0x30ed6c
007ee8e0: mov      r1, r0
007ee8e4: mov      r0, r8
007ee8e8: bl       #0x30eba4
007ee8ec: ldr      r1, [r4, #0x10]
007ee8f0: mov      r8, r0
007ee8f4: mov      r0, r7
007ee8f8: bl       #0x30ed6c
007ee8fc: ldr      r1, [r4, #0x18]
007ee900: mov      r7, r0
007ee904: mov      r0, r6
007ee908: bl       #0x30ed6c
007ee90c: mov      r1, r0
007ee910: mov      r0, r7
007ee914: bl       #0x30eba4
007ee918: ldr      r1, [r4, #8]
007ee91c: bl       #0x30eba4
007ee920: ldr      r1, [r4, #4]
007ee924: mov      r6, r0
007ee928: mov      r0, r8
007ee92c: bl       #0x30eba4
007ee930: str      r6, [r5, #4]
007ee934: str      r0, [r5]
007ee938: mov      r0, r5
007ee93c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN16b2BlockAllocatorC2Ev
007e8fdc: push     {r4, r5, r6, r7, r8, lr}
007e8fe0: mov      r3, #0x80
007e8fe4: mov      r6, #0
007e8fe8: mov      r5, r0
007e8fec: str      r3, [r0, #8]
007e8ff0: str      r6, [r0, #4]
007e8ff4: mov      r0, #0x400
007e8ff8: bl       #0x7f34f4
007e8ffc: ldr      r4, [pc, #0xa8]
007e9000: ldr      r2, [r5, #8]
007e9004: ldr      r7, [pc, #0xa4]
007e9008: add      r4, pc, r4
007e900c: lsl      r2, r2, #3
007e9010: str      r0, [r5]
007e9014: mov      r1, r6
007e9018: bl       #0x30e460
007e901c: ldr      r3, [r4, r7]
007e9020: str      r6, [r5, #0x40]
007e9024: str      r6, [r5, #0xc]
007e9028: str      r6, [r5, #0x10]
007e902c: str      r6, [r5, #0x14]
007e9030: str      r6, [r5, #0x18]
007e9034: str      r6, [r5, #0x1c]
007e9038: str      r6, [r5, #0x20]
007e903c: str      r6, [r5, #0x24]
007e9040: str      r6, [r5, #0x28]
007e9044: str      r6, [r5, #0x2c]
007e9048: str      r6, [r5, #0x30]
007e904c: str      r6, [r5, #0x34]
007e9050: str      r6, [r5, #0x38]
007e9054: str      r6, [r5, #0x3c]
007e9058: ldrb     r2, [r3]
007e905c: cmp      r2, r6
007e9060: bne      #0x7e90a4
007e9064: ldr      r3, [pc, #0x48]
007e9068: ldr      r6, [pc, #0x48]
007e906c: movw     r0, #0x281
007e9070: ldr      ip, [r4, r3]
007e9074: mov      r3, #1
007e9078: ldr      r1, [ip, r2, lsl #2]
007e907c: cmp      r3, r1
007e9080: ldr      r1, [r4, r6]
007e9084: addgt    r2, r2, #1
007e9088: strb     r2, [r3, r1]
007e908c: add      r3, r3, #1
007e9090: cmp      r3, r0
007e9094: bne      #0x7e9078
007e9098: ldr      r3, [r4, r7]
007e909c: mov      r2, #1
007e90a0: strb     r2, [r3]
007e90a4: mov      r0, r5
007e90a8: pop      {r4, r5, r6, r7, r8, pc}
007e90ac: andseq   fp, sl, r8, lsl #21
007e90b0: andeq    r4, r0, r0, lsr #17
007e90b4: strheq   r2, [r0], -r8
007e90b8: strdeq   r1, r2, [r0], -ip

# _ZN13b2CircleShape17UpdateSweepRadiusERK6b2Vec2
007e993c: push     {r4, r5, r6, r7, r8, lr}
007e9940: mov      r4, r0
007e9944: mov      r5, r1
007e9948: ldr      r0, [r0, #0x30]
007e994c: ldr      r1, [r1]
007e9950: bl       #0x30e3ac
007e9954: ldr      r1, [r5, #4]
007e9958: mov      r7, r0
007e995c: ldr      r0, [r4, #0x34]
007e9960: bl       #0x30e3ac
007e9964: mov      r1, r7
007e9968: mov      r6, r0
007e996c: mov      r0, r7
007e9970: bl       #0x30ed6c
007e9974: mov      r1, r6
007e9978: mov      r5, r0
007e997c: mov      r0, r6
007e9980: bl       #0x30ed6c
007e9984: mov      r1, r0
007e9988: mov      r0, r5
007e998c: bl       #0x30eba4
007e9990: bl       #0x30e124
007e9994: ldr      r1, [r4, #0x38]
007e9998: bl       #0x30eba4
007e999c: movw     r1, #0xd70a
007e99a0: movt     r1, #0x3d23
007e99a4: bl       #0x30e3ac
007e99a8: str      r0, [r4, #0x10]
007e99ac: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6b2Body7SetMassEPK10b2MassData
007e1b28: push     {r4, r5, r6, r7, r8, lr}
007e1b2c: ldr      r2, [r0, #0x58]
007e1b30: mov      r3, #0x19000
007e1b34: add      r3, r3, #0x1d4
007e1b38: ldrb     r3, [r2, r3]
007e1b3c: mov      r4, r0
007e1b40: mov      r5, r1
007e1b44: cmp      r3, #0
007e1b48: bne      #0x7e1ce8
007e1b4c: mov      r1, #0
007e1b50: str      r1, [r0, #0x78]
007e1b54: str      r1, [r0, #0x7c]
007e1b58: str      r1, [r0, #0x80]
007e1b5c: ldr      r6, [r5]
007e1b60: str      r6, [r0, #0x74]
007e1b64: mov      r0, r6
007e1b68: bl       #0x30e2f8
007e1b6c: cmp      r0, #0
007e1b70: beq      #0x7e1b84
007e1b74: mov      r1, r6
007e1b78: mov      r0, #0x3f800000
007e1b7c: bl       #0x30ec94
007e1b80: str      r0, [r4, #0x78]
007e1b84: ldrh     r3, [r4]
007e1b88: mov      r1, #0
007e1b8c: tst      r3, #0x40
007e1b90: ldreq    r6, [r5, #0xc]
007e1b94: ldrne    r6, [r4, #0x7c]
007e1b98: streq    r6, [r4, #0x7c]
007e1b9c: mov      r0, r6
007e1ba0: bl       #0x30e2f8
007e1ba4: cmp      r0, #0
007e1ba8: bne      #0x7e1d10
007e1bac: ldr      r3, [r5, #4]
007e1bb0: ldr      r1, [r4, #0xc]
007e1bb4: str      r3, [r4, #0x1c]
007e1bb8: ldr      r3, [r5, #8]
007e1bbc: ldr      r6, [r4, #0x1c]
007e1bc0: str      r3, [r4, #0x20]
007e1bc4: mov      r0, r6
007e1bc8: bl       #0x30ed6c
007e1bcc: ldr      r5, [r4, #0x20]
007e1bd0: mov      r7, r0
007e1bd4: ldr      r1, [r4, #0x14]
007e1bd8: mov      r0, r5
007e1bdc: bl       #0x30ed6c
007e1be0: mov      r1, r0
007e1be4: mov      r0, r7
007e1be8: bl       #0x30eba4
007e1bec: ldr      r1, [r4, #0x10]
007e1bf0: mov      r7, r0
007e1bf4: mov      r0, r6
007e1bf8: bl       #0x30ed6c
007e1bfc: ldr      r1, [r4, #0x18]
007e1c00: mov      r6, r0
007e1c04: mov      r0, r5
007e1c08: bl       #0x30ed6c
007e1c0c: mov      r1, r0
007e1c10: mov      r0, r6
007e1c14: bl       #0x30eba4
007e1c18: ldr      r1, [r4, #4]
007e1c1c: mov      r6, r0
007e1c20: mov      r0, r7
007e1c24: bl       #0x30eba4
007e1c28: ldr      r1, [r4, #8]
007e1c2c: mov      r5, r0
007e1c30: mov      r0, r6
007e1c34: bl       #0x30eba4
007e1c38: str      r5, [r4, #0x2c]
007e1c3c: str      r0, [r4, #0x30]
007e1c40: ldr      r5, [r4, #0x64]
007e1c44: ldr      r2, [r4, #0x2c]
007e1c48: ldr      r3, [r4, #0x30]
007e1c4c: cmp      r5, #0
007e1c50: str      r2, [r4, #0x24]
007e1c54: str      r3, [r4, #0x28]
007e1c58: beq      #0x7e1c80
007e1c5c: add      r6, r4, #0x1c
007e1c60: ldr      r3, [r5]
007e1c64: mov      r0, r5
007e1c68: mov      r1, r6
007e1c6c: mov      lr, pc
007e1c70: ldr      pc, [r3, #0x1c]
007e1c74: ldr      r5, [r5, #8]
007e1c78: cmp      r5, #0
007e1c7c: bne      #0x7e1c60
007e1c80: ldr      r0, [r4, #0x78]
007e1c84: mov      r1, #0
007e1c88: bl       #0x30df8c
007e1c8c: cmp      r0, #0
007e1c90: ldrh     r5, [r4, #2]
007e1c94: bne      #0x7e1cec
007e1c98: mov      r3, #1
007e1c9c: strh     r3, [r4, #2]
007e1ca0: mov      r3, #1
007e1ca4: sxth     r5, r5
007e1ca8: cmp      r5, r3
007e1cac: beq      #0x7e1ce8
007e1cb0: ldr      r5, [r4, #0x64]
007e1cb4: cmp      r5, #0
007e1cb8: beq      #0x7e1ce8
007e1cbc: mov      r6, #0x19000
007e1cc0: add      r6, r6, #0x1d8
007e1cc4: add      r7, r4, #4
007e1cc8: ldr      r3, [r4, #0x58]
007e1ccc: mov      r0, r5
007e1cd0: mov      r2, r7
007e1cd4: ldr      r1, [r3, r6]
007e1cd8: bl       #0x7e61b0
007e1cdc: ldr      r5, [r5, #8]
007e1ce0: cmp      r5, #0
007e1ce4: bne      #0x7e1cc8
007e1ce8: pop      {r4, r5, r6, r7, r8, pc}
007e1cec: ldr      r0, [r4, #0x80]
007e1cf0: mov      r1, #0
007e1cf4: bl       #0x30df8c
007e1cf8: cmp      r0, #0
007e1cfc: movne    r3, #0
007e1d00: strhne   r3, [r4, #2]
007e1d04: movne    r3, #0
007e1d08: bne      #0x7e1ca4
007e1d0c: b        #0x7e1c98
007e1d10: mov      r1, r6
007e1d14: mov      r0, #0x3f800000
007e1d18: bl       #0x30ec94
007e1d1c: str      r0, [r4, #0x80]
007e1d20: b        #0x7e1bac

# _ZN13b2CircleShapeC1EPK10b2ShapeDef
007e99b0: push     {r4, r5, r6, lr}
007e99b4: ldr      r5, [pc, #0x44]
007e99b8: mov      r4, r0
007e99bc: mov      r6, r1
007e99c0: bl       #0x7e6068
007e99c4: ldr      r3, [pc, #0x38]
007e99c8: add      r5, pc, r5
007e99cc: mov      r2, #0
007e99d0: ldr      r3, [r5, r3]
007e99d4: str      r2, [r4, #4]
007e99d8: mov      r0, r4
007e99dc: add      r3, r3, #8
007e99e0: str      r3, [r4]
007e99e4: ldr      r3, [r6, #0x20]
007e99e8: str      r3, [r4, #0x30]
007e99ec: ldr      r3, [r6, #0x24]
007e99f0: str      r3, [r4, #0x34]
007e99f4: ldr      r3, [r6, #0x28]
007e99f8: str      r3, [r4, #0x38]
007e99fc: pop      {r4, r5, r6, pc}
007e9a00: andseq   fp, sl, r8, asr #1
007e9a04: andeq    r0, r0, ip, ror #16

# _ZNK16b2PrismaticJoint13GetLowerLimitEv
007eefb4: ldr      r0, [r0, #0xb8]
007eefb8: bx       lr

# _Z7b2Alloci
007f34f4: ldr      r3, [pc, #0x2c]
007f34f8: ldr      r2, [pc, #0x2c]
007f34fc: push     {r4, lr}
007f3500: add      r3, pc, r3
007f3504: ldr      r2, [r3, r2]
007f3508: add      r4, r0, #4
007f350c: mov      r0, r4
007f3510: ldr      r3, [r2]
007f3514: add      r3, r4, r3
007f3518: str      r3, [r2]
007f351c: bl       #0x30e6f4
007f3520: str      r4, [r0], #4
007f3524: pop      {r4, pc}
007f3528: mulseq   sl, r0, r5
007f352c: andeq    r0, r0, r4, lsl #16

# _ZN16b2StackAllocatorD2Ev
007f3590: bx       lr

# _ZN16b2PrismaticJointC1EPK19b2PrismaticJointDef
007efa08: push     {r4, r5, r6, lr}
007efa0c: ldr      r6, [pc, #0xf0]
007efa10: mov      r4, r0
007efa14: mov      r5, r1
007efa18: bl       #0x7eb1e0
007efa1c: ldr      r2, [pc, #0xe4]
007efa20: add      r6, pc, r6
007efa24: mov      r3, #0
007efa28: ldr      r2, [r6, r2]
007efa2c: mov      r0, r4
007efa30: add      r2, r2, #8
007efa34: str      r2, [r4]
007efa38: ldr      r2, [r5, #0x14]
007efa3c: str      r2, [r4, #0x44]
007efa40: ldr      r2, [r5, #0x18]
007efa44: str      r2, [r4, #0x48]
007efa48: ldr      r2, [r5, #0x1c]
007efa4c: str      r2, [r4, #0x4c]
007efa50: ldr      r2, [r5, #0x20]
007efa54: str      r2, [r4, #0x50]
007efa58: ldr      r2, [r5, #0x24]
007efa5c: str      r2, [r4, #0x54]
007efa60: ldr      r2, [r5, #0x28]
007efa64: ldr      ip, [r4, #0x54]
007efa68: add      r1, r2, #0x80000000
007efa6c: str      ip, [r4, #0x60]
007efa70: str      r1, [r4, #0x5c]
007efa74: str      r2, [r4, #0x58]
007efa78: ldr      r2, [r5, #0x2c]
007efa7c: str      r3, [r4, #0x68]
007efa80: str      r3, [r4, #0x6c]
007efa84: str      r3, [r4, #0x70]
007efa88: str      r3, [r4, #0x74]
007efa8c: str      r3, [r4, #0x78]
007efa90: str      r3, [r4, #0x7c]
007efa94: str      r3, [r4, #0x80]
007efa98: str      r3, [r4, #0x84]
007efa9c: str      r3, [r4, #0x88]
007efaa0: str      r3, [r4, #0x8c]
007efaa4: str      r3, [r4, #0x90]
007efaa8: str      r3, [r4, #0x94]
007efaac: str      r3, [r4, #0x98]
007efab0: str      r3, [r4, #0x9c]
007efab4: str      r2, [r4, #0x64]
007efab8: str      r3, [r4, #0xa0]
007efabc: str      r3, [r4, #0xb4]
007efac0: str      r3, [r4, #0xa4]
007efac4: str      r3, [r4, #0xa8]
007efac8: str      r3, [r4, #0xac]
007efacc: str      r3, [r4, #0xb0]
007efad0: ldr      r3, [r5, #0x34]
007efad4: str      r3, [r4, #0xb8]
007efad8: ldr      r3, [r5, #0x38]
007efadc: str      r3, [r4, #0xbc]
007efae0: ldr      r3, [r5, #0x40]
007efae4: str      r3, [r4, #0xc0]
007efae8: ldr      r3, [r5, #0x44]
007efaec: str      r3, [r4, #0xc4]
007efaf0: ldrb     r3, [r5, #0x30]
007efaf4: strb     r3, [r4, #0xc8]
007efaf8: ldrb     r3, [r5, #0x3c]
007efafc: strb     r3, [r4, #0xc9]
007efb00: pop      {r4, r5, r6, pc}
007efb04: andseq   r5, sl, r0, ror r0
007efb08: andeq    r0, r0, ip, lsr #13

# _ZN7b2JointC2EPK10b2JointDef
007eb1e0: ldr      r2, [pc, #0x54]
007eb1e4: ldr      ip, [pc, #0x54]
007eb1e8: str      r4, [sp, #-4]!
007eb1ec: add      r2, pc, r2
007eb1f0: ldr      ip, [r2, ip]
007eb1f4: mov      r4, #0
007eb1f8: add      ip, ip, #8
007eb1fc: str      ip, [r0]
007eb200: ldr      ip, [r1]
007eb204: str      r4, [r0, #8]
007eb208: str      r4, [r0, #0xc]
007eb20c: str      ip, [r0, #4]
007eb210: ldr      ip, [r1, #8]
007eb214: str      ip, [r0, #0x30]
007eb218: ldr      r2, [r1, #0xc]
007eb21c: str      r2, [r0, #0x34]
007eb220: ldrb     r2, [r1, #0x10]
007eb224: strb     r4, [r0, #0x3c]
007eb228: strb     r2, [r0, #0x3d]
007eb22c: ldr      r2, [r1, #4]
007eb230: str      r2, [r0, #0x40]
007eb234: ldm      sp!, {r4}
007eb238: bx       lr
007eb23c: andseq   sb, sl, r4, lsr #17
007eb240: strheq   r4, [r0], -r4

# _ZN16b2PrismaticJoint9SetLimitsEff
007eefc4: str      r2, [r0, #0xbc]
007eefc8: str      r1, [r0, #0xb8]
007eefcc: bx       lr

# _ZN18b2RevoluteJointDef10InitializeEP6b2BodyS1_RK6b2Vec2
007f1d5c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007f1d60: mov      r6, r0
007f1d64: str      r2, [r6, #0xc]
007f1d68: str      r1, [r6, #8]
007f1d6c: ldr      r0, [r3]
007f1d70: mov      r4, r1
007f1d74: ldr      r1, [r1, #4]
007f1d78: mov      r5, r2
007f1d7c: mov      r7, r3
007f1d80: bl       #0x30e3ac
007f1d84: ldr      r1, [r4, #8]
007f1d88: mov      sl, r0
007f1d8c: ldr      r0, [r7, #4]
007f1d90: bl       #0x30e3ac
007f1d94: ldr      r1, [r4, #0xc]
007f1d98: mov      r8, r0
007f1d9c: mov      r0, sl
007f1da0: bl       #0x30ed6c
007f1da4: ldr      r1, [r4, #0x10]
007f1da8: mov      sb, r0
007f1dac: mov      r0, r8
007f1db0: bl       #0x30ed6c
007f1db4: mov      r1, r0
007f1db8: mov      r0, sb
007f1dbc: bl       #0x30eba4
007f1dc0: ldr      r1, [r4, #0x14]
007f1dc4: mov      sb, r0
007f1dc8: mov      r0, sl
007f1dcc: bl       #0x30ed6c
007f1dd0: ldr      r1, [r4, #0x18]
007f1dd4: mov      sl, r0
007f1dd8: mov      r0, r8
007f1ddc: bl       #0x30ed6c
007f1de0: mov      r1, r0
007f1de4: mov      r0, sl
007f1de8: bl       #0x30eba4
007f1dec: str      sb, [r6, #0x14]
007f1df0: str      r0, [r6, #0x18]
007f1df4: ldr      r1, [r5, #4]
007f1df8: ldr      r0, [r7]
007f1dfc: bl       #0x30e3ac
007f1e00: ldr      r1, [r5, #8]
007f1e04: mov      r8, r0
007f1e08: ldr      r0, [r7, #4]
007f1e0c: bl       #0x30e3ac
007f1e10: ldr      r1, [r5, #0xc]
007f1e14: mov      r7, r0
007f1e18: mov      r0, r8
007f1e1c: bl       #0x30ed6c
007f1e20: ldr      r1, [r5, #0x10]
007f1e24: mov      sl, r0
007f1e28: mov      r0, r7
007f1e2c: bl       #0x30ed6c
007f1e30: mov      r1, r0
007f1e34: mov      r0, sl
007f1e38: bl       #0x30eba4
007f1e3c: ldr      r1, [r5, #0x14]
007f1e40: mov      sl, r0
007f1e44: mov      r0, r8
007f1e48: bl       #0x30ed6c
007f1e4c: ldr      r1, [r5, #0x18]
007f1e50: mov      r8, r0
007f1e54: mov      r0, r7
007f1e58: bl       #0x30ed6c
007f1e5c: mov      r1, r0
007f1e60: mov      r0, r8
007f1e64: bl       #0x30eba4
007f1e68: str      r0, [r6, #0x20]
007f1e6c: str      sl, [r6, #0x1c]
007f1e70: ldr      r0, [r5, #0x38]
007f1e74: ldr      r1, [r4, #0x38]
007f1e78: bl       #0x30e3ac
007f1e7c: str      r0, [r6, #0x24]
007f1e80: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK16b2PrismaticJoint14IsLimitEnabledEv
007eefa4: ldrb     r0, [r0, #0xc8]
007eefa8: bx       lr

# _ZN11b2DebugDrawC2Ev
007e8c9c: ldr      r3, [pc, #0x1c]
007e8ca0: ldr      r2, [pc, #0x1c]
007e8ca4: mov      ip, #0
007e8ca8: add      r3, pc, r3
007e8cac: ldr      r2, [r3, r2]
007e8cb0: str      ip, [r0, #4]
007e8cb4: add      r2, r2, #8
007e8cb8: str      r2, [r0]
007e8cbc: bx       lr
007e8cc0: andseq   fp, sl, r8, ror #27
007e8cc4: strdeq   r1, r2, [r0], -r8

# _ZN22b2PolyAndCircleContactD1Ev
007ebeb0: bx       lr

# _ZN16b2PolygonContact6CreateEP7b2ShapeS1_P16b2BlockAllocator
007ecb54: push     {r4, r5, r6, lr}
007ecb58: mov      r6, r0
007ecb5c: mov      r5, r1
007ecb60: mov      r0, r2
007ecb64: mov      r1, #0x94
007ecb68: bl       #0x7e90bc
007ecb6c: mov      r1, r6
007ecb70: mov      r4, r0
007ecb74: mov      r2, r5
007ecb78: bl       #0x7ecab0
007ecb7c: mov      r0, r4
007ecb80: pop      {r4, r5, r6, pc}

# _ZN15b2RevoluteJoint17SetMaxMotorTorqueEf
007f2ba0: str      r1, [r0, #0x80]
007f2ba4: bx       lr

# _ZN15b2RevoluteJoint11EnableMotorEb
007f2b88: strb     r1, [r0, #0x7c]
007f2b8c: bx       lr

# _ZNK15b2RevoluteJoint14IsMotorEnabledEv
007f2b80: ldrb     r0, [r0, #0x7c]
007f2b84: bx       lr

# _ZN15b2ContactFilter13ShouldCollideEP7b2ShapeS1_
007e8c48: ldrh     ip, [r1, #0x26]
007e8c4c: ldrsh    r0, [r2, #0x26]
007e8c50: sxth     r3, ip
007e8c54: cmp      r0, r3
007e8c58: beq      #0x7e8c84
007e8c5c: ldrh     r0, [r2, #0x22]
007e8c60: ldrh     r3, [r1, #0x24]
007e8c64: ands     r0, r0, r3
007e8c68: bxeq     lr
007e8c6c: ldrh     r2, [r2, #0x24]
007e8c70: ldrh     r3, [r1, #0x22]
007e8c74: tst      r2, r3
007e8c78: moveq    r0, #0
007e8c7c: movne    r0, #1
007e8c80: bx       lr
007e8c84: cmp      ip, #0
007e8c88: beq      #0x7e8c5c
007e8c8c: cmp      r0, #0
007e8c90: movle    r0, #0
007e8c94: movgt    r0, #1
007e8c98: bx       lr

# _ZN15b2RevoluteJoint13SetMotorSpeedEf
007f2b98: str      r1, [r0, #0x84]
007f2b9c: bx       lr

# _ZNK12b2MouseJoint10GetAnchor1Ev
007eb778: ldr      r2, [r1, #0x4c]
007eb77c: str      r2, [r0]
007eb780: ldr      r2, [r1, #0x50]
007eb784: str      r2, [r0, #4]
007eb788: bx       lr

# _ZNK15b2RevoluteJoint13GetUpperLimitEv
007f2bc0: ldr      r0, [r0, #0x94]
007f2bc4: bx       lr

# _ZN7b2World8RefilterEP7b2Shape
007e7afc: mov      r3, #0x19000
007e7b00: add      r3, r3, #0x1d8
007e7b04: ldr      r3, [r0, r3]
007e7b08: ldr      r2, [r1, #0xc]
007e7b0c: mov      r0, r1
007e7b10: mov      r1, r3
007e7b14: add      r2, r2, #4
007e7b18: b        #0x7e61b0

# _ZN8b2IslandC2EiiiP16b2StackAllocatorP17b2ContactListener
007eb15c: push     {r4, r5, r6, r7, r8, lr}
007eb160: ldr      ip, [sp, #0x18]
007eb164: mov      r5, r2
007eb168: ldr      r2, [sp, #0x1c]
007eb16c: mov      r6, #0
007eb170: mov      r7, r3
007eb174: mov      r3, r1
007eb178: mov      r4, r0
007eb17c: str      r2, [r0, #4]
007eb180: str      r3, [r0, #0x20]
007eb184: str      ip, [r0]
007eb188: str      r5, [r0, #0x24]
007eb18c: str      r7, [r0, #0x28]
007eb190: str      r6, [r0, #0x14]
007eb194: str      r6, [r0, #0x1c]
007eb198: str      r6, [r0, #0x18]
007eb19c: lsl      r1, r1, #2
007eb1a0: mov      r0, ip
007eb1a4: bl       #0x7f3644
007eb1a8: lsl      r1, r5, #2
007eb1ac: str      r0, [r4, #8]
007eb1b0: ldr      r0, [r4]
007eb1b4: bl       #0x7f3644
007eb1b8: lsl      r1, r7, #2
007eb1bc: str      r0, [r4, #0xc]
007eb1c0: ldr      r0, [r4]
007eb1c4: bl       #0x7f3644
007eb1c8: str      r6, [r4, #0x2c]
007eb1cc: str      r0, [r4, #0x10]
007eb1d0: mov      r0, r4
007eb1d4: pop      {r4, r5, r6, r7, r8, pc}

# _ZN16b2PolygonContactD1Ev
007eca90: bx       lr

# _ZNK15b2RevoluteJoint17GetReactionTorqueEv
007f2b34: ldr      r0, [r0, #0x60]
007f2b38: bx       lr

# _ZNK15b2RevoluteJoint10GetAnchor2Ev
007f2a90: push     {r4, r5, r6, r7, r8, lr}
007f2a94: ldr      r4, [r1, #0x34]
007f2a98: ldr      r7, [r1, #0x4c]
007f2a9c: ldr      r6, [r1, #0x50]
007f2aa0: mov      r5, r0
007f2aa4: ldr      r1, [r4, #0xc]
007f2aa8: mov      r0, r7
007f2aac: bl       #0x30ed6c
007f2ab0: ldr      r1, [r4, #0x14]
007f2ab4: mov      r8, r0
007f2ab8: mov      r0, r6
007f2abc: bl       #0x30ed6c
007f2ac0: mov      r1, r0
007f2ac4: mov      r0, r8
007f2ac8: bl       #0x30eba4
007f2acc: ldr      r1, [r4, #0x10]
007f2ad0: mov      r8, r0
007f2ad4: mov      r0, r7
007f2ad8: bl       #0x30ed6c
007f2adc: ldr      r1, [r4, #0x18]
007f2ae0: mov      r7, r0
007f2ae4: mov      r0, r6
007f2ae8: bl       #0x30ed6c
007f2aec: mov      r1, r0
007f2af0: mov      r0, r7
007f2af4: bl       #0x30eba4
007f2af8: ldr      r1, [r4, #8]
007f2afc: bl       #0x30eba4
007f2b00: ldr      r1, [r4, #4]
007f2b04: mov      r6, r0
007f2b08: mov      r0, r8
007f2b0c: bl       #0x30eba4
007f2b10: str      r6, [r5, #4]
007f2b14: str      r0, [r5]
007f2b18: mov      r0, r5
007f2b1c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12b2BroadPhase12DestroyProxyEi
007e2acc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e2ad0: mov      r3, #0x5d000
007e2ad4: add      r3, r3, #0x34
007e2ad8: ldr      r3, [r0, r3]
007e2adc: mov      r4, r0
007e2ae0: add      fp, r1, #0x4800
007e2ae4: ldr      r0, [pc, #0x260]
007e2ae8: sub      sp, sp, #0x34
007e2aec: add      fp, fp, #1
007e2af0: mov      r5, r1
007e2af4: lsl      r3, r3, #1
007e2af8: add      sb, r4, fp, lsl #4
007e2afc: add      r1, sp, #0x2c
007e2b00: add      r2, sp, #0x28
007e2b04: add      r0, pc, r0
007e2b08: str      r3, [sp, #0x14]
007e2b0c: add      sb, sb, #4
007e2b10: sub      r7, r3, #2
007e2b14: mov      r6, #0
007e2b18: str      r1, [sp, #0x1c]
007e2b1c: str      r2, [sp, #0x18]
007e2b20: mov      r8, #6
007e2b24: str      r5, [sp, #0x20]
007e2b28: str      fp, [sp, #0x24]
007e2b2c: str      r0, [sp, #0x10]
007e2b30: ldrh     r2, [sb]
007e2b34: mov      r3, #0x6000
007e2b38: mul      r5, r3, r6
007e2b3c: str      r2, [sp, #0x2c]
007e2b40: ldrh     r0, [sb, #4]
007e2b44: mul      r3, r8, r2
007e2b48: mla      r1, r2, r8, r8
007e2b4c: add      r5, r5, #0x50000
007e2b50: sub      lr, r0, #1
007e2b54: mul      ip, r8, r0
007e2b58: add      r5, r4, r5
007e2b5c: add      r5, r5, #0x16
007e2b60: rsb      r2, r2, lr
007e2b64: str      r0, [sp, #0x28]
007e2b68: add      r1, r5, r1
007e2b6c: mul      r2, r8, r2
007e2b70: add      r0, r5, r3
007e2b74: ldrh     fp, [r5, ip]
007e2b78: ldrh     sl, [r5, r3]
007e2b7c: bl       #0x30df38
007e2b80: ldr      r3, [sp, #0x28]
007e2b84: ldr      r0, [sp, #0x14]
007e2b88: mla      r1, r3, r8, r8
007e2b8c: rsb      r2, r3, r0
007e2b90: sub      r2, r2, #1
007e2b94: sub      r3, r3, #1
007e2b98: mla      r0, r8, r3, r5
007e2b9c: add      r1, r5, r1
007e2ba0: mul      r2, r8, r2
007e2ba4: bl       #0x30df38
007e2ba8: ldr      r3, [sp, #0x2c]
007e2bac: cmp      r3, r7
007e2bb0: bge      #0x7e2bf0
007e2bb4: mla      r2, r8, r3, r5
007e2bb8: add      r2, r2, #2
007e2bbc: ldrh     r1, [r2]
007e2bc0: ldrh     r0, [r2, #-2]
007e2bc4: add      r2, r2, #6
007e2bc8: add      r1, r6, r1, lsl #3
007e2bcc: add      r1, r1, #0x24000
007e2bd0: tst      r0, #1
007e2bd4: add      r1, r4, r1, lsl #1
007e2bd8: strheq   r3, [r1, #0x14]
007e2bdc: strhne   r3, [r1, #0x18]
007e2be0: add      r3, r3, #1
007e2be4: cmp      r3, r7
007e2be8: blt      #0x7e2bbc
007e2bec: ldr      r3, [sp, #0x2c]
007e2bf0: ldr      r2, [sp, #0x28]
007e2bf4: sub      r2, r2, #1
007e2bf8: cmp      r2, r3
007e2bfc: ble      #0x7e2c28
007e2c00: mla      r2, r8, r3, r5
007e2c04: add      r2, r2, #4
007e2c08: ldrh     r1, [r2]
007e2c0c: add      r3, r3, #1
007e2c10: sub      r1, r1, #1
007e2c14: strh     r1, [r2], #6
007e2c18: ldr      r1, [sp, #0x28]
007e2c1c: sub      r1, r1, #1
007e2c20: cmp      r1, r3
007e2c24: bgt      #0x7e2c08
007e2c28: str      r6, [sp, #0xc]
007e2c2c: mov      r3, sl
007e2c30: add      r6, r6, #1
007e2c34: mov      r0, r4
007e2c38: ldr      r1, [sp, #0x1c]
007e2c3c: ldr      r2, [sp, #0x18]
007e2c40: str      fp, [sp]
007e2c44: stmib    sp, {r5, r7}
007e2c48: bl       #0x7e2820
007e2c4c: cmp      r6, #2
007e2c50: add      sb, sb, #2
007e2c54: bne      #0x7e2b30
007e2c58: mov      r8, #0x5d000
007e2c5c: add      r8, r8, #0x18
007e2c60: ldr      r3, [r4, r8]
007e2c64: ldr      r5, [sp, #0x20]
007e2c68: ldr      fp, [sp, #0x24]
007e2c6c: cmp      r3, #0
007e2c70: ble      #0x7e2ca0
007e2c74: add      r7, r4, #0x5c000
007e2c78: add      r7, r7, #0x16
007e2c7c: mov      r6, #0
007e2c80: mov      r0, r4
007e2c84: mov      r1, r5
007e2c88: ldrh     r2, [r7], #2
007e2c8c: bl       #0x7e4224
007e2c90: ldr      r3, [r4, r8]
007e2c94: add      r6, r6, #1
007e2c98: cmp      r3, r6
007e2c9c: bgt      #0x7e2c80
007e2ca0: mov      r0, r4
007e2ca4: bl       #0x7e42e0
007e2ca8: mov      r3, #0x5d000
007e2cac: add      r3, r3, #0x18
007e2cb0: mov      r6, #0
007e2cb4: str      r6, [r4, r3]
007e2cb8: mov      r0, r4
007e2cbc: bl       #0x7e276c
007e2cc0: add      ip, r4, r5, lsl #4
007e2cc4: add      r3, ip, #0x48000
007e2cc8: mov      r0, #0x48000
007e2ccc: mov      r1, r3
007e2cd0: add      r0, r0, #0x20
007e2cd4: str      r6, [ip, r0]
007e2cd8: add      fp, r4, fp, lsl #4
007e2cdc: mvn      r0, #0
007e2ce0: add      r3, r3, #0x12
007e2ce4: add      r1, r1, #0x18
007e2ce8: mov      r2, #0x50000
007e2cec: strh     r0, [r1, #4]
007e2cf0: add      r2, r2, #0x14
007e2cf4: strh     r0, [fp, #4]
007e2cf8: strh     r0, [r3, #4]
007e2cfc: strh     r0, [fp, #8]
007e2d00: strh     r0, [r3, #8]
007e2d04: ldrh     r1, [r4, r2]
007e2d08: mov      r3, #0x5d000
007e2d0c: add      r3, r3, #0x34
007e2d10: strh     r1, [fp, #4]
007e2d14: strh     r5, [r4, r2]
007e2d18: ldr      r0, [sp, #0x10]
007e2d1c: ldr      r2, [pc, #0x2c]
007e2d20: ldr      r1, [r4, r3]
007e2d24: ldr      r2, [r0, r2]
007e2d28: sub      r1, r1, #1
007e2d2c: str      r1, [r4, r3]
007e2d30: ldrb     r3, [r2]
007e2d34: cmp      r3, r6
007e2d38: beq      #0x7e2d44
007e2d3c: mov      r0, r4
007e2d40: bl       #0x7e2ac4
007e2d44: add      sp, sp, #0x34
007e2d48: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e2d4c: andseq   r1, fp, ip, lsl #31
007e2d50: andeq    r2, r0, r8, ror #26

# _ZN11b2DebugDrawD0Ev
007e8d38: push     {r4, lr}
007e8d3c: mov      r4, r0
007e8d40: bl       #0x30e2b0
007e8d44: mov      r0, r4
007e8d48: pop      {r4, pc}

# _ZN16b2PrismaticJoint11EnableMotorEb
007eefd8: strb     r1, [r0, #0xc9]
007eefdc: bx       lr

# _ZN13b2PairManager7AddPairEii
007e3fa8: cmp      r1, r2
007e3fac: movgt    r3, r1
007e3fb0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e3fb4: mov      r4, r1
007e3fb8: mov      r5, r2
007e3fbc: movgt    r4, r2
007e3fc0: movgt    r5, r3
007e3fc4: orr      r3, r4, r5, lsl #16
007e3fc8: mvn      r6, r3
007e3fcc: add      r6, r6, r3, lsl #15
007e3fd0: movw     r3, #0x809
007e3fd4: eor      r6, r6, r6, lsr #12
007e3fd8: mov      r1, r4
007e3fdc: add      r6, r6, r6, lsl #2
007e3fe0: mov      r2, r5
007e3fe4: eor      r6, r6, r6, lsr #4
007e3fe8: mul      r6, r3, r6
007e3fec: mov      r7, r0
007e3ff0: eor      r6, r6, r6, lsr #16
007e3ff4: lsl      r6, r6, #0x12
007e3ff8: lsr      r6, r6, #0x12
007e3ffc: mov      r3, r6
007e4000: bl       #0x7e3ef4
007e4004: subs     r3, r0, #0
007e4008: beq      #0x7e4014
007e400c: mov      r0, r3
007e4010: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e4014: mov      r2, #0x30000
007e4018: add      r2, r2, #8
007e401c: ldrh     r1, [r7, r2]
007e4020: mov      r3, #0xc
007e4024: add      r6, r6, #0x20000
007e4028: mul      r3, r3, r1
007e402c: add      r6, r6, #8
007e4030: add      ip, r7, r3
007e4034: ldrh     sl, [ip, #0x10]
007e4038: add      r8, ip, #8
007e403c: add      r6, r7, r6, lsl #1
007e4040: strh     sl, [r7, r2]
007e4044: strh     r5, [r8, #6]
007e4048: strh     r4, [r8, #4]
007e404c: str      r0, [ip, #8]
007e4050: strh     r0, [ip, #0x12]
007e4054: ldrh     r0, [r6, #4]
007e4058: mov      r2, #0x30000
007e405c: add      r2, r2, #0xc
007e4060: strh     r0, [ip, #0x10]
007e4064: strh     r1, [r6, #4]
007e4068: ldr      r1, [r7, r2]
007e406c: add      r3, r3, #8
007e4070: add      r3, r7, r3
007e4074: add      r1, r1, #1
007e4078: str      r1, [r7, r2]
007e407c: mov      r0, r3
007e4080: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN12b2PolygonDef8SetAsBoxEffRK6b2Vec2f
007e5f40: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e5f44: sub      sp, sp, #0x14
007e5f48: ldr      r6, [sp, #0x38]
007e5f4c: mov      r5, r3
007e5f50: mov      r4, r0
007e5f54: bl       #0x7e44b8
007e5f58: mov      r0, r6
007e5f5c: bl       #0x30e754
007e5f60: mov      r8, r0
007e5f64: mov      r0, r6
007e5f68: bl       #0x30eb08
007e5f6c: ldr      r3, [r5]
007e5f70: ldr      fp, [r4, #0x60]
007e5f74: mov      sb, r0
007e5f78: str      r3, [sp, #4]
007e5f7c: ldr      r5, [r5, #4]
007e5f80: add      r3, r0, #0x80000000
007e5f84: cmp      fp, #0
007e5f88: str      r5, [sp, #8]
007e5f8c: str      r3, [sp, #0xc]
007e5f90: ble      #0x7e6024
007e5f94: mov      r5, #0
007e5f98: ldr      r7, [r4, #0x20]
007e5f9c: mov      r0, r8
007e5fa0: ldr      r6, [r4, #0x24]
007e5fa4: mov      r1, r7
007e5fa8: bl       #0x30ed6c
007e5fac: mov      r1, r6
007e5fb0: mov      sl, r0
007e5fb4: ldr      r0, [sp, #0xc]
007e5fb8: bl       #0x30ed6c
007e5fbc: mov      r1, r0
007e5fc0: mov      r0, sl
007e5fc4: bl       #0x30eba4
007e5fc8: mov      r1, r7
007e5fcc: mov      sl, r0
007e5fd0: mov      r0, sb
007e5fd4: bl       #0x30ed6c
007e5fd8: mov      r1, r6
007e5fdc: mov      r7, r0
007e5fe0: mov      r0, r8
007e5fe4: bl       #0x30ed6c
007e5fe8: mov      r1, r0
007e5fec: mov      r0, r7
007e5ff0: bl       #0x30eba4
007e5ff4: mov      r1, r0
007e5ff8: ldr      r0, [sp, #8]
007e5ffc: bl       #0x30eba4
007e6000: str      r0, [r4, #0x24]
007e6004: ldr      r0, [sp, #4]
007e6008: mov      r1, sl
007e600c: bl       #0x30eba4
007e6010: add      r5, r5, #1
007e6014: cmp      r5, fp
007e6018: str      r0, [r4, #0x20]
007e601c: add      r4, r4, #8
007e6020: blt      #0x7e5f98
007e6024: add      sp, sp, #0x14
007e6028: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN9b2Contact7DestroyEPS_P16b2BlockAllocator
007e9c98: ldr      r3, [r0, #8]
007e9c9c: ldr      r2, [pc, #0x78]
007e9ca0: push     {r4, lr}
007e9ca4: cmp      r3, #0
007e9ca8: add      r2, pc, r2
007e9cac: ble      #0x7e9ce4
007e9cb0: ldr      r3, [r0, #0x34]
007e9cb4: mov      ip, #0
007e9cb8: ldr      r3, [r3, #0xc]
007e9cbc: ldrh     lr, [r3]
007e9cc0: str      ip, [r3, #0x8c]
007e9cc4: bic      lr, lr, #8
007e9cc8: strh     lr, [r3]
007e9ccc: ldr      r3, [r0, #0x38]
007e9cd0: ldr      r3, [r3, #0xc]
007e9cd4: ldrh     lr, [r3]
007e9cd8: str      ip, [r3, #0x8c]
007e9cdc: bic      ip, lr, #8
007e9ce0: strh     ip, [r3]
007e9ce4: ldr      r3, [r0, #0x34]
007e9ce8: ldr      ip, [r0, #0x38]
007e9cec: ldr      r3, [r3, #4]
007e9cf0: ldr      lr, [ip, #4]
007e9cf4: mov      ip, #0x18
007e9cf8: mul      ip, ip, r3
007e9cfc: mov      r3, #0xc
007e9d00: mla      r3, r3, lr, ip
007e9d04: ldr      ip, [pc, #0x14]
007e9d08: ldr      r2, [r2, ip]
007e9d0c: add      r3, r2, r3
007e9d10: mov      lr, pc
007e9d14: ldr      pc, [r3, #4]
007e9d18: pop      {r4, pc}
007e9d1c: andseq   sl, sl, r8, ror #27
007e9d20: strdeq   r1, r2, [r0], -r4

# _ZN6b2Body8SetXFormERK6b2Vec2f
007e164c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e1650: ldr      r8, [r0, #0x58]
007e1654: mov      r3, #0x19000
007e1658: add      r3, r3, #0x1d4
007e165c: ldrb     r3, [r8, r3]
007e1660: mov      r4, r0
007e1664: mov      r5, r1
007e1668: cmp      r3, #0
007e166c: mov      r6, r2
007e1670: bne      #0x7e169c
007e1674: ldrh     r3, [r0]
007e1678: tst      r3, #2
007e167c: beq      #0x7e16a4
007e1680: mov      r0, #0
007e1684: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e1688: ldr      r8, [r4, #0x58]
007e168c: mov      r3, #0x19000
007e1690: add      r3, r3, #0x1d8
007e1694: ldr      r0, [r8, r3]
007e1698: bl       #0x7e2ac8
007e169c: mov      r0, #1
007e16a0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e16a4: mov      r0, r2
007e16a8: bl       #0x30e754
007e16ac: mov      r7, r0
007e16b0: mov      r0, r6
007e16b4: bl       #0x30eb08
007e16b8: add      r2, r0, #0x80000000
007e16bc: str      r2, [r4, #0x14]
007e16c0: str      r7, [r4, #0xc]
007e16c4: str      r0, [r4, #0x10]
007e16c8: str      r7, [r4, #0x18]
007e16cc: ldr      r3, [r5]
007e16d0: ldr      sb, [r4, #0x1c]
007e16d4: mov      sl, r0
007e16d8: str      r3, [r4, #4]
007e16dc: ldr      r3, [r5, #4]
007e16e0: mov      r1, r7
007e16e4: mov      r0, sb
007e16e8: str      r3, [r4, #8]
007e16ec: mov      fp, r2
007e16f0: bl       #0x30ed6c
007e16f4: mov      r1, fp
007e16f8: mov      r5, r0
007e16fc: ldr      r0, [r4, #0x20]
007e1700: bl       #0x30ed6c
007e1704: mov      r1, r0
007e1708: mov      r0, r5
007e170c: bl       #0x30eba4
007e1710: mov      r1, sl
007e1714: mov      r5, r0
007e1718: mov      r0, sb
007e171c: bl       #0x30ed6c
007e1720: mov      r1, r7
007e1724: mov      sl, r0
007e1728: ldr      r0, [r4, #0x20]
007e172c: bl       #0x30ed6c
007e1730: mov      r1, r0
007e1734: mov      r0, sl
007e1738: bl       #0x30eba4
007e173c: ldr      r1, [r4, #4]
007e1740: mov      r7, r0
007e1744: mov      r0, r5
007e1748: bl       #0x30eba4
007e174c: ldr      r1, [r4, #8]
007e1750: mov      r5, r0
007e1754: mov      r0, r7
007e1758: bl       #0x30eba4
007e175c: str      r5, [r4, #0x2c]
007e1760: str      r0, [r4, #0x30]
007e1764: ldr      r5, [r4, #0x64]
007e1768: ldr      r2, [r4, #0x2c]
007e176c: ldr      r3, [r4, #0x30]
007e1770: cmp      r5, #0
007e1774: str      r2, [r4, #0x24]
007e1778: str      r3, [r4, #0x28]
007e177c: str      r6, [r4, #0x34]
007e1780: str      r6, [r4, #0x38]
007e1784: beq      #0x7e168c
007e1788: mov      r7, #0x19000
007e178c: add      r7, r7, #0x1d8
007e1790: add      r6, r4, #4
007e1794: b        #0x7e17a8
007e1798: ldr      r5, [r5, #8]
007e179c: cmp      r5, #0
007e17a0: beq      #0x7e1688
007e17a4: ldr      r8, [r4, #0x58]
007e17a8: ldr      r1, [r8, r7]
007e17ac: mov      r0, r5
007e17b0: mov      r2, r6
007e17b4: mov      r3, r6
007e17b8: bl       #0x7e63d0
007e17bc: cmp      r0, #0
007e17c0: bne      #0x7e1798
007e17c4: ldrh     r2, [r4]
007e17c8: ldr      r5, [r4, #0x64]
007e17cc: mov      r3, #0
007e17d0: orr      r2, r2, #2
007e17d4: cmp      r5, #0
007e17d8: strh     r2, [r4]
007e17dc: str      r3, [r4, #0x48]
007e17e0: str      r3, [r4, #0x40]
007e17e4: str      r3, [r4, #0x44]
007e17e8: beq      #0x7e1680
007e17ec: mov      r6, #0x19000
007e17f0: add      r6, r6, #0x1d8
007e17f4: ldr      r3, [r4, #0x58]
007e17f8: mov      r0, r5
007e17fc: ldr      r1, [r3, r6]
007e1800: bl       #0x7e6180
007e1804: ldr      r5, [r5, #8]
007e1808: cmp      r5, #0
007e180c: bne      #0x7e17f4
007e1810: mov      r0, #0
007e1814: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN13b2PairManager4FindEii
007e3f68: cmp      r1, r2
007e3f6c: movgt    r3, r1
007e3f70: movgt    r1, r2
007e3f74: movgt    r2, r3
007e3f78: orr      r3, r1, r2, lsl #16
007e3f7c: mvn      ip, r3
007e3f80: add      r3, ip, r3, lsl #15
007e3f84: movw     ip, #0x809
007e3f88: eor      r3, r3, r3, lsr #12
007e3f8c: add      r3, r3, r3, lsl #2
007e3f90: eor      r3, r3, r3, lsr #4
007e3f94: mul      r3, ip, r3
007e3f98: eor      r3, r3, r3, lsr #16
007e3f9c: lsl      r3, r3, #0x12
007e3fa0: lsr      r3, r3, #0x12
007e3fa4: b        #0x7e3ef4

# _ZNK16b2PrismaticJoint13GetUpperLimitEv
007eefbc: ldr      r0, [r0, #0xbc]
007eefc0: bx       lr

# _ZN22b2PolyAndCircleContact7DestroyEP9b2ContactP16b2BlockAllocator
007ebf60: push     {r4, r5, r6, lr}
007ebf64: ldr      r3, [r0]
007ebf68: mov      r5, r1
007ebf6c: mov      r4, r0
007ebf70: mov      lr, pc
007ebf74: ldr      pc, [r3, #4]
007ebf78: mov      r0, r5
007ebf7c: mov      r1, r4
007ebf80: mov      r2, #0x94
007ebf84: pop      {r4, r5, r6, lr}
007ebf88: b        #0x7e8da0

# _ZN19b2PrismaticJointDef10InitializeEP6b2BodyS1_RK6b2Vec2S4_
007ed658: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007ed65c: mov      r5, r0
007ed660: str      r2, [r5, #0xc]
007ed664: str      r1, [r5, #8]
007ed668: ldr      r0, [r3]
007ed66c: mov      r4, r1
007ed670: ldr      r1, [r1, #4]
007ed674: mov      r6, r2
007ed678: mov      r7, r3
007ed67c: bl       #0x30e3ac
007ed680: ldr      r1, [r4, #8]
007ed684: mov      sl, r0
007ed688: ldr      r0, [r7, #4]
007ed68c: bl       #0x30e3ac
007ed690: ldr      r1, [r4, #0xc]
007ed694: mov      r8, r0
007ed698: mov      r0, sl
007ed69c: bl       #0x30ed6c
007ed6a0: ldr      r1, [r4, #0x10]
007ed6a4: mov      sb, r0
007ed6a8: mov      r0, r8
007ed6ac: bl       #0x30ed6c
007ed6b0: mov      r1, r0
007ed6b4: mov      r0, sb
007ed6b8: bl       #0x30eba4
007ed6bc: ldr      r1, [r4, #0x14]
007ed6c0: mov      sb, r0
007ed6c4: mov      r0, sl
007ed6c8: bl       #0x30ed6c
007ed6cc: ldr      r1, [r4, #0x18]
007ed6d0: mov      sl, r0
007ed6d4: mov      r0, r8
007ed6d8: bl       #0x30ed6c
007ed6dc: mov      r1, r0
007ed6e0: mov      r0, sl
007ed6e4: bl       #0x30eba4
007ed6e8: str      sb, [r5, #0x14]
007ed6ec: str      r0, [r5, #0x18]
007ed6f0: ldr      r1, [r6, #4]
007ed6f4: ldr      r0, [r7]
007ed6f8: bl       #0x30e3ac
007ed6fc: ldr      r1, [r6, #8]
007ed700: mov      r8, r0
007ed704: ldr      r0, [r7, #4]
007ed708: bl       #0x30e3ac
007ed70c: ldr      r1, [r6, #0xc]
007ed710: mov      r7, r0
007ed714: mov      r0, r8
007ed718: bl       #0x30ed6c
007ed71c: ldr      r1, [r6, #0x10]
007ed720: mov      sl, r0
007ed724: mov      r0, r7
007ed728: bl       #0x30ed6c
007ed72c: mov      r1, r0
007ed730: mov      r0, sl
007ed734: bl       #0x30eba4
007ed738: ldr      r1, [r6, #0x14]
007ed73c: mov      sl, r0
007ed740: mov      r0, r8
007ed744: bl       #0x30ed6c
007ed748: ldr      r1, [r6, #0x18]
007ed74c: mov      r8, r0
007ed750: mov      r0, r7
007ed754: bl       #0x30ed6c
007ed758: mov      r1, r0
007ed75c: mov      r0, r8
007ed760: bl       #0x30eba4
007ed764: ldr      r3, [sp, #0x20]
007ed768: str      r0, [r5, #0x20]
007ed76c: str      sl, [r5, #0x1c]
007ed770: ldr      r8, [r3]
007ed774: ldr      r1, [r4, #0xc]
007ed778: ldr      r7, [r3, #4]
007ed77c: mov      r0, r8
007ed780: bl       #0x30ed6c
007ed784: ldr      r1, [r4, #0x10]
007ed788: mov      sl, r0
007ed78c: mov      r0, r7
007ed790: bl       #0x30ed6c
007ed794: mov      r1, r0
007ed798: mov      r0, sl
007ed79c: bl       #0x30eba4
007ed7a0: ldr      r1, [r4, #0x14]
007ed7a4: mov      sl, r0
007ed7a8: mov      r0, r8
007ed7ac: bl       #0x30ed6c
007ed7b0: ldr      r1, [r4, #0x18]
007ed7b4: mov      r8, r0
007ed7b8: mov      r0, r7
007ed7bc: bl       #0x30ed6c
007ed7c0: mov      r1, r0
007ed7c4: mov      r0, r8
007ed7c8: bl       #0x30eba4
007ed7cc: str      sl, [r5, #0x24]
007ed7d0: str      r0, [r5, #0x28]
007ed7d4: ldr      r0, [r6, #0x38]
007ed7d8: ldr      r1, [r4, #0x38]
007ed7dc: bl       #0x30e3ac
007ed7e0: str      r0, [r5, #0x2c]
007ed7e4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN13b2PulleyJoint23InitVelocityConstraintsERK10b2TimeStep
007f09c0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007f09c4: ldr      r6, [r0, #0x30]
007f09c8: sub      sp, sp, #0x2c
007f09cc: str      r1, [sp, #0xc]
007f09d0: mov      r4, r0
007f09d4: ldr      r1, [r6, #0x1c]
007f09d8: ldr      r0, [r0, #0x58]
007f09dc: bl       #0x30e3ac
007f09e0: ldr      r1, [r6, #0x20]
007f09e4: mov      r8, r0
007f09e8: ldr      r0, [r4, #0x5c]
007f09ec: bl       #0x30e3ac
007f09f0: ldr      r1, [r6, #0xc]
007f09f4: mov      r7, r0
007f09f8: mov      r0, r8
007f09fc: bl       #0x30ed6c
007f0a00: ldr      r1, [r6, #0x14]
007f0a04: mov      r5, r0
007f0a08: mov      r0, r7
007f0a0c: bl       #0x30ed6c
007f0a10: mov      r1, r0
007f0a14: mov      r0, r5
007f0a18: bl       #0x30eba4
007f0a1c: ldr      r1, [r6, #0x10]
007f0a20: mov      fp, r0
007f0a24: mov      r0, r8
007f0a28: bl       #0x30ed6c
007f0a2c: ldr      r1, [r6, #0x18]
007f0a30: mov      r8, r0
007f0a34: mov      r0, r7
007f0a38: bl       #0x30ed6c
007f0a3c: mov      r1, r0
007f0a40: mov      r0, r8
007f0a44: bl       #0x30eba4
007f0a48: ldr      r5, [r4, #0x34]
007f0a4c: mov      sb, r0
007f0a50: ldr      r0, [r4, #0x60]
007f0a54: ldr      r1, [r5, #0x1c]
007f0a58: bl       #0x30e3ac
007f0a5c: ldr      r1, [r5, #0x20]
007f0a60: mov      r8, r0
007f0a64: ldr      r0, [r4, #0x64]
007f0a68: bl       #0x30e3ac
007f0a6c: ldr      r1, [r5, #0xc]
007f0a70: mov      r7, r0
007f0a74: mov      r0, r8
007f0a78: bl       #0x30ed6c
007f0a7c: ldr      r1, [r5, #0x14]
007f0a80: mov      sl, r0
007f0a84: mov      r0, r7
007f0a88: bl       #0x30ed6c
007f0a8c: mov      r1, r0
007f0a90: mov      r0, sl
007f0a94: bl       #0x30eba4
007f0a98: ldr      r1, [r5, #0x10]
007f0a9c: mov      sl, r0
007f0aa0: mov      r0, r8
007f0aa4: bl       #0x30ed6c
007f0aa8: ldr      r1, [r5, #0x18]
007f0aac: mov      r8, r0
007f0ab0: mov      r0, r7
007f0ab4: bl       #0x30ed6c
007f0ab8: mov      r1, r0
007f0abc: mov      r0, r8
007f0ac0: bl       #0x30eba4
007f0ac4: str      r0, [sp, #0x14]
007f0ac8: ldr      r1, [r6, #0x2c]
007f0acc: mov      r0, fp
007f0ad0: bl       #0x30eba4
007f0ad4: ldr      r1, [r6, #0x30]
007f0ad8: mov      r2, r0
007f0adc: mov      r0, sb
007f0ae0: str      r2, [sp, #4]
007f0ae4: bl       #0x30eba4
007f0ae8: str      r0, [sp, #0x10]
007f0aec: ldr      r1, [r5, #0x2c]
007f0af0: mov      r0, sl
007f0af4: bl       #0x30eba4
007f0af8: str      r0, [sp, #0x18]
007f0afc: ldr      r1, [r5, #0x30]
007f0b00: ldr      r0, [sp, #0x14]
007f0b04: bl       #0x30eba4
007f0b08: str      r0, [sp, #0x1c]
007f0b0c: ldr      r7, [r4, #0x44]
007f0b10: ldr      r1, [r4, #0x48]
007f0b14: ldr      r8, [r7, #4]
007f0b18: mov      r0, r8
007f0b1c: bl       #0x30eba4
007f0b20: ldr      r7, [r7, #8]
007f0b24: ldr      r1, [r4, #0x4c]
007f0b28: mov      ip, r0
007f0b2c: mov      r0, r7
007f0b30: str      ip, [sp, #8]
007f0b34: bl       #0x30eba4
007f0b38: str      r0, [sp, #0x20]
007f0b3c: ldr      r1, [r4, #0x50]
007f0b40: mov      r0, r8
007f0b44: bl       #0x30eba4
007f0b48: str      r0, [sp, #0x24]
007f0b4c: ldr      r1, [r4, #0x54]
007f0b50: mov      r0, r7
007f0b54: bl       #0x30eba4
007f0b58: ldmib    sp, {r2, ip}
007f0b5c: mov      r3, r0
007f0b60: mov      r1, ip
007f0b64: mov      r0, r2
007f0b68: str      r3, [sp, #8]
007f0b6c: bl       #0x30e3ac
007f0b70: ldr      r1, [sp, #0x20]
007f0b74: mov      r8, r0
007f0b78: ldr      r0, [sp, #0x10]
007f0b7c: bl       #0x30e3ac
007f0b80: str      r0, [r4, #0x6c]
007f0b84: ldr      r3, [sp, #8]
007f0b88: str      r8, [r4, #0x68]
007f0b8c: mov      r7, r0
007f0b90: mov      r1, r3
007f0b94: ldr      r0, [sp, #0x1c]
007f0b98: bl       #0x30e3ac
007f0b9c: str      r0, [r4, #0x74]
007f0ba0: ldr      r1, [sp, #0x24]
007f0ba4: ldr      r0, [sp, #0x18]
007f0ba8: bl       #0x30e3ac
007f0bac: mov      r1, r8
007f0bb0: str      r0, [r4, #0x70]
007f0bb4: mov      r0, r8
007f0bb8: bl       #0x30ed6c
007f0bbc: mov      r1, r7
007f0bc0: mov      r8, r0
007f0bc4: mov      r0, r7
007f0bc8: bl       #0x30ed6c
007f0bcc: mov      r1, r0
007f0bd0: mov      r0, r8
007f0bd4: bl       #0x30eba4
007f0bd8: bl       #0x30e124
007f0bdc: mov      r8, r0
007f0be0: ldr      r0, [r4, #0x70]
007f0be4: mov      r1, r0
007f0be8: bl       #0x30ed6c
007f0bec: mov      r7, r0
007f0bf0: ldr      r0, [r4, #0x74]
007f0bf4: mov      r1, r0
007f0bf8: bl       #0x30ed6c
007f0bfc: mov      r1, r0
007f0c00: mov      r0, r7
007f0c04: bl       #0x30eba4
007f0c08: bl       #0x30e124
007f0c0c: movw     r1, #0xd70a
007f0c10: mov      r7, r0
007f0c14: movt     r1, #0x3ba3
007f0c18: mov      r0, r8
007f0c1c: bl       #0x30e2f8
007f0c20: cmp      r0, #0
007f0c24: moveq    r3, #0
007f0c28: streq    r3, [r4, #0x6c]
007f0c2c: streq    r3, [r4, #0x68]
007f0c30: beq      #0x7f0c6c
007f0c34: mov      r1, r8
007f0c38: mov      r0, #0x3f800000
007f0c3c: bl       #0x30ec94
007f0c40: mov      r3, r0
007f0c44: mov      r1, r0
007f0c48: ldr      r0, [r4, #0x68]
007f0c4c: str      r3, [sp, #8]
007f0c50: bl       #0x30ed6c
007f0c54: str      r0, [r4, #0x68]
007f0c58: ldr      r3, [sp, #8]
007f0c5c: ldr      r0, [r4, #0x6c]
007f0c60: mov      r1, r3
007f0c64: bl       #0x30ed6c
007f0c68: str      r0, [r4, #0x6c]
007f0c6c: movw     r1, #0xd70a
007f0c70: mov      r0, r7
007f0c74: movt     r1, #0x3ba3
007f0c78: bl       #0x30e2f8
007f0c7c: cmp      r0, #0
007f0c80: moveq    r3, #0
007f0c84: streq    r3, [r4, #0x74]
007f0c88: streq    r3, [r4, #0x70]
007f0c8c: beq      #0x7f0cc8
007f0c90: mov      r1, r7
007f0c94: mov      r0, #0x3f800000
007f0c98: bl       #0x30ec94
007f0c9c: mov      r3, r0
007f0ca0: mov      r1, r0
007f0ca4: ldr      r0, [r4, #0x70]
007f0ca8: str      r3, [sp, #8]
007f0cac: bl       #0x30ed6c
007f0cb0: str      r0, [r4, #0x70]
007f0cb4: ldr      r3, [sp, #8]
007f0cb8: ldr      r0, [r4, #0x74]
007f0cbc: mov      r1, r3
007f0cc0: bl       #0x30ed6c
007f0cc4: str      r0, [r4, #0x74]
007f0cc8: mov      r1, r8
007f0ccc: ldr      r0, [r4, #0x78]
007f0cd0: bl       #0x30e3ac
007f0cd4: ldr      r1, [r4, #0x7c]
007f0cd8: mov      r3, r0
007f0cdc: mov      r0, r7
007f0ce0: str      r3, [sp, #8]
007f0ce4: bl       #0x30ed6c
007f0ce8: ldr      r3, [sp, #8]
007f0cec: mov      r1, r0
007f0cf0: mov      r0, r3
007f0cf4: bl       #0x30e3ac
007f0cf8: mov      r1, #0
007f0cfc: bl       #0x30e2f8
007f0d00: cmp      r0, #0
007f0d04: moveq    r3, #2
007f0d08: movne    r3, #0
007f0d0c: streq    r3, [r4, #0xac]
007f0d10: movne    r2, #0
007f0d14: moveq    r3, #0
007f0d18: strne    r3, [r4, #0xac]
007f0d1c: streq    r3, [r4, #0xa0]
007f0d20: strne    r2, [r4, #0x94]
007f0d24: mov      r1, r8
007f0d28: ldr      r0, [r4, #0x80]
007f0d2c: bl       #0x30e2f8
007f0d30: cmp      r0, #0
007f0d34: movne    r3, #0
007f0d38: moveq    r3, #2
007f0d3c: strne    r3, [r4, #0xb0]
007f0d40: streq    r3, [r4, #0xb0]
007f0d44: movne    r3, #0
007f0d48: moveq    r3, #0
007f0d4c: strne    r3, [r4, #0x98]
007f0d50: streq    r3, [r4, #0xa4]
007f0d54: mov      r1, r7
007f0d58: ldr      r0, [r4, #0x84]
007f0d5c: bl       #0x30e2f8
007f0d60: cmp      r0, #0
007f0d64: movne    r3, #0
007f0d68: moveq    r3, #2
007f0d6c: strne    r3, [r4, #0xb4]
007f0d70: streq    r3, [r4, #0xb4]
007f0d74: movne    r3, #0
007f0d78: moveq    r3, #0
007f0d7c: strne    r3, [r4, #0x9c]
007f0d80: streq    r3, [r4, #0xa8]
007f0d84: ldr      r1, [r4, #0x6c]
007f0d88: mov      r0, fp
007f0d8c: bl       #0x30ed6c
007f0d90: ldr      r1, [r4, #0x68]
007f0d94: mov      r7, r0
007f0d98: mov      r0, sb
007f0d9c: bl       #0x30ed6c
007f0da0: mov      r1, r0
007f0da4: mov      r0, r7
007f0da8: bl       #0x30e3ac
007f0dac: ldr      r1, [r4, #0x74]
007f0db0: mov      r7, r0
007f0db4: mov      r0, sl
007f0db8: bl       #0x30ed6c
007f0dbc: ldr      r1, [r4, #0x70]
007f0dc0: mov      r8, r0
007f0dc4: ldr      r0, [sp, #0x14]
007f0dc8: bl       #0x30ed6c
007f0dcc: mov      r1, r0
007f0dd0: mov      r0, r8
007f0dd4: bl       #0x30e3ac
007f0dd8: ldr      r1, [r6, #0x80]
007f0ddc: mov      r8, r0
007f0de0: mov      r0, r7
007f0de4: bl       #0x30ed6c
007f0de8: mov      r1, r7
007f0dec: bl       #0x30ed6c
007f0df0: ldr      r1, [r6, #0x78]
007f0df4: bl       #0x30eba4
007f0df8: str      r0, [r4, #0x8c]
007f0dfc: ldr      r1, [r5, #0x80]
007f0e00: mov      r7, r0
007f0e04: mov      r0, r8
007f0e08: bl       #0x30ed6c
007f0e0c: mov      r1, r8
007f0e10: bl       #0x30ed6c
007f0e14: ldr      r1, [r5, #0x78]
007f0e18: bl       #0x30eba4
007f0e1c: mov      r1, r7
007f0e20: mov      r8, r0
007f0e24: str      r0, [r4, #0x90]
007f0e28: mov      r0, #0x3f800000
007f0e2c: bl       #0x30ec94
007f0e30: mov      r1, r8
007f0e34: str      r0, [r4, #0x8c]
007f0e38: mov      r0, #0x3f800000
007f0e3c: bl       #0x30ec94
007f0e40: ldr      r3, [r4, #0x7c]
007f0e44: str      r0, [r4, #0x90]
007f0e48: mov      r1, r3
007f0e4c: mov      r0, r3
007f0e50: bl       #0x30ed6c
007f0e54: mov      r1, r0
007f0e58: mov      r0, r8
007f0e5c: bl       #0x30ed6c
007f0e60: mov      r1, r0
007f0e64: mov      r0, r7
007f0e68: bl       #0x30eba4
007f0e6c: mov      r1, r0
007f0e70: mov      r0, #0x3f800000
007f0e74: bl       #0x30ec94
007f0e78: str      r0, [r4, #0x88]
007f0e7c: ldr      r2, [sp, #0xc]
007f0e80: ldrb     r3, [r2, #0x10]
007f0e84: cmp      r3, #0
007f0e88: beq      #0x7f1024
007f0e8c: ldr      r3, [r4, #0x94]
007f0e90: ldr      r1, [r4, #0x98]
007f0e94: ldr      r7, [r2]
007f0e98: add      r0, r3, #0x80000000
007f0e9c: str      r3, [sp, #8]
007f0ea0: bl       #0x30e3ac
007f0ea4: mov      r1, r0
007f0ea8: mov      r0, r7
007f0eac: bl       #0x30ed6c
007f0eb0: ldr      r1, [r4, #0x68]
007f0eb4: mov      r8, r0
007f0eb8: bl       #0x30ed6c
007f0ebc: str      r0, [sp, #0xc]
007f0ec0: ldr      r1, [r4, #0x6c]
007f0ec4: mov      r0, r8
007f0ec8: bl       #0x30ed6c
007f0ecc: ldr      r3, [sp, #8]
007f0ed0: ldr      r1, [r4, #0x7c]
007f0ed4: mov      r8, r0
007f0ed8: mov      r0, r3
007f0edc: add      r1, r1, #0x80000000
007f0ee0: bl       #0x30ed6c
007f0ee4: ldr      r1, [r4, #0x9c]
007f0ee8: bl       #0x30e3ac
007f0eec: mov      r1, r0
007f0ef0: mov      r0, r7
007f0ef4: bl       #0x30ed6c
007f0ef8: ldr      r1, [r4, #0x70]
007f0efc: str      r0, [sp, #8]
007f0f00: bl       #0x30ed6c
007f0f04: ldr      r3, [sp, #8]
007f0f08: ldr      r1, [r4, #0x74]
007f0f0c: mov      r7, r0
007f0f10: mov      r0, r3
007f0f14: bl       #0x30ed6c
007f0f18: ldr      r3, [r6, #0x78]
007f0f1c: ldr      r1, [sp, #0xc]
007f0f20: mov      r4, r0
007f0f24: mov      r0, r3
007f0f28: str      r3, [sp, #8]
007f0f2c: bl       #0x30ed6c
007f0f30: mov      r1, r0
007f0f34: ldr      r0, [r6, #0x40]
007f0f38: bl       #0x30eba4
007f0f3c: str      r0, [r6, #0x40]
007f0f40: ldr      r3, [sp, #8]
007f0f44: mov      r1, r8
007f0f48: mov      r0, r3
007f0f4c: bl       #0x30ed6c
007f0f50: mov      r1, r0
007f0f54: ldr      r0, [r6, #0x44]
007f0f58: bl       #0x30eba4
007f0f5c: mov      r1, r8
007f0f60: str      r0, [r6, #0x44]
007f0f64: mov      r0, fp
007f0f68: bl       #0x30ed6c
007f0f6c: ldr      r1, [sp, #0xc]
007f0f70: mov      r8, r0
007f0f74: mov      r0, sb
007f0f78: bl       #0x30ed6c
007f0f7c: mov      r1, r0
007f0f80: mov      r0, r8
007f0f84: bl       #0x30e3ac
007f0f88: ldr      r1, [r6, #0x80]
007f0f8c: bl       #0x30ed6c
007f0f90: mov      r1, r0
007f0f94: ldr      r0, [r6, #0x48]
007f0f98: bl       #0x30eba4
007f0f9c: str      r0, [r6, #0x48]
007f0fa0: ldr      r6, [r5, #0x78]
007f0fa4: mov      r1, r7
007f0fa8: mov      r0, r6
007f0fac: bl       #0x30ed6c
007f0fb0: mov      r1, r0
007f0fb4: ldr      r0, [r5, #0x40]
007f0fb8: bl       #0x30eba4
007f0fbc: mov      r1, r4
007f0fc0: str      r0, [r5, #0x40]
007f0fc4: mov      r0, r6
007f0fc8: bl       #0x30ed6c
007f0fcc: mov      r1, r0
007f0fd0: ldr      r0, [r5, #0x44]
007f0fd4: bl       #0x30eba4
007f0fd8: mov      r1, r4
007f0fdc: str      r0, [r5, #0x44]
007f0fe0: mov      r0, sl
007f0fe4: bl       #0x30ed6c
007f0fe8: mov      r1, r7
007f0fec: mov      r4, r0
007f0ff0: ldr      r0, [sp, #0x14]
007f0ff4: bl       #0x30ed6c
007f0ff8: mov      r1, r0
007f0ffc: mov      r0, r4
007f1000: bl       #0x30e3ac
007f1004: ldr      r1, [r5, #0x80]
007f1008: bl       #0x30ed6c
007f100c: mov      r1, r0
007f1010: ldr      r0, [r5, #0x48]
007f1014: bl       #0x30eba4
007f1018: str      r0, [r5, #0x48]
007f101c: add      sp, sp, #0x2c
007f1020: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007f1024: mov      r3, #0
007f1028: str      r3, [r4, #0x9c]
007f102c: str      r3, [r4, #0x94]
007f1030: str      r3, [r4, #0x98]
007f1034: b        #0x7f101c

# _ZN15b2CircleContactD1Ev
007f3a90: bx       lr

# _ZN16b2BlockAllocatorC1Ev
007e8efc: push     {r4, r5, r6, r7, r8, lr}
007e8f00: mov      r3, #0x80
007e8f04: mov      r6, #0
007e8f08: mov      r5, r0
007e8f0c: str      r3, [r0, #8]
007e8f10: str      r6, [r0, #4]
007e8f14: mov      r0, #0x400
007e8f18: bl       #0x7f34f4
007e8f1c: ldr      r4, [pc, #0xa8]
007e8f20: ldr      r2, [r5, #8]
007e8f24: ldr      r7, [pc, #0xa4]
007e8f28: add      r4, pc, r4
007e8f2c: lsl      r2, r2, #3
007e8f30: str      r0, [r5]
007e8f34: mov      r1, r6
007e8f38: bl       #0x30e460
007e8f3c: ldr      r3, [r4, r7]
007e8f40: str      r6, [r5, #0x40]
007e8f44: str      r6, [r5, #0xc]
007e8f48: str      r6, [r5, #0x10]
007e8f4c: str      r6, [r5, #0x14]
007e8f50: str      r6, [r5, #0x18]
007e8f54: str      r6, [r5, #0x1c]
007e8f58: str      r6, [r5, #0x20]
007e8f5c: str      r6, [r5, #0x24]
007e8f60: str      r6, [r5, #0x28]
007e8f64: str      r6, [r5, #0x2c]
007e8f68: str      r6, [r5, #0x30]
007e8f6c: str      r6, [r5, #0x34]
007e8f70: str      r6, [r5, #0x38]
007e8f74: str      r6, [r5, #0x3c]
007e8f78: ldrb     r2, [r3]
007e8f7c: cmp      r2, r6
007e8f80: bne      #0x7e8fc4
007e8f84: ldr      r3, [pc, #0x48]
007e8f88: ldr      r6, [pc, #0x48]
007e8f8c: movw     r0, #0x281
007e8f90: ldr      ip, [r4, r3]
007e8f94: mov      r3, #1
007e8f98: ldr      r1, [ip, r2, lsl #2]
007e8f9c: cmp      r3, r1
007e8fa0: ldr      r1, [r4, r6]
007e8fa4: addgt    r2, r2, #1
007e8fa8: strb     r2, [r3, r1]
007e8fac: add      r3, r3, #1
007e8fb0: cmp      r3, r0
007e8fb4: bne      #0x7e8f98
007e8fb8: ldr      r3, [r4, r7]
007e8fbc: mov      r2, #1
007e8fc0: strb     r2, [r3]
007e8fc4: mov      r0, r5
007e8fc8: pop      {r4, r5, r6, r7, r8, pc}
007e8fcc: andseq   fp, sl, r8, ror #22
007e8fd0: andeq    r4, r0, r0, lsr #17
007e8fd4: strheq   r2, [r0], -r8
007e8fd8: strdeq   r1, r2, [r0], -ip

# _ZN7b2WorldD2Ev
007e7ffc: push     {r4, r5, r6, lr}
007e8000: mov      r5, #0x19000
007e8004: add      r3, r5, #0x254
007e8008: mov      r4, r0
007e800c: ldr      r1, [r0, r3]
007e8010: add      r6, r5, #0x1d8
007e8014: bl       #0x7e7db0
007e8018: ldr      r0, [r4, r6]
007e801c: bl       #0x7e247c
007e8020: ldr      r0, [r4, r6]
007e8024: ldr      r6, [pc, #0x44]
007e8028: bl       #0x7f34bc
007e802c: ldr      r2, [pc, #0x40]
007e8030: ldr      r3, [pc, #0x40]
007e8034: add      r6, pc, r6
007e8038: ldr      r2, [r6, r2]
007e803c: ldr      r3, [r6, r3]
007e8040: add      r1, r5, #0x1e4
007e8044: add      r2, r2, #8
007e8048: add      r3, r3, #8
007e804c: add      r5, r5, #0x1dc
007e8050: str      r2, [r4, r1]
007e8054: add      r0, r4, #0x44
007e8058: str      r3, [r4, r5]
007e805c: bl       #0x7f3594
007e8060: mov      r0, r4
007e8064: bl       #0x7e8ddc
007e8068: mov      r0, r4
007e806c: pop      {r4, r5, r6, pc}
007e8070: andseq   ip, sl, ip, asr sl
007e8074: andeq    r2, r0, r4, lsr r3
007e8078: muleq    r0, r4, r8

# _ZNK13b2PulleyJoint17GetReactionTorqueEv
007f0470: mov      r0, #0
007f0474: bx       lr

# _ZN7b2Shape7DestroyEPS_P16b2BlockAllocator
007e64f4: push     {r4, r5, r6, lr}
007e64f8: ldr      r3, [r0, #4]
007e64fc: mov      r4, r0
007e6500: mov      r5, r1
007e6504: cmp      r3, #0
007e6508: bne      #0x7e652c
007e650c: ldr      r3, [r0]
007e6510: mov      lr, pc
007e6514: ldr      pc, [r3, #0x14]
007e6518: mov      r0, r5
007e651c: mov      r1, r4
007e6520: mov      r2, #0x3c
007e6524: pop      {r4, r5, r6, lr}
007e6528: b        #0x7e8da0
007e652c: cmp      r3, #1
007e6530: beq      #0x7e6538
007e6534: pop      {r4, r5, r6, pc}
007e6538: ldr      r3, [r0]
007e653c: mov      lr, pc
007e6540: ldr      pc, [r3, #0x14]
007e6544: mov      r0, r5
007e6548: mov      r1, r4
007e654c: mov      r2, #0x11c
007e6550: pop      {r4, r5, r6, lr}
007e6554: b        #0x7e8da0

# _ZN16b2StackAllocatorC1Ev
007f3560: mov      r3, #0x19000
007f3564: mov      r1, #0
007f3568: push     {r4, r5}
007f356c: add      ip, r3, #8
007f3570: add      r5, r3, #0x18c
007f3574: add      r4, r3, #4
007f3578: str      r1, [r0, r5]
007f357c: str      r1, [r0, r4]
007f3580: str      r1, [r0, ip]
007f3584: str      r1, [r0, r3]
007f3588: pop      {r4, r5}
007f358c: bx       lr

# _ZN13b2CircleShapeD0Ev
007e9a60: ldr      r3, [pc, #0x2c]
007e9a64: ldr      r2, [pc, #0x2c]
007e9a68: push     {r4, lr}
007e9a6c: add      r3, pc, r3
007e9a70: ldr      r2, [r3, r2]
007e9a74: mov      r4, r0
007e9a78: add      r2, r2, #8
007e9a7c: str      r2, [r0]
007e9a80: bl       #0x7e6178
007e9a84: mov      r0, r4
007e9a88: bl       #0x30e2b0
007e9a8c: mov      r0, r4
007e9a90: pop      {r4, pc}
007e9a94: andseq   fp, sl, r4, lsr #32
007e9a98: andeq    r0, r0, ip, ror #16

# _ZN15b2CircleContactD0Ev
007f3a9c: push     {r4, lr}
007f3aa0: mov      r4, r0
007f3aa4: bl       #0x30e2b0
007f3aa8: mov      r0, r4
007f3aac: pop      {r4, pc}

# _ZN16b2BlockAllocator5ClearEv
007e8e6c: push     {r4, r5, r6, lr}
007e8e70: ldr      r3, [r0, #4]
007e8e74: mov      r4, r0
007e8e78: cmp      r3, #0
007e8e7c: ble      #0x7e8ea4
007e8e80: mov      r5, #0
007e8e84: ldr      r3, [r4]
007e8e88: add      r3, r3, r5, lsl #3
007e8e8c: ldr      r0, [r3, #4]
007e8e90: bl       #0x7f34bc
007e8e94: ldr      r3, [r4, #4]
007e8e98: add      r5, r5, #1
007e8e9c: cmp      r3, r5
007e8ea0: bgt      #0x7e8e84
007e8ea4: ldr      r2, [r4, #8]
007e8ea8: mov      r5, #0
007e8eac: str      r5, [r4, #4]
007e8eb0: lsl      r2, r2, #3
007e8eb4: ldr      r0, [r4]
007e8eb8: mov      r1, r5
007e8ebc: bl       #0x30e460
007e8ec0: str      r5, [r4, #0x40]
007e8ec4: str      r5, [r4, #0xc]
007e8ec8: str      r5, [r4, #0x10]
007e8ecc: str      r5, [r4, #0x14]
007e8ed0: str      r5, [r4, #0x18]
007e8ed4: str      r5, [r4, #0x1c]
007e8ed8: str      r5, [r4, #0x20]
007e8edc: str      r5, [r4, #0x24]
007e8ee0: str      r5, [r4, #0x28]
007e8ee4: str      r5, [r4, #0x2c]
007e8ee8: str      r5, [r4, #0x30]
007e8eec: str      r5, [r4, #0x34]
007e8ef0: str      r5, [r4, #0x38]
007e8ef4: str      r5, [r4, #0x3c]
007e8ef8: pop      {r4, r5, r6, pc}

# _ZN12b2PolygonDef8SetAsBoxEff
007e44b8: add      ip, r1, #0x80000000
007e44bc: add      r3, r2, #0x80000000
007e44c0: str      r4, [sp, #-4]!
007e44c4: mov      r4, #4
007e44c8: str      r2, [r0, #0x3c]
007e44cc: str      r4, [r0, #0x60]
007e44d0: str      r3, [r0, #0x2c]
007e44d4: str      r1, [r0, #0x30]
007e44d8: str      ip, [r0, #0x38]
007e44dc: str      ip, [r0, #0x20]
007e44e0: str      r3, [r0, #0x24]
007e44e4: str      r1, [r0, #0x28]
007e44e8: str      r2, [r0, #0x34]
007e44ec: ldm      sp!, {r4}
007e44f0: bx       lr

# _ZN7b2World11CreateJointEPK10b2JointDef
007e7c80: push     {r4, r5, r6, r7, r8, lr}
007e7c84: mov      r6, r0
007e7c88: mov      r4, r1
007e7c8c: mov      r0, r1
007e7c90: mov      r1, r6
007e7c94: bl       #0x7eb37c
007e7c98: mov      r2, #0
007e7c9c: mov      r3, #0x19000
007e7ca0: str      r2, [r0, #8]
007e7ca4: add      r3, r3, #0x234
007e7ca8: ldr      r2, [r6, r3]
007e7cac: mov      r5, r0
007e7cb0: mov      r1, #0
007e7cb4: str      r2, [r0, #0xc]
007e7cb8: ldr      r3, [r6, r3]
007e7cbc: mov      r2, #0x19000
007e7cc0: add      r2, r2, #0x234
007e7cc4: cmp      r3, #0
007e7cc8: strne    r0, [r3, #8]
007e7ccc: mov      r3, #0x19000
007e7cd0: str      r0, [r6, r2]
007e7cd4: add      r3, r3, #0x244
007e7cd8: ldr      r2, [r6, r3]
007e7cdc: add      r2, r2, #1
007e7ce0: str      r2, [r6, r3]
007e7ce4: ldr      r2, [r0, #0x34]
007e7ce8: ldr      r3, [r0, #0x30]
007e7cec: str      r1, [r0, #0x18]
007e7cf0: str      r2, [r0, #0x10]
007e7cf4: str      r0, [r5, #0x14]
007e7cf8: ldr      r2, [r3, #0x6c]
007e7cfc: str      r2, [r0, #0x1c]
007e7d00: ldr      r1, [r3, #0x6c]
007e7d04: add      r2, r0, #0x10
007e7d08: cmp      r1, #0
007e7d0c: strne    r2, [r1, #8]
007e7d10: ldrne    r3, [r0, #0x30]
007e7d14: str      r2, [r3, #0x6c]
007e7d18: ldr      r2, [r0, #0x30]
007e7d1c: ldr      r3, [r0, #0x34]
007e7d20: str      r2, [r0, #0x20]
007e7d24: mov      r2, #0
007e7d28: str      r2, [r0, #0x28]
007e7d2c: str      r0, [r5, #0x24]
007e7d30: ldr      r2, [r3, #0x6c]
007e7d34: str      r2, [r0, #0x2c]
007e7d38: ldr      r1, [r3, #0x6c]
007e7d3c: add      r2, r0, #0x20
007e7d40: cmp      r1, #0
007e7d44: strne    r2, [r1, #8]
007e7d48: ldrne    r3, [r0, #0x34]
007e7d4c: str      r2, [r3, #0x6c]
007e7d50: ldrb     r3, [r4, #0x10]
007e7d54: cmp      r3, #0
007e7d58: bne      #0x7e7da8
007e7d5c: ldr      r3, [r4, #0xc]
007e7d60: ldr      r8, [r4, #8]
007e7d64: ldr      r2, [r3, #0x68]
007e7d68: ldr      r1, [r8, #0x68]
007e7d6c: cmp      r1, r2
007e7d70: movge    r8, r3
007e7d74: ldr      r4, [r8, #0x64]
007e7d78: cmp      r4, #0
007e7d7c: beq      #0x7e7da8
007e7d80: mov      r7, #0x19000
007e7d84: add      r8, r8, #4
007e7d88: add      r7, r7, #0x1d8
007e7d8c: mov      r0, r4
007e7d90: ldr      r1, [r6, r7]
007e7d94: mov      r2, r8
007e7d98: bl       #0x7e61b0
007e7d9c: ldr      r4, [r4, #8]
007e7da0: cmp      r4, #0
007e7da4: bne      #0x7e7d8c
007e7da8: mov      r0, r5
007e7dac: pop      {r4, r5, r6, r7, r8, pc}

# _ZN13b2NullContact8EvaluateEP17b2ContactListener
007e65c4: bx       lr

# _ZNK14b2PolygonShape11ComputeAABBEP6b2AABBRK7b2XForm
007e4b54: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e4b58: mov      r5, r2
007e4b5c: ldr      r7, [r2, #8]
007e4b60: ldr      r2, [r2, #0x10]
007e4b64: ldr      r8, [r0, #0x38]
007e4b68: sub      sp, sp, #0x14
007e4b6c: str      r2, [sp, #0xc]
007e4b70: mov      r4, r0
007e4b74: ldr      sl, [r0, #0x3c]
007e4b78: mov      r6, r1
007e4b7c: mov      r0, r7
007e4b80: mov      r1, r8
007e4b84: bl       #0x30ed6c
007e4b88: mov      r1, sl
007e4b8c: mov      sb, r0
007e4b90: ldr      r0, [sp, #0xc]
007e4b94: bl       #0x30ed6c
007e4b98: mov      r1, r0
007e4b9c: mov      r0, sb
007e4ba0: bl       #0x30eba4
007e4ba4: ldr      r3, [r5, #0xc]
007e4ba8: mov      fp, r0
007e4bac: mov      r0, r8
007e4bb0: str      r3, [sp, #8]
007e4bb4: ldr      r2, [r5, #0x14]
007e4bb8: mov      r1, r3
007e4bbc: str      r2, [sp, #4]
007e4bc0: bl       #0x30ed6c
007e4bc4: ldr      r1, [sp, #4]
007e4bc8: mov      r8, r0
007e4bcc: mov      r0, sl
007e4bd0: bl       #0x30ed6c
007e4bd4: mov      r1, r0
007e4bd8: mov      r0, r8
007e4bdc: bl       #0x30eba4
007e4be0: ldr      r3, [r4, #0x40]
007e4be4: mov      sl, r0
007e4be8: mov      r0, r7
007e4bec: mov      r1, r3
007e4bf0: str      r3, [sp]
007e4bf4: bl       #0x30ed6c
007e4bf8: ldr      r1, [r4, #0x44]
007e4bfc: mov      r8, r0
007e4c00: ldr      r0, [sp, #0xc]
007e4c04: bl       #0x30ed6c
007e4c08: mov      r1, r0
007e4c0c: mov      r0, r8
007e4c10: bl       #0x30eba4
007e4c14: ldr      r3, [sp]
007e4c18: mov      sb, r0
007e4c1c: ldr      r0, [sp, #8]
007e4c20: mov      r1, r3
007e4c24: bl       #0x30ed6c
007e4c28: ldr      r1, [r4, #0x44]
007e4c2c: mov      r8, r0
007e4c30: ldr      r0, [sp, #4]
007e4c34: bl       #0x30ed6c
007e4c38: mov      r1, r0
007e4c3c: mov      r0, r8
007e4c40: bl       #0x30eba4
007e4c44: mov      r1, #0
007e4c48: mov      r8, r0
007e4c4c: mov      r0, fp
007e4c50: bl       #0x30e2f8
007e4c54: cmp      r0, #0
007e4c58: moveq    r3, fp
007e4c5c: addeq    r3, r3, #0x80000000
007e4c60: mov      r0, sl
007e4c64: mov      r1, #0
007e4c68: moveq    fp, r3
007e4c6c: bl       #0x30e2f8
007e4c70: mov      r1, #0
007e4c74: cmp      r0, #0
007e4c78: mov      r0, sb
007e4c7c: addeq    sl, sl, #0x80000000
007e4c80: bl       #0x30e2f8
007e4c84: mov      r1, #0
007e4c88: cmp      r0, #0
007e4c8c: mov      r0, r8
007e4c90: addeq    sb, sb, #0x80000000
007e4c94: bl       #0x30e2f8
007e4c98: ldr      r3, [r4, #0x50]
007e4c9c: cmp      r0, #0
007e4ca0: mov      r0, fp
007e4ca4: mov      r1, r3
007e4ca8: addeq    r8, r8, #0x80000000
007e4cac: str      r3, [sp]
007e4cb0: bl       #0x30ed6c
007e4cb4: ldr      r1, [r4, #0x54]
007e4cb8: mov      fp, r0
007e4cbc: mov      r0, sb
007e4cc0: bl       #0x30ed6c
007e4cc4: mov      r1, r0
007e4cc8: mov      r0, fp
007e4ccc: bl       #0x30eba4
007e4cd0: ldr      r3, [sp]
007e4cd4: mov      sb, r0
007e4cd8: mov      r0, sl
007e4cdc: mov      r1, r3
007e4ce0: bl       #0x30ed6c
007e4ce4: ldr      r1, [r4, #0x54]
007e4ce8: mov      sl, r0
007e4cec: mov      r0, r8
007e4cf0: bl       #0x30ed6c
007e4cf4: mov      r1, r0
007e4cf8: mov      r0, sl
007e4cfc: bl       #0x30eba4
007e4d00: ldr      r8, [r4, #0x48]
007e4d04: mov      sl, r0
007e4d08: mov      r0, r7
007e4d0c: mov      r1, r8
007e4d10: bl       #0x30ed6c
007e4d14: ldr      r4, [r4, #0x4c]
007e4d18: mov      r7, r0
007e4d1c: ldr      r0, [sp, #0xc]
007e4d20: mov      r1, r4
007e4d24: bl       #0x30ed6c
007e4d28: mov      r1, r0
007e4d2c: mov      r0, r7
007e4d30: bl       #0x30eba4
007e4d34: mov      r1, r8
007e4d38: mov      r7, r0
007e4d3c: ldr      r0, [sp, #8]
007e4d40: bl       #0x30ed6c
007e4d44: mov      r1, r4
007e4d48: mov      r8, r0
007e4d4c: ldr      r0, [sp, #4]
007e4d50: bl       #0x30ed6c
007e4d54: mov      r1, r0
007e4d58: mov      r0, r8
007e4d5c: bl       #0x30eba4
007e4d60: ldr      r1, [r5]
007e4d64: mov      r8, r0
007e4d68: mov      r0, r7
007e4d6c: bl       #0x30eba4
007e4d70: ldr      r1, [r5, #4]
007e4d74: mov      r4, r0
007e4d78: mov      r0, r8
007e4d7c: bl       #0x30eba4
007e4d80: mov      r1, sl
007e4d84: mov      r5, r0
007e4d88: bl       #0x30e3ac
007e4d8c: mov      r1, sb
007e4d90: str      r0, [r6, #4]
007e4d94: mov      r0, r4
007e4d98: bl       #0x30e3ac
007e4d9c: mov      r1, r5
007e4da0: str      r0, [r6]
007e4da4: mov      r0, sl
007e4da8: bl       #0x30eba4
007e4dac: mov      r1, r4
007e4db0: str      r0, [r6, #0xc]
007e4db4: mov      r0, sb
007e4db8: bl       #0x30eba4
007e4dbc: str      r0, [r6, #8]
007e4dc0: add      sp, sp, #0x14
007e4dc4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN14b2PolygonShapeC2EPK10b2ShapeDef
007e5c34: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e5c38: ldr      r6, [pc, #0x2f8]
007e5c3c: sub      sp, sp, #0x2c
007e5c40: mov      r4, r0
007e5c44: mov      r5, r1
007e5c48: bl       #0x7e6068
007e5c4c: ldr      r3, [pc, #0x2e8]
007e5c50: add      r6, pc, r6
007e5c54: mov      r2, #1
007e5c58: ldr      r3, [r6, r3]
007e5c5c: str      r2, [r4, #4]
007e5c60: add      r3, r3, #8
007e5c64: str      r3, [r4]
007e5c68: ldr      r3, [r5, #0x60]
007e5c6c: cmp      r3, #0
007e5c70: str      r3, [r4, #0x118]
007e5c74: ble      #0x7e5d1c
007e5c78: mov      r2, r5
007e5c7c: mov      r6, r4
007e5c80: mov      r3, r4
007e5c84: mov      r1, #0
007e5c88: ldr      r0, [r2, #0x20]
007e5c8c: add      r1, r1, #1
007e5c90: str      r0, [r3, #0x58]
007e5c94: ldr      r0, [r2, #0x24]
007e5c98: add      r2, r2, #8
007e5c9c: str      r0, [r3, #0x5c]
007e5ca0: ldr      sl, [r4, #0x118]
007e5ca4: add      r3, r3, #8
007e5ca8: cmp      sl, r1
007e5cac: bgt      #0x7e5c88
007e5cb0: cmp      sl, #0
007e5cb4: ble      #0x7e5d1c
007e5cb8: mov      r8, #0
007e5cbc: add      r7, r8, #1
007e5cc0: cmp      r7, sl
007e5cc4: movlt    sl, r7
007e5cc8: movge    sl, #0
007e5ccc: add      sl, sl, #0xb
007e5cd0: ldr      r0, [r4, sl, lsl #3]
007e5cd4: ldr      r1, [r6, #0x58]
007e5cd8: bl       #0x30e3ac
007e5cdc: add      sl, r4, sl, lsl #3
007e5ce0: ldr      r1, [r6, #0x5c]
007e5ce4: mov      sb, r0
007e5ce8: ldr      r0, [sl, #4]
007e5cec: bl       #0x30e3ac
007e5cf0: add      r8, r8, #0x13
007e5cf4: add      sb, sb, #0x80000000
007e5cf8: str      r0, [r6, #0x98]
007e5cfc: str      sb, [r6, #0x9c]
007e5d00: add      r0, r4, r8, lsl #3
007e5d04: bl       #0x7e5524
007e5d08: ldr      sl, [r4, #0x118]
007e5d0c: add      r6, r6, #8
007e5d10: mov      r8, r7
007e5d14: cmp      sl, r7
007e5d18: bgt      #0x7e5cbc
007e5d1c: ldr      r2, [r5, #0x60]
007e5d20: add      r0, sp, #0x20
007e5d24: add      r1, r5, #0x20
007e5d28: bl       #0x7e44f4
007e5d2c: ldr      r3, [sp, #0x24]
007e5d30: ldr      r2, [sp, #0x20]
007e5d34: add      r0, r4, #0x38
007e5d38: str      r3, [r4, #0x34]
007e5d3c: str      r2, [r4, #0x30]
007e5d40: add      r1, r4, #0x58
007e5d44: ldr      r2, [r4, #0x118]
007e5d48: bl       #0x7e55ac
007e5d4c: ldr      r3, [r4, #0x118]
007e5d50: cmp      r3, #0
007e5d54: str      r3, [sp, #0x18]
007e5d58: ble      #0x7e5f2c
007e5d5c: ldr      r3, [r4, #0x30]
007e5d60: mov      r5, r4
007e5d64: mov      r6, #0
007e5d68: str      r3, [sp, #0x10]
007e5d6c: ldr      r3, [r4, #0x34]
007e5d70: mov      r2, r4
007e5d74: str      r3, [sp, #0x14]
007e5d78: ldr      r3, [sp, #0x18]
007e5d7c: sub      r3, r3, #1
007e5d80: str      r3, [sp, #0x1c]
007e5d84: cmp      r6, #0
007e5d88: ldreq    r3, [sp, #0x1c]
007e5d8c: subne    r3, r6, #1
007e5d90: ldr      r0, [r5, #0x58]
007e5d94: add      r3, r3, #0x13
007e5d98: add      ip, r2, r3, lsl #3
007e5d9c: ldr      r1, [sp, #0x10]
007e5da0: ldr      r7, [r2, r3, lsl #3]
007e5da4: ldr      r8, [ip, #4]
007e5da8: str      r2, [sp, #4]
007e5dac: bl       #0x30e3ac
007e5db0: ldr      r1, [sp, #0x14]
007e5db4: mov      sl, r0
007e5db8: ldr      r0, [r5, #0x5c]
007e5dbc: bl       #0x30e3ac
007e5dc0: mov      r1, sl
007e5dc4: mov      fp, r0
007e5dc8: mov      r0, r7
007e5dcc: bl       #0x30ed6c
007e5dd0: mov      r1, fp
007e5dd4: mov      r4, r0
007e5dd8: mov      r0, r8
007e5ddc: bl       #0x30ed6c
007e5de0: mov      r1, r0
007e5de4: mov      r0, r4
007e5de8: bl       #0x30eba4
007e5dec: movw     r1, #0xd70a
007e5df0: movt     r1, #0x3d23
007e5df4: bl       #0x30e3ac
007e5df8: ldr      r4, [r5, #0x98]
007e5dfc: mov      sb, r0
007e5e00: mov      r1, sl
007e5e04: mov      r0, r4
007e5e08: bl       #0x30ed6c
007e5e0c: ldr      sl, [r5, #0x9c]
007e5e10: mov      r3, r0
007e5e14: mov      r1, fp
007e5e18: mov      r0, sl
007e5e1c: str      r3, [sp, #8]
007e5e20: bl       #0x30ed6c
007e5e24: ldr      r3, [sp, #8]
007e5e28: mov      r1, r0
007e5e2c: add      r6, r6, #1
007e5e30: mov      r0, r3
007e5e34: bl       #0x30eba4
007e5e38: movw     r1, #0xd70a
007e5e3c: movt     r1, #0x3d23
007e5e40: bl       #0x30e3ac
007e5e44: mov      r1, r7
007e5e48: mov      fp, r0
007e5e4c: mov      r0, sl
007e5e50: bl       #0x30ed6c
007e5e54: mov      r1, r8
007e5e58: mov      r3, r0
007e5e5c: mov      r0, r4
007e5e60: str      r3, [sp, #8]
007e5e64: bl       #0x30ed6c
007e5e68: ldr      r3, [sp, #8]
007e5e6c: mov      r1, r0
007e5e70: mov      r0, r3
007e5e74: bl       #0x30e3ac
007e5e78: mov      r1, r0
007e5e7c: mov      r0, #0x3f800000
007e5e80: bl       #0x30ec94
007e5e84: mov      r1, sb
007e5e88: str      r0, [sp, #0xc]
007e5e8c: mov      r0, sl
007e5e90: bl       #0x30ed6c
007e5e94: mov      r1, fp
007e5e98: mov      sl, r0
007e5e9c: mov      r0, r8
007e5ea0: bl       #0x30ed6c
007e5ea4: mov      r1, r0
007e5ea8: mov      r0, sl
007e5eac: bl       #0x30e3ac
007e5eb0: mov      r1, r0
007e5eb4: ldr      r0, [sp, #0xc]
007e5eb8: bl       #0x30ed6c
007e5ebc: mov      r1, fp
007e5ec0: mov      r8, r0
007e5ec4: mov      r0, r7
007e5ec8: bl       #0x30ed6c
007e5ecc: mov      r1, sb
007e5ed0: mov      r7, r0
007e5ed4: mov      r0, r4
007e5ed8: bl       #0x30ed6c
007e5edc: mov      r1, r0
007e5ee0: mov      r0, r7
007e5ee4: bl       #0x30e3ac
007e5ee8: mov      r1, r0
007e5eec: ldr      r0, [sp, #0xc]
007e5ef0: bl       #0x30ed6c
007e5ef4: mov      r1, r0
007e5ef8: ldr      r0, [sp, #0x14]
007e5efc: bl       #0x30eba4
007e5f00: mov      r1, r8
007e5f04: str      r0, [r5, #0xdc]
007e5f08: ldr      r0, [sp, #0x10]
007e5f0c: bl       #0x30eba4
007e5f10: ldr      r3, [sp, #0x18]
007e5f14: str      r0, [r5, #0xd8]
007e5f18: ldr      r2, [sp, #4]
007e5f1c: cmp      r6, r3
007e5f20: add      r5, r5, #8
007e5f24: blt      #0x7e5d84
007e5f28: mov      r4, r2
007e5f2c: mov      r0, r4
007e5f30: add      sp, sp, #0x2c
007e5f34: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e5f38: andseq   lr, sl, r0, asr #28
007e5f3c: andeq    r1, r0, r8, lsl #17

# _ZNK13b2CircleShape16ComputeSweptAABBEP6b2AABBRK7b2XFormS4_
007e93fc: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e9400: ldr      r5, [r0, #0x30]
007e9404: ldr      r8, [r0, #0x34]
007e9408: mov      r7, r1
007e940c: mov      r4, r0
007e9410: ldr      r1, [r2, #8]
007e9414: mov      r0, r5
007e9418: mov      sl, r3
007e941c: mov      r6, r2
007e9420: bl       #0x30ed6c
007e9424: ldr      r1, [r6, #0x10]
007e9428: mov      sb, r0
007e942c: mov      r0, r8
007e9430: bl       #0x30ed6c
007e9434: mov      r1, r0
007e9438: mov      r0, sb
007e943c: bl       #0x30eba4
007e9440: ldr      r1, [r6, #0xc]
007e9444: mov      sb, r0
007e9448: mov      r0, r5
007e944c: bl       #0x30ed6c
007e9450: ldr      r1, [r6, #0x14]
007e9454: mov      fp, r0
007e9458: mov      r0, r8
007e945c: bl       #0x30ed6c
007e9460: mov      r1, r0
007e9464: mov      r0, fp
007e9468: bl       #0x30eba4
007e946c: ldr      r1, [r6]
007e9470: mov      fp, r0
007e9474: mov      r0, sb
007e9478: bl       #0x30eba4
007e947c: ldr      r1, [r6, #4]
007e9480: mov      sb, r0
007e9484: mov      r0, fp
007e9488: bl       #0x30eba4
007e948c: ldr      r1, [sl, #8]
007e9490: mov      r6, r0
007e9494: mov      r0, r5
007e9498: bl       #0x30ed6c
007e949c: ldr      r1, [sl, #0x10]
007e94a0: mov      fp, r0
007e94a4: mov      r0, r8
007e94a8: bl       #0x30ed6c
007e94ac: mov      r1, r0
007e94b0: mov      r0, fp
007e94b4: bl       #0x30eba4
007e94b8: ldr      r1, [sl, #0xc]
007e94bc: mov      fp, r0
007e94c0: mov      r0, r5
007e94c4: bl       #0x30ed6c
007e94c8: ldr      r1, [sl, #0x14]
007e94cc: mov      r5, r0
007e94d0: mov      r0, r8
007e94d4: bl       #0x30ed6c
007e94d8: mov      r1, r0
007e94dc: mov      r0, r5
007e94e0: bl       #0x30eba4
007e94e4: ldr      r1, [sl]
007e94e8: mov      r5, r0
007e94ec: mov      r0, fp
007e94f0: bl       #0x30eba4
007e94f4: ldr      r1, [sl, #4]
007e94f8: mov      r8, r0
007e94fc: mov      r0, r5
007e9500: bl       #0x30eba4
007e9504: mov      r1, r8
007e9508: mov      r5, r0
007e950c: mov      r0, sb
007e9510: bl       #0x30e70c
007e9514: mov      r1, r5
007e9518: cmp      r0, #0
007e951c: mov      r0, r6
007e9520: movne    fp, sb
007e9524: moveq    fp, r8
007e9528: bl       #0x30e70c
007e952c: mov      r1, r8
007e9530: cmp      r0, #0
007e9534: mov      r0, sb
007e9538: moveq    sl, r5
007e953c: movne    sl, r6
007e9540: bl       #0x30e2f8
007e9544: mov      r1, r5
007e9548: cmp      r0, #0
007e954c: mov      r0, r6
007e9550: moveq    sb, r8
007e9554: bl       #0x30e2f8
007e9558: cmp      r0, #0
007e955c: moveq    r6, r5
007e9560: ldr      r5, [r4, #0x38]
007e9564: mov      r0, fp
007e9568: mov      r1, r5
007e956c: bl       #0x30e3ac
007e9570: mov      r1, r5
007e9574: str      r0, [r7]
007e9578: mov      r0, sl
007e957c: bl       #0x30e3ac
007e9580: str      r0, [r7, #4]
007e9584: ldr      r4, [r4, #0x38]
007e9588: mov      r1, sb
007e958c: mov      r0, r4
007e9590: bl       #0x30eba4
007e9594: mov      r1, r6
007e9598: str      r0, [r7, #8]
007e959c: mov      r0, r4
007e95a0: bl       #0x30eba4
007e95a4: str      r0, [r7, #0xc]
007e95a8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6b2AABB7IsValidEv
007e32b0: push     {r4, r5, r6, lr}
007e32b4: ldr      r5, [r0]
007e32b8: mov      r4, r0
007e32bc: ldr      r0, [r0, #8]
007e32c0: mov      r1, r5
007e32c4: bl       #0x30e3ac
007e32c8: mov      r1, #0
007e32cc: bl       #0x30e4b4
007e32d0: cmp      r0, #0
007e32d4: ldr      r6, [r4, #0xc]
007e32d8: ldr      r1, [r4, #4]
007e32dc: beq      #0x7e3308
007e32e0: mov      r0, r6
007e32e4: bl       #0x30e3ac
007e32e8: mov      r1, #0
007e32ec: bl       #0x30e4b4
007e32f0: cmp      r0, #0
007e32f4: beq      #0x7e3308
007e32f8: mov      r0, r5
007e32fc: bl       #0x30e640
007e3300: cmp      r0, #0
007e3304: bne      #0x7e3310
007e3308: mov      r0, #0
007e330c: pop      {r4, r5, r6, pc}
007e3310: ldr      r0, [r4, #4]
007e3314: bl       #0x30e640
007e3318: cmp      r0, #0
007e331c: beq      #0x7e3308
007e3320: ldr      r0, [r4, #8]
007e3324: bl       #0x30e640
007e3328: cmp      r0, #0
007e332c: beq      #0x7e3308
007e3330: ldr      r0, [r4, #0xc]
007e3334: bl       #0x30e640
007e3338: subs     r0, r0, #0
007e333c: movne    r0, #1
007e3340: pop      {r4, r5, r6, pc}

# _ZN13b2PulleyJointD1Ev
007f04f8: bx       lr

# _ZN15b2CircleContact6CreateEP7b2ShapeS1_P16b2BlockAllocator
007f3b6c: push     {r4, r5, r6, lr}
007f3b70: mov      r6, r0
007f3b74: mov      r5, r1
007f3b78: mov      r0, r2
007f3b7c: mov      r1, #0x94
007f3b80: bl       #0x7e90bc
007f3b84: mov      r1, r6
007f3b88: mov      r4, r0
007f3b8c: mov      r2, r5
007f3b90: bl       #0x7f3ab0
007f3b94: mov      r0, r4
007f3b98: pop      {r4, r5, r6, pc}

# _ZNK13b2PulleyJoint16GetGroundAnchor2Ev
007f04b4: push     {r4, r5, r6, r7, r8, lr}
007f04b8: ldr      r6, [r1, #0x44]
007f04bc: mov      r4, r0
007f04c0: mov      r5, r1
007f04c4: ldr      r0, [r6, #8]
007f04c8: ldr      r1, [r1, #0x54]
007f04cc: bl       #0x30eba4
007f04d0: ldr      r1, [r5, #0x50]
007f04d4: mov      r7, r0
007f04d8: ldr      r0, [r6, #4]
007f04dc: bl       #0x30eba4
007f04e0: str      r7, [r4, #4]
007f04e4: str      r0, [r4]
007f04e8: mov      r0, r4
007f04ec: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7b2Shape11SynchronizeEP12b2BroadPhaseRK7b2XFormS4_
007e63d0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e63d4: ldrh     ip, [r0, #0x20]
007e63d8: mov      r4, r1
007e63dc: movw     r1, #0xffff
007e63e0: cmp      ip, r1
007e63e4: sub      sp, sp, #0x10
007e63e8: mov      r5, r0
007e63ec: beq      #0x7e64b4
007e63f0: ldr      ip, [r0]
007e63f4: mov      r1, sp
007e63f8: mov      lr, pc
007e63fc: ldr      pc, [ip, #0xc]
007e6400: mov      r3, #0x5d000
007e6404: add      r3, r3, #0x24
007e6408: ldr      r1, [r4, r3]
007e640c: ldr      r0, [sp]
007e6410: bl       #0x30e3ac
007e6414: mov      r3, #0x5d000
007e6418: add      r3, r3, #0x28
007e641c: mov      r7, r0
007e6420: ldr      r1, [r4, r3]
007e6424: ldr      r0, [sp, #4]
007e6428: bl       #0x30e3ac
007e642c: mov      r3, #0x5d000
007e6430: add      r3, r3, #0x1c
007e6434: mov      r8, r0
007e6438: ldr      r1, [sp, #8]
007e643c: ldr      r0, [r4, r3]
007e6440: bl       #0x30e3ac
007e6444: mov      r3, #0x5d000
007e6448: add      r3, r3, #0x20
007e644c: mov      sb, r0
007e6450: ldr      r1, [sp, #0xc]
007e6454: ldr      r0, [r4, r3]
007e6458: bl       #0x30e3ac
007e645c: mov      r1, sb
007e6460: mov      sl, r0
007e6464: mov      r0, r7
007e6468: bl       #0x30e2f8
007e646c: mov      r1, sl
007e6470: cmp      r0, #0
007e6474: mov      r0, r8
007e6478: moveq    r7, sb
007e647c: bl       #0x30e2f8
007e6480: cmp      r0, #0
007e6484: moveq    r8, sl
007e6488: mov      r0, r7
007e648c: mov      r1, r8
007e6490: bl       #0x30e2f8
007e6494: cmp      r0, #0
007e6498: moveq    r7, r8
007e649c: mov      r0, r7
007e64a0: mov      r1, #0
007e64a4: bl       #0x30e70c
007e64a8: cmp      r0, #0
007e64ac: mov      r6, sp
007e64b0: bne      #0x7e64c0
007e64b4: mov      r0, #0
007e64b8: add      sp, sp, #0x10
007e64bc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e64c0: mov      r0, r4
007e64c4: ldrh     r1, [r5, #0x20]
007e64c8: mov      r2, sp
007e64cc: bl       #0x7e3344
007e64d0: mov      r0, #1
007e64d4: b        #0x7e64b8

# _ZN7b2ShapeD0Ev
007e64d8: push     {r4, lr}
007e64dc: mov      r4, r0
007e64e0: bl       #0x7e617c
007e64e4: mov      r0, r4
007e64e8: bl       #0x30e2b0
007e64ec: mov      r0, r4
007e64f0: pop      {r4, pc}

# _ZN9b2ContactD0Ev
007e8090: push     {r4, lr}
007e8094: mov      r4, r0
007e8098: bl       #0x30e2b0
007e809c: mov      r0, r4
007e80a0: pop      {r4, pc}

# _ZN7b2World8SolveTOIERK10b2TimeStep
007e8458: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e845c: mov      r4, #0x19000
007e8460: sub      sp, sp, #0x74
007e8464: str      r0, [sp, #0xc]
007e8468: add      r4, r4, #0x23c
007e846c: ldr      r2, [r0, r4]
007e8470: mov      r3, #0x19000
007e8474: add      r3, r3, #0x264
007e8478: ldr      ip, [r0, r3]
007e847c: str      r1, [sp, #0x18]
007e8480: mov      r1, r2
007e8484: ldr      r2, [sp, #0xc]
007e8488: str      ip, [sp, #4]
007e848c: mov      r3, #0
007e8490: add      r2, r2, #0x44
007e8494: str      r2, [sp, #0x24]
007e8498: ldr      ip, [sp, #0x24]
007e849c: add      r0, sp, #0x2c
007e84a0: mov      r2, #0x20
007e84a4: str      ip, [sp]
007e84a8: str      r0, [sp, #0x1c]
007e84ac: bl       #0x7eb0e0
007e84b0: ldr      r0, [sp, #0xc]
007e84b4: ldr      r1, [r0, r4]
007e84b8: ldr      r0, [sp, #0x24]
007e84bc: lsl      r1, r1, #2
007e84c0: bl       #0x7f3644
007e84c4: ldr      r1, [sp, #0xc]
007e84c8: mov      r3, #0x19000
007e84cc: add      r3, r3, #0x230
007e84d0: ldr      r3, [r1, r3]
007e84d4: mov      r8, r0
007e84d8: cmp      r3, #0
007e84dc: beq      #0x7e8500
007e84e0: mov      r1, #0
007e84e4: ldrh     r2, [r3]
007e84e8: str      r1, [r3, #0x3c]
007e84ec: bic      r2, r2, #4
007e84f0: strh     r2, [r3]
007e84f4: ldr      r3, [r3, #0x60]
007e84f8: cmp      r3, #0
007e84fc: bne      #0x7e84e4
007e8500: ldr      r2, [sp, #0xc]
007e8504: mov      r3, #0x19000
007e8508: add      r3, r3, #0x238
007e850c: ldr      r3, [r2, r3]
007e8510: cmp      r3, #0
007e8514: beq      #0x7e8530
007e8518: ldr      r2, [r3, #4]
007e851c: bic      r2, r2, #0xc
007e8520: str      r2, [r3, #4]
007e8524: ldr      r3, [r3, #0x10]
007e8528: cmp      r3, #0
007e852c: bne      #0x7e8518
007e8530: mov      r3, #0x19000
007e8534: add      r3, r3, #0x25c
007e8538: str      r3, [sp, #0x20]
007e853c: ldr      ip, [sp, #0xc]
007e8540: mov      r3, #0x19000
007e8544: add      r3, r3, #0x238
007e8548: ldr      r4, [ip, r3]
007e854c: cmp      r4, #0
007e8550: beq      #0x7e8a78
007e8554: mov      r5, #0
007e8558: mov      r6, #0x3f800000
007e855c: str      r5, [sp, #0x10]
007e8560: str      r8, [sp, #0x14]
007e8564: b        #0x7e85b0
007e8568: ldr      r5, [r4, #0x44]
007e856c: mov      r1, #0x34000000
007e8570: mov      r0, r5
007e8574: bl       #0x30e2f8
007e8578: cmp      r0, #0
007e857c: movne    r7, #1
007e8580: uxtb     r7, r7
007e8584: cmp      r7, #0
007e8588: beq      #0x7e85a4
007e858c: mov      r1, r6
007e8590: mov      r0, r5
007e8594: bl       #0x30e70c
007e8598: cmp      r0, #0
007e859c: movne    r6, r5
007e85a0: strne    r4, [sp, #0x10]
007e85a4: ldr      r4, [r4, #0x10]
007e85a8: cmp      r4, #0
007e85ac: beq      #0x7e86f4
007e85b0: ldr      r3, [r4, #4]
007e85b4: ands     r7, r3, #3
007e85b8: bne      #0x7e85a4
007e85bc: tst      r3, #8
007e85c0: bne      #0x7e8568
007e85c4: ldr      r5, [r4, #0x34]
007e85c8: ldr      sb, [r4, #0x38]
007e85cc: ldr      r8, [r5, #0xc]
007e85d0: ldr      sl, [sb, #0xc]
007e85d4: ldrsh    r3, [r8, #2]
007e85d8: cmp      r3, #0
007e85dc: beq      #0x7e86d0
007e85e0: ldrh     r3, [r8]
007e85e4: tst      r3, #8
007e85e8: bne      #0x7e86d0
007e85ec: ldr      fp, [r8, #0x3c]
007e85f0: ldr      r7, [sl, #0x3c]
007e85f4: mov      r0, fp
007e85f8: mov      r1, r7
007e85fc: bl       #0x30e70c
007e8600: cmp      r0, #0
007e8604: bne      #0x7e8ad8
007e8608: mov      r1, r7
007e860c: mov      r0, fp
007e8610: bl       #0x30e2f8
007e8614: cmp      r0, #0
007e8618: moveq    r7, fp
007e861c: addeq    r8, r8, #0x1c
007e8620: addeq    sl, sl, #0x1c
007e8624: bne      #0x7e8af8
007e8628: mov      r1, r8
007e862c: mov      r2, sb
007e8630: mov      r3, sl
007e8634: mov      r0, r5
007e8638: bl       #0x7f3728
007e863c: mov      r1, #0
007e8640: mov      r5, r0
007e8644: bl       #0x30e2f8
007e8648: cmp      r0, #0
007e864c: beq      #0x7e86a0
007e8650: mov      r0, r5
007e8654: mov      r1, #0x3f800000
007e8658: bl       #0x30e70c
007e865c: cmp      r0, #0
007e8660: beq      #0x7e86a0
007e8664: mov      r1, r5
007e8668: mov      r0, #0x3f800000
007e866c: bl       #0x30e3ac
007e8670: mov      r1, r7
007e8674: bl       #0x30ed6c
007e8678: mov      r1, r0
007e867c: mov      r0, r5
007e8680: bl       #0x30eba4
007e8684: mov      r1, #0x3f800000
007e8688: mov      r5, r0
007e868c: bl       #0x30e70c
007e8690: cmp      r0, #0
007e8694: moveq    r7, #1
007e8698: moveq    r5, #0x3f800000
007e869c: beq      #0x7e86bc
007e86a0: mov      r0, r5
007e86a4: mov      r1, #0x34000000
007e86a8: bl       #0x30e2f8
007e86ac: cmp      r0, #0
007e86b0: mov      r7, #0
007e86b4: movne    r7, #1
007e86b8: uxtb     r7, r7
007e86bc: ldr      r3, [r4, #4]
007e86c0: str      r5, [r4, #0x44]
007e86c4: orr      r3, r3, #8
007e86c8: str      r3, [r4, #4]
007e86cc: b        #0x7e8584
007e86d0: ldrsh    r3, [sl, #2]
007e86d4: cmp      r3, #0
007e86d8: beq      #0x7e85a4
007e86dc: ldrh     r3, [sl]
007e86e0: tst      r3, #8
007e86e4: beq      #0x7e85ec
007e86e8: ldr      r4, [r4, #0x10]
007e86ec: cmp      r4, #0
007e86f0: bne      #0x7e85b0
007e86f4: ldr      r5, [sp, #0x10]
007e86f8: ldr      r8, [sp, #0x14]
007e86fc: cmp      r5, #0
007e8700: beq      #0x7e8a78
007e8704: mov      r1, #0x3f800000
007e8708: mov      r0, r6
007e870c: sub      r1, r1, #0xc8
007e8710: bl       #0x30e2f8
007e8714: cmp      r0, #0
007e8718: bne      #0x7e8a78
007e871c: ldr      r2, [r5, #0x34]
007e8720: ldr      r3, [r5, #0x38]
007e8724: mov      r1, r6
007e8728: ldr      r7, [r2, #0xc]
007e872c: ldr      sl, [r3, #0xc]
007e8730: add      r0, r7, #0x1c
007e8734: bl       #0x7e3a94
007e8738: ldr      r2, [r7, #0x28]
007e873c: ldr      r3, [r7, #0x34]
007e8740: ldr      r1, [r7, #0x24]
007e8744: str      r2, [r7, #0x30]
007e8748: str      r3, [r7, #0x38]
007e874c: str      r1, [r7, #0x2c]
007e8750: mov      r0, r7
007e8754: bl       #0x7e761c
007e8758: mov      r1, r6
007e875c: add      r0, sl, #0x1c
007e8760: bl       #0x7e3a94
007e8764: ldr      r2, [sl, #0x28]
007e8768: ldr      r1, [sl, #0x24]
007e876c: ldr      r3, [sl, #0x34]
007e8770: str      r2, [sl, #0x30]
007e8774: str      r1, [sl, #0x2c]
007e8778: str      r3, [sl, #0x38]
007e877c: mov      r0, sl
007e8780: bl       #0x7e761c
007e8784: ldr      r0, [sp, #0xc]
007e8788: mov      r3, #0x19000
007e878c: add      r3, r3, #0x264
007e8790: ldr      r1, [r0, r3]
007e8794: mov      r0, r5
007e8798: bl       #0x7e9d24
007e879c: ldr      r3, [r5, #4]
007e87a0: ldr      r2, [r5, #8]
007e87a4: bic      r3, r3, #8
007e87a8: cmp      r2, #0
007e87ac: str      r3, [r5, #4]
007e87b0: beq      #0x7e853c
007e87b4: ldrsh    r3, [r7, #2]
007e87b8: str      r4, [sp, #0x44]
007e87bc: str      r4, [sp, #0x40]
007e87c0: cmp      r3, #0
007e87c4: moveq    r7, sl
007e87c8: str      r4, [sp, #0x48]
007e87cc: str      r7, [r8]
007e87d0: ldrh     r3, [r7]
007e87d4: mov      r1, #1
007e87d8: orr      r3, r3, #4
007e87dc: strh     r3, [r7]
007e87e0: ldr      r2, [sp, #0x40]
007e87e4: sub      r7, r1, #1
007e87e8: ldr      r3, [r8, r7, lsl #2]
007e87ec: ldr      ip, [sp, #0x34]
007e87f0: add      r0, r2, #1
007e87f4: str      r3, [ip, r2, lsl #2]
007e87f8: str      r0, [sp, #0x40]
007e87fc: ldrh     r0, [r3]
007e8800: ldrsh    r2, [r3, #2]
007e8804: bic      r0, r0, #8
007e8808: cmp      r2, #0
007e880c: strh     r0, [r3]
007e8810: addeq    ip, r8, r1, lsl #2
007e8814: beq      #0x7e88bc
007e8818: ldr      r4, [r3, #0x70]
007e881c: cmp      r4, #0
007e8820: beq      #0x7e88ac
007e8824: ldr      r3, [sp, #0x48]
007e8828: ldr      r2, [sp, #0x50]
007e882c: cmp      r3, r2
007e8830: beq      #0x7e88a0
007e8834: ldr      r2, [r4, #4]
007e8838: ldr      r1, [r2, #4]
007e883c: tst      r1, #7
007e8840: bne      #0x7e88a0
007e8844: ldr      r1, [r2, #8]
007e8848: add      r0, r3, #1
007e884c: cmp      r1, #0
007e8850: beq      #0x7e88a0
007e8854: ldr      r1, [sp, #0x38]
007e8858: str      r2, [r1, r3, lsl #2]
007e885c: str      r0, [sp, #0x48]
007e8860: ldr      r3, [r4, #4]
007e8864: ldr      r2, [r3, #4]
007e8868: orr      r2, r2, #4
007e886c: str      r2, [r3, #4]
007e8870: ldr      r5, [r4]
007e8874: ldrh     r3, [r5]
007e8878: tst      r3, #4
007e887c: bne      #0x7e88a0
007e8880: ldrsh    r3, [r5, #2]
007e8884: cmp      r3, #0
007e8888: bne      #0x7e8a94
007e888c: str      r5, [r8, r7, lsl #2]
007e8890: ldrh     r3, [r5]
007e8894: add      r7, r7, #1
007e8898: orr      r3, r3, #4
007e889c: strh     r3, [r5]
007e88a0: ldr      r4, [r4, #0xc]
007e88a4: cmp      r4, #0
007e88a8: bne      #0x7e8824
007e88ac: cmp      r7, #0
007e88b0: beq      #0x7e890c
007e88b4: mov      r1, r7
007e88b8: b        #0x7e87e0
007e88bc: cmp      r7, #0
007e88c0: add      r3, ip, r2
007e88c4: beq      #0x7e890c
007e88c8: ldr      r1, [sp, #0x40]
007e88cc: ldr      r3, [r3, #-8]
007e88d0: ldr      lr, [sp, #0x34]
007e88d4: add      r0, r1, #1
007e88d8: sub      r7, r7, #1
007e88dc: str      r3, [lr, r1, lsl #2]
007e88e0: str      r0, [sp, #0x40]
007e88e4: ldrh     r1, [r3]
007e88e8: ldrsh    r0, [r3, #2]
007e88ec: sub      r2, r2, #4
007e88f0: bic      r1, r1, #8
007e88f4: cmp      r0, #0
007e88f8: strh     r1, [r3]
007e88fc: bne      #0x7e8818
007e8900: cmp      r7, #0
007e8904: add      r3, ip, r2
007e8908: bne      #0x7e88c8
007e890c: mov      r1, r6
007e8910: mov      r0, #0x3f800000
007e8914: bl       #0x30e3ac
007e8918: ldr      r2, [sp, #0x18]
007e891c: ldr      r1, [r2]
007e8920: bl       #0x30ed6c
007e8924: mov      r3, r0
007e8928: mov      r1, r0
007e892c: mov      r0, #0x3f800000
007e8930: str      r3, [sp, #0x5c]
007e8934: bl       #0x30ec94
007e8938: ldr      ip, [sp, #0x18]
007e893c: add      r1, sp, #0x5c
007e8940: ldr      r3, [ip, #0xc]
007e8944: str      r0, [sp, #0x60]
007e8948: ldr      r0, [sp, #0x1c]
007e894c: str      r3, [sp, #0x68]
007e8950: bl       #0x7ea878
007e8954: ldr      r3, [sp, #0x40]
007e8958: cmp      r3, #0
007e895c: ble      #0x7e8a18
007e8960: mov      r4, #0
007e8964: b        #0x7e8978
007e8968: ldr      r3, [sp, #0x40]
007e896c: add      r4, r4, #1
007e8970: cmp      r3, r4
007e8974: ble      #0x7e8a18
007e8978: ldr      r3, [sp, #0x34]
007e897c: ldr      r5, [r3, r4, lsl #2]
007e8980: ldrh     r2, [r5]
007e8984: bic      r3, r2, #4
007e8988: lsl      r3, r3, #0x10
007e898c: tst      r2, #0xa
007e8990: lsr      r3, r3, #0x10
007e8994: strh     r3, [r5]
007e8998: bne      #0x7e8968
007e899c: ldrsh    r3, [r5, #2]
007e89a0: cmp      r3, #0
007e89a4: beq      #0x7e8968
007e89a8: mov      r0, r5
007e89ac: bl       #0x7e20ac
007e89b0: cmp      r0, #0
007e89b4: bne      #0x7e89e0
007e89b8: ldr      r1, [sp, #0xc]
007e89bc: ldr      r0, [sp, #0x20]
007e89c0: ldr      r3, [r1, r0]
007e89c4: cmp      r3, #0
007e89c8: beq      #0x7e89e0
007e89cc: mov      r0, r3
007e89d0: mov      r1, r5
007e89d4: ldr      r3, [r3]
007e89d8: mov      lr, pc
007e89dc: ldr      pc, [r3, #8]
007e89e0: ldr      r3, [r5, #0x70]
007e89e4: cmp      r3, #0
007e89e8: beq      #0x7e8968
007e89ec: ldr      r2, [r3, #4]
007e89f0: ldr      r1, [r2, #4]
007e89f4: bic      r1, r1, #8
007e89f8: str      r1, [r2, #4]
007e89fc: ldr      r3, [r3, #0xc]
007e8a00: cmp      r3, #0
007e8a04: bne      #0x7e89ec
007e8a08: ldr      r3, [sp, #0x40]
007e8a0c: add      r4, r4, #1
007e8a10: cmp      r3, r4
007e8a14: bgt      #0x7e8978
007e8a18: ldr      r3, [sp, #0x48]
007e8a1c: cmp      r3, #0
007e8a20: ble      #0x7e8a4c
007e8a24: mov      r3, #0
007e8a28: ldr      r2, [sp, #0x38]
007e8a2c: ldr      r2, [r2, r3, lsl #2]
007e8a30: add      r3, r3, #1
007e8a34: ldr      r1, [r2, #4]
007e8a38: bic      r1, r1, #0xc
007e8a3c: str      r1, [r2, #4]
007e8a40: ldr      r2, [sp, #0x48]
007e8a44: cmp      r2, r3
007e8a48: bgt      #0x7e8a28
007e8a4c: ldr      r2, [sp, #0xc]
007e8a50: mov      r3, #0x19000
007e8a54: add      r3, r3, #0x1d8
007e8a58: ldr      r0, [r2, r3]
007e8a5c: bl       #0x7e2ac8
007e8a60: ldr      ip, [sp, #0xc]
007e8a64: mov      r3, #0x19000
007e8a68: add      r3, r3, #0x238
007e8a6c: ldr      r4, [ip, r3]
007e8a70: cmp      r4, #0
007e8a74: bne      #0x7e8554
007e8a78: ldr      r0, [sp, #0x24]
007e8a7c: mov      r1, r8
007e8a80: bl       #0x7f35a8
007e8a84: ldr      r0, [sp, #0x1c]
007e8a88: bl       #0x7eb078
007e8a8c: add      sp, sp, #0x74
007e8a90: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e8a94: add      r0, r5, #0x1c
007e8a98: mov      r1, r6
007e8a9c: bl       #0x7e3a94
007e8aa0: ldr      r1, [r5, #0x24]
007e8aa4: ldr      r3, [r5, #0x34]
007e8aa8: ldr      r2, [r5, #0x28]
007e8aac: str      r1, [r5, #0x2c]
007e8ab0: str      r3, [r5, #0x38]
007e8ab4: str      r2, [r5, #0x30]
007e8ab8: mov      r0, r5
007e8abc: bl       #0x7e761c
007e8ac0: ldrh     r3, [r5]
007e8ac4: mov      r1, #0
007e8ac8: str      r1, [r5, #0x8c]
007e8acc: bic      r3, r3, #8
007e8ad0: strh     r3, [r5]
007e8ad4: b        #0x7e888c
007e8ad8: add      r8, r8, #0x1c
007e8adc: mov      r0, r8
007e8ae0: mov      r1, r7
007e8ae4: bl       #0x7e3a94
007e8ae8: add      sl, sl, #0x1c
007e8aec: ldr      r5, [r4, #0x34]
007e8af0: ldr      sb, [r4, #0x38]
007e8af4: b        #0x7e8628
007e8af8: add      sl, sl, #0x1c
007e8afc: mov      r0, sl
007e8b00: mov      r1, fp
007e8b04: bl       #0x7e3a94
007e8b08: mov      r7, fp
007e8b0c: ldr      r5, [r4, #0x34]
007e8b10: ldr      sb, [r4, #0x38]
007e8b14: add      r8, r8, #0x1c
007e8b18: b        #0x7e8628

# _ZNK12b2MouseJoint10GetAnchor2Ev
007eb78c: push     {r4, r5, r6, r7, r8, lr}
007eb790: ldr      r4, [r1, #0x34]
007eb794: ldr      r7, [r1, #0x44]
007eb798: ldr      r6, [r1, #0x48]
007eb79c: mov      r5, r0
007eb7a0: ldr      r1, [r4, #0xc]
007eb7a4: mov      r0, r7
007eb7a8: bl       #0x30ed6c
007eb7ac: ldr      r1, [r4, #0x14]
007eb7b0: mov      r8, r0
007eb7b4: mov      r0, r6
007eb7b8: bl       #0x30ed6c
007eb7bc: mov      r1, r0
007eb7c0: mov      r0, r8
007eb7c4: bl       #0x30eba4
007eb7c8: ldr      r1, [r4, #0x10]
007eb7cc: mov      r8, r0
007eb7d0: mov      r0, r7
007eb7d4: bl       #0x30ed6c
007eb7d8: ldr      r1, [r4, #0x18]
007eb7dc: mov      r7, r0
007eb7e0: mov      r0, r6
007eb7e4: bl       #0x30ed6c
007eb7e8: mov      r1, r0
007eb7ec: mov      r0, r7
007eb7f0: bl       #0x30eba4
007eb7f4: ldr      r1, [r4, #8]
007eb7f8: bl       #0x30eba4
007eb7fc: ldr      r1, [r4, #4]
007eb800: mov      r6, r0
007eb804: mov      r0, r8
007eb808: bl       #0x30eba4
007eb80c: str      r6, [r5, #4]
007eb810: str      r0, [r5]
007eb814: mov      r0, r5
007eb818: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7b2JointD1Ev
007eb1d8: bx       lr

# _ZN9b2ContactC1EP7b2ShapeS1_
007e9de0: ldr      r3, [pc, #0xc8]
007e9de4: ldr      ip, [pc, #0xc8]
007e9de8: push     {r4, r5, r6, lr}
007e9dec: add      r3, pc, r3
007e9df0: ldr      ip, [r3, ip]
007e9df4: mov      r4, r0
007e9df8: mov      r0, #0
007e9dfc: add      ip, ip, #8
007e9e00: str      r0, [r4, #4]
007e9e04: str      ip, [r4]
007e9e08: mov      r0, r1
007e9e0c: ldrb     r1, [r1, #0x28]
007e9e10: cmp      r1, #0
007e9e14: beq      #0x7e9ea0
007e9e18: mov      r3, #1
007e9e1c: str      r3, [r4, #4]
007e9e20: mov      r3, #0
007e9e24: str      r3, [r4, #8]
007e9e28: str      r2, [r4, #0x38]
007e9e2c: str      r0, [r4, #0x34]
007e9e30: ldr      r1, [r2, #0x18]
007e9e34: ldr      r0, [r0, #0x18]
007e9e38: bl       #0x30ed6c
007e9e3c: bl       #0x30e124
007e9e40: ldr      r3, [r4, #0x38]
007e9e44: ldr      r2, [r4, #0x34]
007e9e48: str      r0, [r4, #0x3c]
007e9e4c: ldr      r5, [r3, #0x1c]
007e9e50: ldr      r6, [r2, #0x1c]
007e9e54: mov      r1, r5
007e9e58: mov      r0, r6
007e9e5c: bl       #0x30e2f8
007e9e60: cmp      r0, #0
007e9e64: mov      r3, #0
007e9e68: movne    r5, r6
007e9e6c: str      r5, [r4, #0x40]
007e9e70: str      r3, [r4, #0x24]
007e9e74: str      r3, [r4, #0xc]
007e9e78: str      r3, [r4, #0x10]
007e9e7c: str      r3, [r4, #0x18]
007e9e80: str      r3, [r4, #0x1c]
007e9e84: str      r3, [r4, #0x20]
007e9e88: str      r3, [r4, #0x14]
007e9e8c: str      r3, [r4, #0x28]
007e9e90: str      r3, [r4, #0x2c]
007e9e94: str      r3, [r4, #0x30]
007e9e98: mov      r0, r4
007e9e9c: pop      {r4, r5, r6, pc}
007e9ea0: ldrb     r3, [r2, #0x28]
007e9ea4: cmp      r3, #0
007e9ea8: beq      #0x7e9e20
007e9eac: b        #0x7e9e18
007e9eb0: andseq   sl, sl, r4, lsr #25
007e9eb4: andeq    r2, r0, r4, lsr r3

# _ZN13b2PulleyJoint24SolveVelocityConstraintsERK10b2TimeStep
007efc10: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007efc14: ldr      r6, [r0, #0x30]
007efc18: sub      sp, sp, #0x24
007efc1c: mov      r4, r0
007efc20: mov      sl, r1
007efc24: ldr      r0, [r0, #0x58]
007efc28: ldr      r1, [r6, #0x1c]
007efc2c: bl       #0x30e3ac
007efc30: ldr      r1, [r6, #0x20]
007efc34: mov      r8, r0
007efc38: ldr      r0, [r4, #0x5c]
007efc3c: bl       #0x30e3ac
007efc40: ldr      r1, [r6, #0xc]
007efc44: mov      r7, r0
007efc48: mov      r0, r8
007efc4c: bl       #0x30ed6c
007efc50: ldr      r1, [r6, #0x14]
007efc54: mov      r5, r0
007efc58: mov      r0, r7
007efc5c: bl       #0x30ed6c
007efc60: mov      r1, r0
007efc64: mov      r0, r5
007efc68: bl       #0x30eba4
007efc6c: ldr      r1, [r6, #0x10]
007efc70: mov      fp, r0
007efc74: mov      r0, r8
007efc78: bl       #0x30ed6c
007efc7c: ldr      r1, [r6, #0x18]
007efc80: mov      r8, r0
007efc84: mov      r0, r7
007efc88: bl       #0x30ed6c
007efc8c: mov      r1, r0
007efc90: mov      r0, r8
007efc94: bl       #0x30eba4
007efc98: ldr      r5, [r4, #0x34]
007efc9c: str      r0, [sp, #0x14]
007efca0: ldr      r0, [r4, #0x60]
007efca4: ldr      r1, [r5, #0x1c]
007efca8: bl       #0x30e3ac
007efcac: ldr      r1, [r5, #0x20]
007efcb0: mov      r8, r0
007efcb4: ldr      r0, [r4, #0x64]
007efcb8: bl       #0x30e3ac
007efcbc: ldr      r1, [r5, #0xc]
007efcc0: mov      r7, r0
007efcc4: mov      r0, r8
007efcc8: bl       #0x30ed6c
007efccc: ldr      r1, [r5, #0x14]
007efcd0: mov      sb, r0
007efcd4: mov      r0, r7
007efcd8: bl       #0x30ed6c
007efcdc: mov      r1, r0
007efce0: mov      r0, sb
007efce4: bl       #0x30eba4
007efce8: ldr      r1, [r5, #0x10]
007efcec: mov      sb, r0
007efcf0: mov      r0, r8
007efcf4: bl       #0x30ed6c
007efcf8: ldr      r1, [r5, #0x18]
007efcfc: mov      r8, r0
007efd00: mov      r0, r7
007efd04: bl       #0x30ed6c
007efd08: mov      r1, r0
007efd0c: mov      r0, r8
007efd10: bl       #0x30eba4
007efd14: ldr      r3, [r4, #0xac]
007efd18: mov      r7, r0
007efd1c: cmp      r3, #2
007efd20: beq      #0x7f002c
007efd24: ldr      r3, [r4, #0xb0]
007efd28: cmp      r3, #2
007efd2c: beq      #0x7efea8
007efd30: ldr      r3, [r4, #0xb4]
007efd34: cmp      r3, #2
007efd38: bne      #0x7efea0
007efd3c: ldr      r6, [r5, #0x48]
007efd40: mov      r0, r7
007efd44: add      r1, r6, #0x80000000
007efd48: bl       #0x30ed6c
007efd4c: mov      r1, sb
007efd50: mov      r8, r0
007efd54: mov      r0, r6
007efd58: bl       #0x30ed6c
007efd5c: mov      fp, r0
007efd60: ldr      r0, [sl, #4]
007efd64: ldr      r1, [r4, #0x90]
007efd68: ldr      r6, [r4, #0x9c]
007efd6c: add      r0, r0, #0x80000000
007efd70: bl       #0x30ed6c
007efd74: ldr      r1, [r5, #0x40]
007efd78: mov      r3, r0
007efd7c: mov      r0, r8
007efd80: str      r3, [sp, #4]
007efd84: bl       #0x30eba4
007efd88: ldr      r1, [r4, #0x70]
007efd8c: bl       #0x30ed6c
007efd90: ldr      r1, [r5, #0x44]
007efd94: mov      r8, r0
007efd98: mov      r0, fp
007efd9c: bl       #0x30eba4
007efda0: ldr      r1, [r4, #0x74]
007efda4: bl       #0x30ed6c
007efda8: mov      r1, r0
007efdac: mov      r0, r8
007efdb0: bl       #0x30eba4
007efdb4: ldr      r3, [sp, #4]
007efdb8: add      r1, r0, #0x80000000
007efdbc: mov      r0, r3
007efdc0: bl       #0x30ed6c
007efdc4: mov      r1, r6
007efdc8: bl       #0x30eba4
007efdcc: mov      r1, #0
007efdd0: mov      r8, r0
007efdd4: bl       #0x30e70c
007efdd8: cmp      r0, #0
007efddc: movne    r8, #0
007efde0: str      r8, [r4, #0x9c]
007efde4: ldr      r3, [sl]
007efde8: mov      r1, r6
007efdec: mov      r0, r8
007efdf0: add      r6, r3, #0x80000000
007efdf4: bl       #0x30e3ac
007efdf8: mov      r1, r0
007efdfc: mov      r0, r6
007efe00: bl       #0x30ed6c
007efe04: ldr      r1, [r4, #0x70]
007efe08: mov      r8, r0
007efe0c: bl       #0x30ed6c
007efe10: ldr      r1, [r4, #0x74]
007efe14: mov      r6, r0
007efe18: mov      r0, r8
007efe1c: bl       #0x30ed6c
007efe20: ldr      r8, [r5, #0x78]
007efe24: mov      r4, r0
007efe28: mov      r1, r6
007efe2c: mov      r0, r8
007efe30: bl       #0x30ed6c
007efe34: mov      r1, r0
007efe38: ldr      r0, [r5, #0x40]
007efe3c: bl       #0x30eba4
007efe40: mov      r1, r4
007efe44: str      r0, [r5, #0x40]
007efe48: mov      r0, r8
007efe4c: bl       #0x30ed6c
007efe50: mov      r1, r0
007efe54: ldr      r0, [r5, #0x44]
007efe58: bl       #0x30eba4
007efe5c: mov      r1, r4
007efe60: str      r0, [r5, #0x44]
007efe64: mov      r0, sb
007efe68: bl       #0x30ed6c
007efe6c: mov      r1, r6
007efe70: mov      r4, r0
007efe74: mov      r0, r7
007efe78: bl       #0x30ed6c
007efe7c: mov      r1, r0
007efe80: mov      r0, r4
007efe84: bl       #0x30e3ac
007efe88: ldr      r1, [r5, #0x80]
007efe8c: bl       #0x30ed6c
007efe90: mov      r1, r0
007efe94: ldr      r0, [r5, #0x48]
007efe98: bl       #0x30eba4
007efe9c: str      r0, [r5, #0x48]
007efea0: add      sp, sp, #0x24
007efea4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007efea8: ldr      r8, [r6, #0x48]
007efeac: ldr      r0, [sp, #0x14]
007efeb0: add      r1, r8, #0x80000000
007efeb4: bl       #0x30ed6c
007efeb8: mov      r1, fp
007efebc: mov      r3, r0
007efec0: mov      r0, r8
007efec4: str      r3, [sp, #4]
007efec8: bl       #0x30ed6c
007efecc: mov      r2, r0
007efed0: ldr      r0, [sl, #4]
007efed4: ldr      ip, [r4, #0x98]
007efed8: ldr      r1, [r4, #0x8c]
007efedc: add      r0, r0, #0x80000000
007efee0: str      r2, [sp, #0xc]
007efee4: str      ip, [sp, #0x10]
007efee8: bl       #0x30ed6c
007efeec: ldr      r3, [sp, #4]
007efef0: mov      ip, r0
007efef4: ldr      r1, [r6, #0x40]
007efef8: mov      r0, r3
007efefc: str      ip, [sp, #8]
007eff00: bl       #0x30eba4
007eff04: ldr      r1, [r4, #0x68]
007eff08: bl       #0x30ed6c
007eff0c: ldr      r2, [sp, #0xc]
007eff10: ldr      r1, [r6, #0x44]
007eff14: mov      r8, r0
007eff18: mov      r0, r2
007eff1c: bl       #0x30eba4
007eff20: ldr      r1, [r4, #0x6c]
007eff24: bl       #0x30ed6c
007eff28: mov      r1, r0
007eff2c: mov      r0, r8
007eff30: bl       #0x30eba4
007eff34: ldr      ip, [sp, #8]
007eff38: add      r1, r0, #0x80000000
007eff3c: mov      r0, ip
007eff40: bl       #0x30ed6c
007eff44: ldr      r1, [sp, #0x10]
007eff48: bl       #0x30eba4
007eff4c: mov      r1, #0
007eff50: mov      r8, r0
007eff54: bl       #0x30e70c
007eff58: cmp      r0, #0
007eff5c: movne    r8, #0
007eff60: str      r8, [r4, #0x98]
007eff64: ldr      r3, [sl]
007eff68: ldr      r1, [sp, #0x10]
007eff6c: mov      r0, r8
007eff70: add      r8, r3, #0x80000000
007eff74: bl       #0x30e3ac
007eff78: mov      r1, r0
007eff7c: mov      r0, r8
007eff80: bl       #0x30ed6c
007eff84: ldr      r1, [r4, #0x68]
007eff88: mov      r8, r0
007eff8c: bl       #0x30ed6c
007eff90: str      r0, [sp, #0x10]
007eff94: ldr      r1, [r4, #0x6c]
007eff98: mov      r0, r8
007eff9c: bl       #0x30ed6c
007effa0: ldr      r3, [r6, #0x78]
007effa4: mov      r8, r0
007effa8: ldr      r1, [sp, #0x10]
007effac: mov      r0, r3
007effb0: str      r3, [sp, #4]
007effb4: bl       #0x30ed6c
007effb8: mov      r1, r0
007effbc: ldr      r0, [r6, #0x40]
007effc0: bl       #0x30eba4
007effc4: str      r0, [r6, #0x40]
007effc8: ldr      r3, [sp, #4]
007effcc: mov      r1, r8
007effd0: mov      r0, r3
007effd4: bl       #0x30ed6c
007effd8: mov      r1, r0
007effdc: ldr      r0, [r6, #0x44]
007effe0: bl       #0x30eba4
007effe4: mov      r1, r8
007effe8: str      r0, [r6, #0x44]
007effec: mov      r0, fp
007efff0: bl       #0x30ed6c
007efff4: ldr      r1, [sp, #0x10]
007efff8: mov      r8, r0
007efffc: ldr      r0, [sp, #0x14]
007f0000: bl       #0x30ed6c
007f0004: mov      r1, r0
007f0008: mov      r0, r8
007f000c: bl       #0x30e3ac
007f0010: ldr      r1, [r6, #0x80]
007f0014: bl       #0x30ed6c
007f0018: mov      r1, r0
007f001c: ldr      r0, [r6, #0x48]
007f0020: bl       #0x30eba4
007f0024: str      r0, [r6, #0x48]
007f0028: b        #0x7efd30
007f002c: ldr      r8, [r6, #0x48]
007f0030: ldr      r0, [sp, #0x14]
007f0034: add      r1, r8, #0x80000000
007f0038: bl       #0x30ed6c
007f003c: mov      r1, fp
007f0040: mov      r3, r0
007f0044: mov      r0, r8
007f0048: str      r3, [sp, #4]
007f004c: bl       #0x30ed6c
007f0050: ldr      r3, [sp, #4]
007f0054: ldr      r1, [r6, #0x40]
007f0058: mov      r8, r0
007f005c: mov      r0, r3
007f0060: bl       #0x30eba4
007f0064: ldr      r1, [r6, #0x44]
007f0068: mov      r3, r0
007f006c: mov      r0, r8
007f0070: str      r3, [sp, #4]
007f0074: bl       #0x30eba4
007f0078: ldr      r8, [r5, #0x48]
007f007c: mov      r2, r0
007f0080: mov      r0, r7
007f0084: add      r1, r8, #0x80000000
007f0088: str      r2, [sp, #0xc]
007f008c: bl       #0x30ed6c
007f0090: mov      r1, sb
007f0094: mov      ip, r0
007f0098: mov      r0, r8
007f009c: str      ip, [sp, #8]
007f00a0: bl       #0x30ed6c
007f00a4: str      r0, [sp, #0x18]
007f00a8: ldr      r0, [sl, #4]
007f00ac: ldr      lr, [r4, #0x94]
007f00b0: ldr      r1, [r4, #0x88]
007f00b4: add      r0, r0, #0x80000000
007f00b8: str      lr, [sp, #0x10]
007f00bc: bl       #0x30ed6c
007f00c0: ldr      r3, [sp, #4]
007f00c4: str      r0, [sp, #0x1c]
007f00c8: ldr      r1, [r4, #0x68]
007f00cc: mov      r0, r3
007f00d0: bl       #0x30ed6c
007f00d4: ldr      r2, [sp, #0xc]
007f00d8: ldr      r1, [r4, #0x6c]
007f00dc: mov      r8, r0
007f00e0: mov      r0, r2
007f00e4: bl       #0x30ed6c
007f00e8: mov      r1, r0
007f00ec: mov      r0, r8
007f00f0: bl       #0x30eba4
007f00f4: ldr      ip, [sp, #8]
007f00f8: add      r3, r0, #0x80000000
007f00fc: ldr      r1, [r5, #0x40]
007f0100: mov      r0, ip
007f0104: str      r3, [sp, #4]
007f0108: bl       #0x30eba4
007f010c: ldr      r1, [r4, #0x70]
007f0110: bl       #0x30ed6c
007f0114: ldr      r1, [r5, #0x44]
007f0118: mov      r8, r0
007f011c: ldr      r0, [sp, #0x18]
007f0120: bl       #0x30eba4
007f0124: ldr      r1, [r4, #0x74]
007f0128: bl       #0x30ed6c
007f012c: mov      r1, r0
007f0130: mov      r0, r8
007f0134: bl       #0x30eba4
007f0138: ldr      r1, [r4, #0x7c]
007f013c: bl       #0x30ed6c
007f0140: ldr      r3, [sp, #4]
007f0144: mov      r1, r0
007f0148: mov      r0, r3
007f014c: bl       #0x30e3ac
007f0150: mov      r1, r0
007f0154: ldr      r0, [sp, #0x1c]
007f0158: bl       #0x30ed6c
007f015c: ldr      r1, [sp, #0x10]
007f0160: bl       #0x30eba4
007f0164: mov      r1, #0
007f0168: mov      r8, r0
007f016c: bl       #0x30e70c
007f0170: cmp      r0, #0
007f0174: movne    r8, #0
007f0178: ldr      r1, [sp, #0x10]
007f017c: mov      r0, r8
007f0180: str      r8, [r4, #0x94]
007f0184: bl       #0x30e3ac
007f0188: ldr      r8, [sl]
007f018c: mov      r3, r0
007f0190: mov      r1, r0
007f0194: add      r8, r8, #0x80000000
007f0198: mov      r0, r8
007f019c: str      r3, [sp, #4]
007f01a0: bl       #0x30ed6c
007f01a4: ldr      r1, [r4, #0x68]
007f01a8: str      r0, [sp, #0xc]
007f01ac: bl       #0x30ed6c
007f01b0: ldr      r2, [sp, #0xc]
007f01b4: str      r0, [sp, #0x10]
007f01b8: ldr      r1, [r4, #0x6c]
007f01bc: mov      r0, r2
007f01c0: bl       #0x30ed6c
007f01c4: str      r0, [sp, #0x18]
007f01c8: ldr      r1, [r4, #0x7c]
007f01cc: mov      r0, r8
007f01d0: bl       #0x30ed6c
007f01d4: ldr      r3, [sp, #4]
007f01d8: mov      r1, r3
007f01dc: bl       #0x30ed6c
007f01e0: ldr      r1, [r4, #0x70]
007f01e4: mov      r8, r0
007f01e8: bl       #0x30ed6c
007f01ec: str      r0, [sp, #0x1c]
007f01f0: ldr      r1, [r4, #0x74]
007f01f4: mov      r0, r8
007f01f8: bl       #0x30ed6c
007f01fc: ldr      r3, [r6, #0x78]
007f0200: ldr      r1, [sp, #0x10]
007f0204: mov      r8, r0
007f0208: mov      r0, r3
007f020c: str      r3, [sp, #4]
007f0210: bl       #0x30ed6c
007f0214: mov      r1, r0
007f0218: ldr      r0, [r6, #0x40]
007f021c: bl       #0x30eba4
007f0220: str      r0, [r6, #0x40]
007f0224: ldr      r3, [sp, #4]
007f0228: ldr      r1, [sp, #0x18]
007f022c: mov      r0, r3
007f0230: bl       #0x30ed6c
007f0234: mov      r1, r0
007f0238: ldr      r0, [r6, #0x44]
007f023c: bl       #0x30eba4
007f0240: str      r0, [r6, #0x44]
007f0244: ldr      r1, [sp, #0x18]
007f0248: mov      r0, fp
007f024c: bl       #0x30ed6c
007f0250: ldr      r1, [sp, #0x10]
007f0254: mov      r3, r0
007f0258: ldr      r0, [sp, #0x14]
007f025c: str      r3, [sp, #4]
007f0260: bl       #0x30ed6c
007f0264: ldr      r3, [sp, #4]
007f0268: mov      r1, r0
007f026c: mov      r0, r3
007f0270: bl       #0x30e3ac
007f0274: ldr      r1, [r6, #0x80]
007f0278: bl       #0x30ed6c
007f027c: mov      r1, r0
007f0280: ldr      r0, [r6, #0x48]
007f0284: bl       #0x30eba4
007f0288: str      r0, [r6, #0x48]
007f028c: ldr      r3, [r5, #0x78]
007f0290: ldr      r1, [sp, #0x1c]
007f0294: mov      r0, r3
007f0298: str      r3, [sp, #4]
007f029c: bl       #0x30ed6c
007f02a0: mov      r1, r0
007f02a4: ldr      r0, [r5, #0x40]
007f02a8: bl       #0x30eba4
007f02ac: str      r0, [r5, #0x40]
007f02b0: ldr      r3, [sp, #4]
007f02b4: mov      r1, r8
007f02b8: mov      r0, r3
007f02bc: bl       #0x30ed6c
007f02c0: mov      r1, r0
007f02c4: ldr      r0, [r5, #0x44]
007f02c8: bl       #0x30eba4
007f02cc: mov      r1, r8
007f02d0: str      r0, [r5, #0x44]
007f02d4: mov      r0, sb
007f02d8: bl       #0x30ed6c
007f02dc: ldr      r1, [sp, #0x1c]
007f02e0: mov      r8, r0
007f02e4: mov      r0, r7
007f02e8: bl       #0x30ed6c
007f02ec: mov      r1, r0
007f02f0: mov      r0, r8
007f02f4: bl       #0x30e3ac
007f02f8: ldr      r1, [r5, #0x80]
007f02fc: bl       #0x30ed6c
007f0300: mov      r1, r0
007f0304: ldr      r0, [r5, #0x48]
007f0308: bl       #0x30eba4
007f030c: str      r0, [r5, #0x48]
007f0310: b        #0x7efd24

# _ZNK16b2StackAllocator16GetMaxAllocationEv
007f3598: mov      r3, #0x19000
007f359c: add      r3, r3, #8
007f35a0: ldr      r0, [r0, r3]
007f35a4: bx       lr

# _ZNK13b2PulleyJoint10GetAnchor1Ev
007f0314: push     {r4, r5, r6, r7, r8, lr}
007f0318: ldr      r4, [r1, #0x30]
007f031c: ldr      r7, [r1, #0x58]
007f0320: ldr      r6, [r1, #0x5c]
007f0324: mov      r5, r0
007f0328: ldr      r1, [r4, #0xc]
007f032c: mov      r0, r7
007f0330: bl       #0x30ed6c
007f0334: ldr      r1, [r4, #0x14]
007f0338: mov      r8, r0
007f033c: mov      r0, r6
007f0340: bl       #0x30ed6c
007f0344: mov      r1, r0
007f0348: mov      r0, r8
007f034c: bl       #0x30eba4
007f0350: ldr      r1, [r4, #0x10]
007f0354: mov      r8, r0
007f0358: mov      r0, r7
007f035c: bl       #0x30ed6c
007f0360: ldr      r1, [r4, #0x18]
007f0364: mov      r7, r0
007f0368: mov      r0, r6
007f036c: bl       #0x30ed6c
007f0370: mov      r1, r0
007f0374: mov      r0, r7
007f0378: bl       #0x30eba4
007f037c: ldr      r1, [r4, #8]
007f0380: bl       #0x30eba4
007f0384: ldr      r1, [r4, #4]
007f0388: mov      r6, r0
007f038c: mov      r0, r8
007f0390: bl       #0x30eba4
007f0394: str      r6, [r5, #4]
007f0398: str      r0, [r5]
007f039c: mov      r0, r5
007f03a0: pop      {r4, r5, r6, r7, r8, pc}

# _ZN16b2PolygonContact12GetManifoldsEv
007eca94: add      r0, r0, #0x48
007eca98: bx       lr

# _ZN7b2Joint7DestroyEPS_P16b2BlockAllocator
007eb2bc: push     {r4, r5, r6, lr}
007eb2c0: mov      r4, r0
007eb2c4: ldr      r3, [r0]
007eb2c8: mov      r5, r1
007eb2cc: mov      lr, pc
007eb2d0: ldr      pc, [r3, #0x10]
007eb2d4: ldr      r3, [r4, #4]
007eb2d8: sub      r3, r3, #1
007eb2dc: cmp      r3, #5
007eb2e0: addls    pc, pc, r3, lsl #2
007eb2e4: b        #0x7eb378
007eb2e8: b        #0x7eb314
007eb2ec: b        #0x7eb328
007eb2f0: b        #0x7eb33c
007eb2f4: b        #0x7eb350
007eb2f8: b        #0x7eb364
007eb2fc: b        #0x7eb300
007eb300: mov      r0, r5
007eb304: mov      r1, r4
007eb308: mov      r2, #0xa4
007eb30c: pop      {r4, r5, r6, lr}
007eb310: b        #0x7e8da0
007eb314: mov      r0, r5
007eb318: mov      r1, r4
007eb31c: mov      r2, #0x9c
007eb320: pop      {r4, r5, r6, lr}
007eb324: b        #0x7e8da0
007eb328: mov      r0, r5
007eb32c: mov      r1, r4
007eb330: mov      r2, #0xd0
007eb334: pop      {r4, r5, r6, lr}
007eb338: b        #0x7e8da0
007eb33c: mov      r0, r5
007eb340: mov      r1, r4
007eb344: mov      r2, #0x78
007eb348: pop      {r4, r5, r6, lr}
007eb34c: b        #0x7e8da0
007eb350: mov      r0, r5
007eb354: mov      r1, r4
007eb358: mov      r2, #0xb8
007eb35c: pop      {r4, r5, r6, lr}
007eb360: b        #0x7e8da0
007eb364: mov      r0, r5
007eb368: mov      r1, r4
007eb36c: mov      r2, #0x80
007eb370: pop      {r4, r5, r6, lr}
007eb374: b        #0x7e8da0
007eb378: pop      {r4, r5, r6, pc}

# _ZN7b2World19SetBoundaryListenerEP18b2BoundaryListener
007e65e4: mov      r3, #0x19000
007e65e8: add      r3, r3, #0x25c
007e65ec: str      r1, [r0, r3]
007e65f0: bx       lr

# _ZN7b2World5SolveERK10b2TimeStep
007e76dc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e76e0: mov      r3, #0x19000
007e76e4: mov      r7, r0
007e76e8: add      r3, r3, #0x270
007e76ec: mov      r0, #0x19000
007e76f0: mov      ip, #0
007e76f4: str      ip, [r7, r3]
007e76f8: mov      r2, r0
007e76fc: mov      lr, r0
007e7700: add      r0, r0, #0x23c
007e7704: ldr      r0, [r7, r0]
007e7708: mov      ip, #0x19000
007e770c: add      ip, ip, #0x264
007e7710: sub      sp, sp, #0x4c
007e7714: ldr      ip, [r7, ip]
007e7718: add      lr, lr, #0x244
007e771c: add      r2, r2, #0x240
007e7720: ldr      r3, [r7, lr]
007e7724: ldr      r2, [r7, r2]
007e7728: str      r1, [sp, #0x10]
007e772c: mov      r1, r0
007e7730: add      r0, r7, #0x44
007e7734: str      r0, [sp, #0xc]
007e7738: str      ip, [sp, #4]
007e773c: ldr      ip, [sp, #0xc]
007e7740: add      sl, sp, #0x18
007e7744: mov      r0, sl
007e7748: str      ip, [sp]
007e774c: bl       #0x7eb0e0
007e7750: mov      r3, #0x19000
007e7754: add      r3, r3, #0x230
007e7758: ldr      r3, [r7, r3]
007e775c: cmp      r3, #0
007e7760: beq      #0x7e777c
007e7764: ldrh     r2, [r3]
007e7768: bic      r2, r2, #4
007e776c: strh     r2, [r3]
007e7770: ldr      r3, [r3, #0x60]
007e7774: cmp      r3, #0
007e7778: bne      #0x7e7764
007e777c: mov      r3, #0x19000
007e7780: add      r3, r3, #0x238
007e7784: ldr      r3, [r7, r3]
007e7788: cmp      r3, #0
007e778c: beq      #0x7e77a8
007e7790: ldr      r2, [r3, #4]
007e7794: bic      r2, r2, #4
007e7798: str      r2, [r3, #4]
007e779c: ldr      r3, [r3, #0x10]
007e77a0: cmp      r3, #0
007e77a4: bne      #0x7e7790
007e77a8: mov      r3, #0x19000
007e77ac: add      r3, r3, #0x234
007e77b0: ldr      r3, [r7, r3]
007e77b4: cmp      r3, #0
007e77b8: beq      #0x7e77d0
007e77bc: mov      r2, #0
007e77c0: strb     r2, [r3, #0x3c]
007e77c4: ldr      r3, [r3, #0xc]
007e77c8: cmp      r3, #0
007e77cc: bne      #0x7e77c0
007e77d0: mov      r3, #0x19000
007e77d4: add      r3, r3, #0x23c
007e77d8: ldr      r1, [r7, r3]
007e77dc: ldr      r0, [sp, #0xc]
007e77e0: lsl      r1, r1, #2
007e77e4: bl       #0x7f3644
007e77e8: mov      r3, #0x19000
007e77ec: add      r3, r3, #0x230
007e77f0: ldr      r6, [r7, r3]
007e77f4: mov      r4, r0
007e77f8: cmp      r6, #0
007e77fc: beq      #0x7e7a50
007e7800: mov      sb, #0x19000
007e7804: mov      r8, sb
007e7808: add      r3, r7, #0x19000
007e780c: add      r0, r8, #0x250
007e7810: add      r3, r3, #0x248
007e7814: str      r3, [sp, #0x14]
007e7818: add      sb, sb, #0x274
007e781c: str      r0, [sp, #8]
007e7820: add      r8, r8, #0x270
007e7824: mov      r5, #1
007e7828: ldrh     r3, [r6]
007e782c: ands     r3, r3, #0xe
007e7830: bne      #0x7e7a44
007e7834: ldrsh    r2, [r6, #2]
007e7838: cmp      r2, #0
007e783c: beq      #0x7e7a44
007e7840: str      r3, [sp, #0x30]
007e7844: str      r3, [sp, #0x2c]
007e7848: str      r3, [sp, #0x34]
007e784c: str      r6, [r4]
007e7850: ldrh     r3, [r6]
007e7854: mov      fp, #1
007e7858: orr      r3, r3, #4
007e785c: strh     r3, [r6]
007e7860: ldr      r3, [sp, #0x2c]
007e7864: sub      r1, fp, #1
007e7868: ldr      r0, [r4, r1, lsl #2]
007e786c: ldr      ip, [sp, #0x20]
007e7870: add      r2, r3, #1
007e7874: str      r0, [ip, r3, lsl #2]
007e7878: str      r2, [sp, #0x2c]
007e787c: ldrh     r2, [r0]
007e7880: ldrsh    r3, [r0, #2]
007e7884: bic      r2, r2, #8
007e7888: cmp      r3, #0
007e788c: strh     r2, [r0]
007e7890: addeq    fp, r4, fp, lsl #2
007e7894: beq      #0x7e7984
007e7898: ldr      r3, [r0, #0x70]
007e789c: cmp      r3, #0
007e78a0: beq      #0x7e7910
007e78a4: ldr      r2, [r3, #4]
007e78a8: ldr      ip, [r2, #4]
007e78ac: tst      ip, #5
007e78b0: bne      #0x7e7904
007e78b4: ldr      ip, [r2, #8]
007e78b8: cmp      ip, #0
007e78bc: beq      #0x7e7904
007e78c0: ldr      ip, [sp, #0x34]
007e78c4: ldr      fp, [sp, #0x24]
007e78c8: add      lr, ip, #1
007e78cc: str      r2, [fp, ip, lsl #2]
007e78d0: str      lr, [sp, #0x34]
007e78d4: ldr      r2, [r3, #4]
007e78d8: ldr      ip, [r2, #4]
007e78dc: orr      ip, ip, #4
007e78e0: str      ip, [r2, #4]
007e78e4: ldr      r2, [r3]
007e78e8: ldrh     ip, [r2]
007e78ec: tst      ip, #4
007e78f0: streq    r2, [r4, r1, lsl #2]
007e78f4: ldrheq   ip, [r2]
007e78f8: addeq    r1, r1, #1
007e78fc: orreq    ip, ip, #4
007e7900: strheq   ip, [r2]
007e7904: ldr      r3, [r3, #0xc]
007e7908: cmp      r3, #0
007e790c: bne      #0x7e78a4
007e7910: ldr      r3, [r0, #0x6c]
007e7914: cmp      r3, #0
007e7918: beq      #0x7e7974
007e791c: ldr      r2, [r3, #4]
007e7920: ldrb     r0, [r2, #0x3c]
007e7924: cmp      r0, #0
007e7928: bne      #0x7e7968
007e792c: ldr      r0, [sp, #0x30]
007e7930: ldr      lr, [sp, #0x28]
007e7934: add      ip, r0, #1
007e7938: str      r2, [lr, r0, lsl #2]
007e793c: str      ip, [sp, #0x30]
007e7940: ldr      r2, [r3, #4]
007e7944: strb     r5, [r2, #0x3c]
007e7948: ldr      r2, [r3]
007e794c: ldrh     r0, [r2]
007e7950: tst      r0, #4
007e7954: streq    r2, [r4, r1, lsl #2]
007e7958: ldrheq   r0, [r2]
007e795c: addeq    r1, r1, #1
007e7960: orreq    r0, r0, #4
007e7964: strheq   r0, [r2]
007e7968: ldr      r3, [r3, #0xc]
007e796c: cmp      r3, #0
007e7970: bne      #0x7e791c
007e7974: cmp      r1, #0
007e7978: beq      #0x7e79d4
007e797c: mov      fp, r1
007e7980: b        #0x7e7860
007e7984: cmp      r1, #0
007e7988: add      r0, fp, r3
007e798c: beq      #0x7e79d4
007e7990: ldr      r2, [sp, #0x2c]
007e7994: ldr      r0, [r0, #-8]
007e7998: ldr      lr, [sp, #0x20]
007e799c: add      ip, r2, #1
007e79a0: sub      r1, r1, #1
007e79a4: str      r0, [lr, r2, lsl #2]
007e79a8: str      ip, [sp, #0x2c]
007e79ac: ldrh     r2, [r0]
007e79b0: ldrsh    ip, [r0, #2]
007e79b4: sub      r3, r3, #4
007e79b8: bic      r2, r2, #8
007e79bc: cmp      ip, #0
007e79c0: strh     r2, [r0]
007e79c4: bne      #0x7e7898
007e79c8: cmp      r1, #0
007e79cc: add      r0, fp, r3
007e79d0: bne      #0x7e7990
007e79d4: ldr      r2, [sp, #8]
007e79d8: ldrb     r3, [r7, sb]
007e79dc: mov      r0, sl
007e79e0: ldrb     ip, [r7, r2]
007e79e4: ldr      r1, [sp, #0x10]
007e79e8: ldr      r2, [sp, #0x14]
007e79ec: str      ip, [sp]
007e79f0: bl       #0x7ea9d0
007e79f4: ldr      r0, [sp, #0x2c]
007e79f8: ldr      r3, [r7, r8]
007e79fc: ldr      r2, [sp, #0x44]
007e7a00: cmp      r2, r3
007e7a04: strge    r2, [r7, r8]
007e7a08: strlt    r3, [r7, r8]
007e7a0c: cmp      r0, #0
007e7a10: ble      #0x7e7a44
007e7a14: mov      r3, #0
007e7a18: ldr      r2, [sp, #0x20]
007e7a1c: ldr      r2, [r2, r3, lsl #2]
007e7a20: add      r3, r3, #1
007e7a24: ldrsh    r1, [r2, #2]
007e7a28: cmp      r1, #0
007e7a2c: ldrheq   r1, [r2]
007e7a30: biceq    r1, r1, #4
007e7a34: strheq   r1, [r2]
007e7a38: ldreq    r0, [sp, #0x2c]
007e7a3c: cmp      r0, r3
007e7a40: bgt      #0x7e7a18
007e7a44: ldr      r6, [r6, #0x60]
007e7a48: cmp      r6, #0
007e7a4c: bne      #0x7e7828
007e7a50: mov      r1, r4
007e7a54: ldr      r0, [sp, #0xc]
007e7a58: bl       #0x7f35a8
007e7a5c: mov      r3, #0x19000
007e7a60: add      r3, r3, #0x230
007e7a64: ldr      r4, [r7, r3]
007e7a68: cmp      r4, #0
007e7a6c: beq      #0x7e7adc
007e7a70: mov      r5, #0x19000
007e7a74: add      r5, r5, #0x25c
007e7a78: b        #0x7e7a88
007e7a7c: ldr      r4, [r4, #0x60]
007e7a80: cmp      r4, #0
007e7a84: beq      #0x7e7adc
007e7a88: ldrh     r3, [r4]
007e7a8c: tst      r3, #0xa
007e7a90: bne      #0x7e7a7c
007e7a94: ldrsh    r3, [r4, #2]
007e7a98: cmp      r3, #0
007e7a9c: beq      #0x7e7a7c
007e7aa0: mov      r0, r4
007e7aa4: bl       #0x7e20ac
007e7aa8: cmp      r0, #0
007e7aac: bne      #0x7e7a7c
007e7ab0: ldr      r3, [r7, r5]
007e7ab4: mov      r1, r4
007e7ab8: cmp      r3, #0
007e7abc: mov      r0, r3
007e7ac0: beq      #0x7e7a7c
007e7ac4: ldr      r3, [r3]
007e7ac8: mov      lr, pc
007e7acc: ldr      pc, [r3, #8]
007e7ad0: ldr      r4, [r4, #0x60]
007e7ad4: cmp      r4, #0
007e7ad8: bne      #0x7e7a88
007e7adc: mov      r3, #0x19000
007e7ae0: add      r3, r3, #0x1d8
007e7ae4: ldr      r0, [r7, r3]
007e7ae8: bl       #0x7e2ac8
007e7aec: mov      r0, sl
007e7af0: bl       #0x7eb078
007e7af4: add      sp, sp, #0x4c
007e7af8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN7b2Shape11CreateProxyEP12b2BroadPhaseRK7b2XForm
007e62d4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e62d8: sub      sp, sp, #0x10
007e62dc: mov      r4, r1
007e62e0: ldr      r3, [r0]
007e62e4: mov      r1, sp
007e62e8: mov      r5, r0
007e62ec: mov      lr, pc
007e62f0: ldr      pc, [r3, #8]
007e62f4: mov      r3, #0x5d000
007e62f8: add      r3, r3, #0x24
007e62fc: ldr      r1, [r4, r3]
007e6300: ldr      r0, [sp]
007e6304: bl       #0x30e3ac
007e6308: mov      r3, #0x5d000
007e630c: add      r3, r3, #0x28
007e6310: mov      r7, r0
007e6314: ldr      r1, [r4, r3]
007e6318: ldr      r0, [sp, #4]
007e631c: bl       #0x30e3ac
007e6320: mov      r3, #0x5d000
007e6324: add      r3, r3, #0x1c
007e6328: mov      r8, r0
007e632c: ldr      r1, [sp, #8]
007e6330: ldr      r0, [r4, r3]
007e6334: bl       #0x30e3ac
007e6338: mov      r3, #0x5d000
007e633c: add      r3, r3, #0x20
007e6340: mov      sb, r0
007e6344: ldr      r1, [sp, #0xc]
007e6348: ldr      r0, [r4, r3]
007e634c: bl       #0x30e3ac
007e6350: mov      r1, sb
007e6354: mov      sl, r0
007e6358: mov      r0, r7
007e635c: bl       #0x30e2f8
007e6360: mov      r1, sl
007e6364: cmp      r0, #0
007e6368: mov      r0, r8
007e636c: moveq    r7, sb
007e6370: bl       #0x30e2f8
007e6374: cmp      r0, #0
007e6378: moveq    r8, sl
007e637c: mov      r0, r7
007e6380: mov      r1, r8
007e6384: bl       #0x30e2f8
007e6388: cmp      r0, #0
007e638c: moveq    r7, r8
007e6390: mov      r0, r7
007e6394: mov      r1, #0
007e6398: bl       #0x30e70c
007e639c: cmp      r0, #0
007e63a0: mvneq    r3, #0
007e63a4: mov      r6, sp
007e63a8: strheq   r3, [r5, #0x20]
007e63ac: bne      #0x7e63b8
007e63b0: add      sp, sp, #0x10
007e63b4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e63b8: mov      r0, r4
007e63bc: mov      r1, sp
007e63c0: mov      r2, r5
007e63c4: bl       #0x7e2d54
007e63c8: strh     r0, [r5, #0x20]
007e63cc: b        #0x7e63b0

# _ZN16b2PolygonContactD0Ev
007eca9c: push     {r4, lr}
007ecaa0: mov      r4, r0
007ecaa4: bl       #0x30e2b0
007ecaa8: mov      r0, r4
007ecaac: pop      {r4, pc}

# _ZN15b2CircleContactC1EP7b2ShapeS1_
007f3ab0: push     {r4, r5, r6, lr}
007f3ab4: ldr      r5, [pc, #0x34]
007f3ab8: mov      r4, r0
007f3abc: bl       #0x7e9eb8
007f3ac0: ldr      r3, [pc, #0x2c]
007f3ac4: add      r5, pc, r5
007f3ac8: mov      r2, #0
007f3acc: ldr      r3, [r5, r3]
007f3ad0: mov      r1, #0
007f3ad4: str      r1, [r4, #0x90]
007f3ad8: add      r3, r3, #8
007f3adc: str      r3, [r4]
007f3ae0: str      r2, [r4, #0x60]
007f3ae4: str      r2, [r4, #0x5c]
007f3ae8: mov      r0, r4
007f3aec: pop      {r4, r5, r6, pc}
007f3af0: andseq   r0, sl, ip, asr #31
007f3af4: andeq    r3, r0, r4, lsl #2

# _ZN13b2PulleyJointD0Ev
007f04fc: push     {r4, lr}
007f0500: mov      r4, r0
007f0504: bl       #0x30e2b0
007f0508: mov      r0, r4
007f050c: pop      {r4, pc}

# _ZNK13b2PulleyJoint10GetLength1Ev
007f08c0: push     {r4, r5, r6, r7, r8, lr}
007f08c4: ldr      r5, [r0, #0x30]
007f08c8: ldr      r7, [r0, #0x58]
007f08cc: mov      r4, r0
007f08d0: ldr      r6, [r0, #0x5c]
007f08d4: ldr      r1, [r5, #0xc]
007f08d8: mov      r0, r7
007f08dc: bl       #0x30ed6c
007f08e0: ldr      r1, [r5, #0x14]
007f08e4: mov      r8, r0
007f08e8: mov      r0, r6
007f08ec: bl       #0x30ed6c
007f08f0: mov      r1, r0
007f08f4: mov      r0, r8
007f08f8: bl       #0x30eba4
007f08fc: ldr      r1, [r5, #0x10]
007f0900: mov      r8, r0
007f0904: mov      r0, r7
007f0908: bl       #0x30ed6c
007f090c: ldr      r1, [r5, #0x18]
007f0910: mov      r7, r0
007f0914: mov      r0, r6
007f0918: bl       #0x30ed6c
007f091c: mov      r1, r0
007f0920: mov      r0, r7
007f0924: bl       #0x30eba4
007f0928: ldr      r1, [r5, #4]
007f092c: mov      r6, r0
007f0930: mov      r0, r8
007f0934: bl       #0x30eba4
007f0938: ldr      r1, [r5, #8]
007f093c: mov      r8, r0
007f0940: mov      r0, r6
007f0944: bl       #0x30eba4
007f0948: ldr      r5, [r4, #0x44]
007f094c: ldr      r1, [r4, #0x48]
007f0950: mov      r6, r0
007f0954: ldr      r0, [r5, #4]
007f0958: bl       #0x30eba4
007f095c: ldr      r1, [r4, #0x4c]
007f0960: mov      r7, r0
007f0964: ldr      r0, [r5, #8]
007f0968: bl       #0x30eba4
007f096c: mov      r1, r7
007f0970: mov      r5, r0
007f0974: mov      r0, r8
007f0978: bl       #0x30e3ac
007f097c: mov      r1, r5
007f0980: mov      r4, r0
007f0984: mov      r0, r6
007f0988: bl       #0x30e3ac
007f098c: mov      r1, r4
007f0990: mov      r5, r0
007f0994: mov      r0, r4
007f0998: bl       #0x30ed6c
007f099c: mov      r1, r5
007f09a0: mov      r4, r0
007f09a4: mov      r0, r5
007f09a8: bl       #0x30ed6c
007f09ac: mov      r1, r0
007f09b0: mov      r0, r4
007f09b4: bl       #0x30eba4
007f09b8: pop      {r4, r5, r6, r7, r8, lr}
007f09bc: b        #0x30e124

# _ZN7b2World12SetDebugDrawEP11b2DebugDraw
007e6614: mov      r3, #0x19000
007e6618: add      r3, r3, #0x268
007e661c: str      r1, [r0, r3]
007e6620: bx       lr

# _ZN7b2JointD0Ev
007eb2a8: push     {r4, lr}
007eb2ac: mov      r4, r0
007eb2b0: bl       #0x30e2b0
007eb2b4: mov      r0, r4
007eb2b8: pop      {r4, pc}

# _ZN6b2Body17SetMassFromShapesEv
007e1818: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e181c: ldr      r2, [r0, #0x58]
007e1820: mov      r3, #0x19000
007e1824: add      r3, r3, #0x1d4
007e1828: ldrb     r2, [r2, r3]
007e182c: ldr      r3, [pc, #0x2ec]
007e1830: sub      sp, sp, #0x10
007e1834: cmp      r2, #0
007e1838: mov      r4, r0
007e183c: add      r3, pc, r3
007e1840: bne      #0x7e1aa8
007e1844: ldr      r1, [pc, #0x2d8]
007e1848: ldr      r5, [r0, #0x64]
007e184c: mov      r2, #0
007e1850: ldr      r3, [r3, r1]
007e1854: str      r2, [r0, #0x80]
007e1858: str      r2, [r0, #0x74]
007e185c: str      r2, [r0, #0x78]
007e1860: str      r2, [r0, #0x7c]
007e1864: cmp      r5, #0
007e1868: ldr      r8, [r3]
007e186c: ldr      r7, [r3, #4]
007e1870: mov      r6, r5
007e1874: beq      #0x7e1948
007e1878: mov      sb, sp
007e187c: ldr      r3, [r5]
007e1880: mov      r0, r5
007e1884: mov      r1, sp
007e1888: mov      lr, pc
007e188c: ldr      pc, [r3, #0x10]
007e1890: ldr      r1, [sp]
007e1894: ldr      r0, [r4, #0x74]
007e1898: bl       #0x30eba4
007e189c: ldr      r6, [sp]
007e18a0: str      r0, [r4, #0x74]
007e18a4: ldr      r1, [sp, #4]
007e18a8: mov      sl, r0
007e18ac: mov      r0, r6
007e18b0: bl       #0x30ed6c
007e18b4: mov      r1, r0
007e18b8: mov      r0, r8
007e18bc: bl       #0x30eba4
007e18c0: ldr      r1, [sp, #8]
007e18c4: mov      r8, r0
007e18c8: mov      r0, r6
007e18cc: bl       #0x30ed6c
007e18d0: mov      r1, r0
007e18d4: mov      r0, r7
007e18d8: bl       #0x30eba4
007e18dc: ldr      r1, [sp, #0xc]
007e18e0: mov      r7, r0
007e18e4: ldr      r0, [r4, #0x7c]
007e18e8: bl       #0x30eba4
007e18ec: str      r0, [r4, #0x7c]
007e18f0: ldr      r5, [r5, #8]
007e18f4: cmp      r5, #0
007e18f8: bne      #0x7e187c
007e18fc: mov      r0, sl
007e1900: mov      r1, #0
007e1904: bl       #0x30e2f8
007e1908: cmp      r0, #0
007e190c: beq      #0x7e1944
007e1910: mov      r1, sl
007e1914: mov      r0, #0x3f800000
007e1918: bl       #0x30ec94
007e191c: mov      r5, r0
007e1920: str      r0, [r4, #0x78]
007e1924: mov      r1, r5
007e1928: mov      r0, r8
007e192c: bl       #0x30ed6c
007e1930: mov      r1, r5
007e1934: mov      r8, r0
007e1938: mov      r0, r7
007e193c: bl       #0x30ed6c
007e1940: mov      r7, r0
007e1944: ldr      r6, [r4, #0x64]
007e1948: ldr      r5, [r4, #0x7c]
007e194c: mov      r1, #0
007e1950: mov      r0, r5
007e1954: bl       #0x30e2f8
007e1958: cmp      r0, #0
007e195c: bne      #0x7e1ab0
007e1960: mov      r3, #0
007e1964: str      r3, [r4, #0x80]
007e1968: str      r3, [r4, #0x7c]
007e196c: str      r7, [r4, #0x20]
007e1970: str      r8, [r4, #0x1c]
007e1974: ldr      r1, [r4, #0xc]
007e1978: mov      r0, r8
007e197c: bl       #0x30ed6c
007e1980: ldr      r1, [r4, #0x14]
007e1984: mov      r5, r0
007e1988: mov      r0, r7
007e198c: bl       #0x30ed6c
007e1990: mov      r1, r0
007e1994: mov      r0, r5
007e1998: bl       #0x30eba4
007e199c: ldr      r1, [r4, #0x10]
007e19a0: mov      r5, r0
007e19a4: mov      r0, r8
007e19a8: bl       #0x30ed6c
007e19ac: ldr      r1, [r4, #0x18]
007e19b0: mov      r8, r0
007e19b4: mov      r0, r7
007e19b8: bl       #0x30ed6c
007e19bc: mov      r1, r0
007e19c0: mov      r0, r8
007e19c4: bl       #0x30eba4
007e19c8: ldr      r1, [r4, #4]
007e19cc: mov      r7, r0
007e19d0: mov      r0, r5
007e19d4: bl       #0x30eba4
007e19d8: ldr      r1, [r4, #8]
007e19dc: mov      r5, r0
007e19e0: mov      r0, r7
007e19e4: bl       #0x30eba4
007e19e8: str      r5, [r4, #0x2c]
007e19ec: str      r0, [r4, #0x30]
007e19f0: ldr      r2, [r4, #0x2c]
007e19f4: ldr      r3, [r4, #0x30]
007e19f8: subs     r5, r6, #0
007e19fc: str      r2, [r4, #0x24]
007e1a00: str      r3, [r4, #0x28]
007e1a04: beq      #0x7e1a2c
007e1a08: add      r6, r4, #0x1c
007e1a0c: ldr      r3, [r5]
007e1a10: mov      r0, r5
007e1a14: mov      r1, r6
007e1a18: mov      lr, pc
007e1a1c: ldr      pc, [r3, #0x1c]
007e1a20: ldr      r5, [r5, #8]
007e1a24: cmp      r5, #0
007e1a28: bne      #0x7e1a0c
007e1a2c: ldr      r0, [r4, #0x78]
007e1a30: mov      r1, #0
007e1a34: bl       #0x30df8c
007e1a38: cmp      r0, #0
007e1a3c: ldrh     r5, [r4, #2]
007e1a40: beq      #0x7e1b10
007e1a44: ldr      r0, [r4, #0x80]
007e1a48: mov      r1, #0
007e1a4c: bl       #0x30df8c
007e1a50: cmp      r0, #0
007e1a54: movne    r3, #0
007e1a58: strhne   r3, [r4, #2]
007e1a5c: movne    r3, #0
007e1a60: beq      #0x7e1b10
007e1a64: sxth     r5, r5
007e1a68: cmp      r5, r3
007e1a6c: beq      #0x7e1aa8
007e1a70: ldr      r5, [r4, #0x64]
007e1a74: cmp      r5, #0
007e1a78: beq      #0x7e1aa8
007e1a7c: mov      r6, #0x19000
007e1a80: add      r6, r6, #0x1d8
007e1a84: add      r7, r4, #4
007e1a88: ldr      r3, [r4, #0x58]
007e1a8c: mov      r0, r5
007e1a90: mov      r2, r7
007e1a94: ldr      r1, [r3, r6]
007e1a98: bl       #0x7e61b0
007e1a9c: ldr      r5, [r5, #8]
007e1aa0: cmp      r5, #0
007e1aa4: bne      #0x7e1a88
007e1aa8: add      sp, sp, #0x10
007e1aac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e1ab0: ldrh     r3, [r4]
007e1ab4: tst      r3, #0x40
007e1ab8: bne      #0x7e1960
007e1abc: mov      r1, r8
007e1ac0: mov      r0, r8
007e1ac4: bl       #0x30ed6c
007e1ac8: mov      r1, r7
007e1acc: mov      sl, r0
007e1ad0: mov      r0, r7
007e1ad4: bl       #0x30ed6c
007e1ad8: mov      r1, r0
007e1adc: mov      r0, sl
007e1ae0: bl       #0x30eba4
007e1ae4: ldr      r1, [r4, #0x74]
007e1ae8: bl       #0x30ed6c
007e1aec: mov      r1, r0
007e1af0: mov      r0, r5
007e1af4: bl       #0x30e3ac
007e1af8: mov      r1, r0
007e1afc: str      r0, [r4, #0x7c]
007e1b00: mov      r0, #0x3f800000
007e1b04: bl       #0x30ec94
007e1b08: str      r0, [r4, #0x80]
007e1b0c: b        #0x7e196c
007e1b10: mov      r3, #1
007e1b14: strh     r3, [r4, #2]
007e1b18: mov      r3, #1
007e1b1c: b        #0x7e1a64
007e1b20: andseq   r3, fp, r4, asr r2
007e1b24: andeq    r0, r0, r0, asr #18

# _ZN13b2PairManagerC1Ev
007e3e54: str      r4, [sp, #-4]!
007e3e58: mov      r3, #0
007e3e5c: add      r2, r0, r3
007e3e60: add      r3, r3, #2
007e3e64: add      r2, r2, #0x40000
007e3e68: mvn      r1, #0
007e3e6c: cmp      r3, #0x8000
007e3e70: strh     r1, [r2, #0x14]
007e3e74: bne      #0x7e3e5c
007e3e78: mov      r3, #0x30000
007e3e7c: add      r3, r3, #8
007e3e80: mov      r2, #0
007e3e84: strh     r2, [r0, r3]
007e3e88: mov      r1, #0
007e3e8c: mov      r3, r0
007e3e90: mov      r2, #1
007e3e94: movw     ip, #0x4001
007e3e98: strh     r2, [r3, #0x10]
007e3e9c: add      r2, r2, #1
007e3ea0: mvn      r4, #0
007e3ea4: cmp      r2, ip
007e3ea8: strh     r4, [r3, #0xc]
007e3eac: strh     r4, [r3, #0xe]
007e3eb0: str      r1, [r3, #8]
007e3eb4: strh     r1, [r3, #0x12]
007e3eb8: add      r3, r3, #0xc
007e3ebc: bne      #0x7e3e98
007e3ec0: mov      ip, #0x30000
007e3ec4: mov      r2, ip
007e3ec8: mov      r3, #0x40000
007e3ecc: add      ip, ip, #4
007e3ed0: add      r2, r2, #0xc
007e3ed4: add      r3, r3, #0x10
007e3ed8: strh     r4, [r0, ip]
007e3edc: str      r1, [r0, r2]
007e3ee0: str      r1, [r0, r3]
007e3ee4: ldm      sp!, {r4}
007e3ee8: bx       lr

# _ZN16b2PolygonContactC1EP7b2ShapeS1_
007ecab0: push     {r4, r5, r6, lr}
007ecab4: ldr      r4, [pc, #0x28]
007ecab8: mov      r5, r0
007ecabc: bl       #0x7e9eb8
007ecac0: ldr      r3, [pc, #0x20]
007ecac4: add      r4, pc, r4
007ecac8: mov      r2, #0
007ecacc: ldr      r3, [r4, r3]
007ecad0: str      r2, [r5, #0x90]
007ecad4: mov      r0, r5
007ecad8: add      r3, r3, #8
007ecadc: str      r3, [r5]
007ecae0: pop      {r4, r5, r6, pc}
007ecae4: andseq   r7, sl, ip, asr #31
007ecae8: andeq    r2, r0, r8, ror fp

# _ZN6b2BodyC2EPK9b2BodyDefP7b2World
007e1e4c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e1e50: mov      r3, #0
007e1e54: strh     r3, [r0]
007e1e58: ldrb     r3, [r1, #0x2b]
007e1e5c: mov      r4, r0
007e1e60: mov      r5, r1
007e1e64: cmp      r3, #0
007e1e68: movne    r3, #0x20
007e1e6c: strhne   r3, [r0]
007e1e70: ldrb     r3, [r1, #0x2a]
007e1e74: mov      sb, #0x3f800000
007e1e78: cmp      r3, #0
007e1e7c: ldrhne   r3, [r0]
007e1e80: orrne    r3, r3, #0x40
007e1e84: strhne   r3, [r0]
007e1e88: ldrb     r3, [r1, #0x28]
007e1e8c: cmp      r3, #0
007e1e90: ldrhne   r3, [r0]
007e1e94: orrne    r3, r3, #0x10
007e1e98: strhne   r3, [r0]
007e1e9c: ldrb     r3, [r1, #0x29]
007e1ea0: str      r2, [r0, #0x58]
007e1ea4: cmp      r3, #0
007e1ea8: ldrhne   r3, [r0]
007e1eac: orrne    r3, r3, #8
007e1eb0: strhne   r3, [r0]
007e1eb4: ldr      r3, [r1, #0x14]
007e1eb8: str      r3, [r0, #4]
007e1ebc: ldr      r3, [r1, #0x18]
007e1ec0: str      r3, [r0, #8]
007e1ec4: ldr      r7, [r1, #0x1c]
007e1ec8: mov      r0, r7
007e1ecc: bl       #0x30e754
007e1ed0: mov      r6, r0
007e1ed4: mov      r0, r7
007e1ed8: bl       #0x30eb08
007e1edc: add      sl, r0, #0x80000000
007e1ee0: str      r6, [r4, #0xc]
007e1ee4: str      sl, [r4, #0x14]
007e1ee8: str      r0, [r4, #0x10]
007e1eec: str      r6, [r4, #0x18]
007e1ef0: ldr      r3, [r5, #4]
007e1ef4: mov      r7, r0
007e1ef8: mov      r1, r6
007e1efc: str      r3, [r4, #0x1c]
007e1f00: ldr      r3, [r5, #8]
007e1f04: str      sb, [r4, #0x3c]
007e1f08: ldr      r8, [r4, #0x1c]
007e1f0c: str      r3, [r4, #0x20]
007e1f10: ldr      r3, [r5, #0x1c]
007e1f14: mov      r0, r8
007e1f18: str      r3, [r4, #0x34]
007e1f1c: str      r3, [r4, #0x38]
007e1f20: bl       #0x30ed6c
007e1f24: mov      r1, sl
007e1f28: mov      fp, r0
007e1f2c: ldr      r0, [r4, #0x20]
007e1f30: bl       #0x30ed6c
007e1f34: mov      r1, r0
007e1f38: mov      r0, fp
007e1f3c: bl       #0x30eba4
007e1f40: mov      r1, r7
007e1f44: mov      sl, r0
007e1f48: mov      r0, r8
007e1f4c: bl       #0x30ed6c
007e1f50: mov      r1, r6
007e1f54: mov      r7, r0
007e1f58: ldr      r0, [r4, #0x20]
007e1f5c: bl       #0x30ed6c
007e1f60: mov      r1, r0
007e1f64: mov      r0, r7
007e1f68: bl       #0x30eba4
007e1f6c: ldr      r1, [r4, #4]
007e1f70: mov      r7, r0
007e1f74: mov      r0, sl
007e1f78: bl       #0x30eba4
007e1f7c: ldr      r1, [r4, #8]
007e1f80: mov      r6, r0
007e1f84: mov      r0, r7
007e1f88: bl       #0x30eba4
007e1f8c: str      r6, [r4, #0x2c]
007e1f90: str      r0, [r4, #0x30]
007e1f94: ldr      r1, [r4, #0x2c]
007e1f98: ldr      r2, [r4, #0x30]
007e1f9c: mov      r3, #0
007e1fa0: str      r1, [r4, #0x24]
007e1fa4: str      r2, [r4, #0x28]
007e1fa8: str      r3, [r4, #0x60]
007e1fac: str      r3, [r4, #0x6c]
007e1fb0: str      r3, [r4, #0x70]
007e1fb4: str      r3, [r4, #0x5c]
007e1fb8: ldr      r2, [r5, #0x20]
007e1fbc: mov      r3, #0
007e1fc0: mov      r1, r3
007e1fc4: str      r2, [r4, #0x84]
007e1fc8: ldr      r2, [r5, #0x24]
007e1fcc: str      r3, [r4, #0x4c]
007e1fd0: str      r3, [r4, #0x50]
007e1fd4: str      r2, [r4, #0x88]
007e1fd8: str      r3, [r4, #0x54]
007e1fdc: str      r3, [r4, #0x40]
007e1fe0: str      r3, [r4, #0x44]
007e1fe4: str      r3, [r4, #0x48]
007e1fe8: str      r3, [r4, #0x8c]
007e1fec: str      r3, [r4, #0x78]
007e1ff0: str      r3, [r4, #0x7c]
007e1ff4: str      r3, [r4, #0x80]
007e1ff8: ldr      r6, [r5]
007e1ffc: str      r6, [r4, #0x74]
007e2000: mov      r0, r6
007e2004: bl       #0x30e2f8
007e2008: cmp      r0, #0
007e200c: beq      #0x7e2020
007e2010: mov      r0, sb
007e2014: mov      r1, r6
007e2018: bl       #0x30ec94
007e201c: str      r0, [r4, #0x78]
007e2020: ldrh     r3, [r4]
007e2024: mov      r1, #0
007e2028: tst      r3, #0x40
007e202c: ldreq    r6, [r5, #0xc]
007e2030: ldrne    r6, [r4, #0x7c]
007e2034: streq    r6, [r4, #0x7c]
007e2038: mov      r0, r6
007e203c: bl       #0x30e2f8
007e2040: cmp      r0, #0
007e2044: beq      #0x7e2058
007e2048: mov      r1, r6
007e204c: mov      r0, #0x3f800000
007e2050: bl       #0x30ec94
007e2054: str      r0, [r4, #0x80]
007e2058: ldr      r0, [r4, #0x78]
007e205c: mov      r1, #0
007e2060: bl       #0x30df8c
007e2064: cmp      r0, #0
007e2068: beq      #0x7e2088
007e206c: ldr      r0, [r4, #0x80]
007e2070: mov      r1, #0
007e2074: bl       #0x30df8c
007e2078: cmp      r0, #0
007e207c: movne    r2, #0
007e2080: strhne   r2, [r4, #2]
007e2084: bne      #0x7e2090
007e2088: mov      r3, #1
007e208c: strh     r3, [r4, #2]
007e2090: ldr      r2, [r5, #0x10]
007e2094: mov      r3, #0
007e2098: str      r3, [r4, #0x68]
007e209c: str      r2, [r4, #0x90]
007e20a0: str      r3, [r4, #0x64]
007e20a4: mov      r0, r4
007e20a8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN11b2DebugDraw8SetFlagsEj
007e8cf4: str      r1, [r0, #4]
007e8cf8: bx       lr

# _ZN7b2World11DestroyBodyEP6b2Body
007e7db0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e7db4: mov      r3, #0x19000
007e7db8: add      r3, r3, #0x1d4
007e7dbc: ldrb     r3, [r0, r3]
007e7dc0: mov      r4, r0
007e7dc4: mov      r7, r1
007e7dc8: cmp      r3, #0
007e7dcc: bne      #0x7e7ef4
007e7dd0: ldr      r5, [r1, #0x6c]
007e7dd4: cmp      r5, #0
007e7dd8: beq      #0x7e7e24
007e7ddc: mov      r8, #0x19000
007e7de0: add      r8, r8, #0x258
007e7de4: b        #0x7e7dec
007e7de8: mov      r5, r6
007e7dec: ldr      r3, [r4, r8]
007e7df0: ldr      r6, [r5, #0xc]
007e7df4: cmp      r3, #0
007e7df8: mov      r0, r3
007e7dfc: beq      #0x7e7e10
007e7e00: ldr      r1, [r5, #4]
007e7e04: ldr      r3, [r3]
007e7e08: mov      lr, pc
007e7e0c: ldr      pc, [r3, #8]
007e7e10: ldr      r1, [r5, #4]
007e7e14: mov      r0, r4
007e7e18: bl       #0x7e7b1c
007e7e1c: cmp      r6, #0
007e7e20: bne      #0x7e7de8
007e7e24: ldr      r5, [r7, #0x64]
007e7e28: cmp      r5, #0
007e7e2c: beq      #0x7e7e8c
007e7e30: mov      sl, #0x19000
007e7e34: mov      r8, sl
007e7e38: add      r8, r8, #0x1d8
007e7e3c: add      sl, sl, #0x258
007e7e40: b        #0x7e7e48
007e7e44: mov      r5, r6
007e7e48: ldr      r3, [r4, sl]
007e7e4c: mov      r1, r5
007e7e50: ldr      r6, [r5, #8]
007e7e54: cmp      r3, #0
007e7e58: mov      r0, r3
007e7e5c: beq      #0x7e7e6c
007e7e60: ldr      r3, [r3]
007e7e64: mov      lr, pc
007e7e68: ldr      pc, [r3, #0xc]
007e7e6c: mov      r0, r5
007e7e70: ldr      r1, [r4, r8]
007e7e74: bl       #0x7e6180
007e7e78: mov      r0, r5
007e7e7c: mov      r1, r4
007e7e80: bl       #0x7e64f4
007e7e84: cmp      r6, #0
007e7e88: bne      #0x7e7e44
007e7e8c: ldr      r3, [r7, #0x5c]
007e7e90: mov      r0, r7
007e7e94: cmp      r3, #0
007e7e98: ldrne    r2, [r7, #0x60]
007e7e9c: strne    r2, [r3, #0x60]
007e7ea0: ldr      r3, [r7, #0x60]
007e7ea4: cmp      r3, #0
007e7ea8: ldrne    r2, [r7, #0x5c]
007e7eac: strne    r2, [r3, #0x5c]
007e7eb0: mov      r3, #0x19000
007e7eb4: add      r3, r3, #0x230
007e7eb8: ldr      r2, [r4, r3]
007e7ebc: cmp      r2, r7
007e7ec0: ldreq    r2, [r7, #0x60]
007e7ec4: streq    r2, [r4, r3]
007e7ec8: mov      r3, #0x19000
007e7ecc: add      r3, r3, #0x23c
007e7ed0: ldr      r2, [r4, r3]
007e7ed4: sub      r2, r2, #1
007e7ed8: str      r2, [r4, r3]
007e7edc: bl       #0x7e1648
007e7ee0: mov      r0, r4
007e7ee4: mov      r1, r7
007e7ee8: mov      r2, #0x94
007e7eec: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
007e7ef0: b        #0x7e8da0
007e7ef4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN11b2DebugDraw11AppendFlagsEj
007e8d04: ldr      r3, [r0, #4]
007e8d08: orr      r3, r3, r1
007e8d0c: str      r3, [r0, #4]
007e8d10: bx       lr

# _ZN6b2Body12DestroyShapeEP7b2Shape
007e1d24: push     {r4, r5, r6, lr}
007e1d28: ldr      r2, [r0, #0x58]
007e1d2c: mov      r3, #0x19000
007e1d30: add      r3, r3, #0x1d4
007e1d34: ldrb     r3, [r2, r3]
007e1d38: mov      r5, r0
007e1d3c: mov      r4, r1
007e1d40: cmp      r3, #0
007e1d44: beq      #0x7e1d4c
007e1d48: pop      {r4, r5, r6, pc}
007e1d4c: mov      r3, #0x19000
007e1d50: add      r3, r3, #0x1d8
007e1d54: ldr      r1, [r2, r3]
007e1d58: mov      r0, r4
007e1d5c: bl       #0x7e6180
007e1d60: ldr      r3, [r5, #0x64]
007e1d64: cmp      r3, #0
007e1d68: beq      #0x7e1d94
007e1d6c: cmp      r4, r3
007e1d70: addeq    r2, r5, #0x64
007e1d74: bne      #0x7e1d84
007e1d78: b        #0x7e1dbc
007e1d7c: cmp      r4, r3
007e1d80: beq      #0x7e1dbc
007e1d84: add      r2, r3, #8
007e1d88: ldr      r3, [r3, #8]
007e1d8c: cmp      r3, #0
007e1d90: bne      #0x7e1d7c
007e1d94: mov      r3, #0
007e1d98: str      r3, [r4, #8]
007e1d9c: str      r3, [r4, #0xc]
007e1da0: ldr      r3, [r5, #0x68]
007e1da4: ldr      r1, [r5, #0x58]
007e1da8: mov      r0, r4
007e1dac: sub      r3, r3, #1
007e1db0: str      r3, [r5, #0x68]
007e1db4: pop      {r4, r5, r6, lr}
007e1db8: b        #0x7e64f4
007e1dbc: ldr      r3, [r4, #8]
007e1dc0: str      r3, [r2]
007e1dc4: b        #0x7e1d94

# _ZN6b2BodyD1Ev
007e1648: bx       lr

# _ZN13b2CircleShapeD1Ev
007e9668: ldr      r3, [pc, #0x24]
007e966c: ldr      r2, [pc, #0x24]
007e9670: push     {r4, lr}
007e9674: add      r3, pc, r3
007e9678: ldr      r2, [r3, r2]
007e967c: mov      r4, r0
007e9680: add      r2, r2, #8
007e9684: str      r2, [r0]
007e9688: bl       #0x7e6178
007e968c: mov      r0, r4
007e9690: pop      {r4, pc}
007e9694: andseq   fp, sl, ip, lsl r4
007e9698: andeq    r0, r0, ip, ror #16

# _ZN14b2PolygonShapeD0Ev
007e602c: ldr      r3, [pc, #0x2c]
007e6030: ldr      r2, [pc, #0x2c]
007e6034: push     {r4, lr}
007e6038: add      r3, pc, r3
007e603c: ldr      r2, [r3, r2]
007e6040: mov      r4, r0
007e6044: add      r2, r2, #8
007e6048: str      r2, [r0]
007e604c: bl       #0x7e6178
007e6050: mov      r0, r4
007e6054: bl       #0x30e2b0
007e6058: mov      r0, r4
007e605c: pop      {r4, pc}
007e6060: andseq   lr, sl, r8, asr sl
007e6064: andeq    r1, r0, r8, lsl #17

# _ZN16b2BlockAllocatorD2Ev
007e8e24: push     {r4, r5, r6, lr}
007e8e28: ldr      r3, [r0, #4]
007e8e2c: mov      r5, r0
007e8e30: cmp      r3, #0
007e8e34: ble      #0x7e8e5c
007e8e38: mov      r4, #0
007e8e3c: ldr      r3, [r5]
007e8e40: add      r3, r3, r4, lsl #3
007e8e44: ldr      r0, [r3, #4]
007e8e48: bl       #0x7f34bc
007e8e4c: ldr      r3, [r5, #4]
007e8e50: add      r4, r4, #1
007e8e54: cmp      r3, r4
007e8e58: bgt      #0x7e8e3c
007e8e5c: ldr      r0, [r5]
007e8e60: bl       #0x7f34bc
007e8e64: mov      r0, r5
007e8e68: pop      {r4, r5, r6, pc}

# _ZN7b2JointC1EPK10b2JointDef
007eb244: ldr      r2, [pc, #0x54]
007eb248: ldr      ip, [pc, #0x54]
007eb24c: str      r4, [sp, #-4]!
007eb250: add      r2, pc, r2
007eb254: ldr      ip, [r2, ip]
007eb258: mov      r4, #0
007eb25c: add      ip, ip, #8
007eb260: str      ip, [r0]
007eb264: ldr      ip, [r1]
007eb268: str      r4, [r0, #8]
007eb26c: str      r4, [r0, #0xc]
007eb270: str      ip, [r0, #4]
007eb274: ldr      ip, [r1, #8]
007eb278: str      ip, [r0, #0x30]
007eb27c: ldr      r2, [r1, #0xc]
007eb280: str      r2, [r0, #0x34]
007eb284: ldrb     r2, [r1, #0x10]
007eb288: strb     r4, [r0, #0x3c]
007eb28c: strb     r2, [r0, #0x3d]
007eb290: ldr      r2, [r1, #4]
007eb294: str      r2, [r0, #0x40]
007eb298: ldm      sp!, {r4}
007eb29c: bx       lr
007eb2a0: andseq   sb, sl, r0, asr #16
007eb2a4: strheq   r4, [r0], -r4

# _ZN7b2WorldC1ERK6b2AABBRK6b2Vec2b
007e80b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e80bc: mov      r4, r0
007e80c0: sub      sp, sp, #0x44
007e80c4: mov      r7, r2
007e80c8: stmib    sp, {r1, r3}
007e80cc: ldr      r8, [pc, #0x1a4]
007e80d0: bl       #0x7e8efc
007e80d4: add      r0, r4, #0x44
007e80d8: bl       #0x7f3560
007e80dc: ldr      fp, [pc, #0x198]
007e80e0: ldr      lr, [pc, #0x198]
007e80e4: ldr      r3, [pc, #0x198]
007e80e8: add      r8, pc, r8
007e80ec: ldr      fp, [r8, fp]
007e80f0: ldr      lr, [r8, lr]
007e80f4: ldr      r3, [r8, r3]
007e80f8: mov      r5, #0x19000
007e80fc: add      sb, r5, #0x1dc
007e8100: add      ip, r5, #0x1e4
007e8104: add      fp, fp, #8
007e8108: add      lr, lr, #8
007e810c: str      r3, [sp, #0xc]
007e8110: str      fp, [r4, sb]
007e8114: str      lr, [r4, ip]
007e8118: ldr      lr, [sp, #0xc]
007e811c: add      ip, ip, #0x7c
007e8120: mov      r6, #0
007e8124: str      lr, [r4, ip]
007e8128: add      lr, r5, #0x21c
007e812c: sub      ip, ip, #0x48
007e8130: str      r6, [r4, ip]
007e8134: str      r6, [r4, lr]
007e8138: add      ip, ip, #0x14
007e813c: add      lr, lr, #0x3c
007e8140: strb     r6, [r4, ip]
007e8144: str      r6, [r4, lr]
007e8148: add      ip, ip, #0x30
007e814c: add      lr, lr, #0xc
007e8150: str      r6, [r4, ip]
007e8154: str      r6, [r4, lr]
007e8158: add      ip, ip, #0xc
007e815c: sub      lr, lr, #0x34
007e8160: str      r6, [r4, ip]
007e8164: str      r6, [r4, lr]
007e8168: sub      ip, ip, #0x30
007e816c: add      lr, lr, #4
007e8170: str      r6, [r4, ip]
007e8174: movw     r1, #0x9275
007e8178: str      r6, [r4, lr]
007e817c: add      ip, ip, #4
007e8180: add      lr, lr, #0xc
007e8184: movw     r2, #0x9276
007e8188: str      r6, [r4, ip]
007e818c: add      r3, r5, #0x1e0
007e8190: str      r6, [r4, lr]
007e8194: mov      sl, #1
007e8198: movt     r1, #1
007e819c: movt     r2, #1
007e81a0: add      ip, ip, #8
007e81a4: add      lr, lr, #0x34
007e81a8: str      r6, [r4, ip]
007e81ac: strb     sl, [r4, lr]
007e81b0: str      r6, [r4, r3]
007e81b4: strb     sl, [r4, r1]
007e81b8: strb     sl, [r4, r2]
007e81bc: ldr      r2, [sp, #8]
007e81c0: add      r0, r5, #0x250
007e81c4: add      ip, r5, #0x1d4
007e81c8: strb     r2, [r4, r0]
007e81cc: ldr      r1, [r7]
007e81d0: mov      r2, r4
007e81d4: add      r0, r5, #0x248
007e81d8: str      r1, [r2, r0]!
007e81dc: ldr      lr, [r7, #4]
007e81e0: mov      r0, #0x5d000
007e81e4: mov      r7, #0
007e81e8: add      r1, r5, #0x26c
007e81ec: add      r0, r0, #0x3c
007e81f0: str      lr, [r2, #4]
007e81f4: strb     r6, [r4, ip]
007e81f8: str      r7, [r4, r1]
007e81fc: str      r4, [r4, r3]
007e8200: bl       #0x7f34f4
007e8204: add      r2, r4, r5
007e8208: ldr      r1, [sp, #4]
007e820c: add      r2, r2, #0x1dc
007e8210: mov      sb, r0
007e8214: bl       #0x7e3048
007e8218: add      r3, r5, #0x1d8
007e821c: str      sb, [r4, r3]
007e8220: mov      r0, r4
007e8224: add      r1, sp, #0x14
007e8228: str      r7, [sp, #0x38]
007e822c: strb     sl, [sp, #0x3c]
007e8230: strb     r6, [sp, #0x3f]
007e8234: str      r7, [sp, #0x18]
007e8238: str      r7, [sp, #0x1c]
007e823c: str      r7, [sp, #0x14]
007e8240: str      r7, [sp, #0x20]
007e8244: str      r6, [sp, #0x24]
007e8248: str      r7, [sp, #0x28]
007e824c: str      r7, [sp, #0x2c]
007e8250: str      r7, [sp, #0x30]
007e8254: str      r7, [sp, #0x34]
007e8258: strb     r6, [sp, #0x3d]
007e825c: strb     r6, [sp, #0x3e]
007e8260: bl       #0x7e7ef8
007e8264: add      r5, r5, #0x254
007e8268: str      r0, [r4, r5]
007e826c: mov      r0, r4
007e8270: add      sp, sp, #0x44
007e8274: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e8278: andseq   ip, sl, r8, lsr #19
007e827c: andeq    r2, r0, r8, lsl #1
007e8280: andeq    r2, r0, r8, lsl #19
007e8284: andeq    r1, r0, r0, asr #1

# _ZN7b2Joint23InitPositionConstraintsEv
007eb1dc: bx       lr

# _ZNK14b2PolygonShape16ComputeSweptAABBEP6b2AABBRK7b2XFormS4_
007e4dc8: push     {r4, r5, r6, r7, r8, lr}
007e4dcc: sub      sp, sp, #0x20
007e4dd0: mov      r5, r1
007e4dd4: mov      r4, r0
007e4dd8: mov      r6, r3
007e4ddc: add      r1, sp, #0x10
007e4de0: ldr      r3, [r0]
007e4de4: mov      lr, pc
007e4de8: ldr      pc, [r3, #8]
007e4dec: mov      r2, r6
007e4df0: ldr      r3, [r4]
007e4df4: mov      r0, r4
007e4df8: mov      r1, sp
007e4dfc: mov      lr, pc
007e4e00: ldr      pc, [r3, #8]
007e4e04: ldr      r4, [sp, #0x10]
007e4e08: ldr      r6, [sp]
007e4e0c: mov      r0, r4
007e4e10: mov      r1, r6
007e4e14: bl       #0x30e70c
007e4e18: cmp      r0, #0
007e4e1c: ldr      r7, [sp, #0x14]
007e4e20: moveq    r4, r6
007e4e24: ldr      r6, [sp, #4]
007e4e28: mov      r0, r7
007e4e2c: mov      r1, r6
007e4e30: bl       #0x30e70c
007e4e34: cmp      r0, #0
007e4e38: ldr      r8, [sp, #8]
007e4e3c: moveq    r7, r6
007e4e40: ldr      r6, [sp, #0x18]
007e4e44: mov      r1, r8
007e4e48: str      r7, [r5, #4]
007e4e4c: str      r4, [r5]
007e4e50: mov      r0, r6
007e4e54: bl       #0x30e2f8
007e4e58: ldr      r4, [sp, #0x1c]
007e4e5c: ldr      r7, [sp, #0xc]
007e4e60: cmp      r0, #0
007e4e64: mov      r0, r4
007e4e68: mov      r1, r7
007e4e6c: moveq    r6, r8
007e4e70: bl       #0x30e2f8
007e4e74: cmp      r0, #0
007e4e78: moveq    r4, r7
007e4e7c: str      r6, [r5, #8]
007e4e80: str      r4, [r5, #0xc]
007e4e84: add      sp, sp, #0x20
007e4e88: pop      {r4, r5, r6, r7, r8, pc}

# _ZN15b2ContactFilterD1Ev
007e8c40: bx       lr

# _ZN15b2RevoluteJointD0Ev
007f2bd8: push     {r4, lr}
007f2bdc: mov      r4, r0
007f2be0: bl       #0x30e2b0
007f2be4: mov      r0, r4
007f2be8: pop      {r4, pc}

# _ZN7b2ShapeD1Ev
007e617c: bx       lr

# _ZN12b2MouseJointD1Ev
007eb838: bx       lr

# _ZNK12b2MouseJoint16GetReactionForceEv
007eb81c: ldr      ip, [r1, #0x54]
007eb820: ldr      r2, [r1, #0x58]
007eb824: str      ip, [r0]
007eb828: str      r2, [r0, #4]
007eb82c: bx       lr

# _ZNK15b2RevoluteJoint10GetAnchor1Ev
007f2a00: push     {r4, r5, r6, r7, r8, lr}
007f2a04: ldr      r4, [r1, #0x30]
007f2a08: ldr      r7, [r1, #0x44]
007f2a0c: ldr      r6, [r1, #0x48]
007f2a10: mov      r5, r0
007f2a14: ldr      r1, [r4, #0xc]
007f2a18: mov      r0, r7
007f2a1c: bl       #0x30ed6c
007f2a20: ldr      r1, [r4, #0x14]
007f2a24: mov      r8, r0
007f2a28: mov      r0, r6
007f2a2c: bl       #0x30ed6c
007f2a30: mov      r1, r0
007f2a34: mov      r0, r8
007f2a38: bl       #0x30eba4
007f2a3c: ldr      r1, [r4, #0x10]
007f2a40: mov      r8, r0
007f2a44: mov      r0, r7
007f2a48: bl       #0x30ed6c
007f2a4c: ldr      r1, [r4, #0x18]
007f2a50: mov      r7, r0
007f2a54: mov      r0, r6
007f2a58: bl       #0x30ed6c
007f2a5c: mov      r1, r0
007f2a60: mov      r0, r7
007f2a64: bl       #0x30eba4
007f2a68: ldr      r1, [r4, #8]
007f2a6c: bl       #0x30eba4
007f2a70: ldr      r1, [r4, #4]
007f2a74: mov      r6, r0
007f2a78: mov      r0, r8
007f2a7c: bl       #0x30eba4
007f2a80: str      r6, [r5, #4]
007f2a84: str      r0, [r5]
007f2a88: mov      r0, r5
007f2a8c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN16b2PrismaticJointD0Ev
007eeffc: push     {r4, lr}
007ef000: mov      r4, r0
007ef004: bl       #0x30e2b0
007ef008: mov      r0, r4
007ef00c: pop      {r4, pc}

# _ZN8b2Island6ReportEP19b2ContactConstraint
007ea6a8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ea6ac: sub      sp, sp, #0x44
007ea6b0: str      r0, [sp, #4]
007ea6b4: ldr      r3, [r0, #4]
007ea6b8: cmp      r3, #0
007ea6bc: beq      #0x7ea870
007ea6c0: ldr      r3, [r0, #0x1c]
007ea6c4: cmp      r3, #0
007ea6c8: ble      #0x7ea870
007ea6cc: str      r1, [sp, #0x10]
007ea6d0: add      r2, sp, #0x1c
007ea6d4: mov      r1, #0
007ea6d8: str      r1, [sp, #0x14]
007ea6dc: str      r2, [sp, #8]
007ea6e0: ldr      r1, [sp, #4]
007ea6e4: ldr      r2, [sp, #0x14]
007ea6e8: ldr      r3, [r1, #0xc]
007ea6ec: ldr      r3, [r3, r2, lsl #2]
007ea6f0: ldr      r2, [r3, #0x34]
007ea6f4: mov      r0, r3
007ea6f8: str      r2, [sp, #0x1c]
007ea6fc: ldr      r1, [r3, #0x38]
007ea700: str      r1, [sp, #0x20]
007ea704: ldr      r1, [r3, #8]
007ea708: str      r1, [sp, #0xc]
007ea70c: ldr      r3, [r3]
007ea710: ldr      r4, [r2, #0xc]
007ea714: mov      lr, pc
007ea718: ldr      pc, [r3]
007ea71c: ldr      r2, [sp, #0xc]
007ea720: cmp      r2, #0
007ea724: ble      #0x7ea848
007ea728: mov      r3, #0
007ea72c: mov      r7, r0
007ea730: str      r3, [sp]
007ea734: ldr      r3, [r7, #0x40]
007ea738: str      r3, [sp, #0x2c]
007ea73c: ldr      r3, [r7, #0x44]
007ea740: str      r3, [sp, #0x30]
007ea744: ldr      r3, [r7, #0x48]
007ea748: cmp      r3, #0
007ea74c: ble      #0x7ea82c
007ea750: ldr      r6, [sp, #0x10]
007ea754: mov      r5, r7
007ea758: mov      r8, #0
007ea75c: ldr      sb, [r5]
007ea760: ldr      r1, [r4, #0xc]
007ea764: ldr      sl, [r5, #4]
007ea768: mov      r0, sb
007ea76c: bl       #0x30ed6c
007ea770: ldr      r1, [r4, #0x14]
007ea774: mov      fp, r0
007ea778: mov      r0, sl
007ea77c: bl       #0x30ed6c
007ea780: mov      r1, r0
007ea784: mov      r0, fp
007ea788: bl       #0x30eba4
007ea78c: ldr      r1, [r4, #0x10]
007ea790: mov      fp, r0
007ea794: mov      r0, sb
007ea798: bl       #0x30ed6c
007ea79c: ldr      r1, [r4, #0x18]
007ea7a0: mov      sb, r0
007ea7a4: mov      r0, sl
007ea7a8: bl       #0x30ed6c
007ea7ac: mov      r1, r0
007ea7b0: mov      r0, sb
007ea7b4: bl       #0x30eba4
007ea7b8: ldr      r1, [r4, #4]
007ea7bc: mov      sl, r0
007ea7c0: mov      r0, fp
007ea7c4: bl       #0x30eba4
007ea7c8: ldr      r1, [r4, #8]
007ea7cc: mov      sb, r0
007ea7d0: mov      r0, sl
007ea7d4: bl       #0x30eba4
007ea7d8: str      sb, [sp, #0x24]
007ea7dc: str      r0, [sp, #0x28]
007ea7e0: ldr      r2, [r6, #0x20]
007ea7e4: ldr      r1, [sp, #4]
007ea7e8: add      r8, r8, #1
007ea7ec: ldr      r3, [r1, #4]
007ea7f0: str      r2, [sp, #0x34]
007ea7f4: ldr      r2, [r6, #0x24]
007ea7f8: mov      r0, r3
007ea7fc: ldr      r1, [sp, #8]
007ea800: str      r2, [sp, #0x38]
007ea804: ldr      r2, [r5, #0x1c]
007ea808: add      r6, r6, #0x40
007ea80c: add      r5, r5, #0x20
007ea810: str      r2, [sp, #0x3c]
007ea814: ldr      r3, [r3]
007ea818: mov      lr, pc
007ea81c: ldr      pc, [r3, #0x14]
007ea820: ldr      r3, [r7, #0x48]
007ea824: cmp      r3, r8
007ea828: bgt      #0x7ea75c
007ea82c: ldr      r2, [sp]
007ea830: ldr      r3, [sp, #0xc]
007ea834: add      r7, r7, #0x4c
007ea838: add      r2, r2, #1
007ea83c: cmp      r2, r3
007ea840: str      r2, [sp]
007ea844: bne      #0x7ea734
007ea848: ldr      r1, [sp, #4]
007ea84c: ldr      r2, [sp, #0x14]
007ea850: ldr      r3, [r1, #0x1c]
007ea854: ldr      r1, [sp, #0x10]
007ea858: add      r2, r2, #1
007ea85c: cmp      r3, r2
007ea860: add      r1, r1, #0xa0
007ea864: str      r2, [sp, #0x14]
007ea868: str      r1, [sp, #0x10]
007ea86c: bgt      #0x7ea6e0
007ea870: add      sp, sp, #0x44
007ea874: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK13b2CircleShape11ComputeAABBEP6b2AABBRK7b2XForm
007e9320: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e9324: ldr      r5, [r0, #0x30]
007e9328: ldr      r8, [r0, #0x34]
007e932c: mov      r7, r1
007e9330: mov      r4, r0
007e9334: ldr      r1, [r2, #8]
007e9338: mov      r0, r5
007e933c: mov      r6, r2
007e9340: bl       #0x30ed6c
007e9344: ldr      r1, [r6, #0x10]
007e9348: mov      sl, r0
007e934c: mov      r0, r8
007e9350: bl       #0x30ed6c
007e9354: mov      r1, r0
007e9358: mov      r0, sl
007e935c: bl       #0x30eba4
007e9360: ldr      r1, [r6, #0xc]
007e9364: mov      sl, r0
007e9368: mov      r0, r5
007e936c: bl       #0x30ed6c
007e9370: ldr      r1, [r6, #0x14]
007e9374: mov      r5, r0
007e9378: mov      r0, r8
007e937c: bl       #0x30ed6c
007e9380: mov      r1, r0
007e9384: mov      r0, r5
007e9388: bl       #0x30eba4
007e938c: ldr      r1, [r6]
007e9390: mov      r5, r0
007e9394: mov      r0, sl
007e9398: bl       #0x30eba4
007e939c: ldr      r1, [r6, #4]
007e93a0: mov      r8, r0
007e93a4: mov      r0, r5
007e93a8: bl       #0x30eba4
007e93ac: ldr      r6, [r4, #0x38]
007e93b0: mov      r5, r0
007e93b4: mov      r0, r8
007e93b8: mov      r1, r6
007e93bc: bl       #0x30e3ac
007e93c0: mov      r1, r6
007e93c4: str      r0, [r7]
007e93c8: mov      r0, r5
007e93cc: bl       #0x30e3ac
007e93d0: str      r0, [r7, #4]
007e93d4: ldr      r4, [r4, #0x38]
007e93d8: mov      r1, r8
007e93dc: mov      r0, r4
007e93e0: bl       #0x30eba4
007e93e4: mov      r1, r5
007e93e8: str      r0, [r7, #8]
007e93ec: mov      r0, r4
007e93f0: bl       #0x30eba4
007e93f4: str      r0, [r7, #0xc]
007e93f8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
