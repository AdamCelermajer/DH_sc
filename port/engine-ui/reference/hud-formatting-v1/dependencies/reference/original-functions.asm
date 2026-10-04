
# _ZN17CharAISkillScript7GetInfoEjPf
003daca8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003dacac: ldr      r4, [pc, #0x1b8]
003dacb0: ldr      r8, [pc, #0x1b8]
003dacb4: sub      sp, sp, #0x40
003dacb8: add      r4, pc, r4
003dacbc: ldr      r3, [r4, r8]
003dacc0: mov      r7, r0
003dacc4: add      r5, sp, #0x14
003dacc8: ldr      r3, [r3]
003daccc: mov      r0, sp
003dacd0: mov      sb, r1
003dacd4: str      r3, [sp, #0x3c]
003dacd8: mov      sl, r2
003dacdc: bl       #0x3192b4
003dace0: mov      r0, r5
003dace4: bl       #0x31b434
003dace8: ldr      r3, [r7, #4]
003dacec: mov      r6, sp
003dacf0: ldr      r0, [r3, #0x3e4]
003dacf4: cmp      r0, #0
003dacf8: beq      #0x3dae58
003dacfc: ldr      r1, [pc, #0x170]
003dad00: mov      r3, r5
003dad04: add      r2, r7, #0xc
003dad08: add      r1, pc, r1
003dad0c: bl       #0x37c390
003dad10: ldr      r3, [sp, #0x1c]
003dad14: cmp      r3, #0
003dad18: beq      #0x3dad48
003dad1c: mov      r0, r5
003dad20: bl       #0x31b398
003dad24: mov      r0, sp
003dad28: bl       #0x319228
003dad2c: ldr      r3, [r4, r8]
003dad30: ldr      r2, [sp, #0x3c]
003dad34: ldr      r3, [r3]
003dad38: cmp      r2, r3
003dad3c: bne      #0x3dae68
003dad40: add      sp, sp, #0x40
003dad44: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dad48: mov      r1, sb
003dad4c: mov      r0, sp
003dad50: bl       #0x3cdd78
003dad54: ldr      r0, [sp, #0x38]
003dad58: ldm      r0, {r1, r2}
003dad5c: cmp      r1, r2
003dad60: beq      #0x3dad6c
003dad64: add      r3, sp, #0x10
003dad68: bl       #0x31c3cc
003dad6c: ldr      r3, [r7, #4]
003dad70: ldr      r1, [pc, #0x100]
003dad74: mov      r2, sp
003dad78: ldr      r0, [r3, #0x3e4]
003dad7c: add      r1, pc, r1
003dad80: mov      r3, r5
003dad84: bl       #0x37c390
003dad88: ldr      sb, [sp, #0x1c]
003dad8c: cmp      sb, #0
003dad90: bne      #0x3dad1c
003dad94: cmp      sl, #0
003dad98: beq      #0x3dad1c
003dad9c: ldr      r2, [sp, #0x38]
003dada0: mov      r3, #0
003dada4: str      r3, [sl]
003dada8: ldr      r3, [r2]
003dadac: ldr      r2, [r2, #4]
003dadb0: rsb      r3, r3, r2
003dadb4: asr      r3, r3, #4
003dadb8: add      r2, r3, r3, lsl #3
003dadbc: add      r2, r2, r2, lsl #6
003dadc0: add      r2, r3, r2, lsl #3
003dadc4: add      r2, r2, r2, lsl #15
003dadc8: add      r3, r3, r2, lsl #3
003dadcc: cmp      r3, #0
003dadd0: beq      #0x3dad1c
003dadd4: mov      r0, r5
003dadd8: mov      r1, sb
003daddc: bl       #0x3da43c
003dade0: ldr      r3, [r0, #4]
003dade4: cmp      r3, #3
003dade8: bne      #0x3dad1c
003dadec: mov      r1, sb
003dadf0: mov      r0, r5
003dadf4: bl       #0x3da43c
003dadf8: bl       #0x31bbf0
003dadfc: bl       #0x30e4cc
003dae00: ldr      r7, [r7, #4]
003dae04: mov      r1, r0
003dae08: add      r2, sp, #0xc
003dae0c: add      r7, r7, #0x3b4
003dae10: mov      r0, r7
003dae14: add      r3, sp, #8
003dae18: bl       #0x3db344
003dae1c: cmp      r0, #0
003dae20: beq      #0x3dad1c
003dae24: ldr      r0, [sp, #0xc]
003dae28: bl       #0x30e2e0
003dae2c: mov      r7, r0
003dae30: ldr      r0, [sp, #8]
003dae34: bl       #0x30e2e0
003dae38: mov      r1, r0
003dae3c: mov      r0, r7
003dae40: bl       #0x30ec94
003dae44: mov      r1, r0
003dae48: mov      r0, #0x3f800000
003dae4c: bl       #0x30e3ac
003dae50: str      r0, [sl]
003dae54: b        #0x3dad1c
003dae58: cmp      sl, #0
003dae5c: movne    r3, #0
003dae60: strne    r3, [sl]
003dae64: b        #0x3dad1c
003dae68: bl       #0x30e310
003dae6c: ldrsbeq  sb, [fp], #-0xd8
003dae70: andeq    r4, r0, ip, lsr #1
003dae74: subeq    sl, lr, r8, asr #22
003dae78: subeq    sl, lr, ip, lsr fp

# _ZNK13StringManager12getStringIdxEiij
005088c4: push     {r4, r5, r6, r7, r8, sl, lr}
005088c8: ldr      r6, [pc, #0x33c]
005088cc: cmn      r3, #1
005088d0: mov      r4, r3
005088d4: add      r6, pc, r6
005088d8: sub      sp, sp, #0xc
005088dc: mov      r7, r0
005088e0: mov      r5, r1
005088e4: mov      r8, r2
005088e8: moveq    r4, #0
005088ec: beq      #0x50891c
005088f0: cmp      r4, #8
005088f4: bls      #0x50891c
005088f8: ldr      r3, [pc, #0x310]
005088fc: ldr      r3, [r6, r3]
00508900: ldr      r3, [r3]
00508904: cmp      r3, #2
00508908: moveq    r3, #0
0050890c: streq    r3, [r3]
00508910: beq      #0x50891c
00508914: cmp      r3, #1
00508918: beq      #0x508b6c
0050891c: cmp      r5, #0
00508920: blt      #0x508a6c
00508924: cmp      r5, #0x24
00508928: ble      #0x508948
0050892c: ldr      r3, [pc, #0x2dc]
00508930: ldr      r3, [r6, r3]
00508934: ldr      r3, [r3]
00508938: cmp      r3, #2
0050893c: beq      #0x508a80
00508940: cmp      r3, #1
00508944: beq      #0x508bd8
00508948: mov      r0, r7
0050894c: mov      r1, r4
00508950: mov      r2, r5
00508954: bl       #0x507668
00508958: subs     r3, r0, #0
0050895c: beq      #0x508aa0
00508960: mov      r3, #0x25
00508964: mla      r3, r3, r4, r5
00508968: add      r3, r3, #2
0050896c: ldr      r3, [r7, r3, lsl #2]
00508970: cmp      r3, #0
00508974: beq      #0x508ac8
00508978: mov      sl, #0x25
0050897c: mla      sl, sl, r4, r5
00508980: add      sl, sl, #0x29c
00508984: add      sl, r7, sl, lsl #1
00508988: ldrh     r3, [sl, #4]
0050898c: cmp      r3, #0
00508990: bne      #0x5089b4
00508994: ldr      r2, [pc, #0x274]
00508998: ldr      r2, [r6, r2]
0050899c: ldr      r2, [r2]
005089a0: cmp      r2, #2
005089a4: streq    r3, [r3]
005089a8: beq      #0x5089b4
005089ac: cmp      r2, #1
005089b0: beq      #0x508ba0
005089b4: sxth     r3, r3
005089b8: cmp      r3, r8
005089bc: bgt      #0x508a38
005089c0: ldr      r3, [pc, #0x248]
005089c4: ldr      r3, [r6, r3]
005089c8: ldr      r3, [r3]
005089cc: cmp      r3, #2
005089d0: beq      #0x508b58
005089d4: cmp      r3, #1
005089d8: beq      #0x5089ec
005089dc: ldr      r0, [pc, #0x230]
005089e0: add      r0, pc, r0
005089e4: add      sp, sp, #0xc
005089e8: pop      {r4, r5, r6, r7, r8, sl, pc}
005089ec: ldr      r0, [pc, #0x224]
005089f0: ldr      r1, [pc, #0x224]
005089f4: ldr      r2, [pc, #0x224]
005089f8: ldr      r0, [r6, r0]
005089fc: ldr      r3, [pc, #0x220]
00508a00: mov      ip, #0x1f8
00508a04: add      r1, pc, r1
00508a08: add      r3, pc, r3
00508a0c: add      r0, r0, #0xa8
00508a10: add      r2, pc, r2
00508a14: str      ip, [sp]
00508a18: bl       #0x30e004
00508a1c: mov      r3, #0x25
00508a20: mla      r3, r3, r4, r5
00508a24: add      r3, r3, #0x29c
00508a28: add      r3, r7, r3, lsl #1
00508a2c: ldrsh    r3, [r3, #4]
00508a30: cmp      r8, r3
00508a34: bge      #0x5089dc
00508a38: mov      r3, #0x25
00508a3c: mla      r4, r3, r4, r5
00508a40: add      r4, r4, #2
00508a44: ldr      r3, [r7, r4, lsl #2]
00508a48: ldr      r0, [r3, r8, lsl #2]
00508a4c: cmp      r0, #0
00508a50: beq      #0x508a60
00508a54: ldrsb    r3, [r0]
00508a58: cmp      r3, #0
00508a5c: bne      #0x5089e4
00508a60: ldr      r0, [pc, #0x1c0]
00508a64: add      r0, pc, r0
00508a68: b        #0x5089e4
00508a6c: ldr      r3, [pc, #0x19c]
00508a70: ldr      r3, [r6, r3]
00508a74: ldr      r3, [r3]
00508a78: cmp      r3, #2
00508a7c: bne      #0x508b1c
00508a80: mov      r3, #0
00508a84: str      r3, [r3]
00508a88: mov      r0, r7
00508a8c: mov      r1, r4
00508a90: mov      r2, r5
00508a94: bl       #0x507668
00508a98: subs     r3, r0, #0
00508a9c: bne      #0x508960
00508aa0: mov      r0, r7
00508aa4: mov      r1, r4
00508aa8: mov      r2, r5
00508aac: bl       #0x50851c
00508ab0: mov      r3, #0x25
00508ab4: mla      r3, r3, r4, r5
00508ab8: add      r3, r3, #2
00508abc: ldr      r3, [r7, r3, lsl #2]
00508ac0: cmp      r3, #0
00508ac4: bne      #0x508978
00508ac8: ldr      r2, [pc, #0x140]
00508acc: ldr      r2, [r6, r2]
00508ad0: ldr      r2, [r2]
00508ad4: cmp      r2, #2
00508ad8: streq    r3, [r3]
00508adc: beq      #0x508978
00508ae0: cmp      r2, #1
00508ae4: bne      #0x508978
00508ae8: ldr      r0, [pc, #0x128]
00508aec: ldr      r1, [pc, #0x138]
00508af0: ldr      r2, [pc, #0x138]
00508af4: ldr      r0, [r6, r0]
00508af8: ldr      r3, [pc, #0x134]
00508afc: movw     ip, #0x1f5
00508b00: add      r1, pc, r1
00508b04: add      r2, pc, r2
00508b08: add      r3, pc, r3
00508b0c: add      r0, r0, #0xa8
00508b10: str      ip, [sp]
00508b14: bl       #0x30e004
00508b18: b        #0x508978
00508b1c: cmp      r3, #1
00508b20: bne      #0x508948
00508b24: ldr      r0, [pc, #0xec]
00508b28: ldr      r1, [pc, #0x108]
00508b2c: ldr      r2, [pc, #0x108]
00508b30: ldr      r0, [r6, r0]
00508b34: ldr      r3, [pc, #0x104]
00508b38: movw     ip, #0x1ea
00508b3c: add      r1, pc, r1
00508b40: add      r2, pc, r2
00508b44: add      r3, pc, r3
00508b48: add      r0, r0, #0xa8
00508b4c: str      ip, [sp]
00508b50: bl       #0x30e004
00508b54: b        #0x508948
00508b58: ldr      r0, [pc, #0xe4]
00508b5c: mov      r3, #0
00508b60: str      r3, [r3]
00508b64: add      r0, pc, r0
00508b68: b        #0x5089e4
00508b6c: ldr      r0, [pc, #0xa4]
00508b70: ldr      r1, [pc, #0xd0]
00508b74: ldr      r2, [pc, #0xd0]
00508b78: ldr      r0, [r6, r0]
00508b7c: ldr      r3, [pc, #0xcc]
00508b80: movw     ip, #0x1e9
00508b84: add      r1, pc, r1
00508b88: add      r2, pc, r2
00508b8c: add      r3, pc, r3
00508b90: add      r0, r0, #0xa8
00508b94: str      ip, [sp]
00508b98: bl       #0x30e004
00508b9c: b        #0x50891c
00508ba0: ldr      r0, [pc, #0x70]
00508ba4: ldr      r1, [pc, #0xa8]
00508ba8: ldr      r2, [pc, #0xa8]
00508bac: ldr      r0, [r6, r0]
00508bb0: ldr      r3, [pc, #0xa4]
00508bb4: movw     ip, #0x1f7
00508bb8: add      r1, pc, r1
00508bbc: add      r3, pc, r3
00508bc0: add      r0, r0, #0xa8
00508bc4: add      r2, pc, r2
00508bc8: str      ip, [sp]
00508bcc: bl       #0x30e004
00508bd0: ldrh     r3, [sl, #4]
00508bd4: b        #0x5089b4
00508bd8: ldr      r0, [pc, #0x38]
00508bdc: ldr      r1, [pc, #0x7c]
00508be0: ldr      r2, [pc, #0x7c]
00508be4: ldr      r0, [r6, r0]
00508be8: ldr      r3, [pc, #0x78]
00508bec: movw     ip, #0x1eb
00508bf0: add      r1, pc, r1
00508bf4: add      r2, pc, r2
00508bf8: add      r3, pc, r3
00508bfc: add      r0, r0, #0xa8
00508c00: str      ip, [sp]
00508c04: bl       #0x30e004
00508c08: b        #0x508948
00508c0c: strheq   ip, [r8], #-0x1c
00508c10: andeq    r3, r0, r0, asr #19
00508c14: eorseq   r3, sp, r0, lsr r3
00508c18: andeq    r1, r0, r0, asr #19
00508c1c: ldrsbteq r5, [fp], -r4
00508c20: eorseq   r3, sp, r8, lsl #6
00508c24: eorseq   r3, sp, r0, asr #2
00508c28: eorseq   r3, sp, r4, ror #5
00508c2c: ldrsbteq r5, [fp], -r8
00508c30: eorseq   r3, sp, ip, asr #3
00508c34: eorseq   r3, sp, r0, asr #32
00508c38: mlaseq   fp, ip, r8, r5
00508c3c: eorseq   r3, sp, r0, ror #2
00508c40: eorseq   r3, sp, r4
00508c44: eorseq   r3, sp, ip, lsr #3
00508c48: eorseq   r5, fp, r4, asr r8
00508c4c: ldrshteq r3, [sp], -r8
00508c50: ldrhteq  r2, [sp], -ip
00508c54: eorseq   r5, fp, r0, lsr #16
00508c58: eorseq   r3, sp, ip, lsr #2
00508c5c: eorseq   r2, sp, ip, lsl #31
00508c60: eorseq   r5, fp, r8, ror #15
00508c64: ldrhteq  r3, [sp], -ip
00508c68: eorseq   r2, sp, r0, asr pc
