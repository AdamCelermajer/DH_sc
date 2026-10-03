
# _ZNK10GameObject15IsAtDestinationEv
0039361c: push     {r4, r5, r6, lr}
00393620: mov      r2, r0
00393624: ldr      r3, [r2, #0x200]!
00393628: mov      r4, r0
0039362c: cmp      r3, r2
00393630: beq      #0x3936a8
00393634: ldr      r3, [r3]
00393638: cmp      r2, r3
0039363c: bne      #0x393634
00393640: ldr      r1, [r4, #0x160]
00393644: ldr      r0, [r4, #0x208]
00393648: bl       #0x30e3ac
0039364c: ldr      r1, [r4, #0x164]
00393650: mov      r6, r0
00393654: ldr      r0, [r4, #0x20c]
00393658: bl       #0x30e3ac
0039365c: mov      r1, r6
00393660: mov      r5, r0
00393664: mov      r0, r6
00393668: bl       #0x30ed6c
0039366c: mov      r1, r5
00393670: mov      r4, r0
00393674: mov      r0, r5
00393678: bl       #0x30ed6c
0039367c: mov      r1, r0
00393680: mov      r0, r4
00393684: bl       #0x30eba4
00393688: mov      r1, #0x45000000
0039368c: add      r1, r1, #0xc80000
00393690: bl       #0x30e70c
00393694: cmp      r0, #0
00393698: mov      r0, #0
0039369c: movne    r0, #1
003936a0: uxtb     r0, r0
003936a4: pop      {r4, r5, r6, pc}
003936a8: ldr      r1, [r0, #0x160]
003936ac: ldr      r0, [r0, #0x1a8]
003936b0: bl       #0x30e3ac
003936b4: ldr      r1, [r4, #0x164]
003936b8: mov      r6, r0
003936bc: ldr      r0, [r4, #0x1ac]
003936c0: bl       #0x30e3ac
003936c4: mov      r1, r6
003936c8: mov      r5, r0
003936cc: mov      r0, r6
003936d0: bl       #0x30ed6c
003936d4: mov      r1, r5
003936d8: mov      r4, r0
003936dc: mov      r0, r5
003936e0: bl       #0x30ed6c
003936e4: mov      r1, r0
003936e8: mov      r0, r4
003936ec: bl       #0x30eba4
003936f0: mov      r1, #0x45000000
003936f4: add      r1, r1, #0xc80000
003936f8: bl       #0x30e70c
003936fc: cmp      r0, #0
00393700: mov      r0, #0
00393704: movne    r0, #1
00393708: uxtb     r0, r0
0039370c: pop      {r4, r5, r6, pc}

# _ZNK9Character29IsUpdatingPositionFromPhysicsEv
003a2e44: ldr      r0, [r0, #0x520]
003a2e48: ubfx     r0, r0, #1, #1
003a2e4c: bx       lr

# _ZN10GameObject10UpdatePathEv
003940c0: push     {r4, r5, r6, r7, r8, sl, lr}
003940c4: ldr      r6, [pc, #0x258]
003940c8: ldr      r7, [pc, #0x258]
003940cc: ldr      ip, [r0, #0x160]
003940d0: add      r6, pc, r6
003940d4: ldr      r3, [r6, r7]
003940d8: ldr      r2, [r0, #0x164]
003940dc: sub      sp, sp, #0x2c
003940e0: ldr      r1, [r3]
003940e4: ldr      r3, [r0, #0x168]
003940e8: str      ip, [r0, #0x1e0]
003940ec: str      r1, [sp, #0x24]
003940f0: str      r3, [r0, #0x1e8]
003940f4: str      r2, [r0, #0x1e4]
003940f8: ldr      r3, [r0]
003940fc: mov      r5, r0
00394100: mov      lr, pc
00394104: ldr      pc, [r3, #0x5c]
00394108: cmp      r0, #0
0039410c: beq      #0x3941a8
00394110: mov      r3, r5
00394114: ldr      r4, [r3, #0x200]!
00394118: cmp      r4, r3
0039411c: beq      #0x394150
00394120: ldr      r4, [r4]
00394124: cmp      r3, r4
00394128: bne      #0x394120
0039412c: ldr      r3, [pc, #0x1f8]
00394130: add      r8, r5, #0x1a8
00394134: add      r1, r5, #0x1c8
00394138: ldr      r0, [r6, r3]
0039413c: mov      r2, r8
00394140: bl       #0x52d838
00394144: mov      r0, r5
00394148: mov      r1, r8
0039414c: bl       #0x393600
00394150: mov      r0, r5
00394154: bl       #0x39361c
00394158: cmp      r0, #0
0039415c: beq      #0x3941d4
00394160: ldrb     r3, [r5, #0x1b4]
00394164: cmp      r3, #0
00394168: beq      #0x394184
0039416c: ldr      r3, [r5, #0x200]
00394170: cmp      r3, r4
00394174: beq      #0x394314
00394178: ldr      r3, [r3]
0039417c: cmp      r3, r4
00394180: bne      #0x394178
00394184: ldrb     r3, [r5, #0x1b5]
00394188: cmp      r3, #0
0039418c: beq      #0x3941c4
00394190: ldr      r3, [r5, #0x1cc]
00394194: ldrb     r2, [r5, #0x1c4]
00394198: orr      r3, r3, #2
0039419c: cmp      r2, #0
003941a0: str      r3, [r5, #0x1cc]
003941a4: bne      #0x394278
003941a8: ldr      r3, [r6, r7]
003941ac: ldr      r2, [sp, #0x24]
003941b0: ldr      r3, [r3]
003941b4: cmp      r2, r3
003941b8: bne      #0x394320
003941bc: add      sp, sp, #0x2c
003941c0: pop      {r4, r5, r6, r7, r8, sl, pc}
003941c4: ldr      r3, [r5, #0x1cc]
003941c8: bic      r3, r3, #2
003941cc: str      r3, [r5, #0x1cc]
003941d0: b        #0x3941a8
003941d4: mov      r4, #1
003941d8: ldr      r1, [r5, #0x164]
003941dc: ldr      r0, [r5, #0x1ac]
003941e0: strb     r4, [r5, #0x1b4]
003941e4: bl       #0x30e3ac
003941e8: ldr      r1, [r5, #0x168]
003941ec: mov      sl, r0
003941f0: ldr      r0, [r5, #0x1b0]
003941f4: bl       #0x30e3ac
003941f8: ldr      r1, [r5, #0x160]
003941fc: mov      r8, r0
00394200: ldr      r0, [r5, #0x1a8]
00394204: bl       #0x30e3ac
00394208: mov      r1, sp
0039420c: str      r0, [sp]
00394210: mov      r2, r4
00394214: mov      r0, r5
00394218: str      sl, [sp, #4]
0039421c: str      r8, [sp, #8]
00394220: bl       #0x393be8
00394224: ldrb     r3, [r5, #0x1b5]
00394228: cmp      r3, #0
0039422c: beq      #0x3941c4
00394230: ldr      r3, [r5]
00394234: mov      r0, r5
00394238: mov      lr, pc
0039423c: ldr      pc, [r3, #0x7c]
00394240: cmp      r0, #0
00394244: beq      #0x394184
00394248: ldr      r3, [pc, #0xdc]
0039424c: add      r8, r5, #0x1b8
00394250: add      r1, r5, #0x1c8
00394254: ldr      r0, [r6, r3]
00394258: mov      r2, r8
0039425c: bl       #0x527cc4
00394260: mov      r0, r5
00394264: mov      r1, r8
00394268: mov      r2, r4
0039426c: bl       #0x393be8
00394270: ldrb     r3, [r5, #0x1b5]
00394274: b        #0x394188
00394278: ldr      r3, [pc, #0xb0]
0039427c: add      r4, sp, #0xc
00394280: ldr      r8, [r6, r3]
00394284: mov      r0, r8
00394288: bl       #0x337888
0039428c: mov      r0, r4
00394290: mov      r1, #0x15
00394294: str      r4, [sp, #0x1c]
00394298: str      r4, [sp, #0x20]
0039429c: bl       #0x31167c
003942a0: ldr      r1, [pc, #0x8c]
003942a4: mov      r2, #0x14
003942a8: ldr      r0, [sp, #0x20]
003942ac: add      r1, pc, r1
003942b0: bl       #0x30e868
003942b4: add      r3, r0, #0x14
003942b8: str      r3, [sp, #0x1c]
003942bc: mov      r3, #0
003942c0: strb     r3, [r0, #0x14]
003942c4: mov      r1, r4
003942c8: mov      r0, r8
003942cc: bl       #0x337a88
003942d0: mov      r8, r0
003942d4: eor      r8, r8, #1
003942d8: mov      r0, r4
003942dc: bl       #0x3139ac
003942e0: tst      r8, #0xff
003942e4: beq      #0x3941a8
003942e8: ldr      r3, [pc, #0x3c]
003942ec: add      r4, r5, #0x1b8
003942f0: add      r2, r5, #0x1c8
003942f4: mov      r1, r4
003942f8: ldr      r0, [r6, r3]
003942fc: bl       #0x525d60
00394300: mov      r0, r5
00394304: mov      r1, r4
00394308: mov      r2, #1
0039430c: bl       #0x393be8
00394310: b        #0x3941a8
00394314: mov      r0, r5
00394318: bl       #0x3938f8
0039431c: b        #0x394184
00394320: bl       #0x30e310
00394324: rsbeq    r0, r0, r0, asr #19
00394328: andeq    r4, r0, ip, lsr #1
0039432c: andeq    r1, r0, r4, lsl #4
00394330: andeq    r0, r0, r4, lsl #17
00394334: ldrheq   lr, [r2], #-0x5c

# _ZN10GameObject14SetDestinationERK7Point3DIfE
00393600: ldr      r3, [r1]
00393604: str      r3, [r0, #0x1a8]
00393608: ldr      r3, [r1, #4]
0039360c: str      r3, [r0, #0x1ac]
00393610: ldr      r3, [r1, #8]
00393614: str      r3, [r0, #0x1b0]
00393618: bx       lr

# _ZN10GameObject4StopEv
003938f8: push     {r4, r5, r6, lr}
003938fc: ldr      r5, [pc, #0xe0]
00393900: ldr      r3, [pc, #0xe0]
00393904: mov      r4, r0
00393908: add      r5, pc, r5
0039390c: ldr      r0, [r5, r3]
00393910: add      r1, r4, #0x1c8
00393914: bl       #0x52aae4
00393918: ldr      r3, [pc, #0xcc]
0039391c: ldr      r1, [r4, #0x168]
00393920: ldr      ip, [r4, #0x160]
00393924: ldr      r0, [r4, #0x164]
00393928: ldr      r3, [r5, r3]
0039392c: mov      r2, #0
00393930: str      r1, [r4, #0x1b0]
00393934: str      ip, [r4, #0x1a8]
00393938: str      r0, [r4, #0x1ac]
0039393c: strb     r2, [r4, #0x1b5]
00393940: strb     r2, [r4, #0x1b4]
00393944: ldr      r2, [r3]
00393948: ldr      r1, [r4, #0x2dc]
0039394c: str      r2, [r4, #0x1b8]
00393950: ldr      r2, [r3, #4]
00393954: cmp      r1, #0
00393958: str      r2, [r4, #0x1bc]
0039395c: ldr      r3, [r3, #8]
00393960: str      r3, [r4, #0x1c0]
00393964: beq      #0x3939e0
00393968: ldr      r3, [r4]
0039396c: mov      r0, r4
00393970: mov      lr, pc
00393974: ldr      pc, [r3, #0x64]
00393978: cmp      r0, #0
0039397c: beq      #0x3939e0
00393980: mov      r5, #0
00393984: mov      r2, r5
00393988: ldr      r0, [r4, #0x2dc]
0039398c: mov      r1, r5
00393990: bl       #0x46e918
00393994: ldr      r0, [r4, #0x2dc]
00393998: mov      r1, r5
0039399c: bl       #0x46e978
003939a0: ldr      r2, [r4, #0x164]
003939a4: ldr      r0, [r4, #0x2dc]
003939a8: ldr      r1, [r4, #0x160]
003939ac: bl       #0x46ea80
003939b0: ldr      r3, [r4, #0x2dc]
003939b4: ldr      r3, [r3, #0x14]
003939b8: ldrh     r2, [r3]
003939bc: str      r5, [r3, #0x54]
003939c0: str      r5, [r3, #0x8c]
003939c4: orr      r2, r2, #8
003939c8: strh     r2, [r3]
003939cc: str      r5, [r3, #0x40]
003939d0: str      r5, [r3, #0x44]
003939d4: str      r5, [r3, #0x48]
003939d8: str      r5, [r3, #0x4c]
003939dc: str      r5, [r3, #0x50]
003939e0: pop      {r4, r5, r6, pc}
003939e4: rsbeq    r1, r0, r8, lsl #3
003939e8: andeq    r1, r0, r4, lsl #4
003939ec: andeq    r3, r0, ip, lsr #30

# _ZNK9Character14IsUpdatingPathEv
003a2e2c: ldr      r0, [r0, #0x520]
003a2e30: ubfx     r0, r0, #7, #1
003a2e34: bx       lr
