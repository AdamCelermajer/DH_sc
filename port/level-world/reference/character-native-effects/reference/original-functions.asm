
# _ZN16CharStateMachine15SM_SetStunStateEjbPvb
003c5ffc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6000: mov      r5, r0
003c6004: sub      sp, sp, #8
003c6008: ldr      r0, [r0, #4]
003c600c: mov      sl, r1
003c6010: mov      r8, r2
003c6014: mov      r6, r3
003c6018: ldrb     r7, [sp, #0x28]
003c601c: bl       #0x3a3158
003c6020: ldr      r4, [pc, #0x104]
003c6024: cmp      r0, #0
003c6028: add      r4, pc, r4
003c602c: beq      #0x3c6038
003c6030: add      sp, sp, #8
003c6034: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c6038: ldr      r0, [r5, #4]
003c603c: bl       #0x3a3228
003c6040: subs     sb, r0, #0
003c6044: blt      #0x3c6030
003c6048: ldr      r3, [pc, #0xe0]
003c604c: ldr      r3, [r4, r3]
003c6050: ldr      r3, [r3]
003c6054: cmp      sb, r3
003c6058: bge      #0x3c6030
003c605c: ldr      ip, [r5, #0x2c]
003c6060: ands     ip, ip, #2
003c6064: beq      #0x3c6100
003c6068: ldr      r3, [pc, #0xc4]
003c606c: ldr      r2, [pc, #0xc4]
003c6070: ldr      r1, [pc, #0xc4]
003c6074: ldr      r3, [r4, r3]
003c6078: ldr      r2, [r4, r2]
003c607c: add      r1, pc, r1
003c6080: ldr      r3, [r3]
003c6084: ldr      r0, [r2, #0x2c]
003c6088: mov      r2, #0xa0
003c608c: mla      sb, r2, sb, r3
003c6090: ldr      r2, [pc, #0xa8]
003c6094: ldr      r4, [sb, #0x8c]
003c6098: add      r2, pc, r2
003c609c: bl       #0x4c4bdc
003c60a0: ands     r0, r0, #0x200
003c60a4: bne      #0x3c60f4
003c60a8: add      r4, r0, r4
003c60ac: cmp      r7, #0
003c60b0: str      r4, [r5, #0x28]
003c60b4: beq      #0x3c60e0
003c60b8: mov      r3, r6
003c60bc: mov      r0, r5
003c60c0: mov      r1, #9
003c60c4: movw     r2, #0xc35c
003c60c8: bl       #0x3c1938
003c60cc: cmp      r8, #0
003c60d0: ldrne    r3, [r5, #0x24]
003c60d4: orrne    r3, r3, #0x800
003c60d8: strne    r3, [r5, #0x24]
003c60dc: b        #0x3c6030
003c60e0: mov      r2, r6
003c60e4: mov      r0, r5
003c60e8: movw     r1, #0xc35c
003c60ec: bl       #0x3c5684
003c60f0: b        #0x3c60cc
003c60f4: ldr      r0, [r5, #4]
003c60f8: bl       #0x3a53e0
003c60fc: b        #0x3c60a8
003c6100: ldr      r0, [r5, #4]
003c6104: mov      r3, #0x2b
003c6108: mov      r1, sl
003c610c: mov      r2, ip
003c6110: add      r0, r0, #0x3b4
003c6114: str      ip, [sp]
003c6118: bl       #0x3dbe24
003c611c: ldr      r3, [r5, #0x2c]
003c6120: orr      r3, r3, #2
003c6124: str      r3, [r5, #0x2c]
003c6128: b        #0x3c6068
003c612c: subseq   lr, ip, r8, ror #20
003c6130: andeq    r2, r0, r0, asr #17
003c6134: andeq    r4, r0, r4, asr #16
003c6138: strdeq   r3, r4, [r0], -r4
003c613c: subeq    lr, pc, ip, lsr fp
003c6140: subeq    lr, pc, r0, lsr fp

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

# _ZN8CSScared7OnEventEiP9CharacterP16CharStateMachineiPv
003c2b28: push     {r4, lr}
003c2b2c: sub      sp, sp, #0x10
003c2b30: ldr      r3, [sp, #0x18]
003c2b34: mov      r4, r2
003c2b38: cmp      r3, #0x23
003c2b3c: bne      #0x3c2bdc
003c2b40: mov      r3, #0
003c2b44: movw     r0, #0x270e
003c2b48: str      r3, [sp, #0xc]
003c2b4c: str      r3, [sp, #4]
003c2b50: str      r3, [sp, #8]
003c2b54: bl       #0x3c26a0
003c2b58: bl       #0x30e964
003c2b5c: movw     r1, #0xb717
003c2b60: movt     r1, #0x38d1
003c2b64: bl       #0x30ed6c
003c2b68: movw     r1, #0xb717
003c2b6c: movt     r1, #0x3951
003c2b70: bl       #0x30eba4
003c2b74: str      r0, [sp, #4]
003c2b78: movw     r0, #0x270e
003c2b7c: bl       #0x3c26a0
003c2b80: bl       #0x30e964
003c2b84: movw     r1, #0xb717
003c2b88: movt     r1, #0x38d1
003c2b8c: bl       #0x30ed6c
003c2b90: movw     r1, #0xb717
003c2b94: movt     r1, #0x3951
003c2b98: bl       #0x30eba4
003c2b9c: str      r0, [sp, #8]
003c2ba0: mov      r0, #0x64
003c2ba4: bl       #0x3c26a0
003c2ba8: cmp      r0, #0x31
003c2bac: ldrle    r3, [sp, #4]
003c2bb0: mov      r0, #0x64
003c2bb4: addle    r3, r3, #0x80000000
003c2bb8: strle    r3, [sp, #4]
003c2bbc: bl       #0x3c26a0
003c2bc0: cmp      r0, #0x31
003c2bc4: ldrle    r3, [sp, #8]
003c2bc8: add      r1, sp, #4
003c2bcc: addle    r3, r3, #0x80000000
003c2bd0: strle    r3, [sp, #8]
003c2bd4: ldr      r0, [r4, #0x378]
003c2bd8: bl       #0x405374
003c2bdc: add      sp, sp, #0x10
003c2be0: pop      {r4, pc}

# _ZN16CharStateMachine16SM_SetScareStateEjbPvb
003c6144: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6148: mov      r5, r0
003c614c: sub      sp, sp, #8
003c6150: ldr      r0, [r0, #4]
003c6154: mov      sl, r1
003c6158: mov      r8, r2
003c615c: mov      r6, r3
003c6160: ldrb     r7, [sp, #0x28]
003c6164: bl       #0x3a3158
003c6168: ldr      r4, [pc, #0x104]
003c616c: cmp      r0, #0
003c6170: add      r4, pc, r4
003c6174: beq      #0x3c6180
003c6178: add      sp, sp, #8
003c617c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c6180: ldr      r0, [r5, #4]
003c6184: bl       #0x3a3228
003c6188: subs     sb, r0, #0
003c618c: blt      #0x3c6178
003c6190: ldr      r3, [pc, #0xe0]
003c6194: ldr      r3, [r4, r3]
003c6198: ldr      r3, [r3]
003c619c: cmp      sb, r3
003c61a0: bge      #0x3c6178
003c61a4: ldr      ip, [r5, #0x2c]
003c61a8: ands     ip, ip, #4
003c61ac: beq      #0x3c6248
003c61b0: ldr      r3, [pc, #0xc4]
003c61b4: ldr      r2, [pc, #0xc4]
003c61b8: ldr      r1, [pc, #0xc4]
003c61bc: ldr      r3, [r4, r3]
003c61c0: ldr      r2, [r4, r2]
003c61c4: add      r1, pc, r1
003c61c8: ldr      r3, [r3]
003c61cc: ldr      r0, [r2, #0x2c]
003c61d0: mov      r2, #0xa0
003c61d4: mla      sb, r2, sb, r3
003c61d8: ldr      r2, [pc, #0xa8]
003c61dc: ldr      r4, [sb, #0x7c]
003c61e0: add      r2, pc, r2
003c61e4: bl       #0x4c4bdc
003c61e8: ands     r0, r0, #0x100
003c61ec: bne      #0x3c623c
003c61f0: add      r4, r0, r4
003c61f4: cmp      r7, #0
003c61f8: str      r4, [r5, #0x28]
003c61fc: beq      #0x3c6228
003c6200: mov      r3, r6
003c6204: mov      r0, r5
003c6208: mov      r1, #8
003c620c: movw     r2, #0xc35d
003c6210: bl       #0x3c1938
003c6214: cmp      r8, #0
003c6218: ldrne    r3, [r5, #0x24]
003c621c: orrne    r3, r3, #0x400
003c6220: strne    r3, [r5, #0x24]
003c6224: b        #0x3c6178
003c6228: mov      r2, r6
003c622c: mov      r0, r5
003c6230: movw     r1, #0xc35d
003c6234: bl       #0x3c5684
003c6238: b        #0x3c6214
003c623c: ldr      r0, [r5, #4]
003c6240: bl       #0x3a53e0
003c6244: b        #0x3c61f0
003c6248: ldr      r0, [r5, #4]
003c624c: mov      r3, #0x2c
003c6250: mov      r1, sl
003c6254: mov      r2, ip
003c6258: add      r0, r0, #0x3b4
003c625c: str      ip, [sp]
003c6260: bl       #0x3dbe24
003c6264: ldr      r3, [r5, #0x2c]
003c6268: orr      r3, r3, #4
003c626c: str      r3, [r5, #0x2c]
003c6270: b        #0x3c61b0
003c6274: subseq   lr, ip, r0, lsr #18
003c6278: andeq    r2, r0, r0, asr #17
003c627c: andeq    r4, r0, r4, asr #16
003c6280: strdeq   r3, r4, [r0], -r4
003c6284: strdeq   lr, pc, [pc], #-0x94
003c6288: subeq    lr, pc, r8, ror #19

# _ZNK9Character6IsBossEv
003a3158: push     {r4, lr}
003a315c: bl       #0x3a3024
003a3160: ldr      r0, [r0, #0x14]
003a3164: ubfx     r0, r0, #2, #1
003a3168: pop      {r4, pc}

# _ZN9CSStunned7OnEventEiP9CharacterP16CharStateMachineiPv
003c0030: bx       lr

# _ZN8CSScared6OnBlurEiP9CharacterP16CharStateMachinei
003c4834: push     {r4, r5, r6, r7, r8, lr}
003c4838: ldr      r4, [pc, #0x90]
003c483c: ldr      r6, [pc, #0x90]
003c4840: ldr      r1, [pc, #0x90]
003c4844: add      r4, pc, r4
003c4848: ldr      r3, [r4, r6]
003c484c: ldr      r8, [r4, r1]
003c4850: sub      sp, sp, #0x20
003c4854: ldr      r3, [r3]
003c4858: mov      r0, r8
003c485c: mov      r7, r2
003c4860: str      r3, [sp, #0x1c]
003c4864: bl       #0x337888
003c4868: ldr      r1, [pc, #0x6c]
003c486c: add      r5, sp, #4
003c4870: mov      r2, sp
003c4874: add      r1, pc, r1
003c4878: mov      r0, r5
003c487c: bl       #0x3140ec
003c4880: mov      r1, r5
003c4884: mov      r0, r8
003c4888: bl       #0x337a88
003c488c: mov      r0, r5
003c4890: bl       #0x318254
003c4894: ldr      r0, [r7, #0x378]
003c4898: mov      r1, #0
003c489c: bl       #0x4053d0
003c48a0: ldr      r0, [r7, #0x2dc]
003c48a4: cmp      r0, #0
003c48a8: beq      #0x3c48b0
003c48ac: bl       #0x46eb20
003c48b0: ldr      r3, [r4, r6]
003c48b4: ldr      r2, [sp, #0x1c]
003c48b8: ldr      r3, [r3]
003c48bc: cmp      r2, r3
003c48c0: bne      #0x3c48cc
003c48c4: add      sp, sp, #0x20
003c48c8: pop      {r4, r5, r6, r7, r8, pc}
003c48cc: bl       #0x30e310
003c48d0: subseq   r0, sp, ip, asr #4
003c48d4: andeq    r4, r0, ip, lsr #1
003c48d8: andeq    r0, r0, r4, lsl #17
003c48dc: ldrsbeq  r0, [r0], #-0x5c

# _Z11GetNewStateI8CSScaredEP9CharStatev
003c0534: ldr      r3, [pc, #0xc]
003c0538: ldr      r2, [pc, #0xc]
003c053c: add      r3, pc, r3
003c0540: ldr      r0, [r3, r2]
003c0544: bx       lr
003c0548: subseq   r4, sp, r4, asr r5
003c054c: andeq    r4, r0, ip, lsr #24

# _ZN8CSScared8OnUpdateEiP9CharacterP16CharStateMachine
003c4788: push     {r4, r5, r6, r7, r8, sl, lr}
003c478c: ldr      r4, [pc, #0x90]
003c4790: ldr      r5, [pc, #0x90]
003c4794: ldr      r8, [r2, #0x528]
003c4798: add      r4, pc, r4
003c479c: ldr      r3, [r4, r5]
003c47a0: sub      sp, sp, #0x24
003c47a4: ands     r8, r8, #4
003c47a8: ldr      r3, [r3]
003c47ac: mov      r6, r2
003c47b0: str      r3, [sp, #0x1c]
003c47b4: bne      #0x3c4804
003c47b8: ldr      r3, [pc, #0x6c]
003c47bc: add      r7, sp, #4
003c47c0: ldr      sl, [r4, r3]
003c47c4: mov      r0, sl
003c47c8: bl       #0x337888
003c47cc: ldr      r1, [pc, #0x5c]
003c47d0: mov      r2, sp
003c47d4: mov      r0, r7
003c47d8: add      r1, pc, r1
003c47dc: bl       #0x3140ec
003c47e0: mov      r1, r7
003c47e4: mov      r0, sl
003c47e8: bl       #0x337a88
003c47ec: mov      r0, r7
003c47f0: bl       #0x318254
003c47f4: add      r0, r6, #0x490
003c47f8: add      r0, r0, #0xc
003c47fc: mov      r1, r8
003c4800: bl       #0x3c948c
003c4804: ldr      r3, [r4, r5]
003c4808: ldr      r2, [sp, #0x1c]
003c480c: ldr      r3, [r3]
003c4810: cmp      r2, r3
003c4814: bne      #0x3c4820
003c4818: add      sp, sp, #0x24
003c481c: pop      {r4, r5, r6, r7, r8, sl, pc}
003c4820: bl       #0x30e310
003c4824: ldrsheq  r0, [sp], #-0x28
003c4828: andeq    r4, r0, ip, lsr #1
003c482c: andeq    r0, r0, r4, lsl #17
003c4830: ldrsheq  r0, [r0], #-0x60

# _ZN8CSScared7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c45c4: push     {r4, r5, r6, r7, r8, lr}
003c45c8: ldr      r4, [pc, #0x198]
003c45cc: ldr      r7, [pc, #0x198]
003c45d0: ldr      r1, [pc, #0x198]
003c45d4: add      r4, pc, r4
003c45d8: ldr      r3, [r4, r7]
003c45dc: ldr      r8, [r4, r1]
003c45e0: sub      sp, sp, #0x30
003c45e4: ldr      r3, [r3]
003c45e8: mov      r0, r8
003c45ec: mov      r5, r2
003c45f0: str      r3, [sp, #0x2c]
003c45f4: bl       #0x337888
003c45f8: ldr      r1, [pc, #0x174]
003c45fc: add      r6, sp, #0x14
003c4600: add      r2, sp, #0x10
003c4604: mov      r0, r6
003c4608: add      r1, pc, r1
003c460c: bl       #0x3140ec
003c4610: mov      r1, r6
003c4614: mov      r0, r8
003c4618: bl       #0x337a88
003c461c: mov      r0, r6
003c4620: bl       #0x318254
003c4624: mov      r3, #0x2240
003c4628: str      r3, [r5, #0x520]
003c462c: ldr      r3, [pc, #0x144]
003c4630: mov      r0, r5
003c4634: add      r6, r5, #0x490
003c4638: ldr      r3, [r4, r3]
003c463c: add      r6, r6, #0xc
003c4640: ldr      r8, [r3]
003c4644: bl       #0x3a3228
003c4648: ldr      r3, [pc, #0x12c]
003c464c: ldr      r1, [pc, #0x12c]
003c4650: ldr      r2, [r4, r3]
003c4654: mov      r3, #0xa0
003c4658: mla      r3, r3, r0, r8
003c465c: ldr      r0, [r2, #0x2c]
003c4660: ldr      r2, [pc, #0x11c]
003c4664: add      r1, pc, r1
003c4668: ldr      r8, [r3, #0x7c]
003c466c: add      r2, pc, r2
003c4670: bl       #0x4c4bdc
003c4674: ands     r0, r0, #0x100
003c4678: bne      #0x3c4758
003c467c: add      r1, r0, r8
003c4680: mov      r0, r6
003c4684: bl       #0x3cacb0
003c4688: mov      r3, #0
003c468c: movw     r0, #0x270e
003c4690: str      r3, [sp, #0xc]
003c4694: str      r3, [sp, #4]
003c4698: str      r3, [sp, #8]
003c469c: bl       #0x3c26a0
003c46a0: bl       #0x30e964
003c46a4: movw     r1, #0xb717
003c46a8: movt     r1, #0x38d1
003c46ac: bl       #0x30ed6c
003c46b0: movw     r1, #0xb717
003c46b4: movt     r1, #0x3951
003c46b8: bl       #0x30eba4
003c46bc: str      r0, [sp, #4]
003c46c0: movw     r0, #0x270e
003c46c4: bl       #0x3c26a0
003c46c8: bl       #0x30e964
003c46cc: movw     r1, #0xb717
003c46d0: movt     r1, #0x38d1
003c46d4: bl       #0x30ed6c
003c46d8: movw     r1, #0xb717
003c46dc: movt     r1, #0x3951
003c46e0: bl       #0x30eba4
003c46e4: str      r0, [sp, #8]
003c46e8: mov      r0, #0x64
003c46ec: bl       #0x3c26a0
003c46f0: cmp      r0, #0x31
003c46f4: ldrle    r3, [sp, #4]
003c46f8: mov      r0, #0x64
003c46fc: addle    r3, r3, #0x80000000
003c4700: strle    r3, [sp, #4]
003c4704: bl       #0x3c26a0
003c4708: cmp      r0, #0x31
003c470c: ldrle    r3, [sp, #8]
003c4710: add      r1, sp, #4
003c4714: addle    r3, r3, #0x80000000
003c4718: strle    r3, [sp, #8]
003c471c: ldr      r0, [r5, #0x378]
003c4720: bl       #0x405374
003c4724: mov      r0, r5
003c4728: bl       #0x3bc6b8
003c472c: ldr      r0, [r5, #0x2dc]
003c4730: cmp      r0, #0
003c4734: beq      #0x3c473c
003c4738: bl       #0x46eae0
003c473c: ldr      r3, [r4, r7]
003c4740: ldr      r2, [sp, #0x2c]
003c4744: ldr      r3, [r3]
003c4748: cmp      r2, r3
003c474c: bne      #0x3c4764
003c4750: add      sp, sp, #0x30
003c4754: pop      {r4, r5, r6, r7, r8, pc}
003c4758: mov      r0, r5
003c475c: bl       #0x3a53e0
003c4760: b        #0x3c467c
003c4764: bl       #0x30e310
003c4768: ldrheq   r0, [sp], #-0x4c
003c476c: andeq    r4, r0, ip, lsr #1
003c4770: andeq    r0, r0, r4, lsl #17
003c4774: subseq   r0, r0, r8, asr #16
003c4778: andeq    r4, r0, r4, asr #16
003c477c: strdeq   r3, r4, [r0], -r4
003c4780: subseq   r0, r0, r4, asr r5
003c4784: subseq   r0, r0, ip, asr r5

# _ZNK15PyDataConstants11getConstantEPKcS1_
004c4bdc: push     {r4, lr}
004c4be0: add      r4, r0, #4
004c4be4: sub      sp, sp, #8
004c4be8: str      r1, [sp, #4]
004c4bec: mov      r0, r4
004c4bf0: add      r1, sp, #4
004c4bf4: str      r2, [sp]
004c4bf8: bl       #0x4c4998
004c4bfc: cmp      r0, r4
004c4c00: beq      #0x4c4c28
004c4c04: add      r4, r0, #0x28
004c4c08: mov      r0, r4
004c4c0c: mov      r1, sp
004c4c10: bl       #0x414484
004c4c14: cmp      r0, r4
004c4c18: ldrne    r0, [r0, #0x28]
004c4c1c: beq      #0x4c4c28
004c4c20: add      sp, sp, #8
004c4c24: pop      {r4, pc}
004c4c28: mov      r0, #0
004c4c2c: b        #0x4c4c20

# _ZN9CSStunned8OnUpdateEiP9CharacterP16CharStateMachine
003c549c: push     {r4, r5, r6, r7, r8, sl, lr}
003c54a0: ldr      r4, [pc, #0x90]
003c54a4: ldr      r5, [pc, #0x90]
003c54a8: ldr      r8, [r2, #0x528]
003c54ac: add      r4, pc, r4
003c54b0: ldr      r3, [r4, r5]
003c54b4: sub      sp, sp, #0x24
003c54b8: ands     r8, r8, #2
003c54bc: ldr      r3, [r3]
003c54c0: mov      r6, r2
003c54c4: str      r3, [sp, #0x1c]
003c54c8: bne      #0x3c5518
003c54cc: ldr      r3, [pc, #0x6c]
003c54d0: add      r7, sp, #4
003c54d4: ldr      sl, [r4, r3]
003c54d8: mov      r0, sl
003c54dc: bl       #0x337888
003c54e0: ldr      r1, [pc, #0x5c]
003c54e4: mov      r2, sp
003c54e8: mov      r0, r7
003c54ec: add      r1, pc, r1
003c54f0: bl       #0x3140ec
003c54f4: mov      r1, r7
003c54f8: mov      r0, sl
003c54fc: bl       #0x337a88
003c5500: mov      r0, r7
003c5504: bl       #0x318254
003c5508: add      r0, r6, #0x4f0
003c550c: add      r0, r0, #0xc
003c5510: mov      r1, r8
003c5514: bl       #0x3c1a00
003c5518: ldr      r3, [r4, r5]
003c551c: ldr      r2, [sp, #0x1c]
003c5520: ldr      r3, [r3]
003c5524: cmp      r2, r3
003c5528: bne      #0x3c5534
003c552c: add      sp, sp, #0x24
003c5530: pop      {r4, r5, r6, r7, r8, sl, pc}
003c5534: bl       #0x30e310
003c5538: subseq   pc, ip, r4, ror #11
003c553c: andeq    r4, r0, ip, lsr #1
003c5540: andeq    r0, r0, r4, lsl #17
003c5544: subeq    pc, pc, r4, asr #19

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

# _ZN9CSStunned7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3cc0: push     {r4, r5, r6, r7, r8, lr}
003c3cc4: ldr      r4, [pc, #0x150]
003c3cc8: ldr      r8, [pc, #0x150]
003c3ccc: ldr      r1, [pc, #0x150]
003c3cd0: add      r4, pc, r4
003c3cd4: ldr      r3, [r4, r8]
003c3cd8: ldr      r6, [r4, r1]
003c3cdc: sub      sp, sp, #0x40
003c3ce0: ldr      r3, [r3]
003c3ce4: mov      r0, r6
003c3ce8: mov      r5, r2
003c3cec: str      r3, [sp, #0x3c]
003c3cf0: bl       #0x337888
003c3cf4: ldr      r1, [pc, #0x12c]
003c3cf8: add      r7, sp, #0x24
003c3cfc: add      r2, sp, #8
003c3d00: mov      r0, r7
003c3d04: add      r1, pc, r1
003c3d08: bl       #0x3140ec
003c3d0c: mov      r1, r7
003c3d10: mov      r0, r6
003c3d14: bl       #0x337a88
003c3d18: mov      r0, r7
003c3d1c: bl       #0x318254
003c3d20: mov      r0, r6
003c3d24: bl       #0x337888
003c3d28: ldr      r1, [pc, #0xfc]
003c3d2c: add      r7, sp, #0xc
003c3d30: add      r2, sp, #4
003c3d34: add      r1, pc, r1
003c3d38: mov      r0, r7
003c3d3c: bl       #0x3140ec
003c3d40: mov      r1, r7
003c3d44: mov      r0, r6
003c3d48: bl       #0x337a88
003c3d4c: mov      r0, r7
003c3d50: bl       #0x318254
003c3d54: movw     r3, #0x2202
003c3d58: str      r3, [r5, #0x520]
003c3d5c: ldr      r3, [pc, #0xcc]
003c3d60: mov      r0, r5
003c3d64: add      r6, r5, #0x490
003c3d68: ldr      r3, [r4, r3]
003c3d6c: add      r6, r6, #0xc
003c3d70: ldr      r7, [r3]
003c3d74: bl       #0x3a3228
003c3d78: ldr      r3, [pc, #0xb4]
003c3d7c: ldr      r1, [pc, #0xb4]
003c3d80: ldr      r2, [r4, r3]
003c3d84: mov      r3, #0xa0
003c3d88: mla      r3, r3, r0, r7
003c3d8c: ldr      r0, [r2, #0x2c]
003c3d90: ldr      r2, [pc, #0xa4]
003c3d94: add      r1, pc, r1
003c3d98: ldr      r7, [r3, #0x8c]
003c3d9c: add      r2, pc, r2
003c3da0: bl       #0x4c4bdc
003c3da4: ands     r0, r0, #0x200
003c3da8: bne      #0x3c3e0c
003c3dac: add      r1, r0, r7
003c3db0: mov      r0, r6
003c3db4: bl       #0x3cacb0
003c3db8: ldr      r3, [r5]
003c3dbc: mov      r0, r5
003c3dc0: mov      lr, pc
003c3dc4: ldr      pc, [r3, #0x28]
003c3dc8: cmp      r0, #0
003c3dcc: ldrne    r3, [r5, #0x378]
003c3dd0: movne    r2, #1
003c3dd4: mov      r0, r5
003c3dd8: strbne   r2, [r3, #8]
003c3ddc: bl       #0x3bc6b8
003c3de0: ldr      r0, [r5, #0x2dc]
003c3de4: cmp      r0, #0
003c3de8: beq      #0x3c3df0
003c3dec: bl       #0x46eae0
003c3df0: ldr      r3, [r4, r8]
003c3df4: ldr      r2, [sp, #0x3c]
003c3df8: ldr      r3, [r3]
003c3dfc: cmp      r2, r3
003c3e00: bne      #0x3c3e18
003c3e04: add      sp, sp, #0x40
003c3e08: pop      {r4, r5, r6, r7, r8, pc}
003c3e0c: mov      r0, r5
003c3e10: bl       #0x3a53e0
003c3e14: b        #0x3c3dac
003c3e18: bl       #0x30e310
003c3e1c: subseq   r0, sp, r0, asr #27
003c3e20: andeq    r4, r0, ip, lsr #1
003c3e24: andeq    r0, r0, r4, lsl #17
003c3e28: subseq   r1, r0, ip, asr #2
003c3e2c: subseq   r1, r0, ip, ror r1
003c3e30: andeq    r4, r0, r4, asr #16
003c3e34: strdeq   r3, r4, [r0], -r4
003c3e38: subseq   r0, r0, r4, lsr #28
003c3e3c: subseq   r0, r0, ip, lsr #28

# _ZN8CSScared6OnInitEiP9CharacterP16CharStateMachine
003c861c: push     {r4, r5, r6, r7, lr}
003c8620: add      r5, r2, #0x4f0
003c8624: add      r5, r5, #0xc
003c8628: sub      sp, sp, #0x3c
003c862c: mov      r4, #0
003c8630: mov      r6, r1
003c8634: mov      r0, r5
003c8638: mov      r2, #0x2c
003c863c: mov      r3, #3
003c8640: str      r4, [sp, #0x30]
003c8644: str      r4, [sp, #0x34]
003c8648: str      r4, [sp]
003c864c: str      r4, [sp, #4]
003c8650: bl       #0x3c7b18
003c8654: mov      r0, r5
003c8658: mov      r1, r6
003c865c: mov      r2, #0x22
003c8660: mov      r3, #3
003c8664: str      r4, [sp, #0x28]
003c8668: str      r4, [sp, #0x2c]
003c866c: str      r4, [sp]
003c8670: str      r4, [sp, #4]
003c8674: ldr      r7, [pc, #0xa8]
003c8678: bl       #0x3c7b18
003c867c: mov      r0, r5
003c8680: mov      r1, r6
003c8684: movw     r2, #0xc358
003c8688: mov      r3, #0xc
003c868c: str      r4, [sp, #0x20]
003c8690: str      r4, [sp, #0x24]
003c8694: str      r4, [sp]
003c8698: str      r4, [sp, #4]
003c869c: bl       #0x3c7b18
003c86a0: ldr      r3, [pc, #0x80]
003c86a4: add      r7, pc, r7
003c86a8: mov      r0, r5
003c86ac: ldr      ip, [r7, r3]
003c86b0: mov      r1, r6
003c86b4: movw     r2, #0xc35a
003c86b8: mov      r3, #0xb
003c86bc: str      ip, [sp]
003c86c0: str      ip, [sp, #0x18]
003c86c4: str      r4, [sp, #0x1c]
003c86c8: str      r4, [sp, #4]
003c86cc: bl       #0x3c7b18
003c86d0: ldr      r3, [pc, #0x54]
003c86d4: mov      r0, r5
003c86d8: mov      r1, r6
003c86dc: ldr      r7, [r7, r3]
003c86e0: movw     r2, #0xc35b
003c86e4: mov      r3, #0xa
003c86e8: str      r7, [sp, #0x10]
003c86ec: str      r4, [sp, #0x14]
003c86f0: str      r7, [sp]
003c86f4: str      r4, [sp, #4]
003c86f8: bl       #0x3c7b18
003c86fc: mov      r0, r5
003c8700: mov      r1, r6
003c8704: movw     r2, #0xc35c
003c8708: mov      r3, #9
003c870c: str      r7, [sp]
003c8710: stmib    sp, {r4, r7}
003c8714: str      r4, [sp, #0xc]
003c8718: bl       #0x3c7b18
003c871c: add      sp, sp, #0x3c
003c8720: pop      {r4, r5, r6, r7, pc}
003c8724: subseq   ip, ip, ip, ror #7
003c8728: andeq    r2, r0, r4, lsl #29
003c872c: andeq    r3, r0, ip, asr #9

# _ZN9CSStunned6OnBlurEiP9CharacterP16CharStateMachinei
003c3b4c: push     {r4, r5, r6, r7, r8, lr}
003c3b50: ldr      r4, [pc, #0x90]
003c3b54: ldr      r6, [pc, #0x90]
003c3b58: ldr      r1, [pc, #0x90]
003c3b5c: add      r4, pc, r4
003c3b60: ldr      r3, [r4, r6]
003c3b64: ldr      r8, [r4, r1]
003c3b68: sub      sp, sp, #0x20
003c3b6c: ldr      r3, [r3]
003c3b70: mov      r0, r8
003c3b74: mov      r7, r2
003c3b78: str      r3, [sp, #0x1c]
003c3b7c: bl       #0x337888
003c3b80: ldr      r1, [pc, #0x6c]
003c3b84: add      r5, sp, #4
003c3b88: mov      r2, sp
003c3b8c: add      r1, pc, r1
003c3b90: mov      r0, r5
003c3b94: bl       #0x3140ec
003c3b98: mov      r1, r5
003c3b9c: mov      r0, r8
003c3ba0: bl       #0x337a88
003c3ba4: mov      r0, r5
003c3ba8: bl       #0x318254
003c3bac: ldr      r3, [r7, #0x378]
003c3bb0: mov      r2, #0
003c3bb4: strb     r2, [r3, #8]
003c3bb8: ldr      r0, [r7, #0x2dc]
003c3bbc: cmp      r0, r2
003c3bc0: beq      #0x3c3bc8
003c3bc4: bl       #0x46eb20
003c3bc8: ldr      r3, [r4, r6]
003c3bcc: ldr      r2, [sp, #0x1c]
003c3bd0: ldr      r3, [r3]
003c3bd4: cmp      r2, r3
003c3bd8: bne      #0x3c3be4
003c3bdc: add      sp, sp, #0x20
003c3be0: pop      {r4, r5, r6, r7, r8, pc}
003c3be4: bl       #0x30e310
003c3be8: subseq   r0, sp, r4, lsr pc
003c3bec: andeq    r4, r0, ip, lsr #1
003c3bf0: andeq    r0, r0, r4, lsl #17
003c3bf4: subseq   r1, r0, r4, asr #5

# _ZNK9Character18GetCharAnimTableIdEv
003a3228: mov      r3, #0x1000
003a322c: ldr      r0, [r0, r3]
003a3230: ldr      r3, [pc, #0x24]
003a3234: cmp      r0, #0
003a3238: add      r3, pc, r3
003a323c: blt      #0x3a3254
003a3240: ldr      r2, [pc, #0x18]
003a3244: ldr      r3, [r3, r2]
003a3248: ldr      r3, [r3]
003a324c: cmp      r0, r3
003a3250: bxlt     lr
003a3254: mov      r0, #0x11
003a3258: bx       lr
003a325c: subseq   r1, pc, r8, asr r8
003a3260: andeq    r2, r0, r0, asr #17

# _Z11GetNewStateI9CSStunnedEP9CharStatev
003c0550: ldr      r3, [pc, #0xc]
003c0554: ldr      r2, [pc, #0xc]
003c0558: add      r3, pc, r3
003c055c: ldr      r0, [r3, r2]
003c0560: bx       lr
003c0564: subseq   r4, sp, r8, lsr r5
003c0568: andeq    r4, r0, r8, lsl r7

# _ZN9CSStunned6OnInitEiP9CharacterP16CharStateMachine
003c8730: push     {r4, r5, r6, r7, lr}
003c8734: add      r6, r2, #0x4f0
003c8738: add      r6, r6, #0xc
003c873c: sub      sp, sp, #0x24
003c8740: mov      r4, #0
003c8744: mov      r0, r6
003c8748: movw     r2, #0xc358
003c874c: mov      r3, #0xc
003c8750: ldr      r5, [pc, #0x74]
003c8754: mov      r7, r1
003c8758: str      r4, [sp, #0x18]
003c875c: str      r4, [sp, #0x1c]
003c8760: str      r4, [sp]
003c8764: str      r4, [sp, #4]
003c8768: bl       #0x3c7b18
003c876c: ldr      r3, [pc, #0x5c]
003c8770: add      r5, pc, r5
003c8774: mov      r0, r6
003c8778: ldr      ip, [r5, r3]
003c877c: mov      r1, r7
003c8780: movw     r2, #0xc35a
003c8784: mov      r3, #0xb
003c8788: str      ip, [sp]
003c878c: str      ip, [sp, #0x10]
003c8790: str      r4, [sp, #0x14]
003c8794: str      r4, [sp, #4]
003c8798: bl       #0x3c7b18
003c879c: ldr      r3, [pc, #0x30]
003c87a0: mov      r0, r6
003c87a4: mov      r1, r7
003c87a8: ldr      ip, [r5, r3]
003c87ac: movw     r2, #0xc35b
003c87b0: mov      r3, #0xa
003c87b4: str      ip, [sp]
003c87b8: stmib    sp, {r4, ip}
003c87bc: str      r4, [sp, #0xc]
003c87c0: bl       #0x3c7b18
003c87c4: add      sp, sp, #0x24
003c87c8: pop      {r4, r5, r6, r7, pc}
003c87cc: subseq   ip, ip, r0, lsr #6
003c87d0: andeq    r2, r0, r4, lsl #29
003c87d4: andeq    r3, r0, ip, asr #9
