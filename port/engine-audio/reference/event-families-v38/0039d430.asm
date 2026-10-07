# _ZN14ProjectileTrap8ActivateEv
0039d430 push     {r4, r5, r6, r7, lr}
0039d434 ldr      r6, [r0, #0x2d8]
0039d438 ldr      r5, [pc, #0xdc]
0039d43c sub      sp, sp, #0x24
0039d440 cmp      r6, #0
0039d444 mov      r4, r0
0039d448 add      r5, pc, r5
0039d44c beq      #0x39d4ec
0039d450 mov      r2, #1
0039d454 strb     r2, [r0, #0x3c4]
0039d458 ldr      ip, [r6, #0x38]
0039d45c mov      r3, #0
0039d460 mov      r1, r3
0039d464 mov      r0, ip
0039d468 ldr      ip, [ip]
0039d46c str      r3, [sp]
0039d470 mov      lr, pc
0039d474 ldr      pc, [ip, #0x1c]
0039d478 ldr      r3, [r4, #0x3c0]
0039d47c cmp      r3, #0
0039d480 blt      #0x39d4e4
0039d484 ldr      r2, [pc, #0x94]
0039d488 ldr      r3, [r4]
0039d48c mov      r0, r4
0039d490 ldr      r2, [r5, r2]
0039d494 ldr      r7, [r2]
0039d498 mov      lr, pc
0039d49c ldr      pc, [r3, #0xec]
0039d4a0 ldr      lr, [r4, #0x168]
0039d4a4 ldr      r6, [r4, #0x160]
0039d4a8 ldr      r5, [r4, #0x164]
0039d4ac mov      ip, #0xbf000000
0039d4b0 add      ip, ip, #0x800000
0039d4b4 mov      r1, r0
0039d4b8 str      lr, [sp, #0x1c]
0039d4bc mov      r0, r7
0039d4c0 mov      lr, #1
0039d4c4 add      r2, sp, #0x14
0039d4c8 mov      r3, #0
0039d4cc str      r6, [sp, #0x14]
0039d4d0 str      r5, [sp, #0x18]
0039d4d4 str      lr, [sp]
0039d4d8 str      ip, [sp, #8]
0039d4dc str      ip, [sp, #4]
0039d4e0 bl       #0x36b5d8
0039d4e4 add      sp, sp, #0x24
0039d4e8 pop      {r4, r5, r6, r7, pc}
0039d4ec ldr      r3, [r0, #0x3f0]
0039d4f0 cmp      r3, #0
0039d4f4 beq      #0x39d478
0039d4f8 add      r7, r0, #0x3e0
0039d4fc mov      r0, r7
0039d500 ldr      r1, [r4, #0x3e4]
0039d504 bl       #0x39861c
0039d508 str      r7, [r4, #0x3ec]
0039d50c str      r6, [r4, #0x3f0]
0039d510 str      r7, [r4, #0x3e8]
0039d514 str      r6, [r4, #0x3e4]
0039d518 b        #0x39d478
0039d51c subseq   r7, pc, r8, asr #12
0039d520 andeq    r0, r0, r4, lsr #27
