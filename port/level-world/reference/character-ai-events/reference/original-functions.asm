
# _ZN6CharAI10_SpellBlurEv
003d8b28: push     {r4, lr}
003d8b2c: mvn      r1, #0
003d8b30: mov      r4, r0
003d8b34: ldr      r0, [r0, #4]
003d8b38: bl       #0x3bb98c
003d8b3c: ldr      r3, [r4, #0xc0]
003d8b40: ldr      r2, [r4, #0xc4]
003d8b44: rsb      r2, r3, r2
003d8b48: cmp      r0, r2, asr #2
003d8b4c: bhs      #0x3d8b78
003d8b50: ldr      r3, [r3, r0, lsl #2]
003d8b54: cmp      r3, #0
003d8b58: beq      #0x3d8b78
003d8b5c: ldr      r0, [r4, #4]
003d8b60: mvn      r1, #0
003d8b64: bl       #0x3bb98c
003d8b68: ldr      r3, [r4, #0xc0]
003d8b6c: ldr      r0, [r3, r0, lsl #2]
003d8b70: pop      {r4, lr}
003d8b74: b        #0x3da6c0
003d8b78: pop      {r4, pc}

# _ZN6CharAI19_OnEndOfAnimSectionEv
003d3ae4: mov      r0, #1
003d3ae8: bx       lr

# _ZN6CharAI12_UpdateRegenEv
003cb77c: push     {r4, lr}
003cb780: ldr      r3, [r0, #4]
003cb784: mov      r4, r0
003cb788: mov      r0, r3
003cb78c: ldr      r3, [r3]
003cb790: mov      lr, pc
003cb794: ldr      pc, [r3, #0x54]
003cb798: cmp      r0, #0
003cb79c: beq      #0x3cb7a4
003cb7a0: pop      {r4, pc}
003cb7a4: mov      r0, r4
003cb7a8: ldr      r4, [r4, #4]
003cb7ac: bl       #0x3d4bc4
003cb7b0: mov      r1, r0
003cb7b4: mov      r0, r4
003cb7b8: pop      {r4, lr}
003cb7bc: b        #0x3bdd90

# _ZN6CharAI11_SpellFocusEv
003d8038: push     {r4, lr}
003d803c: mvn      r1, #0
003d8040: mov      r4, r0
003d8044: ldr      r0, [r0, #4]
003d8048: bl       #0x3bb98c
003d804c: ldr      r3, [r4, #0xc0]
003d8050: ldr      r2, [r4, #0xc4]
003d8054: rsb      r2, r3, r2
003d8058: cmp      r0, r2, asr #2
003d805c: bhs      #0x3d8088
003d8060: ldr      r3, [r3, r0, lsl #2]
003d8064: cmp      r3, #0
003d8068: beq      #0x3d8088
003d806c: ldr      r0, [r4, #4]
003d8070: mvn      r1, #0
003d8074: bl       #0x3bb98c
003d8078: ldr      r3, [r4, #0xc0]
003d807c: ldr      r0, [r3, r0, lsl #2]
003d8080: pop      {r4, lr}
003d8084: b        #0x3da8b8
003d8088: pop      {r4, pc}

# _ZN6CharAI12_OnAnimEventEPKc
003d4434: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d4438: ldr      r4, [pc, #0x528]
003d443c: ldr      r7, [pc, #0x528]
003d4440: mov      r6, r0
003d4444: add      r4, pc, r4
003d4448: ldr      r3, [r4, r7]
003d444c: ldr      r0, [r0, #4]
003d4450: sub      sp, sp, #0xd4
003d4454: ldr      r3, [r3]
003d4458: add      r0, r0, #0x490
003d445c: add      r0, r0, #0xc
003d4460: mov      r5, r1
003d4464: str      r3, [sp, #0xcc]
003d4468: bl       #0x3c932c
003d446c: mov      r8, r0
003d4470: ldr      r0, [r6, #4]
003d4474: add      r0, r0, #0x490
003d4478: add      r0, r0, #0xc
003d447c: bl       #0x3c934c
003d4480: ldr      r1, [pc, #0x4e8]
003d4484: mov      r0, r5
003d4488: mov      r2, #3
003d448c: add      r1, pc, r1
003d4490: bl       #0x30ec7c
003d4494: cmp      r0, #0
003d4498: beq      #0x3d4634
003d449c: ldr      r1, [pc, #0x4d0]
003d44a0: mov      r0, r5
003d44a4: mov      r2, #3
003d44a8: add      r1, pc, r1
003d44ac: bl       #0x30ec7c
003d44b0: cmp      r0, #0
003d44b4: beq      #0x3d464c
003d44b8: ldr      r1, [pc, #0x4b8]
003d44bc: mov      r0, r5
003d44c0: mov      r2, #3
003d44c4: add      r1, pc, r1
003d44c8: bl       #0x30ec7c
003d44cc: subs     sl, r0, #0
003d44d0: beq      #0x3d45bc
003d44d4: ldr      r1, [pc, #0x4a0]
003d44d8: mov      r0, r5
003d44dc: mov      r2, #4
003d44e0: add      r1, pc, r1
003d44e4: bl       #0x30ec7c
003d44e8: cmp      r0, #0
003d44ec: bne      #0x3d465c
003d44f0: ldr      r3, [pc, #0x488]
003d44f4: add      r5, r5, #4
003d44f8: ldr      r3, [r4, r3]
003d44fc: ldr      sb, [r3]
003d4500: cmp      sb, #0
003d4504: beq      #0x3d459c
003d4508: ldr      r3, [pc, #0x474]
003d450c: mov      r8, r0
003d4510: ldr      r3, [r4, r3]
003d4514: ldr      fp, [r3]
003d4518: b        #0x3d4528
003d451c: add      r8, r8, #1
003d4520: cmp      r8, sb
003d4524: beq      #0x3d459c
003d4528: mov      r0, r5
003d452c: ldr      r1, [fp, r8, lsl #2]
003d4530: bl       #0x30e31c
003d4534: subs     sl, r0, #0
003d4538: bne      #0x3d451c
003d453c: cmn      r8, #1
003d4540: beq      #0x3d459c
003d4544: ldr      r3, [pc, #0x43c]
003d4548: ldr      r0, [r6, #4]
003d454c: ldr      r3, [r4, r3]
003d4550: ldr      sb, [r3]
003d4554: bl       #0x3935dc
003d4558: ldr      lr, [r0, #4]
003d455c: ldr      r5, [r0]
003d4560: ldr      r6, [r0, #8]
003d4564: mov      ip, #0xbf000000
003d4568: add      ip, ip, #0x800000
003d456c: str      lr, [sp, #0x14]
003d4570: mov      r0, sb
003d4574: mov      lr, #1
003d4578: mov      r1, r8
003d457c: mov      r3, sl
003d4580: add      r2, sp, #0x10
003d4584: str      r5, [sp, #0x10]
003d4588: str      r6, [sp, #0x18]
003d458c: str      lr, [sp]
003d4590: str      ip, [sp, #8]
003d4594: str      ip, [sp, #4]
003d4598: bl       #0x36b5d8
003d459c: ldr      r3, [r4, r7]
003d45a0: ldr      r2, [sp, #0xcc]
003d45a4: mov      r0, #1
003d45a8: ldr      r3, [r3]
003d45ac: cmp      r2, r3
003d45b0: bne      #0x3d4964
003d45b4: add      sp, sp, #0xd4
003d45b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d45bc: ldr      r3, [pc, #0x3c8]
003d45c0: add      r5, r5, #3
003d45c4: ldr      r3, [r4, r3]
003d45c8: ldr      sb, [r3]
003d45cc: cmp      sb, #0
003d45d0: beq      #0x3d459c
003d45d4: ldr      r3, [pc, #0x3b4]
003d45d8: ldr      r3, [r4, r3]
003d45dc: ldr      fp, [r3]
003d45e0: b        #0x3d45f0
003d45e4: add      sl, sl, #1
003d45e8: cmp      sl, sb
003d45ec: beq      #0x3d459c
003d45f0: mov      r0, r5
003d45f4: ldr      r1, [fp, sl, lsl #2]
003d45f8: bl       #0x30e31c
003d45fc: subs     r8, r0, #0
003d4600: bne      #0x3d45e4
003d4604: cmn      sl, #1
003d4608: beq      #0x3d459c
003d460c: ldr      r0, [r6, #4]
003d4610: bl       #0x3935dc
003d4614: ldr      r3, [pc, #0x378]
003d4618: mov      r2, r0
003d461c: mov      r1, sl
003d4620: ldr      r0, [r4, r3]
003d4624: mov      r3, r8
003d4628: str      r8, [sp]
003d462c: bl       #0x495d14
003d4630: b        #0x3d459c
003d4634: mov      r0, r6
003d4638: add      r1, r5, #3
003d463c: ldr      r3, [r6]
003d4640: mov      lr, pc
003d4644: ldr      pc, [r3, #0x94]
003d4648: b        #0x3d459c
003d464c: mov      r0, r6
003d4650: add      r1, r5, #3
003d4654: bl       #0x3d3af4
003d4658: b        #0x3d459c
003d465c: ldr      r0, [r6, #4]
003d4660: add      r0, r0, #0x4f0
003d4664: add      r0, r0, #0xc
003d4668: bl       #0x3c01ac
003d466c: sub      r0, r0, #5
003d4670: cmp      r0, #8
003d4674: addls    pc, pc, r0, lsl #2
003d4678: b        #0x3d459c
003d467c: b        #0x3d4750
003d4680: b        #0x3d46f8
003d4684: b        #0x3d46a0
003d4688: b        #0x3d459c
003d468c: b        #0x3d459c
003d4690: b        #0x3d459c
003d4694: b        #0x3d459c
003d4698: b        #0x3d459c
003d469c: b        #0x3d482c
003d46a0: ldr      r1, [pc, #0x2f0]
003d46a4: mov      r0, r5
003d46a8: add      r1, pc, r1
003d46ac: bl       #0x30e31c
003d46b0: cmp      r0, #0
003d46b4: bne      #0x3d459c
003d46b8: ldr      r3, [pc, #0x2dc]
003d46bc: add      r5, sp, #0x3c
003d46c0: ldr      r8, [r4, r3]
003d46c4: mov      r0, r8
003d46c8: bl       #0x337888
003d46cc: add      r1, sp, #0x24
003d46d0: mov      r0, r5
003d46d4: bl       #0x3d43ec
003d46d8: mov      r1, r5
003d46dc: mov      r0, r8
003d46e0: bl       #0x337a88
003d46e4: mov      r0, r5
003d46e8: bl       #0x3139ac
003d46ec: mov      r0, r6
003d46f0: bl       #0x3d8ba4
003d46f4: b        #0x3d459c
003d46f8: ldr      r1, [pc, #0x2a0]
003d46fc: mov      r0, r5
003d4700: add      r1, pc, r1
003d4704: bl       #0x30e31c
003d4708: cmp      r0, #0
003d470c: bne      #0x3d459c
003d4710: ldr      r3, [pc, #0x284]
003d4714: add      r5, sp, #0x54
003d4718: ldr      r8, [r4, r3]
003d471c: mov      r0, r8
003d4720: bl       #0x337888
003d4724: add      r1, sp, #0x28
003d4728: mov      r0, r5
003d472c: bl       #0x3d43ec
003d4730: mov      r1, r5
003d4734: mov      r0, r8
003d4738: bl       #0x337a88
003d473c: mov      r0, r5
003d4740: bl       #0x3139ac
003d4744: mov      r0, r6
003d4748: bl       #0x3d8bf8
003d474c: b        #0x3d459c
003d4750: ldr      r3, [r6, #4]
003d4754: add      r1, sp, #0x20
003d4758: mov      r2, r1
003d475c: mov      r0, r3
003d4760: ldr      ip, [r3]
003d4764: add      r3, sp, #0x1c
003d4768: mov      lr, pc
003d476c: ldr      pc, [ip, #0x128]
003d4770: cmp      r0, #0
003d4774: beq      #0x3d488c
003d4778: ldr      r1, [pc, #0x224]
003d477c: mov      r0, r5
003d4780: add      r1, pc, r1
003d4784: bl       #0x30e31c
003d4788: cmp      r0, #0
003d478c: beq      #0x3d47c0
003d4790: ldr      r1, [pc, #0x210]
003d4794: mov      r0, r5
003d4798: add      r1, pc, r1
003d479c: bl       #0x30e31c
003d47a0: cmp      r0, #0
003d47a4: beq      #0x3d47c0
003d47a8: ldr      r1, [pc, #0x1fc]
003d47ac: mov      r0, r5
003d47b0: add      r1, pc, r1
003d47b4: bl       #0x30e31c
003d47b8: cmp      r0, #0
003d47bc: bne      #0x3d459c
003d47c0: ldr      r3, [pc, #0x1d4]
003d47c4: add      r5, sp, #0xb4
003d47c8: ldr      r8, [r4, r3]
003d47cc: mov      r0, r8
003d47d0: bl       #0x337888
003d47d4: add      r1, sp, #0x38
003d47d8: mov      r0, r5
003d47dc: bl       #0x3d43ec
003d47e0: mov      r1, r5
003d47e4: mov      r0, r8
003d47e8: bl       #0x337a88
003d47ec: mov      r0, r5
003d47f0: bl       #0x3139ac
003d47f4: ldr      r3, [pc, #0x1b4]
003d47f8: mov      ip, #0
003d47fc: ldr      r2, [r6, #4]
003d4800: ldr      r0, [r4, r3]
003d4804: ldr      r3, [pc, #0x1a8]
003d4808: ldr      r1, [sp, #0x1c]
003d480c: str      ip, [sp]
003d4810: ldr      lr, [r4, r3]
003d4814: mov      r3, ip
003d4818: str      ip, [sp, #8]
003d481c: str      lr, [sp, #4]
003d4820: str      ip, [sp, #0xc]
003d4824: bl       #0x3e701c
003d4828: b        #0x3d459c
003d482c: ldr      r1, [pc, #0x184]
003d4830: mov      r0, r5
003d4834: add      r1, pc, r1
003d4838: bl       #0x30e31c
003d483c: cmp      r0, #0
003d4840: bne      #0x3d459c
003d4844: ldr      r3, [pc, #0x150]
003d4848: add      r5, sp, #0x6c
003d484c: ldr      r8, [r4, r3]
003d4850: mov      r0, r8
003d4854: bl       #0x337888
003d4858: add      r1, sp, #0x2c
003d485c: mov      r0, r5
003d4860: bl       #0x3d43ec
003d4864: mov      r1, r5
003d4868: mov      r0, r8
003d486c: bl       #0x337a88
003d4870: mov      r0, r5
003d4874: bl       #0x3139ac
003d4878: mov      r0, r6
003d487c: ldr      r3, [r6]
003d4880: mov      lr, pc
003d4884: ldr      pc, [r3, #0xa0]
003d4888: b        #0x3d459c
003d488c: ldr      r1, [pc, #0x128]
003d4890: mov      r0, r5
003d4894: add      r1, pc, r1
003d4898: bl       #0x30e31c
003d489c: subs     sl, r0, #0
003d48a0: beq      #0x3d4910
003d48a4: ldr      r1, [pc, #0x114]
003d48a8: mov      r0, r5
003d48ac: add      r1, pc, r1
003d48b0: bl       #0x30e31c
003d48b4: cmp      r0, #0
003d48b8: bne      #0x3d459c
003d48bc: ldr      r3, [pc, #0xd8]
003d48c0: add      r5, sp, #0x84
003d48c4: ldr      sl, [r4, r3]
003d48c8: mov      r0, sl
003d48cc: bl       #0x337888
003d48d0: add      r1, sp, #0x30
003d48d4: mov      r0, r5
003d48d8: bl       #0x3d43ec
003d48dc: mov      r1, r5
003d48e0: mov      r0, sl
003d48e4: bl       #0x337a88
003d48e8: mov      r0, r5
003d48ec: bl       #0x3139ac
003d48f0: mov      r0, r6
003d48f4: sub      r2, r8, #1
003d48f8: ldr      ip, [r6]
003d48fc: ldr      r1, [r6, #0x74]
003d4900: mov      r3, #1
003d4904: mov      lr, pc
003d4908: ldr      pc, [ip, #0xa8]
003d490c: b        #0x3d459c
003d4910: ldr      r3, [pc, #0x84]
003d4914: add      r5, sp, #0x9c
003d4918: ldr      sb, [r4, r3]
003d491c: mov      r0, sb
003d4920: bl       #0x337888
003d4924: add      r1, sp, #0x34
003d4928: mov      r0, r5
003d492c: bl       #0x3d43ec
003d4930: mov      r1, r5
003d4934: mov      r0, sb
003d4938: bl       #0x337a88
003d493c: mov      r0, r5
003d4940: bl       #0x3139ac
003d4944: mov      r0, r6
003d4948: sub      r2, r8, #1
003d494c: mov      r3, sl
003d4950: ldr      ip, [r6]
003d4954: ldr      r1, [r6, #0x74]
003d4958: mov      lr, pc
003d495c: ldr      pc, [ip, #0xa8]
003d4960: b        #0x3d459c
003d4964: bl       #0x30e310
003d4968: subseq   r0, ip, ip, asr #12
003d496c: andeq    r4, r0, ip, lsr #1
003d4970: strheq   r1, [pc], #-4
003d4974: subeq    r1, pc, r0, lsr #1
003d4978: subeq    r1, pc, ip, lsl #1
003d497c: subeq    r1, pc, r8, ror r0
003d4980: andeq    r3, r0, r8, lsr sp
003d4984: andeq    r3, r0, r8, lsr #19
003d4988: andeq    r0, r0, r4, lsr #27
003d498c: andeq    r0, r0, r4, asr #13
003d4990: muleq    r0, r4, r2
003d4994: andeq    r1, r0, r8, lsl #22
003d4998: subeq    r0, pc, r8, lsl #30
003d499c: andeq    r0, r0, r4, lsl #17
003d49a0: subeq    r0, pc, r0, lsl #29
003d49a4: subeq    r0, pc, r0, ror #27
003d49a8: ldrdeq   r0, r1, [pc], #-0xd8
003d49ac: ldrdeq   r0, r1, [pc], #-0xd0
003d49b0: andeq    r0, r0, ip, asr #16
003d49b4: andeq    r1, r0, r4, lsr r8
003d49b8: subeq    r0, pc, ip, ror #26
003d49bc: ldrdeq   r0, r1, [pc], #-0xcc
003d49c0: subeq    r0, pc, r4, ror #25

# _ZN6CharAI13OnScriptTimerEj
003d0ca0: push     {r4, lr}
003d0ca4: ldr      r3, [r0, #0x1c]
003d0ca8: cmp      r3, #0
003d0cac: beq      #0x3d0cc0
003d0cb0: mov      r0, r3
003d0cb4: ldr      r3, [r3]
003d0cb8: mov      lr, pc
003d0cbc: ldr      pc, [r3, #0x90]
003d0cc0: pop      {r4, pc}

# _ZN6CharAI18_OnAnimSequenceEndEv
003d3d30: push     {r4, lr}
003d3d34: ldr      r0, [r0, #4]
003d3d38: add      r0, r0, #0x4f0
003d3d3c: add      r0, r0, #0xc
003d3d40: bl       #0x3c01ac
003d3d44: mov      r0, #1
003d3d48: pop      {r4, pc}

# _ZN6CharAI14_OnAnimStepEndEv
003d3ff8: push     {r4, lr}
003d3ffc: mov      r4, r0
003d4000: ldr      r0, [r0, #4]
003d4004: add      r0, r0, #0x4f0
003d4008: add      r0, r0, #0xc
003d400c: bl       #0x3c01ac
003d4010: cmp      r0, #5
003d4014: beq      #0x3d4038
003d4018: bge      #0x3d4024
003d401c: mov      r0, #1
003d4020: pop      {r4, pc}
003d4024: cmp      r0, #7
003d4028: bgt      #0x3d401c
003d402c: mov      r0, r4
003d4030: pop      {r4, lr}
003d4034: b        #0x3d3d68
003d4038: mov      r0, r4
003d403c: pop      {r4, lr}
003d4040: b        #0x3d3e44

# _ZN6CharAI16_OnAnimStepBeginEv
003d4204: push     {r4, lr}
003d4208: mov      r4, r0
003d420c: ldr      r0, [r0, #4]
003d4210: add      r0, r0, #0x4f0
003d4214: add      r0, r0, #0xc
003d4218: bl       #0x3c01ac
003d421c: sub      r0, r0, #4
003d4220: cmp      r0, #3
003d4224: addls    pc, pc, r0, lsl #2
003d4228: b        #0x3d4260
003d422c: b        #0x3d4254
003d4230: b        #0x3d4248
003d4234: b        #0x3d423c
003d4238: b        #0x3d423c
003d423c: mov      r0, r4
003d4240: pop      {r4, lr}
003d4244: b        #0x3d3dd4
003d4248: mov      r0, r4
003d424c: pop      {r4, lr}
003d4250: b        #0x3d4044
003d4254: mov      r0, r4
003d4258: pop      {r4, lr}
003d425c: b        #0x3d4120
003d4260: mov      r0, #1
003d4264: pop      {r4, pc}

# _ZN6CharAI10_SkillBlurEv
003d8b7c: ldr      r1, [r0, #0xb8]
003d8b80: ldr      r3, [r0, #0xb4]
003d8b84: ldr      r2, [r0, #0xcc]
003d8b88: rsb      r1, r3, r1
003d8b8c: cmp      r2, r1, asr #2
003d8b90: bxhs     lr
003d8b94: ldr      r0, [r3, r2, lsl #2]
003d8b98: cmp      r0, #0
003d8b9c: bxeq     lr
003d8ba0: b        #0x3da6c0

# _ZN6CharAI11_SkillFocusEv
003d808c: ldr      r1, [r0, #0xb8]
003d8090: ldr      r3, [r0, #0xb4]
003d8094: ldr      r2, [r0, #0xcc]
003d8098: rsb      r1, r3, r1
003d809c: cmp      r2, r1, asr #2
003d80a0: bxhs     lr
003d80a4: ldr      r0, [r3, r2, lsl #2]
003d80a8: cmp      r0, #0
003d80ac: bxeq     lr
003d80b0: b        #0x3da8b8

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

# _ZN6CharAI20_OnAnimSequenceBeginEv
003d3d4c: push     {r4, lr}
003d3d50: ldr      r0, [r0, #4]
003d3d54: add      r0, r0, #0x4f0
003d3d58: add      r0, r0, #0xc
003d3d5c: bl       #0x3c01ac
003d3d60: mov      r0, #1
003d3d64: pop      {r4, pc}

# _ZN10GameObject16EnableCollisionsEv
00394a3c: push     {r4, r5, r6, lr}
00394a40: mov      r6, r0
00394a44: ldr      r0, [r0, #0x2dc]
00394a48: ldr      r4, [pc, #0x70]
00394a4c: sub      sp, sp, #8
00394a50: cmp      r0, #0
00394a54: add      r4, pc, r4
00394a58: beq      #0x394a60
00394a5c: bl       #0x46ebe4
00394a60: ldr      r3, [r6]
00394a64: mov      r0, r6
00394a68: mov      lr, pc
00394a6c: ldr      pc, [r3, #0xb4]
00394a70: cmp      r0, #0
00394a74: beq      #0x394ab8
00394a78: ldr      r3, [r6]
00394a7c: mov      r0, r6
00394a80: mov      lr, pc
00394a84: ldr      pc, [r3, #0xb8]
00394a88: ldr      r3, [r6]
00394a8c: mov      r5, r0
00394a90: mov      r0, r6
00394a94: mov      lr, pc
00394a98: ldr      pc, [r3, #0xbc]
00394a9c: ldr      r3, [pc, #0x20]
00394aa0: str      r0, [sp]
00394aa4: add      r1, r6, #0x1c8
00394aa8: ldr      r0, [r4, r3]
00394aac: mov      r2, #1
00394ab0: mov      r3, r5
00394ab4: bl       #0x528234
00394ab8: add      sp, sp, #8
00394abc: pop      {r4, r5, r6, pc}
00394ac0: rsbeq    r0, r0, ip, lsr r0
00394ac4: andeq    r1, r0, r4, lsl #4

# _ZN6CharAI12_OnEndOfAnimEv
003d3aec: mov      r0, #1
003d3af0: bx       lr

# _ZN14CharProperties10HandleDotsEv
003df3f0: push     {r4, r5, r6, r7, r8, lr}
003df3f4: add      r6, r0, #0xa90
003df3f8: sub      sp, sp, #0x30
003df3fc: mov      r5, r0
003df400: add      r6, r6, #4
003df404: mvn      r4, #0
003df408: add      r7, sp, #8
003df40c: add      r2, r4, #0x7f
003df410: mov      r1, r6
003df414: mov      r0, r5
003df418: bl       #0x3dedb4
003df41c: subs     r8, r0, #0
003df420: ble      #0x3df46c
003df424: ldr      r3, [r5, #4]
003df428: mov      r0, r3
003df42c: ldr      r3, [r3]
003df430: mov      lr, pc
003df434: ldr      pc, [r3, #0x34]
003df438: mov      r3, r8
003df43c: subs     r8, r0, #0
003df440: mov      r0, r7
003df444: bne      #0x3df46c
003df448: ldr      r1, [r5, #4]
003df44c: str      r4, [sp]
003df450: mov      r2, r1
003df454: bl       #0x3b2e68
003df458: ldr      r1, [r5, #4]
003df45c: mov      r3, r8
003df460: mov      r0, r7
003df464: mov      r2, r1
003df468: bl       #0x3b10b4
003df46c: add      r4, r4, #1
003df470: cmp      r4, #5
003df474: bne      #0x3df40c
003df478: add      sp, sp, #0x30
003df47c: pop      {r4, r5, r6, r7, r8, pc}
