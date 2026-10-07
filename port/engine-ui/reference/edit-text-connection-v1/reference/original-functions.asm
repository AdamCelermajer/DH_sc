# _ZN7gameswf9smart_ptrINS_23edit_text_character_defEE7set_refEPS1_
0076ca84: push     {r4, r5, r6, lr}
0076ca88: mov      r4, r0
0076ca8c: ldr      r0, [r0]
0076ca90: mov      r5, r1
0076ca94: cmp      r0, r1
0076ca98: beq      #0x76cac0
0076ca9c: cmp      r0, #0
0076caa0: beq      #0x76caa8
0076caa4: bl       #0x75a240
0076caa8: cmp      r5, #0
0076caac: str      r5, [r4]
0076cab0: beq      #0x76cac0
0076cab4: mov      r0, r5
0076cab8: pop      {r4, r5, r6, lr}
0076cabc: b        #0x759c64
0076cac0: pop      {r4, r5, r6, pc}

# _ZN7gameswf6player26create_edit_text_characterEPNS_23edit_text_character_defEPNS_9characterEi
0076cb44: push     {r4, r5, r6, r7, r8, lr}
0076cb48: mov      r5, r0
0076cb4c: ldr      r0, [r0, #0xd4]
0076cb50: sub      sp, sp, #0x10
0076cb54: mov      r7, r2
0076cb58: cmp      r0, #0
0076cb5c: mov      r6, r3
0076cb60: mov      r8, r1
0076cb64: ble      #0x76cbc4
0076cb68: ldr      r3, [r5, #0xd0]
0076cb6c: sub      r0, r0, #1
0076cb70: ldr      r4, [r3, r0, lsl #2]
0076cb74: add      r0, r4, #0xa0
0076cb78: bl       #0x76ca84
0076cb7c: ldr      r3, [r4]
0076cb80: mov      r1, r7
0076cb84: mov      r2, r6
0076cb88: mov      r0, r4
0076cb8c: mov      lr, pc
0076cb90: ldr      pc, [r3, #0x64]
0076cb94: ldr      r3, [r4, #4]
0076cb98: cmp      r3, #1
0076cb9c: beq      #0x76cbec
0076cba0: ldr      r3, [r5, #0x30]
0076cba4: add      r0, r5, #0xd0
0076cba8: str      r3, [r4, #0x34]
0076cbac: ldr      r1, [r5, #0xd4]
0076cbb0: sub      r1, r1, #1
0076cbb4: bl       #0x75586c
0076cbb8: mov      r0, r4
0076cbbc: add      sp, sp, #0x10
0076cbc0: pop      {r4, r5, r6, r7, r8, pc}
0076cbc4: mov      r1, #0
0076cbc8: mov      r0, #0x198
0076cbcc: bl       #0x752ba8
0076cbd0: mov      r1, r5
0076cbd4: mov      r2, r7
0076cbd8: mov      r3, r8
0076cbdc: mov      r4, r0
0076cbe0: str      r6, [sp]
0076cbe4: bl       #0x7916bc
0076cbe8: b        #0x76cbb8
0076cbec: add      r1, sp, #0x10
0076cbf0: str      r4, [r1, #-4]!
0076cbf4: add      r0, r5, #0xc
0076cbf8: bl       #0x43a9c8
0076cbfc: b        #0x76cba0

# _ZN7gameswf23edit_text_character_def9get_boundEPNS_4rectE
0078a2e4: mov      ip, r1
0078a2e8: add      r0, r0, #0x24
0078a2ec: ldm      r0, {r0, r1, r2, r3}
0078a2f0: stm      ip, {r0, r1, r2, r3}
0078a2f4: bx       lr

# _ZNK7gameswf19edit_text_character2isEi
0078a2f8: cmp      r1, #0x20
0078a2fc: beq      #0x78a314
0078a300: cmp      r1, #1
0078a304: beq      #0x78a314
0078a308: rsbs     r0, r1, #1
0078a30c: movlo    r0, #0
0078a310: bx       lr
0078a314: mov      r0, #1
0078a318: bx       lr

# _ZN7gameswf19edit_text_character17get_character_defEv
0078a31c: ldr      r0, [r0, #0xa0]
0078a320: bx       lr

# _ZN7gameswf19edit_text_character22can_handle_mouse_eventEv
0078a338: ldr      r3, [r0, #0xa0]
0078a33c: ldrb     r0, [r3, #0x4b]
0078a340: eor      r0, r0, #1
0078a344: bx       lr

# _ZN7gameswf19edit_text_character8hit_testEff
0078a348: push     {r4, lr}
0078a34c: ldr      r3, [r0]
0078a350: mov      lr, pc
0078a354: ldr      pc, [r3, #0x68]
0078a358: subs     r0, r0, #0
0078a35c: movne    r0, #1
0078a360: pop      {r4, pc}

# _ZNK7gameswf19edit_text_character12get_var_nameEv
0078a364: ldr      r0, [r0, #0xa0]
0078a368: add      r0, r0, #0x34
0078a36c: bx       lr

# _ZN7gameswf19edit_text_character18reset_bounding_boxEff
0078a370: str      r2, [r0, #0x134]
0078a374: str      r1, [r0, #0x12c]
0078a378: str      r1, [r0, #0x128]
0078a37c: str      r2, [r0, #0x130]
0078a380: bx       lr

# _ZN7gameswf19edit_text_character7advanceEf
0078a384: push     {r4, lr}
0078a388: ldr      r3, [r0]
0078a38c: mov      lr, pc
0078a390: ldr      pc, [r3, #0xc]
0078a394: pop      {r4, pc}

# _ZN7gameswf19edit_text_character10align_lineENS_23edit_text_character_def9alignmentEif
0078a398: push     {r4, r5, r6, r7, r8, lr}
0078a39c: mov      r4, r0
0078a3a0: ldr      r0, [r0, #0xa0]
0078a3a4: subs     r5, r1, #0
0078a3a8: mov      r6, r2
0078a3ac: ldr      r1, [r0, #0x24]
0078a3b0: mov      r7, r3
0078a3b4: ldr      r0, [r0, #0x28]
0078a3b8: ldr      r8, [r4, #0x184]
0078a3bc: beq      #0x78a450
0078a3c0: bl       #0x30e3ac
0078a3c4: mov      r1, r8
0078a3c8: bl       #0x30e3ac
0078a3cc: mov      r1, r7
0078a3d0: bl       #0x30e3ac
0078a3d4: mov      r1, #0x42000000
0078a3d8: add      r1, r1, #0xa00000
0078a3dc: bl       #0x30e3ac
0078a3e0: cmp      r5, #2
0078a3e4: mov      r8, r0
0078a3e8: beq      #0x78a454
0078a3ec: cmp      r5, #1
0078a3f0: movne    r8, #0
0078a3f4: ldr      r2, [r4, #0xa8]
0078a3f8: cmp      r6, r2
0078a3fc: bge      #0x78a440
0078a400: mov      r5, #0x30
0078a404: mul      r5, r5, r6
0078a408: ldr      r7, [r4, #0xa4]
0078a40c: mov      r1, r8
0078a410: add      r6, r6, #1
0078a414: add      r7, r7, r5
0078a418: ldrb     r3, [r7, #0x1c]
0078a41c: add      r5, r5, #0x30
0078a420: cmp      r3, #0
0078a424: beq      #0x78a438
0078a428: ldr      r0, [r7, #0x10]
0078a42c: bl       #0x30eba4
0078a430: str      r0, [r7, #0x10]
0078a434: ldr      r2, [r4, #0xa8]
0078a438: cmp      r6, r2
0078a43c: blt      #0x78a408
0078a440: ldr      r0, [r4, #0x154]
0078a444: mov      r1, r8
0078a448: bl       #0x30eba4
0078a44c: str      r0, [r4, #0x154]
0078a450: pop      {r4, r5, r6, r7, r8, pc}
0078a454: mov      r1, #0x3f000000
0078a458: bl       #0x30ed6c
0078a45c: mov      r8, r0
0078a460: b        #0x78a3f4

# _ZN7gameswf5arrayINS_19edit_text_character15text_attributesEE7reserveEi
0078a67c: push     {r4, lr}
0078a680: ldrb     r3, [r0, #0xc]
0078a684: mov      r4, r0
0078a688: cmp      r3, #0
0078a68c: bne      #0x78a6d0
0078a690: cmp      r1, #0
0078a694: ldr      r2, [r0, #8]
0078a698: str      r1, [r0, #8]
0078a69c: bne      #0x78a6d4
0078a6a0: ldr      r0, [r0]
0078a6a4: cmp      r0, #0
0078a6a8: beq      #0x78a6b4
0078a6ac: lsl      r1, r2, #4
0078a6b0: bl       #0x752b38
0078a6b4: mov      r3, #0
0078a6b8: str      r3, [r4]
0078a6bc: pop      {r4, pc}
0078a6c0: lsl      r0, r1, #4
0078a6c4: mov      r1, ip
0078a6c8: bl       #0x752b9c
0078a6cc: str      r0, [r4]
0078a6d0: pop      {r4, pc}
0078a6d4: ldr      ip, [r0]
0078a6d8: cmp      ip, #0
0078a6dc: beq      #0x78a6c0
0078a6e0: mov      r0, ip
0078a6e4: lsl      r1, r1, #4
0078a6e8: lsl      r2, r2, #4
0078a6ec: bl       #0x752bac
0078a6f0: str      r0, [r4]
0078a6f4: pop      {r4, pc}

# _ZN7gameswf19edit_text_character12append_imageERKNS_9tu_stringEii
0078a990: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078a994: sub      sp, sp, #0x64
0078a998: ldr      ip, [r0]
0078a99c: mov      r4, r0
0078a9a0: mov      r5, r2
0078a9a4: mov      r6, r3
0078a9a8: mov      sl, r1
0078a9ac: mov      lr, pc
0078a9b0: ldr      pc, [ip, #0x84]
0078a9b4: ldr      r7, [pc, #0x38c]
0078a9b8: subs     r8, r0, #0
0078a9bc: add      r7, pc, r7
0078a9c0: beq      #0x78a9dc
0078a9c4: ldr      r3, [r8]
0078a9c8: mov      r1, #0x21
0078a9cc: mov      lr, pc
0078a9d0: ldr      pc, [r3, #8]
0078a9d4: cmp      r0, #0
0078a9d8: bne      #0x78acf8
0078a9dc: ldr      r3, [pc, #0x368]
0078a9e0: ldr      r3, [r7, r3]
0078a9e4: ldr      r3, [r3]
0078a9e8: cmp      r3, #0
0078a9ec: beq      #0x78acf0
0078a9f0: ldrsb    r2, [sl]
0078a9f4: mov      r1, r5
0078a9f8: cmn      r2, #1
0078a9fc: addne    r0, sl, #1
0078aa00: ldreq    r0, [sl, #0xc]
0078aa04: mov      r2, r6
0078aa08: blx      r3
0078aa0c: subs     r1, r0, #0
0078aa10: beq      #0x78acf0
0078aa14: ldr      r3, [pc, #0x334]
0078aa18: ldr      r3, [r7, r3]
0078aa1c: ldr      r3, [r3]
0078aa20: mov      r0, r3
0078aa24: ldr      r3, [r3]
0078aa28: mov      lr, pc
0078aa2c: ldr      pc, [r3, #0x18]
0078aa30: cmp      r5, #0
0078aa34: mov      r7, r0
0078aa38: ble      #0x78ad14
0078aa3c: cmp      r6, #0
0078aa40: ble      #0x78ad30
0078aa44: add      r3, sp, #0x3c
0078aa48: add      r0, r3, #4
0078aa4c: str      r3, [sp, #4]
0078aa50: mov      r3, #0x44000000
0078aa54: mvn      r8, #0
0078aa58: mov      r1, r7
0078aa5c: str      r3, [sp, #0x3c]
0078aa60: mov      r7, #0
0078aa64: mov      r3, #2
0078aa68: strb     r3, [sp, #0x5e]
0078aa6c: str      r7, [sp, #0x40]
0078aa70: str      r7, [sp, #0x54]
0078aa74: strh     r7, [sp, #0x58]
0078aa78: strh     r8, [sp, #0x5a]
0078aa7c: strh     r7, [sp, #0x5c]
0078aa80: bl       #0x77a740
0078aa84: mov      r0, r5
0078aa88: bl       #0x30e964
0078aa8c: mov      r1, #0x41000000
0078aa90: add      r1, r1, #0xa00000
0078aa94: bl       #0x30ed6c
0078aa98: mov      r5, #0
0078aa9c: mov      r3, #0x400
0078aaa0: mov      fp, r0
0078aaa4: mov      r0, r6
0078aaa8: strh     r3, [sp, #0x58]
0078aaac: str      r5, [sp, #0x44]
0078aab0: str      r5, [sp, #0x4c]
0078aab4: str      fp, [sp, #0x3c]
0078aab8: strh     r8, [sp, #0x5c]
0078aabc: str      fp, [sp, #0x48]
0078aac0: bl       #0x30e964
0078aac4: mov      r1, #0x41000000
0078aac8: add      r1, r1, #0xa00000
0078aacc: bl       #0x30ed6c
0078aad0: ldr      r1, [r4, #0x160]
0078aad4: str      r0, [sp, #0x50]
0078aad8: bl       #0x30eba4
0078aadc: ldr      r1, [r4, #0xa8]
0078aae0: mov      sb, r0
0078aae4: mov      r0, #0x3f800000
0078aae8: cmp      r1, r7
0078aaec: str      r0, [sp, #0x24]
0078aaf0: mov      r0, #1
0078aaf4: str      r5, [sp, #0x20]
0078aaf8: str      r5, [sp, #0x1c]
0078aafc: str      r8, [sp, #0xc]
0078ab00: strb     r8, [sp, #0x17]
0078ab04: strb     r0, [sp, #0x2a]
0078ab08: str      r7, [sp, #0x34]
0078ab0c: strb     r7, [sp, #0x38]
0078ab10: str      r7, [sp, #0x10]
0078ab14: strb     r8, [sp, #0x14]
0078ab18: strb     r8, [sp, #0x15]
0078ab1c: strb     r8, [sp, #0x16]
0078ab20: strb     r7, [sp, #0x18]
0078ab24: strb     r7, [sp, #0x28]
0078ab28: strb     r7, [sp, #0x29]
0078ab2c: str      r7, [sp, #0x2c]
0078ab30: str      r7, [sp, #0x30]
0078ab34: strle    sb, [sp, #0x20]
0078ab38: addle    r5, sp, #0xc
0078ab3c: ble      #0x78ac3c
0078ab40: mov      r3, #0x30
0078ab44: add      r1, r1, r8
0078ab48: ldr      r7, [r4, #0xa4]
0078ab4c: mul      r1, r3, r1
0078ab50: add      r5, sp, #0x60
0078ab54: ldr      r3, [r7, r1]
0078ab58: add      r7, r7, r1
0078ab5c: str      r3, [r5, #-0x54]!
0078ab60: add      r0, r5, #4
0078ab64: ldr      r1, [r7, #4]
0078ab68: bl       #0x764234
0078ab6c: ldr      r3, [r7, #8]
0078ab70: mov      r0, sb
0078ab74: str      r3, [sp, #0x14]
0078ab78: ldrb     r3, [r7, #0xc]
0078ab7c: strb     r3, [sp, #0x18]
0078ab80: ldr      r3, [r7, #0x10]
0078ab84: str      r3, [sp, #0x1c]
0078ab88: ldr      r6, [r7, #0x14]
0078ab8c: str      r6, [sp, #0x20]
0078ab90: ldr      r3, [r7, #0x18]
0078ab94: mov      r1, r6
0078ab98: str      r3, [sp, #0x24]
0078ab9c: ldrb     r3, [r7, #0x1c]
0078aba0: strb     r3, [sp, #0x28]
0078aba4: ldrb     r3, [r7, #0x1d]
0078aba8: strb     r3, [sp, #0x29]
0078abac: ldrb     r3, [r7, #0x1e]
0078abb0: strb     r3, [sp, #0x2a]
0078abb4: bl       #0x30e2f8
0078abb8: cmp      r0, #0
0078abbc: beq      #0x78ac3c
0078abc0: ldr      sl, [r4, #0xa8]
0078abc4: adds     r7, sl, r8
0078abc8: bmi      #0x78ac38
0078abcc: mov      r3, #0x30
0078abd0: mul      r7, r3, r7
0078abd4: ldr      r8, [r4, #0xa4]
0078abd8: mov      r0, r6
0078abdc: add      r8, r8, r7
0078abe0: ldr      r1, [r8, #0x14]
0078abe4: bl       #0x30df8c
0078abe8: cmp      r0, #0
0078abec: beq      #0x78ac38
0078abf0: sub      r6, sl, #2
0078abf4: mov      r3, #0x30
0078abf8: mul      r6, r3, r6
0078abfc: b        #0x78ac04
0078ac00: add      r8, r8, r7
0078ac04: cmp      sl, #1
0078ac08: str      sb, [r8, #0x14]
0078ac0c: beq      #0x78ac38
0078ac10: ldr      r8, [r4, #0xa4]
0078ac14: ldr      r1, [sp, #0x20]
0078ac18: sub      r7, r7, #0x30
0078ac1c: add      r3, r8, r6
0078ac20: ldr      r0, [r3, #0x14]
0078ac24: bl       #0x30df8c
0078ac28: cmp      r0, #0
0078ac2c: sub      sl, sl, #1
0078ac30: sub      r6, r6, #0x30
0078ac34: bne      #0x78ac00
0078ac38: str      sb, [sp, #0x20]
0078ac3c: ldr      r1, [r4, #0x188]
0078ac40: ldr      r0, [r4, #0x180]
0078ac44: bl       #0x30eba4
0078ac48: mov      r1, #0
0078ac4c: mov      r6, r0
0078ac50: bl       #0x30e2f8
0078ac54: cmp      r0, #0
0078ac58: moveq    r6, #0
0078ac5c: ldr      r0, [r4, #0x15c]
0078ac60: mov      r1, r6
0078ac64: bl       #0x30eba4
0078ac68: mov      r1, #0
0078ac6c: str      r0, [sp, #0x1c]
0078ac70: add      r0, r5, #4
0078ac74: bl       #0x764234
0078ac78: mov      lr, #0x44000000
0078ac7c: ldr      r0, [r4, #0x15c]
0078ac80: mvn      r3, #0
0078ac84: mov      ip, #0
0078ac88: mov      r2, #1
0078ac8c: add      lr, lr, #0x800000
0078ac90: mov      r1, fp
0078ac94: strb     r3, [sp, #0x17]
0078ac98: str      lr, [sp, #0x24]
0078ac9c: strb     r2, [sp, #0x29]
0078aca0: strb     ip, [sp, #0x2a]
0078aca4: strb     r3, [sp, #0x14]
0078aca8: strb     r3, [sp, #0x15]
0078acac: strb     r3, [sp, #0x16]
0078acb0: strb     ip, [sp, #0x18]
0078acb4: strb     r2, [sp, #0x28]
0078acb8: bl       #0x30eba4
0078acbc: str      r0, [r4, #0x15c]
0078acc0: ldr      r1, [sp, #4]
0078acc4: add      r0, r5, #0x20
0078acc8: bl       #0x78a81c
0078accc: add      r0, r4, #0xa4
0078acd0: mov      r1, r5
0078acd4: bl       #0x78a948
0078acd8: mov      r0, r5
0078acdc: bl       #0x78a5b8
0078ace0: ldr      r0, [sp, #0x40]
0078ace4: cmp      r0, #0
0078ace8: beq      #0x78acf0
0078acec: bl       #0x75a240
0078acf0: add      sp, sp, #0x64
0078acf4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078acf8: mov      r0, r8
0078acfc: ldr      r3, [r8]
0078ad00: mov      lr, pc
0078ad04: ldr      pc, [r3, #0x2c]
0078ad08: cmp      r5, #0
0078ad0c: mov      r7, r0
0078ad10: bgt      #0x78aa3c
0078ad14: ldr      r3, [r7]
0078ad18: mov      r0, r7
0078ad1c: mov      lr, pc
0078ad20: ldr      pc, [r3, #0x24]
0078ad24: cmp      r6, #0
0078ad28: mov      r5, r0
0078ad2c: bgt      #0x78aa44
0078ad30: ldr      r3, [r7]
0078ad34: mov      r0, r7
0078ad38: mov      lr, pc
0078ad3c: ldr      pc, [r3, #0x28]
0078ad40: mov      r6, r0
0078ad44: b        #0x78aa44

# _ZN7gameswf5arrayINS_19edit_text_character15text_attributesEE9push_backIS2_EEvRKT_
0078ad54: push     {r4, r5, r6, r7, r8, lr}
0078ad58: ldr      r3, [r0, #4]
0078ad5c: ldr      r2, [r0, #8]
0078ad60: mov      r4, r0
0078ad64: add      r6, r3, #1
0078ad68: cmp      r6, r2
0078ad6c: mov      r5, r1
0078ad70: bgt      #0x78adb0
0078ad74: ldr      r0, [r5]
0078ad78: ldr      r7, [r4]
0078ad7c: cmp      r0, #0
0078ad80: str      r0, [r7, r3, lsl #4]
0078ad84: add      r7, r7, r3, lsl #4
0078ad88: beq      #0x78ad90
0078ad8c: bl       #0x759c64
0078ad90: ldr      r3, [r5, #4]
0078ad94: str      r3, [r7, #4]
0078ad98: ldr      r3, [r5, #8]
0078ad9c: str      r3, [r7, #8]
0078ada0: ldrb     r3, [r5, #0xc]
0078ada4: strb     r3, [r7, #0xc]
0078ada8: str      r6, [r4, #4]
0078adac: pop      {r4, r5, r6, r7, r8, pc}
0078adb0: add      r1, r6, r6, asr #1
0078adb4: bl       #0x78a67c
0078adb8: ldr      r3, [r4, #4]
0078adbc: b        #0x78ad74

# _ZN7gameswf5arrayINS_9smart_ptrINS_9characterEEEE9push_backIPNS_19edit_text_characterEEEvRKT_
0078adc0: push     {r4, r5, r6, lr}
0078adc4: ldr      r3, [r0, #4]
0078adc8: ldr      r2, [r0, #8]
0078adcc: mov      r4, r0
0078add0: add      r5, r3, #1
0078add4: cmp      r5, r2
0078add8: mov      r6, r1
0078addc: bgt      #0x78ae00
0078ade0: ldr      r0, [r6]
0078ade4: ldr      r2, [r4]
0078ade8: cmp      r0, #0
0078adec: str      r0, [r2, r3, lsl #2]
0078adf0: beq      #0x78adf8
0078adf4: bl       #0x759c64
0078adf8: str      r5, [r4, #4]
0078adfc: pop      {r4, r5, r6, pc}
0078ae00: add      r1, r5, r5, asr #1
0078ae04: bl       #0x7557a0
0078ae08: ldr      r3, [r4, #4]
0078ae0c: b        #0x78ade0

# _ZN7gameswf19edit_text_character19update_world_cxformEv
0078ae10: push     {r4, lr}
0078ae14: mov      r4, r0
0078ae18: bl       #0x75488c
0078ae1c: mvn      r2, #0
0078ae20: mvn      r3, #0
0078ae24: strd     r2, r3, [r4, #0xe0]
0078ae28: strd     r2, r3, [r4, #0xd8]
0078ae2c: pop      {r4, pc}

# _ZN7gameswf19edit_text_character19update_world_matrixEv
0078ae30: push     {r4, lr}
0078ae34: mov      r4, r0
0078ae38: bl       #0x754c34
0078ae3c: mvn      r2, #0
0078ae40: mvn      r3, #0
0078ae44: strd     r2, r3, [r4, #0xe0]
0078ae48: strd     r2, r3, [r4, #0xd8]
0078ae4c: pop      {r4, pc}

# _ZN7gameswf19edit_text_character10get_memberERKNS_10tu_stringiEPNS_8as_valueE
0078aee0: push     {r4, r5, r6, r7, r8, sl, lr}
0078aee4: ldr      r4, [pc, #0x1c4]
0078aee8: ldr      r8, [pc, #0x1c4]
0078aeec: sub      sp, sp, #0x1c
0078aef0: add      r4, pc, r4
0078aef4: ldr      r3, [r4, r8]
0078aef8: mov      r7, r0
0078aefc: mov      r0, r1
0078af00: ldr      r3, [r3]
0078af04: mov      r5, r1
0078af08: mov      r6, r2
0078af0c: str      r3, [sp, #0x14]
0078af10: bl       #0x772030
0078af14: sub      r0, r0, #0x16
0078af18: cmp      r0, #9
0078af1c: addls    pc, pc, r0, lsl #2
0078af20: b        #0x78af78
0078af24: b        #0x78afc0
0078af28: b        #0x78afc0
0078af2c: b        #0x78afd0
0078af30: b        #0x78b000
0078af34: b        #0x78b00c
0078af38: b        #0x78b028
0078af3c: b        #0x78b03c
0078af40: b        #0x78b050
0078af44: b        #0x78b064
0078af48: b        #0x78af4c
0078af4c: ldrb     r0, [r7, #0x195]
0078af50: ldrb     r2, [r7, #0x194]
0078af54: ldrb     r3, [r7, #0x196]
0078af58: lsl      r0, r0, #8
0078af5c: orr      r0, r0, r2, lsl #16
0078af60: orr      r0, r0, r3
0078af64: bl       #0x30ed30
0078af68: mov      r2, r0
0078af6c: mov      r3, r1
0078af70: mov      r0, r6
0078af74: bl       #0x797488
0078af78: mov      r0, #6
0078af7c: mov      r1, r5
0078af80: mov      r2, r6
0078af84: bl       #0x76ce08
0078af88: cmp      r0, #0
0078af8c: movne    r0, #1
0078af90: bne      #0x78afa4
0078af94: mov      r0, r7
0078af98: mov      r1, r5
0078af9c: mov      r2, r6
0078afa0: bl       #0x75409c
0078afa4: ldr      r3, [r4, r8]
0078afa8: ldr      r2, [sp, #0x14]
0078afac: ldr      r3, [r3]
0078afb0: cmp      r2, r3
0078afb4: bne      #0x78b0ac
0078afb8: add      sp, sp, #0x1c
0078afbc: pop      {r4, r5, r6, r7, r8, sl, pc}
0078afc0: mov      r0, r6
0078afc4: add      r1, r7, #0x138
0078afc8: bl       #0x7972d8
0078afcc: b        #0x78af78
0078afd0: ldr      r1, [r7, #0x128]
0078afd4: ldr      r0, [r7, #0x12c]
0078afd8: bl       #0x30e3ac
0078afdc: mov      r1, #0x41000000
0078afe0: add      r1, r1, #0xa00000
0078afe4: bl       #0x30ec94
0078afe8: bl       #0x30e8a4
0078afec: mov      r2, r0
0078aff0: mov      r3, r1
0078aff4: mov      r0, r6
0078aff8: bl       #0x797488
0078affc: b        #0x78af78
0078b000: ldr      r1, [r7, #0x130]
0078b004: ldr      r0, [r7, #0x134]
0078b008: b        #0x78afd8
0078b00c: ldrb     r0, [r7, #0x171]
0078b010: ldrb     r2, [r7, #0x170]
0078b014: ldrb     r3, [r7, #0x172]
0078b018: lsl      r0, r0, #8
0078b01c: add      r0, r0, r2, lsl #16
0078b020: add      r0, r0, r3
0078b024: b        #0x78af64
0078b028: ldr      r3, [r7, #0xa0]
0078b02c: mov      r0, r6
0078b030: ldrb     r1, [r3, #0x4e]
0078b034: bl       #0x797230
0078b038: b        #0x78af78
0078b03c: ldr      r3, [r7, #0xa0]
0078b040: mov      r0, r6
0078b044: ldrb     r1, [r3, #0x49]
0078b048: bl       #0x797230
0078b04c: b        #0x78af78
0078b050: ldr      r3, [r7, #0xa0]
0078b054: mov      r0, r6
0078b058: ldrb     r1, [r3, #0x48]
0078b05c: bl       #0x797230
0078b060: b        #0x78af78
0078b064: ldr      r3, [r7, #0xa0]
0078b068: ldrb     r3, [r3, #0x4b]
0078b06c: cmp      r3, #0
0078b070: bne      #0x78b0a0
0078b074: ldr      r1, [pc, #0x3c]
0078b078: add      r1, pc, r1
0078b07c: mov      r0, sp
0078b080: bl       #0x413a7c
0078b084: mov      r0, r6
0078b088: mov      r1, sp
0078b08c: bl       #0x7972d8
0078b090: mov      r0, sp
0078b094: mov      sl, sp
0078b098: bl       #0x41fed8
0078b09c: b        #0x78af78
0078b0a0: ldr      r1, [pc, #0x14]
0078b0a4: add      r1, pc, r1
0078b0a8: b        #0x78b07c
0078b0ac: bl       #0x30e310
0078b0b0: eoreq    sb, r0, r0, lsr #23
0078b0b4: andeq    r4, r0, ip, lsr #1
0078b0b8: andseq   lr, r7, r8, lsr lr
0078b0bc: andseq   lr, r7, r4, lsl lr

# _ZN7gameswf19edit_text_character11show_cursorEv
0078b128: push     {r4, r5, r6, lr}
0078b12c: ldr      r3, [r0, #0x158]
0078b130: ldr      r2, [r0, #0x154]
0078b134: ldr      r1, [r0, #0x174]
0078b138: sub      sp, sp, #0x30
0078b13c: mov      r4, r0
0078b140: mov      r0, r3
0078b144: str      r2, [sp, #0x24]
0078b148: str      r2, [sp, #0x1c]
0078b14c: str      r3, [sp, #0x20]
0078b150: bl       #0x30eba4
0078b154: str      r0, [sp, #0x28]
0078b158: mov      r0, r4
0078b15c: bl       #0x753f74
0078b160: add      ip, sp, #4
0078b164: ldr      r4, [pc, #0xcc]
0078b168: mov      r5, r0
0078b16c: mov      r6, ip
0078b170: ldm      r5!, {r0, r1, r2, r3}
0078b174: stm      r6!, {r0, r1, r2, r3}
0078b178: ldr      r2, [pc, #0xbc]
0078b17c: add      r4, pc, r4
0078b180: ldr      r1, [r5, #4]
0078b184: ldr      r4, [r4, r2]
0078b188: ldr      r0, [r5]
0078b18c: str      r1, [r6, #4]
0078b190: ldr      r5, [r4]
0078b194: str      r0, [r6]
0078b198: cmp      r5, #0
0078b19c: beq      #0x78b230
0078b1a0: mov      r0, r5
0078b1a4: mov      r1, ip
0078b1a8: ldr      r3, [r5]
0078b1ac: mov      lr, pc
0078b1b0: ldr      pc, [r3, #0x50]
0078b1b4: ldr      r0, [r4]
0078b1b8: cmp      r0, #0
0078b1bc: beq      #0x78b230
0078b1c0: ldr      r3, [r0]
0078b1c4: mvn      r1, #0
0078b1c8: mov      r2, #0
0078b1cc: ldr      r3, [r3, #0x78]
0078b1d0: strb     r2, [sp, #0x2d]
0078b1d4: strb     r1, [sp, #0x2c]
0078b1d8: strb     r1, [sp, #0x2f]
0078b1dc: strb     r2, [sp, #0x2e]
0078b1e0: ldr      r1, [sp, #0x2c]
0078b1e4: blx      r3
0078b1e8: ldr      r3, [r4]
0078b1ec: cmp      r3, #0
0078b1f0: beq      #0x78b230
0078b1f4: mov      r1, #0x42000000
0078b1f8: mov      r0, r3
0078b1fc: add      r1, r1, #0x200000
0078b200: ldr      r3, [r3]
0078b204: mov      lr, pc
0078b208: ldr      pc, [r3, #0x7c]
0078b20c: ldr      r3, [r4]
0078b210: cmp      r3, #0
0078b214: beq      #0x78b230
0078b218: mov      r0, r3
0078b21c: add      r1, sp, #0x1c
0078b220: ldr      r3, [r3]
0078b224: mov      r2, #2
0078b228: mov      lr, pc
0078b22c: ldr      pc, [r3, #0x60]
0078b230: add      sp, sp, #0x30
0078b234: pop      {r4, r5, r6, pc}
0078b238: eoreq    sb, r0, r4, lsl sb
0078b23c: strheq   r3, [r0], -r4

# _ZN7gameswf23edit_text_character_def15csm_textsettingEPNS_6streamEi
0078b410: push     {r4, r5, r6, lr}
0078b414: mov      r4, r1
0078b418: mov      r5, r0
0078b41c: mov      r1, #2
0078b420: mov      r0, r4
0078b424: bl       #0x7839a4
0078b428: subs     r0, r0, #0
0078b42c: movne    r0, #1
0078b430: strb     r0, [r5, #0x90]
0078b434: mov      r1, #3
0078b438: mov      r0, r4
0078b43c: bl       #0x7839a4
0078b440: mov      r1, #3
0078b444: str      r0, [r5, #0x94]
0078b448: mov      r0, r4
0078b44c: bl       #0x7839a4
0078b450: mov      r0, r4
0078b454: bl       #0x783ea8
0078b458: str      r0, [r5, #0x98]
0078b45c: mov      r0, r4
0078b460: bl       #0x783ea8
0078b464: str      r0, [r5, #0x9c]
0078b468: mov      r0, r4
0078b46c: pop      {r4, r5, r6, lr}
0078b470: b        #0x783b28

# _ZN7gameswf23edit_text_character_def4readEPNS_6streamEiPNS_20movie_definition_subE
0078b610: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078b614: mov      r4, r1
0078b618: mov      r5, r0
0078b61c: add      r0, r0, #0x24
0078b620: bl       #0x795fec
0078b624: mov      r0, r4
0078b628: bl       #0x783b18
0078b62c: mov      r1, #1
0078b630: mov      r0, r4
0078b634: bl       #0x7839a4
0078b638: mov      r1, #1
0078b63c: mov      r6, r0
0078b640: mov      r0, r4
0078b644: bl       #0x7839a4
0078b648: subs     r0, r0, #0
0078b64c: movne    r0, #1
0078b650: strb     r0, [r5, #0x48]
0078b654: mov      r1, #1
0078b658: mov      r0, r4
0078b65c: bl       #0x7839a4
0078b660: subs     r0, r0, #0
0078b664: movne    r0, #1
0078b668: strb     r0, [r5, #0x49]
0078b66c: mov      r1, #1
0078b670: mov      r0, r4
0078b674: bl       #0x7839a4
0078b678: subs     r0, r0, #0
0078b67c: movne    r0, #1
0078b680: strb     r0, [r5, #0x4a]
0078b684: mov      r1, #1
0078b688: mov      r0, r4
0078b68c: bl       #0x7839a4
0078b690: subs     r0, r0, #0
0078b694: movne    r0, #1
0078b698: strb     r0, [r5, #0x4b]
0078b69c: mov      r1, #1
0078b6a0: mov      r0, r4
0078b6a4: bl       #0x7839a4
0078b6a8: mov      r1, #1
0078b6ac: mov      sl, r0
0078b6b0: mov      r0, r4
0078b6b4: bl       #0x7839a4
0078b6b8: mov      r1, #1
0078b6bc: mov      r8, r0
0078b6c0: mov      r0, r4
0078b6c4: bl       #0x7839a4
0078b6c8: mov      r1, #1
0078b6cc: mov      sb, r0
0078b6d0: mov      r0, r4
0078b6d4: bl       #0x7839a4
0078b6d8: mov      r1, #1
0078b6dc: mov      r0, r4
0078b6e0: bl       #0x7839a4
0078b6e4: subs     r0, r0, #0
0078b6e8: movne    r0, #1
0078b6ec: strb     r0, [r5, #0x4c]
0078b6f0: mov      r1, #1
0078b6f4: mov      r0, r4
0078b6f8: bl       #0x7839a4
0078b6fc: mov      r1, #1
0078b700: mov      r7, r0
0078b704: mov      r0, r4
0078b708: bl       #0x7839a4
0078b70c: subs     r0, r0, #0
0078b710: movne    r0, #1
0078b714: strb     r0, [r5, #0x4d]
0078b718: mov      r1, #1
0078b71c: mov      r0, r4
0078b720: bl       #0x7839a4
0078b724: subs     r0, r0, #0
0078b728: movne    r0, #1
0078b72c: strb     r0, [r5, #0x4e]
0078b730: mov      r1, #1
0078b734: mov      r0, r4
0078b738: bl       #0x7839a4
0078b73c: mov      r1, #1
0078b740: mov      r0, r4
0078b744: bl       #0x7839a4
0078b748: subs     r0, r0, #0
0078b74c: movne    r0, #1
0078b750: strb     r0, [r5, #0x4f]
0078b754: mov      r1, #1
0078b758: mov      r0, r4
0078b75c: bl       #0x7839a4
0078b760: subs     r0, r0, #0
0078b764: movne    r0, #1
0078b768: cmp      sb, #0
0078b76c: strb     r0, [r5, #0x50]
0078b770: bne      #0x78b838
0078b774: cmp      sl, #0
0078b778: bne      #0x78b828
0078b77c: cmp      r8, #0
0078b780: bne      #0x78b818
0078b784: cmp      r7, #0
0078b788: bne      #0x78b7c0
0078b78c: mov      r0, r4
0078b790: add      r1, r5, #0x34
0078b794: bl       #0x7841c8
0078b798: cmp      r6, #0
0078b79c: bne      #0x78b7a4
0078b7a0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0078b7a4: add      r5, r5, #0x7c
0078b7a8: mov      r0, r4
0078b7ac: mov      r1, r5
0078b7b0: bl       #0x7841c8
0078b7b4: mov      r0, r5
0078b7b8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0078b7bc: b        #0x78b4d8
0078b7c0: mov      r0, r4
0078b7c4: bl       #0x783b28
0078b7c8: str      r0, [r5, #0x68]
0078b7cc: mov      r0, r4
0078b7d0: bl       #0x783c14
0078b7d4: bl       #0x30e2e0
0078b7d8: str      r0, [r5, #0x6c]
0078b7dc: mov      r0, r4
0078b7e0: bl       #0x783c14
0078b7e4: bl       #0x30e2e0
0078b7e8: str      r0, [r5, #0x70]
0078b7ec: mov      r0, r4
0078b7f0: bl       #0x783c48
0078b7f4: sxth     r0, r0
0078b7f8: bl       #0x30e964
0078b7fc: str      r0, [r5, #0x74]
0078b800: mov      r0, r4
0078b804: bl       #0x783c48
0078b808: sxth     r0, r0
0078b80c: bl       #0x30e964
0078b810: str      r0, [r5, #0x78]
0078b814: b        #0x78b78c
0078b818: mov      r0, r4
0078b81c: bl       #0x783c14
0078b820: str      r0, [r5, #0x64]
0078b824: b        #0x78b784
0078b828: add      r0, r5, #0x60
0078b82c: mov      r1, r4
0078b830: bl       #0x796888
0078b834: b        #0x78b77c
0078b838: mov      r0, r4
0078b83c: bl       #0x783c14
0078b840: str      r0, [r5, #0x54]
0078b844: mov      r0, r4
0078b848: bl       #0x783c14
0078b84c: bl       #0x30e2e0
0078b850: str      r0, [r5, #0x5c]
0078b854: b        #0x78b774

# _ZN7gameswf19edit_text_character24get_topmost_mouse_entityEff
0078bb58: push     {r4, r5, r6, r7, lr}
0078bb5c: mov      r4, r0
0078bb60: ldrb     r0, [r0, #0x9b]
0078bb64: sub      sp, sp, #0x14
0078bb68: mov      ip, r1
0078bb6c: cmp      r0, #0
0078bb70: mov      r3, r2
0078bb74: bne      #0x78bb84
0078bb78: mov      r0, #0
0078bb7c: add      sp, sp, #0x14
0078bb80: pop      {r4, r5, r6, r7, pc}
0078bb84: ldr      r0, [r4, #0x4c]
0078bb88: mov      lr, #0
0078bb8c: add      r1, sp, #8
0078bb90: mov      r2, sp
0078bb94: str      lr, [sp, #0xc]
0078bb98: str      ip, [sp]
0078bb9c: str      r3, [sp, #4]
0078bba0: str      lr, [sp, #8]
0078bba4: bl       #0x753d7c
0078bba8: ldr      r5, [r4, #0xa0]
0078bbac: ldr      r6, [sp, #8]
0078bbb0: ldr      r7, [sp, #0xc]
0078bbb4: ldr      r1, [r5, #0x24]
0078bbb8: mov      r0, r6
0078bbbc: bl       #0x30e70c
0078bbc0: cmp      r0, #0
0078bbc4: bne      #0x78bb78
0078bbc8: mov      r0, r6
0078bbcc: ldr      r1, [r5, #0x28]
0078bbd0: bl       #0x30e2f8
0078bbd4: cmp      r0, #0
0078bbd8: bne      #0x78bb78
0078bbdc: mov      r0, r7
0078bbe0: ldr      r1, [r5, #0x2c]
0078bbe4: bl       #0x30e70c
0078bbe8: cmp      r0, #0
0078bbec: bne      #0x78bb78
0078bbf0: mov      r0, r7
0078bbf4: ldr      r1, [r5, #0x30]
0078bbf8: bl       #0x30e2f8
0078bbfc: cmp      r0, #0
0078bc00: moveq    r0, r4
0078bc04: beq      #0x78bb7c
0078bc08: b        #0x78bb78

# _ZN7gameswf5arrayINS_19edit_text_character15text_attributesEE6resizeEi
0078bc0c: push     {r4, r5, r6, r7, r8, lr}
0078bc10: ldr      r6, [r0, #4]
0078bc14: mov      r4, r0
0078bc18: mov      r5, r1
0078bc1c: cmp      r6, r1
0078bc20: ble      #0x78bc50
0078bc24: lsl      r8, r1, #4
0078bc28: mov      r7, r1
0078bc2c: ldr      r3, [r4]
0078bc30: add      r7, r7, #1
0078bc34: ldr      r0, [r3, r8]
0078bc38: add      r8, r8, #0x10
0078bc3c: cmp      r0, #0
0078bc40: beq      #0x78bc48
0078bc44: bl       #0x75a240
0078bc48: cmp      r7, r6
0078bc4c: bne      #0x78bc2c
0078bc50: cmp      r5, #0
0078bc54: beq      #0x78bc64
0078bc58: ldr      r3, [r4, #8]
0078bc5c: cmp      r5, r3
0078bc60: bgt      #0x78bcbc
0078bc64: cmp      r6, r5
0078bc68: bge      #0x78bcb4
0078bc6c: mov      r1, r6
0078bc70: mov      r2, #0
0078bc74: lsl      r6, r6, #4
0078bc78: mov      r7, #0xc
0078bc7c: mov      ip, #1
0078bc80: ldr      r0, [r4]
0078bc84: add      r1, r1, #1
0078bc88: cmp      r1, r5
0078bc8c: add      r3, r0, r6
0078bc90: str      r2, [r0, r6]
0078bc94: strb     r2, [r3, #0xc]
0078bc98: str      r7, [r3, #4]
0078bc9c: strb     r2, [r3, #8]
0078bca0: strb     r2, [r3, #9]
0078bca4: strb     r2, [r3, #0xa]
0078bca8: strb     ip, [r3, #0xb]
0078bcac: add      r6, r6, #0x10
0078bcb0: bne      #0x78bc80
0078bcb4: str      r5, [r4, #4]
0078bcb8: pop      {r4, r5, r6, r7, r8, pc}
0078bcbc: mov      r0, r4
0078bcc0: add      r1, r5, r5, asr #1
0078bcc4: bl       #0x78a67c
0078bcc8: b        #0x78bc64

# _ZN7gameswf19edit_text_character14preload_glyphsEPKNS_6filterE
0078c0ec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078c0f0: ldr      r3, [r0, #0xa8]
0078c0f4: mov      r6, #0
0078c0f8: sub      sp, sp, #0x24
0078c0fc: cmp      r3, r6
0078c100: mov      r4, r0
0078c104: str      r1, [sp, #0xc]
0078c108: str      r6, [sp, #0x10]
0078c10c: str      r6, [sp, #0x18]
0078c110: strb     r6, [sp, #0x1c]
0078c114: addle    r7, sp, #0x10
0078c118: ble      #0x78c290
0078c11c: str      r6, [sp, #8]
0078c120: ldr      sb, [r4, #0xa4]
0078c124: mov      r3, r6
0078c128: cmp      r3, #0
0078c12c: add      r7, sp, #0x10
0078c130: mov      r5, r6
0078c134: add      sb, sb, r6
0078c138: ble      #0x78c238
0078c13c: str      r5, [sp, #0x14]
0078c140: ldr      r3, [sb, #0x24]
0078c144: cmp      r3, #0
0078c148: movle    r3, r5
0078c14c: ble      #0x78c208
0078c150: mov      sl, r5
0078c154: mov      r3, r5
0078c158: b        #0x78c184
0078c15c: ldr      r2, [sp, #0x10]
0078c160: ldrh     fp, [fp, #0x20]
0078c164: lsl      r3, r3, #1
0078c168: add      sl, sl, #0x24
0078c16c: strh     fp, [r2, r3]
0078c170: str      r8, [sp, #0x14]
0078c174: ldr      r2, [sb, #0x24]
0078c178: mov      r3, r8
0078c17c: cmp      r8, r2
0078c180: bge      #0x78c1b0
0078c184: ldr      r2, [sp, #0x18]
0078c188: ldr      fp, [sb, #0x20]
0078c18c: add      r8, r3, #1
0078c190: cmp      r8, r2
0078c194: add      fp, fp, sl
0078c198: ble      #0x78c15c
0078c19c: mov      r0, r7
0078c1a0: add      r1, r8, r8, asr #1
0078c1a4: bl       #0x779e7c
0078c1a8: ldr      r3, [sp, #0x14]
0078c1ac: b        #0x78c15c
0078c1b0: ldr      r3, [r4, #0x30]
0078c1b4: cmp      r3, #0
0078c1b8: beq      #0x78c1cc
0078c1bc: ldr      r0, [r4, #0x2c]
0078c1c0: ldrb     r2, [r0, #4]
0078c1c4: cmp      r2, #0
0078c1c8: beq      #0x78c25c
0078c1cc: mov      r1, #0x41000000
0078c1d0: ldr      r0, [sb, #0x18]
0078c1d4: add      r1, r1, #0xa00000
0078c1d8: ldr      sl, [r3, #0xac]
0078c1dc: bl       #0x30ec94
0078c1e0: bl       #0x30e4cc
0078c1e4: ldr      ip, [sp, #0xc]
0078c1e8: ldr      r3, [sb, #4]
0078c1ec: mov      r2, r8
0078c1f0: str      r0, [sp]
0078c1f4: ldr      r1, [sp, #0x10]
0078c1f8: mov      r0, sl
0078c1fc: str      ip, [sp, #4]
0078c200: bl       #0x78b240
0078c204: ldr      r3, [sp, #0x14]
0078c208: ldr      r0, [sp, #8]
0078c20c: ldr      r2, [r4, #0xa8]
0078c210: add      r6, r6, #0x30
0078c214: add      r0, r0, #1
0078c218: cmp      r0, r2
0078c21c: str      r0, [sp, #8]
0078c220: bge      #0x78c288
0078c224: ldr      r3, [sp, #0x14]
0078c228: ldr      sb, [r4, #0xa4]
0078c22c: cmp      r3, #0
0078c230: add      sb, sb, r6
0078c234: bgt      #0x78c13c
0078c238: bge      #0x78c13c
0078c23c: lsl      r2, r3, #1
0078c240: ldr      r1, [sp, #0x10]
0078c244: mov      r0, #0
0078c248: adds     r3, r3, #1
0078c24c: strh     r0, [r1, r2]
0078c250: add      r2, r2, #2
0078c254: bne      #0x78c240
0078c258: b        #0x78c13c
0078c25c: ldr      r1, [r0]
0078c260: sub      r1, r1, #1
0078c264: cmp      r1, #0
0078c268: str      r1, [r0]
0078c26c: bne      #0x78c274
0078c270: bl       #0x752b38
0078c274: ldr      r8, [sp, #0x14]
0078c278: mov      r3, r5
0078c27c: str      r5, [r4, #0x2c]
0078c280: str      r5, [r4, #0x30]
0078c284: b        #0x78c1cc
0078c288: cmp      r3, #0
0078c28c: ble      #0x78c2ac
0078c290: mov      r3, #0
0078c294: mov      r0, r7
0078c298: mov      r1, r3
0078c29c: str      r3, [sp, #0x14]
0078c2a0: bl       #0x779e7c
0078c2a4: add      sp, sp, #0x24
0078c2a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078c2ac: bge      #0x78c290
0078c2b0: lsl      r2, r3, #1
0078c2b4: ldr      r1, [sp, #0x10]
0078c2b8: mov      ip, #0
0078c2bc: adds     r3, r3, #1
0078c2c0: strh     ip, [r1, r2]
0078c2c4: add      r2, r2, #2
0078c2c8: bne      #0x78c2b4
0078c2cc: b        #0x78c290

# _ZN7gameswf19edit_text_character14preload_glyphsEv
0078c2d0: push     {r4, r5, r6, lr}
0078c2d4: ldr      r3, [r0, #0x50]
0078c2d8: mov      r6, r0
0078c2dc: ldr      r2, [r3, #8]
0078c2e0: cmp      r2, #0
0078c2e4: ble      #0x78c318
0078c2e8: mov      r4, #0
0078c2ec: mov      r5, r4
0078c2f0: ldr      r1, [r3, #4]
0078c2f4: mov      r0, r6
0078c2f8: add      r5, r5, #1
0078c2fc: add      r1, r1, r4
0078c300: bl       #0x78c0ec
0078c304: ldr      r3, [r6, #0x50]
0078c308: add      r4, r4, #0x2c
0078c30c: ldr      r2, [r3, #8]
0078c310: cmp      r5, r2
0078c314: blt      #0x78c2f0
0078c318: mov      r0, r6
0078c31c: mov      r1, #0
0078c320: pop      {r4, r5, r6, lr}
0078c324: b        #0x78c0ec

# _ZN7gameswf19edit_text_characterD1Ev
0078c328: push     {r4, r5, r6, lr}
0078c32c: ldr      r3, [pc, #0xb8]
0078c330: ldr      r2, [pc, #0xb8]
0078c334: mov      r4, r0
0078c338: add      r3, pc, r3
0078c33c: ldr      r0, [r0, #0x178]
0078c340: ldr      r2, [r3, r2]
0078c344: cmp      r0, #0
0078c348: add      r2, r2, #8
0078c34c: str      r2, [r4]
0078c350: beq      #0x78c358
0078c354: bl       #0x75a240
0078c358: ldrb     r3, [r4, #0x138]
0078c35c: cmp      r3, #0xff
0078c360: beq      #0x78c3dc
0078c364: add      r5, r4, #0xc4
0078c368: add      r0, r4, #0xd8
0078c36c: bl       #0x78ba28
0078c370: mov      r0, r5
0078c374: bl       #0x78b8d0
0078c378: mov      r0, r5
0078c37c: mov      r1, #0
0078c380: add      r5, r4, #0xb4
0078c384: bl       #0x76150c
0078c388: mov      r0, r5
0078c38c: mov      r1, #0
0078c390: bl       #0x76146c
0078c394: mov      r0, r5
0078c398: mov      r1, #0
0078c39c: add      r5, r4, #0xa4
0078c3a0: bl       #0x7613e4
0078c3a4: mov      r1, #0
0078c3a8: mov      r0, r5
0078c3ac: bl       #0x78bccc
0078c3b0: mov      r0, r5
0078c3b4: mov      r1, #0
0078c3b8: bl       #0x78a5f4
0078c3bc: ldr      r0, [r4, #0xa0]
0078c3c0: cmp      r0, #0
0078c3c4: beq      #0x78c3cc
0078c3c8: bl       #0x75a240
0078c3cc: mov      r0, r4
0078c3d0: bl       #0x75dd74
0078c3d4: mov      r0, r4
0078c3d8: pop      {r4, r5, r6, pc}
0078c3dc: ldr      r0, [r4, #0x144]
0078c3e0: ldr      r1, [r4, #0x140]
0078c3e4: bl       #0x752b38
0078c3e8: b        #0x78c364
0078c3ec: eoreq    r8, r0, r8, asr r7
0078c3f0: andeq    r3, r0, r0, ror #30

# _ZN7gameswf19edit_text_characterD0Ev
0078c3f4: push     {r4, lr}
0078c3f8: mov      r4, r0
0078c3fc: bl       #0x78c328
0078c400: mov      r0, r4
0078c404: bl       #0x30e2b0
0078c408: mov      r0, r4
0078c40c: pop      {r4, pc}

# _ZN7gameswf19edit_text_characterD2Ev
0078c410: push     {r4, r5, r6, lr}
0078c414: ldr      r3, [pc, #0xb8]
0078c418: ldr      r2, [pc, #0xb8]
0078c41c: mov      r4, r0
0078c420: add      r3, pc, r3
0078c424: ldr      r0, [r0, #0x178]
0078c428: ldr      r2, [r3, r2]
0078c42c: cmp      r0, #0
0078c430: add      r2, r2, #8
0078c434: str      r2, [r4]
0078c438: beq      #0x78c440
0078c43c: bl       #0x75a240
0078c440: ldrb     r3, [r4, #0x138]
0078c444: cmp      r3, #0xff
0078c448: beq      #0x78c4c4
0078c44c: add      r5, r4, #0xc4
0078c450: add      r0, r4, #0xd8
0078c454: bl       #0x78ba28
0078c458: mov      r0, r5
0078c45c: bl       #0x78b8d0
0078c460: mov      r0, r5
0078c464: mov      r1, #0
0078c468: add      r5, r4, #0xb4
0078c46c: bl       #0x76150c
0078c470: mov      r0, r5
0078c474: mov      r1, #0
0078c478: bl       #0x76146c
0078c47c: mov      r0, r5
0078c480: mov      r1, #0
0078c484: add      r5, r4, #0xa4
0078c488: bl       #0x7613e4
0078c48c: mov      r1, #0
0078c490: mov      r0, r5
0078c494: bl       #0x78bccc
0078c498: mov      r0, r5
0078c49c: mov      r1, #0
0078c4a0: bl       #0x78a5f4
0078c4a4: ldr      r0, [r4, #0xa0]
0078c4a8: cmp      r0, #0
0078c4ac: beq      #0x78c4b4
0078c4b0: bl       #0x75a240
0078c4b4: mov      r0, r4
0078c4b8: bl       #0x75dd74
0078c4bc: mov      r0, r4
0078c4c0: pop      {r4, r5, r6, pc}
0078c4c4: ldr      r0, [r4, #0x144]
0078c4c8: ldr      r1, [r4, #0x140]
0078c4cc: bl       #0x752b38
0078c4d0: b        #0x78c44c
0078c4d4: eoreq    r8, r0, r0, ror r6
0078c4d8: andeq    r3, r0, r0, ror #30

# _ZN7gameswf23edit_text_character_defD1Ev
0078c4dc: push     {r4, lr}
0078c4e0: ldr      r3, [pc, #0x70]
0078c4e4: ldr      r2, [pc, #0x70]
0078c4e8: ldrsb    r1, [r0, #0x7c]
0078c4ec: add      r3, pc, r3
0078c4f0: ldr      r2, [r3, r2]
0078c4f4: cmn      r1, #1
0078c4f8: mov      r4, r0
0078c4fc: add      r2, r2, #8
0078c500: str      r2, [r0]
0078c504: beq      #0x78c524
0078c508: ldrsb    r3, [r4, #0x34]
0078c50c: cmn      r3, #1
0078c510: beq      #0x78c53c
0078c514: mov      r0, r4
0078c518: bl       #0x75de78
0078c51c: mov      r0, r4
0078c520: pop      {r4, pc}
0078c524: ldr      r0, [r0, #0x88]
0078c528: ldr      r1, [r4, #0x84]
0078c52c: bl       #0x752b38
0078c530: ldrsb    r3, [r4, #0x34]
0078c534: cmn      r3, #1
0078c538: bne      #0x78c514
0078c53c: ldr      r0, [r4, #0x40]
0078c540: ldr      r1, [r4, #0x3c]
0078c544: bl       #0x752b38
0078c548: mov      r0, r4
0078c54c: bl       #0x75de78
0078c550: mov      r0, r4
0078c554: pop      {r4, pc}
0078c558: eoreq    r8, r0, r4, lsr #11
0078c55c: andeq    r1, r0, ip, ror #12

# _ZN7gameswf23edit_text_character_defD0Ev
0078c560: push     {r4, lr}
0078c564: mov      r4, r0
0078c568: bl       #0x78c4dc
0078c56c: mov      r0, r4
0078c570: bl       #0x30e2b0
0078c574: mov      r0, r4
0078c578: pop      {r4, pc}

# _ZN7gameswf23edit_text_character_defD2Ev
0078c57c: push     {r4, lr}
0078c580: ldr      r3, [pc, #0x70]
0078c584: ldr      r2, [pc, #0x70]
0078c588: ldrsb    r1, [r0, #0x7c]
0078c58c: add      r3, pc, r3
0078c590: ldr      r2, [r3, r2]
0078c594: cmn      r1, #1
0078c598: mov      r4, r0
0078c59c: add      r2, r2, #8
0078c5a0: str      r2, [r0]
0078c5a4: beq      #0x78c5c4
0078c5a8: ldrsb    r3, [r4, #0x34]
0078c5ac: cmn      r3, #1
0078c5b0: beq      #0x78c5dc
0078c5b4: mov      r0, r4
0078c5b8: bl       #0x75de78
0078c5bc: mov      r0, r4
0078c5c0: pop      {r4, pc}
0078c5c4: ldr      r0, [r0, #0x88]
0078c5c8: ldr      r1, [r4, #0x84]
0078c5cc: bl       #0x752b38
0078c5d0: ldrsb    r3, [r4, #0x34]
0078c5d4: cmn      r3, #1
0078c5d8: bne      #0x78c5b4
0078c5dc: ldr      r0, [r4, #0x40]
0078c5e0: ldr      r1, [r4, #0x3c]
0078c5e4: bl       #0x752b38
0078c5e8: mov      r0, r4
0078c5ec: bl       #0x75de78
0078c5f0: mov      r0, r4
0078c5f4: pop      {r4, pc}
0078c5f8: eoreq    r8, r0, r4, lsl #10
0078c5fc: andeq    r1, r0, ip, ror #12

# _ZN7gameswf23edit_text_character_def25create_character_instanceEPNS_9characterEi
0078cad4: push     {r4, r5, r6, lr}
0078cad8: ldr      r3, [r0, #0x58]
0078cadc: mov      r4, r0
0078cae0: mov      r6, r1
0078cae4: cmp      r3, #0
0078cae8: mov      r5, r2
0078caec: beq      #0x78cb4c
0078caf0: ldr      r0, [r4, #0x1c]
0078caf4: cmp      r0, #0
0078caf8: beq      #0x78cb0c
0078cafc: ldr      r3, [r4, #0x18]
0078cb00: ldrb     r2, [r3, #4]
0078cb04: cmp      r2, #0
0078cb08: beq      #0x78cb20
0078cb0c: mov      r1, r4
0078cb10: mov      r2, r6
0078cb14: mov      r3, r5
0078cb18: pop      {r4, r5, r6, lr}
0078cb1c: b        #0x76cb44
0078cb20: ldr      r1, [r3]
0078cb24: sub      r1, r1, #1
0078cb28: cmp      r1, #0
0078cb2c: str      r1, [r3]
0078cb30: bne      #0x78cb3c
0078cb34: mov      r0, r3
0078cb38: bl       #0x752b38
0078cb3c: mov      r0, #0
0078cb40: str      r0, [r4, #0x18]
0078cb44: str      r0, [r4, #0x1c]
0078cb48: b        #0x78cb0c
0078cb4c: ldr      r3, [r0, #0x20]
0078cb50: cmp      r3, #0
0078cb54: beq      #0x78caf0
0078cb58: mov      r0, r3
0078cb5c: ldr      r1, [r4, #0x54]
0078cb60: ldr      r3, [r3]
0078cb64: mov      lr, pc
0078cb68: ldr      pc, [r3, #0x7c]
0078cb6c: cmp      r0, #0
0078cb70: str      r0, [r4, #0x58]
0078cb74: bne      #0x78caf0
0078cb78: ldr      r0, [pc, #0xc]
0078cb7c: ldr      r1, [r4, #0x54]
0078cb80: add      r0, pc, r0
0078cb84: bl       #0x761184
0078cb88: b        #0x78caf0
0078cb8c: ldrsheq  sp, [r7], -r8

# _ZN7gameswf19edit_text_character11append_textERKNS_9tu_stringERNS0_15text_attributesEb
0078cb90: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078cb94: mov      r4, r0
0078cb98: sub      sp, sp, #0xa4
0078cb9c: ldr      r0, [r2, #4]
0078cba0: mov      r7, r2
0078cba4: str      r3, [sp, #0x34]
0078cba8: mov      fp, r1
0078cbac: bl       #0x30e964
0078cbb0: ldr      r3, [r4, #0x30]
0078cbb4: mov      r5, r0
0078cbb8: cmp      r3, #0
0078cbbc: beq      #0x78cbd0
0078cbc0: ldr      r0, [r4, #0x2c]
0078cbc4: ldrb     r2, [r0, #4]
0078cbc8: cmp      r2, #0
0078cbcc: beq      #0x78d69c
0078cbd0: ldr      r3, [r3, #0xac]
0078cbd4: mov      r1, #0x44000000
0078cbd8: add      r1, r1, #0x800000
0078cbdc: ldr      r3, [r3, #0xc]
0078cbe0: ldr      r0, [r3, #4]
0078cbe4: bl       #0x30ed6c
0078cbe8: mov      r1, r0
0078cbec: mov      r0, r5
0078cbf0: bl       #0x30ec94
0078cbf4: str      r0, [sp, #4]
0078cbf8: ldr      r5, [r7]
0078cbfc: ldr      r3, [r5, #0x7c]
0078cc00: cmp      r3, #0
0078cc04: beq      #0x78cc18
0078cc08: mov      r1, #0x41000000
0078cc0c: add      r1, r1, #0xa00000
0078cc10: bl       #0x30ec94
0078cc14: str      r0, [sp, #4]
0078cc18: mov      r0, r5
0078cc1c: bl       #0x7cf41c
0078cc20: mov      r6, r0
0078cc24: ldr      r0, [r7, #4]
0078cc28: bl       #0x30e964
0078cc2c: mov      r5, r0
0078cc30: ldr      r0, [r7]
0078cc34: bl       #0x7cf520
0078cc38: mov      r1, r6
0078cc3c: bl       #0x30ec94
0078cc40: mov      r1, r0
0078cc44: mov      r0, r5
0078cc48: bl       #0x30ed6c
0078cc4c: mov      r1, #0
0078cc50: str      r0, [sp, #0x24]
0078cc54: bl       #0x30df8c
0078cc58: cmp      r0, #0
0078cc5c: beq      #0x78cc6c
0078cc60: ldr      r0, [r7, #4]
0078cc64: bl       #0x30e964
0078cc68: str      r0, [sp, #0x24]
0078cc6c: mov      ip, #0x3f800000
0078cc70: ldr      r5, [r7]
0078cc74: ldr      r0, [r7, #4]
0078cc78: mov      r3, #0
0078cc7c: mvn      r2, #0
0078cc80: mov      r1, #0
0078cc84: str      ip, [sp, #0x60]
0078cc88: mov      ip, #1
0078cc8c: strb     r3, [sp, #0x74]
0078cc90: str      r3, [sp, #0x4c]
0078cc94: strb     r3, [sp, #0x54]
0078cc98: strb     r3, [sp, #0x64]
0078cc9c: strb     r3, [sp, #0x65]
0078cca0: str      r3, [sp, #0x68]
0078cca4: str      r3, [sp, #0x6c]
0078cca8: str      r3, [sp, #0x70]
0078ccac: strb     r2, [sp, #0x53]
0078ccb0: strb     ip, [sp, #0x66]
0078ccb4: str      r2, [sp, #0x48]
0078ccb8: strb     r2, [sp, #0x50]
0078ccbc: strb     r2, [sp, #0x51]
0078ccc0: strb     r2, [sp, #0x52]
0078ccc4: str      r1, [sp, #0x5c]
0078ccc8: str      r1, [sp, #0x58]
0078cccc: bl       #0x30e964
0078ccd0: ldr      r1, [r4, #0x160]
0078ccd4: bl       #0x30eba4
0078ccd8: ldr      r1, [r5, #0x58]
0078ccdc: mov      r6, r0
0078cce0: ldr      r0, [r5, #0x5c]
0078cce4: bl       #0x30e3ac
0078cce8: ldr      r1, [sp, #4]
0078ccec: bl       #0x30ed6c
0078ccf0: mov      r1, r0
0078ccf4: mov      r0, r6
0078ccf8: bl       #0x30eba4
0078ccfc: ldr      r3, [r4, #0xa8]
0078cd00: mov      sl, r0
0078cd04: cmp      r3, #0
0078cd08: ble      #0x78d610
0078cd0c: mov      r1, #0x30
0078cd10: sub      r3, r3, #1
0078cd14: ldr      r6, [r4, #0xa4]
0078cd18: mul      r3, r1, r3
0078cd1c: add      r2, sp, #0xa0
0078cd20: str      r2, [sp, #8]
0078cd24: ldr      r2, [r6, r3]
0078cd28: add      r6, r6, r3
0078cd2c: ldr      r3, [sp, #8]
0078cd30: str      r2, [r3, #-0x58]!
0078cd34: str      r3, [sp, #8]
0078cd38: add      r0, r3, #4
0078cd3c: ldr      r1, [r6, #4]
0078cd40: bl       #0x764234
0078cd44: ldr      r3, [r6, #8]
0078cd48: mov      r0, sl
0078cd4c: str      r3, [sp, #0x50]
0078cd50: ldrb     r3, [r6, #0xc]
0078cd54: strb     r3, [sp, #0x54]
0078cd58: ldr      r3, [r6, #0x10]
0078cd5c: str      r3, [sp, #0x58]
0078cd60: ldr      r5, [r6, #0x14]
0078cd64: str      r5, [sp, #0x5c]
0078cd68: ldr      r3, [r6, #0x18]
0078cd6c: mov      r1, r5
0078cd70: str      r3, [sp, #0x60]
0078cd74: ldrb     r3, [r6, #0x1c]
0078cd78: strb     r3, [sp, #0x64]
0078cd7c: ldrb     r3, [r6, #0x1d]
0078cd80: strb     r3, [sp, #0x65]
0078cd84: ldrb     r3, [r6, #0x1e]
0078cd88: strb     r3, [sp, #0x66]
0078cd8c: bl       #0x30e2f8
0078cd90: cmp      r0, #0
0078cd94: bne      #0x78d590
0078cd98: ldr      r5, [r7]
0078cd9c: ldr      r1, [r4, #0x188]
0078cda0: ldr      r0, [r4, #0x180]
0078cda4: bl       #0x30eba4
0078cda8: mov      r1, #0
0078cdac: mov      r6, r0
0078cdb0: bl       #0x30e2f8
0078cdb4: ldr      r2, [sp, #8]
0078cdb8: cmp      r0, #0
0078cdbc: moveq    r6, #0
0078cdc0: mov      r1, r5
0078cdc4: add      r0, r2, #4
0078cdc8: str      r6, [sp, #0x58]
0078cdcc: bl       #0x764234
0078cdd0: ldr      r2, [r7, #8]
0078cdd4: ldrb     r3, [r7, #0xc]
0078cdd8: ldr      r0, [r7, #4]
0078cddc: str      r2, [sp, #0x50]
0078cde0: strb     r3, [sp, #0x54]
0078cde4: bl       #0x30e964
0078cde8: str      r0, [sp, #0x60]
0078cdec: ldr      r1, [r4, #0x15c]
0078cdf0: mov      r3, #1
0078cdf4: ldr      r0, [sp, #0x58]
0078cdf8: strb     r3, [sp, #0x66]
0078cdfc: strb     r3, [sp, #0x64]
0078ce00: strb     r3, [sp, #0x65]
0078ce04: bl       #0x30eba4
0078ce08: ldr      r1, [sp, #0x5c]
0078ce0c: str      r0, [sp, #0x1c]
0078ce10: ldr      r3, [r7]
0078ce14: str      r0, [sp, #0x58]
0078ce18: str      r1, [sp, #0x28]
0078ce1c: ldr      r5, [r4, #0x18c]
0078ce20: ldr      r1, [r3, #0x5c]
0078ce24: ldr      r0, [sp, #4]
0078ce28: bl       #0x30ed6c
0078ce2c: mov      r1, r5
0078ce30: bl       #0x30eba4
0078ce34: ldr      r2, [sp, #0x1c]
0078ce38: str      r0, [sp, #0x30]
0078ce3c: mov      r1, #0
0078ce40: str      r2, [r4, #0x154]
0078ce44: ldr      r3, [sp, #0x28]
0078ce48: add      r2, sp, #0xa0
0078ce4c: mvn      sl, #0
0078ce50: str      r3, [r4, #0x158]
0078ce54: ldrsb    r3, [fp]
0078ce58: ldr      r6, [sp, #0x1c]
0078ce5c: cmn      r3, #1
0078ce60: ldr      r3, [pc, #0x85c]
0078ce64: ldreq    fp, [fp, #0xc]
0078ce68: addne    fp, fp, #1
0078ce6c: add      r3, pc, r3
0078ce70: str      r3, [sp, #0x2c]
0078ce74: ldr      r3, [pc, #0x84c]
0078ce78: str      r1, [sp, #0x14]
0078ce7c: str      fp, [r2, #-4]!
0078ce80: add      r3, pc, r3
0078ce84: str      r3, [sp, #0x38]
0078ce88: ldr      r3, [pc, #0x83c]
0078ce8c: ldr      r1, [sp, #0x28]
0078ce90: str      r2, [sp, #0x10]
0078ce94: add      r3, pc, r3
0078ce98: str      r3, [sp, #0x3c]
0078ce9c: ldr      r0, [sp, #0x10]
0078cea0: add      r3, r4, #0xa4
0078cea4: str      r3, [sp, #0x20]
0078cea8: str      r1, [sp, #0xc]
0078ceac: bl       #0x752494
0078ceb0: subs     r8, r0, #0
0078ceb4: beq      #0x78cf24
0078ceb8: mov      r2, r8
0078cebc: mov      r1, sl
0078cec0: ldr      r0, [r7]
0078cec4: bl       #0x7ce4a8
0078cec8: ldr      r1, [sp, #4]
0078cecc: bl       #0x30ed6c
0078ced0: mov      r1, r0
0078ced4: mov      r0, r6
0078ced8: bl       #0x30eba4
0078cedc: cmp      r8, #0xa
0078cee0: movne    r3, #0
0078cee4: moveq    r3, #1
0078cee8: cmp      r8, #0xa
0078ceec: cmpne    r8, #0xd
0078cef0: mov      r5, r8
0078cef4: mov      r6, r0
0078cef8: bne      #0x78d0a8
0078cefc: cmp      sl, #0xd
0078cf00: movne    sl, #0
0078cf04: andeq    sl, r3, #1
0078cf08: cmp      sl, #0
0078cf0c: beq      #0x78cfd8
0078cf10: mov      sl, r5
0078cf14: ldr      r0, [sp, #0x10]
0078cf18: bl       #0x752494
0078cf1c: subs     r8, r0, #0
0078cf20: bne      #0x78ceb8
0078cf24: ldr      r3, [r7]
0078cf28: ldr      r0, [sp, #4]
0078cf2c: ldr      r1, [r3, #0x5c]
0078cf30: bl       #0x30ed6c
0078cf34: mov      r1, r0
0078cf38: ldr      r0, [r4, #0x154]
0078cf3c: bl       #0x30eba4
0078cf40: str      r0, [r4, #0x154]
0078cf44: ldr      r0, [r7, #4]
0078cf48: bl       #0x30e964
0078cf4c: ldr      r5, [r7]
0078cf50: mov      r7, r0
0078cf54: ldr      r1, [r5, #0x58]
0078cf58: ldr      r0, [r5, #0x5c]
0078cf5c: bl       #0x30e3ac
0078cf60: ldr      r1, [sp, #4]
0078cf64: bl       #0x30ed6c
0078cf68: mov      r1, r0
0078cf6c: mov      r0, r7
0078cf70: bl       #0x30eba4
0078cf74: mov      r1, r0
0078cf78: ldr      r0, [r4, #0x158]
0078cf7c: bl       #0x30e3ac
0078cf80: str      r0, [r4, #0x158]
0078cf84: ldr      r0, [sp, #0x20]
0078cf88: ldr      r1, [sp, #8]
0078cf8c: bl       #0x78a948
0078cf90: ldr      r1, [sp, #0x1c]
0078cf94: mov      r0, r6
0078cf98: bl       #0x30e3ac
0078cf9c: mov      r1, r0
0078cfa0: ldr      r0, [r4, #0x15c]
0078cfa4: bl       #0x30eba4
0078cfa8: str      r0, [r4, #0x15c]
0078cfac: ldr      r1, [sp, #0x28]
0078cfb0: ldr      r0, [sp, #0xc]
0078cfb4: bl       #0x30e3ac
0078cfb8: mov      r1, r0
0078cfbc: ldr      r0, [r4, #0x160]
0078cfc0: bl       #0x30eba4
0078cfc4: str      r0, [r4, #0x160]
0078cfc8: ldr      r0, [sp, #8]
0078cfcc: bl       #0x78a5b8
0078cfd0: add      sp, sp, #0xa4
0078cfd4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078cfd8: ldr      r1, [sp, #8]
0078cfdc: ldr      r0, [sp, #0x20]
0078cfe0: bl       #0x78a948
0078cfe4: mov      r3, r6
0078cfe8: ldr      r2, [r4, #0x164]
0078cfec: mov      r0, r4
0078cff0: ldr      r1, [r4, #0x17c]
0078cff4: bl       #0x78a398
0078cff8: ldr      r1, [r4, #0x188]
0078cffc: ldr      r0, [r4, #0x180]
0078d000: bl       #0x30eba4
0078d004: mov      r1, #0
0078d008: mov      r6, r0
0078d00c: bl       #0x30e2f8
0078d010: ldr      r1, [sp, #0x30]
0078d014: cmp      r0, #0
0078d018: ldr      r0, [sp, #0x24]
0078d01c: moveq    r6, #0
0078d020: bl       #0x30eba4
0078d024: mov      r1, r0
0078d028: ldr      r0, [sp, #0xc]
0078d02c: bl       #0x30eba4
0078d030: ldr      r2, [sp, #8]
0078d034: str      r0, [sp, #0xc]
0078d038: mov      r1, #0
0078d03c: add      r0, r2, #0x20
0078d040: bl       #0x78a4ec
0078d044: ldr      r3, [sp, #8]
0078d048: ldr      r1, [r7]
0078d04c: mov      sl, r5
0078d050: add      r0, r3, #4
0078d054: bl       #0x764234
0078d058: ldr      r2, [r7, #8]
0078d05c: ldrb     r3, [r7, #0xc]
0078d060: ldr      r1, [sp, #0xc]
0078d064: ldr      r0, [r7, #4]
0078d068: str      r2, [sp, #0x50]
0078d06c: strb     r3, [sp, #0x54]
0078d070: str      r1, [sp, #0x5c]
0078d074: str      r6, [sp, #0x58]
0078d078: bl       #0x30e964
0078d07c: ldr      r2, [r4, #0xa8]
0078d080: mov      r3, #1
0078d084: mvn      r1, #0
0078d088: str      r0, [sp, #0x60]
0078d08c: strb     r3, [sp, #0x66]
0078d090: str      r1, [r4, #0x16c]
0078d094: str      r2, [r4, #0x164]
0078d098: strb     r3, [sp, #0x64]
0078d09c: strb     r3, [sp, #0x65]
0078d0a0: str      r2, [r4, #0x168]
0078d0a4: b        #0x78cf14
0078d0a8: cmp      r8, #8
0078d0ac: beq      #0x78d334
0078d0b0: cmp      r8, #0x11
0078d0b4: moveq    r2, #0
0078d0b8: streq    r2, [sp, #0x18]
0078d0bc: beq      #0x78d314
0078d0c0: cmp      r8, #0x20
0078d0c4: beq      #0x78d30c
0078d0c8: cmp      r8, #0xa0
0078d0cc: bne      #0x78d520
0078d0d0: mov      r5, #0x20
0078d0d4: mov      r1, #0x3f800000
0078d0d8: mov      sb, r5
0078d0dc: str      r1, [sp, #0x18]
0078d0e0: mov      r8, r5
0078d0e4: mov      r3, #0x44000000
0078d0e8: ldr      r0, [r7, #4]
0078d0ec: mov      r1, #0
0078d0f0: mvn      r2, #0
0078d0f4: str      r3, [sp, #0x78]
0078d0f8: mov      r3, #0
0078d0fc: strh     r2, [sp, #0x96]
0078d100: strh     r3, [sp, #0x98]
0078d104: str      r1, [sp, #0x7c]
0078d108: str      r1, [sp, #0x90]
0078d10c: strh     r1, [sp, #0x94]
0078d110: strb     r1, [sp, #0x9a]
0078d114: bl       #0x30e964
0078d118: mov      r1, #0x41000000
0078d11c: add      r1, r1, #0xa00000
0078d120: bl       #0x30ec94
0078d124: bl       #0x30e4cc
0078d128: ldr      fp, [r7]
0078d12c: add      sl, sp, #0x78
0078d130: mov      r3, r0
0078d134: mov      r1, sl
0078d138: mov      r0, fp
0078d13c: mov      r2, sb
0078d140: bl       #0x7d01bc
0078d144: cmp      r0, #0
0078d148: bne      #0x78d184
0078d14c: ldr      r2, [sp, #0x2c]
0078d150: ldr      r3, [r2]
0078d154: cmp      r3, #9
0078d158: bgt      #0x78d184
0078d15c: add      r3, r3, #1
0078d160: str      r3, [r2]
0078d164: ldr      r2, [r7]
0078d168: ldr      r0, [sp, #0x38]
0078d16c: mov      r1, r8
0078d170: ldrsb    r3, [r2, #0x30]
0078d174: cmn      r3, #1
0078d178: addne    r2, r2, #0x31
0078d17c: ldreq    r2, [r2, #0x3c]
0078d180: bl       #0x761184
0078d184: ldr      r1, [r4, #0x190]
0078d188: ldr      r0, [sp, #0x78]
0078d18c: bl       #0x30eba4
0078d190: ldr      r1, [sp, #0x18]
0078d194: mov      fp, r0
0078d198: ldr      r0, [sp, #4]
0078d19c: bl       #0x30ed6c
0078d1a0: mov      r1, r0
0078d1a4: mov      r0, fp
0078d1a8: bl       #0x30ed6c
0078d1ac: cmp      r8, #0x1000
0078d1b0: str      r0, [sp, #0x78]
0078d1b4: bls      #0x78d1c8
0078d1b8: movw     r1, #0xcccd
0078d1bc: movt     r1, #0x3f8c
0078d1c0: bl       #0x30ed6c
0078d1c4: str      r0, [sp, #0x78]
0078d1c8: ldr      r0, [r7, #4]
0078d1cc: bl       #0x30e964
0078d1d0: mov      r1, #0x41000000
0078d1d4: add      r1, r1, #0xa00000
0078d1d8: bl       #0x30ec94
0078d1dc: bl       #0x30e4cc
0078d1e0: ldr      r3, [sp, #8]
0078d1e4: mov      r1, sl
0078d1e8: strh     r0, [sp, #0x94]
0078d1ec: add      r8, r3, #0x20
0078d1f0: mov      r0, r8
0078d1f4: strh     sb, [sp, #0x98]
0078d1f8: bl       #0x78a81c
0078d1fc: mov      r0, r6
0078d200: ldr      r1, [sp, #0x78]
0078d204: bl       #0x30eba4
0078d208: ldr      r3, [r4, #0xa0]
0078d20c: mov      sl, r0
0078d210: ldr      r1, [r3, #0x24]
0078d214: ldr      r0, [r3, #0x28]
0078d218: bl       #0x30e3ac
0078d21c: ldr      r1, [r4, #0x184]
0078d220: bl       #0x30e3ac
0078d224: mov      r1, #0x42000000
0078d228: add      r1, r1, #0xa00000
0078d22c: bl       #0x30e3ac
0078d230: mov      r1, sl
0078d234: bl       #0x30e9ac
0078d238: cmp      r0, #0
0078d23c: moveq    r6, sl
0078d240: bne      #0x78d36c
0078d244: ldr      r2, [sp, #0x14]
0078d248: ldr      r3, [r4, #0x150]
0078d24c: cmp      r2, r3
0078d250: strlt    r6, [r4, #0x154]
0078d254: ldrlt    r3, [sp, #0xc]
0078d258: strlt    r3, [r4, #0x158]
0078d25c: ldr      r1, [sp, #0x14]
0078d260: ldr      r3, [r7]
0078d264: ldr      r0, [sp, #4]
0078d268: add      r1, r1, #1
0078d26c: str      r1, [sp, #0x14]
0078d270: ldr      r1, [r3, #0x58]
0078d274: bl       #0x30ed6c
0078d278: ldr      r1, [sp, #0xc]
0078d27c: bl       #0x30eba4
0078d280: ldr      sl, [r4, #0x128]
0078d284: mov      r8, r0
0078d288: mov      r0, r6
0078d28c: mov      r1, sl
0078d290: bl       #0x30e2f8
0078d294: ldr      sb, [r4, #0x130]
0078d298: cmp      r0, #0
0078d29c: moveq    sl, r6
0078d2a0: mov      r1, sb
0078d2a4: str      sl, [r4, #0x128]
0078d2a8: mov      r0, r8
0078d2ac: bl       #0x30e2f8
0078d2b0: ldr      sl, [r4, #0x12c]
0078d2b4: cmp      r0, #0
0078d2b8: moveq    sb, r8
0078d2bc: mov      r1, sl
0078d2c0: str      sb, [r4, #0x130]
0078d2c4: mov      r0, r6
0078d2c8: bl       #0x30e2f8
0078d2cc: ldr      sb, [r4, #0x134]
0078d2d0: cmp      r0, #0
0078d2d4: movne    sl, r6
0078d2d8: mov      r0, r8
0078d2dc: str      sl, [r4, #0x12c]
0078d2e0: mov      r1, sb
0078d2e4: bl       #0x30e2f8
0078d2e8: cmp      r0, #0
0078d2ec: ldr      r0, [sp, #0x7c]
0078d2f0: moveq    r8, sb
0078d2f4: str      r8, [r4, #0x134]
0078d2f8: cmp      r0, #0
0078d2fc: beq      #0x78cf10
0078d300: bl       #0x75a240
0078d304: mov      sl, r5
0078d308: b        #0x78cf14
0078d30c: mov      r3, #0x3f800000
0078d310: str      r3, [sp, #0x18]
0078d314: ldr      r2, [sp, #0x6c]
0078d318: ldr      r3, [r4, #0xa8]
0078d31c: mov      r5, #0x20
0078d320: str      r2, [r4, #0x16c]
0078d324: str      r3, [r4, #0x168]
0078d328: mov      sb, r5
0078d32c: mov      r8, r5
0078d330: b        #0x78d0e4
0078d334: ldr      r8, [sp, #0x6c]
0078d338: cmp      r8, #0
0078d33c: ble      #0x78cf10
0078d340: mov      r3, #0x24
0078d344: sub      r8, r8, #1
0078d348: mul      r8, r3, r8
0078d34c: ldr      sl, [sp, #0x68]
0078d350: ldr      r1, [sl, r8]
0078d354: bl       #0x30e3ac
0078d358: mov      r3, #0
0078d35c: str      r3, [sl, r8]
0078d360: mov      r6, r0
0078d364: mov      sl, r5
0078d368: b        #0x78cf14
0078d36c: ldr      r0, [sp, #0x20]
0078d370: ldr      r1, [sp, #8]
0078d374: bl       #0x78a948
0078d378: ldr      r1, [sp, #0x30]
0078d37c: ldr      r0, [sp, #0x24]
0078d380: bl       #0x30eba4
0078d384: mov      r1, r0
0078d388: ldr      r0, [sp, #0xc]
0078d38c: bl       #0x30eba4
0078d390: mov      r1, #0
0078d394: str      r0, [sp, #0xc]
0078d398: mov      r0, r8
0078d39c: ldr      r6, [r4, #0x180]
0078d3a0: bl       #0x78a4ec
0078d3a4: ldr      r1, [sp, #8]
0078d3a8: add      r0, r1, #4
0078d3ac: ldr      r1, [r7]
0078d3b0: bl       #0x764234
0078d3b4: ldr      r2, [r7, #8]
0078d3b8: ldrb     r3, [r7, #0xc]
0078d3bc: ldr      r0, [r7, #4]
0078d3c0: str      r2, [sp, #0x50]
0078d3c4: ldr      r2, [sp, #0xc]
0078d3c8: strb     r3, [sp, #0x54]
0078d3cc: str      r6, [sp, #0x58]
0078d3d0: str      r2, [sp, #0x5c]
0078d3d4: bl       #0x30e964
0078d3d8: ldr      sb, [r4, #0xa8]
0078d3dc: ldr      ip, [r4, #0x16c]
0078d3e0: ldr      r1, [r4, #0xa4]
0078d3e4: mov      r3, #1
0078d3e8: sub      sb, sb, #1
0078d3ec: mov      r2, #0x30
0078d3f0: cmn      ip, #1
0078d3f4: str      r0, [sp, #0x60]
0078d3f8: strb     r3, [sp, #0x65]
0078d3fc: strb     r3, [sp, #0x64]
0078d400: mla      fp, r2, sb, r1
0078d404: beq      #0x78d620
0078d408: ldr      r8, [r4, #0x168]
0078d40c: mov      r3, #0x24
0078d410: mul      lr, r3, ip
0078d414: mla      r3, r2, r8, r1
0078d418: mov      r0, sl
0078d41c: ldr      r3, [r3, #0x20]
0078d420: ldr      r1, [r3, lr]
0078d424: str      ip, [sp]
0078d428: bl       #0x30e3ac
0078d42c: ldr      ip, [sp]
0078d430: ldr      r3, [fp, #0x24]
0078d434: cmp      r8, sb
0078d438: movne    sl, #0
0078d43c: addeq    sl, ip, #1
0078d440: cmp      sl, r3
0078d444: mov      r2, r0
0078d448: bge      #0x78d4d8
0078d44c: ldr      r1, [sp, #8]
0078d450: mov      r8, #0x24
0078d454: mul      r8, r8, sl
0078d458: add      r3, r1, #0x20
0078d45c: ldr      sb, [fp, #0x20]
0078d460: str      r5, [sp, #0x18]
0078d464: str      r7, [sp, #0x44]
0078d468: mov      r5, r0
0078d46c: str      r4, [sp, #0x40]
0078d470: mov      r7, r3
0078d474: add      r1, sb, r8
0078d478: mov      r0, r7
0078d47c: bl       #0x78a81c
0078d480: ldr      sb, [fp, #0x20]
0078d484: mov      r0, r6
0078d488: add      sl, sl, #1
0078d48c: ldr      r4, [sb, r8]
0078d490: add      r8, r8, #0x24
0078d494: mov      r1, r4
0078d498: bl       #0x30eba4
0078d49c: mov      r1, r4
0078d4a0: mov      r6, r0
0078d4a4: mov      r0, r5
0078d4a8: bl       #0x30e3ac
0078d4ac: ldr      r3, [fp, #0x24]
0078d4b0: mov      r5, r0
0078d4b4: cmp      sl, r3
0078d4b8: blt      #0x78d474
0078d4bc: ldr      r4, [sp, #0x40]
0078d4c0: ldr      r5, [sp, #0x18]
0078d4c4: ldr      r7, [sp, #0x44]
0078d4c8: ldr      sb, [r4, #0xa8]
0078d4cc: ldr      r8, [r4, #0x168]
0078d4d0: mov      r2, r0
0078d4d4: sub      sb, sb, #1
0078d4d8: cmp      sb, r8
0078d4dc: ldreq    r1, [r4, #0x16c]
0078d4e0: movne    r1, #0
0078d4e4: add      r0, fp, #0x20
0078d4e8: str      r2, [sp]
0078d4ec: bl       #0x78a4ec
0078d4f0: ldr      r2, [sp]
0078d4f4: mov      r3, r2
0078d4f8: mov      r0, r4
0078d4fc: ldr      r2, [r4, #0x164]
0078d500: ldr      r1, [r4, #0x17c]
0078d504: bl       #0x78a398
0078d508: ldr      r3, [r4, #0xa8]
0078d50c: mvn      r2, #0
0078d510: str      r2, [r4, #0x16c]
0078d514: str      r3, [r4, #0x164]
0078d518: str      r3, [r4, #0x168]
0078d51c: b        #0x78d244
0078d520: cmp      r8, #0x26
0078d524: movne    r2, #0x3f800000
0078d528: uxthne   sb, r8
0078d52c: strne    r2, [sp, #0x18]
0078d530: bne      #0x78d0e4
0078d534: ldr      r3, [sp, #0x34]
0078d538: cmp      r3, #0
0078d53c: beq      #0x78d57c
0078d540: ldr      r5, [sp, #0x9c]
0078d544: ldr      r1, [sp, #0x3c]
0078d548: mov      r2, #5
0078d54c: mov      r0, r5
0078d550: bl       #0x30ec7c
0078d554: cmp      r0, #0
0078d558: bne      #0x78d57c
0078d55c: add      r3, r5, #5
0078d560: mov      r1, #0x3f800000
0078d564: mov      r5, #0x20
0078d568: str      r3, [sp, #0x9c]
0078d56c: mov      sb, r5
0078d570: str      r1, [sp, #0x18]
0078d574: mov      r8, r5
0078d578: b        #0x78d0e4
0078d57c: mov      r2, #0x3f800000
0078d580: mov      r5, #0x26
0078d584: str      r2, [sp, #0x18]
0078d588: mov      sb, r5
0078d58c: b        #0x78d0e4
0078d590: ldr      r8, [r4, #0xa8]
0078d594: subs     r6, r8, #1
0078d598: bmi      #0x78d608
0078d59c: mov      r1, #0x30
0078d5a0: mul      r6, r1, r6
0078d5a4: ldr      sb, [r4, #0xa4]
0078d5a8: mov      r1, r5
0078d5ac: add      sb, sb, r6
0078d5b0: ldr      r0, [sb, #0x14]
0078d5b4: bl       #0x30df8c
0078d5b8: cmp      r0, #0
0078d5bc: beq      #0x78d608
0078d5c0: sub      r5, r8, #2
0078d5c4: mov      r2, #0x30
0078d5c8: mul      r5, r2, r5
0078d5cc: b        #0x78d5d4
0078d5d0: add      sb, sb, r6
0078d5d4: cmp      r8, #1
0078d5d8: str      sl, [sb, #0x14]
0078d5dc: beq      #0x78d608
0078d5e0: ldr      sb, [r4, #0xa4]
0078d5e4: ldr      r1, [sp, #0x5c]
0078d5e8: sub      r6, r6, #0x30
0078d5ec: add      r3, sb, r5
0078d5f0: ldr      r0, [r3, #0x14]
0078d5f4: bl       #0x30df8c
0078d5f8: cmp      r0, #0
0078d5fc: sub      r8, r8, #1
0078d600: sub      r5, r5, #0x30
0078d604: bne      #0x78d5d0
0078d608: str      sl, [sp, #0x5c]
0078d60c: b        #0x78cd98
0078d610: add      r3, sp, #0x48
0078d614: str      r0, [sp, #0x5c]
0078d618: str      r3, [sp, #8]
0078d61c: b        #0x78cd9c
0078d620: ldr      r1, [fp, #0x24]
0078d624: cmp      r1, #0
0078d628: movle    r2, sl
0078d62c: ble      #0x78d4f4
0078d630: ldr      r3, [fp, #0x20]
0078d634: mov      sb, #0x24
0078d638: sub      r1, r1, #1
0078d63c: mla      r1, sb, r1, r3
0078d640: mov      r0, r8
0078d644: bl       #0x78a81c
0078d648: ldr      r3, [fp, #0x24]
0078d64c: ldr      r2, [fp, #0x20]!
0078d650: mov      r0, r6
0078d654: sub      r3, r3, #1
0078d658: mul      sb, sb, r3
0078d65c: ldr      r8, [r2, sb]
0078d660: str      r3, [sp]
0078d664: mov      r1, r8
0078d668: bl       #0x30eba4
0078d66c: mov      r1, r8
0078d670: mov      r6, r0
0078d674: mov      r0, sl
0078d678: bl       #0x30e3ac
0078d67c: ldr      r3, [sp]
0078d680: mov      r2, r0
0078d684: mov      r0, fp
0078d688: mov      r1, r3
0078d68c: str      r2, [sp]
0078d690: bl       #0x78a4ec
0078d694: ldr      r2, [sp]
0078d698: b        #0x78d4f4
0078d69c: ldr      r1, [r0]
0078d6a0: sub      r1, r1, #1
0078d6a4: cmp      r1, #0
0078d6a8: str      r1, [r0]
0078d6ac: bne      #0x78d6b4
0078d6b0: bl       #0x752b38
0078d6b4: mov      r3, #0
0078d6b8: str      r3, [r4, #0x2c]
0078d6bc: str      r3, [r4, #0x30]
0078d6c0: b        #0x78cbd0
0078d6c4: eoreq    pc, sb, ip, asr #23
0078d6c8: andseq   sp, r7, r8, asr #32
0078d6cc: andseq   sp, r7, ip, lsr #32

# _ZN7gameswf23edit_text_character_defC1EPNS_6playerEPNS_20movie_definition_subE
0078d6d0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078d6d4: ldr      r4, [pc, #0xe8]
0078d6d8: mov      r5, r0
0078d6dc: ldr      r7, [pc, #0xe4]
0078d6e0: mov      r6, r2
0078d6e4: bl       #0x75ea44
0078d6e8: ldr      ip, [r5, #0x44]
0078d6ec: ldr      r0, [r5, #0x8c]
0078d6f0: add      r4, pc, r4
0078d6f4: mvn      r1, #0
0078d6f8: ldr      r7, [r4, r7]
0078d6fc: bfi      r0, r1, #0, #0x18
0078d700: bfi      ip, r1, #0, #0x18
0078d704: mov      r3, #0
0078d708: str      r6, [r5, #0x20]
0078d70c: lsr      sb, ip, #0x18
0078d710: lsr      sl, r0, #0x18
0078d714: mov      r6, #0x43000000
0078d718: mov      r2, #0
0078d71c: mov      r8, #1
0078d720: add      r7, r7, #8
0078d724: bfi      sb, r3, #0, #1
0078d728: bfi      sl, r3, #0, #1
0078d72c: add      r6, r6, #0x700000
0078d730: str      r0, [r5, #0x8c]
0078d734: str      ip, [r5, #0x44]
0078d738: str      r7, [r5]
0078d73c: str      r6, [r5, #0x5c]
0078d740: strb     r8, [r5, #0x7c]
0078d744: strb     sb, [r5, #0x47]
0078d748: strb     sl, [r5, #0x8f]
0078d74c: strb     r8, [r5, #0x34]
0078d750: strb     r3, [r5, #0x35]
0078d754: strb     r3, [r5, #0x48]
0078d758: strb     r3, [r5, #0x49]
0078d75c: strb     r3, [r5, #0x4a]
0078d760: strb     r3, [r5, #0x4b]
0078d764: strb     r3, [r5, #0x4c]
0078d768: strb     r3, [r5, #0x4d]
0078d76c: strb     r3, [r5, #0x4e]
0078d770: strb     r3, [r5, #0x4f]
0078d774: strb     r3, [r5, #0x50]
0078d778: str      r1, [r5, #0x54]
0078d77c: str      r3, [r5, #0x58]
0078d780: strb     r1, [r5, #0x63]
0078d784: str      r3, [r5, #0x64]
0078d788: str      r3, [r5, #0x68]
0078d78c: str      r2, [r5, #0x6c]
0078d790: str      r2, [r5, #0x70]
0078d794: str      r2, [r5, #0x74]
0078d798: str      r2, [r5, #0x78]
0078d79c: strb     r3, [r5, #0x7d]
0078d7a0: strb     r3, [r5, #0x90]
0078d7a4: str      r3, [r5, #0x94]
0078d7a8: mov      r0, r5
0078d7ac: str      r2, [r5, #0x9c]
0078d7b0: strb     r3, [r5, #0x62]
0078d7b4: str      r2, [r5, #0x98]
0078d7b8: strb     r3, [r5, #0x60]
0078d7bc: strb     r3, [r5, #0x61]
0078d7c0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0078d7c4: eoreq    r7, r0, r0, lsr #7
0078d7c8: andeq    r1, r0, ip, ror #12

# _ZN7gameswf23edit_text_character_defC2EPNS_6playerEPNS_20movie_definition_subE
0078d874: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078d878: ldr      r4, [pc, #0xe8]
0078d87c: mov      r5, r0
0078d880: ldr      r7, [pc, #0xe4]
0078d884: mov      r6, r2
0078d888: bl       #0x75ea44
0078d88c: ldr      ip, [r5, #0x44]
0078d890: ldr      r0, [r5, #0x8c]
0078d894: add      r4, pc, r4
0078d898: mvn      r1, #0
0078d89c: ldr      r7, [r4, r7]
0078d8a0: bfi      r0, r1, #0, #0x18
0078d8a4: bfi      ip, r1, #0, #0x18
0078d8a8: mov      r3, #0
0078d8ac: str      r6, [r5, #0x20]
0078d8b0: lsr      sb, ip, #0x18
0078d8b4: lsr      sl, r0, #0x18
0078d8b8: mov      r6, #0x43000000
0078d8bc: mov      r2, #0
0078d8c0: mov      r8, #1
0078d8c4: add      r7, r7, #8
0078d8c8: bfi      sb, r3, #0, #1
0078d8cc: bfi      sl, r3, #0, #1
0078d8d0: add      r6, r6, #0x700000
0078d8d4: str      r0, [r5, #0x8c]
0078d8d8: str      ip, [r5, #0x44]
0078d8dc: str      r7, [r5]
0078d8e0: str      r6, [r5, #0x5c]
0078d8e4: strb     r8, [r5, #0x7c]
0078d8e8: strb     sb, [r5, #0x47]
0078d8ec: strb     sl, [r5, #0x8f]
0078d8f0: strb     r8, [r5, #0x34]
0078d8f4: strb     r3, [r5, #0x35]
0078d8f8: strb     r3, [r5, #0x48]
0078d8fc: strb     r3, [r5, #0x49]
0078d900: strb     r3, [r5, #0x4a]
0078d904: strb     r3, [r5, #0x4b]
0078d908: strb     r3, [r5, #0x4c]
0078d90c: strb     r3, [r5, #0x4d]
0078d910: strb     r3, [r5, #0x4e]
0078d914: strb     r3, [r5, #0x4f]
0078d918: strb     r3, [r5, #0x50]
0078d91c: str      r1, [r5, #0x54]
0078d920: str      r3, [r5, #0x58]
0078d924: strb     r1, [r5, #0x63]
0078d928: str      r3, [r5, #0x64]
0078d92c: str      r3, [r5, #0x68]
0078d930: str      r2, [r5, #0x6c]
0078d934: str      r2, [r5, #0x70]
0078d938: str      r2, [r5, #0x74]
0078d93c: str      r2, [r5, #0x78]
0078d940: strb     r3, [r5, #0x7d]
0078d944: strb     r3, [r5, #0x90]
0078d948: str      r3, [r5, #0x94]
0078d94c: mov      r0, r5
0078d950: str      r2, [r5, #0x9c]
0078d954: strb     r3, [r5, #0x62]
0078d958: str      r2, [r5, #0x98]
0078d95c: strb     r3, [r5, #0x60]
0078d960: strb     r3, [r5, #0x61]
0078d964: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN7gameswf23edit_text_character_defC1EPNS_6playerEii
0078d970: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078d974: ldr      r4, [pc, #0x154]
0078d978: mov      r6, r0
0078d97c: mov      r5, r2
0078d980: mov      sl, r3
0078d984: mov      r8, r1
0078d988: bl       #0x75ea44
0078d98c: ldr      lr, [pc, #0x140]
0078d990: ldr      ip, [r6, #0x44]
0078d994: add      r4, pc, r4
0078d998: ldr      r2, [r6, #0x8c]
0078d99c: ldr      lr, [r4, lr]
0078d9a0: mvn      r0, #0
0078d9a4: bfi      ip, r0, #0, #0x18
0078d9a8: bfi      r2, r0, #0, #0x18
0078d9ac: lsr      r1, ip, #0x18
0078d9b0: add      lr, lr, #8
0078d9b4: mov      r7, #0
0078d9b8: str      lr, [r6]
0078d9bc: mov      fp, r1
0078d9c0: mov      lr, #0x43000000
0078d9c4: lsr      r1, r2, #0x18
0078d9c8: mov      r3, #0
0078d9cc: add      lr, lr, #0x700000
0078d9d0: bfi      r1, r7, #0, #1
0078d9d4: mov      sb, #1
0078d9d8: bfi      fp, r7, #0, #1
0078d9dc: str      ip, [r6, #0x44]
0078d9e0: str      lr, [r6, #0x5c]
0078d9e4: str      r3, [r6, #0x6c]
0078d9e8: str      r3, [r6, #0x70]
0078d9ec: str      r3, [r6, #0x74]
0078d9f0: str      r3, [r6, #0x78]
0078d9f4: str      r0, [r6, #0x54]
0078d9f8: strb     r0, [r6, #0x60]
0078d9fc: strb     r0, [r6, #0x61]
0078da00: strb     r0, [r6, #0x62]
0078da04: strb     r0, [r6, #0x63]
0078da08: strb     sb, [r6, #0x7c]
0078da0c: strb     fp, [r6, #0x47]
0078da10: strb     sb, [r6, #0x34]
0078da14: str      r7, [r6, #0x20]
0078da18: strb     r7, [r6, #0x35]
0078da1c: strb     r7, [r6, #0x48]
0078da20: strb     r7, [r6, #0x49]
0078da24: strb     r7, [r6, #0x4a]
0078da28: strb     r7, [r6, #0x4b]
0078da2c: strb     r7, [r6, #0x4c]
0078da30: strb     r7, [r6, #0x4d]
0078da34: strb     r7, [r6, #0x4e]
0078da38: strb     r7, [r6, #0x4f]
0078da3c: strb     r7, [r6, #0x50]
0078da40: str      r7, [r6, #0x58]
0078da44: str      r7, [r6, #0x64]
0078da48: str      r7, [r6, #0x68]
0078da4c: strb     r7, [r6, #0x7d]
0078da50: mov      r0, r5
0078da54: str      r2, [r6, #0x8c]
0078da58: str      r3, [r6, #0x2c]
0078da5c: str      r3, [r6, #0x98]
0078da60: str      r3, [r6, #0x9c]
0078da64: str      r3, [r6, #0x24]
0078da68: strb     r1, [r6, #0x8f]
0078da6c: strb     r7, [r6, #0x90]
0078da70: str      r7, [r6, #0x94]
0078da74: bl       #0x30e964
0078da78: mov      r1, #0x41000000
0078da7c: add      r1, r1, #0xa00000
0078da80: bl       #0x30ed6c
0078da84: str      r0, [r6, #0x28]
0078da88: mov      r0, sl
0078da8c: bl       #0x30e964
0078da90: mov      r1, #0x41000000
0078da94: add      r1, r1, #0xa00000
0078da98: bl       #0x30ed6c
0078da9c: mov      r1, r7
0078daa0: str      r0, [r6, #0x30]
0078daa4: strb     r7, [r6, #0x60]
0078daa8: strb     r7, [r6, #0x61]
0078daac: strb     r7, [r6, #0x62]
0078dab0: mov      r0, #0x88
0078dab4: bl       #0x752ba8
0078dab8: mov      r1, r8
0078dabc: mov      r4, r0
0078dac0: bl       #0x7cf8d8
0078dac4: str      r4, [r6, #0x58]
0078dac8: mov      r0, r6
0078dacc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN7gameswf23edit_text_character_defC2EPNS_6playerEii
0078dad8: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078dadc: ldr      r4, [pc, #0x154]
0078dae0: mov      r6, r0
0078dae4: mov      r5, r2
0078dae8: mov      sl, r3
0078daec: mov      r8, r1
0078daf0: bl       #0x75ea44
0078daf4: ldr      lr, [pc, #0x140]
0078daf8: ldr      ip, [r6, #0x44]
0078dafc: add      r4, pc, r4
0078db00: ldr      r2, [r6, #0x8c]
0078db04: ldr      lr, [r4, lr]
0078db08: mvn      r0, #0
0078db0c: bfi      ip, r0, #0, #0x18
0078db10: bfi      r2, r0, #0, #0x18
0078db14: lsr      r1, ip, #0x18
0078db18: add      lr, lr, #8
0078db1c: mov      r7, #0
0078db20: str      lr, [r6]
0078db24: mov      fp, r1
0078db28: mov      lr, #0x43000000
0078db2c: lsr      r1, r2, #0x18
0078db30: mov      r3, #0
0078db34: add      lr, lr, #0x700000
0078db38: bfi      r1, r7, #0, #1
0078db3c: mov      sb, #1
0078db40: bfi      fp, r7, #0, #1
0078db44: str      ip, [r6, #0x44]
0078db48: str      lr, [r6, #0x5c]
0078db4c: str      r3, [r6, #0x6c]
0078db50: str      r3, [r6, #0x70]
0078db54: str      r3, [r6, #0x74]
0078db58: str      r3, [r6, #0x78]
0078db5c: str      r0, [r6, #0x54]
0078db60: strb     r0, [r6, #0x60]
0078db64: strb     r0, [r6, #0x61]
0078db68: strb     r0, [r6, #0x62]
0078db6c: strb     r0, [r6, #0x63]
0078db70: strb     sb, [r6, #0x7c]
0078db74: strb     fp, [r6, #0x47]
0078db78: strb     sb, [r6, #0x34]
0078db7c: str      r7, [r6, #0x20]
0078db80: strb     r7, [r6, #0x35]
0078db84: strb     r7, [r6, #0x48]
0078db88: strb     r7, [r6, #0x49]
0078db8c: strb     r7, [r6, #0x4a]
0078db90: strb     r7, [r6, #0x4b]
0078db94: strb     r7, [r6, #0x4c]
0078db98: strb     r7, [r6, #0x4d]
0078db9c: strb     r7, [r6, #0x4e]
0078dba0: strb     r7, [r6, #0x4f]
0078dba4: strb     r7, [r6, #0x50]
0078dba8: str      r7, [r6, #0x58]
0078dbac: str      r7, [r6, #0x64]
0078dbb0: str      r7, [r6, #0x68]
0078dbb4: strb     r7, [r6, #0x7d]
0078dbb8: mov      r0, r5
0078dbbc: str      r2, [r6, #0x8c]
0078dbc0: str      r3, [r6, #0x2c]
0078dbc4: str      r3, [r6, #0x98]
0078dbc8: str      r3, [r6, #0x9c]
0078dbcc: str      r3, [r6, #0x24]
0078dbd0: strb     r1, [r6, #0x8f]
0078dbd4: strb     r7, [r6, #0x90]
0078dbd8: str      r7, [r6, #0x94]
0078dbdc: bl       #0x30e964
0078dbe0: mov      r1, #0x41000000
0078dbe4: add      r1, r1, #0xa00000
0078dbe8: bl       #0x30ed6c
0078dbec: str      r0, [r6, #0x28]
0078dbf0: mov      r0, sl
0078dbf4: bl       #0x30e964
0078dbf8: mov      r1, #0x41000000
0078dbfc: add      r1, r1, #0xa00000
0078dc00: bl       #0x30ed6c
0078dc04: mov      r1, r7
0078dc08: str      r0, [r6, #0x30]
0078dc0c: strb     r7, [r6, #0x60]
0078dc10: strb     r7, [r6, #0x61]
0078dc14: strb     r7, [r6, #0x62]
0078dc18: mov      r0, #0x88
0078dc1c: bl       #0x752ba8
0078dc20: mov      r1, r8
0078dc24: mov      r4, r0
0078dc28: bl       #0x7cf8d8
0078dc2c: str      r4, [r6, #0x58]
0078dc30: mov      r0, r6
0078dc34: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078dc38: mlaeq    r0, r4, pc, r6
0078dc3c: andeq    r1, r0, ip, ror #12

# _ZN7gameswf19edit_text_character8get_rootEv
0078dde8: push     {r4, lr}
0078ddec: ldr      r3, [r0, #0x40]
0078ddf0: mov      r4, r0
0078ddf4: cmp      r3, #0
0078ddf8: beq      #0x78de0c
0078ddfc: ldr      r0, [r0, #0x3c]
0078de00: ldrb     r2, [r0, #4]
0078de04: cmp      r2, #0
0078de08: beq      #0x78de20
0078de0c: mov      r0, r3
0078de10: ldr      r3, [r3]
0078de14: mov      lr, pc
0078de18: ldr      pc, [r3, #0x54]
0078de1c: pop      {r4, pc}
0078de20: ldr      r1, [r0]
0078de24: sub      r1, r1, #1
0078de28: cmp      r1, #0
0078de2c: str      r1, [r0]
0078de30: bne      #0x78de38
0078de34: bl       #0x752b38
0078de38: mov      r3, #0
0078de3c: str      r3, [r4, #0x40]
0078de40: str      r3, [r4, #0x3c]
0078de44: mov      r0, r3
0078de48: ldr      r3, [r3]
0078de4c: mov      lr, pc
0078de50: ldr      pc, [r3, #0x54]
0078de54: pop      {r4, pc}

# _ZN7gameswf11html_reader5parseEPNS_19edit_text_characterE
0078e354: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078e358: ldr      r8, [pc, #0xc14]
0078e35c: ldr      r2, [pc, #0xc14]
0078e360: sub      sp, sp, #0x3b4
0078e364: add      r8, pc, r8
0078e368: str      r2, [sp, #4]
0078e36c: ldr      r2, [r8, r2]
0078e370: ldrb     r3, [r1, #0x138]
0078e374: mov      r4, r1
0078e378: ldr      r2, [r2]
0078e37c: sxtb     r3, r3
0078e380: cmn      r3, #1
0078e384: str      r2, [sp, #0x3ac]
0078e388: ldreq    r3, [r1, #0x13c]
0078e38c: mov      r6, r0
0078e390: sub      r3, r3, #1
0078e394: cmp      r3, #0
0078e398: bne      #0x78e3bc
0078e39c: ldr      sb, [sp, #4]
0078e3a0: ldr      r2, [sp, #0x3ac]
0078e3a4: ldr      r3, [r8, sb]
0078e3a8: ldr      r3, [r3]
0078e3ac: cmp      r2, r3
0078e3b0: bne      #0x78eee8
0078e3b4: add      sp, sp, #0x3b4
0078e3b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078e3bc: ldr      r2, [r1, #0x170]
0078e3c0: add      r5, sp, #0x44
0078e3c4: ldr      r1, [r1, #0x178]
0078e3c8: mov      r3, #0
0078e3cc: mov      r0, r5
0078e3d0: mov      ip, #0xc
0078e3d4: str      ip, [sp, #0x48]
0078e3d8: strb     r3, [sp, #0x50]
0078e3dc: str      r3, [sp, #0x44]
0078e3e0: str      r2, [sp, #0x4c]
0078e3e4: bl       #0x764234
0078e3e8: ldr      r0, [r4, #0x174]
0078e3ec: bl       #0x30e4cc
0078e3f0: mov      r1, r5
0078e3f4: str      r0, [sp, #0x48]
0078e3f8: mov      r0, r6
0078e3fc: bl       #0x78ad54
0078e400: ldrb     fp, [r4, #0x138]
0078e404: ldr      r3, [pc, #0xb70]
0078e408: ldr      ip, [pc, #0xb70]
0078e40c: sxtb     sb, fp
0078e410: cmn      sb, #1
0078e414: add      r3, pc, r3
0078e418: ldreq    r5, [r4, #0x144]
0078e41c: str      r3, [sp, #0x10]
0078e420: ldr      r3, [pc, #0xb5c]
0078e424: addne    r5, r4, #0x138
0078e428: mov      r7, #0
0078e42c: add      r3, pc, r3
0078e430: str      r3, [sp, #0x18]
0078e434: ldr      r3, [pc, #0xb4c]
0078e438: addne    r5, r5, #1
0078e43c: str      r7, [sp, #0x14]
0078e440: add      r3, pc, r3
0078e444: str      r3, [sp, #0x1c]
0078e448: ldr      r3, [pc, #0xb3c]
0078e44c: str      ip, [sp, #0x24]
0078e450: add      r3, pc, r3
0078e454: str      r3, [sp, #0x20]
0078e458: cmn      sb, #1
0078e45c: ldrbne   r3, [r4, #0x138]
0078e460: ldreq    r3, [r4, #0x13c]
0078e464: sxtbne   r3, r3
0078e468: sub      r3, r3, #1
0078e46c: cmp      r7, r3
0078e470: bge      #0x78e4f0
0078e474: ldrsb    r3, [r5, r7]
0078e478: add      sl, r5, r7
0078e47c: cmp      r3, #0x3c
0078e480: beq      #0x78e568
0078e484: mov      r0, sl
0078e488: mov      r1, #0x3c
0078e48c: bl       #0x30ec28
0078e490: subs     r7, r0, #0
0078e494: bne      #0x78e504
0078e498: cmn      sb, #1
0078e49c: ldrbne   r3, [r4, #0x138]
0078e4a0: ldreq    r3, [r4, #0x13c]
0078e4a4: add      r7, sp, #0x258
0078e4a8: sxtbne   r3, r3
0078e4ac: sub      r3, r3, #1
0078e4b0: add      r5, r5, r3
0078e4b4: mov      r1, sl
0078e4b8: rsb      r2, sl, r5
0078e4bc: mov      r0, r7
0078e4c0: bl       #0x751eb4
0078e4c4: ldr      r2, [r6, #4]
0078e4c8: ldr      r3, [r6]
0078e4cc: mov      r0, r4
0078e4d0: sub      r2, r2, #1
0078e4d4: add      r2, r3, r2, lsl #4
0078e4d8: mov      r1, r7
0078e4dc: mov      r3, #1
0078e4e0: bl       #0x78cb90
0078e4e4: ldrb     r3, [sp, #0x258]
0078e4e8: cmp      r3, #0xff
0078e4ec: beq      #0x78eacc
0078e4f0: ldr      r0, [sp, #0x44]
0078e4f4: cmp      r0, #0
0078e4f8: beq      #0x78e39c
0078e4fc: bl       #0x75a240
0078e500: b        #0x78e39c
0078e504: mov      r1, sl
0078e508: add      sl, sp, #0x258
0078e50c: rsb      r2, r1, r7
0078e510: mov      r0, sl
0078e514: bl       #0x751eb4
0078e518: ldr      r2, [r6, #4]
0078e51c: ldr      r3, [r6]
0078e520: mov      r1, sl
0078e524: sub      r2, r2, #1
0078e528: add      r2, r3, r2, lsl #4
0078e52c: mov      r0, r4
0078e530: mov      r3, #1
0078e534: bl       #0x78cb90
0078e538: ldrb     r3, [sp, #0x258]
0078e53c: rsb      r7, r5, r7
0078e540: cmp      r3, #0xff
0078e544: beq      #0x78e558
0078e548: ldrb     fp, [r4, #0x138]
0078e54c: mov      r3, fp
0078e550: sxtb     sb, fp
0078e554: b        #0x78e458
0078e558: ldr      r0, [sp, #0x264]
0078e55c: ldr      r1, [sp, #0x260]
0078e560: bl       #0x752b38
0078e564: b        #0x78e548
0078e568: mov      r0, sl
0078e56c: mov      r1, #0x3e
0078e570: bl       #0x30ec28
0078e574: cmp      r0, #0
0078e578: str      r0, [sp]
0078e57c: beq      #0x78e4f0
0078e580: cmn      sb, #1
0078e584: ldrbne   r3, [r4, #0x138]
0078e588: ldreq    r3, [r4, #0x13c]
0078e58c: add      r7, r7, #1
0078e590: sxtbne   r3, r3
0078e594: sub      r3, r3, #1
0078e598: cmp      r7, r3
0078e59c: bge      #0x78e4f0
0078e5a0: ldrsb    r3, [r5, r7]
0078e5a4: add      r7, r5, r7
0078e5a8: cmp      r3, #0x2f
0078e5ac: bne      #0x78e5dc
0078e5b0: ldr      r1, [r6, #4]
0078e5b4: cmp      r1, #1
0078e5b8: ble      #0x78e5cc
0078e5bc: sub      r1, r1, #1
0078e5c0: mov      r0, r6
0078e5c4: bl       #0x78bc0c
0078e5c8: ldrb     fp, [r4, #0x138]
0078e5cc: ldr      r2, [sp]
0078e5d0: rsb      r7, r5, #1
0078e5d4: add      r7, r2, r7
0078e5d8: b        #0x78e54c
0078e5dc: add      sb, sp, #0x58
0078e5e0: mov      r1, #0
0078e5e4: mov      r2, #0x200
0078e5e8: mov      r0, sb
0078e5ec: bl       #0x30e460
0078e5f0: ldr      lr, [sp]
0078e5f4: mvn      r2, sl
0078e5f8: mov      r1, r7
0078e5fc: mov      sl, #0
0078e600: add      r2, lr, r2
0078e604: add      r7, sp, #0x3b0
0078e608: mov      r0, sb
0078e60c: bl       #0x30e868
0078e610: str      sl, [r7, #-0x35c]!
0078e614: mov      r2, sb
0078e618: mov      r1, r7
0078e61c: mov      r0, r6
0078e620: bl       #0x78e060
0078e624: ldm      r6, {r3, sb}
0078e628: mov      r2, #0xc
0078e62c: mov      fp, #1
0078e630: add      r1, sp, #0x34
0078e634: str      r2, [sp, #0x38]
0078e638: str      sl, [sp, #0x34]
0078e63c: strb     sl, [sp, #0x3c]
0078e640: strb     sl, [sp, #0x3d]
0078e644: strb     sl, [sp, #0x3e]
0078e648: strb     sl, [sp, #0x40]
0078e64c: str      r1, [sp, #0xc]
0078e650: sub      sb, sb, #1
0078e654: strb     fp, [sp, #0x3f]
0078e658: ldr      r1, [r3, sb, lsl #4]
0078e65c: ldr      r0, [sp, #0xc]
0078e660: add      sb, r3, sb, lsl #4
0078e664: bl       #0x764234
0078e668: ldr      r2, [sb, #4]
0078e66c: ldr      r3, [sp, #0x3a8]
0078e670: add      lr, sp, #0x370
0078e674: str      r2, [sp, #0x38]
0078e678: ldr      r1, [sb, #8]
0078e67c: mvn      r2, #0
0078e680: bfi      r3, r2, #0, #0x18
0078e684: str      r1, [sp, #0x3c]
0078e688: ldrb     ip, [sb, #0xc]
0078e68c: lsr      r2, r3, #0x18
0078e690: add      sb, sp, #0x384
0078e694: bfi      r2, sl, #0, #1
0078e698: ldr      r1, [sp, #0x10]
0078e69c: mov      r0, sb
0078e6a0: str      r3, [sp, #0x3a8]
0078e6a4: str      lr, [sp, #8]
0078e6a8: strb     ip, [sp, #0x40]
0078e6ac: strb     r2, [sp, #0x3ab]
0078e6b0: strb     sl, [sp, #0x399]
0078e6b4: strb     fp, [sp, #0x398]
0078e6b8: add      sl, sp, #0x398
0078e6bc: bl       #0x413a7c
0078e6c0: mov      r1, sb
0078e6c4: ldr      r0, [sp, #8]
0078e6c8: bl       #0x75302c
0078e6cc: ldr      r1, [sp, #8]
0078e6d0: mov      r0, r7
0078e6d4: mov      r2, sl
0078e6d8: bl       #0x78dfbc
0078e6dc: mov      sb, r0
0078e6e0: ldrb     r0, [sp, #0x370]
0078e6e4: sxtb     r3, r0
0078e6e8: cmn      r3, #1
0078e6ec: beq      #0x78e9e8
0078e6f0: ldrb     r1, [sp, #0x384]
0078e6f4: sxtb     r3, r1
0078e6f8: cmn      r3, #1
0078e6fc: beq      #0x78e9d8
0078e700: cmp      sb, #0
0078e704: bne      #0x78e738
0078e708: ldrb     r2, [sp, #0x398]
0078e70c: sxtb     sb, r2
0078e710: cmn      sb, #1
0078e714: beq      #0x78e7c0
0078e718: ldr      r0, [sp, #0x34]
0078e71c: cmp      r0, #0
0078e720: beq      #0x78e728
0078e724: bl       #0x75a240
0078e728: mov      r0, r7
0078e72c: bl       #0x78c600
0078e730: ldrb     fp, [r4, #0x138]
0078e734: b        #0x78e5cc
0078e738: ldrb     r3, [sp, #0x398]
0078e73c: ldr      r1, [sp, #0x18]
0078e740: sxtb     sb, r3
0078e744: cmn      sb, #1
0078e748: addne    r0, sl, #1
0078e74c: ldreq    r0, [sp, #0x3a4]
0078e750: bl       #0x30e31c
0078e754: cmp      r0, #0
0078e758: bne      #0x78e7d0
0078e75c: ldr      ip, [sp, #0x14]
0078e760: cmp      ip, #0
0078e764: beq      #0x78e7ac
0078e768: ldr      r1, [pc, #0x820]
0078e76c: add      sl, sp, #0x35c
0078e770: mov      r0, sl
0078e774: add      r1, pc, r1
0078e778: bl       #0x413a7c
0078e77c: ldr      r2, [r6, #4]
0078e780: ldr      r3, [r6]
0078e784: mov      r1, sl
0078e788: sub      r2, r2, #1
0078e78c: add      r2, r3, r2, lsl #4
0078e790: mov      r0, r4
0078e794: mov      r3, #1
0078e798: bl       #0x78cb90
0078e79c: mov      r0, sl
0078e7a0: bl       #0x41fed8
0078e7a4: ldrb     lr, [sp, #0x398]
0078e7a8: sxtb     sb, lr
0078e7ac: ldr      r0, [sp, #0x14]
0078e7b0: cmn      sb, #1
0078e7b4: add      r0, r0, #1
0078e7b8: str      r0, [sp, #0x14]
0078e7bc: bne      #0x78e718
0078e7c0: ldr      r0, [sp, #0x3a4]
0078e7c4: ldr      r1, [sp, #0x3a0]
0078e7c8: bl       #0x752b38
0078e7cc: b        #0x78e718
0078e7d0: mov      r0, sl
0078e7d4: ldr      r1, [sp, #0x1c]
0078e7d8: bl       #0x78aebc
0078e7dc: subs     sb, r0, #0
0078e7e0: beq      #0x78eadc
0078e7e4: ldr      r3, [sp, #0x358]
0078e7e8: mvn      r2, #0
0078e7ec: ldr      r1, [pc, #0x7a0]
0078e7f0: bfi      r3, r2, #0, #0x18
0078e7f4: lsr      r2, r3, #0x18
0078e7f8: add      sl, sp, #0x334
0078e7fc: mov      sb, #0
0078e800: add      ip, sp, #0x348
0078e804: bfi      r2, sb, #0, #1
0078e808: add      r1, pc, r1
0078e80c: str      ip, [sp, #0x28]
0078e810: mov      r0, sl
0078e814: mov      ip, #1
0078e818: str      r3, [sp, #0x358]
0078e81c: strb     ip, [sp, #0x348]
0078e820: strb     r2, [sp, #0x35b]
0078e824: strb     sb, [sp, #0x349]
0078e828: bl       #0x413a7c
0078e82c: mov      r1, sl
0078e830: ldr      r2, [sp, #0x28]
0078e834: mov      r0, r7
0078e838: bl       #0x78dfbc
0078e83c: mov      fp, r0
0078e840: mov      r0, sl
0078e844: bl       #0x41fed8
0078e848: cmp      fp, sb
0078e84c: bne      #0x78ea80
0078e850: ldr      r3, [sp, #0x330]
0078e854: mvn      r2, #0
0078e858: ldr      r1, [pc, #0x738]
0078e85c: bfi      r3, r2, #0, #0x18
0078e860: mov      ip, #0
0078e864: lsr      r2, r3, #0x18
0078e868: add      sl, sp, #0x30c
0078e86c: add      lr, sp, #0x320
0078e870: bfi      r2, ip, #0, #1
0078e874: add      r1, pc, r1
0078e878: str      lr, [sp, #8]
0078e87c: mov      r0, sl
0078e880: mov      lr, #1
0078e884: str      r3, [sp, #0x330]
0078e888: strb     lr, [sp, #0x320]
0078e88c: strb     r2, [sp, #0x333]
0078e890: strb     ip, [sp, #0x321]
0078e894: bl       #0x413a7c
0078e898: mov      r0, r7
0078e89c: mov      r1, sl
0078e8a0: ldr      r2, [sp, #8]
0078e8a4: bl       #0x78dfbc
0078e8a8: cmp      r0, #0
0078e8ac: bne      #0x78e9f8
0078e8b0: mov      r0, sl
0078e8b4: bl       #0x41fed8
0078e8b8: ldr      r3, [sp, #0x308]
0078e8bc: mvn      r2, #0
0078e8c0: ldr      r1, [pc, #0x6d4]
0078e8c4: bfi      r3, r2, #0, #0x18
0078e8c8: mov      ip, #0
0078e8cc: lsr      r2, r3, #0x18
0078e8d0: add      sb, sp, #0x2e4
0078e8d4: bfi      r2, ip, #0, #1
0078e8d8: mov      lr, #1
0078e8dc: add      r1, pc, r1
0078e8e0: mov      r0, sb
0078e8e4: add      sl, sp, #0x2f8
0078e8e8: str      r3, [sp, #0x308]
0078e8ec: strb     lr, [sp, #0x2f8]
0078e8f0: strb     ip, [sp, #0x2f9]
0078e8f4: strb     r2, [sp, #0x30b]
0078e8f8: bl       #0x413a7c
0078e8fc: mov      r1, sb
0078e900: mov      r2, sl
0078e904: mov      r0, r7
0078e908: bl       #0x78dfbc
0078e90c: mov      fp, r0
0078e910: mov      r0, sb
0078e914: bl       #0x41fed8
0078e918: cmp      fp, #0
0078e91c: beq      #0x78e9a8
0078e920: ldrb     r2, [sp, #0x2f8]
0078e924: mov      r1, #0x25
0078e928: sxtb     sb, r2
0078e92c: cmn      sb, #1
0078e930: addne    r0, sl, #1
0078e934: ldreq    r0, [sp, #0x304]
0078e938: bl       #0x30ec28
0078e93c: cmp      r0, #0
0078e940: beq      #0x78ecac
0078e944: cmn      sb, #1
0078e948: ldrbne   r3, [sp, #0x2f8]
0078e94c: ldreq    r1, [sp, #0x2fc]
0078e950: mov      r0, sl
0078e954: sxtbne   r1, r3
0078e958: sub      r1, r1, #1
0078e95c: sub      r1, r1, #1
0078e960: bl       #0x78b94c
0078e964: ldrb     sb, [sp, #0x2f8]
0078e968: ldr      r1, [r6, #4]
0078e96c: ldr      r2, [r6]
0078e970: sxtb     r3, sb
0078e974: cmn      r3, #1
0078e978: add      r2, r2, r1, lsl #4
0078e97c: addne    r0, sl, #1
0078e980: ldreq    r0, [sp, #0x304]
0078e984: ldr      sb, [r2, #-0xc]
0078e988: bl       #0x30e094
0078e98c: mul      r0, sb, r0
0078e990: movw     r3, #0x851f
0078e994: movt     r3, #0x51eb
0078e998: smull    ip, r3, r3, r0
0078e99c: asr      r0, r0, #0x1f
0078e9a0: rsb      r3, r0, r3, asr #5
0078e9a4: str      r3, [sp, #0x38]
0078e9a8: ldr      r1, [sp, #0xc]
0078e9ac: mov      r0, r6
0078e9b0: bl       #0x78ad54
0078e9b4: mov      r0, sl
0078e9b8: bl       #0x41fed8
0078e9bc: ldr      r0, [sp, #8]
0078e9c0: bl       #0x41fed8
0078e9c4: ldr      r0, [sp, #0x28]
0078e9c8: bl       #0x41fed8
0078e9cc: ldrb     r0, [sp, #0x398]
0078e9d0: sxtb     sb, r0
0078e9d4: b        #0x78e710
0078e9d8: ldr      r0, [sp, #0x390]
0078e9dc: ldr      r1, [sp, #0x38c]
0078e9e0: bl       #0x752b38
0078e9e4: b        #0x78e700
0078e9e8: ldr      r0, [sp, #0x37c]
0078e9ec: ldr      r1, [sp, #0x378]
0078e9f0: bl       #0x752b38
0078e9f4: b        #0x78e6f0
0078e9f8: ldrb     r0, [sp, #0x320]
0078e9fc: sxtb     r3, r0
0078ea00: cmn      r3, #1
0078ea04: ldreq    r3, [sp, #0x324]
0078ea08: sub      r3, r3, #1
0078ea0c: cmp      r3, #0
0078ea10: ble      #0x78e8b0
0078ea14: mov      r0, sl
0078ea18: bl       #0x41fed8
0078ea1c: ldrb     r1, [sp, #0x320]
0078ea20: sxtb     sl, r1
0078ea24: cmn      sl, #1
0078ea28: ldrne    r2, [sp, #8]
0078ea2c: ldreq    r3, [sp, #0x32c]
0078ea30: addne    r3, r2, #1
0078ea34: ldrsb    r3, [r3]
0078ea38: cmp      r3, #0x23
0078ea3c: beq      #0x78ebc0
0078ea40: cmn      sl, #1
0078ea44: ldrne    r1, [sp, #8]
0078ea48: ldreq    r0, [sp, #0x32c]
0078ea4c: addne    r0, r1, #1
0078ea50: bl       #0x30e094
0078ea54: orr      r0, r0, #0xff000000
0078ea58: bl       #0x30ed30
0078ea5c: bl       #0x30ea24
0078ea60: mvn      r1, #0
0078ea64: ubfx     r3, r0, #0x10, #8
0078ea68: ubfx     r2, r0, #8, #8
0078ea6c: strb     r1, [sp, #0x3f]
0078ea70: strb     r0, [sp, #0x3e]
0078ea74: strb     r2, [sp, #0x3d]
0078ea78: strb     r3, [sp, #0x3c]
0078ea7c: b        #0x78e8b8
0078ea80: mov      r0, r4
0078ea84: bl       #0x780374
0078ea88: mov      r1, sb
0078ea8c: mov      fp, r0
0078ea90: mov      r0, #0x88
0078ea94: bl       #0x752ba8
0078ea98: mov      r1, fp
0078ea9c: mov      sl, r0
0078eaa0: bl       #0x7cf8d8
0078eaa4: mov      r0, sl
0078eaa8: ldr      r1, [sp, #0x34]
0078eaac: bl       #0x7ce628
0078eab0: add      r0, sl, #0x30
0078eab4: ldr      r1, [sp, #0x28]
0078eab8: bl       #0x752f50
0078eabc: ldr      r0, [sp, #0xc]
0078eac0: mov      r1, sl
0078eac4: bl       #0x764234
0078eac8: b        #0x78e850
0078eacc: ldr      r0, [sp, #0x264]
0078ead0: ldr      r1, [sp, #0x260]
0078ead4: bl       #0x752b38
0078ead8: b        #0x78e4f0
0078eadc: mov      r0, sl
0078eae0: ldr      r1, [sp, #0x20]
0078eae4: bl       #0x78aebc
0078eae8: subs     fp, r0, #0
0078eaec: beq      #0x78eb4c
0078eaf0: mov      r0, r4
0078eaf4: bl       #0x780374
0078eaf8: mov      r1, sb
0078eafc: mov      fp, r0
0078eb00: mov      r0, #0x88
0078eb04: bl       #0x752ba8
0078eb08: mov      r1, fp
0078eb0c: mov      sl, r0
0078eb10: bl       #0x7cf8d8
0078eb14: mov      r0, sl
0078eb18: ldr      r1, [sp, #0x34]
0078eb1c: bl       #0x7ce628
0078eb20: mov      r3, #1
0078eb24: strb     r3, [sl, #0x4d]
0078eb28: mov      r1, sl
0078eb2c: ldr      r0, [sp, #0xc]
0078eb30: bl       #0x764234
0078eb34: ldr      r1, [sp, #0xc]
0078eb38: mov      r0, r6
0078eb3c: bl       #0x78ad54
0078eb40: ldrb     r1, [sp, #0x398]
0078eb44: sxtb     sb, r1
0078eb48: b        #0x78e710
0078eb4c: ldr      r2, [sp, #0x24]
0078eb50: mov      r0, sl
0078eb54: add      r1, pc, r2
0078eb58: bl       #0x78aebc
0078eb5c: cmp      r0, #0
0078eb60: beq      #0x78ec74
0078eb64: mov      r0, r4
0078eb68: bl       #0x780374
0078eb6c: mov      r1, fp
0078eb70: mov      sb, r0
0078eb74: mov      r0, #0x88
0078eb78: bl       #0x752ba8
0078eb7c: mov      r1, sb
0078eb80: mov      sl, r0
0078eb84: bl       #0x7cf8d8
0078eb88: mov      r0, sl
0078eb8c: ldr      r1, [sp, #0x34]
0078eb90: bl       #0x7ce628
0078eb94: mov      r3, #1
0078eb98: strb     r3, [sl, #0x4c]
0078eb9c: mov      r1, sl
0078eba0: ldr      r0, [sp, #0xc]
0078eba4: bl       #0x764234
0078eba8: mov      r0, r6
0078ebac: ldr      r1, [sp, #0xc]
0078ebb0: bl       #0x78ad54
0078ebb4: ldrb     r3, [sp, #0x398]
0078ebb8: sxtb     sb, r3
0078ebbc: b        #0x78e710
0078ebc0: cmn      sl, #1
0078ebc4: ldrbne   sb, [sp, #0x320]
0078ebc8: ldreq    r3, [sp, #0x324]
0078ebcc: sxtbne   r3, sb
0078ebd0: sub      r3, r3, #1
0078ebd4: sub      r3, r3, #1
0078ebd8: cmp      r3, #0
0078ebdc: ble      #0x78eee0
0078ebe0: ldr      r2, [pc, #0x3b8]
0078ebe4: ldr      ip, [sp, #8]
0078ebe8: ldr      sb, [sp, #0x32c]
0078ebec: ldr      r2, [r8, r2]
0078ebf0: mov      r1, #0
0078ebf4: mov      r0, #0xff000000
0078ebf8: ldr      r2, [r2]
0078ebfc: add      fp, ip, #1
0078ec00: str      r2, [sp, #0x2c]
0078ec04: b        #0x78ec0c
0078ec08: add      r1, r1, #4
0078ec0c: cmn      sl, #1
0078ec10: moveq    r2, sb
0078ec14: movne    r2, fp
0078ec18: ldrsb    r2, [r2, r3]
0078ec1c: cmp      r2, #0xff
0078ec20: ldrls    lr, [sp, #0x2c]
0078ec24: addls    r2, lr, r2, lsl #1
0078ec28: ldrshls  r2, [r2, #2]
0078ec2c: uxtb     r2, r2
0078ec30: uxtb     ip, r2
0078ec34: sub      lr, ip, #0x30
0078ec38: uxtb     lr, lr
0078ec3c: cmp      lr, #9
0078ec40: sxtbls   r2, r2
0078ec44: subls    r2, r2, #0x30
0078ec48: orrls    r0, r0, r2, lsl r1
0078ec4c: bls      #0x78ec68
0078ec50: sub      ip, ip, #0x61
0078ec54: uxtb     ip, ip
0078ec58: cmp      ip, #5
0078ec5c: sxtbls   r2, r2
0078ec60: subls    r2, r2, #0x57
0078ec64: orrls    r0, r0, r2, lsl r1
0078ec68: subs     r3, r3, #1
0078ec6c: bne      #0x78ec08
0078ec70: b        #0x78ea58
0078ec74: ldr      r1, [pc, #0x328]
0078ec78: mov      r0, sl
0078ec7c: add      r1, pc, r1
0078ec80: bl       #0x78aebc
0078ec84: subs     fp, r0, #0
0078ec88: beq      #0x78ed50
0078ec8c: mov      r3, #1
0078ec90: ldr      r1, [sp, #0xc]
0078ec94: mov      r0, r6
0078ec98: strb     r3, [sp, #0x40]
0078ec9c: bl       #0x78ad54
0078eca0: ldrb     ip, [sp, #0x398]
0078eca4: sxtb     sb, ip
0078eca8: b        #0x78e710
0078ecac: cmn      sb, #1
0078ecb0: ldreq    fp, [sp, #0x304]
0078ecb4: addne    r3, sl, #1
0078ecb8: moveq    r1, #0x2b
0078ecbc: moveq    r0, fp
0078ecc0: movne    r0, r3
0078ecc4: movne    r1, #0x2b
0078ecc8: movne    fp, r3
0078eccc: bl       #0x30ec28
0078ecd0: cmp      r0, fp
0078ecd4: beq      #0x78ef30
0078ecd8: cmn      sb, #1
0078ecdc: ldreq    fp, [sp, #0x304]
0078ece0: addne    r3, sl, #1
0078ece4: moveq    r1, #0x2d
0078ece8: moveq    r0, fp
0078ecec: movne    r0, r3
0078ecf0: movne    r1, #0x2d
0078ecf4: movne    fp, r3
0078ecf8: bl       #0x30ec28
0078ecfc: cmp      r0, fp
0078ed00: beq      #0x78eeec
0078ed04: cmn      sb, #1
0078ed08: addne    r0, sl, #1
0078ed0c: ldreq    r0, [sp, #0x304]
0078ed10: bl       #0x30e094
0078ed14: cmp      r0, #0
0078ed18: ble      #0x78e9a8
0078ed1c: ldrb     lr, [sp, #0x2f8]
0078ed20: sxtb     r3, lr
0078ed24: cmn      r3, #1
0078ed28: addne    r0, sl, #1
0078ed2c: ldreq    r0, [sp, #0x304]
0078ed30: bl       #0x30e094
0078ed34: bl       #0x30e964
0078ed38: mov      r1, #0x41000000
0078ed3c: add      r1, r1, #0xa00000
0078ed40: bl       #0x30ed6c
0078ed44: bl       #0x30e4cc
0078ed48: str      r0, [sp, #0x38]
0078ed4c: b        #0x78e9a8
0078ed50: ldr      r1, [pc, #0x250]
0078ed54: mov      r0, sl
0078ed58: add      r1, pc, r1
0078ed5c: bl       #0x78aebc
0078ed60: cmp      r0, #0
0078ed64: ldrbeq   lr, [sp, #0x398]
0078ed68: sxtbeq   sb, lr
0078ed6c: beq      #0x78e710
0078ed70: ldr      r3, [sp, #0x2b8]
0078ed74: ldr      ip, [sp, #0x2e0]
0078ed78: ldr      r2, [sp, #0x2cc]
0078ed7c: mvn      r1, #0
0078ed80: bfi      r3, r1, #0, #0x18
0078ed84: lsr      r0, r3, #0x18
0078ed88: bfi      r2, r1, #0, #0x18
0078ed8c: bfi      ip, r1, #0, #0x18
0078ed90: bfi      r0, fp, #0, #1
0078ed94: add      r1, sp, #0x294
0078ed98: str      r1, [sp, #8]
0078ed9c: strb     r0, [sp, #0x28]
0078eda0: ldr      r1, [pc, #0x204]
0078eda4: str      ip, [sp, #0x2e0]
0078eda8: lsr      lr, ip, #0x18
0078edac: ldrb     ip, [sp, #0x28]
0078edb0: lsr      sl, r2, #0x18
0078edb4: add      sb, sp, #0x2d0
0078edb8: bfi      lr, fp, #0, #1
0078edbc: bfi      sl, fp, #0, #1
0078edc0: add      r1, pc, r1
0078edc4: str      sb, [sp, #0xc]
0078edc8: ldr      r0, [sp, #8]
0078edcc: mov      sb, #1
0078edd0: strb     lr, [sp, #0x2e3]
0078edd4: str      r3, [sp, #0x2b8]
0078edd8: str      r2, [sp, #0x2cc]
0078eddc: strb     ip, [sp, #0x2bb]
0078ede0: strb     sb, [sp, #0x2a8]
0078ede4: strb     sl, [sp, #0x2cf]
0078ede8: strb     sb, [sp, #0x2d0]
0078edec: strb     fp, [sp, #0x2d1]
0078edf0: strb     sb, [sp, #0x2bc]
0078edf4: strb     fp, [sp, #0x2bd]
0078edf8: strb     fp, [sp, #0x2a9]
0078edfc: bl       #0x413a7c
0078ee00: ldr      r2, [sp, #0xc]
0078ee04: ldr      r1, [sp, #8]
0078ee08: mov      r0, r7
0078ee0c: bl       #0x78dfbc
0078ee10: ldr      r0, [sp, #8]
0078ee14: bl       #0x41fed8
0078ee18: ldr      r1, [pc, #0x190]
0078ee1c: add      sl, sp, #0x280
0078ee20: add      fp, sp, #0x2bc
0078ee24: mov      r0, sl
0078ee28: add      r1, pc, r1
0078ee2c: bl       #0x413a7c
0078ee30: mov      r2, fp
0078ee34: mov      r1, sl
0078ee38: mov      r0, r7
0078ee3c: bl       #0x78dfbc
0078ee40: mov      r0, sl
0078ee44: bl       #0x41fed8
0078ee48: ldr      r1, [pc, #0x164]
0078ee4c: add      sl, sp, #0x26c
0078ee50: add      sb, sp, #0x2a8
0078ee54: mov      r0, sl
0078ee58: add      r1, pc, r1
0078ee5c: bl       #0x413a7c
0078ee60: mov      r1, sl
0078ee64: mov      r2, sb
0078ee68: mov      r0, r7
0078ee6c: bl       #0x78dfbc
0078ee70: mov      r0, sl
0078ee74: bl       #0x41fed8
0078ee78: ldrb     r0, [sp, #0x2bc]
0078ee7c: sxtb     r3, r0
0078ee80: cmn      r3, #1
0078ee84: addne    r0, fp, #1
0078ee88: ldreq    r0, [sp, #0x2c8]
0078ee8c: bl       #0x30e094
0078ee90: ldrb     r3, [sp, #0x2a8]
0078ee94: mov      sl, r0
0078ee98: cmp      r3, #0xff
0078ee9c: addne    r0, sb, #1
0078eea0: ldreq    r0, [sp, #0x2b4]
0078eea4: bl       #0x30e094
0078eea8: ldr      r1, [sp, #0xc]
0078eeac: mov      r3, r0
0078eeb0: mov      r2, sl
0078eeb4: mov      r0, r4
0078eeb8: bl       #0x78a990
0078eebc: mov      r0, sb
0078eec0: bl       #0x41fed8
0078eec4: mov      r0, fp
0078eec8: bl       #0x41fed8
0078eecc: ldr      r0, [sp, #0xc]
0078eed0: bl       #0x41fed8
0078eed4: ldrb     r1, [sp, #0x398]
0078eed8: sxtb     sb, r1
0078eedc: b        #0x78e710
0078eee0: mov      r0, #0xff000000
0078eee4: b        #0x78ea58
0078eee8: bl       #0x30e310
0078eeec: cmn      sb, #1
0078eef0: ldr      r2, [r6, #4]
0078eef4: ldreq    r0, [sp, #0x304]
0078eef8: ldr      r3, [r6]
0078eefc: addne    r0, sl, #1
0078ef00: add      r0, r0, #1
0078ef04: add      r3, r3, r2, lsl #4
0078ef08: ldr      sb, [r3, #-0xc]
0078ef0c: bl       #0x30e094
0078ef10: bl       #0x30e964
0078ef14: mov      r1, #0x41000000
0078ef18: add      r1, r1, #0xa00000
0078ef1c: bl       #0x30ed6c
0078ef20: bl       #0x30e4cc
0078ef24: rsb      r0, r0, sb
0078ef28: str      r0, [sp, #0x38]
0078ef2c: b        #0x78e9a8
0078ef30: cmn      sb, #1
0078ef34: ldr      r2, [r6, #4]
0078ef38: ldreq    r0, [sp, #0x304]
0078ef3c: ldr      r3, [r6]
0078ef40: addne    r0, sl, #1
0078ef44: add      r0, r0, #1
0078ef48: add      r3, r3, r2, lsl #4
0078ef4c: ldr      sb, [r3, #-0xc]
0078ef50: bl       #0x30e094
0078ef54: bl       #0x30e964
0078ef58: mov      r1, #0x41000000
0078ef5c: add      r1, r1, #0xa00000
0078ef60: bl       #0x30ed6c
0078ef64: bl       #0x30e4cc
0078ef68: add      r0, r0, sb
0078ef6c: str      r0, [sp, #0x38]
0078ef70: b        #0x78e9a8
0078ef74: eoreq    r6, r0, ip, lsr #14
0078ef78: andeq    r4, r0, ip, lsr #1
0078ef7c: ldrsbeq  r2, [r5], -r4
0078ef80: andseq   ip, r7, r4, lsl sp
0078ef84: andseq   sl, r3, ip, lsl sp
0078ef88: andseq   fp, r7, r8, lsl fp
0078ef8c: andseq   sp, r7, r8, asr #8
0078ef90: andseq   sp, r3, ip, ror r2
0078ef94: andseq   fp, r7, r8, asr r7
0078ef98: andseq   r4, r5, ip, ror #5
0078ef9c: ldrsheq  r6, [r3], -r4
0078efa0: andeq    r3, r0, r0, ror #13
0078efa4: andseq   r8, r5, ip, asr #18
0078efa8: andseq   fp, r7, r0, lsl r2
0078efac: ldrheq   fp, [r7], -r0
0078efb0: andseq   pc, r4, r0, lsl #27
0078efb4: ldrsbeq  sl, [r7], -r8

# _ZN7gameswf19edit_text_character11format_textEb
0078efb8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078efbc: mvn      r2, #0
0078efc0: mvn      r3, #0
0078efc4: mov      r4, r0
0078efc8: strd     r2, r3, [r0, #0xe0]
0078efcc: strd     r2, r3, [r0, #0xd8]
0078efd0: sub      sp, sp, #0x10
0078efd4: add      r0, r0, #0xa4
0078efd8: mov      r6, r1
0078efdc: mov      r1, #0
0078efe0: bl       #0x78bccc
0078efe4: mov      r5, #0
0078efe8: mov      r3, #0
0078efec: mvn      r2, #0
0078eff0: mov      r1, r3
0078eff4: str      r2, [r4, #0x16c]
0078eff8: str      r3, [r4, #0x15c]
0078effc: str      r3, [r4, #0x160]
0078f000: str      r5, [r4, #0x164]
0078f004: str      r5, [r4, #0x168]
0078f008: mov      r0, r4
0078f00c: mov      r2, r3
0078f010: bl       #0x78a370
0078f014: ldr      r1, [r4, #0x178]
0078f018: cmp      r1, r5
0078f01c: beq      #0x78f194
0078f020: cmp      r6, r5
0078f024: beq      #0x78f19c
0078f028: mov      r0, sp
0078f02c: mov      r1, r4
0078f030: str      r5, [sp]
0078f034: str      r5, [sp, #4]
0078f038: str      r5, [sp, #8]
0078f03c: strb     r5, [sp, #0xc]
0078f040: bl       #0x78e354
0078f044: mov      r0, sp
0078f048: mov      r1, r5
0078f04c: bl       #0x78bc0c
0078f050: mov      r0, sp
0078f054: mov      r1, r5
0078f058: mov      r6, sp
0078f05c: bl       #0x78a67c
0078f060: ldr      r3, [r4, #0x15c]
0078f064: mov      r0, r4
0078f068: ldr      r1, [r4, #0x17c]
0078f06c: ldr      r2, [r4, #0x164]
0078f070: bl       #0x78a398
0078f074: ldr      r3, [r4, #0xa0]
0078f078: ldrb     r5, [r3, #0x49]
0078f07c: cmp      r5, #0
0078f080: bne      #0x78f178
0078f084: ldr      r8, [r4, #0xa8]
0078f088: cmp      r8, #1
0078f08c: ble      #0x78f178
0078f090: ldr      r6, [r4, #0xa4]
0078f094: mov      r7, r5
0078f098: mov      sb, #0
0078f09c: add      r3, r6, r5
0078f0a0: ldrb     r2, [r3, #0x1d]
0078f0a4: mov      r1, sb
0078f0a8: add      r7, r7, #1
0078f0ac: cmp      r2, #0
0078f0b0: add      r5, r5, #0x30
0078f0b4: beq      #0x78f0f4
0078f0b8: ldr      sl, [r3, #0x14]
0078f0bc: mov      r0, sl
0078f0c0: bl       #0x30e2f8
0078f0c4: cmp      r0, #0
0078f0c8: beq      #0x78f0f4
0078f0cc: cmp      r7, r8
0078f0d0: beq      #0x78f100
0078f0d4: add      r3, r6, r5
0078f0d8: ldrb     r2, [r3, #0x1d]
0078f0dc: mov      sb, sl
0078f0e0: mov      r1, sb
0078f0e4: cmp      r2, #0
0078f0e8: add      r7, r7, #1
0078f0ec: add      r5, r5, #0x30
0078f0f0: bne      #0x78f0b8
0078f0f4: cmp      r7, r8
0078f0f8: mov      sl, sb
0078f0fc: bne      #0x78f0d4
0078f100: mov      r0, sl
0078f104: mov      r1, #0xbf000000
0078f108: bl       #0x30ed6c
0078f10c: mov      r1, #0xbf000000
0078f110: mov      r5, r0
0078f114: ldr      r0, [r6, #0x18]
0078f118: bl       #0x30ed6c
0078f11c: ldr      r1, [r6, #0x14]
0078f120: bl       #0x30eba4
0078f124: mov      r1, r0
0078f128: mov      r0, r5
0078f12c: bl       #0x30eba4
0078f130: mov      r5, #0
0078f134: mov      sl, r0
0078f138: mov      r7, r5
0078f13c: b        #0x78f144
0078f140: ldr      r6, [r4, #0xa4]
0078f144: add      r6, r6, r5
0078f148: ldrb     r3, [r6, #0x1d]
0078f14c: mov      r1, sl
0078f150: add      r7, r7, #1
0078f154: cmp      r3, #0
0078f158: add      r5, r5, #0x30
0078f15c: beq      #0x78f170
0078f160: ldr      r0, [r6, #0x14]
0078f164: bl       #0x30eba4
0078f168: str      r0, [r6, #0x14]
0078f16c: ldr      r8, [r4, #0xa8]
0078f170: cmp      r7, r8
0078f174: blt      #0x78f140
0078f178: mov      r0, r4
0078f17c: bl       #0x78dde8
0078f180: ldrb     r3, [r0, #0x87]
0078f184: cmp      r3, #0
0078f188: beq      #0x78f194
0078f18c: mov      r0, r4
0078f190: bl       #0x78c2d0
0078f194: add      sp, sp, #0x10
0078f198: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0078f19c: ldr      r3, [r4, #0x170]
0078f1a0: mov      r2, #0xc
0078f1a4: mov      r0, sp
0078f1a8: stmib    sp, {r2, r3}
0078f1ac: str      r6, [sp]
0078f1b0: strb     r6, [sp, #0xc]
0078f1b4: bl       #0x764234
0078f1b8: ldr      r0, [r4, #0x174]
0078f1bc: bl       #0x30e4cc
0078f1c0: mov      r2, sp
0078f1c4: str      r0, [sp, #4]
0078f1c8: mov      r3, r6
0078f1cc: mov      r0, r4
0078f1d0: add      r1, r4, #0x138
0078f1d4: bl       #0x78cb90
0078f1d8: ldr      r0, [sp]
0078f1dc: cmp      r0, #0
0078f1e0: beq      #0x78f060
0078f1e4: bl       #0x75a240
0078f1e8: b        #0x78f060

# _ZN7gameswf19edit_text_character8set_textERKNS_9tu_stringEb
0078f1ec: push     {r4, r5, r6, r7, r8, lr}
0078f1f0: add      r6, r0, #0x138
0078f1f4: cmp      r6, r1
0078f1f8: mov      r4, r0
0078f1fc: mov      r5, r1
0078f200: mov      r7, r2
0078f204: beq      #0x78f234
0078f208: ldrb     r3, [r0, #0x138]
0078f20c: cmp      r3, #0xff
0078f210: ldrsb    r3, [r1]
0078f214: addne    r0, r6, #1
0078f218: ldreq    r0, [r4, #0x144]
0078f21c: cmn      r3, #1
0078f220: addne    r1, r1, #1
0078f224: ldreq    r1, [r5, #0xc]
0078f228: bl       #0x30e31c
0078f22c: cmp      r0, #0
0078f230: bne      #0x78f238
0078f234: pop      {r4, r5, r6, r7, r8, pc}
0078f238: mov      r1, r5
0078f23c: mov      r0, r6
0078f240: bl       #0x752f50
0078f244: ldr      r3, [r4, #0xa0]
0078f248: ldr      r1, [r3, #0x64]
0078f24c: cmp      r1, #0
0078f250: ble      #0x78f278
0078f254: ldrb     r3, [r4, #0x138]
0078f258: sxtb     r3, r3
0078f25c: cmn      r3, #1
0078f260: ldreq    r3, [r4, #0x13c]
0078f264: sub      r3, r3, #1
0078f268: cmp      r1, r3
0078f26c: bge      #0x78f278
0078f270: mov      r0, r6
0078f274: bl       #0x751d14
0078f278: mov      r0, r4
0078f27c: mov      r1, r7
0078f280: pop      {r4, r5, r6, r7, r8, lr}
0078f284: b        #0x78efb8

# _ZN7gameswf19edit_text_character12reset_formatEPNS_13as_textformatE
0078f288: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078f28c: ldr      r7, [pc, #0x718]
0078f290: ldr      r8, [pc, #0x718]
0078f294: ldr      r3, [r1]
0078f298: add      r7, pc, r7
0078f29c: ldr      r2, [r7, r8]
0078f2a0: mov      r4, r1
0078f2a4: sub      sp, sp, #0x10c
0078f2a8: ldr      r1, [r2]
0078f2ac: add      sb, sp, #0xf0
0078f2b0: mov      r2, #0
0078f2b4: str      r1, [sp, #0x104]
0078f2b8: ldr      r1, [pc, #0x6f4]
0078f2bc: strb     r2, [sp, #9]
0078f2c0: strb     r2, [sp, #8]
0078f2c4: add      r1, pc, r1
0078f2c8: mov      r6, r0
0078f2cc: add      r5, sp, #8
0078f2d0: mov      r0, sb
0078f2d4: ldr      sl, [r3, #0x20]
0078f2d8: bl       #0x413a7c
0078f2dc: mov      r1, sb
0078f2e0: mov      r0, r4
0078f2e4: mov      r2, r5
0078f2e8: blx      sl
0078f2ec: ldrsb    r3, [sp, #0xf0]
0078f2f0: mov      sl, r0
0078f2f4: cmn      r3, #1
0078f2f8: beq      #0x78f8e8
0078f2fc: cmp      sl, #0
0078f300: bne      #0x78f6b0
0078f304: ldr      r1, [pc, #0x6ac]
0078f308: ldr      r3, [r4]
0078f30c: add      sb, sp, #0xdc
0078f310: add      r1, pc, r1
0078f314: mov      r0, sb
0078f318: ldr      sl, [r3, #0x20]
0078f31c: bl       #0x413a7c
0078f320: mov      r0, r4
0078f324: mov      r1, sb
0078f328: mov      r2, r5
0078f32c: blx      sl
0078f330: ldrsb    r3, [sp, #0xdc]
0078f334: mov      sl, r0
0078f338: cmn      r3, #1
0078f33c: beq      #0x78f928
0078f340: cmp      sl, #0
0078f344: bne      #0x78f6f0
0078f348: ldr      r1, [pc, #0x66c]
0078f34c: ldr      r3, [r4]
0078f350: add      sb, sp, #0xc8
0078f354: add      r1, pc, r1
0078f358: mov      r0, sb
0078f35c: ldr      sl, [r3, #0x20]
0078f360: bl       #0x413a7c
0078f364: mov      r0, r4
0078f368: mov      r1, sb
0078f36c: mov      r2, r5
0078f370: blx      sl
0078f374: ldrsb    r3, [sp, #0xc8]
0078f378: mov      sl, r0
0078f37c: cmn      r3, #1
0078f380: beq      #0x78f8f8
0078f384: cmp      sl, #0
0078f388: bne      #0x78f6d0
0078f38c: ldr      r1, [pc, #0x62c]
0078f390: ldr      r3, [r4]
0078f394: add      sb, sp, #0xb4
0078f398: add      r1, pc, r1
0078f39c: mov      r0, sb
0078f3a0: ldr      sl, [r3, #0x20]
0078f3a4: bl       #0x413a7c
0078f3a8: mov      r0, r4
0078f3ac: mov      r1, sb
0078f3b0: mov      r2, r5
0078f3b4: blx      sl
0078f3b8: ldrsb    r3, [sp, #0xb4]
0078f3bc: mov      sl, r0
0078f3c0: cmn      r3, #1
0078f3c4: beq      #0x78f908
0078f3c8: cmp      sl, #0
0078f3cc: bne      #0x78f89c
0078f3d0: ldr      r1, [pc, #0x5ec]
0078f3d4: ldr      r3, [r4]
0078f3d8: add      sb, sp, #0xa0
0078f3dc: add      r1, pc, r1
0078f3e0: mov      r0, sb
0078f3e4: ldr      sl, [r3, #0x20]
0078f3e8: bl       #0x413a7c
0078f3ec: mov      r0, r4
0078f3f0: mov      r1, sb
0078f3f4: mov      r2, r5
0078f3f8: blx      sl
0078f3fc: ldrsb    r3, [sp, #0xa0]
0078f400: mov      sl, r0
0078f404: cmn      r3, #1
0078f408: beq      #0x78f978
0078f40c: cmp      sl, #0
0078f410: bne      #0x78f87c
0078f414: ldr      r1, [pc, #0x5ac]
0078f418: ldr      r3, [r4]
0078f41c: add      sb, sp, #0x8c
0078f420: add      r1, pc, r1
0078f424: mov      r0, sb
0078f428: ldr      sl, [r3, #0x20]
0078f42c: bl       #0x413a7c
0078f430: mov      r0, r4
0078f434: mov      r1, sb
0078f438: mov      r2, r5
0078f43c: blx      sl
0078f440: ldrsb    r3, [sp, #0x8c]
0078f444: mov      sl, r0
0078f448: cmn      r3, #1
0078f44c: beq      #0x78f988
0078f450: cmp      sl, #0
0078f454: bne      #0x78f850
0078f458: ldr      r1, [pc, #0x56c]
0078f45c: ldr      r3, [r4]
0078f460: add      sb, sp, #0x78
0078f464: add      r1, pc, r1
0078f468: mov      r0, sb
0078f46c: ldr      sl, [r3, #0x20]
0078f470: bl       #0x413a7c
0078f474: mov      r0, r4
0078f478: mov      r1, sb
0078f47c: mov      r2, r5
0078f480: blx      sl
0078f484: ldrsb    r3, [sp, #0x78]
0078f488: mov      sl, r0
0078f48c: cmn      r3, #1
0078f490: beq      #0x78f918
0078f494: cmp      sl, #0
0078f498: bne      #0x78f830
0078f49c: ldr      r1, [pc, #0x52c]
0078f4a0: ldr      r3, [r4]
0078f4a4: add      sb, sp, #0x64
0078f4a8: add      r1, pc, r1
0078f4ac: mov      r0, sb
0078f4b0: ldr      sl, [r3, #0x20]
0078f4b4: bl       #0x413a7c
0078f4b8: mov      r0, r4
0078f4bc: mov      r1, sb
0078f4c0: mov      r2, r5
0078f4c4: blx      sl
0078f4c8: ldrsb    r3, [sp, #0x64]
0078f4cc: mov      sl, r0
0078f4d0: cmn      r3, #1
0078f4d4: beq      #0x78f968
0078f4d8: cmp      sl, #0
0078f4dc: bne      #0x78f794
0078f4e0: ldr      r1, [r6, #0x178]
0078f4e4: add      sb, sp, #0x50
0078f4e8: mov      r0, sb
0078f4ec: add      r1, r1, #0x30
0078f4f0: bl       #0x75302c
0078f4f4: ldr      r1, [pc, #0x4d8]
0078f4f8: ldr      r3, [r4]
0078f4fc: add      fp, sp, #0x3c
0078f500: add      r1, pc, r1
0078f504: mov      r0, fp
0078f508: ldr      sl, [r3, #0x20]
0078f50c: bl       #0x413a7c
0078f510: mov      r0, r4
0078f514: mov      r1, fp
0078f518: mov      r2, r5
0078f51c: blx      sl
0078f520: ldrsb    r3, [sp, #0x3c]
0078f524: mov      sl, r0
0078f528: cmn      r3, #1
0078f52c: beq      #0x78f998
0078f530: cmp      sl, #0
0078f534: bne      #0x78f77c
0078f538: ldr      r3, [r6, #0x178]
0078f53c: ldr      r1, [pc, #0x494]
0078f540: ldr      r2, [r4]
0078f544: ldrb     r3, [r3, #0x4d]
0078f548: add      fp, sp, #0x28
0078f54c: add      r1, pc, r1
0078f550: mov      r0, fp
0078f554: ldr      sl, [r2, #0x20]
0078f558: str      r3, [sp, #4]
0078f55c: bl       #0x413a7c
0078f560: mov      r0, r4
0078f564: mov      r1, fp
0078f568: mov      r2, r5
0078f56c: blx      sl
0078f570: ldrsb    r3, [sp, #0x28]
0078f574: mov      sl, r0
0078f578: cmn      r3, #1
0078f57c: beq      #0x78f948
0078f580: cmp      sl, #0
0078f584: bne      #0x78f76c
0078f588: ldr      r3, [r4]
0078f58c: ldr      r1, [pc, #0x448]
0078f590: ldr      r2, [r6, #0x178]
0078f594: ldr      r3, [r3, #0x20]
0078f598: add      fp, sp, #0x14
0078f59c: add      r1, pc, r1
0078f5a0: mov      r0, fp
0078f5a4: ldrb     sl, [r2, #0x4c]
0078f5a8: str      r3, [sp]
0078f5ac: bl       #0x413a7c
0078f5b0: mov      r0, r4
0078f5b4: ldr      r3, [sp]
0078f5b8: mov      r1, fp
0078f5bc: mov      r2, r5
0078f5c0: blx      r3
0078f5c4: ldrsb    r3, [sp, #0x14]
0078f5c8: mov      r4, r0
0078f5cc: cmn      r3, #1
0078f5d0: beq      #0x78f958
0078f5d4: cmp      r4, #0
0078f5d8: bne      #0x78f710
0078f5dc: ldr      r1, [r6, #0x178]
0078f5e0: ldrb     r3, [r1, #0x4c]
0078f5e4: cmp      r3, sl
0078f5e8: beq      #0x78f72c
0078f5ec: ldr      r3, [r6]
0078f5f0: mov      r0, r6
0078f5f4: mov      r1, sb
0078f5f8: mov      lr, pc
0078f5fc: ldr      pc, [r3, #0x84]
0078f600: subs     r4, r0, #0
0078f604: beq      #0x78f620
0078f608: ldr      r3, [r4]
0078f60c: mov      r1, #0x13
0078f610: mov      lr, pc
0078f614: ldr      pc, [r3, #8]
0078f618: cmp      r0, #0
0078f61c: bne      #0x78f8bc
0078f620: mov      r0, r6
0078f624: bl       #0x780374
0078f628: mov      r1, #0
0078f62c: mov      r4, r0
0078f630: mov      r0, #0x88
0078f634: bl       #0x752ba8
0078f638: mov      r1, r4
0078f63c: mov      fp, r0
0078f640: bl       #0x7cf8d8
0078f644: mov      r1, fp
0078f648: add      r0, r6, #0x178
0078f64c: bl       #0x764234
0078f650: ldr      r3, [r6, #0x178]
0078f654: ldr      r2, [sp, #4]
0078f658: mov      r1, sb
0078f65c: strb     r2, [r3, #0x4d]
0078f660: ldr      r3, [r6, #0x178]
0078f664: strb     sl, [r3, #0x4c]
0078f668: ldr      r0, [r6, #0x178]
0078f66c: add      r0, r0, #0x30
0078f670: bl       #0x752f50
0078f674: mov      r0, r6
0078f678: mov      r1, #0
0078f67c: bl       #0x78efb8
0078f680: ldrsb    r3, [sp, #0x50]
0078f684: cmn      r3, #1
0078f688: beq      #0x78f938
0078f68c: mov      r0, r5
0078f690: bl       #0x797124
0078f694: ldr      r3, [r7, r8]
0078f698: ldr      r2, [sp, #0x104]
0078f69c: ldr      r3, [r3]
0078f6a0: cmp      r2, r3
0078f6a4: bne      #0x78f9a8
0078f6a8: add      sp, sp, #0x10c
0078f6ac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078f6b0: mov      r0, r5
0078f6b4: bl       #0x797a54
0078f6b8: bl       #0x30e6a0
0078f6bc: mov      r1, #0x41000000
0078f6c0: add      r1, r1, #0xa00000
0078f6c4: bl       #0x30ed6c
0078f6c8: str      r0, [r6, #0x180]
0078f6cc: b        #0x78f304
0078f6d0: mov      r0, r5
0078f6d4: bl       #0x797a54
0078f6d8: bl       #0x30e6a0
0078f6dc: mov      r1, #0x41000000
0078f6e0: add      r1, r1, #0xa00000
0078f6e4: bl       #0x30ed6c
0078f6e8: str      r0, [r6, #0x184]
0078f6ec: b        #0x78f38c
0078f6f0: mov      r0, r5
0078f6f4: bl       #0x797a54
0078f6f8: bl       #0x30e6a0
0078f6fc: mov      r1, #0x41000000
0078f700: add      r1, r1, #0xa00000
0078f704: bl       #0x30ed6c
0078f708: str      r0, [r6, #0x188]
0078f70c: b        #0x78f348
0078f710: mov      r0, r5
0078f714: bl       #0x797960
0078f718: ldr      r1, [r6, #0x178]
0078f71c: mov      sl, r0
0078f720: ldrb     r3, [r1, #0x4c]
0078f724: cmp      r3, sl
0078f728: bne      #0x78f5ec
0078f72c: ldrb     r3, [r1, #0x4d]
0078f730: ldr      r2, [sp, #4]
0078f734: cmp      r3, r2
0078f738: bne      #0x78f5ec
0078f73c: ldrsb    r3, [sp, #0x50]
0078f740: cmn      r3, #1
0078f744: ldrsb    r3, [r1, #0x30]
0078f748: addne    r0, sb, #1
0078f74c: ldreq    r0, [sp, #0x5c]
0078f750: cmn      r3, #1
0078f754: addne    r1, r1, #0x31
0078f758: ldreq    r1, [r1, #0x3c]
0078f75c: bl       #0x30e31c
0078f760: cmp      r0, #0
0078f764: beq      #0x78f674
0078f768: b        #0x78f5ec
0078f76c: mov      r0, r5
0078f770: bl       #0x797960
0078f774: str      r0, [sp, #4]
0078f778: b        #0x78f588
0078f77c: mov      r0, r5
0078f780: bl       #0x420a84
0078f784: mov      r1, r0
0078f788: mov      r0, sb
0078f78c: bl       #0x752f50
0078f790: b        #0x78f538
0078f794: mov      r0, r5
0078f798: bl       #0x420a84
0078f79c: ldrsb    r3, [r0]
0078f7a0: ldr      r1, [pc, #0x238]
0078f7a4: cmn      r3, #1
0078f7a8: ldreq    r0, [r0, #0xc]
0078f7ac: addne    r0, r0, #1
0078f7b0: add      r1, pc, r1
0078f7b4: bl       #0x30e31c
0078f7b8: cmp      r0, #0
0078f7bc: streq    r0, [r6, #0x17c]
0078f7c0: beq      #0x78f4e0
0078f7c4: mov      r0, r5
0078f7c8: bl       #0x420a84
0078f7cc: ldr      r1, [pc, #0x210]
0078f7d0: add      r1, pc, r1
0078f7d4: bl       #0x78aebc
0078f7d8: cmp      r0, #0
0078f7dc: movne    r3, #2
0078f7e0: strne    r3, [r6, #0x17c]
0078f7e4: bne      #0x78f4e0
0078f7e8: mov      r0, r5
0078f7ec: bl       #0x420a84
0078f7f0: ldr      r1, [pc, #0x1f0]
0078f7f4: add      r1, pc, r1
0078f7f8: bl       #0x78aebc
0078f7fc: cmp      r0, #0
0078f800: movne    r3, #1
0078f804: strne    r3, [r6, #0x17c]
0078f808: bne      #0x78f4e0
0078f80c: mov      r0, r5
0078f810: bl       #0x420a84
0078f814: ldr      r1, [pc, #0x1d0]
0078f818: add      r1, pc, r1
0078f81c: bl       #0x78aebc
0078f820: cmp      r0, #0
0078f824: movne    r3, #3
0078f828: strne    r3, [r6, #0x17c]
0078f82c: b        #0x78f4e0
0078f830: mov      r0, r5
0078f834: bl       #0x797a54
0078f838: bl       #0x30e6a0
0078f83c: mov      r1, #0x41000000
0078f840: add      r1, r1, #0xa00000
0078f844: bl       #0x30ed6c
0078f848: str      r0, [r6, #0x174]
0078f84c: b        #0x78f49c
0078f850: mov      r0, r5
0078f854: bl       #0x797a54
0078f858: bl       #0x30ea24
0078f85c: asr      r3, r0, #8
0078f860: asr      r2, r0, #0x10
0078f864: strb     r3, [r6, #0x171]
0078f868: mvn      r3, #0
0078f86c: strb     r2, [r6, #0x170]
0078f870: strb     r0, [r6, #0x172]
0078f874: strb     r3, [r6, #0x173]
0078f878: b        #0x78f458
0078f87c: mov      r0, r5
0078f880: bl       #0x797a54
0078f884: bl       #0x30e6a0
0078f888: mov      r1, #0x41000000
0078f88c: add      r1, r1, #0xa00000
0078f890: bl       #0x30ed6c
0078f894: str      r0, [r6, #0x190]
0078f898: b        #0x78f414
0078f89c: mov      r0, r5
0078f8a0: bl       #0x797a54
0078f8a4: bl       #0x30e6a0
0078f8a8: mov      r1, #0x41000000
0078f8ac: add      r1, r1, #0xa00000
0078f8b0: bl       #0x30ed6c
0078f8b4: str      r0, [r6, #0x18c]
0078f8b8: b        #0x78f3d0
0078f8bc: mov      r1, #0x13
0078f8c0: ldr      r3, [r4]
0078f8c4: mov      r0, r4
0078f8c8: mov      lr, pc
0078f8cc: ldr      pc, [r3, #8]
0078f8d0: cmp      r0, #0
0078f8d4: movne    r1, r4
0078f8d8: moveq    r1, #0
0078f8dc: add      r0, r6, #0x178
0078f8e0: bl       #0x764234
0078f8e4: b        #0x78f650
0078f8e8: ldr      r0, [sp, #0xfc]
0078f8ec: ldr      r1, [sp, #0xf8]
0078f8f0: bl       #0x752b38
0078f8f4: b        #0x78f2fc
0078f8f8: ldr      r0, [sp, #0xd4]
0078f8fc: ldr      r1, [sp, #0xd0]
0078f900: bl       #0x752b38
0078f904: b        #0x78f384
0078f908: ldr      r0, [sp, #0xc0]
0078f90c: ldr      r1, [sp, #0xbc]
0078f910: bl       #0x752b38
0078f914: b        #0x78f3c8
0078f918: ldr      r0, [sp, #0x84]
0078f91c: ldr      r1, [sp, #0x80]
0078f920: bl       #0x752b38
0078f924: b        #0x78f494
0078f928: ldr      r0, [sp, #0xe8]
0078f92c: ldr      r1, [sp, #0xe4]
0078f930: bl       #0x752b38
0078f934: b        #0x78f340
0078f938: ldr      r0, [sp, #0x5c]
0078f93c: ldr      r1, [sp, #0x58]
0078f940: bl       #0x752b38
0078f944: b        #0x78f68c
0078f948: ldr      r0, [sp, #0x34]
0078f94c: ldr      r1, [sp, #0x30]
0078f950: bl       #0x752b38
0078f954: b        #0x78f580
0078f958: ldr      r0, [sp, #0x20]
0078f95c: ldr      r1, [sp, #0x1c]
0078f960: bl       #0x752b38
0078f964: b        #0x78f5d4
0078f968: ldr      r0, [sp, #0x70]
0078f96c: ldr      r1, [sp, #0x6c]
0078f970: bl       #0x752b38
0078f974: b        #0x78f4d8
0078f978: ldr      r0, [sp, #0xac]
0078f97c: ldr      r1, [sp, #0xa8]
0078f980: bl       #0x752b38
0078f984: b        #0x78f40c
0078f988: ldr      r0, [sp, #0x98]
0078f98c: ldr      r1, [sp, #0x94]
0078f990: bl       #0x752b38
0078f994: b        #0x78f450
0078f998: ldr      r0, [sp, #0x48]
0078f99c: ldr      r1, [sp, #0x44]
0078f9a0: bl       #0x752b38
0078f9a4: b        #0x78f530
0078f9a8: bl       #0x30e310

# _ZN7gameswf19edit_text_character14set_text_valueERKNS_9tu_stringEb
00790ab0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00790ab4: ldr      r4, [pc, #0x1e0]
00790ab8: ldr      r6, [pc, #0x1e0]
00790abc: sub      sp, sp, #0x50
00790ac0: add      r4, pc, r4
00790ac4: ldr      r3, [r4, r6]
00790ac8: mov      r5, r0
00790acc: mov      sl, r1
00790ad0: ldr      r3, [r3]
00790ad4: str      r3, [sp, #0x4c]
00790ad8: bl       #0x78f1ec
00790adc: mov      r0, r5
00790ae0: bl       #0x78a364
00790ae4: ldrsb    r3, [r0]
00790ae8: cmn      r3, #1
00790aec: ldreq    r3, [r0, #4]
00790af0: sub      r3, r3, #1
00790af4: cmp      r3, #0
00790af8: ble      #0x790c1c
00790afc: ldr      r7, [r5, #0x40]
00790b00: cmp      r7, #0
00790b04: beq      #0x790b18
00790b08: ldr      r0, [r5, #0x3c]
00790b0c: ldrb     r3, [r0, #4]
00790b10: cmp      r3, #0
00790b14: beq      #0x790c38
00790b18: ldr      r3, [sp, #0x48]
00790b1c: mvn      r1, #0
00790b20: mov      r2, #0
00790b24: bfi      r3, r1, #0, #0x18
00790b28: lsr      r1, r3, #0x18
00790b2c: bfi      r1, r2, #0, #1
00790b30: mov      ip, #1
00790b34: mov      r0, r5
00790b38: str      r3, [sp, #0x48]
00790b3c: strb     ip, [sp, #0x38]
00790b40: strb     r2, [sp, #0x39]
00790b44: strb     r1, [sp, #0x4b]
00790b48: bl       #0x78a364
00790b4c: add      r8, sp, #0x24
00790b50: mov      r1, r0
00790b54: mov      r0, r8
00790b58: bl       #0x75302c
00790b5c: mov      r0, r5
00790b60: add      r5, sp, #0x38
00790b64: bl       #0x78a364
00790b68: mov      r1, r5
00790b6c: mov      r2, r8
00790b70: bl       #0x7cd130
00790b74: cmp      r0, #0
00790b78: beq      #0x790b98
00790b7c: ldrsb    r3, [sp, #0x38]
00790b80: mov      r0, r7
00790b84: cmn      r3, #1
00790b88: addne    r1, r5, #1
00790b8c: ldreq    r1, [sp, #0x44]
00790b90: bl       #0x76b284
00790b94: mov      r7, r0
00790b98: cmp      r7, #0
00790b9c: beq      #0x790c04
00790ba0: ldr      r3, [r7]
00790ba4: add      sb, sp, #0x10
00790ba8: mov      r1, r8
00790bac: mov      r0, sb
00790bb0: ldr      r8, [r3, #0x1c]
00790bb4: bl       #0x75302c
00790bb8: ldrsb    r3, [sl]
00790bbc: add      r5, sp, #4
00790bc0: mov      r0, r5
00790bc4: cmn      r3, #1
00790bc8: ldreq    r1, [sl, #0xc]
00790bcc: mov      r3, #0
00790bd0: addne    r1, sl, #1
00790bd4: strb     r3, [sp, #5]
00790bd8: strb     r3, [sp, #4]
00790bdc: bl       #0x797350
00790be0: mov      r1, sb
00790be4: mov      r2, r5
00790be8: mov      r0, r7
00790bec: blx      r8
00790bf0: mov      r0, r5
00790bf4: bl       #0x797124
00790bf8: ldrsb    r3, [sp, #0x10]
00790bfc: cmn      r3, #1
00790c00: beq      #0x790c60
00790c04: ldrsb    r3, [sp, #0x24]
00790c08: cmn      r3, #1
00790c0c: beq      #0x790c70
00790c10: ldrsb    r3, [sp, #0x38]
00790c14: cmn      r3, #1
00790c18: beq      #0x790c88
00790c1c: ldr      r3, [r4, r6]
00790c20: ldr      r2, [sp, #0x4c]
00790c24: ldr      r3, [r3]
00790c28: cmp      r2, r3
00790c2c: bne      #0x790c98
00790c30: add      sp, sp, #0x50
00790c34: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00790c38: ldr      r1, [r0]
00790c3c: sub      r1, r1, #1
00790c40: cmp      r1, #0
00790c44: str      r1, [r0]
00790c48: bne      #0x790c50
00790c4c: bl       #0x752b38
00790c50: mov      r7, #0
00790c54: str      r7, [r5, #0x3c]
00790c58: str      r7, [r5, #0x40]
00790c5c: b        #0x790b18
00790c60: ldr      r0, [sp, #0x1c]
00790c64: ldr      r1, [sp, #0x18]
00790c68: bl       #0x752b38
00790c6c: b        #0x790c04
00790c70: ldr      r0, [sp, #0x30]
00790c74: ldr      r1, [sp, #0x2c]
00790c78: bl       #0x752b38
00790c7c: ldrsb    r3, [sp, #0x38]
00790c80: cmn      r3, #1
00790c84: bne      #0x790c1c
00790c88: ldr      r0, [sp, #0x44]
00790c8c: ldr      r1, [sp, #0x40]
00790c90: bl       #0x752b38
00790c94: b        #0x790c1c
00790c98: bl       #0x30e310

# _ZN7gameswf19edit_text_character10set_memberERKNS_10tu_stringiERKNS_8as_valueE
00790ca4: push     {r4, r5, r6, r7, r8, sl, lr}
00790ca8: ldr      r4, [pc, #0x298]
00790cac: ldr      r6, [pc, #0x298]
00790cb0: sub      sp, sp, #0x34
00790cb4: add      r4, pc, r4
00790cb8: ldr      r3, [r4, r6]
00790cbc: mov      r5, r0
00790cc0: mov      r0, r1
00790cc4: ldr      r3, [r3]
00790cc8: mov      r7, r1
00790ccc: mov      r8, r2
00790cd0: str      r3, [sp, #0x2c]
00790cd4: bl       #0x772030
00790cd8: sub      r0, r0, #0x16
00790cdc: cmp      r0, #9
00790ce0: addls    pc, pc, r0, lsl #2
00790ce4: b        #0x790d44
00790ce8: b        #0x790da8
00790cec: b        #0x790e0c
00790cf0: b        #0x790d44
00790cf4: b        #0x790d44
00790cf8: b        #0x790e70
00790cfc: b        #0x790ea8
00790d00: b        #0x790ec8
00790d04: b        #0x790ee8
00790d08: b        #0x790d10
00790d0c: b        #0x790d70
00790d10: mov      r0, r8
00790d14: bl       #0x420a84
00790d18: ldrsb    r3, [r0]
00790d1c: ldr      r1, [pc, #0x22c]
00790d20: cmn      r3, #1
00790d24: addne    r0, r0, #1
00790d28: ldreq    r0, [r0, #0xc]
00790d2c: add      r1, pc, r1
00790d30: bl       #0x751d10
00790d34: cmp      r0, #0
00790d38: bne      #0x790f08
00790d3c: ldr      r3, [r5, #0xa0]
00790d40: strb     r0, [r3, #0x4b]
00790d44: mov      r2, r8
00790d48: mov      r0, r5
00790d4c: mov      r1, r7
00790d50: bl       #0x75361c
00790d54: ldr      r3, [r4, r6]
00790d58: ldr      r2, [sp, #0x2c]
00790d5c: ldr      r3, [r3]
00790d60: cmp      r2, r3
00790d64: bne      #0x790f44
00790d68: add      sp, sp, #0x34
00790d6c: pop      {r4, r5, r6, r7, r8, sl, pc}
00790d70: mov      r0, r8
00790d74: bl       #0x797a54
00790d78: bl       #0x30ea24
00790d7c: mvn      r1, #0
00790d80: ubfx     r3, r0, #0x10, #8
00790d84: ubfx     r2, r0, #8, #8
00790d88: strb     r1, [r5, #0x197]
00790d8c: strb     r0, [r5, #0x196]
00790d90: strb     r2, [r5, #0x195]
00790d94: strb     r3, [r5, #0x194]
00790d98: mov      r0, r5
00790d9c: mov      r1, #0
00790da0: bl       #0x78efb8
00790da4: b        #0x790d44
00790da8: ldr      r3, [sp, #0x28]
00790dac: mvn      r2, #0
00790db0: mov      sl, #0
00790db4: bfi      r3, r2, #0, #0x18
00790db8: lsr      r2, r3, #0x18
00790dbc: bfi      r2, sl, #0, #1
00790dc0: mov      ip, #1
00790dc4: add      r1, sp, #0x18
00790dc8: mov      r0, r8
00790dcc: str      r3, [sp, #0x28]
00790dd0: strb     ip, [sp, #0x18]
00790dd4: strb     r2, [sp, #0x2b]
00790dd8: strb     sl, [sp, #0x19]
00790ddc: bl       #0x797b7c
00790de0: mov      r2, sl
00790de4: mov      r1, r0
00790de8: mov      r0, r5
00790dec: bl       #0x790ab0
00790df0: ldrsb    r3, [sp, #0x18]
00790df4: cmn      r3, #1
00790df8: bne      #0x790d44
00790dfc: ldr      r0, [sp, #0x24]
00790e00: ldr      r1, [sp, #0x20]
00790e04: bl       #0x752b38
00790e08: b        #0x790d44
00790e0c: ldr      r3, [sp, #0x14]
00790e10: mvn      r1, #0
00790e14: mov      r2, #0
00790e18: bfi      r3, r1, #0, #0x18
00790e1c: lsr      ip, r3, #0x18
00790e20: bfi      ip, r2, #0, #1
00790e24: mov      sl, #1
00790e28: add      r1, sp, #4
00790e2c: mov      r0, r8
00790e30: str      r3, [sp, #0x14]
00790e34: strb     r2, [sp, #5]
00790e38: strb     ip, [sp, #0x17]
00790e3c: strb     sl, [sp, #4]
00790e40: bl       #0x797b7c
00790e44: mov      r2, sl
00790e48: mov      r1, r0
00790e4c: mov      r0, r5
00790e50: bl       #0x790ab0
00790e54: ldrsb    r3, [sp, #4]
00790e58: cmn      r3, #1
00790e5c: bne      #0x790d44
00790e60: ldr      r0, [sp, #0x10]
00790e64: ldr      r1, [sp, #0xc]
00790e68: bl       #0x752b38
00790e6c: b        #0x790d44
00790e70: mov      r0, r8
00790e74: bl       #0x797a54
00790e78: bl       #0x30ea24
00790e7c: ubfx     r3, r0, #8, #8
00790e80: ubfx     r2, r0, #0x10, #8
00790e84: strb     r3, [r5, #0x171]
00790e88: mvn      r3, #0
00790e8c: strb     r0, [r5, #0x172]
00790e90: strb     r2, [r5, #0x170]
00790e94: strb     r3, [r5, #0x173]
00790e98: mov      r0, r5
00790e9c: mov      r1, #0
00790ea0: bl       #0x78efb8
00790ea4: b        #0x790d44
00790ea8: mov      r0, r8
00790eac: ldr      sl, [r5, #0xa0]
00790eb0: bl       #0x797960
00790eb4: mov      r1, #0
00790eb8: strb     r0, [sl, #0x4e]
00790ebc: mov      r0, r5
00790ec0: bl       #0x78efb8
00790ec4: b        #0x790d44
00790ec8: mov      r0, r8
00790ecc: ldr      sl, [r5, #0xa0]
00790ed0: bl       #0x797960
00790ed4: mov      r1, #0
00790ed8: strb     r0, [sl, #0x49]
00790edc: mov      r0, r5
00790ee0: bl       #0x78efb8
00790ee4: b        #0x790d44
00790ee8: mov      r0, r8
00790eec: ldr      sl, [r5, #0xa0]
00790ef0: bl       #0x797960
00790ef4: mov      r1, #0
00790ef8: strb     r0, [sl, #0x48]
00790efc: mov      r0, r5
00790f00: bl       #0x78efb8
00790f04: b        #0x790d44
00790f08: mov      r0, r8
00790f0c: bl       #0x420a84
00790f10: ldrsb    r3, [r0]
00790f14: ldr      r1, [pc, #0x38]
00790f18: cmn      r3, #1
00790f1c: addne    r0, r0, #1
00790f20: ldreq    r0, [r0, #0xc]
00790f24: add      r1, pc, r1
00790f28: bl       #0x751d10
00790f2c: cmp      r0, #0
00790f30: bne      #0x790d44
00790f34: ldr      r3, [r5, #0xa0]
00790f38: mov      r2, #1
00790f3c: strb     r2, [r3, #0x4b]
00790f40: b        #0x790d44
00790f44: bl       #0x30e310

# _ZN7gameswf19edit_text_character8on_eventERKNS_8event_idE
00790f58: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00790f5c: ldr      r4, [pc, #0x560]
00790f60: ldr      r6, [pc, #0x560]
00790f64: ldr      r3, [r0, #0xa0]
00790f68: add      r4, pc, r4
00790f6c: ldr      r2, [r4, r6]
00790f70: sub      sp, sp, #0x18c
00790f74: mov      r5, r0
00790f78: ldr      r2, [r2]
00790f7c: mov      r8, r1
00790f80: str      r2, [sp, #0x184]
00790f84: ldrb     r7, [r3, #0x4b]
00790f88: cmp      r7, #0
00790f8c: bne      #0x790fac
00790f90: ldrb     r3, [r1]
00790f94: cmp      r3, #0x14
00790f98: beq      #0x790fcc
00790f9c: cmp      r3, #0x15
00790fa0: beq      #0x791168
00790fa4: cmp      r3, #8
00790fa8: beq      #0x791080
00790fac: mov      r0, #0
00790fb0: ldr      r3, [r4, r6]
00790fb4: ldr      r2, [sp, #0x184]
00790fb8: ldr      r3, [r3]
00790fbc: cmp      r2, r3
00790fc0: bne      #0x7914c0
00790fc4: add      sp, sp, #0x18c
00790fc8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00790fcc: bl       #0x78dde8
00790fd0: mov      r1, r5
00790fd4: bl       #0x774918
00790fd8: ldrb     r7, [r5, #0x14c]
00790fdc: cmp      r7, #0
00790fe0: bne      #0x791298
00790fe4: ldr      r1, [pc, #0x4e0]
00790fe8: ldr      r3, [r5]
00790fec: add      r8, sp, #0x170
00790ff0: strb     r7, [sp, #0x13c]
00790ff4: strb     r7, [sp, #0x13d]
00790ff8: add      r1, pc, r1
00790ffc: mov      r0, r8
00791000: add      sb, sp, #0x13c
00791004: ldr      sl, [r3, #0x20]
00791008: bl       #0x413a7c
0079100c: mov      r1, r8
00791010: mov      r2, sb
00791014: mov      r0, r5
00791018: blx      sl
0079101c: mov      sl, r0
00791020: mov      r0, r8
00791024: bl       #0x41fed8
00791028: cmp      sl, #0
0079102c: bne      #0x7913f8
00791030: mov      r0, r5
00791034: bl       #0x78dde8
00791038: mov      r1, r5
0079103c: add      r0, r0, #0xa8
00791040: bl       #0x760f4c
00791044: ldrb     r3, [r5, #0x138]
00791048: mov      r2, #1
0079104c: strb     r2, [r5, #0x14c]
00791050: sxtb     r3, r3
00791054: cmn      r3, #1
00791058: ldreq    r3, [r5, #0x13c]
0079105c: mov      r0, r5
00791060: mov      r1, #0
00791064: sub      r3, r3, #1
00791068: str      r3, [r5, #0x150]
0079106c: bl       #0x78efb8
00791070: mov      r0, sb
00791074: bl       #0x797124
00791078: mov      r0, #1
0079107c: b        #0x790fb0
00791080: add      r7, sp, #0x148
00791084: add      r1, r5, #0x138
00791088: mov      r0, r7
0079108c: bl       #0x75302c
00791090: ldrb     r0, [r5, #0x138]
00791094: ldr      r2, [r5, #0x150]
00791098: sxtb     r3, r0
0079109c: cmn      r3, #1
007910a0: ldreq    r1, [r5, #0x13c]
007910a4: subne    r1, r3, #1
007910a8: subeq    r1, r1, #1
007910ac: cmp      r1, r2
007910b0: movge    r1, r2
007910b4: str      r1, [r5, #0x150]
007910b8: ldrb     r2, [r8, #1]
007910bc: sub      r3, r2, #8
007910c0: cmp      r3, #0x26
007910c4: addls    pc, pc, r3, lsl #2
007910c8: b        #0x791388
007910cc: b        #0x7913bc
007910d0: b        #0x791388
007910d4: b        #0x791388
007910d8: b        #0x791388
007910dc: b        #0x791388
007910e0: b        #0x791388
007910e4: b        #0x791388
007910e8: b        #0x791388
007910ec: b        #0x791388
007910f0: b        #0x791388
007910f4: b        #0x791388
007910f8: b        #0x791388
007910fc: b        #0x791388
00791100: b        #0x791388
00791104: b        #0x791388
00791108: b        #0x791388
0079110c: b        #0x791388
00791110: b        #0x791388
00791114: b        #0x791388
00791118: b        #0x791388
0079111c: b        #0x791388
00791120: b        #0x791388
00791124: b        #0x791388
00791128: b        #0x791388
0079112c: b        #0x791388
00791130: b        #0x7912dc
00791134: b        #0x791330
00791138: b        #0x7912dc
0079113c: b        #0x791330
00791140: b        #0x791308
00791144: b        #0x791330
00791148: b        #0x79134c
0079114c: b        #0x7912dc
00791150: b        #0x791388
00791154: b        #0x791388
00791158: b        #0x791388
0079115c: b        #0x791388
00791160: b        #0x791388
00791164: b        #0x7912a0
00791168: ldrb     r3, [r0, #0x14c]
0079116c: cmp      r3, #0
00791170: beq      #0x791298
00791174: ldr      r1, [pc, #0x354]
00791178: ldr      r3, [r0]
0079117c: add      r8, sp, #0x15c
00791180: strb     r7, [sp, #0x10c]
00791184: strb     r7, [sp, #0x10d]
00791188: add      r1, pc, r1
0079118c: mov      r0, r8
00791190: add      sb, sp, #0x10c
00791194: ldr      sl, [r3, #0x20]
00791198: bl       #0x413a7c
0079119c: mov      r1, r8
007911a0: mov      r2, sb
007911a4: mov      r0, r5
007911a8: blx      sl
007911ac: mov      sl, r0
007911b0: mov      r0, r8
007911b4: bl       #0x41fed8
007911b8: cmp      sl, #0
007911bc: beq      #0x791260
007911c0: mov      r0, r5
007911c4: bl       #0x780374
007911c8: add      r8, sp, #0x10
007911cc: mov      r1, r0
007911d0: add      sl, sp, #0x100
007911d4: mov      r0, r8
007911d8: bl       #0x75eb4c
007911dc: mov      r1, sl
007911e0: mov      r0, r8
007911e4: strb     r7, [sp, #0x100]
007911e8: strb     r7, [sp, #0x101]
007911ec: bl       #0x769098
007911f0: mov      r0, sl
007911f4: bl       #0x797124
007911f8: mov      r3, #5
007911fc: mov      r0, r5
00791200: strb     r7, [sp, #0xf4]
00791204: strb     r3, [sp, #0xf5]
00791208: str      r5, [sp, #0xf8]
0079120c: bl       #0x759c64
00791210: ldr      fp, [sp, #0x14]
00791214: ldr      ip, [pc, #0x2b8]
00791218: add      r7, sp, #0xe8
0079121c: add      sl, sp, #0xf4
00791220: add      ip, pc, ip
00791224: mov      lr, #1
00791228: mov      r1, sb
0079122c: mov      r2, r8
00791230: mov      r3, sl
00791234: mov      r0, r7
00791238: sub      fp, fp, #1
0079123c: str      lr, [sp]
00791240: stmib    sp, {fp, ip}
00791244: bl       #0x7ba904
00791248: mov      r0, r7
0079124c: bl       #0x797124
00791250: mov      r0, sl
00791254: bl       #0x797124
00791258: mov      r0, r8
0079125c: bl       #0x75e03c
00791260: mov      r7, #0
00791264: strb     r7, [r5, #0x14c]
00791268: mov      r0, r5
0079126c: bl       #0x78dde8
00791270: mov      r1, r5
00791274: add      r0, r0, #0xa8
00791278: bl       #0x760e44
0079127c: mov      r0, r5
00791280: mov      r1, r7
00791284: bl       #0x78efb8
00791288: mov      r0, sb
0079128c: bl       #0x797124
00791290: mov      r0, #1
00791294: b        #0x790fb0
00791298: mov      r0, #1
0079129c: b        #0x790fb0
007912a0: ldrb     r0, [sp, #0x148]
007912a4: sxtb     r3, r0
007912a8: cmn      r3, #1
007912ac: ldreq    r2, [sp, #0x14c]
007912b0: movne    r2, r3
007912b4: sub      r2, r2, #1
007912b8: cmp      r1, r2
007912bc: blt      #0x79149c
007912c0: cmn      r3, #1
007912c4: bne      #0x790fac
007912c8: ldr      r0, [sp, #0x154]
007912cc: ldr      r1, [sp, #0x150]
007912d0: bl       #0x752b38
007912d4: mov      r0, #0
007912d8: b        #0x790fb0
007912dc: sxtb     r0, r0
007912e0: cmn      r0, #1
007912e4: ldreq    r0, [r5, #0x13c]
007912e8: sub      r0, r0, #1
007912ec: str      r0, [r5, #0x150]
007912f0: mov      r0, r5
007912f4: mov      r1, #0
007912f8: bl       #0x78efb8
007912fc: ldrb     r2, [sp, #0x148]
00791300: sxtb     r3, r2
00791304: b        #0x7912c0
00791308: cmp      r1, #0
0079130c: movle    r1, #0
00791310: subgt    r1, r1, #1
00791314: str      r1, [r5, #0x150]
00791318: mov      r0, r5
0079131c: mov      r1, #0
00791320: bl       #0x78efb8
00791324: ldrb     r0, [sp, #0x148]
00791328: sxtb     r3, r0
0079132c: b        #0x7912c0
00791330: mov      r1, #0
00791334: mov      r0, r5
00791338: str      r1, [r5, #0x150]
0079133c: bl       #0x78efb8
00791340: ldrb     r0, [sp, #0x148]
00791344: sxtb     r3, r0
00791348: b        #0x7912c0
0079134c: sxtb     r3, r0
00791350: cmn      r3, #1
00791354: ldreq    r2, [r5, #0x13c]
00791358: movne    r2, r3
0079135c: sub      r2, r2, #1
00791360: cmp      r1, r2
00791364: addlt    r1, r1, #1
00791368: blt      #0x791380
0079136c: cmn      r3, #1
00791370: ldrbne   r1, [r5, #0x138]
00791374: ldreq    r1, [r5, #0x13c]
00791378: sxtbne   r1, r1
0079137c: sub      r1, r1, #1
00791380: str      r1, [r5, #0x150]
00791384: b        #0x7912f0
00791388: sxtb     r2, r2
0079138c: mov      r0, r7
00791390: bl       #0x78b0c0
00791394: ldr      r3, [r5, #0x150]
00791398: mov      r0, r5
0079139c: mov      r1, r7
007913a0: add      r3, r3, #1
007913a4: str      r3, [r5, #0x150]
007913a8: mov      r2, #0
007913ac: bl       #0x790ab0
007913b0: ldrb     r0, [sp, #0x148]
007913b4: sxtb     r3, r0
007913b8: b        #0x7912c0
007913bc: cmp      r1, #0
007913c0: ble      #0x7913b0
007913c4: sub      r1, r1, #1
007913c8: mov      r0, r7
007913cc: bl       #0x78b94c
007913d0: ldr      r3, [r5, #0x150]
007913d4: mov      r2, #0
007913d8: mov      r0, r5
007913dc: sub      r3, r3, #1
007913e0: str      r3, [r5, #0x150]
007913e4: mov      r1, r7
007913e8: bl       #0x790ab0
007913ec: ldrb     r2, [sp, #0x148]
007913f0: sxtb     r3, r2
007913f4: b        #0x7912c0
007913f8: mov      r0, r5
007913fc: bl       #0x780374
00791400: add      r8, sp, #0x7c
00791404: add      sl, sp, #0x130
00791408: mov      r1, r0
0079140c: mov      r0, r8
00791410: bl       #0x75eb4c
00791414: mov      r1, sl
00791418: mov      r0, r8
0079141c: strb     r7, [sp, #0x130]
00791420: strb     r7, [sp, #0x131]
00791424: bl       #0x769098
00791428: mov      r0, sl
0079142c: bl       #0x797124
00791430: mov      r0, r5
00791434: mov      r3, #5
00791438: strb     r7, [sp, #0x124]
0079143c: strb     r3, [sp, #0x125]
00791440: str      r5, [sp, #0x128]
00791444: bl       #0x759c64
00791448: ldr      fp, [sp, #0x80]
0079144c: ldr      ip, [pc, #0x84]
00791450: add      r7, sp, #0x118
00791454: add      sl, sp, #0x124
00791458: add      ip, pc, ip
0079145c: mov      lr, #1
00791460: mov      r1, sb
00791464: mov      r2, r8
00791468: mov      r3, sl
0079146c: mov      r0, r7
00791470: sub      fp, fp, #1
00791474: str      lr, [sp]
00791478: stmib    sp, {fp, ip}
0079147c: bl       #0x7ba904
00791480: mov      r0, r7
00791484: bl       #0x797124
00791488: mov      r0, sl
0079148c: bl       #0x797124
00791490: mov      r0, r8
00791494: bl       #0x75e03c
00791498: b        #0x791030
0079149c: mov      r0, r7
007914a0: bl       #0x78b94c
007914a4: mov      r2, #0
007914a8: mov      r0, r5
007914ac: mov      r1, r7
007914b0: bl       #0x790ab0
007914b4: ldrb     r2, [sp, #0x148]
007914b8: sxtb     r3, r2
007914bc: b        #0x7912c0
007914c0: bl       #0x30e310
007914c4: eoreq    r3, r0, r8, lsr #22
007914c8: andeq    r4, r0, ip, lsr #1
007914cc: ldrsheq  r8, [r7], -r0
007914d0: andseq   r8, r7, r0, ror lr
007914d4: andseq   ip, r3, r8, lsl r4
007914d8: andseq   ip, r3, r0, ror #3

# _ZN7gameswf19edit_text_character4initEv
007914dc: push     {r4, r5, r6, r7, r8, lr}
007914e0: ldr      r5, [pc, #0x1b4]
007914e4: ldr      r6, [pc, #0x1b4]
007914e8: ldr      r3, [r0, #0xa0]
007914ec: add      r5, pc, r5
007914f0: ldr      r2, [r5, r6]
007914f4: mvn      r7, #0
007914f8: sub      sp, sp, #0x80
007914fc: ldr      r1, [r2]
00791500: mov      r8, #0
00791504: mov      r2, #0
00791508: str      r1, [sp, #0x7c]
0079150c: str      r7, [r0, #0x16c]
00791510: str      r2, [r0, #0x168]
00791514: strb     r2, [r0, #0x14c]
00791518: str      r2, [r0, #0x150]
0079151c: str      r8, [r0, #0x154]
00791520: str      r8, [r0, #0x158]
00791524: str      r8, [r0, #0x15c]
00791528: str      r8, [r0, #0x160]
0079152c: str      r2, [r0, #0x164]
00791530: ldr      r2, [r3, #0x60]
00791534: mov      r4, r0
00791538: add      r0, r0, #0x178
0079153c: str      r2, [r4, #0x170]
00791540: ldr      r2, [r3, #0x5c]
00791544: str      r2, [r4, #0x174]
00791548: ldr      r1, [r3, #0x58]
0079154c: bl       #0x764234
00791550: ldr      r1, [r4, #0xa0]
00791554: ldr      r3, [r1, #0x68]
00791558: str      r3, [r4, #0x17c]
0079155c: ldr      r3, [r1, #0x6c]
00791560: str      r3, [r4, #0x180]
00791564: ldr      r3, [r1, #0x70]
00791568: str      r3, [r4, #0x184]
0079156c: ldr      r3, [r1, #0x74]
00791570: str      r3, [r4, #0x188]
00791574: ldr      r3, [r1, #0x78]
00791578: strb     r7, [r4, #0x197]
0079157c: strb     r7, [r4, #0x194]
00791580: strb     r7, [r4, #0x195]
00791584: strb     r7, [r4, #0x196]
00791588: str      r8, [r4, #0x190]
0079158c: str      r3, [r4, #0x18c]
00791590: ldrsb    r3, [r1, #0x7c]
00791594: cmp      r3, r7
00791598: add      r7, sp, #0x68
0079159c: addne    r1, r1, #0x7d
007915a0: ldreq    r1, [r1, #0x88]
007915a4: mov      r0, r7
007915a8: bl       #0x413a7c
007915ac: mov      r0, r4
007915b0: mov      r1, r7
007915b4: mov      r2, #0
007915b8: bl       #0x78f1ec
007915bc: ldrsb    r3, [sp, #0x68]
007915c0: cmn      r3, #1
007915c4: beq      #0x791678
007915c8: ldr      r3, [r4]
007915cc: mov      r0, r4
007915d0: mov      lr, pc
007915d4: ldr      pc, [r3, #0xc]
007915d8: add      r7, sp, #0x54
007915dc: mov      r1, r0
007915e0: mov      r0, r7
007915e4: bl       #0x413a7c
007915e8: mov      r0, r4
007915ec: mov      r1, r7
007915f0: mov      r2, #0
007915f4: bl       #0x790ab0
007915f8: ldrsb    r3, [sp, #0x54]
007915fc: cmn      r3, #1
00791600: beq      #0x791688
00791604: add      r8, r4, #0xb4
00791608: mov      r1, #0
0079160c: mov      r0, r8
00791610: bl       #0x76146c
00791614: mov      r0, sp
00791618: bl       #0x784a8c
0079161c: mov      r1, sp
00791620: mov      r0, r8
00791624: bl       #0x761970
00791628: mov      r0, sp
0079162c: bl       #0x784cac
00791630: mov      r0, r4
00791634: bl       #0x78a364
00791638: ldrsb    r3, [r0]
0079163c: ldr      r1, [r5, r6]
00791640: mov      r7, sp
00791644: cmn      r3, #1
00791648: ldreq    r3, [r0, #4]
0079164c: sub      r3, r3, #1
00791650: cmp      r3, #0
00791654: movle    r3, #0
00791658: movgt    r3, #1
0079165c: strb     r3, [r4, #0x9d]
00791660: ldr      r2, [sp, #0x7c]
00791664: ldr      r3, [r1]
00791668: cmp      r2, r3
0079166c: bne      #0x791698
00791670: add      sp, sp, #0x80
00791674: pop      {r4, r5, r6, r7, r8, pc}
00791678: ldr      r0, [sp, #0x74]
0079167c: ldr      r1, [sp, #0x70]
00791680: bl       #0x752b38
00791684: b        #0x7915c8
00791688: ldr      r0, [sp, #0x60]
0079168c: ldr      r1, [sp, #0x5c]
00791690: bl       #0x752b38
00791694: b        #0x791604
00791698: bl       #0x30e310
0079169c: eoreq    r3, r0, r4, lsr #11
007916a0: andeq    r4, r0, ip, lsr #1

# _ZN7gameswf19edit_text_character7recycleEPNS_9characterEi
007916a4: push     {r4, lr}
007916a8: mov      r4, r0
007916ac: bl       #0x754958
007916b0: mov      r0, r4
007916b4: pop      {r4, lr}
007916b8: b        #0x7914dc

# _ZN7gameswf19edit_text_characterC1EPNS_6playerEPNS_9characterEPNS_23edit_text_character_defEi
007916bc: push     {r4, r5, r6, r7, lr}
007916c0: sub      sp, sp, #0xc
007916c4: mov      r6, r3
007916c8: mov      ip, #0x20
007916cc: ldr      r3, [sp, #0x20]
007916d0: ldr      r5, [pc, #0x13c]
007916d4: mov      r4, r0
007916d8: str      ip, [sp]
007916dc: bl       #0x754b28
007916e0: ldr      r3, [pc, #0x130]
007916e4: add      r5, pc, r5
007916e8: cmp      r6, #0
007916ec: ldr      r3, [r5, r3]
007916f0: str      r6, [r4, #0xa0]
007916f4: add      r3, r3, #8
007916f8: str      r3, [r4]
007916fc: beq      #0x791708
00791700: mov      r0, r6
00791704: bl       #0x759c64
00791708: ldr      r0, [r4, #0x148]
0079170c: mvn      r2, #0
00791710: mov      r3, #0
00791714: bfi      r0, r2, #0, #0x18
00791718: lsr      ip, r0, #0x18
0079171c: mov      r1, #0
00791720: bfi      ip, r3, #0, #1
00791724: mvn      r6, #0
00791728: mvn      r7, #0
0079172c: mov      lr, #1
00791730: strb     lr, [r4, #0x138]
00791734: str      r3, [r4, #0xa4]
00791738: str      r3, [r4, #0xa8]
0079173c: str      r3, [r4, #0xac]
00791740: strb     r3, [r4, #0xb0]
00791744: str      r3, [r4, #0xb4]
00791748: str      r3, [r4, #0xb8]
0079174c: str      r3, [r4, #0xbc]
00791750: strb     r3, [r4, #0xc0]
00791754: str      r3, [r4, #0xc4]
00791758: str      r3, [r4, #0xc8]
0079175c: str      r3, [r4, #0xcc]
00791760: strb     r3, [r4, #0xd0]
00791764: str      r3, [r4, #0xe8]
00791768: str      r3, [r4, #0xec]
0079176c: str      r3, [r4, #0xf0]
00791770: strb     r3, [r4, #0xf4]
00791774: str      r3, [r4, #0xf8]
00791778: str      r3, [r4, #0xfc]
0079177c: str      r3, [r4, #0x100]
00791780: strb     r3, [r4, #0x104]
00791784: str      r3, [r4, #0x108]
00791788: str      r3, [r4, #0x10c]
0079178c: str      r3, [r4, #0x110]
00791790: strb     r3, [r4, #0x114]
00791794: str      r3, [r4, #0x118]
00791798: str      r3, [r4, #0x11c]
0079179c: str      r3, [r4, #0x120]
007917a0: strb     r3, [r4, #0x124]
007917a4: strb     r3, [r4, #0x139]
007917a8: strd     r6, r7, [r4, #0xe0]
007917ac: strd     r6, r7, [r4, #0xd8]
007917b0: str      r0, [r4, #0x148]
007917b4: mov      r0, r4
007917b8: strb     ip, [r4, #0x14b]
007917bc: str      r1, [r4, #0x160]
007917c0: str      r3, [r4, #0x178]
007917c4: strb     r2, [r4, #0x197]
007917c8: strb     r3, [r4, #0x14c]
007917cc: str      r3, [r4, #0x150]
007917d0: str      r1, [r4, #0x154]
007917d4: str      r1, [r4, #0x158]
007917d8: str      r1, [r4, #0x15c]
007917dc: str      r3, [r4, #0x164]
007917e0: str      r3, [r4, #0x168]
007917e4: str      r2, [r4, #0x16c]
007917e8: strb     r2, [r4, #0x170]
007917ec: strb     r2, [r4, #0x171]
007917f0: strb     r2, [r4, #0x172]
007917f4: strb     r2, [r4, #0x173]
007917f8: strb     r2, [r4, #0x194]
007917fc: strb     r2, [r4, #0x195]
00791800: strb     r2, [r4, #0x196]
00791804: bl       #0x7914dc
00791808: mov      r0, r4
0079180c: add      sp, sp, #0xc
00791810: pop      {r4, r5, r6, r7, pc}
00791814: eoreq    r3, r0, ip, lsr #7
00791818: andeq    r3, r0, r0, ror #30

# _ZN7gameswf19edit_text_characterC2EPNS_6playerEPNS_9characterEPNS_23edit_text_character_defEi
00791964: push     {r4, r5, r6, r7, lr}
00791968: sub      sp, sp, #0xc
0079196c: mov      r6, r3
00791970: mov      ip, #0x20
00791974: ldr      r3, [sp, #0x20]
00791978: ldr      r5, [pc, #0x13c]
0079197c: mov      r4, r0
00791980: str      ip, [sp]
00791984: bl       #0x754b28
00791988: ldr      r3, [pc, #0x130]
0079198c: add      r5, pc, r5
00791990: cmp      r6, #0
00791994: ldr      r3, [r5, r3]
00791998: str      r6, [r4, #0xa0]
0079199c: add      r3, r3, #8
007919a0: str      r3, [r4]
007919a4: beq      #0x7919b0
007919a8: mov      r0, r6
007919ac: bl       #0x759c64
007919b0: ldr      r0, [r4, #0x148]
007919b4: mvn      r2, #0
007919b8: mov      r3, #0
007919bc: bfi      r0, r2, #0, #0x18
007919c0: lsr      ip, r0, #0x18
007919c4: mov      r1, #0
007919c8: bfi      ip, r3, #0, #1
007919cc: mvn      r6, #0
007919d0: mvn      r7, #0
007919d4: mov      lr, #1
007919d8: strb     lr, [r4, #0x138]
007919dc: str      r3, [r4, #0xa4]
007919e0: str      r3, [r4, #0xa8]
007919e4: str      r3, [r4, #0xac]
007919e8: strb     r3, [r4, #0xb0]
007919ec: str      r3, [r4, #0xb4]
007919f0: str      r3, [r4, #0xb8]
007919f4: str      r3, [r4, #0xbc]
007919f8: strb     r3, [r4, #0xc0]
007919fc: str      r3, [r4, #0xc4]
00791a00: str      r3, [r4, #0xc8]
00791a04: str      r3, [r4, #0xcc]
00791a08: strb     r3, [r4, #0xd0]
00791a0c: str      r3, [r4, #0xe8]
00791a10: str      r3, [r4, #0xec]
00791a14: str      r3, [r4, #0xf0]
00791a18: strb     r3, [r4, #0xf4]
00791a1c: str      r3, [r4, #0xf8]
00791a20: str      r3, [r4, #0xfc]
00791a24: str      r3, [r4, #0x100]
00791a28: strb     r3, [r4, #0x104]
00791a2c: str      r3, [r4, #0x108]
00791a30: str      r3, [r4, #0x10c]
00791a34: str      r3, [r4, #0x110]
00791a38: strb     r3, [r4, #0x114]
00791a3c: str      r3, [r4, #0x118]
00791a40: str      r3, [r4, #0x11c]
00791a44: str      r3, [r4, #0x120]
00791a48: strb     r3, [r4, #0x124]
00791a4c: strb     r3, [r4, #0x139]
00791a50: strd     r6, r7, [r4, #0xe0]
00791a54: strd     r6, r7, [r4, #0xd8]
00791a58: str      r0, [r4, #0x148]
00791a5c: mov      r0, r4
00791a60: strb     ip, [r4, #0x14b]
00791a64: str      r1, [r4, #0x160]
00791a68: str      r3, [r4, #0x178]
00791a6c: strb     r2, [r4, #0x197]
00791a70: strb     r3, [r4, #0x14c]
00791a74: str      r3, [r4, #0x150]
00791a78: str      r1, [r4, #0x154]
00791a7c: str      r1, [r4, #0x158]
00791a80: str      r1, [r4, #0x15c]
00791a84: str      r3, [r4, #0x164]
00791a88: str      r3, [r4, #0x168]
00791a8c: str      r2, [r4, #0x16c]
00791a90: strb     r2, [r4, #0x170]
00791a94: strb     r2, [r4, #0x171]
00791a98: strb     r2, [r4, #0x172]
00791a9c: strb     r2, [r4, #0x173]
00791aa0: strb     r2, [r4, #0x194]
00791aa4: strb     r2, [r4, #0x195]
00791aa8: strb     r2, [r4, #0x196]
00791aac: bl       #0x7914dc
00791ab0: mov      r0, r4
00791ab4: add      sp, sp, #0xc
00791ab8: pop      {r4, r5, r6, r7, pc}
00791abc: eoreq    r3, r0, r4, lsl #2
00791ac0: andeq    r3, r0, r0, ror #30

# _ZN7gameswf19edit_text_character9to_stringEv
00791ac4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00791ac8: ldr      r4, [pc, #0x270]
00791acc: ldr      r6, [pc, #0x270]
00791ad0: sub      sp, sp, #0x64
00791ad4: add      r4, pc, r4
00791ad8: ldr      r3, [r4, r6]
00791adc: mov      r5, r0
00791ae0: ldr      r3, [r3]
00791ae4: str      r3, [sp, #0x5c]
00791ae8: bl       #0x78a364
00791aec: ldrsb    r3, [r0]
00791af0: cmn      r3, #1
00791af4: ldreq    r3, [r0, #4]
00791af8: sub      r3, r3, #1
00791afc: cmp      r3, #0
00791b00: ble      #0x791c34
00791b04: ldr      r7, [r5, #0x40]
00791b08: cmp      r7, #0
00791b0c: beq      #0x791b20
00791b10: ldr      r0, [r5, #0x3c]
00791b14: ldrb     r3, [r0, #4]
00791b18: cmp      r3, #0
00791b1c: beq      #0x791c64
00791b20: ldr      r3, [sp, #0x58]
00791b24: mvn      r1, #0
00791b28: mov      r2, #0
00791b2c: bfi      r3, r1, #0, #0x18
00791b30: lsr      r1, r3, #0x18
00791b34: bfi      r1, r2, #0, #1
00791b38: mov      ip, #1
00791b3c: mov      r0, r5
00791b40: str      r3, [sp, #0x58]
00791b44: strb     ip, [sp, #0x48]
00791b48: strb     r2, [sp, #0x49]
00791b4c: strb     r1, [sp, #0x5b]
00791b50: bl       #0x78a364
00791b54: add      r8, sp, #0x34
00791b58: mov      r1, r0
00791b5c: mov      r0, r8
00791b60: bl       #0x75302c
00791b64: add      sl, sp, #0x48
00791b68: mov      r0, r5
00791b6c: bl       #0x78a364
00791b70: mov      r1, sl
00791b74: mov      r2, r8
00791b78: bl       #0x7cd130
00791b7c: cmp      r0, #0
00791b80: beq      #0x791ba0
00791b84: ldrsb    r3, [sp, #0x48]
00791b88: mov      r0, r7
00791b8c: cmn      r3, #1
00791b90: addne    r1, sl, #1
00791b94: ldreq    r1, [sp, #0x54]
00791b98: bl       #0x76b284
00791b9c: mov      r7, r0
00791ba0: cmp      r7, #0
00791ba4: beq      #0x791c1c
00791ba8: mov      sb, #0
00791bac: strb     sb, [sp]
00791bb0: strb     sb, [sp, #1]
00791bb4: ldr      r3, [r7]
00791bb8: add      fp, sp, #0x20
00791bbc: mov      r1, r8
00791bc0: mov      r0, fp
00791bc4: ldr      sl, [r3, #0x20]
00791bc8: bl       #0x75302c
00791bcc: mov      r0, r7
00791bd0: mov      r1, fp
00791bd4: mov      r2, sp
00791bd8: blx      sl
00791bdc: cmp      r0, #0
00791be0: mov      r8, sp
00791be4: moveq    sb, r0
00791be8: beq      #0x791c00
00791bec: ldrsb    r3, [sp, #1]
00791bf0: cmp      r3, #5
00791bf4: ldreq    sb, [sp, #4]
00791bf8: subs     sb, r5, sb
00791bfc: movne    sb, #1
00791c00: ldrsb    r3, [sp, #0x20]
00791c04: cmn      r3, #1
00791c08: beq      #0x791d2c
00791c0c: cmp      sb, #0
00791c10: bne      #0x791cb4
00791c14: mov      r0, sp
00791c18: bl       #0x797124
00791c1c: ldrsb    r3, [sp, #0x34]
00791c20: cmn      r3, #1
00791c24: beq      #0x791c8c
00791c28: ldrsb    r3, [sp, #0x48]
00791c2c: cmn      r3, #1
00791c30: beq      #0x791ca4
00791c34: ldrb     r3, [r5, #0x138]
00791c38: ldr      r2, [sp, #0x5c]
00791c3c: cmp      r3, #0xff
00791c40: ldr      r3, [r4, r6]
00791c44: addne    r0, r5, #0x138
00791c48: addne    r0, r0, #1
00791c4c: ldr      r3, [r3]
00791c50: ldreq    r0, [r5, #0x144]
00791c54: cmp      r2, r3
00791c58: bne      #0x791d3c
00791c5c: add      sp, sp, #0x64
00791c60: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00791c64: ldr      r1, [r0]
00791c68: sub      r1, r1, #1
00791c6c: cmp      r1, #0
00791c70: str      r1, [r0]
00791c74: bne      #0x791c7c
00791c78: bl       #0x752b38
00791c7c: mov      r7, #0
00791c80: str      r7, [r5, #0x3c]
00791c84: str      r7, [r5, #0x40]
00791c88: b        #0x791b20
00791c8c: ldr      r0, [sp, #0x40]
00791c90: ldr      r1, [sp, #0x3c]
00791c94: bl       #0x752b38
00791c98: ldrsb    r3, [sp, #0x48]
00791c9c: cmn      r3, #1
00791ca0: bne      #0x791c34
00791ca4: ldr      r0, [sp, #0x54]
00791ca8: ldr      r1, [sp, #0x50]
00791cac: bl       #0x752b38
00791cb0: b        #0x791c34
00791cb4: mov      r0, sp
00791cb8: bl       #0x420a84
00791cbc: ldrsb    r3, [r0]
00791cc0: cmn      r3, #1
00791cc4: ldrb     r3, [r5, #0x138]
00791cc8: addne    r0, r0, #1
00791ccc: ldreq    r0, [r0, #0xc]
00791cd0: cmp      r3, #0xff
00791cd4: addne    r1, r5, #0x138
00791cd8: addne    r1, r1, #1
00791cdc: ldreq    r1, [r5, #0x144]
00791ce0: bl       #0x30e31c
00791ce4: cmp      r0, #0
00791ce8: beq      #0x791c14
00791cec: mov      r0, sp
00791cf0: bl       #0x420a84
00791cf4: ldrsb    r3, [r0]
00791cf8: add      r7, sp, #0xc
00791cfc: cmn      r3, #1
00791d00: addne    r1, r0, #1
00791d04: ldreq    r1, [r0, #0xc]
00791d08: mov      r0, r7
00791d0c: bl       #0x413a7c
00791d10: mov      r0, r5
00791d14: mov      r1, r7
00791d18: mov      r2, #0
00791d1c: bl       #0x78f1ec
00791d20: mov      r0, r7
00791d24: bl       #0x41fed8
00791d28: b        #0x791c14
00791d2c: ldr      r0, [sp, #0x2c]
00791d30: ldr      r1, [sp, #0x28]
00791d34: bl       #0x752b38
00791d38: b        #0x791c0c
00791d3c: bl       #0x30e310

# _ZN7gameswf19edit_text_character7displayEv
007928cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007928d0: mov      r4, r0
007928d4: ldr      r0, [r0, #0x30]
007928d8: ldr      r5, [pc, #0xc40]
007928dc: sub      sp, sp, #0xd4
007928e0: cmp      r0, #0
007928e4: add      r5, pc, r5
007928e8: beq      #0x7928fc
007928ec: ldr      r3, [r4, #0x2c]
007928f0: ldrb     r2, [r3, #4]
007928f4: cmp      r2, #0
007928f8: beq      #0x792b30
007928fc: bl       #0x76d5b4
00792900: ldrb     r3, [r0, #0x85]
00792904: cmp      r3, #0
00792908: beq      #0x792918
0079290c: ldrb     r3, [r0, #0x86]
00792910: cmp      r3, #0
00792914: beq      #0x792e10
00792918: ldr      r3, [r4, #0xa0]
0079291c: ldrb     r3, [r3, #0x4e]
00792920: cmp      r3, #0
00792924: bne      #0x792bac
00792928: ldr      r6, [pc, #0xbf4]
0079292c: ldr      r7, [r4, #0x30]
00792930: cmp      r7, #0
00792934: beq      #0x792948
00792938: ldr      r0, [r4, #0x2c]
0079293c: ldrb     r3, [r0, #4]
00792940: cmp      r3, #0
00792944: beq      #0x792aec
00792948: ldr      r3, [r7, #0xac]
0079294c: mov      r1, #0x3f800000
00792950: ldr      r3, [r3, #0xc]
00792954: ldr      r0, [r3, #4]
00792958: bl       #0x30df8c
0079295c: cmp      r0, #0
00792960: beq      #0x792acc
00792964: ldr      r3, [r5, r6]
00792968: ldr      r2, [r4, #0xa0]
0079296c: ldr      r3, [r3]
00792970: ldr      r2, [r2, #0x94]
00792974: cmp      r3, #0
00792978: beq      #0x79298c
0079297c: subs     r2, r2, #0
00792980: movne    r2, #1
00792984: strb     r2, [r3, #4]
00792988: ldr      r7, [r4, #0x30]
0079298c: cmp      r7, #0
00792990: beq      #0x7929a4
00792994: ldr      r0, [r4, #0x2c]
00792998: ldrb     r3, [r0, #4]
0079299c: cmp      r3, #0
007929a0: beq      #0x792dc0
007929a4: ldrb     r3, [r7, #0x98]
007929a8: cmp      r3, #0
007929ac: bne      #0x792b68
007929b0: cmp      r7, #0
007929b4: beq      #0x7929c8
007929b8: ldr      r3, [r4, #0x2c]
007929bc: ldrb     r8, [r3, #4]
007929c0: cmp      r8, #0
007929c4: beq      #0x792e24
007929c8: ldrb     r3, [r7, #0x98]
007929cc: cmp      r3, #0
007929d0: beq      #0x7929f8
007929d4: ldr      r3, [r5, r6]
007929d8: ldr      r3, [r3]
007929dc: cmp      r3, #0
007929e0: beq      #0x7929f8
007929e4: mov      r0, r3
007929e8: add      r1, r4, #0xd8
007929ec: ldr      r3, [r3]
007929f0: mov      lr, pc
007929f4: ldr      pc, [r3, #0x4c]
007929f8: ldr      r3, [pc, #0xb28]
007929fc: add      sl, sp, #0xb0
00792a00: mov      r2, #0
00792a04: ldr      r1, [r5, r3]
00792a08: add      r3, sl, #8
00792a0c: str      r2, [r3], #4
00792a10: ldr      r0, [r1]
00792a14: str      r2, [r3], #4
00792a18: str      r2, [r3], #4
00792a1c: mov      r1, #0x3f800000
00792a20: cmp      r0, r2
00792a24: str      r2, [r3]
00792a28: str      r2, [sp, #0xb4]
00792a2c: str      r1, [sp, #0xc0]
00792a30: str      r1, [sp, #0xb0]
00792a34: beq      #0x792e64
00792a38: ldr      r3, [r4, #0xa8]
00792a3c: cmp      r3, #0
00792a40: ble      #0x792a70
00792a44: ldr      r3, [r4, #0xa0]
00792a48: mov      ip, #0
00792a4c: mov      r0, ip
00792a50: ldr      r3, [r3, #0x20]
00792a54: mov      r1, r4
00792a58: add      r2, r4, #0xa4
00792a5c: str      ip, [sp]
00792a60: str      ip, [sp, #4]
00792a64: str      ip, [sp, #8]
00792a68: str      ip, [sp, #0xc]
00792a6c: bl       #0x78faa0
00792a70: ldr      r3, [r4, #0x30]
00792a74: cmp      r3, #0
00792a78: beq      #0x792a8c
00792a7c: ldr      r0, [r4, #0x2c]
00792a80: ldrb     r2, [r0, #4]
00792a84: cmp      r2, #0
00792a88: beq      #0x792e3c
00792a8c: ldrb     r3, [r3, #0x98]
00792a90: cmp      r3, #0
00792a94: bne      #0x792de8
00792a98: ldrb     r3, [r4, #0x14c]
00792a9c: cmp      r3, #0
00792aa0: bne      #0x792b5c
00792aa4: ldr      r3, [r4, #0x54]
00792aa8: cmp      r3, #0
00792aac: beq      #0x792ac4
00792ab0: ldr      r3, [r3, #0x60]
00792ab4: cmp      r3, #0
00792ab8: beq      #0x792ac4
00792abc: mov      r0, r4
00792ac0: bl       #0x753f9c
00792ac4: add      sp, sp, #0xd4
00792ac8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00792acc: ldr      r3, [r5, r6]
00792ad0: ldr      r3, [r3]
00792ad4: cmp      r3, #0
00792ad8: beq      #0x79298c
00792adc: mov      r2, #0
00792ae0: strb     r2, [r3, #4]
00792ae4: ldr      r7, [r4, #0x30]
00792ae8: b        #0x79298c
00792aec: ldr      r1, [r0]
00792af0: sub      r1, r1, #1
00792af4: cmp      r1, #0
00792af8: str      r1, [r0]
00792afc: bne      #0x792b04
00792b00: bl       #0x752b38
00792b04: mov      r7, #0
00792b08: str      r7, [r4, #0x2c]
00792b0c: str      r7, [r4, #0x30]
00792b10: ldr      r3, [r7, #0xac]
00792b14: mov      r1, #0x3f800000
00792b18: ldr      r3, [r3, #0xc]
00792b1c: ldr      r0, [r3, #4]
00792b20: bl       #0x30df8c
00792b24: cmp      r0, #0
00792b28: bne      #0x792964
00792b2c: b        #0x792acc
00792b30: ldr      r1, [r3]
00792b34: sub      r1, r1, #1
00792b38: cmp      r1, #0
00792b3c: str      r1, [r3]
00792b40: bne      #0x792b4c
00792b44: mov      r0, r3
00792b48: bl       #0x752b38
00792b4c: mov      r0, #0
00792b50: str      r0, [r4, #0x2c]
00792b54: str      r0, [r4, #0x30]
00792b58: b        #0x7928fc
00792b5c: mov      r0, r4
00792b60: bl       #0x78b128
00792b64: b        #0x792aa4
00792b68: add      r7, r4, #0xd8
00792b6c: mov      r0, r7
00792b70: mov      r1, r4
00792b74: bl       #0x7738a4
00792b78: cmp      r0, #0
00792b7c: ldreq    r7, [r4, #0x30]
00792b80: beq      #0x7929b0
00792b84: ldr      r3, [r5, r6]
00792b88: ldr      r3, [r3]
00792b8c: cmp      r3, #0
00792b90: beq      #0x792a98
00792b94: mov      r0, r3
00792b98: mov      r1, r7
00792b9c: ldr      r3, [r3]
00792ba0: mov      lr, pc
00792ba4: ldr      pc, [r3, #0x64]
00792ba8: b        #0x792a98
00792bac: mov      r0, r4
00792bb0: bl       #0x753f74
00792bb4: add      sl, sp, #0xb0
00792bb8: ldr      r6, [pc, #0x964]
00792bbc: mov      ip, r0
00792bc0: mov      lr, sl
00792bc4: ldm      ip!, {r0, r1, r2, r3}
00792bc8: stm      lr!, {r0, r1, r2, r3}
00792bcc: ldr      r2, [r5, r6]
00792bd0: ldm      ip, {r0, r1}
00792bd4: ldr      ip, [r2]
00792bd8: stm      lr, {r0, r1}
00792bdc: cmp      ip, #0
00792be0: beq      #0x792bf8
00792be4: mov      r0, ip
00792be8: mov      r1, sl
00792bec: ldr      r3, [ip]
00792bf0: mov      lr, pc
00792bf4: ldr      pc, [r3, #0x50]
00792bf8: ldr      r3, [r4, #0xa0]
00792bfc: mov      r7, #0
00792c00: add      r1, sp, #0x48
00792c04: str      r1, [sp, #0x28]
00792c08: str      r7, [sp, #0x90]
00792c0c: str      r7, [sp, #0x94]
00792c10: str      r7, [sp, #0x98]
00792c14: str      r7, [sp, #0x9c]
00792c18: str      r7, [sp, #0xa0]
00792c1c: str      r7, [sp, #0xa4]
00792c20: str      r7, [sp, #0xa8]
00792c24: str      r7, [sp, #0xac]
00792c28: ldr      sl, [r3, #0x24]
00792c2c: ldr      r8, [r3, #0x2c]
00792c30: mov      r0, r1
00792c34: str      sl, [sp, #0x90]
00792c38: str      r8, [sp, #0x94]
00792c3c: ldr      fp, [r3, #0x28]
00792c40: ldr      ip, [r3, #0x2c]
00792c44: mov      r1, #0
00792c48: str      fp, [sp, #0x98]
00792c4c: str      ip, [sp, #0x9c]
00792c50: ldr      r2, [r3, #0x24]
00792c54: str      r2, [sp, #0x20]
00792c58: ldr      lr, [r3, #0x30]
00792c5c: mov      r2, #0x48
00792c60: str      lr, [sp, #0x24]
00792c64: ldr      lr, [sp, #0x20]
00792c68: str      lr, [sp, #0xa0]
00792c6c: ldr      lr, [sp, #0x24]
00792c70: str      lr, [sp, #0xa4]
00792c74: ldr      lr, [r3, #0x30]
00792c78: str      lr, [sp, #0x1c]
00792c7c: ldr      r3, [r3, #0x28]
00792c80: str      lr, [sp, #0xac]
00792c84: str      ip, [sp, #0x14]
00792c88: str      r3, [sp, #0xa8]
00792c8c: str      r3, [sp, #0x18]
00792c90: bl       #0x30e460
00792c94: ldr      sb, [r5, r6]
00792c98: ldr      r3, [sp, #0x18]
00792c9c: ldr      r1, [sp, #0x1c]
00792ca0: ldr      r2, [sb]
00792ca4: ldr      ip, [sp, #0x14]
00792ca8: ldr      lr, [sp, #0x20]
00792cac: str      r3, [sp, #0x78]
00792cb0: str      r1, [sp, #0x7c]
00792cb4: str      r3, [sp, #0x60]
00792cb8: ldr      r1, [sp, #0x24]
00792cbc: ldr      r3, [sp, #0x1c]
00792cc0: cmp      r2, #0
00792cc4: str      fp, [sp, #0x70]
00792cc8: str      ip, [sp, #0x74]
00792ccc: str      lr, [sp, #0x80]
00792cd0: str      r1, [sp, #0x84]
00792cd4: str      sl, [sp, #0x88]
00792cd8: str      r8, [sp, #0x8c]
00792cdc: str      sl, [sp, #0x48]
00792ce0: str      r8, [sp, #0x4c]
00792ce4: str      fp, [sp, #0x50]
00792ce8: str      ip, [sp, #0x54]
00792cec: str      lr, [sp, #0x58]
00792cf0: str      r1, [sp, #0x5c]
00792cf4: str      r3, [sp, #0x64]
00792cf8: str      sl, [sp, #0x68]
00792cfc: str      r8, [sp, #0x6c]
00792d00: beq      #0x79292c
00792d04: ldr      r3, [r2]
00792d08: mov      r0, r2
00792d0c: mov      r1, #0
00792d10: add      r2, r4, #0x194
00792d14: mov      lr, pc
00792d18: ldr      pc, [r3, #0x6c]
00792d1c: ldr      r3, [sb]
00792d20: cmp      r3, #0
00792d24: beq      #0x79292c
00792d28: mov      r0, r3
00792d2c: ldr      r1, [sp, #0x28]
00792d30: ldr      r3, [r3]
00792d34: mov      r2, #4
00792d38: mov      lr, pc
00792d3c: ldr      pc, [r3, #0x58]
00792d40: ldr      r0, [sb]
00792d44: cmp      r0, #0
00792d48: beq      #0x79292c
00792d4c: ldr      r3, [r0]
00792d50: mov      r2, #0
00792d54: mvn      r1, #0
00792d58: ldr      r3, [r3, #0x78]
00792d5c: strb     r1, [sp, #0xcb]
00792d60: strb     r2, [sp, #0xc8]
00792d64: strb     r2, [sp, #0xca]
00792d68: strb     r2, [sp, #0xc9]
00792d6c: ldr      r1, [sp, #0xc8]
00792d70: blx      r3
00792d74: ldr      r3, [sb]
00792d78: cmp      r3, #0
00792d7c: beq      #0x79292c
00792d80: mov      r0, r3
00792d84: mov      r1, r7
00792d88: ldr      r3, [r3]
00792d8c: mov      lr, pc
00792d90: ldr      pc, [r3, #0x7c]
00792d94: ldr      r3, [sb]
00792d98: cmp      r3, #0
00792d9c: beq      #0x79292c
00792da0: ldr      ip, [sp, #0x28]
00792da4: mov      r0, r3
00792da8: mov      r2, #5
00792dac: add      r1, ip, #0x20
00792db0: ldr      r3, [r3]
00792db4: mov      lr, pc
00792db8: ldr      pc, [r3, #0x60]
00792dbc: b        #0x79292c
00792dc0: ldr      r1, [r0]
00792dc4: sub      r1, r1, #1
00792dc8: cmp      r1, #0
00792dcc: str      r1, [r0]
00792dd0: bne      #0x792dd8
00792dd4: bl       #0x752b38
00792dd8: mov      r7, #0
00792ddc: str      r7, [r4, #0x2c]
00792de0: str      r7, [r4, #0x30]
00792de4: b        #0x7929a4
00792de8: ldr      r3, [r5, r6]
00792dec: ldr      r3, [r3]
00792df0: cmp      r3, #0
00792df4: beq      #0x792a98
00792df8: mov      r0, r3
00792dfc: mov      r1, #0
00792e00: ldr      r3, [r3]
00792e04: mov      lr, pc
00792e08: ldr      pc, [r3, #0x4c]
00792e0c: b        #0x792a98
00792e10: add      r1, sp, #0xd0
00792e14: str      r4, [r1, #-4]!
00792e18: add      r0, r0, #0x98
00792e1c: bl       #0x78adc0
00792e20: b        #0x792ac4
00792e24: add      r0, r4, #0x2c
00792e28: mov      r1, r8
00792e2c: bl       #0x41fe84
00792e30: mov      r7, r8
00792e34: str      r8, [r4, #0x30]
00792e38: b        #0x7929c8
00792e3c: ldr      r1, [r0]
00792e40: sub      r1, r1, #1
00792e44: cmp      r1, #0
00792e48: str      r1, [r0]
00792e4c: bne      #0x792e54
00792e50: bl       #0x752b38
00792e54: mov      r3, #0
00792e58: str      r3, [r4, #0x2c]
00792e5c: str      r3, [r4, #0x30]
00792e60: b        #0x792a8c
00792e64: ldr      r3, [r4, #0x50]
00792e68: ldr      r2, [r3, #8]
00792e6c: cmp      r2, #0
00792e70: bgt      #0x793514
00792e74: mov      r7, r4
00792e78: ldr      r8, [r7, #0x40]
00792e7c: cmp      r8, #0
00792e80: beq      #0x792a38
00792e84: ldr      r0, [r7, #0x3c]
00792e88: ldrb     r3, [r0, #4]
00792e8c: cmp      r3, #0
00792e90: beq      #0x7932a8
00792e94: ldr      r3, [r8, #0x50]
00792e98: mov      r7, r8
00792e9c: ldr      r2, [r3, #8]
00792ea0: cmp      r2, #0
00792ea4: ble      #0x792e78
00792ea8: str      r2, [sp, #0x24]
00792eac: ldr      r1, [sp, #0x24]
00792eb0: subs     r2, r1, #1
00792eb4: bmi      #0x792a38
00792eb8: mov      sb, #0x2c
00792ebc: mul      sb, sb, r2
00792ec0: add      r2, r4, #0xa4
00792ec4: str      r2, [sp, #0x34]
00792ec8: mov      ip, #1
00792ecc: add      r1, sp, #0x90
00792ed0: add      r2, sp, #0xc8
00792ed4: mov      fp, #0
00792ed8: str      ip, [sp, #0x3c]
00792edc: str      r1, [sp, #0x1c]
00792ee0: str      r2, [sp, #0x38]
00792ee4: str      r8, [sp, #0x2c]
00792ee8: str      r5, [sp, #0x40]
00792eec: str      r6, [sp, #0x44]
00792ef0: str      sl, [sp, #0x20]
00792ef4: ldr      r5, [r3, #4]
00792ef8: ldr      r3, [r5, sb]
00792efc: add      r5, r5, sb
00792f00: cmp      r3, #2
00792f04: beq      #0x7932d0
00792f08: cmp      r3, #0
00792f0c: bne      #0x793138
00792f10: ldr      r0, [r5, #0x20]
00792f14: bl       #0x30e4cc
00792f18: mov      r6, r0
00792f1c: ldr      r0, [r5, #0x24]
00792f20: bl       #0x30e4cc
00792f24: str      r0, [sp, #0x28]
00792f28: ldr      r8, [r5, #8]
00792f2c: ldr      r7, [r5, #0xc]
00792f30: mov      r0, r8
00792f34: bl       #0x30e754
00792f38: mov      r1, r0
00792f3c: rsb      r0, r6, #0
00792f40: str      r1, [sp, #0x18]
00792f44: bl       #0x30e964
00792f48: ldr      r1, [sp, #0x18]
00792f4c: mov      sl, r0
00792f50: mov      r0, r7
00792f54: bl       #0x30ed6c
00792f58: mov      r1, r0
00792f5c: mov      r0, sl
00792f60: bl       #0x30eba4
00792f64: str      r0, [sp, #0x30]
00792f68: mov      r0, r8
00792f6c: bl       #0x30eb08
00792f70: ldr      lr, [sp, #0x28]
00792f74: mov      sl, r0
00792f78: rsb      r0, lr, #0
00792f7c: bl       #0x30e964
00792f80: mov      r1, sl
00792f84: mov      r8, r0
00792f88: mov      r0, r7
00792f8c: bl       #0x30ed6c
00792f90: mov      r1, r0
00792f94: mov      r0, r8
00792f98: bl       #0x30eba4
00792f9c: mvn      r1, #0
00792fa0: strb     r1, [sp, #0xc8]
00792fa4: strb     r1, [sp, #0xc9]
00792fa8: strb     r1, [sp, #0xca]
00792fac: strb     r1, [sp, #0xcb]
00792fb0: ldrb     lr, [r5, #4]
00792fb4: ldr      ip, [sp, #0x20]
00792fb8: mov      r7, r0
00792fbc: strb     lr, [sp, #0xca]
00792fc0: ldrb     lr, [r5, #5]
00792fc4: ldm      ip!, {r0, r1, r2, r3}
00792fc8: strb     lr, [sp, #0xc9]
00792fcc: ldrb     lr, [r5, #6]
00792fd0: strb     lr, [sp, #0xc8]
00792fd4: ldrb     lr, [r5, #7]
00792fd8: strb     lr, [sp, #0xcb]
00792fdc: ldr      lr, [sp, #0x1c]
00792fe0: stm      lr!, {r0, r1, r2, r3}
00792fe4: ldm      ip, {r0, r1}
00792fe8: str      r1, [lr, #4]
00792fec: mov      r1, #0x41000000
00792ff0: str      r0, [lr]
00792ff4: add      r1, r1, #0xa00000
00792ff8: ldr      r0, [sp, #0x30]
00792ffc: bl       #0x30ed6c
00793000: mov      r1, #0x41000000
00793004: mov      r8, r0
00793008: add      r1, r1, #0xa00000
0079300c: mov      r0, r7
00793010: bl       #0x30ed6c
00793014: ldr      r1, [sp, #0x90]
00793018: mov      r7, r0
0079301c: mov      r0, r8
00793020: bl       #0x30ed6c
00793024: ldr      r1, [sp, #0x94]
00793028: mov      r5, r0
0079302c: mov      r0, r7
00793030: bl       #0x30ed6c
00793034: mov      r1, r0
00793038: mov      r0, r5
0079303c: bl       #0x30eba4
00793040: ldr      r1, [sp, #0x98]
00793044: bl       #0x30eba4
00793048: mvn      r1, #0x800000
0079304c: mov      r5, r0
00793050: bl       #0x30e4b4
00793054: cmp      r0, #0
00793058: beq      #0x793130
0079305c: mvn      r1, #0x80000000
00793060: mov      r0, r5
00793064: sub      r1, r1, #0x800000
00793068: bl       #0x30e9ac
0079306c: cmp      r0, #0
00793070: beq      #0x793130
00793074: ldr      r1, [sp, #0x9c]
00793078: mov      r0, r8
0079307c: str      r5, [sp, #0x98]
00793080: bl       #0x30ed6c
00793084: ldr      r1, [sp, #0xa0]
00793088: mov      r5, r0
0079308c: mov      r0, r7
00793090: bl       #0x30ed6c
00793094: mov      r1, r0
00793098: mov      r0, r5
0079309c: bl       #0x30eba4
007930a0: ldr      r1, [sp, #0xa4]
007930a4: bl       #0x30eba4
007930a8: mvn      r1, #0x800000
007930ac: mov      r5, r0
007930b0: bl       #0x30e4b4
007930b4: cmp      r0, #0
007930b8: beq      #0x7934d4
007930bc: mvn      r1, #0x80000000
007930c0: mov      r0, r5
007930c4: sub      r1, r1, #0x800000
007930c8: bl       #0x30e9ac
007930cc: cmp      r0, #0
007930d0: beq      #0x7934d4
007930d4: ldr      r1, [sp, #0x28]
007930d8: ldr      r3, [r4, #0xa0]
007930dc: str      r5, [sp, #0xa4]
007930e0: uxtb     ip, r1
007930e4: ldr      r3, [r3, #0x20]
007930e8: str      ip, [sp, #0xc]
007930ec: ldr      ip, [sp, #0x38]
007930f0: uxtb     r6, r6
007930f4: mov      lr, #0
007930f8: ldr      r0, [sp, #0x1c]
007930fc: mov      r1, r4
00793100: ldr      r2, [sp, #0x34]
00793104: str      r6, [sp, #8]
00793108: stm      sp, {ip, lr}
0079310c: bl       #0x78faa0
00793110: ldr      r2, [sp, #0x24]
00793114: add      fp, fp, #1
00793118: sub      sb, sb, #0x2c
0079311c: cmp      fp, r2
00793120: beq      #0x7934f0
00793124: ldr      ip, [sp, #0x2c]
00793128: ldr      r3, [ip, #0x50]
0079312c: b        #0x792ef4
00793130: mov      r5, #0
00793134: b        #0x793074
00793138: cmp      r3, #1
0079313c: bne      #0x793110
00793140: ldr      r0, [r5, #0x20]
00793144: bl       #0x8be2a0
00793148: uxtb     r6, r0
0079314c: ldr      r0, [r5, #0x24]
00793150: bl       #0x8be2a0
00793154: uxtb     r5, r0
00793158: orrs     r1, r5, r6
0079315c: beq      #0x793110
00793160: ldr      ip, [sp, #0x20]
00793164: ldr      lr, [sp, #0x1c]
00793168: ldm      ip!, {r0, r1, r2, r3}
0079316c: stm      lr!, {r0, r1, r2, r3}
00793170: ldm      ip, {r0, r1}
00793174: stm      lr, {r0, r1}
00793178: rsb      r0, r6, #0
0079317c: bl       #0x30e964
00793180: mov      r1, #0x41000000
00793184: add      r1, r1, #0xa00000
00793188: bl       #0x30ed6c
0079318c: mov      r7, r0
00793190: rsb      r0, r5, #0
00793194: bl       #0x30e964
00793198: mov      r1, #0x41000000
0079319c: add      r1, r1, #0xa00000
007931a0: bl       #0x30ed6c
007931a4: ldr      r1, [sp, #0x90]
007931a8: mov      sl, r0
007931ac: mov      r0, r7
007931b0: bl       #0x30ed6c
007931b4: ldr      r1, [sp, #0x94]
007931b8: mov      r8, r0
007931bc: mov      r0, sl
007931c0: bl       #0x30ed6c
007931c4: mov      r1, r0
007931c8: mov      r0, r8
007931cc: bl       #0x30eba4
007931d0: ldr      r1, [sp, #0x98]
007931d4: bl       #0x30eba4
007931d8: mvn      r1, #0x800000
007931dc: mov      r8, r0
007931e0: bl       #0x30e4b4
007931e4: cmp      r0, #0
007931e8: beq      #0x7932a0
007931ec: mvn      r1, #0x80000000
007931f0: mov      r0, r8
007931f4: sub      r1, r1, #0x800000
007931f8: bl       #0x30e9ac
007931fc: cmp      r0, #0
00793200: beq      #0x7932a0
00793204: mov      r0, r7
00793208: ldr      r1, [sp, #0x9c]
0079320c: str      r8, [sp, #0x98]
00793210: bl       #0x30ed6c
00793214: ldr      r1, [sp, #0xa0]
00793218: mov      r7, r0
0079321c: mov      r0, sl
00793220: bl       #0x30ed6c
00793224: mov      r1, r0
00793228: mov      r0, r7
0079322c: bl       #0x30eba4
00793230: ldr      r1, [sp, #0xa4]
00793234: bl       #0x30eba4
00793238: mvn      r1, #0x800000
0079323c: mov      r7, r0
00793240: bl       #0x30e4b4
00793244: cmp      r0, #0
00793248: beq      #0x79350c
0079324c: mvn      r1, #0x80000000
00793250: mov      r0, r7
00793254: sub      r1, r1, #0x800000
00793258: bl       #0x30e9ac
0079325c: cmp      r0, #0
00793260: beq      #0x79350c
00793264: ldr      r3, [r4, #0xa0]
00793268: str      r7, [sp, #0xa4]
0079326c: mov      ip, #0
00793270: ldr      r3, [r3, #0x20]
00793274: mov      r1, r4
00793278: ldr      r0, [sp, #0x1c]
0079327c: ldr      r2, [sp, #0x34]
00793280: str      r6, [sp, #8]
00793284: str      r5, [sp, #0xc]
00793288: str      ip, [sp]
0079328c: str      ip, [sp, #4]
00793290: bl       #0x78faa0
00793294: mov      r1, #0
00793298: str      r1, [sp, #0x3c]
0079329c: b        #0x793110
007932a0: mov      r8, #0
007932a4: b        #0x793204
007932a8: ldr      r1, [r0]
007932ac: sub      r1, r1, #1
007932b0: cmp      r1, #0
007932b4: str      r1, [r0]
007932b8: bne      #0x7932c0
007932bc: bl       #0x752b38
007932c0: mov      r3, #0
007932c4: str      r3, [r7, #0x40]
007932c8: str      r3, [r7, #0x3c]
007932cc: b        #0x792a38
007932d0: ldr      r3, [r5, #0x24]
007932d4: mvn      ip, #0
007932d8: ldr      sl, [r5, #0x20]
007932dc: strb     ip, [sp, #0xc8]
007932e0: strb     ip, [sp, #0xc9]
007932e4: strb     ip, [sp, #0xca]
007932e8: strb     ip, [sp, #0xcb]
007932ec: str      r3, [sp, #0x28]
007932f0: ldrb     r3, [r5, #4]
007932f4: strb     r3, [sp, #0xca]
007932f8: ldrb     r3, [r5, #5]
007932fc: strb     r3, [sp, #0xc9]
00793300: ldrb     r3, [r5, #6]
00793304: strb     r3, [sp, #0xc8]
00793308: ldrb     r7, [r5, #7]
0079330c: strb     r7, [sp, #0xcb]
00793310: ldr      r6, [r5, #0x24]
00793314: ldr      r8, [r5, #0x20]
00793318: ldr      r5, [r5, #0x10]
0079331c: mov      r1, r6
00793320: mov      r0, r8
00793324: bl       #0x30e70c
00793328: cmp      r0, #0
0079332c: mov      r0, r7
00793330: moveq    r6, r8
00793334: bl       #0x30e964
00793338: mov      r7, r0
0079333c: mov      r0, r5
00793340: bl       #0x30e964
00793344: mov      r1, #0x41000000
00793348: add      r1, r1, #0x200000
0079334c: bl       #0x30ec94
00793350: mov      r1, r0
00793354: mov      r0, r7
00793358: bl       #0x30ed6c
0079335c: bl       #0x30e4cc
00793360: cmp      r0, #0xfe
00793364: mvngt    r2, #0
00793368: strbgt   r2, [sp, #0xcb]
0079336c: ble      #0x7934dc
00793370: ldr      ip, [sp, #0x20]
00793374: ldr      lr, [sp, #0x1c]
00793378: ldm      ip!, {r0, r1, r2, r3}
0079337c: stm      lr!, {r0, r1, r2, r3}
00793380: ldm      ip, {r0, r1}
00793384: stm      lr, {r0, r1}
00793388: mov      r0, sl
0079338c: bl       #0x30e4cc
00793390: rsb      r0, r0, #0
00793394: bl       #0x30e964
00793398: mov      r1, #0x41000000
0079339c: add      r1, r1, #0xa00000
007933a0: bl       #0x30ed6c
007933a4: mov      r8, r0
007933a8: ldr      r0, [sp, #0x28]
007933ac: bl       #0x30e4cc
007933b0: rsb      r0, r0, #0
007933b4: bl       #0x30e964
007933b8: mov      r1, #0x41000000
007933bc: add      r1, r1, #0xa00000
007933c0: bl       #0x30ed6c
007933c4: ldr      r1, [sp, #0x90]
007933c8: mov      r7, r0
007933cc: mov      r0, r8
007933d0: bl       #0x30ed6c
007933d4: ldr      r1, [sp, #0x94]
007933d8: mov      r5, r0
007933dc: mov      r0, r7
007933e0: bl       #0x30ed6c
007933e4: mov      r1, r0
007933e8: mov      r0, r5
007933ec: bl       #0x30eba4
007933f0: ldr      r1, [sp, #0x98]
007933f4: bl       #0x30eba4
007933f8: mvn      r1, #0x800000
007933fc: mov      r5, r0
00793400: bl       #0x30e4b4
00793404: cmp      r0, #0
00793408: beq      #0x7934cc
0079340c: mvn      r1, #0x80000000
00793410: mov      r0, r5
00793414: sub      r1, r1, #0x800000
00793418: bl       #0x30e9ac
0079341c: cmp      r0, #0
00793420: beq      #0x7934cc
00793424: ldr      r1, [sp, #0x9c]
00793428: mov      r0, r8
0079342c: str      r5, [sp, #0x98]
00793430: bl       #0x30ed6c
00793434: ldr      r1, [sp, #0xa0]
00793438: mov      r5, r0
0079343c: mov      r0, r7
00793440: bl       #0x30ed6c
00793444: mov      r1, r0
00793448: mov      r0, r5
0079344c: bl       #0x30eba4
00793450: ldr      r1, [sp, #0xa4]
00793454: bl       #0x30eba4
00793458: mvn      r1, #0x800000
0079345c: mov      r5, r0
00793460: bl       #0x30e4b4
00793464: cmp      r0, #0
00793468: beq      #0x793504
0079346c: mvn      r1, #0x80000000
00793470: mov      r0, r5
00793474: sub      r1, r1, #0x800000
00793478: bl       #0x30e9ac
0079347c: cmp      r0, #0
00793480: beq      #0x793504
00793484: ldr      r3, [r4, #0xa0]
00793488: ldr      r1, [sp, #0x38]
0079348c: str      r5, [sp, #0xa4]
00793490: mov      r0, r6
00793494: ldr      r5, [r3, #0x20]
00793498: str      r1, [sp]
0079349c: bl       #0x8be2a0
007934a0: uxtb     ip, r0
007934a4: str      ip, [sp, #4]
007934a8: mov      r3, r5
007934ac: mov      ip, #0
007934b0: mov      r1, r4
007934b4: ldr      r0, [sp, #0x1c]
007934b8: ldr      r2, [sp, #0x34]
007934bc: str      ip, [sp, #8]
007934c0: str      ip, [sp, #0xc]
007934c4: bl       #0x78faa0
007934c8: b        #0x793110
007934cc: mov      r5, #0
007934d0: b        #0x793424
007934d4: mov      r5, #0
007934d8: b        #0x7930d4
007934dc: uxtb     r0, r0
007934e0: cmp      r0, #0
007934e4: strb     r0, [sp, #0xcb]
007934e8: beq      #0x793110
007934ec: b        #0x793370
007934f0: add      r1, sp, #0x3c
007934f4: ldm      r1, {r1, r5, r6}
007934f8: cmp      r1, #0
007934fc: beq      #0x792a70
00793500: b        #0x792a38
00793504: mov      r5, #0
00793508: b        #0x793484
0079350c: mov      r7, #0
00793510: b        #0x793264
00793514: str      r2, [sp, #0x24]
00793518: mov      r8, r4
0079351c: b        #0x792eac
00793520: eoreq    r2, r0, ip, lsr #3
00793524: strheq   r3, [r0], -r4
00793528: strheq   r3, [r0], -r8

# _ZNK7gameswf13as_textformat2isEi
007a67b4: cmp      r1, #0x23
007a67b8: moveq    r0, #1
007a67bc: bxeq     lr
007a67c0: rsbs     r0, r1, #1
007a67c4: movlo    r0, #0
007a67c8: bx       lr

# _ZN7gameswf13as_textformatC1EPNS_6playerE
007a681c: push     {r4, r5, r6, lr}
007a6820: ldr      r4, [pc, #0x20]
007a6824: mov      r5, r0
007a6828: bl       #0x76bce0
007a682c: ldr      r3, [pc, #0x18]
007a6830: add      r4, pc, r4
007a6834: mov      r0, r5
007a6838: ldr      r3, [r4, r3]
007a683c: add      r3, r3, #8
007a6840: str      r3, [r5]
007a6844: pop      {r4, r5, r6, pc}
007a6848: andseq   lr, lr, r0, ror #4
007a684c: andeq    r1, r0, r4, lsl sb

# _ZN7gameswf13as_textformatC2EPNS_6playerE
007a6850: push     {r4, r5, r6, lr}
007a6854: ldr      r4, [pc, #0x20]
007a6858: mov      r5, r0
007a685c: bl       #0x76bce0
007a6860: ldr      r3, [pc, #0x18]
007a6864: add      r4, pc, r4
007a6868: mov      r0, r5
007a686c: ldr      r3, [r4, r3]
007a6870: add      r3, r3, #8
007a6874: str      r3, [r5]
007a6878: pop      {r4, r5, r6, pc}
007a687c: andseq   lr, lr, ip, lsr #4
007a6880: andeq    r1, r0, r4, lsl sb

# _ZN7gameswf13as_textformatD1Ev
007a6884: ldr      r3, [pc, #0x24]
007a6888: ldr      r2, [pc, #0x24]
007a688c: push     {r4, lr}
007a6890: add      r3, pc, r3
007a6894: ldr      r2, [r3, r2]
007a6898: mov      r4, r0
007a689c: add      r2, r2, #8
007a68a0: str      r2, [r0]
007a68a4: bl       #0x769a9c
007a68a8: mov      r0, r4
007a68ac: pop      {r4, pc}
007a68b0: andseq   lr, lr, r0, lsl #4
007a68b4: andeq    r1, r0, r4, lsl sb

# _ZN7gameswf13as_textformatD0Ev
007a6b40: ldr      r3, [pc, #0x2c]
007a6b44: ldr      r2, [pc, #0x2c]
007a6b48: push     {r4, lr}
007a6b4c: add      r3, pc, r3
007a6b50: ldr      r2, [r3, r2]
007a6b54: mov      r4, r0
007a6b58: add      r2, r2, #8
007a6b5c: str      r2, [r0]
007a6b60: bl       #0x769a9c
007a6b64: mov      r0, r4
007a6b68: bl       #0x30e2b0
007a6b6c: mov      r0, r4
007a6b70: pop      {r4, pc}
007a6b74: andseq   sp, lr, r4, asr #30
007a6b78: andeq    r1, r0, r4, lsl sb

