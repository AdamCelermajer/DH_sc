
# _ZN8RenderFX13CreateContextERNS_24InitializationParametersE
007a9708: push     {r4, r5, r6, r7, lr}
007a970c: mov      r1, #0
007a9710: mov      r4, r0
007a9714: sub      sp, sp, #0xc
007a9718: mov      r0, #0x2c
007a971c: bl       #0x752ba8
007a9720: mov      r5, r0
007a9724: bl       #0x76ce38
007a9728: mov      r1, #0
007a972c: mov      r0, #0x2c
007a9730: bl       #0x752ba8
007a9734: ldr      ip, [r4, #0x20]
007a9738: ldr      r2, [r4, #0x10]
007a973c: ldrb     r3, [r4, #0x1c]
007a9740: ldr      r1, [r4, #0xc]
007a9744: mov      r6, r0
007a9748: str      ip, [sp]
007a974c: bl       #0x7d0dfc
007a9750: str      r6, [r5, #0xc]
007a9754: mov      r1, #0
007a9758: mov      r0, #0x10
007a975c: bl       #0x752ba8
007a9760: ldr      r6, [pc, #0x3c]
007a9764: ldrb     r3, [r4, #0x1c]
007a9768: ldr      r1, [r4, #0x14]
007a976c: ldr      r2, [r4, #0x18]
007a9770: mov      r7, r0
007a9774: bl       #0x7a9698
007a9778: ldr      r3, [pc, #0x28]
007a977c: add      r6, pc, r6
007a9780: mov      r0, r5
007a9784: ldr      r3, [r6, r3]
007a9788: add      r3, r3, #8
007a978c: str      r3, [r7]
007a9790: str      r7, [r5, #0x10]
007a9794: ldr      r3, [r4]
007a9798: str      r3, [r5, #0x28]
007a979c: add      sp, sp, #0xc
007a97a0: pop      {r4, r5, r6, r7, pc}
007a97a4: andseq   fp, lr, r4, lsl r3
007a97a8: andeq    r2, r0, r4, ror r5
