
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

# _ZN6glitch7collada25CSceneNodeAnimatorBlender7compileEPSt6vectorIhNS_4core10SAllocatorIhLNS_6memory13E_MEMORY_HINTE0EEEE
0065ed98: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065ed9c: sub      sp, sp, #0x2c
0065eda0: ldr      r3, [r0]
0065eda4: str      r1, [sp, #8]
0065eda8: mov      r4, r0
0065edac: mov      lr, pc
0065edb0: ldr      pc, [r3, #0x78]
0065edb4: ldr      r1, [r4, #0x2c]
0065edb8: ldr      r2, [r4, #0x28]
0065edbc: ldr      r3, [r4]
0065edc0: mov      r5, r0
0065edc4: rsb      r2, r2, r1
0065edc8: asr      r2, r2, #2
0065edcc: mov      r0, r4
0065edd0: str      r2, [sp, #0xc]
0065edd4: mov      lr, pc
0065edd8: ldr      pc, [r3, #0x70]
0065eddc: ldr      r1, [sp, #8]
0065ede0: str      r0, [sp, #4]
0065ede4: cmp      r1, #0
0065ede8: beq      #0x65efbc
0065edec: add      r2, sp, #0x28
0065edf0: mov      r5, #0
0065edf4: str      r5, [r2, #-8]!
0065edf8: ldr      r1, [sp, #0xc]
0065edfc: add      r0, r4, #0x34
0065ee00: bl       #0x368b60
0065ee04: ldr      r2, [r4, #0x34]
0065ee08: ldr      r1, [r4, #0x38]
0065ee0c: rsb      r1, r2, r1
0065ee10: asrs     r1, r1, #2
0065ee14: beq      #0x65ee34
0065ee18: mov      r3, #0
0065ee1c: b        #0x65ee24
0065ee20: ldr      r2, [r4, #0x34]
0065ee24: str      r5, [r2, r3, lsl #2]
0065ee28: add      r3, r3, #1
0065ee2c: cmp      r3, r1
0065ee30: bne      #0x65ee20
0065ee34: add      r2, sp, #0x28
0065ee38: mov      r5, #0
0065ee3c: str      r5, [r2, #-0xc]!
0065ee40: add      r0, r4, #0x4c
0065ee44: ldr      r1, [sp, #4]
0065ee48: bl       #0x65ec40
0065ee4c: ldr      r1, [sp, #8]
0065ee50: ldr      r3, [r4, #0x28]
0065ee54: ldm      r1, {r0, r2}
0065ee58: ldr      fp, [r3]
0065ee5c: cmp      r2, r0
0065ee60: beq      #0x65ee70
0065ee64: mov      r1, r5
0065ee68: rsb      r2, r0, r2
0065ee6c: bl       #0x30e460
0065ee70: ldr      r2, [sp, #4]
0065ee74: cmp      r2, #0
0065ee78: ble      #0x65ef68
0065ee7c: mov      sb, #0
0065ee80: str      sb, [sp]
0065ee84: mov      r1, sb
0065ee88: ldr      r3, [r4]
0065ee8c: mov      r0, r4
0065ee90: mov      lr, pc
0065ee94: ldr      pc, [r3, #0x74]
0065ee98: ldr      r3, [sp, #8]
0065ee9c: ldr      r1, [sp]
0065eea0: mov      r5, r0
0065eea4: ldr      r2, [r3]
0065eea8: ldr      r3, [r4, #0x4c]
0065eeac: mov      r0, fp
0065eeb0: add      r2, r2, r1
0065eeb4: str      r2, [r3, sb, lsl #2]
0065eeb8: ldr      r2, [r4, #0x4c]
0065eebc: mov      r1, sb
0065eec0: mov      r3, #0
0065eec4: ldr      r7, [r2, sb, lsl #2]
0065eec8: ldr      ip, [fp]
0065eecc: mov      r2, r7
0065eed0: mov      lr, pc
0065eed4: ldr      pc, [ip, #0x68]
0065eed8: ldr      r3, [fp]
0065eedc: mov      r0, fp
0065eee0: mov      r1, sb
0065eee4: mov      lr, pc
0065eee8: ldr      pc, [r3, #0x54]
0065eeec: ldr      r3, [r4, #0x28]
0065eef0: ldr      r8, [r4, #0x2c]
0065eef4: mov      sl, r0
0065eef8: rsb      r8, r3, r8
0065eefc: asr      r8, r8, #2
0065ef00: cmp      r8, #1
0065ef04: bls      #0x65ef44
0065ef08: add      r7, r7, r5
0065ef0c: mov      r6, #1
0065ef10: b        #0x65ef18
0065ef14: ldr      r3, [r4, #0x28]
0065ef18: ldr      r3, [r3, r6, lsl #2]
0065ef1c: mov      r2, r7
0065ef20: add      r6, r6, #1
0065ef24: mov      r0, r3
0065ef28: mov      r1, sl
0065ef2c: ldr      r3, [r3]
0065ef30: mov      lr, pc
0065ef34: ldr      pc, [r3, #0x60]
0065ef38: cmp      r6, r8
0065ef3c: add      r7, r7, r5
0065ef40: bne      #0x65ef14
0065ef44: ldr      r2, [sp, #4]
0065ef48: add      sb, sb, #1
0065ef4c: cmp      sb, r2
0065ef50: beq      #0x65ef68
0065ef54: ldr      r1, [sp]
0065ef58: ldr      r3, [sp, #0xc]
0065ef5c: mla      r1, r3, r5, r1
0065ef60: str      r1, [sp]
0065ef64: b        #0x65ee84
0065ef68: mov      r5, #0
0065ef6c: add      r2, sp, #0x28
0065ef70: str      r5, [r2, #-0x10]!
0065ef74: add      r0, r4, #0x58
0065ef78: ldr      r1, [sp, #4]
0065ef7c: bl       #0x65ec40
0065ef80: add      r2, sp, #0x28
0065ef84: str      r5, [r2, #-0x14]!
0065ef88: ldr      r1, [sp, #4]
0065ef8c: add      r0, r4, #0x64
0065ef90: bl       #0x65ed54
0065ef94: ldr      r2, [r4, #0x2c]
0065ef98: ldr      r3, [r4, #0x28]
0065ef9c: strb     r5, [r4, #0x24]
0065efa0: rsb      r3, r3, r2
0065efa4: lsrs     r3, r3, #2
0065efa8: beq      #0x65efb4
0065efac: mov      r0, r4
0065efb0: bl       #0x667f18
0065efb4: add      sp, sp, #0x2c
0065efb8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065efbc: add      r2, sp, #0x28
0065efc0: mov      r3, #0
0065efc4: strb     r3, [r2, #-1]!
0065efc8: add      r3, r4, #0x40
0065efcc: str      r3, [sp, #8]
0065efd0: ldr      r3, [sp, #0xc]
0065efd4: ldr      r0, [sp, #8]
0065efd8: mul      r1, r3, r5
0065efdc: bl       #0x57d94c
0065efe0: b        #0x65edec

# _ZN15AnimatorBlenderC2Ev
003670c8: push     {r4, r5, r6, lr}
003670cc: mov      r6, r1
003670d0: ldr      r5, [pc, #0x78]
003670d4: add      r1, r1, #4
003670d8: mov      r4, r0
003670dc: bl       #0x366f7c
003670e0: ldr      r2, [r6]
003670e4: ldr      r3, [pc, #0x68]
003670e8: add      r5, pc, r5
003670ec: str      r2, [r4]
003670f0: ldr      r3, [r5, r3]
003670f4: ldr      r1, [r2, #-0xc]
003670f8: ldr      r0, [r6, #0x24]
003670fc: add      r2, r3, #0xa0
00367100: mov      r3, #0
00367104: str      r0, [r4, r1]
00367108: str      r2, [r4, #4]
0036710c: mov      r2, #0
00367110: str      r3, [r4, #0x84]
00367114: str      r3, [r4, #0x70]
00367118: str      r3, [r4, #0x74]
0036711c: str      r3, [r4, #0x78]
00367120: str      r3, [r4, #0x7c]
00367124: str      r2, [r4, #0x80]
00367128: add      r0, r4, #0x88
0036712c: mov      r1, r4
00367130: bl       #0x364330
00367134: ldr      r3, [pc, #0x1c]
00367138: str      r4, [r4, #0xc4]
0036713c: mov      r0, r4
00367140: ldr      r3, [r5, r3]
00367144: add      r3, r3, #8
00367148: str      r3, [r4, #0x88]
0036714c: pop      {r4, r5, r6, pc}
00367150: rsbeq    sp, r2, r8, lsr #19
00367154: andeq    r3, r0, r4, lsr #6
00367158: andeq    r4, r0, r0, ror #21

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

# _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E17getBlendedValueExEPvPfiS6_
006130d4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006130d8: mov      ip, #0
006130dc: sub      sp, sp, #0x3c
006130e0: subs     r6, r2, #0
006130e4: mov      r2, #0x3f800000
006130e8: mov      r8, r0
006130ec: str      r2, [sp, #0x34]
006130f0: mov      r5, r1
006130f4: mov      sb, r3
006130f8: str      ip, [sp, #0x28]
006130fc: str      ip, [sp, #0x2c]
00613100: str      ip, [sp, #0x30]
00613104: ble      #0x61328c
00613108: mov      r1, ip
0061310c: ldr      r0, [r5]
00613110: bl       #0x30df8c
00613114: cmp      r0, #0
00613118: moveq    r3, #0
0061311c: moveq    r7, r5
00613120: moveq    r4, r3
00613124: beq      #0x613224
00613128: mov      r7, r5
0061312c: mov      r4, #0
00613130: b        #0x613144
00613134: ldr      r0, [r7, #4]!
00613138: bl       #0x30df8c
0061313c: cmp      r0, #0
00613140: beq      #0x613220
00613144: add      r4, r4, #1
00613148: cmp      r4, r6
0061314c: mov      r1, #0
00613150: bne      #0x613134
00613154: add      r4, r6, #1
00613158: mov      sl, #0
0061315c: cmp      r6, r4
00613160: ble      #0x6131f8
00613164: add      r3, sp, #4
00613168: add      r5, r5, r4, lsl #2
0061316c: add      r8, r8, r4, lsl #4
00613170: add      fp, sp, #0x28
00613174: str      r3, [sp, #0x24]
00613178: b        #0x61318c
0061317c: cmp      r4, r6
00613180: add      r5, r5, #4
00613184: add      r8, r8, #0x10
00613188: beq      #0x6131f8
0061318c: ldr      r7, [r5]
00613190: mov      r1, #0
00613194: add      r4, r4, #1
00613198: mov      r0, r7
0061319c: bl       #0x30df8c
006131a0: cmp      r0, #0
006131a4: bne      #0x61317c
006131a8: mov      r0, sl
006131ac: mov      r1, r7
006131b0: bl       #0x30eba4
006131b4: ldr      ip, [sp, #0x24]
006131b8: mov      sl, r0
006131bc: ldm      r8, {r0, r1, r2, r3}
006131c0: stm      ip, {r0, r1, r2, r3}
006131c4: mov      r1, sl
006131c8: mov      r0, r7
006131cc: bl       #0x30ec94
006131d0: ldm      fp, {r1, r2, r3}
006131d4: ldr      ip, [sp, #0x34]
006131d8: str      r0, [sp, #0x14]
006131dc: mov      r0, fp
006131e0: str      ip, [sp]
006131e4: bl       #0x612d00
006131e8: cmp      r4, r6
006131ec: add      r5, r5, #4
006131f0: add      r8, r8, #0x10
006131f4: bne      #0x61318c
006131f8: ldr      r1, [sp, #0x2c]
006131fc: ldr      r3, [sp, #0x30]
00613200: ldr      r2, [sp, #0x34]
00613204: ldr      r0, [sp, #0x28]
00613208: str      r1, [sb, #4]
0061320c: str      r2, [sb, #0xc]
00613210: str      r0, [sb]
00613214: str      r3, [sb, #8]
00613218: add      sp, sp, #0x3c
0061321c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00613220: lsl      r3, r4, #4
00613224: ldr      sl, [r7]
00613228: add      r2, r8, r3
0061322c: ldr      r7, [r8, r3]
00613230: ldr      fp, [r2, #0xc]
00613234: ldr      r3, [r2, #4]
00613238: ldr      r2, [r2, #8]
0061323c: mov      r0, sl
00613240: mov      r1, #0x3f800000
00613244: str      r3, [sp, #0x2c]
00613248: str      r2, [sp, #0x30]
0061324c: str      r2, [sp, #0x1c]
00613250: str      r3, [sp, #0x20]
00613254: str      r7, [sp, #0x28]
00613258: str      fp, [sp, #0x34]
0061325c: bl       #0x30df8c
00613260: cmp      r0, #0
00613264: ldr      r2, [sp, #0x1c]
00613268: ldr      r3, [sp, #0x20]
0061326c: beq      #0x613284
00613270: str      fp, [sb, #0xc]
00613274: str      r7, [sb]
00613278: str      r3, [sb, #4]
0061327c: str      r2, [sb, #8]
00613280: b        #0x613218
00613284: add      r4, r4, #1
00613288: b        #0x61315c
0061328c: mov      r4, #1
00613290: b        #0x613158
