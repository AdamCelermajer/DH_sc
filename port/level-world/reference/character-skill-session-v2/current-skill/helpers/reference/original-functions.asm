
# _ZNK9Character16SG_GetSkillLevelEj
003bbed0: movw     r3, #0x14e8
003bbed4: ldr      r0, [r0, r3]
003bbed8: cmp      r0, #0
003bbedc: beq      #0x3bbee4
003bbee0: b        #0x4668dc
003bbee4: mvn      r0, #0
003bbee8: bx       lr

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

# _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
0037cb24: ldr      r3, [pc, #0x58]
0037cb28: ldr      r2, [pc, #0x58]
0037cb2c: push     {r4, r5, r6, lr}
0037cb30: add      r3, pc, r3
0037cb34: ldr      r5, [r3, r2]
0037cb38: sub      sp, sp, #0x78
0037cb3c: add      r4, sp, #4
0037cb40: ldr      r3, [r5]
0037cb44: str      r3, [sp, #0x74]
0037cb48: ldr      r6, [r0, #0x24]
0037cb4c: mov      r0, r4
0037cb50: bl       #0x37ca9c
0037cb54: mov      r0, r6
0037cb58: mov      r1, r4
0037cb5c: bl       #0x3195c0
0037cb60: mov      r0, r4
0037cb64: bl       #0x3193e8
0037cb68: ldr      r2, [sp, #0x74]
0037cb6c: ldr      r3, [r5]
0037cb70: cmp      r2, r3
0037cb74: bne      #0x37cb80
0037cb78: add      sp, sp, #0x78
0037cb7c: pop      {r4, r5, r6, pc}
0037cb80: bl       #0x30e310
0037cb84: rsbeq    r7, r1, r0, ror #30
0037cb88: andeq    r4, r0, ip, lsr #1
