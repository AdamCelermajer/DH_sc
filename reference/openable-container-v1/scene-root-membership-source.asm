_ZN6glitch5scene10ISceneNode8addChildEPS1_
00598864 cmp      r1, r0
00598868 cmpne    r1, #0
0059886c push     {r4, r5, r6, lr}
00598870 mov      r5, r0
00598874 mov      r4, r1
00598878 bne      #0x598880
0059887c pop      {r4, r5, r6, pc}
00598880 ldr      r3, [r1]
00598884 mov      r0, r1
00598888 ldr      r3, [r3, #-0xc]
0059888c add      r3, r1, r3
00598890 ldr      r2, [r3, #4]
00598894 add      r2, r2, #1
00598898 str      r2, [r3, #4]
0059889c ldr      r3, [r1]
005988a0 mov      lr, pc
005988a4 ldr      pc, [r3, #0x68]
005988a8 ldr      r2, [r5, #0xf8]
005988ac add      r3, r4, #4
005988b0 add      r1, r5, #0xf4
005988b4 str      r2, [r4, #8]
005988b8 str      r3, [r2]
005988bc str      r3, [r5, #0xf8]
005988c0 str      r1, [r4, #4]
005988c4 ldr      r3, [r5, #0xf0]
005988c8 mov      r0, r4
005988cc mov      r1, r5
005988d0 add      r3, r3, #1
005988d4 str      r3, [r5, #0xf0]
005988d8 bl       #0x5971e0 ; _ZN6glitch5scene10ISceneNode9setParentEPS1_
005988dc ldr      r0, [r5, #0x110]
005988e0 cmp      r0, #0
005988e4 beq      #0x5988ec
005988e8 bl       #0x5890b4 ; _ZN6glitch5scene13CSceneManager22notifyHierarchyChangedEv
005988ec ldr      r1, [r5, #0x11c]
005988f0 mov      r0, r4
005988f4 ldr      r3, [r4]
005988f8 and      r1, r1, #1
005988fc mov      lr, pc
00598900 ldr      pc, [r3, #0xec]
00598904 pop      {r4, r5, r6, pc}
_ZN6glitch5scene10ISceneNode15removeAnimatorsEv
00598658 push     {r4, r5, r6, lr}
0059865c mov      r6, r0
00598660 mov      r5, r0
00598664 ldr      r4, [r6, #0xfc]!
00598668 b        #0x59869c
0059866c ldr      r3, [r4, #8]
00598670 mov      r1, r5
00598674 mov      r0, r3
00598678 ldr      r3, [r3]
0059867c mov      lr, pc
00598680 ldr      pc, [r3, #0x2c]
00598684 ldr      r3, [r4, #8]
00598688 ldr      r2, [r3]
0059868c ldr      r0, [r2, #-0xc]
00598690 add      r0, r3, r0
00598694 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00598698 ldr      r4, [r4]
0059869c cmp      r6, r4
005986a0 bne      #0x59866c
005986a4 ldr      r0, [r5, #0xfc]
005986a8 cmp      r4, r0
005986ac bne      #0x5986b8
005986b0 b        #0x5986c8
005986b4 mov      r0, r6
005986b8 ldr      r6, [r0]
005986bc bl       #0x310450 ; _Z10GlitchFreePv
005986c0 cmp      r4, r6
005986c4 bne      #0x5986b4
005986c8 ldr      r0, [r5, #0x110]
005986cc str      r4, [r5, #0x100]
005986d0 str      r4, [r5, #0xfc]
005986d4 cmp      r0, #0
005986d8 beq      #0x5986e4
005986dc pop      {r4, r5, r6, lr}
005986e0 b        #0x5890b4
005986e4 pop      {r4, r5, r6, pc}
_ZN6glitch5scene10ISceneNode15setSceneManagerEPNS0_13CSceneManagerE
00588e30 push     {r4, r5, r6, r7, r8, lr}
00588e34 mov      r7, r0
00588e38 str      r1, [r7, #0x110]
00588e3c mov      r5, r0
00588e40 mov      r6, r1
00588e44 ldr      r4, [r5, #0xf4]!
00588e48 b        #0x588e64
00588e4c cmp      r4, #0
00588e50 moveq    r0, r4
00588e54 subne    r0, r4, #4
00588e58 mov      r1, r6
00588e5c bl       #0x588e30 ; _ZN6glitch5scene10ISceneNode15setSceneManagerEPNS0_13CSceneManagerE
00588e60 ldr      r4, [r4]
00588e64 cmp      r5, r4
00588e68 bne      #0x588e4c
00588e6c mov      r0, r7
00588e70 ldr      r3, [r7]
00588e74 mov      lr, pc
00588e78 ldr      pc, [r3, #0xf0]
00588e7c pop      {r4, r5, r6, r7, r8, pc}
_ZN6glitch5scene13CSceneManager18registerSceneNodesEPNS0_10ISceneNodeE
0058b88c push     {r4, r5, r6, r7, r8, lr}
0058b890 subs     r5, r1, #0
0058b894 mov      r8, r0
0058b898 beq      #0x58b974
0058b89c mov      r0, r5
0058b8a0 bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
0058b8a4 ldr      r6, [r5, #4]
0058b8a8 mov      r7, r0
0058b8ac add      r5, r5, #4
0058b8b0 mov      r4, r0
0058b8b4 cmp      r5, #0
0058b8b8 moveq    r3, r5
0058b8bc subne    r3, r5, #4
0058b8c0 ldr      r3, [r3, #0x11c]
0058b8c4 tst      r3, #1
0058b8c8 beq      #0x58b934
0058b8cc cmp      r5, #0
0058b8d0 moveq    r1, r5
0058b8d4 subne    r1, r5, #4
0058b8d8 mov      r0, r8
0058b8dc bl       #0x58ab28 ; _ZNK6glitch5scene13CSceneManager8isCulledEPKNS0_10ISceneNodeE
0058b8e0 cmp      r0, #0
0058b8e4 bne      #0x58b934
0058b8e8 cmp      r5, #0
0058b8ec moveq    r3, r5
0058b8f0 subne    r3, r5, #4
0058b8f4 mov      r0, r3
0058b8f8 ldr      r3, [r3]
0058b8fc mov      lr, pc
0058b900 ldr      pc, [r3, #0x10]
0058b904 cmp      r0, #0
0058b908 beq      #0x58b934
0058b90c cmp      r5, #0
0058b910 moveq    r4, r5
0058b914 subne    r4, r5, #4
0058b918 ldr      r5, [r4, #0xf4]
0058b91c add      r6, r4, #0xf4
0058b920 cmp      r6, r5
0058b924 beq      #0x58b940
0058b928 cmp      r4, r7
0058b92c bne      #0x58b8b4
0058b930 pop      {r4, r5, r6, r7, r8, pc}
0058b934 ldr      r5, [r5]
0058b938 cmp      r6, r5
0058b93c bne      #0x58b928
0058b940 cmp      r7, r4
0058b944 beq      #0x58b9e4
0058b948 mov      r0, r4
0058b94c bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
0058b950 ldr      r5, [r4, #4]
0058b954 add      r6, r0, #0xf4
0058b958 cmp      r6, r5
0058b95c movne    r4, r0
0058b960 bne      #0x58b928
0058b964 cmp      r7, r0
0058b968 mov      r4, r0
0058b96c bne      #0x58b948
0058b970 pop      {r4, r5, r6, r7, r8, pc}
0058b974 ldrb     r3, [r0, #0x288]
0058b978 cmp      r3, #0
0058b97c bne      #0x58b9e8
0058b980 ldr      r4, [r8, #0x270]
0058b984 ldr      r5, [r8, #0x274]
0058b988 cmp      r4, r5
0058b98c bne      #0x58b9a0
0058b990 b        #0x58b970
0058b994 add      r4, r4, #4
0058b998 cmp      r4, r5
0058b99c beq      #0x58b9e0
0058b9a0 ldr      r1, [r4]
0058b9a4 ldr      r3, [r1, #0x11c]
0058b9a8 tst      r3, #1
0058b9ac beq      #0x58b994
0058b9b0 mov      r0, r8
0058b9b4 bl       #0x58ab28 ; _ZNK6glitch5scene13CSceneManager8isCulledEPKNS0_10ISceneNodeE
0058b9b8 cmp      r0, #0
0058b9bc bne      #0x58b994
0058b9c0 ldr      r3, [r4]
0058b9c4 add      r4, r4, #4
0058b9c8 mov      r0, r3
0058b9cc ldr      r3, [r3]
0058b9d0 mov      lr, pc
0058b9d4 ldr      pc, [r3, #0x10]
0058b9d8 cmp      r4, r5
0058b9dc bne      #0x58b9a0
0058b9e0 pop      {r4, r5, r6, r7, r8, pc}
0058b9e4 pop      {r4, r5, r6, r7, r8, pc}
0058b9e8 bl       #0x58b7ac ; _ZN6glitch5scene13CSceneManager15collectAllNodesEv
0058b9ec b        #0x58b980
