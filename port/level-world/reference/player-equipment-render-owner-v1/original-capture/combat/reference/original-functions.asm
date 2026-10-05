
# _ZNK13ItemInventory9HasShieldEv
00400110: push     {r4, lr}
00400114: mov      r1, #2
00400118: mov      r4, r0
0040011c: bl       #0x3fc6a8
00400120: mov      r3, #0xc
00400124: mul      r3, r3, r0
00400128: ldr      r2, [r4, #0x14]
0040012c: ldr      r3, [r2, r3]
00400130: ldr      r0, [r3, #8]
00400134: cmp      r0, #0
00400138: beq      #0x400154
0040013c: ldr      r0, [r0]
00400140: bl       #0x3f9e08
00400144: ldr      r0, [r0, #0x58]
00400148: cmp      r0, #6
0040014c: movne    r0, #0
00400150: moveq    r0, #1
00400154: pop      {r4, pc}

# _ZNK14CharProperties20PROPS_GetBonusDamageEb
003df8ac: push     {r4, r5, r6, r7, r8, lr}
003df8b0: mov      r4, r0
003df8b4: ldr      r0, [r0, #4]
003df8b8: cmp      r1, #0
003df8bc: movne    r1, #2
003df8c0: moveq    r1, #1
003df8c4: add      r0, r0, #0x37c
003df8c8: bl       #0x3ffe3c
003df8cc: cmp      r0, #0
003df8d0: beq      #0x3df938
003df8d4: bl       #0x3f9e08
003df8d8: ldr      r2, [r0, #0x94]
003df8dc: cmn      r2, #1
003df8e0: beq      #0x3df938
003df8e4: add      r5, r4, #0xa90
003df8e8: add      r5, r5, #4
003df8ec: add      r2, r2, #0x53
003df8f0: mov      r1, r5
003df8f4: mov      r0, r4
003df8f8: bl       #0x3dedb4
003df8fc: mov      r7, r0
003df900: ldr      r0, [r4, #4]
003df904: mov      r1, #1
003df908: add      r0, r0, #0x37c
003df90c: bl       #0x4001a0
003df910: cmp      r0, #0
003df914: bne      #0x3df940
003df918: ldr      r3, [r4, #4]
003df91c: add      r6, r0, r7
003df920: add      r0, r3, #0x37c
003df924: bl       #0x40019c
003df928: cmp      r0, #0
003df92c: bne      #0x3df968
003df930: add      r0, r6, r0
003df934: pop      {r4, r5, r6, r7, r8, pc}
003df938: mov      r0, #0
003df93c: pop      {r4, r5, r6, r7, r8, pc}
003df940: mov      r1, r5
003df944: mov      r2, #0x5b
003df948: mov      r0, r4
003df94c: bl       #0x3dedb4
003df950: ldr      r3, [r4, #4]
003df954: add      r6, r0, r7
003df958: add      r0, r3, #0x37c
003df95c: bl       #0x40019c
003df960: cmp      r0, #0
003df964: beq      #0x3df930
003df968: mov      r0, r4
003df96c: mov      r1, r5
003df970: mov      r2, #0x5a
003df974: bl       #0x3dedb4
003df978: add      r0, r6, r0
003df97c: pop      {r4, r5, r6, r7, r8, pc}
