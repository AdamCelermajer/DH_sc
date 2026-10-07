_ZN13RootSceneNode10setVisibleEb
0035c26c cmp      r1, #0
0035c270 strb     r1, [r0, #0x209]
0035c274 bxne     lr
0035c278 b        #0x596ec4
_ZN6glitch5scene10ISceneNode10setVisibleEb
00596ec4 push     {r4, r5, r6, lr}
00596ec8 ldrb     r3, [r0, #0x120]
00596ecc mov      r5, r0
00596ed0 cmp      r3, r1
00596ed4 beq      #0x596f40
00596ed8 ldr      r3, [r0, #0x11c]
00596edc cmp      r1, #0
00596ee0 strb     r1, [r0, #0x120]
00596ee4 and      r2, r3, #1
00596ee8 bne      #0x596f44
00596eec bic      r3, r3, #1
00596ef0 str      r3, [r5, #0x11c]
00596ef4 and      r3, r3, #1
00596ef8 cmp      r2, r3
00596efc beq      #0x596f40
00596f00 mov      r6, r5
00596f04 ldr      r4, [r6, #0xf4]!
00596f08 b        #0x596f34
00596f0c ldr      r1, [r5, #0x11c]
00596f10 cmp      r4, #0
00596f14 moveq    r3, r4
00596f18 subne    r3, r4, #4
00596f1c mov      r0, r3
00596f20 and      r1, r1, #1
00596f24 ldr      r3, [r3]
00596f28 mov      lr, pc
00596f2c ldr      pc, [r3, #0xec]
00596f30 ldr      r4, [r4]
00596f34 cmp      r6, r4
00596f38 bne      #0x596f0c
00596f3c pop      {r4, r5, r6, pc}
00596f40 pop      {r4, r5, r6, pc}
00596f44 ldrb     r1, [r0, #0x121]
00596f48 cmp      r1, #0
00596f4c beq      #0x596eec
00596f50 orr      r3, r3, #1
00596f54 str      r3, [r0, #0x11c]
00596f58 b        #0x596ef4
