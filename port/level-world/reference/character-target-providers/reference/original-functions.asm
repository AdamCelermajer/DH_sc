
# _ZN10ObjectBase9GetHandleEv
0033dd2c: ldr      r3, [pc, #0x34]
0033dd30: ldr      r2, [pc, #0x34]
0033dd34: push     {r4, lr}
0033dd38: add      r3, pc, r3
0033dd3c: ldr      r2, [r3, r2]
0033dd40: ldr      ip, [r1, #0x2c]
0033dd44: mov      r4, r0
0033dd48: ldr      lr, [r2, #0x38]
0033dd4c: mov      r2, #0xc
0033dd50: ldr      r3, [lr, #0x78]
0033dd54: str      r3, [ip, #8]
0033dd58: ldr      r1, [r1, #0x2c]
0033dd5c: bl       #0x30df38
0033dd60: mov      r0, r4
0033dd64: pop      {r4, pc}
0033dd68: rsbeq    r6, r5, r8, asr sp
0033dd6c: strdeq   r3, r4, [r0], -r4

# _ZNK9Character11GetCharTypeEv
003a3054: push     {r4, lr}
003a3058: bl       #0x3a3024
003a305c: ldr      r0, [r0, #0x38]
003a3060: pop      {r4, pc}

# _ZNK9Character8IsFaerieEv
003a3094: push     {r4, lr}
003a3098: bl       #0x3a3054
003a309c: cmp      r0, #3
003a30a0: movne    r0, #0
003a30a4: moveq    r0, #1
003a30a8: pop      {r4, pc}

# _ZNK9Character20GetInteractionRadiusEv
003a374c: add      r0, r0, #0x3c8
003a3750: b        #0x3d4c34

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

# _ZNK9Character11GetCharAIIdEv
003a2fec: ldr      r0, [r0, #0xffc]
003a2ff0: ldr      r3, [pc, #0x24]
003a2ff4: cmp      r0, #0
003a2ff8: add      r3, pc, r3
003a2ffc: blt      #0x3a3014
003a3000: ldr      r2, [pc, #0x18]
003a3004: ldr      r3, [r3, r2]
003a3008: ldr      r3, [r3]
003a300c: cmp      r0, r3
003a3010: bxlt     lr
003a3014: mov      r0, #8
003a3018: bx       lr

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

# _ZNK9Character9IsMonsterEv
003a3064: push     {r4, lr}
003a3068: bl       #0x3a3054
003a306c: cmp      r0, #4
003a3070: movne    r0, #0
003a3074: moveq    r0, #1
003a3078: pop      {r4, pc}

# _ZNK9Character10IsSummonedEv
003a30ac: push     {r4, lr}
003a30b0: bl       #0x3a3054
003a30b4: cmp      r0, #5
003a30b8: movne    r0, #0
003a30bc: moveq    r0, #1
003a30c0: pop      {r4, pc}

# _ZN12ObjectHandlecvP9CharacterEv
0033ff54: push     {r4, lr}
0033ff58: mov      r1, #0
0033ff5c: bl       #0x33fdc0
0033ff60: subs     r4, r0, #0
0033ff64: bne      #0x33ff70
0033ff68: mov      r0, #0
0033ff6c: pop      {r4, pc}
0033ff70: ldr      r3, [r4]
0033ff74: mov      lr, pc
0033ff78: ldr      pc, [r3, #0x24]
0033ff7c: cmp      r0, #0
0033ff80: beq      #0x33ff68
0033ff84: mov      r0, r4
0033ff88: pop      {r4, pc}

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

# _ZNK9Character9GetCharAIEv
003a3024: ldr      r3, [pc, #0x20]
003a3028: ldr      r2, [pc, #0x20]
003a302c: push     {r4, lr}
003a3030: add      r3, pc, r3
003a3034: ldr      r2, [r3, r2]
003a3038: ldr      r4, [r2]
003a303c: bl       #0x3a2fec
003a3040: mov      r3, #0x44
003a3044: mla      r0, r3, r0, r4
003a3048: pop      {r4, pc}
003a304c: subseq   r1, pc, r0, ror #20
003a3050: andeq    r0, r0, r8, asr r7

# _ZNK9Character8IsPlayerEv
003a49f0: push     {r4, r5, r6, lr}
003a49f4: mov      r5, r0
003a49f8: bl       #0x3a3054
003a49fc: cmp      r0, #0
003a4a00: beq      #0x3a4a14
003a4a04: cmp      r0, #1
003a4a08: movne    r0, #0
003a4a0c: moveq    r0, #1
003a4a10: pop      {r4, r5, r6, pc}
003a4a14: ldr      r4, [r5, #0x44]
003a4a18: ldr      r1, [pc, #0x18]
003a4a1c: mov      r0, r4
003a4a20: add      r1, pc, r1
003a4a24: bl       #0x30ebd4
003a4a28: cmp      r4, r0
003a4a2c: movne    r0, #0
003a4a30: moveq    r0, #1
003a4a34: pop      {r4, r5, r6, pc}
003a4a38: subseq   lr, r1, r8, asr #14

# _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0033fc88: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0033fc8c: ldr      r5, [pc, #0x124]
0033fc90: ldr      sb, [pc, #0x124]
0033fc94: ldr      r4, [r0, #4]
0033fc98: add      r5, pc, r5
0033fc9c: ldr      r3, [r5, sb]
0033fca0: sub      sp, sp, #0x48
0033fca4: cmp      r4, #0
0033fca8: ldr      r3, [r3]
0033fcac: mov      r8, r0
0033fcb0: str      r3, [sp, #0x44]
0033fcb4: beq      #0x33fda8
0033fcb8: ldr      r7, [r1]
0033fcbc: mov      r2, r0
0033fcc0: b        #0x33fccc
0033fcc4: mov      r2, r4
0033fcc8: mov      r4, r3
0033fccc: ldr      r3, [r4, #0x10]
0033fcd0: cmp      r3, r7
0033fcd4: ldrlt    r3, [r4, #0xc]
0033fcd8: ldrge    r3, [r4, #8]
0033fcdc: movlt    r4, r2
0033fce0: cmp      r3, #0
0033fce4: bne      #0x33fcc4
0033fce8: cmp      r8, r4
0033fcec: beq      #0x33fd20
0033fcf0: ldr      r3, [r4, #0x10]
0033fcf4: mov      r0, r4
0033fcf8: cmp      r3, r7
0033fcfc: bgt      #0x33fd20
0033fd00: ldr      r3, [r5, sb]
0033fd04: ldr      r2, [sp, #0x44]
0033fd08: add      r0, r0, #0x14
0033fd0c: ldr      r3, [r3]
0033fd10: cmp      r2, r3
0033fd14: bne      #0x33fdb4
0033fd18: add      sp, sp, #0x48
0033fd1c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0033fd20: add      r6, sp, #0x28
0033fd24: mov      r0, r6
0033fd28: mov      r1, #0x10
0033fd2c: str      r6, [sp, #0x38]
0033fd30: str      r6, [sp, #0x3c]
0033fd34: bl       #0x31167c
0033fd38: ldr      r2, [sp, #0x38]
0033fd3c: mov      r3, #0
0033fd40: add      sl, sp, #0x48
0033fd44: strb     r3, [r2]
0033fd48: str      r7, [sl, #-0x40]!
0033fd4c: add      r7, sl, #4
0033fd50: mov      r0, r7
0033fd54: ldr      r1, [sp, #0x3c]
0033fd58: ldr      r2, [sp, #0x38]
0033fd5c: str      r3, [sp, #0x40]
0033fd60: str      r7, [sp, #0x1c]
0033fd64: str      r7, [sp, #0x20]
0033fd68: bl       #0x3116e8
0033fd6c: ldr      ip, [sp, #0x40]
0033fd70: mov      r1, r8
0033fd74: mov      r3, sl
0033fd78: mov      r2, sp
0033fd7c: add      r0, sp, #4
0033fd80: str      ip, [sp, #0x24]
0033fd84: str      r4, [sp]
0033fd88: bl       #0x33f914
0033fd8c: ldr      r4, [sp, #4]
0033fd90: mov      r0, r7
0033fd94: bl       #0x3139ac
0033fd98: mov      r0, r6
0033fd9c: bl       #0x3139ac
0033fda0: mov      r0, r4
0033fda4: b        #0x33fd00
0033fda8: ldr      r7, [r1]
0033fdac: mov      r4, r0
0033fdb0: b        #0x33fce8
0033fdb4: bl       #0x30e310

# _ZNK9Character11IsCharacterEv
003a2e1c: mov      r0, #1
003a2e20: bx       lr

# _ZNK9Character9IsZonableEv
003a36e4: push     {r4, lr}
003a36e8: ldr      r3, [r0]
003a36ec: mov      r4, r0
003a36f0: mov      lr, pc
003a36f4: ldr      pc, [r3, #0x28]
003a36f8: cmp      r0, #0
003a36fc: beq      #0x3a3708
003a3700: mov      r0, #0
003a3704: pop      {r4, pc}
003a3708: mov      r0, r4
003a370c: bl       #0x3a3094
003a3710: cmp      r0, #0
003a3714: bne      #0x3a3700
003a3718: mov      r0, r4
003a371c: pop      {r4, lr}
003a3720: b        #0x38ab60

# _ZN12ObjectHandle9GetObjectEb
0033fdc0: push     {r4, r5, r6, r7, r8, lr}
0033fdc4: ldr      r4, [r0]
0033fdc8: ldr      r5, [pc, #0xc0]
0033fdcc: sub      sp, sp, #8
0033fdd0: cmp      r4, #0
0033fdd4: mov      r6, r0
0033fdd8: mov      r7, r1
0033fddc: add      r5, pc, r5
0033fde0: beq      #0x33fe20
0033fde4: ldr      r3, [pc, #0xa8]
0033fde8: ldr      r4, [r0, #4]
0033fdec: ldr      r3, [r5, r3]
0033fdf0: cmp      r4, #0
0033fdf4: ldr      r0, [r3, #0x38]
0033fdf8: ldr      r8, [r0, #0x78]
0033fdfc: beq      #0x33fe0c
0033fe00: ldr      r3, [r6, #8]
0033fe04: cmp      r3, r8
0033fe08: beq      #0x33fe20
0033fe0c: add      r0, r0, #0xc
0033fe10: mov      r1, r6
0033fe14: bl       #0x33fc88
0033fe18: ldr      r4, [r0, #0x18]
0033fe1c: stmib    r6, {r4, r8}
0033fe20: cmp      r7, #0
0033fe24: beq      #0x33fe30
0033fe28: cmp      r4, #0
0033fe2c: beq      #0x33fe3c
0033fe30: mov      r0, r4
0033fe34: add      sp, sp, #8
0033fe38: pop      {r4, r5, r6, r7, r8, pc}
0033fe3c: ldr      r3, [pc, #0x54]
0033fe40: ldr      r3, [r5, r3]
0033fe44: ldr      r3, [r3]
0033fe48: cmp      r3, #2
0033fe4c: streq    r4, [r4]
0033fe50: beq      #0x33fe30
0033fe54: cmp      r3, #1
0033fe58: bne      #0x33fe30
0033fe5c: ldr      r0, [pc, #0x38]
0033fe60: ldr      r1, [pc, #0x38]
0033fe64: ldr      r2, [pc, #0x38]
0033fe68: ldr      r0, [r5, r0]
0033fe6c: ldr      r3, [pc, #0x34]
0033fe70: mov      ip, #0x31
0033fe74: add      r1, pc, r1
0033fe78: add      r2, pc, r2
0033fe7c: add      r3, pc, r3
0033fe80: add      r0, r0, #0xa8
0033fe84: str      ip, [sp]
0033fe88: bl       #0x30e004
0033fe8c: b        #0x33fe30
0033fe90: strhteq  r4, [r5], #-0xc4
0033fe94: strdeq   r3, r4, [r0], -r4
0033fe98: andeq    r3, r0, r0, asr #19
0033fe9c: andeq    r1, r0, r0, asr #19
0033fea0: subseq   lr, r7, r4, ror #10

# _ZNK10GameObject13MeetConditionEv
0038ab60: mov      r0, #1
0038ab64: bx       lr
