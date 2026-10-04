
# _ZN16MultiMenuManager11LoadSWFFileEPKci
00437d68: ldr      r3, [pc, #0xac]
00437d6c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00437d70: cmp      r2, #2
00437d74: mov      r4, r2
00437d78: ldr      r2, [pc, #0xa0]
00437d7c: add      r3, pc, r3
00437d80: add      r5, r4, #0x4c
00437d84: ldr      r3, [r3, r2]
00437d88: moveq    r2, #1
00437d8c: movne    r2, #0
00437d90: strb     r2, [r3]
00437d94: add      r5, r0, r5, lsl #2
00437d98: ldr      sl, [r5, #4]
00437d9c: mov      r6, r0
00437da0: mov      r7, r1
00437da4: cmp      sl, #0
00437da8: beq      #0x437db4
00437dac: mov      r0, sl
00437db0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00437db4: mov      r1, #8
00437db8: mov      r0, #0x124
00437dbc: bl       #0x310570
00437dc0: mov      r8, r0
00437dc4: bl       #0x7a86e4
00437dc8: str      r8, [r5, #4]
00437dcc: mov      r2, sl
00437dd0: ldr      r3, [r8]
00437dd4: mov      r0, r8
00437dd8: mov      r1, r7
00437ddc: mov      lr, pc
00437de0: ldr      pc, [r3, #8]
00437de4: ldr      r0, [r5, #4]
00437de8: mov      r1, #1
00437dec: bl       #0x7a7cb4
00437df0: mov      r1, #8
00437df4: mov      r0, #0x34
00437df8: bl       #0x310570
00437dfc: add      r4, r6, r4, lsl #2
00437e00: mov      r7, r0
00437e04: ldr      r1, [r5, #4]
00437e08: bl       #0x42ccd0
00437e0c: str      r7, [r4, #0x144]
00437e10: ldr      sl, [r5, #4]
00437e14: mov      r0, sl
00437e18: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00437e1c: subseq   ip, r5, r4, lsl sp
00437e20: strheq   r0, [r0], -ip

# _ZN8RenderFX10SetContextEPN7gameswf9characterE
007a7ee8: str      r1, [r0, #0x40]
007a7eec: bx       lr

# _ZN11MenuManager11LoadSWFFileEPKci
0042d290: push     {r4, r5, r6, r7, r8, lr}
0042d294: mov      r6, r0
0042d298: mov      r8, r1
0042d29c: mov      r0, #0x124
0042d2a0: mov      r1, #8
0042d2a4: mov      r7, r2
0042d2a8: bl       #0x310570
0042d2ac: add      r4, r7, #0x22
0042d2b0: add      r4, r6, r4, lsl #2
0042d2b4: mov      r5, r0
0042d2b8: bl       #0x7a86e4
0042d2bc: str      r5, [r4, #4]
0042d2c0: ldr      r3, [r5]
0042d2c4: mov      r2, #0
0042d2c8: mov      r0, r5
0042d2cc: mov      r1, r8
0042d2d0: mov      lr, pc
0042d2d4: ldr      pc, [r3, #8]
0042d2d8: ldr      r0, [r4, #4]
0042d2dc: mov      r1, #1
0042d2e0: bl       #0x7a7cb4
0042d2e4: mov      r1, #8
0042d2e8: mov      r0, #0x34
0042d2ec: bl       #0x310570
0042d2f0: add      r6, r6, r7, lsl #2
0042d2f4: mov      r5, r0
0042d2f8: ldr      r1, [r4, #4]
0042d2fc: bl       #0x42ccd0
0042d300: str      r5, [r6, #0x9c]
0042d304: ldr      r0, [r4, #4]
0042d308: pop      {r4, r5, r6, r7, r8, pc}
