
# _ZN7gameswf19shape_character_def16point_test_localEff
0077a570: push     {r4, r5, r6, r7, r8, lr}
0077a574: mov      r6, r0
0077a578: mov      r7, r1
0077a57c: mov      r0, r1
0077a580: ldr      r1, [r6, #0x54]
0077a584: mov      r8, r2
0077a588: bl       #0x30e70c
0077a58c: cmp      r0, #0
0077a590: bne      #0x77a5a8
0077a594: mov      r0, r7
0077a598: ldr      r1, [r6, #0x58]
0077a59c: bl       #0x30e2f8
0077a5a0: cmp      r0, #0
0077a5a4: beq      #0x77a5b0
0077a5a8: mov      r0, #0
0077a5ac: pop      {r4, r5, r6, r7, r8, pc}
0077a5b0: mov      r0, r8
0077a5b4: ldr      r1, [r6, #0x5c]
0077a5b8: bl       #0x30e70c
0077a5bc: cmp      r0, #0
0077a5c0: bne      #0x77a5a8
0077a5c4: mov      r0, r8
0077a5c8: ldr      r1, [r6, #0x60]
0077a5cc: bl       #0x30e2f8
0077a5d0: cmp      r0, #0
0077a5d4: bne      #0x77a5a8
0077a5d8: ldr      r3, [r6, #0x48]
0077a5dc: cmp      r3, #0
0077a5e0: ble      #0x77a5a8
0077a5e4: mov      r4, #0
0077a5e8: mov      r5, r4
0077a5ec: b        #0x77a600
0077a5f0: ldr      r3, [r6, #0x48]
0077a5f4: add      r4, r4, #0x28
0077a5f8: cmp      r5, r3
0077a5fc: bge      #0x77a5a8
0077a600: ldr      r0, [r6, #0x44]
0077a604: mov      r1, r7
0077a608: mov      r2, r8
0077a60c: add      r0, r0, r4
0077a610: bl       #0x779ff4
0077a614: cmp      r0, #0
0077a618: add      r5, r5, #1
0077a61c: beq      #0x77a5f0
0077a620: mov      r0, #1
0077a624: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7gameswf14place_object_24readEPNS_6playerEPNS_6streamEiiPNS_20movie_definition_subE
0075cb6c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0075cb70: ldr      r4, [pc, #0xd08]
0075cb74: ldr      ip, [pc, #0xd08]
0075cb78: sub      sp, sp, #0x104
0075cb7c: add      r4, pc, r4
0075cb80: str      ip, [sp, #0x24]
0075cb84: ldr      ip, [r4, ip]
0075cb88: add      sl, sp, #0x70
0075cb8c: add      lr, sl, #8
0075cb90: ldr      r5, [ip]
0075cb94: str      r4, [sp, #0x1c]
0075cb98: mov      r4, #0
0075cb9c: str      r5, [sp, #0xfc]
0075cba0: str      r4, [lr], #4
0075cba4: str      r4, [lr], #4
0075cba8: str      r4, [lr], #4
0075cbac: str      r4, [lr]
0075cbb0: ldr      r7, [sp, #0xf8]
0075cbb4: mov      r5, r2
0075cbb8: mvn      r2, #0
0075cbbc: bfi      r7, r2, #0, #0x18
0075cbc0: lsr      r2, r7, #0x18
0075cbc4: mov      ip, #0x3f800000
0075cbc8: mov      lr, #0
0075cbcc: bfi      r2, r4, #0, #1
0075cbd0: mov      r6, #1
0075cbd4: cmp      r5, #4
0075cbd8: str      r7, [sp, #0xf8]
0075cbdc: str      ip, [sp, #0x68]
0075cbe0: str      lr, [sp, #0x6c]
0075cbe4: strb     r2, [sp, #0xfb]
0075cbe8: str      r0, [sp, #0x20]
0075cbec: str      r1, [sp, #0xc]
0075cbf0: str      r3, [sp, #0x30]
0075cbf4: str      r4, [sp, #0x74]
0075cbf8: str      ip, [sp, #0x70]
0075cbfc: str      ip, [sp, #0x80]
0075cc00: str      ip, [sp, #0x50]
0075cc04: str      ip, [sp, #0x58]
0075cc08: str      ip, [sp, #0x60]
0075cc0c: str      lr, [sp, #0x54]
0075cc10: str      lr, [sp, #0x5c]
0075cc14: str      lr, [sp, #0x64]
0075cc18: str      r4, [sp, #0x88]
0075cc1c: str      r4, [sp, #0x8c]
0075cc20: str      r4, [sp, #0x90]
0075cc24: str      r4, [sp, #0x94]
0075cc28: strb     r4, [sp, #0x98]
0075cc2c: strb     r6, [sp, #0xe8]
0075cc30: strb     r4, [sp, #0xe9]
0075cc34: ldr      r7, [sp, #0x128]
0075cc38: bne      #0x75ce10
0075cc3c: mov      r0, r1
0075cc40: bl       #0x783c14
0075cc44: mov      sb, r0
0075cc48: ldr      r0, [sp, #0xc]
0075cc4c: bl       #0x783c14
0075cc50: ldr      r1, [sp, #0xc]
0075cc54: mov      r5, r0
0075cc58: mov      r0, sl
0075cc5c: bl       #0x7965d4
0075cc60: add      r6, r7, #0x34
0075cc64: add      r1, sp, #0x100
0075cc68: str      r5, [r1, #-0x1c]!
0075cc6c: mov      r0, r6
0075cc70: bl       #0x759e68
0075cc74: cmp      r0, #0
0075cc78: ldrge    r3, [r7, #0x34]
0075cc7c: add      r1, sp, #0x100
0075cc80: movlt    fp, r4
0075cc84: addge    r0, r3, r0, lsl #4
0075cc88: add      r4, r7, #0x30
0075cc8c: ldrge    fp, [r0, #0x14]
0075cc90: str      r5, [r1, #-0x20]!
0075cc94: mov      r0, r4
0075cc98: bl       #0x759f28
0075cc9c: cmp      r0, #0
0075cca0: ldrge    r3, [r7, #0x30]
0075cca4: movlt    r7, #0
0075cca8: addge    r0, r3, r0, lsl #4
0075ccac: ldrge    r7, [r0, #0x14]
0075ccb0: ldr      r0, [sp, #0xc]
0075ccb4: bl       #0x783c7c
0075ccb8: mov      r8, r0
0075ccbc: ldr      r0, [sp, #0xc]
0075ccc0: bl       #0x783cbc
0075ccc4: cmp      r8, r0
0075ccc8: addge    lr, sp, #0x50
0075cccc: strge    lr, [sp, #8]
0075ccd0: blt      #0x75d4bc
0075ccd4: mov      r1, #0
0075ccd8: mov      r0, #0x54
0075ccdc: bl       #0x752b9c
0075cce0: ldr      r1, [sp, #0x1c]
0075cce4: ldr      r2, [pc, #0xb9c]
0075cce8: mov      r3, #0
0075ccec: strh     r3, [r0, #0x10]
0075ccf0: ldr      r2, [r1, r2]
0075ccf4: strh     sb, [r0, #0xe]
0075ccf8: str      fp, [r0, #0x18]
0075ccfc: add      r2, r2, #8
0075cd00: str      r2, [r0]
0075cd04: mov      r2, #0x1c
0075cd08: strb     r2, [r0, #6]
0075cd0c: mov      r2, #4
0075cd10: str      r7, [r0, #0x14]
0075cd14: strb     r3, [r0, #5]
0075cd18: strb     r3, [r0, #4]
0075cd1c: strb     r3, [r0, #7]
0075cd20: strb     r3, [r0, #8]
0075cd24: strb     r3, [r0, #9]
0075cd28: strh     r3, [r0, #0xa]
0075cd2c: strh     r2, [r0, #0x12]
0075cd30: strh     r5, [r0, #0xc]
0075cd34: add      ip, r0, #0x1c
0075cd38: mov      r8, r0
0075cd3c: ldm      sl!, {r0, r1, r2, r3}
0075cd40: stm      ip!, {r0, r1, r2, r3}
0075cd44: ldm      sl, {r0, r1}
0075cd48: mov      r3, #0x34
0075cd4c: stm      ip, {r0, r1}
0075cd50: strb     r3, [r8, #5]
0075cd54: ldr      lr, [sp, #8]
0075cd58: add      ip, r8, r3
0075cd5c: ldm      lr!, {r0, r1, r2, r3}
0075cd60: stm      ip!, {r0, r1, r2, r3}
0075cd64: ldm      lr, {r0, r1, r2, r3}
0075cd68: stm      ip, {r0, r1, r2, r3}
0075cd6c: str      r5, [sp, #0xdc]
0075cd70: ldrb     r3, [r8, #6]
0075cd74: mov      r0, r6
0075cd78: add      r1, sp, #0xdc
0075cd7c: cmp      r3, #0
0075cd80: addne    r3, r8, r3
0075cd84: add      r2, sp, #0xd8
0075cd88: str      r3, [sp, #0xd8]
0075cd8c: bl       #0x75bde8
0075cd90: str      r5, [sp, #0xd4]
0075cd94: ldrb     r3, [r8, #5]
0075cd98: mov      r0, r4
0075cd9c: add      r1, sp, #0xd4
0075cda0: cmp      r3, #0
0075cda4: addne    r3, r8, r3
0075cda8: add      r2, sp, #0xd0
0075cdac: str      r3, [sp, #0xd0]
0075cdb0: bl       #0x75ad60
0075cdb4: add      r3, sp, #0x88
0075cdb8: str      r3, [sp, #0x2c]
0075cdbc: ldrsb    r3, [sp, #0xe8]
0075cdc0: cmn      r3, #1
0075cdc4: beq      #0x75d4ac
0075cdc8: ldr      ip, [sp, #0x2c]
0075cdcc: mov      r1, #0
0075cdd0: add      r4, ip, #4
0075cdd4: mov      r0, r4
0075cdd8: bl       #0x755ad8
0075cddc: mov      r0, r4
0075cde0: mov      r1, #0
0075cde4: bl       #0x752ec8
0075cde8: ldr      r2, [sp, #0x1c]
0075cdec: ldr      r1, [sp, #0x24]
0075cdf0: mov      r0, r8
0075cdf4: ldr      r3, [r2, r1]
0075cdf8: ldr      r2, [sp, #0xfc]
0075cdfc: ldr      r3, [r3]
0075ce00: cmp      r2, r3
0075ce04: bne      #0x75d87c
0075ce08: add      sp, sp, #0x104
0075ce0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0075ce10: cmp      r5, #0x46
0075ce14: movne    r4, #0
0075ce18: moveq    r4, #1
0075ce1c: cmp      r5, #0x1a
0075ce20: movne    r8, r4
0075ce24: orreq    r8, r4, #1
0075ce28: cmp      r8, #0
0075ce2c: addeq    r4, sp, #0x88
0075ce30: streq    r4, [sp, #0x2c]
0075ce34: beq      #0x75cdbc
0075ce38: ldr      r0, [sp, #0xc]
0075ce3c: bl       #0x783b18
0075ce40: mov      r1, r6
0075ce44: ldr      r0, [sp, #0xc]
0075ce48: bl       #0x7839a4
0075ce4c: subs     r0, r0, #0
0075ce50: movne    r0, #1
0075ce54: str      r0, [sp, #0x18]
0075ce58: mov      r1, r6
0075ce5c: ldr      r0, [sp, #0xc]
0075ce60: bl       #0x7839a4
0075ce64: mov      r1, r6
0075ce68: str      r0, [sp, #0x10]
0075ce6c: ldr      r0, [sp, #0xc]
0075ce70: bl       #0x7839a4
0075ce74: mov      r1, r6
0075ce78: str      r0, [sp, #0x2c]
0075ce7c: ldr      r0, [sp, #0xc]
0075ce80: bl       #0x7839a4
0075ce84: mov      r1, r6
0075ce88: str      r0, [sp, #8]
0075ce8c: ldr      r0, [sp, #0xc]
0075ce90: bl       #0x7839a4
0075ce94: mov      r1, r6
0075ce98: str      r0, [sp, #0x14]
0075ce9c: ldr      r0, [sp, #0xc]
0075cea0: bl       #0x7839a4
0075cea4: mov      r1, r6
0075cea8: mov      fp, r0
0075ceac: ldr      r0, [sp, #0xc]
0075ceb0: bl       #0x7839a4
0075ceb4: subs     r0, r0, #0
0075ceb8: movne    r0, #1
0075cebc: str      r0, [sp, #0x3c]
0075cec0: mov      r1, r6
0075cec4: ldr      r0, [sp, #0xc]
0075cec8: bl       #0x7839a4
0075cecc: subs     r0, r0, #0
0075ced0: movne    r0, #1
0075ced4: cmp      r4, #0
0075ced8: moveq    r8, r4
0075cedc: str      r0, [sp, #0x40]
0075cee0: streq    r8, [sp, #0x34]
0075cee4: bne      #0x75d81c
0075cee8: ldr      r0, [sp, #0xc]
0075ceec: bl       #0x783c14
0075cef0: ldr      ip, [sp, #0x3c]
0075cef4: mov      sb, r0
0075cef8: cmp      ip, #0
0075cefc: streq    ip, [sp, #0x48]
0075cf00: bne      #0x75d774
0075cf04: cmp      fp, #0
0075cf08: bne      #0x75d7f4
0075cf0c: mvn      lr, #0
0075cf10: mov      r4, fp
0075cf14: str      lr, [sp, #0x38]
0075cf18: mov      r6, fp
0075cf1c: ldr      r1, [sp, #0x14]
0075cf20: cmp      r1, #0
0075cf24: mvneq    r2, #0
0075cf28: moveq    fp, r6
0075cf2c: streq    r2, [sp, #0x28]
0075cf30: bne      #0x75d7d8
0075cf34: ldr      r3, [sp, #8]
0075cf38: cmp      r3, #0
0075cf3c: bne      #0x75d7c8
0075cf40: ldr      ip, [sp, #0x2c]
0075cf44: cmp      ip, #0
0075cf48: moveq    r6, fp
0075cf4c: mvneq    fp, #0
0075cf50: bne      #0x75d7b0
0075cf54: ldr      lr, [sp, #0x10]
0075cf58: cmp      lr, #0
0075cf5c: bne      #0x75d7a0
0075cf60: cmp      r8, #0
0075cf64: bne      #0x75d4d0
0075cf68: ldr      r0, [sp, #0x34]
0075cf6c: cmp      r0, #0
0075cf70: mvneq    r1, #0
0075cf74: streq    r1, [sp, #0x14]
0075cf78: bne      #0x75d4d0
0075cf7c: ldr      r2, [sp, #0x18]
0075cf80: cmp      r2, #0
0075cf84: mvneq    r3, #0
0075cf88: strne    r6, [sp, #0x4c]
0075cf8c: addne    r6, r6, #0x10
0075cf90: streq    r6, [sp, #0x44]
0075cf94: streq    r3, [sp, #0x4c]
0075cf98: strne    r6, [sp, #0x44]
0075cf9c: cmp      r8, #0
0075cfa0: addeq    r4, sp, #0x88
0075cfa4: streq    r4, [sp, #0x2c]
0075cfa8: bne      #0x75d788
0075cfac: ldr      lr, [sp, #0x34]
0075cfb0: cmp      lr, #0
0075cfb4: bne      #0x75d764
0075cfb8: add      r1, sp, #0x100
0075cfbc: add      r4, r7, #0x34
0075cfc0: str      sb, [r1, #-0x34]!
0075cfc4: mov      r0, r4
0075cfc8: bl       #0x759e68
0075cfcc: cmp      r0, #0
0075cfd0: ldrge    r3, [r7, #0x34]
0075cfd4: movlt    r0, #0
0075cfd8: add      r1, sp, #0x100
0075cfdc: addge    r3, r3, r0, lsl #4
0075cfe0: ldrge    r3, [r3, #0x14]
0075cfe4: add      r6, r7, #0x30
0075cfe8: strlt    r0, [sp, #0x34]
0075cfec: strge    r3, [sp, #0x34]
0075cff0: mov      r0, r6
0075cff4: str      sb, [r1, #-0x38]!
0075cff8: bl       #0x759f28
0075cffc: cmp      r0, #0
0075d000: ldrge    r3, [r7, #0x30]
0075d004: ldr      r1, [sp, #0x44]
0075d008: movlt    r7, #0
0075d00c: addge    r3, r3, r0, lsl #4
0075d010: add      r0, r1, #0x1c
0075d014: mov      r1, #0
0075d018: ldrge    r7, [r3, #0x14]
0075d01c: bl       #0x752b9c
0075d020: ldr      ip, [sp, #0x1c]
0075d024: ldr      r2, [pc, #0x85c]
0075d028: ldr      r3, [sp, #0x38]
0075d02c: strh     r5, [r0, #0x12]
0075d030: ldr      r2, [ip, r2]
0075d034: cmp      r3, #0
0075d038: mov      r3, #0
0075d03c: add      r2, r2, #8
0075d040: strb     r3, [r0, #9]
0075d044: str      r2, [r0]
0075d048: ldr      lr, [sp, #0x48]
0075d04c: mov      r8, r0
0075d050: strh     lr, [r0, #0xe]
0075d054: ldr      r0, [sp, #0x34]
0075d058: str      r7, [r8, #0x14]
0075d05c: str      r0, [r8, #0x18]
0075d060: ldr      r1, [sp, #0x10]
0075d064: strh     r1, [r8, #0x10]
0075d068: ldr      r2, [sp, #8]
0075d06c: strb     r3, [r8, #4]
0075d070: strb     r3, [r8, #5]
0075d074: strh     r2, [r8, #0xa]
0075d078: strb     r3, [r8, #6]
0075d07c: strb     r3, [r8, #7]
0075d080: strb     r3, [r8, #8]
0075d084: strh     sb, [r8, #0xc]
0075d088: bne      #0x75d0d0
0075d08c: mov      r3, #0x1c
0075d090: strb     r3, [r8, #6]
0075d094: add      ip, r8, r3
0075d098: ldm      sl!, {r0, r1, r2, r3}
0075d09c: stm      ip!, {r0, r1, r2, r3}
0075d0a0: ldm      sl, {r0, r1}
0075d0a4: add      r2, sp, #0xc0
0075d0a8: stm      ip, {r0, r1}
0075d0ac: str      sb, [sp, #0xc4]
0075d0b0: ldrb     r3, [r8, #6]
0075d0b4: mov      r0, r4
0075d0b8: add      r1, sp, #0xc4
0075d0bc: cmp      r3, #0
0075d0c0: ldreq    r3, [sp, #0x38]
0075d0c4: addne    r3, r8, r3
0075d0c8: str      r3, [sp, #0xc0]
0075d0cc: bl       #0x75bde8
0075d0d0: ldr      r3, [sp, #0x28]
0075d0d4: cmn      r3, #1
0075d0d8: beq      #0x75d124
0075d0dc: add      lr, r3, #0x1c
0075d0e0: uxtb     lr, lr
0075d0e4: strb     lr, [r8, #5]
0075d0e8: add      ip, sp, #0x50
0075d0ec: add      lr, r8, lr
0075d0f0: ldm      ip!, {r0, r1, r2, r3}
0075d0f4: stm      lr!, {r0, r1, r2, r3}
0075d0f8: ldm      ip, {r0, r1, r2, r3}
0075d0fc: stm      lr, {r0, r1, r2, r3}
0075d100: str      sb, [sp, #0xbc]
0075d104: ldrb     r3, [r8, #5]
0075d108: mov      r0, r6
0075d10c: add      r1, sp, #0xbc
0075d110: cmp      r3, #0
0075d114: addne    r3, r8, r3
0075d118: add      r2, sp, #0xb8
0075d11c: str      r3, [sp, #0xb8]
0075d120: bl       #0x75ad60
0075d124: cmn      fp, #1
0075d128: beq      #0x75d14c
0075d12c: add      r4, fp, #0x1c
0075d130: uxtb     r4, r4
0075d134: strb     r4, [r8, #4]
0075d138: ldr      ip, [sp, #0x20]
0075d13c: add      r1, sp, #0xe8
0075d140: add      r0, ip, #0x2c
0075d144: bl       #0x75c2cc
0075d148: str      r0, [r8, r4]
0075d14c: ldr      lr, [sp, #0x14]
0075d150: cmn      lr, #1
0075d154: beq      #0x75d1fc
0075d158: add      r1, lr, #0x1c
0075d15c: uxtb     r1, r1
0075d160: mov      r3, #0
0075d164: add      r2, r8, r1
0075d168: strb     r1, [r8, #7]
0075d16c: str      r3, [r8, r1]
0075d170: strb     r3, [r2, #0x10]
0075d174: str      r3, [r2, #4]
0075d178: str      r3, [r2, #8]
0075d17c: str      r3, [r2, #0xc]
0075d180: ldrb     r3, [r8, #7]
0075d184: ldr      r2, [sp, #0x88]
0075d188: cmp      r3, #0
0075d18c: addne    r3, r8, r3
0075d190: str      r2, [r3]
0075d194: ldrb     r4, [r8, #7]
0075d198: ldr      r1, [sp, #0x90]
0075d19c: cmp      r4, #0
0075d1a0: addne    r4, r8, r4
0075d1a4: add      r0, r4, #4
0075d1a8: bl       #0x755ad8
0075d1ac: ldr      r3, [r4, #8]
0075d1b0: cmp      r3, #0
0075d1b4: ble      #0x75d1fc
0075d1b8: mov      r5, #0
0075d1bc: mov      r6, r5
0075d1c0: ldr      ip, [r4, #4]
0075d1c4: ldr      lr, [sp, #0x8c]
0075d1c8: add      r6, r6, #1
0075d1cc: add      ip, ip, r5
0075d1d0: add      lr, lr, r5
0075d1d4: ldm      lr!, {r0, r1, r2, r3}
0075d1d8: stm      ip!, {r0, r1, r2, r3}
0075d1dc: ldm      lr!, {r0, r1, r2, r3}
0075d1e0: stm      ip!, {r0, r1, r2, r3}
0075d1e4: ldm      lr, {r0, r1, r2}
0075d1e8: stm      ip, {r0, r1, r2}
0075d1ec: ldr      r3, [r4, #8]
0075d1f0: add      r5, r5, #0x2c
0075d1f4: cmp      r6, r3
0075d1f8: blt      #0x75d1c0
0075d1fc: ldr      r0, [sp, #0x18]
0075d200: cmp      r0, #0
0075d204: beq      #0x75d500
0075d208: ldr      r1, [sp, #0x4c]
0075d20c: mov      r2, #0
0075d210: add      r3, r1, #0x1c
0075d214: uxtb     r3, r3
0075d218: cmp      r3, #0
0075d21c: strb     r3, [r8, #8]
0075d220: addne    r3, r8, r3
0075d224: strb     r2, [r3, #0xc]
0075d228: str      r2, [r3]
0075d22c: str      r2, [r3, #4]
0075d230: str      r2, [r3, #8]
0075d234: ldr      r0, [sp, #0xc]
0075d238: bl       #0x783c14
0075d23c: ldr      r2, [sp, #0x30]
0075d240: cmp      r2, #5
0075d244: bgt      #0x75d53c
0075d248: ldr      r0, [sp, #0xc]
0075d24c: bl       #0x783c14
0075d250: ldr      r3, [pc, #0x634]
0075d254: ldr      r2, [pc, #0x634]
0075d258: str      r8, [sp, #0x14]
0075d25c: add      r3, pc, r3
0075d260: str      r3, [sp, #0x10]
0075d264: ldr      r3, [pc, #0x628]
0075d268: ldr      r4, [sp, #0x10]
0075d26c: add      r2, pc, r2
0075d270: add      r3, pc, r3
0075d274: add      r3, r3, #0x24
0075d278: add      r4, r4, #0x20
0075d27c: str      r2, [sp, #0x34]
0075d280: str      r4, [sp, #0x38]
0075d284: mov      sb, r3
0075d288: ldr      r0, [sp, #0xc]
0075d28c: bl       #0x783b18
0075d290: ldr      r0, [sp, #0xc]
0075d294: bl       #0x783c7c
0075d298: ldr      ip, [sp, #0x30]
0075d29c: mov      r5, r0
0075d2a0: cmp      ip, #5
0075d2a4: ble      #0x75d49c
0075d2a8: ldr      r0, [sp, #0xc]
0075d2ac: bl       #0x783f1c
0075d2b0: mov      r8, r0
0075d2b4: cmp      r8, #0
0075d2b8: beq      #0x75d814
0075d2bc: ldr      r0, [sp, #0xc]
0075d2c0: bl       #0x783f1c
0075d2c4: ands     r3, r8, #0x20000
0075d2c8: mov      r4, r0
0075d2cc: streq    r3, [sp, #0x28]
0075d2d0: bne      #0x75d5a4
0075d2d4: add      lr, sp, #0xac
0075d2d8: mov      r0, lr
0075d2dc: str      lr, [sp, #0x18]
0075d2e0: bl       #0x7bae08
0075d2e4: ldr      r0, [sp, #0x18]
0075d2e8: ldr      r1, [sp, #0xc]
0075d2ec: bl       #0x7bafa4
0075d2f0: ldr      r3, [sp, #0xac]
0075d2f4: str      r5, [sp, #0xb4]
0075d2f8: ldr      r2, [r3]
0075d2fc: cmp      r4, r2
0075d300: bne      #0x75d4dc
0075d304: ldr      r0, [sp, #0x10]
0075d308: ldr      r4, [r0, #0x20]
0075d30c: ands     r4, r4, #1
0075d310: beq      #0x75d5c8
0075d314: cmp      r8, #0x80000
0075d318: bhi      #0x75d5b8
0075d31c: mov      r5, #0
0075d320: add      lr, sp, #0x9c
0075d324: mov      r7, #1
0075d328: mov      r4, r5
0075d32c: str      lr, [sp, #8]
0075d330: b        #0x75d344
0075d334: add      r5, r5, #1
0075d338: cmp      r5, #0x13
0075d33c: beq      #0x75d46c
0075d340: lsl      r7, r7, #1
0075d344: tst      r7, r8
0075d348: beq      #0x75d334
0075d34c: mov      r1, #0
0075d350: mov      r0, #0x14
0075d354: bl       #0x752ba8
0075d358: strb     r4, [r0]
0075d35c: strb     r4, [r0, #1]
0075d360: mov      r6, r0
0075d364: mov      r0, #0
0075d368: strh     r0, [r6, #2]
0075d36c: str      r4, [r6, #4]
0075d370: strb     r4, [r6, #8]
0075d374: strb     r4, [r6, #9]
0075d378: ldr      r2, [sb, r5, lsl #3]
0075d37c: add      r3, sb, r5, lsl #3
0075d380: cmp      r5, #0x11
0075d384: str      r2, [r6]
0075d388: ldr      r3, [r3, #4]
0075d38c: mov      r0, #0x7c
0075d390: str      r3, [r6, #4]
0075d394: ldreq    r1, [sp, #0x28]
0075d398: strbeq   r1, [r6, #1]
0075d39c: mov      r1, r4
0075d3a0: str      r4, [sp, #0x9c]
0075d3a4: str      r4, [sp, #0xa0]
0075d3a8: str      r4, [sp, #0xa4]
0075d3ac: strb     r4, [sp, #0xa8]
0075d3b0: bl       #0x752ba8
0075d3b4: ldr      ip, [sp, #8]
0075d3b8: ldr      r2, [sp, #0x18]
0075d3bc: ldr      r1, [sp, #0x20]
0075d3c0: mov      r3, r4
0075d3c4: mov      sl, r0
0075d3c8: str      ip, [sp]
0075d3cc: bl       #0x7d2e38
0075d3d0: ldr      r3, [sp, #0xac]
0075d3d4: add      r0, r6, #8
0075d3d8: mov      r1, sl
0075d3dc: ldr      r3, [r3]
0075d3e0: str      r3, [sl, #0x5c]
0075d3e4: bl       #0x797250
0075d3e8: ldr      lr, [sp, #0x14]
0075d3ec: ldrb     sl, [lr, #8]
0075d3f0: cmp      sl, #0
0075d3f4: ldrne    r0, [sp, #0x14]
0075d3f8: moveq    sl, r4
0075d3fc: addne    sl, r0, sl
0075d400: ldr      r3, [sl, #4]
0075d404: ldr      r2, [sl, #8]
0075d408: add      fp, r3, #1
0075d40c: cmp      fp, r2
0075d410: bgt      #0x75d548
0075d414: ldr      r2, [sl]
0075d418: str      r6, [r2, r3, lsl #2]
0075d41c: str      fp, [sl, #4]
0075d420: ldr      r6, [sp, #0xa0]
0075d424: cmp      r6, #0
0075d428: ble      #0x75d56c
0075d42c: mov      sl, #0
0075d430: ldr      r3, [sp, #0x9c]
0075d434: ldr      r0, [r3, sl, lsl #3]
0075d438: cmp      r0, #0
0075d43c: beq      #0x75d444
0075d440: bl       #0x75a240
0075d444: add      sl, sl, #1
0075d448: cmp      sl, r6
0075d44c: bne      #0x75d430
0075d450: ldr      r0, [sp, #8]
0075d454: mov      r1, r4
0075d458: str      r4, [sp, #0xa0]
0075d45c: bl       #0x75a314
0075d460: add      r5, r5, #1
0075d464: cmp      r5, #0x13
0075d468: bne      #0x75d340
0075d46c: ldr      r0, [sp, #0xac]
0075d470: cmp      r0, #0
0075d474: beq      #0x75d288
0075d478: bl       #0x75b9f4
0075d47c: ldr      r0, [sp, #0xc]
0075d480: bl       #0x783b18
0075d484: ldr      r0, [sp, #0xc]
0075d488: bl       #0x783c7c
0075d48c: ldr      ip, [sp, #0x30]
0075d490: mov      r5, r0
0075d494: cmp      ip, #5
0075d498: bgt      #0x75d2a8
0075d49c: ldr      r0, [sp, #0xc]
0075d4a0: bl       #0x783c14
0075d4a4: mov      r8, r0
0075d4a8: b        #0x75d2b4
0075d4ac: ldr      r0, [sp, #0xf4]
0075d4b0: ldr      r1, [sp, #0xf0]
0075d4b4: bl       #0x752b38
0075d4b8: b        #0x75cdc8
0075d4bc: add      r0, sp, #0x50
0075d4c0: ldr      r1, [sp, #0xc]
0075d4c4: str      r0, [sp, #8]
0075d4c8: bl       #0x796358
0075d4cc: b        #0x75ccd4
0075d4d0: str      r6, [sp, #0x14]
0075d4d4: add      r6, r4, #0x14
0075d4d8: b        #0x75cf7c
0075d4dc: ldr      r0, [pc, #0x3b4]
0075d4e0: mov      r1, r4
0075d4e4: ldr      r8, [sp, #0x14]
0075d4e8: add      r0, pc, r0
0075d4ec: bl       #0x761184
0075d4f0: ldr      r0, [sp, #0xac]
0075d4f4: cmp      r0, #0
0075d4f8: beq      #0x75d500
0075d4fc: bl       #0x75b9f4
0075d500: ldr      r1, [sp, #0x3c]
0075d504: cmp      r1, #0
0075d508: bne      #0x75d520
0075d50c: ldr      r3, [sp, #0x40]
0075d510: cmp      r3, #0
0075d514: movne    r3, #1
0075d518: strbne   r3, [r8, #9]
0075d51c: b        #0x75cdbc
0075d520: ldr      r2, [sp, #0x40]
0075d524: cmp      r2, #0
0075d528: movne    r3, #2
0075d52c: strbne   r3, [r8, #9]
0075d530: ldreq    r4, [sp, #0x40]
0075d534: strbeq   r4, [r8, #9]
0075d538: b        #0x75cdbc
0075d53c: ldr      r0, [sp, #0xc]
0075d540: bl       #0x783f1c
0075d544: b        #0x75d250
0075d548: mov      r0, sl
0075d54c: add      r1, fp, fp, asr #1
0075d550: bl       #0x75a298
0075d554: ldm      sl, {r2, r3}
0075d558: str      r6, [r2, r3, lsl #2]
0075d55c: str      fp, [sl, #4]
0075d560: ldr      r6, [sp, #0xa0]
0075d564: cmp      r6, #0
0075d568: bgt      #0x75d42c
0075d56c: bge      #0x75d450
0075d570: lsl      r3, r6, #3
0075d574: ldr      r2, [sp, #0x9c]
0075d578: adds     r6, r6, #1
0075d57c: add      r1, r2, r3
0075d580: str      r4, [r2, r3]
0075d584: str      r4, [r1, #4]
0075d588: add      r3, r3, #8
0075d58c: bne      #0x75d574
0075d590: ldr      r0, [sp, #8]
0075d594: mov      r1, r4
0075d598: str      r4, [sp, #0xa0]
0075d59c: bl       #0x75a314
0075d5a0: b        #0x75d460
0075d5a4: ldr      r0, [sp, #0xc]
0075d5a8: bl       #0x783b28
0075d5ac: sub      r4, r4, #1
0075d5b0: str      r0, [sp, #0x28]
0075d5b4: b        #0x75d2d4
0075d5b8: ldr      r0, [sp, #0x34]
0075d5bc: mov      r1, r8
0075d5c0: bl       #0x761184
0075d5c4: b        #0x75d31c
0075d5c8: ldr      r0, [sp, #0x38]
0075d5cc: bl       #0x30e76c
0075d5d0: cmp      r0, #0
0075d5d4: beq      #0x75d314
0075d5d8: ldr      ip, [sp, #0x10]
0075d5dc: mov      r1, #0xa
0075d5e0: mov      r2, #0x11
0075d5e4: strb     r1, [ip, #0x24]
0075d5e8: mov      r1, #0xc
0075d5ec: strb     r1, [ip, #0x2c]
0075d5f0: mov      r1, #0xb
0075d5f4: strb     r1, [ip, #0x34]
0075d5f8: mov      r1, #0xf
0075d5fc: strb     r1, [ip, #0x3c]
0075d600: mov      r1, #0xd
0075d604: strb     r1, [ip, #0x44]
0075d608: mov      r1, #0xe
0075d60c: strb     r1, [ip, #0x4c]
0075d610: mov      r1, #0x10
0075d614: strb     r1, [ip, #0x54]
0075d618: mov      r1, #0x12
0075d61c: strb     r1, [ip, #0x64]
0075d620: mov      r1, #9
0075d624: strb     r4, [ip, #0x25]
0075d628: strh     r4, [ip, #0x26]
0075d62c: str      r4, [ip, #0x28]
0075d630: strb     r4, [ip, #0x2d]
0075d634: strh     r4, [ip, #0x2e]
0075d638: str      r4, [ip, #0x30]
0075d63c: strb     r4, [ip, #0x35]
0075d640: strh     r4, [ip, #0x36]
0075d644: str      r4, [ip, #0x38]
0075d648: strb     r4, [ip, #0x3d]
0075d64c: strh     r4, [ip, #0x3e]
0075d650: str      r4, [ip, #0x40]
0075d654: strb     r4, [ip, #0x45]
0075d658: strh     r4, [ip, #0x46]
0075d65c: str      r4, [ip, #0x48]
0075d660: strb     r4, [ip, #0x4d]
0075d664: strh     r4, [ip, #0x4e]
0075d668: str      r4, [ip, #0x50]
0075d66c: strb     r4, [ip, #0x55]
0075d670: strh     r4, [ip, #0x56]
0075d674: str      r4, [ip, #0x58]
0075d678: strb     r2, [ip, #0x5c]
0075d67c: strb     r4, [ip, #0x5d]
0075d680: strh     r4, [ip, #0x5e]
0075d684: str      r4, [ip, #0x60]
0075d688: strb     r1, [ip, #0x6c]
0075d68c: mov      r1, #1
0075d690: strb     r1, [ip, #0x74]
0075d694: mov      r1, #2
0075d698: strb     r1, [ip, #0x7c]
0075d69c: mov      r1, #3
0075d6a0: strb     r1, [ip, #0x84]
0075d6a4: mov      r1, #4
0075d6a8: strb     r1, [ip, #0x8c]
0075d6ac: mov      r1, #5
0075d6b0: strb     r1, [ip, #0x94]
0075d6b4: mov      r1, #6
0075d6b8: strb     r1, [ip, #0x9c]
0075d6bc: mov      r1, #7
0075d6c0: strb     r4, [ip, #0x65]
0075d6c4: strb     r1, [ip, #0xa4]
0075d6c8: strh     r4, [ip, #0x66]
0075d6cc: str      r4, [ip, #0x68]
0075d6d0: strb     r4, [ip, #0x6d]
0075d6d4: strh     r4, [ip, #0x6e]
0075d6d8: str      r4, [ip, #0x70]
0075d6dc: strb     r4, [ip, #0x75]
0075d6e0: strh     r4, [ip, #0x76]
0075d6e4: str      r4, [ip, #0x78]
0075d6e8: strb     r4, [ip, #0x7d]
0075d6ec: strh     r4, [ip, #0x7e]
0075d6f0: str      r4, [ip, #0x80]
0075d6f4: strb     r4, [ip, #0x85]
0075d6f8: strh     r4, [ip, #0x86]
0075d6fc: str      r4, [ip, #0x88]
0075d700: strb     r4, [ip, #0x8d]
0075d704: strh     r4, [ip, #0x8e]
0075d708: str      r4, [ip, #0x90]
0075d70c: strb     r4, [ip, #0x95]
0075d710: strh     r4, [ip, #0x96]
0075d714: str      r4, [ip, #0x98]
0075d718: strb     r4, [ip, #0x9d]
0075d71c: strh     r4, [ip, #0x9e]
0075d720: str      r4, [ip, #0xa0]
0075d724: strb     r4, [ip, #0xa5]
0075d728: mov      r1, #8
0075d72c: strh     r4, [ip, #0xa6]
0075d730: strb     r2, [ip, #0xad]
0075d734: mov      r2, #0x13
0075d738: str      r4, [ip, #0xa8]
0075d73c: strb     r1, [ip, #0xac]
0075d740: strb     r2, [ip, #0xb4]
0075d744: strb     r4, [ip, #0xb5]
0075d748: str      r4, [ip, #0xb8]
0075d74c: strh     r4, [ip, #0xae]
0075d750: str      r4, [ip, #0xb0]
0075d754: strh     r4, [ip, #0xb6]
0075d758: ldr      r0, [sp, #0x38]
0075d75c: bl       #0x30ea3c
0075d760: b        #0x75d314
0075d764: ldr      r0, [sp, #0xc]
0075d768: bl       #0x783b28
0075d76c: str      r0, [sp, #0x88]
0075d770: b        #0x75cfb8
0075d774: ldr      r0, [sp, #0xc]
0075d778: bl       #0x783c14
0075d77c: uxth     r0, r0
0075d780: str      r0, [sp, #0x48]
0075d784: b        #0x75cf04
0075d788: add      ip, sp, #0x88
0075d78c: ldr      r0, [sp, #0xc]
0075d790: mov      r1, ip
0075d794: str      ip, [sp, #0x2c]
0075d798: bl       #0x758ca8
0075d79c: b        #0x75cfac
0075d7a0: ldr      r0, [sp, #0xc]
0075d7a4: bl       #0x783c14
0075d7a8: str      r0, [sp, #0x10]
0075d7ac: b        #0x75cf60
0075d7b0: ldr      r0, [sp, #0xc]
0075d7b4: add      r1, sp, #0xe8
0075d7b8: add      r6, r4, #4
0075d7bc: bl       #0x7841c8
0075d7c0: mov      r4, r6
0075d7c4: b        #0x75cf54
0075d7c8: ldr      r0, [sp, #0xc]
0075d7cc: bl       #0x783c14
0075d7d0: str      r0, [sp, #8]
0075d7d4: b        #0x75cf40
0075d7d8: add      r0, sp, #0x50
0075d7dc: ldr      r1, [sp, #0xc]
0075d7e0: add      fp, r4, #0x20
0075d7e4: bl       #0x796064
0075d7e8: mov      r4, fp
0075d7ec: str      r6, [sp, #0x28]
0075d7f0: b        #0x75cf34
0075d7f4: mov      r0, sl
0075d7f8: ldr      r1, [sp, #0xc]
0075d7fc: bl       #0x7965d4
0075d800: mov      r4, #0x18
0075d804: mov      r0, #0
0075d808: str      r0, [sp, #0x38]
0075d80c: mov      r6, r4
0075d810: b        #0x75cf1c
0075d814: ldr      r8, [sp, #0x14]
0075d818: b        #0x75d500
0075d81c: mov      r1, #3
0075d820: ldr      r0, [sp, #0xc]
0075d824: bl       #0x7839a4
0075d828: mov      r1, r6
0075d82c: ldr      r0, [sp, #0xc]
0075d830: bl       #0x7839a4
0075d834: mov      r1, r6
0075d838: ldr      r0, [sp, #0xc]
0075d83c: bl       #0x7839a4
0075d840: mov      r1, r6
0075d844: ldr      r0, [sp, #0xc]
0075d848: bl       #0x7839a4
0075d84c: mov      r1, r6
0075d850: ldr      r0, [sp, #0xc]
0075d854: bl       #0x7839a4
0075d858: subs     r0, r0, #0
0075d85c: movne    r0, #1
0075d860: str      r0, [sp, #0x34]
0075d864: mov      r1, r6
0075d868: ldr      r0, [sp, #0xc]
0075d86c: bl       #0x7839a4
0075d870: subs     r8, r0, #0
0075d874: movne    r8, #1
0075d878: b        #0x75cee8
0075d87c: bl       #0x30e310
0075d880: eoreq    r7, r3, r4, lsl pc
0075d884: andeq    r4, r0, ip, lsr #1
0075d888: andeq    r0, r0, ip, asr #18
0075d88c: eoreq    pc, sb, ip, asr r2
0075d890: ldrsbeq  fp, [sl], -r4
0075d894: eoreq    pc, sb, r8, asr #4
0075d898: andseq   fp, sl, r0, lsr #14

# _ZN7gameswf25button_character_instance24get_topmost_mouse_entityEff
007c6e50: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007c6e54: mov      r7, r0
007c6e58: ldrb     r0, [r0, #0x9b]
007c6e5c: sub      sp, sp, #0x18
007c6e60: mov      ip, r1
007c6e64: cmp      r0, #0
007c6e68: mov      r3, r2
007c6e6c: bne      #0x7c6e7c
007c6e70: mov      r0, #0
007c6e74: add      sp, sp, #0x18
007c6e78: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007c6e7c: add      sl, sp, #0x10
007c6e80: ldr      r0, [r7, #0x4c]
007c6e84: mov      r8, #0
007c6e88: add      r2, sp, #8
007c6e8c: mov      r1, sl
007c6e90: str      r3, [sp, #0xc]
007c6e94: str      ip, [sp, #8]
007c6e98: str      r8, [sp, #0x10]
007c6e9c: str      r8, [sp, #0x14]
007c6ea0: bl       #0x753d7c
007c6ea4: ldr      r3, [r7, #0xa0]
007c6ea8: ldr      r2, [r3, #0x28]
007c6eac: cmp      r2, #0
007c6eb0: ble      #0x7c6e70
007c6eb4: mov      r5, #0
007c6eb8: mov      r6, r5
007c6ebc: mov      sb, sp
007c6ec0: ldr      r4, [r3, #0x24]
007c6ec4: mov      r1, sp
007c6ec8: mov      r2, sl
007c6ecc: add      r4, r4, r5
007c6ed0: ldr      ip, [r4, #8]
007c6ed4: add      r0, r4, #0x14
007c6ed8: cmp      ip, #0
007c6edc: blt      #0x7c6f20
007c6ee0: ldrb     ip, [r4, #2]
007c6ee4: cmp      ip, #0
007c6ee8: beq      #0x7c6f20
007c6eec: str      r8, [sp]
007c6ef0: str      r8, [sp, #4]
007c6ef4: bl       #0x753d7c
007c6ef8: ldr      r3, [r4, #0xc]
007c6efc: ldr      r1, [sp]
007c6f00: ldr      r2, [sp, #4]
007c6f04: mov      r0, r3
007c6f08: ldr      r3, [r3]
007c6f0c: mov      lr, pc
007c6f10: ldr      pc, [r3, #0x10]
007c6f14: cmp      r0, #0
007c6f18: bne      #0x7c6f38
007c6f1c: ldr      r3, [r7, #0xa0]
007c6f20: ldr      r2, [r3, #0x28]
007c6f24: add      r6, r6, #1
007c6f28: add      r5, r5, #0x64
007c6f2c: cmp      r6, r2
007c6f30: blt      #0x7c6ec0
007c6f34: b        #0x7c6e70
007c6f38: mov      r0, r7
007c6f3c: b        #0x7c6e74

# _ZN7gameswf9character16get_world_matrixEv
00753f74: push     {r4, lr}
00753f78: mov      r4, r0
00753f7c: bl       #0x753ee8
00753f80: subs     r3, r0, #0
00753f84: beq      #0x753f94
00753f88: ldr      r3, [r3]
00753f8c: mov      lr, pc
00753f90: ldr      pc, [r3, #0x118]
00753f94: add      r0, r4, #0x78
00753f98: pop      {r4, pc}

# _ZN7gameswf15sprite_instance24get_topmost_mouse_entityEff
0077f00c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0077f010: mov      r4, r0
0077f014: ldrb     r0, [r0, #0x9b]
0077f018: sub      sp, sp, #0x1c
0077f01c: str      r1, [sp, #4]
0077f020: cmp      r0, #0
0077f024: str      r2, [sp]
0077f028: moveq    r5, r0
0077f02c: beq      #0x77f138
0077f030: ldr      r3, [r4, #0x54]
0077f034: cmp      r3, #0
0077f038: beq      #0x77f058
0077f03c: ldr      r0, [r3, #0x68]
0077f040: cmp      r0, #0
0077f044: beq      #0x77f058
0077f048: mov      r1, r4
0077f04c: add      r2, sp, #4
0077f050: mov      r3, sp
0077f054: bl       #0x777514
0077f058: ldr      ip, [sp, #4]
0077f05c: ldr      r0, [r4, #0x4c]
0077f060: mov      r3, #0
0077f064: str      ip, [sp, #8]
0077f068: ldr      ip, [sp]
0077f06c: add      r1, sp, #0x10
0077f070: add      r2, sp, #8
0077f074: str      r3, [sp, #0x14]
0077f078: str      ip, [sp, #0xc]
0077f07c: str      r3, [sp, #0x10]
0077f080: bl       #0x753d7c
0077f084: ldr      r8, [r4, #0xac]
0077f088: subs     r7, r8, #1
0077f08c: bmi      #0x77f178
0077f090: ldr      sl, [pc, #0xec]
0077f094: mov      r6, #0
0077f098: lsl      r7, r7, #2
0077f09c: add      sl, pc, sl
0077f0a0: mov      sb, r6
0077f0a4: mov      fp, r6
0077f0a8: ldr      r3, [r4, #0xa8]
0077f0ac: ldr      r5, [r3, r7]
0077f0b0: subs     r0, r5, #0
0077f0b4: beq      #0x77f11c
0077f0b8: ldrb     r3, [r5, #0x9b]
0077f0bc: cmp      r3, #0
0077f0c0: beq      #0x77f11c
0077f0c4: ldr      r1, [sp, #0x10]
0077f0c8: ldr      r2, [sp, #0x14]
0077f0cc: ldr      r3, [r5]
0077f0d0: mov      lr, pc
0077f0d4: ldr      pc, [r3, #0x68]
0077f0d8: subs     fp, r0, #0
0077f0dc: beq      #0x77f0f8
0077f0e0: ldr      r3, [fp]
0077f0e4: mov      lr, pc
0077f0e8: ldr      pc, [r3, #0x15c]
0077f0ec: cmp      r0, #0
0077f0f0: bne      #0x77f170
0077f0f4: mov      sb, #1
0077f0f8: ldr      r3, [r5, #0x44]
0077f0fc: mov      r1, sl
0077f100: ldrsb    r2, [r3]
0077f104: add      r0, r3, #1
0077f108: cmn      r2, #1
0077f10c: ldreq    r0, [r3, #0xc]
0077f110: bl       #0x30e31c
0077f114: cmp      r0, #0
0077f118: beq      #0x77f12c
0077f11c: add      r6, r6, #1
0077f120: cmp      r6, r8
0077f124: sub      r7, r7, #4
0077f128: bne      #0x77f0a8
0077f12c: cmp      sb, #0
0077f130: bne      #0x77f144
0077f134: mov      r5, fp
0077f138: mov      r0, r5
0077f13c: add      sp, sp, #0x1c
0077f140: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0077f144: mov      r5, #0
0077f148: ldr      r3, [r4]
0077f14c: mov      r0, r4
0077f150: mov      lr, pc
0077f154: ldr      pc, [r3, #0x15c]
0077f158: cmp      r0, #0
0077f15c: movne    r5, r4
0077f160: bne      #0x77f138
0077f164: cmp      r5, #0
0077f168: bne      #0x77f138
0077f16c: b        #0x77f134
0077f170: mov      r5, fp
0077f174: b        #0x77f148
0077f178: mov      fp, #0
0077f17c: mov      r5, fp
0077f180: b        #0x77f138
0077f184: andseq   sl, r8, r4, ror #20

# _ZN7gameswf12display_list7displayEv
007552ac: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007552b0: ldr      r7, [r0, #4]
007552b4: ldr      sb, [pc, #0x1e8]
007552b8: sub      sp, sp, #0xc
007552bc: cmp      r7, #0
007552c0: mov      r6, r0
007552c4: add      sb, pc, sb
007552c8: ble      #0x75541c
007552cc: ldr      fp, [pc, #0x1d4]
007552d0: mov      r4, #0
007552d4: str      r4, [sp, #4]
007552d8: str      r4, [sp]
007552dc: mov      sl, r4
007552e0: ldr      r3, [r6]
007552e4: ldr      r5, [r3, r4, lsl #2]
007552e8: ldrb     r3, [r5, #0x9b]
007552ec: cmp      r3, #0
007552f0: beq      #0x75542c
007552f4: ldr      r8, [r5, #0x48]
007552f8: mov      r1, #0
007552fc: ldr      r0, [r8, #0x18]
00755300: bl       #0x30df8c
00755304: cmp      r0, #0
00755308: beq      #0x755320
0075530c: ldr      r0, [r8, #0x1c]
00755310: mov      r1, #0
00755314: bl       #0x30df8c
00755318: cmp      r0, #0
0075531c: bne      #0x75542c
00755320: cmp      sl, #0
00755324: beq      #0x75535c
00755328: ldrh     r3, [r5, #0x94]
0075532c: ldr      r2, [sp]
00755330: cmp      r3, r2
00755334: ble      #0x75535c
00755338: ldr      r3, [sb, fp]
0075533c: ldr      sl, [r3]
00755340: cmp      sl, #0
00755344: beq      #0x75535c
00755348: mov      r0, sl
0075534c: ldr      r3, [sl]
00755350: mov      lr, pc
00755354: ldr      pc, [r3, #0x90]
00755358: mov      sl, #0
0075535c: ldrh     r3, [r5, #0x96]
00755360: cmp      r3, #0
00755364: bne      #0x75543c
00755368: ldr      r3, [r5]
0075536c: mov      r0, r5
00755370: mov      lr, pc
00755374: ldr      pc, [r3, #0x120]
00755378: ldrh     r2, [r5, #0x96]
0075537c: cmp      r2, #0
00755380: beq      #0x7553e0
00755384: ldr      r3, [sb, fp]
00755388: ldr      r3, [r3]
0075538c: cmp      r3, #0
00755390: beq      #0x7553a8
00755394: mov      r0, r3
00755398: ldr      r3, [r3]
0075539c: mov      lr, pc
007553a0: ldr      pc, [r3, #0x8c]
007553a4: ldrh     r2, [r5, #0x96]
007553a8: ldr      r3, [sp, #4]
007553ac: str      r2, [sp]
007553b0: cmp      r3, #0
007553b4: beq      #0x755424
007553b8: ldr      r3, [sb, fp]
007553bc: ldr      r3, [r3]
007553c0: cmp      r3, #0
007553c4: beq      #0x755424
007553c8: mov      r0, r3
007553cc: ldr      r1, [sp, #4]
007553d0: ldr      r3, [r3]
007553d4: mov      lr, pc
007553d8: ldr      pc, [r3, #0x30]
007553dc: mov      sl, #1
007553e0: ldr      r7, [r6, #4]
007553e4: add      r4, r4, #1
007553e8: cmp      r4, r7
007553ec: blt      #0x7552e0
007553f0: cmp      sl, #0
007553f4: beq      #0x75541c
007553f8: ldr      r3, [pc, #0xa8]
007553fc: ldr      r3, [sb, r3]
00755400: ldr      r3, [r3]
00755404: cmp      r3, #0
00755408: beq      #0x75541c
0075540c: mov      r0, r3
00755410: ldr      r3, [r3]
00755414: mov      lr, pc
00755418: ldr      pc, [r3, #0x90]
0075541c: add      sp, sp, #0xc
00755420: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00755424: ldr      r7, [r6, #4]
00755428: mov      sl, #1
0075542c: add      r4, r4, #1
00755430: cmp      r4, r7
00755434: blt      #0x7552e0
00755438: b        #0x7553f0
0075543c: add      r7, r5, #0x2c
00755440: mov      r0, r7
00755444: bl       #0x755260
00755448: ldr      r3, [r5, #0x30]
0075544c: ldr      r3, [r3, #0xa0]
00755450: cmp      r3, #0
00755454: movle    r3, #0
00755458: strle    r3, [sp, #4]
0075545c: ble      #0x755480
00755460: mov      r0, r7
00755464: bl       #0x755260
00755468: ldr      r3, [r5, #0x30]
0075546c: ldr      r2, [r3, #0xa0]
00755470: ldr      r3, [r3, #0x9c]
00755474: sub      r2, r2, #1
00755478: ldr      r3, [r3, r2, lsl #2]
0075547c: str      r3, [sp, #4]
00755480: ldr      r3, [sb, fp]
00755484: ldr      r3, [r3]
00755488: cmp      r3, #0
0075548c: beq      #0x755368
00755490: mov      r0, r3
00755494: ldr      r3, [r3]
00755498: mov      lr, pc
0075549c: ldr      pc, [r3, #0x88]
007554a0: b        #0x755368
007554a4: eoreq    pc, r3, ip, asr #15
007554a8: strheq   r3, [r0], -r4
