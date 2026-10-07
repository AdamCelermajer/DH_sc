
# _ZN12VisualObject17SetAnimControllerEP14AnimController
00470a84: push     {r4, r5, r6, lr}
00470a88: ldr      r3, [r0, #0x38]
00470a8c: mov      r4, r0
00470a90: mov      r5, r1
00470a94: cmp      r3, r1
00470a98: beq      #0x470ac0
00470a9c: cmp      r3, #0
00470aa0: beq      #0x470abc
00470aa4: mov      r0, r3
00470aa8: ldr      r3, [r3]
00470aac: mov      lr, pc
00470ab0: ldr      pc, [r3, #4]
00470ab4: mov      r3, #0
00470ab8: str      r3, [r4, #0x38]
00470abc: str      r5, [r4, #0x38]
00470ac0: pop      {r4, r5, r6, pc}

# _ZN14AnimController8SetScaleEfj
00474920: ldr      r3, [pc, #0x58]
00474924: push     {r4, lr}
00474928: mov      r4, r1
0047492c: ldr      r1, [pc, #0x50]
00474930: add      r3, pc, r3
00474934: ldr      ip, [r3, r1]
00474938: ldrb     r3, [ip]
0047493c: cmp      r3, #0
00474940: bne      #0x474948
00474944: pop      {r4, pc}
00474948: mov      r1, r2
0047494c: bl       #0x4748b8
00474950: subs     r3, r0, #0
00474954: beq      #0x474944
00474958: ldr      r3, [r3]
0047495c: mov      lr, pc
00474960: ldr      pc, [r3, #0x44]
00474964: subs     r3, r0, #0
00474968: beq      #0x474944
0047496c: ldr      r3, [r3]
00474970: mov      r1, r4
00474974: mov      lr, pc
00474978: ldr      pc, [r3, #0x48]
0047497c: pop      {r4, pc}
00474980: subseq   r0, r2, r0, ror #2
00474984: andeq    r3, r0, ip, ror #27

# _ZN14AnimControllerD0Ev
00474d14: push     {r4, lr}
00474d18: mov      r4, r0
00474d1c: bl       #0x4746c8
00474d20: mov      r0, r4
00474d24: bl       #0x310440
00474d28: mov      r0, r4
00474d2c: pop      {r4, pc}

# _ZN14AnimController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474d10: b        #0x474cac

# _ZN14AnimController17SetCallbacksOnAllEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474cac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00474cb0: mov      r6, r0
00474cb4: sub      sp, sp, #8
00474cb8: ldr      r0, [r0, #4]
00474cbc: mov      r7, r1
00474cc0: mov      r8, r2
00474cc4: mov      sl, r3
00474cc8: ldr      sb, [sp, #0x28]
00474ccc: bl       #0x597094
00474cd0: ldr      r4, [r0]
00474cd4: mov      r5, r0
00474cd8: cmp      r4, r0
00474cdc: beq      #0x474d08
00474ce0: ldr      r1, [r4, #8]
00474ce4: mov      r0, r6
00474ce8: mov      r2, r7
00474cec: mov      r3, r8
00474cf0: str      sl, [sp]
00474cf4: str      sb, [sp, #4]
00474cf8: bl       #0x474c78
00474cfc: ldr      r4, [r4]
00474d00: cmp      r5, r4
00474d04: bne      #0x474ce0
00474d08: add      sp, sp, #8
00474d0c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN14AnimControllerC1EP13RootSceneNodeb
00474d30: push     {r4, r5, r6, lr}
00474d34: ldr      r4, [pc, #0xe4]
00474d38: ldr      r3, [pc, #0xe4]
00474d3c: cmp      r1, #0
00474d40: add      r4, pc, r4
00474d44: ldr      r3, [r4, r3]
00474d48: sub      sp, sp, #8
00474d4c: mov      r5, r0
00474d50: add      r3, r3, #8
00474d54: str      r3, [r0]
00474d58: mov      r6, r2
00474d5c: str      r1, [r0, #4]
00474d60: beq      #0x474dc8
00474d64: ldr      r3, [r1]
00474d68: cmp      r6, #0
00474d6c: ldr      r3, [r3, #-0xc]
00474d70: add      r1, r1, r3
00474d74: ldr      r3, [r1, #4]
00474d78: add      r3, r3, #1
00474d7c: str      r3, [r1, #4]
00474d80: bne      #0x474db0
00474d84: ldr      r3, [pc, #0x9c]
00474d88: mov      r0, r5
00474d8c: mov      r2, r5
00474d90: ldr      r1, [r4, r3]
00474d94: ldr      r3, [pc, #0x90]
00474d98: str      r5, [sp]
00474d9c: ldr      r3, [r4, r3]
00474da0: bl       #0x474cac
00474da4: mov      r0, r5
00474da8: add      sp, sp, #8
00474dac: pop      {r4, r5, r6, pc}
00474db0: ldr      r3, [r5, #4]
00474db4: mov      r0, r3
00474db8: ldr      r3, [r3]
00474dbc: mov      lr, pc
00474dc0: ldr      pc, [r3, #0x74]
00474dc4: b        #0x474da4
00474dc8: ldr      r3, [pc, #0x60]
00474dcc: ldr      r3, [r4, r3]
00474dd0: ldr      r3, [r3]
00474dd4: cmp      r3, #2
00474dd8: streq    r1, [r1]
00474ddc: beq      #0x474d64
00474de0: cmp      r3, #1
00474de4: bne      #0x474d64
00474de8: ldr      r0, [pc, #0x44]
00474dec: ldr      r1, [pc, #0x44]
00474df0: ldr      r2, [pc, #0x44]
00474df4: ldr      r0, [r4, r0]
00474df8: ldr      r3, [pc, #0x40]
00474dfc: add      r1, pc, r1
00474e00: mov      ip, #0x1c
00474e04: add      r0, r0, #0xa8
00474e08: add      r2, pc, r2
00474e0c: add      r3, pc, r3
00474e10: str      ip, [sp]
00474e14: bl       #0x30e004
00474e18: ldr      r1, [r5, #4]
00474e1c: b        #0x474d64
00474e20: subseq   pc, r1, r0, asr sp
00474e24: andeq    r4, r0, r0, lsl r8
00474e28: andeq    r3, r0, r0, ror r7
00474e2c: andeq    r2, r0, r0, lsl #18
00474e30: andeq    r3, r0, r0, asr #19
00474e34: andeq    r1, r0, r0, asr #19
00474e38: ldrdeq   sb, sl, [r4], #-0x5c
00474e3c: subeq    r8, r5, r8, lsr r8
00474e40: subeq    r8, r5, ip, asr sb
