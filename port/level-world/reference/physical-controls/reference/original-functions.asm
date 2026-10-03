
# _ZN14PhysicalObject17addLinearVelocityEffff
0046e864: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0046e868: mov      r4, r1
0046e86c: mov      r5, r0
0046e870: mov      r1, #0
0046e874: mov      r0, r4
0046e878: mov      r6, r2
0046e87c: mov      sl, r3
0046e880: bl       #0x30df8c
0046e884: cmp      r0, #0
0046e888: ldr      r8, [sp, #0x20]
0046e88c: beq      #0x46e8a4
0046e890: mov      r0, r6
0046e894: mov      r1, #0
0046e898: bl       #0x30df8c
0046e89c: cmp      r0, #0
0046e8a0: bne      #0x46e8bc
0046e8a4: ldr      r3, [r5, #0x14]
0046e8a8: mov      r1, #0
0046e8ac: ldrh     r2, [r3]
0046e8b0: str      r1, [r3, #0x8c]
0046e8b4: bic      r2, r2, #8
0046e8b8: strh     r2, [r3]
0046e8bc: ldr      r5, [r5, #0x14]
0046e8c0: mov      r0, r4
0046e8c4: ldr      r1, [r5, #0x40]
0046e8c8: bl       #0x30eba4
0046e8cc: ldr      r4, [r5, #0x44]
0046e8d0: mov      r7, r0
0046e8d4: mov      r0, r6
0046e8d8: mov      r1, r4
0046e8dc: bl       #0x30eba4
0046e8e0: mov      r1, sl
0046e8e4: mov      r4, r0
0046e8e8: mov      r0, r7
0046e8ec: bl       #0x30e2f8
0046e8f0: mov      r1, r8
0046e8f4: cmp      r0, #0
0046e8f8: mov      r0, r4
0046e8fc: movne    r7, sl
0046e900: bl       #0x30e2f8
0046e904: cmp      r0, #0
0046e908: movne    r4, r8
0046e90c: str      r7, [r5, #0x40]
0046e910: str      r4, [r5, #0x44]
0046e914: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK14PhysicalObject9getRadiusEv
0046e750: push     {r4, lr}
0046e754: mov      r1, #0x42000000
0046e758: ldr      r0, [r0, #0xc]
0046e75c: add      r1, r1, #0xc80000
0046e760: bl       #0x30ed6c
0046e764: pop      {r4, pc}

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

# _ZNK14PhysicalObject8getAngleEv
0046e858: ldr      r3, [r0, #0x14]
0046e85c: ldr      r0, [r3, #0x38]
0046e860: bx       lr

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

# _ZN14PhysicalObject18setAngularVelocityEf
0046e978: ldr      r3, [r0, #0x14]
0046e97c: mov      ip, #0
0046e980: ldrh     r2, [r3]
0046e984: str      ip, [r3, #0x8c]
0046e988: bic      r2, r2, #8
0046e98c: strh     r2, [r3]
0046e990: ldr      r3, [r0, #0x14]
0046e994: str      r1, [r3, #0x48]
0046e998: bx       lr

# _ZN10GameObject4StopEv
003938f8: push     {r4, r5, r6, lr}
003938fc: ldr      r5, [pc, #0xe0]
00393900: ldr      r3, [pc, #0xe0]
00393904: mov      r4, r0
00393908: add      r5, pc, r5
0039390c: ldr      r0, [r5, r3]
00393910: add      r1, r4, #0x1c8
00393914: bl       #0x52aae4
00393918: ldr      r3, [pc, #0xcc]
0039391c: ldr      r1, [r4, #0x168]
00393920: ldr      ip, [r4, #0x160]
00393924: ldr      r0, [r4, #0x164]
00393928: ldr      r3, [r5, r3]
0039392c: mov      r2, #0
00393930: str      r1, [r4, #0x1b0]
00393934: str      ip, [r4, #0x1a8]
00393938: str      r0, [r4, #0x1ac]
0039393c: strb     r2, [r4, #0x1b5]
00393940: strb     r2, [r4, #0x1b4]
00393944: ldr      r2, [r3]
00393948: ldr      r1, [r4, #0x2dc]
0039394c: str      r2, [r4, #0x1b8]
00393950: ldr      r2, [r3, #4]
00393954: cmp      r1, #0
00393958: str      r2, [r4, #0x1bc]
0039395c: ldr      r3, [r3, #8]
00393960: str      r3, [r4, #0x1c0]
00393964: beq      #0x3939e0
00393968: ldr      r3, [r4]
0039396c: mov      r0, r4
00393970: mov      lr, pc
00393974: ldr      pc, [r3, #0x64]
00393978: cmp      r0, #0
0039397c: beq      #0x3939e0
00393980: mov      r5, #0
00393984: mov      r2, r5
00393988: ldr      r0, [r4, #0x2dc]
0039398c: mov      r1, r5
00393990: bl       #0x46e918
00393994: ldr      r0, [r4, #0x2dc]
00393998: mov      r1, r5
0039399c: bl       #0x46e978
003939a0: ldr      r2, [r4, #0x164]
003939a4: ldr      r0, [r4, #0x2dc]
003939a8: ldr      r1, [r4, #0x160]
003939ac: bl       #0x46ea80
003939b0: ldr      r3, [r4, #0x2dc]
003939b4: ldr      r3, [r3, #0x14]
003939b8: ldrh     r2, [r3]
003939bc: str      r5, [r3, #0x54]
003939c0: str      r5, [r3, #0x8c]
003939c4: orr      r2, r2, #8
003939c8: strh     r2, [r3]
003939cc: str      r5, [r3, #0x40]
003939d0: str      r5, [r3, #0x44]
003939d4: str      r5, [r3, #0x48]
003939d8: str      r5, [r3, #0x4c]
003939dc: str      r5, [r3, #0x50]
003939e0: pop      {r4, r5, r6, pc}
003939e4: rsbeq    r1, r0, r8, lsl #3
003939e8: andeq    r1, r0, r4, lsl #4
003939ec: andeq    r3, r0, ip, lsr #30

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
