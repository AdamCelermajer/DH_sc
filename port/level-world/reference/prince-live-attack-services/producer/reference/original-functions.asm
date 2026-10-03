
# _ZNK9Character18GetInteractionTypeEP10GameObject
003a47e8: push     {r4, r5, r6, lr}
003a47ec: subs     r5, r1, #0
003a47f0: mov      r4, r0
003a47f4: beq      #0x3a4808
003a47f8: add      r0, r0, #0x3c8
003a47fc: bl       #0x3d574c
003a4800: cmp      r0, #0
003a4804: bne      #0x3a4820
003a4808: mov      r0, r4
003a480c: bl       #0x3a30ac
003a4810: cmp      r0, #0
003a4814: beq      #0x3a4840
003a4818: mvn      r0, #0
003a481c: pop      {r4, r5, r6, pc}
003a4820: mov      r0, r5
003a4824: ldr      r3, [r5]
003a4828: mov      lr, pc
003a482c: ldr      pc, [r3, #0x28]
003a4830: cmp      r0, #0
003a4834: bne      #0x3a4808
003a4838: mov      r0, #8
003a483c: pop      {r4, r5, r6, pc}
003a4840: ldr      r3, [r4]
003a4844: mov      r0, r4
003a4848: mov      lr, pc
003a484c: ldr      pc, [r3, #0x34]
003a4850: cmp      r0, #0
003a4854: bne      #0x3a4818
003a4858: mov      r0, r4
003a485c: bl       #0x3a3064
003a4860: cmp      r0, #0
003a4864: bne      #0x3a4838
003a4868: add      r0, r0, #3
003a486c: pop      {r4, r5, r6, pc}

# _ZNK9Character13IsInteractiveEP10GameObject
003a4870: push     {r4, r5, r6, lr}
003a4874: ldr      r3, [r0]
003a4878: mov      r4, r0
003a487c: mov      r5, r1
003a4880: mov      lr, pc
003a4884: ldr      pc, [r3, #0x34]
003a4888: cmp      r0, #0
003a488c: beq      #0x3a48ac
003a4890: cmp      r5, #0
003a4894: beq      #0x3a48ac
003a4898: mov      r1, r5
003a489c: add      r0, r4, #0x3c8
003a48a0: bl       #0x3d511c
003a48a4: cmp      r0, #0
003a48a8: bne      #0x3a48cc
003a48ac: ldrb     r3, [r4, #0x81]
003a48b0: cmp      r3, #0
003a48b4: bne      #0x3a48c4
003a48b8: ldrb     r3, [r4, #0x8a]
003a48bc: cmp      r3, #0
003a48c0: bne      #0x3a48e4
003a48c4: mov      r0, #0
003a48c8: pop      {r4, r5, r6, pc}
003a48cc: mov      r0, r4
003a48d0: bl       #0x3a3064
003a48d4: cmp      r0, #0
003a48d8: bne      #0x3a48ac
003a48dc: mov      r0, #1
003a48e0: pop      {r4, r5, r6, pc}
003a48e4: mov      r0, r4
003a48e8: bl       #0x3a3094
003a48ec: cmp      r0, #0
003a48f0: bne      #0x3a48c4
003a48f4: mov      r0, r4
003a48f8: bl       #0x3a30ac
003a48fc: cmp      r0, #0
003a4900: bne      #0x3a48c4
003a4904: ldr      r3, [r4]
003a4908: mov      r0, r4
003a490c: mov      lr, pc
003a4910: ldr      pc, [r3, #0x34]
003a4914: cmp      r0, #0
003a4918: bne      #0x3a48c4
003a491c: ldr      r3, [r4, #0x520]
003a4920: tst      r3, #0x2000
003a4924: beq      #0x3a48c4
003a4928: ldrb     r0, [r4, #0x415]
003a492c: pop      {r4, r5, r6, pc}

# _ZN14ObjectSearcher14RoomObjectList5ResetEv
004a18fc: ldr      r1, [r0, #4]
004a1900: ldr      r2, [r1]
004a1904: str      r1, [r0, #0xc]
004a1908: str      r2, [r0, #8]
004a190c: ldr      r2, [r2, #8]
004a1910: ldr      r2, [r2]
004a1914: str      r2, [r0, #0x10]
004a1918: b        #0x4a18a8

# _ZN14ObjectSearcher14RoomObjectList7GetCharEv
004a1cfc: push     {r4, lr}
004a1d00: sub      sp, sp, #0x10
004a1d04: ldr      r3, [r0]
004a1d08: mov      lr, pc
004a1d0c: ldr      pc, [r3, #0x18]
004a1d10: add      r4, sp, #4
004a1d14: mov      r1, r0
004a1d18: mov      r0, r4
004a1d1c: bl       #0x33dd2c
004a1d20: mov      r0, r4
004a1d24: bl       #0x33ff54
004a1d28: add      sp, sp, #0x10
004a1d2c: pop      {r4, pc}

# _ZNK9Character14CanRangeAttackERiS0_S0_
003a4cd0: push     {r4, r5, r6, r7}
003a4cd4: movw     r4, #0x1078
003a4cd8: mov      ip, r0
003a4cdc: ldr      r0, [r0, r4]
003a4ce0: mov      r6, r1
003a4ce4: mov      r5, r2
003a4ce8: cmn      r0, #1
003a4cec: mov      r7, r3
003a4cf0: beq      #0x3a4d28
003a4cf4: movw     r3, #0x1070
003a4cf8: ldr      r3, [ip, r3]
003a4cfc: mov      r0, #1
003a4d00: asr      r3, r3, #8
003a4d04: str      r3, [r1]
003a4d08: movw     r3, #0x1074
003a4d0c: ldr      r3, [ip, r3]
003a4d10: asr      r3, r3, #8
003a4d14: str      r3, [r2]
003a4d18: ldr      r3, [ip, r4]
003a4d1c: str      r3, [r7]
003a4d20: pop      {r4, r5, r6, r7}
003a4d24: bx       lr
003a4d28: add      r0, ip, #0x37c
003a4d2c: pop      {r4, r5, r6, r7}
003a4d30: b        #0x3ffebc
