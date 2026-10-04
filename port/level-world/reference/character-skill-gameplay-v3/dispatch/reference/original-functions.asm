
# _ZNK9Character7HasManaEi
003bd40c: push     {r4, r5, r6, lr}
003bd410: sub      sp, sp, #8
003bd414: mov      r4, r0
003bd418: mov      r5, r1
003bd41c: bl       #0x7fd794
003bd420: ldrb     r3, [r0, #5]
003bd424: ldr      r6, [pc, #0xb4]
003bd428: cmp      r3, #0
003bd42c: add      r6, pc, r6
003bd430: bne      #0x3bd464
003bd434: cmp      r5, #0
003bd438: blt      #0x3bd488
003bd43c: add      r1, r4, #0xff0
003bd440: add      r1, r1, #4
003bd444: add      r0, r4, #0x560
003bd448: mov      r2, #0x29
003bd44c: bl       #0x3dedb4
003bd450: cmp      r5, r0
003bd454: movgt    r0, #0
003bd458: movle    r0, #1
003bd45c: add      sp, sp, #8
003bd460: pop      {r4, r5, r6, pc}
003bd464: ldr      r3, [r4]
003bd468: mov      r0, r4
003bd46c: mov      lr, pc
003bd470: ldr      pc, [r3, #0x54]
003bd474: cmp      r0, #0
003bd478: movne    r0, #1
003bd47c: bne      #0x3bd45c
003bd480: cmp      r5, #0
003bd484: bge      #0x3bd43c
003bd488: ldr      r3, [pc, #0x54]
003bd48c: ldr      r3, [r6, r3]
003bd490: ldr      r3, [r3]
003bd494: cmp      r3, #2
003bd498: moveq    r3, #0
003bd49c: streq    r3, [r3]
003bd4a0: beq      #0x3bd43c
003bd4a4: cmp      r3, #1
003bd4a8: bne      #0x3bd43c
003bd4ac: ldr      r0, [pc, #0x34]
003bd4b0: ldr      r1, [pc, #0x34]
003bd4b4: ldr      r2, [pc, #0x34]
003bd4b8: ldr      r0, [r6, r0]
003bd4bc: ldr      r3, [pc, #0x30]
003bd4c0: mov      ip, #0x91
003bd4c4: add      r1, pc, r1
003bd4c8: add      r2, pc, r2
003bd4cc: add      r3, pc, r3
003bd4d0: add      r0, r0, #0xa8
003bd4d4: str      ip, [sp]
003bd4d8: bl       #0x30e004
003bd4dc: b        #0x3bd43c
003bd4e0: subseq   r7, sp, r4, ror #12
003bd4e4: andeq    r3, r0, r0, asr #19
003bd4e8: andeq    r1, r0, r0, asr #19
003bd4ec: subseq   r0, r0, r4, lsl pc
003bd4f0: subseq   r7, r0, r8, lsr #8
003bd4f4: subseq   r7, r0, r4, lsr r4

# _ZN9Character7UseManaEi
003bdef4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003bdef8: ldr      r4, [pc, #0x1a4]
003bdefc: ldr      r5, [pc, #0x1a4]
003bdf00: sub      sp, sp, #0x48
003bdf04: add      r4, pc, r4
003bdf08: ldr      r3, [r4, r5]
003bdf0c: mov      r6, r0
003bdf10: mov      r7, r1
003bdf14: ldr      r3, [r3]
003bdf18: str      r3, [sp, #0x44]
003bdf1c: bl       #0x7fd794
003bdf20: ldrb     r3, [r0, #5]
003bdf24: cmp      r3, #0
003bdf28: bne      #0x3bdf74
003bdf2c: cmp      r7, #0
003bdf30: blt      #0x3bdf94
003bdf34: ldr      sb, [pc, #0x170]
003bdf38: ldr      r3, [pc, #0x170]
003bdf3c: add      sb, pc, sb
003bdf40: ldr      r0, [r4, r3]
003bdf44: mov      r1, sb
003bdf48: bl       #0x320e14
003bdf4c: cmp      r0, #0
003bdf50: beq      #0x3bdfec
003bdf54: mov      r0, #1
003bdf58: ldr      r3, [r4, r5]
003bdf5c: ldr      r2, [sp, #0x44]
003bdf60: ldr      r3, [r3]
003bdf64: cmp      r2, r3
003bdf68: bne      #0x3be0a0
003bdf6c: add      sp, sp, #0x48
003bdf70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003bdf74: ldr      r3, [r6]
003bdf78: mov      r0, r6
003bdf7c: mov      lr, pc
003bdf80: ldr      pc, [r3, #0x54]
003bdf84: cmp      r0, #0
003bdf88: bne      #0x3bdf54
003bdf8c: cmp      r7, #0
003bdf90: bge      #0x3bdf34
003bdf94: ldr      r3, [pc, #0x118]
003bdf98: ldr      r3, [r4, r3]
003bdf9c: ldr      r3, [r3]
003bdfa0: cmp      r3, #2
003bdfa4: moveq    r3, #0
003bdfa8: streq    r3, [r3]
003bdfac: beq      #0x3bdf34
003bdfb0: cmp      r3, #1
003bdfb4: bne      #0x3bdf34
003bdfb8: ldr      r0, [pc, #0xf8]
003bdfbc: ldr      r1, [pc, #0xf8]
003bdfc0: ldr      r2, [pc, #0xf8]
003bdfc4: ldr      r0, [r4, r0]
003bdfc8: ldr      r3, [pc, #0xf4]
003bdfcc: mov      ip, #0x9f
003bdfd0: add      r1, pc, r1
003bdfd4: add      r2, pc, r2
003bdfd8: add      r3, pc, r3
003bdfdc: add      r0, r0, #0xa8
003bdfe0: str      ip, [sp]
003bdfe4: bl       #0x30e004
003bdfe8: b        #0x3bdf34
003bdfec: ldr      r3, [pc, #0xd4]
003bdff0: add      r8, sp, #0x2c
003bdff4: ldr      sl, [r4, r3]
003bdff8: mov      r0, sl
003bdffc: bl       #0x337888
003be000: mov      r1, sb
003be004: add      r2, sp, #0x10
003be008: mov      r0, r8
003be00c: bl       #0x3140ec
003be010: mov      r1, r8
003be014: mov      r0, sl
003be018: bl       #0x337a88
003be01c: mov      sb, r0
003be020: mov      r0, r8
003be024: bl       #0x318254
003be028: cmp      sb, #0
003be02c: bne      #0x3bdf54
003be030: movw     r3, #0x14f0
003be034: ldrb     r3, [r6, r3]
003be038: cmp      r3, #0
003be03c: bne      #0x3bdf54
003be040: mov      r0, r6
003be044: mov      r1, r7
003be048: bl       #0x3bd40c
003be04c: cmp      r0, #0
003be050: beq      #0x3bdf58
003be054: rsb      r2, r7, #0
003be058: add      r0, r6, #0x560
003be05c: mov      r1, #0x29
003be060: bl       #0x3e0708
003be064: mov      r0, sl
003be068: bl       #0x337888
003be06c: ldr      r1, [pc, #0x58]
003be070: add      r6, sp, #0x14
003be074: add      r2, sp, #0xc
003be078: add      r1, pc, r1
003be07c: mov      r0, r6
003be080: bl       #0x3140ec
003be084: mov      r1, r6
003be088: mov      r0, sl
003be08c: bl       #0x337a88
003be090: mov      r0, r6
003be094: bl       #0x318254
003be098: mov      r0, #1
003be09c: b        #0x3bdf58
003be0a0: bl       #0x30e310
003be0a4: subseq   r6, sp, ip, lsl #23
003be0a8: andeq    r4, r0, ip, lsr #1
003be0ac: subseq   r6, r0, r4, lsr sl
003be0b0: strdeq   r3, r4, [r0], -r4
003be0b4: andeq    r3, r0, r0, asr #19
003be0b8: andeq    r1, r0, r0, asr #19
003be0bc: subseq   r0, r0, r8, lsl #8
003be0c0: subseq   r6, r0, ip, lsl sb
003be0c4: subseq   r6, r0, r8, lsr #18
003be0c8: andeq    r0, r0, r4, lsl #17
003be0cc: subseq   r6, r0, r8, lsl r8

# _ZN10AISDefault13OnScriptTimerEj
003dcc80: push     {r4, r5, r6, lr}
003dcc84: sub      sp, sp, #8
003dcc88: mov      r5, r0
003dcc8c: mov      r6, r1
003dcc90: mov      r0, sp
003dcc94: bl       #0x3192b4
003dcc98: mov      r0, sp
003dcc9c: mov      r1, r6
003dcca0: bl       #0x3cdd78
003dcca4: ldr      r1, [pc, #0x20]
003dcca8: mov      r0, r5
003dccac: mov      r2, sp
003dccb0: add      r1, pc, r1
003dccb4: bl       #0x37c41c
003dccb8: mov      r0, sp
003dccbc: mov      r4, sp
003dccc0: bl       #0x319228
003dccc4: add      sp, sp, #8
003dccc8: pop      {r4, r5, r6, pc}
003dcccc: strdeq   r8, sb, [lr], #-0xc0

# _ZN16CharStateMachine16SM_SetSkillStateEjbPvb
003c6670: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6674: mov      r4, r0
003c6678: ldr      r0, [r0, #4]
003c667c: mov      sb, r2
003c6680: mov      r6, r3
003c6684: mov      r7, r1
003c6688: ldrb     r8, [sp, #0x20]
003c668c: bl       #0x3bc784
003c6690: ldr      r5, [pc, #0x7c]
003c6694: ldr      r3, [pc, #0x7c]
003c6698: ldr      r1, [pc, #0x7c]
003c669c: add      r5, pc, r5
003c66a0: ldr      r3, [r5, r3]
003c66a4: ldr      r2, [pc, #0x74]
003c66a8: ldr      sl, [r0, #4]
003c66ac: add      r1, pc, r1
003c66b0: ldr      r0, [r3, #0x2c]
003c66b4: add      r2, pc, r2
003c66b8: bl       #0x4c4bdc
003c66bc: ands     r0, r0, #0x200000
003c66c0: bne      #0x3c6708
003c66c4: add      sl, r0, sl
003c66c8: cmp      r8, #0
003c66cc: str      sl, [r4, #0x28]
003c66d0: str      r7, [r4, #0x54]
003c66d4: strb     sb, [r4, #0x58]
003c66d8: bne      #0x3c66f0
003c66dc: mov      r0, r4
003c66e0: mov      r2, r6
003c66e4: movw     r1, #0xc355
003c66e8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c66ec: b        #0x3c5684
003c66f0: mov      r0, r4
003c66f4: mov      r3, r6
003c66f8: mov      r1, #6
003c66fc: movw     r2, #0xc355
003c6700: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c6704: b        #0x3c1938
003c6708: ldr      r0, [r4, #4]
003c670c: bl       #0x3a53e0
003c6710: b        #0x3c66c4
003c6714: ldrsheq  lr, [ip], #-0x34
003c6718: strdeq   r3, r4, [r0], -r4
003c671c: subeq    lr, pc, ip, lsl #10
003c6720: subeq    lr, pc, r4, lsl r5

# _ZN10AISDefault11OnAnimEventEPKc
003dca50: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dca54: sub      sp, sp, #0x3c
003dca58: add      r5, sp, #0x30
003dca5c: mov      r6, r0
003dca60: mov      r7, r1
003dca64: mov      r0, r5
003dca68: bl       #0x3192b4
003dca6c: mov      r0, r5
003dca70: mov      r1, r7
003dca74: bl       #0x39ec10
003dca78: ldr      r3, [r6, #0x98]
003dca7c: mov      r0, r5
003dca80: ldr      r4, [pc, #0x1d0]
003dca84: ldr      r1, [r3, #0x4f4]
003dca88: bl       #0x3cdd78
003dca8c: ldr      r1, [pc, #0x1c8]
003dca90: mov      r0, r6
003dca94: mov      r2, r5
003dca98: add      r1, pc, r1
003dca9c: bl       #0x37c41c
003dcaa0: ldr      r1, [pc, #0x1b8]
003dcaa4: mov      r0, r7
003dcaa8: add      r4, pc, r4
003dcaac: add      r1, pc, r1
003dcab0: bl       #0x30e31c
003dcab4: cmp      r0, #0
003dcab8: beq      #0x3dcad4
003dcabc: ldr      r1, [pc, #0x1a0]
003dcac0: mov      r0, r7
003dcac4: add      r1, pc, r1
003dcac8: bl       #0x30e31c
003dcacc: cmp      r0, #0
003dcad0: bne      #0x3dcc0c
003dcad4: ldr      r1, [pc, #0x18c]
003dcad8: mov      r3, #0
003dcadc: mov      r0, r7
003dcae0: add      r1, pc, r1
003dcae4: str      r3, [sp, #0x2c]
003dcae8: str      r3, [sp, #0x24]
003dcaec: str      r3, [sp, #0x28]
003dcaf0: bl       #0x30e31c
003dcaf4: cmp      r0, #0
003dcaf8: beq      #0x3dcc1c
003dcafc: add      r0, sp, #0xc
003dcb00: ldr      r1, [r6, #0x98]
003dcb04: bl       #0x3a57f4
003dcb08: ldr      r3, [sp, #0xc]
003dcb0c: str      r3, [sp, #0x24]
003dcb10: ldr      r3, [sp, #0x10]
003dcb14: str      r3, [sp, #0x28]
003dcb18: ldr      r3, [sp, #0x14]
003dcb1c: str      r3, [sp, #0x2c]
003dcb20: ldr      r0, [r6, #0x98]
003dcb24: bl       #0x3a3300
003dcb28: ldr      r7, [pc, #0x13c]
003dcb2c: mov      ip, #0
003dcb30: add      fp, sp, #0x24
003dcb34: mov      r1, r0
003dcb38: mov      r3, ip
003dcb3c: mov      r2, fp
003dcb40: ldr      r0, [r4, r7]
003dcb44: str      ip, [sp]
003dcb48: bl       #0x495d14
003dcb4c: ldr      r2, [r6, #0x98]
003dcb50: movw     r3, #0x1014
003dcb54: ldr      r3, [r2, r3]
003dcb58: cmp      r3, #0
003dcb5c: blt      #0x3dcc44
003dcb60: ldr      r1, [pc, #0x108]
003dcb64: ldr      r1, [r4, r1]
003dcb68: ldr      r1, [r1]
003dcb6c: cmp      r3, r1
003dcb70: bge      #0x3dcc44
003dcb74: ldr      r1, [pc, #0xf8]
003dcb78: mov      r0, #0x18
003dcb7c: ldr      r1, [r4, r1]
003dcb80: ldr      r1, [r1]
003dcb84: mla      r3, r0, r3, r1
003dcb88: ldrb     r3, [r3, #0x14]
003dcb8c: ldr      sl, [r2, #0x1d8]
003dcb90: cmp      sl, #0
003dcb94: ldrne    sl, [sl, #0x3c]
003dcb98: cmp      r3, #0
003dcb9c: beq      #0x3dcc0c
003dcba0: cmp      sl, #0
003dcba4: beq      #0x3dcc0c
003dcba8: ldr      r3, [pc, #0xc8]
003dcbac: ldr      r3, [r4, r3]
003dcbb0: ldr      sb, [r3]
003dcbb4: cmp      sb, #0
003dcbb8: ble      #0x3dcc0c
003dcbbc: ldr      r3, [pc, #0xb8]
003dcbc0: mov      r8, #0
003dcbc4: ldr      r3, [r4, r3]
003dcbc8: ldr      r6, [r3]
003dcbcc: b        #0x3dcbdc
003dcbd0: cmp      r8, sb
003dcbd4: add      r6, r6, #0x20
003dcbd8: beq      #0x3dcc0c
003dcbdc: ldr      r0, [r6, #0xc]
003dcbe0: mov      r1, sl
003dcbe4: bl       #0x30e31c
003dcbe8: subs     ip, r0, #0
003dcbec: add      r8, r8, #1
003dcbf0: bne      #0x3dcbd0
003dcbf4: ldr      r1, [r6, #4]
003dcbf8: ldr      r0, [r4, r7]
003dcbfc: mov      r2, fp
003dcc00: mov      r3, ip
003dcc04: str      ip, [sp]
003dcc08: bl       #0x495d14
003dcc0c: mov      r0, r5
003dcc10: bl       #0x319228
003dcc14: add      sp, sp, #0x3c
003dcc18: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003dcc1c: add      r0, sp, #0x18
003dcc20: ldr      r1, [r6, #0x98]
003dcc24: bl       #0x3a5874
003dcc28: ldr      r3, [sp, #0x18]
003dcc2c: str      r3, [sp, #0x24]
003dcc30: ldr      r3, [sp, #0x1c]
003dcc34: str      r3, [sp, #0x28]
003dcc38: ldr      r3, [sp, #0x20]
003dcc3c: str      r3, [sp, #0x2c]
003dcc40: b        #0x3dcb20
003dcc44: ldr      r3, [pc, #0x28]
003dcc48: ldr      r3, [r4, r3]
003dcc4c: ldr      r3, [r3]
003dcc50: ldrb     r3, [r3, #0x14]
003dcc54: b        #0x3dcb8c
003dcc58: subseq   r7, fp, r8, ror #31
003dcc5c: umaaleq  r6, lr, r8, r4
003dcc60: ldrdeq   r8, sb, [lr], #-0xe4
003dcc64: subeq    r8, lr, ip, asr #29
003dcc68: subeq    r8, lr, r0, lsr #29
003dcc6c: andeq    r1, r0, r8, lsl #22
003dcc70: andeq    r1, r0, ip, ror #3
003dcc74: andeq    r0, r0, r4, asr #30
003dcc78: andeq    r1, r0, ip, lsr #23
003dcc7c: strheq   r3, [r0], -r4
