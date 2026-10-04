
# _ZN16CharStateMachine15SM_SetIdleStateEb
003c1a00: strb     r1, [r0, #0x3c]
003c1a04: mvn      r2, #0
003c1a08: mov      r1, #3
003c1a0c: mov      r3, #0
003c1a10: b        #0x3c1938

# _ZN16CharStateMachine9_SetStateEiiPv
003c1938: push     {r4, r5, r6, r7, r8, sl, lr}
003c193c: ldr      ip, [r0, #0x20]
003c1940: sub      sp, sp, #0x14
003c1944: mov      r4, r0
003c1948: cmp      ip, #0
003c194c: mvneq    r7, #0
003c1950: mov      r5, r1
003c1954: mov      r8, r2
003c1958: mov      sl, r3
003c195c: moveq    r6, r7
003c1960: beq      #0x3c1990
003c1964: ldr      r3, [ip, #4]
003c1968: ldr      r6, [ip]
003c196c: ldr      r2, [r0, #4]
003c1970: ldr      ip, [r3]
003c1974: mov      r0, r3
003c1978: str      r1, [sp]
003c197c: mov      r3, r4
003c1980: mov      r1, r6
003c1984: mov      lr, pc
003c1988: ldr      pc, [ip, #0x10]
003c198c: mov      r7, r6
003c1990: mov      r0, r4
003c1994: mov      r1, r5
003c1998: bl       #0x3c0084
003c199c: cmp      r0, #0
003c19a0: streq    r0, [r4, #0x20]
003c19a4: bne      #0x3c19c0
003c19a8: ldr      r0, [r4, #4]
003c19ac: mov      r2, r7
003c19b0: mov      r1, #0x1d
003c19b4: add      sp, sp, #0x14
003c19b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003c19bc: b        #0x3a4d5c
003c19c0: mov      r1, r5
003c19c4: mov      r0, r4
003c19c8: bl       #0x3c184c
003c19cc: cmp      r6, r5
003c19d0: movne    r3, #0
003c19d4: str      r0, [r4, #0x20]
003c19d8: strne    r3, [r4, #0x60]
003c19dc: ldm      r0, {r1, r3}
003c19e0: ldr      r2, [r4, #4]
003c19e4: ldr      ip, [r3]
003c19e8: mov      r0, r3
003c19ec: stm      sp, {r6, r8, sl}
003c19f0: mov      r3, r4
003c19f4: mov      lr, pc
003c19f8: ldr      pc, [ip, #0xc]
003c19fc: b        #0x3c19a8

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

# _ZN16CharStateMachine19SM_SetPreSpawnStateEv
003c1a64: mov      r1, #0x11
003c1a68: mvn      r2, #0
003c1a6c: mov      r3, #0
003c1a70: b        #0x3c1938

# _ZNK9Character16GetPreSetAIStateEv
003a5784: push     {r4, lr}
003a5788: movw     r3, #0x13dc
003a578c: ldr      r2, [r0, r3]
003a5790: movw     r3, #0x13e0
003a5794: ldr      r3, [r0, r3]
003a5798: cmp      r2, r3
003a579c: beq      #0x3a57e4
003a57a0: ldr      r1, [pc, #0x44]
003a57a4: add      r4, r0, #0x13c0
003a57a8: add      r4, r4, #0xc
003a57ac: add      r1, pc, r1
003a57b0: mov      r0, r4
003a57b4: bl       #0x3a5720
003a57b8: cmp      r0, #0
003a57bc: bne      #0x3a57c4
003a57c0: pop      {r4, pc}
003a57c4: ldr      r1, [pc, #0x24]
003a57c8: mov      r0, r4
003a57cc: add      r1, pc, r1
003a57d0: bl       #0x3a5720
003a57d4: cmp      r0, #0
003a57d8: bne      #0x3a57e4
003a57dc: mov      r0, #0x11
003a57e0: pop      {r4, pc}
003a57e4: mov      r0, #3
003a57e8: pop      {r4, pc}
003a57ec: subseq   sp, r1, r4, lsl #22
003a57f0: subseq   sp, r1, ip, ror #21
