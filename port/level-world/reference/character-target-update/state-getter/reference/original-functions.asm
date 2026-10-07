
# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr
