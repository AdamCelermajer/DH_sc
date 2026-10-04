
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

# _ZNK9Character18GetCharFaeryListIdEv
003ae5a0: movw     r3, #0x106c
003ae5a4: ldr      r0, [r0, r3]
003ae5a8: ldr      r3, [pc, #0x24]
003ae5ac: cmp      r0, #0
003ae5b0: add      r3, pc, r3
003ae5b4: blt      #0x3ae5cc
003ae5b8: ldr      r2, [pc, #0x18]
003ae5bc: ldr      r3, [r3, r2]
003ae5c0: ldr      r3, [r3]
003ae5c4: cmp      r0, r3
003ae5c8: bxlt     lr
003ae5cc: mov      r0, #0
003ae5d0: bx       lr
003ae5d4: subseq   r6, lr, r0, ror #9
003ae5d8: andeq    r4, r0, r4, lsl r5

# _ZNK16CharStateMachine15SM_IsUsingSkillEv
003c02e8: push     {r4, lr}
003c02ec: bl       #0x3c01ac
003c02f0: cmp      r0, #6
003c02f4: movne    r0, #0
003c02f8: moveq    r0, #1
003c02fc: pop      {r4, pc}
