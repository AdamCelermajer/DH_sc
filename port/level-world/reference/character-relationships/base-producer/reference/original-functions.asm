
# _ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310: push     {r4, r5, r6, r7, r8, lr}
0033f314: ldr      r6, [pc, #0x198]
0033f318: ldr      r2, [pc, #0x198]
0033f31c: ldr      r3, [pc, #0x198]
0033f320: add      r6, pc, r6
0033f324: ldr      r2, [r6, r2]
0033f328: ldr      r3, [r6, r3]
0033f32c: mov      r4, r0
0033f330: add      r2, r2, #8
0033f334: add      r0, r3, #8
0033f338: add      r3, r4, #8
0033f33c: str      r2, [r4]
0033f340: str      r0, [r4, #4]
0033f344: mov      r7, r1
0033f348: mov      r0, r3
0033f34c: str      r3, [r4, #0x18]
0033f350: str      r3, [r4, #0x1c]
0033f354: mov      r1, #0x10
0033f358: bl       #0x31167c
0033f35c: ldr      r2, [pc, #0x15c]
0033f360: ldr      r1, [r4, #0x18]
0033f364: mov      r5, #0
0033f368: ldr      r2, [r6, r2]
0033f36c: strb     r5, [r1]
0033f370: add      r3, r4, #0x30
0033f374: add      r1, r2, #0x74
0033f378: add      r0, r2, #8
0033f37c: add      r2, r2, #0x68
0033f380: stm      r4, {r0, r2}
0033f384: str      r1, [r4, #0x24]
0033f388: mov      r0, r3
0033f38c: str      r5, [r4, #0x20]
0033f390: strb     r5, [r4, #0x28]
0033f394: strb     r5, [r4, #0x29]
0033f398: str      r5, [r4, #0x2c]
0033f39c: str      r3, [r4, #0x40]
0033f3a0: str      r3, [r4, #0x44]
0033f3a4: mov      r1, #0x10
0033f3a8: bl       #0x31167c
0033f3ac: ldr      r2, [r4, #0x40]
0033f3b0: add      r3, r4, #0x48
0033f3b4: mov      r0, r3
0033f3b8: strb     r5, [r2]
0033f3bc: mov      r1, #0x10
0033f3c0: str      r3, [r4, #0x58]
0033f3c4: str      r3, [r4, #0x5c]
0033f3c8: bl       #0x31167c
0033f3cc: ldr      r2, [r4, #0x58]
0033f3d0: add      r3, r4, #0x68
0033f3d4: mvn      r6, #0
0033f3d8: strb     r5, [r2]
0033f3dc: mov      r1, #0x10
0033f3e0: mov      r0, r3
0033f3e4: strb     r5, [r4, #0x60]
0033f3e8: str      r3, [r4, #0x78]
0033f3ec: str      r3, [r4, #0x7c]
0033f3f0: str      r6, [r4, #0x64]
0033f3f4: bl       #0x31167c
0033f3f8: ldr      r3, [r4, #0x78]
0033f3fc: add      r0, r4, #0x8c
0033f400: strb     r5, [r3]
0033f404: mov      r3, #1
0033f408: strb     r3, [r4, #0x8a]
0033f40c: strb     r5, [r4, #0x81]
0033f410: strb     r5, [r4, #0x84]
0033f414: strb     r5, [r4, #0x85]
0033f418: strb     r5, [r4, #0x86]
0033f41c: strb     r5, [r4, #0x88]
0033f420: strb     r5, [r4, #0x89]
0033f424: bl       #0x33ed7c
0033f428: add      r0, r4, #0xb0
0033f42c: bl       #0x33ed7c
0033f430: add      r3, r4, #0xd4
0033f434: mov      r0, r3
0033f438: str      r3, [r4, #0xe4]
0033f43c: str      r3, [r4, #0xe8]
0033f440: mov      r1, #0x10
0033f444: bl       #0x31167c
0033f448: ldr      r3, [r4, #0xe4]
0033f44c: mov      r1, r5
0033f450: mov      r0, #0xc
0033f454: strb     r5, [r3]
0033f458: mov      r3, #0
0033f45c: str      r3, [r4, #0x114]
0033f460: strb     r5, [r4, #0xf0]
0033f464: strb     r5, [r4, #0xf1]
0033f468: strb     r5, [r4, #0xf8]
0033f46c: str      r5, [r4, #0xfc]
0033f470: str      r5, [r4, #0x100]
0033f474: str      r5, [r4, #0x104]
0033f478: strb     r5, [r4, #0x10c]
0033f47c: strb     r5, [r4, #0x118]
0033f480: strb     r5, [r4, #0x119]
0033f484: str      r5, [r4, #0x11c]
0033f488: str      r7, [r4, #0xf4]
0033f48c: str      r6, [r4, #0x110]
0033f490: str      r6, [r4, #0xec]
0033f494: str      r6, [r4, #0x108]
0033f498: bl       #0x310570
0033f49c: mov      r5, r0
0033f4a0: bl       #0x33f50c
0033f4a4: str      r5, [r4, #0x2c]
0033f4a8: mov      r0, r4
0033f4ac: str      r4, [r5, #4]
0033f4b0: pop      {r4, r5, r6, r7, r8, pc}
0033f4b4: rsbeq    r5, r5, r0, ror r7
0033f4b8: andeq    r1, r0, ip, lsl #1
0033f4bc: ldrdeq   r3, r4, [r0], -ip
0033f4c0: andeq    r3, r0, r4, lsl #23
