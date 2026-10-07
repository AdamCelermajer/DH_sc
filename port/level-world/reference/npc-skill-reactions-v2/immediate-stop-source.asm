_ZN12CharAnimator9ANIM_StopEv
003c9924 push     {r4, lr}
003c9928 ldr      r3, [r0, #4]
003c992c mov      r2, #0
003c9930 str      r2, [r0, #0x2c]
003c9934 ldr      r3, [r3, #0x2d8]
003c9938 mov      r4, r0
003c993c cmp      r3, r2
003c9940 beq      #0x3c9968
003c9944 ldr      r3, [r3, #0x38]
003c9948 mov      r1, #1
003c994c mov      r0, r3
003c9950 ldr      r3, [r3]
003c9954 mov      lr, pc
003c9958 ldr      pc, [r3, #0x24]
003c995c ldrb     r2, [r4, #0x48]
003c9960 cmp      r2, #0
003c9964 beq      #0x3c996c
003c9968 pop      {r4, pc}
003c996c ldr      r0, [r4, #4]
003c9970 mov      r3, #1
003c9974 mov      r1, #0x22
003c9978 strb     r3, [r4, #0x48]
003c997c pop      {r4, lr}
003c9980 b        #0x3a4d5c
_ZN24BlendedAnimSetController8StopClipEbj
00476780 push     {r4, r5, r6, lr}
00476784 mov      r4, r1
00476788 mov      r1, r2
0047678c bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00476790 ldr      r2, [r0, #0x70]
00476794 ldr      r3, [r0, #0x28]
00476798 mov      r6, r0
0047679c ldr      r3, [r3, r2, lsl #2]
004767a0 cmp      r3, #0
004767a4 beq      #0x476800
004767a8 mov      r0, r3
004767ac ldr      r3, [r3]
004767b0 mov      lr, pc
004767b4 ldr      pc, [r3, #0x44]
004767b8 mov      r1, #0
004767bc mov      r5, r0
004767c0 mov      r0, r6
004767c4 bl       #0x3666d8 ; _ZN15AnimatorBlender8SetScaleEf
004767c8 cmp      r5, #0
004767cc beq      #0x4767d8
004767d0 cmp      r4, #0
004767d4 bne      #0x4767dc
004767d8 pop      {r4, r5, r6, pc}
004767dc ldr      r3, [r5]
004767e0 mov      r0, r5
004767e4 ldr      r4, [r3, #0xc]
004767e8 mov      lr, pc
004767ec ldr      pc, [r3, #0x30]
004767f0 mov      r1, r0
004767f4 mov      r0, r5
004767f8 blx      r4
004767fc pop      {r4, r5, r6, pc}
00476800 mov      r1, #0
00476804 pop      {r4, r5, r6, lr}
00476808 b        #0x3666d8
_ZN15AnimatorBlender8SetScaleEf
003666d8 push     {r4, r5, r6, r7, r8, lr}
003666dc ldr      r3, [r0, #0x28]
003666e0 ldr      r6, [r0, #0x2c]
003666e4 mov      r5, r0
003666e8 mov      r7, r1
003666ec rsb      r6, r3, r6
003666f0 asrs     r6, r6, #2
003666f4 beq      #0x36673c
003666f8 mov      r4, #0
003666fc b        #0x366704
00366700 ldr      r3, [r5, #0x28]
00366704 ldr      r3, [r3, r4, lsl #2]
00366708 add      r4, r4, #1
0036670c mov      r0, r3
00366710 ldr      r3, [r3]
00366714 mov      lr, pc
00366718 ldr      pc, [r3, #0x44]
0036671c subs     r3, r0, #0
00366720 mov      r1, r7
00366724 beq      #0x366734
00366728 ldr      r3, [r3]
0036672c mov      lr, pc
00366730 ldr      pc, [r3, #0x48]
00366734 cmp      r4, r6
00366738 bne      #0x366700
0036673c pop      {r4, r5, r6, r7, r8, pc}
