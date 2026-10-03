
# _GLOBAL__I_.._.._sources_Game_Objects_Characters_StateMachine_CharStateMachine.cpp
003c1bec: push     {r4, r5, r6, lr}
003c1bf0: ldr      r5, [pc, #0x88c]
003c1bf4: mov      r3, #0x3f000000
003c1bf8: ldr      r4, [pc, #0x888]
003c1bfc: add      r5, pc, r5
003c1c00: str      r3, [r5, #8]
003c1c04: str      r3, [r5]
003c1c08: str      r3, [r5, #4]
003c1c0c: bl       #0x809a38
003c1c10: ldr      r3, [pc, #0x874]
003c1c14: strb     r0, [r5, #0xc]
003c1c18: ldr      r0, [pc, #0x870]
003c1c1c: add      r4, pc, r4
003c1c20: ldr      r1, [r4, r3]
003c1c24: add      r0, pc, r0
003c1c28: bl       #0x80a2e8
003c1c2c: ldr      r3, [pc, #0x860]
003c1c30: strb     r0, [r5, #0xd]
003c1c34: ldr      r0, [pc, #0x85c]
003c1c38: ldr      r1, [r4, r3]
003c1c3c: add      r0, pc, r0
003c1c40: bl       #0x80a2e8
003c1c44: ldr      r3, [pc, #0x850]
003c1c48: strb     r0, [r5, #0xe]
003c1c4c: ldr      r0, [pc, #0x84c]
003c1c50: ldr      r1, [r4, r3]
003c1c54: add      r0, pc, r0
003c1c58: bl       #0x80a2e8
003c1c5c: ldr      r3, [pc, #0x840]
003c1c60: strb     r0, [r5, #0xf]
003c1c64: ldr      r0, [pc, #0x83c]
003c1c68: ldr      r1, [r4, r3]
003c1c6c: add      r0, pc, r0
003c1c70: bl       #0x80a2e8
003c1c74: ldr      r3, [pc, #0x830]
003c1c78: strb     r0, [r5, #0x10]
003c1c7c: ldr      r0, [pc, #0x82c]
003c1c80: ldr      r1, [r4, r3]
003c1c84: add      r0, pc, r0
003c1c88: bl       #0x80a2e8
003c1c8c: ldr      r3, [pc, #0x820]
003c1c90: strb     r0, [r5, #0x11]
003c1c94: ldr      r0, [pc, #0x81c]
003c1c98: ldr      r1, [r4, r3]
003c1c9c: add      r0, pc, r0
003c1ca0: bl       #0x80a2e8
003c1ca4: ldr      r3, [pc, #0x810]
003c1ca8: strb     r0, [r5, #0x12]
003c1cac: ldr      r0, [pc, #0x80c]
003c1cb0: ldr      r1, [r4, r3]
003c1cb4: add      r0, pc, r0
003c1cb8: bl       #0x80a2e8
003c1cbc: ldr      r3, [pc, #0x800]
003c1cc0: strb     r0, [r5, #0x13]
003c1cc4: ldr      r0, [pc, #0x7fc]
003c1cc8: ldr      r1, [r4, r3]
003c1ccc: add      r0, pc, r0
003c1cd0: bl       #0x80a2e8
003c1cd4: ldr      r3, [pc, #0x7f0]
003c1cd8: strb     r0, [r5, #0x14]
003c1cdc: ldr      r0, [pc, #0x7ec]
003c1ce0: ldr      r1, [r4, r3]
003c1ce4: add      r0, pc, r0
003c1ce8: bl       #0x80a2e8
003c1cec: ldr      r3, [pc, #0x7e0]
003c1cf0: strb     r0, [r5, #0x15]
003c1cf4: ldr      r0, [pc, #0x7dc]
003c1cf8: ldr      r1, [r4, r3]
003c1cfc: add      r0, pc, r0
003c1d00: bl       #0x80a2e8
003c1d04: ldr      r3, [pc, #0x7d0]
003c1d08: strb     r0, [r5, #0x16]
003c1d0c: ldr      r0, [pc, #0x7cc]
003c1d10: ldr      r1, [r4, r3]
003c1d14: add      r0, pc, r0
003c1d18: bl       #0x80a2e8
003c1d1c: ldr      r3, [pc, #0x7c0]
003c1d20: strb     r0, [r5, #0x17]
003c1d24: ldr      r0, [pc, #0x7bc]
003c1d28: ldr      r1, [r4, r3]
003c1d2c: add      r0, pc, r0
003c1d30: bl       #0x80a2e8
003c1d34: ldr      r3, [pc, #0x7b0]
003c1d38: strb     r0, [r5, #0x18]
003c1d3c: ldr      r0, [pc, #0x7ac]
003c1d40: ldr      r1, [r4, r3]
003c1d44: add      r0, pc, r0
003c1d48: bl       #0x80a2e8
003c1d4c: strb     r0, [r5, #0x19]
003c1d50: bl       #0x8099a0
003c1d54: ldr      r3, [pc, #0x798]
003c1d58: strb     r0, [r5, #0x1a]
003c1d5c: ldr      r3, [r4, r3]
003c1d60: ldr      r2, [r3]
003c1d64: tst      r2, #1
003c1d68: beq      #0x3c2450
003c1d6c: ldr      r3, [pc, #0x784]
003c1d70: ldr      r3, [r4, r3]
003c1d74: ldr      r2, [r3]
003c1d78: tst      r2, #1
003c1d7c: beq      #0x3c241c
003c1d80: ldr      r3, [pc, #0x774]
003c1d84: ldr      r3, [r4, r3]
003c1d88: ldr      r2, [r3]
003c1d8c: tst      r2, #1
003c1d90: bne      #0x3c1dd4
003c1d94: ldr      r2, [pc, #0x764]
003c1d98: ldr      r5, [pc, #0x764]
003c1d9c: ldr      lr, [pc, #0x764]
003c1da0: ldr      ip, [r4, r2]
003c1da4: ldr      r2, [pc, #0x760]
003c1da8: ldr      r5, [r4, r5]
003c1dac: add      lr, pc, lr
003c1db0: ldr      r1, [r4, r2]
003c1db4: ldr      r2, [pc, #0x754]
003c1db8: add      r5, r5, #8
003c1dbc: mov      r6, #1
003c1dc0: mov      r0, ip
003c1dc4: ldr      r2, [r4, r2]
003c1dc8: str      r6, [r3]
003c1dcc: stm      ip, {r5, lr}
003c1dd0: bl       #0x30e304
003c1dd4: ldr      r3, [pc, #0x738]
003c1dd8: ldr      r3, [r4, r3]
003c1ddc: ldr      r2, [r3]
003c1de0: tst      r2, #1
003c1de4: bne      #0x3c1e28
003c1de8: ldr      r2, [pc, #0x728]
003c1dec: ldr      r5, [pc, #0x728]
003c1df0: ldr      lr, [pc, #0x728]
003c1df4: ldr      ip, [r4, r2]
003c1df8: ldr      r2, [pc, #0x724]
003c1dfc: ldr      r5, [r4, r5]
003c1e00: add      lr, pc, lr
003c1e04: ldr      r1, [r4, r2]
003c1e08: ldr      r2, [pc, #0x700]
003c1e0c: add      r5, r5, #8
003c1e10: mov      r6, #1
003c1e14: mov      r0, ip
003c1e18: ldr      r2, [r4, r2]
003c1e1c: str      r6, [r3]
003c1e20: stm      ip, {r5, lr}
003c1e24: bl       #0x30e304
003c1e28: ldr      r3, [pc, #0x6f8]
003c1e2c: ldr      r3, [r4, r3]
003c1e30: ldr      r2, [r3]
003c1e34: tst      r2, #1
003c1e38: bne      #0x3c1e7c
003c1e3c: ldr      r2, [pc, #0x6e8]
003c1e40: ldr      r5, [pc, #0x6e8]
003c1e44: ldr      lr, [pc, #0x6e8]
003c1e48: ldr      ip, [r4, r2]
003c1e4c: ldr      r2, [pc, #0x6e4]
003c1e50: ldr      r5, [r4, r5]
003c1e54: add      lr, pc, lr
003c1e58: ldr      r1, [r4, r2]
003c1e5c: ldr      r2, [pc, #0x6ac]
003c1e60: add      r5, r5, #8
003c1e64: mov      r6, #1
003c1e68: mov      r0, ip
003c1e6c: ldr      r2, [r4, r2]
003c1e70: str      r6, [r3]
003c1e74: stm      ip, {r5, lr}
003c1e78: bl       #0x30e304
003c1e7c: ldr      r3, [pc, #0x6b8]
003c1e80: ldr      r3, [r4, r3]
003c1e84: ldr      r2, [r3]
003c1e88: tst      r2, #1
003c1e8c: bne      #0x3c1ed0
003c1e90: ldr      r2, [pc, #0x6a8]
003c1e94: ldr      r5, [pc, #0x6a8]
003c1e98: ldr      lr, [pc, #0x6a8]
003c1e9c: ldr      ip, [r4, r2]
003c1ea0: ldr      r2, [pc, #0x6a4]
003c1ea4: ldr      r5, [r4, r5]
003c1ea8: add      lr, pc, lr
003c1eac: ldr      r1, [r4, r2]
003c1eb0: ldr      r2, [pc, #0x658]
003c1eb4: add      r5, r5, #8
003c1eb8: mov      r6, #1
003c1ebc: mov      r0, ip
003c1ec0: ldr      r2, [r4, r2]
003c1ec4: str      r6, [r3]
003c1ec8: stm      ip, {r5, lr}
003c1ecc: bl       #0x30e304
003c1ed0: ldr      r3, [pc, #0x678]
003c1ed4: ldr      r3, [r4, r3]
003c1ed8: ldr      r2, [r3]
003c1edc: tst      r2, #1
003c1ee0: bne      #0x3c1f24
003c1ee4: ldr      r2, [pc, #0x668]
003c1ee8: ldr      r5, [pc, #0x668]
003c1eec: ldr      lr, [pc, #0x668]
003c1ef0: ldr      ip, [r4, r2]
003c1ef4: ldr      r2, [pc, #0x664]
003c1ef8: ldr      r5, [r4, r5]
003c1efc: add      lr, pc, lr
003c1f00: ldr      r1, [r4, r2]
003c1f04: ldr      r2, [pc, #0x604]
003c1f08: add      r5, r5, #8
003c1f0c: mov      r6, #1
003c1f10: mov      r0, ip
003c1f14: ldr      r2, [r4, r2]
003c1f18: str      r6, [r3]
003c1f1c: stm      ip, {r5, lr}
003c1f20: bl       #0x30e304
003c1f24: ldr      r3, [pc, #0x638]
003c1f28: ldr      r3, [r4, r3]
003c1f2c: ldr      r2, [r3]
003c1f30: tst      r2, #1
003c1f34: bne      #0x3c1f78
003c1f38: ldr      r2, [pc, #0x628]
003c1f3c: ldr      r5, [pc, #0x628]
003c1f40: ldr      lr, [pc, #0x628]
003c1f44: ldr      ip, [r4, r2]
003c1f48: ldr      r2, [pc, #0x624]
003c1f4c: ldr      r5, [r4, r5]
003c1f50: add      lr, pc, lr
003c1f54: ldr      r1, [r4, r2]
003c1f58: ldr      r2, [pc, #0x5b0]
003c1f5c: add      r5, r5, #8
003c1f60: mov      r6, #1
003c1f64: mov      r0, ip
003c1f68: ldr      r2, [r4, r2]
003c1f6c: str      r6, [r3]
003c1f70: stm      ip, {r5, lr}
003c1f74: bl       #0x30e304
003c1f78: ldr      r3, [pc, #0x5f8]
003c1f7c: ldr      r3, [r4, r3]
003c1f80: ldr      r2, [r3]
003c1f84: tst      r2, #1
003c1f88: bne      #0x3c1fcc
003c1f8c: ldr      r2, [pc, #0x5e8]
003c1f90: ldr      r5, [pc, #0x5e8]
003c1f94: ldr      lr, [pc, #0x5e8]
003c1f98: ldr      ip, [r4, r2]
003c1f9c: ldr      r2, [pc, #0x5e4]
003c1fa0: ldr      r5, [r4, r5]
003c1fa4: add      lr, pc, lr
003c1fa8: ldr      r1, [r4, r2]
003c1fac: ldr      r2, [pc, #0x55c]
003c1fb0: add      r5, r5, #8
003c1fb4: mov      r6, #1
003c1fb8: mov      r0, ip
003c1fbc: ldr      r2, [r4, r2]
003c1fc0: str      r6, [r3]
003c1fc4: stm      ip, {r5, lr}
003c1fc8: bl       #0x30e304
003c1fcc: ldr      r3, [pc, #0x5b8]
003c1fd0: ldr      r3, [r4, r3]
003c1fd4: ldr      r2, [r3]
003c1fd8: tst      r2, #1
003c1fdc: bne      #0x3c2020
003c1fe0: ldr      r2, [pc, #0x5a8]
003c1fe4: ldr      r5, [pc, #0x5a8]
003c1fe8: ldr      lr, [pc, #0x5a8]
003c1fec: ldr      ip, [r4, r2]
003c1ff0: ldr      r2, [pc, #0x5a4]
003c1ff4: ldr      r5, [r4, r5]
003c1ff8: add      lr, pc, lr
003c1ffc: ldr      r1, [r4, r2]
003c2000: ldr      r2, [pc, #0x508]
003c2004: add      r5, r5, #8
003c2008: mov      r6, #1
003c200c: mov      r0, ip
003c2010: ldr      r2, [r4, r2]
003c2014: str      r6, [r3]
003c2018: stm      ip, {r5, lr}
003c201c: bl       #0x30e304
003c2020: ldr      r3, [pc, #0x578]
003c2024: ldr      r3, [r4, r3]
003c2028: ldr      r2, [r3]
003c202c: tst      r2, #1
003c2030: bne      #0x3c2074
003c2034: ldr      r2, [pc, #0x568]
003c2038: ldr      r5, [pc, #0x568]
003c203c: ldr      lr, [pc, #0x568]
003c2040: ldr      ip, [r4, r2]
003c2044: ldr      r2, [pc, #0x564]
003c2048: ldr      r5, [r4, r5]
003c204c: add      lr, pc, lr
003c2050: ldr      r1, [r4, r2]
003c2054: ldr      r2, [pc, #0x4b4]
003c2058: add      r5, r5, #8
003c205c: mov      r6, #1
003c2060: mov      r0, ip
003c2064: ldr      r2, [r4, r2]
003c2068: str      r6, [r3]
003c206c: stm      ip, {r5, lr}
003c2070: bl       #0x30e304
003c2074: ldr      r3, [pc, #0x538]
003c2078: ldr      r3, [r4, r3]
003c207c: ldr      r2, [r3]
003c2080: tst      r2, #1
003c2084: bne      #0x3c20c8
003c2088: ldr      r2, [pc, #0x528]
003c208c: ldr      r5, [pc, #0x528]
003c2090: ldr      lr, [pc, #0x528]
003c2094: ldr      ip, [r4, r2]
003c2098: ldr      r2, [pc, #0x524]
003c209c: ldr      r5, [r4, r5]
003c20a0: add      lr, pc, lr
003c20a4: ldr      r1, [r4, r2]
003c20a8: ldr      r2, [pc, #0x460]
003c20ac: add      r5, r5, #8
003c20b0: mov      r6, #1
003c20b4: mov      r0, ip
003c20b8: ldr      r2, [r4, r2]
003c20bc: str      r6, [r3]
003c20c0: stm      ip, {r5, lr}
003c20c4: bl       #0x30e304
003c20c8: ldr      r3, [pc, #0x4f8]
003c20cc: ldr      r3, [r4, r3]
003c20d0: ldr      r2, [r3]
003c20d4: tst      r2, #1
003c20d8: bne      #0x3c211c
003c20dc: ldr      r2, [pc, #0x4e8]
003c20e0: ldr      r5, [pc, #0x4e8]
003c20e4: ldr      lr, [pc, #0x4e8]
003c20e8: ldr      ip, [r4, r2]
003c20ec: ldr      r2, [pc, #0x4e4]
003c20f0: ldr      r5, [r4, r5]
003c20f4: add      lr, pc, lr
003c20f8: ldr      r1, [r4, r2]
003c20fc: ldr      r2, [pc, #0x40c]
003c2100: add      r5, r5, #8
003c2104: mov      r6, #1
003c2108: mov      r0, ip
003c210c: ldr      r2, [r4, r2]
003c2110: str      r6, [r3]
003c2114: stm      ip, {r5, lr}
003c2118: bl       #0x30e304
003c211c: ldr      r3, [pc, #0x4b8]
003c2120: ldr      r3, [r4, r3]
003c2124: ldr      r2, [r3]
003c2128: tst      r2, #1
003c212c: bne      #0x3c2170
003c2130: ldr      r2, [pc, #0x4a8]
003c2134: ldr      r5, [pc, #0x4a8]
003c2138: ldr      lr, [pc, #0x4a8]
003c213c: ldr      ip, [r4, r2]
003c2140: ldr      r2, [pc, #0x4a4]
003c2144: ldr      r5, [r4, r5]
003c2148: add      lr, pc, lr
003c214c: ldr      r1, [r4, r2]
003c2150: ldr      r2, [pc, #0x3b8]
003c2154: add      r5, r5, #8
003c2158: mov      r6, #1
003c215c: mov      r0, ip
003c2160: ldr      r2, [r4, r2]
003c2164: str      r6, [r3]
003c2168: stm      ip, {r5, lr}
003c216c: bl       #0x30e304
003c2170: ldr      r3, [pc, #0x478]
003c2174: ldr      r3, [r4, r3]
003c2178: ldr      r2, [r3]
003c217c: tst      r2, #1
003c2180: bne      #0x3c21c4
003c2184: ldr      r2, [pc, #0x468]
003c2188: ldr      r5, [pc, #0x468]
003c218c: ldr      lr, [pc, #0x468]
003c2190: ldr      ip, [r4, r2]
003c2194: ldr      r2, [pc, #0x464]
003c2198: ldr      r5, [r4, r5]
003c219c: add      lr, pc, lr
003c21a0: ldr      r1, [r4, r2]
003c21a4: ldr      r2, [pc, #0x364]
003c21a8: add      r5, r5, #8
003c21ac: mov      r6, #1
003c21b0: mov      r0, ip
003c21b4: ldr      r2, [r4, r2]
003c21b8: str      r6, [r3]
003c21bc: stm      ip, {r5, lr}
003c21c0: bl       #0x30e304
003c21c4: ldr      r3, [pc, #0x438]
003c21c8: ldr      r3, [r4, r3]
003c21cc: ldr      r2, [r3]
003c21d0: tst      r2, #1
003c21d4: bne      #0x3c2218
003c21d8: ldr      r2, [pc, #0x428]
003c21dc: ldr      r5, [pc, #0x428]
003c21e0: ldr      lr, [pc, #0x428]
003c21e4: ldr      ip, [r4, r2]
003c21e8: ldr      r2, [pc, #0x424]
003c21ec: ldr      r5, [r4, r5]
003c21f0: add      lr, pc, lr
003c21f4: ldr      r1, [r4, r2]
003c21f8: ldr      r2, [pc, #0x310]
003c21fc: add      r5, r5, #8
003c2200: mov      r6, #1
003c2204: mov      r0, ip
003c2208: ldr      r2, [r4, r2]
003c220c: str      r6, [r3]
003c2210: stm      ip, {r5, lr}
003c2214: bl       #0x30e304
003c2218: ldr      r3, [pc, #0x3f8]
003c221c: ldr      r3, [r4, r3]
003c2220: ldr      r2, [r3]
003c2224: tst      r2, #1
003c2228: bne      #0x3c226c
003c222c: ldr      r2, [pc, #0x3e8]
003c2230: ldr      r5, [pc, #0x3e8]
003c2234: ldr      lr, [pc, #0x3e8]
003c2238: ldr      ip, [r4, r2]
003c223c: ldr      r2, [pc, #0x3e4]
003c2240: ldr      r5, [r4, r5]
003c2244: add      lr, pc, lr
003c2248: ldr      r1, [r4, r2]
003c224c: ldr      r2, [pc, #0x2bc]
003c2250: add      r5, r5, #8
003c2254: mov      r6, #1
003c2258: mov      r0, ip
003c225c: ldr      r2, [r4, r2]
003c2260: str      r6, [r3]
003c2264: stm      ip, {r5, lr}
003c2268: bl       #0x30e304
003c226c: ldr      r3, [pc, #0x3b8]
003c2270: ldr      r3, [r4, r3]
003c2274: ldr      r2, [r3]
003c2278: tst      r2, #1
003c227c: bne      #0x3c22c0
003c2280: ldr      r2, [pc, #0x3a8]
003c2284: ldr      r5, [pc, #0x3a8]
003c2288: ldr      lr, [pc, #0x3a8]
003c228c: ldr      ip, [r4, r2]
003c2290: ldr      r2, [pc, #0x3a4]
003c2294: ldr      r5, [r4, r5]
003c2298: add      lr, pc, lr
003c229c: ldr      r1, [r4, r2]
003c22a0: ldr      r2, [pc, #0x268]
003c22a4: add      r5, r5, #8
003c22a8: mov      r6, #1
003c22ac: mov      r0, ip
003c22b0: ldr      r2, [r4, r2]
003c22b4: str      r6, [r3]
003c22b8: stm      ip, {r5, lr}
003c22bc: bl       #0x30e304
003c22c0: ldr      r3, [pc, #0x378]
003c22c4: ldr      r3, [r4, r3]
003c22c8: ldr      r2, [r3]
003c22cc: tst      r2, #1
003c22d0: bne      #0x3c2314
003c22d4: ldr      r2, [pc, #0x368]
003c22d8: ldr      r5, [pc, #0x368]
003c22dc: ldr      lr, [pc, #0x368]
003c22e0: ldr      ip, [r4, r2]
003c22e4: ldr      r2, [pc, #0x364]
003c22e8: ldr      r5, [r4, r5]
003c22ec: add      lr, pc, lr
003c22f0: ldr      r1, [r4, r2]
003c22f4: ldr      r2, [pc, #0x214]
003c22f8: add      r5, r5, #8
003c22fc: mov      r6, #1
003c2300: mov      r0, ip
003c2304: ldr      r2, [r4, r2]
003c2308: str      r6, [r3]
003c230c: stm      ip, {r5, lr}
003c2310: bl       #0x30e304
003c2314: ldr      r3, [pc, #0x338]
003c2318: ldr      r3, [r4, r3]
003c231c: ldr      r2, [r3]
003c2320: tst      r2, #1
003c2324: bne      #0x3c2368
003c2328: ldr      r2, [pc, #0x328]
003c232c: ldr      r5, [pc, #0x328]
003c2330: ldr      lr, [pc, #0x328]
003c2334: ldr      ip, [r4, r2]
003c2338: ldr      r2, [pc, #0x324]
003c233c: ldr      r5, [r4, r5]
003c2340: add      lr, pc, lr
003c2344: ldr      r1, [r4, r2]
003c2348: ldr      r2, [pc, #0x1c0]
003c234c: add      r5, r5, #8
003c2350: mov      r6, #1
003c2354: mov      r0, ip
003c2358: ldr      r2, [r4, r2]
003c235c: str      r6, [r3]
003c2360: stm      ip, {r5, lr}
003c2364: bl       #0x30e304
003c2368: ldr      r3, [pc, #0x2f8]
003c236c: ldr      r3, [r4, r3]
003c2370: ldr      r2, [r3]
003c2374: tst      r2, #1
003c2378: bne      #0x3c23bc
003c237c: ldr      r2, [pc, #0x2e8]
003c2380: ldr      r5, [pc, #0x2e8]
003c2384: ldr      lr, [pc, #0x2e8]
003c2388: ldr      ip, [r4, r2]
003c238c: ldr      r2, [pc, #0x2e4]
003c2390: ldr      r5, [r4, r5]
003c2394: add      lr, pc, lr
003c2398: ldr      r1, [r4, r2]
003c239c: ldr      r2, [pc, #0x16c]
003c23a0: add      r5, r5, #8
003c23a4: mov      r6, #1
003c23a8: mov      r0, ip
003c23ac: ldr      r2, [r4, r2]
003c23b0: str      r6, [r3]
003c23b4: stm      ip, {r5, lr}
003c23b8: bl       #0x30e304
003c23bc: ldr      r3, [pc, #0x2b8]
003c23c0: ldr      r3, [r4, r3]
003c23c4: ldr      r2, [r3]
003c23c8: tst      r2, #1
003c23cc: bne      #0x3c2418
003c23d0: ldr      r2, [pc, #0x2a8]
003c23d4: ldr      r6, [pc, #0x2a8]
003c23d8: ldr      r1, [pc, #0x2a8]
003c23dc: ldr      ip, [r4, r2]
003c23e0: ldr      r2, [pc, #0x128]
003c23e4: ldr      r6, [r4, r6]
003c23e8: ldr      r5, [pc, #0x29c]
003c23ec: ldr      r2, [r4, r2]
003c23f0: ldr      r1, [r4, r1]
003c23f4: add      r5, pc, r5
003c23f8: add      r6, r6, #8
003c23fc: mov      r4, #1
003c2400: mov      r0, ip
003c2404: str      r4, [r3]
003c2408: str      r5, [ip, #4]
003c240c: str      r6, [ip]
003c2410: pop      {r4, r5, r6, lr}
003c2414: b        #0x30e304
003c2418: pop      {r4, r5, r6, pc}
003c241c: mov      r2, #1
003c2420: str      r2, [r3]
003c2424: ldr      r3, [pc, #0x264]
003c2428: ldr      r5, [r4, r3]
003c242c: mov      r0, r5
003c2430: bl       #0x32d79c
003c2434: ldr      r3, [pc, #0x258]
003c2438: mov      r0, r5
003c243c: ldr      r1, [r4, r3]
003c2440: ldr      r3, [pc, #0xc8]
003c2444: ldr      r2, [r4, r3]
003c2448: bl       #0x30e304
003c244c: b        #0x3c1d80
003c2450: mov      r2, #1
003c2454: str      r2, [r3]
003c2458: ldr      r3, [pc, #0x238]
003c245c: ldr      r5, [r4, r3]
003c2460: mov      r0, r5
003c2464: bl       #0x3790a8
003c2468: ldr      r3, [pc, #0x22c]
003c246c: mov      r0, r5
003c2470: ldr      r1, [r4, r3]
003c2474: ldr      r3, [pc, #0x94]
003c2478: ldr      r2, [r4, r3]
003c247c: bl       #0x30e304
003c2480: b        #0x3c1d6c
003c2484: subseq   r0, lr, r0, lsr #28
003c2488: subseq   r2, sp, r4, ror lr
003c248c: andeq    r2, r0, r4, lsr #30
003c2490: strdeq   sp, lr, [pc], #-0x2c
003c2494: andeq    r1, r0, r8, asr #16
003c2498: subeq    sp, pc, ip, asr #5
003c249c: strheq   r3, [r0], -r0
003c24a0: umaaleq  sp, pc, ip, r2
003c24a4: andeq    r0, r0, r4, lsl #13
003c24a8: subeq    sp, pc, r4, ror r2
003c24ac: andeq    r0, r0, ip, asr lr
003c24b0: subeq    sp, pc, ip, asr #4
003c24b4: andeq    r2, r0, r8, ror #12
003c24b8: subeq    sp, pc, r4, lsr #4
003c24bc: andeq    r0, r0, r0, lsr #13
003c24c0: strdeq   sp, lr, [pc], #-0x14
003c24c4: andeq    r0, r0, r4, asr sl
003c24c8: subeq    sp, pc, ip, asr #3
003c24cc: andeq    r3, r0, r4, asr #22
003c24d0: subeq    sp, pc, r4, lsr #3
003c24d4: andeq    r1, r0, r0, lsr #29
003c24d8: subeq    sp, pc, ip, ror r1
003c24dc: andeq    r1, r0, r8, lsl r6
003c24e0: subeq    sp, pc, r4, asr r1
003c24e4: andeq    r2, r0, r4, lsr r8
003c24e8: subeq    sp, pc, ip, lsr #2
003c24ec: strdeq   r3, r4, [r0], -r0
003c24f0: subeq    sp, pc, ip, ror #3
003c24f4: strdeq   r0, r1, [r0], -r4
003c24f8: andeq    r0, r0, ip, lsr #31
003c24fc: andeq    r3, r0, r8, lsr #30
003c2500: andeq    r0, r0, r0, lsl #15
003c2504: andeq    r0, r0, r8, asr #14
003c2508: ldrsbeq  r2, [r0], #-0xec
003c250c: andeq    r0, r0, ip, asr r6
003c2510: muleq    r0, r0, r8
003c2514: muleq    r0, r8, sl
003c2518: ldrdeq   r2, r3, [r0], -r8
003c251c: strdeq   r3, r4, [r0], -ip

# _ZN9Character10CSM_AttackEiPviRi
003ad22c: ldr      r0, [r0, #0x528]
003ad230: and      r0, r0, #1
003ad234: eor      r0, r0, #1
003ad238: bx       lr

# _ZN16CharStateMachine17SM_SetAttackStateEPvb
003c6488: cmp      r2, #0
003c648c: bne      #0x3c649c
003c6490: mov      r2, r1
003c6494: movw     r1, #0xc354
003c6498: b        #0x3c5684
003c649c: mov      r3, r1
003c64a0: movw     r2, #0xc354
003c64a4: mov      r1, #5
003c64a8: b        #0x3c1938

# _ZN16CharStateMachine15RaiseStateEventEiPv
003c5684: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c5688: ldr      r5, [pc, #0x1e0]
003c568c: ldr      r7, [pc, #0x1e0]
003c5690: mov      r6, r1
003c5694: add      r5, pc, r5
003c5698: ldr      r1, [r5, r7]
003c569c: sub      sp, sp, #0x30
003c56a0: sub      r3, r6, #0x2a
003c56a4: ldr      r1, [r1]
003c56a8: mov      r4, r0
003c56ac: mov      r8, r2
003c56b0: str      r1, [sp, #0x2c]
003c56b4: cmp      r3, #6
003c56b8: addls    pc, pc, r3, lsl #2
003c56bc: b        #0x3c5700
003c56c0: b        #0x3c584c
003c56c4: b        #0x3c583c
003c56c8: b        #0x3c582c
003c56cc: b        #0x3c5700
003c56d0: b        #0x3c5700
003c56d4: b        #0x3c5700
003c56d8: b        #0x3c56dc
003c56dc: mov      r1, #0
003c56e0: bl       #0x3c0260
003c56e4: cmp      r0, #0
003c56e8: beq      #0x3c5700
003c56ec: ldr      r3, [r4, #4]
003c56f0: ldr      r0, [r3, #0x2dc]
003c56f4: cmp      r0, #0
003c56f8: beq      #0x3c5700
003c56fc: bl       #0x46eb20
003c5700: ldr      r3, [r4, #0x20]
003c5704: cmp      r3, #0
003c5708: beq      #0x3c574c
003c570c: ldm      r3, {r1, r3}
003c5710: ldr      r2, [r4, #4]
003c5714: ldr      ip, [r3]
003c5718: mov      r0, r3
003c571c: str      r6, [sp]
003c5720: mov      r3, r4
003c5724: str      r8, [sp, #4]
003c5728: mov      lr, pc
003c572c: ldr      pc, [ip, #0x18]
003c5730: ldr      r3, [r4, #0x20]
003c5734: mov      r0, r4
003c5738: mov      r2, r6
003c573c: ldr      r1, [r3]
003c5740: bl       #0x3c00e0
003c5744: cmp      r0, #0
003c5748: bne      #0x3c5768
003c574c: ldr      r3, [r5, r7]
003c5750: ldr      r2, [sp, #0x2c]
003c5754: ldr      r3, [r3]
003c5758: cmp      r2, r3
003c575c: bne      #0x3c586c
003c5760: add      sp, sp, #0x30
003c5764: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c5768: ldr      r3, [pc, #0x108]
003c576c: add      sl, sp, #0x14
003c5770: ldr      sb, [r5, r3]
003c5774: mov      r0, sb
003c5778: bl       #0x337888
003c577c: ldr      r1, [pc, #0xf8]
003c5780: add      r2, sp, #0x10
003c5784: mov      r0, sl
003c5788: add      r1, pc, r1
003c578c: bl       #0x3140ec
003c5790: mov      r1, sl
003c5794: mov      r0, sb
003c5798: bl       #0x337a88
003c579c: mov      r0, sl
003c57a0: bl       #0x318254
003c57a4: ldr      r3, [r4, #0x20]
003c57a8: mov      r0, r4
003c57ac: mov      r2, r6
003c57b0: ldr      r1, [r3]
003c57b4: bl       #0x3c1694
003c57b8: ldr      r1, [r0, #8]
003c57bc: str      r1, [sp, #0xc]
003c57c0: ldr      r3, [r0]
003c57c4: cmp      r3, #0
003c57c8: beq      #0x3c585c
003c57cc: ldr      r3, [r0, #4]
003c57d0: ldr      r2, [r4, #4]
003c57d4: tst      r3, #1
003c57d8: ldrne    r1, [r0]
003c57dc: ldrne    ip, [r2, r3, asr #1]
003c57e0: addne    r0, r2, r3, asr #1
003c57e4: ldreq    ip, [r0]
003c57e8: addeq    r0, r2, r3, asr #1
003c57ec: ldr      r3, [r4, #0x20]
003c57f0: add      r2, sp, #0xc
003c57f4: ldrne    ip, [ip, r1]
003c57f8: ldr      r3, [r3]
003c57fc: mov      r1, r6
003c5800: str      r2, [sp]
003c5804: mov      r2, r8
003c5808: blx      ip
003c580c: cmp      r0, #0
003c5810: beq      #0x3c574c
003c5814: ldr      r1, [sp, #0xc]
003c5818: mov      r0, r4
003c581c: mov      r2, r6
003c5820: mov      r3, r8
003c5824: bl       #0x3c1938
003c5828: b        #0x3c574c
003c582c: ldr      r3, [r0, #0x2c]
003c5830: bic      r3, r3, #4
003c5834: str      r3, [r0, #0x2c]
003c5838: b        #0x3c5700
003c583c: ldr      r3, [r0, #0x2c]
003c5840: bic      r3, r3, #2
003c5844: str      r3, [r0, #0x2c]
003c5848: b        #0x3c5700
003c584c: ldr      r3, [r0, #0x2c]
003c5850: bic      r3, r3, #1
003c5854: str      r3, [r0, #0x2c]
003c5858: b        #0x3c5700
003c585c: ldr      r3, [r0, #4]
003c5860: tst      r3, #1
003c5864: beq      #0x3c5818
003c5868: b        #0x3c57cc
003c586c: bl       #0x30e310
003c5870: ldrsheq  pc, [ip], #-0x3c
003c5874: andeq    r4, r0, ip, lsr #1
003c5878: andeq    r0, r0, r4, lsl #17
003c587c: subeq    pc, pc, r8, lsr #15

# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6
