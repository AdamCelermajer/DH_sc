
# _ZN6CharAI14OnStateChangedEii
003d0bec: push     {r4, lr}
003d0bf0: ldr      r3, [r0, #0x1c]
003d0bf4: cmp      r3, #0
003d0bf8: beq      #0x3d0c0c
003d0bfc: mov      r0, r3
003d0c00: ldr      r3, [r3]
003d0c04: mov      lr, pc
003d0c08: ldr      pc, [r3, #0x20]
003d0c0c: pop      {r4, pc}

# _ZNK16CharStateMachine9_GetEventEii
003c1694: push     {r4, r5, r6, r7, lr}
003c1698: sub      sp, sp, #0xc
003c169c: mov      r4, r2
003c16a0: mov      r7, r0
003c16a4: mov      r5, r1
003c16a8: bl       #0x3c0084
003c16ac: ldr      r6, [pc, #0x174]
003c16b0: cmp      r0, #0
003c16b4: add      r6, pc, r6
003c16b8: bne      #0x3c16dc
003c16bc: ldr      r3, [pc, #0x168]
003c16c0: ldr      r3, [r6, r3]
003c16c4: ldr      r3, [r3]
003c16c8: cmp      r3, #2
003c16cc: streq    r0, [r0]
003c16d0: beq      #0x3c16dc
003c16d4: cmp      r3, #1
003c16d8: beq      #0x3c17c0
003c16dc: mov      r0, r7
003c16e0: mov      r1, r5
003c16e4: mov      r2, r4
003c16e8: bl       #0x3c00e0
003c16ec: cmp      r0, #0
003c16f0: bne      #0x3c1714
003c16f4: ldr      r3, [pc, #0x130]
003c16f8: ldr      r3, [r6, r3]
003c16fc: ldr      r3, [r3]
003c1700: cmp      r3, #2
003c1704: streq    r0, [r0]
003c1708: beq      #0x3c1714
003c170c: cmp      r3, #1
003c1710: beq      #0x3c17f4
003c1714: ldr      r3, [r7, #0xc]
003c1718: add      r7, r7, #8
003c171c: cmp      r3, #0
003c1720: beq      #0x3c1764
003c1724: mov      r1, r7
003c1728: b        #0x3c1730
003c172c: mov      r3, r2
003c1730: ldr      r2, [r3, #0x10]
003c1734: cmp      r5, r2
003c1738: ldrgt    r2, [r3, #0xc]
003c173c: ldrle    r2, [r3, #8]
003c1740: movgt    r3, r1
003c1744: mov      r1, r3
003c1748: cmp      r2, #0
003c174c: bne      #0x3c172c
003c1750: cmp      r7, r3
003c1754: beq      #0x3c1764
003c1758: ldr      r2, [r3, #0x10]
003c175c: cmp      r5, r2
003c1760: movge    r7, r3
003c1764: ldr      r3, [r7, #0x20]
003c1768: add      r5, r7, #0x1c
003c176c: cmp      r3, #0
003c1770: beq      #0x3c17b4
003c1774: mov      r1, r5
003c1778: b        #0x3c1780
003c177c: mov      r3, r2
003c1780: ldr      r2, [r3, #0x10]
003c1784: cmp      r2, r4
003c1788: ldrlt    r2, [r3, #0xc]
003c178c: ldrge    r2, [r3, #8]
003c1790: movlt    r3, r1
003c1794: mov      r1, r3
003c1798: cmp      r2, #0
003c179c: bne      #0x3c177c
003c17a0: cmp      r5, r3
003c17a4: beq      #0x3c17b4
003c17a8: ldr      r2, [r3, #0x10]
003c17ac: cmp      r2, r4
003c17b0: movle    r5, r3
003c17b4: add      r0, r5, #0x14
003c17b8: add      sp, sp, #0xc
003c17bc: pop      {r4, r5, r6, r7, pc}
003c17c0: ldr      r0, [pc, #0x68]
003c17c4: ldr      r1, [pc, #0x68]
003c17c8: ldr      r2, [pc, #0x68]
003c17cc: ldr      r0, [r6, r0]
003c17d0: ldr      r3, [pc, #0x64]
003c17d4: mov      ip, #0xd0
003c17d8: add      r1, pc, r1
003c17dc: add      r2, pc, r2
003c17e0: add      r3, pc, r3
003c17e4: add      r0, r0, #0xa8
003c17e8: str      ip, [sp]
003c17ec: bl       #0x30e004
003c17f0: b        #0x3c16dc
003c17f4: ldr      r0, [pc, #0x34]
003c17f8: ldr      r1, [pc, #0x40]
003c17fc: ldr      r2, [pc, #0x40]
003c1800: ldr      r0, [r6, r0]
003c1804: ldr      r3, [pc, #0x3c]
003c1808: mov      ip, #0xd1
003c180c: add      r1, pc, r1
003c1810: add      r2, pc, r2
003c1814: add      r3, pc, r3
003c1818: add      r0, r0, #0xa8
003c181c: str      ip, [sp]
003c1820: bl       #0x30e004
003c1824: b        #0x3c1714
003c1828: ldrsbeq  r3, [sp], #-0x3c
003c182c: andeq    r3, r0, r0, asr #19
003c1830: andeq    r1, r0, r0, asr #19
003c1834: subeq    ip, pc, r0, lsl #24
003c1838: subseq   r3, r0, r4, ror r4
003c183c: subseq   r3, r0, r8, lsl #8
003c1840: subeq    ip, pc, ip, asr #23
003c1844: subseq   r3, r0, r8, asr r4
003c1848: ldrsbeq  r3, [r0], #-0x34

# _ZNK16CharStateMachine9_HasEventEii
003c00e0: str      r4, [sp, #-4]!
003c00e4: ldr      r3, [r0, #0xc]
003c00e8: add      r0, r0, #8
003c00ec: cmp      r3, #0
003c00f0: beq      #0x3c0198
003c00f4: mov      r4, r0
003c00f8: b        #0x3c0100
003c00fc: mov      r3, ip
003c0100: ldr      ip, [r3, #0x10]
003c0104: cmp      r1, ip
003c0108: ldrgt    ip, [r3, #0xc]
003c010c: ldrle    ip, [r3, #8]
003c0110: movgt    r3, r4
003c0114: mov      r4, r3
003c0118: cmp      ip, #0
003c011c: bne      #0x3c00fc
003c0120: cmp      r0, r3
003c0124: beq      #0x3c018c
003c0128: ldr      ip, [r3, #0x10]
003c012c: cmp      r1, ip
003c0130: blt      #0x3c0198
003c0134: cmp      r0, r3
003c0138: beq      #0x3c018c
003c013c: ldr      r1, [r3, #0x20]
003c0140: add      r3, r3, #0x1c
003c0144: cmp      r1, #0
003c0148: beq      #0x3c018c
003c014c: mov      r4, r3
003c0150: b        #0x3c0158
003c0154: mov      r1, ip
003c0158: ldr      ip, [r1, #0x10]
003c015c: cmp      r2, ip
003c0160: ldrgt    ip, [r1, #0xc]
003c0164: ldrle    ip, [r1, #8]
003c0168: movgt    r1, r4
003c016c: mov      r4, r1
003c0170: cmp      ip, #0
003c0174: bne      #0x3c0154
003c0178: cmp      r3, r1
003c017c: beq      #0x3c018c
003c0180: ldr      r0, [r1, #0x10]
003c0184: cmp      r2, r0
003c0188: bge      #0x3c01a0
003c018c: mov      r0, #0
003c0190: ldm      sp!, {r4}
003c0194: bx       lr
003c0198: mov      r3, r0
003c019c: b        #0x3c0134
003c01a0: subs     r0, r1, r3
003c01a4: movne    r0, #1
003c01a8: b        #0x3c0190
