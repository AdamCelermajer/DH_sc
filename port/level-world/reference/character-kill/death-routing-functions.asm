
# _ZN6CharAI13_SkillCleanUpEv
003d8ae0: push     {r4, r5, r6, lr}
003d8ae4: ldr      r3, [r0, #0xb4]
003d8ae8: ldr      r6, [r0, #0xb8]
003d8aec: mov      r5, r0
003d8af0: rsb      r6, r3, r6
003d8af4: asrs     r6, r6, #2
003d8af8: beq      #0x3d8b24
003d8afc: mov      r4, #0
003d8b00: b        #0x3d8b08
003d8b04: ldr      r3, [r5, #0xb4]
003d8b08: ldr      r0, [r3, r4, lsl #2]
003d8b0c: add      r4, r4, #1
003d8b10: cmp      r0, #0
003d8b14: beq      #0x3d8b1c
003d8b18: bl       #0x3daafc
003d8b1c: cmp      r4, r6
003d8b20: bne      #0x3d8b04
003d8b24: pop      {r4, r5, r6, pc}

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

# _ZN6CharAI13_SpellCleanUpEv
003d8a98: push     {r4, r5, r6, lr}
003d8a9c: ldr      r3, [r0, #0xc0]
003d8aa0: ldr      r6, [r0, #0xc4]
003d8aa4: mov      r5, r0
003d8aa8: rsb      r6, r3, r6
003d8aac: asrs     r6, r6, #2
003d8ab0: beq      #0x3d8adc
003d8ab4: mov      r4, #0
003d8ab8: b        #0x3d8ac0
003d8abc: ldr      r3, [r5, #0xc0]
003d8ac0: ldr      r0, [r3, r4, lsl #2]
003d8ac4: add      r4, r4, #1
003d8ac8: cmp      r0, #0
003d8acc: beq      #0x3d8ad4
003d8ad0: bl       #0x3daafc
003d8ad4: cmp      r4, r6
003d8ad8: bne      #0x3d8abc
003d8adc: pop      {r4, r5, r6, pc}

# _ZN6CharAI10AI_SetDeadEv
003d6cdc: mov      r1, #0
003d6ce0: push     {r4, lr}
003d6ce4: mov      r2, r1
003d6ce8: mov      r4, r0
003d6cec: bl       #0x3d6890
003d6cf0: mov      r0, r4
003d6cf4: bl       #0x3d49c4
003d6cf8: ldr      r0, [r4, #4]
003d6cfc: mov      r1, #0
003d6d00: mov      r2, r1
003d6d04: add      r0, r0, #0x4f0
003d6d08: mov      r3, #1
003d6d0c: add      r0, r0, #0xc
003d6d10: bl       #0x3c58c8
003d6d14: ldr      r0, [r4, #4]
003d6d18: ldr      r1, [r4, #0x10]
003d6d1c: add      r0, r0, #0x3b4
003d6d20: bl       #0x3db2d8
003d6d24: ldr      r0, [r4, #4]
003d6d28: ldr      r1, [r4, #0x14]
003d6d2c: add      r0, r0, #0x3b4
003d6d30: bl       #0x3db2d8
003d6d34: mvn      r3, #0
003d6d38: str      r3, [r4, #0x14]
003d6d3c: str      r3, [r4, #0x10]
003d6d40: mov      r0, r4
003d6d44: bl       #0x3d5fa8
003d6d48: mov      r0, r4
003d6d4c: mov      r1, #0
003d6d50: bl       #0x3d6abc
003d6d54: mov      r0, r4
003d6d58: bl       #0x3d8ae0
003d6d5c: mov      r0, r4
003d6d60: pop      {r4, lr}
003d6d64: b        #0x3d8a98

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZN12v2Controller8Cmd_KillEP10GameObjectb
0040570c: push     {r4, lr}
00405710: ldr      r3, [r0, #4]
00405714: mov      r0, r3
00405718: ldr      r3, [r3]
0040571c: mov      lr, pc
00405720: ldr      pc, [r3, #0x58]
00405724: pop      {r4, pc}

# _ZN6CharAI6OnDiedEP10GameObject
003d1000: push     {r4, r5, r6, lr}
003d1004: mov      r4, r0
003d1008: ldr      r0, [r0, #0x34]
003d100c: mov      r5, r1
003d1010: cmp      r0, #0
003d1014: beq      #0x3d1024
003d1018: ldr      r1, [r4, #4]
003d101c: mov      r2, r5
003d1020: bl       #0x3d2628
003d1024: ldr      r3, [r4, #0x1c]
003d1028: cmp      r3, #0
003d102c: beq      #0x3d1044
003d1030: mov      r0, r3
003d1034: mov      r1, r5
003d1038: ldr      r3, [r3]
003d103c: mov      lr, pc
003d1040: ldr      pc, [r3, #0x24]
003d1044: mov      r0, r4
003d1048: pop      {r4, r5, r6, lr}
003d104c: b        #0x3d6cdc

# _ZN10CharTimers8TMR_StopEj
003db2d8: ldr      r3, [r0, #8]
003db2dc: ldr      r2, [r0, #0xc]
003db2e0: rsb      r2, r3, r2
003db2e4: cmp      r1, r2, asr #5
003db2e8: addlo    r3, r3, r1, lsl #5
003db2ec: movlo    r2, #0
003db2f0: strblo   r2, [r3, #0x14]
003db2f4: bx       lr

# _ZN6CharAI16AI_ClearAllAggroEv
003d5fa8: push     {r4, r5, r6, r7, r8, sl, lr}
003d5fac: mov      r3, #0
003d5fb0: sub      sp, sp, #0x14
003d5fb4: mov      r5, r0
003d5fb8: ldr      r1, [r5, #0x8c]
003d5fbc: mov      r0, sp
003d5fc0: str      r3, [sp, #8]
003d5fc4: str      r3, [sp]
003d5fc8: str      r3, [sp, #4]
003d5fcc: ldr      r4, [r5, #0x84]
003d5fd0: bl       #0x3d5e14
003d5fd4: mov      sl, sp
003d5fd8: add      r6, r5, #0x7c
003d5fdc: add      r7, r5, #4
003d5fe0: add      r8, sp, #0xc
003d5fe4: cmp      r4, r6
003d5fe8: beq      #0x3d60a4
003d5fec: ldr      r0, [r4, #0x10]
003d5ff0: ldr      r3, [r0, #0x460]
003d5ff4: cmp      r3, #0
003d5ff8: beq      #0x3d6058
003d5ffc: add      r0, r0, #0x450
003d6000: add      r0, r0, #0xc
003d6004: ldr      ip, [r7]
003d6008: mov      r1, r0
003d600c: b        #0x3d6014
003d6010: mov      r3, r2
003d6014: ldr      r2, [r3, #0x10]
003d6018: cmp      r2, ip
003d601c: ldrlo    r2, [r3, #0xc]
003d6020: ldrhs    r2, [r3, #8]
003d6024: movlo    r3, r1
003d6028: mov      r1, r3
003d602c: cmp      r2, #0
003d6030: bne      #0x3d6010
003d6034: cmp      r0, r3
003d6038: beq      #0x3d6058
003d603c: ldr      r1, [r5, #4]
003d6040: ldr      r2, [r3, #0x10]
003d6044: cmp      r1, r2
003d6048: blo      #0x3d6058
003d604c: mov      r1, r8
003d6050: str      r3, [sp, #0xc]
003d6054: bl       #0x3d5d9c
003d6058: ldmib    sp, {r1, r3}
003d605c: cmp      r1, r3
003d6060: beq      #0x3d614c
003d6064: ldr      r3, [r4, #0x10]
003d6068: str      r3, [r1]
003d606c: ldr      r3, [sp, #4]
003d6070: add      r3, r3, #4
003d6074: str      r3, [sp, #4]
003d6078: ldr      r2, [r4, #0xc]
003d607c: cmp      r2, #0
003d6080: bne      #0x3d608c
003d6084: b        #0x3d6118
003d6088: mov      r2, r3
003d608c: ldr      r3, [r2, #8]
003d6090: cmp      r3, #0
003d6094: bne      #0x3d6088
003d6098: mov      r4, r2
003d609c: cmp      r4, r6
003d60a0: bne      #0x3d5fec
003d60a4: ldr      r3, [r5, #0x8c]
003d60a8: cmp      r3, #0
003d60ac: bne      #0x3d615c
003d60b0: ldm      sp, {r0, r3}
003d60b4: rsb      r3, r0, r3
003d60b8: lsrs     r3, r3, #2
003d60bc: beq      #0x3d60f0
003d60c0: mov      r4, #0
003d60c4: ldr      r3, [r0, r4, lsl #2]
003d60c8: ldr      r1, [r5, #4]
003d60cc: add      r4, r4, #1
003d60d0: add      r0, r3, #0x3c8
003d60d4: ldr      r3, [r3, #0x3c8]
003d60d8: mov      lr, pc
003d60dc: ldr      pc, [r3, #0x3c]
003d60e0: ldm      sp, {r0, r3}
003d60e4: rsb      r3, r0, r3
003d60e8: cmp      r4, r3, asr #2
003d60ec: blo      #0x3d60c4
003d60f0: cmp      r0, #0
003d60f4: beq      #0x3d6110
003d60f8: ldr      r1, [sp, #8]
003d60fc: rsb      r1, r0, r1
003d6100: bic      r1, r1, #3
003d6104: cmp      r1, #0x80
003d6108: bhi      #0x3d6180
003d610c: bl       #0x708f00
003d6110: add      sp, sp, #0x14
003d6114: pop      {r4, r5, r6, r7, r8, sl, pc}
003d6118: ldr      r3, [r4, #4]
003d611c: ldr      r1, [r3, #0xc]
003d6120: cmp      r4, r1
003d6124: bne      #0x3d6140
003d6128: mov      r4, r3
003d612c: ldr      r3, [r3, #4]
003d6130: ldr      r2, [r3, #0xc]
003d6134: cmp      r2, r4
003d6138: beq      #0x3d6128
003d613c: ldr      r2, [r4, #0xc]
003d6140: cmp      r3, r2
003d6144: movne    r4, r3
003d6148: b        #0x3d5fe4
003d614c: mov      r0, sp
003d6150: add      r2, r4, #0x10
003d6154: bl       #0x3d5ee0
003d6158: b        #0x3d6078
003d615c: mov      r0, r4
003d6160: ldr      r1, [r5, #0x80]
003d6164: bl       #0x3cd34c
003d6168: mov      r3, #0
003d616c: str      r4, [r5, #0x88]
003d6170: str      r3, [r5, #0x8c]
003d6174: str      r4, [r5, #0x84]
003d6178: str      r3, [r5, #0x80]
003d617c: b        #0x3d60b0
003d6180: bl       #0x310440
003d6184: b        #0x3d6110

# _ZN6CharAI24AI_ClearAllAggroTowardMeEb
003d6abc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6ac0: mov      r3, #0
003d6ac4: sub      sp, sp, #0x10
003d6ac8: mov      r5, r0
003d6acc: mov      r7, r1
003d6ad0: mov      r0, sp
003d6ad4: ldr      r1, [r5, #0xa4]
003d6ad8: str      r3, [sp, #8]
003d6adc: str      r3, [sp]
003d6ae0: str      r3, [sp, #4]
003d6ae4: ldr      r4, [r5, #0x9c]
003d6ae8: bl       #0x3d5e14
003d6aec: mov      sb, sp
003d6af0: add      r6, r5, #0x94
003d6af4: add      r8, r5, #4
003d6af8: add      sl, sp, #0xc
003d6afc: cmp      r4, r6
003d6b00: beq      #0x3d6bd4
003d6b04: cmp      r7, #0
003d6b08: beq      #0x3d6c64
003d6b0c: ldr      r0, [r4, #0x10]
003d6b10: ldr      r2, [r5, #4]
003d6b14: ldr      r3, [r0, #0x408]
003d6b18: cmp      r2, r3
003d6b1c: beq      #0x3d6c48
003d6b20: ldr      r3, [r0, #0x448]
003d6b24: cmp      r3, #0
003d6b28: beq      #0x3d6b88
003d6b2c: add      r0, r0, #0x440
003d6b30: add      r0, r0, #4
003d6b34: ldr      ip, [r8]
003d6b38: mov      r1, r0
003d6b3c: b        #0x3d6b44
003d6b40: mov      r3, r2
003d6b44: ldr      r2, [r3, #0x10]
003d6b48: cmp      r2, ip
003d6b4c: ldrlo    r2, [r3, #0xc]
003d6b50: ldrhs    r2, [r3, #8]
003d6b54: movlo    r3, r1
003d6b58: mov      r1, r3
003d6b5c: cmp      r2, #0
003d6b60: bne      #0x3d6b40
003d6b64: cmp      r0, r3
003d6b68: beq      #0x3d6b88
003d6b6c: ldr      r1, [r5, #4]
003d6b70: ldr      r2, [r3, #0x10]
003d6b74: cmp      r1, r2
003d6b78: blo      #0x3d6b88
003d6b7c: mov      r1, sl
003d6b80: str      r3, [sp, #0xc]
003d6b84: bl       #0x3d5d9c
003d6b88: ldmib    sp, {r1, r3}
003d6b8c: cmp      r1, r3
003d6b90: beq      #0x3d6ca0
003d6b94: ldr      r3, [r4, #0x10]
003d6b98: str      r3, [r1]
003d6b9c: ldr      r3, [sp, #4]
003d6ba0: add      r3, r3, #4
003d6ba4: str      r3, [sp, #4]
003d6ba8: ldr      r2, [r4, #0xc]
003d6bac: cmp      r2, #0
003d6bb0: bne      #0x3d6bbc
003d6bb4: b        #0x3d6c6c
003d6bb8: mov      r2, r3
003d6bbc: ldr      r3, [r2, #8]
003d6bc0: cmp      r3, #0
003d6bc4: bne      #0x3d6bb8
003d6bc8: mov      r4, r2
003d6bcc: cmp      r4, r6
003d6bd0: bne      #0x3d6b04
003d6bd4: ldr      r3, [r5, #0xa4]
003d6bd8: cmp      r3, #0
003d6bdc: bne      #0x3d6cb0
003d6be0: ldm      sp, {r0, r3}
003d6be4: rsb      r3, r0, r3
003d6be8: lsrs     r3, r3, #2
003d6bec: beq      #0x3d6c20
003d6bf0: mov      r4, #0
003d6bf4: ldr      r3, [r5, #4]
003d6bf8: ldr      r1, [r0, r4, lsl #2]
003d6bfc: add      r4, r4, #1
003d6c00: add      r0, r3, #0x3c8
003d6c04: ldr      r3, [r3, #0x3c8]
003d6c08: mov      lr, pc
003d6c0c: ldr      pc, [r3, #0x3c]
003d6c10: ldm      sp, {r0, r3}
003d6c14: rsb      r3, r0, r3
003d6c18: cmp      r4, r3, asr #2
003d6c1c: blo      #0x3d6bf4
003d6c20: cmp      r0, #0
003d6c24: beq      #0x3d6c40
003d6c28: ldr      r1, [sp, #8]
003d6c2c: rsb      r1, r0, r1
003d6c30: bic      r1, r1, #3
003d6c34: cmp      r1, #0x80
003d6c38: bhi      #0x3d6cd4
003d6c3c: bl       #0x708f00
003d6c40: add      sp, sp, #0x10
003d6c44: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6c48: mov      r1, #0
003d6c4c: add      r0, r0, #0x3c8
003d6c50: mov      r2, r1
003d6c54: bl       #0x3d6890
003d6c58: ldr      r0, [r4, #0x10]
003d6c5c: add      r0, r0, #0x3c8
003d6c60: bl       #0x3d49c4
003d6c64: ldr      r0, [r4, #0x10]
003d6c68: b        #0x3d6b20
003d6c6c: ldr      r3, [r4, #4]
003d6c70: ldr      r1, [r3, #0xc]
003d6c74: cmp      r4, r1
003d6c78: bne      #0x3d6c94
003d6c7c: mov      r4, r3
003d6c80: ldr      r3, [r3, #4]
003d6c84: ldr      r2, [r3, #0xc]
003d6c88: cmp      r2, r4
003d6c8c: beq      #0x3d6c7c
003d6c90: ldr      r2, [r4, #0xc]
003d6c94: cmp      r3, r2
003d6c98: movne    r4, r3
003d6c9c: b        #0x3d6afc
003d6ca0: mov      r0, sp
003d6ca4: add      r2, r4, #0x10
003d6ca8: bl       #0x3d5ee0
003d6cac: b        #0x3d6ba8
003d6cb0: mov      r0, r4
003d6cb4: ldr      r1, [r5, #0x98]
003d6cb8: bl       #0x3cd34c
003d6cbc: mov      r3, #0
003d6cc0: str      r4, [r5, #0xa0]
003d6cc4: str      r3, [r5, #0xa4]
003d6cc8: str      r4, [r5, #0x9c]
003d6ccc: str      r3, [r5, #0x98]
003d6cd0: b        #0x3d6be0
003d6cd4: bl       #0x310440
003d6cd8: b        #0x3d6c40

# _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr      r5, [pc, #0x204]
003d6898: ldr      r7, [pc, #0x204]
003d689c: sub      sp, sp, #0x78
003d68a0: add      r5, pc, r5
003d68a4: ldr      r3, [r5, r7]
003d68a8: mov      r4, r0
003d68ac: cmp      r2, #0
003d68b0: ldr      r3, [r3]
003d68b4: mov      r6, r1
003d68b8: str      r1, [r4, #0x3c]
003d68bc: str      r3, [sp, #0x74]
003d68c0: bne      #0x3d6a20
003d68c4: ldr      r3, [r0, #0x40]
003d68c8: ldr      sb, [pc, #0x1d8]
003d68cc: add      r8, sp, #0x5c
003d68d0: cmp      r3, r1
003d68d4: ldrne    r1, [r0, #4]
003d68d8: ldr      sl, [r5, sb]
003d68dc: movwne   r3, #0x14d0
003d68e0: strhne   r2, [r1, r3]
003d68e4: mov      r0, sl
003d68e8: bl       #0x337888
003d68ec: ldr      r1, [pc, #0x1b8]
003d68f0: add      r2, sp, #0x10
003d68f4: mov      r0, r8
003d68f8: add      r1, pc, r1
003d68fc: bl       #0x3140ec
003d6900: mov      r0, sl
003d6904: mov      r1, r8
003d6908: bl       #0x337a88
003d690c: mov      sl, r0
003d6910: ldr      r0, [sp, #0x70]
003d6914: cmp      r0, r8
003d6918: beq      #0x3d6938
003d691c: cmp      r0, #0
003d6920: beq      #0x3d6938
003d6924: ldr      r1, [sp, #0x5c]
003d6928: rsb      r1, r0, r1
003d692c: cmp      r1, #0x80
003d6930: bhi      #0x3d6a4c
003d6934: bl       #0x708f00
003d6938: cmp      sl, #0
003d693c: beq      #0x3d69a4
003d6940: ldr      r3, [r4, #0x40]
003d6944: cmp      r3, r6
003d6948: beq      #0x3d69a4
003d694c: subs     r3, r3, #0
003d6950: movne    r3, #1
003d6954: subs     r2, r6, #0
003d6958: movne    r2, #1
003d695c: tst      r2, r3
003d6960: bne      #0x3d6a28
003d6964: cmp      r3, #0
003d6968: beq      #0x3d6a18
003d696c: ldr      sl, [r5, sb]
003d6970: add      r8, sp, #0x2c
003d6974: mov      r0, sl
003d6978: bl       #0x337888
003d697c: ldr      r1, [pc, #0x12c]
003d6980: add      r2, sp, #8
003d6984: mov      r0, r8
003d6988: add      r1, pc, r1
003d698c: bl       #0x3140ec
003d6990: mov      r0, sl
003d6994: mov      r1, r8
003d6998: bl       #0x337a88
003d699c: mov      r0, r8
003d69a0: bl       #0x318254
003d69a4: cmp      r6, #0
003d69a8: str      r6, [r4, #0x40]
003d69ac: beq      #0x3d69fc
003d69b0: ldr      r0, [r4, #4]
003d69b4: bl       #0x3a2fec
003d69b8: ldr      r2, [r4, #0x44]
003d69bc: ldr      r3, [r4, #0x40]
003d69c0: cmp      r3, r2
003d69c4: movne    r2, #0
003d69c8: strbne   r2, [r4, #0x4c]
003d69cc: movne    r2, r3
003d69d0: str      r2, [r4, #0x44]
003d69d4: mov      r0, r3
003d69d8: ldr      r3, [r3]
003d69dc: mov      lr, pc
003d69e0: ldr      pc, [r3, #0x34]
003d69e4: eor      r0, r0, #1
003d69e8: strb     r0, [r4, #0x48]
003d69ec: ldr      r1, [r4, #0x40]
003d69f0: mov      r0, r4
003d69f4: bl       #0x3d4ed8
003d69f8: strb     r0, [r4, #0x49]
003d69fc: ldr      r3, [r5, r7]
003d6a00: ldr      r2, [sp, #0x74]
003d6a04: ldr      r3, [r3]
003d6a08: cmp      r2, r3
003d6a0c: bne      #0x3d6a9c
003d6a10: add      sp, sp, #0x78
003d6a14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp      r2, #0
003d6a1c: bne      #0x3d6a5c
003d6a20: str      r6, [r4, #0x40]
003d6a24: b        #0x3d69fc
003d6a28: ldr      sl, [r5, sb]
003d6a2c: add      r8, sp, #0x44
003d6a30: mov      r0, sl
003d6a34: bl       #0x337888
003d6a38: ldr      r1, [pc, #0x74]
003d6a3c: add      r2, sp, #0xc
003d6a40: mov      r0, r8
003d6a44: add      r1, pc, r1
003d6a48: b        #0x3d698c
003d6a4c: bl       #0x310440
003d6a50: cmp      sl, #0
003d6a54: beq      #0x3d69a4
003d6a58: b        #0x3d6940
003d6a5c: ldr      sl, [r5, sb]
003d6a60: add      r8, sp, #0x14
003d6a64: mov      r0, sl
003d6a68: bl       #0x337888
003d6a6c: ldr      r1, [pc, #0x44]
003d6a70: add      r2, sp, #4
003d6a74: mov      r0, r8
003d6a78: add      r1, pc, r1
003d6a7c: bl       #0x3140ec
003d6a80: mov      r1, r8
003d6a84: mov      r0, sl
003d6a88: bl       #0x337a88
003d6a8c: mov      r0, r8
003d6a90: bl       #0x318254
003d6a94: str      r6, [r4, #0x40]
003d6a98: b        #0x3d69b0
003d6a9c: bl       #0x30e310
003d6aa0: ldrsheq  lr, [fp], #-0x10
003d6aa4: andeq    r4, r0, ip, lsr #1
003d6aa8: andeq    r0, r0, r4, lsl #17
003d6aac: subeq    lr, lr, r0, ror #27
003d6ab0: subeq    lr, lr, r8, ror #26
003d6ab4: subeq    lr, lr, ip, lsr #25
003d6ab8: subeq    lr, lr, r8, ror ip

# _ZN6CharAI9GroupInfo6OnDiedEP9CharacterP10GameObject
003d2628: push     {r4, r5, r6, r7, r8, lr}
003d262c: ldr      r3, [r1, #0x400]
003d2630: mov      r5, r0
003d2634: mov      r6, r2
003d2638: cmp      r3, #2
003d263c: beq      #0x3d2654
003d2640: cmp      r3, #1
003d2644: beq      #0x3d26e0
003d2648: cmp      r3, #3
003d264c: beq      #0x3d2758
003d2650: pop      {r4, r5, r6, r7, r8, pc}
003d2654: ldr      r3, [r0, #0xc]
003d2658: ldr      r7, [r0, #0x10]
003d265c: mov      r2, #1
003d2660: strb     r2, [r0, #0x28]
003d2664: rsb      r7, r3, r7
003d2668: asrs     r7, r7, #2
003d266c: beq      #0x3d269c
003d2670: mov      r4, #0
003d2674: b        #0x3d267c
003d2678: ldr      r3, [r5, #0xc]
003d267c: ldr      r3, [r3, r4, lsl #2]
003d2680: mov      r1, r6
003d2684: add      r4, r4, #1
003d2688: ldr      r0, [r3, #0x378]
003d268c: mov      r2, #0
003d2690: bl       #0x40570c
003d2694: cmp      r4, r7
003d2698: bne      #0x3d2678
003d269c: ldr      r3, [r5, #0x18]
003d26a0: ldr      r7, [r5, #0x1c]
003d26a4: rsb      r7, r3, r7
003d26a8: asrs     r7, r7, #2
003d26ac: beq      #0x3d2650
003d26b0: mov      r4, #0
003d26b4: b        #0x3d26bc
003d26b8: ldr      r3, [r5, #0x18]
003d26bc: ldr      r3, [r3, r4, lsl #2]
003d26c0: mov      r1, r6
003d26c4: add      r4, r4, #1
003d26c8: ldr      r0, [r3, #0x378]
003d26cc: mov      r2, #0
003d26d0: bl       #0x40570c
003d26d4: cmp      r4, r7
003d26d8: bne      #0x3d26b8
003d26dc: pop      {r4, r5, r6, r7, r8, pc}
003d26e0: ldrb     r4, [r0, #0x28]
003d26e4: cmp      r4, #0
003d26e8: bne      #0x3d2650
003d26ec: ldr      r7, [r0, #0x10]
003d26f0: ldr      r2, [r0, #0xc]
003d26f4: strb     r3, [r0, #0x28]
003d26f8: rsb      r7, r2, r7
003d26fc: asrs     r7, r7, #2
003d2700: beq      #0x3d2650
003d2704: mov      r6, r3
003d2708: b        #0x3d271c
003d270c: add      r4, r4, #1
003d2710: cmp      r4, r7
003d2714: strb     r6, [r5, #0x28]
003d2718: beq      #0x3d2754
003d271c: cmp      r6, #0
003d2720: beq      #0x3d270c
003d2724: ldr      r3, [r5, #0xc]
003d2728: ldr      r3, [r3, r4, lsl #2]
003d272c: add      r4, r4, #1
003d2730: mov      r0, r3
003d2734: ldr      r3, [r3]
003d2738: mov      lr, pc
003d273c: ldr      pc, [r3, #0x34]
003d2740: cmp      r0, #0
003d2744: moveq    r6, #0
003d2748: cmp      r4, r7
003d274c: strb     r6, [r5, #0x28]
003d2750: bne      #0x3d271c
003d2754: pop      {r4, r5, r6, r7, r8, pc}
003d2758: ldr      r4, [r0, #0x24]
003d275c: cmp      r4, #0
003d2760: bne      #0x3d2650
003d2764: ldr      r7, [r0, #0x1c]
003d2768: ldr      r3, [r0, #0x18]
003d276c: rsb      r7, r3, r7
003d2770: asrs     r7, r7, #2
003d2774: moveq    r6, #1
003d2778: beq      #0x3d27c4
003d277c: mov      r6, #1
003d2780: b        #0x3d2790
003d2784: add      r4, r4, #1
003d2788: cmp      r4, r7
003d278c: beq      #0x3d27c4
003d2790: cmp      r6, #0
003d2794: beq      #0x3d2784
003d2798: ldr      r3, [r5, #0x18]
003d279c: ldr      r3, [r3, r4, lsl #2]
003d27a0: add      r4, r4, #1
003d27a4: mov      r0, r3
003d27a8: ldr      r3, [r3]
003d27ac: mov      lr, pc
003d27b0: ldr      pc, [r3, #0x34]
003d27b4: cmp      r0, #0
003d27b8: moveq    r6, #0
003d27bc: cmp      r4, r7
003d27c0: bne      #0x3d2790
003d27c4: str      r6, [r5, #0x24]
003d27c8: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6
