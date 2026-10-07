
# _ZN7gameswf4root18notify_mouse_stateEiii
00774128: str      r3, [r0, #0x44]
0077412c: str      r1, [r0, #0x3c]
00774130: str      r2, [r0, #0x40]
00774134: bx       lr

# _ZN7gameswf4rootC1EPNS_6playerEPNS_14movie_def_implE
00775da0: push     {r4, r5, r6, r7, r8, sl, lr}
00775da4: ldr      r5, [pc, #0x1bc]
00775da8: sub      sp, sp, #0xc
00775dac: mov      r7, r2
00775db0: mov      r4, r0
00775db4: mov      r6, r1
00775db8: bl       #0x759c04
00775dbc: ldr      r3, [pc, #0x1a8]
00775dc0: add      r5, pc, r5
00775dc4: cmp      r7, #0
00775dc8: ldr      r3, [r5, r3]
00775dcc: str      r7, [r4, #0xc]
00775dd0: add      r3, r3, #8
00775dd4: str      r3, [r4]
00775dd8: beq      #0x775de4
00775ddc: mov      r0, r7
00775de0: bl       #0x759c64
00775de4: mov      r7, #0
00775de8: mov      r3, #0
00775dec: mov      r2, #1
00775df0: mov      r5, #0x3f800000
00775df4: mvn      r1, #0
00775df8: str      r2, [r4, #0x20]
00775dfc: str      r2, [r4, #0x1c]
00775e00: str      r3, [r4, #0x48]
00775e04: str      r3, [r4, #0x4c]
00775e08: str      r3, [r4, #0x60]
00775e0c: str      r3, [r4, #0x64]
00775e10: str      r3, [r4, #0x70]
00775e14: str      r3, [r4, #0x74]
00775e18: strb     r1, [r4, #0x3b]
00775e1c: add      r0, r4, #0xc8
00775e20: mov      r1, r6
00775e24: str      r7, [r4, #0x10]
00775e28: str      r7, [r4, #0x14]
00775e2c: str      r7, [r4, #0x18]
00775e30: str      r5, [r4, #0x34]
00775e34: strb     r7, [r4, #0x38]
00775e38: strb     r7, [r4, #0x39]
00775e3c: strb     r7, [r4, #0x3a]
00775e40: str      r7, [r4, #0x3c]
00775e44: str      r7, [r4, #0x40]
00775e48: str      r7, [r4, #0x44]
00775e4c: str      r7, [r4, #0x50]
00775e50: str      r7, [r4, #0x54]
00775e54: str      r7, [r4, #0x58]
00775e58: strb     r7, [r4, #0x5c]
00775e5c: strb     r7, [r4, #0x5d]
00775e60: strb     r7, [r4, #0x5e]
00775e64: str      r5, [r4, #0x68]
00775e68: str      r5, [r4, #0x6c]
00775e6c: str      r7, [r4, #0x78]
00775e70: str      r7, [r4, #0x7c]
00775e74: strb     r7, [r4, #0x80]
00775e78: strb     r7, [r4, #0x81]
00775e7c: strb     r7, [r4, #0x82]
00775e80: strb     r7, [r4, #0x84]
00775e84: strb     r7, [r4, #0x85]
00775e88: str      r3, [r4, #0x94]
00775e8c: strb     r7, [r4, #0x86]
00775e90: strb     r7, [r4, #0x87]
00775e94: str      r7, [r4, #0x88]
00775e98: str      r5, [r4, #0x8c]
00775e9c: str      r5, [r4, #0x90]
00775ea0: str      r7, [r4, #0x98]
00775ea4: str      r7, [r4, #0x9c]
00775ea8: str      r7, [r4, #0xa0]
00775eac: strb     r7, [r4, #0xa4]
00775eb0: str      r7, [r4, #0xa8]
00775eb4: str      r7, [r4, #0xac]
00775eb8: str      r7, [r4, #0xb0]
00775ebc: strb     r7, [r4, #0xb4]
00775ec0: str      r7, [r4, #0xb8]
00775ec4: str      r7, [r4, #0xbc]
00775ec8: str      r7, [r4, #0xc0]
00775ecc: strb     r7, [r4, #0xc4]
00775ed0: str      r7, [r4, #0xc8]
00775ed4: str      r7, [r4, #0xcc]
00775ed8: bl       #0x75e9ac
00775edc: ldr      r3, [r4, #0xc]
00775ee0: mov      r0, r3
00775ee4: ldr      r3, [r3]
00775ee8: mov      lr, pc
00775eec: ldr      pc, [r3, #0x30]
00775ef0: ldr      r3, [r4, #0xc]
00775ef4: mov      r8, r0
00775ef8: mov      r0, r3
00775efc: ldr      r3, [r3]
00775f00: mov      lr, pc
00775f04: ldr      pc, [r3, #0x34]
00775f08: mov      sl, r0
00775f0c: mov      r0, r8
00775f10: bl       #0x30e4cc
00775f14: mov      r8, r0
00775f18: mov      r0, sl
00775f1c: bl       #0x30e4cc
00775f20: mov      r3, r8
00775f24: mov      r2, r7
00775f28: mov      r1, r7
00775f2c: str      r0, [sp]
00775f30: mov      r0, r4
00775f34: bl       #0x775d38
00775f38: mov      r0, r4
00775f3c: bl       #0x7741a0
00775f40: mov      r1, r0
00775f44: mov      r0, r5
00775f48: bl       #0x30ec94
00775f4c: mov      r1, r4
00775f50: str      r0, [r4, #0x90]
00775f54: mov      r0, r6
00775f58: bl       #0x76d71c
00775f5c: mov      r0, r4
00775f60: add      sp, sp, #0xc
00775f64: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZN7gameswf4root15get_mouse_stateEPiS1_S1_
00774138: ldr      ip, [r0, #0x3c]
0077413c: str      ip, [r1]
00774140: ldr      r1, [r0, #0x40]
00774144: str      r1, [r2]
00774148: ldr      r2, [r0, #0x44]
0077414c: str      r2, [r3]
00774150: bx       lr
