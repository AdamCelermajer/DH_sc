
# _ZNK16CharStateMachine9SM_IsIdleEb
003c0260: push     {r4, lr}
003c0264: mov      r4, r1
003c0268: bl       #0x3c01ac
003c026c: cmp      r0, #0xd
003c0270: beq      #0x3c028c
003c0274: cmp      r0, #0x12
003c0278: beq      #0x3c0294
003c027c: cmp      r0, #3
003c0280: beq      #0x3c028c
003c0284: mov      r0, #0
003c0288: pop      {r4, pc}
003c028c: mov      r0, #1
003c0290: pop      {r4, pc}
003c0294: eor      r0, r4, #1
003c0298: pop      {r4, pc}

# _ZN10GameObject11SetPositionERK7Point3DIfEb
00393db4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00393db8: ldr      r5, [r0, #0x2e0]
00393dbc: mov      r4, r0
00393dc0: mov      r6, r1
00393dc4: cmp      r5, #0
00393dc8: mov      r7, r2
00393dcc: beq      #0x393e2c
00393dd0: ldr      r1, [r0, #0x164]
00393dd4: ldr      r0, [r6, #4]
00393dd8: bl       #0x30e3ac
00393ddc: ldr      r1, [r4, #0x168]
00393de0: mov      sl, r0
00393de4: ldr      r0, [r6, #8]
00393de8: bl       #0x30e3ac
00393dec: ldr      r1, [r4, #0x160]
00393df0: mov      r8, r0
00393df4: ldr      r0, [r6]
00393df8: bl       #0x30e3ac
00393dfc: mov      r1, r0
00393e00: ldr      r0, [r5, #0xc]
00393e04: bl       #0x30eba4
00393e08: mov      r1, sl
00393e0c: str      r0, [r5, #0xc]
00393e10: ldr      r0, [r5, #0x10]
00393e14: bl       #0x30eba4
00393e18: mov      r1, r8
00393e1c: str      r0, [r5, #0x10]
00393e20: ldr      r0, [r5, #0x14]
00393e24: bl       #0x30eba4
00393e28: str      r0, [r5, #0x14]
00393e2c: ldr      r3, [r6]
00393e30: mov      r0, r4
00393e34: str      r3, [r4, #0x160]
00393e38: ldr      r3, [r6, #4]
00393e3c: str      r3, [r4, #0x164]
00393e40: ldr      r3, [r6, #8]
00393e44: str      r3, [r4, #0x168]
00393e48: bl       #0x38aac8
00393e4c: ldr      r0, [r4, #0x2dc]
00393e50: cmp      r0, #0
00393e54: beq      #0x393e64
00393e58: ldr      r1, [r4, #0x160]
00393e5c: ldr      r2, [r4, #0x164]
00393e60: bl       #0x46ea80
00393e64: ldr      r0, [r4, #0x2d8]
00393e68: cmp      r0, #0
00393e6c: beq      #0x393e74
00393e70: bl       #0x470cb8
00393e74: cmp      r7, #0
00393e78: bne      #0x393e80
00393e7c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393e80: mov      r0, r4
00393e84: mov      r1, r6
00393e88: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393e8c: b        #0x393600

# _ZNK16CharStateMachine20SM_IsAwaitingToSpawnEv
003c0230: push     {r4, lr}
003c0234: bl       #0x3c01ac
003c0238: cmp      r0, #0x11
003c023c: movne    r0, #0
003c0240: moveq    r0, #1
003c0244: pop      {r4, pc}

# _ZN10GameObject12EnableZoningEv
0038c790: push     {r4, r5, r6, lr}
0038c794: ldrb     r2, [r0, #0x2ee]
0038c798: ldr      r3, [pc, #0xd4]
0038c79c: mov      r4, r0
0038c7a0: cmp      r2, #0
0038c7a4: add      r3, pc, r3
0038c7a8: beq      #0x38c81c
0038c7ac: ldr      r3, [r4]
0038c7b0: mov      r0, r4
0038c7b4: mov      lr, pc
0038c7b8: ldr      pc, [r3, #0xc4]
0038c7bc: cmp      r0, #0
0038c7c0: beq      #0x38c7d4
0038c7c4: ldr      r0, [r4, #0x2d8]
0038c7c8: cmp      r0, #0
0038c7cc: beq      #0x38c7d4
0038c7d0: bl       #0x4713d0
0038c7d4: ldr      r3, [r4]
0038c7d8: mov      r0, r4
0038c7dc: ldr      r5, [r3, #0x3c]
0038c7e0: mov      lr, pc
0038c7e4: ldr      pc, [r3, #0xc4]
0038c7e8: cmp      r0, #0
0038c7ec: beq      #0x38c80c
0038c7f0: ldrb     r3, [r4, #0x2ee]
0038c7f4: cmp      r3, #0
0038c7f8: ldrbne   r1, [r4, #0x2f0]
0038c7fc: beq      #0x38c80c
0038c800: mov      r0, r4
0038c804: blx      r5
0038c808: pop      {r4, r5, r6, pc}
0038c80c: mov      r1, #1
0038c810: mov      r0, r4
0038c814: blx      r5
0038c818: pop      {r4, r5, r6, pc}
0038c81c: mov      r2, #1
0038c820: strb     r2, [r0, #0x2ee]
0038c824: ldr      r2, [pc, #0x4c]
0038c828: mov      r1, r0
0038c82c: ldr      r3, [r3, r2]
0038c830: ldr      r0, [r3, #0x38]
0038c834: bl       #0x3462b8
0038c838: ldr      r0, [r4, #0x2f4]
0038c83c: cmp      r0, #0
0038c840: beq      #0x38c7ac
0038c844: mov      r1, r4
0038c848: bl       #0x39672c
0038c84c: ldr      r3, [r4, #0x2f4]
0038c850: ldrb     r3, [r3, #0x389]
0038c854: cmp      r3, #0
0038c858: beq      #0x38c868
0038c85c: mov      r0, r4
0038c860: bl       #0x38c710
0038c864: b        #0x38c7ac
0038c868: mov      r0, r4
0038c86c: bl       #0x38c69c
0038c870: b        #0x38c7ac
0038c874: rsbeq    r8, r0, ip, ror #5
0038c878: strdeq   r3, r4, [r0], -r4

# _ZN6CharAI8OnUpdateEv
003d1050: push     {r4, lr}
003d1054: ldr      r3, [r0, #0x1c]
003d1058: sub      sp, sp, #0x10
003d105c: mov      r4, r0
003d1060: cmp      r3, #0
003d1064: beq      #0x3d1078
003d1068: mov      r0, r3
003d106c: ldr      r3, [r3]
003d1070: mov      lr, pc
003d1074: ldr      pc, [r3, #0x18]
003d1078: ldr      r0, [r4, #4]
003d107c: mov      r1, #0
003d1080: add      r0, r0, #0x4f0
003d1084: add      r0, r0, #0xc
003d1088: bl       #0x3c0260
003d108c: cmp      r0, #0
003d1090: beq      #0x3d10c8
003d1094: ldr      r3, [r4, #4]
003d1098: ldr      r2, [r3, #0x408]
003d109c: cmp      r2, #0
003d10a0: beq      #0x3d10f4
003d10a4: ldr      r4, [r3, #0x2d8]
003d10a8: mov      r0, r3
003d10ac: bl       #0x38c600
003d10b0: cmp      r4, #0
003d10b4: beq      #0x3d10c0
003d10b8: mov      r0, r4
003d10bc: bl       #0x4713d0
003d10c0: add      sp, sp, #0x10
003d10c4: pop      {r4, pc}
003d10c8: ldr      r0, [r4, #4]
003d10cc: add      r0, r0, #0x4f0
003d10d0: add      r0, r0, #0xc
003d10d4: bl       #0x3c0230
003d10d8: cmp      r0, #0
003d10dc: ldreq    r3, [r4, #4]
003d10e0: beq      #0x3d10a4
003d10e4: ldr      r3, [r4, #4]
003d10e8: ldr      r2, [r3, #0x408]
003d10ec: cmp      r2, #0
003d10f0: bne      #0x3d10a4
003d10f4: ldr      r2, [r3, #0x418]
003d10f8: cmp      r2, #0
003d10fc: bne      #0x3d10a4
003d1100: ldrb     r2, [r3, #0x2ee]
003d1104: cmp      r2, #0
003d1108: bne      #0x3d10c0
003d110c: mov      r0, r3
003d1110: ldr      r3, [r3]
003d1114: mov      lr, pc
003d1118: ldr      pc, [r3, #0xc4]
003d111c: cmp      r0, #0
003d1120: beq      #0x3d10c0
003d1124: ldr      r0, [r4, #4]
003d1128: bl       #0x38c790
003d112c: ldr      r3, [r4, #4]
003d1130: mov      r0, r3
003d1134: ldr      r3, [r3]
003d1138: mov      lr, pc
003d113c: ldr      pc, [r3, #0x34]
003d1140: cmp      r0, #0
003d1144: bne      #0x3d10c0
003d1148: ldr      r0, [r4, #4]
003d114c: ldrb     r3, [r0, #0x85]
003d1150: cmp      r3, #0
003d1154: bne      #0x3d10c0
003d1158: add      r1, r0, #0x1440
003d115c: mov      r2, #1
003d1160: add      r1, r1, #0x10
003d1164: bl       #0x393db4
003d1168: ldr      r3, [r4, #4]
003d116c: ldr      r2, [r3, #0x2d8]
003d1170: cmp      r2, #0
003d1174: beq      #0x3d10c0
003d1178: ldr      r0, [r2, #8]
003d117c: cmp      r0, #0
003d1180: beq      #0x3d10c0
003d1184: movw     r2, #0x1450
003d1188: ldr      lr, [r3, r2]
003d118c: movw     r2, #0x1454
003d1190: ldr      ip, [r3, r2]
003d1194: movw     r2, #0x1458
003d1198: ldr      r2, [r3, r2]
003d119c: ldr      r3, [r0]
003d11a0: add      r1, sp, #4
003d11a4: ldr      r3, [r3, #0xa4]
003d11a8: str      lr, [sp, #4]
003d11ac: str      ip, [sp, #8]
003d11b0: str      r2, [sp, #0xc]
003d11b4: blx      r3
003d11b8: b        #0x3d10c0

# _ZN12VisualObject14SyncVisibilityEv
004713d0: push     {r4, r5, r6, lr}
004713d4: ldr      r4, [r0, #4]
004713d8: mov      r5, r0
004713dc: cmp      r4, #0
004713e0: beq      #0x471438
004713e4: ldrb     r3, [r4, #0x80]
004713e8: cmp      r3, #0
004713ec: bne      #0x471400
004713f0: mov      r1, #0
004713f4: mov      r0, r5
004713f8: pop      {r4, r5, r6, lr}
004713fc: b        #0x471368
00471400: ldr      r3, [r4]
00471404: mov      r0, r4
00471408: mov      lr, pc
0047140c: ldr      pc, [r3, #0xc4]
00471410: cmp      r0, #0
00471414: beq      #0x471430
00471418: ldrb     r3, [r4, #0x2ee]
0047141c: cmp      r3, #0
00471420: beq      #0x471430
00471424: ldrb     r3, [r4, #0x2f0]
00471428: cmp      r3, #0
0047142c: beq      #0x4713f0
00471430: mov      r1, #1
00471434: b        #0x4713f4
00471438: pop      {r4, r5, r6, pc}

# _ZN10GameObject13DisableZoningEv
0038c600: push     {r4, r5, r6, lr}
0038c604: ldrb     r3, [r0, #0x2ee]
0038c608: ldr      r5, [pc, #0x84]
0038c60c: mov      r4, r0
0038c610: cmp      r3, #0
0038c614: add      r5, pc, r5
0038c618: beq      #0x38c64c
0038c61c: ldr      r0, [r0, #0x2f4]
0038c620: cmp      r0, #0
0038c624: beq      #0x38c630
0038c628: mov      r1, r4
0038c62c: bl       #0x3968cc
0038c630: ldr      r3, [pc, #0x60]
0038c634: mov      r1, r4
0038c638: ldr      r3, [r5, r3]
0038c63c: ldr      r0, [r3, #0x38]
0038c640: bl       #0x344184
0038c644: mov      r3, #0
0038c648: strb     r3, [r4, #0x2ee]
0038c64c: ldr      r3, [r4]
0038c650: mov      r0, r4
0038c654: ldr      r5, [r3, #0x3c]
0038c658: mov      lr, pc
0038c65c: ldr      pc, [r3, #0xc4]
0038c660: cmp      r0, #0
0038c664: beq      #0x38c684
0038c668: ldrb     r3, [r4, #0x2ee]
0038c66c: cmp      r3, #0
0038c670: ldrbne   r1, [r4, #0x2f0]
0038c674: beq      #0x38c684
0038c678: mov      r0, r4
0038c67c: blx      r5
0038c680: pop      {r4, r5, r6, pc}
0038c684: mov      r1, #1
0038c688: mov      r0, r4
0038c68c: blx      r5
0038c690: pop      {r4, r5, r6, pc}
0038c694: rsbeq    r8, r0, ip, ror r4
0038c698: strdeq   r3, r4, [r0], -r4
