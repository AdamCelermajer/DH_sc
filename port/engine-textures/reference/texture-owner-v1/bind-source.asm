_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE10setTextureEjPNS0_8ITextureENS0_14E_TEXTURE_TYPEE
005b26f0: push {r4, r5, r6, r7, r8, lr}
005b26f4: mov r4, r0
005b26f8: ldr r0, [r0, #0x4c]
005b26fc: mov r5, r1
005b2700: mov r6, r2
005b2704: cmp r1, r0
005b2708: mov r7, r3
005b270c: bhs #0x5b277c
005b2710: add r3, r3, #0x21
005b2714: add r3, r4, r3, lsl #5
005b2718: ldr r8, [r3, r1, lsl #2]
005b271c: cmp r8, r2
005b2720: beq #0x5b2784
005b2724: cmp r2, #0
005b2728: str r2, [r3, r5, lsl #2]
005b272c: beq #0x5b27a4
005b2730: ldr r3, [r4, #0x84]
005b2734: ldr r2, [r4, #0x268]
005b2738: add r3, r3, #1
005b273c: cmp r1, r2
005b2740: str r3, [r4, #0x84]
005b2744: beq #0x5b2758
005b2748: add r0, r1, #0x8400
005b274c: add r0, r0, #0xc0
005b2750: bl #0x30e1e4
005b2754: str r5, [r4, #0x268]
005b2758: ldrb r1, [r6, #0x3f]
005b275c: and r1, r1, #8
005b2760: uxtb r1, r1
005b2764: cmp r1, #0
005b2768: bne #0x5b27ac
005b276c: mov r0, r6
005b2770: bl #0x5fde9c
005b2774: mov r0, #1
005b2778: pop {r4, r5, r6, r7, r8, pc}
005b277c: mov r0, #0
005b2780: pop {r4, r5, r6, r7, r8, pc}
005b2784: cmp r8, #0
005b2788: beq #0x5b27a4
005b278c: ldrh r3, [r8, #0x40]
005b2790: bic r3, r3, #2
005b2794: lsl r3, r3, #0x13
005b2798: lsr r3, r3, #0x13
005b279c: cmp r3, #0
005b27a0: bne #0x5b27d8
005b27a4: mov r0, #1
005b27a8: pop {r4, r5, r6, r7, r8, pc}
005b27ac: ldr r3, [pc, #0x54]
005b27b0: ldr r1, [r6, #0x54]
005b27b4: add r3, pc, r3
005b27b8: add r3, r3, #0xa4
005b27bc: ldr r0, [r3, r7, lsl #2]
005b27c0: bl #0x30e7c0
005b27c4: mov r0, r6
005b27c8: mov r1, #0
005b27cc: bl #0x5b044c
005b27d0: mov r0, #1
005b27d4: pop {r4, r5, r6, r7, r8, pc}
005b27d8: ldr r3, [r4, #0x268]
005b27dc: cmp r1, r3
005b27e0: beq #0x5b27f4
005b27e4: add r0, r1, #0x8400
005b27e8: add r0, r0, #0xc0
005b27ec: bl #0x30e1e4
005b27f0: str r5, [r4, #0x268]
005b27f4: mov r0, r8
005b27f8: mov r1, #0
005b27fc: bl #0x5b044c
005b2800: mov r0, #1
005b2804: pop {r4, r5, r6, r7, r8, pc}
005b2808: eorseq sp, r2, r0, lsl #17

_ZN6glitch5video19CCommonGLDriverBase18SCommonShadowStateC1Ev
006dcd90: mov r2, #0
006dcd94: mov r1, #4
006dcd98: str r2, [r0, #0x10]
006dcd9c: str r1, [r0, #0x18]
006dcda0: str r2, [r0, #0x14]
006dcda4: str r2, [r0, #0x1c]
006dcda8: str r2, [r0, #0x20]
006dcdac: str r2, [r0, #0x24]
006dcdb0: str r2, [r0, #0x28]
006dcdb4: str r2, [r0, #0x2c]
006dcdb8: str r2, [r0]
006dcdbc: str r2, [r0, #4]
006dcdc0: str r2, [r0, #8]
006dcdc4: str r2, [r0, #0xc]
006dcdc8: bx lr
