
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
