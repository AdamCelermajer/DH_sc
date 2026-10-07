_ZN14AnimController8PlayClipEPKcbij
004749f4 push     {r4, r5, r6, r7, r8, sb, sl, lr}
004749f8 mov      r4, r1
004749fc ldr      r1, [sp, #0x20]
00474a00 mov      sl, r2
00474a04 mov      r8, r0
00474a08 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00474a0c subs     r6, r0, #0
00474a10 beq      #0x474b00
00474a14 ldr      r3, [r6]
00474a18 mov      lr, pc
00474a1c ldr      pc, [r3, #0x44]
00474a20 mov      r5, r0
00474a24 mov      r0, r6
00474a28 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00474a2c cmp      r5, #0
00474a30 mov      sb, r0
00474a34 beq      #0x474a6c
00474a38 ldr      r3, [r5]
00474a3c mov      r0, r5
00474a40 mov      lr, pc
00474a44 ldr      pc, [r3, #0x1c]
00474a48 cmp      r0, #0
00474a4c ble      #0x474a6c
00474a50 ldr      r3, [r5]
00474a54 mov      r0, r5
00474a58 mov      r1, r4
00474a5c mov      lr, pc
00474a60 ldr      pc, [r3, #0x18]
00474a64 cmn      r0, #1
00474a68 beq      #0x474b0c
00474a6c ldr      r3, [r5]
00474a70 mov      r0, r5
00474a74 mov      lr, pc
00474a78 ldr      pc, [r3, #0x38]
00474a7c mov      r1, r4
00474a80 mov      r7, r0
00474a84 ldr      r3, [r6]
00474a88 mov      r0, r6
00474a8c mov      lr, pc
00474a90 ldr      pc, [r3, #0x38]
00474a94 mov      r4, r0
00474a98 ldr      r3, [r6]
00474a9c mov      r0, r6
00474aa0 mov      r1, r4
00474aa4 mov      lr, pc
00474aa8 ldr      pc, [r3, #0x30]
00474aac cmp      r7, r4
00474ab0 beq      #0x474b14
00474ab4 mov      r1, sl
00474ab8 mov      r0, r5
00474abc ldr      r3, [r5]
00474ac0 mov      lr, pc
00474ac4 ldr      pc, [r3, #0x40]
00474ac8 ldr      r3, [r5]
00474acc mov      r0, r5
00474ad0 mov      r1, #0x3f800000
00474ad4 mov      lr, pc
00474ad8 ldr      pc, [r3, #0x48]
00474adc ldr      r0, [r8, #4]
00474ae0 mov      r1, #0
00474ae4 bl       #0x35d624 ; _ZN13RootSceneNode7NewAnimEb
00474ae8 ldr      r3, [r8, #4]
00474aec mov      r0, #1
00474af0 ldr      r2, [r3, #0x11c]
00474af4 orr      r2, r2, #0x200
00474af8 str      r2, [r3, #0x11c]
00474afc pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474b00 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00474b04 mov      r0, r6
00474b08 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474b0c mov      r0, #0
00474b10 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474b14 ldr      r3, [r5]
00474b18 mov      r0, r5
00474b1c mov      lr, pc
00474b20 ldr      pc, [r3, #0x44]
00474b24 cmp      r0, #0
00474b28 bne      #0x474ab4
00474b2c cmp      sb, #0
00474b30 ldr      r3, [r5]
00474b34 ldr      r1, [r5, #0x10]
00474b38 ldrne    sb, [sb, #0x10]
00474b3c ldr      r3, [r3, #0xc]
00474b40 mov      r0, r5
00474b44 add      r1, sb, r1
00474b48 blx      r3
00474b4c b        #0x474ab4
_ZN14AnimControllerC1EP13RootSceneNodeb
00474d30 push     {r4, r5, r6, lr}
00474d34 ldr      r4, [pc, #0xe4]
00474d38 ldr      r3, [pc, #0xe4]
00474d3c cmp      r1, #0
00474d40 add      r4, pc, r4
00474d44 ldr      r3, [r4, r3]
00474d48 sub      sp, sp, #8
00474d4c mov      r5, r0
00474d50 add      r3, r3, #8
00474d54 str      r3, [r0]
00474d58 mov      r6, r2
00474d5c str      r1, [r0, #4]
00474d60 beq      #0x474dc8
00474d64 ldr      r3, [r1]
00474d68 cmp      r6, #0
00474d6c ldr      r3, [r3, #-0xc]
00474d70 add      r1, r1, r3
00474d74 ldr      r3, [r1, #4]
00474d78 add      r3, r3, #1
00474d7c str      r3, [r1, #4]
00474d80 bne      #0x474db0
00474d84 ldr      r3, [pc, #0x9c]
00474d88 mov      r0, r5
00474d8c mov      r2, r5
00474d90 ldr      r1, [r4, r3]
00474d94 ldr      r3, [pc, #0x90]
00474d98 str      r5, [sp]
00474d9c ldr      r3, [r4, r3]
00474da0 bl       #0x474cac ; _ZN14AnimController17SetCallbacksOnAllEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474da4 mov      r0, r5
00474da8 add      sp, sp, #8
00474dac pop      {r4, r5, r6, pc}
00474db0 ldr      r3, [r5, #4]
00474db4 mov      r0, r3
00474db8 ldr      r3, [r3]
00474dbc mov      lr, pc
00474dc0 ldr      pc, [r3, #0x74]
00474dc4 b        #0x474da4
00474dc8 ldr      r3, [pc, #0x60]
00474dcc ldr      r3, [r4, r3]
00474dd0 ldr      r3, [r3]
00474dd4 cmp      r3, #2
00474dd8 streq    r1, [r1]
00474ddc beq      #0x474d64
00474de0 cmp      r3, #1
00474de4 bne      #0x474d64
00474de8 ldr      r0, [pc, #0x44]
00474dec ldr      r1, [pc, #0x44]
00474df0 ldr      r2, [pc, #0x44]
00474df4 ldr      r0, [r4, r0]
00474df8 ldr      r3, [pc, #0x40]
00474dfc add      r1, pc, r1
00474e00 mov      ip, #0x1c
00474e04 add      r0, r0, #0xa8
00474e08 add      r2, pc, r2
00474e0c add      r3, pc, r3
00474e10 str      ip, [sp]
00474e14 bl       #0x30e004
00474e18 ldr      r1, [r5, #4]
00474e1c b        #0x474d64
00474e20 subseq   pc, r1, r0, asr sp
00474e24 andeq    r4, r0, r0, lsl r8
00474e28 andeq    r3, r0, r0, ror r7
00474e2c andeq    r2, r0, r0, lsl #18
00474e30 andeq    r3, r0, r0, asr #19
00474e34 andeq    r1, r0, r0, asr #19
00474e38 ldrdeq   sb, sl, [r4], #-0x5c
00474e3c subeq    r8, r5, r8, lsr r8
00474e40 subeq    r8, r5, ip, asr sb
_ZN10GameObject8InitPostEv
0038be5c push     {r4, r5, r6, r7, r8, lr}
0038be60 mov      r4, r0
0038be64 bl       #0x33ec0c ; _ZN10ObjectBase8InitPostEv
0038be68 mov      r0, r4
0038be6c bl       #0x38bd64 ; _ZN10GameObject21CheckSpawnProbabilityEv
0038be70 ldr      r3, [r4, #0x274]
0038be74 ldr      r5, [pc, #0x204]
0038be78 cmp      r0, r3
0038be7c add      r5, pc, r5
0038be80 bge      #0x38c02c
0038be84 ldr      r0, [r4, #0x120]
0038be88 mov      r3, #0
0038be8c movw     r1, #0xb717
0038be90 str      r3, [r4, #0x2dc]
0038be94 movt     r1, #0x38d1
0038be98 bic      r0, r0, #0x80000000
0038be9c bl       #0x30e70c
0038bea0 cmp      r0, #0
0038bea4 ldr      r0, [r4, #0x124]
0038bea8 movne    r3, #0x3f800000
0038beac movw     r1, #0xb717
0038beb0 strne    r3, [r4, #0x120]
0038beb4 movt     r1, #0x38d1
0038beb8 bic      r0, r0, #0x80000000
0038bebc bl       #0x30e70c
0038bec0 cmp      r0, #0
0038bec4 ldr      r0, [r4, #0x128]
0038bec8 movne    r3, #0x3f800000
0038becc movw     r1, #0xb717
0038bed0 strne    r3, [r4, #0x124]
0038bed4 movt     r1, #0x38d1
0038bed8 bic      r0, r0, #0x80000000
0038bedc bl       #0x30e70c
0038bee0 cmp      r0, #0
0038bee4 movne    r3, #0x3f800000
0038bee8 movw     r1, #0xfa35
0038beec strne    r3, [r4, #0x128]
0038bef0 ldr      r0, [r4, #0x16c]
0038bef4 movt     r1, #0x3c8e
0038bef8 bl       #0x30ed6c
0038befc movw     r1, #0xfa35
0038bf00 str      r0, [r4, #0x16c]
0038bf04 movt     r1, #0x3c8e
0038bf08 ldr      r0, [r4, #0x170]
0038bf0c bl       #0x30ed6c
0038bf10 movw     r1, #0xfa35
0038bf14 str      r0, [r4, #0x170]
0038bf18 movt     r1, #0x3c8e
0038bf1c ldr      r0, [r4, #0x174]
0038bf20 bl       #0x30ed6c
0038bf24 mov      r2, #1
0038bf28 str      r0, [r4, #0x178]
0038bf2c str      r0, [r4, #0x174]
0038bf30 add      r1, r4, #0x160
0038bf34 mov      r0, r4
0038bf38 bl       #0x393db4 ; _ZN10GameObject11SetPositionERK7Point3DIfEb
0038bf3c ldr      r0, [r4, #0x144]
0038bf40 ldr      r1, [r4, #0x120]
0038bf44 bl       #0x30ed6c
0038bf48 ldr      r1, [r4, #0x124]
0038bf4c str      r0, [r4, #0x144]
0038bf50 ldr      r0, [r4, #0x148]
0038bf54 bl       #0x30ed6c
0038bf58 ldr      r1, [r4, #0x128]
0038bf5c str      r0, [r4, #0x148]
0038bf60 ldr      r0, [r4, #0x14c]
0038bf64 bl       #0x30ed6c
0038bf68 ldr      r1, [r4, #0x120]
0038bf6c str      r0, [r4, #0x14c]
0038bf70 ldr      r0, [r4, #0x150]
0038bf74 bl       #0x30ed6c
0038bf78 ldr      r1, [r4, #0x124]
0038bf7c str      r0, [r4, #0x150]
0038bf80 ldr      r0, [r4, #0x154]
0038bf84 bl       #0x30ed6c
0038bf88 ldr      r1, [r4, #0x128]
0038bf8c str      r0, [r4, #0x154]
0038bf90 ldr      r0, [r4, #0x158]
0038bf94 bl       #0x30ed6c
0038bf98 str      r0, [r4, #0x158]
0038bf9c mov      r0, r4
0038bfa0 bl       #0x38aac8 ; _ZN10GameObject18UpdateAbsoluteAABBEv
0038bfa4 ldr      r0, [r4, #0x2d8]
0038bfa8 cmp      r0, #0
0038bfac beq      #0x38c040
0038bfb0 bl       #0x38ba74 ; _ZN12VisualObject4SyncEv
0038bfb4 ldrb     r3, [r4, #0x15c]
0038bfb8 ldr      r7, [r4, #0x36c]
0038bfbc cmp      r3, #0
0038bfc0 movne    r3, #0
0038bfc4 strbne   r3, [r4, #0x28]
0038bfc8 ldr      r3, [r4, #0x368]
0038bfcc cmp      r3, r7
0038bfd0 beq      #0x38c02c
0038bfd4 ldr      r3, [pc, #0xa8]
0038bfd8 ldr      r3, [r5, r3]
0038bfdc ldr      r8, [r3]
0038bfe0 cmp      r8, #0
0038bfe4 beq      #0x38c030
0038bfe8 ldr      r3, [pc, #0x98]
0038bfec mov      r6, #0
0038bff0 ldr      r3, [r5, r3]
0038bff4 ldr      r5, [r3]
0038bff8 b        #0x38c008
0038bffc add      r6, r6, #1
0038c000 cmp      r6, r8
0038c004 beq      #0x38c030
0038c008 ldr      r1, [r5, r6, lsl #2]
0038c00c mov      r0, r7
0038c010 bl       #0x30e31c
0038c014 cmp      r0, #0
0038c018 bne      #0x38bffc
0038c01c uxth     r6, r6
0038c020 mov      r3, #0x370
0038c024 strh     r6, [r4, r3]
0038c028 pop      {r4, r5, r6, r7, r8, pc}
0038c02c pop      {r4, r5, r6, r7, r8, pc}
0038c030 movw     r6, #0xffff
0038c034 mov      r3, #0x370
0038c038 strh     r6, [r4, r3]
0038c03c pop      {r4, r5, r6, r7, r8, pc}
0038c040 bl       #0x38174c ; _ZN6Device17IsHighPerformanceEv
0038c044 cmp      r0, #0
0038c048 bne      #0x38c068
0038c04c ldrb     r3, [r4, #0x60]
0038c050 cmp      r3, #0
0038c054 beq      #0x38c068
0038c058 ldrb     r3, [r4, #0x10c]
0038c05c cmp      r3, #0
0038c060 strne    r0, [r4, #0x2d8]
0038c064 bne      #0x38bfb4
0038c068 mov      r0, r4
0038c06c bl       #0x394eb0 ; _ZN10GameObject16LoadVisualObjectEv
0038c070 ldr      r0, [r4, #0x2d8]
0038c074 cmp      r0, #0
0038c078 beq      #0x38bfb4
0038c07c b        #0x38bfb0
0038c080 rsbeq    r8, r0, r4, lsl ip
0038c084 andeq    r3, r0, r8, lsr sp
0038c088 andeq    r3, r0, r8, lsr #19
