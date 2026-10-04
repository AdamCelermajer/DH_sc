
# _ZN16CharStateMachine15SM_SetIdleStateEb
003c1a00: strb     r1, [r0, #0x3c]
003c1a04: mvn      r2, #0
003c1a08: mov      r1, #3
003c1a0c: mov      r3, #0
003c1a10: b        #0x3c1938

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

# _ZN14PhysicalObject5unpinEv
0046eae0: push     {r4, lr}
0046eae4: ldrb     r3, [r0, #0x27]
0046eae8: mov      r4, r0
0046eaec: cmp      r3, #0
0046eaf0: beq      #0x46eb1c
0046eaf4: mov      r3, #0
0046eaf8: strb     r3, [r0, #0x27]
0046eafc: ldr      r0, [r0, #0x14]
0046eb00: bl       #0x7e1818
0046eb04: ldr      r3, [r4, #0x14]
0046eb08: mov      r1, #0
0046eb0c: ldrh     r2, [r3]
0046eb10: str      r1, [r3, #0x8c]
0046eb14: bic      r2, r2, #8
0046eb18: strh     r2, [r3]
0046eb1c: pop      {r4, pc}

# _ZN6CharAIC1Ev
003ced50: ldr      r3, [pc, #0x14c]
003ced54: ldr      r2, [pc, #0x14c]
003ced58: push     {r4, r5, lr}
003ced5c: add      r3, pc, r3
003ced60: ldr      r2, [r3, r2]
003ced64: mov      r4, r0
003ced68: mov      r1, #0
003ced6c: add      r2, r2, #8
003ced70: str      r2, [r4]
003ced74: ldr      r2, [pc, #0x130]
003ced78: mov      r0, #1
003ced7c: mvn      ip, #0
003ced80: mov      r5, r4
003ced84: strb     r0, [r4, #0x55]
003ced88: str      r1, [r4, #8]
003ced8c: str      r1, [r4, #0xc]
003ced90: strb     r1, [r4, #0x18]
003ced94: str      r1, [r4, #0x1c]
003ced98: str      r1, [r4, #0x20]
003ced9c: strb     r1, [r4, #0x24]
003ceda0: str      r1, [r4, #0x28]
003ceda4: strb     r1, [r4, #0x2c]
003ceda8: str      r1, [r4, #0x30]
003cedac: str      r1, [r4, #0x34]
003cedb0: str      r1, [r4, #0x3c]
003cedb4: str      r1, [r4, #0x40]
003cedb8: str      r1, [r4, #0x44]
003cedbc: strb     r1, [r4, #0x49]
003cedc0: strb     r0, [r4, #0x4a]
003cedc4: strb     r0, [r4, #0x4b]
003cedc8: strb     r1, [r4, #0x4c]
003cedcc: strb     r0, [r4, #0x4d]
003cedd0: str      r1, [r4, #0x50]
003cedd4: strb     r0, [r4, #0x54]
003cedd8: str      r1, [r4, #0x58]
003ceddc: mov      r0, r4
003cede0: str      r1, [r4, #0x60]
003cede4: str      ip, [r4, #0x10]
003cede8: str      ip, [r4, #0x14]
003cedec: str      ip, [r4, #0x38]
003cedf0: strb     r1, [r5, #0x5c]!
003cedf4: str      r5, [r4, #0x68]
003cedf8: str      r5, [r4, #0x64]
003cedfc: str      r1, [r4, #0x6c]
003cee00: str      r1, [r4, #0x80]
003cee04: strb     r1, [r0, #0x7c]!
003cee08: ldr      r5, [r3, r2]
003cee0c: mov      r2, r4
003cee10: str      r0, [r4, #0x88]
003cee14: str      r0, [r4, #0x84]
003cee18: str      r1, [r4, #0x8c]
003cee1c: str      r1, [r4, #0x98]
003cee20: add      r0, r4, #0xac
003cee24: strb     r1, [r2, #0x94]!
003cee28: str      r2, [r4, #0xa0]
003cee2c: str      r0, [r4, #0xb0]
003cee30: str      ip, [r4, #0xcc]
003cee34: strb     r1, [r4, #0xd1]
003cee38: str      r2, [r4, #0x9c]
003cee3c: str      r1, [r4, #0xa4]
003cee40: str      r0, [r4, #0xac]
003cee44: str      r1, [r4, #0xb4]
003cee48: str      r1, [r4, #0xb8]
003cee4c: str      r1, [r4, #0xbc]
003cee50: str      r1, [r4, #0xc0]
003cee54: str      r1, [r4, #0xc4]
003cee58: str      r1, [r4, #0xc8]
003cee5c: strb     r1, [r4, #0xd0]
003cee60: ldr      r1, [r5, #0x18]
003cee64: ldr      r2, [r5, #0x10]
003cee68: sub      sp, sp, #0xc
003cee6c: sub      r3, r1, #4
003cee70: cmp      r2, r3
003cee74: str      r4, [sp, #4]
003cee78: beq      #0x3cee98
003cee7c: str      r4, [r2]
003cee80: ldr      r3, [r5, #0x10]
003cee84: add      r3, r3, #4
003cee88: str      r3, [r5, #0x10]
003cee8c: mov      r0, r4
003cee90: add      sp, sp, #0xc
003cee94: pop      {r4, r5, pc}
003cee98: add      r0, sp, #4
003cee9c: bl       #0x3ce810
003ceea0: b        #0x3cee8c
003ceea4: subseq   r5, ip, r4, lsr sp
003ceea8: andeq    r4, r0, ip, asr #12
003ceeac: andeq    r4, r0, ip, lsr #19

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

# _ZN9Character8InitPostEv
003b4d60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr      r5, [pc, #0x828]
003b4d68: ldr      r6, [pc, #0x828]
003b4d6c: movw     r3, #0x1394
003b4d70: add      r5, pc, r5
003b4d74: ldr      r2, [r5, r6]
003b4d78: ldrb     r7, [r0, r3]
003b4d7c: sub      sp, sp, #0xfc
003b4d80: ldr      r2, [r2]
003b4d84: cmp      r7, #0
003b4d88: mov      r4, r0
003b4d8c: str      r2, [sp, #0xf4]
003b4d90: beq      #0x3b4db0
003b4d94: ldr      r3, [r5, r6]
003b4d98: ldr      r2, [sp, #0xf4]
003b4d9c: ldr      r3, [r3]
003b4da0: cmp      r2, r3
003b4da4: bne      #0x3b5590
003b4da8: add      sp, sp, #0xfc
003b4dac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov      r2, #1
003b4db4: strb     r2, [r0, r3]
003b4db8: bl       #0x38bd64
003b4dbc: ldr      r3, [r4, #0x274]
003b4dc0: cmp      r0, r3
003b4dc4: bge      #0x3b4d94
003b4dc8: ldr      fp, [pc, #0x7cc]
003b4dcc: add      r8, sp, #0xdc
003b4dd0: ldr      sl, [r5, fp]
003b4dd4: mov      r0, sl
003b4dd8: bl       #0x337888
003b4ddc: ldr      r1, [pc, #0x7bc]
003b4de0: add      r2, sp, #0x48
003b4de4: mov      r0, r8
003b4de8: add      r1, pc, r1
003b4dec: bl       #0x3140ec
003b4df0: mov      r1, r8
003b4df4: mov      r0, sl
003b4df8: bl       #0x337a88
003b4dfc: mov      r0, r8
003b4e00: bl       #0x318254
003b4e04: movw     r3, #0x13fc
003b4e08: ldr      r2, [r4, r3]
003b4e0c: movw     r3, #0x13f8
003b4e10: ldr      r3, [r4, r3]
003b4e14: cmp      r3, r2
003b4e18: beq      #0x3b4e6c
003b4e1c: ldr      r1, [pc, #0x780]
003b4e20: add      r8, sp, #0x24
003b4e24: ldr      r3, [r4, #0x64]
003b4e28: ldr      r1, [r5, r1]
003b4e2c: mov      r0, r8
003b4e30: ldr      r1, [r1, #0x38]
003b4e34: str      r7, [sp]
003b4e38: str      r7, [sp, #4]
003b4e3c: bl       #0x34aca0
003b4e40: mov      r0, r8
003b4e44: mov      r1, r7
003b4e48: bl       #0x33fdc0
003b4e4c: cmp      r0, #0
003b4e50: beq      #0x3b4e6c
003b4e54: mov      r0, r8
003b4e58: bl       #0x33fee4
003b4e5c: subs     r1, r0, #0
003b4e60: beq      #0x3b4e6c
003b4e64: ldr      r0, [r4, #0x378]
003b4e68: bl       #0x405540
003b4e6c: mov      r0, r4
003b4e70: movw     r7, #0x13c8
003b4e74: bl       #0x3b3d38
003b4e78: ldrsh    r3, [r4, r7]
003b4e7c: cmn      r3, #1
003b4e80: movweq   r1, #0xffff
003b4e84: beq      #0x3b4ec4
003b4e88: ldr      sl, [r5, fp]
003b4e8c: add      r8, sp, #0xc4
003b4e90: mov      r0, sl
003b4e94: bl       #0x337888
003b4e98: ldr      r1, [pc, #0x708]
003b4e9c: add      r2, sp, #0x44
003b4ea0: mov      r0, r8
003b4ea4: add      r1, pc, r1
003b4ea8: bl       #0x3140ec
003b4eac: mov      r1, r8
003b4eb0: mov      r0, sl
003b4eb4: bl       #0x337a88
003b4eb8: mov      r0, r8
003b4ebc: bl       #0x318254
003b4ec0: ldrh     r1, [r4, r7]
003b4ec4: add      r7, r4, #0x560
003b4ec8: sxth     r1, r1
003b4ecc: mov      r0, r7
003b4ed0: bl       #0x3df2a4
003b4ed4: mov      r0, r7
003b4ed8: mov      r1, #1
003b4edc: bl       #0x3e0810
003b4ee0: mov      r0, r4
003b4ee4: bl       #0x3a54d4
003b4ee8: subs     r8, r0, #0
003b4eec: beq      #0x3b4f04
003b4ef0: bl       #0x30de54
003b4ef4: mov      r1, r8
003b4ef8: add      r2, r8, r0
003b4efc: add      r0, r4, #0x290
003b4f00: bl       #0x3109e0
003b4f04: ldr      sl, [r5, fp]
003b4f08: add      r8, sp, #0xac
003b4f0c: mov      r0, sl
003b4f10: bl       #0x337888
003b4f14: ldr      r1, [pc, #0x690]
003b4f18: add      r2, sp, #0x40
003b4f1c: mov      r0, r8
003b4f20: add      r1, pc, r1
003b4f24: bl       #0x3140ec
003b4f28: mov      r1, r8
003b4f2c: mov      r0, sl
003b4f30: bl       #0x337a88
003b4f34: mov      r0, r8
003b4f38: bl       #0x318254
003b4f3c: ldr      r0, [r4, #0x59c]
003b4f40: bl       #0x30e964
003b4f44: movw     r1, #0x74bc
003b4f48: movt     r1, #0x3c13
003b4f4c: bl       #0x30ed6c
003b4f50: str      r0, [r4, #0x120]
003b4f54: ldr      r0, [r4, #0x5a0]
003b4f58: bl       #0x30e964
003b4f5c: movw     r1, #0x74bc
003b4f60: movt     r1, #0x3c13
003b4f64: bl       #0x30ed6c
003b4f68: str      r0, [r4, #0x124]
003b4f6c: ldr      r0, [r4, #0x5a4]
003b4f70: bl       #0x30e964
003b4f74: movw     r1, #0xd70a
003b4f78: movt     r1, #0x3c23
003b4f7c: bl       #0x30ed6c
003b4f80: str      r0, [r4, #0x128]
003b4f84: mov      r0, r4
003b4f88: bl       #0x38be5c
003b4f8c: mov      r0, r4
003b4f90: bl       #0x38ab60
003b4f94: subs     r1, r0, #0
003b4f98: beq      #0x3b5488
003b4f9c: mov      r1, #2
003b4fa0: mov      r0, r4
003b4fa4: bl       #0x3bc4d0
003b4fa8: ldr      r3, [pc, #0x600]
003b4fac: mov      r0, r4
003b4fb0: ldr      r3, [r5, r3]
003b4fb4: ldr      r8, [r3]
003b4fb8: bl       #0x3a2fec
003b4fbc: mov      r3, #0x44
003b4fc0: mla      r8, r3, r0, r8
003b4fc4: ldr      r3, [r4]
003b4fc8: mov      r0, r4
003b4fcc: mov      lr, pc
003b4fd0: ldr      pc, [r3, #0x28]
003b4fd4: cmp      r0, #0
003b4fd8: bne      #0x3b4ff4
003b4fdc: ldrb     r3, [r8, #0x10]
003b4fe0: cmp      r3, #0
003b4fe4: beq      #0x3b4ff4
003b4fe8: mov      r3, #1
003b4fec: strb     r3, [r4, #0x3ec]
003b4ff0: b        #0x3b5014
003b4ff4: ldr      r3, [r4]
003b4ff8: mov      r0, r4
003b4ffc: mov      lr, pc
003b5000: ldr      pc, [r3, #0x28]
003b5004: cmp      r0, #0
003b5008: bne      #0x3b54a4
003b500c: add      r0, r4, #0x3c8
003b5010: bl       #0x3cf1f0
003b5014: movw     r3, #0x1488
003b5018: ldr      r2, [pc, #0x594]
003b501c: ldr      r1, [r4, r3]
003b5020: ldr      r3, [r8, #0x30]
003b5024: str      r2, [sp, #0xc]
003b5028: mov      r2, r4
003b502c: add      r1, r1, r3
003b5030: ldr      r3, [sp, #0xc]
003b5034: ldr      sb, [pc, #0x57c]
003b5038: add      sl, sp, #0x94
003b503c: ldr      r0, [r5, r3]
003b5040: bl       #0x495430
003b5044: movw     r3, #0x1484
003b5048: str      r0, [r4, r3]
003b504c: mov      r0, r4
003b5050: bl       #0x3b4738
003b5054: ldr      r8, [r5, fp]
003b5058: add      sb, pc, sb
003b505c: mov      r0, r8
003b5060: bl       #0x337888
003b5064: add      r2, sp, #0x3c
003b5068: mov      r0, sl
003b506c: mov      r1, sb
003b5070: bl       #0x3140ec
003b5074: mov      r1, sl
003b5078: mov      r0, r8
003b507c: bl       #0x337a88
003b5080: mov      r0, sl
003b5084: bl       #0x318254
003b5088: add      r0, r4, #0x490
003b508c: add      r0, r0, #0xc
003b5090: bl       #0x3c9f4c
003b5094: add      sl, sp, #0x7c
003b5098: mov      r0, r8
003b509c: bl       #0x337888
003b50a0: add      r2, sp, #0x38
003b50a4: mov      r0, sl
003b50a8: mov      r1, sb
003b50ac: bl       #0x3140ec
003b50b0: mov      r1, sl
003b50b4: mov      r0, r8
003b50b8: bl       #0x337a88
003b50bc: mov      r0, sl
003b50c0: bl       #0x318254
003b50c4: mov      r0, r4
003b50c8: bl       #0x3b3b00
003b50cc: add      sl, sp, #0x64
003b50d0: mov      r0, r8
003b50d4: bl       #0x337888
003b50d8: add      r2, sp, #0x34
003b50dc: mov      r1, sb
003b50e0: mov      r0, sl
003b50e4: bl       #0x3140ec
003b50e8: mov      r1, sl
003b50ec: mov      r0, r8
003b50f0: bl       #0x337a88
003b50f4: mov      r0, sl
003b50f8: bl       #0x318254
003b50fc: ldr      r3, [r4]
003b5100: mov      r0, r4
003b5104: mov      lr, pc
003b5108: ldr      pc, [r3, #0x28]
003b510c: cmp      r0, #0
003b5110: beq      #0x3b538c
003b5114: ldr      r3, [r4, #0x2d8]
003b5118: cmp      r3, #0
003b511c: beq      #0x3b512c
003b5120: ldr      r0, [r3, #8]
003b5124: mov      r1, #0
003b5128: bl       #0x59719c
003b512c: ldr      r2, [pc, #0x470]
003b5130: mov      r0, r4
003b5134: mov      r1, #4
003b5138: str      r2, [sp, #0x10]
003b513c: bl       #0x3bc4d0
003b5140: ldr      r3, [sp, #0x10]
003b5144: ldr      r0, [r5, r3]
003b5148: bl       #0x31f594
003b514c: cmp      r0, #0
003b5150: beq      #0x3b5160
003b5154: ldr      r1, [r0, #0x118]
003b5158: mov      r0, r4
003b515c: bl       #0x3bb950
003b5160: ldr      r2, [sp, #0x10]
003b5164: mov      r1, r4
003b5168: ldr      r3, [r5, r2]
003b516c: ldr      r0, [r3, #0x40]
003b5170: bl       #0x36effc
003b5174: cmp      r0, #0
003b5178: bne      #0x3b54dc
003b517c: movw     r3, #0x13c8
003b5180: ldrsh    r1, [r4, r3]
003b5184: mov      r0, r7
003b5188: bl       #0x3df2a4
003b518c: mov      r0, r7
003b5190: bl       #0x3df480
003b5194: mov      r0, r7
003b5198: mov      r1, #1
003b519c: bl       #0x3e0810
003b51a0: ldr      r2, [sp, #0x10]
003b51a4: mov      r1, r4
003b51a8: ldr      r3, [r5, r2]
003b51ac: ldr      r0, [r3, #0x40]
003b51b0: bl       #0x36effc
003b51b4: cmp      r0, #0
003b51b8: bne      #0x3b54d0
003b51bc: movw     r3, #0xc9ff
003b51c0: movt     r3, #0x3b9a
003b51c4: str      r3, [r4, #0x3a4]
003b51c8: mov      r2, #0
003b51cc: mov      r0, r7
003b51d0: mov      r1, #0xc2
003b51d4: bl       #0x3df6e0
003b51d8: ldr      r2, [sp, #0xc]
003b51dc: bic      r0, r0, r0, asr #31
003b51e0: strb     r0, [r4, #0x3a8]
003b51e4: ldr      r3, [r5, r2]
003b51e8: ldr      r2, [r3, #0x1c]
003b51ec: ldr      r3, [r3, #0x20]
003b51f0: rsb      r3, r2, r3
003b51f4: asr      r3, r3, #3
003b51f8: add      r2, r3, r3, lsl #2
003b51fc: add      r2, r2, r2, lsl #4
003b5200: add      r2, r2, r2, lsl #8
003b5204: add      r2, r2, r2, lsl #16
003b5208: add      r3, r3, r2, lsl #1
003b520c: cmp      r3, #0
003b5210: beq      #0x3b538c
003b5214: mov      r0, #0x24
003b5218: mov      r1, #0
003b521c: bl       #0x31056c
003b5220: movw     r3, #0x1494
003b5224: str      r0, [r4, r3]
003b5228: ldr      r3, [pc, #0x38c]
003b522c: mov      sb, r0
003b5230: ldr      r3, [r5, r3]
003b5234: ldr      sl, [r3]
003b5238: cmp      sl, #0
003b523c: beq      #0x3b550c
003b5240: ldr      r3, [pc, #0x378]
003b5244: ldr      r2, [pc, #0x378]
003b5248: str      r7, [sp, #0x18]
003b524c: ldr      r3, [r5, r3]
003b5250: add      r2, pc, r2
003b5254: mov      r8, #0
003b5258: ldr      r3, [r3]
003b525c: str      r0, [sp, #0x14]
003b5260: mov      sb, r2
003b5264: mov      r7, r3
003b5268: b        #0x3b5278
003b526c: add      r8, r8, #1
003b5270: cmp      r8, sl
003b5274: beq      #0x3b5504
003b5278: mov      r0, sb
003b527c: ldr      r1, [r7, r8, lsl #2]
003b5280: bl       #0x30e31c
003b5284: cmp      r0, #0
003b5288: bne      #0x3b526c
003b528c: ldr      sb, [sp, #0x14]
003b5290: ldr      r7, [sp, #0x18]
003b5294: mov      r1, r8
003b5298: ldr      r3, [sp, #0xc]
003b529c: str      r7, [sp, #0x14]
003b52a0: str      fp, [sp, #0x18]
003b52a4: ldr      r2, [r5, r3]
003b52a8: ldr      r3, [pc, #0x318]
003b52ac: str      r6, [sp, #0x1c]
003b52b0: mov      r8, #0
003b52b4: movw     sl, #0x1494
003b52b8: mov      r7, r1
003b52bc: mov      r6, r2
003b52c0: mov      fp, r3
003b52c4: b        #0x3b52cc
003b52c8: ldr      sb, [r4, sl]
003b52cc: mov      r0, r6
003b52d0: add      r1, r7, r8
003b52d4: mov      r2, #0
003b52d8: bl       #0x495430
003b52dc: str      r0, [sb, r8, lsl #2]
003b52e0: ldr      r3, [r4, sl]
003b52e4: ldr      r3, [r3, r8, lsl #2]
003b52e8: cmp      r3, #0
003b52ec: beq      #0x3b5350
003b52f0: ldr      r2, [r5, fp]
003b52f4: mov      r0, r3
003b52f8: mov      r1, #0
003b52fc: ldr      lr, [r2]
003b5300: ldr      ip, [r2, #4]
003b5304: ldr      r2, [r2, #8]
003b5308: str      lr, [r3, #0x34]
003b530c: str      ip, [r3, #0x38]
003b5310: str      r2, [r3, #0x3c]
003b5314: bl       #0x492aa0
003b5318: ldr      r3, [r4, sl]
003b531c: mov      r1, #0
003b5320: ldr      r0, [r3, r8, lsl #2]
003b5324: bl       #0x492ef0
003b5328: ldr      r3, [r4, sl]
003b532c: ldr      r0, [r3, r8, lsl #2]
003b5330: bl       #0x49267c
003b5334: ldr      r3, [r0]
003b5338: mov      lr, pc
003b533c: ldr      pc, [r3, #0x44]
003b5340: mov      r1, #1
003b5344: ldr      r3, [r0]
003b5348: mov      lr, pc
003b534c: ldr      pc, [r3, #0x40]
003b5350: add      r8, r8, #1
003b5354: cmp      r8, #9
003b5358: bne      #0x3b52c8
003b535c: ldr      r2, [sp, #0x10]
003b5360: mov      r1, r4
003b5364: ldr      r7, [sp, #0x14]
003b5368: ldr      r3, [r5, r2]
003b536c: ldr      fp, [sp, #0x18]
003b5370: ldr      r6, [sp, #0x1c]
003b5374: ldr      r0, [r3, #0x40]
003b5378: bl       #0x36effc
003b537c: cmp      r0, #0
003b5380: bne      #0x3b5514
003b5384: mov      r0, r4
003b5388: bl       #0x3a41a0
003b538c: add      r8, r4, #0xff0
003b5390: add      r8, r8, #4
003b5394: mov      r1, r8
003b5398: mov      r2, #0xd2
003b539c: mov      r0, r7
003b53a0: bl       #0x3dedb4
003b53a4: bl       #0x30e964
003b53a8: mov      r3, #0x1440
003b53ac: str      r0, [r4, r3]
003b53b0: mov      r2, #0xd3
003b53b4: mov      r1, r8
003b53b8: mov      r0, r7
003b53bc: bl       #0x3dedb4
003b53c0: bl       #0x30e964
003b53c4: movw     r3, #0x1444
003b53c8: str      r0, [r4, r3]
003b53cc: add      r1, r4, #0x160
003b53d0: mov      r0, r4
003b53d4: bl       #0x3a58f4
003b53d8: add      r1, r4, #0x1440
003b53dc: mov      r0, r4
003b53e0: add      r1, r1, #0x10
003b53e4: mov      r2, #1
003b53e8: bl       #0x393db4
003b53ec: ldr      ip, [r4, #0x16c]
003b53f0: ldr      r0, [r4, #0x2d8]
003b53f4: ldr      r1, [r4, #0x170]
003b53f8: ldr      r2, [r4, #0x174]
003b53fc: movw     r3, #0x145c
003b5400: str      ip, [r4, r3]
003b5404: movw     r3, #0x1460
003b5408: str      r1, [r4, r3]
003b540c: cmp      r0, #0
003b5410: movw     r3, #0x1464
003b5414: str      r2, [r4, r3]
003b5418: beq      #0x3b5420
003b541c: bl       #0x470a54
003b5420: mov      r1, #0
003b5424: mov      r2, #1
003b5428: mov      r0, r4
003b542c: bl       #0x3a59ac
003b5430: mov      r0, r4
003b5434: bl       #0x3b3a70
003b5438: ldrb     r1, [r4, #0x3ec]
003b543c: cmp      r1, #0
003b5440: beq      #0x3b54c4
003b5444: mov      r0, r4
003b5448: bl       #0x3d37d0
003b544c: ldr      r7, [r5, fp]
003b5450: add      r4, sp, #0x4c
003b5454: mov      r0, r7
003b5458: bl       #0x337888
003b545c: ldr      r1, [pc, #0x168]
003b5460: add      r2, sp, #0x30
003b5464: mov      r0, r4
003b5468: add      r1, pc, r1
003b546c: bl       #0x3140ec
003b5470: mov      r0, r7
003b5474: mov      r1, r4
003b5478: bl       #0x337a88
003b547c: mov      r0, r4
003b5480: bl       #0x318254
003b5484: b        #0x3b4d94
003b5488: mov      r0, r4
003b548c: ldr      r3, [r4]
003b5490: mov      lr, pc
003b5494: ldr      pc, [r3, #0x40]
003b5498: mov      r0, r4
003b549c: bl       #0x33ddb4
003b54a0: b        #0x3b4d94
003b54a4: ldr      r3, [pc, #0xf8]
003b54a8: mov      r1, r4
003b54ac: ldr      r3, [r5, r3]
003b54b0: ldr      r0, [r3, #0x40]
003b54b4: bl       #0x36effc
003b54b8: cmp      r0, #0
003b54bc: bne      #0x3b500c
003b54c0: b        #0x3b4fe8
003b54c4: add      r0, r4, #0x3c8
003b54c8: bl       #0x3ce7c0
003b54cc: b        #0x3b5444
003b54d0: mov      r0, r4
003b54d4: bl       #0x3b3a90
003b54d8: b        #0x3b51bc
003b54dc: mov      r0, r4
003b54e0: bl       #0x3b395c
003b54e4: mov      r0, r7
003b54e8: bl       #0x3defac
003b54ec: movw     r3, #0x14e8
003b54f0: ldr      r0, [r4, r3]
003b54f4: cmp      r0, #0
003b54f8: beq      #0x3b517c
003b54fc: bl       #0x4679e8
003b5500: b        #0x3b517c
003b5504: ldr      sb, [sp, #0x14]
003b5508: ldr      r7, [sp, #0x18]
003b550c: mvn      r1, #0
003b5510: b        #0x3b5298
003b5514: ldr      r3, [sp, #0xc]
003b5518: mov      r2, #0
003b551c: movw     r8, #0x149c
003b5520: ldr      r0, [r5, r3]
003b5524: ldr      r3, [pc, #0xa4]
003b5528: ldr      r3, [r5, r3]
003b552c: ldr      r3, [r3]
003b5530: ldr      r1, [r3, #0x88]
003b5534: bl       #0x495430
003b5538: cmp      r0, #0
003b553c: str      r0, [r4, r8]
003b5540: beq      #0x3b5384
003b5544: mov      r2, #0
003b5548: str      r2, [r0, #0x3c]
003b554c: str      r2, [r0, #0x34]
003b5550: str      r2, [r0, #0x38]
003b5554: mov      r1, #0
003b5558: bl       #0x492aa0
003b555c: mov      r1, #0
003b5560: ldr      r0, [r4, r8]
003b5564: bl       #0x492ef0
003b5568: ldr      r0, [r4, r8]
003b556c: bl       #0x49267c
003b5570: ldr      r3, [r0]
003b5574: mov      lr, pc
003b5578: ldr      pc, [r3, #0x44]
003b557c: mov      r1, #1
003b5580: ldr      r3, [r0]
003b5584: mov      lr, pc
003b5588: ldr      pc, [r3, #0x40]
003b558c: b        #0x3b5384
003b5590: bl       #0x30e310
003b5594: subseq   pc, sp, r0, lsr #26
003b5598: andeq    r4, r0, ip, lsr #1
003b559c: andeq    r0, r0, r4, lsl #17
003b55a0: subseq   pc, r0, r8, lsr #32
003b55a4: strdeq   r3, r4, [r0], -r4
003b55a8: subseq   lr, r0, ip, ror #30
003b55ac: ldrsheq  lr, [r0], #-0xe0
003b55b0: andeq    r0, r0, r8, asr r7
003b55b4: andeq    r1, r0, r8, lsl #22
003b55b8: ldrheq   lr, [r0], #-0xd8
003b55bc: andeq    r0, r0, r4, asr #13
003b55c0: muleq    r0, r4, r2
003b55c4: subseq   lr, r0, r0, ror #24
003b55c8: andeq    r3, r0, ip, lsr #30
003b55cc: subseq   lr, r0, r8, lsr #19
003b55d0: andeq    r3, r0, r8, asr #5

# _ZN12CharAnimatorC1Ev
003c906c: ldr      r1, [pc, #0x68]
003c9070: ldr      ip, [pc, #0x68]
003c9074: mov      r2, #0
003c9078: add      r1, pc, r1
003c907c: ldr      ip, [r1, ip]
003c9080: push     {r4, r5}
003c9084: add      ip, ip, #8
003c9088: mov      r5, #0x3f800000
003c908c: mvn      r4, #0
003c9090: str      ip, [r0]
003c9094: mov      ip, #1
003c9098: strb     r2, [r0, #0x5c]
003c909c: str      r5, [r0, #0x40]
003c90a0: strb     ip, [r0, #0x48]
003c90a4: str      r4, [r0, #0x50]
003c90a8: str      r2, [r0, #4]
003c90ac: str      r2, [r0, #0x2c]
003c90b0: strb     r2, [r0, #0x30]
003c90b4: str      r5, [r0, #0x34]
003c90b8: strb     r2, [r0, #0x38]
003c90bc: str      r4, [r0, #0x3c]
003c90c0: str      r2, [r0, #0x44]
003c90c4: strb     r2, [r0, #0x49]
003c90c8: strb     r2, [r0, #0x4a]
003c90cc: strb     r2, [r0, #0x54]
003c90d0: str      r2, [r0, #0x58]
003c90d4: pop      {r4, r5}
003c90d8: bx       lr
003c90dc: subseq   fp, ip, r8, lsl sl
003c90e0: andeq    r1, r0, r4, lsl #14

# _ZNK14CharProperties18PROPS_GetWalkSpeedEv
003de6c4: push     {r4, lr}
003de6c8: ldr      r0, [r0, #0xb50]
003de6cc: bl       #0x30e964
003de6d0: mov      r1, #0x3b800000
003de6d4: bl       #0x30ed6c
003de6d8: movw     r1, #0xd70a
003de6dc: movt     r1, #0x3c23
003de6e0: bl       #0x30ed6c
003de6e4: mov      r1, #0x3f800000
003de6e8: bl       #0x30eba4
003de6ec: mov      r1, #0
003de6f0: mov      r4, r0
003de6f4: bl       #0x30e2f8
003de6f8: cmp      r0, #0
003de6fc: moveq    r4, #0
003de700: mov      r0, r4
003de704: pop      {r4, pc}

# _ZN10GameObjectC2EN10ObjectBase6GO_IDSE
0038c398: push     {r4, r5, r6, r7, lr}
0038c39c: ldr      r6, [pc, #0x254]
0038c3a0: sub      sp, sp, #0xc
0038c3a4: mov      r4, r0
0038c3a8: bl       #0x33f310
0038c3ac: ldr      r2, [pc, #0x248]
0038c3b0: add      r6, pc, r6
0038c3b4: mov      r3, #0
0038c3b8: ldr      r2, [r6, r2]
0038c3bc: mov      r5, #0
0038c3c0: str      r3, [r4, #0x120]
0038c3c4: add      r1, r2, #0xe4
0038c3c8: add      r0, r2, #8
0038c3cc: add      r2, r2, #0xd8
0038c3d0: str      r2, [r4, #4]
0038c3d4: str      r1, [r4, #0x24]
0038c3d8: str      r0, [r4]
0038c3dc: str      r3, [r4, #0x124]
0038c3e0: str      r3, [r4, #0x128]
0038c3e4: str      r3, [r4, #0x12c]
0038c3e8: str      r3, [r4, #0x130]
0038c3ec: str      r3, [r4, #0x134]
0038c3f0: str      r3, [r4, #0x138]
0038c3f4: str      r3, [r4, #0x13c]
0038c3f8: str      r3, [r4, #0x140]
0038c3fc: str      r3, [r4, #0x144]
0038c400: str      r3, [r4, #0x148]
0038c404: str      r3, [r4, #0x14c]
0038c408: str      r3, [r4, #0x150]
0038c40c: str      r3, [r4, #0x154]
0038c410: str      r3, [r4, #0x158]
0038c414: str      r3, [r4, #0x160]
0038c418: str      r3, [r4, #0x164]
0038c41c: str      r3, [r4, #0x168]
0038c420: str      r3, [r4, #0x16c]
0038c424: str      r3, [r4, #0x170]
0038c428: str      r3, [r4, #0x174]
0038c42c: str      r3, [r4, #0x178]
0038c430: str      r3, [r4, #0x184]
0038c434: str      r3, [r4, #0x188]
0038c438: str      r3, [r4, #0x18c]
0038c43c: str      r3, [r4, #0x190]
0038c440: str      r3, [r4, #0x194]
0038c444: strb     r5, [r4, #0x15c]
0038c448: str      r5, [r4, #0x180]
0038c44c: add      r0, r4, #0x1c8
0038c450: str      r3, [r4, #0x198]
0038c454: str      r3, [r4, #0x1c0]
0038c458: str      r3, [r4, #0x19c]
0038c45c: str      r3, [r4, #0x1a0]
0038c460: str      r3, [r4, #0x1a4]
0038c464: str      r3, [r4, #0x1a8]
0038c468: str      r3, [r4, #0x1ac]
0038c46c: str      r3, [r4, #0x1b0]
0038c470: strb     r5, [r4, #0x1b4]
0038c474: strb     r5, [r4, #0x1b5]
0038c478: str      r3, [r4, #0x1b8]
0038c47c: str      r3, [r4, #0x1bc]
0038c480: strb     r5, [r4, #0x1c4]
0038c484: bl       #0x524644
0038c488: add      r3, r4, #0x278
0038c48c: mvn      r7, #0
0038c490: mov      r2, #0x64
0038c494: str      r2, [r4, #0x274]
0038c498: mov      r0, r3
0038c49c: str      r3, [r4, #0x288]
0038c4a0: str      r3, [r4, #0x28c]
0038c4a4: str      r5, [r4, #0x26c]
0038c4a8: str      r7, [r4, #0x270]
0038c4ac: mov      r1, #0x10
0038c4b0: bl       #0x31167c
0038c4b4: ldr      r2, [r4, #0x288]
0038c4b8: add      r3, r4, #0x290
0038c4bc: mov      r0, r3
0038c4c0: strb     r5, [r2]
0038c4c4: mov      r1, #0x10
0038c4c8: str      r3, [r4, #0x2a0]
0038c4cc: str      r3, [r4, #0x2a4]
0038c4d0: bl       #0x31167c
0038c4d4: ldr      r2, [r4, #0x2a0]
0038c4d8: add      r3, r4, #0x2a8
0038c4dc: mov      r0, r3
0038c4e0: strb     r5, [r2]
0038c4e4: mov      r1, #0x10
0038c4e8: str      r3, [r4, #0x2b8]
0038c4ec: str      r3, [r4, #0x2bc]
0038c4f0: bl       #0x31167c
0038c4f4: ldr      r2, [r4, #0x2b8]
0038c4f8: add      r3, r4, #0x2c0
0038c4fc: mov      r0, r3
0038c500: strb     r5, [r2]
0038c504: mov      r1, #0x10
0038c508: str      r3, [r4, #0x2d0]
0038c50c: str      r3, [r4, #0x2d4]
0038c510: bl       #0x31167c
0038c514: ldr      r3, [r4, #0x2d0]
0038c518: mov      ip, #1
0038c51c: add      r6, r4, #0x304
0038c520: strb     r5, [r3]
0038c524: mov      r2, ip
0038c528: strb     ip, [r4, #0x2ee]
0038c52c: strb     ip, [r4, #0x2fb]
0038c530: str      r5, [r4, #0x2d8]
0038c534: str      r5, [r4, #0x2dc]
0038c538: str      r5, [r4, #0x2e0]
0038c53c: str      r5, [r4, #0x2e4]
0038c540: str      r5, [r4, #0x2e8]
0038c544: strb     r5, [r4, #0x2ec]
0038c548: strb     r5, [r4, #0x2ed]
0038c54c: strb     r5, [r4, #0x2ef]
0038c550: strb     r5, [r4, #0x2f0]
0038c554: str      r5, [r4, #0x2f4]
0038c558: strb     r5, [r4, #0x2f8]
0038c55c: strb     r5, [r4, #0x2f9]
0038c560: strb     r5, [r4, #0x2fa]
0038c564: strb     r5, [r4, #0x2fc]
0038c568: str      r5, [r4, #0x300]
0038c56c: mov      r3, r5
0038c570: mov      r1, r5
0038c574: mov      r0, r6
0038c578: str      ip, [sp]
0038c57c: bl       #0x4a2730
0038c580: add      r3, r4, #0x358
0038c584: mov      r0, r3
0038c588: str      r3, [r4, #0x368]
0038c58c: str      r3, [r4, #0x36c]
0038c590: mov      r1, #0x10
0038c594: bl       #0x31167c
0038c598: ldr      r1, [r4, #0x368]
0038c59c: mov      r2, #0xc2000000
0038c5a0: mov      r3, #0x42000000
0038c5a4: strb     r5, [r1]
0038c5a8: add      r2, r2, #0xc80000
0038c5ac: add      r3, r3, #0xc80000
0038c5b0: mov      r1, #0x370
0038c5b4: strh     r7, [r4, r1]
0038c5b8: mov      r0, r4
0038c5bc: str      r2, [r4, #0x14c]
0038c5c0: str      r3, [r4, #0x158]
0038c5c4: str      r2, [r4, #0x144]
0038c5c8: str      r2, [r4, #0x148]
0038c5cc: str      r3, [r4, #0x150]
0038c5d0: str      r3, [r4, #0x154]
0038c5d4: strb     r5, [r4, #0x373]
0038c5d8: strb     r5, [r4, #0x372]
0038c5dc: bl       #0x38aac8
0038c5e0: mov      r0, r6
0038c5e4: mov      r1, r4
0038c5e8: bl       #0x4a191c
0038c5ec: mov      r0, r4
0038c5f0: add      sp, sp, #0xc
0038c5f4: pop      {r4, r5, r6, r7, pc}
0038c5f8: rsbeq    r8, r0, r0, ror #13
0038c5fc: andeq    r2, r0, r0, ror sp

# _ZN16CharStateMachineC1Ev
003c1ac4: ldr      ip, [pc, #0x84]
003c1ac8: str      r4, [sp, #-4]!
003c1acc: ldr      r4, [pc, #0x80]
003c1ad0: add      ip, pc, ip
003c1ad4: mov      r2, #0
003c1ad8: ldr      r4, [ip, r4]
003c1adc: mov      r1, r0
003c1ae0: str      r2, [r0, #4]
003c1ae4: add      r4, r4, #8
003c1ae8: str      r4, [r0]
003c1aec: str      r2, [r0, #0xc]
003c1af0: mvn      r4, #0
003c1af4: strb     r2, [r1, #8]!
003c1af8: str      r1, [r0, #0x14]
003c1afc: str      r4, [r0, #0x28]
003c1b00: str      r2, [r0, #0x5c]
003c1b04: str      r1, [r0, #0x10]
003c1b08: str      r2, [r0, #0x18]
003c1b0c: str      r2, [r0, #0x20]
003c1b10: str      r2, [r0, #0x24]
003c1b14: str      r2, [r0, #0x60]
003c1b18: str      r2, [r0, #0x2c]
003c1b1c: str      r2, [r0, #0x30]
003c1b20: str      r2, [r0, #0x34]
003c1b24: str      r2, [r0, #0x38]
003c1b28: str      r2, [r0, #0x3c]
003c1b2c: str      r2, [r0, #0x40]
003c1b30: str      r2, [r0, #0x44]
003c1b34: str      r2, [r0, #0x48]
003c1b38: str      r2, [r0, #0x4c]
003c1b3c: str      r2, [r0, #0x50]
003c1b40: str      r2, [r0, #0x54]
003c1b44: str      r2, [r0, #0x58]
003c1b48: ldm      sp!, {r4}
003c1b4c: bx       lr
003c1b50: subseq   r2, sp, r0, asr #31
003c1b54: strdeq   r2, r3, [r0], -r8

# _ZN6CharAI17InitScriptProcessEb
003ce7c0: push     {r4, r5, r6, lr}
003ce7c4: mov      r4, r0
003ce7c8: ldr      r0, [r0, #4]
003ce7cc: mov      r5, r1
003ce7d0: bl       #0x3b3a70
003ce7d4: mov      r0, r4
003ce7d8: bl       #0x3ce044
003ce7dc: mov      r0, r4
003ce7e0: bl       #0x3d8894
003ce7e4: ldr      r3, [r4]
003ce7e8: mov      r0, r4
003ce7ec: mov      lr, pc
003ce7f0: ldr      pc, [r3, #0xc]
003ce7f4: cmp      r5, #0
003ce7f8: beq      #0x3ce80c
003ce7fc: mov      r0, r4
003ce800: ldr      r3, [r4]
003ce804: mov      lr, pc
003ce808: ldr      pc, [r3, #0x10]
003ce80c: pop      {r4, r5, r6, pc}

# _ZNK9Character8IsPlayerEv
003a49f0: push     {r4, r5, r6, lr}
003a49f4: mov      r5, r0
003a49f8: bl       #0x3a3054
003a49fc: cmp      r0, #0
003a4a00: beq      #0x3a4a14
003a4a04: cmp      r0, #1
003a4a08: movne    r0, #0
003a4a0c: moveq    r0, #1
003a4a10: pop      {r4, r5, r6, pc}
003a4a14: ldr      r4, [r5, #0x44]
003a4a18: ldr      r1, [pc, #0x18]
003a4a1c: mov      r0, r4
003a4a20: add      r1, pc, r1
003a4a24: bl       #0x30ebd4
003a4a28: cmp      r4, r0
003a4a2c: movne    r0, #0
003a4a30: moveq    r0, #1
003a4a34: pop      {r4, r5, r6, pc}
003a4a38: subseq   lr, r1, r8, asr #14

# _ZN10GameObjectC1EN10ObjectBase6GO_IDSE
0038c130: push     {r4, r5, r6, r7, lr}
0038c134: ldr      r6, [pc, #0x254]
0038c138: sub      sp, sp, #0xc
0038c13c: mov      r4, r0
0038c140: bl       #0x33f310
0038c144: ldr      r2, [pc, #0x248]
0038c148: add      r6, pc, r6
0038c14c: mov      r3, #0
0038c150: ldr      r2, [r6, r2]
0038c154: mov      r5, #0
0038c158: str      r3, [r4, #0x120]
0038c15c: add      r1, r2, #0xe4
0038c160: add      r0, r2, #8
0038c164: add      r2, r2, #0xd8
0038c168: str      r2, [r4, #4]
0038c16c: str      r1, [r4, #0x24]
0038c170: str      r0, [r4]
0038c174: str      r3, [r4, #0x124]
0038c178: str      r3, [r4, #0x128]
0038c17c: str      r3, [r4, #0x12c]
0038c180: str      r3, [r4, #0x130]
0038c184: str      r3, [r4, #0x134]
0038c188: str      r3, [r4, #0x138]
0038c18c: str      r3, [r4, #0x13c]
0038c190: str      r3, [r4, #0x140]
0038c194: str      r3, [r4, #0x144]
0038c198: str      r3, [r4, #0x148]
0038c19c: str      r3, [r4, #0x14c]
0038c1a0: str      r3, [r4, #0x150]
0038c1a4: str      r3, [r4, #0x154]
0038c1a8: str      r3, [r4, #0x158]
0038c1ac: str      r3, [r4, #0x160]
0038c1b0: str      r3, [r4, #0x164]
0038c1b4: str      r3, [r4, #0x168]
0038c1b8: str      r3, [r4, #0x16c]
0038c1bc: str      r3, [r4, #0x170]
0038c1c0: str      r3, [r4, #0x174]
0038c1c4: str      r3, [r4, #0x178]
0038c1c8: str      r3, [r4, #0x184]
0038c1cc: str      r3, [r4, #0x188]
0038c1d0: str      r3, [r4, #0x18c]
0038c1d4: str      r3, [r4, #0x190]
0038c1d8: str      r3, [r4, #0x194]
0038c1dc: strb     r5, [r4, #0x15c]
0038c1e0: str      r5, [r4, #0x180]
0038c1e4: add      r0, r4, #0x1c8
0038c1e8: str      r3, [r4, #0x198]
0038c1ec: str      r3, [r4, #0x1c0]
0038c1f0: str      r3, [r4, #0x19c]
0038c1f4: str      r3, [r4, #0x1a0]
0038c1f8: str      r3, [r4, #0x1a4]
0038c1fc: str      r3, [r4, #0x1a8]
0038c200: str      r3, [r4, #0x1ac]
0038c204: str      r3, [r4, #0x1b0]
0038c208: strb     r5, [r4, #0x1b4]
0038c20c: strb     r5, [r4, #0x1b5]
0038c210: str      r3, [r4, #0x1b8]
0038c214: str      r3, [r4, #0x1bc]
0038c218: strb     r5, [r4, #0x1c4]
0038c21c: bl       #0x524644
0038c220: add      r3, r4, #0x278
0038c224: mvn      r7, #0
0038c228: mov      r2, #0x64
0038c22c: str      r2, [r4, #0x274]
0038c230: mov      r0, r3
0038c234: str      r3, [r4, #0x288]
0038c238: str      r3, [r4, #0x28c]
0038c23c: str      r5, [r4, #0x26c]
0038c240: str      r7, [r4, #0x270]
0038c244: mov      r1, #0x10
0038c248: bl       #0x31167c
0038c24c: ldr      r2, [r4, #0x288]
0038c250: add      r3, r4, #0x290
0038c254: mov      r0, r3
0038c258: strb     r5, [r2]
0038c25c: mov      r1, #0x10
0038c260: str      r3, [r4, #0x2a0]
0038c264: str      r3, [r4, #0x2a4]
0038c268: bl       #0x31167c
0038c26c: ldr      r2, [r4, #0x2a0]
0038c270: add      r3, r4, #0x2a8
0038c274: mov      r0, r3
0038c278: strb     r5, [r2]
0038c27c: mov      r1, #0x10
0038c280: str      r3, [r4, #0x2b8]
0038c284: str      r3, [r4, #0x2bc]
0038c288: bl       #0x31167c
0038c28c: ldr      r2, [r4, #0x2b8]
0038c290: add      r3, r4, #0x2c0
0038c294: mov      r0, r3
0038c298: strb     r5, [r2]
0038c29c: mov      r1, #0x10
0038c2a0: str      r3, [r4, #0x2d0]
0038c2a4: str      r3, [r4, #0x2d4]
0038c2a8: bl       #0x31167c
0038c2ac: ldr      r3, [r4, #0x2d0]
0038c2b0: mov      ip, #1
0038c2b4: add      r6, r4, #0x304
0038c2b8: strb     r5, [r3]
0038c2bc: mov      r2, ip
0038c2c0: strb     ip, [r4, #0x2ee]
0038c2c4: strb     ip, [r4, #0x2fb]
0038c2c8: str      r5, [r4, #0x2d8]
0038c2cc: str      r5, [r4, #0x2dc]
0038c2d0: str      r5, [r4, #0x2e0]
0038c2d4: str      r5, [r4, #0x2e4]
0038c2d8: str      r5, [r4, #0x2e8]
0038c2dc: strb     r5, [r4, #0x2ec]
0038c2e0: strb     r5, [r4, #0x2ed]
0038c2e4: strb     r5, [r4, #0x2ef]
0038c2e8: strb     r5, [r4, #0x2f0]
0038c2ec: str      r5, [r4, #0x2f4]
0038c2f0: strb     r5, [r4, #0x2f8]
0038c2f4: strb     r5, [r4, #0x2f9]
0038c2f8: strb     r5, [r4, #0x2fa]
0038c2fc: strb     r5, [r4, #0x2fc]
0038c300: str      r5, [r4, #0x300]
0038c304: mov      r3, r5
0038c308: mov      r1, r5
0038c30c: mov      r0, r6
0038c310: str      ip, [sp]
0038c314: bl       #0x4a2730
0038c318: add      r3, r4, #0x358
0038c31c: mov      r0, r3
0038c320: str      r3, [r4, #0x368]
0038c324: str      r3, [r4, #0x36c]
0038c328: mov      r1, #0x10
0038c32c: bl       #0x31167c
0038c330: ldr      r1, [r4, #0x368]
0038c334: mov      r2, #0xc2000000
0038c338: mov      r3, #0x42000000
0038c33c: strb     r5, [r1]
0038c340: add      r2, r2, #0xc80000
0038c344: add      r3, r3, #0xc80000
0038c348: mov      r1, #0x370
0038c34c: strh     r7, [r4, r1]
0038c350: mov      r0, r4
0038c354: str      r2, [r4, #0x14c]
0038c358: str      r3, [r4, #0x158]
0038c35c: str      r2, [r4, #0x144]
0038c360: str      r2, [r4, #0x148]
0038c364: str      r3, [r4, #0x150]
0038c368: str      r3, [r4, #0x154]
0038c36c: strb     r5, [r4, #0x373]
0038c370: strb     r5, [r4, #0x372]
0038c374: bl       #0x38aac8
0038c378: mov      r0, r6
0038c37c: mov      r1, r4
0038c380: bl       #0x4a191c
0038c384: mov      r0, r4
0038c388: add      sp, sp, #0xc
0038c38c: pop      {r4, r5, r6, r7, pc}
0038c390: rsbeq    r8, r0, r8, asr #18
0038c394: andeq    r2, r0, r0, ror sp

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

# _ZN14PhysicalObject3pinEv
0046eb20: str      lr, [sp, #-4]!
0046eb24: ldrb     r3, [r0, #0x27]
0046eb28: sub      sp, sp, #0x14
0046eb2c: cmp      r3, #0
0046eb30: bne      #0x46eb68
0046eb34: ldr      r2, [r0, #0x14]
0046eb38: mov      r3, #1
0046eb3c: strb     r3, [r0, #0x27]
0046eb40: ldr      r1, [r2, #0x1c]
0046eb44: mov      r0, r2
0046eb48: mov      r3, #0
0046eb4c: str      r1, [sp, #4]
0046eb50: ldr      r2, [r2, #0x20]
0046eb54: mov      r1, sp
0046eb58: str      r3, [sp, #0xc]
0046eb5c: str      r2, [sp, #8]
0046eb60: str      r3, [sp]
0046eb64: bl       #0x7e1b28
0046eb68: add      sp, sp, #0x14
0046eb6c: ldm      sp!, {pc}

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr

# _ZN6CharAI22LoadNInitScriptProcessEb
003cf3a4: push     {r4, lr}
003cf3a8: ldr      r3, [r0, #0x1c]
003cf3ac: sub      sp, sp, #8
003cf3b0: mov      r4, r0
003cf3b4: cmp      r3, #0
003cf3b8: beq      #0x3cf3c8
003cf3bc: mov      r0, #0
003cf3c0: add      sp, sp, #8
003cf3c4: pop      {r4, pc}
003cf3c8: str      r1, [sp, #4]
003cf3cc: bl       #0x3cf1f0
003cf3d0: ldr      r3, [r4, #0x1c]
003cf3d4: ldr      r1, [sp, #4]
003cf3d8: cmp      r3, #0
003cf3dc: beq      #0x3cf3bc
003cf3e0: mov      r0, r4
003cf3e4: bl       #0x3ce7c0
003cf3e8: mov      r0, #1
003cf3ec: b        #0x3cf3c0

# _ZN6CSIdle16IdleCommonUpdateEiP9CharacterP16CharStateMachine
003c0b78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c0b7c: ldrb     r3, [r1, #0x1b5]
003c0b80: ldr      r5, [pc, #0x2e0]
003c0b84: sub      sp, sp, #0x4c
003c0b88: cmp      r3, #0
003c0b8c: mov      r4, r1
003c0b90: add      r5, pc, r5
003c0b94: bne      #0x3c0bb8
003c0b98: ldr      r3, [r1]
003c0b9c: mov      r0, r1
003c0ba0: mov      lr, pc
003c0ba4: ldr      pc, [r3, #0x28]
003c0ba8: cmp      r0, #0
003c0bac: bne      #0x3c0bcc
003c0bb0: add      sp, sp, #0x4c
003c0bb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c0bb8: mov      r1, #0
003c0bbc: mov      r0, r4
003c0bc0: mov      r2, r1
003c0bc4: bl       #0x3a4d5c
003c0bc8: b        #0x3c0bb0
003c0bcc: bl       #0x7fd794
003c0bd0: ldrb     r6, [r0, #5]
003c0bd4: cmp      r6, #0
003c0bd8: bne      #0x3c0bb0
003c0bdc: ldr      r2, [pc, #0x288]
003c0be0: ldr      r1, [pc, #0x288]
003c0be4: ldr      r7, [r5, r2]
003c0be8: str      r2, [sp, #0x14]
003c0bec: ldr      r2, [pc, #0x280]
003c0bf0: add      r1, pc, r1
003c0bf4: ldr      r0, [r7, #0x2c]
003c0bf8: add      r2, pc, r2
003c0bfc: bl       #0x4c4bdc
003c0c00: ldr      r3, [r4, #0x55c]
003c0c04: mov      sb, r0
003c0c08: cmp      r0, r3
003c0c0c: bhi      #0x3c0bb0
003c0c10: ldr      r8, [r7, #0x40]
003c0c14: mov      r0, r8
003c0c18: bl       #0x36d7a8
003c0c1c: subs     sl, r0, #0
003c0c20: ble      #0x3c0bb0
003c0c24: ldr      r3, [pc, #0x24c]
003c0c28: add      r2, sp, #0x30
003c0c2c: str      r2, [sp, #0x24]
003c0c30: add      r3, pc, r3
003c0c34: str      r3, [sp, #0x28]
003c0c38: ldr      r3, [pc, #0x23c]
003c0c3c: mov      r7, r5
003c0c40: add      r3, pc, r3
003c0c44: str      r3, [sp, #0x2c]
003c0c48: add      r3, sp, #0x3c
003c0c4c: str      r3, [sp, #0x20]
003c0c50: b        #0x3c0c60
003c0c54: add      r6, r6, #1
003c0c58: cmp      r6, sl
003c0c5c: beq      #0x3c0bb0
003c0c60: mov      r0, r8
003c0c64: mov      r1, r6
003c0c68: mov      r2, #0
003c0c6c: bl       #0x36e744
003c0c70: ldr      r5, [r0, #0x660]
003c0c74: cmp      r5, #0
003c0c78: cmpne    r4, r5
003c0c7c: beq      #0x3c0c54
003c0c80: add      r0, r5, #0x4f0
003c0c84: add      r0, r0, #0xc
003c0c88: bl       #0x3c01ac
003c0c8c: cmp      r0, #3
003c0c90: bne      #0x3c0c54
003c0c94: ldr      r3, [r5, #0x55c]
003c0c98: cmp      sb, r3
003c0c9c: bhi      #0x3c0c54
003c0ca0: ldr      r2, [sp, #0x14]
003c0ca4: ldr      r1, [sp, #0x28]
003c0ca8: ldr      r3, [r7, r2]
003c0cac: ldr      r2, [sp, #0x2c]
003c0cb0: ldr      r0, [r3, #0x2c]
003c0cb4: bl       #0x4c4bdc
003c0cb8: bl       #0x30e964
003c0cbc: str      r0, [sp, #0xc]
003c0cc0: ldr      r1, [r5, #0x160]
003c0cc4: ldr      r0, [r4, #0x160]
003c0cc8: bl       #0x30e3ac
003c0ccc: str      r0, [sp, #0x10]
003c0cd0: ldr      r1, [r5, #0x164]
003c0cd4: ldr      r0, [r4, #0x164]
003c0cd8: bl       #0x30e3ac
003c0cdc: str      r0, [sp, #0x18]
003c0ce0: ldr      r1, [r5, #0x168]
003c0ce4: ldr      r0, [r4, #0x168]
003c0ce8: bl       #0x30e3ac
003c0cec: str      r0, [sp, #0x1c]
003c0cf0: ldr      r0, [sp, #0x10]
003c0cf4: mov      r1, r0
003c0cf8: bl       #0x30ed6c
003c0cfc: mov      fp, r0
003c0d00: ldr      r0, [sp, #0x18]
003c0d04: mov      r1, r0
003c0d08: bl       #0x30ed6c
003c0d0c: mov      r1, r0
003c0d10: mov      r0, fp
003c0d14: bl       #0x30eba4
003c0d18: mov      fp, r0
003c0d1c: ldr      r0, [sp, #0x1c]
003c0d20: mov      r1, r0
003c0d24: bl       #0x30ed6c
003c0d28: mov      r1, r0
003c0d2c: mov      r0, fp
003c0d30: bl       #0x30eba4
003c0d34: mov      fp, r0
003c0d38: ldr      r0, [sp, #0xc]
003c0d3c: mov      r1, r0
003c0d40: bl       #0x30ed6c
003c0d44: mov      r1, fp
003c0d48: bl       #0x30e2f8
003c0d4c: cmp      r0, #0
003c0d50: beq      #0x3c0c54
003c0d54: mov      r0, fp
003c0d58: mov      r1, #0
003c0d5c: bl       #0x30e2f8
003c0d60: cmp      r0, #0
003c0d64: beq      #0x3c0c54
003c0d68: mov      r0, fp
003c0d6c: bl       #0x30e124
003c0d70: mov      fp, r0
003c0d74: mov      r1, fp
003c0d78: ldr      r0, [sp, #0xc]
003c0d7c: bl       #0x30e3ac
003c0d80: mov      r1, #0x3f400000
003c0d84: bl       #0x30ed6c
003c0d88: mov      r1, fp
003c0d8c: bl       #0x30ec94
003c0d90: ldr      r1, [sp, #0x10]
003c0d94: mov      fp, r0
003c0d98: bl       #0x30ed6c
003c0d9c: ldr      r1, [sp, #0x18]
003c0da0: str      r0, [sp, #0xc]
003c0da4: mov      r0, fp
003c0da8: bl       #0x30ed6c
003c0dac: ldr      r1, [sp, #0x1c]
003c0db0: str      r0, [sp, #0x10]
003c0db4: mov      r0, fp
003c0db8: bl       #0x30ed6c
003c0dbc: ldr      r1, [r4, #0x164]
003c0dc0: mov      fp, r0
003c0dc4: ldr      r0, [sp, #0x10]
003c0dc8: bl       #0x30eba4
003c0dcc: ldr      r1, [r4, #0x168]
003c0dd0: mov      r2, r0
003c0dd4: mov      r0, fp
003c0dd8: str      r2, [sp, #4]
003c0ddc: bl       #0x30eba4
003c0de0: ldr      r1, [r4, #0x160]
003c0de4: mov      ip, r0
003c0de8: ldr      r0, [sp, #0xc]
003c0dec: str      ip, [sp, #8]
003c0df0: bl       #0x30eba4
003c0df4: ldr      r3, [r4, #0x378]
003c0df8: ldmib    sp, {r2, ip}
003c0dfc: str      r0, [sp, #0x3c]
003c0e00: ldr      r1, [sp, #0x20]
003c0e04: mov      r0, r3
003c0e08: str      r2, [sp, #0x40]
003c0e0c: str      ip, [sp, #0x44]
003c0e10: bl       #0x4054e4
003c0e14: ldr      r0, [r5, #0x164]
003c0e18: ldr      r1, [sp, #0x10]
003c0e1c: bl       #0x30e3ac
003c0e20: mov      r1, fp
003c0e24: mov      r3, r0
003c0e28: ldr      r0, [r5, #0x168]
003c0e2c: str      r3, [sp, #8]
003c0e30: bl       #0x30e3ac
003c0e34: ldr      r1, [sp, #0xc]
003c0e38: mov      fp, r0
003c0e3c: ldr      r0, [r5, #0x160]
003c0e40: bl       #0x30e3ac
003c0e44: ldr      r2, [r5, #0x378]
003c0e48: ldr      r3, [sp, #8]
003c0e4c: str      r0, [sp, #0x30]
003c0e50: ldr      r1, [sp, #0x24]
003c0e54: mov      r0, r2
003c0e58: str      r3, [sp, #0x34]
003c0e5c: str      fp, [sp, #0x38]
003c0e60: bl       #0x4054e4
003c0e64: b        #0x3c0c54
003c0e68: subseq   r3, sp, r0, lsl #30
003c0e6c: strdeq   r3, r4, [r0], -r4
003c0e70: subseq   r0, r0, r0, ror #22
003c0e74: subseq   r3, r0, r0, lsl #31
003c0e78: subseq   r0, r0, r0, lsr #22
003c0e7c: subseq   r3, r0, r8, asr pc

# _ZN9Character18InitPhysicalObjectEv
003b4088: push     {r4, r5, r6, r7, r8, sl, lr}
003b408c: sub      sp, sp, #0x24
003b4090: mov      r5, r0
003b4094: bl       #0x3a30dc
003b4098: ldr      r4, [pc, #0x320]
003b409c: subs     r6, r0, #0
003b40a0: add      r4, pc, r4
003b40a4: bne      #0x3b41a8
003b40a8: ldrb     r7, [r5, #0x84]
003b40ac: cmp      r7, #0
003b40b0: bne      #0x3b412c
003b40b4: mov      r0, r5
003b40b8: bl       #0x3a310c
003b40bc: subs     r6, r0, #0
003b40c0: beq      #0x3b41f0
003b40c4: ldr      r3, [pc, #0x2f8]
003b40c8: mov      r1, r7
003b40cc: mov      r0, #0x28
003b40d0: ldr      r3, [r4, r3]
003b40d4: ldr      r8, [r3, #0x44]
003b40d8: bl       #0x310570
003b40dc: mov      ip, #0x400
003b40e0: mvn      r3, #3
003b40e4: str      ip, [sp]
003b40e8: mov      r1, r8
003b40ec: movw     ip, #0xd1f
003b40f0: mov      r2, r5
003b40f4: mov      r6, r0
003b40f8: str      ip, [sp, #4]
003b40fc: str      r7, [sp, #8]
003b4100: bl       #0x3b4010
003b4104: ldr      r3, [pc, #0x2bc]
003b4108: ldr      r3, [r4, r3]
003b410c: mov      r0, r5
003b4110: mov      r1, r6
003b4114: add      r3, r3, #8
003b4118: mov      r2, #1
003b411c: str      r3, [r6]
003b4120: add      sp, sp, #0x24
003b4124: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4128: b        #0x394bf8
003b412c: ldr      r3, [pc, #0x290]
003b4130: mov      r1, r6
003b4134: mov      r0, #0x28
003b4138: ldr      r3, [r4, r3]
003b413c: mov      r7, #1
003b4140: ldr      sl, [r3, #0x44]
003b4144: bl       #0x310570
003b4148: mov      ip, #2
003b414c: mov      r1, sl
003b4150: mov      r2, r5
003b4154: mov      r3, r7
003b4158: str      ip, [sp, #0x10]
003b415c: movw     ip, #0xffff
003b4160: mov      r8, r0
003b4164: str      r6, [sp, #0xc]
003b4168: str      ip, [sp, #0x14]
003b416c: str      r6, [sp]
003b4170: str      r6, [sp, #4]
003b4174: str      r6, [sp, #8]
003b4178: str      r7, [sp, #0x18]
003b417c: bl       #0x46f2f0
003b4180: ldr      r3, [pc, #0x244]
003b4184: mov      r0, r5
003b4188: mov      r1, r8
003b418c: ldr      r3, [r4, r3]
003b4190: mov      r2, r7
003b4194: add      r3, r3, #8
003b4198: str      r3, [r8]
003b419c: add      sp, sp, #0x24
003b41a0: pop      {r4, r5, r6, r7, r8, sl, lr}
003b41a4: b        #0x394bf8
003b41a8: ldr      r3, [pc, #0x214]
003b41ac: mov      r1, #0
003b41b0: mov      r0, #0x28
003b41b4: ldr      r3, [r4, r3]
003b41b8: ldr      r7, [r3, #0x44]
003b41bc: bl       #0x310570
003b41c0: mov      ip, #0
003b41c4: mov      r3, ip
003b41c8: mov      lr, #0x100
003b41cc: mov      r1, r7
003b41d0: mov      r2, r5
003b41d4: mov      r6, r0
003b41d8: str      lr, [sp]
003b41dc: str      ip, [sp, #4]
003b41e0: str      ip, [sp, #8]
003b41e4: bl       #0x3b4010
003b41e8: ldr      r3, [pc, #0x1e0]
003b41ec: b        #0x3b4108
003b41f0: ldr      r3, [r5]
003b41f4: mov      r0, r5
003b41f8: mov      lr, pc
003b41fc: ldr      pc, [r3, #0x28]
003b4200: cmp      r0, #0
003b4204: bne      #0x3b4264
003b4208: mov      r0, r5
003b420c: bl       #0x3a307c
003b4210: cmp      r0, #0
003b4214: beq      #0x3b42d0
003b4218: ldr      r3, [pc, #0x1a4]
003b421c: mov      r1, #0
003b4220: mov      r0, #0x28
003b4224: ldr      r3, [r4, r3]
003b4228: ldr      r7, [r3, #0x44]
003b422c: bl       #0x310570
003b4230: mov      ip, #8
003b4234: str      ip, [sp]
003b4238: movw     ip, #0xd3b
003b423c: mvn      r3, #1
003b4240: str      ip, [sp, #4]
003b4244: mov      r1, r7
003b4248: mov      ip, #0
003b424c: mov      r2, r5
003b4250: mov      r6, r0
003b4254: str      ip, [sp, #8]
003b4258: bl       #0x3b4010
003b425c: ldr      r3, [pc, #0x170]
003b4260: b        #0x3b4108
003b4264: ldr      r3, [pc, #0x158]
003b4268: mov      r1, r6
003b426c: mov      r0, #0x28
003b4270: ldr      r3, [r4, r3]
003b4274: mov      r7, #1
003b4278: ldr      r8, [r3, #0x44]
003b427c: bl       #0x310570
003b4280: mov      ip, #4
003b4284: mov      r1, r8
003b4288: mov      r2, r5
003b428c: str      ip, [sp]
003b4290: mvn      r3, #0
003b4294: movw     ip, #0xd7f
003b4298: mov      r6, r0
003b429c: str      ip, [sp, #4]
003b42a0: str      r7, [sp, #8]
003b42a4: bl       #0x3b4010
003b42a8: ldr      r3, [pc, #0x128]
003b42ac: mov      r0, r5
003b42b0: mov      r1, r6
003b42b4: ldr      r3, [r4, r3]
003b42b8: mov      r2, r7
003b42bc: add      r3, r3, #8
003b42c0: str      r3, [r6]
003b42c4: add      sp, sp, #0x24
003b42c8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b42cc: b        #0x394bf8
003b42d0: mov      r0, r5
003b42d4: bl       #0x3a30ac
003b42d8: subs     r6, r0, #0
003b42dc: bne      #0x3b4218
003b42e0: mov      r0, r5
003b42e4: bl       #0x3a3094
003b42e8: subs     r7, r0, #0
003b42ec: beq      #0x3b4354
003b42f0: ldr      r3, [pc, #0xcc]
003b42f4: mov      r1, r6
003b42f8: mov      r0, #0x28
003b42fc: ldr      r3, [r4, r3]
003b4300: ldr      r8, [r3, #0x44]
003b4304: bl       #0x310570
003b4308: mov      r1, r8
003b430c: mov      r3, r6
003b4310: mov      r2, r5
003b4314: mov      ip, #0x100
003b4318: mov      r7, r0
003b431c: str      ip, [sp]
003b4320: str      r6, [sp, #4]
003b4324: str      r6, [sp, #8]
003b4328: bl       #0x3b4010
003b432c: ldr      r3, [pc, #0x9c]
003b4330: mov      r0, r5
003b4334: mov      r1, r7
003b4338: ldr      r3, [r4, r3]
003b433c: mov      r2, #1
003b4340: add      r3, r3, #8
003b4344: str      r3, [r7]
003b4348: add      sp, sp, #0x24
003b434c: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4350: b        #0x394bf8
003b4354: mov      r0, r5
003b4358: bl       #0x3a3064
003b435c: subs     r1, r0, #0
003b4360: beq      #0x3b43ac
003b4364: ldr      r3, [pc, #0x58]
003b4368: mov      r1, r7
003b436c: mov      r0, #0x28
003b4370: ldr      r3, [r4, r3]
003b4374: ldr      r8, [r3, #0x44]
003b4378: bl       #0x310570
003b437c: mov      ip, #0x10
003b4380: mov      r3, #2
003b4384: str      ip, [sp]
003b4388: mov      r1, r8
003b438c: movw     ip, #0xd3f
003b4390: mov      r2, r5
003b4394: mov      r6, r0
003b4398: str      ip, [sp, #4]
003b439c: str      r7, [sp, #8]
003b43a0: bl       #0x3b4010
003b43a4: ldr      r3, [pc, #0x30]
003b43a8: b        #0x3b4108
003b43ac: mov      r0, r5
003b43b0: mov      r2, r1
003b43b4: add      sp, sp, #0x24
003b43b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b43bc: b        #0x394bf8
003b43c0: ldrsheq  r0, [lr], #-0x90
003b43c4: strdeq   r3, r4, [r0], -r4
003b43c8: andeq    r3, r0, ip, asr pc
003b43cc: andeq    r2, r0, r8, lsl r4
003b43d0: andeq    r0, r0, ip, asr #26
003b43d4: andeq    r3, r0, r4, lsr #4
003b43d8: andeq    r1, r0, ip, asr sp
003b43dc: andeq    r1, r0, r0, lsr #5

# _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003aa1b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add      ip, r0, #0x374
003aa1bc: sub      sp, sp, #0x3c
003aa1c0: mov      r4, r0
003aa1c4: str      ip, [sp, #0xc]
003aa1c8: bl       #0x38c398
003aa1cc: ldr      ip, [sp, #0xc]
003aa1d0: add      r5, r4, #0x4f0
003aa1d4: add      r5, r5, #0xc
003aa1d8: mov      r0, ip
003aa1dc: bl       #0x404db8
003aa1e0: add      r0, r4, #0x3b4
003aa1e4: str      r0, [sp, #0x20]
003aa1e8: add      r0, r4, #0x37c
003aa1ec: bl       #0x3ff330
003aa1f0: add      r2, r4, #0x490
003aa1f4: add      r1, r4, #0x3c8
003aa1f8: add      r2, r2, #0xc
003aa1fc: ldr      r0, [sp, #0x20]
003aa200: str      r1, [sp, #0x1c]
003aa204: str      r2, [sp, #0x14]
003aa208: bl       #0x3dbb0c
003aa20c: ldr      r0, [sp, #0x1c]
003aa210: bl       #0x3cebf0
003aa214: ldr      r0, [sp, #0x14]
003aa218: bl       #0x3c8ff4
003aa21c: add      r3, r4, #0x560
003aa220: mov      r0, r5
003aa224: str      r3, [sp, #0x18]
003aa228: ldr      sb, [pc, #0x50c]
003aa22c: bl       #0x3c1b58
003aa230: ldr      r0, [sp, #0x18]
003aa234: bl       #0x3df084
003aa238: ldr      lr, [pc, #0x500]
003aa23c: add      sb, pc, sb
003aa240: mov      r8, #0
003aa244: ldr      lr, [sb, lr]
003aa248: mov      fp, #1
003aa24c: mvn      r6, #0
003aa250: add      sl, lr, #0x324
003aa254: str      sl, [sp, #0x34]
003aa258: add      sl, lr, #0x180
003aa25c: str      sl, [sp, #0x10]
003aa260: add      sl, lr, #0x1f4
003aa264: str      sl, [sp, #0x24]
003aa268: add      sl, lr, #0x220
003aa26c: str      sl, [sp, #0x28]
003aa270: add      sl, lr, #0x230
003aa274: str      sl, [sp, #0x2c]
003aa278: add      r0, lr, #8
003aa27c: add      r1, lr, #0x15c
003aa280: add      r2, lr, #0x168
003aa284: add      sl, lr, #0x304
003aa288: str      sl, [sp, #0x30]
003aa28c: stm      r4, {r0, r1}
003aa290: str      r2, [r4, #0x24]
003aa294: ldr      r0, [sp, #0x10]
003aa298: add      lr, lr, #0x314
003aa29c: add      r7, r4, #0x1380
003aa2a0: str      r0, [r4, #0x374]
003aa2a4: ldr      r1, [sp, #0x24]
003aa2a8: add      r3, r7, #0x18
003aa2ac: movw     sl, #0x13a8
003aa2b0: str      r1, [r4, #0x37c]
003aa2b4: ldr      r2, [sp, #0x28]
003aa2b8: add      r7, r7, #0x30
003aa2bc: str      r2, [r4, #0x3b4]
003aa2c0: ldr      r0, [sp, #0x2c]
003aa2c4: str      r0, [r4, #0x3c8]
003aa2c8: ldr      r1, [sp, #0x30]
003aa2cc: str      lr, [r4, #0x4fc]
003aa2d0: mov      r0, r3
003aa2d4: str      r1, [r4, #0x49c]
003aa2d8: ldr      r2, [sp, #0x34]
003aa2dc: mov      r1, #0x10
003aa2e0: str      r2, [r4, #0x560]
003aa2e4: movw     r2, #0x1394
003aa2e8: strb     r8, [r4, r2]
003aa2ec: movw     r2, #0x1395
003aa2f0: strb     r8, [r4, r2]
003aa2f4: movw     r2, #0x1396
003aa2f8: strb     fp, [r4, r2]
003aa2fc: movw     r2, #0x1397
003aa300: strb     r6, [r4, r2]
003aa304: movw     r2, #0x13ac
003aa308: str      r3, [r4, r2]
003aa30c: str      r3, [r4, sl]
003aa310: bl       #0x31167c
003aa314: ldr      r3, [r4, sl]
003aa318: mov      sl, #0x13c0
003aa31c: mov      r0, r7
003aa320: strb     r8, [r3]
003aa324: movw     r3, #0x13c4
003aa328: str      r7, [r4, r3]
003aa32c: mov      r1, #0x10
003aa330: str      r7, [r4, sl]
003aa334: bl       #0x31167c
003aa338: ldr      r2, [r4, sl]
003aa33c: add      r7, r4, sl
003aa340: add      r3, r7, #0xc
003aa344: strb     r8, [r2]
003aa348: movw     r2, #0x13c8
003aa34c: strh     r6, [r4, r2]
003aa350: movw     r2, #0x13ca
003aa354: strh     r6, [r4, r2]
003aa358: movw     sl, #0x13dc
003aa35c: movw     r2, #0x13e0
003aa360: str      r3, [r4, r2]
003aa364: mov      r0, r3
003aa368: str      r3, [r4, sl]
003aa36c: mov      r1, #0x10
003aa370: bl       #0x31167c
003aa374: ldr      r3, [r4, sl]
003aa378: add      r7, r7, #0x28
003aa37c: movw     sl, #0x13f8
003aa380: strb     r8, [r3]
003aa384: movw     r3, #0x13e4
003aa388: strb     fp, [r4, r3]
003aa38c: movw     r3, #0x13fc
003aa390: str      r7, [r4, r3]
003aa394: mov      r0, r7
003aa398: str      r7, [r4, sl]
003aa39c: mov      r1, #0x10
003aa3a0: bl       #0x31167c
003aa3a4: ldr      r3, [r4, sl]
003aa3a8: add      r7, r4, #0x1400
003aa3ac: movw     sl, #0x1410
003aa3b0: strb     r8, [r3]
003aa3b4: movw     r3, #0x1414
003aa3b8: str      r7, [r4, r3]
003aa3bc: mov      r0, r7
003aa3c0: str      r7, [r4, sl]
003aa3c4: mov      r1, #0x10
003aa3c8: bl       #0x31167c
003aa3cc: ldr      r3, [r4, sl]
003aa3d0: add      r7, r7, #0x18
003aa3d4: movw     sl, #0x1428
003aa3d8: strb     r8, [r3]
003aa3dc: movw     r3, #0x142c
003aa3e0: str      r7, [r4, r3]
003aa3e4: mov      r0, r7
003aa3e8: str      r7, [r4, sl]
003aa3ec: mov      r1, #0x10
003aa3f0: bl       #0x31167c
003aa3f4: ldr      r2, [r4, sl]
003aa3f8: mov      r3, #0
003aa3fc: mov      r1, #0xbf000000
003aa400: strb     r8, [r2]
003aa404: movw     r2, #0x14a8
003aa408: strb     r6, [r4, r2]
003aa40c: movw     r2, #0x1430
003aa410: strb     fp, [r4, r2]
003aa414: movw     r2, #0x1434
003aa418: str      r8, [r4, r2]
003aa41c: movw     r2, #0x1438
003aa420: str      r8, [r4, r2]
003aa424: movw     r2, #0x1448
003aa428: strb     fp, [r4, r2]
003aa42c: movw     r2, #0x1449
003aa430: strb     r8, [r4, r2]
003aa434: movw     r2, #0x144c
003aa438: str      r8, [r4, r2]
003aa43c: movw     r2, #0x1450
003aa440: str      r3, [r4, r2]
003aa444: movw     r2, #0x1454
003aa448: str      r3, [r4, r2]
003aa44c: movw     r2, #0x1458
003aa450: str      r3, [r4, r2]
003aa454: movw     r2, #0x145c
003aa458: str      r3, [r4, r2]
003aa45c: movw     r2, #0x1460
003aa460: str      r3, [r4, r2]
003aa464: movw     r2, #0x1464
003aa468: str      r3, [r4, r2]
003aa46c: movw     r2, #0x1468
003aa470: str      r3, [r4, r2]
003aa474: movw     r2, #0x146c
003aa478: str      r3, [r4, r2]
003aa47c: movw     r2, #0x1470
003aa480: str      r3, [r4, r2]
003aa484: movw     r2, #0x1474
003aa488: str      r3, [r4, r2]
003aa48c: movw     r2, #0x1478
003aa490: str      r3, [r4, r2]
003aa494: movw     r2, #0x147c
003aa498: str      r3, [r4, r2]
003aa49c: mov      r2, #0x1480
003aa4a0: strb     r8, [r4, r2]
003aa4a4: movw     r2, #0x1481
003aa4a8: strb     r8, [r4, r2]
003aa4ac: movw     r2, #0x1484
003aa4b0: str      r8, [r4, r2]
003aa4b4: movw     r2, #0x1488
003aa4b8: str      r8, [r4, r2]
003aa4bc: movw     r2, #0x148c
003aa4c0: str      r8, [r4, r2]
003aa4c4: movw     r2, #0x1490
003aa4c8: str      r8, [r4, r2]
003aa4cc: movw     r2, #0x1494
003aa4d0: str      r8, [r4, r2]
003aa4d4: movw     r2, #0x1498
003aa4d8: str      r6, [r4, r2]
003aa4dc: movw     r2, #0x149c
003aa4e0: str      r8, [r4, r2]
003aa4e4: movw     r2, #0x14a0
003aa4e8: str      r8, [r4, r2]
003aa4ec: movw     r2, #0x14a4
003aa4f0: str      r8, [r4, r2]
003aa4f4: movw     r2, #0x14aa
003aa4f8: strh     r8, [r4, r2]
003aa4fc: movw     r2, #0x14ac
003aa500: strb     r8, [r4, r2]
003aa504: movw     r2, #0x14d8
003aa508: str      r3, [r4, r2]
003aa50c: add      r1, r1, #0x800000
003aa510: movw     r2, #0x14fc
003aa514: str      r1, [r4, r2]
003aa518: movw     r2, #0x1504
003aa51c: str      r6, [r4, r2]
003aa520: movw     r2, #0x14ad
003aa524: strb     r8, [r4, r2]
003aa528: movw     r2, #0x14b0
003aa52c: str      r3, [r4, r2]
003aa530: movw     r2, #0x14b4
003aa534: str      r3, [r4, r2]
003aa538: movw     r2, #0x14b8
003aa53c: str      r3, [r4, r2]
003aa540: movw     r2, #0x14bc
003aa544: str      r3, [r4, r2]
003aa548: mov      r2, #0x14c0
003aa54c: str      r3, [r4, r2]
003aa550: movw     r2, #0x14c4
003aa554: str      r3, [r4, r2]
003aa558: movw     r3, #0x14c8
003aa55c: strb     r8, [r4, r3]
003aa560: movw     r3, #0x14ca
003aa564: strh     r6, [r4, r3]
003aa568: movw     r3, #0x14cc
003aa56c: str      r8, [r4, r3]
003aa570: movw     r3, #0x14d0
003aa574: strh     r8, [r4, r3]
003aa578: movw     r3, #0x14d4
003aa57c: str      r8, [r4, r3]
003aa580: movw     r3, #0x14dc
003aa584: strb     r8, [r4, r3]
003aa588: movw     r3, #0x14e4
003aa58c: strb     r8, [r4, r3]
003aa590: movw     r3, #0x14e5
003aa594: strb     r8, [r4, r3]
003aa598: movw     r3, #0x14e8
003aa59c: str      r8, [r4, r3]
003aa5a0: movw     r3, #0x14ec
003aa5a4: str      r8, [r4, r3]
003aa5a8: add      r7, r4, #0x1500
003aa5ac: movw     r3, #0x14f0
003aa5b0: add      r0, r4, #0x1a40
003aa5b4: strb     r8, [r4, r3]
003aa5b8: add      r0, r0, #8
003aa5bc: mov      r3, #0x1500
003aa5c0: add      r7, r7, #8
003aa5c4: str      r6, [r4, r3]
003aa5c8: str      r0, [sp, #0x10]
003aa5cc: mov      r0, r7
003aa5d0: bl       #0x3a6a24
003aa5d4: ldr      r0, [sp, #0x10]
003aa5d8: bl       #0x3a6a24
003aa5dc: add      r0, r4, #0x304
003aa5e0: mov      r1, r4
003aa5e4: strb     fp, [r4, #0x28]
003aa5e8: bl       #0x4a191c
003aa5ec: strb     fp, [r4, #0x1c4]
003aa5f0: strb     fp, [r4, #0x85]
003aa5f4: mov      r0, #0x10
003aa5f8: mov      r1, r8
003aa5fc: bl       #0x310570
003aa600: ldr      r3, [pc, #0x13c]
003aa604: ldr      ip, [sp, #0xc]
003aa608: mov      r6, r0
003aa60c: ldr      r3, [sb, r3]
003aa610: cmp      ip, r8
003aa614: strb     r8, [r6, #0xa]
003aa618: add      r3, r3, #8
003aa61c: str      r8, [r0, #0xc]
003aa620: stm      r0, {r3, ip}
003aa624: strb     r8, [r6, #8]
003aa628: strb     r8, [r6, #9]
003aa62c: beq      #0x3aa6e0
003aa630: mov      r0, ip
003aa634: mov      r1, r6
003aa638: bl       #0x404e10
003aa63c: ldr      r3, [r4, #0x378]
003aa640: ldr      r0, [sp, #0x20]
003aa644: mov      r1, r4
003aa648: str      r4, [r3, #0xc]
003aa64c: bl       #0x3db480
003aa650: ldr      r0, [sp, #0x1c]
003aa654: mov      r1, r4
003aa658: bl       #0x3cb7c0
003aa65c: ldr      r0, [sp, #0x14]
003aa660: mov      r1, r4
003aa664: bl       #0x3c9890
003aa668: mov      r0, r5
003aa66c: mov      r1, r4
003aa670: bl       #0x3c1600
003aa674: ldr      r0, [sp, #0x18]
003aa678: mov      r1, r4
003aa67c: bl       #0x3dec0c
003aa680: mov      r6, #0
003aa684: str      r4, [r4, #0x380]
003aa688: mov      r1, r6
003aa68c: mov      r0, r5
003aa690: add      r6, r6, #1
003aa694: bl       #0x3c7318
003aa698: cmp      r6, #0x14
003aa69c: bne      #0x3aa688
003aa6a0: mov      r1, #0
003aa6a4: movw     r2, #0x14e0
003aa6a8: str      r1, [r4, r2]
003aa6ac: mvn      r3, #0
003aa6b0: movw     r2, #0x14f4
003aa6b4: str      r3, [r4, r2]
003aa6b8: str      r7, [r4, #0x100]
003aa6bc: ldr      sl, [sp, #0x10]
003aa6c0: movw     r2, #0x14f8
003aa6c4: mov      r0, r4
003aa6c8: str      sl, [r4, #0x104]
003aa6cc: str      r3, [r4, r2]
003aa6d0: mov      r3, #1
003aa6d4: strb     r3, [r4, #0xf8]
003aa6d8: add      sp, sp, #0x3c
003aa6dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr      r3, [pc, #0x60]
003aa6e4: ldr      r3, [sb, r3]
003aa6e8: ldr      r3, [r3]
003aa6ec: cmp      r3, #2
003aa6f0: streq    ip, [r4, #0x374]
003aa6f4: beq      #0x3aa630
003aa6f8: cmp      r3, #1
003aa6fc: bne      #0x3aa630
003aa700: ldr      r0, [pc, #0x44]
003aa704: ldr      r1, [pc, #0x44]
003aa708: ldr      r2, [pc, #0x44]
003aa70c: ldr      r0, [sb, r0]
003aa710: ldr      r3, [pc, #0x40]
003aa714: mov      lr, #0x44
003aa718: add      r1, pc, r1
003aa71c: add      r0, r0, #0xa8
003aa720: add      r2, pc, r2
003aa724: add      r3, pc, r3
003aa728: str      ip, [sp, #0xc]
003aa72c: str      lr, [sp]
003aa730: bl       #0x30e004
003aa734: ldr      ip, [sp, #0xc]
003aa738: b        #0x3aa630
003aa73c: subseq   sl, lr, r4, asr r8
003aa740: andeq    r2, r0, r8, lsl #28
003aa744: andeq    r2, r0, r4, lsr #21
003aa748: andeq    r3, r0, r0, asr #19
003aa74c: andeq    r1, r0, r0, asr #19
003aa750: subseq   r3, r1, r0, asr #25
003aa754: subseq   r8, r1, r0, lsr #27
003aa758: subseq   r8, r1, ip, lsr #27

# _ZN10GameObject19SetHeadingDirectionERK7Point3DIfEb
00393be8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00393bec: ldr      r3, [r1]
00393bf0: mov      r4, r0
00393bf4: mov      r6, #0
00393bf8: str      r3, [r0, #0x1b8]
00393bfc: ldr      r7, [r1, #4]
00393c00: mov      r0, r3
00393c04: str      r6, [r4, #0x1c0]
00393c08: str      r7, [r4, #0x1bc]
00393c0c: mov      r5, r1
00393c10: mov      r1, r3
00393c14: mov      sl, r2
00393c18: bl       #0x30ed6c
00393c1c: mov      r1, r7
00393c20: mov      r8, r0
00393c24: mov      r0, r7
00393c28: bl       #0x30ed6c
00393c2c: mov      r1, r0
00393c30: mov      r0, r8
00393c34: bl       #0x30eba4
00393c38: mov      r1, r6
00393c3c: bl       #0x30eba4
00393c40: movw     r1, #0xb717
00393c44: movt     r1, #0x38d1
00393c48: mov      r7, r0
00393c4c: bl       #0x30e2f8
00393c50: cmp      r0, #0
00393c54: mov      r6, #0
00393c58: movne    r6, #1
00393c5c: uxtb     r6, r6
00393c60: strb     r6, [r4, #0x1b5]
00393c64: mov      r0, r7
00393c68: mov      r1, #0x3f800000
00393c6c: bl       #0x30e2f8
00393c70: cmp      r0, #0
00393c74: bne      #0x393c9c
00393c78: cmp      r6, #0
00393c7c: beq      #0x393c88
00393c80: cmp      sl, #0
00393c84: bne      #0x393c8c
00393c88: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393c8c: mov      r0, r4
00393c90: mov      r1, r5
00393c94: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393c98: b        #0x393b1c
00393c9c: mov      r0, r7
00393ca0: bl       #0x30e124
00393ca4: mov      r1, r0
00393ca8: mov      r0, #0x3f800000
00393cac: bl       #0x30ec94
00393cb0: mov      r6, r0
00393cb4: mov      r1, r0
00393cb8: ldr      r0, [r4, #0x1b8]
00393cbc: bl       #0x30ed6c
00393cc0: mov      r1, r6
00393cc4: str      r0, [r4, #0x1b8]
00393cc8: ldr      r0, [r4, #0x1bc]
00393ccc: bl       #0x30ed6c
00393cd0: mov      r1, r6
00393cd4: str      r0, [r4, #0x1bc]
00393cd8: ldr      r0, [r4, #0x1c0]
00393cdc: bl       #0x30ed6c
00393ce0: ldrb     r6, [r4, #0x1b5]
00393ce4: str      r0, [r4, #0x1c0]
00393ce8: b        #0x393c78

# _ZN9CharacterC2EN10ObjectBase6GO_IDSE
003a9340: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a9344: add      ip, r0, #0x374
003a9348: sub      sp, sp, #0x3c
003a934c: mov      r4, r0
003a9350: str      ip, [sp, #0xc]
003a9354: bl       #0x38c398
003a9358: ldr      ip, [sp, #0xc]
003a935c: add      r5, r4, #0x4f0
003a9360: add      r5, r5, #0xc
003a9364: mov      r0, ip
003a9368: bl       #0x404db8
003a936c: add      r0, r4, #0x3b4
003a9370: str      r0, [sp, #0x20]
003a9374: add      r0, r4, #0x37c
003a9378: bl       #0x3ff330
003a937c: add      r2, r4, #0x490
003a9380: add      r1, r4, #0x3c8
003a9384: add      r2, r2, #0xc
003a9388: ldr      r0, [sp, #0x20]
003a938c: str      r1, [sp, #0x1c]
003a9390: str      r2, [sp, #0x14]
003a9394: bl       #0x3dbb0c
003a9398: ldr      r0, [sp, #0x1c]
003a939c: bl       #0x3cebf0
003a93a0: ldr      r0, [sp, #0x14]
003a93a4: bl       #0x3c8ff4
003a93a8: add      r3, r4, #0x560
003a93ac: mov      r0, r5
003a93b0: str      r3, [sp, #0x18]
003a93b4: ldr      sb, [pc, #0x50c]
003a93b8: bl       #0x3c1b58
003a93bc: ldr      r0, [sp, #0x18]
003a93c0: bl       #0x3df084
003a93c4: ldr      lr, [pc, #0x500]
003a93c8: add      sb, pc, sb
003a93cc: mov      r8, #0
003a93d0: ldr      lr, [sb, lr]
003a93d4: mov      fp, #1
003a93d8: mvn      r6, #0
003a93dc: add      sl, lr, #0x324
003a93e0: str      sl, [sp, #0x34]
003a93e4: add      sl, lr, #0x180
003a93e8: str      sl, [sp, #0x10]
003a93ec: add      sl, lr, #0x1f4
003a93f0: str      sl, [sp, #0x24]
003a93f4: add      sl, lr, #0x220
003a93f8: str      sl, [sp, #0x28]
003a93fc: add      sl, lr, #0x230
003a9400: str      sl, [sp, #0x2c]
003a9404: add      r0, lr, #8
003a9408: add      r1, lr, #0x15c
003a940c: add      r2, lr, #0x168
003a9410: add      sl, lr, #0x304
003a9414: str      sl, [sp, #0x30]
003a9418: stm      r4, {r0, r1}
003a941c: str      r2, [r4, #0x24]
003a9420: ldr      r0, [sp, #0x10]
003a9424: add      lr, lr, #0x314
003a9428: add      r7, r4, #0x1380
003a942c: str      r0, [r4, #0x374]
003a9430: ldr      r1, [sp, #0x24]
003a9434: add      r3, r7, #0x18
003a9438: movw     sl, #0x13a8
003a943c: str      r1, [r4, #0x37c]
003a9440: ldr      r2, [sp, #0x28]
003a9444: add      r7, r7, #0x30
003a9448: str      r2, [r4, #0x3b4]
003a944c: ldr      r0, [sp, #0x2c]
003a9450: str      r0, [r4, #0x3c8]
003a9454: ldr      r1, [sp, #0x30]
003a9458: str      lr, [r4, #0x4fc]
003a945c: mov      r0, r3
003a9460: str      r1, [r4, #0x49c]
003a9464: ldr      r2, [sp, #0x34]
003a9468: mov      r1, #0x10
003a946c: str      r2, [r4, #0x560]
003a9470: movw     r2, #0x1394
003a9474: strb     r8, [r4, r2]
003a9478: movw     r2, #0x1395
003a947c: strb     r8, [r4, r2]
003a9480: movw     r2, #0x1396
003a9484: strb     fp, [r4, r2]
003a9488: movw     r2, #0x1397
003a948c: strb     r6, [r4, r2]
003a9490: movw     r2, #0x13ac
003a9494: str      r3, [r4, r2]
003a9498: str      r3, [r4, sl]
003a949c: bl       #0x31167c
003a94a0: ldr      r3, [r4, sl]
003a94a4: mov      sl, #0x13c0
003a94a8: mov      r0, r7
003a94ac: strb     r8, [r3]
003a94b0: movw     r3, #0x13c4
003a94b4: str      r7, [r4, r3]
003a94b8: mov      r1, #0x10
003a94bc: str      r7, [r4, sl]
003a94c0: bl       #0x31167c
003a94c4: ldr      r2, [r4, sl]
003a94c8: add      r7, r4, sl
003a94cc: add      r3, r7, #0xc
003a94d0: strb     r8, [r2]
003a94d4: movw     r2, #0x13c8
003a94d8: strh     r6, [r4, r2]
003a94dc: movw     r2, #0x13ca
003a94e0: strh     r6, [r4, r2]
003a94e4: movw     sl, #0x13dc
003a94e8: movw     r2, #0x13e0
003a94ec: str      r3, [r4, r2]
003a94f0: mov      r0, r3
003a94f4: str      r3, [r4, sl]
003a94f8: mov      r1, #0x10
003a94fc: bl       #0x31167c
003a9500: ldr      r3, [r4, sl]
003a9504: add      r7, r7, #0x28
003a9508: movw     sl, #0x13f8
003a950c: strb     r8, [r3]
003a9510: movw     r3, #0x13e4
003a9514: strb     fp, [r4, r3]
003a9518: movw     r3, #0x13fc
003a951c: str      r7, [r4, r3]
003a9520: mov      r0, r7
003a9524: str      r7, [r4, sl]
003a9528: mov      r1, #0x10
003a952c: bl       #0x31167c
003a9530: ldr      r3, [r4, sl]
003a9534: add      r7, r4, #0x1400
003a9538: movw     sl, #0x1410
003a953c: strb     r8, [r3]
003a9540: movw     r3, #0x1414
003a9544: str      r7, [r4, r3]
003a9548: mov      r0, r7
003a954c: str      r7, [r4, sl]
003a9550: mov      r1, #0x10
003a9554: bl       #0x31167c
003a9558: ldr      r3, [r4, sl]
003a955c: add      r7, r7, #0x18
003a9560: movw     sl, #0x1428
003a9564: strb     r8, [r3]
003a9568: movw     r3, #0x142c
003a956c: str      r7, [r4, r3]
003a9570: mov      r0, r7
003a9574: str      r7, [r4, sl]
003a9578: mov      r1, #0x10
003a957c: bl       #0x31167c
003a9580: ldr      r2, [r4, sl]
003a9584: mov      r3, #0
003a9588: mov      r1, #0xbf000000
003a958c: strb     r8, [r2]
003a9590: movw     r2, #0x14a8
003a9594: strb     r6, [r4, r2]
003a9598: movw     r2, #0x1430
003a959c: strb     fp, [r4, r2]
003a95a0: movw     r2, #0x1434
003a95a4: str      r8, [r4, r2]
003a95a8: movw     r2, #0x1438
003a95ac: str      r8, [r4, r2]
003a95b0: movw     r2, #0x1448
003a95b4: strb     fp, [r4, r2]
003a95b8: movw     r2, #0x1449
003a95bc: strb     r8, [r4, r2]
003a95c0: movw     r2, #0x144c
003a95c4: str      r8, [r4, r2]
003a95c8: movw     r2, #0x1450
003a95cc: str      r3, [r4, r2]
003a95d0: movw     r2, #0x1454
003a95d4: str      r3, [r4, r2]
003a95d8: movw     r2, #0x1458
003a95dc: str      r3, [r4, r2]
003a95e0: movw     r2, #0x145c
003a95e4: str      r3, [r4, r2]
003a95e8: movw     r2, #0x1460
003a95ec: str      r3, [r4, r2]
003a95f0: movw     r2, #0x1464
003a95f4: str      r3, [r4, r2]
003a95f8: movw     r2, #0x1468
003a95fc: str      r3, [r4, r2]
003a9600: movw     r2, #0x146c
003a9604: str      r3, [r4, r2]
003a9608: movw     r2, #0x1470
003a960c: str      r3, [r4, r2]
003a9610: movw     r2, #0x1474
003a9614: str      r3, [r4, r2]
003a9618: movw     r2, #0x1478
003a961c: str      r3, [r4, r2]
003a9620: movw     r2, #0x147c
003a9624: str      r3, [r4, r2]
003a9628: mov      r2, #0x1480
003a962c: strb     r8, [r4, r2]
003a9630: movw     r2, #0x1481
003a9634: strb     r8, [r4, r2]
003a9638: movw     r2, #0x1484
003a963c: str      r8, [r4, r2]
003a9640: movw     r2, #0x1488
003a9644: str      r8, [r4, r2]
003a9648: movw     r2, #0x148c
003a964c: str      r8, [r4, r2]
003a9650: movw     r2, #0x1490
003a9654: str      r8, [r4, r2]
003a9658: movw     r2, #0x1494
003a965c: str      r8, [r4, r2]
003a9660: movw     r2, #0x1498
003a9664: str      r6, [r4, r2]
003a9668: movw     r2, #0x149c
003a966c: str      r8, [r4, r2]
003a9670: movw     r2, #0x14a0
003a9674: str      r8, [r4, r2]
003a9678: movw     r2, #0x14a4
003a967c: str      r8, [r4, r2]
003a9680: movw     r2, #0x14aa
003a9684: strh     r8, [r4, r2]
003a9688: movw     r2, #0x14ac
003a968c: strb     r8, [r4, r2]
003a9690: movw     r2, #0x14d8
003a9694: str      r3, [r4, r2]
003a9698: add      r1, r1, #0x800000
003a969c: movw     r2, #0x14fc
003a96a0: str      r1, [r4, r2]
003a96a4: movw     r2, #0x1504
003a96a8: str      r6, [r4, r2]
003a96ac: movw     r2, #0x14ad
003a96b0: strb     r8, [r4, r2]
003a96b4: movw     r2, #0x14b0
003a96b8: str      r3, [r4, r2]
003a96bc: movw     r2, #0x14b4
003a96c0: str      r3, [r4, r2]
003a96c4: movw     r2, #0x14b8
003a96c8: str      r3, [r4, r2]
003a96cc: movw     r2, #0x14bc
003a96d0: str      r3, [r4, r2]
003a96d4: mov      r2, #0x14c0
003a96d8: str      r3, [r4, r2]
003a96dc: movw     r2, #0x14c4
003a96e0: str      r3, [r4, r2]
003a96e4: movw     r3, #0x14c8
003a96e8: strb     r8, [r4, r3]
003a96ec: movw     r3, #0x14ca
003a96f0: strh     r6, [r4, r3]
003a96f4: movw     r3, #0x14cc
003a96f8: str      r8, [r4, r3]
003a96fc: movw     r3, #0x14d0
003a9700: strh     r8, [r4, r3]
003a9704: movw     r3, #0x14d4
003a9708: str      r8, [r4, r3]
003a970c: movw     r3, #0x14dc
003a9710: strb     r8, [r4, r3]
003a9714: movw     r3, #0x14e4
003a9718: strb     r8, [r4, r3]
003a971c: movw     r3, #0x14e5
003a9720: strb     r8, [r4, r3]
003a9724: movw     r3, #0x14e8
003a9728: str      r8, [r4, r3]
003a972c: movw     r3, #0x14ec
003a9730: str      r8, [r4, r3]
003a9734: add      r7, r4, #0x1500
003a9738: movw     r3, #0x14f0
003a973c: add      r0, r4, #0x1a40
003a9740: strb     r8, [r4, r3]
003a9744: add      r0, r0, #8
003a9748: mov      r3, #0x1500
003a974c: add      r7, r7, #8
003a9750: str      r6, [r4, r3]
003a9754: str      r0, [sp, #0x10]
003a9758: mov      r0, r7
003a975c: bl       #0x3a6a24
003a9760: ldr      r0, [sp, #0x10]
003a9764: bl       #0x3a6a24
003a9768: add      r0, r4, #0x304
003a976c: mov      r1, r4
003a9770: strb     fp, [r4, #0x28]
003a9774: bl       #0x4a191c
003a9778: strb     fp, [r4, #0x1c4]
003a977c: strb     fp, [r4, #0x85]
003a9780: mov      r0, #0x10
003a9784: mov      r1, r8
003a9788: bl       #0x310570
003a978c: ldr      r3, [pc, #0x13c]
003a9790: ldr      ip, [sp, #0xc]
003a9794: mov      r6, r0
003a9798: ldr      r3, [sb, r3]
003a979c: cmp      ip, r8
003a97a0: strb     r8, [r6, #0xa]
003a97a4: add      r3, r3, #8
003a97a8: str      r8, [r0, #0xc]
003a97ac: stm      r0, {r3, ip}
003a97b0: strb     r8, [r6, #8]
003a97b4: strb     r8, [r6, #9]
003a97b8: beq      #0x3a986c
003a97bc: mov      r0, ip
003a97c0: mov      r1, r6
003a97c4: bl       #0x404e10
003a97c8: ldr      r3, [r4, #0x378]
003a97cc: ldr      r0, [sp, #0x20]
003a97d0: mov      r1, r4
003a97d4: str      r4, [r3, #0xc]
003a97d8: bl       #0x3db480
003a97dc: ldr      r0, [sp, #0x1c]
003a97e0: mov      r1, r4
003a97e4: bl       #0x3cb7c0
003a97e8: ldr      r0, [sp, #0x14]
003a97ec: mov      r1, r4
003a97f0: bl       #0x3c9890
003a97f4: mov      r0, r5
003a97f8: mov      r1, r4
003a97fc: bl       #0x3c1600
003a9800: ldr      r0, [sp, #0x18]
003a9804: mov      r1, r4
003a9808: bl       #0x3dec0c
003a980c: mov      r6, #0
003a9810: str      r4, [r4, #0x380]
003a9814: mov      r1, r6
003a9818: mov      r0, r5
003a981c: add      r6, r6, #1
003a9820: bl       #0x3c7318
003a9824: cmp      r6, #0x14
003a9828: bne      #0x3a9814
003a982c: mov      r1, #0
003a9830: movw     r2, #0x14e0
003a9834: str      r1, [r4, r2]
003a9838: mvn      r3, #0
003a983c: movw     r2, #0x14f4
003a9840: str      r3, [r4, r2]
003a9844: str      r7, [r4, #0x100]
003a9848: ldr      sl, [sp, #0x10]
003a984c: movw     r2, #0x14f8
003a9850: mov      r0, r4
003a9854: str      sl, [r4, #0x104]
003a9858: str      r3, [r4, r2]
003a985c: mov      r3, #1
003a9860: strb     r3, [r4, #0xf8]
003a9864: add      sp, sp, #0x3c
003a9868: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a986c: ldr      r3, [pc, #0x60]
003a9870: ldr      r3, [sb, r3]
003a9874: ldr      r3, [r3]
003a9878: cmp      r3, #2
003a987c: streq    ip, [r4, #0x374]
003a9880: beq      #0x3a97bc
003a9884: cmp      r3, #1
003a9888: bne      #0x3a97bc
003a988c: ldr      r0, [pc, #0x44]
003a9890: ldr      r1, [pc, #0x44]
003a9894: ldr      r2, [pc, #0x44]
003a9898: ldr      r0, [sb, r0]
003a989c: ldr      r3, [pc, #0x40]
003a98a0: mov      lr, #0x44
003a98a4: add      r1, pc, r1
003a98a8: add      r0, r0, #0xa8
003a98ac: add      r2, pc, r2
003a98b0: add      r3, pc, r3
003a98b4: str      ip, [sp, #0xc]
003a98b8: str      lr, [sp]
003a98bc: bl       #0x30e004
003a98c0: ldr      ip, [sp, #0xc]
003a98c4: b        #0x3a97bc
003a98c8: subseq   fp, lr, r8, asr #13
003a98cc: andeq    r2, r0, r8, lsl #28
003a98d0: andeq    r2, r0, r4, lsr #21
003a98d4: andeq    r3, r0, r0, asr #19
003a98d8: andeq    r1, r0, r0, asr #19
003a98dc: subseq   r4, r1, r4, lsr fp
003a98e0: subseq   sb, r1, r4, lsl ip
003a98e4: subseq   sb, r1, r0, lsr #24
