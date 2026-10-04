
# _ZN8RenderFX10InitializeERNS_24InitializationParametersE
007a9814: push     {r4, r5, r6, r7, r8, lr}
007a9818: ldr      r4, [pc, #0xbc]
007a981c: ldr      r5, [pc, #0xbc]
007a9820: mov      r6, r0
007a9824: add      r4, pc, r4
007a9828: ldr      r3, [r4, r5]
007a982c: ldr      r3, [r3]
007a9830: cmp      r3, #0
007a9834: beq      #0x7a983c
007a9838: pop      {r4, r5, r6, r7, r8, pc}
007a983c: ldr      r0, [pc, #0xa0]
007a9840: add      r0, pc, r0
007a9844: bl       #0x76c740
007a9848: ldr      r0, [pc, #0x98]
007a984c: add      r0, pc, r0
007a9850: bl       #0x76c7c4
007a9854: bl       #0x759b24
007a9858: cmp      r0, #0
007a985c: bne      #0x7a98cc
007a9860: ldr      r0, [pc, #0x84]
007a9864: add      r0, pc, r0
007a9868: bl       #0x76c7c4
007a986c: ldr      r0, [r6]
007a9870: bl       #0x7d6658
007a9874: ldr      r3, [pc, #0x74]
007a9878: mov      r7, r0
007a987c: ldr      r3, [r4, r3]
007a9880: str      r0, [r3]
007a9884: ldr      r3, [r0]
007a9888: mov      lr, pc
007a988c: ldr      pc, [r3, #0xa0]
007a9890: mov      r0, r7
007a9894: ldr      r3, [r7]
007a9898: mov      r1, #1
007a989c: mov      lr, pc
007a98a0: ldr      pc, [r3, #0x84]
007a98a4: ldr      r0, [r6, #4]
007a98a8: cmp      r0, #0
007a98ac: beq      #0x7a98b4
007a98b0: bl       #0x759cdc
007a98b4: bl       #0x76f5c0
007a98b8: mov      r0, r6
007a98bc: bl       #0x7a9708
007a98c0: ldr      r3, [r4, r5]
007a98c4: str      r0, [r3]
007a98c8: pop      {r4, r5, r6, r7, r8, pc}
007a98cc: ldr      r0, [pc, #0x20]
007a98d0: add      r0, pc, r0
007a98d4: bl       #0x761170
007a98d8: b        #0x7a9860
007a98dc: andseq   fp, lr, ip, ror #4
007a98e0: andeq    r4, r0, r0, lsr sb
007a98e4: andeq    r0, r0, ip, lsl #2

# _ZN8RenderFX10InitializeEPN6glitch5video12IVideoDriverE
007a98f8: push     {r4, lr}
007a98fc: mov      r4, r0
007a9900: ldr      r0, [pc, #0x48]
007a9904: sub      sp, sp, #0x28
007a9908: add      r0, pc, r0
007a990c: bl       #0x7611f0
007a9910: add      r0, sp, #0x28
007a9914: mov      r2, #1
007a9918: mov      r3, #0
007a991c: strb     r2, [sp, #0x20]
007a9920: str      r4, [r0, #-0x24]!
007a9924: mov      r2, #0x3f800000
007a9928: str      r3, [sp, #0x1c]
007a992c: str      r2, [sp, #0x24]
007a9930: str      r3, [sp, #8]
007a9934: str      r3, [sp, #0xc]
007a9938: str      r3, [sp, #0x10]
007a993c: str      r3, [sp, #0x14]
007a9940: str      r3, [sp, #0x18]
007a9944: bl       #0x7a9814
007a9948: add      sp, sp, #0x28
007a994c: pop      {r4, pc}
007a9950: andseq   r0, r6, r0, lsr pc

# _ZN8RenderFX27InitializeGlyphTextureCacheEii
007a8d74: ldr      r0, [pc, #4]
007a8d78: add      r0, pc, r0
007a8d7c: b        #0x7611f0
007a8d80: andseq   r1, r6, r0, lsl #19
