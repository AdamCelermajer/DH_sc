_Z22RecursiveSetBoolOnNodePN6glitch5scene10ISceneNodeEbPFvS2_bE
0050e484 push     {r4, r5, r6, r7, r8, lr}
0050e488 subs     r5, r0, #0
0050e48c mov      r6, r2
0050e490 mov      r7, r1
0050e494 beq      #0x50e4cc
0050e498 blx      r2
0050e49c ldr      r4, [r5, #0xf4]!
0050e4a0 cmp      r4, r5
0050e4a4 beq      #0x50e4cc
0050e4a8 cmp      r4, #0
0050e4ac moveq    r0, r4
0050e4b0 subne    r0, r4, #4
0050e4b4 ldr      r4, [r4]
0050e4b8 mov      r1, r7
0050e4bc mov      r2, r6
0050e4c0 bl       #0x50e484 ; _Z22RecursiveSetBoolOnNodePN6glitch5scene10ISceneNodeEbPFvS2_bE
0050e4c4 cmp      r5, r4
0050e4c8 bne      #0x50e4a8
0050e4cc pop      {r4, r5, r6, r7, r8, pc}
_Z14OptimizeStaticPN6glitch5scene10ISceneNodeE
0050f220 push     {r4, r5, r6, lr}
0050f224 mov      r1, #0
0050f228 mov      r4, r0
0050f22c sub      sp, sp, #0x20
0050f230 ldr      r3, [r0]
0050f234 mov      lr, pc
0050f238 ldr      pc, [r3, #0xb8]
0050f23c ldr      r3, [r4]
0050f240 add      r5, sp, #0x14
0050f244 mov      r0, r5
0050f248 mov      r1, r4
0050f24c ldr      r6, [r3, #0xa4]
0050f250 bl       #0x597180 ; _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
0050f254 mov      r1, r5
0050f258 mov      r0, r4
0050f25c blx      r6
0050f260 ldr      r3, [r4]
0050f264 mov      r0, r4
0050f268 add      r5, sp, #4
0050f26c ldr      r6, [r3, #0x9c]
0050f270 mov      lr, pc
0050f274 ldr      pc, [r3, #0x38]
0050f278 mov      r1, r0
0050f27c mov      r0, r5
0050f280 bl       #0x50eeb4 ; _ZN6glitch4core10quaternionaSERKNS0_8CMatrix4IfEE
0050f284 mov      r0, r4
0050f288 mov      r1, r5
0050f28c blx      r6
0050f290 ldr      r3, [r4, #0x11c]
0050f294 mov      r5, r4
0050f298 bic      r3, r3, #0x200
0050f29c str      r3, [r4, #0x11c]
0050f2a0 ldr      r4, [r5, #0xf4]!
0050f2a4 cmp      r4, r5
0050f2a8 beq      #0x50f2c8
0050f2ac cmp      r4, #0
0050f2b0 moveq    r0, r4
0050f2b4 subne    r0, r4, #4
0050f2b8 bl       #0x50f220 ; _Z14OptimizeStaticPN6glitch5scene10ISceneNodeE
0050f2bc ldr      r4, [r4]
0050f2c0 cmp      r5, r4
0050f2c4 bne      #0x50f2ac
0050f2c8 add      sp, sp, #0x20
0050f2cc pop      {r4, r5, r6, pc}
_ZN12VisualObject10SetScalingERK7Point3DIfE
004727ac push     {r4, r5, r6, r7, r8, lr}
004727b0 ldr      r3, [r0, #8]
004727b4 sub      sp, sp, #0x10
004727b8 mov      r5, r0
004727bc cmp      r3, #0
004727c0 mov      r4, r1
004727c4 beq      #0x47282c
004727c8 mov      r0, r3
004727cc ldr      r3, [r3]
004727d0 mov      lr, pc
004727d4 ldr      pc, [r3, #0x90]
004727d8 ldr      r7, [r4]
004727dc ldr      r1, [r0]
004727e0 mov      r6, r0
004727e4 mov      r0, r7
004727e8 bl       #0x30df8c
004727ec cmp      r0, #0
004727f0 ldr      r8, [r4, #8]
004727f4 ldr      r4, [r4, #4]
004727f8 bne      #0x472834
004727fc ldr      r0, [r5, #8]
00472800 add      r1, sp, #4
00472804 ldr      r3, [r0]
00472808 ldr      r3, [r3, #0x94]
0047280c str      r7, [sp, #4]
00472810 str      r4, [sp, #8]
00472814 str      r8, [sp, #0xc]
00472818 blx      r3
0047281c mov      r0, r5
00472820 bl       #0x47211c ; _ZN12VisualObject11CalcMeshBoxEv
00472824 mov      r0, r5
00472828 bl       #0x470a54 ; _ZN12VisualObject12ApplyMeshBoxEv
0047282c add      sp, sp, #0x10
00472830 pop      {r4, r5, r6, r7, r8, pc}
00472834 mov      r0, r4
00472838 ldr      r1, [r6, #4]
0047283c bl       #0x30df8c
00472840 cmp      r0, #0
00472844 beq      #0x4727fc
00472848 ldr      r1, [r6, #8]
0047284c mov      r0, r8
00472850 bl       #0x30df8c
00472854 cmp      r0, #0
00472858 bne      #0x47282c
0047285c b        #0x4727fc
_ZN12VisualObject12SyncPositionEv
00470cb8 ldr      r1, [r0, #4]
00470cbc cmp      r1, #0
00470cc0 bxeq     lr
00470cc4 add      r1, r1, #0x160
00470cc8 b        #0x470c24
