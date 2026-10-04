
# _ZNK9Character16GetCharSkillListEv
003bc5fc: ldr      r3, [pc, #0x20]
003bc600: ldr      r2, [pc, #0x20]
003bc604: push     {r4, lr}
003bc608: add      r3, pc, r3
003bc60c: ldr      r2, [r3, r2]
003bc610: ldr      r4, [r2]
003bc614: bl       #0x3bc5c0
003bc618: mov      r3, #0xc
003bc61c: mla      r0, r3, r0, r4
003bc620: pop      {r4, pc}
003bc624: subseq   r8, sp, r8, lsl #9
003bc628: andeq    r1, r0, r8, asr #3

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

# _ZN9Character7RegenMPEi
003bdbb8: push     {r4, r5, r6, r7, r8, sl, lr}
003bdbbc: ldr      r5, [pc, #0xd0]
003bdbc0: ldr      r8, [pc, #0xd0]
003bdbc4: add      r6, r0, #0xff0
003bdbc8: add      r5, pc, r5
003bdbcc: ldr      r3, [r5, r8]
003bdbd0: add      r6, r6, #4
003bdbd4: add      r7, r0, #0x560
003bdbd8: ldr      r3, [r3]
003bdbdc: mov      r4, r1
003bdbe0: sub      sp, sp, #0x24
003bdbe4: mov      r2, #0x29
003bdbe8: mov      r1, r6
003bdbec: mov      r0, r7
003bdbf0: str      r3, [sp, #0x1c]
003bdbf4: bl       #0x3dedb4
003bdbf8: mov      sl, r0
003bdbfc: mov      r1, r6
003bdc00: mov      r0, r7
003bdc04: mov      r2, #0x2b
003bdc08: bl       #0x3dedb4
003bdc0c: cmp      r4, #0
003bdc10: movlt    r4, r0
003bdc14: add      r3, r4, sl
003bdc18: cmp      r3, r0
003bdc1c: rsbgt    r4, sl, r0
003bdc20: cmp      r4, #0
003bdc24: ble      #0x3bdc74
003bdc28: ldr      r3, [pc, #0x6c]
003bdc2c: add      r6, sp, #4
003bdc30: ldr      sl, [r5, r3]
003bdc34: mov      r0, sl
003bdc38: bl       #0x337888
003bdc3c: ldr      r1, [pc, #0x5c]
003bdc40: mov      r2, sp
003bdc44: mov      r0, r6
003bdc48: add      r1, pc, r1
003bdc4c: bl       #0x3140ec
003bdc50: mov      r1, r6
003bdc54: mov      r0, sl
003bdc58: bl       #0x337a88
003bdc5c: mov      r0, r6
003bdc60: bl       #0x318254
003bdc64: mov      r0, r7
003bdc68: mov      r2, r4
003bdc6c: mov      r1, #0x29
003bdc70: bl       #0x3e0708
003bdc74: ldr      r3, [r5, r8]
003bdc78: ldr      r2, [sp, #0x1c]
003bdc7c: ldr      r3, [r3]
003bdc80: cmp      r2, r3
003bdc84: bne      #0x3bdc90
003bdc88: add      sp, sp, #0x24
003bdc8c: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdc90: bl       #0x30e310
003bdc94: subseq   r6, sp, r8, asr #29
003bdc98: andeq    r4, r0, ip, lsr #1
003bdc9c: andeq    r0, r0, r4, lsl #17
003bdca0: subseq   r6, r0, r8, asr #24

# _ZNK9Character12GetCharFaeryEi
003aeac0: push     {r4, r5, r6, r7, r8, lr}
003aeac4: sub      sp, sp, #8
003aeac8: mov      r5, r1
003aeacc: bl       #0x3ae5a0
003aead0: ldr      r4, [pc, #0x1b8]
003aead4: ldr      r3, [pc, #0x1b8]
003aead8: mov      r6, #0xc
003aeadc: add      r4, pc, r4
003aeae0: ldr      r3, [r4, r3]
003aeae4: cmp      r5, #0
003aeae8: ldr      r3, [r3]
003aeaec: mla      r6, r6, r0, r3
003aeaf0: blt      #0x3aebbc
003aeaf4: ldr      r7, [pc, #0x19c]
003aeaf8: ldr      r1, [pc, #0x19c]
003aeafc: ldr      r2, [pc, #0x19c]
003aeb00: ldr      r3, [r4, r7]
003aeb04: add      r1, pc, r1
003aeb08: add      r2, pc, r2
003aeb0c: ldr      r0, [r3, #0x2c]
003aeb10: bl       #0x4c4bdc
003aeb14: cmp      r5, r0
003aeb18: bge      #0x3aebc0
003aeb1c: ldr      r3, [r4, r7]
003aeb20: ldr      r1, [pc, #0x17c]
003aeb24: ldr      r2, [pc, #0x17c]
003aeb28: ldr      r0, [r3, #0x2c]
003aeb2c: add      r1, pc, r1
003aeb30: add      r2, pc, r2
003aeb34: ldr      r7, [r6, #4]
003aeb38: bl       #0x4c4bdc
003aeb3c: cmp      r7, r0
003aeb40: beq      #0x3aeb68
003aeb44: ldr      r3, [pc, #0x160]
003aeb48: ldr      r3, [r4, r3]
003aeb4c: ldr      r3, [r3]
003aeb50: cmp      r3, #2
003aeb54: moveq    r3, #0
003aeb58: streq    r3, [r3]
003aeb5c: beq      #0x3aeb68
003aeb60: cmp      r3, #1
003aeb64: beq      #0x3aec18
003aeb68: ldr      r2, [pc, #0x140]
003aeb6c: ldr      r3, [r6, #8]
003aeb70: mov      r8, #0x24
003aeb74: ldr      r7, [r4, r2]
003aeb78: ldr      r0, [r3, r5, lsl #2]
003aeb7c: ldr      r3, [r7]
003aeb80: mla      r0, r8, r0, r3
003aeb84: ldr      r3, [r0, #0x20]
003aeb88: cmp      r3, r5
003aeb8c: beq      #0x3aebb4
003aeb90: ldr      r3, [pc, #0x114]
003aeb94: ldr      r3, [r4, r3]
003aeb98: ldr      r3, [r3]
003aeb9c: cmp      r3, #2
003aeba0: moveq    r3, #0
003aeba4: streq    r3, [r3]
003aeba8: beq      #0x3aebb4
003aebac: cmp      r3, #1
003aebb0: beq      #0x3aec4c
003aebb4: add      sp, sp, #8
003aebb8: pop      {r4, r5, r6, r7, r8, pc}
003aebbc: ldr      r7, [pc, #0xd4]
003aebc0: ldr      r3, [pc, #0xe4]
003aebc4: ldr      r3, [r4, r3]
003aebc8: ldr      r3, [r3]
003aebcc: cmp      r3, #2
003aebd0: moveq    r3, #0
003aebd4: streq    r3, [r3]
003aebd8: beq      #0x3aeb1c
003aebdc: cmp      r3, #1
003aebe0: bne      #0x3aeb1c
003aebe4: ldr      r0, [pc, #0xc8]
003aebe8: ldr      r1, [pc, #0xc8]
003aebec: ldr      r2, [pc, #0xc8]
003aebf0: ldr      r0, [r4, r0]
003aebf4: ldr      r3, [pc, #0xc4]
003aebf8: mov      ip, #0x3e
003aebfc: add      r1, pc, r1
003aec00: add      r2, pc, r2
003aec04: add      r3, pc, r3
003aec08: add      r0, r0, #0xa8
003aec0c: str      ip, [sp]
003aec10: bl       #0x30e004
003aec14: b        #0x3aeb1c
003aec18: ldr      r0, [pc, #0x94]
003aec1c: ldr      r1, [pc, #0xa0]
003aec20: ldr      r2, [pc, #0xa0]
003aec24: ldr      r0, [r4, r0]
003aec28: ldr      r3, [pc, #0x9c]
003aec2c: mov      ip, #0x3f
003aec30: add      r1, pc, r1
003aec34: add      r2, pc, r2
003aec38: add      r3, pc, r3
003aec3c: add      r0, r0, #0xa8
003aec40: str      ip, [sp]
003aec44: bl       #0x30e004
003aec48: b        #0x3aeb68
003aec4c: ldr      r0, [pc, #0x60]
003aec50: ldr      r1, [pc, #0x78]
003aec54: ldr      r2, [pc, #0x78]
003aec58: ldr      r0, [r4, r0]
003aec5c: ldr      r3, [pc, #0x74]
003aec60: add      r2, pc, r2
003aec64: mov      ip, #0x40
003aec68: add      r3, pc, r3
003aec6c: add      r1, pc, r1
003aec70: add      r0, r0, #0xa8
003aec74: str      ip, [sp]
003aec78: bl       #0x30e004
003aec7c: ldr      r2, [r6, #8]
003aec80: ldr      r3, [r7]
003aec84: ldr      r0, [r2, r5, lsl #2]
003aec88: mla      r0, r8, r0, r3
003aec8c: b        #0x3aebb4
003aec90: ldrheq   r5, [lr], #-0xf4
003aec94: andeq    r3, r0, r4, asr lr
003aec98: strdeq   r3, r4, [r0], -r4
003aec9c: subseq   r4, r1, r4, ror sp
003aeca0: subseq   lr, r1, r0, ror r2
003aeca4: subseq   r4, r1, ip, asr #26
003aeca8: subseq   lr, r1, r8, asr #4
003aecac: andeq    r3, r0, r0, asr #19
003aecb0: strdeq   r0, r1, [r0], -r8
003aecb4: andeq    r1, r0, r0, asr #19
003aecb8: ldrsbeq  pc, [r0], #-0x7c
003aecbc: subseq   r4, r1, r8, lsl #25
003aecc0: ldrsheq  r4, [r1], #-0xb4
003aecc4: subseq   pc, r0, r8, lsr #15
003aecc8: subseq   r4, r1, r4, asr #25
003aeccc: subseq   r4, r1, r0, asr #23
003aecd0: subseq   pc, r0, ip, ror #14
003aecd4: subseq   r4, r1, r8, lsl #26

# _ZNK16CharStateMachine12SM_IsCastingEv
003c0334: push     {r4, lr}
003c0338: bl       #0x3c01ac
003c033c: cmp      r0, #7
003c0340: movne    r0, #0
003c0344: moveq    r0, #1
003c0348: pop      {r4, pc}

# _ZN17CharAISkillScript13OnSkillUpdateEv
003dabd0: push     {r4, r5, r6, r7, lr}
003dabd4: ldr      r4, [pc, #0xb8]
003dabd8: ldr      r7, [pc, #0xb8]
003dabdc: sub      sp, sp, #0x34
003dabe0: add      r4, pc, r4
003dabe4: ldr      r3, [r4, r7]
003dabe8: add      r5, sp, #4
003dabec: mov      r6, r0
003dabf0: ldr      r3, [r3]
003dabf4: mov      r0, r5
003dabf8: str      r3, [sp, #0x2c]
003dabfc: bl       #0x31b434
003dac00: ldr      r3, [r6, #4]
003dac04: ldr      r0, [r3, #0x3e4]
003dac08: cmp      r0, #0
003dac0c: beq      #0x3dac30
003dac10: ldr      r1, [pc, #0x84]
003dac14: mov      r3, r5
003dac18: add      r2, r6, #0xc
003dac1c: add      r1, pc, r1
003dac20: bl       #0x37c390
003dac24: ldr      r3, [sp, #0xc]
003dac28: cmp      r3, #0
003dac2c: beq      #0x3dac54
003dac30: mov      r0, r5
003dac34: bl       #0x31b398
003dac38: ldr      r3, [r4, r7]
003dac3c: ldr      r2, [sp, #0x2c]
003dac40: ldr      r3, [r3]
003dac44: cmp      r2, r3
003dac48: bne      #0x3dac90
003dac4c: add      sp, sp, #0x34
003dac50: pop      {r4, r5, r6, r7, pc}
003dac54: ldr      r0, [sp, #0x28]
003dac58: ldm      r0, {r1, r2}
003dac5c: cmp      r1, r2
003dac60: beq      #0x3dac6c
003dac64: mov      r3, sp
003dac68: bl       #0x31c3cc
003dac6c: ldr      r3, [r6, #4]
003dac70: ldr      r1, [pc, #0x28]
003dac74: mov      r2, r5
003dac78: ldr      r0, [r3, #0x3e4]
003dac7c: add      r1, pc, r1
003dac80: bl       #0x37c494
003dac84: mov      r0, r5
003dac88: bl       #0x31b398
003dac8c: b        #0x3dac38
003dac90: bl       #0x30e310
003dac94: ldrheq   sb, [fp], #-0xe0
003dac98: andeq    r4, r0, ip, lsr #1
003dac9c: subeq    sl, lr, r4, lsr ip
003daca0: subeq    sl, lr, ip, lsr #24

# _ZN17CharAISkillScriptC1EP9CharacterPKcj
003cde2c: push     {r4, r5, r6, r7, r8, sl, lr}
003cde30: ldr      r5, [pc, #0x11c]
003cde34: ldr      ip, [pc, #0x11c]
003cde38: mov      r4, r0
003cde3c: add      r5, pc, r5
003cde40: ldr      ip, [r5, ip]
003cde44: add      r7, r0, #0xc
003cde48: mov      sl, r1
003cde4c: add      ip, ip, #8
003cde50: str      ip, [r0]
003cde54: sub      sp, sp, #0xc
003cde58: stmib    r4, {r1, r2}
003cde5c: mov      r0, r7
003cde60: mov      r8, r3
003cde64: mov      r6, r2
003cde68: bl       #0x3192b4
003cde6c: mvn      r3, #0
003cde70: cmp      sl, #0
003cde74: str      r3, [r4, #0x18]
003cde78: str      r8, [r4, #0x14]
003cde7c: beq      #0x3cdeac
003cde80: cmp      r6, #0
003cde84: beq      #0x3cdf00
003cde88: mov      r1, r6
003cde8c: mov      r0, r7
003cde90: bl       #0x39ec10
003cde94: mov      r0, r7
003cde98: mov      r1, r8
003cde9c: bl       #0x3cdd78
003cdea0: mov      r0, r4
003cdea4: add      sp, sp, #0xc
003cdea8: pop      {r4, r5, r6, r7, r8, sl, pc}
003cdeac: ldr      r3, [pc, #0xa8]
003cdeb0: ldr      r3, [r5, r3]
003cdeb4: ldr      r3, [r3]
003cdeb8: cmp      r3, #2
003cdebc: streq    sl, [sl]
003cdec0: beq      #0x3cde80
003cdec4: cmp      r3, #1
003cdec8: bne      #0x3cde80
003cdecc: ldr      r0, [pc, #0x8c]
003cded0: ldr      r1, [pc, #0x8c]
003cded4: ldr      r2, [pc, #0x8c]
003cded8: ldr      r0, [r5, r0]
003cdedc: ldr      r3, [pc, #0x88]
003cdee0: mov      ip, #0x2e
003cdee4: add      r1, pc, r1
003cdee8: add      r2, pc, r2
003cdeec: add      r3, pc, r3
003cdef0: add      r0, r0, #0xa8
003cdef4: str      ip, [sp]
003cdef8: bl       #0x30e004
003cdefc: b        #0x3cde80
003cdf00: ldr      r3, [pc, #0x54]
003cdf04: ldr      r3, [r5, r3]
003cdf08: ldr      r3, [r3]
003cdf0c: cmp      r3, #2
003cdf10: streq    r6, [r6]
003cdf14: beq      #0x3cde88
003cdf18: cmp      r3, #1
003cdf1c: bne      #0x3cde88
003cdf20: ldr      r0, [pc, #0x38]
003cdf24: ldr      r1, [pc, #0x44]
003cdf28: ldr      r2, [pc, #0x44]
003cdf2c: ldr      r0, [r5, r0]
003cdf30: ldr      r3, [pc, #0x40]
003cdf34: mov      ip, #0x2e
003cdf38: add      r1, pc, r1
003cdf3c: add      r2, pc, r2
003cdf40: add      r3, pc, r3
003cdf44: add      r0, r0, #0xa8
003cdf48: str      ip, [sp]
003cdf4c: bl       #0x30e004
003cdf50: b        #0x3cde88
003cdf54: subseq   r6, ip, r4, asr ip
003cdf58: andeq    r0, r0, ip, lsr #23
003cdf5c: andeq    r3, r0, r0, asr #19
003cdf60: andeq    r1, r0, r0, asr #19
003cdf64: strdeq   r0, r1, [pc], #-0x44
003cdf68: subeq    r7, pc, r0, lsl r4
003cdf6c: subeq    r7, pc, r4, lsl r4
003cdf70: subeq    r0, pc, r0, lsr #9
003cdf74: subseq   r3, r1, ip, lsr #3
003cdf78: subeq    r7, pc, r0, asr #7

# _ZNK16CharStateMachine15SM_IsUsingSkillEv
003c02e8: push     {r4, lr}
003c02ec: bl       #0x3c01ac
003c02f0: cmp      r0, #6
003c02f4: movne    r0, #0
003c02f8: moveq    r0, #1
003c02fc: pop      {r4, pc}

# _ZNK9Character16GetCharFaeryListEv
003ae5dc: ldr      r3, [pc, #0x20]
003ae5e0: ldr      r2, [pc, #0x20]
003ae5e4: push     {r4, lr}
003ae5e8: add      r3, pc, r3
003ae5ec: ldr      r2, [r3, r2]
003ae5f0: ldr      r4, [r2]
003ae5f4: bl       #0x3ae5a0
003ae5f8: mov      r3, #0xc
003ae5fc: mla      r0, r3, r0, r4
003ae600: pop      {r4, pc}
003ae604: subseq   r6, lr, r8, lsr #9
003ae608: andeq    r3, r0, r4, asr lr

# _ZN9Character7RegenHPEi
003bdca4: push     {r4, r5, r6, r7, r8, sl, lr}
003bdca8: ldr      r5, [pc, #0xd0]
003bdcac: ldr      r8, [pc, #0xd0]
003bdcb0: add      r6, r0, #0xff0
003bdcb4: add      r5, pc, r5
003bdcb8: ldr      r3, [r5, r8]
003bdcbc: add      r6, r6, #4
003bdcc0: add      r7, r0, #0x560
003bdcc4: ldr      r3, [r3]
003bdcc8: mov      r4, r1
003bdccc: sub      sp, sp, #0x24
003bdcd0: mov      r2, #0x24
003bdcd4: mov      r1, r6
003bdcd8: mov      r0, r7
003bdcdc: str      r3, [sp, #0x1c]
003bdce0: bl       #0x3dedb4
003bdce4: mov      sl, r0
003bdce8: mov      r1, r6
003bdcec: mov      r0, r7
003bdcf0: mov      r2, #0x26
003bdcf4: bl       #0x3dedb4
003bdcf8: cmp      r4, #0
003bdcfc: movlt    r4, r0
003bdd00: add      r3, r4, sl
003bdd04: cmp      r3, r0
003bdd08: rsbgt    r4, sl, r0
003bdd0c: cmp      r4, #0
003bdd10: ble      #0x3bdd60
003bdd14: ldr      r3, [pc, #0x6c]
003bdd18: add      r6, sp, #4
003bdd1c: ldr      sl, [r5, r3]
003bdd20: mov      r0, sl
003bdd24: bl       #0x337888
003bdd28: ldr      r1, [pc, #0x5c]
003bdd2c: mov      r2, sp
003bdd30: mov      r0, r6
003bdd34: add      r1, pc, r1
003bdd38: bl       #0x3140ec
003bdd3c: mov      r1, r6
003bdd40: mov      r0, sl
003bdd44: bl       #0x337a88
003bdd48: mov      r0, r6
003bdd4c: bl       #0x318254
003bdd50: mov      r0, r7
003bdd54: mov      r2, r4
003bdd58: mov      r1, #0x24
003bdd5c: bl       #0x3e0708
003bdd60: ldr      r3, [r5, r8]
003bdd64: ldr      r2, [sp, #0x1c]
003bdd68: ldr      r3, [r3]
003bdd6c: cmp      r2, r3
003bdd70: bne      #0x3bdd7c
003bdd74: add      sp, sp, #0x24
003bdd78: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdd7c: bl       #0x30e310
003bdd80: ldrsbeq  r6, [sp], #-0xdc
003bdd84: andeq    r4, r0, ip, lsr #1
003bdd88: andeq    r0, r0, r4, lsl #17
003bdd8c: subseq   r6, r0, ip, asr fp
