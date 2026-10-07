
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
