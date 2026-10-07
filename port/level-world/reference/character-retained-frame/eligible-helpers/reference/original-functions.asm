
# _ZNK9Character10IsMerchantEv
003a30c4: push     {r4, lr}
003a30c8: bl       #0x3a3054
003a30cc: cmp      r0, #7
003a30d0: movne    r0, #0
003a30d4: moveq    r0, #1
003a30d8: pop      {r4, pc}

# _ZNK13ScriptManager17IsCutSceneRunningEv
00455c54: push     {r4, r5, r6, lr}
00455c58: ldr      r6, [r0, #0x1c]
00455c5c: ldr      r3, [r0, #0x18]
00455c60: mov      r5, r0
00455c64: rsb      r3, r3, r6
00455c68: asr      r3, r3, #2
00455c6c: add      r6, r3, r3, lsl #2
00455c70: add      r6, r6, r6, lsl #4
00455c74: add      r6, r6, r6, lsl #8
00455c78: add      r6, r6, r6, lsl #16
00455c7c: add      r6, r3, r6, lsl #1
00455c80: cmp      r6, #0
00455c84: ble      #0x455cb8
00455c88: mov      r4, #0
00455c8c: b        #0x455c98
00455c90: cmp      r4, r6
00455c94: beq      #0x455cb8
00455c98: mov      r1, r4
00455c9c: mov      r0, r5
00455ca0: bl       #0x455bec
00455ca4: cmp      r0, #0
00455ca8: add      r4, r4, #1
00455cac: beq      #0x455c90
00455cb0: mov      r0, #1
00455cb4: pop      {r4, r5, r6, pc}
00455cb8: mov      r0, #0
00455cbc: pop      {r4, r5, r6, pc}

# _ZNK9Character10CanRespawnEv
003a5248: push     {r4, lr}
003a524c: movw     r3, #0x1481
003a5250: ldrb     r3, [r0, r3]
003a5254: mov      r4, r0
003a5258: cmp      r3, #0
003a525c: beq      #0x3a5268
003a5260: mov      r0, #0
003a5264: pop      {r4, pc}
003a5268: add      r1, r0, #0xff0
003a526c: add      r1, r1, #4
003a5270: add      r0, r0, #0x560
003a5274: mov      r2, #0xb
003a5278: bl       #0x3dedb4
003a527c: cmp      r0, #0
003a5280: ble      #0x3a5260
003a5284: ldr      r0, [r4, #0x3fc]
003a5288: cmp      r0, #0
003a528c: beq      #0x3a529c
003a5290: ldr      r1, [r4, #0x3cc]
003a5294: pop      {r4, lr}
003a5298: b        #0x3d2a34
003a529c: mov      r0, #1
003a52a0: pop      {r4, pc}

# _ZN10ObjectBase23TestCullingBeforeUpdateERK4aabbIfE
0033de90: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0033de94: sub      sp, sp, #0x24
0033de98: mov      r5, r0
0033de9c: mov      r6, r1
0033dea0: bl       #0x7fd794
0033dea4: ldrb     r3, [r0, #5]
0033dea8: ldr      r4, [pc, #0x188]
0033deac: cmp      r3, #0
0033deb0: add      r4, pc, r4
0033deb4: bne      #0x33e004
0033deb8: ldrb     r3, [r5, #0x86]
0033debc: cmp      r3, #0
0033dec0: moveq    r0, #1
0033dec4: strbeq   r0, [r5, #0x86]
0033dec8: beq      #0x33ded8
0033decc: cmp      r3, #1
0033ded0: beq      #0x33dee0
0033ded4: mov      r0, #1
0033ded8: add      sp, sp, #0x24
0033dedc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0033dee0: ldr      r3, [r6, #0x14]
0033dee4: str      r3, [sp, #4]
0033dee8: ldr      r3, [r6]
0033deec: str      r3, [sp, #0x18]
0033def0: ldr      r3, [pc, #0x144]
0033def4: ldr      r0, [r4, r3]
0033def8: ldr      r3, [r6, #4]
0033defc: str      r3, [sp, #0x14]
0033df00: ldr      r3, [r6, #8]
0033df04: str      r3, [sp, #0x10]
0033df08: ldr      r3, [r6, #0xc]
0033df0c: str      r3, [sp, #0xc]
0033df10: ldr      r6, [r6, #0x10]
0033df14: str      r6, [sp, #8]
0033df18: bl       #0x31f594
0033df1c: cmp      r0, #0
0033df20: beq      #0x33e02c
0033df24: ldr      r3, [r0, #0x128]
0033df28: mov      r6, #0
0033df2c: mov      fp, r6
0033df30: ldr      r3, [r3, #8]
0033df34: mov      r0, r3
0033df38: ldr      r3, [r3]
0033df3c: mov      lr, pc
0033df40: ldr      pc, [r3, #0x144]
0033df44: str      r5, [sp, #0x1c]
0033df48: mov      r4, r0
0033df4c: ldr      r7, [r4, #0xc]
0033df50: mov      r1, #0
0033df54: mov      r0, r7
0033df58: bl       #0x30e4b4
0033df5c: ldr      r6, [r4, #0x10]
0033df60: cmp      r0, #0
0033df64: mov      r1, #0
0033df68: mov      r0, r6
0033df6c: ldreq    sb, [sp, #0xc]
0033df70: ldrne    sb, [sp, #0x18]
0033df74: bl       #0x30e4b4
0033df78: ldr      r5, [r4, #0x14]
0033df7c: cmp      r0, #0
0033df80: mov      r1, #0
0033df84: mov      r0, r5
0033df88: ldreq    sl, [sp, #8]
0033df8c: ldrne    sl, [sp, #0x14]
0033df90: bl       #0x30e4b4
0033df94: mov      r1, sb
0033df98: cmp      r0, #0
0033df9c: mov      r0, r7
0033dfa0: ldreq    r8, [sp, #4]
0033dfa4: ldrne    r8, [sp, #0x10]
0033dfa8: bl       #0x30ed6c
0033dfac: mov      r1, sl
0033dfb0: mov      r7, r0
0033dfb4: mov      r0, r6
0033dfb8: bl       #0x30ed6c
0033dfbc: mov      r1, r0
0033dfc0: mov      r0, r7
0033dfc4: bl       #0x30eba4
0033dfc8: mov      r1, r8
0033dfcc: mov      r6, r0
0033dfd0: mov      r0, r5
0033dfd4: bl       #0x30ed6c
0033dfd8: mov      r1, r0
0033dfdc: mov      r0, r6
0033dfe0: bl       #0x30eba4
0033dfe4: ldr      r1, [r4, #0x18]
0033dfe8: bl       #0x30eba4
0033dfec: mov      r1, #0
0033dff0: bl       #0x30e2f8
0033dff4: cmp      r0, #0
0033dff8: beq      #0x33e018
0033dffc: mov      r0, #0
0033e000: b        #0x33ded8
0033e004: ldr      r3, [r5]
0033e008: mov      r0, r5
0033e00c: mov      lr, pc
0033e010: ldr      pc, [r3, #0x54]
0033e014: b        #0x33deb8
0033e018: add      fp, fp, #1
0033e01c: cmp      fp, #6
0033e020: add      r4, r4, #0x10
0033e024: bne      #0x33df4c
0033e028: ldr      r5, [sp, #0x1c]
0033e02c: mov      r3, #2
0033e030: strb     r3, [r5, #0x86]
0033e034: b        #0x33ded4
0033e038: rsbeq    r6, r5, r0, ror #23
0033e03c: strdeq   r3, r4, [r0], -r4

# _ZN9Character13UpdateStateFXEv
003a4470: push     {r4, r5, r6, r7, r8, lr}
003a4474: movw     r2, #0x1490
003a4478: ldr      r3, [r0]
003a447c: mov      r4, r0
003a4480: ldr      r6, [r0, r2]
003a4484: mov      lr, pc
003a4488: ldr      pc, [r3, #0x34]
003a448c: ldr      r5, [pc, #0x1f8]
003a4490: cmp      r0, #0
003a4494: add      r5, pc, r5
003a4498: bne      #0x3a45a8
003a449c: cmp      r6, #4
003a44a0: ble      #0x3a45ac
003a44a4: mvn      r7, #0
003a44a8: mov      r6, #0
003a44ac: movw     r3, #0x1490
003a44b0: ldr      r2, [r4, r3]
003a44b4: cmp      r2, r6
003a44b8: beq      #0x3a45a8
003a44bc: cmn      r7, #1
003a44c0: str      r6, [r4, r3]
003a44c4: beq      #0x3a4674
003a44c8: movw     r3, #0x148c
003a44cc: ldr      r3, [r4, r3]
003a44d0: cmp      r3, #0
003a44d4: beq      #0x3a4654
003a44d8: ldr      r6, [pc, #0x1b0]
003a44dc: add      r1, r4, #0x1480
003a44e0: add      r1, r1, #0xc
003a44e4: ldr      r0, [r5, r6]
003a44e8: bl       #0x494978
003a44ec: ldr      r0, [r5, r6]
003a44f0: mov      r1, r7
003a44f4: mov      r2, #0
003a44f8: bl       #0x495430
003a44fc: movw     r6, #0x148c
003a4500: cmp      r0, #0
003a4504: str      r0, [r4, r6]
003a4508: beq      #0x3a45a8
003a450c: str      r4, [r0, #0x28]
003a4510: mov      r1, #1
003a4514: bl       #0x492aa0
003a4518: ldr      r2, [pc, #0x174]
003a451c: ldr      r3, [r4, r6]
003a4520: mov      r1, #0
003a4524: ldr      r2, [r5, r2]
003a4528: mov      r0, r3
003a452c: ldr      lr, [r2]
003a4530: ldr      ip, [r2, #4]
003a4534: ldr      r2, [r2, #8]
003a4538: str      lr, [r3, #0x34]
003a453c: str      ip, [r3, #0x38]
003a4540: str      r2, [r3, #0x3c]
003a4544: bl       #0x492aa0
003a4548: ldr      r0, [r4, r6]
003a454c: mov      r1, #1
003a4550: bl       #0x492ef0
003a4554: ldr      r0, [r4, r6]
003a4558: bl       #0x49267c
003a455c: cmp      r0, #0
003a4560: beq      #0x3a45a8
003a4564: ldr      r0, [r4, r6]
003a4568: bl       #0x49267c
003a456c: ldr      r3, [r0]
003a4570: mov      lr, pc
003a4574: ldr      pc, [r3, #0x44]
003a4578: cmp      r0, #0
003a457c: beq      #0x3a45a8
003a4580: ldr      r0, [r4, r6]
003a4584: bl       #0x49267c
003a4588: ldr      r3, [r0]
003a458c: mov      lr, pc
003a4590: ldr      pc, [r3, #0x44]
003a4594: mov      r1, #1
003a4598: ldr      r3, [r0]
003a459c: mov      lr, pc
003a45a0: ldr      pc, [r3, #0x40]
003a45a4: pop      {r4, r5, r6, r7, r8, pc}
003a45a8: pop      {r4, r5, r6, r7, r8, pc}
003a45ac: add      r7, r4, #0x4f0
003a45b0: add      r7, r7, #0xc
003a45b4: mov      r0, r7
003a45b8: mov      r1, #1
003a45bc: bl       #0x3c0378
003a45c0: cmp      r0, #0
003a45c4: beq      #0x3a45d4
003a45c8: ldr      r3, [r4, #0x520]
003a45cc: tst      r3, #0x800
003a45d0: bne      #0x3a465c
003a45d4: cmp      r6, #3
003a45d8: bgt      #0x3a44a4
003a45dc: mov      r0, r7
003a45e0: mov      r1, #1
003a45e4: bl       #0x3c034c
003a45e8: cmp      r0, #0
003a45ec: beq      #0x3a461c
003a45f0: ldr      r3, [r4, #0x520]
003a45f4: tst      r3, #0x400
003a45f8: beq      #0x3a461c
003a45fc: ldr      r3, [pc, #0x94]
003a4600: mov      r6, #3
003a4604: ldr      r3, [r5, r3]
003a4608: ldr      r3, [r3]
003a460c: ldr      r7, [r3, #0x74]
003a4610: cmn      r7, #1
003a4614: bne      #0x3a44ac
003a4618: b        #0x3a44a8
003a461c: cmp      r6, #1
003a4620: bgt      #0x3a44a4
003a4624: add      r0, r4, #0x560
003a4628: bl       #0x3de6c4
003a462c: mov      r1, #0x3f800000
003a4630: bl       #0x30e70c
003a4634: cmp      r0, #0
003a4638: beq      #0x3a44a4
003a463c: ldr      r3, [pc, #0x54]
003a4640: mov      r6, #1
003a4644: ldr      r3, [r5, r3]
003a4648: ldr      r3, [r3]
003a464c: ldr      r7, [r3, #0x7c]
003a4650: b        #0x3a4610
003a4654: ldr      r6, [pc, #0x34]
003a4658: b        #0x3a44ec
003a465c: ldr      r3, [pc, #0x34]
003a4660: mov      r6, #4
003a4664: ldr      r3, [r5, r3]
003a4668: ldr      r3, [r3]
003a466c: ldr      r7, [r3, #0x80]
003a4670: b        #0x3a4610
003a4674: ldr      r3, [pc, #0x14]
003a4678: add      r1, r4, #0x1480
003a467c: add      r1, r1, #0xc
003a4680: ldr      r0, [r5, r3]
003a4684: pop      {r4, r5, r6, r7, r8, lr}
003a4688: b        #0x494978
003a468c: ldrsheq  r0, [pc], #-0x5c
003a4690: andeq    r1, r0, r8, lsl #22
003a4694: andeq    r3, r0, ip, lsr #30
003a4698: andeq    r3, r0, r8, asr #5

# _ZN12CharAIScript19CallStateConditionsEv
003d8ea0: ldr      r3, [r0, #0xb4]
003d8ea4: cmp      r3, #0
003d8ea8: bxeq     lr
003d8eac: ldr      r1, [r3, #0x2c]
003d8eb0: b        #0x37c514

# _Z17IsMultiplayerGamev
003a42f4: push     {r4, lr}
003a42f8: bl       #0x7fd794
003a42fc: ldrb     r2, [r0, #5]
003a4300: ldr      r3, [pc, #0x34]
003a4304: cmp      r2, #0
003a4308: add      r3, pc, r3
003a430c: beq      #0x3a4318
003a4310: mov      r0, #1
003a4314: pop      {r4, pc}
003a4318: ldr      r2, [pc, #0x20]
003a431c: mov      r1, #1
003a4320: ldr      r3, [r3, r2]
003a4324: ldr      r0, [r3, #0x40]
003a4328: bl       #0x36ead0
003a432c: cmp      r0, #1
003a4330: movle    r0, #0
003a4334: movgt    r0, #1
003a4338: pop      {r4, pc}
003a433c: subseq   r0, pc, r8, lsl #15
003a4340: strdeq   r3, r4, [r0], -r4

# _ZN12CharAIScript15CallStateUpdateEv
003d8eb4: ldr      r3, [r0, #0xb4]
003d8eb8: cmp      r3, #0
003d8ebc: bxeq     lr
003d8ec0: ldr      r1, [r3, #0x14]
003d8ec4: b        #0x37c514

# _ZN10GameObject20UpdateTargetPositionEv
00393d74: push     {r4, lr}
00393d78: ldr      r1, [r0, #0x180]
00393d7c: sub      sp, sp, #0x10
00393d80: mov      r4, r0
00393d84: cmp      r1, #0
00393d88: beq      #0x393dac
00393d8c: add      r0, sp, #4
00393d90: bl       #0x597180
00393d94: ldr      r2, [sp, #8]
00393d98: ldr      r3, [sp, #0xc]
00393d9c: ldr      r1, [sp, #4]
00393da0: str      r2, [r4, #0x188]
00393da4: str      r3, [r4, #0x18c]
00393da8: str      r1, [r4, #0x184]
00393dac: add      sp, sp, #0x10
00393db0: pop      {r4, pc}
