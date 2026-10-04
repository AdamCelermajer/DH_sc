
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

# _ZN20DebugCachedCharacter12RefreshCacheEPKcP6MenuFXPN7gameswf9characterE
00427ca0: push     {r4, r5, r6, r7, r8, lr}
00427ca4: subs     r7, r3, #0
00427ca8: mov      r4, r0
00427cac: mov      r6, r2
00427cb0: mov      r5, r1
00427cb4: beq      #0x427d38
00427cb8: mov      r0, r2
00427cbc: mov      r2, r7
00427cc0: bl       #0x7a8a84
00427cc4: mov      r1, r0
00427cc8: add      r0, r4, #0x28
00427ccc: bl       #0x427ba8
00427cd0: mov      r0, r5
00427cd4: bl       #0x30de54
00427cd8: mov      r1, r5
00427cdc: add      r2, r5, r0
00427ce0: add      r0, r4, #8
00427ce4: bl       #0x3109e0
00427ce8: ldr      r3, [r4, #0x2c]
00427cec: str      r6, [r4, #0x20]
00427cf0: str      r7, [r4, #0x24]
00427cf4: cmp      r3, #0
00427cf8: beq      #0x427d0c
00427cfc: ldr      r0, [r4, #0x28]
00427d00: ldrb     r3, [r0, #4]
00427d04: cmp      r3, #0
00427d08: beq      #0x427d10
00427d0c: pop      {r4, r5, r6, r7, r8, pc}
00427d10: ldr      r1, [r0]
00427d14: sub      r1, r1, #1
00427d18: cmp      r1, #0
00427d1c: str      r1, [r0]
00427d20: bne      #0x427d28
00427d24: bl       #0x752b38
00427d28: mov      r3, #0
00427d2c: str      r3, [r4, #0x2c]
00427d30: str      r3, [r4, #0x28]
00427d34: pop      {r4, r5, r6, r7, r8, pc}
00427d38: mov      r0, r2
00427d3c: bl       #0x7a9160
00427d40: mov      r1, r0
00427d44: add      r0, r4, #0x28
00427d48: bl       #0x427ba8
00427d4c: b        #0x427cd0
