
# _ZNK16CharStateMachine9SM_IsIdleEb
003c0260: push     {r4, lr}
003c0264: mov      r4, r1
003c0268: bl       #0x3c01ac
003c026c: cmp      r0, #0xd
003c0270: beq      #0x3c028c
003c0274: cmp      r0, #0x12
003c0278: beq      #0x3c0294
003c027c: cmp      r0, #3
003c0280: beq      #0x3c028c
003c0284: mov      r0, #0
003c0288: pop      {r4, pc}
003c028c: mov      r0, #1
003c0290: pop      {r4, pc}
003c0294: eor      r0, r4, #1
003c0298: pop      {r4, pc}

# _ZN9Character10CSM_AttackEiPviRi
003ad22c: ldr      r0, [r0, #0x528]
003ad230: and      r0, r0, #1
003ad234: eor      r0, r0, #1
003ad238: bx       lr

# _ZN6CSDead8OnUpdateEiP9CharacterP16CharStateMachine
003c004c: bx       lr

# _ZN6CSDead6OnInitEiP9CharacterP16CharStateMachine
003c8920: push     {r4, r5, r6, lr}
003c8924: add      r5, r2, #0x4f0
003c8928: add      r5, r5, #0xc
003c892c: sub      sp, sp, #0x18
003c8930: mov      r4, #0
003c8934: mov      r6, r1
003c8938: mov      r0, r5
003c893c: mov      r2, #0x2e
003c8940: mov      r3, #2
003c8944: str      r4, [sp, #0x10]
003c8948: str      r4, [sp, #0x14]
003c894c: str      r4, [sp]
003c8950: str      r4, [sp, #4]
003c8954: bl       #0x3c7b18
003c8958: mov      r0, r5
003c895c: mov      r1, r6
003c8960: movw     r2, #0xc359
003c8964: mov      r3, #0x10
003c8968: str      r4, [sp, #4]
003c896c: str      r4, [sp, #8]
003c8970: str      r4, [sp, #0xc]
003c8974: str      r4, [sp]
003c8978: bl       #0x3c7b18
003c897c: add      sp, sp, #0x18
003c8980: pop      {r4, r5, r6, pc}

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

# _ZN8CSAttack7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c404c: push     {r4, r5, r6, r7, r8, sl, lr}
003c4050: ldr      r5, [pc, #0x2b4]
003c4054: ldr      r7, [pc, #0x2b4]
003c4058: ldr      r1, [pc, #0x2b4]
003c405c: add      r5, pc, r5
003c4060: ldr      r3, [r5, r7]
003c4064: ldr      r8, [r5, r1]
003c4068: sub      sp, sp, #0x24
003c406c: ldr      r3, [r3]
003c4070: mov      r0, r8
003c4074: mov      r4, r2
003c4078: str      r3, [sp, #0x1c]
003c407c: bl       #0x337888
003c4080: ldr      r1, [pc, #0x290]
003c4084: add      r6, sp, #4
003c4088: mov      r2, sp
003c408c: add      r1, pc, r1
003c4090: mov      r0, r6
003c4094: bl       #0x3140ec
003c4098: mov      r1, r6
003c409c: mov      r0, r8
003c40a0: bl       #0x337a88
003c40a4: mov      r0, r6
003c40a8: bl       #0x318254
003c40ac: movw     r3, #0x2341
003c40b0: str      r3, [r4, #0x520]
003c40b4: mov      r0, r4
003c40b8: bl       #0x3a3438
003c40bc: cmp      r0, #0
003c40c0: ldrne    r3, [r4, #0x528]
003c40c4: mov      r0, r4
003c40c8: orrne    r3, r3, #1
003c40cc: strne    r3, [r4, #0x528]
003c40d0: ldr      r3, [r4]
003c40d4: mov      lr, pc
003c40d8: ldr      pc, [r3, #0x28]
003c40dc: cmp      r0, #0
003c40e0: beq      #0x3c41c4
003c40e4: ldr      r3, [sp, #0x40]
003c40e8: cmp      r3, #4
003c40ec: beq      #0x3c4248
003c40f0: ldr      r8, [pc, #0x224]
003c40f4: mov      r0, r4
003c40f8: bl       #0x3a3228
003c40fc: ldr      r3, [r5, r8]
003c4100: ldr      r1, [pc, #0x218]
003c4104: ldr      r2, [pc, #0x218]
003c4108: ldr      r0, [r3, #0x2c]
003c410c: add      r1, pc, r1
003c4110: add      r2, pc, r2
003c4114: bl       #0x4c4bdc
003c4118: tst      r0, #0x80
003c411c: bne      #0x3c423c
003c4120: ldr      r3, [pc, #0x200]
003c4124: mov      r0, r4
003c4128: add      r6, r4, #0x490
003c412c: ldr      r3, [r5, r3]
003c4130: add      r6, r6, #0xc
003c4134: ldr      sl, [r3]
003c4138: bl       #0x3a3228
003c413c: ldr      r2, [r5, r8]
003c4140: mov      r3, #0xa0
003c4144: mla      r3, r3, r0, sl
003c4148: ldr      r1, [pc, #0x1dc]
003c414c: ldr      r0, [r2, #0x2c]
003c4150: ldr      r2, [pc, #0x1d8]
003c4154: add      r1, pc, r1
003c4158: ldr      r8, [r3, #8]
003c415c: add      r2, pc, r2
003c4160: bl       #0x4c4bdc
003c4164: ands     r0, r0, #0x80
003c4168: bne      #0x3c4230
003c416c: add      r1, r0, r8
003c4170: mov      r0, r6
003c4174: bl       #0x3cacb0
003c4178: ldr      r0, [r4, #0x2dc]
003c417c: cmp      r0, #0
003c4180: beq      #0x3c4188
003c4184: bl       #0x46eb20
003c4188: add      r0, r4, #0x560
003c418c: bl       #0x3de74c
003c4190: mov      r1, r0
003c4194: str      r0, [r4, #0x52c]
003c4198: mov      r0, r6
003c419c: bl       #0x3c93fc
003c41a0: mov      r0, r4
003c41a4: bl       #0x3bc6b8
003c41a8: ldr      r3, [r5, r7]
003c41ac: ldr      r2, [sp, #0x1c]
003c41b0: ldr      r3, [r3]
003c41b4: cmp      r2, r3
003c41b8: bne      #0x3c4308
003c41bc: add      sp, sp, #0x24
003c41c0: pop      {r4, r5, r6, r7, r8, sl, pc}
003c41c4: ldr      r3, [pc, #0x15c]
003c41c8: mov      r0, r4
003c41cc: mov      r6, #0xa0
003c41d0: ldr      r8, [r5, r3]
003c41d4: ldr      sl, [r8]
003c41d8: bl       #0x3a3228
003c41dc: mla      r0, r6, r0, sl
003c41e0: ldr      r3, [r0, #8]
003c41e4: cmn      r3, #1
003c41e8: beq      #0x3c42b8
003c41ec: mov      r0, r4
003c41f0: ldr      r8, [r8]
003c41f4: bl       #0x3a3228
003c41f8: ldr      r3, [pc, #0x11c]
003c41fc: ldr      r1, [pc, #0x130]
003c4200: ldr      r2, [r5, r3]
003c4204: mla      r3, r6, r0, r8
003c4208: ldr      r0, [r2, #0x2c]
003c420c: ldr      r2, [pc, #0x124]
003c4210: add      r1, pc, r1
003c4214: ldr      r8, [r3, #8]
003c4218: add      r2, pc, r2
003c421c: bl       #0x4c4bdc
003c4220: add      r6, r4, #0x490
003c4224: ands     r0, r0, #0x80
003c4228: add      r6, r6, #0xc
003c422c: beq      #0x3c416c
003c4230: mov      r0, r4
003c4234: bl       #0x3a53e0
003c4238: b        #0x3c416c
003c423c: mov      r0, r4
003c4240: bl       #0x3a53e0
003c4244: b        #0x3c4120
003c4248: ldr      r3, [pc, #0xd8]
003c424c: mov      r0, r4
003c4250: add      r6, r4, #0x490
003c4254: ldr      r3, [r5, r3]
003c4258: add      r6, r6, #0xc
003c425c: ldr      r8, [r3]
003c4260: bl       #0x3a3228
003c4264: ldr      r3, [pc, #0xb0]
003c4268: ldr      r1, [pc, #0xcc]
003c426c: ldr      r2, [r5, r3]
003c4270: mov      r3, #0xa0
003c4274: mla      r3, r3, r0, r8
003c4278: ldr      r0, [r2, #0x2c]
003c427c: ldr      r2, [pc, #0xbc]
003c4280: add      r1, pc, r1
003c4284: ldr      r8, [r3, #4]
003c4288: add      r2, pc, r2
003c428c: bl       #0x4c4bdc
003c4290: ands     r0, r0, #0x40
003c4294: bne      #0x3c42fc
003c4298: add      r1, r0, r8
003c429c: mov      r0, r6
003c42a0: bl       #0x3cacb0
003c42a4: ldr      r0, [r4, #0x2dc]
003c42a8: cmp      r0, #0
003c42ac: beq      #0x3c4188
003c42b0: bl       #0x46eae0
003c42b4: b        #0x3c4188
003c42b8: mov      r0, r4
003c42bc: ldr      r8, [r8]
003c42c0: bl       #0x3a3228
003c42c4: ldr      r3, [pc, #0x50]
003c42c8: ldr      r1, [pc, #0x74]
003c42cc: ldr      r2, [r5, r3]
003c42d0: mla      r3, r6, r0, r8
003c42d4: ldr      r0, [r2, #0x2c]
003c42d8: ldr      r2, [pc, #0x68]
003c42dc: add      r1, pc, r1
003c42e0: ldr      r8, [r3, #4]
003c42e4: add      r2, pc, r2
003c42e8: bl       #0x4c4bdc
003c42ec: add      r6, r4, #0x490
003c42f0: ands     r0, r0, #0x40
003c42f4: add      r6, r6, #0xc
003c42f8: beq      #0x3c4298
003c42fc: mov      r0, r4
003c4300: bl       #0x3a53e0
003c4304: b        #0x3c4298
003c4308: bl       #0x30e310
003c430c: subseq   r0, sp, r4, lsr sl
003c4310: andeq    r4, r0, ip, lsr #1
003c4314: andeq    r0, r0, r4, lsl #17
003c4318: subseq   r0, r0, r4, asr #27
003c431c: strdeq   r3, r4, [r0], -r4
003c4320: subseq   r0, r0, ip, lsr #21
003c4324: ldrheq   r0, [r0], #-0xa8
003c4328: andeq    r4, r0, r4, asr #16
003c432c: subseq   r0, r0, r4, ror #20
003c4330: subseq   r0, r0, ip, ror #20
003c4334: subseq   r0, r0, r8, lsr #19
003c4338: ldrheq   r0, [r0], #-0x90
003c433c: subseq   r0, r0, r8, lsr sb
003c4340: subseq   r0, r0, r0, asr #18
003c4344: ldrsbeq  r0, [r0], #-0x8c
003c4348: subseq   r0, r0, r4, ror #17

# _ZNK14CharProperties20PROPS_GetAttackSpeedEv
003de74c: push     {r4, lr}
003de750: ldr      r0, [r0, #0xb58]
003de754: bl       #0x30e964
003de758: mov      r1, #0x3b800000
003de75c: bl       #0x30ed6c
003de760: movw     r1, #0xd70a
003de764: movt     r1, #0x3c23
003de768: bl       #0x30ed6c
003de76c: mov      r1, #0x3f800000
003de770: bl       #0x30eba4
003de774: mov      r1, #0
003de778: mov      r4, r0
003de77c: bl       #0x30e2f8
003de780: cmp      r0, #0
003de784: moveq    r4, #0
003de788: mov      r0, r4
003de78c: pop      {r4, pc}

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

# _ZN6CSIdle6OnBlurEiP9CharacterP16CharStateMachinei
003c2d3c: ldr      r3, [pc, #0x7c]
003c2d40: ldr      r1, [pc, #0x7c]
003c2d44: push     {r4, r5, r6, r7, lr}
003c2d48: add      r3, pc, r3
003c2d4c: ldr      r5, [r3, r1]
003c2d50: ldr      r1, [pc, #0x70]
003c2d54: mov      r7, r2
003c2d58: ldr      r2, [r5]
003c2d5c: ldr      r6, [r3, r1]
003c2d60: sub      sp, sp, #0x24
003c2d64: str      r2, [sp, #0x1c]
003c2d68: mov      r0, r6
003c2d6c: bl       #0x337888
003c2d70: ldr      r1, [pc, #0x54]
003c2d74: add      r4, sp, #4
003c2d78: mov      r2, sp
003c2d7c: add      r1, pc, r1
003c2d80: mov      r0, r4
003c2d84: bl       #0x3140ec
003c2d88: mov      r1, r4
003c2d8c: mov      r0, r6
003c2d90: bl       #0x337a88
003c2d94: mov      r0, r4
003c2d98: bl       #0x318254
003c2d9c: mov      r3, #0
003c2da0: strb     r3, [r7, #0x538]
003c2da4: ldr      r2, [sp, #0x1c]
003c2da8: ldr      r3, [r5]
003c2dac: cmp      r2, r3
003c2db0: bne      #0x3c2dbc
003c2db4: add      sp, sp, #0x24
003c2db8: pop      {r4, r5, r6, r7, pc}
003c2dbc: bl       #0x30e310
003c2dc0: subseq   r1, sp, r8, asr #26
003c2dc4: andeq    r4, r0, ip, lsr #1
003c2dc8: andeq    r0, r0, r4, lsl #17
003c2dcc: ldrsbeq  r2, [r0], #-4

# _ZN6CSIdle8OnUpdateEiP9CharacterP16CharStateMachine
003c0e80: mov      r0, r1
003c0e84: mov      r1, r2
003c0e88: mov      r2, r3
003c0e8c: b        #0x3c0b78

# _ZN6CSIdle7OnEventEiP9CharacterP16CharStateMachineiPv
003c0004: bx       lr

# _ZN14PhysicalObject9setFilterEsttb
0046ece8: push     {r4, r5, r6, r7, r8, lr}
0046ecec: mov      r4, r0
0046ecf0: ldr      r0, [r0, #0x18]
0046ecf4: mov      r5, r1
0046ecf8: mov      r6, r2
0046ecfc: cmp      r0, #0
0046ed00: mov      r7, r3
0046ed04: ldrb     r8, [sp, #0x18]
0046ed08: beq      #0x46ed28
0046ed0c: strh     r1, [r0, #0x26]
0046ed10: strh     r3, [r0, #0x24]
0046ed14: strh     r2, [r0, #0x22]
0046ed18: ldr      r3, [r4, #4]
0046ed1c: ldr      r1, [r4, #0x18]
0046ed20: ldr      r0, [r3, #0x10]
0046ed24: bl       #0x7e7afc
0046ed28: ldr      r3, [r4, #0x1c]
0046ed2c: cmp      r3, #0
0046ed30: beq      #0x46ed58
0046ed34: cmp      r8, #0
0046ed38: beq      #0x46ed58
0046ed3c: strh     r5, [r3, #0x26]
0046ed40: strh     r7, [r3, #0x24]
0046ed44: strh     r6, [r3, #0x22]
0046ed48: ldr      r3, [r4, #4]
0046ed4c: ldr      r1, [r4, #0x1c]
0046ed50: ldr      r0, [r3, #0x10]
0046ed54: bl       #0x7e7afc
0046ed58: mov      r3, #0
0046ed5c: strb     r3, [r4, #0x26]
0046ed60: pop      {r4, r5, r6, r7, r8, pc}

# _ZN8CSAttack6OnInitEiP9CharacterP16CharStateMachine
003c8284: push     {r4, r5, r6, r7, r8, lr}
003c8288: add      r5, r2, #0x4f0
003c828c: add      r5, r5, #0xc
003c8290: sub      sp, sp, #0x58
003c8294: mov      r4, #0
003c8298: mov      r6, r1
003c829c: mov      r0, r5
003c82a0: mov      r2, #0x22
003c82a4: mov      r3, #3
003c82a8: str      r4, [sp, #0x50]
003c82ac: str      r4, [sp, #0x54]
003c82b0: str      r4, [sp]
003c82b4: str      r4, [sp, #4]
003c82b8: bl       #0x3c7b18
003c82bc: mov      r0, r5
003c82c0: mov      r1, r6
003c82c4: movw     r2, #0xc358
003c82c8: mov      r3, #0xc
003c82cc: str      r4, [sp, #0x48]
003c82d0: str      r4, [sp, #0x4c]
003c82d4: str      r4, [sp]
003c82d8: str      r4, [sp, #4]
003c82dc: bl       #0x3c7b18
003c82e0: mov      r0, r5
003c82e4: mov      r1, r6
003c82e8: movw     r2, #0xc355
003c82ec: mov      r3, #6
003c82f0: str      r4, [sp, #0x40]
003c82f4: str      r4, [sp, #0x44]
003c82f8: str      r4, [sp]
003c82fc: str      r4, [sp, #4]
003c8300: ldr      r8, [pc, #0x120]
003c8304: bl       #0x3c7b18
003c8308: mov      r0, r5
003c830c: mov      r1, r6
003c8310: movw     r2, #0xc356
003c8314: mov      r3, #7
003c8318: str      r4, [sp, #0x38]
003c831c: str      r4, [sp, #0x3c]
003c8320: str      r4, [sp]
003c8324: str      r4, [sp, #4]
003c8328: bl       #0x3c7b18
003c832c: ldr      r3, [pc, #0xf8]
003c8330: add      r8, pc, r8
003c8334: mov      r0, r5
003c8338: ldr      ip, [r8, r3]
003c833c: mov      r1, r6
003c8340: movw     r2, #0xc35a
003c8344: mov      r3, #0xb
003c8348: str      ip, [sp]
003c834c: str      ip, [sp, #0x30]
003c8350: str      r4, [sp, #0x34]
003c8354: str      r4, [sp, #4]
003c8358: bl       #0x3c7b18
003c835c: ldr      r3, [pc, #0xcc]
003c8360: mov      r0, r5
003c8364: mov      r1, r6
003c8368: ldr      r7, [r8, r3]
003c836c: movw     r2, #0xc35b
003c8370: mov      r3, #0xa
003c8374: str      r7, [sp, #0x28]
003c8378: str      r4, [sp, #0x2c]
003c837c: str      r7, [sp]
003c8380: str      r4, [sp, #4]
003c8384: bl       #0x3c7b18
003c8388: mov      r0, r5
003c838c: mov      r1, r6
003c8390: movw     r2, #0xc35c
003c8394: mov      r3, #9
003c8398: str      r7, [sp, #0x20]
003c839c: str      r4, [sp, #0x24]
003c83a0: str      r7, [sp]
003c83a4: str      r4, [sp, #4]
003c83a8: bl       #0x3c7b18
003c83ac: mov      r0, r5
003c83b0: mov      r1, r6
003c83b4: movw     r2, #0xc35d
003c83b8: mov      r3, #8
003c83bc: str      r7, [sp]
003c83c0: str      r7, [sp, #0x18]
003c83c4: str      r4, [sp, #0x1c]
003c83c8: str      r4, [sp, #4]
003c83cc: bl       #0x3c7b18
003c83d0: ldr      r3, [pc, #0x5c]
003c83d4: mov      r0, r5
003c83d8: mov      r1, r6
003c83dc: ldr      ip, [r8, r3]
003c83e0: movw     r2, #0xc351
003c83e4: mov      r3, #4
003c83e8: str      ip, [sp]
003c83ec: str      ip, [sp, #0x10]
003c83f0: str      r4, [sp, #0x14]
003c83f4: str      r4, [sp, #4]
003c83f8: bl       #0x3c7b18
003c83fc: mov      r0, r5
003c8400: mov      r1, r6
003c8404: movw     r2, #0xc357
003c8408: mov      r3, #0xf
003c840c: str      r4, [sp, #4]
003c8410: str      r4, [sp, #8]
003c8414: str      r4, [sp, #0xc]
003c8418: str      r4, [sp]
003c841c: bl       #0x3c7b18
003c8420: add      sp, sp, #0x58
003c8424: pop      {r4, r5, r6, r7, r8, pc}
003c8428: subseq   ip, ip, r0, ror #14
003c842c: andeq    r2, r0, r4, lsl #29
003c8430: andeq    r3, r0, ip, asr #9
003c8434: andeq    r3, r0, r4, lsl #7

# _ZN6CSMove10UpdateTypeEP9CharacterP16CharStateMachine
003c0f18: push     {r4, r5, r6, r7, r8, lr}
003c0f1c: mov      r0, r1
003c0f20: ldr      r3, [r1]
003c0f24: mov      r4, r1
003c0f28: mov      lr, pc
003c0f2c: ldr      pc, [r3, #0x28]
003c0f30: ldr      r6, [pc, #0x224]
003c0f34: cmp      r0, #0
003c0f38: add      r6, pc, r6
003c0f3c: beq      #0x3c1074
003c0f40: ldr      r0, [r4, #0x1b8]
003c0f44: ldr      r8, [r4, #0x1bc]
003c0f48: ldr      r7, [r4, #0x1c0]
003c0f4c: mov      r1, r0
003c0f50: bl       #0x30ed6c
003c0f54: mov      r1, r8
003c0f58: mov      r5, r0
003c0f5c: mov      r0, r8
003c0f60: bl       #0x30ed6c
003c0f64: mov      r1, r0
003c0f68: mov      r0, r5
003c0f6c: bl       #0x30eba4
003c0f70: mov      r1, r7
003c0f74: mov      r5, r0
003c0f78: mov      r0, r7
003c0f7c: bl       #0x30ed6c
003c0f80: mov      r1, r0
003c0f84: mov      r0, r5
003c0f88: bl       #0x30eba4
003c0f8c: ldr      r3, [pc, #0x1cc]
003c0f90: ldr      r5, [r4, #0x53c]
003c0f94: mov      r7, r0
003c0f98: ldr      r3, [r6, r3]
003c0f9c: cmp      r5, #2
003c0fa0: ldr      r3, [r3]
003c0fa4: ldr      r8, [r3, #0x5c]
003c0fa8: ldr      r0, [r3, #0x60]
003c0fac: beq      #0x3c0fd8
003c0fb0: mov      r1, r0
003c0fb4: bl       #0x30ed6c
003c0fb8: mov      r1, r7
003c0fbc: bl       #0x30e70c
003c0fc0: cmp      r0, #0
003c0fc4: bne      #0x3c10f4
003c0fc8: cmp      r5, #0
003c0fcc: beq      #0x3c0ff4
003c0fd0: cmp      r5, #1
003c0fd4: beq      #0x3c1080
003c0fd8: mov      r1, r8
003c0fdc: mov      r0, r8
003c0fe0: bl       #0x30ed6c
003c0fe4: mov      r1, r7
003c0fe8: bl       #0x30e2f8
003c0fec: cmp      r0, #0
003c0ff0: beq      #0x3c1080
003c0ff4: mov      r3, #1
003c0ff8: str      r3, [r4, #0x53c]
003c0ffc: add      r0, r4, #0x560
003c1000: bl       #0x3de6c4
003c1004: ldr      r3, [pc, #0x158]
003c1008: str      r0, [r4, #0x52c]
003c100c: mov      r0, r4
003c1010: ldr      r3, [r6, r3]
003c1014: add      r5, r4, #0x490
003c1018: add      r5, r5, #0xc
003c101c: ldr      r7, [r3]
003c1020: bl       #0x3a3228
003c1024: ldr      r3, [pc, #0x13c]
003c1028: ldr      r1, [pc, #0x13c]
003c102c: ldr      r2, [r6, r3]
003c1030: mov      r3, #0xa0
003c1034: mla      r3, r3, r0, r7
003c1038: ldr      r0, [r2, #0x2c]
003c103c: ldr      r2, [pc, #0x12c]
003c1040: add      r1, pc, r1
003c1044: ldr      r6, [r3, #0x94]
003c1048: add      r2, pc, r2
003c104c: bl       #0x4c4bdc
003c1050: ands     r0, r0, #0x10
003c1054: bne      #0x3c10e8
003c1058: add      r1, r0, r6
003c105c: mov      r0, r5
003c1060: bl       #0x3cacb0
003c1064: ldr      r1, [r4, #0x52c]
003c1068: mov      r0, r5
003c106c: pop      {r4, r5, r6, r7, r8, lr}
003c1070: b        #0x3c93fc
003c1074: ldr      r3, [r4, #0x53c]
003c1078: cmp      r3, #0
003c107c: beq      #0x3c1084
003c1080: pop      {r4, r5, r6, r7, r8, pc}
003c1084: mov      r3, #1
003c1088: str      r3, [r4, #0x53c]
003c108c: add      r0, r4, #0x560
003c1090: bl       #0x3de6c4
003c1094: ldr      r3, [pc, #0xc8]
003c1098: str      r0, [r4, #0x52c]
003c109c: mov      r0, r4
003c10a0: ldr      r3, [r6, r3]
003c10a4: add      r5, r4, #0x490
003c10a8: add      r5, r5, #0xc
003c10ac: ldr      r7, [r3]
003c10b0: bl       #0x3a3228
003c10b4: ldr      r3, [pc, #0xac]
003c10b8: ldr      r1, [pc, #0xb4]
003c10bc: ldr      r2, [r6, r3]
003c10c0: mov      r3, #0xa0
003c10c4: mla      r3, r3, r0, r7
003c10c8: ldr      r0, [r2, #0x2c]
003c10cc: ldr      r2, [pc, #0xa4]
003c10d0: add      r1, pc, r1
003c10d4: ldr      r6, [r3, #0x94]
003c10d8: add      r2, pc, r2
003c10dc: bl       #0x4c4bdc
003c10e0: ands     r0, r0, #0x10
003c10e4: beq      #0x3c1058
003c10e8: mov      r0, r4
003c10ec: bl       #0x3a53e0
003c10f0: b        #0x3c1058
003c10f4: mov      r3, #2
003c10f8: str      r3, [r4, #0x53c]
003c10fc: add      r0, r4, #0x560
003c1100: bl       #0x3de6c4
003c1104: ldr      r3, [pc, #0x58]
003c1108: str      r0, [r4, #0x52c]
003c110c: mov      r0, r4
003c1110: ldr      r3, [r6, r3]
003c1114: add      r5, r4, #0x490
003c1118: add      r5, r5, #0xc
003c111c: ldr      r7, [r3]
003c1120: bl       #0x3a3228
003c1124: ldr      r3, [pc, #0x3c]
003c1128: ldr      r1, [pc, #0x4c]
003c112c: ldr      r2, [r6, r3]
003c1130: mov      r3, #0xa0
003c1134: mla      r3, r3, r0, r7
003c1138: ldr      r0, [r2, #0x2c]
003c113c: ldr      r2, [pc, #0x3c]
003c1140: add      r1, pc, r1
003c1144: ldr      r6, [r3, #0x70]
003c1148: add      r2, pc, r2
003c114c: bl       #0x4c4bdc
003c1150: ands     r0, r0, #0x20
003c1154: beq      #0x3c1058
003c1158: b        #0x3c10e8
003c115c: subseq   r3, sp, r8, asr fp
003c1160: andeq    r3, r0, r8, asr #5
003c1164: andeq    r4, r0, r4, asr #16
003c1168: strdeq   r3, r4, [r0], -r4
003c116c: subseq   r3, r0, r8, ror fp
003c1170: subseq   r3, r0, r0, lsl #23
003c1174: subseq   r3, r0, r8, ror #21
003c1178: ldrsheq  r3, [r0], #-0xa0
003c117c: subseq   r3, r0, r8, ror sl
003c1180: subseq   r3, r0, r0, lsl #21

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

# _ZN6CSMove8OnUpdateEiP9CharacterP16CharStateMachine
003c1184: push     {r4, r5, lr}
003c1188: ldrb     r1, [r2, #0x1b5]
003c118c: sub      sp, sp, #0xc
003c1190: mov      r4, r2
003c1194: cmp      r1, #0
003c1198: mov      r5, r0
003c119c: bne      #0x3c11b8
003c11a0: mov      r0, r4
003c11a4: mov      r1, #0x3f
003c11a8: mov      r2, #0
003c11ac: add      sp, sp, #0xc
003c11b0: pop      {r4, r5, lr}
003c11b4: b        #0x3a4d5c
003c11b8: ldr      r2, [r2, #0x408]
003c11bc: cmp      r2, #0
003c11c0: beq      #0x3c1224
003c11c4: mov      r2, r3
003c11c8: mov      r0, r5
003c11cc: mov      r1, r4
003c11d0: bl       #0x3c0f18
003c11d4: add      r0, r4, #0x560
003c11d8: bl       #0x3de6c4
003c11dc: ldr      r1, [r4, #0x52c]
003c11e0: mov      r5, r0
003c11e4: bl       #0x30e3ac
003c11e8: movw     r1, #0xb717
003c11ec: bic      r0, r0, #0x80000000
003c11f0: movt     r1, #0x38d1
003c11f4: bl       #0x30e70c
003c11f8: cmp      r0, #0
003c11fc: beq      #0x3c1208
003c1200: add      sp, sp, #0xc
003c1204: pop      {r4, r5, pc}
003c1208: add      r0, r4, #0x490
003c120c: add      r0, r0, #0xc
003c1210: mov      r1, r5
003c1214: str      r5, [r4, #0x52c]
003c1218: add      sp, sp, #0xc
003c121c: pop      {r4, r5, lr}
003c1220: b        #0x3c93fc
003c1224: ldr      r2, [r4]
003c1228: mov      r0, r4
003c122c: str      r3, [sp, #4]
003c1230: mov      lr, pc
003c1234: ldr      pc, [r2, #0x28]
003c1238: cmp      r0, #0
003c123c: ldr      r3, [sp, #4]
003c1240: beq      #0x3c1254
003c1244: ldrb     r2, [r4, #0x1b5]
003c1248: cmp      r2, #0
003c124c: beq      #0x3c1200
003c1250: b        #0x3c11c4
003c1254: mov      r0, r4
003c1258: bl       #0x39361c
003c125c: cmp      r0, #0
003c1260: ldr      r3, [sp, #4]
003c1264: beq      #0x3c1244
003c1268: ldr      r2, [r4]
003c126c: mov      r0, r4
003c1270: mov      lr, pc
003c1274: ldr      pc, [r2, #0x54]
003c1278: cmp      r0, #0
003c127c: ldr      r3, [sp, #4]
003c1280: bne      #0x3c1244
003c1284: b        #0x3c11a0

# _ZN16CharStateMachine6UpdateEv
003c628c: push     {r4, r5, r6, lr}
003c6290: mov      r4, r0
003c6294: ldr      r0, [pc, #0xe8]
003c6298: sub      sp, sp, #8
003c629c: ldr      r5, [pc, #0xe4]
003c62a0: add      r0, pc, r0
003c62a4: bl       #0x3136b4
003c62a8: ldr      r3, [pc, #0xdc]
003c62ac: add      r5, pc, r5
003c62b0: ldr      r6, [r4, #0x60]
003c62b4: ldr      r0, [r5, r3]
003c62b8: bl       #0x31f66c
003c62bc: ldr      r3, [r4, #0x2c]
003c62c0: add      r0, r0, r6
003c62c4: str      r0, [r4, #0x60]
003c62c8: tst      r3, #2
003c62cc: bne      #0x3c6314
003c62d0: tst      r3, #4
003c62d4: bne      #0x3c6350
003c62d8: ldr      r3, [r4, #0x20]
003c62dc: cmp      r3, #0
003c62e0: beq      #0x3c6300
003c62e4: ldm      r3, {r1, r2}
003c62e8: mov      r3, r4
003c62ec: mov      r0, r2
003c62f0: ldr      ip, [r2]
003c62f4: ldr      r2, [r4, #4]
003c62f8: mov      lr, pc
003c62fc: ldr      pc, [ip, #0x14]
003c6300: ldr      r0, [pc, #0x88]
003c6304: add      r0, pc, r0
003c6308: add      sp, sp, #8
003c630c: pop      {r4, r5, r6, lr}
003c6310: b        #0x3136b8
003c6314: mov      r0, r4
003c6318: mov      r1, #0
003c631c: bl       #0x3c0378
003c6320: subs     ip, r0, #0
003c6324: bne      #0x3c6344
003c6328: ldr      r2, [r4, #0x24]
003c632c: mov      r3, ip
003c6330: mov      r0, r4
003c6334: ubfx     r2, r2, #0xb, #1
003c6338: mvn      r1, #0
003c633c: str      ip, [sp]
003c6340: bl       #0x3c5ffc
003c6344: ldr      r3, [r4, #0x2c]
003c6348: tst      r3, #4
003c634c: beq      #0x3c62d8
003c6350: mov      r0, r4
003c6354: mov      r1, #0
003c6358: bl       #0x3c034c
003c635c: subs     ip, r0, #0
003c6360: bne      #0x3c62d8
003c6364: ldr      r2, [r4, #0x24]
003c6368: mov      r3, ip
003c636c: mov      r0, r4
003c6370: ubfx     r2, r2, #0xa, #1
003c6374: mvn      r1, #0
003c6378: str      ip, [sp]
003c637c: bl       #0x3c6144
003c6380: b        #0x3c62d8
003c6384: subeq    lr, pc, r0, lsr #25
003c6388: subseq   lr, ip, r4, ror #15
003c638c: strdeq   r3, r4, [r0], -r4
003c6390: subeq    lr, pc, ip, lsr ip

# _ZN6CSIdle6OnInitEiP9CharacterP16CharStateMachine
003c7e60: push     {r4, r5, r6, r7, r8, sl, lr}
003c7e64: add      r5, r2, #0x4f0
003c7e68: add      r5, r5, #0xc
003c7e6c: sub      sp, sp, #0x7c
003c7e70: mov      r4, #0
003c7e74: mov      r6, r1
003c7e78: mov      sl, r2
003c7e7c: mov      r0, r5
003c7e80: mov      r2, #0x23
003c7e84: mov      r3, #3
003c7e88: str      r4, [sp, #0x70]
003c7e8c: str      r4, [sp, #0x74]
003c7e90: str      r4, [sp]
003c7e94: str      r4, [sp, #4]
003c7e98: bl       #0x3c7b18
003c7e9c: mov      r0, r5
003c7ea0: mov      r1, r6
003c7ea4: mov      r2, #0x22
003c7ea8: mov      r3, #3
003c7eac: str      r4, [sp, #0x68]
003c7eb0: str      r4, [sp, #0x6c]
003c7eb4: str      r4, [sp]
003c7eb8: str      r4, [sp, #4]
003c7ebc: ldr      r8, [pc, #0x1d8]
003c7ec0: bl       #0x3c7b18
003c7ec4: mov      r0, r5
003c7ec8: mov      r1, r6
003c7ecc: movw     r2, #0xc358
003c7ed0: mov      r3, #0xc
003c7ed4: str      r4, [sp, #0x60]
003c7ed8: str      r4, [sp, #0x64]
003c7edc: str      r4, [sp]
003c7ee0: str      r4, [sp, #4]
003c7ee4: bl       #0x3c7b18
003c7ee8: ldr      r3, [pc, #0x1b0]
003c7eec: add      r8, pc, r8
003c7ef0: mov      r0, r5
003c7ef4: ldr      ip, [r8, r3]
003c7ef8: mov      r1, r6
003c7efc: movw     r2, #0xc35a
003c7f00: mov      r3, #0xb
003c7f04: str      ip, [sp]
003c7f08: str      ip, [sp, #0x58]
003c7f0c: str      r4, [sp, #0x5c]
003c7f10: str      r4, [sp, #4]
003c7f14: bl       #0x3c7b18
003c7f18: ldr      r3, [pc, #0x184]
003c7f1c: mov      r0, r5
003c7f20: mov      r1, r6
003c7f24: ldr      r7, [r8, r3]
003c7f28: movw     r2, #0xc35b
003c7f2c: mov      r3, #0xa
003c7f30: str      r7, [sp, #0x50]
003c7f34: str      r4, [sp, #0x54]
003c7f38: str      r7, [sp]
003c7f3c: str      r4, [sp, #4]
003c7f40: bl       #0x3c7b18
003c7f44: mov      r0, r5
003c7f48: mov      r1, r6
003c7f4c: movw     r2, #0xc35c
003c7f50: mov      r3, #9
003c7f54: str      r7, [sp, #0x48]
003c7f58: str      r4, [sp, #0x4c]
003c7f5c: str      r7, [sp]
003c7f60: str      r4, [sp, #4]
003c7f64: bl       #0x3c7b18
003c7f68: mov      r0, r5
003c7f6c: mov      r1, r6
003c7f70: movw     r2, #0xc35d
003c7f74: mov      r3, #8
003c7f78: str      r7, [sp]
003c7f7c: str      r7, [sp, #0x40]
003c7f80: str      r4, [sp, #0x44]
003c7f84: str      r4, [sp, #4]
003c7f88: bl       #0x3c7b18
003c7f8c: mov      r0, r5
003c7f90: mov      r1, r6
003c7f94: movw     r2, #0xc356
003c7f98: mov      r3, #7
003c7f9c: str      r4, [sp, #0x38]
003c7fa0: str      r4, [sp, #0x3c]
003c7fa4: str      r4, [sp]
003c7fa8: str      r4, [sp, #4]
003c7fac: bl       #0x3c7b18
003c7fb0: mov      r0, r5
003c7fb4: mov      r1, r6
003c7fb8: movw     r2, #0xc355
003c7fbc: mov      r3, #6
003c7fc0: str      r4, [sp, #0x30]
003c7fc4: str      r4, [sp, #0x34]
003c7fc8: str      r4, [sp]
003c7fcc: str      r4, [sp, #4]
003c7fd0: bl       #0x3c7b18
003c7fd4: ldr      r3, [pc, #0xcc]
003c7fd8: mov      r0, r5
003c7fdc: mov      r1, r6
003c7fe0: ldr      ip, [r8, r3]
003c7fe4: movw     r2, #0xc354
003c7fe8: mov      r3, #5
003c7fec: str      ip, [sp]
003c7ff0: str      ip, [sp, #0x28]
003c7ff4: str      r4, [sp, #0x2c]
003c7ff8: str      r4, [sp, #4]
003c7ffc: bl       #0x3c7b18
003c8000: mov      r0, r5
003c8004: mov      r1, r6
003c8008: movw     r2, #0xc351
003c800c: mov      r3, #4
003c8010: str      r4, [sp, #0x20]
003c8014: str      r4, [sp, #0x24]
003c8018: str      r4, [sp]
003c801c: str      r4, [sp, #4]
003c8020: bl       #0x3c7b18
003c8024: mov      r0, r5
003c8028: mov      r1, r6
003c802c: movw     r2, #0xc352
003c8030: mov      r3, #3
003c8034: str      r4, [sp, #0x18]
003c8038: str      r4, [sp, #0x1c]
003c803c: str      r4, [sp]
003c8040: str      r4, [sp, #4]
003c8044: bl       #0x3c7b18
003c8048: mov      r0, r5
003c804c: mov      r1, r6
003c8050: movw     r2, #0xc353
003c8054: mov      r3, #0xd
003c8058: str      r4, [sp, #0x10]
003c805c: str      r4, [sp, #0x14]
003c8060: str      r4, [sp]
003c8064: str      r4, [sp, #4]
003c8068: bl       #0x3c7b18
003c806c: mov      r0, r5
003c8070: mov      r1, r6
003c8074: movw     r2, #0xc357
003c8078: mov      r3, #0xf
003c807c: str      r4, [sp, #8]
003c8080: str      r4, [sp, #0xc]
003c8084: str      r4, [sp]
003c8088: str      r4, [sp, #4]
003c808c: bl       #0x3c7b18
003c8090: strb     r4, [sl, #0x538]
003c8094: add      sp, sp, #0x7c
003c8098: pop      {r4, r5, r6, r7, r8, sl, pc}
003c809c: subseq   ip, ip, r4, lsr #23
003c80a0: andeq    r2, r0, r4, lsl #29
003c80a4: andeq    r3, r0, ip, asr #9
003c80a8: ldrdeq   r0, r1, [r0], -r4

# _ZN14PhysicalObject11resetFilterEv
0046ec6c: push     {r4, lr}
0046ec70: ldr      r3, [r0, #0x18]
0046ec74: mov      r4, r0
0046ec78: cmp      r3, #0
0046ec7c: beq      #0x46eca8
0046ec80: ldrh     r2, [r0, #0x20]
0046ec84: strh     r2, [r3, #0x22]
0046ec88: ldrh     r2, [r0, #0x22]
0046ec8c: strh     r2, [r3, #0x24]
0046ec90: ldrh     r2, [r0, #0x24]
0046ec94: strh     r2, [r3, #0x26]
0046ec98: ldr      r3, [r0, #4]
0046ec9c: ldr      r1, [r0, #0x18]
0046eca0: ldr      r0, [r3, #0x10]
0046eca4: bl       #0x7e7afc
0046eca8: ldr      r3, [r4, #0x1c]
0046ecac: cmp      r3, #0
0046ecb0: beq      #0x46ecdc
0046ecb4: ldrh     r2, [r4, #0x20]
0046ecb8: strh     r2, [r3, #0x22]
0046ecbc: ldrh     r2, [r4, #0x22]
0046ecc0: strh     r2, [r3, #0x24]
0046ecc4: ldrh     r2, [r4, #0x24]
0046ecc8: strh     r2, [r3, #0x26]
0046eccc: ldr      r3, [r4, #4]
0046ecd0: ldr      r1, [r4, #0x1c]
0046ecd4: ldr      r0, [r3, #0x10]
0046ecd8: bl       #0x7e7afc
0046ecdc: mov      r3, #0
0046ece0: strb     r3, [r4, #0x26]
0046ece4: pop      {r4, pc}

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN9Character20CSM_StoppedAttackingEiPviRi
003ad280: cmp      r3, #5
003ad284: movne    r0, #0
003ad288: ldrbeq   r0, [r0, #0x442]
003ad28c: bx       lr

# _ZN6CSDead7OnEventEiP9CharacterP16CharStateMachineiPv
003c4c3c: push     {r4, r5, r6, r7, r8, lr}
003c4c40: ldr      r4, [pc, #0xec]
003c4c44: ldr      r5, [pc, #0xec]
003c4c48: sub      sp, sp, #0x28
003c4c4c: add      r4, pc, r4
003c4c50: ldr      r3, [r4, r5]
003c4c54: ldr      r1, [sp, #0x40]
003c4c58: mov      r6, r2
003c4c5c: ldr      r3, [r3]
003c4c60: cmp      r1, #0x22
003c4c64: str      r3, [sp, #0x24]
003c4c68: beq      #0x3c4c88
003c4c6c: ldr      r3, [r4, r5]
003c4c70: ldr      r2, [sp, #0x24]
003c4c74: ldr      r3, [r3]
003c4c78: cmp      r2, r3
003c4c7c: bne      #0x3c4d30
003c4c80: add      sp, sp, #0x28
003c4c84: pop      {r4, r5, r6, r7, r8, pc}
003c4c88: ldr      r3, [pc, #0xac]
003c4c8c: add      r7, sp, #0xc
003c4c90: ldr      r8, [r4, r3]
003c4c94: mov      r0, r8
003c4c98: bl       #0x337888
003c4c9c: ldr      r1, [pc, #0x9c]
003c4ca0: add      r2, sp, #8
003c4ca4: mov      r0, r7
003c4ca8: add      r1, pc, r1
003c4cac: bl       #0x3140ec
003c4cb0: mov      r1, r7
003c4cb4: mov      r0, r8
003c4cb8: bl       #0x337a88
003c4cbc: mov      r0, r7
003c4cc0: bl       #0x318254
003c4cc4: mov      r1, #0
003c4cc8: mov      r0, r6
003c4ccc: mov      r2, r1
003c4cd0: bl       #0x394bf8
003c4cd4: ldr      r3, [r6]
003c4cd8: mov      r0, r6
003c4cdc: mov      lr, pc
003c4ce0: ldr      pc, [r3, #0x28]
003c4ce4: subs     r7, r0, #0
003c4ce8: bne      #0x3c4c6c
003c4cec: ldr      r3, [pc, #0x50]
003c4cf0: ldr      r1, [pc, #0x50]
003c4cf4: ldr      r2, [pc, #0x50]
003c4cf8: ldr      r3, [r4, r3]
003c4cfc: add      r1, pc, r1
003c4d00: add      r2, pc, r2
003c4d04: ldr      r0, [r3, #0x2c]
003c4d08: bl       #0x4c4bdc
003c4d0c: mov      r3, #0x2e
003c4d10: mov      r1, r0
003c4d14: mov      r2, r7
003c4d18: add      r0, r6, #0x3b4
003c4d1c: str      r7, [sp]
003c4d20: bl       #0x3dbe24
003c4d24: mov      r3, #0x40
003c4d28: str      r3, [r6, #0x520]
003c4d2c: b        #0x3c4c6c
003c4d30: bl       #0x30e310
003c4d34: subseq   pc, ip, r4, asr #28
003c4d38: andeq    r4, r0, ip, lsr #1
003c4d3c: andeq    r0, r0, r4, lsl #17
003c4d40: subseq   r0, r0, r0, asr r2
003c4d44: strdeq   r3, r4, [r0], -r4
003c4d48: subeq    ip, pc, r4, asr sl
003c4d4c: subseq   r0, r0, r8, lsl #4

# _ZN6CSDead7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c4d50: push     {r4, r5, r6, r7, r8, sl, lr}
003c4d54: ldr      r5, [pc, #0x210]
003c4d58: ldr      r6, [pc, #0x210]
003c4d5c: ldr      r1, [pc, #0x210]
003c4d60: add      r5, pc, r5
003c4d64: ldr      r3, [r5, r6]
003c4d68: ldr      r7, [r5, r1]
003c4d6c: sub      sp, sp, #0x4c
003c4d70: ldr      r3, [r3]
003c4d74: mov      r0, r7
003c4d78: mov      r4, r2
003c4d7c: str      r3, [sp, #0x44]
003c4d80: ldr      sl, [sp, #0x70]
003c4d84: bl       #0x337888
003c4d88: ldr      r1, [pc, #0x1e8]
003c4d8c: add      r8, sp, #0x2c
003c4d90: add      r2, sp, #0x10
003c4d94: add      r1, pc, r1
003c4d98: mov      r0, r8
003c4d9c: bl       #0x3140ec
003c4da0: mov      r1, r8
003c4da4: mov      r0, r7
003c4da8: bl       #0x337a88
003c4dac: mov      r0, r8
003c4db0: bl       #0x318254
003c4db4: mov      r0, r7
003c4db8: bl       #0x337888
003c4dbc: ldr      r1, [pc, #0x1b8]
003c4dc0: add      r8, sp, #0x14
003c4dc4: add      r2, sp, #0xc
003c4dc8: add      r1, pc, r1
003c4dcc: mov      r0, r8
003c4dd0: bl       #0x3140ec
003c4dd4: mov      r1, r8
003c4dd8: mov      r0, r7
003c4ddc: bl       #0x337a88
003c4de0: mov      r0, r8
003c4de4: bl       #0x318254
003c4de8: movw     r3, #0x241
003c4dec: str      r3, [r4, #0x520]
003c4df0: mov      r0, r4
003c4df4: ldr      r3, [r4]
003c4df8: mov      lr, pc
003c4dfc: ldr      pc, [r3, #0x28]
003c4e00: cmp      r0, #0
003c4e04: ldrne    r3, [r4, #0x520]
003c4e08: mov      r1, sl
003c4e0c: ldr      r0, [r4, #0x378]
003c4e10: orrne    r3, r3, #0x2000
003c4e14: strne    r3, [r4, #0x520]
003c4e18: bl       #0x4052bc
003c4e1c: ldr      r3, [r4, #0x378]
003c4e20: mov      r2, #1
003c4e24: mov      r0, r4
003c4e28: strb     r2, [r3, #8]
003c4e2c: bl       #0x3a4068
003c4e30: ldrb     r3, [r4, #0x53a]
003c4e34: cmp      r3, #0
003c4e38: beq      #0x3c4ee8
003c4e3c: add      r0, r4, #0x490
003c4e40: add      r0, r0, #0xc
003c4e44: mov      r1, #0
003c4e48: bl       #0x3c948c
003c4e4c: ldr      r3, [r4, #0x4e8]
003c4e50: cmn      r3, #1
003c4e54: beq      #0x3c4f14
003c4e58: ldr      r0, [r4, #0x2dc]
003c4e5c: cmp      r0, #0
003c4e60: beq      #0x3c4e7c
003c4e64: mov      ip, #0
003c4e68: mov      r1, ip
003c4e6c: movw     r2, #0x51c
003c4e70: mov      r3, #3
003c4e74: str      ip, [sp]
003c4e78: bl       #0x46ece8
003c4e7c: mov      r0, r4
003c4e80: bl       #0x3bc6b8
003c4e84: mov      r0, r4
003c4e88: bl       #0x3a40e4
003c4e8c: mov      r0, r4
003c4e90: bl       #0x3a40b0
003c4e94: add      r0, r4, #0x560
003c4e98: bl       #0x3e0af8
003c4e9c: mov      r0, r4
003c4ea0: mov      r1, #0x2a
003c4ea4: mov      r2, #0
003c4ea8: bl       #0x3a4d5c
003c4eac: mov      r0, r4
003c4eb0: mov      r1, #0x2c
003c4eb4: mov      r2, #0
003c4eb8: bl       #0x3a4d5c
003c4ebc: mov      r2, #0
003c4ec0: mov      r0, r4
003c4ec4: mov      r1, #0x2b
003c4ec8: bl       #0x3a4d5c
003c4ecc: ldr      r3, [r5, r6]
003c4ed0: ldr      r2, [sp, #0x44]
003c4ed4: ldr      r3, [r3]
003c4ed8: cmp      r2, r3
003c4edc: bne      #0x3c4f68
003c4ee0: add      sp, sp, #0x4c
003c4ee4: pop      {r4, r5, r6, r7, r8, sl, pc}
003c4ee8: add      r0, r4, #0x4f0
003c4eec: add      r0, r0, #0xc
003c4ef0: mvn      r1, #0
003c4ef4: bl       #0x3c0b50
003c4ef8: add      r0, r4, #0x490
003c4efc: add      r0, r0, #0xc
003c4f00: mov      r1, #0
003c4f04: bl       #0x3c948c
003c4f08: ldr      r3, [r4, #0x4e8]
003c4f0c: cmn      r3, #1
003c4f10: bne      #0x3c4e58
003c4f14: ldr      r3, [r4]
003c4f18: mov      r0, r4
003c4f1c: mov      lr, pc
003c4f20: ldr      pc, [r3, #0x28]
003c4f24: subs     r7, r0, #0
003c4f28: bne      #0x3c4e58
003c4f2c: ldr      r3, [pc, #0x4c]
003c4f30: ldr      r1, [pc, #0x4c]
003c4f34: ldr      r2, [pc, #0x4c]
003c4f38: ldr      r3, [r5, r3]
003c4f3c: add      r1, pc, r1
003c4f40: add      r2, pc, r2
003c4f44: ldr      r0, [r3, #0x2c]
003c4f48: bl       #0x4c4bdc
003c4f4c: mov      r2, r7
003c4f50: mov      r1, r0
003c4f54: mov      r3, #0x2e
003c4f58: add      r0, r4, #0x3b4
003c4f5c: str      r7, [sp]
003c4f60: bl       #0x3dbe24
003c4f64: b        #0x3c4e58
003c4f68: bl       #0x30e310
003c4f6c: subseq   pc, ip, r0, lsr sp
003c4f70: andeq    r4, r0, ip, lsr #1
003c4f74: andeq    r0, r0, r4, lsl #17
003c4f78: ldrheq   r0, [r0], #-0xc
003c4f7c: subseq   r0, r0, r0, lsr r1
003c4f80: strdeq   r3, r4, [r0], -r4
003c4f84: subeq    ip, pc, r4, lsl r8
003c4f88: subeq    pc, pc, r8, asr #31

# _ZN6CSMove6OnInitEiP9CharacterP16CharStateMachine
003c80ac: push     {r4, r5, r6, r7, r8, lr}
003c80b0: add      r5, r2, #0x4f0
003c80b4: add      r5, r5, #0xc
003c80b8: sub      sp, sp, #0x60
003c80bc: mov      r4, #0
003c80c0: mov      r6, r1
003c80c4: mov      r0, r5
003c80c8: mov      r2, #0x3f
003c80cc: mov      r3, #3
003c80d0: str      r4, [sp, #0x58]
003c80d4: str      r4, [sp, #0x5c]
003c80d8: str      r4, [sp]
003c80dc: str      r4, [sp, #4]
003c80e0: ldr      r8, [pc, #0x18c]
003c80e4: bl       #0x3c7b18
003c80e8: mov      r0, r5
003c80ec: mov      r1, r6
003c80f0: movw     r2, #0xc358
003c80f4: mov      r3, #0xc
003c80f8: str      r4, [sp, #0x50]
003c80fc: str      r4, [sp, #0x54]
003c8100: str      r4, [sp]
003c8104: str      r4, [sp, #4]
003c8108: bl       #0x3c7b18
003c810c: ldr      r3, [pc, #0x164]
003c8110: add      r8, pc, r8
003c8114: mov      r0, r5
003c8118: ldr      ip, [r8, r3]
003c811c: mov      r1, r6
003c8120: movw     r2, #0xc35a
003c8124: mov      r3, #0xb
003c8128: str      ip, [sp]
003c812c: str      ip, [sp, #0x48]
003c8130: str      r4, [sp, #0x4c]
003c8134: str      r4, [sp, #4]
003c8138: bl       #0x3c7b18
003c813c: ldr      r3, [pc, #0x138]
003c8140: mov      r0, r5
003c8144: mov      r1, r6
003c8148: ldr      r7, [r8, r3]
003c814c: movw     r2, #0xc35b
003c8150: mov      r3, #0xa
003c8154: str      r7, [sp, #0x40]
003c8158: str      r4, [sp, #0x44]
003c815c: str      r7, [sp]
003c8160: str      r4, [sp, #4]
003c8164: bl       #0x3c7b18
003c8168: mov      r0, r5
003c816c: mov      r1, r6
003c8170: movw     r2, #0xc35c
003c8174: mov      r3, #9
003c8178: str      r7, [sp, #0x38]
003c817c: str      r4, [sp, #0x3c]
003c8180: str      r7, [sp]
003c8184: str      r4, [sp, #4]
003c8188: bl       #0x3c7b18
003c818c: mov      r0, r5
003c8190: mov      r1, r6
003c8194: movw     r2, #0xc35d
003c8198: mov      r3, #8
003c819c: str      r7, [sp]
003c81a0: str      r7, [sp, #0x30]
003c81a4: str      r4, [sp, #0x34]
003c81a8: str      r4, [sp, #4]
003c81ac: bl       #0x3c7b18
003c81b0: mov      r0, r5
003c81b4: mov      r1, r6
003c81b8: movw     r2, #0xc356
003c81bc: mov      r3, #7
003c81c0: str      r4, [sp, #0x28]
003c81c4: str      r4, [sp, #0x2c]
003c81c8: str      r4, [sp]
003c81cc: str      r4, [sp, #4]
003c81d0: bl       #0x3c7b18
003c81d4: mov      r0, r5
003c81d8: mov      r1, r6
003c81dc: movw     r2, #0xc355
003c81e0: mov      r3, #6
003c81e4: str      r4, [sp, #0x20]
003c81e8: str      r4, [sp, #0x24]
003c81ec: str      r4, [sp]
003c81f0: str      r4, [sp, #4]
003c81f4: bl       #0x3c7b18
003c81f8: ldr      r3, [pc, #0x80]
003c81fc: mov      r0, r5
003c8200: mov      r1, r6
003c8204: ldr      ip, [r8, r3]
003c8208: movw     r2, #0xc354
003c820c: mov      r3, #5
003c8210: str      ip, [sp]
003c8214: str      ip, [sp, #0x18]
003c8218: str      r4, [sp, #0x1c]
003c821c: str      r4, [sp, #4]
003c8220: bl       #0x3c7b18
003c8224: mov      r0, r5
003c8228: mov      r1, r6
003c822c: movw     r2, #0xc353
003c8230: mov      r3, #0xd
003c8234: str      r4, [sp, #0x10]
003c8238: str      r4, [sp, #0x14]
003c823c: str      r4, [sp]
003c8240: str      r4, [sp, #4]
003c8244: bl       #0x3c7b18
003c8248: mov      r0, r5
003c824c: mov      r1, r6
003c8250: movw     r2, #0xc357
003c8254: mov      r3, #0xf
003c8258: str      r4, [sp, #4]
003c825c: str      r4, [sp, #8]
003c8260: str      r4, [sp, #0xc]
003c8264: str      r4, [sp]
003c8268: bl       #0x3c7b18
003c826c: add      sp, sp, #0x60
003c8270: pop      {r4, r5, r6, r7, r8, pc}
003c8274: subseq   ip, ip, r0, lsl #19
003c8278: andeq    r2, r0, r4, lsl #29
003c827c: andeq    r3, r0, ip, asr #9
003c8280: ldrdeq   r0, r1, [r0], -r4

# _ZN6CSMove6OnBlurEiP9CharacterP16CharStateMachinei
003c3aa4: push     {r4, r5, r6, r7, r8, lr}
003c3aa8: ldr      r4, [pc, #0x8c]
003c3aac: ldr      r6, [pc, #0x8c]
003c3ab0: ldr      r1, [pc, #0x8c]
003c3ab4: add      r4, pc, r4
003c3ab8: ldr      r3, [r4, r6]
003c3abc: ldr      r8, [r4, r1]
003c3ac0: sub      sp, sp, #0x20
003c3ac4: ldr      r3, [r3]
003c3ac8: mov      r0, r8
003c3acc: mov      r7, r2
003c3ad0: str      r3, [sp, #0x1c]
003c3ad4: bl       #0x337888
003c3ad8: ldr      r1, [pc, #0x68]
003c3adc: add      r5, sp, #4
003c3ae0: mov      r2, sp
003c3ae4: add      r1, pc, r1
003c3ae8: mov      r0, r5
003c3aec: bl       #0x3140ec
003c3af0: mov      r1, r5
003c3af4: mov      r0, r8
003c3af8: bl       #0x337a88
003c3afc: mov      r0, r5
003c3b00: bl       #0x318254
003c3b04: mov      r0, r7
003c3b08: bl       #0x3938f8
003c3b0c: ldr      r0, [r7, #0x2dc]
003c3b10: cmp      r0, #0
003c3b14: beq      #0x3c3b1c
003c3b18: bl       #0x46eb20
003c3b1c: ldr      r3, [r4, r6]
003c3b20: ldr      r2, [sp, #0x1c]
003c3b24: ldr      r3, [r3]
003c3b28: cmp      r2, r3
003c3b2c: bne      #0x3c3b38
003c3b30: add      sp, sp, #0x20
003c3b34: pop      {r4, r5, r6, r7, r8, pc}
003c3b38: bl       #0x30e310
003c3b3c: ldrsbeq  r0, [sp], #-0xfc
003c3b40: andeq    r4, r0, ip, lsr #1
003c3b44: andeq    r0, r0, r4, lsl #17
003c3b48: subseq   r1, r0, ip, ror #6

# _ZN6CSDead6OnBlurEiP9CharacterP16CharStateMachinei
003c499c: push     {r4, r5, r6, r7, r8, lr}
003c49a0: ldr      r4, [pc, #0x90]
003c49a4: ldr      r6, [pc, #0x90]
003c49a8: ldr      r1, [pc, #0x90]
003c49ac: add      r4, pc, r4
003c49b0: ldr      r3, [r4, r6]
003c49b4: ldr      r8, [r4, r1]
003c49b8: sub      sp, sp, #0x20
003c49bc: ldr      r3, [r3]
003c49c0: mov      r0, r8
003c49c4: mov      r7, r2
003c49c8: str      r3, [sp, #0x1c]
003c49cc: bl       #0x337888
003c49d0: ldr      r1, [pc, #0x6c]
003c49d4: add      r5, sp, #4
003c49d8: mov      r2, sp
003c49dc: add      r1, pc, r1
003c49e0: mov      r0, r5
003c49e4: bl       #0x3140ec
003c49e8: mov      r1, r5
003c49ec: mov      r0, r8
003c49f0: bl       #0x337a88
003c49f4: mov      r0, r5
003c49f8: bl       #0x318254
003c49fc: ldr      r3, [r7, #0x378]
003c4a00: mov      r2, #0
003c4a04: strb     r2, [r3, #8]
003c4a08: ldr      r0, [r7, #0x2dc]
003c4a0c: cmp      r0, r2
003c4a10: beq      #0x3c4a18
003c4a14: bl       #0x46ec6c
003c4a18: ldr      r3, [r4, r6]
003c4a1c: ldr      r2, [sp, #0x1c]
003c4a20: ldr      r3, [r3]
003c4a24: cmp      r2, r3
003c4a28: bne      #0x3c4a34
003c4a2c: add      sp, sp, #0x20
003c4a30: pop      {r4, r5, r6, r7, r8, pc}
003c4a34: bl       #0x30e310
003c4a38: subseq   r0, sp, r4, ror #1
003c4a3c: andeq    r4, r0, ip, lsr #1
003c4a40: andeq    r0, r0, r4, lsl #17
003c4a44: subseq   r0, r0, r4, ror r4

# _ZN6CSIdle7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3020: push     {r4, r5, r6, r7, r8, lr}
003c3024: ldr      r4, [pc, #0xf4]
003c3028: ldr      r7, [pc, #0xf4]
003c302c: ldr      r1, [pc, #0xf4]
003c3030: add      r4, pc, r4
003c3034: ldr      r3, [r4, r7]
003c3038: ldr      r8, [r4, r1]
003c303c: sub      sp, sp, #0x20
003c3040: ldr      r3, [r3]
003c3044: mov      r0, r8
003c3048: mov      r6, r2
003c304c: str      r3, [sp, #0x1c]
003c3050: bl       #0x337888
003c3054: ldr      r1, [pc, #0xd0]
003c3058: add      r5, sp, #4
003c305c: mov      r2, sp
003c3060: add      r1, pc, r1
003c3064: mov      r0, r5
003c3068: bl       #0x3140ec
003c306c: mov      r1, r5
003c3070: mov      r0, r8
003c3074: bl       #0x337a88
003c3078: mov      r0, r5
003c307c: bl       #0x318254
003c3080: ldrb     r3, [r6, #0x538]
003c3084: cmp      r3, #0
003c3088: beq      #0x3c30a8
003c308c: ldr      r3, [r4, r7]
003c3090: ldr      r2, [sp, #0x1c]
003c3094: ldr      r3, [r3]
003c3098: cmp      r2, r3
003c309c: bne      #0x3c311c
003c30a0: add      sp, sp, #0x20
003c30a4: pop      {r4, r5, r6, r7, r8, pc}
003c30a8: mov      r3, #0x2380
003c30ac: str      r3, [r6, #0x520]
003c30b0: ldr      r3, [pc, #0x78]
003c30b4: mov      r0, r6
003c30b8: add      r5, r6, #0x490
003c30bc: ldr      r3, [r4, r3]
003c30c0: add      r5, r5, #0xc
003c30c4: ldr      r8, [r3]
003c30c8: bl       #0x3a3228
003c30cc: ldr      r3, [pc, #0x60]
003c30d0: ldr      r1, [pc, #0x60]
003c30d4: ldr      r2, [r4, r3]
003c30d8: mov      r3, #0xa0
003c30dc: mla      r3, r3, r0, r8
003c30e0: ldr      r0, [r2, #0x2c]
003c30e4: ldr      r2, [pc, #0x50]
003c30e8: add      r1, pc, r1
003c30ec: ldr      r8, [r3, #0x28]
003c30f0: add      r2, pc, r2
003c30f4: bl       #0x4c4bdc
003c30f8: ands     r0, r0, #2
003c30fc: bne      #0x3c3110
003c3100: add      r1, r0, r8
003c3104: mov      r0, r5
003c3108: bl       #0x3cacb0
003c310c: b        #0x3c308c
003c3110: mov      r0, r6
003c3114: bl       #0x3a53e0
003c3118: b        #0x3c3100
003c311c: bl       #0x30e310
003c3120: subseq   r1, sp, r0, ror #20
003c3124: andeq    r4, r0, ip, lsr #1
003c3128: andeq    r0, r0, r4, lsl #17
003c312c: ldrsheq  r1, [r0], #-0xd0
003c3130: andeq    r4, r0, r4, asr #16
003c3134: strdeq   r3, r4, [r0], -r4
003c3138: ldrsbeq  r1, [r0], #-0xa0
003c313c: ldrsbeq  r1, [r0], #-0xa8

# _ZN6CSMove7OnEventEiP9CharacterP16CharStateMachineiPv
003c000c: bx       lr

# _ZN6CSMove7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3bf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c3bfc: ldr      r4, [pc, #0xac]
003c3c00: ldr      r6, [pc, #0xac]
003c3c04: ldr      ip, [pc, #0xac]
003c3c08: add      r4, pc, r4
003c3c0c: ldr      r1, [r4, r6]
003c3c10: ldr      r7, [r4, ip]
003c3c14: sub      sp, sp, #0x20
003c3c18: ldr      r1, [r1]
003c3c1c: mov      sb, r0
003c3c20: mov      r0, r7
003c3c24: mov      r5, r2
003c3c28: mov      sl, r3
003c3c2c: str      r1, [sp, #0x1c]
003c3c30: bl       #0x337888
003c3c34: ldr      r1, [pc, #0x80]
003c3c38: add      r8, sp, #4
003c3c3c: mov      r2, sp
003c3c40: add      r1, pc, r1
003c3c44: mov      r0, r8
003c3c48: bl       #0x3140ec
003c3c4c: mov      r1, r8
003c3c50: mov      r0, r7
003c3c54: bl       #0x337a88
003c3c58: mov      r0, r8
003c3c5c: bl       #0x318254
003c3c60: movw     r3, #0x23c1
003c3c64: str      r3, [r5, #0x520]
003c3c68: mov      r3, #0
003c3c6c: mov      r0, sb
003c3c70: str      r3, [r5, #0x53c]
003c3c74: mov      r2, sl
003c3c78: mov      r1, r5
003c3c7c: bl       #0x3c0f18
003c3c80: ldr      r0, [r5, #0x2dc]
003c3c84: cmp      r0, #0
003c3c88: beq      #0x3c3c90
003c3c8c: bl       #0x46eae0
003c3c90: ldr      r3, [r4, r6]
003c3c94: ldr      r2, [sp, #0x1c]
003c3c98: ldr      r3, [r3]
003c3c9c: cmp      r2, r3
003c3ca0: bne      #0x3c3cac
003c3ca4: add      sp, sp, #0x20
003c3ca8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c3cac: bl       #0x30e310
003c3cb0: subseq   r0, sp, r8, lsl #29
003c3cb4: andeq    r4, r0, ip, lsr #1
003c3cb8: andeq    r0, r0, r4, lsl #17
003c3cbc: subseq   r1, r0, r0, lsl r2

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

# _ZN8CSAttack8OnUpdateEiP9CharacterP16CharStateMachine
003c14f8: push     {r4, r5, r6, lr}
003c14fc: ldr      r1, [r2, #0x408]
003c1500: mov      r4, r2
003c1504: mov      r0, r2
003c1508: bl       #0x393d48
003c150c: add      r0, r4, #0x560
003c1510: bl       #0x3de74c
003c1514: mov      r1, r0
003c1518: mov      r5, r0
003c151c: ldr      r0, [r4, #0x52c]
003c1520: bl       #0x30e3ac
003c1524: movw     r1, #0xb717
003c1528: bic      r0, r0, #0x80000000
003c152c: movt     r1, #0x38d1
003c1530: bl       #0x30e70c
003c1534: cmp      r0, #0
003c1538: beq      #0x3c1540
003c153c: pop      {r4, r5, r6, pc}
003c1540: add      r0, r4, #0x490
003c1544: add      r0, r0, #0xc
003c1548: mov      r1, r5
003c154c: str      r5, [r4, #0x52c]
003c1550: pop      {r4, r5, r6, lr}
003c1554: b        #0x3c93fc
