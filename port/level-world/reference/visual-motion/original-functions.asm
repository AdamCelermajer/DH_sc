
# _ZN14AnimApplicator10ResetDeltaEjRKN6glitch4core8vector3dIfEE
003644cc: ldr      r3, [r2]
003644d0: mov      ip, #0
003644d4: str      r3, [r0, #0x18]
003644d8: ldr      r3, [r2, #4]
003644dc: str      r3, [r0, #0x1c]
003644e0: ldr      r3, [r2, #8]
003644e4: str      r1, [r0, #0x14]
003644e8: str      ip, [r0, #0x2c]
003644ec: str      r3, [r0, #0x20]
003644f0: str      ip, [r0, #0x24]
003644f4: str      ip, [r0, #0x28]
003644f8: bx       lr

# _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00369160: push     {r4, lr}
00369164: subs     r4, r0, #0
00369168: bne      #0x369174
0036916c: mov      r0, #0
00369170: pop      {r4, pc}
00369174: ldr      r3, [r4]
00369178: mov      lr, pc
0036917c: ldr      pc, [r3, #0x24]
00369180: sub      r0, r0, #0xb
00369184: cmp      r0, #4
00369188: addls    pc, pc, r0, lsl #2
0036918c: b        #0x36916c
00369190: b        #0x3691a4
00369194: b        #0x3691ac
00369198: b        #0x3691a4
0036919c: b        #0x3691b4
003691a0: b        #0x3691bc
003691a4: add      r0, r4, #0x58
003691a8: pop      {r4, pc}
003691ac: add      r0, r4, #0x88
003691b0: pop      {r4, pc}
003691b4: add      r0, r4, #0xd8
003691b8: pop      {r4, pc}
003691bc: add      r0, r4, #0x84
003691c0: pop      {r4, pc}

# _ZNK6glitch4core10quaternionmlERKNS0_8vector3dIfEE
0035bc90: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035bc94: ldr      r6, [r1, #4]
0035bc98: ldr      r3, [r2, #8]
0035bc9c: sub      sp, sp, #0x1c
0035bca0: add      ip, r6, #0x80000000
0035bca4: ldr      fp, [r1, #8]
0035bca8: mov      r5, r0
0035bcac: mov      r4, r1
0035bcb0: mov      r0, ip
0035bcb4: mov      r1, r3
0035bcb8: ldr      r8, [r2, #4]
0035bcbc: mov      r7, r2
0035bcc0: str      ip, [sp, #8]
0035bcc4: str      r3, [sp]
0035bcc8: bl       #0x30ed6c
0035bccc: mov      r1, r8
0035bcd0: mov      sl, r0
0035bcd4: mov      r0, fp
0035bcd8: bl       #0x30ed6c
0035bcdc: mov      r1, r0
0035bce0: mov      r0, sl
0035bce4: bl       #0x30eba4
0035bce8: str      r0, [sp, #0x10]
0035bcec: ldr      sb, [r7]
0035bcf0: add      r2, fp, #0x80000000
0035bcf4: mov      r0, r2
0035bcf8: mov      r1, sb
0035bcfc: ldr      sl, [r4]
0035bd00: str      r2, [sp, #4]
0035bd04: bl       #0x30ed6c
0035bd08: ldr      r3, [sp]
0035bd0c: mov      r7, r0
0035bd10: mov      r0, sl
0035bd14: mov      r1, r3
0035bd18: bl       #0x30ed6c
0035bd1c: add      lr, sl, #0x80000000
0035bd20: mov      r1, r0
0035bd24: mov      r0, r7
0035bd28: str      lr, [sp, #0x14]
0035bd2c: bl       #0x30eba4
0035bd30: ldr      r1, [sp, #0x14]
0035bd34: mov      r7, r0
0035bd38: mov      r0, r8
0035bd3c: bl       #0x30ed6c
0035bd40: mov      r1, sb
0035bd44: str      r0, [sp, #0xc]
0035bd48: mov      r0, r6
0035bd4c: bl       #0x30ed6c
0035bd50: mov      r1, r0
0035bd54: ldr      r0, [sp, #0xc]
0035bd58: bl       #0x30eba4
0035bd5c: str      r0, [sp, #0xc]
0035bd60: ldr      r0, [r4, #0xc]
0035bd64: mov      r1, r0
0035bd68: bl       #0x30eba4
0035bd6c: ldr      ip, [sp, #8]
0035bd70: mov      r4, r0
0035bd74: ldr      r1, [sp, #0xc]
0035bd78: mov      r0, ip
0035bd7c: bl       #0x30ed6c
0035bd80: mov      r1, r7
0035bd84: mov      ip, r0
0035bd88: mov      r0, fp
0035bd8c: str      ip, [sp, #8]
0035bd90: bl       #0x30ed6c
0035bd94: ldr      ip, [sp, #8]
0035bd98: mov      r1, r0
0035bd9c: mov      r0, ip
0035bda0: bl       #0x30eba4
0035bda4: mov      r1, r0
0035bda8: bl       #0x30eba4
0035bdac: ldr      r1, [sp, #0x10]
0035bdb0: mov      fp, r0
0035bdb4: mov      r0, r4
0035bdb8: bl       #0x30ed6c
0035bdbc: mov      r1, r0
0035bdc0: mov      r0, sb
0035bdc4: bl       #0x30eba4
0035bdc8: mov      r1, r0
0035bdcc: mov      r0, fp
0035bdd0: bl       #0x30eba4
0035bdd4: str      r0, [r5]
0035bdd8: ldr      r2, [sp, #4]
0035bddc: ldr      r0, [sp, #0x10]
0035bde0: mov      r1, r2
0035bde4: bl       #0x30ed6c
0035bde8: ldr      r1, [sp, #0xc]
0035bdec: mov      sb, r0
0035bdf0: mov      r0, sl
0035bdf4: bl       #0x30ed6c
0035bdf8: mov      r1, r0
0035bdfc: mov      r0, sb
0035be00: bl       #0x30eba4
0035be04: mov      r1, r0
0035be08: bl       #0x30eba4
0035be0c: mov      r1, r7
0035be10: mov      sl, r0
0035be14: mov      r0, r4
0035be18: bl       #0x30ed6c
0035be1c: mov      r1, r0
0035be20: mov      r0, r8
0035be24: bl       #0x30eba4
0035be28: mov      r1, r0
0035be2c: mov      r0, sl
0035be30: bl       #0x30eba4
0035be34: str      r0, [r5, #4]
0035be38: ldr      r1, [sp, #0x14]
0035be3c: mov      r0, r7
0035be40: bl       #0x30ed6c
0035be44: ldr      r1, [sp, #0x10]
0035be48: mov      r7, r0
0035be4c: mov      r0, r6
0035be50: bl       #0x30ed6c
0035be54: mov      r1, r0
0035be58: mov      r0, r7
0035be5c: bl       #0x30eba4
0035be60: mov      r1, r0
0035be64: bl       #0x30eba4
0035be68: ldr      r1, [sp, #0xc]
0035be6c: mov      r6, r0
0035be70: mov      r0, r4
0035be74: bl       #0x30ed6c
0035be78: ldr      r3, [sp]
0035be7c: mov      r1, r0
0035be80: mov      r0, r3
0035be84: bl       #0x30eba4
0035be88: mov      r1, r0
0035be8c: mov      r0, r6
0035be90: bl       #0x30eba4
0035be94: str      r0, [r5, #8]
0035be98: mov      r0, r5
0035be9c: add      sp, sp, #0x1c
0035bea0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN13RootSceneNode19_HandleDisplacementEj
0035cf48: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035cf4c: ldr      r3, [r0, #0x1f0]
0035cf50: sub      sp, sp, #0x4c
0035cf54: mov      r4, r0
0035cf58: mov      r0, r3
0035cf5c: ldr      r3, [r3]
0035cf60: mov      lr, pc
0035cf64: ldr      pc, [r3, #0xa0]
0035cf68: mov      r3, r0
0035cf6c: ldr      r2, [r3]
0035cf70: add      r8, sp, #0x3c
0035cf74: ldr      r5, [r3, #8]
0035cf78: mov      r7, #0
0035cf7c: str      r2, [sp, #4]
0035cf80: mov      r1, r8
0035cf84: mov      r0, r4
0035cf88: ldr      r6, [r3, #4]
0035cf8c: str      r7, [sp, #0x3c]
0035cf90: str      r7, [sp, #0x40]
0035cf94: str      r7, [sp, #0x44]
0035cf98: bl       #0x35cd5c
0035cf9c: ldr      r1, [r4, #0xc8]
0035cfa0: ldr      r0, [sp, #0x3c]
0035cfa4: bl       #0x30ed6c
0035cfa8: str      r0, [sp, #0x3c]
0035cfac: ldr      r1, [r4, #0xcc]
0035cfb0: ldr      r0, [sp, #0x40]
0035cfb4: bl       #0x30ed6c
0035cfb8: mov      r2, r8
0035cfbc: add      r1, r4, #0xb8
0035cfc0: str      r0, [sp, #0x40]
0035cfc4: add      r0, sp, #0x30
0035cfc8: str      r7, [sp, #0x44]
0035cfcc: bl       #0x35bc90
0035cfd0: ldr      r2, [sp, #0x30]
0035cfd4: ldr      r3, [r4]
0035cfd8: mov      r0, r4
0035cfdc: str      r2, [sp, #0x3c]
0035cfe0: ldr      r2, [sp, #0x34]
0035cfe4: str      r2, [sp, #0x40]
0035cfe8: ldr      r2, [sp, #0x38]
0035cfec: str      r2, [sp, #0x44]
0035cff0: ldr      sl, [r3, #0xa4]
0035cff4: mov      lr, pc
0035cff8: ldr      pc, [r3, #0xa0]
0035cffc: ldr      r1, [sp, #0x40]
0035d000: mov      r8, r0
0035d004: ldr      r0, [r0, #4]
0035d008: bl       #0x30eba4
0035d00c: ldr      r1, [sp, #0x44]
0035d010: mov      fp, r0
0035d014: ldr      r0, [r8, #8]
0035d018: bl       #0x30eba4
0035d01c: ldr      r1, [sp, #0x3c]
0035d020: mov      sb, r0
0035d024: ldr      r0, [r8]
0035d028: bl       #0x30eba4
0035d02c: str      fp, [sp, #0x28]
0035d030: str      r0, [sp, #0x24]
0035d034: str      sb, [sp, #0x2c]
0035d038: mov      r0, r4
0035d03c: add      r1, sp, #0x24
0035d040: blx      sl
0035d044: ldr      r0, [r4, #0x1f8]
0035d048: cmp      r0, #0
0035d04c: beq      #0x35d0e8
0035d050: ldr      r1, [sp, #4]
0035d054: ldr      r3, [r0]
0035d058: add      r6, r6, #0x80000000
0035d05c: add      r2, r1, #0x80000000
0035d060: add      r5, r5, #0x80000000
0035d064: ldr      r3, [r3, #0xa4]
0035d068: add      r1, sp, #0x18
0035d06c: str      r2, [sp, #0x18]
0035d070: str      r6, [sp, #0x1c]
0035d074: str      r5, [sp, #0x20]
0035d078: blx      r3
0035d07c: ldr      r3, [r4, #0x1f8]
0035d080: mov      r1, #0
0035d084: mov      r0, r3
0035d088: ldr      r3, [r3]
0035d08c: mov      lr, pc
0035d090: ldr      pc, [r3, #0xb8]
0035d094: ldr      r0, [sp, #0x3c]
0035d098: mov      r1, #0
0035d09c: bl       #0x30df8c
0035d0a0: cmp      r0, #0
0035d0a4: beq      #0x35d0dc
0035d0a8: ldr      r0, [sp, #0x40]
0035d0ac: mov      r1, #0
0035d0b0: bl       #0x30df8c
0035d0b4: cmp      r0, #0
0035d0b8: beq      #0x35d0dc
0035d0bc: ldr      r0, [sp, #0x44]
0035d0c0: mov      r1, #0
0035d0c4: bl       #0x30df8c
0035d0c8: cmp      r0, #0
0035d0cc: mov      r0, #0
0035d0d0: moveq    r0, #1
0035d0d4: uxtb     r0, r0
0035d0d8: b        #0x35d0e0
0035d0dc: mov      r0, #1
0035d0e0: add      sp, sp, #0x4c
0035d0e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035d0e8: ldr      r0, [r4, #0x1f0]
0035d0ec: mov      r1, r7
0035d0f0: mov      r2, r7
0035d0f4: mov      r3, r5
0035d0f8: bl       #0x597154
0035d0fc: ldr      r4, [r4, #0x1f4]
0035d100: cmp      r4, #0
0035d104: beq      #0x35d094
0035d108: ldr      r3, [r4]
0035d10c: mov      r0, r4
0035d110: ldr      r8, [r3, #0xa4]
0035d114: mov      lr, pc
0035d118: ldr      pc, [r3, #0xa0]
0035d11c: mov      r1, r6
0035d120: mov      r7, r0
0035d124: ldr      r0, [r0, #4]
0035d128: bl       #0x30e3ac
0035d12c: mov      r1, r5
0035d130: mov      r6, r0
0035d134: ldr      r0, [r7, #8]
0035d138: bl       #0x30e3ac
0035d13c: ldr      r1, [sp, #4]
0035d140: mov      r5, r0
0035d144: ldr      r0, [r7]
0035d148: bl       #0x30e3ac
0035d14c: str      r6, [sp, #0x10]
0035d150: str      r0, [sp, #0xc]
0035d154: str      r5, [sp, #0x14]
0035d158: mov      r0, r4
0035d15c: add      r1, sp, #0xc
0035d160: blx      r8
0035d164: b        #0x35d094

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

# _ZNK6glitch5scene10ISceneNode11getPositionEv
00597124: add      r0, r0, #0xac
00597128: bx       lr

# _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0059712c: ldr      r3, [r1]
00597130: ldr      r2, [r0, #0x11c]
00597134: str      r3, [r0, #0xac]
00597138: ldr      r3, [r1, #4]
0059713c: orr      r2, r2, #8
00597140: str      r3, [r0, #0xb0]
00597144: ldr      r3, [r1, #8]
00597148: str      r2, [r0, #0x11c]
0059714c: str      r3, [r0, #0xb4]
00597150: bx       lr

# _ZN14AnimApplicator14CalculateDeltaEjRKN6glitch4core8vector3dIfEE
00364444: push     {r4, r5, r6, r7, r8, lr}
00364448: ldr      r3, [r0, #0x14]
0036444c: mov      r4, r0
00364450: mov      r5, r1
00364454: cmp      r3, r1
00364458: mov      r6, r2
0036445c: beq      #0x3644b8
00364460: ldr      r0, [r2, #4]
00364464: ldr      r1, [r4, #0x1c]
00364468: bl       #0x30e3ac
0036446c: ldr      r1, [r4, #0x20]
00364470: mov      r8, r0
00364474: ldr      r0, [r6, #8]
00364478: bl       #0x30e3ac
0036447c: ldr      r1, [r4, #0x18]
00364480: mov      r7, r0
00364484: ldr      r0, [r6]
00364488: bl       #0x30e3ac
0036448c: str      r8, [r4, #0x28]
00364490: str      r0, [r4, #0x24]
00364494: str      r7, [r4, #0x2c]
00364498: ldr      r3, [r6]
0036449c: str      r3, [r4, #0x18]
003644a0: ldr      r3, [r6, #4]
003644a4: str      r3, [r4, #0x1c]
003644a8: ldr      r3, [r6, #8]
003644ac: str      r5, [r4, #0x14]
003644b0: str      r3, [r4, #0x20]
003644b4: pop      {r4, r5, r6, r7, r8, pc}
003644b8: mov      r3, #0
003644bc: str      r3, [r0, #0x2c]
003644c0: str      r3, [r0, #0x24]
003644c4: str      r3, [r0, #0x28]
003644c8: b        #0x364498

# _ZN14AnimApplicator10ResetDeltaEj
003644fc: push     {r4, r5, lr}
00364500: ldr      r3, [r0, #8]
00364504: sub      sp, sp, #0x14
00364508: mov      r4, r0
0036450c: cmp      r3, #0
00364510: mov      r5, r1
00364514: beq      #0x364590
00364518: ldr      r1, [r0, #0xc]
0036451c: cmn      r1, #1
00364520: beq      #0x3645bc
00364524: ldr      r3, [r0, #4]
00364528: mov      r2, #0
0036452c: str      r2, [sp, #0xc]
00364530: cmp      r3, #0
00364534: str      r2, [sp, #4]
00364538: str      r2, [sp, #8]
0036453c: beq      #0x3645ac
00364540: mov      r0, r3
00364544: ldr      r3, [r3]
00364548: mov      lr, pc
0036454c: ldr      pc, [r3, #0x44]
00364550: ldr      r3, [r4, #4]
00364554: cmp      r0, #0
00364558: ldr      r1, [r4, #0xc]
0036455c: ldr      r2, [r3]
00364560: ldr      ip, [r2, #0x7c]
00364564: ldrne    r2, [r0, #0x10]
00364568: beq      #0x3645b4
0036456c: mov      r0, r3
00364570: add      r3, sp, #4
00364574: blx      ip
00364578: ldr      r2, [sp, #8]
0036457c: ldr      r3, [sp, #0xc]
00364580: ldr      r1, [sp, #4]
00364584: str      r2, [r4, #0x1c]
00364588: str      r3, [r4, #0x20]
0036458c: str      r1, [r4, #0x18]
00364590: mov      r3, #0
00364594: str      r5, [r4, #0x14]
00364598: str      r3, [r4, #0x2c]
0036459c: str      r3, [r4, #0x24]
003645a0: str      r3, [r4, #0x28]
003645a4: add      sp, sp, #0x14
003645a8: pop      {r4, r5, pc}
003645ac: ldr      r2, [r3]
003645b0: ldr      ip, [r2, #0x7c]
003645b4: mov      r2, r5
003645b8: b        #0x36456c
003645bc: mov      r0, r3
003645c0: ldr      r3, [r3]
003645c4: mov      lr, pc
003645c8: ldr      pc, [r3, #0xa0]
003645cc: ldr      r3, [r0]
003645d0: str      r3, [r4, #0x18]
003645d4: ldr      r3, [r0, #4]
003645d8: str      r3, [r4, #0x1c]
003645dc: ldr      r3, [r0, #8]
003645e0: str      r3, [r4, #0x20]
003645e4: b        #0x364590

# _ZN13RootSceneNode7NewAnimEb
0035d624: push     {r4, lr}
0035d628: mov      r4, r0
0035d62c: bl       #0x35d4cc
0035d630: ldrb     r3, [r4, #0x1ec]
0035d634: cmp      r3, #0
0035d638: beq      #0x35d654
0035d63c: ldr      r3, [r4, #0x1f0]
0035d640: cmp      r3, #0
0035d644: beq      #0x35d654
0035d648: ldr      r1, [r4, #0x1fc]
0035d64c: cmp      r1, #0
0035d650: bne      #0x35d658
0035d654: pop      {r4, pc}
0035d658: mov      r0, r4
0035d65c: add      r1, r1, #1
0035d660: bl       #0x35ce6c
0035d664: mov      r0, r4
0035d668: ldr      r3, [r4]
0035d66c: ldr      r1, [r4, #0x1fc]
0035d670: mov      lr, pc
0035d674: ldr      pc, [r3, #0x14]
0035d678: pop      {r4, pc}

# _ZN12VisualObject11SetPositionERK7Point3DIfE
00470c24: push     {r4, lr}
00470c28: mov      r4, r0
00470c2c: ldr      r0, [r0, #8]
00470c30: sub      sp, sp, #0x10
00470c34: cmp      r0, #0
00470c38: beq      #0x470c7c
00470c3c: ldr      r3, [r0]
00470c40: ldr      lr, [r1]
00470c44: ldr      ip, [r1, #4]
00470c48: ldr      r2, [r1, #8]
00470c4c: ldr      r3, [r3, #0xa4]
00470c50: add      r1, sp, #4
00470c54: str      lr, [sp, #4]
00470c58: str      ip, [sp, #8]
00470c5c: str      r2, [sp, #0xc]
00470c60: blx      r3
00470c64: ldr      r3, [r4, #8]
00470c68: mov      r1, #0
00470c6c: mov      r0, r3
00470c70: ldr      r3, [r3]
00470c74: mov      lr, pc
00470c78: ldr      pc, [r3, #0xb8]
00470c7c: add      sp, sp, #0x10
00470c80: pop      {r4, pc}

# _ZN14AnimApplicator11AnimateNodeEj
003645e8: push     {r4, r5, r6, r7, lr}
003645ec: ldr      r3, [r0, #8]
003645f0: sub      sp, sp, #0x14
003645f4: mov      r4, r0
003645f8: cmp      r3, #0
003645fc: mov      r5, r1
00364600: beq      #0x3646c0
00364604: ldr      r1, [r0, #0xc]
00364608: mov      r2, #0
0036460c: str      r2, [sp, #0xc]
00364610: cmn      r1, #1
00364614: str      r2, [sp, #4]
00364618: str      r2, [sp, #8]
0036461c: beq      #0x3646f0
00364620: ldr      r3, [r0, #4]
00364624: cmp      r3, #0
00364628: beq      #0x3646e0
0036462c: mov      r0, r3
00364630: ldr      r3, [r3]
00364634: mov      lr, pc
00364638: ldr      pc, [r3, #0x44]
0036463c: ldr      r3, [r4, #4]
00364640: cmp      r0, #0
00364644: ldr      r1, [r4, #0xc]
00364648: ldr      r2, [r3]
0036464c: ldr      ip, [r2, #0x7c]
00364650: ldrne    r2, [r0, #4]
00364654: beq      #0x3646e8
00364658: mov      r0, r3
0036465c: add      r3, sp, #4
00364660: blx      ip
00364664: ldr      r3, [r4, #0x14]
00364668: cmp      r3, r5
0036466c: beq      #0x3646cc
00364670: ldr      r0, [sp, #8]
00364674: ldr      r1, [r4, #0x1c]
00364678: bl       #0x30e3ac
0036467c: ldr      r1, [r4, #0x20]
00364680: mov      r7, r0
00364684: ldr      r0, [sp, #0xc]
00364688: bl       #0x30e3ac
0036468c: ldr      r1, [r4, #0x18]
00364690: mov      r6, r0
00364694: ldr      r0, [sp, #4]
00364698: bl       #0x30e3ac
0036469c: str      r7, [r4, #0x28]
003646a0: str      r0, [r4, #0x24]
003646a4: str      r6, [r4, #0x2c]
003646a8: ldr      r2, [sp, #8]
003646ac: ldr      r3, [sp, #0xc]
003646b0: ldr      r1, [sp, #4]
003646b4: str      r2, [r4, #0x1c]
003646b8: str      r3, [r4, #0x20]
003646bc: str      r1, [r4, #0x18]
003646c0: str      r5, [r4, #0x14]
003646c4: add      sp, sp, #0x14
003646c8: pop      {r4, r5, r6, r7, pc}
003646cc: mov      r3, #0
003646d0: str      r3, [r4, #0x2c]
003646d4: str      r3, [r4, #0x24]
003646d8: str      r3, [r4, #0x28]
003646dc: b        #0x3646a8
003646e0: ldr      r2, [r3]
003646e4: ldr      ip, [r2, #0x7c]
003646e8: mov      r2, r5
003646ec: b        #0x364658
003646f0: mov      r0, r3
003646f4: ldr      r3, [r3]
003646f8: mov      lr, pc
003646fc: ldr      pc, [r3, #0xa0]
00364700: ldr      r3, [r0]
00364704: str      r3, [sp, #4]
00364708: ldr      r3, [r0, #4]
0036470c: str      r3, [sp, #8]
00364710: ldr      r3, [r0, #8]
00364714: str      r3, [sp, #0xc]
00364718: b        #0x364664

# _ZN13RootSceneNode19_EnableDisplacementEb
0035d4cc: push     {r4, r5, r6, r7, r8, lr}
0035d4d0: subs     r5, r1, #0
0035d4d4: mov      r4, r0
0035d4d8: beq      #0x35d4e8
0035d4dc: ldr      r6, [r0, #0x1f0]
0035d4e0: cmp      r6, #0
0035d4e4: beq      #0x35d4f0
0035d4e8: strb     r5, [r4, #0x1ec]
0035d4ec: pop      {r4, r5, r6, r7, r8, pc}
0035d4f0: mov      r1, r6
0035d4f4: bl       #0x35ccdc
0035d4f8: mov      r1, #1
0035d4fc: str      r0, [r4, #0x1f0]
0035d500: mov      r0, r4
0035d504: bl       #0x35ccdc
0035d508: ldr      r3, [r4, #0x1f0]
0035d50c: str      r0, [r4, #0x1f4]
0035d510: cmp      r3, #0
0035d514: beq      #0x35d61c
0035d518: cmp      r0, r3
0035d51c: streq    r6, [r4, #0x1f4]
0035d520: beq      #0x35d548
0035d524: cmp      r0, #0
0035d528: beq      #0x35d548
0035d52c: ldr      r3, [r0]
0035d530: ldr      r3, [r3, #-0xc]
0035d534: add      r0, r0, r3
0035d538: ldr      r3, [r0, #4]
0035d53c: add      r3, r3, #1
0035d540: str      r3, [r0, #4]
0035d544: ldr      r3, [r4, #0x1f0]
0035d548: ldr      r2, [r3]
0035d54c: mov      r1, #0
0035d550: mov      r0, #0x150
0035d554: ldr      r2, [r2, #-0xc]
0035d558: mov      r7, r4
0035d55c: add      r3, r3, r2
0035d560: ldr      r2, [r3, #4]
0035d564: add      r2, r2, #1
0035d568: str      r2, [r3, #4]
0035d56c: bl       #0x5341ac
0035d570: mvn      r1, #0
0035d574: mov      r6, r0
0035d578: bl       #0x5839d8
0035d57c: str      r6, [r4, #0x1f8]
0035d580: mov      r3, r6
0035d584: ldr      r6, [r7, #0xf4]!
0035d588: cmp      r6, r7
0035d58c: bne      #0x35d598
0035d590: b        #0x35d5c4
0035d594: ldr      r3, [r4, #0x1f8]
0035d598: cmp      r6, #0
0035d59c: moveq    r1, r6
0035d5a0: subne    r1, r6, #4
0035d5a4: ldr      r6, [r6]
0035d5a8: mov      r0, r3
0035d5ac: ldr      r3, [r3]
0035d5b0: mov      lr, pc
0035d5b4: ldr      pc, [r3, #0x5c]
0035d5b8: cmp      r7, r6
0035d5bc: bne      #0x35d594
0035d5c0: ldr      r3, [r4, #0x1f8]
0035d5c4: mov      r1, r3
0035d5c8: mov      r0, r4
0035d5cc: ldr      r3, [r4]
0035d5d0: mov      r7, r4
0035d5d4: mov      lr, pc
0035d5d8: ldr      pc, [r3, #0x5c]
0035d5dc: ldr      r6, [r7, #0xfc]!
0035d5e0: b        #0x35d608
0035d5e4: ldr      r0, [r6, #8]
0035d5e8: bl       #0x369160
0035d5ec: subs     r3, r0, #0
0035d5f0: beq      #0x35d614
0035d5f4: ldr      r3, [r3]
0035d5f8: ldr      r1, [r4, #0x1f0]
0035d5fc: mov      lr, pc
0035d600: ldr      pc, [r3, #8]
0035d604: ldr      r6, [r6]
0035d608: cmp      r7, r6
0035d60c: bne      #0x35d5e4
0035d610: b        #0x35d4e8
0035d614: mov      r5, r3
0035d618: b        #0x35d4e8
0035d61c: str      r3, [r4, #0x1f4]
0035d620: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12VisualObject11GetPositionER7Point3DIfE
00470be4: push     {r4, lr}
00470be8: ldr      r3, [r0, #8]
00470bec: mov      r4, r1
00470bf0: cmp      r3, #0
00470bf4: beq      #0x470c20
00470bf8: mov      r0, r3
00470bfc: ldr      r3, [r3]
00470c00: mov      lr, pc
00470c04: ldr      pc, [r3, #0xa0]
00470c08: ldr      r3, [r0]
00470c0c: str      r3, [r4]
00470c10: ldr      r3, [r0, #4]
00470c14: str      r3, [r4, #4]
00470c18: ldr      r3, [r0, #8]
00470c1c: str      r3, [r4, #8]
00470c20: pop      {r4, pc}

# _ZN12VisualObject12SyncPositionEv
00470cb8: ldr      r1, [r0, #4]
00470cbc: cmp      r1, #0
00470cc0: bxeq     lr
00470cc4: add      r1, r1, #0x160
00470cc8: b        #0x470c24

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

# _ZN14AnimApplicator10SetRefNodeEPN6glitch5scene10ISceneNodeE
0036473c: push     {r4, r5, r6, r7, r8, lr}
00364740: ldr      r3, [r0, #8]
00364744: mov      r5, r0
00364748: mov      r4, r1
0036474c: cmp      r3, #0
00364750: beq      #0x364764
00364754: ldr      r2, [r3]
00364758: ldr      r0, [r2, #-0xc]
0036475c: add      r0, r3, r0
00364760: bl       #0x31d584
00364764: cmp      r4, #0
00364768: str      r4, [r5, #8]
0036476c: beq      #0x364834
00364770: ldr      r3, [r4]
00364774: mvn      r2, #0
00364778: ldr      r3, [r3, #-0xc]
0036477c: add      r4, r4, r3
00364780: ldr      r3, [r4, #4]
00364784: add      r3, r3, #1
00364788: str      r3, [r4, #4]
0036478c: ldr      r3, [r5, #4]
00364790: str      r2, [r5, #0xc]
00364794: mov      r0, r3
00364798: ldr      r3, [r3]
0036479c: mov      lr, pc
003647a0: ldr      pc, [r3, #0x70]
003647a4: subs     r7, r0, #0
003647a8: beq      #0x364834
003647ac: mov      r4, #0
003647b0: b        #0x3647c0
003647b4: add      r4, r4, #1
003647b8: cmp      r7, r4
003647bc: beq      #0x364834
003647c0: ldr      r3, [r5, #4]
003647c4: mov      r1, r4
003647c8: mov      r0, r3
003647cc: ldr      r3, [r3]
003647d0: mov      lr, pc
003647d4: ldr      pc, [r3, #0x54]
003647d8: ldr      r3, [r5, #8]
003647dc: mov      r6, r0
003647e0: mov      r0, r3
003647e4: ldr      r3, [r3]
003647e8: mov      lr, pc
003647ec: ldr      pc, [r3, #0x54]
003647f0: mov      r1, r0
003647f4: ldr      r0, [r6, #4]
003647f8: bl       #0x30e31c
003647fc: cmp      r0, #0
00364800: bne      #0x3647b4
00364804: ldr      r3, [r5, #4]
00364808: mov      r1, r4
0036480c: mov      r0, r3
00364810: ldr      r3, [r3]
00364814: mov      lr, pc
00364818: ldr      pc, [r3, #0x54]
0036481c: ldr      r3, [r0, #8]
00364820: cmp      r3, #1
00364824: streq    r4, [r5, #0xc]
00364828: add      r4, r4, #1
0036482c: cmp      r7, r4
00364830: bne      #0x3647c0
00364834: pop      {r4, r5, r6, r7, r8, pc}

# _Z16findSceneNodeRefPN6glitch5scene10ISceneNodeEPKc
0050f780: push     {r4, r5, r6, lr}
0050f784: mov      r5, r1
0050f788: ldr      r3, [r0]
0050f78c: mov      r4, r0
0050f790: mov      lr, pc
0050f794: ldr      pc, [r3, #0x24]
0050f798: mov      r1, r5
0050f79c: bl       #0x30e31c
0050f7a0: cmp      r0, #0
0050f7a4: bne      #0x50f7b0
0050f7a8: mov      r0, r4
0050f7ac: pop      {r4, r5, r6, pc}
0050f7b0: mov      r6, r4
0050f7b4: ldr      r4, [r6, #0xf4]!
0050f7b8: cmp      r4, r6
0050f7bc: bne      #0x50f7d0
0050f7c0: b        #0x50f7f4
0050f7c4: ldr      r4, [r4]
0050f7c8: cmp      r6, r4
0050f7cc: beq      #0x50f7f4
0050f7d0: cmp      r4, #0
0050f7d4: moveq    r0, r4
0050f7d8: subne    r0, r4, #4
0050f7dc: mov      r1, r5
0050f7e0: bl       #0x50f780
0050f7e4: cmp      r0, #0
0050f7e8: beq      #0x50f7c4
0050f7ec: mov      r4, r0
0050f7f0: b        #0x50f7a8
0050f7f4: mov      r4, #0
0050f7f8: b        #0x50f7a8

# _ZN10GameObject14SetDestinationERK7Point3DIfE
00393600: ldr      r3, [r1]
00393604: str      r3, [r0, #0x1a8]
00393608: ldr      r3, [r1, #4]
0039360c: str      r3, [r0, #0x1ac]
00393610: ldr      r3, [r1, #8]
00393614: str      r3, [r0, #0x1b0]
00393618: bx       lr

# _ZN12VisualObject13ApplyPositionEv
0047124c: push     {r4, r5, lr}
00471250: ldr      r3, [r0, #4]
00471254: sub      sp, sp, #0x14
00471258: mov      r4, r0
0047125c: cmp      r3, #0
00471260: beq      #0x471290
00471264: add      r5, sp, #4
00471268: mov      r3, #0
0047126c: mov      r1, r5
00471270: str      r3, [sp, #0xc]
00471274: str      r3, [sp, #4]
00471278: str      r3, [sp, #8]
0047127c: bl       #0x470be4
00471280: ldr      r0, [r4, #4]
00471284: mov      r1, r5
00471288: mov      r2, #0
0047128c: bl       #0x393db4
00471290: add      sp, sp, #0x14
00471294: pop      {r4, r5, pc}

# _ZN13RootSceneNode9onAnimateEj
0035d168: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035d16c: ldr      r7, [pc, #0x338]
0035d170: ldr      sl, [pc, #0x338]
0035d174: ldrb     r3, [r0, #0x209]
0035d178: add      r7, pc, r7
0035d17c: ldr      r2, [r7, sl]
0035d180: sub      sp, sp, #0x3c
0035d184: cmp      r3, #0
0035d188: ldr      r2, [r2]
0035d18c: mov      r5, r0
0035d190: mov      r6, r1
0035d194: str      r2, [sp, #0x34]
0035d198: ldreq    r2, [r0, #0x11c]
0035d19c: beq      #0x35d1b0
0035d1a0: ldr      r2, [r0, #0x11c]
0035d1a4: ands     r4, r2, #1
0035d1a8: movne    r3, #0
0035d1ac: beq      #0x35d310
0035d1b0: tst      r2, #0x400
0035d1b4: beq      #0x35d1e0
0035d1b8: tst      r2, #1
0035d1bc: bne      #0x35d1e0
0035d1c0: ldr      r3, [r7, sl]
0035d1c4: str      r6, [r5, #0x1fc]
0035d1c8: ldr      r2, [sp, #0x34]
0035d1cc: ldr      r3, [r3]
0035d1d0: cmp      r2, r3
0035d1d4: bne      #0x35d4a8
0035d1d8: add      sp, sp, #0x3c
0035d1dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035d1e0: tst      r2, #0x200
0035d1e4: beq      #0x35d1c0
0035d1e8: ldr      r2, [r5, #0x204]
0035d1ec: cmp      r2, #0
0035d1f0: beq      #0x35d330
0035d1f4: cmp      r3, #0
0035d1f8: bne      #0x35d330
0035d1fc: ldr      r3, [pc, #0x2b0]
0035d200: ldr      r3, [r7, r3]
0035d204: ldrb     r3, [r3, #0x30]
0035d208: cmp      r3, #0
0035d20c: bne      #0x35d330
0035d210: ldr      r4, [r2, #0x130]
0035d214: ldr      r3, [r2, #0x140]
0035d218: ldr      lr, [r2, #0x134]
0035d21c: ldr      r0, [r2, #0x138]
0035d220: ldr      r1, [r2, #0x13c]
0035d224: ldr      ip, [r2, #0x12c]
0035d228: str      r4, [sp, #4]
0035d22c: str      lr, [sp, #8]
0035d230: str      ip, [sp]
0035d234: str      r0, [sp, #0xc]
0035d238: str      r1, [sp, #0x10]
0035d23c: str      r3, [sp, #0x14]
0035d240: ldrb     r3, [r2, #0x2f9]
0035d244: cmp      r3, #0
0035d248: moveq    r4, sp
0035d24c: beq      #0x35d270
0035d250: ldr      r3, [r5]
0035d254: mov      r0, r5
0035d258: mov      lr, pc
0035d25c: ldr      pc, [r3, #0x34]
0035d260: mov      r1, r0
0035d264: mov      r0, sp
0035d268: mov      r4, sp
0035d26c: bl       #0x35c150
0035d270: ldr      r3, [pc, #0x240]
0035d274: ldr      r0, [r7, r3]
0035d278: bl       #0x582138
0035d27c: mov      r1, sp
0035d280: bl       #0x35bec8
0035d284: cmp      r0, #0
0035d288: bne      #0x35d330
0035d28c: ldr      r3, [r5, #0x11c]
0035d290: tst      r3, #0x400
0035d294: beq      #0x35d330
0035d298: ldrb     r3, [r5, #0x20a]
0035d29c: cmp      r3, #0
0035d2a0: movne    fp, #1
0035d2a4: bne      #0x35d334
0035d2a8: ldr      r3, [r5]
0035d2ac: mov      r0, r5
0035d2b0: mov      r1, r6
0035d2b4: mov      lr, pc
0035d2b8: ldr      pc, [r3, #0x18]
0035d2bc: ldrb     r3, [r5, #0x1ec]
0035d2c0: cmp      r3, #0
0035d2c4: bne      #0x35d454
0035d2c8: ldrb     r3, [r5, #0x208]
0035d2cc: cmp      r3, #0
0035d2d0: beq      #0x35d2f0
0035d2d4: ldr      r3, [r5]
0035d2d8: mov      r0, r5
0035d2dc: mov      r1, #1
0035d2e0: mov      lr, pc
0035d2e4: ldr      pc, [r3, #0xb8]
0035d2e8: mov      r3, #0
0035d2ec: strb     r3, [r5, #0x208]
0035d2f0: ldr      r3, [pc, #0x1c4]
0035d2f4: mov      fp, #0
0035d2f8: ldr      r3, [r7, r3]
0035d2fc: ldr      r2, [r3]
0035d300: add      r2, r2, #1
0035d304: str      r2, [r3]
0035d308: strb     fp, [r5, #0x20a]
0035d30c: b        #0x35d1c0
0035d310: mov      r1, #1
0035d314: bl       #0x596ec4
0035d318: ldr      r0, [r5, #0x110]
0035d31c: bl       #0x5890a8
0035d320: ldr      r2, [r5, #0x11c]
0035d324: strb     r4, [r5, #0x209]
0035d328: mov      r3, #1
0035d32c: b        #0x35d1b0
0035d330: mov      fp, #0
0035d334: ldr      r3, [pc, #0x184]
0035d338: ldr      r2, [pc, #0x184]
0035d33c: add      r4, sp, #0x1c
0035d340: ldr      r3, [r7, r3]
0035d344: ldr      sb, [r7, r2]
0035d348: mov      r8, r5
0035d34c: ldr      r2, [r3]
0035d350: mov      r0, sb
0035d354: add      r2, r2, #1
0035d358: str      r2, [r3]
0035d35c: bl       #0x337888
0035d360: ldr      r1, [pc, #0x160]
0035d364: add      r2, sp, #0x18
0035d368: mov      r0, r4
0035d36c: add      r1, pc, r1
0035d370: bl       #0x3140ec
0035d374: mov      r1, r4
0035d378: mov      r0, sb
0035d37c: bl       #0x337a88
0035d380: mov      r0, r4
0035d384: bl       #0x3139ac
0035d388: ldr      r4, [r8, #0xfc]!
0035d38c: b        #0x35d3b0
0035d390: ldr      r3, [r4, #8]
0035d394: mov      r1, r5
0035d398: mov      r2, r6
0035d39c: mov      r0, r3
0035d3a0: ldr      r3, [r3]
0035d3a4: mov      lr, pc
0035d3a8: ldr      pc, [r3, #0x10]
0035d3ac: ldr      r4, [r4]
0035d3b0: cmp      r8, r4
0035d3b4: bne      #0x35d390
0035d3b8: ldrb     r3, [r5, #0x1ec]
0035d3bc: cmp      r3, #0
0035d3c0: bne      #0x35d42c
0035d3c4: ldr      r3, [r5, #0x204]
0035d3c8: cmp      r3, #0
0035d3cc: beq      #0x35d43c
0035d3d0: ldr      r3, [r3, #0x110]
0035d3d4: cmn      r3, #1
0035d3d8: beq      #0x35d43c
0035d3dc: mov      r8, r5
0035d3e0: ldr      r4, [r8, #0xf4]!
0035d3e4: b        #0x35d40c
0035d3e8: cmp      r4, #0
0035d3ec: moveq    r3, r4
0035d3f0: subne    r3, r4, #4
0035d3f4: mov      r0, r3
0035d3f8: mov      r1, r6
0035d3fc: ldr      r3, [r3]
0035d400: mov      lr, pc
0035d404: ldr      pc, [r3, #0x14]
0035d408: ldr      r4, [r4]
0035d40c: cmp      r4, r8
0035d410: bne      #0x35d3e8
0035d414: ldr      r3, [r5, #0x11c]
0035d418: eor      fp, fp, #1
0035d41c: strb     fp, [r5, #0x20a]
0035d420: bic      r3, r3, #0x20
0035d424: str      r3, [r5, #0x11c]
0035d428: b        #0x35d1c0
0035d42c: mov      r0, r5
0035d430: mov      r1, r6
0035d434: bl       #0x35cf48
0035d438: b        #0x35d3c4
0035d43c: ldr      r3, [r5]
0035d440: mov      r0, r5
0035d444: mov      r1, #0
0035d448: mov      lr, pc
0035d44c: ldr      pc, [r3, #0xb8]
0035d450: b        #0x35d3dc
0035d454: mov      r8, r5
0035d458: ldr      r4, [r8, #0xfc]!
0035d45c: b        #0x35d47c
0035d460: ldr      r0, [r4, #8]
0035d464: bl       #0x369160
0035d468: mov      r1, r6
0035d46c: ldr      r3, [r0]
0035d470: mov      lr, pc
0035d474: ldr      pc, [r3, #0x10]
0035d478: ldr      r4, [r4]
0035d47c: cmp      r8, r4
0035d480: bne      #0x35d460
0035d484: mov      r0, r5
0035d488: mov      r1, r6
0035d48c: bl       #0x35cf48
0035d490: ldr      r3, [r5, #0x11c]
0035d494: cmp      r0, #0
0035d498: bic      r3, r3, #0x20
0035d49c: str      r3, [r5, #0x11c]
0035d4a0: bne      #0x35d2d4
0035d4a4: b        #0x35d2c8
0035d4a8: bl       #0x30e310
0035d4ac: rsbeq    r7, r3, r8, lsl sb
0035d4b0: andeq    r4, r0, ip, lsr #1
0035d4b4: andeq    r1, r0, r0, lsr #20
0035d4b8: muleq    r0, ip, r5
0035d4bc: muleq    r0, r4, r7
0035d4c0: andeq    r0, r0, r0, asr #30
0035d4c4: andeq    r0, r0, r4, lsl #17
0035d4c8: subseq   r3, r6, ip, lsl #19

# _ZN6glitch5scene10ISceneNode11setPositionEfff
00597154: str      lr, [sp, #-4]!
00597158: sub      sp, sp, #0x14
0059715c: str      r1, [sp, #4]
00597160: str      r3, [sp, #0xc]
00597164: str      r2, [sp, #8]
00597168: ldr      r3, [r0]
0059716c: add      r1, sp, #4
00597170: mov      lr, pc
00597174: ldr      pc, [r3, #0xa4]
00597178: add      sp, sp, #0x14
0059717c: ldm      sp!, {pc}

# _ZN13RootSceneNode11GetAnimRootEb
0035ccdc: cmp      r1, #0
0035cce0: push     {r4, lr}
0035cce4: mov      r4, r0
0035cce8: beq      #0x35cd08
0035ccec: ldr      r1, [pc, #0x58]
0035ccf0: mov      r0, r4
0035ccf4: add      r1, pc, r1
0035ccf8: bl       #0x50f780
0035ccfc: cmp      r0, #0
0035cd00: beq      #0x35cd20
0035cd04: pop      {r4, pc}
0035cd08: ldr      r1, [pc, #0x40]
0035cd0c: add      r1, pc, r1
0035cd10: bl       #0x50f780
0035cd14: cmp      r0, #0
0035cd18: beq      #0x35ccec
0035cd1c: pop      {r4, pc}
0035cd20: ldr      r1, [pc, #0x2c]
0035cd24: mov      r0, r4
0035cd28: add      r1, pc, r1
0035cd2c: bl       #0x50f780
0035cd30: cmp      r0, #0
0035cd34: bne      #0x35cd04
0035cd38: ldr      r1, [pc, #0x18]
0035cd3c: mov      r0, r4
0035cd40: add      r1, pc, r1
0035cd44: pop      {r4, lr}
0035cd48: b        #0x50f780

# _ZN13RootSceneNode10_CalcDeltaERN6glitch4core8vector3dIfEE
0035cd5c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0035cd60: ldr      r2, [pc, #0xec]
0035cd64: sub      sp, sp, #0x14
0035cd68: mov      r6, r0
0035cd6c: add      r2, pc, r2
0035cd70: str      r2, [sp, #0xc]
0035cd74: ldr      r5, [r6, #0xfc]!
0035cd78: mov      r3, #0
0035cd7c: str      r3, [r1, #8]
0035cd80: str      r3, [r1]
0035cd84: str      r3, [r1, #4]
0035cd88: ldr      r7, [pc, #0xc8]
0035cd8c: ldr      sl, [pc, #0xc8]
0035cd90: ldr      sb, [pc, #0xc8]
0035cd94: ldr      r3, [pc, #0xc8]
0035cd98: cmp      r6, r5
0035cd9c: add      r7, pc, r7
0035cda0: mov      r4, r1
0035cda4: add      sl, pc, sl
0035cda8: add      sb, pc, sb
0035cdac: ldr      r8, [pc, #0xb4]
0035cdb0: str      r3, [sp, #8]
0035cdb4: beq      #0x35ce04
0035cdb8: ldr      r0, [r5, #8]
0035cdbc: bl       #0x369160
0035cdc0: subs     fp, r0, #0
0035cdc4: beq      #0x35ce0c
0035cdc8: ldr      r1, [fp, #0x24]
0035cdcc: ldr      r0, [r4]
0035cdd0: bl       #0x30eba4
0035cdd4: str      r0, [r4]
0035cdd8: ldr      r1, [fp, #0x28]
0035cddc: ldr      r0, [r4, #4]
0035cde0: bl       #0x30eba4
0035cde4: str      r0, [r4, #4]
0035cde8: ldr      r1, [fp, #0x2c]
0035cdec: ldr      r0, [r4, #8]
0035cdf0: bl       #0x30eba4
0035cdf4: str      r0, [r4, #8]
0035cdf8: ldr      r5, [r5]
0035cdfc: cmp      r6, r5
0035ce00: bne      #0x35cdb8
0035ce04: add      sp, sp, #0x14
0035ce08: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0035ce0c: ldr      r3, [r7, r8]
0035ce10: ldr      r3, [r3]
0035ce14: cmp      r3, #2
0035ce18: streq    fp, [fp]
0035ce1c: beq      #0x35cdf8
0035ce20: cmp      r3, #1
0035ce24: bne      #0x35cdf8
0035ce28: ldr      ip, [sp, #8]
0035ce2c: mov      r1, sl
0035ce30: mov      r2, sb
0035ce34: ldr      r0, [r7, ip]
0035ce38: ldr      r3, [sp, #0xc]
0035ce3c: mov      ip, #0xf4
0035ce40: add      r0, r0, #0xa8
0035ce44: str      ip, [sp]
0035ce48: bl       #0x30e004
0035ce4c: ldr      r5, [r5]
0035ce50: b        #0x35cdfc
0035ce54: subseq   r3, r6, ip, lsr pc

# _ZN12VisualObject6UpdateEv
00470cf0: bx       lr

# _ZN12CharAnimator6UpdateEv
003caf3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003caf40: ldr      r5, [pc, #0x37c]
003caf44: ldr      r6, [pc, #0x37c]
003caf48: mov      r4, r0
003caf4c: add      r5, pc, r5
003caf50: ldr      r3, [r5, r6]
003caf54: ldr      r0, [pc, #0x370]
003caf58: sub      sp, sp, #0x78
003caf5c: ldr      r3, [r3]
003caf60: add      r0, pc, r0
003caf64: str      r3, [sp, #0x74]
003caf68: bl       #0x3136b4
003caf6c: ldr      r3, [r4, #4]
003caf70: ldr      r3, [r3, #0x520]
003caf74: tst      r3, #0x200
003caf78: bne      #0x3cafb8
003caf7c: ldrb     r3, [r4, #0x5c]
003caf80: cmp      r3, #0
003caf84: bne      #0x3cb128
003caf88: ldr      r0, [pc, #0x340]
003caf8c: mov      r3, #0
003caf90: strb     r3, [r4, #0x5c]
003caf94: add      r0, pc, r0
003caf98: bl       #0x3136b8
003caf9c: ldr      r3, [r5, r6]
003cafa0: ldr      r2, [sp, #0x74]
003cafa4: ldr      r3, [r3]
003cafa8: cmp      r2, r3
003cafac: bne      #0x3cb2c0
003cafb0: add      sp, sp, #0x78
003cafb4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003cafb8: ldrb     r3, [r4, #0x5c]
003cafbc: cmp      r3, #0
003cafc0: beq      #0x3cb11c
003cafc4: ldrb     r3, [r4, #0x4a]
003cafc8: mov      r2, #1
003cafcc: strb     r2, [r4, #0x5c]
003cafd0: cmp      r3, #0
003cafd4: movne    r3, #0
003cafd8: strbne   r3, [r4, #0x4a]
003cafdc: strbne   r2, [r4, #0x49]
003cafe0: beq      #0x3cb134
003cafe4: ldr      sl, [r4, #0x2c]
003cafe8: ldr      r3, [pc, #0x2e4]
003cafec: mov      r8, #0xc
003caff0: mla      r8, r8, sl, r4
003caff4: ldr      r3, [r5, r3]
003caff8: ldr      r2, [r8, #8]
003caffc: mov      r7, #0x14
003cb000: ldr      r3, [r3]
003cb004: ldr      r0, [r4, #4]
003cb008: mov      r1, #0x27
003cb00c: mla      r7, r7, r2, r3
003cb010: mov      r2, #0
003cb014: bl       #0x3a4d5c
003cb018: ldr      r3, [r7, #0x10]
003cb01c: cmp      r3, #1
003cb020: bne      #0x3cb144
003cb024: ldr      r3, [r8, #0x10]
003cb028: ldr      r2, [r7, #8]
003cb02c: add      r3, r3, #1
003cb030: cmp      r3, r2
003cb034: beq      #0x3cb144
003cb038: mov      r8, #0xc
003cb03c: mla      r8, r8, sl, r4
003cb040: str      r3, [r8, #0x10]
003cb044: ldr      r2, [r7, #8]
003cb048: cmp      r2, r3
003cb04c: bhi      #0x3cb1e4
003cb050: mov      r2, #0xc
003cb054: mla      r2, r2, sl, r4
003cb058: ldr      r3, [r2, #0xc]
003cb05c: cmp      r3, #0
003cb060: beq      #0x3cb174
003cb064: subgt    r3, r3, #1
003cb068: strgt    r3, [r2, #0xc]
003cb06c: ldr      r3, [pc, #0x264]
003cb070: add      r7, sp, #0x44
003cb074: ldr      r8, [r5, r3]
003cb078: mov      r0, r8
003cb07c: bl       #0x337888
003cb080: ldr      r1, [pc, #0x254]
003cb084: add      r2, sp, #0xc
003cb088: mov      r0, r7
003cb08c: add      r1, pc, r1
003cb090: bl       #0x3140ec
003cb094: mov      r1, r7
003cb098: mov      r0, r8
003cb09c: bl       #0x337a88
003cb0a0: mov      r0, r7
003cb0a4: bl       #0x3139ac
003cb0a8: mov      r2, #0
003cb0ac: ldr      r0, [r4, #4]
003cb0b0: mov      r1, #0x23
003cb0b4: bl       #0x3a4d5c
003cb0b8: ldr      r2, [r4, #0x50]
003cb0bc: mov      r3, #0xc
003cb0c0: mla      r3, r3, sl, r4
003cb0c4: cmn      r2, #1
003cb0c8: ldr      r7, [r3, #0xc]
003cb0cc: beq      #0x3cb2ac
003cb0d0: mov      r3, #0xc
003cb0d4: mla      sl, r3, sl, r4
003cb0d8: cmp      r7, #0
003cb0dc: mvnlt    r3, #0
003cb0e0: strlt    r3, [sl, #0xc]
003cb0e4: strge    r7, [sl, #0xc]
003cb0e8: mov      r3, #0
003cb0ec: strb     r3, [r4, #0x49]
003cb0f0: ldr      r1, [r4, #0x50]
003cb0f4: cmn      r1, #1
003cb0f8: beq      #0x3cb10c
003cb0fc: mov      r0, r4
003cb100: bl       #0x3cacb0
003cb104: mvn      r3, #0
003cb108: str      r3, [r4, #0x50]
003cb10c: ldr      r0, [pc, #0x1cc]
003cb110: add      r0, pc, r0
003cb114: bl       #0x3136b8
003cb118: b        #0x3caf9c
003cb11c: mov      r0, r4
003cb120: bl       #0x3c9168
003cb124: b        #0x3cafc4
003cb128: mov      r0, r4
003cb12c: bl       #0x3c91c8
003cb130: b        #0x3caf88
003cb134: ldrb     r3, [r4, #0x49]
003cb138: cmp      r3, #0
003cb13c: beq      #0x3cb0f0
003cb140: b        #0x3cafe4
003cb144: ldr      r0, [r4, #4]
003cb148: mov      r1, #0x25
003cb14c: mov      r2, #0
003cb150: bl       #0x3a4d5c
003cb154: ldr      r3, [r7, #0x10]
003cb158: cmp      r3, #1
003cb15c: bne      #0x3cb050
003cb160: add      r3, r3, #0xb
003cb164: mla      r3, r3, sl, r4
003cb168: ldr      r3, [r3, #0x10]
003cb16c: add      r3, r3, #1
003cb170: b        #0x3cb038
003cb174: ldr      r3, [r4, #0x2c]
003cb178: cmp      r3, #0
003cb17c: bne      #0x3cb258
003cb180: ldrb     r7, [r4, #0x48]
003cb184: cmp      r7, #0
003cb188: bne      #0x3cb0e8
003cb18c: ldr      r3, [pc, #0x144]
003cb190: add      r8, sp, #0x14
003cb194: ldr      sl, [r5, r3]
003cb198: mov      r0, sl
003cb19c: bl       #0x337888
003cb1a0: ldr      r1, [pc, #0x13c]
003cb1a4: add      r2, sp, #4
003cb1a8: mov      r0, r8
003cb1ac: add      r1, pc, r1
003cb1b0: bl       #0x3140ec
003cb1b4: mov      r1, r8
003cb1b8: mov      r0, sl
003cb1bc: bl       #0x337a88
003cb1c0: mov      r0, r8
003cb1c4: bl       #0x3139ac
003cb1c8: mov      r3, #1
003cb1cc: strb     r3, [r4, #0x48]
003cb1d0: mov      r2, r7
003cb1d4: ldr      r0, [r4, #4]
003cb1d8: mov      r1, #0x22
003cb1dc: bl       #0x3a4d5c
003cb1e0: b        #0x3cb0e8
003cb1e4: ldr      r3, [pc, #0xec]
003cb1e8: add      sl, sp, #0x5c
003cb1ec: ldr      sb, [r5, r3]
003cb1f0: mov      r0, sb
003cb1f4: bl       #0x337888
003cb1f8: ldr      r1, [pc, #0xe8]
003cb1fc: add      r2, sp, #0x10
003cb200: mov      r0, sl
003cb204: add      r1, pc, r1
003cb208: bl       #0x3140ec
003cb20c: mov      r1, sl
003cb210: mov      r0, sb
003cb214: bl       #0x337a88
003cb218: mov      r0, sl
003cb21c: bl       #0x3139ac
003cb220: mov      r1, #0x23
003cb224: ldr      r0, [r4, #4]
003cb228: mov      r2, #0
003cb22c: bl       #0x3a4d5c
003cb230: ldr      r1, [r8, #0x10]
003cb234: ldr      r3, [r7, #8]
003cb238: cmp      r1, r3
003cb23c: bhs      #0x3cb24c
003cb240: mov      r0, r4
003cb244: bl       #0x3ca79c
003cb248: b        #0x3cb0e8
003cb24c: mov      r0, r4
003cb250: bl       #0x3caf3c
003cb254: b        #0x3cb0e8
003cb258: ldr      r3, [pc, #0x78]
003cb25c: add      r7, sp, #0x2c
003cb260: ldr      r8, [r5, r3]
003cb264: mov      r0, r8
003cb268: bl       #0x337888
003cb26c: ldr      r1, [pc, #0x78]
003cb270: add      r2, sp, #8
003cb274: mov      r0, r7
003cb278: add      r1, pc, r1
003cb27c: bl       #0x3140ec
003cb280: mov      r1, r7
003cb284: mov      r0, r8
003cb288: bl       #0x337a88
003cb28c: mov      r0, r7
003cb290: bl       #0x3139ac
003cb294: ldr      r3, [r4, #0x2c]
003cb298: mov      r0, r4
003cb29c: sub      r3, r3, #1
003cb2a0: str      r3, [r4, #0x2c]
003cb2a4: bl       #0x3caf3c
003cb2a8: b        #0x3cb0e8
003cb2ac: ldr      r1, [r3, #8]
003cb2b0: mov      r0, r4
003cb2b4: ldr      r2, [r4, #0x2c]
003cb2b8: bl       #0x3cab38
003cb2bc: b        #0x3cb0d0
003cb2c0: bl       #0x30e310
003cb2c4: subseq   sb, ip, r4, asr #22
003cb2c8: andeq    r4, r0, ip, lsr #1
003cb2cc: subeq    sl, pc, r0, asr #4
003cb2d0: subeq    sl, pc, ip, lsl #4
003cb2d4: andeq    r3, r0, ip, ror ip
003cb2d8: andeq    r0, r0, r4, lsl #17
003cb2dc: subeq    sb, pc, r4, asr #31
003cb2e0: umaaleq  sl, pc, r0, r0
003cb2e4: subeq    sb, pc, r4, lsr #29
003cb2e8: subeq    sb, pc, ip, asr #28
003cb2ec: ldrdeq   sb, sl, [pc], #-0xd8

# _ZN12CharAnimator10__CallbackEPN6glitch5scene19ITimelineControllerEPv
003c90ec: mov      r3, #1
003c90f0: strb     r3, [r1, #0x49]
003c90f4: bx       lr

# _ZN12VisualObject11SetRotationERK7Point3DIfE
00472874: push     {r4, r5, r6, lr}
00472878: ldr      r3, [r0, #8]
0047287c: sub      sp, sp, #0x10
00472880: mov      r4, r0
00472884: cmp      r3, #0
00472888: beq      #0x472900
0047288c: ldr      r2, [r1]
00472890: ldr      r3, [r1, #8]
00472894: mov      r0, sp
00472898: add      r2, r2, #0x80000000
0047289c: ldr      r1, [r1, #4]
004728a0: add      r3, r3, #0x80000000
004728a4: bl       #0x35c9d8
004728a8: ldr      r3, [r4, #8]
004728ac: mov      r5, sp
004728b0: mov      r0, r3
004728b4: ldr      r3, [r3]
004728b8: mov      lr, pc
004728bc: ldr      pc, [r3, #0x98]
004728c0: ldr      r1, [sp]
004728c4: mov      r6, r0
004728c8: ldr      r0, [r0]
004728cc: bl       #0x30df8c
004728d0: cmp      r0, #0
004728d4: bne      #0x472908
004728d8: ldr      r3, [r4, #8]
004728dc: mov      r1, sp
004728e0: mov      r0, r3
004728e4: ldr      r3, [r3]
004728e8: mov      lr, pc
004728ec: ldr      pc, [r3, #0x9c]
004728f0: mov      r0, r4
004728f4: bl       #0x47211c
004728f8: mov      r0, r4
004728fc: bl       #0x470a54
00472900: add      sp, sp, #0x10
00472904: pop      {r4, r5, r6, pc}
00472908: ldr      r0, [r6, #4]
0047290c: ldr      r1, [sp, #4]
00472910: bl       #0x30df8c
00472914: cmp      r0, #0
00472918: beq      #0x4728d8
0047291c: ldr      r0, [r6, #8]
00472920: ldr      r1, [sp, #8]
00472924: bl       #0x30df8c
00472928: cmp      r0, #0
0047292c: beq      #0x4728d8
00472930: ldr      r0, [r6, #0xc]
00472934: ldr      r1, [sp, #0xc]
00472938: bl       #0x30df8c
0047293c: cmp      r0, #0
00472940: bne      #0x472900
00472944: b        #0x4728d8

# _ZN12CharAnimator8_SetAnimEij
003cab38: push     {r4, r5, r6, r7, r8, sl, lr}
003cab3c: ldr      r4, [pc, #0x150]
003cab40: ldr      r5, [pc, #0x150]
003cab44: sub      sp, sp, #0x44
003cab48: add      r4, pc, r4
003cab4c: ldr      r3, [r4, r5]
003cab50: cmp      r1, #0
003cab54: mov      r6, r0
003cab58: ldr      r3, [r3]
003cab5c: str      r3, [sp, #0x3c]
003cab60: blt      #0x3cab80
003cab64: ldr      r3, [pc, #0x130]
003cab68: ldr      r3, [r4, r3]
003cab6c: ldr      r3, [r3]
003cab70: cmp      r1, r3
003cab74: bge      #0x3cab80
003cab78: cmp      r2, #2
003cab7c: bls      #0x3cab9c
003cab80: ldr      r3, [r4, r5]
003cab84: ldr      r2, [sp, #0x3c]
003cab88: ldr      r3, [r3]
003cab8c: cmp      r2, r3
003cab90: bne      #0x3cac90
003cab94: add      sp, sp, #0x44
003cab98: pop      {r4, r5, r6, r7, r8, sl, pc}
003cab9c: ldr      r3, [pc, #0xfc]
003caba0: str      r1, [r0, #0x4c]
003caba4: mov      r8, #0x14
003caba8: ldr      r0, [r4, r3]
003cabac: mov      r3, #0xc
003cabb0: mla      r3, r3, r2, r6
003cabb4: ldr      r0, [r0]
003cabb8: str      r2, [r6, #0x2c]
003cabbc: ldr      r2, [pc, #0xe0]
003cabc0: mla      r8, r8, r1, r0
003cabc4: str      r1, [r3, #8]
003cabc8: ldr      sl, [r4, r2]
003cabcc: ldr      r2, [r8, #4]
003cabd0: add      r7, sp, #0x24
003cabd4: mov      r0, sl
003cabd8: str      r2, [r3, #0xc]
003cabdc: bl       #0x337888
003cabe0: ldr      r1, [pc, #0xc0]
003cabe4: add      r2, sp, #8
003cabe8: mov      r0, r7
003cabec: add      r1, pc, r1
003cabf0: bl       #0x3140ec
003cabf4: mov      r1, r7
003cabf8: mov      r0, sl
003cabfc: bl       #0x337a88
003cac00: mov      r0, r7
003cac04: bl       #0x3139ac
003cac08: ldr      r0, [r6, #4]
003cac0c: mov      r1, #0x24
003cac10: mov      r2, #0
003cac14: bl       #0x3a4d5c
003cac18: ldr      r3, [r8, #0x10]
003cac1c: cmp      r3, #2
003cac20: beq      #0x3cac34
003cac24: mov      r0, r6
003cac28: mov      r1, #0
003cac2c: bl       #0x3ca79c
003cac30: b        #0x3cab80
003cac34: mov      r0, sl
003cac38: bl       #0x337888
003cac3c: ldr      r1, [pc, #0x68]
003cac40: add      r7, sp, #0xc
003cac44: add      r2, sp, #4
003cac48: add      r1, pc, r1
003cac4c: mov      r0, r7
003cac50: bl       #0x3140ec
003cac54: mov      r0, sl
003cac58: mov      r1, r7
003cac5c: bl       #0x337a88
003cac60: mov      sl, r0
003cac64: eor      sl, sl, #1
003cac68: mov      r0, r7
003cac6c: bl       #0x3139ac
003cac70: tst      sl, #0xff
003cac74: beq      #0x3cac24
003cac78: ldr      r0, [r8, #8]
003cac7c: bl       #0x3ca708
003cac80: mov      r1, r0
003cac84: mov      r0, r6
003cac88: bl       #0x3ca79c
003cac8c: b        #0x3cab80
003cac90: bl       #0x30e310
003cac94: subseq   sb, ip, r8, asr #30
003cac98: andeq    r4, r0, ip, lsr #1
003cac9c: andeq    r2, r0, r8, asr #20
003caca0: andeq    r3, r0, ip, ror ip
003caca4: andeq    r0, r0, r4, lsl #17
003caca8: subeq    sl, pc, r4, ror #8
003cacac: umaaleq  r8, pc, r8, pc

# _ZN12VisualObject12SyncRotationEv
00472948: ldr      r1, [r0, #4]
0047294c: cmp      r1, #0
00472950: bxeq     lr
00472954: add      r1, r1, #0x16c
00472958: b        #0x472874

# _ZN12CharAnimator12_SetAnimStepEj
003ca79c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ca7a0: ldr      r3, [r0, #0x2c]
003ca7a4: mov      r2, #0xc
003ca7a8: ldr      r5, [pc, #0x374]
003ca7ac: mla      r3, r2, r3, r0
003ca7b0: ldr      r2, [pc, #0x370]
003ca7b4: add      r5, pc, r5
003ca7b8: mov      r4, r0
003ca7bc: ldr      r2, [r5, r2]
003ca7c0: ldr      r0, [r3, #8]
003ca7c4: mov      ip, #0x14
003ca7c8: ldr      r2, [r2]
003ca7cc: sub      sp, sp, #0x24
003ca7d0: mla      r2, ip, r0, r2
003ca7d4: ldr      r0, [r2, #8]
003ca7d8: cmp      r0, r1
003ca7dc: bhi      #0x3ca7e8
003ca7e0: add      sp, sp, #0x24
003ca7e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ca7e8: ldr      r2, [r2, #0xc]
003ca7ec: mov      r6, #0x38
003ca7f0: str      r1, [r3, #0x10]
003ca7f4: mla      r6, r6, r1, r2
003ca7f8: ldr      r0, [r4, #4]
003ca7fc: mov      r1, #0x26
003ca800: mov      r2, #0
003ca804: bl       #0x3a4d5c
003ca808: ldr      r3, [r6, #0x28]
003ca80c: cmp      r3, #1
003ca810: beq      #0x3caad8
003ca814: ldr      r3, [r6, #0x10]
003ca818: cmn      r3, #1
003ca81c: beq      #0x3ca9d8
003ca820: ldr      r3, [pc, #0x304]
003ca824: ldr      r0, [r5, r3]
003ca828: bl       #0x31f594
003ca82c: cmp      r0, #0
003ca830: beq      #0x3ca870
003ca834: ldr      r7, [r0, #0x128]
003ca838: cmp      r7, #0
003ca83c: beq      #0x3ca870
003ca840: mov      r0, r7
003ca844: ldr      r1, [r4, #4]
003ca848: bl       #0x40f980
003ca84c: cmp      r0, #0
003ca850: beq      #0x3ca870
003ca854: ldr      r1, [r6, #0x10]
003ca858: cmn      r1, #1
003ca85c: beq      #0x3ca9f4
003ca860: mov      r0, r7
003ca864: mov      r2, #0
003ca868: mov      r3, #1
003ca86c: bl       #0x40f904
003ca870: ldrb     r3, [r6, #0x34]
003ca874: strb     r3, [r4, #0x30]
003ca878: ldrb     r3, [r6, #0x34]
003ca87c: cmp      r3, #0
003ca880: moveq    r7, #1
003ca884: bne      #0x3caa10
003ca888: ldr      r3, [pc, #0x2a0]
003ca88c: ldr      r0, [r4, #4]
003ca890: ldr      sb, [r6, #0x2c]
003ca894: ldr      r3, [r5, r3]
003ca898: ldr      fp, [r3]
003ca89c: bl       #0x3935dc
003ca8a0: ldr      lr, [r0]
003ca8a4: ldr      r8, [r0, #4]
003ca8a8: ldr      sl, [r0, #8]
003ca8ac: mov      ip, #0xbf000000
003ca8b0: add      ip, ip, #0x800000
003ca8b4: str      lr, [sp, #0x14]
003ca8b8: mov      r0, fp
003ca8bc: mov      lr, #1
003ca8c0: mov      r1, sb
003ca8c4: add      r2, sp, #0x14
003ca8c8: mov      r3, #0
003ca8cc: str      r8, [sp, #0x18]
003ca8d0: str      sl, [sp, #0x1c]
003ca8d4: str      lr, [sp]
003ca8d8: str      ip, [sp, #8]
003ca8dc: str      ip, [sp, #4]
003ca8e0: bl       #0x36b5d8
003ca8e4: cmp      r7, #0
003ca8e8: beq      #0x3ca91c
003ca8ec: ldr      r8, [r6, #0x18]
003ca8f0: cmn      r8, #1
003ca8f4: beq      #0x3ca91c
003ca8f8: ldrb     r7, [r6, #4]
003ca8fc: cmp      r7, #0
003ca900: beq      #0x3caa94
003ca904: ldr      r3, [pc, #0x228]
003ca908: mov      r1, r8
003ca90c: ldr      r2, [r4, #4]
003ca910: ldr      r0, [r5, r3]
003ca914: mov      r3, #0
003ca918: bl       #0x495f04
003ca91c: ldr      r2, [r6, #0x30]
003ca920: ldr      r3, [r4, #4]
003ca924: str      r2, [r4, #0x34]
003ca928: ldr      r5, [r3, #0x2d8]
003ca92c: cmp      r5, #0
003ca930: beq      #0x3ca9e8
003ca934: ldr      r3, [r6, #8]
003ca938: cmn      r3, #1
003ca93c: beq      #0x3ca9e8
003ca940: mov      r3, #0
003ca944: strb     r3, [r4, #0x48]
003ca948: mov      r0, r4
003ca94c: bl       #0x3c9b7c
003ca950: ldrb     r3, [r4, #0x54]
003ca954: cmp      r3, #0
003ca958: beq      #0x3caa80
003ca95c: ldrb     r1, [r4, #0x49]
003ca960: ldr      r3, [r5, #0x38]
003ca964: ldrb     r2, [r6, #0x1c]
003ca968: cmp      r1, #0
003ca96c: beq      #0x3caac4
003ca970: mov      r1, #0
003ca974: str      r1, [r3, #0x14]
003ca978: mov      r1, #0
003ca97c: str      r1, [r3, #0xc]
003ca980: strb     r2, [r3, #0x10]
003ca984: ldr      r2, [r5, #0x38]
003ca988: ldr      r1, [r6, #8]
003ca98c: mov      r6, #0
003ca990: ldr      r3, [r4, #0x44]
003ca994: ldr      ip, [r2]
003ca998: mov      r0, r2
003ca99c: str      r6, [sp]
003ca9a0: mov      r2, r6
003ca9a4: mov      lr, pc
003ca9a8: ldr      pc, [ip, #0x1c]
003ca9ac: ldr      r1, [r4, #0x34]
003ca9b0: ldr      r0, [r4, #0x40]
003ca9b4: bl       #0x30ed6c
003ca9b8: ldr      r5, [r5, #0x38]
003ca9bc: mov      r1, r0
003ca9c0: mov      r2, r6
003ca9c4: mov      r0, r5
003ca9c8: ldr      r3, [r5]
003ca9cc: mov      lr, pc
003ca9d0: ldr      pc, [r3, #0x28]
003ca9d4: b        #0x3ca7e0
003ca9d8: ldr      r3, [r6, #0x20]
003ca9dc: cmp      r3, #0
003ca9e0: beq      #0x3ca870
003ca9e4: b        #0x3ca820
003ca9e8: mov      r0, r4
003ca9ec: bl       #0x3c9924
003ca9f0: b        #0x3ca7e0
003ca9f4: ldr      r0, [r6, #0x20]
003ca9f8: cmp      r0, #0
003ca9fc: beq      #0x3ca870
003caa00: ldr      r8, [r6, #0x24]
003caa04: bl       #0x3ca708
003caa08: ldr      r1, [r8, r0, lsl #2]
003caa0c: b        #0x3ca860
003caa10: ldr      r0, [r4, #4]
003caa14: mov      r1, #1
003caa18: add      r0, r0, #0x37c
003caa1c: bl       #0x3ffe3c
003caa20: mov      r7, r0
003caa24: ldr      r0, [r4, #4]
003caa28: mov      r1, #2
003caa2c: add      r0, r0, #0x37c
003caa30: bl       #0x3ffe3c
003caa34: mov      r1, r7
003caa38: mov      sl, r0
003caa3c: mov      r0, r4
003caa40: bl       #0x3c94b8
003caa44: mov      r1, r7
003caa48: eor      r8, r0, #1
003caa4c: ldrb     r2, [r6, #4]
003caa50: mov      r0, r4
003caa54: bl       #0x3c955c
003caa58: uxtb     r8, r8
003caa5c: eor      r0, r0, #1
003caa60: cmp      r8, #0
003caa64: uxtb     r7, r0
003caa68: bne      #0x3caaf0
003caa6c: cmp      r7, #0
003caa70: bne      #0x3cab08
003caa74: cmp      r8, #0
003caa78: beq      #0x3ca8e4
003caa7c: b        #0x3ca888
003caa80: ldr      r2, [r5, #0x38]
003caa84: ldrb     r1, [r6, #0x1c]
003caa88: str      r3, [r2, #0xc]
003caa8c: strb     r1, [r2, #0x10]
003caa90: b        #0x3ca984
003caa94: ldr      r0, [r4, #4]
003caa98: bl       #0x3935dc
003caa9c: ldr      r3, [r4, #4]
003caaa0: mov      r2, r0
003caaa4: ldr      r0, [pc, #0x88]
003caaa8: mov      r1, r8
003caaac: add      r3, r3, #0x16c
003caab0: ldr      r0, [r5, r0]
003caab4: str      r7, [sp, #4]
003caab8: str      r7, [sp]
003caabc: bl       #0x495888
003caac0: b        #0x3ca91c
003caac4: ldrb     r1, [r4, #0x4a]
003caac8: cmp      r1, #0
003caacc: ldreq    r1, [r6, #0xc]
003caad0: beq      #0x3ca974
003caad4: b        #0x3ca970
003caad8: ldr      r2, [r4, #0x2c]
003caadc: mov      r0, r4
003caae0: ldr      r1, [r6, #8]
003caae4: add      r2, r2, #1
003caae8: bl       #0x3cab38
003caaec: b        #0x3ca7e0
003caaf0: mov      r0, r4
003caaf4: mov      r1, sl
003caaf8: bl       #0x3c94b8
003caafc: eor      r0, r0, #1
003cab00: uxtb     r8, r0
003cab04: b        #0x3caa6c
003cab08: mov      r1, sl
003cab0c: mov      r0, r4
003cab10: ldrb     r2, [r6, #4]
003cab14: bl       #0x3c955c
003cab18: eor      r0, r0, #1
003cab1c: uxtb     r8, r0
003cab20: b        #0x3caa74
003cab24: ldrsbeq  sl, [ip], #-0x2c
003cab28: andeq    r3, r0, ip, ror ip
003cab2c: strdeq   r3, r4, [r0], -r4
003cab30: andeq    r0, r0, r4, lsr #27
003cab34: andeq    r1, r0, r8, lsl #22

# _ZN11AnimatorSet7_CBAnimEPN6glitch5scene19ITimelineControllerEPv
00367338: mov      r3, r0
0036733c: mov      r0, r1
00367340: mov      r1, r3
00367344: b        #0x3672c8

# _ZN15AnimatorBlender17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366628: push     {r4, r5, r6, lr}
0036662c: ldr      r2, [r0, #0x70]
00366630: ldr      r3, [r0, #0x28]
00366634: mov      r4, r0
00366638: mov      r6, r1
0036663c: ldr      r3, [r3, r2, lsl #2]
00366640: mov      r0, r3
00366644: ldr      r3, [r3]
00366648: mov      lr, pc
0036664c: ldr      pc, [r3, #0x44]
00366650: cmp      r0, r6
00366654: mov      r5, r0
00366658: beq      #0x366660
0036665c: pop      {r4, r5, r6, pc}
00366660: cmp      r0, #0
00366664: beq      #0x3666bc
00366668: mov      r1, #0x44000000
0036666c: add      r1, r1, #0x7a0000
00366670: ldr      r0, [r0, #0x1c]
00366674: bl       #0x30ed6c
00366678: bl       #0x30e4cc
0036667c: mov      r1, #0x44000000
00366680: mov      r6, r0
00366684: add      r1, r1, #0x7a0000
00366688: ldr      r0, [r5, #0x2c]
0036668c: bl       #0x30ed6c
00366690: bl       #0x30e4cc
00366694: ldr      r3, [r5, #4]
00366698: rsb      r0, r3, r0
0036669c: cmp      r0, r6
003666a0: movge    r3, #0
003666a4: movlt    r3, #1
003666a8: cmp      r0, #0
003666ac: movlt    r3, #0
003666b0: cmp      r3, #0
003666b4: rsbne    r3, r0, r6
003666b8: str      r3, [r4, #0x98]
003666bc: mov      r3, #1
003666c0: strb     r3, [r4, #0xb8]
003666c4: pop      {r4, r5, r6, pc}

# _ZN8Animator17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366224: push     {r4, r5, r6, lr}
00366228: subs     r4, r1, #0
0036622c: mov      r5, r0
00366230: beq      #0x366288
00366234: mov      r1, #0x44000000
00366238: add      r1, r1, #0x7a0000
0036623c: ldr      r0, [r4, #0x1c]
00366240: bl       #0x30ed6c
00366244: bl       #0x30e4cc
00366248: mov      r1, #0x44000000
0036624c: mov      r6, r0
00366250: add      r1, r1, #0x7a0000
00366254: ldr      r0, [r4, #0x2c]
00366258: bl       #0x30ed6c
0036625c: bl       #0x30e4cc
00366260: ldr      r3, [r4, #4]
00366264: rsb      r0, r3, r0
00366268: cmp      r0, r6
0036626c: movge    r3, #0
00366270: movlt    r3, #1
00366274: cmp      r0, #0
00366278: movlt    r3, #0
0036627c: cmp      r3, #0
00366280: rsbne    r3, r0, r6
00366284: str      r3, [r5, #0x68]
00366288: mov      r3, #1
0036628c: strb     r3, [r5, #0x88]
00366290: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada19CTimelineController6updateEi
00667104: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00667108: mov      r4, r0
0066710c: mov      r0, r1
00667110: bl       #0x30e964
00667114: mov      r1, #0x44000000
00667118: add      r1, r1, #0x7a0000
0066711c: bl       #0x30ec94
00667120: ldrb     r3, [r4, #0x3d]
00667124: mov      r5, r0
00667128: ldr      r1, [r4, #0x28]
0066712c: cmp      r3, #0
00667130: ldr      r6, [r4, #0x30]
00667134: bne      #0x6671f8
00667138: mov      r3, #1
0066713c: ldr      r0, [r4, #0x2c]
00667140: strb     r3, [r4, #0x3d]
00667144: mov      r1, #0
00667148: bl       #0x30eba4
0066714c: mov      sl, #0
00667150: mov      r6, r0
00667154: str      r5, [r4, #0x28]
00667158: str      r0, [r4, #0x2c]
0066715c: ldr      r0, [r4, #0x14]
00667160: bl       #0x30e964
00667164: mov      r1, #0x44000000
00667168: add      r1, r1, #0x7a0000
0066716c: bl       #0x30ec94
00667170: mov      r5, r0
00667174: str      sl, [r4, #0x1c]
00667178: mov      r0, r6
0066717c: mov      r1, r5
00667180: bl       #0x30e2f8
00667184: cmp      r0, #0
00667188: ldr      r6, [r4, #0x20]
0066718c: mov      r3, #0
00667190: bne      #0x667280
00667194: uxtb     r3, r3
00667198: cmp      r3, #0
0066719c: beq      #0x667290
006671a0: ldrb     r3, [r4, #0x18]
006671a4: cmp      r3, #0
006671a8: beq      #0x6672b0
006671ac: ldr      r7, [r4, #0x24]
006671b0: mov      r1, #0
006671b4: mov      r0, r7
006671b8: bl       #0x30df8c
006671bc: cmp      r0, #0
006671c0: movne    r1, #0
006671c4: beq      #0x6672e4
006671c8: mov      r0, r6
006671cc: bl       #0x30eba4
006671d0: ldr      r3, [r4, #8]
006671d4: mov      r5, r0
006671d8: str      r0, [r4, #0x2c]
006671dc: cmp      r3, #0
006671e0: beq      #0x667294
006671e4: mov      r0, r4
006671e8: ldr      r1, [r4, #0xc]
006671ec: blx      r3
006671f0: ldr      r5, [r4, #0x2c]
006671f4: b        #0x667294
006671f8: bl       #0x30e3ac
006671fc: mov      r1, r6
00667200: bl       #0x30ed6c
00667204: ldr      r1, [r4, #0x2c]
00667208: mov      r8, r0
0066720c: bl       #0x30eba4
00667210: str      r5, [r4, #0x28]
00667214: mov      r7, r0
00667218: str      r0, [r4, #0x2c]
0066721c: mov      r1, #0
00667220: mov      r0, r8
00667224: bl       #0x30e70c
00667228: cmp      r0, #0
0066722c: mov      sl, r8
00667230: mov      r6, r7
00667234: beq      #0x66715c
00667238: ldr      r0, [r4, #0x10]
0066723c: bl       #0x30e964
00667240: mov      r1, #0x44000000
00667244: add      r1, r1, #0x7a0000
00667248: bl       #0x30ec94
0066724c: ldr      r1, [r4, #0x24]
00667250: mov      r5, r0
00667254: ldr      r0, [r4, #0x20]
00667258: bl       #0x30eba4
0066725c: add      r8, r8, #0x80000000
00667260: mov      r6, r0
00667264: str      r8, [r4, #0x1c]
00667268: mov      r0, r7
0066726c: mov      r1, r5
00667270: bl       #0x30e70c
00667274: cmp      r0, #0
00667278: mov      r3, #0
0066727c: beq      #0x667194
00667280: mov      r3, #1
00667284: uxtb     r3, r3
00667288: cmp      r3, #0
0066728c: bne      #0x6671a0
00667290: ldr      r5, [r4, #0x2c]
00667294: mov      r1, #0x44000000
00667298: add      r1, r1, #0x7a0000
0066729c: mov      r0, r5
006672a0: bl       #0x30ed6c
006672a4: bl       #0x30e4cc
006672a8: str      r0, [r4, #4]
006672ac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
006672b0: ldrb     r3, [r4, #0x3c]
006672b4: str      r5, [r4, #0x2c]
006672b8: cmp      r3, #0
006672bc: bne      #0x667294
006672c0: ldr      r3, [r4, #8]
006672c4: mov      r2, #1
006672c8: strb     r2, [r4, #0x3c]
006672cc: cmp      r3, #0
006672d0: beq      #0x667294
006672d4: mov      r0, r4
006672d8: ldr      r1, [r4, #0xc]
006672dc: blx      r3
006672e0: b        #0x667290
006672e4: mov      r1, r5
006672e8: ldr      r0, [r4, #0x2c]
006672ec: bl       #0x30e3ac
006672f0: mov      r1, r7
006672f4: bl       #0x30e7f0
006672f8: mov      r1, r0
006672fc: b        #0x6671c8

# _ZN14AnimApplicator13CheckCallbackEPN6glitch5scene19ITimelineControllerE
0036440c: push     {r4, lr}
00364410: ldrb     r3, [r0, #0x30]
00364414: mov      r4, r0
00364418: cmp      r3, #0
0036441c: beq      #0x364440
00364420: ldr      r3, [r0, #0x34]
00364424: cmp      r3, #0
00364428: beq      #0x364440
0036442c: mov      r0, r1
00364430: ldr      r1, [r4, #0x38]
00364434: blx      r3
00364438: mov      r3, #0
0036443c: strb     r3, [r4, #0x30]
00364440: pop      {r4, pc}

# _ZN13RootSceneNode12onUpdateTimeEj
0035c0f4: push     {r4, r5, r6, lr}
0035c0f8: ldr      r2, [r0, #0x11c]
0035c0fc: movw     r3, #0x201
0035c100: movt     r3, #0
0035c104: and      r3, r2, r3
0035c108: movw     r2, #0x201
0035c10c: cmp      r3, r2
0035c110: mov      r6, r1
0035c114: bne      #0x35c14c
0035c118: mov      r5, r0
0035c11c: ldr      r4, [r5, #0xfc]!
0035c120: cmp      r5, r4
0035c124: beq      #0x35c14c
0035c128: ldr      r3, [r4, #8]
0035c12c: mov      r1, r6
0035c130: mov      r0, r3
0035c134: ldr      r3, [r3]
0035c138: mov      lr, pc
0035c13c: ldr      pc, [r3, #0x14]
0035c140: ldr      r4, [r4]
0035c144: cmp      r5, r4
0035c148: bne      #0x35c128
0035c14c: pop      {r4, r5, r6, pc}

# _ZN15AnimatorBlender17BlenderApplicator11AnimateNodeEj
00366888: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036688c: mov      r4, r0
00366890: ldr      r2, [r0, #0xc]
00366894: ldr      r0, [pc, #0x1b4]
00366898: sub      sp, sp, #0x3c
0036689c: mov      r3, #0
003668a0: add      r0, pc, r0
003668a4: cmn      r2, #1
003668a8: str      r3, [r4, #0x2c]
003668ac: str      r0, [sp, #0x10]
003668b0: str      r1, [sp, #0xc]
003668b4: str      r3, [sp, #0x2c]
003668b8: str      r3, [sp, #0x30]
003668bc: str      r3, [sp, #0x34]
003668c0: str      r3, [r4, #0x24]
003668c4: str      r3, [r4, #0x28]
003668c8: beq      #0x366a48
003668cc: ldr      r3, [r4, #0x3c]
003668d0: ldr      r1, [r3, #0x2c]
003668d4: ldr      r2, [r3, #0x28]
003668d8: rsb      r3, r2, r1
003668dc: cmp      r3, #3
003668e0: ble      #0x366a48
003668e4: ldr      r3, [pc, #0x168]
003668e8: ldr      r1, [pc, #0x168]
003668ec: mov      r6, #0
003668f0: str      r3, [sp, #0x18]
003668f4: ldr      r3, [pc, #0x160]
003668f8: str      r1, [sp, #0x14]
003668fc: add      r7, sp, #0x2c
00366900: add      r3, pc, r3
00366904: str      r3, [sp, #0x1c]
00366908: ldr      r3, [pc, #0x150]
0036690c: add      r3, pc, r3
00366910: str      r3, [sp, #0x20]
00366914: ldr      r3, [pc, #0x148]
00366918: add      r3, pc, r3
0036691c: str      r3, [sp, #0x24]
00366920: b        #0x3669b4
00366924: mov      r2, r7
00366928: mov      r0, r5
0036692c: ldr      r1, [sp, #0xc]
00366930: bl       #0x364444
00366934: ldr      sl, [r4, #0x3c]
00366938: ldr      r1, [r5, #0x28]
0036693c: add      r6, r6, #1
00366940: ldr      r3, [sl, #0x34]
00366944: ldr      r8, [r3, r8]
00366948: mov      r0, r8
0036694c: bl       #0x30ed6c
00366950: ldr      r1, [r5, #0x2c]
00366954: mov      fp, r0
00366958: mov      r0, r8
0036695c: bl       #0x30ed6c
00366960: ldr      r1, [r5, #0x24]
00366964: mov      sb, r0
00366968: mov      r0, r8
0036696c: bl       #0x30ed6c
00366970: mov      r1, r0
00366974: ldr      r0, [r4, #0x24]
00366978: bl       #0x30eba4
0036697c: mov      r1, fp
00366980: str      r0, [r4, #0x24]
00366984: ldr      r0, [r4, #0x28]
00366988: bl       #0x30eba4
0036698c: mov      r1, sb
00366990: str      r0, [r4, #0x28]
00366994: ldr      r0, [r4, #0x2c]
00366998: bl       #0x30eba4
0036699c: str      r0, [r4, #0x2c]
003669a0: ldr      r3, [sl, #0x2c]
003669a4: ldr      r2, [sl, #0x28]
003669a8: rsb      r3, r2, r3
003669ac: cmp      r6, r3, asr #2
003669b0: bge      #0x366a48
003669b4: ldr      r5, [r2, r6, lsl #2]
003669b8: lsl      r8, r6, #2
003669bc: ldr      r3, [r5]
003669c0: mov      r0, r5
003669c4: mov      lr, pc
003669c8: ldr      pc, [r3, #0x44]
003669cc: ldr      ip, [r5]
003669d0: ldr      r2, [r0, #4]
003669d4: mov      r3, r7
003669d8: mov      r0, r5
003669dc: ldr      r1, [r4, #0xc]
003669e0: mov      lr, pc
003669e4: ldr      pc, [ip, #0x7c]
003669e8: mov      r0, r5
003669ec: bl       #0x369160
003669f0: subs     r5, r0, #0
003669f4: bne      #0x366924
003669f8: ldr      r0, [sp, #0x10]
003669fc: ldr      ip, [sp, #0x14]
00366a00: ldr      r3, [r0, ip]
00366a04: ldr      r3, [r3]
00366a08: cmp      r3, #2
00366a0c: streq    r5, [r5]
00366a10: beq      #0x366924
00366a14: cmp      r3, #1
00366a18: bne      #0x366924
00366a1c: ldr      r2, [sp, #0x10]
00366a20: ldr      r1, [sp, #0x18]
00366a24: movw     ip, #0x18a
00366a28: ldr      r3, [sp, #0x24]
00366a2c: ldr      r0, [r2, r1]
00366a30: ldr      r1, [sp, #0x1c]
00366a34: ldr      r2, [sp, #0x20]
00366a38: add      r0, r0, #0xa8
00366a3c: str      ip, [sp]
00366a40: bl       #0x30e004
00366a44: b        #0x366924
00366a48: add      sp, sp, #0x3c
00366a4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN15AnimatorBlender11animateNodeEPN6glitch5scene10ISceneNodeEj
00366cb4: push     {r4, r5, r6, lr}
00366cb8: ldr      r3, [r0, #0x7c]
00366cbc: mov      r4, r0
00366cc0: mov      r5, r2
00366cc4: cmp      r3, #0
00366cc8: ldr      r0, [r0, #0x84]
00366ccc: blt      #0x366d18
00366cd0: rsb      r0, r0, r2
00366cd4: rsb      r0, r0, r3
00366cd8: cmp      r0, #0
00366cdc: str      r0, [r4, #0x7c]
00366ce0: ble      #0x366d6c
00366ce4: bl       #0x30e964
00366ce8: ldr      r1, [r4, #0x80]
00366cec: bl       #0x30ed6c
00366cf0: ldr      r2, [r4, #0x34]
00366cf4: ldr      ip, [r4, #0x74]
00366cf8: mov      r3, r0
00366cfc: mov      r1, r0
00366d00: str      r3, [r2, ip, lsl #2]
00366d04: mov      r0, #0x3f800000
00366d08: bl       #0x30e3ac
00366d0c: ldr      r2, [r4, #0x70]
00366d10: ldr      r3, [r4, #0x34]
00366d14: str      r0, [r3, r2, lsl #2]
00366d18: ldr      r3, [r4]
00366d1c: mov      r0, r4
00366d20: mov      r1, r5
00366d24: add      r6, r4, #0x88
00366d28: mov      lr, pc
00366d2c: ldr      pc, [r3, #0x50]
00366d30: mov      r1, r5
00366d34: mov      r0, r6
00366d38: bl       #0x366888
00366d3c: ldr      r2, [r4, #0x70]
00366d40: ldr      r3, [r4, #0x28]
00366d44: ldr      r3, [r3, r2, lsl #2]
00366d48: mov      r0, r3
00366d4c: ldr      r3, [r3]
00366d50: mov      lr, pc
00366d54: ldr      pc, [r3, #0x44]
00366d58: mov      r1, r0
00366d5c: mov      r0, r6
00366d60: bl       #0x36440c
00366d64: str      r5, [r4, #0x84]
00366d68: pop      {r4, r5, r6, pc}
00366d6c: ldr      r2, [r4, #0x74]
00366d70: ldr      r3, [r4, #0x34]
00366d74: mov      r1, #0
00366d78: str      r1, [r3, r2, lsl #2]
00366d7c: ldr      r2, [r4, #0x70]
00366d80: ldr      r3, [r4, #0x34]
00366d84: mov      r1, #0x3f800000
00366d88: str      r1, [r3, r2, lsl #2]
00366d8c: b        #0x366d18

# _ZN11AnimatorSet17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
003672c8: push     {r4, r5, r6, lr}
003672cc: subs     r4, r1, #0
003672d0: mov      r5, r0
003672d4: beq      #0x36732c
003672d8: mov      r1, #0x44000000
003672dc: add      r1, r1, #0x7a0000
003672e0: ldr      r0, [r4, #0x1c]
003672e4: bl       #0x30ed6c
003672e8: bl       #0x30e4cc
003672ec: mov      r1, #0x44000000
003672f0: mov      r6, r0
003672f4: add      r1, r1, #0x7a0000
003672f8: ldr      r0, [r4, #0x2c]
003672fc: bl       #0x30ed6c
00367300: bl       #0x30e4cc
00367304: ldr      r3, [r4, #4]
00367308: rsb      r0, r3, r0
0036730c: cmp      r0, r6
00367310: movge    r3, #0
00367314: movlt    r3, #1
00367318: cmp      r0, #0
0036731c: movlt    r3, #0
00367320: cmp      r3, #0
00367324: rsbne    r3, r0, r6
00367328: str      r3, [r5, #0x68]
0036732c: mov      r3, #1
00367330: strb     r3, [r5, #0x88]
00367334: pop      {r4, r5, r6, pc}

# _ZN15AnimatorBlender10updateTimeEj
00366d90: push     {r4, r5, r6, r7, r8, lr}
00366d94: ldr      r3, [r0, #0x7c]
00366d98: mov      r5, r0
00366d9c: mov      r7, r1
00366da0: cmp      r3, #0
00366da4: ldr      r0, [r0, #0x84]
00366da8: blt      #0x366df4
00366dac: rsb      r0, r0, r1
00366db0: rsb      r0, r0, r3
00366db4: cmp      r0, #0
00366db8: str      r0, [r5, #0x7c]
00366dbc: ble      #0x366e94
00366dc0: bl       #0x30e964
00366dc4: ldr      r1, [r5, #0x80]
00366dc8: bl       #0x30ed6c
00366dcc: ldr      r2, [r5, #0x34]
00366dd0: ldr      ip, [r5, #0x74]
00366dd4: mov      r3, r0
00366dd8: mov      r1, r0
00366ddc: str      r3, [r2, ip, lsl #2]
00366de0: mov      r0, #0x3f800000
00366de4: bl       #0x30e3ac
00366de8: ldr      r2, [r5, #0x70]
00366dec: ldr      r3, [r5, #0x34]
00366df0: str      r0, [r3, r2, lsl #2]
00366df4: ldr      r6, [r5, #0x2c]
00366df8: ldr      r3, [r5, #0x28]
00366dfc: rsb      r6, r3, r6
00366e00: asrs     r6, r6, #2
00366e04: beq      #0x366e5c
00366e08: mov      r4, #0
00366e0c: b        #0x366e1c
00366e10: add      r4, r4, #1
00366e14: cmp      r4, r6
00366e18: beq      #0x366e5c
00366e1c: ldr      r3, [r5, #0x34]
00366e20: mov      r1, #0
00366e24: ldr      r0, [r3, r4, lsl #2]
00366e28: bl       #0x30df8c
00366e2c: cmp      r0, #0
00366e30: bne      #0x366e10
00366e34: ldr      r3, [r5, #0x28]
00366e38: mov      r1, r7
00366e3c: ldr      r3, [r3, r4, lsl #2]
00366e40: add      r4, r4, #1
00366e44: mov      r0, r3
00366e48: ldr      r3, [r3]
00366e4c: mov      lr, pc
00366e50: ldr      pc, [r3, #0x14]
00366e54: cmp      r4, r6
00366e58: bne      #0x366e1c
00366e5c: mov      r0, r5
00366e60: bl       #0x366594
00366e64: ldr      r2, [r5, #0x70]
00366e68: ldr      r3, [r5, #0x28]
00366e6c: ldr      r3, [r3, r2, lsl #2]
00366e70: mov      r0, r3
00366e74: ldr      r3, [r3]
00366e78: mov      lr, pc
00366e7c: ldr      pc, [r3, #0x44]
00366e80: mov      r1, r0
00366e84: add      r0, r5, #0x88
00366e88: bl       #0x36440c
00366e8c: str      r7, [r5, #0x84]
00366e90: pop      {r4, r5, r6, r7, r8, pc}
00366e94: ldr      r2, [r5, #0x74]
00366e98: ldr      r3, [r5, #0x34]
00366e9c: mov      r1, #0
00366ea0: str      r1, [r3, r2, lsl #2]
00366ea4: ldr      r2, [r5, #0x70]
00366ea8: ldr      r3, [r5, #0x34]
00366eac: mov      r1, #0x3f800000
00366eb0: str      r1, [r3, r2, lsl #2]
00366eb4: b        #0x366df4

# _ZN8Animator11animateNodeEPN6glitch5scene10ISceneNodeEj
003662c8: push     {r4, r5, r6, lr}
003662cc: mov      r6, r2
003662d0: add      r5, r0, #0x58
003662d4: mov      r4, r0
003662d8: bl       #0x65d528
003662dc: mov      r1, r6
003662e0: mov      r0, r5
003662e4: bl       #0x3645e8
003662e8: mov      r0, r4
003662ec: ldr      r3, [r4]
003662f0: mov      lr, pc
003662f4: ldr      pc, [r3, #0x44]
003662f8: mov      r1, r0
003662fc: mov      r0, r5
00366300: pop      {r4, r5, r6, lr}
00366304: b        #0x36440c

# _ZN11AnimatorSet11animateNodeEPN6glitch5scene10ISceneNodeEj
003673a4: push     {r4, r5, r6, lr}
003673a8: mov      r6, r2
003673ac: add      r5, r0, #0x58
003673b0: mov      r4, r0
003673b4: bl       #0x65f270
003673b8: mov      r1, r6
003673bc: mov      r0, r5
003673c0: bl       #0x3645e8
003673c4: mov      r0, r4
003673c8: ldr      r3, [r4]
003673cc: mov      lr, pc
003673d0: ldr      pc, [r3, #0x44]
003673d4: mov      r1, r0
003673d8: mov      r0, r5
003673dc: pop      {r4, r5, r6, lr}
003673e0: b        #0x36440c

# _ZN14AnimController8PlayClipEjbij
00474b50: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00474b54: mov      r4, r1
00474b58: ldr      r1, [sp, #0x20]
00474b5c: mov      r7, r2
00474b60: mov      r6, r0
00474b64: bl       #0x4748b8
00474b68: subs     sb, r0, #0
00474b6c: beq      #0x474c30
00474b70: ldr      r3, [sb]
00474b74: mov      lr, pc
00474b78: ldr      pc, [r3, #0x44]
00474b7c: mov      r5, r0
00474b80: mov      r0, sb
00474b84: bl       #0x369160
00474b88: cmp      r5, #0
00474b8c: mov      sl, r0
00474b90: beq      #0x474bac
00474b94: ldr      r3, [r5]
00474b98: mov      r0, r5
00474b9c: mov      lr, pc
00474ba0: ldr      pc, [r3, #0x1c]
00474ba4: cmp      r0, r4
00474ba8: bls      #0x474c28
00474bac: ldr      r3, [r5]
00474bb0: mov      r0, r5
00474bb4: mov      lr, pc
00474bb8: ldr      pc, [r3, #0x38]
00474bbc: ldr      r3, [sb]
00474bc0: mov      r8, r0
00474bc4: mov      r1, r4
00474bc8: mov      r0, sb
00474bcc: mov      lr, pc
00474bd0: ldr      pc, [r3, #0x30]
00474bd4: cmp      r8, r4
00474bd8: beq      #0x474c3c
00474bdc: mov      r1, r7
00474be0: mov      r0, r5
00474be4: ldr      r3, [r5]
00474be8: mov      lr, pc
00474bec: ldr      pc, [r3, #0x40]
00474bf0: ldr      r3, [r5]
00474bf4: mov      r0, r5
00474bf8: mov      r1, #0x3f800000
00474bfc: mov      lr, pc
00474c00: ldr      pc, [r3, #0x48]
00474c04: ldr      r0, [r6, #4]
00474c08: mov      r1, #0
00474c0c: bl       #0x35d624
00474c10: ldr      r3, [r6, #4]
00474c14: mov      r0, #1
00474c18: ldr      r2, [r3, #0x11c]
00474c1c: orr      r2, r2, #0x200
00474c20: str      r2, [r3, #0x11c]
00474c24: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474c28: mov      r0, #0
00474c2c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474c30: bl       #0x369160
00474c34: mov      r0, sb
00474c38: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00474c3c: ldr      r3, [r5]
00474c40: mov      r0, r5
00474c44: mov      lr, pc
00474c48: ldr      pc, [r3, #0x44]
00474c4c: cmp      r0, #0
00474c50: bne      #0x474bdc
00474c54: cmp      sl, #0
00474c58: ldr      r3, [r5]
00474c5c: ldr      r1, [r5, #0x10]
00474c60: ldrne    sl, [sl, #0x10]
00474c64: ldr      r3, [r3, #0xc]
00474c68: mov      r0, r5
00474c6c: add      r1, sl, r1
00474c70: blx      r3
00474c74: b        #0x474bdc

# _ZN24BlendedAnimSetController8PlayClipEjbij
0047680c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00476810: mov      r4, r1
00476814: ldr      r1, [sp, #0x28]
00476818: mov      sl, r2
0047681c: mov      r7, r0
00476820: bl       #0x4748b8
00476824: subs     r6, r0, #0
00476828: beq      #0x476900
0047682c: ldr      r1, [r7, #0x14]
00476830: bl       #0x36679c
00476834: cmn      r4, #1
00476838: beq      #0x476900
0047683c: ldr      r2, [r6, #0x70]
00476840: ldr      r3, [r6, #0x28]
00476844: ldr      r5, [r3, r2, lsl #2]
00476848: cmp      r5, #0
0047684c: beq      #0x476908
00476850: ldr      r3, [r5]
00476854: mov      r0, r5
00476858: mov      lr, pc
0047685c: ldr      pc, [r3, #0x44]
00476860: mov      r8, r0
00476864: mov      r0, r5
00476868: bl       #0x65f114
0047686c: mov      sb, r0
00476870: mov      r0, r6
00476874: bl       #0x369160
00476878: mov      r1, r4
0047687c: mov      fp, r0
00476880: mov      r0, r5
00476884: bl       #0x3674ac
00476888: cmn      r0, #1
0047688c: mov      r4, r0
00476890: beq      #0x476900
00476894: ldr      r3, [r8, #0x34]
00476898: cmp      r3, #0
0047689c: beq      #0x4768b4
004768a0: mov      r0, r5
004768a4: ldr      r3, [r5]
004768a8: ldr      r1, [r7, #0xc]
004768ac: mov      lr, pc
004768b0: ldr      pc, [r3, #0x30]
004768b4: cmp      sb, r4
004768b8: beq      #0x476920
004768bc: mov      r1, sl
004768c0: mov      r0, r8
004768c4: ldr      r3, [r8]
004768c8: mov      lr, pc
004768cc: ldr      pc, [r3, #0x40]
004768d0: ldr      r3, [r8]
004768d4: mov      r0, r8
004768d8: mov      r1, #0x3f800000
004768dc: mov      lr, pc
004768e0: ldr      pc, [r3, #0x48]
004768e4: ldrb     r1, [r7, #0x10]
004768e8: ldr      r0, [r7, #4]
004768ec: bl       #0x35d624
004768f0: mov      r0, r6
004768f4: bl       #0x366740
004768f8: mov      r0, #1
004768fc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476900: mov      r0, #0
00476904: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476908: mov      r0, r5
0047690c: bl       #0x65f114
00476910: mov      r0, r6
00476914: bl       #0x369160
00476918: mov      r0, r5
0047691c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476920: ldr      r3, [r8]
00476924: mov      r0, r8
00476928: mov      lr, pc
0047692c: ldr      pc, [r3, #0x44]
00476930: cmp      r0, #0
00476934: bne      #0x4768bc
00476938: cmp      fp, #0
0047693c: ldr      r3, [r8]
00476940: ldr      r1, [r8, #0x10]
00476944: ldrne    fp, [fp, #0x10]
00476948: ldr      r3, [r3, #0xc]
0047694c: mov      r0, r8
00476950: add      r1, fp, r1
00476954: blx      r3
00476958: b        #0x4768bc

# _ZN24BlendedAnimSetControllerC1EP13RootSceneNodei
00476ddc: push     {r4, r5, r6, r7, r8, sl, lr}
00476de0: mov      r5, r2
00476de4: sub      sp, sp, #0x14
00476de8: mov      r2, #1
00476dec: ldr      r7, [pc, #0x258]
00476df0: mov      r4, r0
00476df4: bl       #0x474e44
00476df8: ldr      r3, [pc, #0x250]
00476dfc: add      r7, pc, r7
00476e00: ldr      r2, [pc, #0x24c]
00476e04: ldr      r3, [r7, r3]
00476e08: mov      r8, #0
00476e0c: ldr      r6, [r7, r2]
00476e10: add      r3, r3, #8
00476e14: str      r3, [r4]
00476e18: mov      r3, #1
00476e1c: strb     r3, [r4, #0x10]
00476e20: mov      r1, r5
00476e24: str      r5, [r4, #8]
00476e28: mov      r0, r6
00476e2c: str      r8, [r4, #0xc]
00476e30: str      r8, [r4, #0x14]
00476e34: bl       #0x4762c4
00476e38: mov      r5, r0
00476e3c: ldr      r1, [r4, #8]
00476e40: mov      r0, r6
00476e44: bl       #0x4762c4
00476e48: subs     r3, r5, r8
00476e4c: movne    r3, #1
00476e50: subs     sl, r0, r8
00476e54: movne    sl, #1
00476e58: tst      sl, r3
00476e5c: mov      r6, r0
00476e60: bne      #0x476ea0
00476e64: cmp      r3, #0
00476e68: beq      #0x476e7c
00476e6c: ldr      r3, [r5]
00476e70: ldr      r0, [r3, #-0xc]
00476e74: add      r0, r5, r0
00476e78: bl       #0x31d584
00476e7c: cmp      sl, #0
00476e80: beq      #0x476e94
00476e84: ldr      r3, [r6]
00476e88: ldr      r0, [r3, #-0xc]
00476e8c: add      r0, r6, r0
00476e90: bl       #0x31d584
00476e94: mov      r0, r4
00476e98: add      sp, sp, #0x14
00476e9c: pop      {r4, r5, r6, r7, r8, sl, pc}
00476ea0: mov      r0, r5
00476ea4: bl       #0x65f11c
00476ea8: cmp      r0, r8
00476eac: ble      #0x476fd8
00476eb0: mov      r1, #0
00476eb4: mov      r0, #0xd0
00476eb8: bl       #0x5341ac
00476ebc: mov      r7, r0
00476ec0: bl       #0x367018
00476ec4: mov      r3, #1
00476ec8: str      r5, [sp, #0xc]
00476ecc: strb     r3, [r7, #0x24]
00476ed0: ldr      r3, [sp, #0xc]
00476ed4: add      r8, r7, #0x28
00476ed8: ldr      r2, [r3]
00476edc: ldr      r2, [r2, #-0xc]
00476ee0: add      r3, r3, r2
00476ee4: ldr      r2, [r3, #4]
00476ee8: add      r2, r2, #1
00476eec: str      r2, [r3, #4]
00476ef0: ldr      r1, [r7, #0x2c]
00476ef4: ldr      r3, [r7, #0x30]
00476ef8: cmp      r1, r3
00476efc: beq      #0x47702c
00476f00: ldr      r3, [sp, #0xc]
00476f04: str      r3, [r1]
00476f08: ldr      r3, [r7, #0x2c]
00476f0c: add      r3, r3, #4
00476f10: str      r3, [r7, #0x2c]
00476f14: mov      r3, #1
00476f18: str      r6, [sp, #0xc]
00476f1c: strb     r3, [r7, #0x24]
00476f20: ldr      r3, [sp, #0xc]
00476f24: ldr      r2, [r3]
00476f28: ldr      r2, [r2, #-0xc]
00476f2c: add      r3, r3, r2
00476f30: ldr      r2, [r3, #4]
00476f34: add      r2, r2, #1
00476f38: str      r2, [r3, #4]
00476f3c: ldr      r1, [r7, #0x2c]
00476f40: ldr      r3, [r7, #0x30]
00476f44: cmp      r1, r3
00476f48: beq      #0x47703c
00476f4c: ldr      r3, [sp, #0xc]
00476f50: str      r3, [r1]
00476f54: ldr      r3, [r7, #0x2c]
00476f58: add      r3, r3, #4
00476f5c: str      r3, [r7, #0x2c]
00476f60: mov      r0, r7
00476f64: ldr      r3, [r7]
00476f68: mov      r1, #0
00476f6c: mov      lr, pc
00476f70: ldr      pc, [r3, #0x88]
00476f74: ldr      r3, [r7, #0x34]
00476f78: mov      r2, #0x3f800000
00476f7c: mov      r1, r7
00476f80: str      r2, [r3]
00476f84: ldr      r3, [r7, #0x34]
00476f88: mov      r2, #0
00476f8c: str      r2, [r3, #4]
00476f90: ldr      r3, [r4, #4]
00476f94: mov      r0, r3
00476f98: ldr      r3, [r3]
00476f9c: mov      lr, pc
00476fa0: ldr      pc, [r3, #0x6c]
00476fa4: ldr      r3, [r5]
00476fa8: ldr      r0, [r3, #-0xc]
00476fac: add      r0, r5, r0
00476fb0: bl       #0x31d584
00476fb4: ldr      r3, [r6]
00476fb8: ldr      r0, [r3, #-0xc]
00476fbc: add      r0, r6, r0
00476fc0: bl       #0x31d584
00476fc4: ldr      r3, [r7]
00476fc8: ldr      r0, [r3, #-0xc]
00476fcc: add      r0, r7, r0
00476fd0: bl       #0x31d584
00476fd4: b        #0x476e94
00476fd8: ldr      r3, [pc, #0x78]
00476fdc: ldr      r3, [r7, r3]
00476fe0: ldr      r3, [r3]
00476fe4: cmp      r3, #2
00476fe8: streq    r8, [r8]
00476fec: beq      #0x476eb0
00476ff0: cmp      r3, #1
00476ff4: bne      #0x476eb0
00476ff8: ldr      r0, [pc, #0x5c]
00476ffc: ldr      r1, [pc, #0x5c]
00477000: ldr      r2, [pc, #0x5c]
00477004: ldr      r0, [r7, r0]
00477008: ldr      r3, [pc, #0x58]
0047700c: mov      ip, #0x45
00477010: add      r1, pc, r1
00477014: add      r2, pc, r2
00477018: add      r3, pc, r3
0047701c: add      r0, r0, #0xa8
00477020: str      ip, [sp]
00477024: bl       #0x30e004
00477028: b        #0x476eb0
0047702c: mov      r0, r8
00477030: add      r2, sp, #0xc
00477034: bl       #0x476a9c
00477038: b        #0x476f14
0047703c: mov      r0, r8
00477040: add      r2, sp, #0xc
00477044: bl       #0x476a9c
00477048: b        #0x476f60

# _ZN17AnimSetControllerC1EP13RootSceneNodei
004751b4: push     {r4, r5, r6, lr}
004751b8: mov      r6, r2
004751bc: ldr      r5, [pc, #0x78]
004751c0: mov      r2, #1
004751c4: mov      r4, r0
004751c8: bl       #0x474e44
004751cc: ldr      r3, [pc, #0x6c]
004751d0: add      r5, pc, r5
004751d4: str      r6, [r4, #8]
004751d8: ldr      r3, [r5, r3]
004751dc: mov      r1, r6
004751e0: add      r3, r3, #8
004751e4: str      r3, [r4]
004751e8: mov      r3, #0
004751ec: str      r3, [r4, #0xc]
004751f0: mov      r3, #1
004751f4: strb     r3, [r4, #0x10]
004751f8: ldr      r3, [pc, #0x44]
004751fc: ldr      r0, [r5, r3]
00475200: bl       #0x4762c4
00475204: subs     r5, r0, #0
00475208: beq      #0x475234
0047520c: ldr      r3, [r4, #4]
00475210: mov      r1, r5
00475214: mov      r0, r3
00475218: ldr      r3, [r3]
0047521c: mov      lr, pc
00475220: ldr      pc, [r3, #0x6c]
00475224: ldr      r3, [r5]
00475228: ldr      r0, [r3, #-0xc]
0047522c: add      r0, r5, r0
00475230: bl       #0x31d584
00475234: mov      r0, r4
00475238: pop      {r4, r5, r6, pc}
0047523c: subseq   pc, r1, r0, asr #17
00475240: andeq    r3, r0, ip, lsr #10
00475244: andeq    r4, r0, r8, lsr r8
