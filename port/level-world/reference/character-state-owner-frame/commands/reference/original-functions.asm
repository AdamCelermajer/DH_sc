
# _ZN16CharStateMachine15SM_SetIdleStateEb
003c1a00: strb     r1, [r0, #0x3c]
003c1a04: mvn      r2, #0
003c1a08: mov      r1, #3
003c1a0c: mov      r3, #0
003c1a10: b        #0x3c1938

# _ZN16CharStateMachine15SM_SetDeadStateEbPvb
003c58c8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c58cc: mov      r4, r0
003c58d0: sub      sp, sp, #4
003c58d4: ldr      r0, [r0, #4]
003c58d8: mov      r5, r1
003c58dc: mov      sb, r2
003c58e0: mov      r7, r3
003c58e4: bl       #0x3a3228
003c58e8: ldr      r6, [pc, #0x190]
003c58ec: cmp      r0, #0
003c58f0: add      r6, pc, r6
003c58f4: blt      #0x3c59d8
003c58f8: ldr      r3, [pc, #0x184]
003c58fc: ldr      r3, [r6, r3]
003c5900: ldr      r3, [r3]
003c5904: cmp      r0, r3
003c5908: bge      #0x3c59d8
003c590c: ldr      r3, [pc, #0x174]
003c5910: ldrb     r2, [r4, #0x3f]
003c5914: mov      r8, #0xa0
003c5918: ldr      r3, [r6, r3]
003c591c: cmp      r2, #0
003c5920: ldr      r3, [r3]
003c5924: mla      r8, r8, r0, r3
003c5928: beq      #0x3c5a10
003c592c: ldr      sl, [pc, #0x158]
003c5930: ldr      r1, [pc, #0x158]
003c5934: ldr      r2, [pc, #0x158]
003c5938: ldr      r3, [r6, sl]
003c593c: add      r1, pc, r1
003c5940: add      r2, pc, r2
003c5944: ldr      r0, [r3, #0x2c]
003c5948: ldr      fp, [r8, #0x10]
003c594c: bl       #0x4c4bdc
003c5950: ands     r0, r0, #0x20000
003c5954: bne      #0x3c5a3c
003c5958: ldrb     r3, [r4, #0x3f]
003c595c: add      fp, r0, fp
003c5960: str      fp, [r4, #0x28]
003c5964: cmp      r3, #0
003c5968: beq      #0x3c59e0
003c596c: ldr      r3, [r6, sl]
003c5970: ldr      r1, [pc, #0x120]
003c5974: ldr      r2, [pc, #0x120]
003c5978: ldr      r0, [r3, #0x2c]
003c597c: add      r1, pc, r1
003c5980: add      r2, pc, r2
003c5984: ldr      r8, [r8, #0x18]
003c5988: bl       #0x4c4bdc
003c598c: ands     r0, r0, #0x40000
003c5990: bne      #0x3c5a58
003c5994: add      r6, r0, r8
003c5998: mov      sl, #0
003c599c: str      r6, [r4, #0x38]
003c59a0: strb     r5, [r4, #0x3e]
003c59a4: strb     sl, [r4, #0x3f]
003c59a8: mov      r0, r4
003c59ac: bl       #0x3c01ec
003c59b0: cmp      r0, sl
003c59b4: strne    sl, [r4, #0x20]
003c59b8: cmp      r7, #0
003c59bc: bne      #0x3c5a64
003c59c0: mov      r0, r4
003c59c4: mov      r2, sb
003c59c8: movw     r1, #0xc358
003c59cc: add      sp, sp, #4
003c59d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c59d4: b        #0x3c5684
003c59d8: add      sp, sp, #4
003c59dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c59e0: ldr      r3, [r6, sl]
003c59e4: ldr      r1, [pc, #0xb4]
003c59e8: ldr      r2, [pc, #0xb4]
003c59ec: ldr      r0, [r3, #0x2c]
003c59f0: add      r1, pc, r1
003c59f4: add      r2, pc, r2
003c59f8: ldr      r6, [r8, #0x14]
003c59fc: bl       #0x4c4bdc
003c5a00: ands     r0, r0, #0x10000
003c5a04: bne      #0x3c5a48
003c5a08: add      r6, r0, r6
003c5a0c: b        #0x3c5998
003c5a10: ldr      sl, [pc, #0x74]
003c5a14: ldr      r1, [pc, #0x8c]
003c5a18: ldr      r2, [pc, #0x8c]
003c5a1c: ldr      r3, [r6, sl]
003c5a20: add      r1, pc, r1
003c5a24: add      r2, pc, r2
003c5a28: ldr      r0, [r3, #0x2c]
003c5a2c: ldr      fp, [r8, #0x1c]
003c5a30: bl       #0x4c4bdc
003c5a34: ands     r0, r0, #0x8000
003c5a38: beq      #0x3c5958
003c5a3c: ldr      r0, [r4, #4]
003c5a40: bl       #0x3a53e0
003c5a44: b        #0x3c5958
003c5a48: ldr      r0, [r4, #4]
003c5a4c: bl       #0x3a53e0
003c5a50: add      r6, r0, r6
003c5a54: b        #0x3c5998
003c5a58: ldr      r0, [r4, #4]
003c5a5c: bl       #0x3a53e0
003c5a60: b        #0x3c5994
003c5a64: mov      r0, r4
003c5a68: mov      r3, sb
003c5a6c: mov      r1, #0xc
003c5a70: movw     r2, #0xc358
003c5a74: add      sp, sp, #4
003c5a78: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c5a7c: b        #0x3c1938
003c5a80: subseq   pc, ip, r0, lsr #3
003c5a84: andeq    r2, r0, r0, asr #17
003c5a88: andeq    r4, r0, r4, asr #16
003c5a8c: strdeq   r3, r4, [r0], -r4
003c5a90: subeq    pc, pc, ip, ror r2
003c5a94: subeq    pc, pc, r8, lsl #5
003c5a98: subeq    pc, pc, ip, lsr r2
003c5a9c: subeq    pc, pc, r8, asr #4
003c5aa0: subeq    pc, pc, r8, asr #3

# _ZN16CharStateMachine17SM_SetAttackStateEPvb
003c6488: cmp      r2, #0
003c648c: bne      #0x3c649c
003c6490: mov      r2, r1
003c6494: movw     r1, #0xc354
003c6498: b        #0x3c5684
003c649c: mov      r3, r1
003c64a0: movw     r2, #0xc354
003c64a4: mov      r1, #5
003c64a8: b        #0x3c1938
