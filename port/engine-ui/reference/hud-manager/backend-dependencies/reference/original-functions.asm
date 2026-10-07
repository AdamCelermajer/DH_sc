
# _ZNK6CharAI21IsScriptProcessLoadedEv
003cb458: ldr      r0, [r0, #0x28]
003cb45c: cmp      r0, #6
003cb460: movle    r0, #0
003cb464: movgt    r0, #1
003cb468: bx       lr

# _ZN17CharAISkillScript7GetInfoEjPf
003daca8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003dacac: ldr      r4, [pc, #0x1b8]
003dacb0: ldr      r8, [pc, #0x1b8]
003dacb4: sub      sp, sp, #0x40
003dacb8: add      r4, pc, r4
003dacbc: ldr      r3, [r4, r8]
003dacc0: mov      r7, r0
003dacc4: add      r5, sp, #0x14
003dacc8: ldr      r3, [r3]
003daccc: mov      r0, sp
003dacd0: mov      sb, r1
003dacd4: str      r3, [sp, #0x3c]
003dacd8: mov      sl, r2
003dacdc: bl       #0x3192b4
003dace0: mov      r0, r5
003dace4: bl       #0x31b434
003dace8: ldr      r3, [r7, #4]
003dacec: mov      r6, sp
003dacf0: ldr      r0, [r3, #0x3e4]
003dacf4: cmp      r0, #0
003dacf8: beq      #0x3dae58
003dacfc: ldr      r1, [pc, #0x170]
003dad00: mov      r3, r5
003dad04: add      r2, r7, #0xc
003dad08: add      r1, pc, r1
003dad0c: bl       #0x37c390
003dad10: ldr      r3, [sp, #0x1c]
003dad14: cmp      r3, #0
003dad18: beq      #0x3dad48
003dad1c: mov      r0, r5
003dad20: bl       #0x31b398
003dad24: mov      r0, sp
003dad28: bl       #0x319228
003dad2c: ldr      r3, [r4, r8]
003dad30: ldr      r2, [sp, #0x3c]
003dad34: ldr      r3, [r3]
003dad38: cmp      r2, r3
003dad3c: bne      #0x3dae68
003dad40: add      sp, sp, #0x40
003dad44: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dad48: mov      r1, sb
003dad4c: mov      r0, sp
003dad50: bl       #0x3cdd78
003dad54: ldr      r0, [sp, #0x38]
003dad58: ldm      r0, {r1, r2}
003dad5c: cmp      r1, r2
003dad60: beq      #0x3dad6c
003dad64: add      r3, sp, #0x10
003dad68: bl       #0x31c3cc
003dad6c: ldr      r3, [r7, #4]
003dad70: ldr      r1, [pc, #0x100]
003dad74: mov      r2, sp
003dad78: ldr      r0, [r3, #0x3e4]
003dad7c: add      r1, pc, r1
003dad80: mov      r3, r5
003dad84: bl       #0x37c390
003dad88: ldr      sb, [sp, #0x1c]
003dad8c: cmp      sb, #0
003dad90: bne      #0x3dad1c
003dad94: cmp      sl, #0
003dad98: beq      #0x3dad1c
003dad9c: ldr      r2, [sp, #0x38]
003dada0: mov      r3, #0
003dada4: str      r3, [sl]
003dada8: ldr      r3, [r2]
003dadac: ldr      r2, [r2, #4]
003dadb0: rsb      r3, r3, r2
003dadb4: asr      r3, r3, #4
003dadb8: add      r2, r3, r3, lsl #3
003dadbc: add      r2, r2, r2, lsl #6
003dadc0: add      r2, r3, r2, lsl #3
003dadc4: add      r2, r2, r2, lsl #15
003dadc8: add      r3, r3, r2, lsl #3
003dadcc: cmp      r3, #0
003dadd0: beq      #0x3dad1c
003dadd4: mov      r0, r5
003dadd8: mov      r1, sb
003daddc: bl       #0x3da43c
003dade0: ldr      r3, [r0, #4]
003dade4: cmp      r3, #3
003dade8: bne      #0x3dad1c
003dadec: mov      r1, sb
003dadf0: mov      r0, r5
003dadf4: bl       #0x3da43c
003dadf8: bl       #0x31bbf0
003dadfc: bl       #0x30e4cc
003dae00: ldr      r7, [r7, #4]
003dae04: mov      r1, r0
003dae08: add      r2, sp, #0xc
003dae0c: add      r7, r7, #0x3b4
003dae10: mov      r0, r7
003dae14: add      r3, sp, #8
003dae18: bl       #0x3db344
003dae1c: cmp      r0, #0
003dae20: beq      #0x3dad1c
003dae24: ldr      r0, [sp, #0xc]
003dae28: bl       #0x30e2e0
003dae2c: mov      r7, r0
003dae30: ldr      r0, [sp, #8]
003dae34: bl       #0x30e2e0
003dae38: mov      r1, r0
003dae3c: mov      r0, r7
003dae40: bl       #0x30ec94
003dae44: mov      r1, r0
003dae48: mov      r0, #0x3f800000
003dae4c: bl       #0x30e3ac
003dae50: str      r0, [sl]
003dae54: b        #0x3dad1c
003dae58: cmp      sl, #0
003dae5c: movne    r3, #0
003dae60: strne    r3, [sl]
003dae64: b        #0x3dad1c
003dae68: bl       #0x30e310
003dae6c: ldrsbeq  sb, [fp], #-0xd8
003dae70: andeq    r4, r0, ip, lsr #1
003dae74: subeq    sl, lr, r8, asr #22
003dae78: subeq    sl, lr, ip, lsr fp

# _ZNK16CharStateMachine12SM_IsCastingEv
003c0334: push     {r4, lr}
003c0338: bl       #0x3c01ac
003c033c: cmp      r0, #7
003c0340: movne    r0, #0
003c0344: moveq    r0, #1
003c0348: pop      {r4, pc}

# _ZN17CharAISkillScript19OnSkillCheck_UsableEv
003da9dc: push     {r4, r5, r6, r7, lr}
003da9e0: ldr      r4, [pc, #0x104]
003da9e4: ldr      r7, [pc, #0x104]
003da9e8: sub      sp, sp, #0x34
003da9ec: add      r4, pc, r4
003da9f0: ldr      r3, [r4, r7]
003da9f4: add      r5, sp, #4
003da9f8: mov      r6, r0
003da9fc: ldr      r3, [r3]
003daa00: mov      r0, r5
003daa04: str      r3, [sp, #0x2c]
003daa08: bl       #0x31b434
003daa0c: ldr      r3, [r6, #4]
003daa10: ldr      r0, [r3, #0x3e4]
003daa14: cmp      r0, #0
003daa18: beq      #0x3daa3c
003daa1c: ldr      r1, [pc, #0xd0]
003daa20: mov      r3, r5
003daa24: add      r2, r6, #0xc
003daa28: add      r1, pc, r1
003daa2c: bl       #0x37c390
003daa30: ldr      r3, [sp, #0xc]
003daa34: cmp      r3, #0
003daa38: beq      #0x3daa68
003daa3c: mov      r6, #0
003daa40: mov      r0, r5
003daa44: bl       #0x31b398
003daa48: ldr      r3, [r4, r7]
003daa4c: ldr      r2, [sp, #0x2c]
003daa50: mov      r0, r6
003daa54: ldr      r3, [r3]
003daa58: cmp      r2, r3
003daa5c: bne      #0x3daae8
003daa60: add      sp, sp, #0x34
003daa64: pop      {r4, r5, r6, r7, pc}
003daa68: ldr      r0, [sp, #0x28]
003daa6c: ldm      r0, {r1, r2}
003daa70: cmp      r1, r2
003daa74: beq      #0x3daa80
003daa78: mov      r3, sp
003daa7c: bl       #0x31c3cc
003daa80: ldr      r3, [r6, #4]
003daa84: ldr      r1, [pc, #0x6c]
003daa88: mov      r2, r5
003daa8c: ldr      r0, [r3, #0x3e4]
003daa90: add      r1, pc, r1
003daa94: bl       #0x37c494
003daa98: ldr      r1, [sp, #0xc]
003daa9c: cmp      r1, #0
003daaa0: bne      #0x3daa3c
003daaa4: ldr      r2, [sp, #0x28]
003daaa8: ldr      r3, [r2]
003daaac: ldr      r2, [r2, #4]
003daab0: rsb      r3, r3, r2
003daab4: asr      r3, r3, #4
003daab8: add      r2, r3, r3, lsl #3
003daabc: add      r2, r2, r2, lsl #6
003daac0: add      r2, r3, r2, lsl #3
003daac4: add      r2, r2, r2, lsl #15
003daac8: add      r3, r3, r2, lsl #3
003daacc: cmp      r3, #0
003daad0: beq      #0x3daa3c
003daad4: mov      r0, r5
003daad8: bl       #0x3da43c
003daadc: bl       #0x31bc80
003daae0: mov      r6, r0
003daae4: b        #0x3daa40
003daae8: bl       #0x30e310
003daaec: subseq   sl, fp, r4, lsr #1
003daaf0: andeq    r4, r0, ip, lsr #1
003daaf4: subeq    sl, lr, r8, lsr #28
003daaf8: strdeq   sl, fp, [lr], #-0xd8

# _ZN13ItemInventory18GetCurrentSkillSetEi
003fc6a0: mov      r0, #0
003fc6a4: bx       lr

# _ZNK16CharStateMachine15SM_IsUsingSkillEv
003c02e8: push     {r4, lr}
003c02ec: bl       #0x3c01ac
003c02f0: cmp      r0, #6
003c02f4: movne    r0, #0
003c02f8: moveq    r0, #1
003c02fc: pop      {r4, pc}
