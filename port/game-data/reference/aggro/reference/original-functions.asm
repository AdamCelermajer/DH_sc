
# _ZNK6CharAI11AI_GetAggroEP9Character
003d4ac8: push     {r4, r5, lr}
003d4acc: ldr      r3, [pc, #0xd8]
003d4ad0: subs     r4, r1, #0
003d4ad4: sub      sp, sp, #0xc
003d4ad8: mov      r5, r0
003d4adc: add      r3, pc, r3
003d4ae0: beq      #0x3d4b50
003d4ae4: ldr      r3, [r5, #0x80]
003d4ae8: add      r5, r5, #0x7c
003d4aec: cmp      r3, #0
003d4af0: beq      #0x3d4b48
003d4af4: mov      r1, r5
003d4af8: b        #0x3d4b00
003d4afc: mov      r3, r2
003d4b00: ldr      r2, [r3, #0x10]
003d4b04: cmp      r4, r2
003d4b08: ldrhi    r2, [r3, #0xc]
003d4b0c: ldrls    r2, [r3, #8]
003d4b10: movhi    r3, r1
003d4b14: mov      r1, r3
003d4b18: cmp      r2, #0
003d4b1c: bne      #0x3d4afc
003d4b20: cmp      r5, r3
003d4b24: beq      #0x3d4ba4
003d4b28: ldr      r2, [r3, #0x10]
003d4b2c: cmp      r4, r2
003d4b30: blo      #0x3d4b48
003d4b34: cmp      r5, r3
003d4b38: ldrne    r0, [r3, #0x14]
003d4b3c: beq      #0x3d4ba4
003d4b40: add      sp, sp, #0xc
003d4b44: pop      {r4, r5, pc}
003d4b48: mov      r3, r5
003d4b4c: b        #0x3d4b34
003d4b50: ldr      r2, [pc, #0x58]
003d4b54: ldr      r2, [r3, r2]
003d4b58: ldr      r2, [r2]
003d4b5c: cmp      r2, #2
003d4b60: streq    r4, [r4]
003d4b64: beq      #0x3d4ae4
003d4b68: cmp      r2, #1
003d4b6c: bne      #0x3d4ae4
003d4b70: ldr      r0, [pc, #0x3c]
003d4b74: ldr      r1, [pc, #0x3c]
003d4b78: ldr      r2, [pc, #0x3c]
003d4b7c: ldr      r0, [r3, r0]
003d4b80: ldr      r3, [pc, #0x38]
003d4b84: movw     ip, #0x229
003d4b88: add      r1, pc, r1
003d4b8c: add      r2, pc, r2
003d4b90: add      r3, pc, r3
003d4b94: add      r0, r0, #0xa8
003d4b98: str      ip, [sp]
003d4b9c: bl       #0x30e004
003d4ba0: b        #0x3d4ae4
003d4ba4: mov      r0, #0
003d4ba8: b        #0x3d4b40
003d4bac: ldrheq   pc, [fp], #-0xf4
003d4bb0: andeq    r3, r0, r0, asr #19
003d4bb4: andeq    r1, r0, r0, asr #19
003d4bb8: subeq    sb, lr, r0, asr r8
003d4bbc: ldrsheq  sp, [r1], #-0x4c
003d4bc0: subeq    r0, pc, r0, lsr sl

# _ZN6CharAI13_UpdateTargetEv
003cb908: push     {r4, r5, r6, lr}
003cb90c: mov      r4, r0
003cb910: ldr      r0, [r0, #4]
003cb914: add      r0, r0, #0x4f0
003cb918: add      r0, r0, #0xc
003cb91c: bl       #0x3c0230
003cb920: cmp      r0, #0
003cb924: beq      #0x3cb92c
003cb928: pop      {r4, r5, r6, pc}
003cb92c: ldr      r0, [r4, #4]
003cb930: add      r0, r0, #0x4f0
003cb934: add      r0, r0, #0xc
003cb938: bl       #0x3c01c0
003cb93c: cmp      r0, #0
003cb940: bne      #0x3cb928
003cb944: ldr      r3, [r4, #0x40]
003cb948: cmp      r3, #0
003cb94c: beq      #0x3cb928
003cb950: mov      r0, r3
003cb954: ldr      r1, [r4, #4]
003cb958: ldr      r3, [r3]
003cb95c: mov      lr, pc
003cb960: ldr      pc, [r3, #0x88]
003cb964: subs     r2, r0, #0
003cb968: beq      #0x3cbaa4
003cb96c: ldr      r3, [r4, #0x40]
003cb970: cmp      r3, #0
003cb974: beq      #0x3cb928
003cb978: ldr      r0, [r4, #4]
003cb97c: bl       #0x3a2fec
003cb980: ldr      r3, [r4, #0x40]
003cb984: mov      r0, r3
003cb988: ldr      r3, [r3]
003cb98c: mov      lr, pc
003cb990: ldr      pc, [r3, #0x34]
003cb994: ldrb     r3, [r4, #0x48]
003cb998: eor      r0, r0, #1
003cb99c: uxtb     r5, r0
003cb9a0: cmp      r3, #0
003cb9a4: bne      #0x3cba6c
003cb9a8: cmp      r5, #0
003cb9ac: bne      #0x3cbaf8
003cb9b0: ldr      r1, [r4, #0x40]
003cb9b4: strb     r5, [r4, #0x48]
003cb9b8: cmp      r1, #0
003cb9bc: beq      #0x3cb928
003cb9c0: mov      r0, r4
003cb9c4: bl       #0x3d4ed8
003cb9c8: ldrb     r3, [r4, #0x49]
003cb9cc: mov      r5, r0
003cb9d0: cmp      r3, #0
003cb9d4: bne      #0x3cba88
003cb9d8: cmp      r0, #0
003cb9dc: bne      #0x3cbae4
003cb9e0: ldr      r3, [r4, #0x40]
003cb9e4: strb     r5, [r4, #0x49]
003cb9e8: cmp      r3, #0
003cb9ec: beq      #0x3cb928
003cb9f0: cmp      r5, #0
003cb9f4: beq      #0x3cb928
003cb9f8: mov      r0, r3
003cb9fc: ldr      r1, [r4, #4]
003cba00: ldr      r3, [r3]
003cba04: mov      lr, pc
003cba08: ldr      pc, [r3, #0x88]
003cba0c: cmp      r0, #0
003cba10: beq      #0x3cb928
003cba14: ldr      r3, [r4, #4]
003cba18: mov      r0, r3
003cba1c: ldr      r3, [r3]
003cba20: mov      lr, pc
003cba24: ldr      pc, [r3, #0x124]
003cba28: cmp      r0, #0
003cba2c: beq      #0x3cbabc
003cba30: mov      r0, r4
003cba34: ldr      r1, [r4, #0x40]
003cba38: bl       #0x3d63d8
003cba3c: cmp      r0, #0
003cba40: bne      #0x3cbb0c
003cba44: mov      r0, r4
003cba48: ldr      r1, [r4, #0x40]
003cba4c: bl       #0x3d6604
003cba50: cmp      r0, #0
003cba54: beq      #0x3cbad0
003cba58: ldr      r2, [r4, #0x40]
003cba5c: ldr      r0, [r4, #4]
003cba60: mov      r1, #0xf
003cba64: pop      {r4, r5, r6, lr}
003cba68: b        #0x3a4d5c
003cba6c: cmp      r5, #0
003cba70: bne      #0x3cb9b0
003cba74: ldr      r0, [r4, #4]
003cba78: mov      r1, #0xa
003cba7c: ldr      r2, [r4, #0x40]
003cba80: bl       #0x3a4d5c
003cba84: b        #0x3cb9b0
003cba88: cmp      r0, #0
003cba8c: bne      #0x3cb9e0
003cba90: ldr      r0, [r4, #4]
003cba94: mov      r1, #0xc
003cba98: ldr      r2, [r4, #0x40]
003cba9c: bl       #0x3a4d5c
003cbaa0: b        #0x3cb9e0
003cbaa4: ldr      r0, [r4, #4]
003cbaa8: mov      r1, #0xc
003cbaac: str      r2, [r4, #0x40]
003cbab0: str      r2, [r4, #0x44]
003cbab4: pop      {r4, r5, r6, lr}
003cbab8: b        #0x3a4d5c
003cbabc: mov      r0, r4
003cbac0: ldr      r1, [r4, #0x40]
003cbac4: bl       #0x3d6188
003cbac8: cmp      r0, #0
003cbacc: bne      #0x3cbb20
003cbad0: ldr      r2, [r4, #0x40]
003cbad4: ldr      r0, [r4, #4]
003cbad8: mov      r1, #0xe
003cbadc: pop      {r4, r5, r6, lr}
003cbae0: b        #0x3a4d5c
003cbae4: ldr      r0, [r4, #4]
003cbae8: mov      r1, #0xd
003cbaec: ldr      r2, [r4, #0x40]
003cbaf0: bl       #0x3a4d5c
003cbaf4: b        #0x3cb9e0
003cbaf8: ldr      r0, [r4, #4]
003cbafc: mov      r1, #0xb
003cbb00: ldr      r2, [r4, #0x40]
003cbb04: bl       #0x3a4d5c
003cbb08: b        #0x3cb9b0
003cbb0c: ldr      r2, [r4, #0x40]
003cbb10: ldr      r0, [r4, #4]
003cbb14: mov      r1, #0x10
003cbb18: pop      {r4, r5, r6, lr}
003cbb1c: b        #0x3a4d5c
003cbb20: ldr      r2, [r4, #0x40]
003cbb24: ldr      r0, [r4, #4]
003cbb28: mov      r1, #0x11
003cbb2c: pop      {r4, r5, r6, lr}
003cbb30: b        #0x3a4d5c

# _ZNSt4priv8_Rb_treeIP9CharacterSt4lessIS2_ESt4pairIKS2_fENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS7_SF_SF_
003d73cc: cmp      r1, r2
003d73d0: push     {r4, r5, r6, r7, r8, lr}
003d73d4: mov      r4, r1
003d73d8: mov      r7, r2
003d73dc: mov      r5, r0
003d73e0: mov      r8, r3
003d73e4: beq      #0x3d74a4
003d73e8: ldr      r3, [sp, #0x1c]
003d73ec: cmp      r3, #0
003d73f0: beq      #0x3d7454
003d73f4: mov      r0, r4
003d73f8: bl       #0x3d73ac
003d73fc: ldr      r2, [r8]
003d7400: mov      r3, #0
003d7404: mov      r6, r0
003d7408: str      r2, [r0, #0x10]
003d740c: ldr      r2, [r8, #4]
003d7410: str      r3, [r0, #0xc]
003d7414: str      r3, [r0, #8]
003d7418: str      r2, [r0, #0x14]
003d741c: str      r0, [r7, #0xc]
003d7420: ldr      r3, [r4, #0xc]
003d7424: cmp      r7, r3
003d7428: beq      #0x3d749c
003d742c: mov      r0, r6
003d7430: str      r7, [r6, #4]
003d7434: add      r1, r4, #4
003d7438: bl       #0x313760
003d743c: ldr      r3, [r4, #0x10]
003d7440: mov      r0, r5
003d7444: add      r3, r3, #1
003d7448: str      r3, [r4, #0x10]
003d744c: str      r6, [r5]
003d7450: pop      {r4, r5, r6, r7, r8, pc}
003d7454: ldr      r3, [sp, #0x18]
003d7458: cmp      r3, #0
003d745c: beq      #0x3d74dc
003d7460: mov      r0, r4
003d7464: bl       #0x3d73ac
003d7468: ldr      r2, [r8]
003d746c: mov      r3, #0
003d7470: mov      r6, r0
003d7474: str      r2, [r0, #0x10]
003d7478: ldr      r2, [r8, #4]
003d747c: str      r3, [r0, #0xc]
003d7480: str      r3, [r0, #8]
003d7484: str      r2, [r0, #0x14]
003d7488: str      r0, [r7, #8]
003d748c: ldr      r3, [r4, #8]
003d7490: cmp      r7, r3
003d7494: streq    r0, [r4, #8]
003d7498: b        #0x3d742c
003d749c: str      r6, [r4, #0xc]
003d74a0: b        #0x3d742c
003d74a4: mov      r0, r1
003d74a8: bl       #0x3d73ac
003d74ac: ldr      r2, [r8]
003d74b0: mov      r3, #0
003d74b4: mov      r6, r0
003d74b8: str      r2, [r0, #0x10]
003d74bc: ldr      r2, [r8, #4]
003d74c0: str      r3, [r0, #0xc]
003d74c4: str      r3, [r0, #8]
003d74c8: str      r2, [r0, #0x14]
003d74cc: str      r0, [r4, #8]
003d74d0: str      r0, [r4, #4]
003d74d4: str      r0, [r4, #0xc]
003d74d8: b        #0x3d742c
003d74dc: ldr      r2, [r8]
003d74e0: ldr      r3, [r7, #0x10]
003d74e4: cmp      r2, r3
003d74e8: bhs      #0x3d73f4
003d74ec: b        #0x3d7460

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

# _ZNSt4priv8_Rb_treeIP9CharacterSt4lessIS2_ESt4pairIKS2_fENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueENS_17_Rb_tree_iteratorIS7_SB_EERKS7_
003d7678: push     {r4, r5, r6, r7, r8, sl, lr}
003d767c: ldr      r4, [r2]
003d7680: ldr      r2, [r1, #8]
003d7684: sub      sp, sp, #0x2c
003d7688: mov      r5, r1
003d768c: cmp      r4, r2
003d7690: mov      r7, r0
003d7694: mov      r6, r3
003d7698: beq      #0x3d7808
003d769c: cmp      r4, r1
003d76a0: beq      #0x3d7888
003d76a4: ldrb     r3, [r4]
003d76a8: cmp      r3, #0
003d76ac: beq      #0x3d779c
003d76b0: ldr      ip, [r4, #8]
003d76b4: cmp      ip, #0
003d76b8: bne      #0x3d76c4
003d76bc: b        #0x3d77bc
003d76c0: mov      ip, r3
003d76c4: ldr      r3, [ip, #0xc]
003d76c8: cmp      r3, #0
003d76cc: bne      #0x3d76c0
003d76d0: ldr      r2, [r6]
003d76d4: ldr      r0, [r4, #0x10]
003d76d8: cmp      r2, r0
003d76dc: movhs    r1, #0
003d76e0: movlo    r1, #1
003d76e4: cmp      r1, #0
003d76e8: bne      #0x3d775c
003d76ec: ldr      r8, [r4, #0xc]
003d76f0: cmp      r8, #0
003d76f4: beq      #0x3d78f0
003d76f8: mov      ip, r8
003d76fc: b        #0x3d7704
003d7700: mov      ip, r3
003d7704: ldr      r3, [ip, #8]
003d7708: cmp      r3, #0
003d770c: bne      #0x3d7700
003d7710: cmp      r1, #0
003d7714: bne      #0x3d77ec
003d7718: cmp      r2, r0
003d771c: bls      #0x3d78b0
003d7720: cmp      r5, ip
003d7724: beq      #0x3d7734
003d7728: ldr      r3, [ip, #0x10]
003d772c: cmp      r2, r3
003d7730: bhs      #0x3d77ec
003d7734: cmp      r8, #0
003d7738: bne      #0x3d7868
003d773c: mov      r1, r5
003d7740: mov      r2, r4
003d7744: mov      r3, r6
003d7748: mov      r0, r7
003d774c: str      r8, [sp]
003d7750: str      r4, [sp, #4]
003d7754: bl       #0x3d73cc
003d7758: b        #0x3d7790
003d775c: ldr      r3, [ip, #0x10]
003d7760: cmp      r2, r3
003d7764: bls      #0x3d76ec
003d7768: ldr      lr, [ip, #0xc]
003d776c: cmp      lr, #0
003d7770: beq      #0x3d78d0
003d7774: mov      ip, #0
003d7778: mov      r1, r5
003d777c: mov      r2, r4
003d7780: mov      r3, r6
003d7784: mov      r0, r7
003d7788: stm      sp, {r4, ip}
003d778c: bl       #0x3d73cc
003d7790: mov      r0, r7
003d7794: add      sp, sp, #0x2c
003d7798: pop      {r4, r5, r6, r7, r8, sl, pc}
003d779c: ldr      r3, [r4, #4]
003d77a0: ldr      r3, [r3, #4]
003d77a4: cmp      r4, r3
003d77a8: ldreq    ip, [r4, #0xc]
003d77ac: beq      #0x3d76d0
003d77b0: ldr      ip, [r4, #8]
003d77b4: cmp      ip, #0
003d77b8: bne      #0x3d76c4
003d77bc: ldr      ip, [r4, #4]
003d77c0: ldr      r3, [ip, #8]
003d77c4: cmp      r4, r3
003d77c8: beq      #0x3d77d4
003d77cc: b        #0x3d76d0
003d77d0: mov      ip, r3
003d77d4: ldr      r3, [ip, #4]
003d77d8: ldr      r2, [r3, #8]
003d77dc: cmp      r2, ip
003d77e0: beq      #0x3d77d0
003d77e4: mov      ip, r3
003d77e8: b        #0x3d76d0
003d77ec: mov      r1, r5
003d77f0: mov      r2, r6
003d77f4: add      r0, sp, #8
003d77f8: bl       #0x3d74f0
003d77fc: ldr      r3, [sp, #8]
003d7800: str      r3, [r7]
003d7804: b        #0x3d7790
003d7808: ldr      r2, [r1, #0x10]
003d780c: cmp      r2, #0
003d7810: beq      #0x3d7960
003d7814: ldr      r2, [r3]
003d7818: ldr      ip, [r4, #0x10]
003d781c: cmp      r2, ip
003d7820: blo      #0x3d7978
003d7824: bls      #0x3d78b0
003d7828: ldr      lr, [r4, #0xc]
003d782c: cmp      lr, #0
003d7830: beq      #0x3d7928
003d7834: mov      ip, lr
003d7838: b        #0x3d7840
003d783c: mov      ip, r3
003d7840: ldr      r3, [ip, #8]
003d7844: cmp      r3, #0
003d7848: bne      #0x3d783c
003d784c: cmp      r5, ip
003d7850: beq      #0x3d79c8
003d7854: ldr      r3, [ip, #0x10]
003d7858: cmp      r2, r3
003d785c: bhs      #0x3d798c
003d7860: cmp      lr, #0
003d7864: beq      #0x3d79a8
003d7868: mov      lr, #0
003d786c: mov      r1, r5
003d7870: mov      r2, ip
003d7874: mov      r3, r6
003d7878: mov      r0, r7
003d787c: stm      sp, {ip, lr}
003d7880: bl       #0x3d73cc
003d7884: b        #0x3d7790
003d7888: ldr      r2, [r4, #0xc]
003d788c: ldr      ip, [r3]
003d7890: ldr      lr, [r2, #0x10]
003d7894: cmp      lr, ip
003d7898: bhs      #0x3d78b8
003d789c: mov      ip, #0
003d78a0: str      ip, [sp]
003d78a4: str      r4, [sp, #4]
003d78a8: bl       #0x3d73cc
003d78ac: b        #0x3d7790
003d78b0: str      r4, [r7]
003d78b4: b        #0x3d7790
003d78b8: mov      r2, r3
003d78bc: add      r0, sp, #0x10
003d78c0: bl       #0x3d74f0
003d78c4: ldr      r3, [sp, #0x10]
003d78c8: str      r3, [r7]
003d78cc: b        #0x3d7790
003d78d0: mov      r1, r5
003d78d4: mov      r2, ip
003d78d8: mov      r3, r6
003d78dc: mov      r0, r7
003d78e0: str      lr, [sp]
003d78e4: str      ip, [sp, #4]
003d78e8: bl       #0x3d73cc
003d78ec: b        #0x3d7790
003d78f0: ldr      r3, [r4, #4]
003d78f4: ldr      ip, [r3, #0xc]
003d78f8: cmp      r4, ip
003d78fc: movne    ip, r4
003d7900: bne      #0x3d7918
003d7904: mov      ip, r3
003d7908: ldr      r3, [r3, #4]
003d790c: ldr      sl, [r3, #0xc]
003d7910: cmp      ip, sl
003d7914: beq      #0x3d7904
003d7918: ldr      sl, [ip, #0xc]
003d791c: cmp      r3, sl
003d7920: movne    ip, r3
003d7924: b        #0x3d7710
003d7928: ldr      r3, [r4, #4]
003d792c: ldr      r1, [r3, #0xc]
003d7930: cmp      r4, r1
003d7934: movne    ip, r4
003d7938: bne      #0x3d7950
003d793c: mov      ip, r3
003d7940: ldr      r3, [r3, #4]
003d7944: ldr      r1, [r3, #0xc]
003d7948: cmp      r1, ip
003d794c: beq      #0x3d793c
003d7950: ldr      r1, [ip, #0xc]
003d7954: cmp      r3, r1
003d7958: movne    ip, r3
003d795c: b        #0x3d784c
003d7960: mov      r2, r3
003d7964: add      r0, sp, #0x20
003d7968: bl       #0x3d74f0
003d796c: ldr      r3, [sp, #0x20]
003d7970: str      r3, [r7]
003d7974: b        #0x3d7790
003d7978: mov      ip, #0
003d797c: mov      r2, r4
003d7980: stm      sp, {r4, ip}
003d7984: bl       #0x3d73cc
003d7988: b        #0x3d7790
003d798c: mov      r1, r5
003d7990: mov      r2, r6
003d7994: add      r0, sp, #0x18
003d7998: bl       #0x3d74f0
003d799c: ldr      r3, [sp, #0x18]
003d79a0: str      r3, [r7]
003d79a4: b        #0x3d7790
003d79a8: mov      r1, r5
003d79ac: mov      r2, r4
003d79b0: mov      r3, r6
003d79b4: mov      r0, r7
003d79b8: str      lr, [sp]
003d79bc: str      r4, [sp, #4]
003d79c0: bl       #0x3d73cc
003d79c4: b        #0x3d7790
003d79c8: mov      ip, #0
003d79cc: mov      r1, r5
003d79d0: mov      r2, r4
003d79d4: mov      r3, r6
003d79d8: mov      r0, r7
003d79dc: str      ip, [sp]
003d79e0: str      r4, [sp, #4]
003d79e4: bl       #0x3d73cc
003d79e8: b        #0x3d7790

# _ZNK6CharAI16AI_GetAggroCountEv
003d4a10: ldr      r0, [r0, #0x8c]
003d4a14: bx       lr

# _ZNK6CharAI12AI_IsAggroedEv
003d4a00: ldr      r0, [r0, #0xa4]
003d4a04: subs     r0, r0, #0
003d4a08: movne    r0, #1
003d4a0c: bx       lr

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

# _ZN10AISDefault8OnAttackEiib
003dc12c: push     {r4, r5, r6, r7, lr}
003dc130: mov      r4, r0
003dc134: ldr      r0, [r0, #0x98]
003dc138: sub      sp, sp, #0x34
003dc13c: mov      r6, r3
003dc140: add      r0, r0, #0x3c8
003dc144: bl       #0x3d5450
003dc148: subs     r5, r0, #0
003dc14c: beq      #0x3dc18c
003dc150: add      r7, sp, #8
003dc154: ldr      r1, [r4, #0x98]
003dc158: mov      r3, r6
003dc15c: mov      r2, r5
003dc160: mov      r6, #0
003dc164: mov      r0, r7
003dc168: str      r6, [sp]
003dc16c: bl       #0x3b3368
003dc170: mov      r0, r7
003dc174: ldr      r1, [r4, #0x98]
003dc178: mov      r2, r5
003dc17c: mov      r3, r6
003dc180: bl       #0x3b10b4
003dc184: add      sp, sp, #0x34
003dc188: pop      {r4, r5, r6, r7, pc}
003dc18c: ldr      r1, [r4, #0x98]
003dc190: ldr      r3, [r1, #0x408]
003dc194: cmp      r3, #0
003dc198: beq      #0x3dc184
003dc19c: mov      r0, r3
003dc1a0: ldr      r3, [r3]
003dc1a4: mov      lr, pc
003dc1a8: ldr      pc, [r3, #0x98]
003dc1ac: b        #0x3dc184

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr

# _ZNK6CharAI11AI_HasAggroEv
003d49f0: ldr      r0, [r0, #0x8c]
003d49f4: subs     r0, r0, #0
003d49f8: movne    r0, #1
003d49fc: bx       lr

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

# _ZNK6CharAI18AI_GetHighestAggroEv
003d4a18: push     {r4, r5, r6, r7, r8, lr}
003d4a1c: ldr      r4, [r0, #0x84]
003d4a20: add      r7, r0, #0x7c
003d4a24: cmp      r4, r7
003d4a28: moveq    r8, #0
003d4a2c: beq      #0x3d4ac0
003d4a30: mov      r6, #0
003d4a34: mov      r8, #0
003d4a38: ldr      r5, [r4, #0x14]
003d4a3c: mov      r1, r6
003d4a40: mov      r0, r5
003d4a44: bl       #0x30e2f8
003d4a48: ldr      r2, [r4, #0xc]
003d4a4c: cmp      r0, #0
003d4a50: moveq    r5, r6
003d4a54: ldrne    r8, [r4, #0x10]
003d4a58: cmp      r2, #0
003d4a5c: beq      #0x3d4a88
003d4a60: mov      r4, r2
003d4a64: b        #0x3d4a6c
003d4a68: mov      r4, r3
003d4a6c: ldr      r3, [r4, #8]
003d4a70: cmp      r3, #0
003d4a74: bne      #0x3d4a68
003d4a78: cmp      r7, r4
003d4a7c: beq      #0x3d4ac0
003d4a80: mov      r6, r5
003d4a84: b        #0x3d4a38
003d4a88: ldr      r3, [r4, #4]
003d4a8c: ldr      r1, [r3, #0xc]
003d4a90: cmp      r1, r4
003d4a94: bne      #0x3d4ab0
003d4a98: mov      r4, r3
003d4a9c: ldr      r3, [r3, #4]
003d4aa0: ldr      r2, [r3, #0xc]
003d4aa4: cmp      r2, r4
003d4aa8: beq      #0x3d4a98
003d4aac: ldr      r2, [r4, #0xc]
003d4ab0: cmp      r2, r3
003d4ab4: movne    r4, r3
003d4ab8: cmp      r7, r4
003d4abc: bne      #0x3d4a80
003d4ac0: mov      r0, r8
003d4ac4: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6CharAI11AI_AddAggroEP9Characterf
003d7c68: push     {r4, r5, r6, r7, lr}
003d7c6c: ldr      r3, [pc, #0x110]
003d7c70: subs     r4, r1, #0
003d7c74: sub      sp, sp, #0xc
003d7c78: mov      r5, r0
003d7c7c: add      r3, pc, r3
003d7c80: mov      r6, r2
003d7c84: beq      #0x3d7d18
003d7c88: ldr      r3, [r5, #0x80]
003d7c8c: add      r0, r5, #0x7c
003d7c90: cmp      r3, #0
003d7c94: beq      #0x3d7d10
003d7c98: mov      r1, r0
003d7c9c: b        #0x3d7ca4
003d7ca0: mov      r3, r2
003d7ca4: ldr      r2, [r3, #0x10]
003d7ca8: cmp      r4, r2
003d7cac: ldrhi    r2, [r3, #0xc]
003d7cb0: ldrls    r2, [r3, #8]
003d7cb4: movhi    r3, r1
003d7cb8: mov      r1, r3
003d7cbc: cmp      r2, #0
003d7cc0: bne      #0x3d7ca0
003d7cc4: cmp      r0, r3
003d7cc8: beq      #0x3d7d6c
003d7ccc: ldr      r2, [r3, #0x10]
003d7cd0: cmp      r4, r2
003d7cd4: blo      #0x3d7d10
003d7cd8: cmp      r0, r3
003d7cdc: beq      #0x3d7d6c
003d7ce0: ldr      r7, [r3, #0x14]
003d7ce4: mov      r0, r6
003d7ce8: mov      r1, r7
003d7cec: bl       #0x30eba4
003d7cf0: mov      r1, r4
003d7cf4: mov      r2, r0
003d7cf8: mov      r0, r5
003d7cfc: bl       #0x3d79ec
003d7d00: mov      r1, r7
003d7d04: bl       #0x30e3ac
003d7d08: add      sp, sp, #0xc
003d7d0c: pop      {r4, r5, r6, r7, pc}
003d7d10: mov      r3, r0
003d7d14: b        #0x3d7cd8
003d7d18: ldr      r2, [pc, #0x68]
003d7d1c: ldr      r2, [r3, r2]
003d7d20: ldr      r2, [r2]
003d7d24: cmp      r2, #2
003d7d28: streq    r4, [r4]
003d7d2c: beq      #0x3d7c88
003d7d30: cmp      r2, #1
003d7d34: bne      #0x3d7c88
003d7d38: ldr      r0, [pc, #0x4c]
003d7d3c: ldr      r1, [pc, #0x4c]
003d7d40: ldr      r2, [pc, #0x4c]
003d7d44: ldr      r0, [r3, r0]
003d7d48: ldr      r3, [pc, #0x48]
003d7d4c: movw     ip, #0x292
003d7d50: add      r1, pc, r1
003d7d54: add      r2, pc, r2
003d7d58: add      r3, pc, r3
003d7d5c: add      r0, r0, #0xa8
003d7d60: str      ip, [sp]
003d7d64: bl       #0x30e004
003d7d68: b        #0x3d7c88
003d7d6c: mov      r0, r5
003d7d70: mov      r1, r4
003d7d74: mov      r2, r6
003d7d78: add      sp, sp, #0xc
003d7d7c: pop      {r4, r5, r6, r7, lr}
003d7d80: b        #0x3d79ec
003d7d84: subseq   ip, fp, r4, lsl lr
003d7d88: andeq    r3, r0, r0, asr #19
003d7d8c: andeq    r1, r0, r0, asr #19
003d7d90: subeq    r6, lr, r8, lsl #13
003d7d94: subseq   sl, r1, r4, lsr r3
003d7d98: subeq    sp, lr, r8, ror #16
