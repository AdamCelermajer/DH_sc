
# _ZN10ObjectBase11setUpdatingEb
0033dcf0: strb     r1, [r0, #0x85]
0033dcf4: bx       lr

# _ZN6CharAI6UpdateEv
003cfbf4: push     {r4, r5, r6, lr}
003cfbf8: mov      r5, r0
003cfbfc: ldr      r0, [pc, #0x140]
003cfc00: ldr      r4, [pc, #0x140]
003cfc04: add      r0, pc, r0
003cfc08: bl       #0x3136b4
003cfc0c: ldrb     r3, [r5, #0x18]
003cfc10: add      r4, pc, r4
003cfc14: cmp      r3, #0
003cfc18: bne      #0x3cfc44
003cfc1c: ldr      r6, [r5, #4]
003cfc20: ldr      r3, [r6, #0x378]
003cfc24: ldrb     r2, [r3, #9]
003cfc28: cmp      r2, #0
003cfc2c: bne      #0x3cfc60
003cfc30: ldr      r2, [pc, #0x114]
003cfc34: ldr      r2, [r4, r2]
003cfc38: ldrb     r2, [r2]
003cfc3c: cmp      r2, #0
003cfc40: beq      #0x3cfc54
003cfc44: ldr      r0, [pc, #0x104]
003cfc48: add      r0, pc, r0
003cfc4c: pop      {r4, r5, r6, lr}
003cfc50: b        #0x3136b8
003cfc54: ldrb     r3, [r3, #8]
003cfc58: cmp      r3, #0
003cfc5c: bne      #0x3cfc44
003cfc60: ldr      r3, [r6, #0x520]
003cfc64: tst      r3, #0x100
003cfc68: beq      #0x3cfc44
003cfc6c: ldr      r3, [r6]
003cfc70: mov      r0, r6
003cfc74: mov      lr, pc
003cfc78: ldr      pc, [r3, #0xc4]
003cfc7c: cmp      r0, #0
003cfc80: beq      #0x3cfc90
003cfc84: ldrb     r3, [r6, #0x2ee]
003cfc88: cmp      r3, #0
003cfc8c: bne      #0x3cfd34
003cfc90: ldr      r6, [pc, #0xbc]
003cfc94: ldr      r3, [r5, #4]
003cfc98: mov      r2, #1
003cfc9c: add      r6, pc, r6
003cfca0: ldr      r4, [pc, #0xb0]
003cfca4: strb     r2, [r3, #0x88]
003cfca8: mov      r0, r6
003cfcac: bl       #0x3136b4
003cfcb0: mov      r0, r5
003cfcb4: bl       #0x3cb908
003cfcb8: add      r4, pc, r4
003cfcbc: mov      r0, r6
003cfcc0: ldr      r6, [pc, #0x94]
003cfcc4: bl       #0x3136b8
003cfcc8: mov      r0, r4
003cfccc: bl       #0x3136b4
003cfcd0: mov      r0, r5
003cfcd4: bl       #0x3cc5a4
003cfcd8: add      r6, pc, r6
003cfcdc: mov      r0, r4
003cfce0: ldr      r4, [pc, #0x78]
003cfce4: bl       #0x3136b8
003cfce8: mov      r0, r6
003cfcec: bl       #0x3136b4
003cfcf0: mov      r0, r5
003cfcf4: bl       #0x3cf3f0
003cfcf8: add      r4, pc, r4
003cfcfc: mov      r0, r6
003cfd00: bl       #0x3136b8
003cfd04: mov      r0, r4
003cfd08: bl       #0x3136b4
003cfd0c: mov      r0, r5
003cfd10: ldr      r3, [r5]
003cfd14: mov      lr, pc
003cfd18: ldr      pc, [r3, #0x18]
003cfd1c: mov      r0, r4
003cfd20: bl       #0x3136b8
003cfd24: ldr      r0, [pc, #0x38]
003cfd28: add      r0, pc, r0
003cfd2c: pop      {r4, r5, r6, lr}
003cfd30: b        #0x3136b8
003cfd34: ldrb     r3, [r6, #0x2f0]
003cfd38: cmp      r3, #0
003cfd3c: beq      #0x3cfc44
003cfd40: b        #0x3cfc90
003cfd44: strdeq   r5, r6, [pc], #-0x7c
003cfd48: subseq   r4, ip, r0, lsl #29
003cfd4c: andeq    r3, r0, r0, asr r6
003cfd50: strheq   r5, [pc], #-0x78
003cfd54: subeq    r5, pc, ip, ror r7
003cfd58: subeq    r5, pc, r8, ror r7
003cfd5c: subeq    r5, pc, r0, ror r7
003cfd60: subeq    r5, pc, r8, ror #14
003cfd64: ldrdeq   r5, r6, [pc], #-0x68

# _ZN12VisualObject12SyncPositionEv
00470cb8: ldr      r1, [r0, #4]
00470cbc: cmp      r1, #0
00470cc0: bxeq     lr
00470cc4: add      r1, r1, #0x160
00470cc8: b        #0x470c24

# _ZNK10GameObject9IsZonableEv
003883b8: ldrb     r0, [r0, #0x2ed]
003883bc: eor      r0, r0, #1
003883c0: bx       lr

# _ZNK9Character9IsZonableEv
003a36e4: push     {r4, lr}
003a36e8: ldr      r3, [r0]
003a36ec: mov      r4, r0
003a36f0: mov      lr, pc
003a36f4: ldr      pc, [r3, #0x28]
003a36f8: cmp      r0, #0
003a36fc: beq      #0x3a3708
003a3700: mov      r0, #0
003a3704: pop      {r4, pc}
003a3708: mov      r0, r4
003a370c: bl       #0x3a3094
003a3710: cmp      r0, #0
003a3714: bne      #0x3a3700
003a3718: mov      r0, r4
003a371c: pop      {r4, lr}
003a3720: b        #0x38ab60

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr

# _ZN10GameObject18UpdateAbsoluteAABBEv
0038aac8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov      r4, r0
0038aad0: ldr      sl, [r4, #0x148]
0038aad4: ldr      r0, [r0, #0x144]
0038aad8: ldr      r8, [r4, #0x14c]
0038aadc: ldr      r7, [r4, #0x150]
0038aae0: ldr      r6, [r4, #0x154]
0038aae4: ldr      r5, [r4, #0x158]
0038aae8: ldr      r1, [r4, #0x160]
0038aaec: str      r0, [r4, #0x12c]
0038aaf0: str      sl, [r4, #0x130]
0038aaf4: str      r8, [r4, #0x134]
0038aaf8: str      r7, [r4, #0x138]
0038aafc: str      r6, [r4, #0x13c]
0038ab00: str      r5, [r4, #0x140]
0038ab04: bl       #0x30eba4
0038ab08: ldr      r1, [r4, #0x164]
0038ab0c: str      r0, [r4, #0x12c]
0038ab10: mov      r0, sl
0038ab14: bl       #0x30eba4
0038ab18: ldr      r1, [r4, #0x168]
0038ab1c: str      r0, [r4, #0x130]
0038ab20: mov      r0, r8
0038ab24: bl       #0x30eba4
0038ab28: ldr      r1, [r4, #0x160]
0038ab2c: str      r0, [r4, #0x134]
0038ab30: mov      r0, r7
0038ab34: bl       #0x30eba4
0038ab38: ldr      r1, [r4, #0x164]
0038ab3c: str      r0, [r4, #0x138]
0038ab40: mov      r0, r6
0038ab44: bl       #0x30eba4
0038ab48: ldr      r1, [r4, #0x168]
0038ab4c: str      r0, [r4, #0x13c]
0038ab50: mov      r0, r5
0038ab54: bl       #0x30eba4
0038ab58: str      r0, [r4, #0x140]
0038ab5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK10GameObject6IsDeadEv
003400ac: mov      r0, #0
003400b0: bx       lr

# _ZN10GameObject14SetDestinationERK7Point3DIfE
00393600: ldr      r3, [r1]
00393604: str      r3, [r0, #0x1a8]
00393608: ldr      r3, [r1, #4]
0039360c: str      r3, [r0, #0x1ac]
00393610: ldr      r3, [r1, #8]
00393614: str      r3, [r0, #0x1b0]
00393618: bx       lr

# _ZN12VisualObject10SetVisibleEb
00471368: push     {r4, r5, r6, lr}
0047136c: ldr      r3, [r0, #8]
00471370: ldr      r2, [pc, #0x50]
00471374: mov      r4, r0
00471378: cmp      r3, #0
0047137c: mov      r5, r1
00471380: add      r2, pc, r2
00471384: beq      #0x4713c4
00471388: ldr      r1, [r3, #0x11c]
0047138c: and      r1, r1, #1
00471390: cmp      r5, r1
00471394: beq      #0x4713b0
00471398: ldr      r3, [pc, #0x2c]
0047139c: ldr      r3, [r2, r3]
004713a0: ldr      r3, [r3, #0x10]
004713a4: ldr      r0, [r3, #0x1c]
004713a8: bl       #0x350ee0
004713ac: ldr      r3, [r4, #8]
004713b0: mov      r0, r3
004713b4: mov      r1, r5
004713b8: ldr      r3, [r3]
004713bc: mov      lr, pc
004713c0: ldr      pc, [r3, #0x48]
004713c4: pop      {r4, r5, r6, pc}
004713c8: subseq   r3, r2, r0, lsl r7
004713cc: strdeq   r3, r4, [r0], -r4
