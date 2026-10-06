_ZN6glitch7collada19CTimelineController7setClipEi
00666f7c push     {r4, r5, r6, r7, r8, lr}
00666f80 mov      r3, #0
00666f84 str      r1, [r0, #0x38]
00666f88 strb     r3, [r0, #0x3d]
00666f8c strb     r3, [r0, #0x3c]
00666f90 ldr      r3, [r0]
00666f94 mov      r4, r0
00666f98 mov      lr, pc
00666f9c ldr      pc, [r3, #0x2c]
00666fa0 ldr      r3, [r4]
00666fa4 str      r0, [r4, #0x10]
00666fa8 mov      r0, r4
00666fac mov      lr, pc
00666fb0 ldr      pc, [r3, #0x30]
00666fb4 mov      r7, r0
00666fb8 str      r0, [r4, #0x14]
00666fbc ldr      r0, [r4, #0x10]
00666fc0 bl       #0x30e964
00666fc4 mov      r1, #0x44000000
00666fc8 add      r1, r1, #0x7a0000
00666fcc bl       #0x30ec94
00666fd0 ldr      r6, [r4, #0x10]
00666fd4 mov      r5, r0
00666fd8 str      r0, [r4, #0x20]
00666fdc rsb      r0, r6, r7
00666fe0 bl       #0x30e964
00666fe4 mov      r1, #0x44000000
00666fe8 add      r1, r1, #0x7a0000
00666fec bl       #0x30ec94
00666ff0 str      r6, [r4, #4]
00666ff4 str      r0, [r4, #0x24]
00666ff8 str      r5, [r4, #0x2c]
00666ffc pop      {r4, r5, r6, r7, r8, pc}
_ZN13RootSceneNode7NewAnimEb
0035d624 push     {r4, lr}
0035d628 mov      r4, r0
0035d62c bl       #0x35d4cc ; _ZN13RootSceneNode19_EnableDisplacementEb
0035d630 ldrb     r3, [r4, #0x1ec]
0035d634 cmp      r3, #0
0035d638 beq      #0x35d654
0035d63c ldr      r3, [r4, #0x1f0]
0035d640 cmp      r3, #0
0035d644 beq      #0x35d654
0035d648 ldr      r1, [r4, #0x1fc]
0035d64c cmp      r1, #0
0035d650 bne      #0x35d658
0035d654 pop      {r4, pc}
0035d658 mov      r0, r4
0035d65c add      r1, r1, #1
0035d660 bl       #0x35ce6c ; _ZN13RootSceneNode11_ResetDeltaEj
0035d664 mov      r0, r4
0035d668 ldr      r3, [r4]
0035d66c ldr      r1, [r4, #0x1fc]
0035d670 mov      lr, pc
0035d674 ldr      pc, [r3, #0x14]
0035d678 pop      {r4, pc}
