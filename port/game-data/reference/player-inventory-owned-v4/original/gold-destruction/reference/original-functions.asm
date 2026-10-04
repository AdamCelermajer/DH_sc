
# _ZN12ItemInstanceD0Ev
003faab0: push     {r4, lr}
003faab4: mov      r4, r0
003faab8: bl       #0x3faa64
003faabc: mov      r0, r4
003faac0: bl       #0x310440
003faac4: mov      r0, r4
003faac8: pop      {r4, pc}

# _ZN13ItemInventory7AddGoldEi
003fe164: cmp      r1, #0
003fe168: ldrge    r3, [r0, #0x20]
003fe16c: blt      #0x3fe190
003fe170: cmp      r1, #0
003fe174: ble      #0x3fe188
003fe178: ldr      r2, [r0, #0x28]
003fe17c: rsb      r2, r3, r2
003fe180: cmp      r1, r2
003fe184: bicgt    r1, r2, r2, asr #31
003fe188: add      r1, r3, r1
003fe18c: b        #0x3fdfd8
003fe190: ldr      r3, [r0, #0x20]
003fe194: rsb      r2, r1, #0
003fe198: cmp      r3, r2
003fe19c: bge      #0x3fe188
003fe1a0: rsb      r1, r3, #0
003fe1a4: b        #0x3fe170

# _ZN13ItemInventory7SetGoldEi
003fdfd8: push     {r4, r5, r6, lr}
003fdfdc: ldr      r4, [pc, #0x154]
003fdfe0: subs     r6, r1, #0
003fdfe4: sub      sp, sp, #8
003fdfe8: mov      r5, r0
003fdfec: add      r4, pc, r4
003fdff0: blt      #0x3fe0e0
003fdff4: ldr      r2, [pc, #0x140]
003fdff8: ldr      r1, [r5, #0x28]
003fdffc: ldr      r3, [r5, #4]
003fe000: ldr      r2, [r4, r2]
003fe004: cmp      r6, r1
003fe008: strle    r6, [r5, #0x20]
003fe00c: strgt    r1, [r5, #0x20]
003fe010: cmp      r3, #0
003fe014: ldr      r6, [r2]
003fe018: beq      #0x3fe034
003fe01c: mov      r0, r3
003fe020: ldr      r3, [r3]
003fe024: mov      lr, pc
003fe028: ldr      pc, [r3, #0x28]
003fe02c: cmp      r0, #0
003fe030: bne      #0x3fe03c
003fe034: add      sp, sp, #8
003fe038: pop      {r4, r5, r6, pc}
003fe03c: ldr      r3, [pc, #0xfc]
003fe040: ldr      r1, [r5, #4]
003fe044: ldr      r3, [r4, r3]
003fe048: ldr      r0, [r3, #0x40]
003fe04c: bl       #0x36effc
003fe050: cmp      r0, #0
003fe054: beq      #0x3fe034
003fe058: ldr      r2, [r5, #0x20]
003fe05c: movw     r3, #0x270f
003fe060: cmp      r2, r3
003fe064: ble      #0x3fe034
003fe068: ldr      r0, [pc, #0xd4]
003fe06c: add      r0, pc, r0
003fe070: bl       #0x3a3f70
003fe074: mov      r1, r0
003fe078: mov      r0, r6
003fe07c: bl       #0x3813b8
003fe080: ldr      r2, [r5, #0x20]
003fe084: movw     r3, #0x869f
003fe088: movt     r3, #1
003fe08c: cmp      r2, r3
003fe090: ble      #0x3fe034
003fe094: ldr      r0, [pc, #0xac]
003fe098: add      r0, pc, r0
003fe09c: bl       #0x3a3f70
003fe0a0: mov      r1, r0
003fe0a4: mov      r0, r6
003fe0a8: bl       #0x3813b8
003fe0ac: ldr      r2, [r5, #0x20]
003fe0b0: movw     r3, #0x423f
003fe0b4: movt     r3, #0xf
003fe0b8: cmp      r2, r3
003fe0bc: ble      #0x3fe034
003fe0c0: ldr      r0, [pc, #0x84]
003fe0c4: add      r0, pc, r0
003fe0c8: bl       #0x3a3f70
003fe0cc: mov      r1, r0
003fe0d0: mov      r0, r6
003fe0d4: add      sp, sp, #8
003fe0d8: pop      {r4, r5, r6, lr}
003fe0dc: b        #0x3813b8
003fe0e0: ldr      r3, [pc, #0x68]
003fe0e4: ldr      r3, [r4, r3]
003fe0e8: ldr      r3, [r3]
003fe0ec: cmp      r3, #2
003fe0f0: moveq    r3, #0
003fe0f4: streq    r3, [r3]
003fe0f8: beq      #0x3fdff4
003fe0fc: cmp      r3, #1
003fe100: bne      #0x3fdff4
003fe104: ldr      r0, [pc, #0x48]
003fe108: ldr      r1, [pc, #0x48]
003fe10c: ldr      r2, [pc, #0x48]
003fe110: ldr      r0, [r4, r0]
003fe114: ldr      r3, [pc, #0x44]
003fe118: movw     ip, #0x142
003fe11c: add      r1, pc, r1
003fe120: add      r2, pc, r2
003fe124: add      r3, pc, r3
003fe128: add      r0, r0, #0xa8
003fe12c: str      ip, [sp]
003fe130: bl       #0x30e004
003fe134: b        #0x3fdff4
003fe138: subseq   r6, sb, r4, lsr #21
003fe13c: andeq    r1, r0, r0, ror sp
003fe140: strdeq   r3, r4, [r0], -r4
003fe144: subeq    sb, ip, ip, lsr #7
003fe148: umaaleq  sb, ip, r0, r3
003fe14c: subeq    sb, ip, r4, ror r3
003fe150: andeq    r3, r0, r0, asr #19
003fe154: andeq    r1, r0, r0, asr #19
003fe158: strheq   r0, [ip], #-0x2c
003fe15c: ldrdeq   r6, r7, [ip], #-0x70
003fe160: subeq    sb, ip, r4, lsl #5
