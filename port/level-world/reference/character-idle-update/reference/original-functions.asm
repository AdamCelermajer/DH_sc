
# _ZN13PlayerManager13GetNumPlayersEv
0036d7a8: push     {r4, lr}
0036d7ac: mov      r4, r0
0036d7b0: bl       #0x7fd794
0036d7b4: ldrb     r3, [r0, #5]
0036d7b8: cmp      r3, #0
0036d7bc: bne      #0x36d7c8
0036d7c0: ldr      r0, [r4, #0x6a0]
0036d7c4: pop      {r4, pc}
0036d7c8: bl       #0x320e98
0036d7cc: ldrb     r3, [r0, #0x24]
0036d7d0: cmp      r3, #0
0036d7d4: beq      #0x36d7c0
0036d7d8: bl       #0x800f8c
0036d7dc: ldr      r3, [r0]
0036d7e0: mov      lr, pc
0036d7e4: ldr      pc, [r3, #0x64]
0036d7e8: cmp      r0, #0
0036d7ec: beq      #0x36d7c0
0036d7f0: bl       #0x8100dc
0036d7f4: bl       #0x8100e0
0036d7f8: cmp      r0, #0
0036d7fc: beq      #0x36d7c0
0036d800: ldr      r3, [r4, #0x6a8]
0036d804: ldr      r0, [r4, #0x6ac]
0036d808: rsb      r0, r3, r0
0036d80c: asr      r0, r0, #2
0036d810: pop      {r4, pc}

# _ZN9Character13_GetStateTimeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b6d6c: mov      r0, r1
003b6d70: ldr      r1, [r2, #0x55c]
003b6d74: b        #0x37cb24

# _ZN6CSIdle8OnUpdateEiP9CharacterP16CharStateMachine
003c0e80: mov      r0, r1
003c0e84: mov      r1, r2
003c0e88: mov      r2, r3
003c0e8c: b        #0x3c0b78

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c

# _ZN16CharStateMachine6UpdateEv
003c628c: push     {r4, r5, r6, lr}
003c6290: mov      r4, r0
003c6294: ldr      r0, [pc, #0xe8]
003c6298: sub      sp, sp, #8
003c629c: ldr      r5, [pc, #0xe4]
003c62a0: add      r0, pc, r0
003c62a4: bl       #0x3136b4
003c62a8: ldr      r3, [pc, #0xdc]
003c62ac: add      r5, pc, r5
003c62b0: ldr      r6, [r4, #0x60]
003c62b4: ldr      r0, [r5, r3]
003c62b8: bl       #0x31f66c
003c62bc: ldr      r3, [r4, #0x2c]
003c62c0: add      r0, r0, r6
003c62c4: str      r0, [r4, #0x60]
003c62c8: tst      r3, #2
003c62cc: bne      #0x3c6314
003c62d0: tst      r3, #4
003c62d4: bne      #0x3c6350
003c62d8: ldr      r3, [r4, #0x20]
003c62dc: cmp      r3, #0
003c62e0: beq      #0x3c6300
003c62e4: ldm      r3, {r1, r2}
003c62e8: mov      r3, r4
003c62ec: mov      r0, r2
003c62f0: ldr      ip, [r2]
003c62f4: ldr      r2, [r4, #4]
003c62f8: mov      lr, pc
003c62fc: ldr      pc, [ip, #0x14]
003c6300: ldr      r0, [pc, #0x88]
003c6304: add      r0, pc, r0
003c6308: add      sp, sp, #8
003c630c: pop      {r4, r5, r6, lr}
003c6310: b        #0x3136b8
003c6314: mov      r0, r4
003c6318: mov      r1, #0
003c631c: bl       #0x3c0378
003c6320: subs     ip, r0, #0
003c6324: bne      #0x3c6344
003c6328: ldr      r2, [r4, #0x24]
003c632c: mov      r3, ip
003c6330: mov      r0, r4
003c6334: ubfx     r2, r2, #0xb, #1
003c6338: mvn      r1, #0
003c633c: str      ip, [sp]
003c6340: bl       #0x3c5ffc
003c6344: ldr      r3, [r4, #0x2c]
003c6348: tst      r3, #4
003c634c: beq      #0x3c62d8
003c6350: mov      r0, r4
003c6354: mov      r1, #0
003c6358: bl       #0x3c034c
003c635c: subs     ip, r0, #0
003c6360: bne      #0x3c62d8
003c6364: ldr      r2, [r4, #0x24]
003c6368: mov      r3, ip
003c636c: mov      r0, r4
003c6370: ubfx     r2, r2, #0xa, #1
003c6374: mvn      r1, #0
003c6378: str      ip, [sp]
003c637c: bl       #0x3c6144
003c6380: b        #0x3c62d8
003c6384: subeq    lr, pc, r0, lsr #25
003c6388: subseq   lr, ip, r4, ror #15
003c638c: strdeq   r3, r4, [r0], -r4
003c6390: subeq    lr, pc, ip, lsr ip

# _ZNK9Character8IsPlayerEv
003a49f0: push     {r4, r5, r6, lr}
003a49f4: mov      r5, r0
003a49f8: bl       #0x3a3054
003a49fc: cmp      r0, #0
003a4a00: beq      #0x3a4a14
003a4a04: cmp      r0, #1
003a4a08: movne    r0, #0
003a4a0c: moveq    r0, #1
003a4a10: pop      {r4, r5, r6, pc}
003a4a14: ldr      r4, [r5, #0x44]
003a4a18: ldr      r1, [pc, #0x18]
003a4a1c: mov      r0, r4
003a4a20: add      r1, pc, r1
003a4a24: bl       #0x30ebd4
003a4a28: cmp      r4, r0
003a4a2c: movne    r0, #0
003a4a30: moveq    r0, #1
003a4a34: pop      {r4, r5, r6, pc}
003a4a38: subseq   lr, r1, r8, asr #14

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN12v2Controller10Cmd_MoveToERK7Point3DIfE
004054e4: push     {r4, lr}
004054e8: ldrb     r2, [r0, #9]
004054ec: ldr      r3, [pc, #0x44]
004054f0: cmp      r2, #0
004054f4: add      r3, pc, r3
004054f8: bne      #0x405520
004054fc: ldr      r2, [pc, #0x38]
00405500: ldr      r3, [r3, r2]
00405504: ldrb     r3, [r3]
00405508: cmp      r3, #0
0040550c: beq      #0x405514
00405510: pop      {r4, pc}
00405514: ldrb     r3, [r0, #8]
00405518: cmp      r3, #0
0040551c: bne      #0x405510
00405520: ldr      r3, [r0, #4]
00405524: mov      r0, r3
00405528: ldr      r3, [r3]
0040552c: mov      lr, pc
00405530: ldr      pc, [r3, #0x2c]
00405534: pop      {r4, pc}

# _ZN6CSIdle16IdleCommonUpdateEiP9CharacterP16CharStateMachine
003c0b78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c0b7c: ldrb     r3, [r1, #0x1b5]
003c0b80: ldr      r5, [pc, #0x2e0]
003c0b84: sub      sp, sp, #0x4c
003c0b88: cmp      r3, #0
003c0b8c: mov      r4, r1
003c0b90: add      r5, pc, r5
003c0b94: bne      #0x3c0bb8
003c0b98: ldr      r3, [r1]
003c0b9c: mov      r0, r1
003c0ba0: mov      lr, pc
003c0ba4: ldr      pc, [r3, #0x28]
003c0ba8: cmp      r0, #0
003c0bac: bne      #0x3c0bcc
003c0bb0: add      sp, sp, #0x4c
003c0bb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c0bb8: mov      r1, #0
003c0bbc: mov      r0, r4
003c0bc0: mov      r2, r1
003c0bc4: bl       #0x3a4d5c
003c0bc8: b        #0x3c0bb0
003c0bcc: bl       #0x7fd794
003c0bd0: ldrb     r6, [r0, #5]
003c0bd4: cmp      r6, #0
003c0bd8: bne      #0x3c0bb0
003c0bdc: ldr      r2, [pc, #0x288]
003c0be0: ldr      r1, [pc, #0x288]
003c0be4: ldr      r7, [r5, r2]
003c0be8: str      r2, [sp, #0x14]
003c0bec: ldr      r2, [pc, #0x280]
003c0bf0: add      r1, pc, r1
003c0bf4: ldr      r0, [r7, #0x2c]
003c0bf8: add      r2, pc, r2
003c0bfc: bl       #0x4c4bdc
003c0c00: ldr      r3, [r4, #0x55c]
003c0c04: mov      sb, r0
003c0c08: cmp      r0, r3
003c0c0c: bhi      #0x3c0bb0
003c0c10: ldr      r8, [r7, #0x40]
003c0c14: mov      r0, r8
003c0c18: bl       #0x36d7a8
003c0c1c: subs     sl, r0, #0
003c0c20: ble      #0x3c0bb0
003c0c24: ldr      r3, [pc, #0x24c]
003c0c28: add      r2, sp, #0x30
003c0c2c: str      r2, [sp, #0x24]
003c0c30: add      r3, pc, r3
003c0c34: str      r3, [sp, #0x28]
003c0c38: ldr      r3, [pc, #0x23c]
003c0c3c: mov      r7, r5
003c0c40: add      r3, pc, r3
003c0c44: str      r3, [sp, #0x2c]
003c0c48: add      r3, sp, #0x3c
003c0c4c: str      r3, [sp, #0x20]
003c0c50: b        #0x3c0c60
003c0c54: add      r6, r6, #1
003c0c58: cmp      r6, sl
003c0c5c: beq      #0x3c0bb0
003c0c60: mov      r0, r8
003c0c64: mov      r1, r6
003c0c68: mov      r2, #0
003c0c6c: bl       #0x36e744
003c0c70: ldr      r5, [r0, #0x660]
003c0c74: cmp      r5, #0
003c0c78: cmpne    r4, r5
003c0c7c: beq      #0x3c0c54
003c0c80: add      r0, r5, #0x4f0
003c0c84: add      r0, r0, #0xc
003c0c88: bl       #0x3c01ac
003c0c8c: cmp      r0, #3
003c0c90: bne      #0x3c0c54
003c0c94: ldr      r3, [r5, #0x55c]
003c0c98: cmp      sb, r3
003c0c9c: bhi      #0x3c0c54
003c0ca0: ldr      r2, [sp, #0x14]
003c0ca4: ldr      r1, [sp, #0x28]
003c0ca8: ldr      r3, [r7, r2]
003c0cac: ldr      r2, [sp, #0x2c]
003c0cb0: ldr      r0, [r3, #0x2c]
003c0cb4: bl       #0x4c4bdc
003c0cb8: bl       #0x30e964
003c0cbc: str      r0, [sp, #0xc]
003c0cc0: ldr      r1, [r5, #0x160]
003c0cc4: ldr      r0, [r4, #0x160]
003c0cc8: bl       #0x30e3ac
003c0ccc: str      r0, [sp, #0x10]
003c0cd0: ldr      r1, [r5, #0x164]
003c0cd4: ldr      r0, [r4, #0x164]
003c0cd8: bl       #0x30e3ac
003c0cdc: str      r0, [sp, #0x18]
003c0ce0: ldr      r1, [r5, #0x168]
003c0ce4: ldr      r0, [r4, #0x168]
003c0ce8: bl       #0x30e3ac
003c0cec: str      r0, [sp, #0x1c]
003c0cf0: ldr      r0, [sp, #0x10]
003c0cf4: mov      r1, r0
003c0cf8: bl       #0x30ed6c
003c0cfc: mov      fp, r0
003c0d00: ldr      r0, [sp, #0x18]
003c0d04: mov      r1, r0
003c0d08: bl       #0x30ed6c
003c0d0c: mov      r1, r0
003c0d10: mov      r0, fp
003c0d14: bl       #0x30eba4
003c0d18: mov      fp, r0
003c0d1c: ldr      r0, [sp, #0x1c]
003c0d20: mov      r1, r0
003c0d24: bl       #0x30ed6c
003c0d28: mov      r1, r0
003c0d2c: mov      r0, fp
003c0d30: bl       #0x30eba4
003c0d34: mov      fp, r0
003c0d38: ldr      r0, [sp, #0xc]
003c0d3c: mov      r1, r0
003c0d40: bl       #0x30ed6c
003c0d44: mov      r1, fp
003c0d48: bl       #0x30e2f8
003c0d4c: cmp      r0, #0
003c0d50: beq      #0x3c0c54
003c0d54: mov      r0, fp
003c0d58: mov      r1, #0
003c0d5c: bl       #0x30e2f8
003c0d60: cmp      r0, #0
003c0d64: beq      #0x3c0c54
003c0d68: mov      r0, fp
003c0d6c: bl       #0x30e124
003c0d70: mov      fp, r0
003c0d74: mov      r1, fp
003c0d78: ldr      r0, [sp, #0xc]
003c0d7c: bl       #0x30e3ac
003c0d80: mov      r1, #0x3f400000
003c0d84: bl       #0x30ed6c
003c0d88: mov      r1, fp
003c0d8c: bl       #0x30ec94
003c0d90: ldr      r1, [sp, #0x10]
003c0d94: mov      fp, r0
003c0d98: bl       #0x30ed6c
003c0d9c: ldr      r1, [sp, #0x18]
003c0da0: str      r0, [sp, #0xc]
003c0da4: mov      r0, fp
003c0da8: bl       #0x30ed6c
003c0dac: ldr      r1, [sp, #0x1c]
003c0db0: str      r0, [sp, #0x10]
003c0db4: mov      r0, fp
003c0db8: bl       #0x30ed6c
003c0dbc: ldr      r1, [r4, #0x164]
003c0dc0: mov      fp, r0
003c0dc4: ldr      r0, [sp, #0x10]
003c0dc8: bl       #0x30eba4
003c0dcc: ldr      r1, [r4, #0x168]
003c0dd0: mov      r2, r0
003c0dd4: mov      r0, fp
003c0dd8: str      r2, [sp, #4]
003c0ddc: bl       #0x30eba4
003c0de0: ldr      r1, [r4, #0x160]
003c0de4: mov      ip, r0
003c0de8: ldr      r0, [sp, #0xc]
003c0dec: str      ip, [sp, #8]
003c0df0: bl       #0x30eba4
003c0df4: ldr      r3, [r4, #0x378]
003c0df8: ldmib    sp, {r2, ip}
003c0dfc: str      r0, [sp, #0x3c]
003c0e00: ldr      r1, [sp, #0x20]
003c0e04: mov      r0, r3
003c0e08: str      r2, [sp, #0x40]
003c0e0c: str      ip, [sp, #0x44]
003c0e10: bl       #0x4054e4
003c0e14: ldr      r0, [r5, #0x164]
003c0e18: ldr      r1, [sp, #0x10]
003c0e1c: bl       #0x30e3ac
003c0e20: mov      r1, fp
003c0e24: mov      r3, r0
003c0e28: ldr      r0, [r5, #0x168]
003c0e2c: str      r3, [sp, #8]
003c0e30: bl       #0x30e3ac
003c0e34: ldr      r1, [sp, #0xc]
003c0e38: mov      fp, r0
003c0e3c: ldr      r0, [r5, #0x160]
003c0e40: bl       #0x30e3ac
003c0e44: ldr      r2, [r5, #0x378]
003c0e48: ldr      r3, [sp, #8]
003c0e4c: str      r0, [sp, #0x30]
003c0e50: ldr      r1, [sp, #0x24]
003c0e54: mov      r0, r2
003c0e58: str      r3, [sp, #0x34]
003c0e5c: str      fp, [sp, #0x38]
003c0e60: bl       #0x4054e4
003c0e64: b        #0x3c0c54
003c0e68: subseq   r3, sp, r0, lsl #30
003c0e6c: strdeq   r3, r4, [r0], -r4
003c0e70: subseq   r0, r0, r0, ror #22
003c0e74: subseq   r3, r0, r0, lsl #31
003c0e78: subseq   r0, r0, r0, lsr #22
003c0e7c: subseq   r3, r0, r8, asr pc

# _ZN13PlayerManager9GetPlayerEib
0036e744: push     {r4, lr}
0036e748: mov      r4, r0
0036e74c: bl       #0x36e5b4
0036e750: mov      r2, #0
0036e754: mov      r1, r0
0036e758: mov      r0, r4
0036e75c: pop      {r4, lr}
0036e760: b        #0x36dfb0

# _Z9GetOnlinev
007fd794: b        #0x7fd744

# _ZN10GameObject19SetHeadingDirectionERK7Point3DIfEb
00393be8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00393bec: ldr      r3, [r1]
00393bf0: mov      r4, r0
00393bf4: mov      r6, #0
00393bf8: str      r3, [r0, #0x1b8]
00393bfc: ldr      r7, [r1, #4]
00393c00: mov      r0, r3
00393c04: str      r6, [r4, #0x1c0]
00393c08: str      r7, [r4, #0x1bc]
00393c0c: mov      r5, r1
00393c10: mov      r1, r3
00393c14: mov      sl, r2
00393c18: bl       #0x30ed6c
00393c1c: mov      r1, r7
00393c20: mov      r8, r0
00393c24: mov      r0, r7
00393c28: bl       #0x30ed6c
00393c2c: mov      r1, r0
00393c30: mov      r0, r8
00393c34: bl       #0x30eba4
00393c38: mov      r1, r6
00393c3c: bl       #0x30eba4
00393c40: movw     r1, #0xb717
00393c44: movt     r1, #0x38d1
00393c48: mov      r7, r0
00393c4c: bl       #0x30e2f8
00393c50: cmp      r0, #0
00393c54: mov      r6, #0
00393c58: movne    r6, #1
00393c5c: uxtb     r6, r6
00393c60: strb     r6, [r4, #0x1b5]
00393c64: mov      r0, r7
00393c68: mov      r1, #0x3f800000
00393c6c: bl       #0x30e2f8
00393c70: cmp      r0, #0
00393c74: bne      #0x393c9c
00393c78: cmp      r6, #0
00393c7c: beq      #0x393c88
00393c80: cmp      sl, #0
00393c84: bne      #0x393c8c
00393c88: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393c8c: mov      r0, r4
00393c90: mov      r1, r5
00393c94: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393c98: b        #0x393b1c
00393c9c: mov      r0, r7
00393ca0: bl       #0x30e124
00393ca4: mov      r1, r0
00393ca8: mov      r0, #0x3f800000
00393cac: bl       #0x30ec94
00393cb0: mov      r6, r0
00393cb4: mov      r1, r0
00393cb8: ldr      r0, [r4, #0x1b8]
00393cbc: bl       #0x30ed6c
00393cc0: mov      r1, r6
00393cc4: str      r0, [r4, #0x1b8]
00393cc8: ldr      r0, [r4, #0x1bc]
00393ccc: bl       #0x30ed6c
00393cd0: mov      r1, r6
00393cd4: str      r0, [r4, #0x1bc]
00393cd8: ldr      r0, [r4, #0x1c0]
00393cdc: bl       #0x30ed6c
00393ce0: ldrb     r6, [r4, #0x1b5]
00393ce4: str      r0, [r4, #0x1c0]
00393ce8: b        #0x393c78
