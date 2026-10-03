
# _ZNK6CharAI12AI_CanAttackEP10GameObject
003d67f4: push     {r4, r5, r6, lr}
003d67f8: subs     r5, r1, #0
003d67fc: mov      r4, r0
003d6800: beq      #0x3d687c
003d6804: mov      r0, r4
003d6808: mov      r1, r5
003d680c: bl       #0x3d574c
003d6810: cmp      r0, #0
003d6814: bne      #0x3d6820
003d6818: mov      r0, #0
003d681c: pop      {r4, r5, r6, pc}
003d6820: ldr      r0, [r4, #4]
003d6824: add      r0, r0, #0x37c
003d6828: bl       #0x3ffd38
003d682c: cmp      r0, #0
003d6830: beq      #0x3d6850
003d6834: mov      r0, r4
003d6838: mov      r1, r5
003d683c: bl       #0x3d6188
003d6840: cmp      r0, #0
003d6844: beq      #0x3d6850
003d6848: mov      r0, #1
003d684c: pop      {r4, r5, r6, pc}
003d6850: ldr      r3, [r4, #4]
003d6854: mov      r0, r3
003d6858: ldr      r3, [r3]
003d685c: mov      lr, pc
003d6860: ldr      pc, [r3, #0x124]
003d6864: cmp      r0, #0
003d6868: beq      #0x3d6818
003d686c: mov      r0, r4
003d6870: mov      r1, r5
003d6874: pop      {r4, r5, r6, lr}
003d6878: b        #0x3d6604
003d687c: ldr      r5, [r0, #0x40]
003d6880: cmp      r5, #0
003d6884: bne      #0x3d6804
003d6888: mov      r0, #0
003d688c: pop      {r4, r5, r6, pc}

# _ZNK6CharAI18AI_IsTargetSeekingEv
003d49d0: ldrb     r3, [r0, #0x4a]
003d49d4: cmp      r3, #0
003d49d8: ldrne    r3, [r0, #4]
003d49dc: moveq    r0, r3
003d49e0: ldrne    r0, [r3, #0x520]
003d49e4: eorne    r0, r0, #0x1000
003d49e8: ubfxne   r0, r0, #0xc, #1
003d49ec: bx       lr

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

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr
