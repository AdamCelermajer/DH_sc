
# _ZN10AISDefault7OnTimerEPv
003dbedc: bx       lr

# _ZN6CharAI25_OnAnimStepEnd_SkillSpellEv
003d3d68: push     {r4, r5, r6, lr}
003d3d6c: ldr      r3, [r0, #4]
003d3d70: mov      r4, r0
003d3d74: add      r0, r3, #0x490
003d3d78: add      r0, r0, #0xc
003d3d7c: ldr      r5, [r3, #0x4c8]
003d3d80: bl       #0x3c932c
003d3d84: mov      r6, r0
003d3d88: ldr      r0, [r4, #4]
003d3d8c: add      r0, r0, #0x490
003d3d90: add      r0, r0, #0xc
003d3d94: bl       #0x3c934c
003d3d98: cmp      r6, #0
003d3d9c: cmpeq    r5, #1
003d3da0: bne      #0x3d3db0
003d3da4: ldrb     r3, [r4, #0xd1]
003d3da8: cmp      r3, #0
003d3dac: bne      #0x3d3db8
003d3db0: mov      r0, #1
003d3db4: pop      {r4, r5, r6, pc}
003d3db8: ldr      r0, [r4, #4]
003d3dbc: mov      r1, #1
003d3dc0: add      r0, r0, #0x490
003d3dc4: add      r0, r0, #0xc
003d3dc8: bl       #0x3c948c
003d3dcc: mov      r0, #1
003d3dd0: pop      {r4, r5, r6, pc}

# _ZNK9Character12GetCharSkillEi
003bc784: push     {r4, r5, r6, lr}
003bc788: sub      sp, sp, #8
003bc78c: mov      r5, r1
003bc790: bl       #0x3bc5c0
003bc794: ldr      r4, [pc, #0xa4]
003bc798: ldr      r3, [pc, #0xa4]
003bc79c: mov      r6, #0xc
003bc7a0: add      r4, pc, r4
003bc7a4: ldr      r3, [r4, r3]
003bc7a8: cmp      r5, #0
003bc7ac: ldr      r3, [r3]
003bc7b0: mla      r6, r6, r0, r3
003bc7b4: blt      #0x3bc7c4
003bc7b8: ldr      r3, [r6, #4]
003bc7bc: cmp      r5, r3
003bc7c0: blt      #0x3bc7e8
003bc7c4: ldr      r3, [pc, #0x7c]
003bc7c8: ldr      r3, [r4, r3]
003bc7cc: ldr      r3, [r3]
003bc7d0: cmp      r3, #2
003bc7d4: moveq    r3, #0
003bc7d8: streq    r3, [r3]
003bc7dc: beq      #0x3bc7e8
003bc7e0: cmp      r3, #1
003bc7e4: beq      #0x3bc80c
003bc7e8: ldr      r3, [pc, #0x5c]
003bc7ec: ldr      r2, [r6, #8]
003bc7f0: mov      r0, #0x4c
003bc7f4: ldr      r3, [r4, r3]
003bc7f8: ldr      r2, [r2, r5, lsl #2]
003bc7fc: ldr      r3, [r3]
003bc800: mla      r0, r0, r2, r3
003bc804: add      sp, sp, #8
003bc808: pop      {r4, r5, r6, pc}
003bc80c: ldr      r0, [pc, #0x3c]
003bc810: ldr      r1, [pc, #0x3c]
003bc814: ldr      r2, [pc, #0x3c]
003bc818: ldr      r0, [r4, r0]
003bc81c: ldr      r3, [pc, #0x38]
003bc820: mov      ip, #0x3d
003bc824: add      r1, pc, r1
003bc828: add      r2, pc, r2
003bc82c: add      r3, pc, r3
003bc830: add      r0, r0, #0xa8
003bc834: str      ip, [sp]
003bc838: bl       #0x30e004
003bc83c: b        #0x3bc7e8
003bc840: ldrsheq  r8, [sp], #-0x20
003bc844: andeq    r1, r0, r8, asr #3
003bc848: andeq    r3, r0, r0, asr #19
003bc84c: andeq    r4, r0, ip, lsl r4
003bc850: andeq    r1, r0, r0, asr #19
003bc854: ldrheq   r1, [r0], #-0xb4
003bc858: subseq   r7, r0, r0, lsr #31
003bc85c: ldrsbeq  r7, [r0], #-0xf4

# _ZN6CharAI14AI_CancelSkillEj
003d84e0: push     {r4, r5, lr}
003d84e4: mov      r4, r0
003d84e8: ldr      r2, [r0, #0xb4]
003d84ec: ldr      r0, [r0, #0xb8]
003d84f0: ldr      r3, [pc, #0xc4]
003d84f4: sub      sp, sp, #0xc
003d84f8: rsb      r0, r2, r0
003d84fc: cmp      r1, r0, asr #2
003d8500: mov      r5, r1
003d8504: add      r3, pc, r3
003d8508: blo      #0x3d8530
003d850c: ldr      r1, [pc, #0xac]
003d8510: ldr      r1, [r3, r1]
003d8514: ldr      r1, [r1]
003d8518: cmp      r1, #2
003d851c: moveq    r3, #0
003d8520: streq    r3, [r3]
003d8524: beq      #0x3d8530
003d8528: cmp      r1, #1
003d852c: beq      #0x3d8584
003d8530: ldr      r3, [r2, r5, lsl #2]
003d8534: cmp      r3, #0
003d8538: beq      #0x3d8554
003d853c: ldr      r0, [r4, #4]
003d8540: mov      r1, r5
003d8544: bl       #0x3bc784
003d8548: ldr      r3, [r0, #0x48]
003d854c: cmp      r3, #1
003d8550: beq      #0x3d855c
003d8554: add      sp, sp, #0xc
003d8558: pop      {r4, r5, pc}
003d855c: ldr      r3, [r4, #0xb4]
003d8560: ldr      r0, [r3, r5, lsl #2]
003d8564: bl       #0x3db16c
003d8568: cmp      r0, #0
003d856c: beq      #0x3d8554
003d8570: ldr      r3, [r4, #0xb4]
003d8574: ldr      r0, [r3, r5, lsl #2]
003d8578: add      sp, sp, #0xc
003d857c: pop      {r4, r5, lr}
003d8580: b        #0x3da8b8
003d8584: ldr      r0, [pc, #0x38]
003d8588: ldr      r1, [pc, #0x38]
003d858c: ldr      r2, [pc, #0x38]
003d8590: ldr      r0, [r3, r0]
003d8594: ldr      r3, [pc, #0x34]
003d8598: add      r2, pc, r2
003d859c: movw     ip, #0x122
003d85a0: add      r1, pc, r1
003d85a4: add      r0, r0, #0xa8
003d85a8: add      r3, pc, r3
003d85ac: str      ip, [sp]
003d85b0: bl       #0x30e004
003d85b4: ldr      r2, [r4, #0xb4]
003d85b8: b        #0x3d8530
003d85bc: subseq   ip, fp, ip, lsl #11
003d85c0: andeq    r3, r0, r0, asr #19
003d85c4: andeq    r1, r0, r0, asr #19
003d85c8: subeq    r5, lr, r8, lsr lr
003d85cc: subeq    sp, lr, r8, ror #3
003d85d0: subeq    sp, lr, r0, lsl #3

# _ZN17CharAISkillScript7OnSkillEv
003da794: push     {r4, r5, r6, r7, lr}
003da798: ldr      r4, [pc, #0x108]
003da79c: ldr      r7, [pc, #0x108]
003da7a0: sub      sp, sp, #0x34
003da7a4: add      r4, pc, r4
003da7a8: ldr      r3, [r4, r7]
003da7ac: add      r5, sp, #4
003da7b0: mov      r6, r0
003da7b4: ldr      r3, [r3]
003da7b8: mov      r0, r5
003da7bc: str      r3, [sp, #0x2c]
003da7c0: bl       #0x31b434
003da7c4: ldr      r3, [r6, #4]
003da7c8: ldr      r0, [r3, #0x3e4]
003da7cc: cmp      r0, #0
003da7d0: beq      #0x3da7f4
003da7d4: ldr      r1, [pc, #0xd4]
003da7d8: mov      r3, r5
003da7dc: add      r2, r6, #0xc
003da7e0: add      r1, pc, r1
003da7e4: bl       #0x37c390
003da7e8: ldr      r3, [sp, #0xc]
003da7ec: cmp      r3, #0
003da7f0: beq      #0x3da820
003da7f4: mov      r6, #0
003da7f8: mov      r0, r5
003da7fc: bl       #0x31b398
003da800: ldr      r3, [r4, r7]
003da804: ldr      r2, [sp, #0x2c]
003da808: mov      r0, r6
003da80c: ldr      r3, [r3]
003da810: cmp      r2, r3
003da814: bne      #0x3da8a4
003da818: add      sp, sp, #0x34
003da81c: pop      {r4, r5, r6, r7, pc}
003da820: ldr      r0, [sp, #0x28]
003da824: ldm      r0, {r1, r2}
003da828: cmp      r1, r2
003da82c: beq      #0x3da838
003da830: mov      r3, sp
003da834: bl       #0x31c3cc
003da838: ldr      r3, [r6, #4]
003da83c: ldr      r1, [pc, #0x70]
003da840: mov      r2, r5
003da844: ldr      r0, [r3, #0x3e4]
003da848: add      r1, pc, r1
003da84c: bl       #0x37c494
003da850: ldr      r1, [sp, #0xc]
003da854: cmp      r1, #0
003da858: bne      #0x3da7f4
003da85c: ldr      r2, [sp, #0x28]
003da860: ldr      r3, [r2]
003da864: ldr      r2, [r2, #4]
003da868: rsb      r3, r3, r2
003da86c: asr      r3, r3, #4
003da870: add      r2, r3, r3, lsl #3
003da874: add      r2, r2, r2, lsl #6
003da878: add      r2, r3, r2, lsl #3
003da87c: add      r2, r2, r2, lsl #15
003da880: add      r3, r3, r2, lsl #3
003da884: cmp      r3, #0
003da888: moveq    r6, #1
003da88c: beq      #0x3da7f8
003da890: mov      r0, r5
003da894: bl       #0x3da43c
003da898: bl       #0x31bc80
003da89c: mov      r6, r0
003da8a0: b        #0x3da7f8
003da8a4: bl       #0x30e310
003da8a8: subseq   sl, fp, ip, ror #5
003da8ac: andeq    r4, r0, ip, lsr #1
003da8b0: subeq    fp, lr, r0, ror r0
003da8b4: subeq    fp, lr, r8, lsr #32

# _ZN6CharAI7OnTimerEPv
003d0c10: push     {r4, lr}
003d0c14: ldr      r3, [r0, #0x1c]
003d0c18: cmp      r3, #0
003d0c1c: beq      #0x3d0c30
003d0c20: mov      r0, r3
003d0c24: ldr      r3, [r3]
003d0c28: mov      lr, pc
003d0c2c: ldr      pc, [r3, #0x80]
003d0c30: pop      {r4, pc}

# _ZN6CharAI27_OnAnimStepBegin_SkillSpellEv
003d3dd4: push     {r4, r5, r6, lr}
003d3dd8: ldr      r3, [r0, #4]
003d3ddc: mov      r4, r0
003d3de0: add      r0, r3, #0x490
003d3de4: add      r0, r0, #0xc
003d3de8: ldr      r5, [r3, #0x4c8]
003d3dec: bl       #0x3c932c
003d3df0: mov      r6, r0
003d3df4: ldr      r0, [r4, #4]
003d3df8: add      r0, r0, #0x490
003d3dfc: add      r0, r0, #0xc
003d3e00: bl       #0x3c934c
003d3e04: cmp      r6, #0
003d3e08: cmpeq    r5, #1
003d3e0c: bne      #0x3d3e24
003d3e10: ldrb     r3, [r4, #0xd1]
003d3e14: mov      r1, #1
003d3e18: strb     r1, [r4, #0xd0]
003d3e1c: cmp      r3, #0
003d3e20: bne      #0x3d3e2c
003d3e24: mov      r0, #1
003d3e28: pop      {r4, r5, r6, pc}
003d3e2c: ldr      r0, [r4, #4]
003d3e30: add      r0, r0, #0x490
003d3e34: add      r0, r0, #0xc
003d3e38: bl       #0x3c948c
003d3e3c: mov      r0, #1
003d3e40: pop      {r4, r5, r6, pc}

# _ZN16CharStateMachine16SM_SetSkillStateEjbPvb
003c6670: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6674: mov      r4, r0
003c6678: ldr      r0, [r0, #4]
003c667c: mov      sb, r2
003c6680: mov      r6, r3
003c6684: mov      r7, r1
003c6688: ldrb     r8, [sp, #0x20]
003c668c: bl       #0x3bc784
003c6690: ldr      r5, [pc, #0x7c]
003c6694: ldr      r3, [pc, #0x7c]
003c6698: ldr      r1, [pc, #0x7c]
003c669c: add      r5, pc, r5
003c66a0: ldr      r3, [r5, r3]
003c66a4: ldr      r2, [pc, #0x74]
003c66a8: ldr      sl, [r0, #4]
003c66ac: add      r1, pc, r1
003c66b0: ldr      r0, [r3, #0x2c]
003c66b4: add      r2, pc, r2
003c66b8: bl       #0x4c4bdc
003c66bc: ands     r0, r0, #0x200000
003c66c0: bne      #0x3c6708
003c66c4: add      sl, r0, sl
003c66c8: cmp      r8, #0
003c66cc: str      sl, [r4, #0x28]
003c66d0: str      r7, [r4, #0x54]
003c66d4: strb     sb, [r4, #0x58]
003c66d8: bne      #0x3c66f0
003c66dc: mov      r0, r4
003c66e0: mov      r2, r6
003c66e4: movw     r1, #0xc355
003c66e8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c66ec: b        #0x3c5684
003c66f0: mov      r0, r4
003c66f4: mov      r3, r6
003c66f8: mov      r1, #6
003c66fc: movw     r2, #0xc355
003c6700: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c6704: b        #0x3c1938
003c6708: ldr      r0, [r4, #4]
003c670c: bl       #0x3a53e0
003c6710: b        #0x3c66c4
003c6714: ldrsheq  lr, [ip], #-0x34
003c6718: strdeq   r3, r4, [r0], -r4
003c671c: subeq    lr, pc, ip, lsl #10
003c6720: subeq    lr, pc, r4, lsl r5

# _ZNK14CharProperties18PROPS_GetFromSheetEiPN7Structs19CharacterPropertiesE
003b55d4: str      lr, [sp, #-4]!
003b55d8: ldr      r3, [pc, #0x80]
003b55dc: subs     ip, r2, #0
003b55e0: sub      sp, sp, #0xc
003b55e4: mov      r2, r1
003b55e8: add      r3, pc, r3
003b55ec: beq      #0x3b5600
003b55f0: mov      r1, ip
003b55f4: add      sp, sp, #0xc
003b55f8: pop      {lr}
003b55fc: b        #0x3dedb4
003b5600: ldr      r2, [pc, #0x5c]
003b5604: ldr      r2, [r3, r2]
003b5608: ldr      r2, [r2]
003b560c: cmp      r2, #2
003b5610: streq    ip, [ip]
003b5614: beq      #0x3b5620
003b5618: cmp      r2, #1
003b561c: beq      #0x3b562c
003b5620: mvn      r0, #0
003b5624: add      sp, sp, #0xc
003b5628: ldm      sp!, {pc}
003b562c: ldr      r0, [pc, #0x34]
003b5630: ldr      r1, [pc, #0x34]
003b5634: ldr      r2, [pc, #0x34]
003b5638: ldr      r0, [r3, r0]
003b563c: ldr      r3, [pc, #0x30]
003b5640: movw     ip, #0x137
003b5644: add      r1, pc, r1
003b5648: add      r2, pc, r2
003b564c: add      r3, pc, r3
003b5650: add      r0, r0, #0xa8
003b5654: str      ip, [sp]
003b5658: bl       #0x30e004
003b565c: b        #0x3b5620
003b5660: subseq   pc, sp, r8, lsr #9
003b5664: andeq    r3, r0, r0, asr #19
003b5668: andeq    r1, r0, r0, asr #19

# _ZN9Character24_SetSkillCooldownTimerIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b97e0: push     {r4, r5, r6, r7, r8, lr}
003b97e4: ldr      r5, [r0, #4]
003b97e8: mov      r6, r2
003b97ec: mov      r4, r0
003b97f0: ldm      r5, {r2, r3}
003b97f4: rsb      r3, r2, r3
003b97f8: asr      r3, r3, #4
003b97fc: add      r1, r3, r3, lsl #3
003b9800: add      r1, r1, r1, lsl #6
003b9804: add      r1, r3, r1, lsl #3
003b9808: add      r1, r1, r1, lsl #15
003b980c: add      r3, r3, r1, lsl #3
003b9810: rsb      r3, r3, #0
003b9814: cmp      r3, #1
003b9818: bls      #0x3b9864
003b981c: cmp      r3, #0
003b9820: movne    r0, r2
003b9824: beq      #0x3b9938
003b9828: ldr      r2, [r2, #4]
003b982c: cmp      r2, #3
003b9830: beq      #0x3b9890
003b9834: cmp      r3, #0
003b9838: beq      #0x3b9980
003b983c: bl       #0x31bbf0
003b9840: mov      r7, r0
003b9844: mov      r0, r6
003b9848: bl       #0x3bc5fc
003b984c: mov      r5, r0
003b9850: mov      r0, r7
003b9854: bl       #0x8be2a0
003b9858: ldr      r3, [r5, #4]
003b985c: cmp      r3, r0
003b9860: bhi      #0x3b9868
003b9864: pop      {r4, r5, r6, r7, r8, pc}
003b9868: ldr      r5, [r4, #4]
003b986c: ldm      r5, {r0, r2}
003b9870: rsb      r2, r0, r2
003b9874: asr      r2, r2, #4
003b9878: add      r3, r2, r2, lsl #3
003b987c: add      r3, r3, r3, lsl #6
003b9880: add      r3, r2, r3, lsl #3
003b9884: add      r3, r3, r3, lsl #15
003b9888: add      r3, r2, r3, lsl #3
003b988c: rsb      r3, r3, #0
003b9890: cmp      r3, #1
003b9894: bls      #0x3b9994
003b9898: ldr      r3, [r0, #0x74]
003b989c: cmp      r3, #3
003b98a0: beq      #0x3b98f0
003b98a4: ldr      r5, [r4, #4]
003b98a8: ldm      r5, {r2, r3}
003b98ac: rsb      r3, r2, r3
003b98b0: asr      r3, r3, #4
003b98b4: add      r1, r3, r3, lsl #3
003b98b8: add      r1, r1, r1, lsl #6
003b98bc: add      r1, r3, r1, lsl #3
003b98c0: add      r1, r1, r1, lsl #15
003b98c4: add      r3, r3, r1, lsl #3
003b98c8: rsb      r3, r3, #0
003b98cc: cmp      r3, #1
003b98d0: bhi      #0x3b98e4
003b98d4: ldr      r0, [pc, #0xe4]
003b98d8: add      r0, pc, r0
003b98dc: bl       #0x708eb0
003b98e0: ldr      r2, [r5]
003b98e4: ldr      r3, [r2, #0x74]
003b98e8: cmp      r3, #0
003b98ec: bne      #0x3b9864
003b98f0: mov      r1, #0
003b98f4: mov      r0, r4
003b98f8: bl       #0x37baf8
003b98fc: bl       #0x31bbf0
003b9900: bl       #0x30e4cc
003b9904: ldr      r3, [r6, #0x47c]
003b9908: ldr      r5, [r3, r0, lsl #2]
003b990c: cmp      r5, #0
003b9910: beq      #0x3b9864
003b9914: mov      r0, r4
003b9918: mov      r1, #1
003b991c: bl       #0x37baf8
003b9920: ldr      r3, [r0, #4]
003b9924: cmp      r3, #0
003b9928: bne      #0x3b99a8
003b992c: mvn      r3, #0
003b9930: str      r3, [r5, #0x18]
003b9934: pop      {r4, r5, r6, r7, r8, pc}
003b9938: ldr      r0, [pc, #0x84]
003b993c: add      r0, pc, r0
003b9940: bl       #0x708eb0
003b9944: ldr      r2, [r5]
003b9948: ldr      r5, [r4, #4]
003b994c: ldr      r2, [r2, #4]
003b9950: ldm      r5, {r0, r1}
003b9954: cmp      r2, #3
003b9958: rsb      r1, r0, r1
003b995c: asr      r1, r1, #4
003b9960: add      r3, r1, r1, lsl #3
003b9964: add      r3, r3, r3, lsl #6
003b9968: add      r3, r1, r3, lsl #3
003b996c: add      r3, r3, r3, lsl #15
003b9970: add      r3, r1, r3, lsl #3
003b9974: rsb      r3, r3, #0
003b9978: bne      #0x3b9834
003b997c: b        #0x3b9890
003b9980: ldr      r0, [pc, #0x40]
003b9984: add      r0, pc, r0
003b9988: bl       #0x708eb0
003b998c: ldr      r0, [r5]
003b9990: b        #0x3b983c
003b9994: ldr      r0, [pc, #0x30]
003b9998: add      r0, pc, r0
003b999c: bl       #0x708eb0
003b99a0: ldr      r0, [r5]
003b99a4: b        #0x3b9898
003b99a8: mov      r1, #1
003b99ac: mov      r0, r4
003b99b0: bl       #0x37baf8
003b99b4: bl       #0x38d798
003b99b8: str      r0, [r5, #0x18]
003b99bc: pop      {r4, r5, r6, r7, r8, pc}

# _ZN9Character24_SetSpellCooldownTimerIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b90e4: push     {r4, lr}
003b90e8: ldr      r3, [r0, #4]
003b90ec: sub      sp, sp, #8
003b90f0: ldr      r1, [r3, #4]
003b90f4: ldr      ip, [r3]
003b90f8: rsb      r3, ip, r1
003b90fc: asr      r3, r3, #4
003b9100: add      r1, r3, r3, lsl #3
003b9104: add      r1, r1, r1, lsl #6
003b9108: add      r1, r3, r1, lsl #3
003b910c: add      r1, r1, r1, lsl #15
003b9110: add      r3, r3, r1, lsl #3
003b9114: cmp      r3, #0
003b9118: bne      #0x3b9124
003b911c: add      sp, sp, #8
003b9120: pop      {r4, pc}
003b9124: ldr      r3, [ip, #4]
003b9128: cmp      r3, #3
003b912c: beq      #0x3b917c
003b9130: cmp      r3, #0
003b9134: bne      #0x3b911c
003b9138: mvn      r4, #0
003b913c: mov      r0, r2
003b9140: str      r2, [sp, #4]
003b9144: bl       #0x3ae5dc
003b9148: ldr      r0, [r0, #4]
003b914c: ldr      r2, [sp, #4]
003b9150: cmp      r0, #0
003b9154: beq      #0x3b911c
003b9158: mov      r3, #0
003b915c: ldr      r1, [r2, #0x488]
003b9160: ldr      r1, [r1, r3, lsl #2]
003b9164: add      r3, r3, #1
003b9168: cmp      r1, #0
003b916c: strne    r4, [r1, #0x18]
003b9170: cmp      r3, r0
003b9174: bne      #0x3b915c
003b9178: b        #0x3b911c
003b917c: cmp      r3, #0
003b9180: beq      #0x3b9138
003b9184: mov      r1, #0
003b9188: str      r2, [sp, #4]
003b918c: bl       #0x37baf8
003b9190: bl       #0x38d798
003b9194: ldr      r2, [sp, #4]
003b9198: mov      r4, r0
003b919c: b        #0x3b913c
