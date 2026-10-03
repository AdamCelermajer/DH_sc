
# _ZN10LuaManagerD1Ev
0037a0ac: ldr      r3, [pc, #0x54]
0037a0b0: ldr      r2, [pc, #0x54]
0037a0b4: push     {r4, r5, r6, lr}
0037a0b8: add      r3, pc, r3
0037a0bc: ldr      r2, [r3, r2]
0037a0c0: mov      r4, r0
0037a0c4: add      r2, r2, #8
0037a0c8: str      r2, [r0]
0037a0cc: bl       #0x379fe8
0037a0d0: ldr      r3, [r4, #0x14]
0037a0d4: cmp      r3, #0
0037a0d8: beq      #0x37a100
0037a0dc: add      r5, r4, #4
0037a0e0: mov      r0, r5
0037a0e4: ldr      r1, [r4, #8]
0037a0e8: bl       #0x379fa8
0037a0ec: mov      r3, #0
0037a0f0: str      r5, [r4, #0x10]
0037a0f4: str      r3, [r4, #0x14]
0037a0f8: str      r5, [r4, #0xc]
0037a0fc: str      r3, [r4, #8]
0037a100: mov      r0, r4
0037a104: pop      {r4, r5, r6, pc}

# _ZNSt4priv8_Rb_treeISsSt4lessISsESsNS_9_IdentityISsEENS_11_SetTraitsTISsEESaISsEE13insert_uniqueERKSs
0037a9e8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037a9ec: ldr      r5, [r1, #4]
0037a9f0: sub      sp, sp, #0x14
0037a9f4: mov      sb, r1
0037a9f8: cmp      r5, #0
0037a9fc: mov      r4, r0
0037aa00: mov      r8, r2
0037aa04: beq      #0x37aaec
0037aa08: ldr      r7, [r2, #0x14]
0037aa0c: ldr      fp, [r2, #0x10]
0037aa10: rsb      sl, r7, fp
0037aa14: b        #0x37aa2c
0037aa18: ldr      r3, [r5, #8]
0037aa1c: mov      r1, #1
0037aa20: cmp      r3, #0
0037aa24: beq      #0x37aa84
0037aa28: mov      r5, r3
0037aa2c: ldr      r3, [r5, #0x24]
0037aa30: ldr      r6, [r5, #0x20]
0037aa34: mov      r0, r7
0037aa38: mov      r1, r3
0037aa3c: rsb      r6, r3, r6
0037aa40: cmp      r6, sl
0037aa44: movlt    r2, r6
0037aa48: movge    r2, sl
0037aa4c: bl       #0x30e5e0
0037aa50: cmp      r0, #0
0037aa54: mov      r2, r5
0037aa58: bne      #0x37aa6c
0037aa5c: cmp      sl, r6
0037aa60: blt      #0x37aa18
0037aa64: movle    r0, #0
0037aa68: movgt    r0, #1
0037aa6c: cmp      r0, #0
0037aa70: blt      #0x37aa18
0037aa74: ldr      r3, [r5, #0xc]
0037aa78: mov      r1, #0
0037aa7c: cmp      r3, #0
0037aa80: bne      #0x37aa28
0037aa84: cmp      r1, #0
0037aa88: moveq    sl, r5
0037aa8c: bne      #0x37aaf0
0037aa90: ldr      r0, [r2, #0x24]
0037aa94: ldr      r6, [r2, #0x20]
0037aa98: rsb      fp, r7, fp
0037aa9c: mov      r1, r7
0037aaa0: rsb      r6, r0, r6
0037aaa4: cmp      fp, r6
0037aaa8: movlt    r2, fp
0037aaac: movge    r2, r6
0037aab0: bl       #0x30e5e0
0037aab4: cmp      r0, #0
0037aab8: bne      #0x37aacc
0037aabc: cmp      r6, fp
0037aac0: blt      #0x37ab48
0037aac4: movle    r0, #0
0037aac8: movgt    r0, #1
0037aacc: cmp      r0, #0
0037aad0: movge    r3, #0
0037aad4: strge    sl, [r4]
0037aad8: strbge   r3, [r4, #4]
0037aadc: blt      #0x37ab48
0037aae0: mov      r0, r4
0037aae4: add      sp, sp, #0x14
0037aae8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037aaec: mov      r5, r1
0037aaf0: ldr      r3, [sb, #8]
0037aaf4: cmp      r5, r3
0037aaf8: beq      #0x37abc4
0037aafc: ldrb     r3, [r5]
0037ab00: cmp      r3, #0
0037ab04: bne      #0x37ab18
0037ab08: ldr      r3, [r5, #4]
0037ab0c: ldr      r3, [r3, #4]
0037ab10: cmp      r5, r3
0037ab14: beq      #0x37abb0
0037ab18: ldr      r2, [r5, #8]
0037ab1c: cmp      r2, #0
0037ab20: bne      #0x37ab2c
0037ab24: b        #0x37ab78
0037ab28: mov      r2, r3
0037ab2c: ldr      r3, [r2, #0xc]
0037ab30: cmp      r3, #0
0037ab34: bne      #0x37ab28
0037ab38: mov      sl, r2
0037ab3c: ldr      r7, [r8, #0x14]
0037ab40: ldr      fp, [r8, #0x10]
0037ab44: b        #0x37aa90
0037ab48: mov      r2, r5
0037ab4c: mov      r3, r8
0037ab50: mov      ip, #0
0037ab54: mov      r1, sb
0037ab58: add      r0, sp, #8
0037ab5c: str      ip, [sp]
0037ab60: bl       #0x37a910
0037ab64: ldr      r3, [sp, #8]
0037ab68: mov      r2, #1
0037ab6c: strb     r2, [r4, #4]
0037ab70: str      r3, [r4]
0037ab74: b        #0x37aae0
0037ab78: ldr      r3, [r5, #4]
0037ab7c: ldr      r2, [r3, #8]
0037ab80: cmp      r5, r2
0037ab84: beq      #0x37ab90
0037ab88: b        #0x37abf0
0037ab8c: mov      r3, r2
0037ab90: ldr      r2, [r3, #4]
0037ab94: ldr      r1, [r2, #8]
0037ab98: cmp      r1, r3
0037ab9c: beq      #0x37ab8c
0037aba0: ldr      r7, [r8, #0x14]
0037aba4: ldr      fp, [r8, #0x10]
0037aba8: mov      sl, r2
0037abac: b        #0x37aa90
0037abb0: ldr      r2, [r5, #0xc]
0037abb4: ldr      r7, [r8, #0x14]
0037abb8: ldr      fp, [r8, #0x10]
0037abbc: mov      sl, r2
0037abc0: b        #0x37aa90
0037abc4: mov      r2, r5
0037abc8: mov      r3, r8
0037abcc: mov      r1, sb
0037abd0: add      r0, sp, #0xc
0037abd4: str      r5, [sp]
0037abd8: bl       #0x37a910
0037abdc: ldr      r3, [sp, #0xc]
0037abe0: mov      r2, #1
0037abe4: strb     r2, [r4, #4]
0037abe8: str      r3, [r4]
0037abec: b        #0x37aae0
0037abf0: mov      r2, r3
0037abf4: b        #0x37ab38

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESsNS_9_IdentityISsEENS_11_SetTraitsTISsEESaISsEE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
0037a190: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037a194: ldr      fp, [pc, #0x15c]
0037a198: ldr      r2, [pc, #0x15c]
0037a19c: sub      sp, sp, #0x54
0037a1a0: add      fp, pc, fp
0037a1a4: ldr      r3, [fp, r2]
0037a1a8: str      r2, [sp, #0xc]
0037a1ac: str      r0, [sp, #8]
0037a1b0: ldr      r4, [r0, #4]
0037a1b4: ldr      r3, [r3]
0037a1b8: mov      r8, r1
0037a1bc: cmp      r4, #0
0037a1c0: str      r3, [sp, #0x4c]
0037a1c4: beq      #0x37a2cc
0037a1c8: mov      sl, r0
0037a1cc: add      r7, sp, #0x34
0037a1d0: add      sb, sp, #0x18
0037a1d4: ldr      r1, [r8]
0037a1d8: mov      r2, sb
0037a1dc: mov      r0, r7
0037a1e0: bl       #0x3140ec
0037a1e4: ldr      r3, [r4, #0x24]
0037a1e8: ldr      r1, [sp, #0x48]
0037a1ec: ldr      r6, [r4, #0x20]
0037a1f0: ldr      r5, [sp, #0x44]
0037a1f4: mov      r0, r3
0037a1f8: rsb      r6, r3, r6
0037a1fc: rsb      r5, r1, r5
0037a200: cmp      r5, r6
0037a204: movlt    r2, r5
0037a208: movge    r2, r6
0037a20c: bl       #0x30e5e0
0037a210: subs     r3, r0, #0
0037a214: bne      #0x37a22c
0037a218: cmp      r6, r5
0037a21c: mvnlt    r3, #0
0037a220: blt      #0x37a22c
0037a224: movle    r3, #0
0037a228: movgt    r3, #1
0037a22c: mov      r0, r7
0037a230: str      r3, [sp, #4]
0037a234: bl       #0x3139ac
0037a238: ldr      r3, [sp, #4]
0037a23c: cmp      r3, #0
0037a240: movge    sl, r4
0037a244: ldrlt    r4, [r4, #0xc]
0037a248: ldrge    r4, [r4, #8]
0037a24c: cmp      r4, #0
0037a250: bne      #0x37a1d4
0037a254: ldr      r3, [sp, #8]
0037a258: cmp      sl, r3
0037a25c: beq      #0x37a2d0
0037a260: add      r4, sp, #0x1c
0037a264: ldr      r1, [r8]
0037a268: add      r2, sp, #0x14
0037a26c: mov      r0, r4
0037a270: bl       #0x3140ec
0037a274: ldr      r3, [sp, #0x30]
0037a278: ldr      r1, [sl, #0x24]
0037a27c: ldr      r5, [sl, #0x20]
0037a280: ldr      r6, [sp, #0x2c]
0037a284: mov      r0, r3
0037a288: rsb      r5, r1, r5
0037a28c: rsb      r6, r3, r6
0037a290: cmp      r5, r6
0037a294: movlt    r2, r5
0037a298: movge    r2, r6
0037a29c: bl       #0x30e5e0
0037a2a0: subs     r7, r0, #0
0037a2a4: bne      #0x37a2bc
0037a2a8: cmp      r6, r5
0037a2ac: mvnlt    r7, #0
0037a2b0: blt      #0x37a2bc
0037a2b4: movle    r7, #0
0037a2b8: movgt    r7, #1
0037a2bc: mov      r0, r4
0037a2c0: bl       #0x3139ac
0037a2c4: cmp      r7, #0
0037a2c8: bge      #0x37a2d0
0037a2cc: ldr      sl, [sp, #8]
0037a2d0: ldr      r2, [sp, #0xc]
0037a2d4: mov      r0, sl
0037a2d8: ldr      r3, [fp, r2]
0037a2dc: ldr      r2, [sp, #0x4c]
0037a2e0: ldr      r3, [r3]
0037a2e4: cmp      r2, r3
0037a2e8: bne      #0x37a2f4
0037a2ec: add      sp, sp, #0x54
0037a2f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037a2f4: bl       #0x30e310

# _ZN10LuaManagerD2Ev
0037a12c: ldr      r3, [pc, #0x54]
0037a130: ldr      r2, [pc, #0x54]
0037a134: push     {r4, r5, r6, lr}
0037a138: add      r3, pc, r3
0037a13c: ldr      r2, [r3, r2]
0037a140: mov      r4, r0
0037a144: add      r2, r2, #8
0037a148: str      r2, [r0]
0037a14c: bl       #0x379fe8
0037a150: ldr      r3, [r4, #0x14]
0037a154: cmp      r3, #0
0037a158: beq      #0x37a180
0037a15c: add      r5, r4, #4
0037a160: mov      r0, r5
0037a164: ldr      r1, [r4, #8]
0037a168: bl       #0x379fa8
0037a16c: mov      r3, #0
0037a170: str      r5, [r4, #0x10]
0037a174: str      r3, [r4, #0x14]
0037a178: str      r5, [r4, #0xc]
0037a17c: str      r3, [r4, #8]
0037a180: mov      r0, r4
0037a184: pop      {r4, r5, r6, pc}
0037a188: rsbeq    sl, r1, r8, asr sb
0037a18c: andeq    r3, r0, r8, ror #26

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsP12StreamBufferENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
0037a300: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037a304: ldr      fp, [pc, #0x15c]
0037a308: ldr      r2, [pc, #0x15c]
0037a30c: sub      sp, sp, #0x54
0037a310: add      fp, pc, fp
0037a314: ldr      r3, [fp, r2]
0037a318: str      r2, [sp, #0xc]
0037a31c: str      r0, [sp, #8]
0037a320: ldr      r4, [r0, #4]
0037a324: ldr      r3, [r3]
0037a328: mov      r8, r1
0037a32c: cmp      r4, #0
0037a330: str      r3, [sp, #0x4c]
0037a334: beq      #0x37a43c
0037a338: mov      sl, r0
0037a33c: add      r7, sp, #0x34
0037a340: add      sb, sp, #0x18
0037a344: ldr      r1, [r8]
0037a348: mov      r2, sb
0037a34c: mov      r0, r7
0037a350: bl       #0x3140ec
0037a354: ldr      r3, [r4, #0x24]
0037a358: ldr      r1, [sp, #0x48]
0037a35c: ldr      r6, [r4, #0x20]
0037a360: ldr      r5, [sp, #0x44]
0037a364: mov      r0, r3
0037a368: rsb      r6, r3, r6
0037a36c: rsb      r5, r1, r5
0037a370: cmp      r5, r6
0037a374: movlt    r2, r5
0037a378: movge    r2, r6
0037a37c: bl       #0x30e5e0
0037a380: subs     r3, r0, #0
0037a384: bne      #0x37a39c
0037a388: cmp      r6, r5
0037a38c: mvnlt    r3, #0
0037a390: blt      #0x37a39c
0037a394: movle    r3, #0
0037a398: movgt    r3, #1
0037a39c: mov      r0, r7
0037a3a0: str      r3, [sp, #4]
0037a3a4: bl       #0x3139ac
0037a3a8: ldr      r3, [sp, #4]
0037a3ac: cmp      r3, #0
0037a3b0: movge    sl, r4
0037a3b4: ldrlt    r4, [r4, #0xc]
0037a3b8: ldrge    r4, [r4, #8]
0037a3bc: cmp      r4, #0
0037a3c0: bne      #0x37a344
0037a3c4: ldr      r3, [sp, #8]
0037a3c8: cmp      sl, r3
0037a3cc: beq      #0x37a440
0037a3d0: add      r4, sp, #0x1c
0037a3d4: ldr      r1, [r8]
0037a3d8: add      r2, sp, #0x14
0037a3dc: mov      r0, r4
0037a3e0: bl       #0x3140ec
0037a3e4: ldr      r3, [sp, #0x30]
0037a3e8: ldr      r1, [sl, #0x24]
0037a3ec: ldr      r5, [sl, #0x20]
0037a3f0: ldr      r6, [sp, #0x2c]
0037a3f4: mov      r0, r3
0037a3f8: rsb      r5, r1, r5
0037a3fc: rsb      r6, r3, r6
0037a400: cmp      r5, r6
0037a404: movlt    r2, r5
0037a408: movge    r2, r6
0037a40c: bl       #0x30e5e0
0037a410: subs     r7, r0, #0
0037a414: bne      #0x37a42c
0037a418: cmp      r6, r5
0037a41c: mvnlt    r7, #0
0037a420: blt      #0x37a42c
0037a424: movle    r7, #0
0037a428: movgt    r7, #1
0037a42c: mov      r0, r4
0037a430: bl       #0x3139ac
0037a434: cmp      r7, #0
0037a438: bge      #0x37a440
0037a43c: ldr      sl, [sp, #8]
0037a440: ldr      r2, [sp, #0xc]
0037a444: mov      r0, sl
0037a448: ldr      r3, [fp, r2]
0037a44c: ldr      r2, [sp, #0x4c]
0037a450: ldr      r3, [r3]
0037a454: cmp      r2, r3
0037a458: bne      #0x37a464
0037a45c: add      sp, sp, #0x54
0037a460: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037a464: bl       #0x30e310
0037a468: rsbeq    sl, r1, r0, lsl #15
0037a46c: andeq    r4, r0, ip, lsr #1

# _ZN10LuaManagerD0Ev
0037a110: push     {r4, lr}
0037a114: mov      r4, r0
0037a118: bl       #0x37a0ac
0037a11c: mov      r0, r4
0037a120: bl       #0x310440
0037a124: mov      r0, r4
0037a128: pop      {r4, pc}

# _ZN9LuaScript8_IncludeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037efe4: push     {r4, lr}
0037efe8: ldr      r3, [r0, #4]
0037efec: mov      r4, r2
0037eff0: ldm      r3, {r0, r2}
0037eff4: rsb      r3, r0, r2
0037eff8: asr      r3, r3, #4
0037effc: add      r2, r3, r3, lsl #3
0037f000: add      r2, r2, r2, lsl #6
0037f004: add      r2, r3, r2, lsl #3
0037f008: add      r2, r2, r2, lsl #15
0037f00c: add      r3, r3, r2, lsl #3
0037f010: cmp      r3, #0
0037f014: bne      #0x37f01c
0037f018: pop      {r4, pc}
0037f01c: ldr      r3, [r0, #4]
0037f020: cmp      r3, #4
0037f024: bne      #0x37f018
0037f028: bl       #0x31c49c
0037f02c: mov      r1, r0
0037f030: mov      r0, r4
0037f034: pop      {r4, lr}
0037f038: b        #0x37b574

# _ZNSt3mapISsP12StreamBufferSt4lessISsESaISt4pairIKSsS1_EEEixIPKcEERS1_RKT_
0037b0fc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b100: ldr      r4, [pc, #0x12c]
0037b104: ldr      r7, [pc, #0x12c]
0037b108: sub      sp, sp, #0x6c
0037b10c: add      r4, pc, r4
0037b110: ldr      r3, [r4, r7]
0037b114: mov      r8, r0
0037b118: mov      r6, r1
0037b11c: ldr      r3, [r3]
0037b120: str      r3, [sp, #0x64]
0037b124: bl       #0x37a470
0037b128: cmp      r0, r8
0037b12c: mov      r5, r0
0037b130: beq      #0x37b1c4
0037b134: add      sl, sp, #0x4c
0037b138: ldr      r1, [r6]
0037b13c: add      r2, sp, #0x14
0037b140: mov      r0, sl
0037b144: bl       #0x3140ec
0037b148: ldr      r3, [sp, #0x60]
0037b14c: ldr      r1, [r5, #0x24]
0037b150: ldr      fp, [r5, #0x20]
0037b154: ldr      sb, [sp, #0x5c]
0037b158: mov      r0, r3
0037b15c: rsb      fp, r1, fp
0037b160: rsb      sb, r3, sb
0037b164: cmp      fp, sb
0037b168: movlt    r2, fp
0037b16c: movge    r2, sb
0037b170: bl       #0x30e5e0
0037b174: cmp      r0, #0
0037b178: mov      r3, r5
0037b17c: bne      #0x37b1b8
0037b180: cmp      sb, fp
0037b184: blt      #0x37b1bc
0037b188: mov      r0, sl
0037b18c: str      r3, [sp, #4]
0037b190: bl       #0x3139ac
0037b194: ldr      r3, [sp, #4]
0037b198: ldr      r1, [r4, r7]
0037b19c: ldr      r2, [sp, #0x64]
0037b1a0: add      r0, r3, #0x28
0037b1a4: ldr      r3, [r1]
0037b1a8: cmp      r2, r3
0037b1ac: bne      #0x37b230
0037b1b0: add      sp, sp, #0x6c
0037b1b4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b1b8: bge      #0x37b188
0037b1bc: mov      r0, sl
0037b1c0: bl       #0x3139ac
0037b1c4: add      sl, sp, #0x34
0037b1c8: ldr      r1, [r6]
0037b1cc: add      r2, sp, #0x10
0037b1d0: add      r6, sp, #0x18
0037b1d4: mov      r0, sl
0037b1d8: bl       #0x3140ec
0037b1dc: mov      r0, r6
0037b1e0: ldr      r1, [sp, #0x48]
0037b1e4: ldr      r2, [sp, #0x44]
0037b1e8: str      r6, [sp, #0x28]
0037b1ec: str      r6, [sp, #0x2c]
0037b1f0: bl       #0x3116e8
0037b1f4: mov      r3, r6
0037b1f8: mov      ip, #0
0037b1fc: mov      r1, r8
0037b200: add      r2, sp, #8
0037b204: add      r0, sp, #0xc
0037b208: str      ip, [sp, #0x30]
0037b20c: str      r5, [sp, #8]
0037b210: bl       #0x37abf8
0037b214: ldr      r5, [sp, #0xc]
0037b218: mov      r0, r6
0037b21c: bl       #0x3139ac
0037b220: mov      r0, sl
0037b224: bl       #0x3139ac
0037b228: mov      r3, r5
0037b22c: b        #0x37b198
0037b230: bl       #0x30e310
0037b234: rsbeq    sb, r1, r4, lsl #19
0037b238: andeq    r4, r0, ip, lsr #1
