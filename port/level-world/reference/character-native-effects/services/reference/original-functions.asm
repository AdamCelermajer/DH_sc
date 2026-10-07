
# _ZN6Random9GetRandomEib.clone.1
003c26a0: push     {r4, lr}
003c26a4: ldr      r4, [pc, #0x7c]
003c26a8: cmp      r0, #0
003c26ac: add      r4, pc, r4
003c26b0: beq      #0x3c2710
003c26b4: ldr      r2, [pc, #0x70]
003c26b8: mov      r1, r0
003c26bc: movw     r0, #0xe6ab
003c26c0: ldr      r2, [r4, r2]
003c26c4: movw     r3, #0xdb17
003c26c8: movt     r3, #0x2b52
003c26cc: ldr      lr, [r2]
003c26d0: movw     ip, #0xf26b
003c26d4: movt     ip, #0xda
003c26d8: mul      r0, r0, lr
003c26dc: add      r0, r0, #0x2b000
003c26e0: add      r0, r0, #0x3fc
003c26e4: add      r0, r0, #1
003c26e8: umull    lr, r3, r3, r0
003c26ec: rsb      lr, r3, r0
003c26f0: add      r3, r3, lr, lsr #1
003c26f4: lsr      r3, r3, #0x17
003c26f8: mls      r3, ip, r3, r0
003c26fc: mov      r0, r3
003c2700: str      r3, [r2]
003c2704: bl       #0x30eb2c
003c2708: eor      r0, r1, r1, asr #31
003c270c: sub      r0, r0, r1, asr #31
003c2710: ldr      r3, [pc, #0x18]
003c2714: ldr      r3, [r4, r3]
003c2718: ldr      r2, [r3]
003c271c: add      r2, r2, #1
003c2720: str      r2, [r3]
003c2724: pop      {r4, pc}
003c2728: subseq   r2, sp, r4, ror #7
003c272c: muleq    r0, r4, ip
003c2730: andeq    r1, r0, r8, lsl #1

# _ZN16CharStateMachine15SM_SetIdleStateEb
003c1a00: strb     r1, [r0, #0x3c]
003c1a04: mvn      r2, #0
003c1a08: mov      r1, #3
003c1a0c: mov      r3, #0
003c1a10: b        #0x3c1938

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

# _ZN12CharAnimator8ANIM_SetEi
003cacb0: ldrb     r2, [r0, #0x49]
003cacb4: cmp      r2, #0
003cacb8: strne    r1, [r0, #0x50]
003cacbc: bxne     lr
003cacc0: mov      ip, #0x3f800000
003cacc4: str      ip, [r0, #0x40]
003cacc8: b        #0x3cab38

# _ZNK9Character9GetCharAIEv
003a3024: ldr      r3, [pc, #0x20]
003a3028: ldr      r2, [pc, #0x20]
003a302c: push     {r4, lr}
003a3030: add      r3, pc, r3
003a3034: ldr      r2, [r3, r2]
003a3038: ldr      r4, [r2]
003a303c: bl       #0x3a2fec
003a3040: mov      r3, #0x44
003a3044: mla      r0, r3, r0, r4
003a3048: pop      {r4, pc}
003a304c: subseq   r1, pc, r0, ror #20
003a3050: andeq    r0, r0, r8, asr r7

# _ZN12v2Controller15Cmd_HeadTowardsERK7Point3DIfE
00405374: push     {r4, lr}
00405378: ldrb     r2, [r0, #9]
0040537c: ldr      r3, [pc, #0x44]
00405380: cmp      r2, #0
00405384: add      r3, pc, r3
00405388: bne      #0x4053b0
0040538c: ldr      r2, [pc, #0x38]
00405390: ldr      r3, [r3, r2]
00405394: ldrb     r3, [r3]
00405398: cmp      r3, #0
0040539c: beq      #0x4053a4
004053a0: pop      {r4, pc}
004053a4: ldrb     r3, [r0, #8]
004053a8: cmp      r3, #0
004053ac: bne      #0x4053a0
004053b0: ldr      r3, [r0, #4]
004053b4: mov      r0, r3
004053b8: ldr      r3, [r3]
004053bc: mov      lr, pc
004053c0: ldr      pc, [r3, #0x1c]
004053c4: pop      {r4, pc}
004053c8: subseq   pc, r8, ip, lsl #14
004053cc: andeq    r3, r0, r0, asr r6

# _ZN16CharStateMachine16SM_RegisterEventEiiiM9CharacterFbiPviRiE
003c7b18: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c7b1c: ldr      r4, [r0, #0xc]
003c7b20: sub      sp, sp, #0x30
003c7b24: mov      r5, r2
003c7b28: cmp      r4, #0
003c7b2c: mov      sl, r3
003c7b30: ldr      r7, [sp, #0x50]
003c7b34: ldr      r8, [sp, #0x54]
003c7b38: add      r0, r0, #8
003c7b3c: beq      #0x3c7c48
003c7b40: mov      r2, r0
003c7b44: b        #0x3c7b4c
003c7b48: mov      r4, r3
003c7b4c: ldr      r3, [r4, #0x10]
003c7b50: cmp      r1, r3
003c7b54: ldrgt    r3, [r4, #0xc]
003c7b58: ldrle    r3, [r4, #8]
003c7b5c: movgt    r4, r2
003c7b60: mov      r2, r4
003c7b64: cmp      r3, #0
003c7b68: bne      #0x3c7b48
003c7b6c: cmp      r0, r4
003c7b70: beq      #0x3c7c40
003c7b74: ldr      r3, [r4, #0x10]
003c7b78: cmp      r1, r3
003c7b7c: blt      #0x3c7c48
003c7b80: cmp      r0, r4
003c7b84: beq      #0x3c7c40
003c7b88: ldr      ip, [r4, #0x20]
003c7b8c: add      r6, r4, #0x1c
003c7b90: cmp      ip, #0
003c7b94: moveq    ip, r6
003c7b98: beq      #0x3c7bc8
003c7b9c: mov      r2, r6
003c7ba0: b        #0x3c7ba8
003c7ba4: mov      ip, r3
003c7ba8: ldr      r3, [ip, #0x10]
003c7bac: cmp      r3, r5
003c7bb0: ldrlt    r3, [ip, #0xc]
003c7bb4: ldrge    r3, [ip, #8]
003c7bb8: movlt    ip, r2
003c7bbc: mov      r2, ip
003c7bc0: cmp      r3, #0
003c7bc4: bne      #0x3c7ba4
003c7bc8: cmp      r6, ip
003c7bcc: beq      #0x3c7c88
003c7bd0: ldr      r2, [ip, #0x10]
003c7bd4: mov      r3, ip
003c7bd8: cmp      r2, r5
003c7bdc: bgt      #0x3c7c88
003c7be0: str      r7, [r3, #0x14]
003c7be4: str      r8, [r3, #0x18]
003c7be8: ldr      ip, [r4, #0x20]
003c7bec: cmp      ip, #0
003c7bf0: moveq    ip, r6
003c7bf4: beq      #0x3c7c24
003c7bf8: mov      r2, r6
003c7bfc: b        #0x3c7c04
003c7c00: mov      ip, r3
003c7c04: ldr      r3, [ip, #0x10]
003c7c08: cmp      r3, r5
003c7c0c: ldrlt    r3, [ip, #0xc]
003c7c10: ldrge    r3, [ip, #8]
003c7c14: movlt    ip, r2
003c7c18: mov      r2, ip
003c7c1c: cmp      r3, #0
003c7c20: bne      #0x3c7c00
003c7c24: cmp      r6, ip
003c7c28: beq      #0x3c7c50
003c7c2c: ldr      r2, [ip, #0x10]
003c7c30: mov      r3, ip
003c7c34: cmp      r2, r5
003c7c38: bgt      #0x3c7c50
003c7c3c: str      sl, [r3, #0x1c]
003c7c40: add      sp, sp, #0x30
003c7c44: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c7c48: mov      r4, r0
003c7c4c: b        #0x3c7b80
003c7c50: mov      lr, #0
003c7c54: mov      r3, sp
003c7c58: mov      r1, r6
003c7c5c: add      r0, sp, #0x20
003c7c60: add      r2, sp, #0x24
003c7c64: mvn      r4, #0
003c7c68: str      r5, [sp]
003c7c6c: str      r4, [sp, #0xc]
003c7c70: str      lr, [sp, #4]
003c7c74: str      ip, [sp, #0x24]
003c7c78: str      lr, [sp, #8]
003c7c7c: bl       #0x3c77a4
003c7c80: ldr      r3, [sp, #0x20]
003c7c84: b        #0x3c7c3c
003c7c88: mov      lr, #0
003c7c8c: add      r3, sp, #0x10
003c7c90: add      r0, sp, #0x28
003c7c94: mov      r1, r6
003c7c98: add      r2, sp, #0x2c
003c7c9c: mvn      sb, #0
003c7ca0: str      sb, [sp, #0x1c]
003c7ca4: str      lr, [sp, #0x14]
003c7ca8: str      ip, [sp, #0x2c]
003c7cac: str      r5, [sp, #0x10]
003c7cb0: str      lr, [sp, #0x18]
003c7cb4: bl       #0x3c77a4
003c7cb8: ldr      r3, [sp, #0x28]
003c7cbc: b        #0x3c7be0

# _ZN12v2Controller15Cmd_HeadTowardsEP10GameObject
004053d0: push     {r4, lr}
004053d4: ldrb     r2, [r0, #9]
004053d8: ldr      r3, [pc, #0x44]
004053dc: cmp      r2, #0
004053e0: add      r3, pc, r3
004053e4: bne      #0x40540c
004053e8: ldr      r2, [pc, #0x38]
004053ec: ldr      r3, [r3, r2]
004053f0: ldrb     r3, [r3]
004053f4: cmp      r3, #0
004053f8: beq      #0x405400
004053fc: pop      {r4, pc}
00405400: ldrb     r3, [r0, #8]
00405404: cmp      r3, #0
00405408: bne      #0x4053fc
0040540c: ldr      r3, [r0, #4]
00405410: mov      r0, r3
00405414: ldr      r3, [r3]
00405418: mov      lr, pc
0040541c: ldr      pc, [r3, #0x20]
00405420: pop      {r4, pc}
00405424: ldrheq   pc, [r8], #-0x60
00405428: andeq    r3, r0, r0, asr r6
