
# _ZN10GameObject15UpdateIdleSoundEv
0038ae2c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038ae30: ldr      r4, [pc, #0x2a0]
0038ae34: ldr      r6, [pc, #0x2a0]
0038ae38: sub      sp, sp, #0x2c
0038ae3c: add      r4, pc, r4
0038ae40: ldr      r3, [r4, r6]
0038ae44: mov      r5, r0
0038ae48: ldr      r2, [r3]
0038ae4c: cmp      r2, #0
0038ae50: beq      #0x38aea8
0038ae54: ldrb     r8, [r0, #0x373]
0038ae58: cmp      r8, #0
0038ae5c: bne      #0x38aeb0
0038ae60: ldr      r3, [pc, #0x278]
0038ae64: ldr      sl, [r4, r3]
0038ae68: mov      r0, sl
0038ae6c: bl       #0x31f594
0038ae70: cmp      r0, #0
0038ae74: beq      #0x38aea8
0038ae78: ldr      r0, [sl, #0x40]
0038ae7c: mov      r1, r8
0038ae80: mov      r2, #1
0038ae84: bl       #0x36e478
0038ae88: ldr      r3, [r0, #0x660]
0038ae8c: cmp      r3, #0
0038ae90: beq      #0x38aea8
0038ae94: mov      r0, sl
0038ae98: bl       #0x31f594
0038ae9c: ldr      r3, [r0, #0x130]
0038aea0: cmp      r3, #0x26
0038aea4: beq      #0x38aedc
0038aea8: add      sp, sp, #0x2c
0038aeac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038aeb0: ldrb     r2, [r0, #0x372]
0038aeb4: cmp      r2, #0
0038aeb8: beq      #0x38aea8
0038aebc: mov      r2, #0
0038aec0: strb     r2, [r0, #0x372]
0038aec4: mov      r2, #0x370
0038aec8: ldrsh    r1, [r0, r2]
0038aecc: ldr      r0, [r3]
0038aed0: mov      r2, #0x3e8
0038aed4: bl       #0x369fec
0038aed8: b        #0x38aea8
0038aedc: ldr      r1, [pc, #0x200]
0038aee0: ldr      r2, [pc, #0x200]
0038aee4: ldr      r0, [sl, #0x2c]
0038aee8: add      r1, pc, r1
0038aeec: add      r2, pc, r2
0038aef0: bl       #0x4c4bdc
0038aef4: bl       #0x30e964
0038aef8: ldr      r3, [pc, #0x1ec]
0038aefc: mov      r7, r0
0038af00: mov      r1, r8
0038af04: ldr      r3, [r4, r3]
0038af08: ldr      r0, [sl, #0x40]
0038af0c: mov      r2, #1
0038af10: ldr      ip, [r3, #8]
0038af14: ldr      lr, [r3]
0038af18: ldr      r3, [r3, #4]
0038af1c: str      ip, [sp, #0x24]
0038af20: str      lr, [sp, #0x1c]
0038af24: str      r3, [sp, #0x20]
0038af28: bl       #0x36e478
0038af2c: ldr      r3, [r0, #0x660]
0038af30: cmp      r3, #0
0038af34: beq      #0x38b0c8
0038af38: mov      r1, r8
0038af3c: ldr      r0, [sl, #0x40]
0038af40: mov      r2, #1
0038af44: bl       #0x36e478
0038af48: ldr      r3, [r0, #0x660]
0038af4c: ldr      r1, [r3, #0x160]
0038af50: str      r1, [sp, #0x1c]
0038af54: ldr      sl, [r3, #0x164]
0038af58: str      sl, [sp, #0x20]
0038af5c: ldr      r8, [r3, #0x168]
0038af60: str      r8, [sp, #0x24]
0038af64: ldr      r0, [r5, #0x160]
0038af68: bl       #0x30e3ac
0038af6c: mov      r1, sl
0038af70: mov      sb, r0
0038af74: ldr      r0, [r5, #0x164]
0038af78: bl       #0x30e3ac
0038af7c: mov      r1, r8
0038af80: mov      fp, r0
0038af84: ldr      r0, [r5, #0x168]
0038af88: bl       #0x30e3ac
0038af8c: mov      r1, sb
0038af90: mov      sl, r0
0038af94: mov      r0, sb
0038af98: bl       #0x30ed6c
0038af9c: mov      r1, fp
0038afa0: mov      r8, r0
0038afa4: mov      r0, fp
0038afa8: bl       #0x30ed6c
0038afac: mov      r1, r0
0038afb0: mov      r0, r8
0038afb4: bl       #0x30eba4
0038afb8: mov      r1, sl
0038afbc: mov      r8, r0
0038afc0: mov      r0, sl
0038afc4: bl       #0x30ed6c
0038afc8: mov      r1, r0
0038afcc: mov      r0, r8
0038afd0: bl       #0x30eba4
0038afd4: bl       #0x30e8a4
0038afd8: bl       #0x30e1c0
0038afdc: bl       #0x30e6a0
0038afe0: ldrb     r3, [r5, #0x372]
0038afe4: mov      r8, r0
0038afe8: cmp      r3, #0
0038afec: beq      #0x38b04c
0038aff0: mov      r0, r7
0038aff4: mov      r1, r8
0038aff8: bl       #0x30e9ac
0038affc: cmp      r0, #0
0038b000: beq      #0x38aea8
0038b004: mov      r0, r7
0038b008: mov      r1, #0
0038b00c: bl       #0x30e2f8
0038b010: cmp      r0, #0
0038b014: beq      #0x38aea8
0038b018: ldr      r3, [r4, r6]
0038b01c: mov      r2, #0
0038b020: strb     r2, [r5, #0x372]
0038b024: ldr      r0, [r3]
0038b028: mov      r3, #0x370
0038b02c: ldrsh    r1, [r5, r3]
0038b030: mov      r2, #0x3e8
0038b034: add      r3, sp, #0x1c
0038b038: str      r7, [sp]
0038b03c: bl       #0x36a218
0038b040: ldrb     r3, [r5, #0x372]
0038b044: cmp      r3, #0
0038b048: bne      #0x38aea8
0038b04c: mov      r1, r8
0038b050: mov      r0, r7
0038b054: bl       #0x30e2f8
0038b058: cmp      r0, #0
0038b05c: beq      #0x38aea8
0038b060: mov      r0, r7
0038b064: mov      r1, #0
0038b068: bl       #0x30e2f8
0038b06c: cmp      r0, #0
0038b070: beq      #0x38aea8
0038b074: ldr      r3, [r4, r6]
0038b078: mov      ip, #1
0038b07c: strb     ip, [r5, #0x372]
0038b080: ldr      r7, [r5, #0x160]
0038b084: ldr      r6, [r5, #0x164]
0038b088: ldr      r4, [r5, #0x168]
0038b08c: ldr      r0, [r3]
0038b090: mov      lr, #0xbf000000
0038b094: mov      r3, #0x370
0038b098: ldrsh    r1, [r5, r3]
0038b09c: add      lr, lr, #0x800000
0038b0a0: mov      r3, ip
0038b0a4: add      r2, sp, #0x10
0038b0a8: str      r7, [sp, #0x10]
0038b0ac: str      r6, [sp, #0x14]
0038b0b0: str      r4, [sp, #0x18]
0038b0b4: str      lr, [sp, #8]
0038b0b8: str      ip, [sp]
0038b0bc: str      lr, [sp, #4]
0038b0c0: bl       #0x36b5d8
0038b0c4: b        #0x38aea8
0038b0c8: ldr      r1, [sp, #0x1c]
0038b0cc: ldr      sl, [sp, #0x20]
0038b0d0: ldr      r8, [sp, #0x24]
0038b0d4: b        #0x38af64
0038b0d8: rsbeq    sb, r0, r4, asr ip
0038b0dc: andeq    r0, r0, r4, lsr #27
0038b0e0: strdeq   r3, r4, [r0], -r4
0038b0e4: subseq   r7, r3, r0, ror r4
0038b0e8: subseq   r7, r3, ip, ror r4
0038b0ec: andeq    r3, r0, ip, lsr #30

# _ZN12v2Controller6UpdateEv
003a2f44: bx       lr

# _ZN11AISExternal8OnUpdateEv
003dce64: push     {r4, lr}
003dce68: mov      r4, r0
003dce6c: bl       #0x3dc798
003dce70: ldr      r3, [r4, #0xb8]
003dce74: tst      r3, #1
003dce78: beq      #0x3dce8c
003dce7c: ldr      r1, [pc, #0x1c]
003dce80: mov      r0, r4
003dce84: add      r1, pc, r1
003dce88: bl       #0x37c514
003dce8c: mov      r0, r4
003dce90: bl       #0x3d8eb4
003dce94: mov      r0, r4
003dce98: pop      {r4, lr}
003dce9c: b        #0x3d8ea0
003dcea0: subeq    r8, lr, r4, lsl #25

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

# _ZN9TimerUtil6UpdateEv
00317ae4: push     {r4, r5, r6, lr}
00317ae8: ldrb     r2, [r0, #4]
00317aec: ldr      r3, [pc, #0x2c]
00317af0: mov      r4, r0
00317af4: cmp      r2, #0
00317af8: add      r3, pc, r3
00317afc: bne      #0x317b1c
00317b00: ldr      r2, [pc, #0x1c]
00317b04: ldr      r5, [r0]
00317b08: ldr      r0, [r3, r2]
00317b0c: bl       #0x31f66c
00317b10: rsb      r5, r0, r5
00317b14: bic      r5, r5, r5, asr #31
00317b18: str      r5, [r4]
00317b1c: pop      {r4, r5, r6, pc}
00317b20: mlseq    r7, r8, pc, ip
00317b24: strdeq   r3, r4, [r0], -r4

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN10CharTimers6UpdateEv
003db640: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003db644: ldr      r8, [pc, #0x23c]
003db648: ldr      r3, [pc, #0x23c]
003db64c: ldr      r1, [pc, #0x23c]
003db650: add      r8, pc, r8
003db654: ldr      r2, [r8, r3]
003db658: ldr      r3, [r8, r1]
003db65c: sub      sp, sp, #0x54
003db660: ldrb     sl, [r2, #0x30]
003db664: ldr      r3, [r3]
003db668: str      r1, [sp, #0x10]
003db66c: cmp      sl, #0
003db670: mov      r6, r0
003db674: str      r3, [sp, #0x4c]
003db678: beq      #0x3db69c
003db67c: ldr      r2, [sp, #0x10]
003db680: ldr      r3, [r8, r2]
003db684: ldr      r2, [sp, #0x4c]
003db688: ldr      r3, [r3]
003db68c: cmp      r2, r3
003db690: bne      #0x3db884
003db694: add      sp, sp, #0x54
003db698: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003db69c: ldr      r3, [pc, #0x1f0]
003db6a0: ldr      r0, [r8, r3]
003db6a4: bl       #0x31f66c
003db6a8: str      r0, [sp, #0x14]
003db6ac: ldr      r4, [r6, #8]
003db6b0: ldr      r3, [r6, #0xc]
003db6b4: rsb      r3, r4, r3
003db6b8: asrs     r3, r3, #5
003db6bc: str      r3, [sp, #8]
003db6c0: beq      #0x3db67c
003db6c4: ldr      r2, [pc, #0x1cc]
003db6c8: ldr      r3, [pc, #0x1cc]
003db6cc: mov      sb, r8
003db6d0: str      r2, [sp]
003db6d4: ldr      r2, [pc, #0x1c4]
003db6d8: add      r3, pc, r3
003db6dc: add      r3, r3, #0x13
003db6e0: add      r2, pc, r2
003db6e4: add      r2, r2, #0x13
003db6e8: str      r2, [sp, #4]
003db6ec: str      r3, [sp, #0xc]
003db6f0: b        #0x3db708
003db6f4: ldr      r1, [sp, #8]
003db6f8: add      sl, sl, #1
003db6fc: cmp      sl, r1
003db700: beq      #0x3db87c
003db704: ldr      r4, [r6, #8]
003db708: lsl      r7, sl, #5
003db70c: add      r4, r4, r7
003db710: ldrb     r3, [r4, #0x14]
003db714: cmp      r3, #0
003db718: beq      #0x3db6f4
003db71c: ldrb     r2, [r4, #0x15]
003db720: cmp      r2, #0
003db724: bne      #0x3db6f4
003db728: ldr      r2, [r4, #0x10]
003db72c: ldr      r1, [sp, #0x14]
003db730: add      r5, sp, #0x1c
003db734: add      r8, sp, #0x34
003db738: add      r2, r2, r1
003db73c: str      r2, [r4, #0x10]
003db740: cmp      r3, #0
003db744: beq      #0x3db6f4
003db748: ldr      r2, [r4, #0x10]
003db74c: ldr      r3, [r4, #0xc]
003db750: cmp      r2, r3
003db754: blo      #0x3db6f4
003db758: cmp      r3, #0
003db75c: beq      #0x3db6f4
003db760: ldr      r1, [r4, #8]
003db764: cmp      r1, #0
003db768: strbeq   r1, [r4, #0x14]
003db76c: beq      #0x3db780
003db770: rsb      r3, r3, r2
003db774: subgt    r1, r1, #1
003db778: str      r3, [r4, #0x10]
003db77c: strgt    r1, [r4, #8]
003db780: ldr      r3, [r4, #0x18]
003db784: cmn      r3, #1
003db788: beq      #0x3db7fc
003db78c: ldr      r3, [sp]
003db790: ldr      fp, [sb, r3]
003db794: mov      r0, fp
003db798: bl       #0x337888
003db79c: mov      r0, r5
003db7a0: ldr      r1, [sp, #4]
003db7a4: str      r5, [sp, #0x2c]
003db7a8: str      r5, [sp, #0x30]
003db7ac: bl       #0x3db5f0
003db7b0: mov      r0, fp
003db7b4: mov      r1, r5
003db7b8: bl       #0x337a88
003db7bc: ldr      r0, [sp, #0x30]
003db7c0: cmp      r0, r5
003db7c4: beq      #0x3db7e4
003db7c8: cmp      r0, #0
003db7cc: beq      #0x3db7e4
003db7d0: ldr      r1, [sp, #0x1c]
003db7d4: rsb      r1, r0, r1
003db7d8: cmp      r1, #0x80
003db7dc: bhi      #0x3db86c
003db7e0: bl       #0x708f00
003db7e4: ldmib    r6, {r0, r2}
003db7e8: ldr      r1, [r4, #0x18]
003db7ec: add      r2, r2, r7
003db7f0: bl       #0x3a4d5c
003db7f4: ldrb     r3, [r4, #0x14]
003db7f8: b        #0x3db740
003db7fc: ldr      r2, [sp]
003db800: ldr      fp, [sb, r2]
003db804: mov      r0, fp
003db808: bl       #0x337888
003db80c: mov      r0, r8
003db810: ldr      r1, [sp, #0xc]
003db814: str      r8, [sp, #0x44]
003db818: str      r8, [sp, #0x48]
003db81c: bl       #0x3db5f0
003db820: mov      r0, fp
003db824: mov      r1, r8
003db828: bl       #0x337a88
003db82c: ldr      r0, [sp, #0x48]
003db830: cmp      r0, r8
003db834: beq      #0x3db854
003db838: cmp      r0, #0
003db83c: beq      #0x3db854
003db840: ldr      r1, [sp, #0x34]
003db844: rsb      r1, r0, r1
003db848: cmp      r1, #0x80
003db84c: bhi      #0x3db874
003db850: bl       #0x708f00
003db854: ldmib    r6, {r0, r2}
003db858: mov      r1, #0x29
003db85c: add      r2, r2, r7
003db860: bl       #0x3a4d5c
003db864: ldrb     r3, [r4, #0x14]
003db868: b        #0x3db740
003db86c: bl       #0x310440
003db870: b        #0x3db7e4
003db874: bl       #0x310440
003db878: b        #0x3db854
003db87c: mov      r8, sb
003db880: b        #0x3db67c
003db884: bl       #0x30e310
003db888: subseq   sb, fp, r0, asr #8
003db88c: andeq    r1, r0, r0, lsr #20
003db890: andeq    r4, r0, ip, lsr #1
003db894: strdeq   r3, r4, [r0], -r4
003db898: andeq    r0, r0, r4, lsl #17
003db89c: subeq    sl, lr, r0, ror r2
003db8a0: subeq    sl, lr, r8, ror #4

# _ZN10GameObject19RequireOnlineUpdateEv
0038b8b8: push     {r4, lr}
0038b8bc: mov      r4, r0
0038b8c0: bl       #0x7fd794
0038b8c4: ldrb     r3, [r0, #5]
0038b8c8: cmp      r3, #0
0038b8cc: beq      #0x38b8f4
0038b8d0: ldr      r3, [r4, #0x100]
0038b8d4: cmp      r3, #0
0038b8d8: beq      #0x38b8f4
0038b8dc: bl       #0x7fd794
0038b8e0: bl       #0x7fd5b4
0038b8e4: cmp      r0, #0
0038b8e8: beq      #0x38b8f8
0038b8ec: mov      r3, #1
0038b8f0: strb     r3, [r4, #0x119]
0038b8f4: pop      {r4, pc}
0038b8f8: ldr      r3, [r4]
0038b8fc: mov      r0, r4
0038b900: mov      lr, pc
0038b904: ldr      pc, [r3, #0x54]
0038b908: cmp      r0, #0
0038b90c: bne      #0x38b8f4
0038b910: b        #0x38b8ec

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

# _ZN9Character9CanUpdateEv
003a52a4: push     {r4, r5, r6, lr}
003a52a8: ldr      r6, [r0, #0x2d8]
003a52ac: mov      r4, r0
003a52b0: ldr      r5, [pc, #0x120]
003a52b4: cmp      r6, #0
003a52b8: ldrne    r3, [r6, #8]
003a52bc: movne    r2, #0
003a52c0: add      r5, pc, r5
003a52c4: strbne   r2, [r3, #0x200]
003a52c8: bl       #0x7fd794
003a52cc: ldrb     r3, [r0, #5]
003a52d0: cmp      r3, #0
003a52d4: bne      #0x3a539c
003a52d8: cmp      r6, #0
003a52dc: beq      #0x3a5354
003a52e0: ldr      r3, [pc, #0xf4]
003a52e4: mov      r1, #0
003a52e8: mov      r2, #1
003a52ec: ldr      r3, [r5, r3]
003a52f0: ldr      r5, [r4, #0x418]
003a52f4: ldr      r0, [r3, #0x40]
003a52f8: bl       #0x36e478
003a52fc: ldr      r3, [r0, #0x660]
003a5300: cmp      r5, r3
003a5304: beq      #0x3a5354
003a5308: ldr      r3, [r4, #0x2d8]
003a530c: ldr      r3, [r3, #8]
003a5310: ldr      r3, [r3, #0x118]
003a5314: cmp      r3, #0
003a5318: bne      #0x3a5328
003a531c: ldrb     r3, [r4, #0x2fc]
003a5320: cmp      r3, #0
003a5324: beq      #0x3a5354
003a5328: mov      r0, r4
003a532c: add      r1, r4, #0x12c
003a5330: bl       #0x33de90
003a5334: cmp      r0, #0
003a5338: bne      #0x3a5354
003a533c: mov      r3, #0x1480
003a5340: ldrb     r3, [r4, r3]
003a5344: cmp      r3, #0
003a5348: bne      #0x3a5354
003a534c: mov      r0, #0
003a5350: pop      {r4, r5, r6, pc}
003a5354: ldr      r3, [r4]
003a5358: mov      r0, r4
003a535c: mov      lr, pc
003a5360: ldr      pc, [r3, #0x34]
003a5364: cmp      r0, #0
003a5368: bne      #0x3a53b8
003a536c: cmp      r6, #0
003a5370: beq      #0x3a5394
003a5374: ldrb     r3, [r4, #0x80]
003a5378: cmp      r3, #0
003a537c: beq      #0x3a5394
003a5380: ldr      r2, [r6, #8]
003a5384: mov      r3, #1
003a5388: mov      r0, r3
003a538c: strb     r3, [r2, #0x200]
003a5390: pop      {r4, r5, r6, pc}
003a5394: mov      r0, #1
003a5398: pop      {r4, r5, r6, pc}
003a539c: ldr      r3, [r4]
003a53a0: mov      r0, r4
003a53a4: mov      lr, pc
003a53a8: ldr      pc, [r3, #0x54]
003a53ac: cmp      r0, #0
003a53b0: bne      #0x3a536c
003a53b4: b        #0x3a52d8
003a53b8: ldrb     r3, [r4, #0x80]
003a53bc: cmp      r3, #0
003a53c0: bne      #0x3a536c
003a53c4: mov      r0, r4
003a53c8: bl       #0x3a5248
003a53cc: cmp      r0, #0
003a53d0: beq      #0x3a534c
003a53d4: b        #0x3a536c
003a53d8: ldrsbeq  pc, [lr], #-0x70
003a53dc: strdeq   r3, r4, [r0], -r4
