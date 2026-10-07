
# _ZN8RenderFX9GotoFrameEPN7gameswf9characterEib
007a7d34: push     {r4, r5, r6, lr}
007a7d38: subs     r4, r1, #0
007a7d3c: mov      r5, r2
007a7d40: mov      r6, r3
007a7d44: beq      #0x7a7d8c
007a7d48: ldr      r2, [r4]
007a7d4c: mov      r0, r4
007a7d50: mov      r1, #2
007a7d54: mov      lr, pc
007a7d58: ldr      pc, [r2, #8]
007a7d5c: cmp      r0, #0
007a7d60: beq      #0x7a7d8c
007a7d64: mov      r1, r5
007a7d68: ldr      r3, [r4]
007a7d6c: mov      r0, r4
007a7d70: mov      lr, pc
007a7d74: ldr      pc, [r3, #0x14c]
007a7d78: mov      r0, r4
007a7d7c: eor      r1, r6, #1
007a7d80: ldr      r3, [r4]
007a7d84: mov      lr, pc
007a7d88: ldr      pc, [r3, #0x94]
007a7d8c: pop      {r4, r5, r6, pc}

# _ZN7gameswf15sprite_instance14set_play_stateENS_9character10play_stateE
0077fe10: push     {r4, r5, r6, lr}
0077fe14: mov      r4, r0
0077fe18: mov      r5, r1
0077fe1c: bl       #0x77cba0
0077fe20: subs     ip, r0, #0
0077fe24: beq      #0x77fe50
0077fe28: ldr      r3, [r4, #0xa0]
0077fe2c: ldr      r1, [r3, #0x20]
0077fe30: cmp      r1, #0
0077fe34: blt      #0x77fe50
0077fe38: ldrsb    r2, [r4, #0xe6]
0077fe3c: ldr      r3, [ip]
0077fe40: rsbs     r2, r2, #1
0077fe44: movlo    r2, #0
0077fe48: mov      lr, pc
0077fe4c: ldr      pc, [r3, #0x38]
0077fe50: mov      r0, r4
0077fe54: strb     r5, [r4, #0xe6]
0077fe58: pop      {r4, r5, r6, lr}
0077fe5c: b        #0x7750e8

# _ZN7gameswf15sprite_instance10goto_frameEi
00781a3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00781a40: ldr      r3, [r0, #0xa0]
00781a44: mov      r4, r0
00781a48: mov      r7, r1
00781a4c: mov      r0, r3
00781a50: ldr      r3, [r3]
00781a54: mov      lr, pc
00781a58: ldr      pc, [r3, #0x38]
00781a5c: cmp      r0, r7
00781a60: bgt      #0x781a74
00781a64: mov      r3, #1
00781a68: strb     r3, [r4, #0xe6]
00781a6c: mov      r0, #0
00781a70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00781a74: cmp      r7, #0
00781a78: blt      #0x781a64
00781a7c: ldrsh    r3, [r4, #0xe4]
00781a80: cmp      r3, r7
00781a84: beq      #0x781a64
00781a88: ldr      sl, [r4, #0xc0]
00781a8c: add      r6, r4, #0xcc
00781a90: add      r5, r4, #0xbc
00781a94: cmp      sl, #0
00781a98: ldr      r8, [r4, #0xd0]
00781a9c: bne      #0x781c74
00781aa0: cmp      sl, r8
00781aa4: ble      #0x781ac8
00781aa8: lsl      r3, r8, #2
00781aac: mov      r1, #0
00781ab0: ldr      r2, [r6]
00781ab4: add      r8, r8, #1
00781ab8: cmp      r8, sl
00781abc: str      r1, [r2, r3]
00781ac0: add      r3, r3, #4
00781ac4: bne      #0x781ab0
00781ac8: cmp      sl, #0
00781acc: str      sl, [r4, #0xd0]
00781ad0: ble      #0x781af8
00781ad4: mov      r3, #0
00781ad8: ldr      r1, [r5]
00781adc: ldr      r2, [r6]
00781ae0: ldr      r1, [r1, r3, lsl #2]
00781ae4: str      r1, [r2, r3, lsl #2]
00781ae8: ldr      r2, [r6, #4]
00781aec: add      r3, r3, #1
00781af0: cmp      r3, r2
00781af4: blt      #0x781ad8
00781af8: ldr      r3, [r4, #0xc0]
00781afc: cmp      r3, #0
00781b00: ble      #0x781ccc
00781b04: ldrsh    sb, [r4, #0xe4]
00781b08: mov      r8, #0
00781b0c: str      r8, [r4, #0xc0]
00781b10: cmp      r7, sb
00781b14: blt      #0x781b84
00781b18: ble      #0x781bc8
00781b1c: add      sl, sb, #1
00781b20: cmp      r7, sl
00781b24: ble      #0x781bac
00781b28: mvn      sb, sb
00781b2c: add      sb, sb, r7
00781b30: add      r1, sl, r8
00781b34: ldr      r3, [r4]
00781b38: add      r8, r8, #1
00781b3c: mov      r0, r4
00781b40: mov      r2, #1
00781b44: mov      lr, pc
00781b48: ldr      pc, [r3, #0xc8]
00781b4c: cmp      r8, sb
00781b50: bne      #0x781b30
00781b54: ldr      r3, [r4, #0xc0]
00781b58: cmp      r3, #0
00781b5c: bgt      #0x781bac
00781b60: bge      #0x781bac
00781b64: lsl      r2, r3, #2
00781b68: mov      r0, #0
00781b6c: ldr      r1, [r5]
00781b70: adds     r3, r3, #1
00781b74: str      r0, [r1, r2]
00781b78: add      r2, r2, #4
00781b7c: bne      #0x781b6c
00781b80: b        #0x781bac
00781b84: rsb      sl, r7, sb
00781b88: rsb      r1, r8, sb
00781b8c: mov      r0, r4
00781b90: add      r8, r8, #1
00781b94: bl       #0x77f390
00781b98: cmp      r8, sl
00781b9c: bne      #0x781b88
00781ba0: ldr      r3, [r4, #0xc0]
00781ba4: cmp      r3, #0
00781ba8: ble      #0x781d00
00781bac: mov      r2, #0
00781bb0: str      r2, [r4, #0xc0]
00781bb4: ldr      r3, [r4]
00781bb8: mov      r0, r4
00781bbc: mov      r1, r7
00781bc0: mov      lr, pc
00781bc4: ldr      pc, [r3, #0xc8]
00781bc8: ldr      r8, [r4, #0xc0]
00781bcc: mov      r3, #1
00781bd0: strh     r7, [r4, #0xe4]
00781bd4: cmp      r8, #0
00781bd8: strb     r3, [r4, #0xe6]
00781bdc: ldr      sl, [r4, #0xbc]
00781be0: ble      #0x781c90
00781be4: ldr      sb, [r4, #0xd0]
00781be8: adds     r7, sb, r8
00781bec: beq      #0x781bfc
00781bf0: ldr      r3, [r4, #0xd4]
00781bf4: cmp      r7, r3
00781bf8: bgt      #0x781cf0
00781bfc: cmp      sb, r7
00781c00: lslge    r2, sb, #2
00781c04: bge      #0x781c2c
00781c08: lsl      r2, sb, #2
00781c0c: mov      r3, r2
00781c10: mov      r0, #0
00781c14: ldr      r1, [r6]
00781c18: add      sb, sb, #1
00781c1c: cmp      sb, r7
00781c20: str      r0, [r1, r3]
00781c24: add      r3, r3, #4
00781c28: bne      #0x781c14
00781c2c: str      r7, [r4, #0xd0]
00781c30: mov      r3, #0
00781c34: ldr      r0, [sl, r3, lsl #2]
00781c38: ldr      r1, [r4, #0xcc]
00781c3c: add      r3, r3, #1
00781c40: cmp      r3, r8
00781c44: str      r0, [r1, r2]
00781c48: add      r2, r2, #4
00781c4c: bne      #0x781c34
00781c50: ldr      r8, [r4, #0xc0]
00781c54: cmp      r8, #0
00781c58: ble      #0x781c90
00781c5c: mov      r3, #0
00781c60: mov      r0, r4
00781c64: str      r3, [r4, #0xc0]
00781c68: bl       #0x7750e8
00781c6c: mov      r0, #1
00781c70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00781c74: ldr      r3, [r4, #0xd4]
00781c78: cmp      sl, r3
00781c7c: ble      #0x781aa0
00781c80: mov      r0, r6
00781c84: add      r1, sl, sl, asr #1
00781c88: bl       #0x77e7f8
00781c8c: b        #0x781aa0
00781c90: cmp      r8, #0
00781c94: bge      #0x781c5c
00781c98: lsl      r3, r8, #2
00781c9c: mov      r1, #0
00781ca0: ldr      r2, [r5]
00781ca4: adds     r8, r8, #1
00781ca8: str      r1, [r2, r3]
00781cac: add      r3, r3, #4
00781cb0: bne      #0x781ca0
00781cb4: mov      r3, #0
00781cb8: mov      r0, r4
00781cbc: str      r3, [r4, #0xc0]
00781cc0: bl       #0x7750e8
00781cc4: mov      r0, #1
00781cc8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00781ccc: bge      #0x781b04
00781cd0: lsl      r2, r3, #2
00781cd4: mov      r0, #0
00781cd8: ldr      r1, [r5]
00781cdc: adds     r3, r3, #1
00781ce0: str      r0, [r1, r2]
00781ce4: add      r2, r2, #4
00781ce8: bne      #0x781cd8
00781cec: b        #0x781b04
00781cf0: mov      r0, r6
00781cf4: add      r1, r7, r7, asr #1
00781cf8: bl       #0x77e7f8
00781cfc: b        #0x781bfc
00781d00: bge      #0x781bac
00781d04: lsl      r2, r3, #2
00781d08: mov      r0, #0
00781d0c: ldr      r1, [r5]
00781d10: adds     r3, r3, #1
00781d14: str      r0, [r1, r2]
00781d18: add      r2, r2, #4
00781d1c: bne      #0x781d0c
00781d20: b        #0x781bac

# _ZN20DebugCachedCharacter7GetCharEv
00427d50: push     {r4, r5, r6, lr}
00427d54: ldr      r5, [r0, #0x2c]
00427d58: ldr      r6, [pc, #0x260]
00427d5c: sub      sp, sp, #8
00427d60: cmp      r5, #0
00427d64: mov      r4, r0
00427d68: add      r6, pc, r6
00427d6c: beq      #0x427ebc
00427d70: ldr      r0, [r0, #0x28]
00427d74: ldrb     r3, [r0, #4]
00427d78: cmp      r3, #0
00427d7c: beq      #0x427e9c
00427d80: ldrb     r3, [r4]
00427d84: cmp      r3, #0
00427d88: moveq    r0, r5
00427d8c: bne      #0x427e84
00427d90: cmp      r0, #0
00427d94: beq      #0x427da8
00427d98: ldr      r3, [r4, #0x28]
00427d9c: ldrb     r2, [r3, #4]
00427da0: cmp      r2, #0
00427da4: beq      #0x427f84
00427da8: cmp      r0, r5
00427dac: beq      #0x427e0c
00427db0: ldr      r3, [r4, #4]
00427db4: ldr      r5, [r4, #0x2c]
00427db8: add      r3, r3, #1
00427dbc: cmp      r5, #0
00427dc0: str      r3, [r4, #4]
00427dc4: mov      r6, r5
00427dc8: beq      #0x427e7c
00427dcc: ldr      r0, [r4, #0x28]
00427dd0: ldrb     r3, [r0, #4]
00427dd4: cmp      r3, #0
00427dd8: beq      #0x427e58
00427ddc: ldr      r3, [r6, #0x40]
00427de0: cmp      r3, #0
00427de4: beq      #0x427e08
00427de8: ldr      r0, [r6, #0x3c]
00427dec: ldrb     r2, [r0, #4]
00427df0: cmp      r2, #0
00427df4: beq      #0x427e2c
00427df8: mov      r6, r3
00427dfc: ldr      r3, [r6, #0x40]
00427e00: cmp      r3, #0
00427e04: bne      #0x427de8
00427e08: mov      r0, r5
00427e0c: cmp      r0, #0
00427e10: beq      #0x427e24
00427e14: ldr      r3, [r4, #0x28]
00427e18: ldrb     r2, [r3, #4]
00427e1c: cmp      r2, #0
00427e20: beq      #0x427f50
00427e24: add      sp, sp, #8
00427e28: pop      {r4, r5, r6, pc}
00427e2c: ldr      r1, [r0]
00427e30: sub      r1, r1, #1
00427e34: cmp      r1, #0
00427e38: str      r1, [r0]
00427e3c: bne      #0x427e44
00427e40: bl       #0x752b38
00427e44: mov      r3, #0
00427e48: str      r3, [r6, #0x40]
00427e4c: str      r3, [r6, #0x3c]
00427e50: ldr      r5, [r4, #0x2c]
00427e54: b        #0x427e08
00427e58: ldr      r1, [r0]
00427e5c: sub      r1, r1, #1
00427e60: cmp      r1, #0
00427e64: str      r1, [r0]
00427e68: bne      #0x427e70
00427e6c: bl       #0x752b38
00427e70: mov      r5, #0
00427e74: str      r5, [r4, #0x2c]
00427e78: str      r5, [r4, #0x28]
00427e7c: mov      r0, r5
00427e80: b        #0x427e24
00427e84: mov      r0, r4
00427e88: add      r1, r4, #0x1c
00427e8c: ldm      r1, {r1, r2, r3}
00427e90: bl       #0x427ca0
00427e94: ldr      r0, [r4, #0x2c]
00427e98: b        #0x427d90
00427e9c: ldr      r1, [r0]
00427ea0: sub      r1, r1, #1
00427ea4: cmp      r1, #0
00427ea8: str      r1, [r0]
00427eac: beq      #0x427f7c
00427eb0: mov      r3, #0
00427eb4: str      r3, [r4, #0x2c]
00427eb8: str      r3, [r4, #0x28]
00427ebc: ldr      r3, [pc, #0x100]
00427ec0: ldr      r3, [r6, r3]
00427ec4: ldr      r3, [r3]
00427ec8: cmp      r3, #2
00427ecc: beq      #0x427fb0
00427ed0: cmp      r3, #1
00427ed4: ldrne    r5, [r4, #0x2c]
00427ed8: bne      #0x427d80
00427edc: ldr      r0, [pc, #0xe4]
00427ee0: ldr      r1, [pc, #0xe4]
00427ee4: ldr      r2, [pc, #0xe4]
00427ee8: ldr      r0, [r6, r0]
00427eec: ldr      r3, [pc, #0xe0]
00427ef0: mov      ip, #0x32
00427ef4: add      r1, pc, r1
00427ef8: add      r0, r0, #0xa8
00427efc: add      r2, pc, r2
00427f00: add      r3, pc, r3
00427f04: str      ip, [sp]
00427f08: bl       #0x30e004
00427f0c: ldr      r5, [r4, #0x2c]
00427f10: cmp      r5, #0
00427f14: beq      #0x427d80
00427f18: ldr      r0, [r4, #0x28]
00427f1c: ldrb     r3, [r0, #4]
00427f20: cmp      r3, #0
00427f24: bne      #0x427d80
00427f28: ldr      r1, [r0]
00427f2c: sub      r1, r1, #1
00427f30: cmp      r1, #0
00427f34: str      r1, [r0]
00427f38: bne      #0x427f40
00427f3c: bl       #0x752b38
00427f40: mov      r5, #0
00427f44: str      r5, [r4, #0x28]
00427f48: str      r5, [r4, #0x2c]
00427f4c: b        #0x427d80
00427f50: ldr      r1, [r3]
00427f54: sub      r1, r1, #1
00427f58: cmp      r1, #0
00427f5c: str      r1, [r3]
00427f60: bne      #0x427f6c
00427f64: mov      r0, r3
00427f68: bl       #0x752b38
00427f6c: mov      r0, #0
00427f70: str      r0, [r4, #0x2c]
00427f74: str      r0, [r4, #0x28]
00427f78: b        #0x427e24
00427f7c: bl       #0x752b38
00427f80: b        #0x427eb0
00427f84: ldr      r1, [r3]
00427f88: sub      r1, r1, #1
00427f8c: cmp      r1, #0
00427f90: str      r1, [r3]
00427f94: bne      #0x427fa0
00427f98: mov      r0, r3
00427f9c: bl       #0x752b38
00427fa0: mov      r0, #0
00427fa4: str      r0, [r4, #0x28]
00427fa8: str      r0, [r4, #0x2c]
00427fac: b        #0x427da8
00427fb0: mov      r3, #0
00427fb4: str      r3, [r3]
00427fb8: ldr      r5, [r4, #0x2c]
00427fbc: b        #0x427d80
00427fc0: subseq   ip, r6, r8, lsr #26
00427fc4: andeq    r3, r0, r0, asr #19
00427fc8: andeq    r1, r0, r0, asr #19
00427fcc: subeq    r6, sb, r4, ror #9
00427fd0: strdeq   r1, r2, [sl], #-0x64
00427fd4: subeq    r1, sl, r0, lsl r7
