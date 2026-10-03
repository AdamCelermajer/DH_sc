
# _ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE7reserveEj.clone.2
003dba84: push     {r4, r5, r6, lr}
003dba88: ldr      r2, [r0]
003dba8c: ldr      r3, [r0, #8]
003dba90: sub      sp, sp, #8
003dba94: mov      r1, #0x14
003dba98: rsb      r3, r2, r3
003dba9c: asr      r3, r3, #5
003dbaa0: cmp      r3, #0x13
003dbaa4: mov      r4, r0
003dbaa8: str      r1, [sp, #4]
003dbaac: bhi      #0x3dbaf0
003dbab0: ldr      r3, [r0, #4]
003dbab4: cmp      r2, #0
003dbab8: rsb      r5, r2, r3
003dbabc: asr      r5, r5, #5
003dbac0: beq      #0x3dbaf8
003dbac4: add      r1, sp, #4
003dbac8: bl       #0x3db9dc
003dbacc: mov      r6, r0
003dbad0: mov      r0, r4
003dbad4: bl       #0x3db8a4
003dbad8: ldr      r3, [sp, #4]
003dbadc: add      r5, r6, r5, lsl #5
003dbae0: str      r5, [r4, #4]
003dbae4: add      r3, r6, r3, lsl #5
003dbae8: str      r3, [r4, #8]
003dbaec: str      r6, [r4]
003dbaf0: add      sp, sp, #8
003dbaf4: pop      {r4, r5, r6, pc}
003dbaf8: add      r0, r0, #8
003dbafc: add      r2, sp, #4
003dbb00: bl       #0x3db96c
003dbb04: mov      r6, r0
003dbb08: b        #0x3dbad8

# _ZNK13ItemInventory6HasBowEv
00400080: push     {r4, lr}
00400084: mov      r1, #1
00400088: mov      r4, r0
0040008c: bl       #0x3fc6a8
00400090: mov      r3, #0xc
00400094: mul      r3, r3, r0
00400098: ldr      r2, [r4, #0x14]
0040009c: ldr      r3, [r2, r3]
004000a0: ldr      r0, [r3, #4]
004000a4: cmp      r0, #0
004000a8: beq      #0x4000c4
004000ac: ldr      r0, [r0]
004000b0: bl       #0x3f9e08
004000b4: ldr      r0, [r0, #0x94]
004000b8: cmp      r0, #4
004000bc: movne    r0, #0
004000c0: moveq    r0, #1
004000c4: pop      {r4, pc}

# _ZNSt6vectorIN10CharTimers6_TimerESaIS1_EE9push_backERKS1_
003dbba4: push     {r4, r5, r6, r7, r8, lr}
003dbba8: ldmib    r0, {r3, r7}
003dbbac: ldr      r6, [pc, #0x1b4]
003dbbb0: sub      sp, sp, #8
003dbbb4: cmp      r3, r7
003dbbb8: mov      r5, r0
003dbbbc: mov      r4, r1
003dbbc0: add      r6, pc, r6
003dbbc4: beq      #0x3dbc2c
003dbbc8: ldr      r2, [pc, #0x19c]
003dbbcc: ldr      r2, [r6, r2]
003dbbd0: add      r2, r2, #8
003dbbd4: str      r2, [r3]
003dbbd8: ldr      r2, [r1, #4]
003dbbdc: str      r2, [r3, #4]
003dbbe0: ldr      r2, [r1, #8]
003dbbe4: str      r2, [r3, #8]
003dbbe8: ldr      r2, [r1, #0xc]
003dbbec: str      r2, [r3, #0xc]
003dbbf0: ldr      r2, [r1, #0x10]
003dbbf4: str      r2, [r3, #0x10]
003dbbf8: ldrb     r2, [r1, #0x14]
003dbbfc: strb     r2, [r3, #0x14]
003dbc00: ldrb     r2, [r1, #0x15]
003dbc04: strb     r2, [r3, #0x15]
003dbc08: ldr      r2, [r1, #0x18]
003dbc0c: str      r2, [r3, #0x18]
003dbc10: ldr      r2, [r1, #0x1c]
003dbc14: str      r2, [r3, #0x1c]
003dbc18: ldr      r3, [r0, #4]
003dbc1c: add      r3, r3, #0x20
003dbc20: str      r3, [r0, #4]
003dbc24: add      sp, sp, #8
003dbc28: pop      {r4, r5, r6, r7, r8, pc}
003dbc2c: ldr      r3, [r0]
003dbc30: rsb      r3, r3, r7
003dbc34: asr      r3, r3, #5
003dbc38: cmp      r3, #1
003dbc3c: addhs    r1, r3, r3
003dbc40: addlo    r1, r3, #1
003dbc44: cmn      r1, #0xf8000001
003dbc48: bls      #0x3dbd50
003dbc4c: mvn      r1, #0xf8000000
003dbc50: add      r2, sp, #8
003dbc54: str      r1, [r2, #-4]!
003dbc58: add      r0, r5, #8
003dbc5c: bl       #0x3db96c
003dbc60: ldr      r2, [r5]
003dbc64: mov      r8, r0
003dbc68: rsb      r7, r2, r7
003dbc6c: asr      r7, r7, #5
003dbc70: cmp      r7, #0
003dbc74: ble      #0x3dbd5c
003dbc78: ldr      lr, [pc, #0xec]
003dbc7c: mov      r1, r7
003dbc80: mov      r3, r0
003dbc84: ldr      ip, [r6, lr]
003dbc88: add      ip, ip, #8
003dbc8c: str      ip, [r3]
003dbc90: ldr      r0, [r2, #4]
003dbc94: subs     r1, r1, #1
003dbc98: str      r0, [r3, #4]
003dbc9c: ldr      r0, [r2, #8]
003dbca0: str      r0, [r3, #8]
003dbca4: ldr      r0, [r2, #0xc]
003dbca8: str      r0, [r3, #0xc]
003dbcac: ldr      r0, [r2, #0x10]
003dbcb0: str      r0, [r3, #0x10]
003dbcb4: ldrb     r0, [r2, #0x14]
003dbcb8: strb     r0, [r3, #0x14]
003dbcbc: ldrb     r0, [r2, #0x15]
003dbcc0: strb     r0, [r3, #0x15]
003dbcc4: ldr      r0, [r2, #0x18]
003dbcc8: str      r0, [r3, #0x18]
003dbccc: ldr      r0, [r2, #0x1c]
003dbcd0: add      r2, r2, #0x20
003dbcd4: str      r0, [r3, #0x1c]
003dbcd8: add      r3, r3, #0x20
003dbcdc: bne      #0x3dbc8c
003dbce0: add      r7, r8, r7, lsl #5
003dbce4: ldr      r3, [r6, lr]
003dbce8: mov      r0, r5
003dbcec: add      r3, r3, #8
003dbcf0: str      r3, [r7]
003dbcf4: ldr      r3, [r4, #4]
003dbcf8: str      r3, [r7, #4]
003dbcfc: ldr      r3, [r4, #8]
003dbd00: str      r3, [r7, #8]
003dbd04: ldr      r3, [r4, #0xc]
003dbd08: str      r3, [r7, #0xc]
003dbd0c: ldr      r3, [r4, #0x10]
003dbd10: str      r3, [r7, #0x10]
003dbd14: ldrb     r3, [r4, #0x14]
003dbd18: strb     r3, [r7, #0x14]
003dbd1c: ldrb     r3, [r4, #0x15]
003dbd20: strb     r3, [r7, #0x15]
003dbd24: ldr      r3, [r4, #0x18]
003dbd28: str      r3, [r7, #0x18]
003dbd2c: ldr      r3, [r4, #0x1c]
003dbd30: str      r3, [r7, #0x1c]
003dbd34: bl       #0x3db908
003dbd38: ldr      r3, [sp, #4]
003dbd3c: add      r7, r7, #0x20
003dbd40: str      r8, [r5]
003dbd44: add      r8, r8, r3, lsl #5
003dbd48: stmib    r5, {r7, r8}
003dbd4c: b        #0x3dbc24
003dbd50: cmp      r3, r1
003dbd54: bls      #0x3dbc50
003dbd58: b        #0x3dbc4c
003dbd5c: mov      r7, r0
003dbd60: ldr      lr, [pc, #4]
003dbd64: b        #0x3dbce4
003dbd68: ldrsbeq  r8, [fp], #-0xe0
003dbd6c: andeq    r3, r0, r8, lsl #17

# _ZNK10CharTimers6_Timer6GetRefEv
003db290: ldr      r0, [r0, #0x1c]
003db294: bx       lr

# _ZNK13ItemInventory17HasMainHandWeaponEv
003ffe8c: push     {r4, lr}
003ffe90: mov      r1, #1
003ffe94: mov      r4, r0
003ffe98: bl       #0x3fc6a8
003ffe9c: mov      r3, #0xc
003ffea0: mul      r3, r3, r0
003ffea4: ldr      r2, [r4, #0x14]
003ffea8: ldr      r3, [r2, r3]
003ffeac: ldr      r0, [r3, #4]
003ffeb0: subs     r0, r0, #0
003ffeb4: movne    r0, #1
003ffeb8: pop      {r4, pc}

# _ZN10CharTimers10TMR_ResumeEj
003db2b8: ldr      r3, [r0, #8]
003db2bc: ldr      r2, [r0, #0xc]
003db2c0: rsb      r2, r3, r2
003db2c4: cmp      r1, r2, asr #5
003db2c8: addlo    r3, r3, r1, lsl #5
003db2cc: movlo    r2, #0
003db2d0: strblo   r2, [r3, #0x15]
003db2d4: bx       lr

# _ZN10CharTimersC2Ev
003dbb0c: ldr      r3, [pc, #0x3c]
003dbb10: ldr      r2, [pc, #0x3c]
003dbb14: mov      r1, #0
003dbb18: add      r3, pc, r3
003dbb1c: ldr      r2, [r3, r2]
003dbb20: push     {r4, lr}
003dbb24: add      r2, r2, #8
003dbb28: mov      r4, r0
003dbb2c: str      r1, [r0, #0x10]
003dbb30: str      r2, [r0]
003dbb34: str      r1, [r0, #4]
003dbb38: str      r1, [r0, #8]
003dbb3c: str      r1, [r0, #0xc]
003dbb40: add      r0, r0, #8
003dbb44: bl       #0x3dba84
003dbb48: mov      r0, r4
003dbb4c: pop      {r4, pc}
003dbb50: subseq   r8, fp, r8, ror pc
003dbb54: andeq    r1, r0, r4, asr #30

# _ZN10CharTimers9TMR_StartEjiiPv
003dbe24: push     {r4, r5, r6, lr}
003dbe28: mov      r6, r3
003dbe2c: mov      r4, r1
003dbe30: mov      r5, r2
003dbe34: bl       #0x3dbd70
003dbe38: subs     r3, r0, #0
003dbe3c: beq      #0x3dbe70
003dbe40: mov      r2, #0
003dbe44: mov      r1, #1
003dbe48: strb     r1, [r3, #0x14]
003dbe4c: str      r5, [r3, #8]
003dbe50: str      r4, [r3, #0xc]
003dbe54: str      r2, [r3, #0x10]
003dbe58: str      r6, [r3, #0x18]
003dbe5c: ldr      r1, [sp, #0x10]
003dbe60: ldr      r0, [r3, #4]
003dbe64: strb     r2, [r3, #0x15]
003dbe68: str      r1, [r3, #0x1c]
003dbe6c: pop      {r4, r5, r6, pc}
003dbe70: mvn      r0, #0
003dbe74: pop      {r4, r5, r6, pc}

# _ZN6CharAI20OnAttackDelayExpiredEv
003d0c7c: push     {r4, lr}
003d0c80: ldr      r3, [r0, #0x1c]
003d0c84: cmp      r3, #0
003d0c88: beq      #0x3d0c9c
003d0c8c: mov      r0, r3
003d0c90: ldr      r3, [r3]
003d0c94: mov      lr, pc
003d0c98: ldr      pc, [r3, #0x8c]
003d0c9c: pop      {r4, pc}

# _ZN10CharTimers14_findTimerSlotEv
003dbd70: push     {r4, r5, lr}
003dbd74: mov      r4, r0
003dbd78: ldr      r3, [r4, #0xc]
003dbd7c: ldr      r0, [r0, #8]
003dbd80: ldr      r2, [pc, #0x94]
003dbd84: sub      sp, sp, #0x24
003dbd88: rsb      r3, r0, r3
003dbd8c: asrs     r3, r3, #5
003dbd90: add      r2, pc, r2
003dbd94: beq      #0x3dbdc8
003dbd98: ldrb     r1, [r0, #0x14]
003dbd9c: cmp      r1, #0
003dbda0: movne    r1, #0
003dbda4: bne      #0x3dbdb8
003dbda8: b        #0x3dbe0c
003dbdac: ldrb     r5, [ip, #0x14]
003dbdb0: cmp      r5, #0
003dbdb4: beq      #0x3dbe14
003dbdb8: add      r1, r1, #1
003dbdbc: cmp      r1, r3
003dbdc0: add      ip, r0, r1, lsl #5
003dbdc4: bne      #0x3dbdac
003dbdc8: ldr      ip, [pc, #0x50]
003dbdcc: mov      lr, #0
003dbdd0: add      r0, r4, #8
003dbdd4: ldr      ip, [r2, ip]
003dbdd8: mov      r1, sp
003dbddc: str      r3, [sp, #4]
003dbde0: add      ip, ip, #8
003dbde4: str      ip, [sp]
003dbde8: strb     lr, [sp, #0x15]
003dbdec: strb     lr, [sp, #0x14]
003dbdf0: bl       #0x3dbba4
003dbdf4: ldr      r3, [r4, #8]
003dbdf8: ldr      r0, [r4, #0xc]
003dbdfc: rsb      r0, r3, r0
003dbe00: bic      r0, r0, #0x1f
003dbe04: sub      r0, r0, #0x20
003dbe08: add      r0, r3, r0
003dbe0c: add      sp, sp, #0x24
003dbe10: pop      {r4, r5, pc}
003dbe14: mov      r0, ip
003dbe18: b        #0x3dbe0c
003dbe1c: subseq   r8, fp, r0, lsl #26
003dbe20: andeq    r3, r0, r8, lsl #17

# _ZN10CharTimers9TMR_PauseEj
003db298: ldr      r3, [r0, #8]
003db29c: ldr      r2, [r0, #0xc]
003db2a0: rsb      r2, r3, r2
003db2a4: cmp      r1, r2, asr #5
003db2a8: addlo    r3, r3, r1, lsl #5
003db2ac: movlo    r2, #1
003db2b0: strblo   r2, [r3, #0x15]
003db2b4: bx       lr

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

# _ZNK9Character14GetAttackDelayEv
003a3438: ldr      r3, [pc, #0x24]
003a343c: ldr      r2, [pc, #0x24]
003a3440: push     {r4, lr}
003a3444: add      r3, pc, r3
003a3448: ldr      r2, [r3, r2]
003a344c: ldr      r4, [r2]
003a3450: bl       #0x3a2fec
003a3454: mov      r3, #0x44
003a3458: mla      r4, r3, r0, r4
003a345c: ldr      r0, [r4, #4]
003a3460: pop      {r4, pc}
003a3464: subseq   r1, pc, ip, asr #12
003a3468: andeq    r0, r0, r8, asr r7

# _ZNK13ItemInventory14IsDualWieldingEv
0040019c: b        #0x400158

# _ZNK13ItemInventory12HasTwoHanderEb
004001a0: push     {r4, r5, r6, lr}
004001a4: mov      r5, r1
004001a8: mov      r1, #1
004001ac: mov      r4, r0
004001b0: bl       #0x3fc6a8
004001b4: mov      r3, #0xc
004001b8: mul      r3, r3, r0
004001bc: ldr      r2, [r4, #0x14]
004001c0: ldr      r3, [r2, r3]
004001c4: ldr      r0, [r3, #4]
004001c8: cmp      r0, #0
004001cc: beq      #0x400218
004001d0: ldr      r0, [r0]
004001d4: bl       #0x3f9e08
004001d8: ldr      r3, [r0, #0x58]
004001dc: ldr      r2, [r4, #4]
004001e0: ldr      r0, [r0, #0x68]
004001e4: sub      r3, r3, #4
004001e8: cmp      r3, #1
004001ec: bls      #0x40021c
004001f0: cmp      r5, #0
004001f4: bne      #0x40021c
004001f8: cmn      r0, #4
004001fc: beq      #0x400208
00400200: mov      r0, r5
00400204: pop      {r4, r5, r6, pc}
00400208: movw     r3, #0x1324
0040020c: ldr      r0, [r2, r3]
00400210: rsbs     r0, r0, #1
00400214: movlo    r0, #0
00400218: pop      {r4, r5, r6, pc}
0040021c: cmn      r0, #4
00400220: movne    r0, #0
00400224: moveq    r0, #1
00400228: pop      {r4, r5, r6, pc}

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c

# _ZNK12ItemInstance7GetItemEv
003f9e08: ldr      r3, [pc, #0x1c]
003f9e0c: ldr      r1, [pc, #0x1c]
003f9e10: ldr      r2, [r0, #4]
003f9e14: add      r3, pc, r3
003f9e18: ldr      r1, [r3, r1]
003f9e1c: mov      r0, #0xa4
003f9e20: ldr      r3, [r1]
003f9e24: mla      r0, r0, r2, r3
003f9e28: bx       lr
003f9e2c: subseq   sl, sb, ip, ror ip
003f9e30: andeq    r2, r0, ip, ror #16

# _ZN8CSAttack7OnEventEiP9CharacterP16CharStateMachineiPv
003c1288: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c128c: ldr      r3, [sp, #0x20]
003c1290: ldr      r4, [pc, #0x234]
003c1294: mov      r5, r2
003c1298: cmp      r3, #0x1a
003c129c: add      r4, pc, r4
003c12a0: beq      #0x3c12b0
003c12a4: cmp      r3, #0x1c
003c12a8: beq      #0x3c12d0
003c12ac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c12b0: ldr      r0, [r2, #0x408]
003c12b4: cmp      r0, #0
003c12b8: beq      #0x3c13ac
003c12bc: bl       #0x3935dc
003c12c0: mov      r1, r0
003c12c4: mov      r0, r5
003c12c8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c12cc: b        #0x393cec
003c12d0: ldr      r3, [r2]
003c12d4: mov      r0, r2
003c12d8: mov      lr, pc
003c12dc: ldr      pc, [r3, #0x28]
003c12e0: cmp      r0, #0
003c12e4: beq      #0x3c12ac
003c12e8: ldrb     r3, [r5, #0x1b5]
003c12ec: cmp      r3, #0
003c12f0: beq      #0x3c13dc
003c12f4: ldr      r7, [pc, #0x1d4]
003c12f8: mov      r0, r5
003c12fc: ldr      r6, [pc, #0x1d0]
003c1300: ldr      r3, [r4, r7]
003c1304: add      r8, r5, #0x490
003c1308: add      r8, r8, #0xc
003c130c: ldr      sl, [r3]
003c1310: bl       #0x3a3228
003c1314: ldr      r2, [r4, r6]
003c1318: mov      r3, #0xa0
003c131c: mla      r3, r3, r0, sl
003c1320: ldr      r1, [pc, #0x1b0]
003c1324: ldr      r0, [r2, #0x2c]
003c1328: ldr      r2, [pc, #0x1ac]
003c132c: add      r1, pc, r1
003c1330: ldr      sl, [r3, #4]
003c1334: add      r2, pc, r2
003c1338: bl       #0x4c4bdc
003c133c: ands     r2, r0, #0x40
003c1340: bne      #0x3c14a0
003c1344: ldr      r3, [r4, r7]
003c1348: mov      r0, r5
003c134c: add      r7, r2, sl
003c1350: ldr      sl, [r3]
003c1354: bl       #0x3a3228
003c1358: ldr      r2, [r4, r6]
003c135c: mov      r3, #0xa0
003c1360: mla      r3, r3, r0, sl
003c1364: ldr      r1, [pc, #0x174]
003c1368: ldr      r0, [r2, #0x2c]
003c136c: ldr      r2, [pc, #0x170]
003c1370: add      r1, pc, r1
003c1374: ldr      r4, [r3, #8]
003c1378: add      r2, pc, r2
003c137c: bl       #0x4c4bdc
003c1380: ands     r0, r0, #0x80
003c1384: bne      #0x3c1494
003c1388: add      r2, r0, r4
003c138c: mov      r1, r7
003c1390: mov      r0, r8
003c1394: bl       #0x3caccc
003c1398: ldr      r0, [r5, #0x2dc]
003c139c: cmp      r0, #0
003c13a0: beq      #0x3c12ac
003c13a4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c13a8: b        #0x46eae0
003c13ac: add      r0, r2, #0x37c
003c13b0: bl       #0x3fffa4
003c13b4: cmp      r0, #0
003c13b8: beq      #0x3c12ac
003c13bc: ldrb     r3, [r5, #0x1b5]
003c13c0: cmp      r3, #0
003c13c4: beq      #0x3c12ac
003c13c8: mov      r0, r5
003c13cc: add      r1, r5, #0x1b8
003c13d0: mov      r2, #1
003c13d4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c13d8: b        #0x393be8
003c13dc: ldr      r7, [pc, #0xec]
003c13e0: mov      r0, r5
003c13e4: ldr      r6, [pc, #0xe8]
003c13e8: ldr      r3, [r4, r7]
003c13ec: add      r8, r5, #0x490
003c13f0: add      r8, r8, #0xc
003c13f4: ldr      sl, [r3]
003c13f8: bl       #0x3a3228
003c13fc: ldr      r2, [r4, r6]
003c1400: mov      r3, #0xa0
003c1404: mla      r3, r3, r0, sl
003c1408: ldr      r1, [pc, #0xd8]
003c140c: ldr      r0, [r2, #0x2c]
003c1410: ldr      r2, [pc, #0xd4]
003c1414: add      r1, pc, r1
003c1418: ldr      sl, [r3, #8]
003c141c: add      r2, pc, r2
003c1420: bl       #0x4c4bdc
003c1424: ands     r2, r0, #0x80
003c1428: bne      #0x3c14bc
003c142c: ldr      r3, [r4, r7]
003c1430: mov      r0, r5
003c1434: add      r7, r2, sl
003c1438: ldr      sl, [r3]
003c143c: bl       #0x3a3228
003c1440: ldr      r2, [r4, r6]
003c1444: mov      r3, #0xa0
003c1448: mla      r3, r3, r0, sl
003c144c: ldr      r1, [pc, #0x9c]
003c1450: ldr      r0, [r2, #0x2c]
003c1454: ldr      r2, [pc, #0x98]
003c1458: add      r1, pc, r1
003c145c: ldr      r4, [r3, #4]
003c1460: add      r2, pc, r2
003c1464: bl       #0x4c4bdc
003c1468: ands     r0, r0, #0x40
003c146c: bne      #0x3c14b0
003c1470: add      r2, r0, r4
003c1474: mov      r1, r7
003c1478: mov      r0, r8
003c147c: bl       #0x3caccc
003c1480: ldr      r0, [r5, #0x2dc]
003c1484: cmp      r0, #0
003c1488: beq      #0x3c12ac
003c148c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c1490: b        #0x46eb20
003c1494: mov      r0, r5
003c1498: bl       #0x3a53e0
003c149c: b        #0x3c1388
003c14a0: mov      r0, r5
003c14a4: bl       #0x3a53e0
003c14a8: mov      r2, r0
003c14ac: b        #0x3c1344
003c14b0: mov      r0, r5
003c14b4: bl       #0x3a53e0
003c14b8: b        #0x3c1470
003c14bc: mov      r0, r5
003c14c0: bl       #0x3a53e0
003c14c4: mov      r2, r0
003c14c8: b        #0x3c142c
003c14cc: ldrsheq  r3, [sp], #-0x74
003c14d0: andeq    r4, r0, r4, asr #16
003c14d4: strdeq   r3, r4, [r0], -r4
003c14d8: subseq   r3, r0, ip, lsl #17

# _ZNK13ItemInventory16HasOffHandWeaponEv
00400158: push     {r4, lr}
0040015c: mov      r1, #2
00400160: mov      r4, r0
00400164: bl       #0x3fc6a8
00400168: mov      r3, #0xc
0040016c: mul      r3, r3, r0
00400170: ldr      r2, [r4, #0x14]
00400174: ldr      r3, [r2, r3]
00400178: ldr      r0, [r3, #8]
0040017c: cmp      r0, #0
00400180: beq      #0x400198
00400184: ldr      r0, [r0]
00400188: bl       #0x3f9e08
0040018c: ldr      r0, [r0, #0x58]
00400190: subs     r0, r0, #6
00400194: movne    r0, #1
00400198: pop      {r4, pc}

# _ZN10CharTimers8TMR_StopEj
003db2d8: ldr      r3, [r0, #8]
003db2dc: ldr      r2, [r0, #0xc]
003db2e0: rsb      r2, r3, r2
003db2e4: cmp      r1, r2, asr #5
003db2e8: addlo    r3, r3, r1, lsl #5
003db2ec: movlo    r2, #0
003db2f0: strblo   r2, [r3, #0x14]
003db2f4: bx       lr

# _ZN10CharTimers6UpdateEv
003db640: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003db644: ldr      r8, [pc, #0x23c]
003db648: ldr      r3, [pc, #0x23c]
003db64c: ldr      r1, [pc, #0x23c]
003db650: add      r8, pc, r8
003db654: ldr      r2, [r8, r3]
003db658: ldr      r3, [r8, r1]
003db65c: sub      sp, sp, #0x54
003db660: ldrb     sl, [r2, #0x30]
003db664: ldr      r3, [r3]
003db668: str      r1, [sp, #0x10]
003db66c: cmp      sl, #0
003db670: mov      r6, r0
003db674: str      r3, [sp, #0x4c]
003db678: beq      #0x3db69c
003db67c: ldr      r2, [sp, #0x10]
003db680: ldr      r3, [r8, r2]
003db684: ldr      r2, [sp, #0x4c]
003db688: ldr      r3, [r3]
003db68c: cmp      r2, r3
003db690: bne      #0x3db884
003db694: add      sp, sp, #0x54
003db698: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003db69c: ldr      r3, [pc, #0x1f0]
003db6a0: ldr      r0, [r8, r3]
003db6a4: bl       #0x31f66c
003db6a8: str      r0, [sp, #0x14]
003db6ac: ldr      r4, [r6, #8]
003db6b0: ldr      r3, [r6, #0xc]
003db6b4: rsb      r3, r4, r3
003db6b8: asrs     r3, r3, #5
003db6bc: str      r3, [sp, #8]
003db6c0: beq      #0x3db67c
003db6c4: ldr      r2, [pc, #0x1cc]
003db6c8: ldr      r3, [pc, #0x1cc]
003db6cc: mov      sb, r8
003db6d0: str      r2, [sp]
003db6d4: ldr      r2, [pc, #0x1c4]
003db6d8: add      r3, pc, r3
003db6dc: add      r3, r3, #0x13
003db6e0: add      r2, pc, r2
003db6e4: add      r2, r2, #0x13
003db6e8: str      r2, [sp, #4]
003db6ec: str      r3, [sp, #0xc]
003db6f0: b        #0x3db708
003db6f4: ldr      r1, [sp, #8]
003db6f8: add      sl, sl, #1
003db6fc: cmp      sl, r1
003db700: beq      #0x3db87c
003db704: ldr      r4, [r6, #8]
003db708: lsl      r7, sl, #5
003db70c: add      r4, r4, r7
003db710: ldrb     r3, [r4, #0x14]
003db714: cmp      r3, #0
003db718: beq      #0x3db6f4
003db71c: ldrb     r2, [r4, #0x15]
003db720: cmp      r2, #0
003db724: bne      #0x3db6f4
003db728: ldr      r2, [r4, #0x10]
003db72c: ldr      r1, [sp, #0x14]
003db730: add      r5, sp, #0x1c
003db734: add      r8, sp, #0x34
003db738: add      r2, r2, r1
003db73c: str      r2, [r4, #0x10]
003db740: cmp      r3, #0
003db744: beq      #0x3db6f4
003db748: ldr      r2, [r4, #0x10]
003db74c: ldr      r3, [r4, #0xc]
003db750: cmp      r2, r3
003db754: blo      #0x3db6f4
003db758: cmp      r3, #0
003db75c: beq      #0x3db6f4
003db760: ldr      r1, [r4, #8]
003db764: cmp      r1, #0
003db768: strbeq   r1, [r4, #0x14]
003db76c: beq      #0x3db780
003db770: rsb      r3, r3, r2
003db774: subgt    r1, r1, #1
003db778: str      r3, [r4, #0x10]
003db77c: strgt    r1, [r4, #8]
003db780: ldr      r3, [r4, #0x18]
003db784: cmn      r3, #1
003db788: beq      #0x3db7fc
003db78c: ldr      r3, [sp]
003db790: ldr      fp, [sb, r3]
003db794: mov      r0, fp
003db798: bl       #0x337888
003db79c: mov      r0, r5
003db7a0: ldr      r1, [sp, #4]
003db7a4: str      r5, [sp, #0x2c]
003db7a8: str      r5, [sp, #0x30]
003db7ac: bl       #0x3db5f0
003db7b0: mov      r0, fp
003db7b4: mov      r1, r5
003db7b8: bl       #0x337a88
003db7bc: ldr      r0, [sp, #0x30]
003db7c0: cmp      r0, r5
003db7c4: beq      #0x3db7e4
003db7c8: cmp      r0, #0
003db7cc: beq      #0x3db7e4
003db7d0: ldr      r1, [sp, #0x1c]
003db7d4: rsb      r1, r0, r1
003db7d8: cmp      r1, #0x80
003db7dc: bhi      #0x3db86c
003db7e0: bl       #0x708f00
003db7e4: ldmib    r6, {r0, r2}
003db7e8: ldr      r1, [r4, #0x18]
003db7ec: add      r2, r2, r7
003db7f0: bl       #0x3a4d5c
003db7f4: ldrb     r3, [r4, #0x14]
003db7f8: b        #0x3db740
003db7fc: ldr      r2, [sp]
003db800: ldr      fp, [sb, r2]
003db804: mov      r0, fp
003db808: bl       #0x337888
003db80c: mov      r0, r8
003db810: ldr      r1, [sp, #0xc]
003db814: str      r8, [sp, #0x44]
003db818: str      r8, [sp, #0x48]
003db81c: bl       #0x3db5f0
003db820: mov      r0, fp
003db824: mov      r1, r8
003db828: bl       #0x337a88
003db82c: ldr      r0, [sp, #0x48]
003db830: cmp      r0, r8
003db834: beq      #0x3db854
003db838: cmp      r0, #0
003db83c: beq      #0x3db854
003db840: ldr      r1, [sp, #0x34]
003db844: rsb      r1, r0, r1
003db848: cmp      r1, #0x80
003db84c: bhi      #0x3db874
003db850: bl       #0x708f00
003db854: ldmib    r6, {r0, r2}
003db858: mov      r1, #0x29
003db85c: add      r2, r2, r7
003db860: bl       #0x3a4d5c
003db864: ldrb     r3, [r4, #0x14]
003db868: b        #0x3db740
003db86c: bl       #0x310440
003db870: b        #0x3db7e4
003db874: bl       #0x310440
003db878: b        #0x3db854
003db87c: mov      r8, sb
003db880: b        #0x3db67c
003db884: bl       #0x30e310
003db888: subseq   sb, fp, r0, asr #8
003db88c: andeq    r1, r0, r0, lsr #20
003db890: andeq    r4, r0, ip, lsr #1
003db894: strdeq   r3, r4, [r0], -r4
003db898: andeq    r0, r0, r4, lsl #17
003db89c: subeq    sl, lr, r0, ror r2
003db8a0: subeq    sl, lr, r8, ror #4

# _ZN10AISDefault20OnAttackDelayExpiredEv
003dbee8: bx       lr

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

# _ZNK10CharTimers12TMR_TimeLeftEjRjS0_
003db344: ldr      ip, [r0, #0xc]
003db348: ldr      r0, [r0, #8]
003db34c: rsb      ip, r0, ip
003db350: cmp      r1, ip, asr #5
003db354: bhs      #0x3db380
003db358: add      r1, r0, r1, lsl #5
003db35c: ldrb     r0, [r1, #0x14]
003db360: cmp      r0, #0
003db364: beq      #0x3db380
003db368: ldr      ip, [r1, #0x10]
003db36c: mov      r0, #1
003db370: str      ip, [r2]
003db374: ldr      r2, [r1, #0xc]
003db378: str      r2, [r3]
003db37c: bx       lr
003db380: mov      r0, #0
003db384: bx       lr

# _ZN10CharTimers12SetCharacterEP9Character
003db480: push     {r4, r5, lr}
003db484: ldr      r3, [pc, #0x70]
003db488: subs     r4, r1, #0
003db48c: sub      sp, sp, #0xc
003db490: mov      r5, r0
003db494: add      r3, pc, r3
003db498: beq      #0x3db4a8
003db49c: str      r4, [r5, #4]
003db4a0: add      sp, sp, #0xc
003db4a4: pop      {r4, r5, pc}
003db4a8: ldr      r2, [pc, #0x50]
003db4ac: ldr      r2, [r3, r2]
003db4b0: ldr      r2, [r2]
003db4b4: cmp      r2, #2
003db4b8: streq    r4, [r4]
003db4bc: beq      #0x3db49c
003db4c0: cmp      r2, #1
003db4c4: bne      #0x3db49c
003db4c8: ldr      r0, [pc, #0x34]
003db4cc: ldr      r1, [pc, #0x34]
003db4d0: ldr      r2, [pc, #0x34]
003db4d4: ldr      r0, [r3, r0]
003db4d8: ldr      r3, [pc, #0x30]
003db4dc: mov      ip, #0x3d
003db4e0: add      r1, pc, r1
003db4e4: add      r2, pc, r2
003db4e8: add      r3, pc, r3
003db4ec: add      r0, r0, #0xa8
003db4f0: str      ip, [sp]
003db4f4: bl       #0x30e004
003db4f8: b        #0x3db49c
003db4fc: ldrsheq  sb, [fp], #-0x5c
003db500: andeq    r3, r0, r0, asr #19
003db504: andeq    r1, r0, r0, asr #19
003db508: strdeq   r2, r3, [lr], #-0xe8
003db50c: subseq   r6, r1, r4, lsr #23
003db510: subeq    sl, lr, r8, lsl #8

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr

# _ZN10CharTimers11TMR_StopAllEv
003db2f8: ldr      r2, [r0, #8]
003db2fc: ldr      r1, [r0, #0xc]
003db300: rsb      r1, r2, r1
003db304: asrs     r1, r1, #5
003db308: bxeq     lr
003db30c: mov      r3, #0
003db310: mov      ip, r3
003db314: add      r2, r2, r3, lsl #5
003db318: add      r3, r3, #1
003db31c: cmp      r3, r1
003db320: strb     ip, [r2, #0x14]
003db324: bxeq     lr
003db328: ldr      r2, [r0, #8]
003db32c: add      r2, r2, r3, lsl #5
003db330: add      r3, r3, #1
003db334: cmp      r3, r1
003db338: strb     ip, [r2, #0x14]
003db33c: bne      #0x3db328
003db340: bx       lr

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

# _ZNK13ItemInventory8HasStaffEv
004000c8: push     {r4, lr}
004000cc: mov      r1, #1
004000d0: mov      r4, r0
004000d4: bl       #0x3fc6a8
004000d8: mov      r3, #0xc
004000dc: mul      r3, r3, r0
004000e0: ldr      r2, [r4, #0x14]
004000e4: ldr      r3, [r2, r3]
004000e8: ldr      r0, [r3, #4]
004000ec: cmp      r0, #0
004000f0: beq      #0x40010c
004000f4: ldr      r0, [r0]
004000f8: bl       #0x3f9e08
004000fc: ldr      r0, [r0, #0x94]
00400100: cmp      r0, #5
00400104: movne    r0, #0
00400108: moveq    r0, #1
0040010c: pop      {r4, pc}

# _ZN8CSAttack6OnBlurEiP9CharacterP16CharStateMachinei
003c3f74: push     {r4, r5, r6, r7, r8, lr}
003c3f78: ldr      r4, [pc, #0xbc]
003c3f7c: ldr      r6, [pc, #0xbc]
003c3f80: ldr      r1, [pc, #0xbc]
003c3f84: add      r4, pc, r4
003c3f88: ldr      r3, [r4, r6]
003c3f8c: ldr      r8, [r4, r1]
003c3f90: sub      sp, sp, #0x28
003c3f94: ldr      r3, [r3]
003c3f98: mov      r0, r8
003c3f9c: mov      r5, r2
003c3fa0: str      r3, [sp, #0x24]
003c3fa4: bl       #0x337888
003c3fa8: ldr      r1, [pc, #0x98]
003c3fac: add      r7, sp, #0xc
003c3fb0: add      r2, sp, #8
003c3fb4: add      r1, pc, r1
003c3fb8: mov      r0, r7
003c3fbc: bl       #0x3140ec
003c3fc0: mov      r1, r7
003c3fc4: mov      r0, r8
003c3fc8: bl       #0x337a88
003c3fcc: mov      r0, r7
003c3fd0: bl       #0x318254
003c3fd4: mov      r0, r5
003c3fd8: bl       #0x3a3438
003c3fdc: cmp      r0, #0
003c3fe0: bne      #0x3c4010
003c3fe4: ldr      r0, [r5, #0x2dc]
003c3fe8: cmp      r0, #0
003c3fec: beq      #0x3c3ff4
003c3ff0: bl       #0x46eb20
003c3ff4: ldr      r3, [r4, r6]
003c3ff8: ldr      r2, [sp, #0x24]
003c3ffc: ldr      r3, [r3]
003c4000: cmp      r2, r3
003c4004: bne      #0x3c4038
003c4008: add      sp, sp, #0x28
003c400c: pop      {r4, r5, r6, r7, r8, pc}
003c4010: mov      r0, r5
003c4014: bl       #0x3a3438
003c4018: mov      ip, #0
003c401c: mov      r1, r0
003c4020: mov      r2, ip
003c4024: add      r0, r5, #0x3b4
003c4028: mov      r3, #0x2a
003c402c: str      ip, [sp]
003c4030: bl       #0x3dbe24
003c4034: b        #0x3c3fe4
003c4038: bl       #0x30e310
003c403c: subseq   r0, sp, ip, lsl #22
003c4040: andeq    r4, r0, ip, lsr #1
003c4044: andeq    r0, r0, r4, lsl #17
