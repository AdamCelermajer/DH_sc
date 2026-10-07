
# _ZN9CSInjured6OnInitEiP9CharacterP16CharStateMachine
003c8850: push     {r4, r5, r6, lr}
003c8854: add      r5, r2, #0x4f0
003c8858: add      r5, r5, #0xc
003c885c: sub      sp, sp, #0x30
003c8860: mov      r4, #0
003c8864: mov      r6, r1
003c8868: mov      r0, r5
003c886c: mov      r2, #0x22
003c8870: mov      r3, #3
003c8874: str      r4, [sp, #0x28]
003c8878: str      r4, [sp, #0x2c]
003c887c: str      r4, [sp]
003c8880: str      r4, [sp, #4]
003c8884: bl       #0x3c7b18
003c8888: mov      r0, r5
003c888c: mov      r1, r6
003c8890: movw     r2, #0xc358
003c8894: mov      r3, #0xc
003c8898: str      r4, [sp, #0x20]
003c889c: str      r4, [sp, #0x24]
003c88a0: str      r4, [sp]
003c88a4: str      r4, [sp, #4]
003c88a8: bl       #0x3c7b18
003c88ac: mov      r0, r5
003c88b0: mov      r1, r6
003c88b4: movw     r2, #0xc35a
003c88b8: mov      r3, #0xb
003c88bc: str      r4, [sp, #0x18]
003c88c0: str      r4, [sp, #0x1c]
003c88c4: str      r4, [sp]
003c88c8: str      r4, [sp, #4]
003c88cc: bl       #0x3c7b18
003c88d0: mov      r0, r5
003c88d4: mov      r1, r6
003c88d8: movw     r2, #0xc35b
003c88dc: mov      r3, #0xa
003c88e0: str      r4, [sp, #0x10]
003c88e4: str      r4, [sp, #0x14]
003c88e8: str      r4, [sp]
003c88ec: str      r4, [sp, #4]
003c88f0: bl       #0x3c7b18
003c88f4: mov      r0, r5
003c88f8: mov      r1, r6
003c88fc: movw     r2, #0xc355
003c8900: mov      r3, #6
003c8904: str      r4, [sp, #4]
003c8908: str      r4, [sp, #8]
003c890c: str      r4, [sp, #0xc]
003c8910: str      r4, [sp]
003c8914: bl       #0x3c7b18
003c8918: add      sp, sp, #0x30
003c891c: pop      {r4, r5, r6, pc}

# _ZN7CSSpawn6OnInitEiP9CharacterP16CharStateMachine
003c7d0c: push     {r4, r5, r6, r7, lr}
003c7d10: add      r5, r2, #0x4f0
003c7d14: add      r5, r5, #0xc
003c7d18: sub      sp, sp, #0x34
003c7d1c: mov      r4, #0
003c7d20: mov      r7, r1
003c7d24: mov      r0, r5
003c7d28: mov      r2, #0x22
003c7d2c: mov      r3, #3
003c7d30: str      r4, [sp, #0x28]
003c7d34: str      r4, [sp, #0x2c]
003c7d38: str      r4, [sp]
003c7d3c: str      r4, [sp, #4]
003c7d40: ldr      r6, [pc, #0xa8]
003c7d44: bl       #0x3c7b18
003c7d48: mov      r0, r5
003c7d4c: mov      r1, r7
003c7d50: movw     r2, #0xc358
003c7d54: mov      r3, #0xc
003c7d58: str      r4, [sp, #0x20]
003c7d5c: str      r4, [sp, #0x24]
003c7d60: str      r4, [sp]
003c7d64: str      r4, [sp, #4]
003c7d68: bl       #0x3c7b18
003c7d6c: ldr      r3, [pc, #0x80]
003c7d70: add      r6, pc, r6
003c7d74: mov      r0, r5
003c7d78: ldr      ip, [r6, r3]
003c7d7c: mov      r1, r7
003c7d80: movw     r2, #0xc35a
003c7d84: mov      r3, #0xb
003c7d88: str      ip, [sp]
003c7d8c: str      ip, [sp, #0x18]
003c7d90: str      r4, [sp, #0x1c]
003c7d94: str      r4, [sp, #4]
003c7d98: bl       #0x3c7b18
003c7d9c: ldr      r3, [pc, #0x54]
003c7da0: mov      r0, r5
003c7da4: mov      r1, r7
003c7da8: ldr      r6, [r6, r3]
003c7dac: movw     r2, #0xc35b
003c7db0: mov      r3, #0xa
003c7db4: str      r6, [sp, #0x10]
003c7db8: str      r4, [sp, #0x14]
003c7dbc: str      r6, [sp]
003c7dc0: str      r4, [sp, #4]
003c7dc4: bl       #0x3c7b18
003c7dc8: mov      r0, r5
003c7dcc: mov      r1, r7
003c7dd0: movw     r2, #0xc35c
003c7dd4: mov      r3, #9
003c7dd8: str      r6, [sp]
003c7ddc: stmib    sp, {r4, r6}
003c7de0: str      r4, [sp, #0xc]
003c7de4: bl       #0x3c7b18
003c7de8: add      sp, sp, #0x34
003c7dec: pop      {r4, r5, r6, r7, pc}
003c7df0: subseq   ip, ip, r0, lsr #26
003c7df4: andeq    r2, r0, r4, lsl #29
003c7df8: andeq    r3, r0, ip, asr #9

# _ZN6CSDead6OnInitEiP9CharacterP16CharStateMachine
003c8920: push     {r4, r5, r6, lr}
003c8924: add      r5, r2, #0x4f0
003c8928: add      r5, r5, #0xc
003c892c: sub      sp, sp, #0x18
003c8930: mov      r4, #0
003c8934: mov      r6, r1
003c8938: mov      r0, r5
003c893c: mov      r2, #0x2e
003c8940: mov      r3, #2
003c8944: str      r4, [sp, #0x10]
003c8948: str      r4, [sp, #0x14]
003c894c: str      r4, [sp]
003c8950: str      r4, [sp, #4]
003c8954: bl       #0x3c7b18
003c8958: mov      r0, r5
003c895c: mov      r1, r6
003c8960: movw     r2, #0xc359
003c8964: mov      r3, #0x10
003c8968: str      r4, [sp, #4]
003c896c: str      r4, [sp, #8]
003c8970: str      r4, [sp, #0xc]
003c8974: str      r4, [sp]
003c8978: bl       #0x3c7b18
003c897c: add      sp, sp, #0x18
003c8980: pop      {r4, r5, r6, pc}

# _ZN10CSReviving6OnInitEiP9CharacterP16CharStateMachine
003c8b8c: push     {r4, r5, r6, r7, lr}
003c8b90: add      r5, r2, #0x4f0
003c8b94: add      r5, r5, #0xc
003c8b98: sub      sp, sp, #0x4c
003c8b9c: mov      r4, #0
003c8ba0: mov      r6, r1
003c8ba4: mov      r0, r5
003c8ba8: mov      r2, #0x22
003c8bac: mov      r3, #3
003c8bb0: str      r4, [sp, #0x40]
003c8bb4: str      r4, [sp, #0x44]
003c8bb8: str      r4, [sp]
003c8bbc: str      r4, [sp, #4]
003c8bc0: bl       #0x3c7b18
003c8bc4: mov      r0, r5
003c8bc8: mov      r1, r6
003c8bcc: movw     r2, #0xc358
003c8bd0: mov      r3, #0xc
003c8bd4: str      r4, [sp, #0x38]
003c8bd8: str      r4, [sp, #0x3c]
003c8bdc: str      r4, [sp]
003c8be0: str      r4, [sp, #4]
003c8be4: ldr      r7, [pc, #0xf4]
003c8be8: bl       #0x3c7b18
003c8bec: mov      r0, r5
003c8bf0: mov      r1, r6
003c8bf4: movw     r2, #0xc355
003c8bf8: mov      r3, #6
003c8bfc: str      r4, [sp, #0x30]
003c8c00: str      r4, [sp, #0x34]
003c8c04: str      r4, [sp]
003c8c08: str      r4, [sp, #4]
003c8c0c: bl       #0x3c7b18
003c8c10: ldr      r3, [pc, #0xcc]
003c8c14: add      r7, pc, r7
003c8c18: mov      r0, r5
003c8c1c: ldr      ip, [r7, r3]
003c8c20: mov      r1, r6
003c8c24: movw     r2, #0xc35a
003c8c28: mov      r3, #0xb
003c8c2c: str      ip, [sp]
003c8c30: str      ip, [sp, #0x28]
003c8c34: str      r4, [sp, #0x2c]
003c8c38: str      r4, [sp, #4]
003c8c3c: bl       #0x3c7b18
003c8c40: ldr      r3, [pc, #0xa0]
003c8c44: mov      r0, r5
003c8c48: mov      r1, r6
003c8c4c: ldr      r7, [r7, r3]
003c8c50: movw     r2, #0xc35b
003c8c54: mov      r3, #0xa
003c8c58: str      r7, [sp, #0x20]
003c8c5c: str      r4, [sp, #0x24]
003c8c60: str      r7, [sp]
003c8c64: str      r4, [sp, #4]
003c8c68: bl       #0x3c7b18
003c8c6c: mov      r0, r5
003c8c70: mov      r1, r6
003c8c74: movw     r2, #0xc35c
003c8c78: mov      r3, #9
003c8c7c: str      r7, [sp, #0x18]
003c8c80: str      r4, [sp, #0x1c]
003c8c84: str      r7, [sp]
003c8c88: str      r4, [sp, #4]
003c8c8c: bl       #0x3c7b18
003c8c90: mov      r0, r5
003c8c94: mov      r1, r6
003c8c98: movw     r2, #0xc35d
003c8c9c: mov      r3, #8
003c8ca0: str      r7, [sp]
003c8ca4: str      r7, [sp, #0x10]
003c8ca8: str      r4, [sp, #0x14]
003c8cac: str      r4, [sp, #4]
003c8cb0: bl       #0x3c7b18
003c8cb4: mov      r0, r5
003c8cb8: mov      r1, r6
003c8cbc: movw     r2, #0xc351
003c8cc0: mov      r3, #4
003c8cc4: str      r4, [sp, #4]
003c8cc8: str      r4, [sp, #8]
003c8ccc: str      r4, [sp, #0xc]
003c8cd0: str      r4, [sp]
003c8cd4: bl       #0x3c7b18
003c8cd8: add      sp, sp, #0x4c
003c8cdc: pop      {r4, r5, r6, r7, pc}
003c8ce0: subseq   fp, ip, ip, ror lr
003c8ce4: andeq    r2, r0, r4, lsl #29
003c8ce8: andeq    r3, r0, ip, asr #9

# _ZN13CSLiftingMove6OnInitEiP9CharacterP16CharStateMachine
003c8ee0: push     {r4, r5, r6, r7, lr}
003c8ee4: add      r5, r2, #0x4f0
003c8ee8: add      r5, r5, #0xc
003c8eec: sub      sp, sp, #0x3c
003c8ef0: mov      r4, #0
003c8ef4: mov      r7, r1
003c8ef8: mov      r0, r5
003c8efc: mov      r2, #0x3f
003c8f00: mov      r3, #0x12
003c8f04: str      r4, [sp, #0x30]
003c8f08: str      r4, [sp, #0x34]
003c8f0c: str      r4, [sp]
003c8f10: str      r4, [sp, #4]
003c8f14: ldr      r6, [pc, #0xcc]
003c8f18: bl       #0x3c7b18
003c8f1c: mov      r0, r5
003c8f20: mov      r1, r7
003c8f24: movw     r2, #0xc358
003c8f28: mov      r3, #0xc
003c8f2c: str      r4, [sp, #0x28]
003c8f30: str      r4, [sp, #0x2c]
003c8f34: str      r4, [sp]
003c8f38: str      r4, [sp, #4]
003c8f3c: bl       #0x3c7b18
003c8f40: ldr      r3, [pc, #0xa4]
003c8f44: add      r6, pc, r6
003c8f48: mov      r0, r5
003c8f4c: ldr      ip, [r6, r3]
003c8f50: mov      r1, r7
003c8f54: movw     r2, #0xc35a
003c8f58: mov      r3, #0xb
003c8f5c: str      ip, [sp]
003c8f60: str      ip, [sp, #0x20]
003c8f64: str      r4, [sp, #0x24]
003c8f68: str      r4, [sp, #4]
003c8f6c: bl       #0x3c7b18
003c8f70: ldr      r3, [pc, #0x78]
003c8f74: mov      r0, r5
003c8f78: mov      r1, r7
003c8f7c: ldr      r6, [r6, r3]
003c8f80: movw     r2, #0xc35b
003c8f84: mov      r3, #0xa
003c8f88: str      r6, [sp, #0x18]
003c8f8c: str      r4, [sp, #0x1c]
003c8f90: str      r6, [sp]
003c8f94: str      r4, [sp, #4]
003c8f98: bl       #0x3c7b18
003c8f9c: mov      r0, r5
003c8fa0: mov      r1, r7
003c8fa4: movw     r2, #0xc35c
003c8fa8: mov      r3, #9
003c8fac: str      r6, [sp, #0x10]
003c8fb0: str      r4, [sp, #0x14]
003c8fb4: str      r6, [sp]
003c8fb8: str      r4, [sp, #4]
003c8fbc: bl       #0x3c7b18
003c8fc0: mov      r0, r5
003c8fc4: mov      r1, r7
003c8fc8: movw     r2, #0xc35d
003c8fcc: mov      r3, #8
003c8fd0: str      r6, [sp]
003c8fd4: stmib    sp, {r4, r6}
003c8fd8: str      r4, [sp, #0xc]
003c8fdc: bl       #0x3c7b18
003c8fe0: add      sp, sp, #0x3c
003c8fe4: pop      {r4, r5, r6, r7, pc}
003c8fe8: subseq   fp, ip, ip, asr #22
003c8fec: andeq    r2, r0, r4, lsl #29
003c8ff0: andeq    r3, r0, ip, asr #9

# _ZN8CSLimbus6OnInitEiP9CharacterP16CharStateMachine
003c7cc0: ldr      ip, [pc, #0x3c]
003c7cc4: push     {r4, lr}
003c7cc8: ldr      r3, [pc, #0x38]
003c7ccc: add      ip, pc, ip
003c7cd0: add      r0, r2, #0x4f0
003c7cd4: ldr      r4, [ip, r3]
003c7cd8: sub      sp, sp, #0x10
003c7cdc: mov      lr, #0
003c7ce0: add      r0, r0, #0xc
003c7ce4: mov      r2, #0x2f
003c7ce8: mov      r3, #1
003c7cec: stm      sp, {r4, lr}
003c7cf0: str      r4, [sp, #8]
003c7cf4: str      lr, [sp, #0xc]
003c7cf8: bl       #0x3c7b18
003c7cfc: add      sp, sp, #0x10
003c7d00: pop      {r4, pc}
003c7d04: subseq   ip, ip, r4, asr #27
003c7d08: strheq   r1, [r0], -r0

# _ZN13CSLiftingIdle6OnInitEiP9CharacterP16CharStateMachine
003c8d5c: push     {r4, r5, r6, r7, lr}
003c8d60: add      r5, r2, #0x4f0
003c8d64: add      r5, r5, #0xc
003c8d68: sub      sp, sp, #0x54
003c8d6c: mov      r4, #0
003c8d70: mov      r6, r1
003c8d74: mov      r0, r5
003c8d78: mov      r2, #0x23
003c8d7c: mov      r3, #0x12
003c8d80: str      r4, [sp, #0x48]
003c8d84: str      r4, [sp, #0x4c]
003c8d88: str      r4, [sp]
003c8d8c: str      r4, [sp, #4]
003c8d90: bl       #0x3c7b18
003c8d94: mov      r0, r5
003c8d98: mov      r1, r6
003c8d9c: mov      r2, #0x22
003c8da0: mov      r3, #0x12
003c8da4: str      r4, [sp, #0x40]
003c8da8: str      r4, [sp, #0x44]
003c8dac: str      r4, [sp]
003c8db0: str      r4, [sp, #4]
003c8db4: ldr      r7, [pc, #0x118]
003c8db8: bl       #0x3c7b18
003c8dbc: mov      r0, r5
003c8dc0: mov      r1, r6
003c8dc4: movw     r2, #0xc358
003c8dc8: mov      r3, #0xc
003c8dcc: str      r4, [sp, #0x38]
003c8dd0: str      r4, [sp, #0x3c]
003c8dd4: str      r4, [sp]
003c8dd8: str      r4, [sp, #4]
003c8ddc: bl       #0x3c7b18
003c8de0: ldr      r3, [pc, #0xf0]
003c8de4: add      r7, pc, r7
003c8de8: mov      r0, r5
003c8dec: ldr      ip, [r7, r3]
003c8df0: mov      r1, r6
003c8df4: movw     r2, #0xc35a
003c8df8: mov      r3, #0xb
003c8dfc: str      ip, [sp]
003c8e00: str      ip, [sp, #0x30]
003c8e04: str      r4, [sp, #0x34]
003c8e08: str      r4, [sp, #4]
003c8e0c: bl       #0x3c7b18
003c8e10: ldr      r3, [pc, #0xc4]
003c8e14: mov      r0, r5
003c8e18: mov      r1, r6
003c8e1c: ldr      r7, [r7, r3]
003c8e20: movw     r2, #0xc35b
003c8e24: mov      r3, #0xa
003c8e28: str      r7, [sp, #0x28]
003c8e2c: str      r4, [sp, #0x2c]
003c8e30: str      r7, [sp]
003c8e34: str      r4, [sp, #4]
003c8e38: bl       #0x3c7b18
003c8e3c: mov      r0, r5
003c8e40: mov      r1, r6
003c8e44: movw     r2, #0xc35c
003c8e48: mov      r3, #9
003c8e4c: str      r7, [sp, #0x20]
003c8e50: str      r4, [sp, #0x24]
003c8e54: str      r7, [sp]
003c8e58: str      r4, [sp, #4]
003c8e5c: bl       #0x3c7b18
003c8e60: mov      r0, r5
003c8e64: mov      r1, r6
003c8e68: movw     r2, #0xc35d
003c8e6c: mov      r3, #8
003c8e70: str      r7, [sp]
003c8e74: str      r7, [sp, #0x18]
003c8e78: str      r4, [sp, #0x1c]
003c8e7c: str      r4, [sp, #4]
003c8e80: bl       #0x3c7b18
003c8e84: mov      r0, r5
003c8e88: mov      r1, r6
003c8e8c: movw     r2, #0xc351
003c8e90: mov      r3, #0x13
003c8e94: str      r4, [sp, #0x10]
003c8e98: str      r4, [sp, #0x14]
003c8e9c: str      r4, [sp]
003c8ea0: str      r4, [sp, #4]
003c8ea4: bl       #0x3c7b18
003c8ea8: mov      r0, r5
003c8eac: mov      r1, r6
003c8eb0: movw     r2, #0xc352
003c8eb4: mov      r3, #0x12
003c8eb8: str      r4, [sp, #4]
003c8ebc: str      r4, [sp, #8]
003c8ec0: str      r4, [sp, #0xc]
003c8ec4: str      r4, [sp]
003c8ec8: bl       #0x3c7b18
003c8ecc: add      sp, sp, #0x54
003c8ed0: pop      {r4, r5, r6, r7, pc}
003c8ed4: subseq   fp, ip, ip, lsr #25
003c8ed8: andeq    r2, r0, r4, lsl #29
003c8edc: andeq    r3, r0, ip, asr #9

# _ZN8CSAttack6OnInitEiP9CharacterP16CharStateMachine
003c8284: push     {r4, r5, r6, r7, r8, lr}
003c8288: add      r5, r2, #0x4f0
003c828c: add      r5, r5, #0xc
003c8290: sub      sp, sp, #0x58
003c8294: mov      r4, #0
003c8298: mov      r6, r1
003c829c: mov      r0, r5
003c82a0: mov      r2, #0x22
003c82a4: mov      r3, #3
003c82a8: str      r4, [sp, #0x50]
003c82ac: str      r4, [sp, #0x54]
003c82b0: str      r4, [sp]
003c82b4: str      r4, [sp, #4]
003c82b8: bl       #0x3c7b18
003c82bc: mov      r0, r5
003c82c0: mov      r1, r6
003c82c4: movw     r2, #0xc358
003c82c8: mov      r3, #0xc
003c82cc: str      r4, [sp, #0x48]
003c82d0: str      r4, [sp, #0x4c]
003c82d4: str      r4, [sp]
003c82d8: str      r4, [sp, #4]
003c82dc: bl       #0x3c7b18
003c82e0: mov      r0, r5
003c82e4: mov      r1, r6
003c82e8: movw     r2, #0xc355
003c82ec: mov      r3, #6
003c82f0: str      r4, [sp, #0x40]
003c82f4: str      r4, [sp, #0x44]
003c82f8: str      r4, [sp]
003c82fc: str      r4, [sp, #4]
003c8300: ldr      r8, [pc, #0x120]
003c8304: bl       #0x3c7b18
003c8308: mov      r0, r5
003c830c: mov      r1, r6
003c8310: movw     r2, #0xc356
003c8314: mov      r3, #7
003c8318: str      r4, [sp, #0x38]
003c831c: str      r4, [sp, #0x3c]
003c8320: str      r4, [sp]
003c8324: str      r4, [sp, #4]
003c8328: bl       #0x3c7b18
003c832c: ldr      r3, [pc, #0xf8]
003c8330: add      r8, pc, r8
003c8334: mov      r0, r5
003c8338: ldr      ip, [r8, r3]
003c833c: mov      r1, r6
003c8340: movw     r2, #0xc35a
003c8344: mov      r3, #0xb
003c8348: str      ip, [sp]
003c834c: str      ip, [sp, #0x30]
003c8350: str      r4, [sp, #0x34]
003c8354: str      r4, [sp, #4]
003c8358: bl       #0x3c7b18
003c835c: ldr      r3, [pc, #0xcc]
003c8360: mov      r0, r5
003c8364: mov      r1, r6
003c8368: ldr      r7, [r8, r3]
003c836c: movw     r2, #0xc35b
003c8370: mov      r3, #0xa
003c8374: str      r7, [sp, #0x28]
003c8378: str      r4, [sp, #0x2c]
003c837c: str      r7, [sp]
003c8380: str      r4, [sp, #4]
003c8384: bl       #0x3c7b18
003c8388: mov      r0, r5
003c838c: mov      r1, r6
003c8390: movw     r2, #0xc35c
003c8394: mov      r3, #9
003c8398: str      r7, [sp, #0x20]
003c839c: str      r4, [sp, #0x24]
003c83a0: str      r7, [sp]
003c83a4: str      r4, [sp, #4]
003c83a8: bl       #0x3c7b18
003c83ac: mov      r0, r5
003c83b0: mov      r1, r6
003c83b4: movw     r2, #0xc35d
003c83b8: mov      r3, #8
003c83bc: str      r7, [sp]
003c83c0: str      r7, [sp, #0x18]
003c83c4: str      r4, [sp, #0x1c]
003c83c8: str      r4, [sp, #4]
003c83cc: bl       #0x3c7b18
003c83d0: ldr      r3, [pc, #0x5c]
003c83d4: mov      r0, r5
003c83d8: mov      r1, r6
003c83dc: ldr      ip, [r8, r3]
003c83e0: movw     r2, #0xc351
003c83e4: mov      r3, #4
003c83e8: str      ip, [sp]
003c83ec: str      ip, [sp, #0x10]
003c83f0: str      r4, [sp, #0x14]
003c83f4: str      r4, [sp, #4]
003c83f8: bl       #0x3c7b18
003c83fc: mov      r0, r5
003c8400: mov      r1, r6
003c8404: movw     r2, #0xc357
003c8408: mov      r3, #0xf
003c840c: str      r4, [sp, #4]
003c8410: str      r4, [sp, #8]
003c8414: str      r4, [sp, #0xc]
003c8418: str      r4, [sp]
003c841c: bl       #0x3c7b18
003c8420: add      sp, sp, #0x58
003c8424: pop      {r4, r5, r6, r7, r8, pc}
003c8428: subseq   ip, ip, r0, ror #14
003c842c: andeq    r2, r0, r4, lsl #29
003c8430: andeq    r3, r0, ip, asr #9
003c8434: andeq    r3, r0, r4, lsl #7

# _ZN6CSIdle6OnInitEiP9CharacterP16CharStateMachine
003c7e60: push     {r4, r5, r6, r7, r8, sl, lr}
003c7e64: add      r5, r2, #0x4f0
003c7e68: add      r5, r5, #0xc
003c7e6c: sub      sp, sp, #0x7c
003c7e70: mov      r4, #0
003c7e74: mov      r6, r1
003c7e78: mov      sl, r2
003c7e7c: mov      r0, r5
003c7e80: mov      r2, #0x23
003c7e84: mov      r3, #3
003c7e88: str      r4, [sp, #0x70]
003c7e8c: str      r4, [sp, #0x74]
003c7e90: str      r4, [sp]
003c7e94: str      r4, [sp, #4]
003c7e98: bl       #0x3c7b18
003c7e9c: mov      r0, r5
003c7ea0: mov      r1, r6
003c7ea4: mov      r2, #0x22
003c7ea8: mov      r3, #3
003c7eac: str      r4, [sp, #0x68]
003c7eb0: str      r4, [sp, #0x6c]
003c7eb4: str      r4, [sp]
003c7eb8: str      r4, [sp, #4]
003c7ebc: ldr      r8, [pc, #0x1d8]
003c7ec0: bl       #0x3c7b18
003c7ec4: mov      r0, r5
003c7ec8: mov      r1, r6
003c7ecc: movw     r2, #0xc358
003c7ed0: mov      r3, #0xc
003c7ed4: str      r4, [sp, #0x60]
003c7ed8: str      r4, [sp, #0x64]
003c7edc: str      r4, [sp]
003c7ee0: str      r4, [sp, #4]
003c7ee4: bl       #0x3c7b18
003c7ee8: ldr      r3, [pc, #0x1b0]
003c7eec: add      r8, pc, r8
003c7ef0: mov      r0, r5
003c7ef4: ldr      ip, [r8, r3]
003c7ef8: mov      r1, r6
003c7efc: movw     r2, #0xc35a
003c7f00: mov      r3, #0xb
003c7f04: str      ip, [sp]
003c7f08: str      ip, [sp, #0x58]
003c7f0c: str      r4, [sp, #0x5c]
003c7f10: str      r4, [sp, #4]
003c7f14: bl       #0x3c7b18
003c7f18: ldr      r3, [pc, #0x184]
003c7f1c: mov      r0, r5
003c7f20: mov      r1, r6
003c7f24: ldr      r7, [r8, r3]
003c7f28: movw     r2, #0xc35b
003c7f2c: mov      r3, #0xa
003c7f30: str      r7, [sp, #0x50]
003c7f34: str      r4, [sp, #0x54]
003c7f38: str      r7, [sp]
003c7f3c: str      r4, [sp, #4]
003c7f40: bl       #0x3c7b18
003c7f44: mov      r0, r5
003c7f48: mov      r1, r6
003c7f4c: movw     r2, #0xc35c
003c7f50: mov      r3, #9
003c7f54: str      r7, [sp, #0x48]
003c7f58: str      r4, [sp, #0x4c]
003c7f5c: str      r7, [sp]
003c7f60: str      r4, [sp, #4]
003c7f64: bl       #0x3c7b18
003c7f68: mov      r0, r5
003c7f6c: mov      r1, r6
003c7f70: movw     r2, #0xc35d
003c7f74: mov      r3, #8
003c7f78: str      r7, [sp]
003c7f7c: str      r7, [sp, #0x40]
003c7f80: str      r4, [sp, #0x44]
003c7f84: str      r4, [sp, #4]
003c7f88: bl       #0x3c7b18
003c7f8c: mov      r0, r5
003c7f90: mov      r1, r6
003c7f94: movw     r2, #0xc356
003c7f98: mov      r3, #7
003c7f9c: str      r4, [sp, #0x38]
003c7fa0: str      r4, [sp, #0x3c]
003c7fa4: str      r4, [sp]
003c7fa8: str      r4, [sp, #4]
003c7fac: bl       #0x3c7b18
003c7fb0: mov      r0, r5
003c7fb4: mov      r1, r6
003c7fb8: movw     r2, #0xc355
003c7fbc: mov      r3, #6
003c7fc0: str      r4, [sp, #0x30]
003c7fc4: str      r4, [sp, #0x34]
003c7fc8: str      r4, [sp]
003c7fcc: str      r4, [sp, #4]
003c7fd0: bl       #0x3c7b18
003c7fd4: ldr      r3, [pc, #0xcc]
003c7fd8: mov      r0, r5
003c7fdc: mov      r1, r6
003c7fe0: ldr      ip, [r8, r3]
003c7fe4: movw     r2, #0xc354
003c7fe8: mov      r3, #5
003c7fec: str      ip, [sp]
003c7ff0: str      ip, [sp, #0x28]
003c7ff4: str      r4, [sp, #0x2c]
003c7ff8: str      r4, [sp, #4]
003c7ffc: bl       #0x3c7b18
003c8000: mov      r0, r5
003c8004: mov      r1, r6
003c8008: movw     r2, #0xc351
003c800c: mov      r3, #4
003c8010: str      r4, [sp, #0x20]
003c8014: str      r4, [sp, #0x24]
003c8018: str      r4, [sp]
003c801c: str      r4, [sp, #4]
003c8020: bl       #0x3c7b18
003c8024: mov      r0, r5
003c8028: mov      r1, r6
003c802c: movw     r2, #0xc352
003c8030: mov      r3, #3
003c8034: str      r4, [sp, #0x18]
003c8038: str      r4, [sp, #0x1c]
003c803c: str      r4, [sp]
003c8040: str      r4, [sp, #4]
003c8044: bl       #0x3c7b18
003c8048: mov      r0, r5
003c804c: mov      r1, r6
003c8050: movw     r2, #0xc353
003c8054: mov      r3, #0xd
003c8058: str      r4, [sp, #0x10]
003c805c: str      r4, [sp, #0x14]
003c8060: str      r4, [sp]
003c8064: str      r4, [sp, #4]
003c8068: bl       #0x3c7b18
003c806c: mov      r0, r5
003c8070: mov      r1, r6
003c8074: movw     r2, #0xc357
003c8078: mov      r3, #0xf
003c807c: str      r4, [sp, #8]
003c8080: str      r4, [sp, #0xc]
003c8084: str      r4, [sp]
003c8088: str      r4, [sp, #4]
003c808c: bl       #0x3c7b18
003c8090: strb     r4, [sl, #0x538]
003c8094: add      sp, sp, #0x7c
003c8098: pop      {r4, r5, r6, r7, r8, sl, pc}
003c809c: subseq   ip, ip, r4, lsr #23
003c80a0: andeq    r2, r0, r4, lsl #29
003c80a4: andeq    r3, r0, ip, asr #9
003c80a8: ldrdeq   r0, r1, [r0], -r4

# _ZN10CSInteract6OnInitEiP9CharacterP16CharStateMachine
003c8984: push     {r4, r5, r6, r7, lr}
003c8988: ldr      r7, [pc, #0x18c]
003c898c: ldr      r3, [pc, #0x18c]
003c8990: add      r5, r2, #0x4f0
003c8994: add      r7, pc, r7
003c8998: ldr      ip, [r7, r3]
003c899c: add      r5, r5, #0xc
003c89a0: sub      sp, sp, #0x5c
003c89a4: mov      r4, #0
003c89a8: mov      r6, r1
003c89ac: mov      r0, r5
003c89b0: mov      r2, #0x22
003c89b4: mov      r3, #3
003c89b8: str      ip, [sp]
003c89bc: str      ip, [sp, #0x50]
003c89c0: str      r4, [sp, #0x54]
003c89c4: str      r4, [sp, #4]
003c89c8: bl       #0x3c7b18
003c89cc: mov      r0, r5
003c89d0: mov      r1, r6
003c89d4: movw     r2, #0xc358
003c89d8: mov      r3, #0xc
003c89dc: str      r4, [sp, #0x48]
003c89e0: str      r4, [sp, #0x4c]
003c89e4: str      r4, [sp]
003c89e8: str      r4, [sp, #4]
003c89ec: bl       #0x3c7b18
003c89f0: mov      r0, r5
003c89f4: mov      r1, r6
003c89f8: movw     r2, #0xc35a
003c89fc: mov      r3, #0xb
003c8a00: str      r4, [sp, #0x40]
003c8a04: str      r4, [sp, #0x44]
003c8a08: str      r4, [sp]
003c8a0c: str      r4, [sp, #4]
003c8a10: bl       #0x3c7b18
003c8a14: mov      r0, r5
003c8a18: mov      r1, r6
003c8a1c: movw     r2, #0xc35b
003c8a20: mov      r3, #0xa
003c8a24: str      r4, [sp, #0x38]
003c8a28: str      r4, [sp, #0x3c]
003c8a2c: str      r4, [sp]
003c8a30: str      r4, [sp, #4]
003c8a34: bl       #0x3c7b18
003c8a38: mov      r0, r5
003c8a3c: mov      r1, r6
003c8a40: movw     r2, #0xc35c
003c8a44: mov      r3, #9
003c8a48: str      r4, [sp, #0x30]
003c8a4c: str      r4, [sp, #0x34]
003c8a50: str      r4, [sp]
003c8a54: str      r4, [sp, #4]
003c8a58: bl       #0x3c7b18
003c8a5c: mov      r0, r5
003c8a60: mov      r1, r6
003c8a64: movw     r2, #0xc35d
003c8a68: mov      r3, #8
003c8a6c: str      r4, [sp, #0x28]
003c8a70: str      r4, [sp, #0x2c]
003c8a74: str      r4, [sp]
003c8a78: str      r4, [sp, #4]
003c8a7c: bl       #0x3c7b18
003c8a80: ldr      r3, [pc, #0x9c]
003c8a84: mov      r0, r5
003c8a88: mov      r1, r6
003c8a8c: ldr      r7, [r7, r3]
003c8a90: movw     r2, #0xc355
003c8a94: mov      r3, #6
003c8a98: str      r7, [sp, #0x20]
003c8a9c: str      r4, [sp, #0x24]
003c8aa0: str      r7, [sp]
003c8aa4: str      r4, [sp, #4]
003c8aa8: bl       #0x3c7b18
003c8aac: mov      r0, r5
003c8ab0: mov      r1, r6
003c8ab4: movw     r2, #0xc351
003c8ab8: mov      r3, #4
003c8abc: str      r7, [sp, #0x18]
003c8ac0: str      r4, [sp, #0x1c]
003c8ac4: str      r7, [sp]
003c8ac8: str      r4, [sp, #4]
003c8acc: bl       #0x3c7b18
003c8ad0: mov      r0, r5
003c8ad4: mov      r1, r6
003c8ad8: movw     r2, #0xc352
003c8adc: mov      r3, #3
003c8ae0: str      r7, [sp, #0x10]
003c8ae4: str      r4, [sp, #0x14]
003c8ae8: str      r7, [sp]
003c8aec: str      r4, [sp, #4]
003c8af0: bl       #0x3c7b18
003c8af4: mov      r0, r5
003c8af8: mov      r1, r6
003c8afc: movw     r2, #0xc357
003c8b00: mov      r3, #0xf
003c8b04: str      r7, [sp]
003c8b08: stmib    sp, {r4, r7}
003c8b0c: str      r4, [sp, #0xc]
003c8b10: bl       #0x3c7b18
003c8b14: add      sp, sp, #0x5c
003c8b18: pop      {r4, r5, r6, r7, pc}
003c8b1c: ldrsheq  ip, [ip], #-0xc
003c8b20: muleq    r0, r4, r8
003c8b24: andeq    r0, r0, r0, ror sp

# _ZN13CSKnockedBack6OnInitEiP9CharacterP16CharStateMachine
003c87d8: push     {r4, r5, r6, r7, lr}
003c87dc: add      r6, r2, #0x4f0
003c87e0: add      r6, r6, #0xc
003c87e4: sub      sp, sp, #0x1c
003c87e8: mov      r4, #0
003c87ec: mov      r0, r6
003c87f0: mov      r2, #0x22
003c87f4: mov      r3, #3
003c87f8: ldr      r5, [pc, #0x48]
003c87fc: mov      r7, r1
003c8800: str      r4, [sp, #0x10]
003c8804: str      r4, [sp, #0x14]
003c8808: str      r4, [sp]
003c880c: str      r4, [sp, #4]
003c8810: bl       #0x3c7b18
003c8814: ldr      r3, [pc, #0x30]
003c8818: add      r5, pc, r5
003c881c: mov      r0, r6
003c8820: ldr      ip, [r5, r3]
003c8824: mov      r1, r7
003c8828: movw     r2, #0xc358
003c882c: mov      r3, #0xc
003c8830: str      ip, [sp]
003c8834: stmib    sp, {r4, ip}
003c8838: str      r4, [sp, #0xc]
003c883c: bl       #0x3c7b18
003c8840: add      sp, sp, #0x1c
003c8844: pop      {r4, r5, r6, r7, pc}
003c8848: subseq   ip, ip, r8, ror r2
003c884c: andeq    r3, r0, ip, asr #9

# _ZN7CSSkill6OnInitEiP9CharacterP16CharStateMachine
003c8438: push     {r4, r5, r6, r7, r8, lr}
003c843c: add      r5, r2, #0x4f0
003c8440: add      r5, r5, #0xc
003c8444: sub      sp, sp, #0x50
003c8448: mov      r4, #0
003c844c: mov      r6, r1
003c8450: mov      r0, r5
003c8454: mov      r2, #0x22
003c8458: mov      r3, #3
003c845c: str      r4, [sp, #0x48]
003c8460: str      r4, [sp, #0x4c]
003c8464: str      r4, [sp]
003c8468: str      r4, [sp, #4]
003c846c: ldr      r8, [pc, #0x138]
003c8470: bl       #0x3c7b18
003c8474: mov      r0, r5
003c8478: mov      r1, r6
003c847c: movw     r2, #0xc358
003c8480: mov      r3, #0xc
003c8484: str      r4, [sp, #0x40]
003c8488: str      r4, [sp, #0x44]
003c848c: str      r4, [sp]
003c8490: str      r4, [sp, #4]
003c8494: bl       #0x3c7b18
003c8498: ldr      r3, [pc, #0x110]
003c849c: add      r8, pc, r8
003c84a0: mov      r0, r5
003c84a4: ldr      r7, [r8, r3]
003c84a8: mov      r1, r6
003c84ac: movw     r2, #0xc351
003c84b0: mov      r3, #4
003c84b4: str      r7, [sp, #0x38]
003c84b8: str      r7, [sp]
003c84bc: str      r4, [sp, #0x3c]
003c84c0: str      r4, [sp, #4]
003c84c4: bl       #0x3c7b18
003c84c8: mov      r0, r5
003c84cc: mov      r1, r6
003c84d0: movw     r2, #0xc354
003c84d4: mov      r3, #5
003c84d8: str      r7, [sp, #0x30]
003c84dc: str      r7, [sp]
003c84e0: str      r4, [sp, #0x34]
003c84e4: str      r4, [sp, #4]
003c84e8: bl       #0x3c7b18
003c84ec: mov      r0, r5
003c84f0: mov      r1, r6
003c84f4: movw     r2, #0xc355
003c84f8: mov      r3, #6
003c84fc: str      r7, [sp]
003c8500: str      r7, [sp, #0x28]
003c8504: str      r4, [sp, #0x2c]
003c8508: str      r4, [sp, #4]
003c850c: bl       #0x3c7b18
003c8510: ldr      r3, [pc, #0x9c]
003c8514: mov      r0, r5
003c8518: mov      r1, r6
003c851c: ldr      r7, [r8, r3]
003c8520: movw     r2, #0xc35a
003c8524: mov      r3, #0xb
003c8528: str      r7, [sp, #0x20]
003c852c: str      r4, [sp, #0x24]
003c8530: str      r7, [sp]
003c8534: str      r4, [sp, #4]
003c8538: bl       #0x3c7b18
003c853c: mov      r0, r5
003c8540: mov      r1, r6
003c8544: movw     r2, #0xc35c
003c8548: mov      r3, #9
003c854c: str      r7, [sp, #0x18]
003c8550: str      r4, [sp, #0x1c]
003c8554: str      r7, [sp]
003c8558: str      r4, [sp, #4]
003c855c: bl       #0x3c7b18
003c8560: mov      r0, r5
003c8564: mov      r1, r6
003c8568: movw     r2, #0xc35d
003c856c: mov      r3, #8
003c8570: str      r7, [sp, #0x10]
003c8574: str      r4, [sp, #0x14]
003c8578: str      r7, [sp]
003c857c: str      r4, [sp, #4]
003c8580: bl       #0x3c7b18
003c8584: mov      r0, r5
003c8588: mov      r1, r6
003c858c: movw     r2, #0xc35b
003c8590: mov      r3, #0xa
003c8594: str      r7, [sp]
003c8598: stmib    sp, {r4, r7}
003c859c: str      r4, [sp, #0xc]
003c85a0: bl       #0x3c7b18
003c85a4: add      sp, sp, #0x50
003c85a8: pop      {r4, r5, r6, r7, r8, pc}
003c85ac: ldrsheq  ip, [ip], #-0x54
003c85b0: strheq   r4, [r0], -ip
003c85b4: andeq    r3, r0, ip, asr #9

# _ZN8CSScared6OnInitEiP9CharacterP16CharStateMachine
003c861c: push     {r4, r5, r6, r7, lr}
003c8620: add      r5, r2, #0x4f0
003c8624: add      r5, r5, #0xc
003c8628: sub      sp, sp, #0x3c
003c862c: mov      r4, #0
003c8630: mov      r6, r1
003c8634: mov      r0, r5
003c8638: mov      r2, #0x2c
003c863c: mov      r3, #3
003c8640: str      r4, [sp, #0x30]
003c8644: str      r4, [sp, #0x34]
003c8648: str      r4, [sp]
003c864c: str      r4, [sp, #4]
003c8650: bl       #0x3c7b18
003c8654: mov      r0, r5
003c8658: mov      r1, r6
003c865c: mov      r2, #0x22
003c8660: mov      r3, #3
003c8664: str      r4, [sp, #0x28]
003c8668: str      r4, [sp, #0x2c]
003c866c: str      r4, [sp]
003c8670: str      r4, [sp, #4]
003c8674: ldr      r7, [pc, #0xa8]
003c8678: bl       #0x3c7b18
003c867c: mov      r0, r5
003c8680: mov      r1, r6
003c8684: movw     r2, #0xc358
003c8688: mov      r3, #0xc
003c868c: str      r4, [sp, #0x20]
003c8690: str      r4, [sp, #0x24]
003c8694: str      r4, [sp]
003c8698: str      r4, [sp, #4]
003c869c: bl       #0x3c7b18
003c86a0: ldr      r3, [pc, #0x80]
003c86a4: add      r7, pc, r7
003c86a8: mov      r0, r5
003c86ac: ldr      ip, [r7, r3]
003c86b0: mov      r1, r6
003c86b4: movw     r2, #0xc35a
003c86b8: mov      r3, #0xb
003c86bc: str      ip, [sp]
003c86c0: str      ip, [sp, #0x18]
003c86c4: str      r4, [sp, #0x1c]
003c86c8: str      r4, [sp, #4]
003c86cc: bl       #0x3c7b18
003c86d0: ldr      r3, [pc, #0x54]
003c86d4: mov      r0, r5
003c86d8: mov      r1, r6
003c86dc: ldr      r7, [r7, r3]
003c86e0: movw     r2, #0xc35b
003c86e4: mov      r3, #0xa
003c86e8: str      r7, [sp, #0x10]
003c86ec: str      r4, [sp, #0x14]
003c86f0: str      r7, [sp]
003c86f4: str      r4, [sp, #4]
003c86f8: bl       #0x3c7b18
003c86fc: mov      r0, r5
003c8700: mov      r1, r6
003c8704: movw     r2, #0xc35c
003c8708: mov      r3, #9
003c870c: str      r7, [sp]
003c8710: stmib    sp, {r4, r7}
003c8714: str      r4, [sp, #0xc]
003c8718: bl       #0x3c7b18
003c871c: add      sp, sp, #0x3c
003c8720: pop      {r4, r5, r6, r7, pc}
003c8724: subseq   ip, ip, ip, ror #7
003c8728: andeq    r2, r0, r4, lsl #29
003c872c: andeq    r3, r0, ip, asr #9

# _ZN10CSPreSpawn6OnInitEiP9CharacterP16CharStateMachine
003c8d24: str      lr, [sp, #-4]!
003c8d28: add      r0, r2, #0x4f0
003c8d2c: sub      sp, sp, #0x14
003c8d30: mov      ip, #0
003c8d34: add      r0, r0, #0xc
003c8d38: mov      r2, #0x2d
003c8d3c: mov      r3, #1
003c8d40: str      ip, [sp, #4]
003c8d44: str      ip, [sp, #8]
003c8d48: str      ip, [sp, #0xc]
003c8d4c: str      ip, [sp]
003c8d50: bl       #0x3c7b18
003c8d54: add      sp, sp, #0x14
003c8d58: ldm      sp!, {pc}

# _ZN9CSDespawn6OnInitEiP9CharacterP16CharStateMachine
003c7dfc: push     {r4, r5, r6, lr}
003c7e00: add      r5, r2, #0x4f0
003c7e04: mov      r4, #0
003c7e08: add      r5, r5, #0xc
003c7e0c: sub      sp, sp, #0x18
003c7e10: mov      r6, r1
003c7e14: mov      r0, r5
003c7e18: mov      r3, r4
003c7e1c: mov      r2, #0x40
003c7e20: str      r4, [sp, #0x10]
003c7e24: str      r4, [sp, #0x14]
003c7e28: str      r4, [sp]
003c7e2c: str      r4, [sp, #4]
003c7e30: bl       #0x3c7b18
003c7e34: mov      r0, r5
003c7e38: mov      r1, r6
003c7e3c: mov      r3, r4
003c7e40: mov      r2, #0x22
003c7e44: str      r4, [sp, #8]
003c7e48: str      r4, [sp, #0xc]
003c7e4c: str      r4, [sp]
003c7e50: str      r4, [sp, #4]
003c7e54: bl       #0x3c7b18
003c7e58: add      sp, sp, #0x18
003c7e5c: pop      {r4, r5, r6, pc}

# _ZN9CSRevived6OnInitEiP9CharacterP16CharStateMachine
003c8cec: str      lr, [sp, #-4]!
003c8cf0: add      r0, r2, #0x4f0
003c8cf4: sub      sp, sp, #0x14
003c8cf8: mov      ip, #0
003c8cfc: add      r0, r0, #0xc
003c8d00: mov      r2, #0x22
003c8d04: mov      r3, #3
003c8d08: str      ip, [sp, #4]
003c8d0c: str      ip, [sp, #8]
003c8d10: str      ip, [sp, #0xc]
003c8d14: str      ip, [sp]
003c8d18: bl       #0x3c7b18
003c8d1c: add      sp, sp, #0x14
003c8d20: ldm      sp!, {pc}

# _ZN6CSMove6OnInitEiP9CharacterP16CharStateMachine
003c80ac: push     {r4, r5, r6, r7, r8, lr}
003c80b0: add      r5, r2, #0x4f0
003c80b4: add      r5, r5, #0xc
003c80b8: sub      sp, sp, #0x60
003c80bc: mov      r4, #0
003c80c0: mov      r6, r1
003c80c4: mov      r0, r5
003c80c8: mov      r2, #0x3f
003c80cc: mov      r3, #3
003c80d0: str      r4, [sp, #0x58]
003c80d4: str      r4, [sp, #0x5c]
003c80d8: str      r4, [sp]
003c80dc: str      r4, [sp, #4]
003c80e0: ldr      r8, [pc, #0x18c]
003c80e4: bl       #0x3c7b18
003c80e8: mov      r0, r5
003c80ec: mov      r1, r6
003c80f0: movw     r2, #0xc358
003c80f4: mov      r3, #0xc
003c80f8: str      r4, [sp, #0x50]
003c80fc: str      r4, [sp, #0x54]
003c8100: str      r4, [sp]
003c8104: str      r4, [sp, #4]
003c8108: bl       #0x3c7b18
003c810c: ldr      r3, [pc, #0x164]
003c8110: add      r8, pc, r8
003c8114: mov      r0, r5
003c8118: ldr      ip, [r8, r3]
003c811c: mov      r1, r6
003c8120: movw     r2, #0xc35a
003c8124: mov      r3, #0xb
003c8128: str      ip, [sp]
003c812c: str      ip, [sp, #0x48]
003c8130: str      r4, [sp, #0x4c]
003c8134: str      r4, [sp, #4]
003c8138: bl       #0x3c7b18
003c813c: ldr      r3, [pc, #0x138]
003c8140: mov      r0, r5
003c8144: mov      r1, r6
003c8148: ldr      r7, [r8, r3]
003c814c: movw     r2, #0xc35b
003c8150: mov      r3, #0xa
003c8154: str      r7, [sp, #0x40]
003c8158: str      r4, [sp, #0x44]
003c815c: str      r7, [sp]
003c8160: str      r4, [sp, #4]
003c8164: bl       #0x3c7b18
003c8168: mov      r0, r5
003c816c: mov      r1, r6
003c8170: movw     r2, #0xc35c
003c8174: mov      r3, #9
003c8178: str      r7, [sp, #0x38]
003c817c: str      r4, [sp, #0x3c]
003c8180: str      r7, [sp]
003c8184: str      r4, [sp, #4]
003c8188: bl       #0x3c7b18
003c818c: mov      r0, r5
003c8190: mov      r1, r6
003c8194: movw     r2, #0xc35d
003c8198: mov      r3, #8
003c819c: str      r7, [sp]
003c81a0: str      r7, [sp, #0x30]
003c81a4: str      r4, [sp, #0x34]
003c81a8: str      r4, [sp, #4]
003c81ac: bl       #0x3c7b18
003c81b0: mov      r0, r5
003c81b4: mov      r1, r6
003c81b8: movw     r2, #0xc356
003c81bc: mov      r3, #7
003c81c0: str      r4, [sp, #0x28]
003c81c4: str      r4, [sp, #0x2c]
003c81c8: str      r4, [sp]
003c81cc: str      r4, [sp, #4]
003c81d0: bl       #0x3c7b18
003c81d4: mov      r0, r5
003c81d8: mov      r1, r6
003c81dc: movw     r2, #0xc355
003c81e0: mov      r3, #6
003c81e4: str      r4, [sp, #0x20]
003c81e8: str      r4, [sp, #0x24]
003c81ec: str      r4, [sp]
003c81f0: str      r4, [sp, #4]
003c81f4: bl       #0x3c7b18
003c81f8: ldr      r3, [pc, #0x80]
003c81fc: mov      r0, r5
003c8200: mov      r1, r6
003c8204: ldr      ip, [r8, r3]
003c8208: movw     r2, #0xc354
003c820c: mov      r3, #5
003c8210: str      ip, [sp]
003c8214: str      ip, [sp, #0x18]
003c8218: str      r4, [sp, #0x1c]
003c821c: str      r4, [sp, #4]
003c8220: bl       #0x3c7b18
003c8224: mov      r0, r5
003c8228: mov      r1, r6
003c822c: movw     r2, #0xc353
003c8230: mov      r3, #0xd
003c8234: str      r4, [sp, #0x10]
003c8238: str      r4, [sp, #0x14]
003c823c: str      r4, [sp]
003c8240: str      r4, [sp, #4]
003c8244: bl       #0x3c7b18
003c8248: mov      r0, r5
003c824c: mov      r1, r6
003c8250: movw     r2, #0xc357
003c8254: mov      r3, #0xf
003c8258: str      r4, [sp, #4]
003c825c: str      r4, [sp, #8]
003c8260: str      r4, [sp, #0xc]
003c8264: str      r4, [sp]
003c8268: bl       #0x3c7b18
003c826c: add      sp, sp, #0x60
003c8270: pop      {r4, r5, r6, r7, r8, pc}
003c8274: subseq   ip, ip, r0, lsl #19
003c8278: andeq    r2, r0, r4, lsl #29
003c827c: andeq    r3, r0, ip, asr #9
003c8280: ldrdeq   r0, r1, [r0], -r4

# _ZN6CSAnim6OnInitEiP9CharacterP16CharStateMachine
003c8b28: push     {r4, r5, r6, lr}
003c8b2c: add      r5, r2, #0x4f0
003c8b30: add      r5, r5, #0xc
003c8b34: sub      sp, sp, #0x18
003c8b38: mov      r4, #0
003c8b3c: mov      r6, r1
003c8b40: mov      r0, r5
003c8b44: movw     r2, #0xc351
003c8b48: mov      r3, #4
003c8b4c: str      r4, [sp, #0x10]
003c8b50: str      r4, [sp, #0x14]
003c8b54: str      r4, [sp]
003c8b58: str      r4, [sp, #4]
003c8b5c: bl       #0x3c7b18
003c8b60: mov      r0, r5
003c8b64: mov      r1, r6
003c8b68: movw     r2, #0xc355
003c8b6c: mov      r3, #6
003c8b70: str      r4, [sp, #4]
003c8b74: str      r4, [sp, #8]
003c8b78: str      r4, [sp, #0xc]
003c8b7c: str      r4, [sp]
003c8b80: bl       #0x3c7b18
003c8b84: add      sp, sp, #0x18
003c8b88: pop      {r4, r5, r6, pc}

# _ZN6CSCast6OnInitEiP9CharacterP16CharStateMachine
003c85b8: push     {r4, r5, r6, lr}
003c85bc: add      r5, r2, #0x4f0
003c85c0: add      r5, r5, #0xc
003c85c4: sub      sp, sp, #0x18
003c85c8: mov      r4, #0
003c85cc: mov      r6, r1
003c85d0: mov      r0, r5
003c85d4: mov      r2, #0x22
003c85d8: mov      r3, #3
003c85dc: str      r4, [sp, #0x10]
003c85e0: str      r4, [sp, #0x14]
003c85e4: str      r4, [sp]
003c85e8: str      r4, [sp, #4]
003c85ec: bl       #0x3c7b18
003c85f0: mov      r0, r5
003c85f4: mov      r1, r6
003c85f8: movw     r2, #0xc358
003c85fc: mov      r3, #0xc
003c8600: str      r4, [sp, #4]
003c8604: str      r4, [sp, #8]
003c8608: str      r4, [sp, #0xc]
003c860c: str      r4, [sp]
003c8610: bl       #0x3c7b18
003c8614: add      sp, sp, #0x18
003c8618: pop      {r4, r5, r6, pc}

# _ZN9CSStunned6OnInitEiP9CharacterP16CharStateMachine
003c8730: push     {r4, r5, r6, r7, lr}
003c8734: add      r6, r2, #0x4f0
003c8738: add      r6, r6, #0xc
003c873c: sub      sp, sp, #0x24
003c8740: mov      r4, #0
003c8744: mov      r0, r6
003c8748: movw     r2, #0xc358
003c874c: mov      r3, #0xc
003c8750: ldr      r5, [pc, #0x74]
003c8754: mov      r7, r1
003c8758: str      r4, [sp, #0x18]
003c875c: str      r4, [sp, #0x1c]
003c8760: str      r4, [sp]
003c8764: str      r4, [sp, #4]
003c8768: bl       #0x3c7b18
003c876c: ldr      r3, [pc, #0x5c]
003c8770: add      r5, pc, r5
003c8774: mov      r0, r6
003c8778: ldr      ip, [r5, r3]
003c877c: mov      r1, r7
003c8780: movw     r2, #0xc35a
003c8784: mov      r3, #0xb
003c8788: str      ip, [sp]
003c878c: str      ip, [sp, #0x10]
003c8790: str      r4, [sp, #0x14]
003c8794: str      r4, [sp, #4]
003c8798: bl       #0x3c7b18
003c879c: ldr      r3, [pc, #0x30]
003c87a0: mov      r0, r6
003c87a4: mov      r1, r7
003c87a8: ldr      ip, [r5, r3]
003c87ac: movw     r2, #0xc35b
003c87b0: mov      r3, #0xa
003c87b4: str      ip, [sp]
003c87b8: stmib    sp, {r4, ip}
003c87bc: str      r4, [sp, #0xc]
003c87c0: bl       #0x3c7b18
003c87c4: add      sp, sp, #0x24
003c87c8: pop      {r4, r5, r6, r7, pc}
003c87cc: subseq   ip, ip, r0, lsr #6
003c87d0: andeq    r2, r0, r4, lsl #29
003c87d4: andeq    r3, r0, ip, asr #9
