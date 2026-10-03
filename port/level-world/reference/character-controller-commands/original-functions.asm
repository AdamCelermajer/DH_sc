
# _ZNK10ObjectBase17IsRemotelyUpdatedEv
0033dd10: ldr      r3, [r0, #0x110]
0033dd14: cmn      r3, #1
0033dd18: movne    r0, #1
0033dd1c: ldrbeq   r0, [r0, #0x118]
0033dd20: bx       lr

# _ZNK10GameObject17GetTargetPositionEv
003935dc: ldr      r3, [r0, #0x180]
003935e0: cmp      r3, #0
003935e4: beq      #0x3935f8
003935e8: ldrb     r3, [r0, #0x80]
003935ec: cmp      r3, #0
003935f0: addne    r0, r0, #0x184
003935f4: bxne     lr
003935f8: add      r0, r0, #0x160
003935fc: bx       lr

# _ZThn884_N9Character9Ctrl_StopEv
003ad888: sub      r0, r0, #0x374
003ad88c: b        #0x3ad890

# _ZN9Character9Ctrl_StopEv
003ad890: push     {r4, r5, r6, lr}
003ad894: ldr      r3, [r0]
003ad898: mov      r4, r0
003ad89c: mov      lr, pc
003ad8a0: ldr      pc, [r3, #0x54]
003ad8a4: subs     r5, r0, #0
003ad8a8: beq      #0x3ad8b0
003ad8ac: pop      {r4, r5, r6, pc}
003ad8b0: mov      r0, r4
003ad8b4: bl       #0x3938f8
003ad8b8: mov      r0, r4
003ad8bc: mov      r2, r5
003ad8c0: mov      r1, #0x3f
003ad8c4: pop      {r4, r5, r6, lr}
003ad8c8: b        #0x3a4d5c

# _ZThn884_N9Character11Ctrl_LookAtEP10GameObject
003ad9a4: sub      r0, r0, #0x374
003ad9a8: b        #0x3ad9ac

# _ZN9Character11Ctrl_LookAtEP10GameObject
003ad9ac: cmp      r1, #0
003ad9b0: push     {r4, r5, r6, lr}
003ad9b4: mov      r5, r0
003ad9b8: beq      #0x3ad9d8
003ad9bc: ldr      r3, [r0]
003ad9c0: mov      r0, r1
003ad9c4: ldr      r4, [r3, #0xd0]
003ad9c8: bl       #0x3935dc
003ad9cc: mov      r1, r0
003ad9d0: mov      r0, r5
003ad9d4: blx      r4
003ad9d8: pop      {r4, r5, r6, pc}

# _ZThn884_N9Character11Ctrl_MoveToEP10GameObject
003ad9dc: sub      r0, r0, #0x374
003ad9e0: b        #0x3ad9e4

# _ZN9Character11Ctrl_MoveToEP10GameObject
003ad9e4: push     {r4, r5, r6, lr}
003ad9e8: ldr      r3, [r0]
003ad9ec: mov      r4, r0
003ad9f0: mov      r5, r1
003ad9f4: mov      lr, pc
003ad9f8: ldr      pc, [r3, #0x54]
003ad9fc: cmp      r0, #0
003ada00: bne      #0x3ada24
003ada04: cmp      r5, #0
003ada08: beq      #0x3ada24
003ada0c: mov      r0, r5
003ada10: bl       #0x3935dc
003ada14: mov      r1, r0
003ada18: mov      r0, r4
003ada1c: pop      {r4, r5, r6, lr}
003ada20: b        #0x3939f0
003ada24: pop      {r4, r5, r6, pc}

# _ZN9Character11Ctrl_LookAtERK7Point3DIfE
003addbc: b        #0x393cec

# _ZN12v2Controller10Cmd_LookAtEP10GameObject
004052bc: push     {r4, lr}
004052c0: ldrb     r2, [r0, #9]
004052c4: ldr      r3, [pc, #0x44]
004052c8: cmp      r2, #0
004052cc: add      r3, pc, r3
004052d0: bne      #0x4052f8
004052d4: ldr      r2, [pc, #0x38]
004052d8: ldr      r3, [r3, r2]
004052dc: ldrb     r3, [r3]
004052e0: cmp      r3, #0
004052e4: beq      #0x4052ec
004052e8: pop      {r4, pc}
004052ec: ldrb     r3, [r0, #8]
004052f0: cmp      r3, #0
004052f4: bne      #0x4052e8
004052f8: ldr      r3, [r0, #4]
004052fc: mov      r0, r3
00405300: ldr      r3, [r3]
00405304: mov      lr, pc
00405308: ldr      pc, [r3, #0x14]
0040530c: pop      {r4, pc}
00405310: subseq   pc, r8, r4, asr #15
00405314: andeq    r3, r0, r0, asr r6

# _ZN12v2Controller10Cmd_MoveToEP10GameObject
00405540: push     {r4, lr}
00405544: ldrb     r2, [r0, #9]
00405548: ldr      r3, [pc, #0x44]
0040554c: cmp      r2, #0
00405550: add      r3, pc, r3
00405554: bne      #0x40557c
00405558: ldr      r2, [pc, #0x38]
0040555c: ldr      r3, [r3, r2]
00405560: ldrb     r3, [r3]
00405564: cmp      r3, #0
00405568: beq      #0x405570
0040556c: pop      {r4, pc}
00405570: ldrb     r3, [r0, #8]
00405574: cmp      r3, #0
00405578: bne      #0x40556c
0040557c: ldr      r3, [r0, #4]
00405580: mov      r0, r3
00405584: ldr      r3, [r3]
00405588: mov      lr, pc
0040558c: ldr      pc, [r3, #0x30]
00405590: pop      {r4, pc}
00405594: subseq   pc, r8, r0, asr #10
00405598: andeq    r3, r0, r0, asr r6

# _ZN12v2Controller8Cmd_StopEv
0040559c: push     {r4, lr}
004055a0: ldrb     r2, [r0, #9]
004055a4: ldr      r3, [pc, #0x44]
004055a8: cmp      r2, #0
004055ac: add      r3, pc, r3
004055b0: bne      #0x4055d8
004055b4: ldr      r2, [pc, #0x38]
004055b8: ldr      r3, [r3, r2]
004055bc: ldrb     r3, [r3]
004055c0: cmp      r3, #0
004055c4: beq      #0x4055cc
004055c8: pop      {r4, pc}
004055cc: ldrb     r3, [r0, #8]
004055d0: cmp      r3, #0
004055d4: bne      #0x4055c8
004055d8: ldr      r3, [r0, #4]
004055dc: mov      r0, r3
004055e0: ldr      r3, [r3]
004055e4: mov      lr, pc
004055e8: ldr      pc, [r3, #0x34]
004055ec: pop      {r4, pc}
004055f0: subseq   pc, r8, r4, ror #9
004055f4: andeq    r3, r0, r0, asr r6
