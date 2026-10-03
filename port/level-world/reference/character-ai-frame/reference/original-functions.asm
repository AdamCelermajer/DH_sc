
# _ZN6CharAI13_UpdateTargetEv
003cb908: push     {r4, r5, r6, lr}
003cb90c: mov      r4, r0
003cb910: ldr      r0, [r0, #4]
003cb914: add      r0, r0, #0x4f0
003cb918: add      r0, r0, #0xc
003cb91c: bl       #0x3c0230
003cb920: cmp      r0, #0
003cb924: beq      #0x3cb92c
003cb928: pop      {r4, r5, r6, pc}
003cb92c: ldr      r0, [r4, #4]
003cb930: add      r0, r0, #0x4f0
003cb934: add      r0, r0, #0xc
003cb938: bl       #0x3c01c0
003cb93c: cmp      r0, #0
003cb940: bne      #0x3cb928
003cb944: ldr      r3, [r4, #0x40]
003cb948: cmp      r3, #0
003cb94c: beq      #0x3cb928
003cb950: mov      r0, r3
003cb954: ldr      r1, [r4, #4]
003cb958: ldr      r3, [r3]
003cb95c: mov      lr, pc
003cb960: ldr      pc, [r3, #0x88]
003cb964: subs     r2, r0, #0
003cb968: beq      #0x3cbaa4
003cb96c: ldr      r3, [r4, #0x40]
003cb970: cmp      r3, #0
003cb974: beq      #0x3cb928
003cb978: ldr      r0, [r4, #4]
003cb97c: bl       #0x3a2fec
003cb980: ldr      r3, [r4, #0x40]
003cb984: mov      r0, r3
003cb988: ldr      r3, [r3]
003cb98c: mov      lr, pc
003cb990: ldr      pc, [r3, #0x34]
003cb994: ldrb     r3, [r4, #0x48]
003cb998: eor      r0, r0, #1
003cb99c: uxtb     r5, r0
003cb9a0: cmp      r3, #0
003cb9a4: bne      #0x3cba6c
003cb9a8: cmp      r5, #0
003cb9ac: bne      #0x3cbaf8
003cb9b0: ldr      r1, [r4, #0x40]
003cb9b4: strb     r5, [r4, #0x48]
003cb9b8: cmp      r1, #0
003cb9bc: beq      #0x3cb928
003cb9c0: mov      r0, r4
003cb9c4: bl       #0x3d4ed8
003cb9c8: ldrb     r3, [r4, #0x49]
003cb9cc: mov      r5, r0
003cb9d0: cmp      r3, #0
003cb9d4: bne      #0x3cba88
003cb9d8: cmp      r0, #0
003cb9dc: bne      #0x3cbae4
003cb9e0: ldr      r3, [r4, #0x40]
003cb9e4: strb     r5, [r4, #0x49]
003cb9e8: cmp      r3, #0
003cb9ec: beq      #0x3cb928
003cb9f0: cmp      r5, #0
003cb9f4: beq      #0x3cb928
003cb9f8: mov      r0, r3
003cb9fc: ldr      r1, [r4, #4]
003cba00: ldr      r3, [r3]
003cba04: mov      lr, pc
003cba08: ldr      pc, [r3, #0x88]
003cba0c: cmp      r0, #0
003cba10: beq      #0x3cb928
003cba14: ldr      r3, [r4, #4]
003cba18: mov      r0, r3
003cba1c: ldr      r3, [r3]
003cba20: mov      lr, pc
003cba24: ldr      pc, [r3, #0x124]
003cba28: cmp      r0, #0
003cba2c: beq      #0x3cbabc
003cba30: mov      r0, r4
003cba34: ldr      r1, [r4, #0x40]
003cba38: bl       #0x3d63d8
003cba3c: cmp      r0, #0
003cba40: bne      #0x3cbb0c
003cba44: mov      r0, r4
003cba48: ldr      r1, [r4, #0x40]
003cba4c: bl       #0x3d6604
003cba50: cmp      r0, #0
003cba54: beq      #0x3cbad0
003cba58: ldr      r2, [r4, #0x40]
003cba5c: ldr      r0, [r4, #4]
003cba60: mov      r1, #0xf
003cba64: pop      {r4, r5, r6, lr}
003cba68: b        #0x3a4d5c
003cba6c: cmp      r5, #0
003cba70: bne      #0x3cb9b0
003cba74: ldr      r0, [r4, #4]
003cba78: mov      r1, #0xa
003cba7c: ldr      r2, [r4, #0x40]
003cba80: bl       #0x3a4d5c
003cba84: b        #0x3cb9b0
003cba88: cmp      r0, #0
003cba8c: bne      #0x3cb9e0
003cba90: ldr      r0, [r4, #4]
003cba94: mov      r1, #0xc
003cba98: ldr      r2, [r4, #0x40]
003cba9c: bl       #0x3a4d5c
003cbaa0: b        #0x3cb9e0
003cbaa4: ldr      r0, [r4, #4]
003cbaa8: mov      r1, #0xc
003cbaac: str      r2, [r4, #0x40]
003cbab0: str      r2, [r4, #0x44]
003cbab4: pop      {r4, r5, r6, lr}
003cbab8: b        #0x3a4d5c
003cbabc: mov      r0, r4
003cbac0: ldr      r1, [r4, #0x40]
003cbac4: bl       #0x3d6188
003cbac8: cmp      r0, #0
003cbacc: bne      #0x3cbb20
003cbad0: ldr      r2, [r4, #0x40]
003cbad4: ldr      r0, [r4, #4]
003cbad8: mov      r1, #0xe
003cbadc: pop      {r4, r5, r6, lr}
003cbae0: b        #0x3a4d5c
003cbae4: ldr      r0, [r4, #4]
003cbae8: mov      r1, #0xd
003cbaec: ldr      r2, [r4, #0x40]
003cbaf0: bl       #0x3a4d5c
003cbaf4: b        #0x3cb9e0
003cbaf8: ldr      r0, [r4, #4]
003cbafc: mov      r1, #0xb
003cbb00: ldr      r2, [r4, #0x40]
003cbb04: bl       #0x3a4d5c
003cbb08: b        #0x3cb9b0
003cbb0c: ldr      r2, [r4, #0x40]
003cbb10: ldr      r0, [r4, #4]
003cbb14: mov      r1, #0x10
003cbb18: pop      {r4, r5, r6, lr}
003cbb1c: b        #0x3a4d5c
003cbb20: ldr      r2, [r4, #0x40]
003cbb24: ldr      r0, [r4, #4]
003cbb28: mov      r1, #0x11
003cbb2c: pop      {r4, r5, r6, lr}
003cbb30: b        #0x3a4d5c

# _Z20PushProfilingContextPKc
003136b4: bx       lr

# _ZN6CharAI6UpdateEv
003cfbf4: push     {r4, r5, r6, lr}
003cfbf8: mov      r5, r0
003cfbfc: ldr      r0, [pc, #0x140]
003cfc00: ldr      r4, [pc, #0x140]
003cfc04: add      r0, pc, r0
003cfc08: bl       #0x3136b4
003cfc0c: ldrb     r3, [r5, #0x18]
003cfc10: add      r4, pc, r4
003cfc14: cmp      r3, #0
003cfc18: bne      #0x3cfc44
003cfc1c: ldr      r6, [r5, #4]
003cfc20: ldr      r3, [r6, #0x378]
003cfc24: ldrb     r2, [r3, #9]
003cfc28: cmp      r2, #0
003cfc2c: bne      #0x3cfc60
003cfc30: ldr      r2, [pc, #0x114]
003cfc34: ldr      r2, [r4, r2]
003cfc38: ldrb     r2, [r2]
003cfc3c: cmp      r2, #0
003cfc40: beq      #0x3cfc54
003cfc44: ldr      r0, [pc, #0x104]
003cfc48: add      r0, pc, r0
003cfc4c: pop      {r4, r5, r6, lr}
003cfc50: b        #0x3136b8
003cfc54: ldrb     r3, [r3, #8]
003cfc58: cmp      r3, #0
003cfc5c: bne      #0x3cfc44
003cfc60: ldr      r3, [r6, #0x520]
003cfc64: tst      r3, #0x100
003cfc68: beq      #0x3cfc44
003cfc6c: ldr      r3, [r6]
003cfc70: mov      r0, r6
003cfc74: mov      lr, pc
003cfc78: ldr      pc, [r3, #0xc4]
003cfc7c: cmp      r0, #0
003cfc80: beq      #0x3cfc90
003cfc84: ldrb     r3, [r6, #0x2ee]
003cfc88: cmp      r3, #0
003cfc8c: bne      #0x3cfd34
003cfc90: ldr      r6, [pc, #0xbc]
003cfc94: ldr      r3, [r5, #4]
003cfc98: mov      r2, #1
003cfc9c: add      r6, pc, r6
003cfca0: ldr      r4, [pc, #0xb0]
003cfca4: strb     r2, [r3, #0x88]
003cfca8: mov      r0, r6
003cfcac: bl       #0x3136b4
003cfcb0: mov      r0, r5
003cfcb4: bl       #0x3cb908
003cfcb8: add      r4, pc, r4
003cfcbc: mov      r0, r6
003cfcc0: ldr      r6, [pc, #0x94]
003cfcc4: bl       #0x3136b8
003cfcc8: mov      r0, r4
003cfccc: bl       #0x3136b4
003cfcd0: mov      r0, r5
003cfcd4: bl       #0x3cc5a4
003cfcd8: add      r6, pc, r6
003cfcdc: mov      r0, r4
003cfce0: ldr      r4, [pc, #0x78]
003cfce4: bl       #0x3136b8
003cfce8: mov      r0, r6
003cfcec: bl       #0x3136b4
003cfcf0: mov      r0, r5
003cfcf4: bl       #0x3cf3f0
003cfcf8: add      r4, pc, r4
003cfcfc: mov      r0, r6
003cfd00: bl       #0x3136b8
003cfd04: mov      r0, r4
003cfd08: bl       #0x3136b4
003cfd0c: mov      r0, r5
003cfd10: ldr      r3, [r5]
003cfd14: mov      lr, pc
003cfd18: ldr      pc, [r3, #0x18]
003cfd1c: mov      r0, r4
003cfd20: bl       #0x3136b8
003cfd24: ldr      r0, [pc, #0x38]
003cfd28: add      r0, pc, r0
003cfd2c: pop      {r4, r5, r6, lr}
003cfd30: b        #0x3136b8
003cfd34: ldrb     r3, [r6, #0x2f0]
003cfd38: cmp      r3, #0
003cfd3c: beq      #0x3cfc44
003cfd40: b        #0x3cfc90
003cfd44: strdeq   r5, r6, [pc], #-0x7c
003cfd48: subseq   r4, ip, r0, lsl #29
003cfd4c: andeq    r3, r0, r0, asr r6
003cfd50: strheq   r5, [pc], #-0x78
003cfd54: subeq    r5, pc, ip, ror r7
003cfd58: subeq    r5, pc, r8, ror r7
003cfd5c: subeq    r5, pc, r0, ror r7
003cfd60: subeq    r5, pc, r8, ror #14
003cfd64: ldrdeq   r5, r6, [pc], #-0x68

# _ZN6CharAI12_UpdateAggroEv
003cf3f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003cf3f4: ldr      r4, [pc, #0x7b4]
003cf3f8: ldr      r6, [pc, #0x7b4]
003cf3fc: ldr      r3, [r0, #4]
003cf400: add      r4, pc, r4
003cf404: ldr      r2, [r4, r6]
003cf408: sub      sp, sp, #0x12c
003cf40c: mov      r5, r0
003cf410: ldr      r2, [r2]
003cf414: mov      r0, r3
003cf418: str      r2, [sp, #0x124]
003cf41c: ldr      r3, [r3]
003cf420: mov      lr, pc
003cf424: ldr      pc, [r3, #0x28]
003cf428: cmp      r0, #0
003cf42c: beq      #0x3cf44c
003cf430: ldr      r3, [r4, r6]
003cf434: ldr      r2, [sp, #0x124]
003cf438: ldr      r3, [r3]
003cf43c: cmp      r2, r3
003cf440: bne      #0x3cfb84
003cf444: add      sp, sp, #0x12c
003cf448: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003cf44c: ldr      r3, [r5, #4]
003cf450: mov      r0, r3
003cf454: ldr      r3, [r3]
003cf458: mov      lr, pc
003cf45c: ldr      pc, [r3, #0x28]
003cf460: cmp      r0, #0
003cf464: beq      #0x3cf74c
003cf468: ldr      r0, [r5, #4]
003cf46c: bl       #0x3a310c
003cf470: cmp      r0, #0
003cf474: bne      #0x3cf430
003cf478: ldr      r0, [r5, #4]
003cf47c: bl       #0x3a3064
003cf480: cmp      r0, #0
003cf484: beq      #0x3cf59c
003cf488: ldr      r3, [r5, #4]
003cf48c: mov      r0, r3
003cf490: ldr      r3, [r3]
003cf494: mov      lr, pc
003cf498: ldr      pc, [r3, #0x54]
003cf49c: subs     sl, r0, #0
003cf4a0: bne      #0x3cf59c
003cf4a4: ldr      r0, [r5, #4]
003cf4a8: ldr      r3, [r0, #0x408]
003cf4ac: cmp      r3, #0
003cf4b0: beq      #0x3cf5a0
003cf4b4: add      r0, r0, #0x3c8
003cf4b8: bl       #0x3d4a18
003cf4bc: ldr      r3, [r5, #4]
003cf4c0: add      r7, sp, #0xe0
003cf4c4: mov      r8, r0
003cf4c8: ldr      r1, [r3, #0x408]
003cf4cc: mov      r0, r7
003cf4d0: bl       #0x33dd2c
003cf4d4: mov      r0, r7
003cf4d8: bl       #0x33ff54
003cf4dc: cmp      r0, #0
003cf4e0: cmpne    r8, #0
003cf4e4: mov      r7, r0
003cf4e8: beq      #0x3cf990
003cf4ec: cmp      r8, r0
003cf4f0: beq      #0x3cf990
003cf4f4: ldr      r0, [r5, #4]
003cf4f8: mov      r1, r7
003cf4fc: add      r0, r0, #0x3c8
003cf500: bl       #0x3d4ac8
003cf504: mov      sb, r0
003cf508: ldr      r0, [r5, #4]
003cf50c: mov      r1, r8
003cf510: add      r0, r0, #0x3c8
003cf514: bl       #0x3d4ac8
003cf518: ldr      r3, [pc, #0x698]
003cf51c: mov      r7, r0
003cf520: mov      r0, sb
003cf524: ldr      r3, [r4, r3]
003cf528: ldr      r3, [r3]
003cf52c: ldr      r1, [r3, #8]
003cf530: bl       #0x30ed6c
003cf534: mov      r1, r0
003cf538: mov      r0, r7
003cf53c: bl       #0x30e2f8
003cf540: cmp      r0, #0
003cf544: beq      #0x3cf430
003cf548: ldr      r3, [pc, #0x66c]
003cf54c: add      r7, sp, #0xf4
003cf550: ldr      sb, [r4, r3]
003cf554: mov      r0, sb
003cf558: bl       #0x337888
003cf55c: ldr      r1, [pc, #0x65c]
003cf560: add      r2, sp, #0xec
003cf564: mov      r0, r7
003cf568: add      r1, pc, r1
003cf56c: bl       #0x3140ec
003cf570: mov      r1, r7
003cf574: mov      r0, sb
003cf578: bl       #0x337a88
003cf57c: mov      r0, r7
003cf580: bl       #0x318254
003cf584: ldr      r0, [r5, #4]
003cf588: mov      r1, r8
003cf58c: mov      r2, sl
003cf590: add      r0, r0, #0x3c8
003cf594: bl       #0x3d6890
003cf598: b        #0x3cf430
003cf59c: ldr      r0, [r5, #4]
003cf5a0: bl       #0x3a3094
003cf5a4: cmp      r0, #0
003cf5a8: ldreq    r0, [r5, #4]
003cf5ac: beq      #0x3cf5c0
003cf5b0: ldr      r0, [r5, #4]
003cf5b4: ldr      r3, [r0, #0x418]
003cf5b8: cmp      r3, #0
003cf5bc: bne      #0x3cf430
003cf5c0: ldr      r3, [pc, #0x5fc]
003cf5c4: ldr      r3, [r4, r3]
003cf5c8: ldr      r7, [r3]
003cf5cc: bl       #0x3a2fec
003cf5d0: ldr      r3, [r5, #4]
003cf5d4: mov      r2, #0x44
003cf5d8: mla      r7, r2, r0, r7
003cf5dc: add      r0, r3, #0x4f0
003cf5e0: add      r0, r0, #0xc
003cf5e4: ldr      r8, [r7, #0x3c]
003cf5e8: bl       #0x3c0230
003cf5ec: cmp      r0, #0
003cf5f0: bne      #0x3cf8c4
003cf5f4: mov      r0, r5
003cf5f8: bl       #0x3d49f0
003cf5fc: cmp      r0, #0
003cf600: ldreq    r8, [r7, #0x40]
003cf604: ldr      sl, [r5, #4]
003cf608: add      r7, sp, #0x18
003cf60c: mov      r1, sl
003cf610: mvn      r2, #0x80000000
003cf614: mov      r3, #2
003cf618: mov      sb, #1
003cf61c: mov      r0, r7
003cf620: str      sb, [sp]
003cf624: bl       #0x4a2730
003cf628: ldr      r2, [pc, #0x598]
003cf62c: ldr      r3, [pc, #0x598]
003cf630: mov      r1, r8
003cf634: ldr      r2, [r4, r2]
003cf638: ldr      r3, [r4, r3]
003cf63c: mov      r0, r7
003cf640: ldr      r2, [r2, #0x38]
003cf644: add      r3, r3, #8
003cf648: str      r3, [sp, #0xc0]
003cf64c: add      ip, r2, #0x60
003cf650: str      ip, [sp, #0xc4]
003cf654: ldr      lr, [r2, #0x60]
003cf658: movw     r2, #0xfdb
003cf65c: movt     r2, #0x40c9
003cf660: add      r3, sp, #0xc0
003cf664: str      lr, [sp, #0xc8]
003cf668: str      ip, [sp, #0xcc]
003cf66c: bl       #0x4a3428
003cf670: ldr      r3, [sp, #0x18]
003cf674: ldr      r2, [sp, #0x28]
003cf678: cmp      r2, r3
003cf67c: beq      #0x3cf974
003cf680: ldr      r2, [pc, #0x548]
003cf684: mov      r8, sb
003cf688: ldr      sb, [pc, #0x544]
003cf68c: str      r2, [sp, #0xc]
003cf690: ldr      r2, [pc, #0x540]
003cf694: ldr      sl, [pc, #0x540]
003cf698: add      sb, pc, sb
003cf69c: add      r2, pc, r2
003cf6a0: str      r2, [sp, #0x10]
003cf6a4: ldr      r2, [pc, #0x534]
003cf6a8: add      r2, pc, r2
003cf6ac: str      r2, [sp, #0x14]
003cf6b0: b        #0x3cf6e0
003cf6b4: mov      r2, fp
003cf6b8: ldr      r0, [r5, #4]
003cf6bc: mov      r1, #9
003cf6c0: bl       #0x3a4d5c
003cf6c4: mov      r8, #0
003cf6c8: mov      r0, r7
003cf6cc: bl       #0x38fb18
003cf6d0: ldr      r3, [sp, #0x18]
003cf6d4: ldr      r2, [sp, #0x28]
003cf6d8: cmp      r2, r3
003cf6dc: beq      #0x3cf924
003cf6e0: ldr      r2, [r3, #0xc]
003cf6e4: ands     r2, r2, #1
003cf6e8: bne      #0x3cf704
003cf6ec: ldr      r1, [r4, sl]
003cf6f0: ldr      r1, [r1]
003cf6f4: cmp      r1, #2
003cf6f8: beq      #0x3cf918
003cf6fc: cmp      r1, #1
003cf700: beq      #0x3cf948
003cf704: ldr      fp, [r3]
003cf708: ldr      r0, [r5, #4]
003cf70c: mov      r1, fp
003cf710: add      r0, r0, #0x3c8
003cf714: bl       #0x3d574c
003cf718: cmp      r0, #0
003cf71c: bne      #0x3cf6b4
003cf720: ldr      r0, [r5, #4]
003cf724: mov      r1, fp
003cf728: add      r0, r0, #0x3c8
003cf72c: bl       #0x3d511c
003cf730: cmp      r0, #0
003cf734: beq      #0x3cf8ec
003cf738: mov      r2, fp
003cf73c: ldr      r0, [r5, #4]
003cf740: mov      r1, #7
003cf744: bl       #0x3a4d5c
003cf748: b        #0x3cf6c8
003cf74c: ldr      r0, [r5, #4]
003cf750: bl       #0x3a3094
003cf754: cmp      r0, #0
003cf758: bne      #0x3cf468
003cf75c: mov      r0, r5
003cf760: bl       #0x3cc484
003cf764: cmp      r0, #0
003cf768: beq      #0x3cf884
003cf76c: mov      r3, #0
003cf770: str      r3, [r5, #0xc]
003cf774: ldr      r3, [pc, #0x440]
003cf778: add      r7, sp, #0x10c
003cf77c: ldr      r8, [r4, r3]
003cf780: mov      r0, r8
003cf784: bl       #0x337888
003cf788: ldr      r1, [pc, #0x454]
003cf78c: add      r2, sp, #0xf0
003cf790: mov      r0, r7
003cf794: add      r1, pc, r1
003cf798: bl       #0x3140ec
003cf79c: mov      r0, r8
003cf7a0: mov      r1, r7
003cf7a4: bl       #0x337a88
003cf7a8: mov      r8, r0
003cf7ac: ldr      r0, [sp, #0x120]
003cf7b0: cmp      r0, r7
003cf7b4: beq      #0x3cf7cc
003cf7b8: cmp      r0, #0
003cf7bc: beq      #0x3cf7cc
003cf7c0: ldr      r1, [sp, #0x10c]
003cf7c4: rsb      r1, r0, r1
003cf7c8: bl       #0x31bb44
003cf7cc: cmp      r8, #0
003cf7d0: bne      #0x3cf468
003cf7d4: ldr      r7, [r5, #8]
003cf7d8: cmp      r7, #0
003cf7dc: ble      #0x3cf7fc
003cf7e0: ldr      r3, [pc, #0x3e0]
003cf7e4: ldr      r0, [r4, r3]
003cf7e8: bl       #0x31f66c
003cf7ec: rsb      r0, r0, r7
003cf7f0: cmp      r0, #0
003cf7f4: str      r0, [r5, #8]
003cf7f8: bgt      #0x3cf430
003cf7fc: ldr      r3, [pc, #0x3e4]
003cf800: movw     lr, #0xe6ab
003cf804: movw     r2, #0xdb17
003cf808: ldr      ip, [r4, r3]
003cf80c: movt     r2, #0x2b52
003cf810: movw     r0, #0xf26b
003cf814: ldr      r3, [ip]
003cf818: movt     r0, #0xda
003cf81c: movw     r1, #0x851f
003cf820: mul      r3, lr, r3
003cf824: movt     r1, #0x51eb
003cf828: add      r3, r3, #0x2b000
003cf82c: add      r3, r3, #0x3fc
003cf830: add      r3, r3, #1
003cf834: umull    lr, r2, r2, r3
003cf838: rsb      lr, r2, r3
003cf83c: add      r2, r2, lr, lsr #1
003cf840: ldr      lr, [pc, #0x3a4]
003cf844: lsr      r2, r2, #0x17
003cf848: mls      r2, r0, r2, r3
003cf84c: ldr      lr, [r4, lr]
003cf850: umull    r0, r3, r1, r2
003cf854: mov      r0, #0xc8
003cf858: lsr      r3, r3, #6
003cf85c: mls      r3, r0, r3, r2
003cf860: ldr      r1, [lr]
003cf864: eor      r0, r3, r3, asr #31
003cf868: sub      r0, r0, r3, asr #31
003cf86c: add      r3, r0, #0x64
003cf870: add      r1, r1, #1
003cf874: str      r1, [lr]
003cf878: str      r2, [ip]
003cf87c: str      r3, [r5, #8]
003cf880: b        #0x3cf468
003cf884: ldr      r3, [r5, #0xc]
003cf888: cmp      r3, #0x1f4
003cf88c: bge      #0x3cf76c
003cf890: ldr      r3, [pc, #0x330]
003cf894: ldr      r8, [r5, #8]
003cf898: ldr      r7, [r4, r3]
003cf89c: mov      r0, r7
003cf8a0: bl       #0x31f66c
003cf8a4: rsb      r0, r0, r8
003cf8a8: str      r0, [r5, #8]
003cf8ac: mov      r0, r7
003cf8b0: ldr      r7, [r5, #0xc]
003cf8b4: bl       #0x31f66c
003cf8b8: add      r0, r0, r7
003cf8bc: str      r0, [r5, #0xc]
003cf8c0: b        #0x3cf430
003cf8c4: ldr      sl, [r5, #4]
003cf8c8: movw     r3, #0x143c
003cf8cc: mov      r1, #0
003cf8d0: ldr      sb, [sl, r3]
003cf8d4: mov      r0, sb
003cf8d8: bl       #0x30e2f8
003cf8dc: cmp      r0, #0
003cf8e0: movne    r8, sb
003cf8e4: bne      #0x3cf608
003cf8e8: b        #0x3cf5f4
003cf8ec: ldr      r0, [r5, #4]
003cf8f0: mov      r1, fp
003cf8f4: add      r0, r0, #0x3c8
003cf8f8: bl       #0x3d5a98
003cf8fc: cmp      r0, #0
003cf900: beq      #0x3cf6c8
003cf904: mov      r2, fp
003cf908: ldr      r0, [r5, #4]
003cf90c: mov      r1, #8
003cf910: bl       #0x3a4d5c
003cf914: b        #0x3cf6c8
003cf918: str      r2, [r2]
003cf91c: ldr      r3, [sp, #0x18]
003cf920: b        #0x3cf704
003cf924: cmp      r8, #0
003cf928: bne      #0x3cf974
003cf92c: ldr      r3, [pc, #0x2bc]
003cf930: mov      r0, r7
003cf934: ldr      r3, [r4, r3]
003cf938: add      r3, r3, #8
003cf93c: str      r3, [sp, #0xc0]
003cf940: bl       #0x38d18c
003cf944: b        #0x3cf430
003cf948: ldr      r3, [sp, #0xc]
003cf94c: movw     ip, #0x506
003cf950: mov      r1, sb
003cf954: ldr      r0, [r4, r3]
003cf958: ldr      r2, [sp, #0x10]
003cf95c: ldr      r3, [sp, #0x14]
003cf960: add      r0, r0, #0xa8
003cf964: str      ip, [sp]
003cf968: bl       #0x30e004
003cf96c: ldr      r3, [sp, #0x18]
003cf970: b        #0x3cf704
003cf974: ldr      r2, [r5, #0x40]
003cf978: cmp      r2, #0
003cf97c: beq      #0x3cf92c
003cf980: ldr      r0, [r5, #4]
003cf984: mov      r1, #0xc
003cf988: bl       #0x3a4d5c
003cf98c: b        #0x3cf92c
003cf990: ldr      r0, [r5, #4]
003cf994: mov      r1, r7
003cf998: add      r0, r0, #0x3c8
003cf99c: bl       #0x3d574c
003cf9a0: subs     r8, r0, #0
003cf9a4: beq      #0x3cfb10
003cf9a8: add      r8, sp, #0x6c
003cf9ac: mov      sb, #1
003cf9b0: ldr      r1, [r5, #4]
003cf9b4: mov      r2, sb
003cf9b8: mov      r3, #2
003cf9bc: mov      r0, r8
003cf9c0: str      sb, [sp]
003cf9c4: bl       #0x4a2730
003cf9c8: ldr      r2, [pc, #0x1f8]
003cf9cc: ldr      r3, [pc, #0x1f8]
003cf9d0: ldr      sl, [r4, r2]
003cf9d4: ldr      r3, [r4, r3]
003cf9d8: ldr      r2, [pc, #0x1e4]
003cf9dc: ldr      r1, [sl, #0x38]
003cf9e0: add      r3, r3, #8
003cf9e4: str      r3, [sp, #0xd0]
003cf9e8: add      r3, r1, #0x70
003cf9ec: str      r3, [sp, #0xd4]
003cf9f0: ldr      r1, [r1, #0x70]
003cf9f4: ldr      r2, [r4, r2]
003cf9f8: ldr      r0, [r5, #4]
003cf9fc: str      r3, [sp, #0xdc]
003cfa00: ldr      fp, [r2]
003cfa04: str      r1, [sp, #0xd8]
003cfa08: bl       #0x3a2fec
003cfa0c: mov      r3, #0x44
003cfa10: mla      r0, r3, r0, fp
003cfa14: movw     r2, #0xfdb
003cfa18: movt     r2, #0x40c9
003cfa1c: ldr      r1, [r0, #0x40]
003cfa20: add      r3, sp, #0xd0
003cfa24: mov      r0, r8
003cfa28: bl       #0x4a3428
003cfa2c: ldr      r3, [sp, #0x6c]
003cfa30: ldr      r2, [sp, #0x7c]
003cfa34: cmp      r2, r3
003cfa38: beq      #0x3cfb88
003cfa3c: ldr      r0, [sl, #0x40]
003cfa40: mov      r2, sb
003cfa44: mov      r1, #0
003cfa48: bl       #0x36e478
003cfa4c: ldr      sb, [r5, #4]
003cfa50: ldr      r7, [r0, #0x660]
003cfa54: ldr      sl, [sb, #0x448]
003cfa58: add      sb, sb, #0x440
003cfa5c: add      sb, sb, #4
003cfa60: cmp      sl, #0
003cfa64: beq      #0x3cfb08
003cfa68: mov      r2, sb
003cfa6c: b        #0x3cfa78
003cfa70: mov      r2, sl
003cfa74: mov      sl, r3
003cfa78: ldr      r3, [sl, #0x10]
003cfa7c: cmp      r7, r3
003cfa80: ldrhi    r3, [sl, #0xc]
003cfa84: ldrls    r3, [sl, #8]
003cfa88: movhi    sl, r2
003cfa8c: cmp      r3, #0
003cfa90: bne      #0x3cfa70
003cfa94: cmp      sb, sl
003cfa98: beq      #0x3cfaa8
003cfa9c: ldr      r3, [sl, #0x10]
003cfaa0: cmp      r7, r3
003cfaa4: blo      #0x3cfb08
003cfaa8: ldr      r3, [sp, #0x6c]
003cfaac: ldr      r2, [sp, #0x7c]
003cfab0: cmp      r2, r3
003cfab4: bne      #0x3cfad4
003cfab8: b        #0x3cfb38
003cfabc: mov      r0, r8
003cfac0: bl       #0x38fb18
003cfac4: ldr      r3, [sp, #0x6c]
003cfac8: ldr      r2, [sp, #0x7c]
003cfacc: cmp      r2, r3
003cfad0: beq      #0x3cfb38
003cfad4: ldr      r3, [r3]
003cfad8: cmp      r3, r7
003cfadc: bne      #0x3cfabc
003cfae0: mov      r3, #1
003cfae4: cmp      sb, sl
003cfae8: beq      #0x3cfb40
003cfaec: ldr      r3, [pc, #0xfc]
003cfaf0: mov      r0, r8
003cfaf4: ldr      r3, [r4, r3]
003cfaf8: add      r3, r3, #8
003cfafc: str      r3, [sp, #0xd0]
003cfb00: bl       #0x38d18c
003cfb04: b        #0x3cf430
003cfb08: mov      sl, sb
003cfb0c: b        #0x3cfaa8
003cfb10: mov      r1, r7
003cfb14: mov      r0, r5
003cfb18: bl       #0x3d6d68
003cfb1c: mov      r0, r5
003cfb20: mov      r1, r8
003cfb24: mov      r2, r8
003cfb28: bl       #0x3d6890
003cfb2c: mov      r0, r5
003cfb30: bl       #0x3d49c4
003cfb34: b        #0x3cf430
003cfb38: mov      r3, #0
003cfb3c: b        #0x3cfae4
003cfb40: cmp      r3, #0
003cfb44: beq      #0x3cfaec
003cfb48: ldr      r0, [r5, #4]
003cfb4c: mov      r1, r7
003cfb50: add      r0, r0, #0x3c8
003cfb54: bl       #0x3d574c
003cfb58: cmp      r0, #0
003cfb5c: beq      #0x3cfaec
003cfb60: ldr      r3, [pc, #0x50]
003cfb64: ldr      r0, [r5, #4]
003cfb68: mov      r1, r7
003cfb6c: ldr      r3, [r4, r3]
003cfb70: add      r0, r0, #0x3c8
003cfb74: ldr      r3, [r3]
003cfb78: ldr      r2, [r3, #0x30]
003cfb7c: bl       #0x3d7c68
003cfb80: b        #0x3cfaec
003cfb84: bl       #0x30e310
003cfb88: mov      r1, r7
003cfb8c: mov      r0, r5
003cfb90: bl       #0x3d6d68
003cfb94: mov      r1, #0
003cfb98: mov      r0, r5
003cfb9c: mov      r2, r1
003cfba0: bl       #0x3d6890
003cfba4: mov      r0, r5
003cfba8: bl       #0x3d49c4
003cfbac: b        #0x3cfaec

# _Z19PopProfilingContextPKc
003136b8: bx       lr

# _ZN6CharAI13_UpdateMasterEv
003cc5a4: push     {r4, r5, r6, lr}
003cc5a8: ldr      r3, [r0, #0x50]
003cc5ac: mov      r4, r0
003cc5b0: cmp      r3, #0
003cc5b4: beq      #0x3cc644
003cc5b8: ldr      r0, [r0, #4]
003cc5bc: bl       #0x3a2fec
003cc5c0: ldr      r3, [r4, #0x50]
003cc5c4: mov      r0, r3
003cc5c8: ldr      r3, [r3]
003cc5cc: mov      lr, pc
003cc5d0: ldr      pc, [r3, #0x34]
003cc5d4: ldrb     r3, [r4, #0x54]
003cc5d8: eor      r0, r0, #1
003cc5dc: uxtb     r5, r0
003cc5e0: cmp      r3, #0
003cc5e4: beq      #0x3cc648
003cc5e8: cmp      r5, #0
003cc5ec: beq      #0x3cc6fc
003cc5f0: ldr      r1, [r4, #0x50]
003cc5f4: strb     r5, [r4, #0x54]
003cc5f8: cmp      r1, #0
003cc5fc: beq      #0x3cc644
003cc600: mov      r0, r4
003cc604: bl       #0x3d4ed8
003cc608: ldrb     r3, [r4, #0x55]
003cc60c: mov      r5, r0
003cc610: cmp      r3, #0
003cc614: bne      #0x3cc664
003cc618: cmp      r0, #0
003cc61c: bne      #0x3cc6e8
003cc620: ldr      r3, [r4, #0x50]
003cc624: strb     r5, [r4, #0x55]
003cc628: cmp      r3, #0
003cc62c: beq      #0x3cc644
003cc630: ldrb     r3, [r4, #0x54]
003cc634: cmp      r3, #0
003cc638: beq      #0x3cc644
003cc63c: cmp      r5, #0
003cc640: bne      #0x3cc680
003cc644: pop      {r4, r5, r6, pc}
003cc648: cmp      r5, #0
003cc64c: beq      #0x3cc5f0
003cc650: ldr      r0, [r4, #4]
003cc654: mov      r1, #0x13
003cc658: ldr      r2, [r4, #0x50]
003cc65c: bl       #0x3a4d5c
003cc660: b        #0x3cc5f0
003cc664: cmp      r0, #0
003cc668: bne      #0x3cc620
003cc66c: ldr      r0, [r4, #4]
003cc670: mov      r1, #0x14
003cc674: ldr      r2, [r4, #0x50]
003cc678: bl       #0x3a4d5c
003cc67c: b        #0x3cc620
003cc680: mov      r0, r4
003cc684: bl       #0x3cc484
003cc688: cmp      r0, #0
003cc68c: beq      #0x3cc644
003cc690: ldr      r3, [r4, #4]
003cc694: mov      r0, r3
003cc698: ldr      r3, [r3]
003cc69c: mov      lr, pc
003cc6a0: ldr      pc, [r3, #0x124]
003cc6a4: cmp      r0, #0
003cc6a8: beq      #0x3cc710
003cc6ac: mov      r0, r4
003cc6b0: ldr      r1, [r4, #0x50]
003cc6b4: bl       #0x3d63d8
003cc6b8: cmp      r0, #0
003cc6bc: bne      #0x3cc738
003cc6c0: mov      r0, r4
003cc6c4: ldr      r1, [r4, #0x50]
003cc6c8: bl       #0x3d6604
003cc6cc: cmp      r0, #0
003cc6d0: beq      #0x3cc724
003cc6d4: ldr      r2, [r4, #0x50]
003cc6d8: ldr      r0, [r4, #4]
003cc6dc: mov      r1, #0x17
003cc6e0: pop      {r4, r5, r6, lr}
003cc6e4: b        #0x3a4d5c
003cc6e8: ldr      r0, [r4, #4]
003cc6ec: mov      r1, #0x15
003cc6f0: ldr      r2, [r4, #0x50]
003cc6f4: bl       #0x3a4d5c
003cc6f8: b        #0x3cc620
003cc6fc: ldr      r0, [r4, #4]
003cc700: mov      r1, #0x12
003cc704: ldr      r2, [r4, #0x50]
003cc708: bl       #0x3a4d5c
003cc70c: b        #0x3cc5f0
003cc710: mov      r0, r4
003cc714: ldr      r1, [r4, #0x50]
003cc718: bl       #0x3d6188
003cc71c: cmp      r0, #0
003cc720: bne      #0x3cc74c
003cc724: ldr      r2, [r4, #0x50]
003cc728: ldr      r0, [r4, #4]
003cc72c: mov      r1, #0x16
003cc730: pop      {r4, r5, r6, lr}
003cc734: b        #0x3a4d5c
003cc738: ldr      r2, [r4, #0x50]
003cc73c: ldr      r0, [r4, #4]
003cc740: mov      r1, #0x18
003cc744: pop      {r4, r5, r6, lr}
003cc748: b        #0x3a4d5c
003cc74c: ldr      r2, [r4, #0x50]
003cc750: ldr      r0, [r4, #4]
003cc754: mov      r1, #0x19
003cc758: pop      {r4, r5, r6, lr}
003cc75c: b        #0x3a4d5c
