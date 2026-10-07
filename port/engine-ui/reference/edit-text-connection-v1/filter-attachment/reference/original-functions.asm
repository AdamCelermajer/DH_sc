# _ZN7gameswf14place_object_27executeEPNS_9characterE
0075d8d8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0075d8dc: ldrb     r3, [r0, #9]
0075d8e0: sub      sp, sp, #0x44
0075d8e4: mov      r5, r0
0075d8e8: cmp      r3, #1
0075d8ec: mov      r6, r1
0075d8f0: beq      #0x75d9f4
0075d8f4: blo      #0x75d908
0075d8f8: cmp      r3, #2
0075d8fc: beq      #0x75da6c
0075d900: add      sp, sp, #0x44
0075d904: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0075d908: ldrb     r3, [r0, #4]
0075d90c: ldr      r1, [r1]
0075d910: ldrh     r2, [r0, #0xe]
0075d914: cmp      r3, #0
0075d918: ldr      ip, [r1, #0xb4]
0075d91c: bne      #0x75db1c
0075d920: ldr      r7, [pc, #0x2b4]
0075d924: ldrb     sl, [r5, #8]
0075d928: add      r7, pc, r7
0075d92c: cmp      sl, #0
0075d930: add      r7, r7, #0xc
0075d934: bne      #0x75db34
0075d938: strb     sl, [sp, #0x3c]
0075d93c: str      sl, [sp, #0x30]
0075d940: str      sl, [sp, #0x34]
0075d944: str      sl, [sp, #0x38]
0075d948: add      r4, sp, #0x30
0075d94c: ldrb     sl, [r5, #5]
0075d950: ldrh     sb, [r5, #0x12]
0075d954: ldrb     r8, [r5, #6]
0075d958: ldrb     r3, [r5, #7]
0075d95c: ldrh     r0, [r5, #0xa]
0075d960: subs     sb, sb, #4
0075d964: movne    sb, #1
0075d968: cmp      sl, #0
0075d96c: addne    sl, r5, sl
0075d970: cmp      r8, #0
0075d974: addne    r8, r5, r8
0075d978: cmp      r3, #0
0075d97c: addne    r3, r5, r3
0075d980: cmp      r0, #0
0075d984: ldrh     fp, [r5, #0xc]
0075d988: moveq    r0, #0
0075d98c: beq      #0x75d9b8
0075d990: str      r2, [sp, #0x28]
0075d994: str      r3, [sp, #0x2c]
0075d998: str      ip, [sp, #0x24]
0075d99c: bl       #0x30e2e0
0075d9a0: movw     r1, #0xff00
0075d9a4: movt     r1, #0x477f
0075d9a8: bl       #0x30ec94
0075d9ac: ldr      ip, [sp, #0x24]
0075d9b0: ldr      r3, [sp, #0x2c]
0075d9b4: ldr      r2, [sp, #0x28]
0075d9b8: ldrh     r1, [r5, #0x10]
0075d9bc: str      r3, [sp, #0x10]
0075d9c0: str      r0, [sp, #0x14]
0075d9c4: str      r1, [sp, #0x18]
0075d9c8: str      fp, [sp]
0075d9cc: mov      r1, r2
0075d9d0: stmib    sp, {sb, sl}
0075d9d4: str      r8, [sp, #0xc]
0075d9d8: mov      r0, r6
0075d9dc: mov      r2, r7
0075d9e0: mov      r3, r4
0075d9e4: blx      ip
0075d9e8: mov      r0, r4
0075d9ec: bl       #0x75d89c
0075d9f0: b        #0x75d900
0075d9f4: ldrb     r8, [r0, #5]
0075d9f8: ldrb     r7, [r0, #6]
0075d9fc: ldrb     sb, [r0, #7]
0075da00: cmp      r8, #0
0075da04: addne    r8, r0, r8
0075da08: cmp      r7, #0
0075da0c: addne    r7, r0, r7
0075da10: cmp      sb, #0
0075da14: addne    sb, r0, sb
0075da18: ldrh     sl, [r0, #0xc]
0075da1c: ldrh     r0, [r0, #0xa]
0075da20: ldr      r3, [r1]
0075da24: cmp      r0, #0
0075da28: ldr      r4, [r3, #0xb8]
0075da2c: moveq    r0, #0
0075da30: beq      #0x75da44
0075da34: bl       #0x30e2e0
0075da38: movw     r1, #0xff00
0075da3c: movt     r1, #0x477f
0075da40: bl       #0x30ec94
0075da44: ldrh     r3, [r5, #0x10]
0075da48: mov      r1, sl
0075da4c: str      r0, [sp, #4]
0075da50: str      r3, [sp, #8]
0075da54: str      sb, [sp]
0075da58: mov      r0, r6
0075da5c: mov      r2, r8
0075da60: mov      r3, r7
0075da64: blx      r4
0075da68: b        #0x75d900
0075da6c: ldrb     r3, [r0, #4]
0075da70: ldr      r2, [r1]
0075da74: ldrh     r8, [r0, #0xe]
0075da78: cmp      r3, #0
0075da7c: ldr      ip, [r2, #0xbc]
0075da80: bne      #0x75db64
0075da84: ldr      r7, [pc, #0x154]
0075da88: add      r7, pc, r7
0075da8c: add      r7, r7, #0xc
0075da90: ldrsb    r3, [r7]
0075da94: ldrb     fp, [r5, #5]
0075da98: ldrb     sb, [r5, #6]
0075da9c: cmn      r3, #1
0075daa0: ldrb     sl, [r5, #7]
0075daa4: ldrh     r0, [r5, #0xa]
0075daa8: addne    r7, r7, #1
0075daac: ldreq    r7, [r7, #0xc]
0075dab0: cmp      fp, #0
0075dab4: addne    fp, r5, fp
0075dab8: cmp      sb, #0
0075dabc: addne    sb, r5, sb
0075dac0: cmp      sl, #0
0075dac4: addne    sl, r5, sl
0075dac8: cmp      r0, #0
0075dacc: ldrh     r4, [r5, #0xc]
0075dad0: moveq    r0, #0
0075dad4: beq      #0x75daf0
0075dad8: str      ip, [sp, #0x24]
0075dadc: bl       #0x30e2e0
0075dae0: movw     r1, #0xff00
0075dae4: movt     r1, #0x477f
0075dae8: bl       #0x30ec94
0075daec: ldr      ip, [sp, #0x24]
0075daf0: ldrh     r3, [r5, #0x10]
0075daf4: mov      r1, r8
0075daf8: str      r0, [sp, #0xc]
0075dafc: str      r3, [sp, #0x10]
0075db00: str      fp, [sp]
0075db04: stmib    sp, {sb, sl}
0075db08: mov      r0, r6
0075db0c: mov      r2, r7
0075db10: mov      r3, r4
0075db14: blx      ip
0075db18: b        #0x75d900
0075db1c: ldr      r7, [r0, r3]
0075db20: cmp      r7, #0
0075db24: beq      #0x75d920
0075db28: ldrb     sl, [r5, #8]
0075db2c: cmp      sl, #0
0075db30: beq      #0x75d938
0075db34: add      sl, r5, sl
0075db38: ldr      r8, [sl, #4]
0075db3c: mov      sb, #0
0075db40: str      sb, [sp, #0x30]
0075db44: cmp      r8, sb
0075db48: str      sb, [sp, #0x34]
0075db4c: str      sb, [sp, #0x38]
0075db50: strb     sb, [sp, #0x3c]
0075db54: bge      #0x75db74
0075db58: str      r8, [sp, #0x34]
0075db5c: add      r4, sp, #0x30
0075db60: b        #0x75d94c
0075db64: ldr      r7, [r0, r3]
0075db68: cmp      r7, #0
0075db6c: bne      #0x75da90
0075db70: b        #0x75da84
0075db74: beq      #0x75db58
0075db78: ble      #0x75db58
0075db7c: add      r4, sp, #0x30
0075db80: mov      r0, r4
0075db84: add      r1, r8, r8, asr #1
0075db88: str      r2, [sp, #0x28]
0075db8c: str      ip, [sp, #0x24]
0075db90: bl       #0x75a298
0075db94: ldr      ip, [sp, #0x24]
0075db98: ldr      r2, [sp, #0x28]
0075db9c: mov      r3, sb
0075dba0: ldr      r1, [sp, #0x30]
0075dba4: str      r3, [r1, sb, lsl #2]
0075dba8: add      sb, sb, #1
0075dbac: cmp      sb, r8
0075dbb0: bne      #0x75dba0
0075dbb4: str      sb, [sp, #0x34]
0075dbb8: ldr      r1, [sl]
0075dbbc: ldr      r0, [r1, r3, lsl #2]
0075dbc0: ldr      r1, [sp, #0x30]
0075dbc4: str      r0, [r1, r3, lsl #2]
0075dbc8: ldr      r1, [sp, #0x34]
0075dbcc: add      r3, r3, #1
0075dbd0: cmp      r3, r1
0075dbd4: blt      #0x75dbb8
0075dbd8: b        #0x75d94c
0075dbdc: mlaeq    sb, r0, fp, lr
0075dbe0: eoreq    lr, sb, r0, lsr sl

# _ZN7gameswf14place_object_221execute_state_reverseEPNS_9characterEi
0075a648: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0075a64c: mov      r4, r0
0075a650: ldrb     r0, [r0, #9]
0075a654: ldr      r3, [pc, #0x134]
0075a658: sub      sp, sp, #0x10
0075a65c: cmp      r0, #1
0075a660: mov      r5, r1
0075a664: add      r3, pc, r3
0075a668: mov      r6, r2
0075a66c: beq      #0x75a6ac
0075a670: blo      #0x75a684
0075a674: cmp      r0, #2
0075a678: beq      #0x75a724
0075a67c: add      sp, sp, #0x10
0075a680: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0075a684: ldrh     r2, [r4, #0x12]
0075a688: ldr      r3, [r1]
0075a68c: mov      r0, r5
0075a690: cmp      r2, #4
0075a694: ldrh     r1, [r4, #0xc]
0075a698: ldr      r3, [r3, #0xc4]
0075a69c: mvnne    r2, #0
0075a6a0: ldrheq   r2, [r4, #0xe]
0075a6a4: blx      r3
0075a6a8: b        #0x75a67c
0075a6ac: ldr      r8, [r4, #0x14]
0075a6b0: ldr      r2, [r1]
0075a6b4: ldrh     sl, [r4, #0xc]
0075a6b8: cmp      r8, #0
0075a6bc: ldr      r6, [r2, #0xb8]
0075a6c0: beq      #0x75a784
0075a6c4: ldr      r7, [r4, #0x18]
0075a6c8: cmp      r7, #0
0075a6cc: beq      #0x75a778
0075a6d0: ldrb     sb, [r4, #7]
0075a6d4: ldrh     r0, [r4, #0xa]
0075a6d8: cmp      sb, #0
0075a6dc: addne    sb, r4, sb
0075a6e0: cmp      r0, #0
0075a6e4: moveq    r0, #0
0075a6e8: beq      #0x75a6fc
0075a6ec: bl       #0x30e2e0
0075a6f0: movw     r1, #0xff00
0075a6f4: movt     r1, #0x477f
0075a6f8: bl       #0x30ec94
0075a6fc: ldrh     r3, [r4, #0x10]
0075a700: mov      r1, sl
0075a704: str      r0, [sp, #4]
0075a708: str      r3, [sp, #8]
0075a70c: str      sb, [sp]
0075a710: mov      r0, r5
0075a714: mov      r2, r8
0075a718: mov      r3, r7
0075a71c: blx      r6
0075a720: b        #0x75a67c
0075a724: ldr      ip, [r1]
0075a728: mov      r0, r1
0075a72c: mvn      r3, #0
0075a730: mov      r1, r2
0075a734: ldrh     r2, [r4, #0xc]
0075a738: mov      lr, pc
0075a73c: ldr      pc, [ip, #0xa0]
0075a740: subs     r3, r0, #0
0075a744: beq      #0x75a75c
0075a748: ldr      r3, [r3]
0075a74c: mov      r1, r5
0075a750: mov      lr, pc
0075a754: ldr      pc, [r3, #0xc]
0075a758: b        #0x75a67c
0075a75c: ldr      r0, [pc, #0x30]
0075a760: ldrh     r2, [r4, #0xc]
0075a764: mov      r1, r6
0075a768: add      r0, pc, r0
0075a76c: add      sp, sp, #0x10
0075a770: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0075a774: b        #0x761184
0075a778: ldr      r2, [pc, #0x18]
0075a77c: ldr      r7, [r3, r2]
0075a780: b        #0x75a6d0
0075a784: ldr      r2, [pc, #0x10]
0075a788: ldr      r8, [r3, r2]
0075a78c: b        #0x75a6c4
0075a790: eoreq    sl, r3, ip, lsr #8
0075a794: ldrheq   lr, [sl], -r8
0075a798: andeq    r4, r0, r8, asr #1
0075a79c: andeq    r3, r0, r4, lsl #9

# _ZN7gameswf9character10set_effectERKNS_6effectE
00755cf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00755cfc: ldr      r6, [r0, #0x54]
00755d00: mov      sl, r0
00755d04: mov      r8, r1
00755d08: cmp      r6, #0
00755d0c: beq      #0x755d84
00755d10: ldr      r3, [r8]
00755d14: add      r0, r6, #0x3c
00755d18: str      r3, [r6, #0x38]
00755d1c: ldr      r1, [r8, #8]
00755d20: bl       #0x755ad8
00755d24: ldr      r3, [r6, #0x40]
00755d28: cmp      r3, #0
00755d2c: ble      #0x755d74
00755d30: mov      r5, #0
00755d34: mov      r7, r5
00755d38: ldr      ip, [r6, #0x3c]
00755d3c: ldr      r4, [r8, #4]
00755d40: add      r7, r7, #1
00755d44: add      ip, ip, r5
00755d48: add      r4, r4, r5
00755d4c: ldm      r4!, {r0, r1, r2, r3}
00755d50: stm      ip!, {r0, r1, r2, r3}
00755d54: ldm      r4!, {r0, r1, r2, r3}
00755d58: stm      ip!, {r0, r1, r2, r3}
00755d5c: ldm      r4, {r0, r1, r2}
00755d60: stm      ip, {r0, r1, r2}
00755d64: ldr      r3, [r6, #0x40]
00755d68: add      r5, r5, #0x2c
00755d6c: cmp      r7, r3
00755d70: blt      #0x755d38
00755d74: ldr      r3, [sl, #0x54]
00755d78: add      r3, r3, #0x38
00755d7c: str      r3, [sl, #0x50]
00755d80: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00755d84: mov      r1, r6
00755d88: mov      r0, #0x6c
00755d8c: bl       #0x752ba8
00755d90: mov      r6, r0
00755d94: bl       #0x412160
00755d98: str      r6, [sl, #0x54]
00755d9c: b        #0x755d10

# _ZN7gameswf12display_list18add_display_objectEPNS_9characterEibPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
00756588: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0075658c: sub      sp, sp, #0x1c
00756590: mov      r7, r2
00756594: mov      r4, r1
00756598: mov      r1, r2
0075659c: ldrh     r2, [sp, #0x50]
007565a0: mov      r8, r3
007565a4: mov      r5, r0
007565a8: str      r2, [sp]
007565ac: ldr      r3, [r0, #4]
007565b0: ldr      sb, [sp, #0x40]
007565b4: ldr      sl, [sp, #0x44]
007565b8: str      r3, [sp, #4]
007565bc: bl       #0x754e80
007565c0: ldr      r6, [pc, #0x1f4]
007565c4: cmp      r8, #0
007565c8: mov      fp, r0
007565cc: add      r6, pc, r6
007565d0: beq      #0x7565f4
007565d4: ldr      r1, [sp, #4]
007565d8: cmp      r0, r1
007565dc: movge    r3, #0
007565e0: movlt    r3, #1
007565e4: cmp      r0, #0
007565e8: movlt    r3, #0
007565ec: cmp      r3, #0
007565f0: bne      #0x756738
007565f4: mov      r3, #0
007565f8: add      r8, sp, #0x18
007565fc: str      r3, [r8, #-4]!
00756600: uxth     r7, r7
00756604: strh     r7, [r4, #0x94]
00756608: mov      r0, r8
0075660c: mov      r1, r4
00756610: bl       #0x75518c
00756614: ldr      r3, [sp, #0x14]
00756618: cmp      sb, #0
0075661c: strh     r7, [r3, #0x94]
00756620: ldr      r3, [sp, #0x14]
00756624: beq      #0x75675c
00756628: ldr      r2, [r3, #0x48]
0075662c: cmp      sb, r2
00756630: movne    r2, #1
00756634: strbne   r2, [r3, #0x9a]
00756638: strne    sb, [r3, #0x48]
0075663c: ldrne    r3, [sp, #0x14]
00756640: cmp      sl, #0
00756644: beq      #0x756784
00756648: ldr      r2, [r3, #0x4c]
0075664c: cmp      sl, r2
00756650: movne    r2, #1
00756654: strbne   r2, [r3, #0x99]
00756658: strne    sl, [r3, #0x4c]
0075665c: ldrne    r3, [sp, #0x14]
00756660: ldr      r2, [sp, #0x4c]
00756664: str      r2, [r3, #0x90]
00756668: ldr      r2, [sp, #0x48]
0075666c: ldr      r3, [sp, #0x14]
00756670: ldr      r1, [sp]
00756674: cmp      r2, #0
00756678: strh     r1, [r3, #0x96]
0075667c: ldr      r3, [sp, #0x14]
00756680: beq      #0x756790
00756684: ldr      r2, [r3, #0x50]
00756688: ldr      r1, [sp, #0x48]
0075668c: mov      r0, r5
00756690: cmp      r1, r2
00756694: strne    r1, [r3, #0x50]
00756698: mov      r2, r8
0075669c: mov      r1, fp
007566a0: bl       #0x7556b8
007566a4: ldr      r3, [sp, #0x14]
007566a8: ldr      r3, [r3, #0x44]
007566ac: str      r3, [sp, #0x10]
007566b0: ldrsb    r2, [r3]
007566b4: cmn      r2, #1
007566b8: ldreq    r2, [r3, #4]
007566bc: sub      r2, r2, #1
007566c0: cmp      r2, #0
007566c4: ble      #0x7566fc
007566c8: add      r7, r5, #0x10
007566cc: add      r6, sp, #0x10
007566d0: mov      r0, r7
007566d4: mov      r1, r6
007566d8: bl       #0x755da0
007566dc: cmp      r0, #0
007566e0: blt      #0x7567a0
007566e4: ldr      r3, [r5, #0x10]
007566e8: cmp      r3, #0
007566ec: beq      #0x7567a0
007566f0: ldr      r3, [r3, #4]
007566f4: cmp      r0, r3
007566f8: bgt      #0x7567a0
007566fc: mov      r1, #0
00756700: mov      r2, r1
00756704: ldr      r3, [r4]
00756708: mov      r0, r4
0075670c: mov      lr, pc
00756710: ldr      pc, [r3, #0xc8]
00756714: mov      r0, r5
00756718: mov      r1, r4
0075671c: bl       #0x755bb0
00756720: ldr      r0, [sp, #0x14]
00756724: cmp      r0, #0
00756728: beq      #0x756730
0075672c: bl       #0x75a240
00756730: add      sp, sp, #0x1c
00756734: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00756738: ldr      r3, [r5]
0075673c: ldr      r3, [r3, r0, lsl #2]
00756740: ldrh     r3, [r3, #0x94]
00756744: cmp      r7, r3
00756748: bne      #0x7565f4
0075674c: mov      r0, r5
00756750: mov      r1, fp
00756754: bl       #0x7562e8
00756758: b        #0x7565f4
0075675c: ldr      r2, [pc, #0x5c]
00756760: ldr      sb, [r6, r2]
00756764: ldr      r2, [r3, #0x48]
00756768: cmp      sb, r2
0075676c: movne    r2, #1
00756770: strbne   r2, [r3, #0x9a]
00756774: strne    sb, [r3, #0x48]
00756778: ldrne    r3, [sp, #0x14]
0075677c: cmp      sl, #0
00756780: bne      #0x756648
00756784: ldr      r2, [pc, #0x38]
00756788: ldr      sl, [r6, r2]
0075678c: b        #0x756648
00756790: ldr      r2, [pc, #0x30]
00756794: ldr      r2, [r6, r2]
00756798: str      r2, [sp, #0x48]
0075679c: b        #0x756684
007567a0: ldr      r3, [sp, #0x14]
007567a4: add      r2, sp, #0x18
007567a8: mov      r0, r7
007567ac: str      r3, [r2, #-0xc]!
007567b0: mov      r1, r6
007567b4: bl       #0x756120
007567b8: b        #0x7566fc
007567bc: eoreq    lr, r3, r4, asr #9
007567c0: andeq    r3, r0, r4, lsl #9
007567c4: andeq    r4, r0, r8, asr #1
007567c8: andeq    r3, r0, r4, ror #27

# _ZN7gameswf12display_list19move_display_objectEiPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
00755c0c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00755c10: ldr      r4, [r0, #4]
00755c14: mov      r5, r0
00755c18: mov      r6, r2
00755c1c: cmp      r4, #0
00755c20: mov      sl, r3
00755c24: mov      r7, r1
00755c28: ldr      r8, [sp, #0x20]
00755c2c: ldr      sb, [sp, #0x24]
00755c30: ble      #0x755ce0
00755c34: bl       #0x754e80
00755c38: cmp      r0, r4
00755c3c: movlt    r4, #0
00755c40: movge    r4, #1
00755c44: orrs     r4, r4, r0, lsr #31
00755c48: bne      #0x755cdc
00755c4c: ldr      r3, [r5]
00755c50: ldr      r4, [r3, r0, lsl #2]
00755c54: ldrh     r3, [r4, #0x94]
00755c58: cmp      r7, r3
00755c5c: beq      #0x755c74
00755c60: ldr      r0, [pc, #0x88]
00755c64: mov      r1, r7
00755c68: add      r0, pc, r0
00755c6c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00755c70: b        #0x761184
00755c74: ldr      r3, [r4]
00755c78: mov      r0, r4
00755c7c: mov      lr, pc
00755c80: ldr      pc, [r3, #0x150]
00755c84: cmp      r0, #0
00755c88: beq      #0x755cdc
00755c8c: cmp      r6, #0
00755c90: beq      #0x755ca8
00755c94: ldr      r3, [r4, #0x48]
00755c98: cmp      r6, r3
00755c9c: movne    r3, #1
00755ca0: strne    r6, [r4, #0x48]
00755ca4: strbne   r3, [r4, #0x9a]
00755ca8: cmp      sl, #0
00755cac: beq      #0x755cc4
00755cb0: ldr      r3, [r4, #0x4c]
00755cb4: cmp      sl, r3
00755cb8: movne    r3, #1
00755cbc: strne    sl, [r4, #0x4c]
00755cc0: strbne   r3, [r4, #0x99]
00755cc4: cmp      r8, #0
00755cc8: beq      #0x755cd8
00755ccc: ldr      r3, [r4, #0x50]
00755cd0: cmp      r8, r3
00755cd4: strne    r8, [r4, #0x50]
00755cd8: str      sb, [r4, #0x90]
00755cdc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00755ce0: ldr      r0, [pc, #0xc]
00755ce4: add      r0, pc, r0
00755ce8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00755cec: b        #0x761184
00755cf0: andseq   r2, fp, r8, lsr #25
00755cf4: andseq   r2, fp, ip, ror #23

# _ZN7gameswf12display_list22replace_display_objectEPNS_9characterEiPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
007567cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007567d0: sub      sp, sp, #0x7c
007567d4: mov      sl, r2
007567d8: mov      sb, r1
007567dc: mov      r1, r2
007567e0: ldrh     r2, [sp, #0xac]
007567e4: ldr      r4, [r0, #4]
007567e8: mov      r8, r0
007567ec: mov      fp, r3
007567f0: str      r2, [sp, #0x24]
007567f4: bl       #0x754e80
007567f8: lsr      r6, r0, #0x1f
007567fc: cmp      r0, r4
00756800: orrge    r6, r6, #1
00756804: cmp      r6, #0
00756808: bne      #0x75698c
0075680c: ldr      r3, [r8]
00756810: add      r1, sp, #0x4c
00756814: add      r2, sp, #0x2c
00756818: str      r1, [sp, #0x18]
0075681c: str      r2, [sp, #0x1c]
00756820: ldr      r5, [r3, r0, lsl #2]
00756824: add      r3, sp, #0x64
00756828: str      r3, [sp, #0x20]
0075682c: ldr      ip, [r5, #0x4c]
00756830: ldr      lr, [sp, #0x18]
00756834: ldm      ip!, {r0, r1, r2, r3}
00756838: stm      lr!, {r0, r1, r2, r3}
0075683c: ldm      ip, {r0, r1}
00756840: ldr      ip, [sp, #0x1c]
00756844: stm      lr, {r0, r1}
00756848: ldr      lr, [r5, #0x48]
0075684c: ldm      lr!, {r0, r1, r2, r3}
00756850: stm      ip!, {r0, r1, r2, r3}
00756854: ldm      lr, {r0, r1, r2, r3}
00756858: stm      ip, {r0, r1, r2, r3}
0075685c: ldr      r4, [r5, #0x50]
00756860: ldr      r1, [sp, #0x20]
00756864: ldr      r3, [r4]
00756868: str      r6, [sp, #0x68]
0075686c: str      r6, [sp, #0x6c]
00756870: str      r3, [sp, #0x64]
00756874: str      r6, [sp, #0x70]
00756878: strb     r6, [sp, #0x74]
0075687c: add      r0, r1, #4
00756880: ldr      r1, [r4, #8]
00756884: bl       #0x755ad8
00756888: ldr      r3, [sp, #0x6c]
0075688c: cmp      r3, #0
00756890: ble      #0x7568d4
00756894: mov      r7, r6
00756898: ldr      lr, [r4, #4]
0075689c: ldr      ip, [sp, #0x68]
007568a0: add      r7, r7, #1
007568a4: add      lr, lr, r6
007568a8: add      ip, ip, r6
007568ac: ldm      lr!, {r0, r1, r2, r3}
007568b0: stm      ip!, {r0, r1, r2, r3}
007568b4: ldm      lr!, {r0, r1, r2, r3}
007568b8: stm      ip!, {r0, r1, r2, r3}
007568bc: ldm      lr, {r0, r1, r2}
007568c0: stm      ip, {r0, r1, r2}
007568c4: ldr      r3, [sp, #0x6c]
007568c8: add      r6, r6, #0x2c
007568cc: cmp      r7, r3
007568d0: blt      #0x756898
007568d4: ldr      r3, [r5, #0x54]
007568d8: cmp      r3, #0
007568dc: beq      #0x7569e0
007568e0: ldr      r4, [r5, #0x4c]
007568e4: ldr      r7, [r5, #0x48]
007568e8: ldr      ip, [sp, #0x18]
007568ec: add      r2, r3, #0x20
007568f0: ldr      r6, [r5, #0x50]
007568f4: cmp      r4, r2
007568f8: ldr      lr, [sp, #0x20]
007568fc: moveq    r4, ip
00756900: cmp      r7, r3
00756904: add      r3, r3, #0x38
00756908: ldreq    r7, [sp, #0x1c]
0075690c: cmp      r6, r3
00756910: moveq    r6, lr
00756914: ldr      ip, [sp, #0xa8]
00756918: ldr      lr, [sp, #0x24]
0075691c: mov      r0, r8
00756920: str      ip, [sp, #0xc]
00756924: str      lr, [sp, #0x10]
00756928: ldr      ip, [sp, #0xa0]
0075692c: ldr      lr, [sp, #0xa4]
00756930: mov      r2, sl
00756934: mov      r1, sb
00756938: mov      r3, #1
0075693c: stm      sp, {fp, ip, lr}
00756940: bl       #0x756588
00756944: cmp      fp, #0
00756948: beq      #0x7569f0
0075694c: ldr      r2, [sp, #0xa0]
00756950: cmp      r2, #0
00756954: beq      #0x756a14
00756958: ldr      ip, [sp, #0xa4]
0075695c: cmp      ip, #0
00756960: beq      #0x7569c4
00756964: ldr      r1, [sp, #0x20]
00756968: add      r4, r1, #4
0075696c: mov      r0, r4
00756970: mov      r1, #0
00756974: bl       #0x755ad8
00756978: mov      r0, r4
0075697c: mov      r1, #0
00756980: bl       #0x752ec8
00756984: add      sp, sp, #0x7c
00756988: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0075698c: ldr      ip, [sp, #0xa0]
00756990: ldr      lr, [sp, #0xa4]
00756994: mov      r0, r8
00756998: stmib    sp, {ip, lr}
0075699c: ldr      ip, [sp, #0xa8]
007569a0: ldr      lr, [sp, #0x24]
007569a4: mov      r1, sb
007569a8: mov      r2, sl
007569ac: mov      r3, #1
007569b0: str      fp, [sp]
007569b4: str      ip, [sp, #0xc]
007569b8: str      lr, [sp, #0x10]
007569bc: bl       #0x756588
007569c0: b        #0x756984
007569c4: ldr      lr, [sp, #0x20]
007569c8: cmp      r6, lr
007569cc: beq      #0x756a38
007569d0: ldr      r3, [sb, #0x50]
007569d4: cmp      r6, r3
007569d8: strne    r6, [sb, #0x50]
007569dc: b        #0x756964
007569e0: ldr      r6, [r5, #0x50]
007569e4: ldr      r4, [r5, #0x4c]
007569e8: ldr      r7, [r5, #0x48]
007569ec: b        #0x756914
007569f0: ldr      r1, [sp, #0x1c]
007569f4: cmp      r7, r1
007569f8: beq      #0x756a58
007569fc: ldr      r3, [sb, #0x48]
00756a00: cmp      r7, r3
00756a04: movne    r3, #1
00756a08: strne    r7, [sb, #0x48]
00756a0c: strbne   r3, [sb, #0x9a]
00756a10: b        #0x75694c
00756a14: ldr      r3, [sp, #0x18]
00756a18: cmp      r4, r3
00756a1c: beq      #0x756a48
00756a20: ldr      r3, [sb, #0x4c]
00756a24: cmp      r4, r3
00756a28: movne    r3, #1
00756a2c: strne    r4, [sb, #0x4c]
00756a30: strbne   r3, [sb, #0x99]
00756a34: b        #0x756958
00756a38: mov      r0, sb
00756a3c: mov      r1, lr
00756a40: bl       #0x755cf8
00756a44: b        #0x756964
00756a48: mov      r1, r4
00756a4c: mov      r0, sb
00756a50: bl       #0x4121f8
00756a54: b        #0x756958
00756a58: mov      r1, r7
00756a5c: mov      r0, sb
00756a60: bl       #0x75355c
00756a64: b        #0x75694c

# _ZN7gameswf15sprite_instance18add_display_objectEtRKNS_9tu_stringERKNS_5arrayIPNS_9swf_eventEEEibPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
0077ed84: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0077ed88: sub      sp, sp, #0x2c
0077ed8c: ldr      ip, [r0, #0xa0]
0077ed90: mov      r6, r2
0077ed94: ldrh     r2, [sp, #0x68]
0077ed98: mov      r5, r0
0077ed9c: mov      r7, r3
0077eda0: mov      r0, ip
0077eda4: ldr      r3, [ip]
0077eda8: str      r2, [sp, #0x1c]
0077edac: mov      fp, r1
0077edb0: ldr      sl, [sp, #0x50]
0077edb4: ldrb     sb, [sp, #0x54]
0077edb8: mov      lr, pc
0077edbc: ldr      pc, [r3, #0x5c]
0077edc0: subs     r4, r0, #0
0077edc4: beq      #0x77ef54
0077edc8: add      r8, r5, #0xa8
0077edcc: mov      r0, r8
0077edd0: mov      r1, sl
0077edd4: bl       #0x754fa0
0077edd8: cmp      r0, #0
0077eddc: beq      #0x77edec
0077ede0: ldr      r3, [r0, #0x38]
0077ede4: cmp      fp, r3
0077ede8: beq      #0x77eee0
0077edec: mov      r0, r4
0077edf0: ldr      r3, [r4]
0077edf4: mov      r1, r5
0077edf8: mov      r2, fp
0077edfc: mov      lr, pc
0077ee00: ldr      pc, [r3, #0x18]
0077ee04: subs     r4, r0, #0
0077ee08: beq      #0x77ee10
0077ee0c: bl       #0x759c64
0077ee10: mov      r1, r6
0077ee14: mov      r0, r4
0077ee18: bl       #0x7535b8
0077ee1c: ldr      r6, [r7, #4]
0077ee20: cmp      r6, #0
0077ee24: ble      #0x77ee64
0077ee28: mov      r5, #0
0077ee2c: ldr      r3, [r7]
0077ee30: ldr      r0, [r3, r5, lsl #2]
0077ee34: bl       #0x7baef8
0077ee38: ldr      r2, [r7]
0077ee3c: mov      r1, r0
0077ee40: ldr      r3, [r4]
0077ee44: ldr      r2, [r2, r5, lsl #2]
0077ee48: mov      r0, r4
0077ee4c: add      r5, r5, #1
0077ee50: add      r2, r2, #8
0077ee54: mov      lr, pc
0077ee58: ldr      pc, [r3, #0x1c]
0077ee5c: cmp      r5, r6
0077ee60: bne      #0x77ee2c
0077ee64: ldr      ip, [sp, #0x58]
0077ee68: mov      r0, r8
0077ee6c: mov      r2, sl
0077ee70: str      ip, [sp]
0077ee74: ldr      ip, [sp, #0x5c]
0077ee78: mov      r3, sb
0077ee7c: mov      r1, r4
0077ee80: str      ip, [sp, #4]
0077ee84: ldr      ip, [sp, #0x60]
0077ee88: str      ip, [sp, #8]
0077ee8c: ldr      ip, [sp, #0x64]
0077ee90: str      ip, [sp, #0xc]
0077ee94: ldr      ip, [sp, #0x1c]
0077ee98: str      ip, [sp, #0x10]
0077ee9c: bl       #0x756588
0077eea0: ldr      r3, [r4]
0077eea4: mov      r2, #0
0077eea8: mov      r1, #0x13
0077eeac: ldr      r3, [r3, #0x2c]
0077eeb0: mov      r0, r4
0077eeb4: strb     r1, [sp, #0x20]
0077eeb8: str      r2, [sp, #0x24]
0077eebc: strb     r2, [sp, #0x21]
0077eec0: strh     r2, [sp, #0x22]
0077eec4: add      r1, sp, #0x20
0077eec8: blx      r3
0077eecc: mov      r0, r4
0077eed0: bl       #0x75a240
0077eed4: mov      r0, r4
0077eed8: add      sp, sp, #0x2c
0077eedc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0077eee0: ldr      r1, [r0, #0x44]
0077eee4: cmp      r6, r1
0077eee8: beq      #0x77ef18
0077eeec: ldrsb    r3, [r6]
0077eef0: cmn      r3, #1
0077eef4: ldrsb    r3, [r1]
0077eef8: addne    r0, r6, #1
0077eefc: ldreq    r0, [r6, #0xc]
0077ef00: cmn      r3, #1
0077ef04: addne    r1, r1, #1
0077ef08: ldreq    r1, [r1, #0xc]
0077ef0c: bl       #0x30e31c
0077ef10: cmp      r0, #0
0077ef14: bne      #0x77edec
0077ef18: ldr      r3, [sp, #0x60]
0077ef1c: ldr      ip, [sp, #0x64]
0077ef20: ldr      r2, [sp, #0x1c]
0077ef24: str      r3, [sp]
0077ef28: str      ip, [sp, #4]
0077ef2c: str      r2, [sp, #8]
0077ef30: mov      r0, r5
0077ef34: mov      r1, sl
0077ef38: ldr      r2, [sp, #0x58]
0077ef3c: ldr      r3, [sp, #0x5c]
0077ef40: ldr      ip, [r5]
0077ef44: mov      lr, pc
0077ef48: ldr      pc, [ip, #0xb8]
0077ef4c: mov      r4, #0
0077ef50: b        #0x77eed4
0077ef54: ldr      r0, [pc, #0xc]
0077ef58: mov      r1, fp
0077ef5c: add      r0, pc, r0
0077ef60: bl       #0x761184
0077ef64: b        #0x77eed4
0077ef68: andseq   sl, r8, r4, ror fp

# _ZN7gameswf15sprite_instance22replace_display_objectEPNS_9characterEPKciPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
0077f7e8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0077f7ec: ldr      r4, [pc, #0xd4]
0077f7f0: ldr      r5, [pc, #0xd4]
0077f7f4: sub      sp, sp, #0x34
0077f7f8: add      r4, pc, r4
0077f7fc: subs     ip, r2, #0
0077f800: mov      sl, r3
0077f804: ldr      r2, [r4, r5]
0077f808: ldr      r3, [sp, #0x60]
0077f80c: mov      r8, r0
0077f810: ldr      r2, [r2]
0077f814: str      r3, [sp, #0x10]
0077f818: ldrh     r3, [sp, #0x68]
0077f81c: mov      r6, r1
0077f820: ldr      r7, [sp, #0x58]
0077f824: str      r2, [sp, #0x2c]
0077f828: ldr      fp, [sp, #0x5c]
0077f82c: str      r3, [sp, #0x14]
0077f830: beq      #0x77f840
0077f834: ldrsb    r3, [ip]
0077f838: cmp      r3, #0
0077f83c: bne      #0x77f88c
0077f840: ldr      ip, [sp, #0x10]
0077f844: mov      r2, sl
0077f848: mov      r3, r7
0077f84c: str      ip, [sp, #4]
0077f850: ldr      ip, [sp, #0x64]
0077f854: add      r0, r8, #0xa8
0077f858: mov      r1, r6
0077f85c: str      ip, [sp, #8]
0077f860: ldr      ip, [sp, #0x14]
0077f864: str      fp, [sp]
0077f868: str      ip, [sp, #0xc]
0077f86c: bl       #0x7567cc
0077f870: ldr      r3, [r4, r5]
0077f874: ldr      r2, [sp, #0x2c]
0077f878: ldr      r3, [r3]
0077f87c: cmp      r2, r3
0077f880: bne      #0x77f8c4
0077f884: add      sp, sp, #0x34
0077f888: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0077f88c: add      sb, sp, #0x18
0077f890: mov      r1, ip
0077f894: mov      r0, sb
0077f898: bl       #0x413a7c
0077f89c: mov      r0, r6
0077f8a0: mov      r1, sb
0077f8a4: bl       #0x7535b8
0077f8a8: ldrsb    r3, [sp, #0x18]
0077f8ac: cmn      r3, #1
0077f8b0: bne      #0x77f840
0077f8b4: ldr      r0, [sp, #0x24]
0077f8b8: ldr      r1, [sp, #0x20]
0077f8bc: bl       #0x752b38
0077f8c0: b        #0x77f840
0077f8c4: bl       #0x30e310
0077f8c8: mlaeq    r1, r8, r2, r5
0077f8cc: andeq    r4, r0, ip, lsr #1

# _ZN7gameswf15sprite_instance22replace_display_objectEtPKciPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
0077fb64: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0077fb68: ldr      r4, [pc, #0x140]
0077fb6c: ldr      r7, [pc, #0x140]
0077fb70: mov      r5, r0
0077fb74: add      r4, pc, r4
0077fb78: ldr      r0, [r4, r7]
0077fb7c: mov      r8, r2
0077fb80: sub      sp, sp, #0x3c
0077fb84: ldr      r2, [r0]
0077fb88: ldr      ip, [r5, #0xa0]
0077fb8c: mov      sl, r3
0077fb90: str      r2, [sp, #0x34]
0077fb94: ldr      r2, [sp, #0x64]
0077fb98: ldr      r3, [ip]
0077fb9c: mov      r0, ip
0077fba0: str      r2, [sp, #0x14]
0077fba4: ldr      ip, [sp, #0x68]
0077fba8: ldrh     r2, [sp, #0x70]
0077fbac: mov      r6, r1
0077fbb0: str      ip, [sp, #0x18]
0077fbb4: str      r2, [sp, #0x1c]
0077fbb8: ldr      sb, [sp, #0x60]
0077fbbc: mov      lr, pc
0077fbc0: ldr      pc, [r3, #0x5c]
0077fbc4: subs     r3, r0, #0
0077fbc8: beq      #0x77fc98
0077fbcc: mov      r2, r6
0077fbd0: ldr      r3, [r3]
0077fbd4: mov      r1, r5
0077fbd8: mov      lr, pc
0077fbdc: ldr      pc, [r3, #0x18]
0077fbe0: subs     r6, r0, #0
0077fbe4: beq      #0x77fbec
0077fbe8: bl       #0x759c64
0077fbec: cmp      r8, #0
0077fbf0: beq      #0x77fc00
0077fbf4: ldrsb    r3, [r8]
0077fbf8: cmp      r3, #0
0077fbfc: bne      #0x77fc60
0077fc00: ldr      ip, [sp, #0x14]
0077fc04: add      r0, r5, #0xa8
0077fc08: mov      r2, sl
0077fc0c: str      ip, [sp]
0077fc10: ldr      ip, [sp, #0x18]
0077fc14: mov      r3, sb
0077fc18: mov      r1, r6
0077fc1c: str      ip, [sp, #4]
0077fc20: ldr      ip, [sp, #0x6c]
0077fc24: str      ip, [sp, #8]
0077fc28: ldr      ip, [sp, #0x1c]
0077fc2c: str      ip, [sp, #0xc]
0077fc30: bl       #0x7567cc
0077fc34: cmp      r6, #0
0077fc38: beq      #0x77fc44
0077fc3c: mov      r0, r6
0077fc40: bl       #0x75a240
0077fc44: ldr      r3, [r4, r7]
0077fc48: ldr      r2, [sp, #0x34]
0077fc4c: ldr      r3, [r3]
0077fc50: cmp      r2, r3
0077fc54: bne      #0x77fcac
0077fc58: add      sp, sp, #0x3c
0077fc5c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0077fc60: add      fp, sp, #0x20
0077fc64: mov      r1, r8
0077fc68: mov      r0, fp
0077fc6c: bl       #0x413a7c
0077fc70: mov      r0, r6
0077fc74: mov      r1, fp
0077fc78: bl       #0x7535b8
0077fc7c: ldrsb    r3, [sp, #0x20]
0077fc80: cmn      r3, #1
0077fc84: bne      #0x77fc00
0077fc88: ldr      r0, [sp, #0x2c]
0077fc8c: ldr      r1, [sp, #0x28]
0077fc90: bl       #0x752b38
0077fc94: b        #0x77fc00
0077fc98: ldr      r0, [pc, #0x18]
0077fc9c: mov      r1, r6
0077fca0: add      r0, pc, r0
0077fca4: bl       #0x761184
0077fca8: b        #0x77fc44
0077fcac: bl       #0x30e310
0077fcb0: eoreq    r4, r1, ip, lsl pc
0077fcb4: andeq    r4, r0, ip, lsr #1
0077fcb8: andseq   sb, r8, r0, lsl #30

# _ZN7gameswf14place_object_2D1Ev
0075e230: push     {r4, r5, r6, r7, r8, lr}
0075e234: ldr      r3, [pc, #0xf4]
0075e238: ldr      r2, [pc, #0xf4]
0075e23c: ldrb     r1, [r0, #7]
0075e240: add      r3, pc, r3
0075e244: ldr      r2, [r3, r2]
0075e248: cmp      r1, #0
0075e24c: mov      r7, r0
0075e250: add      r2, r2, #8
0075e254: str      r2, [r0]
0075e258: beq      #0x75e294
0075e25c: add      r1, r0, r1
0075e260: add      r0, r1, #4
0075e264: mov      r1, #0
0075e268: bl       #0x755ad8
0075e26c: ldrb     r4, [r7, #7]
0075e270: mov      r1, #0
0075e274: cmp      r4, #0
0075e278: addne    r4, r7, r4
0075e27c: add      r4, r4, #4
0075e280: mov      r0, r4
0075e284: bl       #0x755ad8
0075e288: mov      r0, r4
0075e28c: mov      r1, #0
0075e290: bl       #0x752ec8
0075e294: ldrb     r6, [r7, #8]
0075e298: cmp      r6, #0
0075e29c: beq      #0x75e300
0075e2a0: add      r6, r7, r6
0075e2a4: ldr      r4, [r6, #4]
0075e2a8: cmp      r4, #0
0075e2ac: ble      #0x75e308
0075e2b0: mov      r5, #0
0075e2b4: ldr      r3, [r6]
0075e2b8: ldr      r8, [r3, r5, lsl #2]
0075e2bc: add      r5, r5, #1
0075e2c0: cmp      r8, #0
0075e2c4: add      r0, r8, #8
0075e2c8: beq      #0x75e2dc
0075e2cc: bl       #0x797124
0075e2d0: mov      r0, r8
0075e2d4: mov      r1, #0
0075e2d8: bl       #0x752b38
0075e2dc: cmp      r5, r4
0075e2e0: bne      #0x75e2b4
0075e2e4: ldr      r4, [r6, #4]
0075e2e8: cmp      r4, #0
0075e2ec: ble      #0x75e308
0075e2f0: mov      r1, #0
0075e2f4: str      r1, [r6, #4]
0075e2f8: mov      r0, r6
0075e2fc: bl       #0x75a298
0075e300: mov      r0, r7
0075e304: pop      {r4, r5, r6, r7, r8, pc}
0075e308: cmp      r4, #0
0075e30c: bge      #0x75e2f0
0075e310: lsl      r3, r4, #2
0075e314: mov      r1, #0
0075e318: ldr      r2, [r6]
0075e31c: adds     r4, r4, #1
0075e320: str      r1, [r2, r3]
0075e324: add      r3, r3, #4
0075e328: bne      #0x75e318
0075e32c: b        #0x75e2f0
0075e330: eoreq    r6, r3, r0, asr r8
0075e334: andeq    r0, r0, ip, asr #18

# _ZN7gameswf6effectD1Ev
00758af8: push     {r4, lr}
00758afc: ldr      ip, [r0, #8]
00758b00: mov      r4, r0
00758b04: add      r0, r0, #4
00758b08: cmp      ip, #0
00758b0c: ble      #0x758b24
00758b10: mov      r1, #0
00758b14: str      r1, [r4, #8]
00758b18: bl       #0x752ec8
00758b1c: mov      r0, r4
00758b20: pop      {r4, pc}
00758b24: bge      #0x758b10
00758b28: mov      r1, #0x2c
00758b2c: mul      r1, r1, ip
00758b30: mov      r2, #0
00758b34: ldr      lr, [r0]
00758b38: adds     ip, ip, #1
00758b3c: add      r3, lr, r1
00758b40: add      r3, r3, #4
00758b44: str      r2, [lr, r1]
00758b48: str      r2, [r3], #4
00758b4c: str      r2, [r3], #4
00758b50: str      r2, [r3], #4
00758b54: str      r2, [r3], #4
00758b58: str      r2, [r3], #4
00758b5c: str      r2, [r3], #4
00758b60: str      r2, [r3], #4
00758b64: str      r2, [r3], #4
00758b68: str      r2, [r3], #4
00758b6c: str      r2, [r3]
00758b70: add      r1, r1, #0x2c
00758b74: bne      #0x758b34
00758b78: mov      r1, #0
00758b7c: str      r1, [r4, #8]
00758b80: bl       #0x752ec8
00758b84: mov      r0, r4
00758b88: pop      {r4, pc}

