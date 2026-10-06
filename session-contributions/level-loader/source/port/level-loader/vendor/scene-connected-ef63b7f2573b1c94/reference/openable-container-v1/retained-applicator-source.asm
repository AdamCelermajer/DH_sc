_ZN14AnimApplicatorC1EPN6glitch7collada18ISceneNodeAnimatorE
00364398 push     {r4, r5}
0036439c ldr      r4, [pc, #0x54]
003643a0 ldr      r5, [pc, #0x54]
003643a4 mov      ip, #0
003643a8 add      r4, pc, r4
003643ac ldr      r5, [r4, r5]
003643b0 mov      r2, #0
003643b4 str      r2, [r0, #0x38]
003643b8 add      r5, r5, #8
003643bc str      r5, [r0]
003643c0 str      r1, [r0, #4]
003643c4 str      ip, [r0, #0x2c]
003643c8 str      r2, [r0, #8]
003643cc str      r2, [r0, #0x10]
003643d0 str      r2, [r0, #0x14]
003643d4 str      ip, [r0, #0x18]
003643d8 str      ip, [r0, #0x1c]
003643dc str      ip, [r0, #0x20]
003643e0 str      ip, [r0, #0x24]
003643e4 str      ip, [r0, #0x28]
003643e8 strb     r2, [r0, #0x30]
003643ec str      r2, [r0, #0x34]
003643f0 pop      {r4, r5}
003643f4 bx       lr
003643f8 rsbeq    r0, r3, r8, ror #13
003643fc andeq    r2, r0, r4, lsl #22
