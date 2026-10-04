
# _ZN12CharAnimator13ANIM_SetSpeedEf
003c93fc: push     {r4, lr}
003c9400: ldr      r2, [r0, #4]
003c9404: str      r1, [r0, #0x40]
003c9408: mov      r3, r0
003c940c: ldr      r2, [r2, #0x2d8]
003c9410: cmp      r2, #0
003c9414: beq      #0x3c9440
003c9418: mov      r0, r1
003c941c: ldr      r1, [r3, #0x34]
003c9420: ldr      r4, [r2, #0x38]
003c9424: bl       #0x30ed6c
003c9428: ldr      r3, [r4]
003c942c: mov      r1, r0
003c9430: mov      r2, #0
003c9434: mov      r0, r4
003c9438: mov      lr, pc
003c943c: ldr      pc, [r3, #0x28]
003c9440: pop      {r4, pc}

# _ZNK9Character9IsMonsterEv
003a3064: push     {r4, lr}
003a3068: bl       #0x3a3054
003a306c: cmp      r0, #4
003a3070: movne    r0, #0
003a3074: moveq    r0, #1
003a3078: pop      {r4, pc}

# _ZNK9Character6IsBossEv
003a3158: push     {r4, lr}
003a315c: bl       #0x3a3024
003a3160: ldr      r0, [r0, #0x14]
003a3164: ubfx     r0, r0, #2, #1
003a3168: pop      {r4, pc}

# _ZNK9Character10IsMiniBossEv
003a3144: push     {r4, lr}
003a3148: bl       #0x3a3024
003a314c: ldr      r0, [r0, #0x14]
003a3150: ubfx     r0, r0, #1, #1
003a3154: pop      {r4, pc}

# _ZN16CharStateMachine10SM_SetAnimEi
003c0b50: ldr      r3, [r0, #0x28]
003c0b54: cmn      r3, #1
003c0b58: mvnne    r2, #0
003c0b5c: strne    r2, [r0, #0x28]
003c0b60: ldr      r0, [r0, #4]
003c0b64: moveq    r3, r1
003c0b68: mov      r1, r3
003c0b6c: add      r0, r0, #0x490
003c0b70: add      r0, r0, #0xc
003c0b74: b        #0x3cacb0

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404
