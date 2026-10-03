
# _ZN6CharAI11_SpellEventEv
003d8ba4: push     {r4, lr}
003d8ba8: mvn      r1, #0
003d8bac: mov      r4, r0
003d8bb0: ldr      r0, [r0, #4]
003d8bb4: bl       #0x3bb98c
003d8bb8: ldr      r3, [r4, #0xc0]
003d8bbc: ldr      r2, [r4, #0xc4]
003d8bc0: rsb      r2, r3, r2
003d8bc4: cmp      r0, r2, asr #2
003d8bc8: bhs      #0x3d8bf4
003d8bcc: ldr      r3, [r3, r0, lsl #2]
003d8bd0: cmp      r3, #0
003d8bd4: beq      #0x3d8bf4
003d8bd8: ldr      r0, [r4, #4]
003d8bdc: mvn      r1, #0
003d8be0: bl       #0x3bb98c
003d8be4: ldr      r3, [r4, #0xc0]
003d8be8: ldr      r0, [r3, r0, lsl #2]
003d8bec: pop      {r4, lr}
003d8bf0: b        #0x3da794
003d8bf4: pop      {r4, pc}

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

# _ZN6CharAI11_SkillEventEv
003d8bf8: ldr      r1, [r0, #0xb8]
003d8bfc: ldr      r3, [r0, #0xb4]
003d8c00: ldr      r2, [r0, #0xcc]
003d8c04: rsb      r1, r3, r1
003d8c08: cmp      r2, r1, asr #2
003d8c0c: bxhs     lr
003d8c10: ldr      r0, [r3, r2, lsl #2]
003d8c14: cmp      r0, #0
003d8c18: bxeq     lr
003d8c1c: b        #0x3da794

# _ZNK12CharAnimator17ANIM_GetStepIndexEv
003c932c: ldrb     r3, [r0, #0x48]
003c9330: cmp      r3, #0
003c9334: ldreq    r3, [r0, #0x2c]
003c9338: moveq    r2, #0xc
003c933c: mvnne    r0, #0
003c9340: mlaeq    r0, r2, r3, r0
003c9344: ldreq    r0, [r0, #0x10]
003c9348: bx       lr
