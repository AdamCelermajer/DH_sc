_ZN10GameObject15SetVisualObjectEP12VisualObject
00394338 push     {r4, r5, r6, lr}
0039433c ldr      r3, [r0, #0x2d8]
00394340 mov      r4, r0
00394344 mov      r5, r1
00394348 cmp      r3, r1
0039434c beq      #0x394374
00394350 cmp      r3, #0
00394354 beq      #0x394370
00394358 mov      r0, r3
0039435c ldr      r3, [r3]
00394360 mov      lr, pc
00394364 ldr      pc, [r3, #4]
00394368 mov      r3, #0
0039436c str      r3, [r4, #0x2d8]
00394370 str      r5, [r4, #0x2d8]
00394374 pop      {r4, r5, r6, pc}
