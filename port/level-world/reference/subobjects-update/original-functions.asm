
# _ZNK10GameObject15IsAtDestinationEv
0039361c: push     {r4, r5, r6, lr}
00393620: mov      r2, r0
00393624: ldr      r3, [r2, #0x200]!
00393628: mov      r4, r0
0039362c: cmp      r3, r2
00393630: beq      #0x3936a8
00393634: ldr      r3, [r3]
00393638: cmp      r2, r3
0039363c: bne      #0x393634
00393640: ldr      r1, [r4, #0x160]
00393644: ldr      r0, [r4, #0x208]
00393648: bl       #0x30e3ac
0039364c: ldr      r1, [r4, #0x164]
00393650: mov      r6, r0
00393654: ldr      r0, [r4, #0x20c]
00393658: bl       #0x30e3ac
0039365c: mov      r1, r6
00393660: mov      r5, r0
00393664: mov      r0, r6
00393668: bl       #0x30ed6c
0039366c: mov      r1, r5
00393670: mov      r4, r0
00393674: mov      r0, r5
00393678: bl       #0x30ed6c
0039367c: mov      r1, r0
00393680: mov      r0, r4
00393684: bl       #0x30eba4
00393688: mov      r1, #0x45000000
0039368c: add      r1, r1, #0xc80000
00393690: bl       #0x30e70c
00393694: cmp      r0, #0
00393698: mov      r0, #0
0039369c: movne    r0, #1
003936a0: uxtb     r0, r0
003936a4: pop      {r4, r5, r6, pc}
003936a8: ldr      r1, [r0, #0x160]
003936ac: ldr      r0, [r0, #0x1a8]
003936b0: bl       #0x30e3ac
003936b4: ldr      r1, [r4, #0x164]
003936b8: mov      r6, r0
003936bc: ldr      r0, [r4, #0x1ac]
003936c0: bl       #0x30e3ac
003936c4: mov      r1, r6
003936c8: mov      r5, r0
003936cc: mov      r0, r6
003936d0: bl       #0x30ed6c
003936d4: mov      r1, r5
003936d8: mov      r4, r0
003936dc: mov      r0, r5
003936e0: bl       #0x30ed6c
003936e4: mov      r1, r0
003936e8: mov      r0, r4
003936ec: bl       #0x30eba4
003936f0: mov      r1, #0x45000000
003936f4: add      r1, r1, #0xc80000
003936f8: bl       #0x30e70c
003936fc: cmp      r0, #0
00393700: mov      r0, #0
00393704: movne    r0, #1
00393708: uxtb     r0, r0
0039370c: pop      {r4, r5, r6, pc}

# _ZN12VisualObject11SyncScalingEv
00472860: ldr      r1, [r0, #4]
00472864: cmp      r1, #0
00472868: bxeq     lr
0047286c: add      r1, r1, #0x120
00472870: b        #0x4727ac

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

# _ZNK9Character29IsUpdatingPositionFromPhysicsEv
003a2e44: ldr      r0, [r0, #0x520]
003a2e48: ubfx     r0, r0, #1, #1
003a2e4c: bx       lr

# _ZNK9Character25IsValidatingFloorPositionEv
003a2e78: ldr      r0, [r0, #0x520]
003a2e7c: eor      r0, r0, #0x40
003a2e80: ubfx     r0, r0, #6, #1
003a2e84: bx       lr

# _ZNK9Character28IsUpdatingRotationFromVisualEv
003a2e50: ldr      r0, [r0, #0x520]
003a2e54: ubfx     r0, r0, #2, #1
003a2e58: bx       lr

# _ZN12VisualObject12SyncPositionEv
00470cb8: ldr      r1, [r0, #4]
00470cbc: cmp      r1, #0
00470cc0: bxeq     lr
00470cc4: add      r1, r1, #0x160
00470cc8: b        #0x470c24

# _ZN7Point3DIfEdVERKf
0034d04c: push     {r4, r5, r6, lr}
0034d050: mov      r4, r0
0034d054: mov      r5, r1
0034d058: ldr      r0, [r0]
0034d05c: ldr      r1, [r1]
0034d060: bl       #0x30ec94
0034d064: str      r0, [r4]
0034d068: ldr      r1, [r5]
0034d06c: ldr      r0, [r4, #4]
0034d070: bl       #0x30ec94
0034d074: str      r0, [r4, #4]
0034d078: ldr      r1, [r5]
0034d07c: ldr      r0, [r4, #8]
0034d080: bl       #0x30ec94
0034d084: str      r0, [r4, #8]
0034d088: mov      r0, r4
0034d08c: pop      {r4, r5, r6, pc}

# _ZNK9Character29IsUpdatingRotationFromPhysicsEv
003a2e5c: ldr      r0, [r0, #0x520]
003a2e60: ubfx     r0, r0, #3, #1
003a2e64: bx       lr

# _ZN14PhysicalObject17setLinearVelocityEff
0046e918: push     {r4, r5, r6, lr}
0046e91c: mov      r4, r1
0046e920: mov      r5, r0
0046e924: mov      r1, #0
0046e928: mov      r0, r4
0046e92c: mov      r6, r2
0046e930: bl       #0x30df8c
0046e934: cmp      r0, #0
0046e938: beq      #0x46e950
0046e93c: mov      r0, r6
0046e940: mov      r1, #0
0046e944: bl       #0x30df8c
0046e948: cmp      r0, #0
0046e94c: bne      #0x46e968
0046e950: ldr      r3, [r5, #0x14]
0046e954: mov      r1, #0
0046e958: ldrh     r2, [r3]
0046e95c: str      r1, [r3, #0x8c]
0046e960: bic      r2, r2, #8
0046e964: strh     r2, [r3]
0046e968: ldr      r3, [r5, #0x14]
0046e96c: str      r4, [r3, #0x40]
0046e970: str      r6, [r3, #0x44]
0046e974: pop      {r4, r5, r6, pc}

# _ZN10GameObject18UpdateAbsoluteAABBEv
0038aac8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov      r4, r0
0038aad0: ldr      sl, [r4, #0x148]
0038aad4: ldr      r0, [r0, #0x144]
0038aad8: ldr      r8, [r4, #0x14c]
0038aadc: ldr      r7, [r4, #0x150]
0038aae0: ldr      r6, [r4, #0x154]
0038aae4: ldr      r5, [r4, #0x158]
0038aae8: ldr      r1, [r4, #0x160]
0038aaec: str      r0, [r4, #0x12c]
0038aaf0: str      sl, [r4, #0x130]
0038aaf4: str      r8, [r4, #0x134]
0038aaf8: str      r7, [r4, #0x138]
0038aafc: str      r6, [r4, #0x13c]
0038ab00: str      r5, [r4, #0x140]
0038ab04: bl       #0x30eba4
0038ab08: ldr      r1, [r4, #0x164]
0038ab0c: str      r0, [r4, #0x12c]
0038ab10: mov      r0, sl
0038ab14: bl       #0x30eba4
0038ab18: ldr      r1, [r4, #0x168]
0038ab1c: str      r0, [r4, #0x130]
0038ab20: mov      r0, r8
0038ab24: bl       #0x30eba4
0038ab28: ldr      r1, [r4, #0x160]
0038ab2c: str      r0, [r4, #0x134]
0038ab30: mov      r0, r7
0038ab34: bl       #0x30eba4
0038ab38: ldr      r1, [r4, #0x164]
0038ab3c: str      r0, [r4, #0x138]
0038ab40: mov      r0, r6
0038ab44: bl       #0x30eba4
0038ab48: ldr      r1, [r4, #0x168]
0038ab4c: str      r0, [r4, #0x13c]
0038ab50: mov      r0, r5
0038ab54: bl       #0x30eba4
0038ab58: str      r0, [r4, #0x140]
0038ab5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK9Character28IsUpdatingVisualWithRotationEv
003a2e68: ldr      r0, [r0, #0x520]
003a2e6c: eor      r0, r0, #0x10
003a2e70: ubfx     r0, r0, #4, #1
003a2e74: bx       lr

# _ZN12VisualObject12SyncRotationEv
00472948: ldr      r1, [r0, #4]
0047294c: cmp      r1, #0
00472950: bxeq     lr
00472954: add      r1, r1, #0x16c
00472958: b        #0x472874

# _ZNK14PhysicalObject8getAngleEv
0046e858: ldr      r3, [r0, #0x14]
0046e85c: ldr      r0, [r3, #0x38]
0046e860: bx       lr

# _ZNK9Character28IsUpdatingPositionFromVisualEv
003a2e38: ldr      r0, [r0, #0x520]
003a2e3c: and      r0, r0, #1
003a2e40: bx       lr

# _ZNK14PhysicalObject11getPositionEv
0046e818: push     {r4, r5, r6, lr}
0046e81c: ldr      r5, [r1, #0x14]
0046e820: mov      r1, #0x42000000
0046e824: mov      r4, r0
0046e828: add      r1, r1, #0xc80000
0046e82c: ldr      r0, [r5, #8]
0046e830: bl       #0x30ed6c
0046e834: mov      r1, #0x42000000
0046e838: mov      r6, r0
0046e83c: add      r1, r1, #0xc80000
0046e840: ldr      r0, [r5, #4]
0046e844: bl       #0x30ed6c
0046e848: str      r6, [r4, #4]
0046e84c: str      r0, [r4]
0046e850: mov      r0, r4
0046e854: pop      {r4, r5, r6, pc}

# _ZN10GameObject16UpdateSubObjectsEv
003943cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003943d0: ldr      r3, [r0, #0x2d8]
003943d4: ldr      r5, [pc, #0x5c8]
003943d8: sub      sp, sp, #0x24
003943dc: cmp      r3, #0
003943e0: mov      r4, r0
003943e4: add      r5, pc, r5
003943e8: beq      #0x3943fc
003943ec: mov      r0, r3
003943f0: ldr      r3, [r3]
003943f4: mov      lr, pc
003943f8: ldr      pc, [r3, #8]
003943fc: ldr      r3, [r4, #0x2dc]
00394400: cmp      r3, #0
00394404: beq      #0x394418
00394408: mov      r0, r3
0039440c: ldr      r3, [r3]
00394410: mov      lr, pc
00394414: ldr      pc, [r3, #0x20]
00394418: ldr      r3, [r4]
0039441c: mov      r0, r4
00394420: mov      lr, pc
00394424: ldr      pc, [r3, #0x60]
00394428: ldr      r3, [r4]
0039442c: mov      r7, r0
00394430: mov      r0, r4
00394434: mov      lr, pc
00394438: ldr      pc, [r3, #0x64]
0039443c: mov      r8, r0
00394440: mov      r0, r4
00394444: bl       #0x39361c
00394448: ldr      r1, [r4, #0x2dc]
0039444c: mov      r6, r0
00394450: cmp      r1, #0
00394454: moveq    sl, r1
00394458: beq      #0x39450c
0039445c: ldr      r3, [r1, #0x14]
00394460: ldrh     r3, [r3]
00394464: tst      r3, #8
00394468: beq      #0x394780
0039446c: mov      sl, #0
00394470: cmp      r8, #0
00394474: beq      #0x394888
00394478: ldr      r1, [r4, #0x160]
0039447c: ldr      r0, [r4, #0x1a8]
00394480: bl       #0x30e3ac
00394484: ldr      r1, [r4, #0x164]
00394488: mov      fp, r0
0039448c: ldr      r0, [r4, #0x1ac]
00394490: bl       #0x30e3ac
00394494: ldr      r1, [r4, #0x168]
00394498: mov      sb, r0
0039449c: ldr      r0, [r4, #0x1b0]
003944a0: bl       #0x30e3ac
003944a4: mov      r1, fp
003944a8: mov      r8, r0
003944ac: mov      r0, fp
003944b0: str      fp, [sp, #0xc]
003944b4: str      sb, [sp, #0x10]
003944b8: str      r8, [sp, #0x14]
003944bc: bl       #0x30ed6c
003944c0: mov      r1, sb
003944c4: mov      fp, r0
003944c8: mov      r0, sb
003944cc: bl       #0x30ed6c
003944d0: mov      r1, r0
003944d4: mov      r0, fp
003944d8: bl       #0x30eba4
003944dc: mov      r1, r8
003944e0: mov      sb, r0
003944e4: mov      r0, r8
003944e8: bl       #0x30ed6c
003944ec: mov      r1, r0
003944f0: mov      r0, sb
003944f4: bl       #0x30eba4
003944f8: mov      r1, #0
003944fc: mov      r8, r0
00394500: bl       #0x30e2f8
00394504: cmp      r0, #0
00394508: bne      #0x39489c
0039450c: cmp      r7, #0
00394510: beq      #0x39468c
00394514: ldr      r0, [r4, #0x2d8]
00394518: cmp      r0, #0
0039451c: beq      #0x39452c
00394520: cmp      sl, #0
00394524: beq      #0x394664
00394528: bl       #0x470cb8
0039452c: ldr      r0, [r4, #0x2dc]
00394530: cmp      r0, #0
00394534: beq      #0x394544
00394538: ldr      r1, [r4, #0x160]
0039453c: ldr      r2, [r4, #0x164]
00394540: bl       #0x46ea80
00394544: ldr      r3, [r4]
00394548: mov      r0, r4
0039454c: mov      lr, pc
00394550: ldr      pc, [r3, #0x68]
00394554: ldr      r3, [r4]
00394558: mov      r7, r0
0039455c: mov      r0, r4
00394560: mov      lr, pc
00394564: ldr      pc, [r3, #0x6c]
00394568: cmp      r7, #0
0039456c: beq      #0x394634
00394570: ldr      r3, [r4, #0x2d8]
00394574: cmp      r3, #0
00394578: beq      #0x394634
0039457c: mov      r0, r3
00394580: bl       #0x470ccc
00394584: ldr      r3, [r4, #0x2d8]
00394588: cmp      r3, #0
0039458c: beq      #0x3945c0
00394590: ldr      r3, [r4]
00394594: mov      r0, r4
00394598: mov      lr, pc
0039459c: ldr      pc, [r3, #0x70]
003945a0: cmp      r0, #0
003945a4: beq      #0x3945b0
003945a8: ldr      r0, [r4, #0x2d8]
003945ac: bl       #0x472948
003945b0: ldr      r0, [r4, #0x2d8]
003945b4: cmp      r0, #0
003945b8: beq      #0x3945c0
003945bc: bl       #0x472860
003945c0: ldr      r3, [r4]
003945c4: mov      r0, r4
003945c8: mov      lr, pc
003945cc: ldr      pc, [r3, #0x74]
003945d0: cmp      r0, #0
003945d4: beq      #0x394824
003945d8: cmp      r6, #0
003945dc: beq      #0x3945f8
003945e0: ldr      r1, [r4, #0x160]
003945e4: ldr      r2, [r4, #0x164]
003945e8: ldr      r3, [r4, #0x168]
003945ec: str      r1, [r4, #0x1a8]
003945f0: str      r2, [r4, #0x1ac]
003945f4: str      r3, [r4, #0x1b0]
003945f8: mov      r0, r4
003945fc: bl       #0x38aac8
00394600: ldr      r3, [r4, #0x2e0]
00394604: cmp      r3, #0
00394608: beq      #0x39462c
0039460c: mov      r0, r3
00394610: ldr      r3, [r3]
00394614: mov      lr, pc
00394618: ldr      pc, [r3, #0xc]
0039461c: ldr      r3, [r4, #0x2e0]
00394620: ldr      r2, [r3, #4]
00394624: cmp      r2, #2
00394628: beq      #0x3946a0
0039462c: add      sp, sp, #0x24
00394630: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00394634: cmp      r0, #0
00394638: beq      #0x394584
0039463c: ldr      r0, [r4, #0x2dc]
00394640: cmp      r0, #0
00394644: beq      #0x394584
00394648: ldr      r3, [r0, #0x14]
0039464c: ldrh     r3, [r3]
00394650: tst      r3, #8
00394654: bne      #0x394584
00394658: bl       #0x46e858
0039465c: str      r0, [r4, #0x174]
00394660: b        #0x394584
00394664: bl       #0x47124c
00394668: ldr      r3, [r4, #0x2dc]
0039466c: cmp      r3, #0
00394670: beq      #0x39468c
00394674: ldr      r3, [r3, #0x14]
00394678: mov      r1, #0
0039467c: ldrh     r2, [r3]
00394680: str      r1, [r3, #0x8c]
00394684: bic      r2, r2, #8
00394688: strh     r2, [r3]
0039468c: ldr      r0, [r4, #0x2d8]
00394690: cmp      r0, #0
00394694: beq      #0x39452c
00394698: bl       #0x470cb8
0039469c: b        #0x39452c
003946a0: ldr      r3, [r3, #0x3c]
003946a4: cmp      r3, #3
003946a8: beq      #0x394964
003946ac: cmp      r3, #4
003946b0: beq      #0x3946bc
003946b4: cmp      r3, #1
003946b8: bne      #0x39462c
003946bc: ldr      r3, [pc, #0x2e4]
003946c0: ldr      r0, [r5, r3]
003946c4: bl       #0x31f594
003946c8: ldr      r5, [r0, #0x128]
003946cc: ldrb     r3, [r5, #0x25]
003946d0: cmp      r3, #0
003946d4: beq      #0x39462c
003946d8: ldr      r4, [r4, #0x2e0]
003946dc: mov      r0, sp
003946e0: ldr      r1, [r5, #4]
003946e4: bl       #0x597180
003946e8: ldr      r1, [sp]
003946ec: ldr      r0, [r4, #0xc]
003946f0: bl       #0x30e3ac
003946f4: ldr      r1, [sp, #4]
003946f8: mov      r8, r0
003946fc: ldr      r0, [r4, #0x10]
00394700: bl       #0x30e3ac
00394704: ldr      r1, [sp, #8]
00394708: mov      r7, r0
0039470c: ldr      r0, [r4, #0x14]
00394710: bl       #0x30e3ac
00394714: mov      r1, r8
00394718: mov      r6, r0
0039471c: mov      r0, r8
00394720: bl       #0x30ed6c
00394724: mov      r1, r7
00394728: mov      r4, r0
0039472c: mov      r0, r7
00394730: bl       #0x30ed6c
00394734: mov      r1, r0
00394738: mov      r0, r4
0039473c: bl       #0x30eba4
00394740: mov      r1, r6
00394744: mov      r4, r0
00394748: mov      r0, r6
0039474c: bl       #0x30ed6c
00394750: mov      r1, r0
00394754: mov      r0, r4
00394758: bl       #0x30eba4
0039475c: mov      r1, #0x40000000
00394760: add      r1, r1, #0x400000
00394764: bl       #0x30e9ac
00394768: cmp      r0, #0
0039476c: beq      #0x39462c
00394770: mov      r0, r5
00394774: mov      r1, #0
00394778: bl       #0x41161c
0039477c: b        #0x39462c
00394780: add      r0, sp, #0x18
00394784: bl       #0x46e818
00394788: ldr      sl, [sp, #0x18]
0039478c: ldr      r1, [r4, #0x160]
00394790: mov      r0, sl
00394794: bl       #0x30e3ac
00394798: mov      r1, #0x3f800000
0039479c: bic      r0, r0, #0x80000000
003947a0: bl       #0x30e2f8
003947a4: cmp      r0, #0
003947a8: bne      #0x3947cc
003947ac: ldr      r1, [r4, #0x164]
003947b0: ldr      r0, [sp, #0x1c]
003947b4: bl       #0x30e3ac
003947b8: mov      r1, #0x3f800000
003947bc: bic      r0, r0, #0x80000000
003947c0: bl       #0x30e2f8
003947c4: cmp      r0, #0
003947c8: beq      #0x39446c
003947cc: ldr      r2, [sp, #0x1c]
003947d0: ldr      r3, [r4]
003947d4: str      sl, [r4, #0x160]
003947d8: str      r2, [r4, #0x164]
003947dc: mov      r0, r4
003947e0: mov      lr, pc
003947e4: ldr      pc, [r3, #0x74]
003947e8: cmp      r0, #1
003947ec: movne    sl, #1
003947f0: bne      #0x394470
003947f4: ldr      r3, [pc, #0x1b0]
003947f8: add      r1, r4, #0x160
003947fc: add      r2, r4, #0x1c8
00394800: ldr      r0, [r5, r3]
00394804: bl       #0x525d84
00394808: subs     sl, r0, #0
0039480c: bne      #0x394470
00394810: ldr      r0, [r4, #0x2dc]
00394814: ldr      r1, [r4, #0x160]
00394818: ldr      r2, [r4, #0x164]
0039481c: bl       #0x46ea80
00394820: b        #0x394470
00394824: ldr      r3, [r4]
00394828: mov      r0, r4
0039482c: mov      lr, pc
00394830: ldr      pc, [r3, #0x78]
00394834: cmp      r0, #0
00394838: bne      #0x39491c
0039483c: add      r7, r4, #0x160
00394840: ldr      r3, [pc, #0x164]
00394844: mov      r1, r7
00394848: add      r2, r4, #0x1c8
0039484c: ldr      r0, [r5, r3]
00394850: bl       #0x525d84
00394854: cmp      r0, #0
00394858: bne      #0x394874
0039485c: ldr      r0, [r4, #0x2dc]
00394860: cmp      r0, #0
00394864: beq      #0x394874
00394868: ldr      r1, [r4, #0x160]
0039486c: ldr      r2, [r4, #0x164]
00394870: bl       #0x46ea80
00394874: ldr      r0, [r4, #0x2d8]
00394878: cmp      r0, #0
0039487c: beq      #0x3945d8
00394880: bl       #0x470cb8
00394884: b        #0x3945d8
00394888: mov      r1, #0
0039488c: ldr      r0, [r4, #0x2dc]
00394890: mov      r2, r1
00394894: bl       #0x46e918
00394898: b        #0x39450c
0039489c: ldr      r3, [r4]
003948a0: mov      r0, r4
003948a4: mov      lr, pc
003948a8: ldr      pc, [r3, #0xa8]
003948ac: mov      r1, #0x43000000
003948b0: mov      sb, r0
003948b4: add      r1, r1, #0xc80000
003948b8: mov      r0, r8
003948bc: bl       #0x30e2f8
003948c0: cmp      r0, #0
003948c4: beq      #0x394980
003948c8: add      r0, sp, #0xc
003948cc: bl       #0x34d0b0
003948d0: ldr      r1, [sp, #0xc]
003948d4: mov      r0, sb
003948d8: bl       #0x30ed6c
003948dc: ldr      r1, [sp, #0x10]
003948e0: mov      fp, r0
003948e4: mov      r0, sb
003948e8: str      fp, [sp, #0xc]
003948ec: bl       #0x30ed6c
003948f0: mov      r1, sb
003948f4: mov      r8, r0
003948f8: ldr      r0, [sp, #0x14]
003948fc: str      r8, [sp, #0x10]
00394900: bl       #0x30ed6c
00394904: str      r0, [sp, #0x14]
00394908: mov      r1, fp
0039490c: mov      r2, r8
00394910: ldr      r0, [r4, #0x2dc]
00394914: bl       #0x46e918
00394918: b        #0x39450c
0039491c: ldr      r3, [pc, #0x84]
00394920: ldr      r0, [r5, r3]
00394924: bl       #0x31f594
00394928: cmp      r0, #0
0039492c: beq      #0x39483c
00394930: add      r7, r4, #0x160
00394934: mov      r1, r7
00394938: add      r2, r4, #0x1b8
0039493c: bl       #0x3f94f4
00394940: cmp      r0, #0
00394944: bne      #0x394840
00394948: ldr      r1, [r4, #0x1e0]
0039494c: ldr      r2, [r4, #0x1e4]
00394950: ldr      r3, [r4, #0x1e8]
00394954: str      r1, [r4, #0x160]
00394958: str      r2, [r4, #0x164]
0039495c: str      r3, [r4, #0x168]
00394960: b        #0x39485c
00394964: ldr      r3, [pc, #0x3c]
00394968: ldr      r0, [r5, r3]
0039496c: bl       #0x31f594
00394970: mov      r1, #1
00394974: ldr      r0, [r0, #0x128]
00394978: bl       #0x41161c
0039497c: b        #0x39462c
00394980: mov      r1, #0
00394984: ldr      r0, [r4, #0x2dc]
00394988: mov      r2, r1
0039498c: bl       #0x46e918
00394990: ldr      r0, [r4, #0x2dc]
00394994: ldr      r1, [r4, #0x160]
00394998: ldr      r2, [r4, #0x164]
0039499c: bl       #0x46ea80
003949a0: b        #0x39450c
003949a4: rsbeq    r0, r0, ip, lsr #13
003949a8: strdeq   r3, r4, [r0], -r4
003949ac: andeq    r1, r0, r4, lsl #4

# _ZN12VisualObject13ApplyPositionEv
0047124c: push     {r4, r5, lr}
00471250: ldr      r3, [r0, #4]
00471254: sub      sp, sp, #0x14
00471258: mov      r4, r0
0047125c: cmp      r3, #0
00471260: beq      #0x471290
00471264: add      r5, sp, #4
00471268: mov      r3, #0
0047126c: mov      r1, r5
00471270: str      r3, [sp, #0xc]
00471274: str      r3, [sp, #4]
00471278: str      r3, [sp, #8]
0047127c: bl       #0x470be4
00471280: ldr      r0, [r4, #4]
00471284: mov      r1, r5
00471288: mov      r2, #0
0047128c: bl       #0x393db4
00471290: add      sp, sp, #0x14
00471294: pop      {r4, r5, pc}

# _ZN12VisualObject13ApplyRotationEv
00470ccc: bx       lr

# _ZN14PhysicalObject11setPositionEff
0046ea80: push     {r4, r5, lr}
0046ea84: mov      r4, r0
0046ea88: mov      r0, r1
0046ea8c: movw     r1, #0xd70a
0046ea90: sub      sp, sp, #0xc
0046ea94: movt     r1, #0x3c23
0046ea98: mov      r5, r2
0046ea9c: bl       #0x30ed6c
0046eaa0: movw     r1, #0xd70a
0046eaa4: str      r0, [sp]
0046eaa8: movt     r1, #0x3c23
0046eaac: mov      r0, r5
0046eab0: bl       #0x30ed6c
0046eab4: ldr      r3, [r4, #0x14]
0046eab8: str      r0, [sp, #4]
0046eabc: mov      r1, sp
0046eac0: mov      r0, r3
0046eac4: ldr      r2, [r3, #0x38]
0046eac8: bl       #0x7e164c
0046eacc: add      sp, sp, #0xc
0046ead0: pop      {r4, r5, pc}

# _ZNK9Character24IsValidatingCameraLimitsEv
003a2e88: push     {r4, lr}
003a2e8c: ldr      r3, [r0]
003a2e90: mov      lr, pc
003a2e94: ldr      pc, [r3, #0x28]
003a2e98: pop      {r4, pc}
