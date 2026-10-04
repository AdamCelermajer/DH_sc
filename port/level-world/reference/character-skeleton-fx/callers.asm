
# _ZN9Character24RegisterCharacterFXTableEv direct call 0x3b47cc
003b47ac: bl       #0x337a88
003b47b0: mov      r0, r5
003b47b4: bl       #0x318254
003b47b8: cmp      sl, #0
003b47bc: blt      #0x3b47d0
003b47c0: ldr      r3, [pc, #0x68]
003b47c4: mov      r1, sl
003b47c8: ldr      r0, [r4, r3]
003b47cc: bl       #0x4967e8
003b47d0: cmp      sb, #0
003b47d4: blt      #0x3b47e8
003b47d8: ldr      r3, [pc, #0x50]
003b47dc: mov      r1, sb

# _ZN9Character24RegisterCharacterFXTableEv direct call 0x3b47e4
003b47c4: mov      r1, sl
003b47c8: ldr      r0, [r4, r3]
003b47cc: bl       #0x4967e8
003b47d0: cmp      sb, #0
003b47d4: blt      #0x3b47e8
003b47d8: ldr      r3, [pc, #0x50]
003b47dc: mov      r1, sb
003b47e0: ldr      r0, [r4, r3]
003b47e4: bl       #0x4967e8
003b47e8: cmp      r8, #0
003b47ec: blt      #0x3b4800
003b47f0: ldr      r3, [pc, #0x38]
003b47f4: mov      r1, r8

# _ZN9Character24RegisterCharacterFXTableEv direct call 0x3b47fc
003b47dc: mov      r1, sb
003b47e0: ldr      r0, [r4, r3]
003b47e4: bl       #0x4967e8
003b47e8: cmp      r8, #0
003b47ec: blt      #0x3b4800
003b47f0: ldr      r3, [pc, #0x38]
003b47f4: mov      r1, r8
003b47f8: ldr      r0, [r4, r3]
003b47fc: bl       #0x4967e8
003b4800: ldr      r3, [r4, r6]
003b4804: ldr      r2, [sp, #0x1c]
003b4808: ldr      r3, [r3]
003b480c: cmp      r2, r3

# _ZN12CharAnimator13_AddAnimTableEiijjj direct call 0x3c9ed8
003c9eb8: bl       #0x3699fc
003c9ebc: ldr      r3, [r7, #0xc]
003c9ec0: add      r3, r3, r5
003c9ec4: ldr      r1, [r3, #0x18]
003c9ec8: cmp      r1, #0
003c9ecc: blt      #0x3c9edc
003c9ed0: ldr      r3, [sp, #0x14]
003c9ed4: ldr      r0, [r4, r3]
003c9ed8: bl       #0x4967e8
003c9edc: ldr      r3, [r7, #8]
003c9ee0: add      r6, r6, #1
003c9ee4: add      r5, r5, #0x38
003c9ee8: cmp      r3, r6

# _ZN17ProjectileManager8PreCacheEv direct call 0x3e7408
003e73e8: ldr      r3, [pc, #0xa4]
003e73ec: ldr      r7, [r5, r3]
003e73f0: mov      r5, r4
003e73f4: ldr      r3, [r7]
003e73f8: add      r5, r5, #1
003e73fc: mov      r0, r8
003e7400: add      r3, r3, r4
003e7404: ldr      r1, [r3, #0x14]
003e7408: bl       #0x4967e8
003e740c: cmp      r5, r6
003e7410: add      r4, r4, #0x48
003e7414: bne      #0x3e73f4
003e7418: add      sp, sp, #8

# _ZN5Level12_LoadProcessEv direct call 0x3f7400
003f73e0: bl       #0x3140ec
003f73e4: mov      r1, r7
003f73e8: mov      r0, r8
003f73ec: bl       #0x337a88
003f73f0: mov      r0, r7
003f73f4: bl       #0x3139ac
003f73f8: ldr      r3, [pc, #0x5e8]
003f73fc: ldr      r0, [r5, r3]
003f7400: bl       #0x496bd8
003f7404: ldr      r3, [r4, #0x130]
003f7408: add      r3, r3, #1
003f740c: str      r3, [r4, #0x130]
003f7410: b        #0x3f6e9c

# _ZN5Level12_LoadProcessEv direct call 0x3f7c28
003f7c08: bl       #0x3140ec
003f7c0c: mov      r1, r7
003f7c10: mov      r0, r8
003f7c14: bl       #0x337a88
003f7c18: mov      r0, r7
003f7c1c: bl       #0x3139ac
003f7c20: ldr      r3, [pc, #-0x240]
003f7c24: ldr      r0, [r5, r3]
003f7c28: bl       #0x495a88
003f7c2c: ldr      r3, [r4, #0x130]
003f7c30: add      r3, r3, #1
003f7c34: str      r3, [r4, #0x130]
003f7c38: b        #0x3f6e9c

# _ZN15VisualFXManager19RegisterFXSetToLoadEi direct call 0x4968fc
004968dc: ble      #0x496914
004968e0: ldr      r3, [sl, #0x10]
004968e4: add      r3, r3, r6
004968e8: ldr      r2, [r3, #0x1c]
004968ec: cmp      r2, #1
004968f0: bne      #0x4968c0
004968f4: ldr      r1, [r3, #4]
004968f8: mov      r0, r5
004968fc: bl       #0x4967e8
00496900: ldr      r3, [sl, #0xc]
00496904: add      r8, r8, #1
00496908: add      r6, r6, #0x30
0049690c: cmp      r3, r8

# _ZN15VisualFXManager16_BuildAnimFXSetsEv direct call 0x496b70
00496b50: str      r3, [sp, #0x34]
00496b54: ldrb     r3, [r5, #4]
00496b58: cmp      r3, #0
00496b5c: beq      #0x496aa0
00496b60: ldr      r3, [r5, #0x10]
00496b64: ldr      r0, [sp, #0xc]
00496b68: add      r3, r3, r4
00496b6c: ldr      r1, [r3, #4]
00496b70: bl       #0x4967e8
00496b74: b        #0x496aa0
00496b78: ldr      r0, [sp, #0x20]
00496b7c: ldr      r2, [sp, #0x24]
00496b80: bl       #0x49448c
