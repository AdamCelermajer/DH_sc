
# _ZN10GameObject4StopEv
003938f8: push     {r4, r5, r6, lr}
003938fc: ldr      r5, [pc, #0xe0]
00393900: ldr      r3, [pc, #0xe0]
00393904: mov      r4, r0
00393908: add      r5, pc, r5
0039390c: ldr      r0, [r5, r3]
00393910: add      r1, r4, #0x1c8
00393914: bl       #0x52aae4
00393918: ldr      r3, [pc, #0xcc]
0039391c: ldr      r1, [r4, #0x168]
00393920: ldr      ip, [r4, #0x160]
00393924: ldr      r0, [r4, #0x164]
00393928: ldr      r3, [r5, r3]
0039392c: mov      r2, #0
00393930: str      r1, [r4, #0x1b0]
00393934: str      ip, [r4, #0x1a8]
00393938: str      r0, [r4, #0x1ac]
0039393c: strb     r2, [r4, #0x1b5]
00393940: strb     r2, [r4, #0x1b4]
00393944: ldr      r2, [r3]
00393948: ldr      r1, [r4, #0x2dc]
0039394c: str      r2, [r4, #0x1b8]
00393950: ldr      r2, [r3, #4]
00393954: cmp      r1, #0
00393958: str      r2, [r4, #0x1bc]
0039395c: ldr      r3, [r3, #8]
00393960: str      r3, [r4, #0x1c0]
00393964: beq      #0x3939e0
00393968: ldr      r3, [r4]
0039396c: mov      r0, r4
00393970: mov      lr, pc
00393974: ldr      pc, [r3, #0x64]
00393978: cmp      r0, #0
0039397c: beq      #0x3939e0
00393980: mov      r5, #0
00393984: mov      r2, r5
00393988: ldr      r0, [r4, #0x2dc]
0039398c: mov      r1, r5
00393990: bl       #0x46e918
00393994: ldr      r0, [r4, #0x2dc]
00393998: mov      r1, r5
0039399c: bl       #0x46e978
003939a0: ldr      r2, [r4, #0x164]
003939a4: ldr      r0, [r4, #0x2dc]
003939a8: ldr      r1, [r4, #0x160]
003939ac: bl       #0x46ea80
003939b0: ldr      r3, [r4, #0x2dc]
003939b4: ldr      r3, [r3, #0x14]
003939b8: ldrh     r2, [r3]
003939bc: str      r5, [r3, #0x54]
003939c0: str      r5, [r3, #0x8c]
003939c4: orr      r2, r2, #8
003939c8: strh     r2, [r3]
003939cc: str      r5, [r3, #0x40]
003939d0: str      r5, [r3, #0x44]
003939d4: str      r5, [r3, #0x48]
003939d8: str      r5, [r3, #0x4c]
003939dc: str      r5, [r3, #0x50]
003939e0: pop      {r4, r5, r6, pc}
003939e4: rsbeq    r1, r0, r8, lsl #3
003939e8: andeq    r1, r0, r4, lsl #4
003939ec: andeq    r3, r0, ip, lsr #30

# _ZN10GameObject6PathToERK7Point3DIfE
003939f0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003939f4: ldrb     r3, [r0, #0x84]
003939f8: ldr      r4, [pc, #0xdc]
003939fc: mov      r6, r0
00393a00: cmp      r3, #0
00393a04: mov      r5, r1
00393a08: add      r4, pc, r4
00393a0c: bne      #0x393ad8
00393a10: mov      r2, r0
00393a14: ldr      r3, [r2, #0x200]!
00393a18: cmp      r3, r2
00393a1c: beq      #0x393ab4
00393a20: ldr      r3, [r3]
00393a24: cmp      r2, r3
00393a28: bne      #0x393a20
00393a2c: ldr      r1, [r5]
00393a30: ldr      r0, [r6, #0x208]
00393a34: bl       #0x30e3ac
00393a38: ldr      r1, [r5, #4]
00393a3c: mov      r7, r0
00393a40: ldr      r0, [r6, #0x20c]
00393a44: bl       #0x30e3ac
00393a48: ldr      r1, [r5, #8]
00393a4c: mov      sl, r0
00393a50: ldr      r0, [r6, #0x210]
00393a54: bl       #0x30e3ac
00393a58: mov      r1, r7
00393a5c: mov      r8, r0
00393a60: mov      r0, r7
00393a64: bl       #0x30ed6c
00393a68: mov      r1, sl
00393a6c: mov      r7, r0
00393a70: mov      r0, sl
00393a74: bl       #0x30ed6c
00393a78: mov      r1, r0
00393a7c: mov      r0, r7
00393a80: bl       #0x30eba4
00393a84: mov      r1, r8
00393a88: mov      r7, r0
00393a8c: mov      r0, r8
00393a90: bl       #0x30ed6c
00393a94: mov      r1, r0
00393a98: mov      r0, r7
00393a9c: bl       #0x30eba4
00393aa0: mov      r1, #0x47000000
00393aa4: add      r1, r1, #0x1c4000
00393aa8: bl       #0x30e2f8
00393aac: cmp      r0, #0
00393ab0: beq      #0x393ad8
00393ab4: ldr      r2, [pc, #0x24]
00393ab8: ldr      r3, [r6, #0x26c]
00393abc: add      r1, r6, #0x1c8
00393ac0: ldr      r0, [r4, r2]
00393ac4: cmp      r3, #0
00393ac8: moveq    r3, #0x1e
00393acc: mov      r2, r5
00393ad0: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393ad4: b        #0x52db48
00393ad8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393adc: rsbeq    r1, r0, r8, lsl #1
00393ae0: andeq    r1, r0, r4, lsl #4

# _ZN10GameObject11LookTowardsERK7Point3DIfE
00393b1c: push     {r4, r5, r6, lr}
00393b20: ldr      r5, [r1, #4]
00393b24: mov      r4, r1
00393b28: mov      r6, r0
00393b2c: mov      r1, #0
00393b30: mov      r0, r5
00393b34: bl       #0x30df8c
00393b38: cmp      r0, #0
00393b3c: beq      #0x393b7c
00393b40: ldr      r4, [r4]
00393b44: mov      r1, #0
00393b48: mov      r0, r4
00393b4c: bl       #0x30e2f8
00393b50: cmp      r0, #0
00393b54: bne      #0x393bd8
00393b58: mov      r0, r4
00393b5c: mov      r1, #0
00393b60: bl       #0x30e70c
00393b64: cmp      r0, #0
00393b68: beq      #0x393bd4
00393b6c: movw     r3, #0xcbe4
00393b70: movt     r3, #0x4096
00393b74: str      r3, [r6, #0x178]
00393b78: pop      {r4, r5, r6, pc}
00393b7c: add      r1, r5, #0x80000000
00393b80: ldr      r0, [r4]
00393b84: bl       #0x30ec94
00393b88: bl       #0x30e79c
00393b8c: str      r0, [r6, #0x178]
00393b90: mov      r5, r0
00393b94: mov      r1, #0
00393b98: ldr      r0, [r4, #4]
00393b9c: bl       #0x30e2f8
00393ba0: cmp      r0, #0
00393ba4: beq      #0x393bd4
00393ba8: ldr      r0, [r4]
00393bac: mov      r1, #0
00393bb0: bl       #0x30e2f8
00393bb4: cmp      r0, #0
00393bb8: movweq   r0, #0xfdb
00393bbc: movwne   r0, #0xfdb
00393bc0: movteq   r0, #0xc049
00393bc4: movtne   r0, #0x4049
00393bc8: mov      r1, r5
00393bcc: bl       #0x30eba4
00393bd0: str      r0, [r6, #0x178]
00393bd4: pop      {r4, r5, r6, pc}
00393bd8: movw     r3, #0xfdb
00393bdc: movt     r3, #0x3fc9
00393be0: str      r3, [r6, #0x178]
00393be4: pop      {r4, r5, r6, pc}

# _ZN10GameObject6LookAtERK7Point3DIfE
00393cec: push     {r4, r5, r6, r7, lr}
00393cf0: mov      r4, r0
00393cf4: sub      sp, sp, #0x14
00393cf8: ldr      r0, [r1, #4]
00393cfc: mov      r5, r1
00393d00: ldr      r1, [r4, #0x164]
00393d04: bl       #0x30e3ac
00393d08: ldr      r1, [r4, #0x168]
00393d0c: mov      r7, r0
00393d10: ldr      r0, [r5, #8]
00393d14: bl       #0x30e3ac
00393d18: ldr      r1, [r4, #0x160]
00393d1c: mov      r6, r0
00393d20: ldr      r0, [r5]
00393d24: bl       #0x30e3ac
00393d28: add      r1, sp, #4
00393d2c: str      r0, [sp, #4]
00393d30: mov      r0, r4
00393d34: str      r7, [sp, #8]
00393d38: str      r6, [sp, #0xc]
00393d3c: bl       #0x393b1c
00393d40: add      sp, sp, #0x14
00393d44: pop      {r4, r5, r6, r7, pc}
