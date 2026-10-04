
# _ZN13ItemInventory14_EquipItemAutoEj
004009f8: push     {r4, r5, r6, r7, r8, lr}
004009fc: mov      r4, r0
00400a00: ldr      r2, [r0, #8]
00400a04: ldr      r0, [r0, #0xc]
00400a08: ldr      r3, [pc, #0x25c]
00400a0c: sub      sp, sp, #8
00400a10: rsb      r0, r2, r0
00400a14: cmp      r1, r0, asr #2
00400a18: mov      r5, r1
00400a1c: add      r3, pc, r3
00400a20: blo      #0x400a48
00400a24: ldr      r1, [pc, #0x244]
00400a28: ldr      r1, [r3, r1]
00400a2c: ldr      r1, [r1]
00400a30: cmp      r1, #2
00400a34: moveq    r3, #0
00400a38: streq    r3, [r3]
00400a3c: beq      #0x400a48
00400a40: cmp      r1, #1
00400a44: beq      #0x400b5c
00400a48: ldr      r7, [r2, r5, lsl #2]
00400a4c: ldr      r0, [r7]
00400a50: bl       #0x3f9e68
00400a54: cmp      r0, #0
00400a58: bne      #0x400a68
00400a5c: mov      r0, #0
00400a60: add      sp, sp, #8
00400a64: pop      {r4, r5, r6, r7, r8, pc}
00400a68: ldr      r0, [r7]
00400a6c: ldr      r8, [r4, #4]
00400a70: bl       #0x3f9e08
00400a74: ldr      r6, [r0, #0x68]
00400a78: ldr      r0, [r7]
00400a7c: bl       #0x3f9e08
00400a80: ldr      r3, [r0, #0x58]
00400a84: cmp      r3, #5
00400a88: beq      #0x400ab0
00400a8c: ldr      r0, [r7]
00400a90: bl       #0x3f9e08
00400a94: ldr      r3, [r0, #0x58]
00400a98: cmp      r3, #4
00400a9c: beq      #0x400ab0
00400aa0: cmp      r6, #1
00400aa4: beq      #0x400b0c
00400aa8: cmn      r6, #4
00400aac: beq      #0x400b94
00400ab0: cmp      r6, #0
00400ab4: blt      #0x400af0
00400ab8: ldr      r3, [r4, #0x14]
00400abc: ldm      r3, {r2, r3}
00400ac0: rsb      r3, r2, r3
00400ac4: cmp      r6, r3, asr #2
00400ac8: bge      #0x400af0
00400acc: cmp      r6, #2
00400ad0: beq      #0x400c14
00400ad4: mov      r0, r4
00400ad8: mov      r1, r6
00400adc: mov      r2, r5
00400ae0: mov      r3, #0
00400ae4: bl       #0x400634
00400ae8: mov      r0, #1
00400aec: b        #0x400a60
00400af0: cmn      r6, #3
00400af4: beq      #0x400b1c
00400af8: cmn      r6, #2
00400afc: beq      #0x400bd4
00400b00: cmn      r6, #4
00400b04: bne      #0x400a5c
00400b08: b        #0x400ba8
00400b0c: movw     r3, #0x1320
00400b10: ldr      r3, [r8, r3]
00400b14: cmp      r3, #0
00400b18: beq      #0x400ab8
00400b1c: mov      r0, r4
00400b20: mov      r1, #1
00400b24: bl       #0x4002d8
00400b28: subs     r3, r0, #0
00400b2c: beq      #0x400c3c
00400b30: mov      r0, r4
00400b34: mov      r1, #2
00400b38: bl       #0x4002d8
00400b3c: subs     r3, r0, #0
00400b40: bne      #0x400a5c
00400b44: mov      r0, r4
00400b48: mov      r2, r5
00400b4c: mov      r1, #2
00400b50: bl       #0x400634
00400b54: mov      r0, #1
00400b58: b        #0x400a60
00400b5c: ldr      r0, [pc, #0x110]
00400b60: ldr      r1, [pc, #0x110]
00400b64: ldr      r2, [pc, #0x110]
00400b68: ldr      r0, [r3, r0]
00400b6c: ldr      r3, [pc, #0x10c]
00400b70: add      r2, pc, r2
00400b74: mov      ip, #0x23
00400b78: add      r1, pc, r1
00400b7c: add      r0, r0, #0xa8
00400b80: add      r3, pc, r3
00400b84: str      ip, [sp]
00400b88: bl       #0x30e004
00400b8c: ldr      r2, [r4, #8]
00400b90: b        #0x400a48
00400b94: movw     r3, #0x1324
00400b98: ldr      r3, [r8, r3]
00400b9c: cmp      r3, #0
00400ba0: movne    r6, #1
00400ba4: bne      #0x400ab8
00400ba8: mov      r1, #2
00400bac: mvn      r2, #0
00400bb0: mov      r0, r4
00400bb4: bl       #0x4003a4
00400bb8: mov      r0, r4
00400bbc: mov      r2, r5
00400bc0: mov      r1, #1
00400bc4: mov      r3, #0
00400bc8: bl       #0x400634
00400bcc: mov      r0, #1
00400bd0: b        #0x400a60
00400bd4: mov      r0, r4
00400bd8: mov      r1, #5
00400bdc: bl       #0x4002d8
00400be0: subs     r3, r0, #0
00400be4: beq      #0x400c54
00400be8: mov      r0, r4
00400bec: mov      r1, #6
00400bf0: bl       #0x4002d8
00400bf4: subs     r3, r0, #0
00400bf8: bne      #0x400a5c
00400bfc: mov      r0, r4
00400c00: mov      r2, r5
00400c04: mov      r1, #6
00400c08: bl       #0x400634
00400c0c: mov      r0, #1
00400c10: b        #0x400a60
00400c14: mov      r0, r4
00400c18: mov      r1, #0
00400c1c: bl       #0x4001a0
00400c20: cmp      r0, #0
00400c24: beq      #0x400ad4
00400c28: mov      r0, r4
00400c2c: mov      r1, #1
00400c30: mvn      r2, #0
00400c34: bl       #0x4003a4
00400c38: b        #0x400ad4
00400c3c: mov      r0, r4
00400c40: mov      r2, r5
00400c44: mov      r1, #1
00400c48: bl       #0x400634
00400c4c: mov      r0, #1
00400c50: b        #0x400a60
00400c54: mov      r0, r4
00400c58: mov      r2, r5
00400c5c: mov      r1, #5
00400c60: bl       #0x400634
00400c64: mov      r0, #1
00400c68: b        #0x400a60
00400c6c: subseq   r4, sb, r4, ror r0
00400c70: andeq    r3, r0, r0, asr #19
00400c74: andeq    r1, r0, r0, asr #19
00400c78: subeq    sp, fp, r0, ror #16
00400c7c: subeq    r6, ip, r8, lsl r8
00400c80: subeq    r6, ip, r8, asr sb

# _ZNK12ItemInstance7GetItemEv
003f9e08: ldr      r3, [pc, #0x1c]
003f9e0c: ldr      r1, [pc, #0x1c]
003f9e10: ldr      r2, [r0, #4]
003f9e14: add      r3, pc, r3
003f9e18: ldr      r1, [r3, r1]
003f9e1c: mov      r0, #0xa4
003f9e20: ldr      r3, [r1]
003f9e24: mla      r0, r0, r2, r3
003f9e28: bx       lr
003f9e2c: subseq   sl, sb, ip, ror ip
003f9e30: andeq    r2, r0, ip, ror #16
