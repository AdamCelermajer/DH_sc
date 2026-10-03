
# _ZN14ObjectSearcher10TargetList18_IsGameObjectValidEP10GameObject
004a1950: push     {r4, lr}
004a1954: mov      r3, r0
004a1958: ldr      r0, [r0, #0x38]
004a195c: mov      r2, r1
004a1960: cmp      r0, #0
004a1964: beq      #0x4a1980
004a1968: cmp      r0, #1
004a196c: beq      #0x4a19a4
004a1970: cmp      r0, #3
004a1974: movne    r0, #0
004a1978: moveq    r0, #1
004a197c: pop      {r4, pc}
004a1980: mov      r0, r1
004a1984: ldr      r1, [r3, #0x2c]
004a1988: ldr      r3, [r2]
004a198c: mov      lr, pc
004a1990: ldr      pc, [r3, #0x90]
004a1994: cmp      r0, #8
004a1998: movne    r0, #0
004a199c: moveq    r0, #1
004a19a0: pop      {r4, pc}
004a19a4: mov      r0, r1
004a19a8: ldr      r1, [r3, #0x2c]
004a19ac: ldr      r3, [r2]
004a19b0: mov      lr, pc
004a19b4: ldr      pc, [r3, #0x90]
004a19b8: adds     r0, r0, #1
004a19bc: movne    r0, #1
004a19c0: pop      {r4, pc}

# _ZNK13ItemInventory14CanMeleeAttackERi
003fff30: push     {r4, r5, r6, lr}
003fff34: ldr      r3, [r0]
003fff38: mov      r4, r0
003fff3c: mov      r5, r1
003fff40: mov      lr, pc
003fff44: ldr      pc, [r3, #8]
003fff48: cmp      r0, #0
003fff4c: beq      #0x3fff58
003fff50: mov      r0, #0
003fff54: pop      {r4, r5, r6, pc}
003fff58: mov      r0, r4
003fff5c: mov      r1, #1
003fff60: bl       #0x3fc6a8
003fff64: mov      r3, #0xc
003fff68: mul      r3, r3, r0
003fff6c: ldr      r2, [r4, #0x14]
003fff70: ldr      r3, [r2, r3]
003fff74: ldr      r3, [r3, #4]
003fff78: cmp      r3, #0
003fff7c: beq      #0x3fff98
003fff80: ldr      r0, [r3]
003fff84: bl       #0x3f9e08
003fff88: ldr      r3, [r0, #0x9c]
003fff8c: mov      r0, #1
003fff90: str      r3, [r5]
003fff94: pop      {r4, r5, r6, pc}
003fff98: str      r3, [r5]
003fff9c: mov      r0, #1
003fffa0: pop      {r4, r5, r6, pc}

# _ZNK9Character11GetCharAIIdEv
003a2fec: ldr      r0, [r0, #0xffc]
003a2ff0: ldr      r3, [pc, #0x24]
003a2ff4: cmp      r0, #0
003a2ff8: add      r3, pc, r3
003a2ffc: blt      #0x3a3014
003a3000: ldr      r2, [pc, #0x18]
003a3004: ldr      r3, [r3, r2]
003a3008: ldr      r3, [r3]
003a300c: cmp      r0, r3
003a3010: bxlt     lr
003a3014: mov      r0, #8
003a3018: bx       lr

# _ZNK13ItemInventory14CanRangeAttackERiS0_S0_
003ffebc: push     {r4, r5, r6, r7, r8, lr}
003ffec0: ldr      ip, [r0]
003ffec4: mov      r4, r0
003ffec8: mov      r5, r1
003ffecc: mov      r6, r2
003ffed0: mov      r7, r3
003ffed4: mov      lr, pc
003ffed8: ldr      pc, [ip, #8]
003ffedc: cmp      r0, #0
003ffee0: beq      #0x3fff2c
003ffee4: mov      r1, #1
003ffee8: mov      r0, r4
003ffeec: bl       #0x3fc6a8
003ffef0: mov      r3, #0xc
003ffef4: ldr      r2, [r4, #0x14]
003ffef8: mul      r3, r3, r0
003ffefc: ldr      r3, [r2, r3]
003fff00: ldr      r3, [r3, #4]
003fff04: ldr      r0, [r3]
003fff08: bl       #0x3f9e08
003fff0c: ldr      r2, [r0, #0x98]
003fff10: mov      r3, r0
003fff14: mov      r0, #1
003fff18: str      r2, [r5]
003fff1c: ldr      r2, [r3, #0x9c]
003fff20: str      r2, [r6]
003fff24: ldr      r3, [r3, #0xa0]
003fff28: str      r3, [r7]
003fff2c: pop      {r4, r5, r6, r7, r8, pc}

# _ZNSt14priority_queueIN14ObjectSearcher10TargetInfoESt5dequeIS1_SaIS1_EENS0_12TargetSorterEE4pushERKS1_
004a2440: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004a2444: ldr      r3, [r0, #0x18]
004a2448: ldr      ip, [r0, #0x10]
004a244c: sub      sp, sp, #0x28
004a2450: sub      r3, r3, #0x14
004a2454: cmp      ip, r3
004a2458: mov      r4, r0
004a245c: mov      lr, r1
004a2460: beq      #0x4a24d8
004a2464: ldm      lr!, {r0, r1, r2, r3}
004a2468: stm      ip!, {r0, r1, r2, r3}
004a246c: ldr      r2, [lr]
004a2470: str      r2, [ip]
004a2474: ldr      ip, [r4, #0x10]
004a2478: add      ip, ip, #0x14
004a247c: str      ip, [r4, #0x10]
004a2480: ldmib    r4, {r5, r6, r7}
004a2484: ldr      lr, [r4]
004a2488: ldr      r8, [r4, #0x1c]
004a248c: ldr      sl, [r4, #0x18]
004a2490: ldr      sb, [r4, #0x14]
004a2494: ldr      r2, [r4, #0x28]
004a2498: mov      r4, #0
004a249c: mov      r3, r4
004a24a0: add      r0, sp, #8
004a24a4: add      r1, sp, #0x18
004a24a8: str      r7, [sp, #0x14]
004a24ac: str      r6, [sp, #0x10]
004a24b0: str      r5, [sp, #0xc]
004a24b4: str      lr, [sp, #8]
004a24b8: str      r8, [sp, #0x24]
004a24bc: str      sl, [sp, #0x20]
004a24c0: str      sb, [sp, #0x1c]
004a24c4: str      ip, [sp, #0x18]
004a24c8: str      r4, [sp]
004a24cc: bl       #0x4a1f30
004a24d0: add      sp, sp, #0x28
004a24d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004a24d8: bl       #0x4a22b4
004a24dc: ldr      ip, [r4, #0x10]
004a24e0: b        #0x4a2480

# _ZN12ObjectHandle9GetObjectEb
0033fdc0: push     {r4, r5, r6, r7, r8, lr}
0033fdc4: ldr      r4, [r0]
0033fdc8: ldr      r5, [pc, #0xc0]
0033fdcc: sub      sp, sp, #8
0033fdd0: cmp      r4, #0
0033fdd4: mov      r6, r0
0033fdd8: mov      r7, r1
0033fddc: add      r5, pc, r5
0033fde0: beq      #0x33fe20
0033fde4: ldr      r3, [pc, #0xa8]
0033fde8: ldr      r4, [r0, #4]
0033fdec: ldr      r3, [r5, r3]
0033fdf0: cmp      r4, #0
0033fdf4: ldr      r0, [r3, #0x38]
0033fdf8: ldr      r8, [r0, #0x78]
0033fdfc: beq      #0x33fe0c
0033fe00: ldr      r3, [r6, #8]
0033fe04: cmp      r3, r8
0033fe08: beq      #0x33fe20
0033fe0c: add      r0, r0, #0xc
0033fe10: mov      r1, r6
0033fe14: bl       #0x33fc88
0033fe18: ldr      r4, [r0, #0x18]
0033fe1c: stmib    r6, {r4, r8}
0033fe20: cmp      r7, #0
0033fe24: beq      #0x33fe30
0033fe28: cmp      r4, #0
0033fe2c: beq      #0x33fe3c
0033fe30: mov      r0, r4
0033fe34: add      sp, sp, #8
0033fe38: pop      {r4, r5, r6, r7, r8, pc}
0033fe3c: ldr      r3, [pc, #0x54]
0033fe40: ldr      r3, [r5, r3]
0033fe44: ldr      r3, [r3]
0033fe48: cmp      r3, #2
0033fe4c: streq    r4, [r4]
0033fe50: beq      #0x33fe30
0033fe54: cmp      r3, #1
0033fe58: bne      #0x33fe30
0033fe5c: ldr      r0, [pc, #0x38]
0033fe60: ldr      r1, [pc, #0x38]
0033fe64: ldr      r2, [pc, #0x38]
0033fe68: ldr      r0, [r5, r0]
0033fe6c: ldr      r3, [pc, #0x34]
0033fe70: mov      ip, #0x31
0033fe74: add      r1, pc, r1
0033fe78: add      r2, pc, r2
0033fe7c: add      r3, pc, r3
0033fe80: add      r0, r0, #0xa8
0033fe84: str      ip, [sp]
0033fe88: bl       #0x30e004
0033fe8c: b        #0x33fe30
0033fe90: strhteq  r4, [r5], #-0xc4
0033fe94: strdeq   r3, r4, [r0], -r4
0033fe98: andeq    r3, r0, r0, asr #19
0033fe9c: andeq    r1, r0, r0, asr #19
0033fea0: subseq   lr, r7, r4, ror #10

# _ZN14ObjectSearcher10TargetList17_IsCharacterValidEP9Character
004a1ab8: push     {r4, r5, r6, lr}
004a1abc: ldr      r3, [r0, #0x30]
004a1ac0: mov      r4, r0
004a1ac4: mov      r5, r1
004a1ac8: cmp      r3, #0
004a1acc: beq      #0x4a1c0c
004a1ad0: movw     r2, #0x1314
004a1ad4: ldr      r2, [r3, r2]
004a1ad8: movw     r3, #0x1310
004a1adc: ldr      r3, [r1, r3]
004a1ae0: cmp      r2, r3
004a1ae4: bge      #0x4a1af0
004a1ae8: mov      r0, #0
004a1aec: pop      {r4, r5, r6, pc}
004a1af0: ldr      r3, [r0, #0x34]
004a1af4: cmn      r3, #0x80000001
004a1af8: beq      #0x4a1c0c
004a1afc: tst      r3, #0x80
004a1b00: bne      #0x4a1c14
004a1b04: tst      r3, #0x10
004a1b08: bne      #0x4a1c34
004a1b0c: tst      r3, #0x20
004a1b10: bne      #0x4a1c4c
004a1b14: tst      r3, #0x40
004a1b18: beq      #0x4a1b28
004a1b1c: ldrb     r2, [r5, #0x2fa]
004a1b20: cmp      r2, #0
004a1b24: bne      #0x4a1c0c
004a1b28: tst      r3, #1
004a1b2c: bne      #0x4a1ba8
004a1b30: tst      r3, #2
004a1b34: bne      #0x4a1c64
004a1b38: tst      r3, #4
004a1b3c: bne      #0x4a1ca8
004a1b40: tst      r3, #8
004a1b44: beq      #0x4a1ae8
004a1b48: ldr      r3, [r5]
004a1b4c: mov      r0, r5
004a1b50: mov      lr, pc
004a1b54: ldr      pc, [r3, #0x34]
004a1b58: cmp      r0, #0
004a1b5c: beq      #0x4a1ae8
004a1b60: ldr      r0, [r4, #0x30]
004a1b64: mov      r1, r5
004a1b68: add      r0, r0, #0x3c8
004a1b6c: bl       #0x3d511c
004a1b70: cmp      r0, #0
004a1b74: beq      #0x4a1ae8
004a1b78: ldr      r0, [r4, #0x30]
004a1b7c: add      r0, r0, #0x4f0
004a1b80: add      r0, r0, #0xc
004a1b84: bl       #0x3c01ac
004a1b88: cmp      r0, #0xf
004a1b8c: beq      #0x4a1ae8
004a1b90: add      r0, r5, #0x4f0
004a1b94: add      r0, r0, #0xc
004a1b98: bl       #0x3c01ac
004a1b9c: subs     r0, r0, #0x10
004a1ba0: movne    r0, #1
004a1ba4: pop      {r4, r5, r6, pc}
004a1ba8: ldr      r3, [r5]
004a1bac: mov      r0, r5
004a1bb0: mov      lr, pc
004a1bb4: ldr      pc, [r3, #0x34]
004a1bb8: cmp      r0, #0
004a1bbc: bne      #0x4a1ca0
004a1bc0: ldr      r0, [r4, #0x30]
004a1bc4: mov      r1, r5
004a1bc8: add      r0, r0, #0x3c8
004a1bcc: bl       #0x3d574c
004a1bd0: cmp      r0, #0
004a1bd4: beq      #0x4a1ca0
004a1bd8: ldr      r3, [r5]
004a1bdc: mov      r0, r5
004a1be0: mov      lr, pc
004a1be4: ldr      pc, [r3, #0x28]
004a1be8: cmp      r0, #0
004a1bec: beq      #0x4a1c0c
004a1bf0: ldr      r3, [r4, #0x30]
004a1bf4: mov      r0, r3
004a1bf8: ldr      r3, [r3]
004a1bfc: mov      lr, pc
004a1c00: ldr      pc, [r3, #0x28]
004a1c04: cmp      r0, #0
004a1c08: bne      #0x4a1ca0
004a1c0c: mov      r0, #1
004a1c10: pop      {r4, r5, r6, pc}
004a1c14: ldr      r3, [r1]
004a1c18: mov      r0, r1
004a1c1c: mov      lr, pc
004a1c20: ldr      pc, [r3, #0x28]
004a1c24: cmp      r0, #0
004a1c28: ldreq    r3, [r4, #0x34]
004a1c2c: beq      #0x4a1b04
004a1c30: b        #0x4a1c0c
004a1c34: mov      r0, r5
004a1c38: bl       #0x3a30c4
004a1c3c: cmp      r0, #0
004a1c40: ldreq    r3, [r4, #0x34]
004a1c44: beq      #0x4a1b0c
004a1c48: b        #0x4a1c0c
004a1c4c: mov      r0, r5
004a1c50: bl       #0x3a30f4
004a1c54: cmp      r0, #0
004a1c58: ldreq    r3, [r4, #0x34]
004a1c5c: beq      #0x4a1b14
004a1c60: b        #0x4a1c0c
004a1c64: ldr      r3, [r5]
004a1c68: mov      r0, r5
004a1c6c: mov      lr, pc
004a1c70: ldr      pc, [r3, #0x34]
004a1c74: cmp      r0, #0
004a1c78: beq      #0x4a1c84
004a1c7c: ldr      r3, [r4, #0x34]
004a1c80: b        #0x4a1b38
004a1c84: ldr      r0, [r4, #0x30]
004a1c88: mov      r1, r5
004a1c8c: add      r0, r0, #0x3c8
004a1c90: bl       #0x3d5a98
004a1c94: cmp      r0, #0
004a1c98: bne      #0x4a1c0c
004a1c9c: b        #0x4a1c7c
004a1ca0: ldr      r3, [r4, #0x34]
004a1ca4: b        #0x4a1b30
004a1ca8: ldr      r0, [r4, #0x30]
004a1cac: mov      r1, r5
004a1cb0: add      r0, r0, #0x3c8
004a1cb4: bl       #0x3d511c
004a1cb8: cmp      r0, #0
004a1cbc: ldreq    r3, [r4, #0x34]
004a1cc0: beq      #0x4a1b40
004a1cc4: b        #0x4a1c0c
