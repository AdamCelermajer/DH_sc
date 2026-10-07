_ZN6glitch7collada18CSceneNodeAnimatorD1Ev
0065dab0 push     {r4, r5, r6, lr}
0065dab4 ldr      r5, [pc, #0x6c]
0065dab8 ldr      r3, [pc, #0x6c]
0065dabc mov      r4, r0
0065dac0 add      r5, pc, r5
0065dac4 ldr      r3, [r5, r3]
0065dac8 add      r2, r3, #0xa8
0065dacc add      r1, r3, #0xc
0065dad0 add      r3, r3, #0xc4
0065dad4 str      r1, [r0]
0065dad8 str      r3, [r0, #0x58]
0065dadc str      r2, [r0, #4]
0065dae0 bl       #0x65d628 ; _ZN6glitch7collada18CSceneNodeAnimator21removeAnimationTracksEv
0065dae4 ldr      r0, [r4, #0x54]
0065dae8 cmp      r0, #0
0065daec beq      #0x65daf4
0065daf0 bl       #0x60bbd4 ; _ZN6glitch7collada21intrusive_ptr_releaseEPNS0_15CAnimationBlockE
0065daf4 ldr      r0, [r4, #0x44]
0065daf8 cmp      r0, #0
0065dafc beq      #0x65db04
0065db00 bl       #0x310450 ; _Z10GlitchFreePv
0065db04 add      r0, r4, #0x28
0065db08 bl       #0x619474 ; _ZN6glitch7collada16CColladaDatabaseD1Ev
0065db0c ldr      r1, [pc, #0x1c]
0065db10 mov      r0, r4
0065db14 ldr      r1, [r5, r1]
0065db18 add      r1, r1, #4
0065db1c bl       #0x6697c4 ; _ZN6glitch7collada18ISceneNodeAnimatorD2Ev
0065db20 mov      r0, r4
0065db24 pop      {r4, r5, r6, pc}
0065db28 ldrsbteq r6, [r3], -r0
0065db2c andeq    r4, r0, r8, lsl #23
0065db30 andeq    r3, r0, ip, lsl #18
_ZN6glitch5scene18ISceneNodeAnimatorD1Ev
005998b0 push     {r4, lr}
005998b4 ldr      r2, [pc, #0x50]
005998b8 ldr      r3, [pc, #0x50]
005998bc ldr      r1, [r0, #8]
005998c0 add      r2, pc, r2
005998c4 ldr      r3, [r2, r3]
005998c8 mov      r4, r0
005998cc cmp      r1, #0
005998d0 add      r2, r3, #0x68
005998d4 add      r0, r3, #0xc
005998d8 add      r3, r3, #0x84
005998dc str      r0, [r4]
005998e0 str      r3, [r4, #0xc]
005998e4 str      r2, [r4, #4]
005998e8 beq      #0x5998fc
005998ec ldr      r3, [r1]
005998f0 ldr      r0, [r3, #-0xc]
005998f4 add      r0, r1, r0
005998f8 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
005998fc mov      r0, r4
00599900 bl       #0x6a1194 ; _ZN6glitch7IObjectD2Ev
00599904 mov      r0, r4
00599908 pop      {r4, pc}
0059990c ldrsbteq fp, [pc], -r0
00599910 andeq    r2, r0, r8, lsl #6
