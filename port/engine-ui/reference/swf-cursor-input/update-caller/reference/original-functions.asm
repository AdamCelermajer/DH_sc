
# _ZN6MenuFX6UpdateEib
007ad88c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007ad890: mov      r4, r0
007ad894: mov      r5, r1
007ad898: bl       #0x7ad68c
007ad89c: ldr      r8, [r4, #0x118]
007ad8a0: cmp      r8, #0
007ad8a4: ble      #0x7ad8cc
007ad8a8: ldr      r3, [r4, #0x114]
007ad8ac: sub      r8, r8, #1
007ad8b0: mov      r1, r5
007ad8b4: ldr      r3, [r3, r8, lsl #2]
007ad8b8: mov      r0, r3
007ad8bc: ldr      r3, [r3]
007ad8c0: mov      lr, pc
007ad8c4: ldr      pc, [r3, #0x20]
007ad8c8: ldr      r8, [r4, #0x118]
007ad8cc: subs     r7, r8, #2
007ad8d0: bmi      #0x7ad980
007ad8d4: lsl      r7, r7, #2
007ad8d8: mov      sl, #0
007ad8dc: b        #0x7ad8f8
007ad8e0: ldrb     r3, [r2, #0x9b]
007ad8e4: cmp      r3, #0
007ad8e8: bne      #0x7ad958
007ad8ec: cmp      r8, #1
007ad8f0: sub      r7, r7, #4
007ad8f4: beq      #0x7ad980
007ad8f8: ldr      r3, [r4, #0x114]
007ad8fc: sub      r8, r8, #1
007ad900: ldr      r6, [r3, r7]
007ad904: ldr      r2, [r6, #0x4c]
007ad908: cmp      r2, #0
007ad90c: beq      #0x7ad8e0
007ad910: ldr      r3, [r6, #0x48]
007ad914: ldrb     r1, [r3, #4]
007ad918: cmp      r1, #0
007ad91c: bne      #0x7ad8e0
007ad920: ldr      r2, [r3]
007ad924: mov      r0, r3
007ad928: sub      r2, r2, #1
007ad92c: cmp      r2, #0
007ad930: mov      r1, r2
007ad934: str      r2, [r3]
007ad938: bne      #0x7ad940
007ad93c: bl       #0x752b38
007ad940: str      sl, [r6, #0x4c]
007ad944: str      sl, [r6, #0x48]
007ad948: mov      r2, sl
007ad94c: ldrb     r3, [r2, #0x9b]
007ad950: cmp      r3, #0
007ad954: beq      #0x7ad8ec
007ad958: ldr      r3, [r4, #0x114]
007ad95c: mov      r1, r5
007ad960: ldr      r3, [r3, r7]
007ad964: sub      r7, r7, #4
007ad968: mov      r0, r3
007ad96c: ldr      r3, [r3]
007ad970: mov      lr, pc
007ad974: ldr      pc, [r3, #0x28]
007ad978: cmp      r8, #1
007ad97c: bne      #0x7ad8f8
007ad980: ldr      r2, [r4, #0x108]
007ad984: cmp      r2, #0
007ad988: ble      #0x7ada04
007ad98c: mov      r5, #0
007ad990: mov      r8, r5
007ad994: b        #0x7ad9a4
007ad998: add      r5, r5, #1
007ad99c: cmp      r5, r2
007ad9a0: bge      #0x7ada00
007ad9a4: ldr      r3, [r4, #0x104]
007ad9a8: lsl      r7, r5, #2
007ad9ac: ldr      r6, [r3, r5, lsl #2]
007ad9b0: ldr      r3, [r6, #0x58]
007ad9b4: cmp      r3, #2
007ad9b8: bne      #0x7ad998
007ad9bc: ldr      r3, [r6, #0x4c]
007ad9c0: cmp      r3, #0
007ad9c4: beq      #0x7ad9d8
007ad9c8: ldr      r0, [r6, #0x48]
007ad9cc: ldrb     r2, [r0, #4]
007ad9d0: cmp      r2, #0
007ad9d4: beq      #0x7ada08
007ad9d8: mov      r0, r3
007ad9dc: ldr      r3, [r3]
007ad9e0: mov      lr, pc
007ad9e4: ldr      pc, [r3, #0x98]
007ad9e8: cmp      r0, #1
007ad9ec: beq      #0x7ada44
007ad9f0: ldr      r2, [r4, #0x108]
007ad9f4: add      r5, r5, #1
007ad9f8: cmp      r5, r2
007ad9fc: blt      #0x7ad9a4
007ada00: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007ada04: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007ada08: ldr      r1, [r0]
007ada0c: sub      r1, r1, #1
007ada10: cmp      r1, #0
007ada14: str      r1, [r0]
007ada18: bne      #0x7ada20
007ada1c: bl       #0x752b38
007ada20: str      r8, [r6, #0x4c]
007ada24: str      r8, [r6, #0x48]
007ada28: mov      r3, r8
007ada2c: mov      r0, r3
007ada30: ldr      r3, [r3]
007ada34: mov      lr, pc
007ada38: ldr      pc, [r3, #0x98]
007ada3c: cmp      r0, #1
007ada40: bne      #0x7ad9f0
007ada44: ldr      r3, [r4, #0x104]
007ada48: ldr      r6, [r3, r7]
007ada4c: ldr      r3, [r6, #0x4c]
007ada50: cmp      r3, #0
007ada54: beq      #0x7ada7c
007ada58: ldr      r2, [r6, #0x48]
007ada5c: ldrb     sl, [r2, #4]
007ada60: cmp      sl, #0
007ada64: bne      #0x7ada7c
007ada68: add      r0, r6, #0x48
007ada6c: mov      r1, sl
007ada70: bl       #0x41fe84
007ada74: str      sl, [r6, #0x4c]
007ada78: mov      r3, sl
007ada7c: ldrb     r3, [r3, #0x9b]
007ada80: cmp      r3, #0
007ada84: beq      #0x7ad9f0
007ada88: ldr      r3, [r4, #0x74]
007ada8c: cmp      r3, #0
007ada90: bne      #0x7ad9f0
007ada94: ldr      r3, [r4, #0x9c]
007ada98: cmp      r3, #0
007ada9c: bne      #0x7ad9f0
007adaa0: ldr      r3, [r4, #0xc4]
007adaa4: cmp      r3, #0
007adaa8: bne      #0x7ad9f0
007adaac: ldr      r3, [r4, #0xec]
007adab0: cmp      r3, #0
007adab4: bne      #0x7ad9f0
007adab8: ldr      r3, [r4, #0x104]
007adabc: ldr      r6, [r3, r7]
007adac0: ldr      r3, [r6, #0x4c]
007adac4: cmp      r3, #0
007adac8: beq      #0x7adadc
007adacc: ldr      r0, [r6, #0x48]
007adad0: ldrb     r2, [r0, #4]
007adad4: cmp      r2, #0
007adad8: beq      #0x7adae8
007adadc: strb     r8, [r3, #0x9b]
007adae0: ldr      r2, [r4, #0x108]
007adae4: b        #0x7ad9f4
007adae8: ldr      r1, [r0]
007adaec: sub      r1, r1, #1
007adaf0: cmp      r1, #0
007adaf4: str      r1, [r0]
007adaf8: bne      #0x7adb00
007adafc: bl       #0x752b38
007adb00: mov      r3, r8
007adb04: str      r8, [r6, #0x4c]
007adb08: str      r8, [r6, #0x48]
007adb0c: strb     r8, [r3, #0x9b]
007adb10: b        #0x7adae0
