
# _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsERNS4_12ReturnValuesE
0037c390: push     {r4, r5, r6, r7, r8, lr}
0037c394: mov      r5, r3
0037c398: ldr      r3, [r3, #0x24]
0037c39c: ldr      r4, [pc, #0x70]
0037c3a0: sub      sp, sp, #8
0037c3a4: ldr      lr, [r3]
0037c3a8: ldr      ip, [r3, #4]
0037c3ac: add      r4, pc, r4
0037c3b0: mov      r6, r0
0037c3b4: cmp      lr, ip
0037c3b8: mov      r7, r1
0037c3bc: mov      r8, r2
0037c3c0: beq      #0x37c3d8
0037c3c4: mov      r0, r3
0037c3c8: mov      r1, lr
0037c3cc: mov      r2, ip
0037c3d0: add      r3, sp, #4
0037c3d4: bl       #0x31c3cc
0037c3d8: mov      r1, r7
0037c3dc: mov      r0, r6
0037c3e0: bl       #0x37c314
0037c3e4: mov      r2, r8
0037c3e8: mov      r1, r0
0037c3ec: mov      r3, r5
0037c3f0: add      r0, r6, #4
0037c3f4: bl       #0x31abe8
0037c3f8: ldr      r3, [pc, #0x18]
0037c3fc: ldr      r3, [r4, r3]
0037c400: ldr      r2, [r3]
0037c404: add      r2, r2, #1
0037c408: str      r2, [r3]
0037c40c: add      sp, sp, #8
0037c410: pop      {r4, r5, r6, r7, r8, pc}
0037c414: rsbeq    r8, r1, r4, ror #13
0037c418: strdeq   r2, r3, [r0], -ip

# _ZN9LuaScript12_PushVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037be00: ldr      r3, [r2, #0x5c]
0037be04: push     {r4, r5, r6, lr}
0037be08: cmp      r3, #0
0037be0c: mov      r4, r2
0037be10: beq      #0x37be38
0037be14: add      r5, r2, #0x4c
0037be18: mov      r0, r5
0037be1c: ldr      r1, [r2, #0x50]
0037be20: bl       #0x37bd7c
0037be24: mov      r3, #0
0037be28: str      r5, [r4, #0x58]
0037be2c: str      r3, [r4, #0x5c]
0037be30: str      r5, [r4, #0x54]
0037be34: str      r3, [r4, #0x50]
0037be38: mov      r3, #1
0037be3c: strb     r3, [r4, #0x64]
0037be40: pop      {r4, r5, r6, pc}

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjSsENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE5eraseENS_17_Rb_tree_iteratorIS5_S9_EE
0037be48: push     {r4, r5, r6, lr}
0037be4c: mov      r4, r0
0037be50: add      r3, r4, #0xc
0037be54: ldr      r0, [r1]
0037be58: add      r2, r4, #8
0037be5c: add      r1, r4, #4
0037be60: bl       #0x336004
0037be64: add      r3, r0, #0x14
0037be68: mov      r5, r0
0037be6c: ldr      r0, [r3, #0x14]
0037be70: cmp      r0, r3
0037be74: beq      #0x37be94
0037be78: cmp      r0, #0
0037be7c: beq      #0x37be94
0037be80: ldr      r1, [r5, #0x14]
0037be84: rsb      r1, r0, r1
0037be88: cmp      r1, #0x80
0037be8c: bhi      #0x37beb8
0037be90: bl       #0x708f00
0037be94: cmp      r5, #0
0037be98: beq      #0x37bea8
0037be9c: mov      r0, r5
0037bea0: mov      r1, #0x2c
0037bea4: bl       #0x708f00
0037bea8: ldr      r3, [r4, #0x10]
0037beac: sub      r3, r3, #1
0037beb0: str      r3, [r4, #0x10]
0037beb4: pop      {r4, r5, r6, pc}
0037beb8: bl       #0x310440
0037bebc: b        #0x37be94

# _ZNSt3mapIjSsSt4lessIjESaISt4pairIKjSsEEEixIjEERSsRKT_
0037dac4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0037dac8: ldr      r5, [pc, #0x16c]
0037dacc: ldr      r6, [pc, #0x16c]
0037dad0: ldr      r4, [r0, #4]
0037dad4: add      r5, pc, r5
0037dad8: ldr      r3, [r5, r6]
0037dadc: sub      sp, sp, #0x40
0037dae0: cmp      r4, #0
0037dae4: ldr      r3, [r3]
0037dae8: mov      r8, r0
0037daec: mov      r7, r1
0037daf0: str      r3, [sp, #0x3c]
0037daf4: moveq    r4, r0
0037daf8: beq      #0x37db2c
0037dafc: ldr      r1, [r1]
0037db00: mov      r2, r0
0037db04: b        #0x37db10
0037db08: mov      r2, r4
0037db0c: mov      r4, r3
0037db10: ldr      r3, [r4, #0x10]
0037db14: cmp      r3, r1
0037db18: ldrlo    r3, [r4, #0xc]
0037db1c: ldrhs    r3, [r4, #8]
0037db20: movlo    r4, r2
0037db24: cmp      r3, #0
0037db28: bne      #0x37db08
0037db2c: cmp      r8, r4
0037db30: beq      #0x37db48
0037db34: ldr      r2, [r7]
0037db38: ldr      r3, [r4, #0x10]
0037db3c: mov      r0, r4
0037db40: cmp      r2, r3
0037db44: bhs      #0x37dc04
0037db48: add      sb, sp, #0x24
0037db4c: mov      r0, sb
0037db50: mov      r1, #0x10
0037db54: str      sb, [sp, #0x34]
0037db58: str      sb, [sp, #0x38]
0037db5c: bl       #0x31167c
0037db60: ldr      r3, [sp, #0x34]
0037db64: mov      r2, #0
0037db68: add      sl, sp, #0x40
0037db6c: strb     r2, [r3]
0037db70: ldr      r3, [r7]
0037db74: ldr      r1, [sp, #0x38]
0037db78: ldr      r2, [sp, #0x34]
0037db7c: str      r3, [sl, #-0x38]!
0037db80: add      r7, sl, #4
0037db84: mov      r0, r7
0037db88: str      r7, [sp, #0x1c]
0037db8c: str      r7, [sp, #0x20]
0037db90: bl       #0x3116e8
0037db94: add      r0, sp, #4
0037db98: mov      r1, r8
0037db9c: mov      r3, sl
0037dba0: mov      r2, sp
0037dba4: str      r4, [sp]
0037dba8: bl       #0x37cfdc
0037dbac: ldr      r0, [sp, #0x20]
0037dbb0: ldr      r4, [sp, #4]
0037dbb4: cmp      r0, r7
0037dbb8: beq      #0x37dbd8
0037dbbc: cmp      r0, #0
0037dbc0: beq      #0x37dbd8
0037dbc4: ldr      r1, [sp, #0xc]
0037dbc8: rsb      r1, r0, r1
0037dbcc: cmp      r1, #0x80
0037dbd0: bhi      #0x37dc30
0037dbd4: bl       #0x708f00
0037dbd8: ldr      r0, [sp, #0x38]
0037dbdc: cmp      r0, sb
0037dbe0: beq      #0x37dc00
0037dbe4: cmp      r0, #0
0037dbe8: beq      #0x37dc00
0037dbec: ldr      r1, [sp, #0x24]
0037dbf0: rsb      r1, r0, r1
0037dbf4: cmp      r1, #0x80
0037dbf8: bhi      #0x37dc24
0037dbfc: bl       #0x708f00
0037dc00: mov      r0, r4
0037dc04: ldr      r3, [r5, r6]
0037dc08: ldr      r2, [sp, #0x3c]
0037dc0c: add      r0, r0, #0x14
0037dc10: ldr      r3, [r3]
0037dc14: cmp      r2, r3
0037dc18: bne      #0x37dc38
0037dc1c: add      sp, sp, #0x40
0037dc20: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0037dc24: bl       #0x310440
0037dc28: mov      r0, r4
0037dc2c: b        #0x37dc04
0037dc30: bl       #0x310440
0037dc34: b        #0x37dbd8
0037dc38: bl       #0x30e310
0037dc3c: strhteq  r6, [r1], #-0xfc
0037dc40: andeq    r4, r0, ip, lsr #1

# _ZN9LuaScript11_PopVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037dc44: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0037dc48: ldr      sb, [pc, #0x174]
0037dc4c: ldr      r4, [r2, #0x54]
0037dc50: sub      sp, sp, #8
0037dc54: mov      r8, r2
0037dc58: add      sb, pc, sb
0037dc5c: add      r7, r2, #0x4c
0037dc60: add      r5, r2, #0x34
0037dc64: add      sl, sp, #4
0037dc68: cmp      r7, r4
0037dc6c: beq      #0x37dcd4
0037dc70: ldr      r0, [r4, #0x28]
0037dc74: ldr      r6, [r4, #0x24]
0037dc78: rsb      r6, r0, r6
0037dc7c: cmp      r6, #0
0037dc80: ble      #0x37dd44
0037dc84: mov      r0, r5
0037dc88: add      r1, r4, #0x10
0037dc8c: bl       #0x37dac4
0037dc90: add      r3, r4, #0x14
0037dc94: cmp      r3, r0
0037dc98: beq      #0x37dca8
0037dc9c: ldr      r1, [r4, #0x28]
0037dca0: ldr      r2, [r4, #0x24]
0037dca4: bl       #0x3109e0
0037dca8: ldr      r2, [r4, #0xc]
0037dcac: cmp      r2, #0
0037dcb0: bne      #0x37dcbc
0037dcb4: b        #0x37dd10
0037dcb8: mov      r2, r3
0037dcbc: ldr      r3, [r2, #8]
0037dcc0: cmp      r3, #0
0037dcc4: bne      #0x37dcb8
0037dcc8: mov      r4, r2
0037dccc: cmp      r7, r4
0037dcd0: bne      #0x37dc70
0037dcd4: ldr      r3, [r8, #0x5c]
0037dcd8: cmp      r3, #0
0037dcdc: beq      #0x37dd00
0037dce0: mov      r0, r7
0037dce4: ldr      r1, [r8, #0x50]
0037dce8: bl       #0x37bd7c
0037dcec: mov      r3, #0
0037dcf0: str      r7, [r8, #0x58]
0037dcf4: str      r3, [r8, #0x5c]
0037dcf8: str      r7, [r8, #0x54]
0037dcfc: str      r3, [r8, #0x50]
0037dd00: mov      r3, #0
0037dd04: strb     r3, [r8, #0x64]
0037dd08: add      sp, sp, #8
0037dd0c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0037dd10: ldr      r3, [r4, #4]
0037dd14: ldr      r1, [r3, #0xc]
0037dd18: cmp      r4, r1
0037dd1c: bne      #0x37dd38
0037dd20: mov      r4, r3
0037dd24: ldr      r3, [r3, #4]
0037dd28: ldr      r2, [r3, #0xc]
0037dd2c: cmp      r2, r4
0037dd30: beq      #0x37dd20
0037dd34: ldr      r2, [r4, #0xc]
0037dd38: cmp      r2, r3
0037dd3c: movne    r4, r3
0037dd40: b        #0x37dc68
0037dd44: mov      r1, sb
0037dd48: mov      r2, r6
0037dd4c: bl       #0x30e5e0
0037dd50: cmp      r0, #0
0037dd54: bne      #0x37dc84
0037dd58: cmp      r6, #0
0037dd5c: bne      #0x37dc84
0037dd60: ldr      r3, [r8, #0x38]
0037dd64: cmp      r3, #0
0037dd68: ldrne    r0, [r4, #0x10]
0037dd6c: movne    r1, r5
0037dd70: bne      #0x37dd7c
0037dd74: b        #0x37dca8
0037dd78: mov      r3, r2
0037dd7c: ldr      r2, [r3, #0x10]
0037dd80: cmp      r2, r0
0037dd84: ldrlo    r2, [r3, #0xc]
0037dd88: ldrhs    r2, [r3, #8]
0037dd8c: movlo    r3, r1
0037dd90: mov      r1, r3
0037dd94: cmp      r2, #0
0037dd98: bne      #0x37dd78
0037dd9c: cmp      r5, r3
0037dda0: beq      #0x37dca8
0037dda4: ldr      r2, [r3, #0x10]
0037dda8: cmp      r2, r0
0037ddac: bhi      #0x37dca8
0037ddb0: mov      r0, r5
0037ddb4: mov      r1, sl
0037ddb8: str      r3, [sp, #4]
0037ddbc: bl       #0x37be48
0037ddc0: b        #0x37dca8
0037ddc4: ldrheq   sp, [r4], #-0xb0

# _ZNK9LuaScript12_GetFuncNameEPKc
0037c314: push     {r4, r5, r6, lr}
0037c318: mov      r5, r0
0037c31c: mov      r0, r1
0037c320: mov      r4, r1
0037c324: bl       #0x37c164
0037c328: ldr      r3, [r5, #0x38]
0037c32c: add      r5, r5, #0x34
0037c330: cmp      r3, #0
0037c334: beq      #0x37c388
0037c338: mov      r1, r5
0037c33c: b        #0x37c344
0037c340: mov      r3, r2
0037c344: ldr      r2, [r3, #0x10]
0037c348: cmp      r0, r2
0037c34c: ldrhi    r2, [r3, #0xc]
0037c350: ldrls    r2, [r3, #8]
0037c354: movhi    r3, r1
0037c358: mov      r1, r3
0037c35c: cmp      r2, #0
0037c360: bne      #0x37c340
0037c364: cmp      r5, r3
0037c368: beq      #0x37c380
0037c36c: ldr      r2, [r3, #0x10]
0037c370: cmp      r0, r2
0037c374: blo      #0x37c388
0037c378: cmp      r5, r3
0037c37c: ldrne    r4, [r3, #0x28]
0037c380: mov      r0, r4
0037c384: pop      {r4, r5, r6, pc}
0037c388: mov      r3, r5
0037c38c: b        #0x37c378

# _ZN6glitch4core10hashStringEPKc
0037c164: push     {r4, r5, r6, r7, lr}
0037c168: ldr      r4, [pc, #0x118]
0037c16c: ldr      r3, [pc, #0x118]
0037c170: ldr      r7, [pc, #0x118]
0037c174: add      r4, pc, r4
0037c178: ldr      r5, [r4, r3]
0037c17c: ldr      r3, [r4, r7]
0037c180: sub      sp, sp, #0x24
0037c184: ldr      r2, [r5]
0037c188: ldr      r3, [r3]
0037c18c: mov      r6, r0
0037c190: tst      r2, #1
0037c194: str      r3, [sp, #0x1c]
0037c198: beq      #0x37c244
0037c19c: add      r5, sp, #4
0037c1a0: mov      r0, r6
0037c1a4: str      r5, [sp, #0x14]
0037c1a8: str      r5, [sp, #0x18]
0037c1ac: bl       #0x30de54
0037c1b0: mov      r1, r6
0037c1b4: add      r2, r6, r0
0037c1b8: mov      r0, r5
0037c1bc: bl       #0x3116e8
0037c1c0: ldr      r0, [sp, #0x18]
0037c1c4: ldr      ip, [sp, #0x14]
0037c1c8: cmp      r0, ip
0037c1cc: moveq    r6, #0
0037c1d0: beq      #0x37c200
0037c1d4: mov      r2, r0
0037c1d8: mov      r6, #0
0037c1dc: ldrsb    r1, [r2], #1
0037c1e0: movw     r3, #0x79b9
0037c1e4: movt     r3, #0x9e37
0037c1e8: add      r3, r1, r3
0037c1ec: add      r3, r3, r6, lsl #6
0037c1f0: add      r3, r3, r6, lsr #2
0037c1f4: cmp      r2, ip
0037c1f8: eor      r6, r6, r3
0037c1fc: bne      #0x37c1dc
0037c200: cmp      r0, r5
0037c204: beq      #0x37c224
0037c208: cmp      r0, #0
0037c20c: beq      #0x37c224
0037c210: ldr      r1, [sp, #4]
0037c214: rsb      r1, r0, r1
0037c218: cmp      r1, #0x80
0037c21c: bhi      #0x37c27c
0037c220: bl       #0x708f00
0037c224: ldr      r3, [r4, r7]
0037c228: ldr      r2, [sp, #0x1c]
0037c22c: mov      r0, r6
0037c230: ldr      r3, [r3]
0037c234: cmp      r2, r3
0037c238: bne      #0x37c284
0037c23c: add      sp, sp, #0x24
0037c240: pop      {r4, r5, r6, r7, pc}
0037c244: mov      r0, r5
0037c248: bl       #0x30e76c
0037c24c: cmp      r0, #0
0037c250: beq      #0x37c19c
0037c254: mov      r0, r5
0037c258: bl       #0x30ea3c
0037c25c: ldr      r3, [pc, #0x30]
0037c260: ldr      r0, [r4, r3]
0037c264: ldr      r3, [pc, #0x2c]
0037c268: ldr      r1, [r4, r3]
0037c26c: ldr      r3, [pc, #0x28]
0037c270: ldr      r2, [r4, r3]
0037c274: bl       #0x30e304
0037c278: b        #0x37c19c
0037c27c: bl       #0x310440
0037c280: b        #0x37c224
0037c284: bl       #0x30e310
0037c288: rsbeq    r8, r1, ip, lsl sb
0037c28c: muleq    r0, ip, sb
0037c290: andeq    r4, r0, ip, lsr #1
0037c294: andeq    r1, r0, r4, ror #13
0037c298: andeq    r2, r0, r4, lsr #14
0037c29c: muleq    r0, r0, r8

# _ZN9LuaScriptC1Eb
0037c584: push     {r4, r5, r6, r7, r8, lr}
0037c588: ldr      r6, [pc, #0xd8]
0037c58c: ldr      r3, [pc, #0xd8]
0037c590: mov      r7, r0
0037c594: add      r6, pc, r6
0037c598: ldr      r3, [r6, r3]
0037c59c: mov      r4, r0
0037c5a0: mov      r8, r1
0037c5a4: add      r3, r3, #8
0037c5a8: str      r3, [r7], #4
0037c5ac: mov      r0, r7
0037c5b0: bl       #0x31b268
0037c5b4: ldr      r2, [pc, #0xb4]
0037c5b8: mov      r5, #0
0037c5bc: mov      r3, r4
0037c5c0: ldr      r2, [r6, r2]
0037c5c4: str      r7, [r4, #0x14]
0037c5c8: str      r5, [r4, #0x18]
0037c5cc: add      r2, r2, #8
0037c5d0: str      r2, [r4, #0x10]
0037c5d4: str      r5, [r4, #0x20]
0037c5d8: mov      r2, r4
0037c5dc: strb     r5, [r3, #0x1c]!
0037c5e0: str      r3, [r4, #0x28]
0037c5e4: str      r3, [r4, #0x24]
0037c5e8: str      r5, [r4, #0x2c]
0037c5ec: mov      r3, r4
0037c5f0: str      r5, [r4, #0x38]
0037c5f4: strb     r5, [r2, #0x34]!
0037c5f8: str      r2, [r4, #0x40]
0037c5fc: str      r2, [r4, #0x3c]
0037c600: add      r0, r4, #0x68
0037c604: str      r5, [r4, #0x44]
0037c608: str      r5, [r4, #0x50]
0037c60c: strb     r5, [r3, #0x4c]!
0037c610: str      r3, [r4, #0x58]
0037c614: str      r3, [r4, #0x54]
0037c618: str      r5, [r4, #0x5c]
0037c61c: strb     r5, [r4, #0x64]
0037c620: str      r0, [r4, #0x78]
0037c624: str      r0, [r4, #0x7c]
0037c628: mov      r1, #0x10
0037c62c: bl       #0x31167c
0037c630: ldr      r2, [r4, #0x78]
0037c634: mov      r3, r4
0037c638: cmp      r8, r5
0037c63c: strb     r5, [r2]
0037c640: str      r5, [r4, #0x84]
0037c644: strb     r5, [r3, #0x80]!
0037c648: str      r3, [r4, #0x8c]
0037c64c: str      r5, [r4, #0x90]
0037c650: str      r3, [r4, #0x88]
0037c654: bne      #0x37c660
0037c658: mov      r0, r4
0037c65c: bl       #0x37b5a0
0037c660: mov      r0, r4
0037c664: pop      {r4, r5, r6, r7, r8, pc}

# _ZN3sfc6script3lua8Instance5pCallEPKcRKNS1_9ArgumentsERNS1_12ReturnValuesE
0031abe8: push     {r4, r5, r6, lr}
0031abec: mov      r4, r0
0031abf0: mov      r0, r1
0031abf4: mvn      r1, #0x2700
0031abf8: mov      r5, r2
0031abfc: sub      r1, r1, #0x11
0031ac00: mov      r2, r0
0031ac04: ldr      r0, [r4, #4]
0031ac08: mov      r6, r3
0031ac0c: bl       #0x84c1ec
0031ac10: mov      r0, r4
0031ac14: mov      r1, r5
0031ac18: mov      r2, r6
0031ac1c: pop      {r4, r5, r6, lr}
0031ac20: b        #0x31ab4c

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjSsENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0037bd7c: push     {r4, r5, r6, lr}
0037bd80: subs     r4, r1, #0
0037bd84: mov      r6, r0
0037bd88: bne      #0x37bda8
0037bd8c: b        #0x37bdfc
0037bd90: bl       #0x708f00
0037bd94: mov      r0, r4
0037bd98: mov      r1, #0x2c
0037bd9c: bl       #0x708f00
0037bda0: subs     r4, r5, #0
0037bda4: beq      #0x37bdfc
0037bda8: mov      r0, r6
0037bdac: ldr      r1, [r4, #0xc]
0037bdb0: bl       #0x37bd7c
0037bdb4: add      r2, r4, #0x14
0037bdb8: ldr      r3, [r2, #0x14]
0037bdbc: ldr      r5, [r4, #8]
0037bdc0: cmp      r3, r2
0037bdc4: mov      r0, r3
0037bdc8: beq      #0x37bd94
0037bdcc: cmp      r3, #0
0037bdd0: beq      #0x37bd94
0037bdd4: ldr      r1, [r2]
0037bdd8: rsb      r1, r3, r1
0037bddc: cmp      r1, #0x80
0037bde0: bls      #0x37bd90
0037bde4: bl       #0x310440
0037bde8: mov      r0, r4
0037bdec: mov      r1, #0x2c
0037bdf0: bl       #0x708f00
0037bdf4: subs     r4, r5, #0
0037bdf8: bne      #0x37bda8
0037bdfc: pop      {r4, r5, r6, pc}

# _ZN9LuaScript13_AddToVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ec70: push     {r4, r5, r6, r7, lr}
0037ec74: ldr      r5, [r0, #4]
0037ec78: mov      r6, r2
0037ec7c: sub      sp, sp, #0xc
0037ec80: ldm      r5, {r1, r3}
0037ec84: mov      r4, r0
0037ec88: rsb      r3, r1, r3
0037ec8c: asr      r3, r3, #4
0037ec90: add      r2, r3, r3, lsl #3
0037ec94: add      r2, r2, r2, lsl #6
0037ec98: add      r2, r3, r2, lsl #3
0037ec9c: add      r2, r2, r2, lsl #15
0037eca0: add      r3, r3, r2, lsl #3
0037eca4: rsb      r3, r3, #0
0037eca8: cmp      r3, #1
0037ecac: bls      #0x37ecc4
0037ecb0: cmp      r3, #0
0037ecb4: beq      #0x37eccc
0037ecb8: ldr      r3, [r1, #4]
0037ecbc: cmp      r3, #4
0037ecc0: beq      #0x37ece8
0037ecc4: add      sp, sp, #0xc
0037ecc8: pop      {r4, r5, r6, r7, pc}
0037eccc: ldr      r0, [pc, #0x1a0]
0037ecd0: add      r0, pc, r0
0037ecd4: bl       #0x708eb0
0037ecd8: ldr      r1, [r5]
0037ecdc: ldr      r3, [r1, #4]
0037ece0: cmp      r3, #4
0037ece4: bne      #0x37ecc4
0037ece8: mov      r0, r4
0037ecec: mov      r1, #1
0037ecf0: bl       #0x37baf8
0037ecf4: ldr      r3, [r0, #4]
0037ecf8: cmp      r3, #4
0037ecfc: bne      #0x37ecc4
0037ed00: ldr      r5, [r4, #4]
0037ed04: ldm      r5, {r0, r3}
0037ed08: rsb      r3, r0, r3
0037ed0c: asr      r3, r3, #4
0037ed10: add      r2, r3, r3, lsl #3
0037ed14: add      r2, r2, r2, lsl #6
0037ed18: add      r2, r3, r2, lsl #3
0037ed1c: add      r2, r2, r2, lsl #15
0037ed20: add      r3, r3, r2, lsl #3
0037ed24: cmp      r3, #0
0037ed28: bne      #0x37ed3c
0037ed2c: ldr      r0, [pc, #0x144]
0037ed30: add      r0, pc, r0
0037ed34: bl       #0x708eb0
0037ed38: ldr      r0, [r5]
0037ed3c: bl       #0x31c49c
0037ed40: bl       #0x37c164
0037ed44: ldrb     r3, [r6, #0x64]
0037ed48: str      r0, [sp, #4]
0037ed4c: cmp      r3, #0
0037ed50: beq      #0x37edac
0037ed54: ldr      r3, [r6, #0x50]
0037ed58: add      ip, r6, #0x4c
0037ed5c: cmp      r3, #0
0037ed60: beq      #0x37ee28
0037ed64: mov      r1, ip
0037ed68: b        #0x37ed70
0037ed6c: mov      r3, r2
0037ed70: ldr      r2, [r3, #0x10]
0037ed74: cmp      r0, r2
0037ed78: ldrhi    r2, [r3, #0xc]
0037ed7c: ldrls    r2, [r3, #8]
0037ed80: movhi    r3, r1
0037ed84: mov      r1, r3
0037ed88: cmp      r2, #0
0037ed8c: bne      #0x37ed6c
0037ed90: cmp      ip, r3
0037ed94: beq      #0x37ee30
0037ed98: ldr      r2, [r3, #0x10]
0037ed9c: cmp      r0, r2
0037eda0: blo      #0x37ee28
0037eda4: cmp      ip, r3
0037eda8: beq      #0x37ee30
0037edac: add      r6, r6, #0x34
0037edb0: add      r5, sp, #4
0037edb4: mov      r1, r5
0037edb8: mov      r0, r6
0037edbc: bl       #0x37dac4
0037edc0: ldr      r4, [r4, #4]
0037edc4: mov      r5, r0
0037edc8: ldm      r4, {r0, r3}
0037edcc: rsb      r3, r0, r3
0037edd0: asr      r3, r3, #4
0037edd4: add      r2, r3, r3, lsl #3
0037edd8: add      r2, r2, r2, lsl #6
0037eddc: add      r2, r3, r2, lsl #3
0037ede0: add      r2, r2, r2, lsl #15
0037ede4: add      r3, r3, r2, lsl #3
0037ede8: rsb      r3, r3, #0
0037edec: cmp      r3, #1
0037edf0: bhi      #0x37ee04
0037edf4: ldr      r0, [pc, #0x80]
0037edf8: add      r0, pc, r0
0037edfc: bl       #0x708eb0
0037ee00: ldr      r0, [r4]
0037ee04: add      r0, r0, #0x70
0037ee08: bl       #0x31c49c
0037ee0c: mov      r4, r0
0037ee10: bl       #0x30de54
0037ee14: mov      r1, r4
0037ee18: add      r2, r4, r0
0037ee1c: mov      r0, r5
0037ee20: bl       #0x3109e0
0037ee24: b        #0x37ecc4
0037ee28: mov      r3, ip
0037ee2c: b        #0x37eda4
0037ee30: add      r5, sp, #4
0037ee34: mov      r0, ip
0037ee38: mov      r1, r5
0037ee3c: bl       #0x37dac4
0037ee40: add      r6, r6, #0x34
0037ee44: mov      r7, r0
0037ee48: mov      r1, r5
0037ee4c: mov      r0, r6
0037ee50: bl       #0x37dac4
0037ee54: cmp      r7, r0
0037ee58: mov      r3, r0
0037ee5c: beq      #0x37edb4
0037ee60: mov      r0, r7
0037ee64: ldr      r2, [r3, #0x10]
0037ee68: ldr      r1, [r3, #0x14]
0037ee6c: bl       #0x3109e0
0037ee70: b        #0x37edb4

# _ZNK9LuaScript11IsInVFTableEPKc
0037c2a0: push     {r4, lr}
0037c2a4: mov      r4, r0
0037c2a8: mov      r0, r1
0037c2ac: bl       #0x37c164
0037c2b0: ldr      r3, [r4, #0x38]
0037c2b4: add      r4, r4, #0x34
0037c2b8: cmp      r3, #0
0037c2bc: beq      #0x37c300
0037c2c0: mov      r1, r4
0037c2c4: b        #0x37c2cc
0037c2c8: mov      r3, r2
0037c2cc: ldr      r2, [r3, #0x10]
0037c2d0: cmp      r0, r2
0037c2d4: ldrhi    r2, [r3, #0xc]
0037c2d8: ldrls    r2, [r3, #8]
0037c2dc: movhi    r3, r1
0037c2e0: mov      r1, r3
0037c2e4: cmp      r2, #0
0037c2e8: bne      #0x37c2c8
0037c2ec: cmp      r4, r3
0037c2f0: beq      #0x37c300
0037c2f4: ldr      r2, [r3, #0x10]
0037c2f8: cmp      r0, r2
0037c2fc: bhs      #0x37c308
0037c300: mov      r0, #0
0037c304: pop      {r4, pc}
0037c308: subs     r0, r3, r4
0037c30c: movne    r0, #1
0037c310: pop      {r4, pc}

# _ZN10LuaManager7AddFileEP9LuaScriptPKc
0037b23c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b240: ldr      r4, [pc, #0x2e0]
0037b244: ldr      r6, [pc, #0x2e0]
0037b248: sub      sp, sp, #0x7c
0037b24c: add      r4, pc, r4
0037b250: ldr      r3, [r4, r6]
0037b254: subs     r7, r1, #0
0037b258: mov      sl, r0
0037b25c: ldr      r3, [r3]
0037b260: mov      r5, r2
0037b264: str      r3, [sp, #0x74]
0037b268: beq      #0x37b318
0037b26c: cmp      r5, #0
0037b270: beq      #0x37b280
0037b274: ldrsb    r3, [r5]
0037b278: cmp      r3, #0
0037b27c: bne      #0x37b2a4
0037b280: mov      r5, #0
0037b284: ldr      r3, [r4, r6]
0037b288: ldr      r2, [sp, #0x74]
0037b28c: mov      r0, r5
0037b290: ldr      r3, [r3]
0037b294: cmp      r2, r3
0037b298: bne      #0x37b524
0037b29c: add      sp, sp, #0x7c
0037b2a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b2a4: add      r8, sp, #0x5c
0037b2a8: mov      r0, r8
0037b2ac: add      r1, r7, #0x68
0037b2b0: mov      r2, r5
0037b2b4: bl       #0x3338cc
0037b2b8: ldr      r1, [pc, #0x270]
0037b2bc: mov      r0, r5
0037b2c0: add      r1, pc, r1
0037b2c4: bl       #0x30ebd4
0037b2c8: cmp      r0, #0
0037b2cc: beq      #0x37b3f8
0037b2d0: ldr      r1, [pc, #0x25c]
0037b2d4: mov      r2, #5
0037b2d8: add      r1, pc, r1
0037b2dc: bl       #0x30ec7c
0037b2e0: cmp      r0, #0
0037b2e4: bne      #0x37b36c
0037b2e8: ldr      r3, [sp, #0x70]
0037b2ec: add      r1, sp, #0x78
0037b2f0: add      r5, r7, #0x80
0037b2f4: str      r3, [r1, #-0x5c]!
0037b2f8: mov      r0, r5
0037b2fc: bl       #0x37a190
0037b300: cmp      r5, r0
0037b304: beq      #0x37b380
0037b308: mov      r5, #1
0037b30c: mov      r0, r8
0037b310: bl       #0x3139ac
0037b314: b        #0x37b284
0037b318: ldr      r3, [pc, #0x218]
0037b31c: ldr      r3, [r4, r3]
0037b320: ldr      r3, [r3]
0037b324: cmp      r3, #2
0037b328: streq    r7, [r7]
0037b32c: beq      #0x37b26c
0037b330: cmp      r3, #1
0037b334: bne      #0x37b26c
0037b338: ldr      r0, [pc, #0x1fc]
0037b33c: ldr      r1, [pc, #0x1fc]
0037b340: ldr      r2, [pc, #0x1fc]
0037b344: ldr      r0, [r4, r0]
0037b348: ldr      r3, [pc, #0x1f8]
0037b34c: mov      ip, #0x23
0037b350: add      r1, pc, r1
0037b354: add      r2, pc, r2
0037b358: add      r3, pc, r3
0037b35c: add      r0, r0, #0xa8
0037b360: str      ip, [sp]
0037b364: bl       #0x30e004
0037b368: b        #0x37b26c
0037b36c: ldr      r1, [pc, #0x1d8]
0037b370: mov      r0, r8
0037b374: add      r1, pc, r1
0037b378: bl       #0x379ef8
0037b37c: b        #0x37b2e8
0037b380: ldr      r3, [sp, #0x70]
0037b384: add      r1, sp, #0x78
0037b388: add      sl, sl, #4
0037b38c: str      r3, [r1, #-0x60]!
0037b390: mov      r0, sl
0037b394: bl       #0x37a300
0037b398: cmp      r0, sl
0037b39c: mov      sb, r0
0037b3a0: beq      #0x37b444
0037b3a4: ldr      sl, [r0, #0x28]
0037b3a8: mov      r2, #0
0037b3ac: mov      r3, #0
0037b3b0: ldr      r1, [sl]
0037b3b4: mov      r0, sl
0037b3b8: mov      lr, pc
0037b3bc: ldr      pc, [r1, #0x20]
0037b3c0: cmp      sl, #0
0037b3c4: beq      #0x37b4d0
0037b3c8: add      sb, sp, #0x24
0037b3cc: add      r1, r7, #4
0037b3d0: mov      r2, sl
0037b3d4: mov      r0, sb
0037b3d8: bl       #0x31acf4
0037b3dc: ldr      r3, [sp, #0x28]
0037b3e0: cmp      r3, #0
0037b3e4: beq      #0x37b40c
0037b3e8: mov      r0, sb
0037b3ec: bl       #0x31a68c
0037b3f0: mov      r5, #0
0037b3f4: b        #0x37b30c
0037b3f8: ldr      r1, [pc, #0x150]
0037b3fc: mov      r0, r8
0037b400: add      r1, pc, r1
0037b404: bl       #0x379ef8
0037b408: b        #0x37b2e8
0037b40c: add      r7, sp, #0x44
0037b410: mov      r0, sb
0037b414: bl       #0x31a68c
0037b418: ldr      r1, [sp, #0x70]
0037b41c: add      r2, sp, #0x20
0037b420: mov      r0, r7
0037b424: bl       #0x3140ec
0037b428: add      r0, sp, #8
0037b42c: mov      r1, r5
0037b430: mov      r2, r7
0037b434: bl       #0x37a9e8
0037b438: mov      r0, r7
0037b43c: bl       #0x3139ac
0037b440: b        #0x37b308
0037b444: ldr      r3, [pc, #0x108]
0037b448: mov      r2, #0
0037b44c: ldr      r1, [sp, #0x70]
0037b450: ldr      fp, [r4, r3]
0037b454: mov      r3, r2
0037b458: ldr      r0, [fp, #0x10]
0037b45c: ldr      ip, [r0, #0x34]
0037b460: mov      r0, ip
0037b464: ldr      ip, [ip]
0037b468: mov      lr, pc
0037b46c: ldr      pc, [ip, #0x88]
0037b470: cmp      r0, #0
0037b474: str      r0, [sp, #0x14]
0037b478: moveq    r5, r0
0037b47c: beq      #0x37b30c
0037b480: mov      r1, #0
0037b484: mov      r0, #0x30
0037b488: bl       #0x310570
0037b48c: ldr      r1, [sp, #0x14]
0037b490: mov      sl, r0
0037b494: bl       #0x3172d8
0037b498: ldr      r3, [sp, #0x70]
0037b49c: add      r1, sp, #0x78
0037b4a0: mov      r0, sb
0037b4a4: str      r3, [r1, #-0x68]!
0037b4a8: bl       #0x37b0fc
0037b4ac: str      sl, [r0]
0037b4b0: ldr      r3, [fp, #0x10]
0037b4b4: add      r1, sp, #0x14
0037b4b8: ldr      r3, [r3, #0x34]
0037b4bc: mov      r0, r3
0037b4c0: ldr      r3, [r3]
0037b4c4: mov      lr, pc
0037b4c8: ldr      pc, [r3, #0x78]
0037b4cc: b        #0x37b3c0
0037b4d0: ldr      r3, [pc, #0x60]
0037b4d4: ldr      r3, [r4, r3]
0037b4d8: ldr      r3, [r3]
0037b4dc: cmp      r3, #2
0037b4e0: streq    sl, [sl]
0037b4e4: beq      #0x37b3c8
0037b4e8: cmp      r3, #1
0037b4ec: bne      #0x37b3c8
0037b4f0: ldr      r0, [pc, #0x44]
0037b4f4: ldr      r1, [pc, #0x5c]
0037b4f8: ldr      r2, [pc, #0x5c]
0037b4fc: ldr      r0, [r4, r0]
0037b500: ldr      r3, [pc, #0x58]
0037b504: mov      ip, #0x61
0037b508: add      r1, pc, r1
0037b50c: add      r2, pc, r2
0037b510: add      r3, pc, r3
0037b514: add      r0, r0, #0xa8
0037b518: str      ip, [sp]
0037b51c: bl       #0x30e004
0037b520: b        #0x37b3c8
0037b524: bl       #0x30e310
0037b528: rsbeq    sb, r1, r4, asr #16
0037b52c: andeq    r4, r0, ip, lsr #1
0037b530: subseq   r6, r4, r0, ror r7
0037b534: subseq   r6, r4, r0, ror #14
0037b538: andeq    r3, r0, r0, asr #19
0037b53c: andeq    r1, r0, r0, asr #19
0037b540: subseq   r3, r4, r8, lsl #1
0037b544: subseq   r6, r4, r4, lsl #13
0037b548: subseq   r6, r4, r8, lsl #13
0037b54c: subseq   r6, r7, r4, lsl sp
0037b550: subseq   r6, r4, r8, lsr r6
0037b554: strdeq   r3, r4, [r0], -r4
0037b558: ldrsbeq  r2, [r4], #-0xe0
0037b55c: subseq   r6, r4, r4, lsr r5
0037b560: ldrsbeq  r6, [r4], #-0x40

# _ZN9LuaScript12BindFunctionEv
0037b5a0: push     {r4, r5, r6, lr}
0037b5a4: add      r4, r0, #4
0037b5a8: mov      r5, r0
0037b5ac: mov      r0, r4
0037b5b0: bl       #0x31b010
0037b5b4: mov      r0, r4
0037b5b8: bl       #0x31b000
0037b5bc: mov      r0, r4
0037b5c0: bl       #0x31aff8
0037b5c4: mov      r0, r4
0037b5c8: ldr      r4, [pc, #0x3a8]
0037b5cc: bl       #0x31b008
0037b5d0: ldr      r3, [pc, #0x3a4]
0037b5d4: ldr      r1, [pc, #0x3a4]
0037b5d8: add      r4, pc, r4
0037b5dc: add      r6, r5, #0x10
0037b5e0: ldr      r2, [r4, r3]
0037b5e4: mov      r0, r6
0037b5e8: mov      r3, r5
0037b5ec: add      r1, pc, r1
0037b5f0: bl       #0x31a4d4
0037b5f4: ldr      r3, [pc, #0x388]
0037b5f8: ldr      r1, [pc, #0x388]
0037b5fc: mov      r0, r6
0037b600: ldr      r2, [r4, r3]
0037b604: add      r1, pc, r1
0037b608: mov      r3, r5
0037b60c: bl       #0x31a4d4
0037b610: ldr      r3, [pc, #0x374]
0037b614: ldr      r1, [pc, #0x374]
0037b618: mov      r0, r6
0037b61c: ldr      r2, [r4, r3]
0037b620: add      r1, pc, r1
0037b624: mov      r3, r5
0037b628: bl       #0x31a4d4
0037b62c: ldr      r3, [pc, #0x360]
0037b630: ldr      r1, [pc, #0x360]
0037b634: mov      r0, r6
0037b638: ldr      r2, [r4, r3]
0037b63c: add      r1, pc, r1
0037b640: mov      r3, r5
0037b644: bl       #0x31a4d4
0037b648: ldr      r3, [pc, #0x34c]
0037b64c: ldr      r1, [pc, #0x34c]
0037b650: mov      r0, r6
0037b654: ldr      r2, [r4, r3]
0037b658: add      r1, pc, r1
0037b65c: mov      r3, r5
0037b660: bl       #0x31a4d4
0037b664: ldr      r3, [pc, #0x338]
0037b668: ldr      r1, [pc, #0x338]
0037b66c: mov      r0, r6
0037b670: ldr      r2, [r4, r3]
0037b674: add      r1, pc, r1
0037b678: mov      r3, r5
0037b67c: bl       #0x31a4d4
0037b680: ldr      r3, [pc, #0x324]
0037b684: ldr      r1, [pc, #0x324]
0037b688: mov      r0, r6
0037b68c: ldr      r2, [r4, r3]
0037b690: add      r1, pc, r1
0037b694: mov      r3, r5
0037b698: bl       #0x31a4d4
0037b69c: ldr      r3, [pc, #0x310]
0037b6a0: ldr      r1, [pc, #0x310]
0037b6a4: mov      r0, r6
0037b6a8: ldr      r2, [r4, r3]
0037b6ac: add      r1, pc, r1
0037b6b0: mov      r3, r5
0037b6b4: bl       #0x31a4d4
0037b6b8: ldr      r3, [pc, #0x2fc]
0037b6bc: ldr      r1, [pc, #0x2fc]
0037b6c0: mov      r0, r6
0037b6c4: ldr      r2, [r4, r3]
0037b6c8: add      r1, pc, r1
0037b6cc: mov      r3, r5
0037b6d0: bl       #0x31a4d4
0037b6d4: ldr      r3, [pc, #0x2e8]
0037b6d8: ldr      r1, [pc, #0x2e8]
0037b6dc: mov      r0, r6
0037b6e0: ldr      r2, [r4, r3]
0037b6e4: add      r1, pc, r1
0037b6e8: mov      r3, r5
0037b6ec: bl       #0x31a4d4
0037b6f0: ldr      r3, [pc, #0x2d4]
0037b6f4: ldr      r1, [pc, #0x2d4]
0037b6f8: mov      r0, r6
0037b6fc: ldr      r2, [r4, r3]
0037b700: add      r1, pc, r1
0037b704: mov      r3, r5
0037b708: bl       #0x31a4d4
0037b70c: ldr      r3, [pc, #0x2c0]
0037b710: ldr      r1, [pc, #0x2c0]
0037b714: mov      r0, r6
0037b718: ldr      r2, [r4, r3]
0037b71c: add      r1, pc, r1
0037b720: mov      r3, r5
0037b724: bl       #0x31a4d4
0037b728: ldr      r3, [pc, #0x2ac]
0037b72c: ldr      r1, [pc, #0x2ac]
0037b730: mov      r0, r6
0037b734: ldr      r2, [r4, r3]
0037b738: add      r1, pc, r1
0037b73c: mov      r3, r5
0037b740: bl       #0x31a4d4
0037b744: ldr      r3, [pc, #0x298]
0037b748: ldr      r1, [pc, #0x298]
0037b74c: mov      r0, r6
0037b750: ldr      r2, [r4, r3]
0037b754: add      r1, pc, r1
0037b758: mov      r3, r5
0037b75c: bl       #0x31a4d4
0037b760: ldr      r3, [pc, #0x284]
0037b764: ldr      r1, [pc, #0x284]
0037b768: mov      r0, r6
0037b76c: ldr      r2, [r4, r3]
0037b770: add      r1, pc, r1
0037b774: mov      r3, r5
0037b778: bl       #0x31a4d4
0037b77c: ldr      r3, [pc, #0x270]
0037b780: ldr      r1, [pc, #0x270]
0037b784: mov      r0, r6
0037b788: ldr      r2, [r4, r3]
0037b78c: add      r1, pc, r1
0037b790: mov      r3, r5
0037b794: bl       #0x31a4d4
0037b798: ldr      r3, [pc, #0x25c]
0037b79c: ldr      r1, [pc, #0x25c]
0037b7a0: mov      r0, r6
0037b7a4: ldr      r2, [r4, r3]
0037b7a8: add      r1, pc, r1
0037b7ac: mov      r3, r5
0037b7b0: bl       #0x31a4d4
0037b7b4: ldr      r3, [pc, #0x248]
0037b7b8: ldr      r1, [pc, #0x248]
0037b7bc: mov      r0, r6
0037b7c0: ldr      r2, [r4, r3]
0037b7c4: add      r1, pc, r1
0037b7c8: mov      r3, r5
0037b7cc: bl       #0x31a4d4
0037b7d0: ldr      r3, [pc, #0x234]
0037b7d4: ldr      r1, [pc, #0x234]
0037b7d8: mov      r0, r6
0037b7dc: ldr      r2, [r4, r3]
0037b7e0: add      r1, pc, r1
0037b7e4: mov      r3, r5
0037b7e8: bl       #0x31a4d4
0037b7ec: ldr      r3, [pc, #0x220]
0037b7f0: ldr      r1, [pc, #0x220]
0037b7f4: mov      r0, r6
0037b7f8: ldr      r2, [r4, r3]
0037b7fc: add      r1, pc, r1
0037b800: mov      r3, r5
0037b804: bl       #0x31a4d4
0037b808: ldr      r3, [pc, #0x20c]
0037b80c: ldr      r1, [pc, #0x20c]
0037b810: mov      r0, r6
0037b814: ldr      r2, [r4, r3]
0037b818: add      r1, pc, r1
0037b81c: mov      r3, r5
0037b820: bl       #0x31a4d4
0037b824: ldr      r3, [pc, #0x1f8]
0037b828: ldr      r1, [pc, #0x1f8]
0037b82c: mov      r0, r6
0037b830: ldr      r2, [r4, r3]
0037b834: add      r1, pc, r1
0037b838: mov      r3, r5
0037b83c: bl       #0x31a4d4
0037b840: ldr      r3, [pc, #0x1e4]
0037b844: ldr      r1, [pc, #0x1e4]
0037b848: mov      r0, r6
0037b84c: ldr      r2, [r4, r3]
0037b850: add      r1, pc, r1
0037b854: mov      r3, r5
0037b858: bl       #0x31a4d4
0037b85c: ldr      r3, [pc, #0x1d0]
0037b860: ldr      r1, [pc, #0x1d0]
0037b864: mov      r0, r6
0037b868: ldr      r2, [r4, r3]
0037b86c: add      r1, pc, r1
0037b870: mov      r3, r5
0037b874: bl       #0x31a4d4
0037b878: ldr      r3, [pc, #0x1bc]
0037b87c: ldr      r1, [pc, #0x1bc]
0037b880: mov      r0, r6
0037b884: ldr      r2, [r4, r3]
0037b888: add      r1, pc, r1
0037b88c: mov      r3, r5
0037b890: bl       #0x31a4d4
0037b894: ldr      r3, [pc, #0x1a8]
0037b898: ldr      r1, [pc, #0x1a8]
0037b89c: mov      r0, r6
0037b8a0: ldr      r2, [r4, r3]
0037b8a4: add      r1, pc, r1
0037b8a8: mov      r3, r5
0037b8ac: bl       #0x31a4d4
0037b8b0: ldr      r3, [pc, #0x194]
0037b8b4: ldr      r1, [pc, #0x194]
0037b8b8: mov      r0, r6
0037b8bc: ldr      r2, [r4, r3]
0037b8c0: add      r1, pc, r1
0037b8c4: mov      r3, r5
0037b8c8: bl       #0x31a4d4
0037b8cc: ldr      r3, [pc, #0x180]
0037b8d0: ldr      r1, [pc, #0x180]
0037b8d4: mov      r0, r6
0037b8d8: ldr      r2, [r4, r3]
0037b8dc: add      r1, pc, r1
0037b8e0: mov      r3, r5
0037b8e4: bl       #0x31a4d4
0037b8e8: ldr      r3, [pc, #0x16c]
0037b8ec: ldr      r1, [pc, #0x16c]
0037b8f0: mov      r0, r6
0037b8f4: ldr      r2, [r4, r3]
0037b8f8: add      r1, pc, r1
0037b8fc: mov      r3, r5
0037b900: bl       #0x31a4d4
0037b904: ldr      r3, [pc, #0x158]
0037b908: ldr      r1, [pc, #0x158]
0037b90c: mov      r0, r6
0037b910: ldr      r2, [r4, r3]
0037b914: add      r1, pc, r1
0037b918: mov      r3, r5
0037b91c: bl       #0x31a4d4
0037b920: ldr      r3, [pc, #0x144]
0037b924: ldr      r1, [pc, #0x144]
0037b928: mov      r0, r6
0037b92c: ldr      r2, [r4, r3]
0037b930: add      r1, pc, r1
0037b934: mov      r3, r5
0037b938: bl       #0x31a4d4
0037b93c: ldr      r3, [pc, #0x130]
0037b940: ldr      r1, [pc, #0x130]
0037b944: mov      r0, r6
0037b948: ldr      r2, [r4, r3]
0037b94c: add      r1, pc, r1
0037b950: mov      r3, r5
0037b954: bl       #0x31a4d4
0037b958: ldr      r3, [pc, #0x11c]
0037b95c: ldr      r1, [pc, #0x11c]
0037b960: mov      r0, r6
0037b964: ldr      r2, [r4, r3]
0037b968: add      r1, pc, r1
0037b96c: mov      r3, r5
0037b970: pop      {r4, r5, r6, lr}
0037b974: b        #0x31a4d4
0037b978: strhteq  sb, [r1], #-0x48
0037b97c: andeq    r3, r0, r0, asr ip
0037b980: subseq   r6, r4, ip, asr r4
0037b984: andeq    r3, r0, r0, asr #20
0037b988: subseq   r6, r4, ip, asr #8
0037b98c: andeq    r1, r0, r4, lsr r7
0037b990: subseq   r6, r4, r8, lsr r4
0037b994: andeq    r0, r0, r0, ror #29
0037b998: subseq   r6, r4, r4, lsr #8
0037b99c: muleq    r0, ip, fp
0037b9a0: subseq   r6, r4, r0, lsl r4
0037b9a4: muleq    r0, r8, ip
0037b9a8: subseq   r6, r4, r4, lsl #8
0037b9ac: muleq    r0, ip, r5
0037b9b0: ldrsheq  r6, [r4], #-0x38
0037b9b4: andeq    r3, r0, r8, ror #15
0037b9b8: subseq   r6, r4, ip, ror #7
0037b9bc: andeq    r4, r0, r4, asr #23
0037b9c0: ldrsbeq  r6, [r4], #-0x38
0037b9c4: muleq    r0, r0, r8
0037b9c8: subseq   r6, r4, ip, asr #7
0037b9cc: andeq    r1, r0, r4, ror #7
0037b9d0: subseq   r6, r4, r0, asr #7
0037b9d4: strdeq   r4, r5, [r0], -r4
0037b9d8: ldrheq   r6, [r4], #-0x34
0037b9dc: andeq    r4, r0, r0, lsl #6
0037b9e0: subseq   r6, r4, r0, lsr #7
0037b9e4: andeq    r3, r0, ip, asr #10
0037b9e8: subseq   r6, r4, ip, lsl #7
0037b9ec: andeq    r0, r0, r8, lsr r7
0037b9f0: subseq   r6, r4, r8, ror r3
0037b9f4: andeq    r4, r0, r4, lsr r6
0037b9f8: subseq   r6, r4, r4, ror #6
0037b9fc: muleq    r0, r0, r0
0037ba00: subseq   r6, r4, r0, asr r3
0037ba04: andeq    r1, r0, ip, lsl #13
0037ba08: subseq   r6, r4, ip, lsr r3
0037ba0c: andeq    r1, r0, ip, lsl #9
0037ba10: subseq   r6, r4, r0, lsr r3
0037ba14: andeq    r2, r0, ip, lsr #29
0037ba18: subseq   r6, r4, r4, lsr #6
0037ba1c: andeq    r2, r0, ip, lsl #5
0037ba20: subseq   r6, r4, r8, lsl r3
0037ba24: ldrdeq   r4, r5, [r0], -ip
0037ba28: subseq   r6, r4, ip, lsl #6
0037ba2c: muleq    r0, r8, pc
0037ba30: subseq   r6, r4, r0, lsl #6
0037ba34: andeq    r1, r0, r0, asr #21
0037ba38: ldrsheq  r6, [r4], #-0x24
0037ba3c: andeq    r3, r0, r8, lsr #11
0037ba40: ldrsheq  r6, [r4], #-0x20
0037ba44: andeq    r4, r0, r0, asr r0
0037ba48: subseq   r6, r4, ip, ror #5
0037ba4c: strdeq   r3, r4, [r0], -r4
0037ba50: subseq   r6, r4, r8, ror #5
0037ba54: andeq    r2, r0, ip, lsl #8
0037ba58: subseq   r6, r4, r4, ror #5
0037ba5c: andeq    r1, r0, ip, ror #15
0037ba60: ldrsbeq  r6, [r4], #-0x28
0037ba64: muleq    r0, r8, sb
0037ba68: subseq   r6, r4, ip, asr #5
0037ba6c: andeq    r3, r0, r8, ror pc
0037ba70: subseq   r6, r4, r0, asr #5
0037ba74: andeq    r2, r0, ip, lsr #13
0037ba78: ldrheq   r6, [r4], #-0x24
0037ba7c: andeq    r2, r0, r8, lsl #30
0037ba80: subseq   r6, r4, r8, lsr #5

# _ZN9LuaScript4LoadEPKc
0037b574: ldr      r3, [pc, #0x1c]
0037b578: ldr      r2, [pc, #0x1c]
0037b57c: mov      ip, r0
0037b580: add      r3, pc, r3
0037b584: ldr      r0, [r3, r2]
0037b588: mov      r2, r1
0037b58c: mov      r1, ip
0037b590: ldr      r0, [r0, #0x3c]
0037b594: b        #0x37b23c
0037b598: rsbeq    sb, r1, r0, lsl r5
0037b59c: strdeq   r3, r4, [r0], -r4
