
# _Z18NativeLoadSettingsRKN7gameswf7fn_callE
0043b2d8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0043b2dc: ldr      r4, [pc, #0x98]
0043b2e0: ldr      r5, [pc, #0x98]
0043b2e4: mov      r1, #0
0043b2e8: add      r4, pc, r4
0043b2ec: ldr      r6, [r4, r5]
0043b2f0: ldr      r0, [r6, #0x4c]
0043b2f4: bl       #0x46e584
0043b2f8: bl       #0x43a8f0
0043b2fc: ldr      r3, [pc, #0x80]
0043b300: ldr      r3, [r4, r3]
0043b304: ldr      r7, [r3]
0043b308: cmp      r7, #0
0043b30c: beq      #0x43b360
0043b310: ldr      r1, [pc, #0x70]
0043b314: mov      r0, r6
0043b318: add      r1, pc, r1
0043b31c: bl       #0x320e44
0043b320: ldr      r1, [pc, #0x64]
0043b324: mov      r8, r0
0043b328: mov      r0, r6
0043b32c: add      r1, pc, r1
0043b330: bl       #0x320e44
0043b334: mov      sl, r0
0043b338: mov      r0, r8
0043b33c: bl       #0x30e964
0043b340: mov      r6, r0
0043b344: mov      r0, sl
0043b348: bl       #0x30e964
0043b34c: mov      r1, r6
0043b350: mov      r2, r0
0043b354: mov      r3, #0
0043b358: mov      r0, r7
0043b35c: bl       #0x369c2c
0043b360: ldr      r4, [r4, r5]
0043b364: ldr      r0, [r4, #0x4c]
0043b368: bl       #0x46d514
0043b36c: mov      r1, r0
0043b370: ldr      r0, [r4, #0x4c]
0043b374: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0043b378: b        #0x46d104
0043b37c: subseq   sb, r5, r8, lsr #15
0043b380: strdeq   r3, r4, [r0], -r4
0043b384: andeq    r0, r0, r4, lsr #27
0043b388: umaaleq  r5, r8, r8, lr
0043b38c: subeq    r5, r8, r4, lsr #25

# _Z26NativeIsMultiplayerEnabledRKN7gameswf7fn_callE
00439f18: ldr      r3, [pc, #0x74]
00439f1c: ldr      r2, [pc, #0x74]
00439f20: push     {r4, lr}
00439f24: add      r3, pc, r3
00439f28: ldr      r2, [r3, r2]
00439f2c: ldrb     r2, [r2]
00439f30: cmp      r2, #0
00439f34: bne      #0x439f84
00439f38: ldr      r2, [pc, #0x5c]
00439f3c: ldr      r2, [r3, r2]
00439f40: ldrb     r1, [r2]
00439f44: cmp      r1, #0
00439f48: bne      #0x439f84
00439f4c: ldr      r2, [pc, #0x4c]
00439f50: ldr      r3, [r3, r2]
00439f54: ldrb     r3, [r3]
00439f58: cmp      r3, #0
00439f5c: bne      #0x439f78
00439f60: ldr      r4, [r0]
00439f64: bl       #0x38174c
00439f68: mov      r1, r0
00439f6c: mov      r0, r4
00439f70: pop      {r4, lr}
00439f74: b        #0x797230
00439f78: ldr      r0, [r0]
00439f7c: pop      {r4, lr}
00439f80: b        #0x797230
00439f84: ldr      r0, [r0]
00439f88: mov      r1, #1
00439f8c: pop      {r4, lr}
00439f90: b        #0x797230
00439f94: subseq   sl, r5, ip, ror #22
00439f98: andeq    r4, r0, r8, asr r4
00439f9c: andeq    r2, r0, r8, ror #14
00439fa0: andeq    r1, r0, r0, ror #13
