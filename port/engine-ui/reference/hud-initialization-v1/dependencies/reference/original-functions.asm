
# _ZN9Character13SG_GetSkillIdEj
003bbeec: movw     r3, #0x14e8
003bbef0: ldr      r0, [r0, r3]
003bbef4: cmp      r0, #0
003bbef8: beq      #0x3bbf00
003bbefc: b        #0x466a08
003bbf00: mvn      r0, #0
003bbf04: bx       lr

# _ZN9Character15SG_GetSkillSlotEj
003bbe84: movw     r3, #0x14e8
003bbe88: ldr      r0, [r0, r3]
003bbe8c: cmp      r0, #0
003bbe90: beq      #0x3bbe98
003bbe94: b        #0x46752c
003bbe98: mvn      r0, #0
003bbe9c: bx       lr

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

# _ZN9Character28SG_GetGameDifficultyUnlockedEv
003bb918: movw     r3, #0x14e8
003bb91c: ldr      r3, [r0, r3]
003bb920: cmp      r3, #0
003bb924: mvneq    r0, #0
003bb928: ldrne    r0, [r3, #0x3c]
003bb92c: bx       lr

# _ZN9Character17CanIncrementSkillEi
003bc9ec: push     {r4, r5, r6, r7, r8, lr}
003bc9f0: movw     r3, #0x14e8
003bc9f4: ldr      r3, [r0, r3]
003bc9f8: mov      r4, r0
003bc9fc: mov      r7, r1
003bca00: cmp      r3, #0
003bca04: beq      #0x3bca48
003bca08: ldr      r3, [r3, #0x80]
003bca0c: cmp      r3, #0
003bca10: beq      #0x3bca48
003bca14: add      r3, r3, r1, lsl #3
003bca18: ldrh     r5, [r3, #4]
003bca1c: bl       #0x3bd120
003bca20: mov      r1, r7
003bca24: mov      r6, r0
003bca28: mov      r0, r4
003bca2c: bl       #0x3bc784
003bca30: ldr      r0, [r0, #0x20]
003bca34: rsb      r6, r0, r6
003bca38: cmp      r5, r6
003bca3c: movgt    r0, #0
003bca40: movle    r0, #1
003bca44: pop      {r4, r5, r6, r7, r8, pc}
003bca48: mov      r0, #0
003bca4c: pop      {r4, r5, r6, r7, r8, pc}

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

# _ZN9Character17SG_GetFaerieLevelEji
003bbc18: movw     r3, #0x14e8
003bbc1c: ldr      r0, [r0, r3]
003bbc20: ldr      r3, [pc, #0x28]
003bbc24: cmp      r0, #0
003bbc28: add      r3, pc, r3
003bbc2c: beq      #0x3bbc48
003bbc30: cmn      r2, #1
003bbc34: bne      #0x3bbc44
003bbc38: ldr      r2, [pc, #0x14]
003bbc3c: ldr      r3, [r3, r2]
003bbc40: ldr      r2, [r3]
003bbc44: b        #0x466700
003bbc48: mvn      r0, #0
003bbc4c: bx       lr
003bbc50: subseq   r8, sp, r8, ror #28
003bbc54: muleq    r0, ip, sl

# _ZNK9Character21SG_GetCurrentFaerieIdEi
003bb98c: movw     r3, #0x14e8
003bb990: ldr      r0, [r0, r3]
003bb994: ldr      r3, [pc, #0x34]
003bb998: cmp      r0, #0
003bb99c: add      r3, pc, r3
003bb9a0: bxeq     lr
003bb9a4: cmn      r1, #1
003bb9a8: beq      #0x3bb9b8
003bb9ac: add      r0, r0, r1, lsl #2
003bb9b0: ldr      r0, [r0, #0xac]
003bb9b4: bx       lr
003bb9b8: ldr      r2, [pc, #0x14]
003bb9bc: ldr      r3, [r3, r2]
003bb9c0: ldr      r3, [r3]
003bb9c4: add      r0, r0, r3, lsl #2
003bb9c8: ldr      r0, [r0, #0xac]
003bb9cc: bx       lr
003bb9d0: ldrsheq  sb, [sp], #-4
003bb9d4: muleq    r0, ip, sl

# _Z19NativeGetPlayerCharib
0043c388: ldr      r3, [pc, #0x6c]
0043c38c: ldr      r2, [pc, #0x6c]
0043c390: push     {r4, r5, r6, lr}
0043c394: add      r3, pc, r3
0043c398: ldr      r2, [r3, r2]
0043c39c: subs     r4, r0, #0
0043c3a0: mov      r5, r1
0043c3a4: ldr      r6, [r2, #0x40]
0043c3a8: blt      #0x43c3bc
0043c3ac: mov      r0, r6
0043c3b0: bl       #0x36d7a8
0043c3b4: cmp      r4, r0
0043c3b8: blt      #0x43c3c4
0043c3bc: mov      r0, #0
0043c3c0: pop      {r4, r5, r6, pc}
0043c3c4: cmp      r5, #0
0043c3c8: bne      #0x43c3e4
0043c3cc: mov      r0, r6
0043c3d0: mov      r1, r4
0043c3d4: mov      r2, r5
0043c3d8: bl       #0x36e478
0043c3dc: ldr      r0, [r0, #0x660]
0043c3e0: pop      {r4, r5, r6, pc}
0043c3e4: mov      r0, r6
0043c3e8: mov      r1, r4
0043c3ec: mov      r2, #0
0043c3f0: bl       #0x36e2ac
0043c3f4: ldr      r0, [r0, #0x660]
0043c3f8: pop      {r4, r5, r6, pc}
0043c3fc: ldrsheq  r8, [r5], #-0x6c
0043c400: strdeq   r3, r4, [r0], -r4
