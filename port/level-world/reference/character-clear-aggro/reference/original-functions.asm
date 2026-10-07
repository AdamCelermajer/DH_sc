
# _ZN9Character11_ClearAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b8648: push     {r4, lr}
003b864c: ldr      r3, [r0, #4]
003b8650: ldm      r3, {r0, r1}
003b8654: rsb      r3, r0, r1
003b8658: asr      r3, r3, #4
003b865c: add      r1, r3, r3, lsl #3
003b8660: add      r1, r1, r1, lsl #6
003b8664: add      r1, r3, r1, lsl #3
003b8668: add      r1, r1, r1, lsl #15
003b866c: add      r3, r3, r1, lsl #3
003b8670: cmp      r3, #0
003b8674: bne      #0x3b867c
003b8678: pop      {r4, pc}
003b867c: ldr      r3, [r0, #4]
003b8680: cmp      r3, #2
003b8684: beq      #0x3b8690
003b8688: cmp      r3, #7
003b868c: bne      #0x3b8678
003b8690: add      r4, r2, #0x3c8
003b8694: bl       #0x31b5a0
003b8698: mov      r1, r0
003b869c: mov      r0, r4
003b86a0: pop      {r4, lr}
003b86a4: b        #0x3d6d68

# _ZN6CharAI13AI_ClearAggroEP9Character
003d6d68: push     {r4, r5, lr}
003d6d6c: subs     r4, r1, #0
003d6d70: sub      sp, sp, #0xc
003d6d74: mov      r5, r0
003d6d78: beq      #0x3d6e98
003d6d7c: ldr      r2, [r0, #0x80]
003d6d80: add      r0, r0, #0x7c
003d6d84: cmp      r2, #0
003d6d88: beq      #0x3d6ea0
003d6d8c: mov      ip, r0
003d6d90: mov      r3, r2
003d6d94: b        #0x3d6d9c
003d6d98: mov      r3, r1
003d6d9c: ldr      r1, [r3, #0x10]
003d6da0: cmp      r1, r4
003d6da4: ldrlo    r1, [r3, #0xc]
003d6da8: ldrhs    r1, [r3, #8]
003d6dac: movlo    r3, ip
003d6db0: mov      ip, r3
003d6db4: cmp      r1, #0
003d6db8: bne      #0x3d6d98
003d6dbc: cmp      r0, r3
003d6dc0: beq      #0x3d6e88
003d6dc4: ldr      r1, [r3, #0x10]
003d6dc8: cmp      r1, r4
003d6dcc: bhi      #0x3d6ea0
003d6dd0: cmp      r0, r3
003d6dd4: beq      #0x3d6e88
003d6dd8: cmp      r2, #0
003d6ddc: beq      #0x3d6e20
003d6de0: mov      r1, r0
003d6de4: b        #0x3d6dec
003d6de8: mov      r2, r3
003d6dec: ldr      r3, [r2, #0x10]
003d6df0: cmp      r3, r4
003d6df4: ldrlo    r3, [r2, #0xc]
003d6df8: ldrhs    r3, [r2, #8]
003d6dfc: movlo    r2, r1
003d6e00: mov      r1, r2
003d6e04: cmp      r3, #0
003d6e08: bne      #0x3d6de8
003d6e0c: cmp      r0, r2
003d6e10: beq      #0x3d6e20
003d6e14: ldr      r3, [r2, #0x10]
003d6e18: cmp      r3, r4
003d6e1c: bls      #0x3d6ec0
003d6e20: ldr      r3, [r4, #0x460]
003d6e24: cmp      r3, #0
003d6e28: beq      #0x3d6eb8
003d6e2c: add      r0, r4, #0x450
003d6e30: add      r0, r0, #0xc
003d6e34: ldr      r1, [r5, #4]
003d6e38: mov      ip, r0
003d6e3c: b        #0x3d6e44
003d6e40: mov      r3, r2
003d6e44: ldr      r2, [r3, #0x10]
003d6e48: cmp      r1, r2
003d6e4c: ldrhi    r2, [r3, #0xc]
003d6e50: ldrls    r2, [r3, #8]
003d6e54: movhi    r3, ip
003d6e58: mov      ip, r3
003d6e5c: cmp      r2, #0
003d6e60: bne      #0x3d6e40
003d6e64: cmp      r0, r3
003d6e68: beq      #0x3d6e78
003d6e6c: ldr      r2, [r3, #0x10]
003d6e70: cmp      r1, r2
003d6e74: bhs      #0x3d6ea8
003d6e78: ldr      r3, [r4, #0x3c8]
003d6e7c: add      r0, r4, #0x3c8
003d6e80: mov      lr, pc
003d6e84: ldr      pc, [r3, #0x3c]
003d6e88: ldr      r2, [r5, #4]
003d6e8c: ldr      r3, [r4, #0x408]
003d6e90: cmp      r2, r3
003d6e94: beq      #0x3d6ed0
003d6e98: add      sp, sp, #0xc
003d6e9c: pop      {r4, r5, pc}
003d6ea0: mov      r3, r0
003d6ea4: b        #0x3d6dd0
003d6ea8: add      r1, sp, #8
003d6eac: str      r3, [r1, #-8]!
003d6eb0: mov      r1, sp
003d6eb4: bl       #0x3d5d9c
003d6eb8: ldr      r1, [r5, #4]
003d6ebc: b        #0x3d6e78
003d6ec0: add      r1, sp, #8
003d6ec4: str      r2, [r1, #-4]!
003d6ec8: bl       #0x3d5d9c
003d6ecc: b        #0x3d6e20
003d6ed0: mov      r1, #0
003d6ed4: add      r0, r4, #0x3c8
003d6ed8: mov      r2, r1
003d6edc: bl       #0x3d6890
003d6ee0: ldr      r0, [r4, #0x378]
003d6ee4: bl       #0x40559c
003d6ee8: b        #0x3d6e98

# _ZNSt4priv8_Rb_treeIP9CharacterSt4lessIS2_ESt4pairIKS2_fENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE5eraseENS_17_Rb_tree_iteratorIS7_SB_EE
003d5d9c: push     {r4, lr}
003d5da0: mov      r4, r0
003d5da4: add      r2, r4, #8
003d5da8: ldr      r0, [r1]
003d5dac: add      r3, r4, #0xc
003d5db0: add      r1, r4, #4
003d5db4: bl       #0x336004
003d5db8: cmp      r0, #0
003d5dbc: beq      #0x3d5dc8
003d5dc0: mov      r1, #0x18
003d5dc4: bl       #0x708f00
003d5dc8: ldr      r3, [r4, #0x10]
003d5dcc: sub      r3, r3, #1
003d5dd0: str      r3, [r4, #0x10]
003d5dd4: pop      {r4, pc}

# _ZNK3sfc6script3lua5Value11getUserDataEv
0031b5a0: ldr      r3, [r0, #4]
0031b5a4: cmp      r3, #2
0031b5a8: beq      #0x31b5b8
0031b5ac: cmp      r3, #7
0031b5b0: movne    r0, #0
0031b5b4: bxne     lr
0031b5b8: ldr      r0, [r0, #0x6c]
0031b5bc: bx       lr

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

# _ZN6CharAI11AI_SetAggroEP9Characterf
003d79ec: push     {r4, r5, r6, r7, lr}
003d79f0: ldr      r3, [pc, #0x258]
003d79f4: subs     r4, r1, #0
003d79f8: sub      sp, sp, #0x2c
003d79fc: mov      r6, r0
003d7a00: add      r3, pc, r3
003d7a04: mov      r5, r2
003d7a08: beq      #0x3d7bd4
003d7a0c: ldr      r3, [r6, #4]
003d7a10: mov      r0, r3
003d7a14: ldr      r3, [r3]
003d7a18: mov      lr, pc
003d7a1c: ldr      pc, [r3, #0x28]
003d7a20: cmp      r0, #0
003d7a24: beq      #0x3d7a38
003d7a28: mov      r5, #0
003d7a2c: mov      r0, r5
003d7a30: add      sp, sp, #0x2c
003d7a34: pop      {r4, r5, r6, r7, pc}
003d7a38: ldr      r3, [r6, #4]
003d7a3c: mov      r0, r3
003d7a40: ldr      r3, [r3]
003d7a44: mov      lr, pc
003d7a48: ldr      pc, [r3, #0x34]
003d7a4c: cmp      r0, #0
003d7a50: bne      #0x3d7a28
003d7a54: ldr      r3, [r4]
003d7a58: mov      r0, r4
003d7a5c: mov      lr, pc
003d7a60: ldr      pc, [r3, #0x34]
003d7a64: cmp      r0, #0
003d7a68: bne      #0x3d7a28
003d7a6c: ldr      ip, [r6, #0x80]
003d7a70: add      r7, r6, #0x7c
003d7a74: cmp      ip, #0
003d7a78: movne    r1, r7
003d7a7c: movne    r3, ip
003d7a80: bne      #0x3d7a8c
003d7a84: b        #0x3d7bcc
003d7a88: mov      r3, r2
003d7a8c: ldr      r2, [r3, #0x10]
003d7a90: cmp      r4, r2
003d7a94: ldrhi    r2, [r3, #0xc]
003d7a98: ldrls    r2, [r3, #8]
003d7a9c: movhi    r3, r1
003d7aa0: mov      r1, r3
003d7aa4: cmp      r2, #0
003d7aa8: bne      #0x3d7a88
003d7aac: cmp      r7, r3
003d7ab0: beq      #0x3d7c34
003d7ab4: ldr      r2, [r3, #0x10]
003d7ab8: cmp      r4, r2
003d7abc: blo      #0x3d7bcc
003d7ac0: cmp      r7, r3
003d7ac4: beq      #0x3d7c34
003d7ac8: cmp      ip, #0
003d7acc: moveq    ip, r7
003d7ad0: beq      #0x3d7b00
003d7ad4: mov      r2, r7
003d7ad8: b        #0x3d7ae0
003d7adc: mov      ip, r3
003d7ae0: ldr      r3, [ip, #0x10]
003d7ae4: cmp      r4, r3
003d7ae8: ldrhi    r3, [ip, #0xc]
003d7aec: ldrls    r3, [ip, #8]
003d7af0: movhi    ip, r2
003d7af4: mov      r2, ip
003d7af8: cmp      r3, #0
003d7afc: bne      #0x3d7adc
003d7b00: cmp      r7, ip
003d7b04: beq      #0x3d7b18
003d7b08: ldr      r2, [ip, #0x10]
003d7b0c: mov      r3, ip
003d7b10: cmp      r4, r2
003d7b14: bhs      #0x3d7b40
003d7b18: add      r3, sp, #0x10
003d7b1c: mov      lr, #0
003d7b20: mov      r1, r7
003d7b24: add      r0, sp, #0x20
003d7b28: add      r2, sp, #0x24
003d7b2c: str      lr, [sp, #0x14]
003d7b30: str      ip, [sp, #0x24]
003d7b34: str      r4, [sp, #0x10]
003d7b38: bl       #0x3d7678
003d7b3c: ldr      r3, [sp, #0x20]
003d7b40: str      r5, [r3, #0x14]
003d7b44: ldr      ip, [r4, #0x460]
003d7b48: add      r1, r4, #0x450
003d7b4c: add      r1, r1, #0xc
003d7b50: cmp      ip, #0
003d7b54: beq      #0x3d7c28
003d7b58: ldr      r6, [r6, #4]
003d7b5c: mov      r2, r1
003d7b60: b        #0x3d7b68
003d7b64: mov      ip, r3
003d7b68: ldr      r3, [ip, #0x10]
003d7b6c: cmp      r6, r3
003d7b70: ldrhi    r3, [ip, #0xc]
003d7b74: ldrls    r3, [ip, #8]
003d7b78: movhi    ip, r2
003d7b7c: mov      r2, ip
003d7b80: cmp      r3, #0
003d7b84: bne      #0x3d7b64
003d7b88: cmp      r1, ip
003d7b8c: beq      #0x3d7ba0
003d7b90: ldr      r2, [ip, #0x10]
003d7b94: mov      r3, ip
003d7b98: cmp      r6, r2
003d7b9c: bhs      #0x3d7bc4
003d7ba0: add      r3, sp, #8
003d7ba4: mov      lr, #0
003d7ba8: add      r0, sp, #0x18
003d7bac: add      r2, sp, #0x1c
003d7bb0: str      r6, [sp, #8]
003d7bb4: str      lr, [sp, #0xc]
003d7bb8: str      ip, [sp, #0x1c]
003d7bbc: bl       #0x3d7678
003d7bc0: ldr      r3, [sp, #0x18]
003d7bc4: str      r5, [r3, #0x14]
003d7bc8: b        #0x3d7a2c
003d7bcc: mov      r3, r7
003d7bd0: b        #0x3d7ac0
003d7bd4: ldr      r2, [pc, #0x78]
003d7bd8: ldr      r2, [r3, r2]
003d7bdc: ldr      r2, [r2]
003d7be0: cmp      r2, #2
003d7be4: streq    r4, [r4]
003d7be8: beq      #0x3d7a0c
003d7bec: cmp      r2, #1
003d7bf0: bne      #0x3d7a0c
003d7bf4: ldr      r0, [pc, #0x5c]
003d7bf8: ldr      r1, [pc, #0x5c]
003d7bfc: ldr      r2, [pc, #0x5c]
003d7c00: ldr      r0, [r3, r0]
003d7c04: ldr      r3, [pc, #0x58]
003d7c08: mov      ip, #0x274
003d7c0c: add      r1, pc, r1
003d7c10: add      r2, pc, r2
003d7c14: add      r3, pc, r3
003d7c18: add      r0, r0, #0xa8
003d7c1c: str      ip, [sp]
003d7c20: bl       #0x30e004
003d7c24: b        #0x3d7a0c
003d7c28: ldr      r6, [r6, #4]
003d7c2c: mov      ip, r1
003d7c30: b        #0x3d7b88
003d7c34: ldr      r3, [r4, #0x3c8]
003d7c38: add      r0, r4, #0x3c8
003d7c3c: ldr      r1, [r6, #4]
003d7c40: mov      lr, pc
003d7c44: ldr      pc, [r3, #0x38]
003d7c48: ldr      ip, [r6, #0x80]
003d7c4c: b        #0x3d7ac8

# _ZN12v2Controller8Cmd_StopEv
0040559c: push     {r4, lr}
004055a0: ldrb     r2, [r0, #9]
004055a4: ldr      r3, [pc, #0x44]
004055a8: cmp      r2, #0
004055ac: add      r3, pc, r3
004055b0: bne      #0x4055d8
004055b4: ldr      r2, [pc, #0x38]
004055b8: ldr      r3, [r3, r2]
004055bc: ldrb     r3, [r3]
004055c0: cmp      r3, #0
004055c4: beq      #0x4055cc
004055c8: pop      {r4, pc}
004055cc: ldrb     r3, [r0, #8]
004055d0: cmp      r3, #0
004055d4: bne      #0x4055c8
004055d8: ldr      r3, [r0, #4]
004055dc: mov      r0, r3
004055e0: ldr      r3, [r3]
004055e4: mov      lr, pc
004055e8: ldr      pc, [r3, #0x34]
004055ec: pop      {r4, pc}
004055f0: subseq   pc, r8, r4, ror #9
004055f4: andeq    r3, r0, r0, asr r6

# _ZNSt4priv10_Rb_globalIbE20_Rebalance_for_eraseEPNS_18_Rb_tree_node_baseERS3_S4_S4_
00336004: push     {r4, r5, r6, r7}
00336008: ldr      ip, [r0, #8]
0033600c: cmp      ip, #0
00336010: ldreq    ip, [r0, #0xc]
00336014: beq      #0x3363ac
00336018: ldr      r5, [r0, #0xc]
0033601c: cmp      r5, #0
00336020: bne      #0x33602c
00336024: b        #0x3363ac
00336028: mov      r5, r4
0033602c: ldr      r4, [r5, #8]
00336030: cmp      r4, #0
00336034: bne      #0x336028
00336038: cmp      r0, r5
0033603c: ldr      r4, [r5, #0xc]
00336040: beq      #0x3363b4
00336044: str      r5, [ip, #4]
00336048: ldr      r3, [r0, #8]
0033604c: str      r3, [r5, #8]
00336050: ldr      ip, [r0, #0xc]
00336054: cmp      ip, r5
00336058: beq      #0x336084
0033605c: ldr      ip, [r5, #4]
00336060: cmp      r4, #0
00336064: strne    ip, [r4, #4]
00336068: ldrne    r3, [r5, #4]
0033606c: moveq    r3, ip
00336070: str      r4, [r3, #8]
00336074: ldr      r3, [r0, #0xc]
00336078: str      r3, [r5, #0xc]
0033607c: ldr      r3, [r0, #0xc]
00336080: str      r5, [r3, #4]
00336084: ldr      r3, [r1]
00336088: cmp      r3, r0
0033608c: streq    r5, [r1]
00336090: beq      #0x3360a8
00336094: ldr      r3, [r0, #4]
00336098: ldr      r2, [r3, #8]
0033609c: cmp      r2, r0
003360a0: streq    r5, [r3, #8]
003360a4: strne    r5, [r3, #0xc]
003360a8: ldr      r2, [r0, #4]
003360ac: ldrb     r3, [r5]
003360b0: str      r2, [r5, #4]
003360b4: ldrb     r2, [r0]
003360b8: strb     r2, [r5]
003360bc: strb     r3, [r0]
003360c0: mov      r5, r0
003360c4: cmp      r3, #0
003360c8: movne    r6, #0
003360cc: movne    r7, #1
003360d0: beq      #0x3360fc
003360d4: ldr      r3, [r1]
003360d8: cmp      r3, r4
003360dc: beq      #0x3363a0
003360e0: cmp      r4, #0
003360e4: beq      #0x336108
003360e8: ldrb     r3, [r4]
003360ec: cmp      r3, #0
003360f0: bne      #0x336108
003360f4: mov      r3, #1
003360f8: strb     r3, [r4]
003360fc: mov      r0, r5
00336100: pop      {r4, r5, r6, r7}
00336104: bx       lr
00336108: ldr      r3, [ip, #8]
0033610c: cmp      r3, r4
00336110: beq      #0x3361bc
00336114: ldrb     r0, [r3]
00336118: cmp      r0, #0
0033611c: bne      #0x336178
00336120: strb     r7, [r3]
00336124: ldr      r2, [ip, #8]
00336128: strb     r0, [ip]
0033612c: ldr      r3, [r2, #0xc]
00336130: str      r3, [ip, #8]
00336134: ldr      r3, [r2, #0xc]
00336138: cmp      r3, #0
0033613c: strne    ip, [r3, #4]
00336140: ldr      r3, [ip, #4]
00336144: str      r3, [r2, #4]
00336148: ldr      r3, [r1]
0033614c: cmp      ip, r3
00336150: streq    r2, [r1]
00336154: beq      #0x33616c
00336158: ldr      r3, [ip, #4]
0033615c: ldr      r0, [r3, #0xc]
00336160: cmp      ip, r0
00336164: streq    r2, [r3, #0xc]
00336168: strne    r2, [r3, #8]
0033616c: str      ip, [r2, #0xc]
00336170: ldr      r3, [ip, #8]
00336174: str      r2, [ip, #4]
00336178: ldr      r2, [r3, #0xc]
0033617c: cmp      r2, #0
00336180: beq      #0x336190
00336184: ldrb     r0, [r2]
00336188: cmp      r0, #0
0033618c: beq      #0x3362c0
00336190: ldr      r2, [r3, #8]
00336194: cmp      r2, #0
00336198: beq      #0x3361a8
0033619c: ldrb     r2, [r2]
003361a0: cmp      r2, #0
003361a4: beq      #0x336338
003361a8: strb     r6, [r3]
003361ac: ldr      r3, [ip, #4]
003361b0: mov      r4, ip
003361b4: mov      ip, r3
003361b8: b        #0x3360d4
003361bc: ldr      r3, [ip, #0xc]
003361c0: ldrb     r0, [r3]
003361c4: cmp      r0, #0
003361c8: bne      #0x336224
003361cc: strb     r7, [r3]
003361d0: ldr      r2, [ip, #0xc]
003361d4: strb     r0, [ip]
003361d8: ldr      r3, [r2, #8]
003361dc: str      r3, [ip, #0xc]
003361e0: ldr      r3, [r2, #8]
003361e4: cmp      r3, #0
003361e8: strne    ip, [r3, #4]
003361ec: ldr      r3, [ip, #4]
003361f0: str      r3, [r2, #4]
003361f4: ldr      r3, [r1]
003361f8: cmp      ip, r3
003361fc: streq    r2, [r1]
00336200: beq      #0x336218
00336204: ldr      r3, [ip, #4]
00336208: ldr      r0, [r3, #8]
0033620c: cmp      ip, r0
00336210: streq    r2, [r3, #8]
00336214: strne    r2, [r3, #0xc]
00336218: str      ip, [r2, #8]
0033621c: ldr      r3, [ip, #0xc]
00336220: str      r2, [ip, #4]
00336224: ldr      r2, [r3, #8]
00336228: cmp      r2, #0
0033622c: beq      #0x33623c
00336230: ldrb     r0, [r2]
00336234: cmp      r0, #0
00336238: beq      #0x33643c
0033623c: ldr      r2, [r3, #0xc]
00336240: cmp      r2, #0
00336244: beq      #0x3361a8
00336248: ldrb     r2, [r2]
0033624c: cmp      r2, #0
00336250: bne      #0x3361a8
00336254: ldrb     r0, [ip]
00336258: mov      r2, #1
0033625c: strb     r0, [r3]
00336260: strb     r2, [ip]
00336264: ldr      r3, [r3, #0xc]
00336268: cmp      r3, #0
0033626c: strbne   r2, [r3]
00336270: ldr      r3, [ip, #0xc]
00336274: ldr      r2, [r3, #8]
00336278: str      r2, [ip, #0xc]
0033627c: ldr      r2, [r3, #8]
00336280: cmp      r2, #0
00336284: strne    ip, [r2, #4]
00336288: ldr      r2, [ip, #4]
0033628c: str      r2, [r3, #4]
00336290: ldr      r2, [r1]
00336294: cmp      ip, r2
00336298: streq    r3, [r1]
0033629c: beq      #0x3362b4
003362a0: ldr      r2, [ip, #4]
003362a4: ldr      r1, [r2, #8]
003362a8: cmp      ip, r1
003362ac: streq    r3, [r2, #8]
003362b0: strne    r3, [r2, #0xc]
003362b4: str      ip, [r3, #8]
003362b8: str      r3, [ip, #4]
003362bc: b        #0x3363a0
003362c0: ldr      r0, [r3, #8]
003362c4: cmp      r0, #0
003362c8: beq      #0x3362d8
003362cc: ldrb     r0, [r0]
003362d0: cmp      r0, #0
003362d4: beq      #0x336338
003362d8: mov      r0, #1
003362dc: strb     r0, [r2]
003362e0: ldr      r2, [r3, #0xc]
003362e4: mov      r0, #0
003362e8: strb     r0, [r3]
003362ec: ldr      r0, [r2, #8]
003362f0: str      r0, [r3, #0xc]
003362f4: ldr      r0, [r2, #8]
003362f8: cmp      r0, #0
003362fc: strne    r3, [r0, #4]
00336300: ldr      r0, [r3, #4]
00336304: str      r0, [r2, #4]
00336308: ldr      r0, [r1]
0033630c: cmp      r3, r0
00336310: streq    r2, [r1]
00336314: beq      #0x33632c
00336318: ldr      r0, [r3, #4]
0033631c: ldr      r6, [r0, #8]
00336320: cmp      r3, r6
00336324: streq    r2, [r0, #8]
00336328: strne    r2, [r0, #0xc]
0033632c: str      r3, [r2, #8]
00336330: str      r2, [r3, #4]
00336334: ldr      r3, [ip, #8]
00336338: ldrb     r0, [ip]
0033633c: mov      r2, #1
00336340: strb     r0, [r3]
00336344: strb     r2, [ip]
00336348: ldr      r3, [r3, #8]
0033634c: cmp      r3, #0
00336350: strbne   r2, [r3]
00336354: ldr      r3, [ip, #8]
00336358: ldr      r2, [r3, #0xc]
0033635c: str      r2, [ip, #8]
00336360: ldr      r2, [r3, #0xc]
00336364: cmp      r2, #0
00336368: strne    ip, [r2, #4]
0033636c: ldr      r2, [ip, #4]
00336370: str      r2, [r3, #4]
00336374: ldr      r2, [r1]
00336378: cmp      ip, r2
0033637c: streq    r3, [r1]
00336380: beq      #0x336398
00336384: ldr      r2, [ip, #4]
00336388: ldr      r1, [r2, #0xc]
0033638c: cmp      ip, r1
00336390: streq    r3, [r2, #0xc]
00336394: strne    r3, [r2, #8]
00336398: str      ip, [r3, #0xc]
0033639c: str      r3, [ip, #4]
003363a0: cmp      r4, #0
003363a4: beq      #0x3360fc
003363a8: b        #0x3360f4
003363ac: mov      r4, ip
003363b0: mov      r5, r0
003363b4: ldr      ip, [r5, #4]
003363b8: cmp      r4, #0
003363bc: strne    ip, [r4, #4]
003363c0: ldr      r6, [r1]
003363c4: cmp      r6, r0
003363c8: streq    r4, [r1]
003363cc: beq      #0x3363e4
003363d0: ldr      r6, [r0, #4]
003363d4: ldr      r7, [r6, #8]
003363d8: cmp      r7, r0
003363dc: streq    r4, [r6, #8]
003363e0: strne    r4, [r6, #0xc]
003363e4: ldr      r6, [r2]
003363e8: cmp      r6, r0
003363ec: beq      #0x3364b8
003363f0: ldr      r2, [r3]
003363f4: cmp      r2, r0
003363f8: ldrbne   r3, [r5]
003363fc: bne      #0x3360c4
00336400: ldr      r2, [r0, #8]
00336404: cmp      r2, #0
00336408: ldreq    r2, [r0, #4]
0033640c: streq    r2, [r3]
00336410: ldrbeq   r3, [r5]
00336414: beq      #0x3360c4
00336418: mov      r0, r4
0033641c: b        #0x336424
00336420: mov      r0, r2
00336424: ldr      r2, [r0, #0xc]
00336428: cmp      r2, #0
0033642c: bne      #0x336420
00336430: str      r0, [r3]
00336434: ldrb     r3, [r5]
00336438: b        #0x3360c4
0033643c: ldr      r0, [r3, #0xc]
00336440: cmp      r0, #0
00336444: beq      #0x336454
00336448: ldrb     r0, [r0]
0033644c: cmp      r0, #0
00336450: beq      #0x336254
00336454: mov      r0, #1
00336458: strb     r0, [r2]
0033645c: ldr      r2, [r3, #8]
00336460: mov      r0, #0
00336464: strb     r0, [r3]
00336468: ldr      r0, [r2, #0xc]
0033646c: str      r0, [r3, #8]
00336470: ldr      r0, [r2, #0xc]
00336474: cmp      r0, #0
00336478: strne    r3, [r0, #4]
0033647c: ldr      r0, [r3, #4]
00336480: str      r0, [r2, #4]
00336484: ldr      r0, [r1]
00336488: cmp      r3, r0
0033648c: streq    r2, [r1]
00336490: beq      #0x3364a8
00336494: ldr      r0, [r3, #4]
00336498: ldr      r6, [r0, #0xc]
0033649c: cmp      r3, r6
003364a0: streq    r2, [r0, #0xc]
003364a4: strne    r2, [r0, #8]
003364a8: str      r3, [r2, #0xc]
003364ac: str      r2, [r3, #4]
003364b0: ldr      r3, [ip, #0xc]
003364b4: b        #0x336254
003364b8: ldr      r6, [r0, #0xc]
003364bc: cmp      r6, #0
003364c0: ldreq    r6, [r0, #4]
003364c4: streq    r6, [r2]
003364c8: beq      #0x3363f0
003364cc: mov      r7, r4
003364d0: b        #0x3364d8
003364d4: mov      r7, r6
003364d8: ldr      r6, [r7, #8]
003364dc: cmp      r6, #0
003364e0: bne      #0x3364d4
003364e4: str      r7, [r2]
003364e8: b        #0x3363f0
