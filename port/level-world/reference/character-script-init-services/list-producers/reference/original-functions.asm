
# _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp

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
