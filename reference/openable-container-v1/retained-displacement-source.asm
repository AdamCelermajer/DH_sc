_ZN13RootSceneNode19_EnableDisplacementEb
0035d4cc push     {r4, r5, r6, r7, r8, lr}
0035d4d0 subs     r5, r1, #0
0035d4d4 mov      r4, r0
0035d4d8 beq      #0x35d4e8
0035d4dc ldr      r6, [r0, #0x1f0]
0035d4e0 cmp      r6, #0
0035d4e4 beq      #0x35d4f0
0035d4e8 strb     r5, [r4, #0x1ec]
0035d4ec pop      {r4, r5, r6, r7, r8, pc}
0035d4f0 mov      r1, r6
0035d4f4 bl       #0x35ccdc ; _ZN13RootSceneNode11GetAnimRootEb
0035d4f8 mov      r1, #1
0035d4fc str      r0, [r4, #0x1f0]
0035d500 mov      r0, r4
0035d504 bl       #0x35ccdc ; _ZN13RootSceneNode11GetAnimRootEb
0035d508 ldr      r3, [r4, #0x1f0]
0035d50c str      r0, [r4, #0x1f4]
0035d510 cmp      r3, #0
0035d514 beq      #0x35d61c
0035d518 cmp      r0, r3
0035d51c streq    r6, [r4, #0x1f4]
0035d520 beq      #0x35d548
0035d524 cmp      r0, #0
0035d528 beq      #0x35d548
0035d52c ldr      r3, [r0]
0035d530 ldr      r3, [r3, #-0xc]
0035d534 add      r0, r0, r3
0035d538 ldr      r3, [r0, #4]
0035d53c add      r3, r3, #1
0035d540 str      r3, [r0, #4]
0035d544 ldr      r3, [r4, #0x1f0]
0035d548 ldr      r2, [r3]
0035d54c mov      r1, #0
0035d550 mov      r0, #0x150
0035d554 ldr      r2, [r2, #-0xc]
0035d558 mov      r7, r4
0035d55c add      r3, r3, r2
0035d560 ldr      r2, [r3, #4]
0035d564 add      r2, r2, #1
0035d568 str      r2, [r3, #4]
0035d56c bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
0035d570 mvn      r1, #0
0035d574 mov      r6, r0
0035d578 bl       #0x5839d8 ; _ZN6glitch5scene15CEmptySceneNodeC1Ei
0035d57c str      r6, [r4, #0x1f8]
0035d580 mov      r3, r6
0035d584 ldr      r6, [r7, #0xf4]!
0035d588 cmp      r6, r7
0035d58c bne      #0x35d598
0035d590 b        #0x35d5c4
0035d594 ldr      r3, [r4, #0x1f8]
0035d598 cmp      r6, #0
0035d59c moveq    r1, r6
0035d5a0 subne    r1, r6, #4
0035d5a4 ldr      r6, [r6]
0035d5a8 mov      r0, r3
0035d5ac ldr      r3, [r3]
0035d5b0 mov      lr, pc
0035d5b4 ldr      pc, [r3, #0x5c]
0035d5b8 cmp      r7, r6
0035d5bc bne      #0x35d594
0035d5c0 ldr      r3, [r4, #0x1f8]
0035d5c4 mov      r1, r3
0035d5c8 mov      r0, r4
0035d5cc ldr      r3, [r4]
0035d5d0 mov      r7, r4
0035d5d4 mov      lr, pc
0035d5d8 ldr      pc, [r3, #0x5c]
0035d5dc ldr      r6, [r7, #0xfc]!
0035d5e0 b        #0x35d608
0035d5e4 ldr      r0, [r6, #8]
0035d5e8 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
0035d5ec subs     r3, r0, #0
0035d5f0 beq      #0x35d614
0035d5f4 ldr      r3, [r3]
0035d5f8 ldr      r1, [r4, #0x1f0]
0035d5fc mov      lr, pc
0035d600 ldr      pc, [r3, #8]
0035d604 ldr      r6, [r6]
0035d608 cmp      r7, r6
0035d60c bne      #0x35d5e4
0035d610 b        #0x35d4e8
0035d614 mov      r5, r3
0035d618 b        #0x35d4e8
0035d61c str      r3, [r4, #0x1f4]
0035d620 pop      {r4, r5, r6, r7, r8, pc}
