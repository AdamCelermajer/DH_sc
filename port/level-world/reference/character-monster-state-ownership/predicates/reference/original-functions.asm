
# _ZN9Character10CSM_AttackEiPviRi
003ad22c: ldr      r0, [r0, #0x528]
003ad230: and      r0, r0, #1
003ad234: eor      r0, r0, #1
003ad238: bx       lr

# _ZN9Character22CSM_CanStopInteractionEiPviRi
003ad29c: ldr      r3, [r0, #0x544]
003ad2a0: sub      r2, r3, #4
003ad2a4: cmp      r2, #1
003ad2a8: movhi    r0, #1
003ad2ac: bxhi     lr
003ad2b0: ldr      r2, [pc, #0xc]
003ad2b4: add      r2, pc, r2
003ad2b8: add      r3, r2, r3
003ad2bc: ldrb     r0, [r3, #-4]
003ad2c0: bx       lr
003ad2c4: subseq   r6, r1, r8, asr #9

# _ZN9Character9CSM_SpawnEiPviRi
003ad2e4: cmp      r3, #0
003ad2e8: push     {r4, lr}
003ad2ec: mov      r4, r0
003ad2f0: bne      #0x3ad2fc
003ad2f4: pop      {r4, lr}
003ad2f8: b        #0x3a5248
003ad2fc: cmp      r3, #0x11
003ad300: beq      #0x3ad30c
003ad304: mov      r0, #1
003ad308: pop      {r4, pc}
003ad30c: ldr      r0, [r0, #0x3fc]
003ad310: cmp      r0, #0
003ad314: beq      #0x3ad328
003ad318: ldr      r1, [r4, #0x3cc]
003ad31c: bl       #0x3d24fc
003ad320: cmp      r0, #0
003ad324: beq      #0x3ad330
003ad328: movw     r3, #0x1430
003ad32c: ldrb     r0, [r4, r3]
003ad330: pop      {r4, pc}

# _ZN9Character11CSM_InjuredEiPviRi
003ad23c: mov      r0, #1
003ad240: bx       lr

# _ZN9Character20CSM_StoppedAttackingEiPviRi
003ad280: cmp      r3, #5
003ad284: movne    r0, #0
003ad288: ldrbeq   r0, [r0, #0x442]
003ad28c: bx       lr

# _ZN9Character15CSM_InterruptedEiPviRi
003ad244: cmp      r3, #6
003ad248: beq      #0x3ad264
003ad24c: cmp      r3, #0xa
003ad250: beq      #0x3ad270
003ad254: cmp      r3, #5
003ad258: movne    r0, #1
003ad25c: ldrbeq   r0, [r0, #0x441]
003ad260: bx       lr
003ad264: ldr      r0, [r0, #0x520]
003ad268: ubfx     r0, r0, #0x10, #1
003ad26c: bx       lr
003ad270: ldr      r0, [r0, #0x528]
003ad274: eor      r0, r0, #0x20
003ad278: ubfx     r0, r0, #5, #1
003ad27c: bx       lr

# _ZN9Character13CSM_StopSkillEiPviRi
003ad290: ldr      r0, [r0, #0x520]
003ad294: ubfx     r0, r0, #0xf, #1
003ad298: bx       lr

# _ZN9Character20CSM_AfterInteractionEiPviRi
003ad2c8: ldr      r3, [r0, #0x544]
003ad2cc: mov      r0, #1
003ad2d0: cmp      r3, #4
003ad2d4: ldreq    r3, [sp]
003ad2d8: moveq    r2, #0x12
003ad2dc: streq    r2, [r3]
003ad2e0: bx       lr
