
# _ZN8RenderFX9SetBoundsEiiiiN7gameswf10scale_modeE
007a9b30: push     {r4, r5, r6, r7, r8, sl, lr}
007a9b34: sub      sp, sp, #0xc
007a9b38: ldr      r0, [r0, #0x38]
007a9b3c: mov      sl, r1
007a9b40: mov      r8, r2
007a9b44: mov      r7, r3
007a9b48: ldr      r6, [sp, #0x28]
007a9b4c: ldr      r5, [sp, #0x2c]
007a9b50: bl       #0x76d5b4
007a9b54: subs     r4, r0, #0
007a9b58: beq      #0x7a9b8c
007a9b5c: bl       #0x759c64
007a9b60: mov      r0, r4
007a9b64: mov      r1, sl
007a9b68: mov      r2, r8
007a9b6c: mov      r3, r7
007a9b70: str      r6, [sp]
007a9b74: str      r5, [sp, #4]
007a9b78: bl       #0x7755f4
007a9b7c: mov      r0, r4
007a9b80: add      sp, sp, #0xc
007a9b84: pop      {r4, r5, r6, r7, r8, sl, lr}
007a9b88: b        #0x75a240
007a9b8c: mov      r1, sl
007a9b90: mov      r2, r8
007a9b94: mov      r3, r7
007a9b98: str      r6, [sp, #0x28]
007a9b9c: str      r5, [sp, #0x2c]
007a9ba0: add      sp, sp, #0xc
007a9ba4: pop      {r4, r5, r6, r7, r8, sl, lr}
007a9ba8: b        #0x7755f4

# _ZN12GameSWFUtils16SwfTextureLoaderEPKcii
00416c14: push     {r4, r5, r6, r7, lr}
00416c18: ldr      r4, [pc, #0x344]
00416c1c: ldr      r6, [pc, #0x344]
00416c20: ldr      r1, [pc, #0x344]
00416c24: add      r4, pc, r4
00416c28: ldr      r3, [r4, r6]
00416c2c: sub      sp, sp, #0x10c
00416c30: add      r1, pc, r1
00416c34: ldr      r3, [r3]
00416c38: mov      r7, r0
00416c3c: str      r3, [sp, #0x104]
00416c40: bl       #0x30e31c
00416c44: cmp      r0, #0
00416c48: beq      #0x416d04
00416c4c: ldr      r1, [pc, #0x31c]
00416c50: mov      r0, r7
00416c54: add      r1, pc, r1
00416c58: bl       #0x30e31c
00416c5c: cmp      r0, #0
00416c60: beq      #0x416d50
00416c64: ldr      r1, [pc, #0x308]
00416c68: mov      r0, r7
00416c6c: add      r1, pc, r1
00416c70: bl       #0x30e31c
00416c74: cmp      r0, #0
00416c78: beq      #0x416dd8
00416c7c: ldr      r1, [pc, #0x2f4]
00416c80: mov      r0, r7
00416c84: add      r1, pc, r1
00416c88: bl       #0x30e31c
00416c8c: cmp      r0, #0
00416c90: beq      #0x416d9c
00416c94: ldr      r1, [pc, #0x2e0]
00416c98: add      r5, sp, #4
00416c9c: mov      r2, r7
00416ca0: add      r1, pc, r1
00416ca4: mov      r0, r5
00416ca8: bl       #0x30eae4
00416cac: ldr      r7, [pc, #0x2cc]
00416cb0: ldr      r3, [r4, r7]
00416cb4: mov      r2, r5
00416cb8: mov      r0, sp
00416cbc: ldr      r1, [r3, #0x10]
00416cc0: mov      r3, #0
00416cc4: ldr      r1, [r1, #0x10]
00416cc8: ldr      r1, [r1, #0xe0]
00416ccc: bl       #0x5ed210
00416cd0: ldr      r5, [sp]
00416cd4: cmp      r5, #0
00416cd8: beq      #0x416ce4
00416cdc: mov      r0, r5
00416ce0: bl       #0x31d584
00416ce4: ldr      r3, [r4, r6]
00416ce8: ldr      r2, [sp, #0x104]
00416cec: mov      r0, r5
00416cf0: ldr      r3, [r3]
00416cf4: cmp      r2, r3
00416cf8: bne      #0x416f60
00416cfc: add      sp, sp, #0x10c
00416d00: pop      {r4, r5, r6, r7, pc}
00416d04: ldr      r7, [pc, #0x274]
00416d08: ldr      r3, [r4, r7]
00416d0c: ldr      r0, [r3, #0x4c]
00416d10: bl       #0x46d514
00416d14: cmp      r0, #4
00416d18: beq      #0x416e68
00416d1c: cmp      r0, #5
00416d20: beq      #0x416e08
00416d24: ldr      lr, [pc, #0x258]
00416d28: add      r5, sp, #4
00416d2c: mov      ip, r5
00416d30: add      lr, pc, lr
00416d34: ldm      lr!, {r0, r1, r2, r3}
00416d38: stm      ip!, {r0, r1, r2, r3}
00416d3c: ldm      lr!, {r0, r1, r2, r3}
00416d40: stm      ip!, {r0, r1, r2, r3}
00416d44: ldrh     lr, [lr]
00416d48: strh     lr, [ip]
00416d4c: b        #0x416cb0
00416d50: ldr      r7, [pc, #0x228]
00416d54: ldr      r3, [r4, r7]
00416d58: ldr      r0, [r3, #0x4c]
00416d5c: bl       #0x46d514
00416d60: cmp      r0, #4
00416d64: beq      #0x416e38
00416d68: cmp      r0, #5
00416d6c: beq      #0x416e78
00416d70: ldr      lr, [pc, #0x210]
00416d74: add      r5, sp, #4
00416d78: add      lr, pc, lr
00416d7c: mov      ip, r5
00416d80: ldm      lr!, {r0, r1, r2, r3}
00416d84: stm      ip!, {r0, r1, r2, r3}
00416d88: ldm      lr!, {r0, r1, r2, r3}
00416d8c: stm      ip!, {r0, r1, r2, r3}
00416d90: ldm      lr, {r0, r1}
00416d94: stm      ip, {r0, r1}
00416d98: b        #0x416cb0
00416d9c: ldr      r7, [pc, #0x1dc]
00416da0: ldr      r3, [r4, r7]
00416da4: ldr      r0, [r3, #0x4c]
00416da8: bl       #0x46d514
00416dac: sub      r0, r0, #1
00416db0: cmp      r0, #6
00416db4: addls    pc, pc, r0, lsl #2
00416db8: b        #0x416f30
00416dbc: b        #0x416f20
00416dc0: b        #0x416f10
00416dc4: b        #0x416f00
00416dc8: b        #0x416edc
00416dcc: b        #0x416ecc
00416dd0: b        #0x416ebc
00416dd4: b        #0x416e88
00416dd8: ldr      r7, [pc, #0x1a0]
00416ddc: ldr      r3, [r4, r7]
00416de0: ldr      r0, [r3, #0x4c]
00416de4: bl       #0x46d514
00416de8: cmp      r0, #4
00416dec: beq      #0x416f50
00416df0: cmp      r0, #5
00416df4: beq      #0x416f40
00416df8: ldr      lr, [pc, #0x18c]
00416dfc: add      r5, sp, #4
00416e00: add      lr, pc, lr
00416e04: b        #0x416d7c
00416e08: ldr      lr, [pc, #0x180]
00416e0c: add      r5, sp, #4
00416e10: add      lr, pc, lr
00416e14: mov      ip, r5
00416e18: ldm      lr!, {r0, r1, r2, r3}
00416e1c: stm      ip!, {r0, r1, r2, r3}
00416e20: ldm      lr!, {r0, r1, r2, r3}
00416e24: stm      ip!, {r0, r1, r2, r3}
00416e28: ldm      lr, {r0, r1}
00416e2c: str      r0, [ip], #4
00416e30: strh     r1, [ip]
00416e34: b        #0x416cb0
00416e38: ldr      lr, [pc, #0x154]
00416e3c: add      r5, sp, #4
00416e40: add      lr, pc, lr
00416e44: mov      ip, r5
00416e48: ldm      lr!, {r0, r1, r2, r3}
00416e4c: stm      ip!, {r0, r1, r2, r3}
00416e50: ldm      lr!, {r0, r1, r2, r3}
00416e54: stm      ip!, {r0, r1, r2, r3}
00416e58: ldm      lr, {r0, r1}
00416e5c: str      r0, [ip], #4
00416e60: strb     r1, [ip]
00416e64: b        #0x416cb0
00416e68: ldr      lr, [pc, #0x128]
00416e6c: add      r5, sp, #4
00416e70: add      lr, pc, lr
00416e74: b        #0x416e44
00416e78: ldr      lr, [pc, #0x11c]
00416e7c: add      r5, sp, #4
00416e80: add      lr, pc, lr
00416e84: b        #0x416e14
00416e88: ldr      lr, [pc, #0x110]
00416e8c: add      r5, sp, #4
00416e90: add      lr, pc, lr
00416e94: mov      ip, r5
00416e98: ldm      lr!, {r0, r1, r2, r3}
00416e9c: stm      ip!, {r0, r1, r2, r3}
00416ea0: ldm      lr!, {r0, r1, r2, r3}
00416ea4: stm      ip!, {r0, r1, r2, r3}
00416ea8: ldr      r3, [lr]
00416eac: strh     r3, [ip], #2
00416eb0: lsr      r3, r3, #0x10
00416eb4: strb     r3, [ip]
00416eb8: b        #0x416cb0
00416ebc: ldr      lr, [pc, #0xe0]
00416ec0: add      r5, sp, #4
00416ec4: add      lr, pc, lr
00416ec8: b        #0x416e94
00416ecc: ldr      lr, [pc, #0xd4]
00416ed0: add      r5, sp, #4
00416ed4: add      lr, pc, lr
00416ed8: b        #0x416e94
00416edc: ldr      lr, [pc, #0xc8]
00416ee0: add      r5, sp, #4
00416ee4: add      lr, pc, lr
00416ee8: mov      ip, r5
00416eec: ldm      lr!, {r0, r1, r2, r3}
00416ef0: stm      ip!, {r0, r1, r2, r3}
00416ef4: ldm      lr, {r0, r1, r2, r3}
00416ef8: stm      ip, {r0, r1, r2, r3}
00416efc: b        #0x416cb0
00416f00: ldr      lr, [pc, #0xa8]
00416f04: add      r5, sp, #4
00416f08: add      lr, pc, lr
00416f0c: b        #0x416e94
00416f10: ldr      lr, [pc, #0x9c]
00416f14: add      r5, sp, #4
00416f18: add      lr, pc, lr
00416f1c: b        #0x416e94
00416f20: ldr      lr, [pc, #0x90]
00416f24: add      r5, sp, #4
00416f28: add      lr, pc, lr
00416f2c: b        #0x416e94
00416f30: ldr      lr, [pc, #0x84]
00416f34: add      r5, sp, #4
00416f38: add      lr, pc, lr
00416f3c: b        #0x416ee8
00416f40: ldr      lr, [pc, #0x78]
00416f44: add      r5, sp, #4
00416f48: add      lr, pc, lr
00416f4c: b        #0x416e14
00416f50: ldr      lr, [pc, #0x6c]
00416f54: add      r5, sp, #4
00416f58: add      lr, pc, lr
00416f5c: b        #0x416e44
00416f60: bl       #0x30e310
00416f64: subseq   sp, r7, ip, ror #28
00416f68: andeq    r4, r0, ip, lsr #1
00416f6c: ldrdeq   r1, r2, [fp], #-0x48
00416f70: subeq    r1, fp, ip, asr #9
00416f74: strdeq   r1, r2, [fp], #-0x4c
00416f78: subeq    r1, fp, r4, lsl #10
00416f7c: subeq    r1, fp, r0, lsl r6
00416f80: strdeq   r3, r4, [r0], -r4
00416f84: ldrdeq   fp, ip, [sl], #-0
00416f88: subeq    r1, fp, r8, asr #7
00416f8c: subeq    fp, sl, r8, lsl #2
00416f90: subeq    fp, sl, r8, lsl r0
00416f94: subeq    fp, sl, r8, ror r0
00416f98: subeq    fp, sl, r8, asr #32
00416f9c: subeq    sl, sl, r8, lsr #31
00416fa0: subeq    r1, fp, r8, lsl #7
00416fa4: subeq    r1, fp, ip, ror r3
00416fa8: strheq   r1, [fp], #-0x34
00416fac: subeq    r1, fp, r4, lsl #7
00416fb0: subeq    r1, fp, r8, ror #5
00416fb4: strheq   r1, [fp], #-0x20
00416fb8: subeq    r1, fp, r8, ror r2
00416fbc: subeq    r1, fp, r0, lsr r3
00416fc0: subeq    sl, sl, r0, ror #29
00416fc4: subeq    sl, sl, r0, ror #30
