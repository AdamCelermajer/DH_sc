_ZN6glitch5scene13CSceneManager22notifyHierarchyChangedEv
005890b4 mov      r3, #1
005890b8 strb     r3, [r0, #0x288]
005890bc b        #0x5890a8
_ZN6glitch5scene10ISceneNode9setParentEPS1_
005971e0 push     {r4, r5, r6, lr}
005971e4 ldr      r3, [r0]
005971e8 mov      r4, r0
005971ec mov      r5, r1
005971f0 ldr      r3, [r3, #-0xc]
005971f4 add      r3, r0, r3
005971f8 ldr      r2, [r3, #4]
005971fc add      r2, r2, #1
00597200 str      r2, [r3, #4]
00597204 ldr      r3, [r0]
00597208 mov      lr, pc
0059720c ldr      pc, [r3, #0x68]
00597210 ldr      r3, [r4, #0x11c]
00597214 cmp      r5, #0
00597218 str      r5, [r4, #0xec]
0059721c orr      r3, r3, #0x40
00597220 str      r3, [r4, #0x11c]
00597224 beq      #0x597240
00597228 ldr      r1, [r5, #0x110]
0059722c ldr      r3, [r4, #0x110]
00597230 cmp      r3, r1
00597234 beq      #0x597240
00597238 mov      r0, r4
0059723c bl       #0x588e30 ; _ZN6glitch5scene10ISceneNode15setSceneManagerEPNS0_13CSceneManagerE
00597240 ldr      r3, [r4]
00597244 ldr      r0, [r3, #-0xc]
00597248 add      r0, r4, r0
0059724c pop      {r4, r5, r6, lr}
00597250 b        #0x31d584
_ZN6glitch5scene10ISceneNode23notifyVisibilityChangedEb
00596e34 push     {r4, r5, r6, lr}
00596e38 ldrb     r2, [r0, #0x120]
00596e3c ldr      r3, [r0, #0x11c]
00596e40 mov      r5, r0
00596e44 cmp      r2, #0
00596e48 strb     r1, [r0, #0x121]
00596e4c and      r2, r3, #1
00596e50 beq      #0x596e5c
00596e54 cmp      r1, #0
00596e58 bne      #0x596eb4
00596e5c bic      r3, r3, #1
00596e60 str      r3, [r5, #0x11c]
00596e64 and      r3, r3, #1
00596e68 cmp      r2, r3
00596e6c beq      #0x596eb0
00596e70 mov      r6, r5
00596e74 ldr      r4, [r6, #0xf4]!
00596e78 b        #0x596ea4
00596e7c ldr      r1, [r5, #0x11c]
00596e80 cmp      r4, #0
00596e84 moveq    r3, r4
00596e88 subne    r3, r4, #4
00596e8c mov      r0, r3
00596e90 and      r1, r1, #1
00596e94 ldr      r3, [r3]
00596e98 mov      lr, pc
00596e9c ldr      pc, [r3, #0xec]
00596ea0 ldr      r4, [r4]
00596ea4 cmp      r6, r4
00596ea8 bne      #0x596e7c
00596eac pop      {r4, r5, r6, pc}
00596eb0 pop      {r4, r5, r6, pc}
00596eb4 orr      r3, r3, #1
00596eb8 str      r3, [r0, #0x11c]
00596ebc b        #0x596e64
