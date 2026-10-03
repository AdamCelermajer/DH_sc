
# _ZN12CharAnimator17ANIM_SkipNextStepEv
003c9464: ldr      r1, [r0, #0x2c]
003c9468: b        #0x3c9444

# _ZN12CharAnimator13ANIM_StopLoopEb
003c948c: ldrb     r3, [r0, #0x48]
003c9490: cmp      r3, #0
003c9494: bxne     lr
003c9498: ldr      r2, [r0, #0x2c]
003c949c: cmp      r1, #0
003c94a0: mov      r1, #0xc
003c94a4: mla      r2, r1, r2, r0
003c94a8: str      r3, [r2, #0xc]
003c94ac: movne    r3, #1
003c94b0: strbne   r3, [r0, #0x4a]
003c94b4: bx       lr

# _ZNK12CharAnimator17ANIM_GetStepCountEv
003c934c: ldrb     r2, [r0, #0x48]
003c9350: ldr      r3, [pc, #0x38]
003c9354: cmp      r2, #0
003c9358: add      r3, pc, r3
003c935c: movne    r0, #0
003c9360: bxne     lr
003c9364: ldr      r2, [r0, #0x2c]
003c9368: mov      r1, #0xc
003c936c: mla      r0, r1, r2, r0
003c9370: ldr      r2, [pc, #0x1c]
003c9374: mov      r1, #0x14
003c9378: ldr      r2, [r3, r2]
003c937c: ldr      r3, [r0, #8]
003c9380: ldr      r2, [r2]
003c9384: mla      r3, r1, r3, r2
003c9388: ldr      r0, [r3, #8]
003c938c: bx       lr
003c9390: subseq   fp, ip, r8, lsr r7
003c9394: andeq    r3, r0, ip, ror ip

# _ZN12CharAnimator12ANIM_SetStepEjj
003c946c: ldrb     r3, [r0, #0x48]
003c9470: cmp      r3, #0
003c9474: moveq    r3, #0xc
003c9478: mlaeq    r0, r3, r2, r0
003c947c: streq    r1, [r0, #0x10]
003c9480: bx       lr

# _ZN12CharAnimator17ANIM_SkipNextStepEj
003c9444: ldrb     r3, [r0, #0x48]
003c9448: cmp      r3, #0
003c944c: moveq    r3, #0xc
003c9450: mlaeq    r0, r3, r1, r0
003c9454: ldreq    r3, [r0, #0x10]
003c9458: addeq    r3, r3, #1
003c945c: streq    r3, [r0, #0x10]
003c9460: bx       lr

# _ZNK12CharAnimator17ANIM_GetStepIndexEv
003c932c: ldrb     r3, [r0, #0x48]
003c9330: cmp      r3, #0
003c9334: ldreq    r3, [r0, #0x2c]
003c9338: moveq    r2, #0xc
003c933c: mvnne    r0, #0
003c9340: mlaeq    r0, r2, r3, r0
003c9344: ldreq    r0, [r0, #0x10]
003c9348: bx       lr

# _ZN12CharAnimator12ANIM_SetStepEj
003c9484: ldr      r2, [r0, #0x2c]
003c9488: b        #0x3c946c
