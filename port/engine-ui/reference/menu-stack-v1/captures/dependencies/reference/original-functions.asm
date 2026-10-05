
# _ZN6MenuFX8GetStateEPKc
007a82c4: push     {r4, r5, r6, r7, r8, lr}
007a82c8: ldr      r6, [r0, #0x108]
007a82cc: mov      r7, r1
007a82d0: cmp      r6, #0
007a82d4: ble      #0x7a8310
007a82d8: ldr      r8, [r0, #0x104]
007a82dc: mov      r4, #0
007a82e0: b        #0x7a82ec
007a82e4: cmp      r4, r6
007a82e8: beq      #0x7a8310
007a82ec: ldr      r5, [r8, r4, lsl #2]
007a82f0: mov      r1, r7
007a82f4: add      r4, r4, #1
007a82f8: add      r0, r5, #8
007a82fc: bl       #0x30e31c
007a8300: cmp      r0, #0
007a8304: bne      #0x7a82e4
007a8308: mov      r0, r5
007a830c: pop      {r4, r5, r6, r7, r8, pc}
007a8310: mov      r5, #0
007a8314: mov      r0, r5
007a8318: pop      {r4, r5, r6, r7, r8, pc}

# _ZN11MenuManager16RegisterListenerEP8MenuBase
00431750: push     {r4, lr}
00431754: ldr      r2, [r0, #0x74]
00431758: mov      r3, r1
0043175c: sub      sp, sp, #0x10
00431760: cmp      r2, #0
00431764: add      r1, r0, #0x70
00431768: beq      #0x4317c4
0043176c: mov      r4, r1
00431770: b        #0x431778
00431774: mov      r2, ip
00431778: ldr      ip, [r2, #0x10]
0043177c: cmp      r3, ip
00431780: ldrhi    ip, [r2, #0xc]
00431784: ldrls    ip, [r2, #8]
00431788: movhi    r2, r4
0043178c: mov      r4, r2
00431790: cmp      ip, #0
00431794: bne      #0x431774
00431798: cmp      r1, r2
0043179c: beq      #0x4317cc
004317a0: ldr      r0, [r2, #0x10]
004317a4: cmp      r3, r0
004317a8: blo      #0x4317c4
004317ac: cmp      r1, r2
004317b0: movne    r3, #1
004317b4: strbne   r3, [r2, #0x14]
004317b8: beq      #0x4317cc
004317bc: add      sp, sp, #0x10
004317c0: pop      {r4, pc}
004317c4: mov      r2, r1
004317c8: b        #0x4317ac
004317cc: str      r3, [sp, #8]
004317d0: mov      r0, sp
004317d4: mov      r3, #1
004317d8: add      r2, sp, #8
004317dc: strb     r3, [sp, #0xc]
004317e0: bl       #0x4315d0
004317e4: b        #0x4317bc

# _ZN11MenuManager18UnRegisterListenerEP8MenuBase
0042e110: push     {r4, r5, r6, r7, r8, sl, lr}
0042e114: ldr      r5, [pc, #0xdc]
0042e118: ldr      r7, [pc, #0xdc]
0042e11c: ldr      r2, [pc, #0xdc]
0042e120: add      r5, pc, r5
0042e124: ldr      r3, [r5, r7]
0042e128: ldr      sl, [r5, r2]
0042e12c: sub      sp, sp, #0x24
0042e130: ldr      r3, [r3]
0042e134: mov      r8, r0
0042e138: mov      r0, sl
0042e13c: str      r3, [sp, #0x1c]
0042e140: mov      r4, r1
0042e144: bl       #0x337888
0042e148: ldr      r1, [pc, #0xb4]
0042e14c: add      r6, sp, #4
0042e150: mov      r2, sp
0042e154: add      r1, pc, r1
0042e158: mov      r0, r6
0042e15c: bl       #0x3140ec
0042e160: mov      r1, r6
0042e164: mov      r0, sl
0042e168: bl       #0x337a88
0042e16c: mov      r0, r6
0042e170: bl       #0x318254
0042e174: ldr      r3, [r8, #0x74]
0042e178: add      r8, r8, #0x70
0042e17c: cmp      r3, #0
0042e180: beq      #0x42e1ec
0042e184: mov      r1, r8
0042e188: b        #0x42e194
0042e18c: mov      r1, r3
0042e190: mov      r3, r2
0042e194: ldr      r2, [r3, #0x10]
0042e198: cmp      r4, r2
0042e19c: ldrhi    r2, [r3, #0xc]
0042e1a0: ldrls    r2, [r3, #8]
0042e1a4: movhi    r3, r1
0042e1a8: cmp      r2, #0
0042e1ac: bne      #0x42e18c
0042e1b0: cmp      r8, r3
0042e1b4: beq      #0x42e1d0
0042e1b8: ldr      r2, [r3, #0x10]
0042e1bc: cmp      r4, r2
0042e1c0: blo      #0x42e1ec
0042e1c4: cmp      r8, r3
0042e1c8: movne    r2, #0
0042e1cc: strbne   r2, [r3, #0x14]
0042e1d0: ldr      r3, [r5, r7]
0042e1d4: ldr      r2, [sp, #0x1c]
0042e1d8: ldr      r3, [r3]
0042e1dc: cmp      r2, r3
0042e1e0: bne      #0x42e1f4
0042e1e4: add      sp, sp, #0x24
0042e1e8: pop      {r4, r5, r6, r7, r8, sl, pc}
0042e1ec: mov      r3, r8
0042e1f0: b        #0x42e1c4
0042e1f4: bl       #0x30e310
0042e1f8: subseq   r6, r6, r0, ror sb
0042e1fc: andeq    r4, r0, ip, lsr #1
0042e200: andeq    r0, r0, r4, lsl #17
0042e204: subeq    fp, sb, r4, lsl #31

# _ZN8RenderFX10SetContextEPN7gameswf9characterE
007a7ee8: str      r1, [r0, #0x40]
007a7eec: bx       lr

# _ZN8MenuBase10SetVisibleEb
004223bc: push     {r4, r5, r6, lr}
004223c0: ldr      r3, [r0]
004223c4: mov      r4, r0
004223c8: mov      r5, r1
004223cc: mov      lr, pc
004223d0: ldr      pc, [r3, #0x3c]
004223d4: cmp      r0, #0
004223d8: beq      #0x422400
004223dc: ldr      r3, [r4, #0x4c]
004223e0: cmp      r3, #0
004223e4: beq      #0x4223f8
004223e8: ldr      r0, [r4, #0x48]
004223ec: ldrb     r2, [r0, #4]
004223f0: cmp      r2, #0
004223f4: beq      #0x422404
004223f8: strb     r5, [r3, #0x9b]
004223fc: strb     r5, [r4, #0x74]
00422400: pop      {r4, r5, r6, pc}
00422404: ldr      r1, [r0]
00422408: sub      r1, r1, #1
0042240c: cmp      r1, #0
00422410: str      r1, [r0]
00422414: bne      #0x42241c
00422418: bl       #0x752b38
0042241c: mov      r3, #0
00422420: str      r3, [r4, #0x48]
00422424: str      r3, [r4, #0x4c]
00422428: b        #0x4223f8

# _ZNK7gameswf8weak_ptrINS_9characterEE7get_ptrEv
00438224: push     {r4, lr}
00438228: mov      r4, r0
0043822c: ldr      r0, [r0, #4]
00438230: cmp      r0, #0
00438234: beq      #0x438248
00438238: ldr      r3, [r4]
0043823c: ldrb     r2, [r3, #4]
00438240: cmp      r2, #0
00438244: beq      #0x43824c
00438248: pop      {r4, pc}
0043824c: ldr      r1, [r3]
00438250: sub      r1, r1, #1
00438254: cmp      r1, #0
00438258: str      r1, [r3]
0043825c: bne      #0x438268
00438260: mov      r0, r3
00438264: bl       #0x752b38
00438268: mov      r0, #0
0043826c: str      r0, [r4, #4]
00438270: str      r0, [r4]
00438274: pop      {r4, pc}

# _ZNK8MenuBase9IsVisibleEv
0041f3f4: ldrb     r0, [r0, #0x74]
0041f3f8: bx       lr

# _ZNK7gameswf8weak_ptrINS_9characterEEptEv
004381d0: push     {r4, lr}
004381d4: mov      r4, r0
004381d8: ldr      r0, [r0, #4]
004381dc: cmp      r0, #0
004381e0: beq      #0x4381f4
004381e4: ldr      r3, [r4]
004381e8: ldrb     r2, [r3, #4]
004381ec: cmp      r2, #0
004381f0: beq      #0x4381f8
004381f4: pop      {r4, pc}
004381f8: ldr      r1, [r3]
004381fc: sub      r1, r1, #1
00438200: cmp      r1, #0
00438204: str      r1, [r3]
00438208: bne      #0x438214
0043820c: mov      r0, r3
00438210: bl       #0x752b38
00438214: mov      r0, #0
00438218: str      r0, [r4, #4]
0043821c: str      r0, [r4]
00438220: pop      {r4, pc}

# _ZN16MultiMenuManagerC1Ev
00437e24: push     {r4, r5, r6, lr}
00437e28: ldr      r5, [pc, #0x6c]
00437e2c: mov      r4, r0
00437e30: bl       #0x7a8750
00437e34: ldr      r2, [pc, #0x64]
00437e38: add      r5, pc, r5
00437e3c: mov      r3, #0
00437e40: ldr      r2, [r5, r2]
00437e44: str      r3, [r4, #0x150]
00437e48: str      r3, [r4, #0x124]
00437e4c: add      r1, r2, #0x54
00437e50: add      r2, r2, #8
00437e54: str      r2, [r4]
00437e58: str      r1, [r4, #0x100]
00437e5c: str      r3, [r4, #0x128]
00437e60: str      r3, [r4, #0x12c]
00437e64: strb     r3, [r4, #0x130]
00437e68: str      r3, [r4, #0x154]
00437e6c: str      r3, [r4, #0x158]
00437e70: str      r3, [r4, #0x15c]
00437e74: strb     r3, [r4, #0x160]
00437e78: str      r3, [r4, #0x134]
00437e7c: str      r3, [r4, #0x144]
00437e80: str      r3, [r4, #0x138]
00437e84: str      r3, [r4, #0x148]
00437e88: str      r3, [r4, #0x13c]
00437e8c: str      r3, [r4, #0x14c]
00437e90: str      r3, [r4, #0x140]
00437e94: mov      r0, r4
00437e98: pop      {r4, r5, r6, pc}
00437e9c: subseq   ip, r5, r8, asr ip
00437ea0: andeq    r2, r0, r4, lsr r1

# _ZN6MenuFX15GetCurrentStateEv
007a7f30: ldr      r3, [r0, #0x118]
007a7f34: cmp      r3, #0
007a7f38: ldrgt    r2, [r0, #0x114]
007a7f3c: subgt    r3, r3, #1
007a7f40: movle    r0, #0
007a7f44: ldrgt    r0, [r2, r3, lsl #2]
007a7f48: bx       lr

# _ZN16MultiMenuManager14IsStateInStackEPN6MenuFX5StateE
00437924: push     {r4, r5, r6}
00437928: ldr      r3, [r0, #0x128]
0043792c: cmp      r3, #0
00437930: ble      #0x437988
00437934: ldr      r6, [r0, #0x124]
00437938: mov      r5, #0
0043793c: ldr      r2, [r6, r5, lsl #2]
00437940: ldr      ip, [r2, #0x118]
00437944: cmp      ip, #0
00437948: ble      #0x43797c
0043794c: ldr      r4, [r2, #0x114]
00437950: ldr      r2, [r4]
00437954: cmp      r2, r1
00437958: beq      #0x437990
0043795c: mov      r2, #0
00437960: b        #0x437970
00437964: ldr      r0, [r4, r2, lsl #2]
00437968: cmp      r0, r1
0043796c: beq      #0x437990
00437970: add      r2, r2, #1
00437974: cmp      r2, ip
00437978: bne      #0x437964
0043797c: add      r5, r5, #1
00437980: cmp      r5, r3
00437984: bne      #0x43793c
00437988: mov      r0, #0
0043798c: b        #0x437994
00437990: mov      r0, #1
00437994: pop      {r4, r5, r6}
00437998: bx       lr
