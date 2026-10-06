# 0x434624 _GLOBAL__I_.._.._sources_Game_Menus_MenuMessageManager.cpp
00434624: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00434628: ldr r5, [pc, #0x6f0]
0043462c: ldr r2, [pc, #0x6f0]
00434630: ldr r3, [pc, #0x6f0]
00434634: add r5, pc, r5
00434638: ldr r4, [r5, r2]
0043463c: add r3, pc, r3
00434640: mov r2, #0x3f000000
00434644: sub sp, sp, #0x1c
00434648: str r2, [r3, #8]
0043464c: str r2, [r3]
00434650: str r2, [r3, #4]
00434654: mov r0, r4
00434658: bl #0x41aeec
0043465c: ldr r3, [pc, #0x6c8]
00434660: ldr r7, [pc, #0x6c8]
00434664: mov r0, r4
00434668: ldr r6, [r5, r3]
0043466c: ldr r4, [r5, r7]
00434670: mov r1, r6
00434674: mov r2, r4
00434678: bl #0x30e304
0043467c: ldr r3, [pc, #0x6b0]
00434680: ldr r8, [r5, r3]
00434684: mov r0, r8
00434688: bl #0x41aeec
0043468c: mov r1, r6
00434690: mov r2, r4
00434694: mov r0, r8
00434698: bl #0x30e304
0043469c: ldr r3, [pc, #0x694]
004346a0: ldr r8, [r5, r3]
004346a4: mov r0, r8
004346a8: bl #0x41aeec
004346ac: mov r1, r6
004346b0: mov r2, r4
004346b4: mov r0, r8
004346b8: bl #0x30e304
004346bc: ldr r3, [pc, #0x678]
004346c0: ldr r8, [r5, r3]
004346c4: mov r0, r8
004346c8: bl #0x41aeec
004346cc: mov r1, r6
004346d0: mov r2, r4
004346d4: mov r0, r8
004346d8: bl #0x30e304
004346dc: ldr r3, [pc, #0x65c]
004346e0: ldr r8, [r5, r3]
004346e4: mov r0, r8
004346e8: bl #0x41aeec
004346ec: mov r1, r6
004346f0: mov r2, r4
004346f4: mov r0, r8
004346f8: bl #0x30e304
004346fc: ldr r3, [pc, #0x640]
00434700: ldr r8, [r5, r3]
00434704: mov r0, r8
00434708: bl #0x41aeec
0043470c: mov r2, r4
00434710: mov r0, r8
00434714: mov r1, r6
00434718: bl #0x30e304
0043471c: ldr r3, [pc, #0x624]
00434720: ldr r3, [r5, r3]
00434724: ldr r2, [r3]
00434728: tst r2, #1
0043472c: beq #0x434cb0
00434730: ldr r3, [pc, #0x614]
00434734: ldr r3, [r5, r3]
00434738: ldr r2, [r3]
0043473c: tst r2, #1
00434740: beq #0x434cf0
00434744: ldr r3, [pc, #0x604]
00434748: ldr r3, [r5, r3]
0043474c: ldr r6, [r3]
00434750: ands r6, r6, #1
00434754: bne #0x434848
00434758: ldr r1, [pc, #0x5f4]
0043475c: ldr r2, [pc, #0x5f4]
00434760: add sb, sp, #0x14
00434764: ldr sl, [r5, r1]
00434768: ldr r2, [r5, r2]
0043476c: str r1, [sp, #0xc]
00434770: mov r4, sl
00434774: mov r1, #1
00434778: add r2, r2, #8
0043477c: str r1, [r3]
00434780: str r2, [r4], #4
00434784: add sl, sl, #0xa4
00434788: mov r8, #8
0043478c: mov fp, #0x70
00434790: mov r1, r8
00434794: str r6, [r4]
00434798: str r6, [r4, #4]
0043479c: str r6, [r4, #8]
004347a0: str r6, [r4, #0xc]
004347a4: str r6, [r4, #0x10]
004347a8: str r6, [r4, #0x14]
004347ac: str r6, [r4, #0x18]
004347b0: str r6, [r4, #0x1c]
004347b4: str r6, [r4, #0x20]
004347b8: str r8, [r4, #0x24]
004347bc: add r0, r4, #0x20
004347c0: mov r2, r6
004347c4: bl #0x329510
004347c8: ldr r2, [r4, #0x24]
004347cc: str r0, [r4, #0x20]
004347d0: mov r3, r0
004347d4: sub r2, r2, #1
004347d8: lsr r2, r2, #1
004347dc: mov r0, sb
004347e0: stmib sp, {r2, r3}
004347e4: str fp, [sp, #0x14]
004347e8: bl #0x708ec0
004347ec: ldmib sp, {r2, r3}
004347f0: add r1, r3, r2, lsl #2
004347f4: str r0, [r3, r2, lsl #2]
004347f8: str r1, [r4, #0xc]
004347fc: ldr r0, [r3, r2, lsl #2]
00434800: str r1, [r4, #0x1c]
00434804: add r1, r0, #0x70
00434808: stmib r4, {r0, r1}
0043480c: ldr r3, [r3, r2, lsl #2]
00434810: str r0, [r4]
00434814: add r2, r3, #0x70
00434818: str r2, [r4, #0x18]
0043481c: str r3, [r4, #0x10]
00434820: str r3, [r4, #0x14]
00434824: add r4, r4, #0x28
00434828: cmp r4, sl
0043482c: bne #0x434790
00434830: ldr r2, [sp, #0xc]
00434834: ldr r3, [pc, #0x520]
00434838: ldr r0, [r5, r2]
0043483c: ldr r1, [r5, r3]
00434840: ldr r2, [r5, r7]
00434844: bl #0x30e304
00434848: ldr r3, [pc, #0x510]
0043484c: ldr ip, [r5, r3]
00434850: ldr r3, [ip]
00434854: ands r3, r3, #1
00434858: bne #0x434928
0043485c: ldr r6, [pc, #0x500]
00434860: ldr r2, [pc, #0x500]
00434864: mov lr, #8
00434868: ldr r6, [r5, r6]
0043486c: ldr r4, [r5, r2]
00434870: mov r8, #1
00434874: add r6, r6, lr
00434878: str r8, [ip]
0043487c: mov r1, lr
00434880: mov r2, r3
00434884: str lr, [r4, #0x28]
00434888: str r6, [r4]
0043488c: str r3, [r4, #4]
00434890: str r3, [r4, #8]
00434894: str r3, [r4, #0xc]
00434898: str r3, [r4, #0x10]
0043489c: str r3, [r4, #0x14]
004348a0: str r3, [r4, #0x18]
004348a4: str r3, [r4, #0x1c]
004348a8: str r3, [r4, #0x20]
004348ac: str r3, [r4, #0x24]
004348b0: add r0, r4, #0x24
004348b4: bl #0x329570
004348b8: mov r3, #0x70
004348bc: mov r6, r0
004348c0: add r0, sp, #0x18
004348c4: ldr r8, [r4, #0x28]
004348c8: str r3, [r0, #-4]!
004348cc: str r6, [r4, #0x24]
004348d0: bl #0x708ec0
004348d4: sub r8, r8, #1
004348d8: lsr r8, r8, #1
004348dc: str r0, [r6, r8, lsl #2]
004348e0: add r3, r6, r8, lsl #2
004348e4: str r3, [r4, #0x10]
004348e8: ldr ip, [r6, r8, lsl #2]
004348ec: str r3, [r4, #0x20]
004348f0: ldr r2, [pc, #0x474]
004348f4: add r3, ip, #0x70
004348f8: str r3, [r4, #0xc]
004348fc: str ip, [r4, #8]
00434900: ldr r3, [r6, r8, lsl #2]
00434904: ldr r1, [r5, r2]
00434908: mov r0, r4
0043490c: add lr, r3, #0x70
00434910: ldr r2, [r5, r7]
00434914: str lr, [r4, #0x1c]
00434918: str ip, [r4, #4]
0043491c: str r3, [r4, #0x14]
00434920: str r3, [r4, #0x18]
00434924: bl #0x30e304
00434928: ldr r3, [pc, #0x440]
0043492c: ldr ip, [r5, r3]
00434930: ldr r3, [ip]
00434934: ands r3, r3, #1
00434938: bne #0x434a08
0043493c: ldr r6, [pc, #0x430]
00434940: ldr r2, [pc, #0x430]
00434944: mov lr, #8
00434948: ldr r6, [r5, r6]
0043494c: ldr r4, [r5, r2]
00434950: mov r8, #1
00434954: add r6, r6, lr
00434958: str r8, [ip]
0043495c: mov r1, lr
00434960: mov r2, r3
00434964: str lr, [r4, #0x28]
00434968: str r6, [r4]
0043496c: str r3, [r4, #4]
00434970: str r3, [r4, #8]
00434974: str r3, [r4, #0xc]
00434978: str r3, [r4, #0x10]
0043497c: str r3, [r4, #0x14]
00434980: str r3, [r4, #0x18]
00434984: str r3, [r4, #0x1c]
00434988: str r3, [r4, #0x20]
0043498c: str r3, [r4, #0x24]
00434990: add r0, r4, #0x24
00434994: bl #0x3295d0
00434998: mov r3, #0x80
0043499c: mov r6, r0
004349a0: add r0, sp, #0x18
004349a4: ldr r8, [r4, #0x28]
004349a8: str r3, [r0, #-4]!
004349ac: str r6, [r4, #0x24]
004349b0: bl #0x708ec0
004349b4: sub r8, r8, #1
004349b8: lsr r8, r8, #1
004349bc: str r0, [r6, r8, lsl #2]
004349c0: add r3, r6, r8, lsl #2
004349c4: str r3, [r4, #0x10]
004349c8: ldr ip, [r6, r8, lsl #2]
004349cc: str r3, [r4, #0x20]
004349d0: ldr r2, [pc, #0x3a4]
004349d4: add r3, ip, #0x80
004349d8: str r3, [r4, #0xc]
004349dc: str ip, [r4, #8]
004349e0: ldr r3, [r6, r8, lsl #2]
004349e4: ldr r1, [r5, r2]
004349e8: mov r0, r4
004349ec: add lr, r3, #0x80
004349f0: ldr r2, [r5, r7]
004349f4: str lr, [r4, #0x1c]
004349f8: str ip, [r4, #4]
004349fc: str r3, [r4, #0x14]
00434a00: str r3, [r4, #0x18]
00434a04: bl #0x30e304
00434a08: ldr r3, [pc, #0x370]
00434a0c: ldr ip, [r5, r3]
00434a10: ldr r3, [ip]
00434a14: ands r3, r3, #1
00434a18: bne #0x434ae8
00434a1c: ldr r6, [pc, #0x360]
00434a20: ldr r2, [pc, #0x360]
00434a24: mov lr, #8
00434a28: ldr r6, [r5, r6]
00434a2c: ldr r4, [r5, r2]
00434a30: mov r8, #1
00434a34: add r6, r6, lr
00434a38: str r8, [ip]
00434a3c: mov r1, lr
00434a40: mov r2, r3
00434a44: str lr, [r4, #0x28]
00434a48: str r6, [r4]
00434a4c: str r3, [r4, #4]
00434a50: str r3, [r4, #8]
00434a54: str r3, [r4, #0xc]
00434a58: str r3, [r4, #0x10]
00434a5c: str r3, [r4, #0x14]
00434a60: str r3, [r4, #0x18]
00434a64: str r3, [r4, #0x1c]
00434a68: str r3, [r4, #0x20]
00434a6c: str r3, [r4, #0x24]
00434a70: add r0, r4, #0x24
00434a74: bl #0x329368
00434a78: mov r3, #0x4c
00434a7c: mov r6, r0
00434a80: add r0, sp, #0x18
00434a84: ldr r8, [r4, #0x28]
00434a88: str r3, [r0, #-4]!
00434a8c: str r6, [r4, #0x24]
00434a90: bl #0x708ec0
00434a94: sub r8, r8, #1
00434a98: lsr r8, r8, #1
00434a9c: str r0, [r6, r8, lsl #2]
00434aa0: add r3, r6, r8, lsl #2
00434aa4: str r3, [r4, #0x10]
00434aa8: ldr ip, [r6, r8, lsl #2]
00434aac: str r3, [r4, #0x20]
00434ab0: ldr r2, [pc, #0x2d4]
00434ab4: add r3, ip, #0x4c
00434ab8: str r3, [r4, #0xc]
00434abc: str ip, [r4, #8]
00434ac0: ldr r3, [r6, r8, lsl #2]
00434ac4: ldr r1, [r5, r2]
00434ac8: mov r0, r4
00434acc: add lr, r3, #0x4c
00434ad0: ldr r2, [r5, r7]
00434ad4: str lr, [r4, #0x1c]
00434ad8: str ip, [r4, #4]
00434adc: str r3, [r4, #0x14]
00434ae0: str r3, [r4, #0x18]
00434ae4: bl #0x30e304
00434ae8: ldr r3, [pc, #0x2a0]
00434aec: ldr ip, [r5, r3]
00434af0: ldr r3, [ip]
00434af4: ands r3, r3, #1
00434af8: bne #0x434bc8
00434afc: ldr r6, [pc, #0x290]
00434b00: ldr r2, [pc, #0x290]
00434b04: mov lr, #8
00434b08: ldr r6, [r5, r6]
00434b0c: ldr r4, [r5, r2]
00434b10: mov r8, #1
00434b14: add r6, r6, lr
00434b18: str r8, [ip]
00434b1c: mov r1, lr
00434b20: mov r2, r3
00434b24: str lr, [r4, #0x28]
00434b28: str r6, [r4]
00434b2c: str r3, [r4, #4]
00434b30: str r3, [r4, #8]
00434b34: str r3, [r4, #0xc]
00434b38: str r3, [r4, #0x10]
00434b3c: str r3, [r4, #0x14]
00434b40: str r3, [r4, #0x18]
00434b44: str r3, [r4, #0x1c]
00434b48: str r3, [r4, #0x20]
00434b4c: str r3, [r4, #0x24]
00434b50: add r0, r4, #0x24
00434b54: bl #0x3293c8
00434b58: mov r3, #0x68
00434b5c: mov r6, r0
00434b60: add r0, sp, #0x18
00434b64: ldr r8, [r4, #0x28]
00434b68: str r3, [r0, #-4]!
00434b6c: str r6, [r4, #0x24]
00434b70: bl #0x708ec0
00434b74: sub r8, r8, #1
00434b78: lsr r8, r8, #1
00434b7c: str r0, [r6, r8, lsl #2]
00434b80: add r3, r6, r8, lsl #2
00434b84: str r3, [r4, #0x10]
00434b88: ldr ip, [r6, r8, lsl #2]
00434b8c: str r3, [r4, #0x20]
00434b90: ldr r2, [pc, #0x204]
00434b94: add r3, ip, #0x68
00434b98: str r3, [r4, #0xc]
00434b9c: str ip, [r4, #8]
00434ba0: ldr r3, [r6, r8, lsl #2]
00434ba4: ldr r1, [r5, r2]
00434ba8: mov r0, r4
00434bac: add lr, r3, #0x68
00434bb0: ldr r2, [r5, r7]
00434bb4: str lr, [r4, #0x1c]
00434bb8: str ip, [r4, #4]
00434bbc: str r3, [r4, #0x14]
00434bc0: str r3, [r4, #0x18]
00434bc4: bl #0x30e304
00434bc8: ldr r3, [pc, #0x1d0]
00434bcc: ldr ip, [r5, r3]
00434bd0: ldr r3, [ip]
00434bd4: ands r3, r3, #1
00434bd8: bne #0x434ca8
00434bdc: ldr r6, [pc, #0x1c0]
00434be0: ldr r2, [pc, #0x1c0]
00434be4: mov lr, #8
00434be8: ldr r6, [r5, r6]
00434bec: ldr r4, [r5, r2]
00434bf0: mov r8, #1
00434bf4: add r6, r6, lr
00434bf8: str r8, [ip]
00434bfc: mov r1, lr
00434c00: mov r2, r3
00434c04: str lr, [r4, #0x28]
00434c08: str r6, [r4]
00434c0c: str r3, [r4, #4]
00434c10: str r3, [r4, #8]
00434c14: str r3, [r4, #0xc]
00434c18: str r3, [r4, #0x10]
00434c1c: str r3, [r4, #0x14]
00434c20: str r3, [r4, #0x18]
00434c24: str r3, [r4, #0x1c]
00434c28: str r3, [r4, #0x20]
00434c2c: str r3, [r4, #0x24]
00434c30: add r0, r4, #0x24
00434c34: bl #0x329428
00434c38: mov r3, #0x78
00434c3c: mov r6, r0
00434c40: add r0, sp, #0x18
00434c44: ldr r8, [r4, #0x28]
00434c48: str r3, [r0, #-4]!
00434c4c: str r6, [r4, #0x24]
00434c50: bl #0x708ec0
00434c54: sub r8, r8, #1
00434c58: lsr r8, r8, #1
00434c5c: str r0, [r6, r8, lsl #2]
00434c60: add r3, r6, r8, lsl #2
00434c64: str r3, [r4, #0x10]
00434c68: ldr ip, [r6, r8, lsl #2]
00434c6c: str r3, [r4, #0x20]
00434c70: ldr r1, [pc, #0x134]
00434c74: add r3, ip, #0x78
00434c78: str r3, [r4, #0xc]
00434c7c: str ip, [r4, #8]
00434c80: ldr r3, [r6, r8, lsl #2]
00434c84: ldr r2, [r5, r7]
00434c88: mov r0, r4
00434c8c: add lr, r3, #0x78
00434c90: ldr r1, [r5, r1]
00434c94: str lr, [r4, #0x1c]
00434c98: str ip, [r4, #4]
00434c9c: str r3, [r4, #0x14]
00434ca0: str r3, [r4, #0x18]
00434ca4: bl #0x30e304
00434ca8: add sp, sp, #0x1c
00434cac: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00434cb0: mov r2, #1
00434cb4: str r2, [r3]
00434cb8: ldr r3, [pc, #0xf0]
00434cbc: ldr r6, [r5, r3]
00434cc0: mov r0, r6
00434cc4: bl #0x3790a8
00434cc8: ldr r3, [pc, #0xe4]
00434ccc: mov r2, r4
00434cd0: mov r0, r6
00434cd4: ldr r1, [r5, r3]
00434cd8: bl #0x30e304
00434cdc: ldr r3, [pc, #0x68]
00434ce0: ldr r3, [r5, r3]
00434ce4: ldr r2, [r3]
00434ce8: tst r2, #1
00434cec: bne #0x434744
00434cf0: mov r2, #1
00434cf4: str r2, [r3]
00434cf8: ldr r3, [pc, #0xb8]
00434cfc: ldr r4, [r5, r3]
00434d00: mov r0, r4
00434d04: bl #0x32d79c
00434d08: ldr r3, [pc, #0xac]
00434d0c: mov r0, r4
00434d10: ldr r2, [r5, r7]
00434d14: ldr r1, [r5, r3]
00434d18: bl #0x30e304
00434d1c: b #0x434744
00434d20: subseq r0, r6, ip, asr r4
00434d24: andeq r2, r0, r0, ror r0
00434d28: subseq r0, r7, r0, ror #26
00434d2c: andeq r2, r0, r4, asr sl
00434d30: muleq r0, r0, r8
00434d34: andeq r3, r0, r4, asr #11
00434d38: andeq r0, r0, r8, lsl #18
00434d3c: andeq r2, r0, r4, lsr r2
00434d40: andeq r4, r0, r4, asr r0
00434d44: andeq r4, r0, r8, lsl #10
00434d48: strdeq r0, r1, [r0], -r4
00434d4c: andeq r0, r0, ip, lsr #31
00434d50: andeq r2, r0, ip, asr sb
00434d54: andeq r1, r0, ip, ror #9
00434d58: ldrdeq r1, r2, [r0], -r0
00434d5c: strheq r3, [r0], -ip
00434d60: andeq r2, r0, r4, lsl r2
00434d64: andeq r2, r0, r0, lsl #4
00434d68: ldrdeq r0, r1, [r0], -r8
00434d6c: strdeq r2, r3, [r0], -r4
00434d70: andeq r1, r0, r0, lsl r3
00434d74: andeq r3, r0, r8, ror r1
00434d78: andeq r1, r0, r0, lsl ip
00434d7c: andeq r1, r0, r8, lsl #18
00434d80: andeq r3, r0, ip, lsr #18
00434d84: andeq r1, r0, r8, ror r4
00434d88: andeq r1, r0, r4, ror lr
00434d8c: muleq r0, r4, sb
00434d90: muleq r0, r8, r6
00434d94: andeq r1, r0, r8, ror r1
00434d98: andeq r4, r0, r0, lsl #18
00434d9c: strdeq r1, r2, [r0], -r8
00434da0: andeq r3, r0, r8, asr r8
00434da4: andeq r2, r0, r0, asr #5
00434da8: strheq r2, [r0], -ip
00434dac: andeq r1, r0, ip, asr #28
00434db0: andeq r2, r0, r4, lsl r7
00434db4: muleq r0, ip, r5
00434db8: strdeq r3, r4, [r0], -r4
00434dbc: andeq r0, r0, r0, asr #17

# 0x442734 _ZNK18MenuMessageManagerI19CharMenuTutorialMsgLi1EE6InvokeEPKci.clone.50
00442734: push {r4, r5, r6, r7, lr}
00442738: sub sp, sp, #0x24
0044273c: mov r6, r0
00442740: bl #0x42ca8c
00442744: bl #0x42cb8c
00442748: ldr r4, [pc, #0xdc]
0044274c: subs r5, r0, #0
00442750: add r4, pc, r4
00442754: beq #0x4427d8
00442758: ldr r7, [pc, #0xd0]
0044275c: ldr r3, [r4, r7]
00442760: ldr r2, [r3, #0x2c]
00442764: cmp r2, #0
00442768: beq #0x442804
0044276c: ldr r0, [r3, #0x28]
00442770: ldrb r3, [r0, #4]
00442774: cmp r3, #0
00442778: beq #0x4427e0
0044277c: ldr r0, [r4, r7]
00442780: bl #0x427d50
00442784: mov ip, #0
00442788: mov r2, #0
0044278c: mov r3, #0
00442790: strb ip, [sp, #0xc]
00442794: mov ip, #2
00442798: strd r2, r3, [sp, #0x18]
0044279c: strb ip, [sp, #0xd]
004427a0: mov ip, #0
004427a4: str ip, [sp, #0x10]
004427a8: ldr ip, [sp, #0x1c]
004427ac: add r4, sp, #0xc
004427b0: mov r1, r0
004427b4: str ip, [r4, #8]
004427b8: mov r0, r5
004427bc: mov ip, #1
004427c0: mov r2, r6
004427c4: mov r3, r4
004427c8: str ip, [sp]
004427cc: bl #0x7abe0c
004427d0: mov r0, r4
004427d4: bl #0x797124
004427d8: add sp, sp, #0x24
004427dc: pop {r4, r5, r6, r7, pc}
004427e0: ldr r1, [r0]
004427e4: sub r1, r1, #1
004427e8: cmp r1, #0
004427ec: str r1, [r0]
004427f0: beq #0x442824
004427f4: ldr r3, [r4, r7]
004427f8: mov r2, #0
004427fc: str r2, [r3, #0x2c]
00442800: str r2, [r3, #0x28]
00442804: ldr r3, [pc, #0x28]
00442808: ldr r0, [r4, r7]
0044280c: mov r2, r5
00442810: ldr r1, [r4, r3]
00442814: mov r3, #0
00442818: ldr r1, [r1]
0044281c: bl #0x427ca0
00442820: b #0x44277c
00442824: bl #0x752b38
00442828: b #0x4427f4
0044282c: subseq r2, r5, r0, asr #6
00442830: andeq r4, r0, r4, asr r0
00442834: andeq r4, r0, r4, asr #12

# 0x45a0fc _ZNK18MenuMessageManagerI19CharMenuTutorialMsgLi1EE6InvokeEPKci.clone.31
0045a0fc: push {r4, r5, r6, r7, r8, lr}
0045a100: sub sp, sp, #0x20
0045a104: mov r6, r0
0045a108: bl #0x42ca8c
0045a10c: bl #0x42cb8c
0045a110: ldr r4, [pc, #0xa4]
0045a114: subs r5, r0, #0
0045a118: add r4, pc, r4
0045a11c: beq #0x45a198
0045a120: ldr r7, [pc, #0x98]
0045a124: ldr r8, [r4, r7]
0045a128: add r0, r8, #0x28
0045a12c: bl #0x386144
0045a130: ldr r3, [r8, #0x2c]
0045a134: cmp r3, #0
0045a138: beq #0x45a1a0
0045a13c: ldr r0, [r4, r7]
0045a140: bl #0x427d50
0045a144: mov ip, #0
0045a148: mov r2, #0
0045a14c: mov r3, #0
0045a150: strb ip, [sp, #0xc]
0045a154: mov ip, #2
0045a158: strd r2, r3, [sp, #0x18]
0045a15c: strb ip, [sp, #0xd]
0045a160: mov ip, #0
0045a164: str ip, [sp, #0x10]
0045a168: ldr ip, [sp, #0x1c]
0045a16c: add r4, sp, #0xc
0045a170: mov r1, r0
0045a174: str ip, [r4, #8]
0045a178: mov r0, r5
0045a17c: mov ip, #1
0045a180: mov r2, r6
0045a184: mov r3, r4
0045a188: str ip, [sp]
0045a18c: bl #0x7abe0c
0045a190: mov r0, r4
0045a194: bl #0x797124
0045a198: add sp, sp, #0x20
0045a19c: pop {r4, r5, r6, r7, r8, pc}
0045a1a0: ldr r2, [pc, #0x1c]
0045a1a4: mov r0, r8
0045a1a8: ldr r1, [r4, r2]
0045a1ac: mov r2, r5
0045a1b0: ldr r1, [r1]
0045a1b4: bl #0x427ca0
0045a1b8: b #0x45a13c
0045a1bc: subseq sl, r3, r8, ror sb
0045a1c0: andeq r4, r0, r4, asr r0
0045a1c4: andeq r4, r0, r4, asr #12

# 0x45b654 _ZNSaI19CharMenuTutorialMsgE8allocateEjPKv.clone.3
0045b654: str lr, [sp, #-4]!
0045b658: sub sp, sp, #0xc
0045b65c: add r0, sp, #8
0045b660: mov r3, #0x68
0045b664: str r3, [r0, #-4]!
0045b668: bl #0x708ec0
0045b66c: add sp, sp, #0xc
0045b670: ldm sp!, {pc}

# 0x508ef4 _ZN13StringManager5parseERSsPKcz
00508ef4: push {r2, r3}
00508ef8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508efc: ldr r5, [pc, #0xab4]
00508f00: ldr r2, [pc, #0xab4]
00508f04: sub sp, sp, #0xac
00508f08: add r5, pc, r5
00508f0c: ldr r3, [r5, r2]
00508f10: ldr r4, [sp, #0xd0]
00508f14: str r2, [sp, #0x14]
00508f18: ldr r3, [r3]
00508f1c: add r2, sp, #0xd4
00508f20: cmp r4, #0
00508f24: str r2, [sp, #0x54]
00508f28: str r0, [sp, #0x18]
00508f2c: str r3, [sp, #0xa4]
00508f30: mov r7, r1
00508f34: beq #0x508f44
00508f38: ldrsb r3, [r4]
00508f3c: cmp r3, #0
00508f40: bne #0x508f74
00508f44: mov r8, #0
00508f48: ldr r2, [sp, #0x14]
00508f4c: mov r0, r8
00508f50: ldr r3, [r5, r2]
00508f54: ldr r2, [sp, #0xa4]
00508f58: ldr r3, [r3]
00508f5c: cmp r2, r3
00508f60: bne #0x5099b4
00508f64: add sp, sp, #0xac
00508f68: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508f6c: add sp, sp, #8
00508f70: bx lr
00508f74: ldr r3, [pc, #0xa44]
00508f78: ldr r6, [pc, #0xa44]
00508f7c: ldr r2, [pc, #0xa44]
00508f80: ldr r8, [r5, r3]
00508f84: add r6, pc, r6
00508f88: add r2, pc, r2
00508f8c: ldr r0, [r8, #0x2c]
00508f90: mov r1, r6
00508f94: str r3, [sp, #0x24]
00508f98: bl #0x4c4bdc
00508f9c: mov r1, r0
00508fa0: ldr r0, [sp, #0x18]
00508fa4: bl #0x508edc
00508fa8: ldr r2, [pc, #0xa1c]
00508fac: str r0, [sp, #0x40]
00508fb0: mov r1, r6
00508fb4: add r2, pc, r2
00508fb8: ldr r0, [r8, #0x2c]
00508fbc: bl #0x4c4bdc
00508fc0: mov r1, r0
00508fc4: ldr r0, [sp, #0x18]
00508fc8: bl #0x508edc
00508fcc: ldr r2, [pc, #0x9fc]
00508fd0: str r0, [sp, #0x38]
00508fd4: mov r1, r6
00508fd8: add r2, pc, r2
00508fdc: ldr r0, [r8, #0x2c]
00508fe0: bl #0x4c4bdc
00508fe4: mov r1, r0
00508fe8: ldr r0, [sp, #0x18]
00508fec: bl #0x508edc
00508ff0: bl #0x30e094
00508ff4: str r0, [sp, #0x2c]
00508ff8: ldrb r3, [r4]
00508ffc: cmp r3, #0
00509000: moveq r8, r3
00509004: beq #0x509248
00509008: ldr r2, [pc, #0x9c4]
0050900c: mov r8, #0
00509010: add r4, r4, #1
00509014: add r2, pc, r2
00509018: str r2, [sp, #0x34]
0050901c: ldr r2, [pc, #0x9b4]
00509020: ldr ip, [sp, #0x34]
00509024: mov r6, r8
00509028: add r2, pc, r2
0050902c: str r2, [sp, #0x3c]
00509030: ldr r2, [pc, #0x9a4]
00509034: add ip, ip, #6
00509038: str ip, [sp, #0x48]
0050903c: add r2, pc, r2
00509040: str r2, [sp, #0x4c]
00509044: ldr r2, [pc, #0x994]
00509048: str r5, [sp, #0x28]
0050904c: add r2, pc, r2
00509050: str r2, [sp, #0x44]
00509054: b #0x509088
00509058: sxtb r3, r3
0050905c: cmp r3, #0x5e
00509060: moveq r6, #1
00509064: beq #0x50907c
00509068: cmp r3, #0x7c
0050906c: beq #0x5092b0
00509070: mov r0, r7
00509074: mov r2, r4
00509078: bl #0x310804
0050907c: ldrb r3, [r4], #1
00509080: cmp r3, #0
00509084: beq #0x509244
00509088: cmp r6, #0
0050908c: sub r1, r4, #1
00509090: beq #0x509058
00509094: sxtb r3, r3
00509098: sub r2, r3, #0x23
0050909c: cmp r2, #0x53
005090a0: addls pc, pc, r2, lsl #2
005090a4: b #0x509230
005090a8: b #0x5096f4
005090ac: b #0x5095fc
005090b0: b #0x509230
005090b4: b #0x509230
005090b8: b #0x509230
005090bc: b #0x509230
005090c0: b #0x509230
005090c4: b #0x5096f4
005090c8: b #0x509230
005090cc: b #0x509230
005090d0: b #0x509230
005090d4: b #0x509230
005090d8: b #0x509230
005090dc: b #0x509230
005090e0: b #0x509230
005090e4: b #0x509230
005090e8: b #0x509230
005090ec: b #0x509230
005090f0: b #0x509230
005090f4: b #0x509230
005090f8: b #0x509230
005090fc: b #0x509230
00509100: b #0x509230
00509104: b #0x509230
00509108: b #0x509230
0050910c: b #0x509230
00509110: b #0x509230
00509114: b #0x509230
00509118: b #0x509230
0050911c: b #0x509230
00509120: b #0x509230
00509124: b #0x509230
00509128: b #0x509230
0050912c: b #0x509230
00509130: b #0x509230
00509134: b #0x509230
00509138: b #0x509230
0050913c: b #0x509230
00509140: b #0x509230
00509144: b #0x509230
00509148: b #0x509230
0050914c: b #0x509230
00509150: b #0x509230
00509154: b #0x509230
00509158: b #0x509230
0050915c: b #0x509230
00509160: b #0x509230
00509164: b #0x509230
00509168: b #0x509230
0050916c: b #0x509230
00509170: b #0x509230
00509174: b #0x509230
00509178: b #0x509230
0050917c: b #0x509230
00509180: b #0x509230
00509184: b #0x509230
00509188: b #0x509230
0050918c: b #0x509230
00509190: b #0x509230
00509194: b #0x5096f4
00509198: b #0x509230
0050919c: b #0x509230
005091a0: b #0x509230
005091a4: b #0x509230
005091a8: b #0x509230
005091ac: b #0x509540
005091b0: b #0x509230
005091b4: b #0x509388
005091b8: b #0x509388
005091bc: b #0x509388
005091c0: b #0x509388
005091c4: b #0x509230
005091c8: b #0x509540
005091cc: b #0x509230
005091d0: b #0x509388
005091d4: b #0x509350
005091d8: b #0x509230
005091dc: b #0x509540
005091e0: b #0x509230
005091e4: b #0x509230
005091e8: b #0x50932c
005091ec: b #0x5092ec
005091f0: b #0x509230
005091f4: b #0x5091f8
005091f8: ldr ip, [sp, #0x28]
005091fc: ldr lr, [sp, #0x24]
00509200: add r5, sp, #0x98
00509204: mov r1, r5
00509208: mov r2, #0xa
0050920c: mov r3, #1
00509210: ldr r0, [ip, lr]
00509214: bl #0x31f6b0
00509218: mov r0, r5
0050921c: bl #0x30de54
00509220: add r2, r5, r0
00509224: mov r1, r5
00509228: mov r0, r7
0050922c: bl #0x310804
00509230: mov r8, #1
00509234: mov r6, #0
00509238: ldrb r3, [r4], #1
0050923c: cmp r3, #0
00509240: bne #0x509088
00509244: ldr r5, [sp, #0x28]
00509248: ldr r3, [r7, #0x14]
0050924c: ldr r0, [r7, #0x10]
00509250: mov r1, #0
00509254: rsb r0, r3, r0
00509258: add r0, r0, #0x80
0050925c: bl #0x31056c
00509260: mov r4, r0
00509264: ldr r0, [sp, #0x18]
00509268: ldr r6, [r7, #0x14]
0050926c: bl #0x50750c
00509270: mov r1, r4
00509274: mov r3, r0
00509278: mvn r2, #0
0050927c: mov r0, r6
00509280: bl #0x752a18
00509284: mov r0, r4
00509288: bl #0x30de54
0050928c: mov r1, r4
00509290: add r2, r4, r0
00509294: mov r0, r7
00509298: bl #0x3109e0
0050929c: cmp r4, #0
005092a0: beq #0x508f48
005092a4: mov r0, r4
005092a8: bl #0x310440
005092ac: b #0x508f48
005092b0: ldr r2, [pc, #0x72c]
005092b4: add r5, sp, #0x78
005092b8: mov r1, #0x20
005092bc: add r2, pc, r2
005092c0: mov r3, #0x11
005092c4: mov r0, r5
005092c8: bl #0x30e244
005092cc: mov r0, r5
005092d0: bl #0x30de54
005092d4: mov r1, r5
005092d8: add r2, r5, r0
005092dc: mov r0, r7
005092e0: bl #0x310804
005092e4: mov r8, #1
005092e8: b #0x50907c
005092ec: ldr r3, [sp, #0x28]
005092f0: ldr lr, [sp, #0x24]
005092f4: add r5, sp, #0x58
005092f8: mov r1, r5
005092fc: ldr r0, [r3, lr]
00509300: mov r2, #0x20
00509304: bl #0x3206d0
00509308: mov r0, r5
0050930c: bl #0x30de54
00509310: mov r1, r5
00509314: add r2, r5, r0
00509318: mov r0, r7
0050931c: bl #0x310804
00509320: mov r8, #1
00509324: mov r6, #0
00509328: b #0x50907c
0050932c: ldr r3, [sp, #0x54]
00509330: add r2, r3, #4
00509334: str r2, [sp, #0x54]
00509338: ldr r5, [r3]
0050933c: cmp r5, #0
00509340: bne #0x509218
00509344: ldr r2, [sp, #0x48]
00509348: ldr r5, [sp, #0x34]
0050934c: b #0x509224
00509350: add r5, sp, #0x78
00509354: mov r1, #0x20
00509358: ldr r2, [sp, #0x3c]
0050935c: mov r0, r5
00509360: bl #0x30e244
00509364: mov r0, r5
00509368: bl #0x30de54
0050936c: mov r1, r5
00509370: add r2, r5, r0
00509374: mov r0, r7
00509378: bl #0x310804
0050937c: mov r8, #1
00509380: mov r6, #0
00509384: b #0x509238
00509388: cmp r3, #0x66
0050938c: beq #0x509790
00509390: cmp r3, #0x67
00509394: beq #0x509904
00509398: cmp r3, #0x68
0050939c: beq #0x509954
005093a0: cmp r3, #0x69
005093a4: beq #0x509984
005093a8: cmp r3, #0x6d
005093ac: beq #0x509810
005093b0: ldrsb r3, [r4, #-1]
005093b4: cmp r3, #0x6d
005093b8: beq #0x5097cc
005093bc: mov r1, #0
005093c0: ldr r0, [sp, #0x20]
005093c4: bl #0x30e70c
005093c8: cmp r0, #0
005093cc: movweq r1, #0xd70a
005093d0: movwne r1, #0xd70a
005093d4: movteq r1, #0x3ba3
005093d8: movtne r1, #0xbba3
005093dc: ldr r0, [sp, #0x20]
005093e0: bl #0x30eba4
005093e4: str r0, [sp, #0x20]
005093e8: add r1, sp, #0x50
005093ec: ldr r0, [sp, #0x20]
005093f0: bl #0x30ea30
005093f4: mov r1, #0
005093f8: mov r5, r0
005093fc: ldr r0, [sp, #0x20]
00509400: bl #0x30e70c
00509404: cmp r0, #0
00509408: movweq r1, #0xd70a
0050940c: movwne r1, #0xd70a
00509410: movteq r1, #0x3ba3
00509414: movtne r1, #0xbba3
00509418: mov r0, r5
0050941c: bl #0x30e3ac
00509420: mov r6, r0
00509424: ldr r0, [sp, #0x50]
00509428: bl #0x30e4cc
0050942c: ldr r1, [sp, #0x2c]
00509430: str r0, [sp, #0x1c]
00509434: cmp r1, r0
00509438: bgt #0x509770
0050943c: ldr r2, [sp, #0x1c]
00509440: movw r3, #0xde83
00509444: ldr ip, [sp, #0x1c]
00509448: movt r3, #0x431b
0050944c: smull r2, r3, r3, r2
00509450: asr r1, ip, #0x1f
00509454: mov r2, #0xf4000
00509458: rsb r3, r1, r3, asr #18
0050945c: add r2, r2, #0x240
00509460: mls r2, r2, r3, ip
00509464: movw r0, #0x4dd3
00509468: mov lr, ip
0050946c: movt r0, #0x1062
00509470: smull lr, ip, r0, lr
00509474: smull lr, r0, r0, r2
00509478: asr r2, r2, #0x1f
0050947c: rsb lr, r2, r0, asr #6
00509480: ldr r0, [sp, #0x1c]
00509484: rsb ip, r1, ip, asr #6
00509488: mov r2, #0x3e8
0050948c: cmp r3, #0
00509490: mls ip, r2, ip, r0
00509494: bne #0x5098ac
00509498: cmp lr, #0
0050949c: beq #0x509840
005094a0: ldr r2, [pc, #0x540]
005094a4: mov r3, lr
005094a8: ldr lr, [sp, #0x38]
005094ac: add r5, sp, #0x78
005094b0: add r2, pc, r2
005094b4: mov r0, r5
005094b8: mov r1, #0x20
005094bc: str ip, [sp, #4]
005094c0: str lr, [sp]
005094c4: bl #0x30e244
005094c8: mov r0, r5
005094cc: bl #0x30de54
005094d0: mov r1, r5
005094d4: add r2, r5, r0
005094d8: mov r0, r7
005094dc: bl #0x310804
005094e0: bic r6, r6, #0x80000000
005094e4: movw r1, #0xb717
005094e8: mov r0, r6
005094ec: movt r1, #0x38d1
005094f0: bl #0x30e70c
005094f4: cmp r0, #0
005094f8: bne #0x509230
005094fc: mov r0, r7
00509500: ldr r1, [sp, #0x40]
00509504: bl #0x3f1b80
00509508: ldrsb r3, [r4, #-1]
0050950c: cmp r3, #0x6d
00509510: beq #0x5097f0
00509514: mov r0, r6
00509518: bl #0x30e8a4
0050951c: ldr r2, [sp, #0x44]
00509520: strd r0, r1, [sp]
00509524: mov r0, r5
00509528: mov r1, #0x10
0050952c: bl #0x30e244
00509530: add r1, r5, #2
00509534: mov r0, r7
00509538: bl #0x3f1b80
0050953c: b #0x509230
00509540: cmp r3, #0x64
00509544: beq #0x5097b4
00509548: cmp r3, #0x6b
0050954c: beq #0x5098d8
00509550: cmp r3, #0x70
00509554: beq #0x509934
00509558: ldr lr, [sp, #0x1c]
0050955c: ldr r0, [sp, #0x2c]
00509560: cmp lr, r0
00509564: blt #0x509750
00509568: ldr r1, [sp, #0x1c]
0050956c: movw r3, #0xde83
00509570: ldr r2, [sp, #0x1c]
00509574: movt r3, #0x431b
00509578: smull r1, r3, r3, r1
0050957c: ldr ip, [sp, #0x1c]
00509580: asr r1, r2, #0x1f
00509584: mov r2, #0xf4000
00509588: rsb r3, r1, r3, asr #18
0050958c: add r2, r2, #0x240
00509590: mls r2, r2, r3, ip
00509594: movw r0, #0x4dd3
00509598: mov lr, ip
0050959c: movt r0, #0x1062
005095a0: smull lr, ip, r0, lr
005095a4: smull lr, r0, r0, r2
005095a8: asr r2, r2, #0x1f
005095ac: rsb lr, r2, r0, asr #6
005095b0: ldr r0, [sp, #0x1c]
005095b4: rsb ip, r1, ip, asr #6
005095b8: mov r2, #0x3e8
005095bc: cmp r3, #0
005095c0: mls ip, r2, ip, r0
005095c4: bne #0x509880
005095c8: cmp lr, #0
005095cc: beq #0x509860
005095d0: ldr r2, [pc, #0x414]
005095d4: mov r3, lr
005095d8: ldr lr, [sp, #0x38]
005095dc: add r5, sp, #0x78
005095e0: add r2, pc, r2
005095e4: mov r0, r5
005095e8: mov r1, #0x20
005095ec: str ip, [sp, #4]
005095f0: str lr, [sp]
005095f4: bl #0x30e244
005095f8: b #0x509364
005095fc: ldr r2, [sp, #0x54]
00509600: add r3, r2, #4
00509604: str r3, [sp, #0x54]
00509608: ldr r8, [r2]
0050960c: add r3, r3, #4
00509610: str r3, [sp, #0x54]
00509614: ldrb r3, [r8]
00509618: ldr sb, [r2, #4]
0050961c: cmp r3, #0
00509620: beq #0x5096e8
00509624: add r5, sp, #0x78
00509628: mov r6, #0
0050962c: add r0, r5, #1
00509630: add r8, r8, #1
00509634: mov fp, r5
00509638: mov sl, r6
0050963c: str r0, [sp, #0x30]
00509640: b #0x5096ac
00509644: cmp r6, #0
00509648: bne #0x50970c
0050964c: ldrb r2, [sb]
00509650: cmp r2, #0
00509654: beq #0x509668
00509658: cmp r2, r3
0050965c: strbeq r3, [fp], #1
00509660: addeq sb, sb, #1
00509664: beq #0x509694
00509668: mov r1, #0
0050966c: strb r3, [fp]
00509670: strb r1, [fp, #1]
00509674: mov r0, r5
00509678: bl #0x30de54
0050967c: mov r1, r5
00509680: add r2, r5, r0
00509684: mov r0, r7
00509688: bl #0x310804
0050968c: mov fp, r5
00509690: mov sl, #0
00509694: ldrsb r3, [sb]
00509698: cmp r3, #0
0050969c: beq #0x50971c
005096a0: ldrb r3, [r8], #1
005096a4: cmp r3, #0
005096a8: beq #0x5096e8
005096ac: cmp sl, #0
005096b0: sub r1, r8, #1
005096b4: bne #0x509644
005096b8: sxtb r3, r3
005096bc: cmp r3, #0x24
005096c0: bne #0x50970c
005096c4: strb r3, [sp, #0x78]
005096c8: ldrsb r3, [sb]
005096cc: ldr fp, [sp, #0x30]
005096d0: mov sl, #1
005096d4: cmp r3, #0x24
005096d8: ldrb r3, [r8], #1
005096dc: addeq sb, sb, #1
005096e0: cmp r3, #0
005096e4: bne #0x5096ac
005096e8: mov r6, r3
005096ec: mov r8, #1
005096f0: b #0x50907c
005096f4: mov r0, r7
005096f8: mov r2, r4
005096fc: bl #0x310804
00509700: mov r8, #1
00509704: mov r6, #0
00509708: b #0x509238
0050970c: mov r0, r7
00509710: mov r2, r8
00509714: bl #0x310804
00509718: b #0x5096a0
0050971c: ldr r3, [sp, #0x54]
00509720: mov r6, #1
00509724: add r2, r3, #4
00509728: str r2, [sp, #0x54]
0050972c: ldr r1, [r3]
00509730: mov r0, r1
00509734: str r1, [sp, #0x10]
00509738: bl #0x30de54
0050973c: ldr r1, [sp, #0x10]
00509740: add r2, r1, r0
00509744: mov r0, r7
00509748: bl #0x310804
0050974c: b #0x5096a0
00509750: ldr r2, [pc, #0x298]
00509754: add r5, sp, #0x78
00509758: mov r0, r5
0050975c: add r2, pc, r2
00509760: mov r1, #0x20
00509764: mov r3, lr
00509768: bl #0x30e244
0050976c: b #0x509364
00509770: ldr r2, [pc, #0x27c]
00509774: add r5, sp, #0x78
00509778: mov r0, r5
0050977c: add r2, pc, r2
00509780: mov r1, #0x20
00509784: ldr r3, [sp, #0x1c]
00509788: bl #0x30e244
0050978c: b #0x5094c8
00509790: ldr r3, [sp, #0x54]
00509794: add r3, r3, #7
00509798: bic r3, r3, #7
0050979c: add r2, r3, #8
005097a0: str r2, [sp, #0x54]
005097a4: ldrd r0, r1, [r3]
005097a8: bl #0x30e6a0
005097ac: str r0, [sp, #0x20]
005097b0: b #0x5093b0
005097b4: ldr r3, [sp, #0x54]
005097b8: add r2, r3, #4
005097bc: str r2, [sp, #0x54]
005097c0: ldr r3, [r3]
005097c4: str r3, [sp, #0x1c]
005097c8: b #0x509558
005097cc: mov r1, #0
005097d0: ldr r0, [sp, #0x20]
005097d4: bl #0x30e70c
005097d8: cmp r0, #0
005097dc: movweq r1, #0xcccd
005097e0: movwne r1, #0xcccd
005097e4: movteq r1, #0x3d4c
005097e8: movtne r1, #0xbd4c
005097ec: b #0x5093dc
005097f0: mov r0, r6
005097f4: bl #0x30e8a4
005097f8: ldr r2, [sp, #0x4c]
005097fc: strd r0, r1, [sp]
00509800: mov r0, r5
00509804: mov r1, #0x10
00509808: bl #0x30e244
0050980c: b #0x509530
00509810: ldr r3, [sp, #0x54]
00509814: add r3, r3, #7
00509818: bic r3, r3, #7
0050981c: add r2, r3, #8
00509820: str r2, [sp, #0x54]
00509824: ldrd r0, r1, [r3]
00509828: bl #0x30e6a0
0050982c: mov r1, #0x42000000
00509830: add r1, r1, #0xc80000
00509834: bl #0x30ec94
00509838: str r0, [sp, #0x20]
0050983c: b #0x5093b0
00509840: ldr r2, [pc, #0x1b0]
00509844: add r5, sp, #0x78
00509848: mov r3, ip
0050984c: add r2, pc, r2
00509850: mov r0, r5
00509854: mov r1, #0x20
00509858: bl #0x30e244
0050985c: b #0x5094c8
00509860: ldr r2, [pc, #0x194]
00509864: add r5, sp, #0x78
00509868: mov r3, ip
0050986c: add r2, pc, r2
00509870: mov r0, r5
00509874: mov r1, #0x20
00509878: bl #0x30e244
0050987c: b #0x509364
00509880: ldr r2, [pc, #0x178]
00509884: str ip, [sp, #0xc]
00509888: ldr ip, [sp, #0x38]
0050988c: add r5, sp, #0x78
00509890: add r2, pc, r2
00509894: mov r0, r5
00509898: mov r1, #0x20
0050989c: stm sp, {ip, lr}
005098a0: str ip, [sp, #8]
005098a4: bl #0x30e244
005098a8: b #0x509364
005098ac: ldr r2, [pc, #0x150]
005098b0: str ip, [sp, #0xc]
005098b4: ldr ip, [sp, #0x38]
005098b8: add r5, sp, #0x78
005098bc: add r2, pc, r2
005098c0: mov r0, r5
005098c4: mov r1, #0x20
005098c8: stm sp, {ip, lr}
005098cc: str ip, [sp, #8]
005098d0: bl #0x30e244
005098d4: b #0x5094c8
005098d8: ldr r2, [sp, #0x54]
005098dc: movw r3, #0x4dd3
005098e0: movt r3, #0x1062
005098e4: add r1, r2, #4
005098e8: str r1, [sp, #0x54]
005098ec: ldr r2, [r2]
005098f0: smull ip, r3, r3, r2
005098f4: asr r2, r2, #0x1f
005098f8: rsb r2, r2, r3, asr #6
005098fc: str r2, [sp, #0x1c]
00509900: b #0x509558
00509904: ldr r3, [sp, #0x54]
00509908: add r3, r3, #7
0050990c: bic r3, r3, #7
00509910: add r2, r3, #8
00509914: str r2, [sp, #0x54]
00509918: ldrd r0, r1, [r3]
0050991c: bl #0x30e6a0
00509920: mov r1, #0x44000000
00509924: add r1, r1, #0x7a0000
00509928: bl #0x30ec94
0050992c: str r0, [sp, #0x20]
00509930: b #0x5093b0
00509934: ldr r3, [sp, #0x54]
00509938: add r2, r3, #4
0050993c: str r2, [sp, #0x54]
00509940: ldr r3, [r3]
00509944: mov r2, #0x64
00509948: mul r2, r2, r3
0050994c: str r2, [sp, #0x1c]
00509950: b #0x509558
00509954: ldr r3, [sp, #0x54]
00509958: add r3, r3, #7
0050995c: bic r3, r3, #7
00509960: add r2, r3, #8
00509964: str r2, [sp, #0x54]
00509968: ldrd r0, r1, [r3]
0050996c: bl #0x30e6a0
00509970: mov r1, #0x42000000
00509974: add r1, r1, #0xc80000
00509978: bl #0x30ed6c
0050997c: str r0, [sp, #0x20]
00509980: b #0x5093b0
00509984: ldr r3, [sp, #0x54]
00509988: add r3, r3, #7
0050998c: bic r3, r3, #7
00509990: add r2, r3, #8
00509994: str r2, [sp, #0x54]
00509998: ldrd r0, r1, [r3]
0050999c: bl #0x30e6a0
005099a0: mov r1, #0x40000000
005099a4: add r1, r1, #0xa00000
005099a8: bl #0x30ec94
005099ac: str r0, [sp, #0x20]
005099b0: b #0x5093b0
005099b4: bl #0x30e310
005099b8: subeq fp, r8, r8, lsl #23
005099bc: andeq r4, r0, ip, lsr #1
005099c0: strdeq r3, r4, [r0], -r4
005099c4: eorseq r5, fp, r4, lsr #25
005099c8: eorseq r2, sp, r8, lsl lr
005099cc: eorseq r2, sp, ip, lsl #28
005099d0: eorseq r2, sp, r8, lsl #28
005099d4: eorseq r2, sp, ip, ror #27
005099d8: eorseq r2, ip, r8, asr #19
005099dc: ldrshteq r2, [sp], -r4
005099e0: ldrsbteq r2, [sp], -ip
005099e4: eorseq r2, sp, ip, asr sb
005099e8: eorseq r2, sp, r8, ror #18
005099ec: eorseq r2, sp, r8, lsr r8
005099f0: eorseq r8, fp, r4, asr r7
005099f4: eorseq r8, fp, r4, lsr r7
005099f8: eorseq r8, fp, r4, ror #12
005099fc: eorseq r8, fp, r4, asr #12
00509a00: eorseq r2, sp, r8, ror r5
00509a04: eorseq r2, sp, ip, asr #10

# 0x43aa18 _Z19NativeShowStatusBarRKN7gameswf7fn_callE
0043aa18: push {r4, lr}
0043aa1c: ldr r3, [r0, #0xc]
0043aa20: ldr r2, [r0, #0x14]
0043aa24: mov r0, #0xc
0043aa28: ldr r3, [r3]
0043aa2c: ldr r4, [pc, #0x1c]
0043aa30: mla r0, r0, r2, r3
0043aa34: bl #0x797960
0043aa38: ldr r3, [pc, #0x14]
0043aa3c: add r4, pc, r4
0043aa40: mov r1, r0
0043aa44: ldr r0, [r4, r3]
0043aa48: pop {r4, lr}
0043aa4c: b #0x31f668
0043aa50: subseq sl, r5, r4, asr r0
0043aa54: strdeq r3, r4, [r0], -r4

# 0x4c7b2c _ZN7Structs31SkipAllCharMenuTutorialMessages8finalizeEv
004c7b2c: b #0x4c6c68

# 0x4c7af8 _ZN7Structs31SkipAllCharMenuTutorialMessagesD1Ev
004c7af8: ldr r3, [pc, #0x24]
004c7afc: ldr r2, [pc, #0x24]
004c7b00: push {r4, lr}
004c7b04: add r3, pc, r3
004c7b08: ldr r2, [r3, r2]
004c7b0c: mov r4, r0
004c7b10: add r2, r2, #8
004c7b14: str r2, [r0]
004c7b18: bl #0x4c6c60
004c7b1c: mov r0, r4
004c7b20: pop {r4, pc}
004c7b24: subeq ip, ip, ip, lsl #31
004c7b28: andeq r2, r0, r4, asr #29

# 0x4c7a58 _ZN7Structs27SkipCharMenuTutorialMessageD2Ev
004c7a58: ldr r3, [pc, #0x24]
004c7a5c: ldr r2, [pc, #0x24]
004c7a60: push {r4, lr}
004c7a64: add r3, pc, r3
004c7a68: ldr r2, [r3, r2]
004c7a6c: mov r4, r0
004c7a70: add r2, r2, #8
004c7a74: str r2, [r0]
004c7a78: bl #0x4c6c60
004c7a7c: mov r0, r4
004c7a80: pop {r4, pc}
004c7a84: subeq sp, ip, ip, lsr #32
004c7a88: andeq r0, r0, r0, lsl #24

# 0x4ff8a0 _ZN7Structs27SkipCharMenuTutorialMessage4readEP11IStreamBase
004ff8a0: b #0x4ff828

# 0x329ec4 _ZN18MenuMessageManagerI19CharMenuTutorialMsgLi1EED0Ev
00329ec4: ldr r3, [pc, #0x44]
00329ec8: ldr r2, [pc, #0x44]
00329ecc: push {r4, r5, r6, lr}
00329ed0: add r3, pc, r3
00329ed4: ldr r2, [r3, r2]
00329ed8: mov r4, r0
00329edc: mov r6, r0
00329ee0: add r2, r2, #8
00329ee4: add r5, r0, #4
00329ee8: str r2, [r4], #0x2c
00329eec: sub r4, r4, #0x28
00329ef0: mov r0, r4
00329ef4: bl #0x329e10
00329ef8: cmp r5, r4
00329efc: bne #0x329eec
00329f00: mov r0, r6
00329f04: bl #0x310440
00329f08: mov r0, r6
00329f0c: pop {r4, r5, r6, pc}
00329f10: rsbeq sl, r6, r0, asr #23
00329f14: andeq r1, r0, r8, ror r1

# 0x43a124 _Z19NativeSetMultitouchRKN7gameswf7fn_callE
0043a124: push {r4, lr}
0043a128: ldr r3, [r0, #0x10]
0043a12c: cmp r3, #1
0043a130: movne r4, #1
0043a134: beq #0x43a144
0043a138: bl #0x42ca8c
0043a13c: strb r4, [r0, #0x110]
0043a140: pop {r4, pc}
0043a144: ldr r3, [r0, #0xc]
0043a148: ldr r2, [r0, #0x14]
0043a14c: mov r0, #0xc
0043a150: ldr r3, [r3]
0043a154: mla r0, r0, r2, r3
0043a158: bl #0x797960
0043a15c: mov r4, r0
0043a160: bl #0x42ca8c
0043a164: strb r4, [r0, #0x110]
0043a168: pop {r4, pc}

# 0x424ef0 _ZN8MenuBase19FS_GetParsedString2EPKcS1_Pv
00424ef0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00424ef4: ldr r0, [pc, #0x538]
00424ef8: sub sp, sp, #0xac
00424efc: ldr r3, [pc, #0x534]
00424f00: str r0, [sp, #8]
00424f04: ldr r4, [sp, #8]
00424f08: str r3, [sp, #0x10]
00424f0c: subs r0, r1, #0
00424f10: add r4, pc, r4
00424f14: ldr r3, [r4, r3]
00424f18: str r4, [sp, #8]
00424f1c: str r2, [sp, #0x1c]
00424f20: ldr r3, [r3]
00424f24: str r3, [sp, #0xa4]
00424f28: beq #0x425264
00424f2c: ldr ip, [pc, #0x508]
00424f30: add r0, sp, #0x78
00424f34: str r0, [sp, #0xc]
00424f38: ldr r3, [r4, ip]
00424f3c: str ip, [sp, #0x20]
00424f40: mov r7, #0
00424f44: ldr r0, [r3, #0x34]
00424f48: bl #0x508c74
00424f4c: str r0, [sp, #0x2c]
00424f50: ldr r0, [sp, #0xc]
00424f54: mov r1, #0x10
00424f58: add r4, sp, #0x90
00424f5c: str r0, [sp, #0x88]
00424f60: str r0, [sp, #0x8c]
00424f64: bl #0x31167c
00424f68: ldr r3, [sp, #0x88]
00424f6c: strb r7, [r3]
00424f70: ldr ip, [sp, #0x1c]
00424f74: ldr r0, [ip, #4]
00424f78: strb r7, [sp, #0x64]
00424f7c: strb r7, [sp, #0x65]
00424f80: bl #0x7a7cac
00424f84: bl #0x774154
00424f88: ldr r3, [r0]
00424f8c: mov r5, r0
00424f90: mov r1, #0x10
00424f94: ldr r6, [r3, #0x20]
00424f98: mov r0, r4
00424f9c: mov r3, #1
00424fa0: strb r3, [sp, #0x90]
00424fa4: strb r7, [sp, #0x91]
00424fa8: bl #0x751d14
00424fac: ldrsb r3, [sp, #0x90]
00424fb0: ldr r1, [pc, #0x488]
00424fb4: mov r2, #0x11
00424fb8: cmn r3, #1
00424fbc: addne r0, r4, #1
00424fc0: ldreq r0, [sp, #0x9c]
00424fc4: add r1, pc, r1
00424fc8: bl #0x30e868
00424fcc: ldr r3, [sp, #0xa0]
00424fd0: mvn r2, #0
00424fd4: add r0, sp, #0x64
00424fd8: bfi r3, r2, #0, #0x18
00424fdc: lsr r2, r3, #0x18
00424fe0: str r0, [sp, #0x24]
00424fe4: bfc r2, #0, #1
00424fe8: str r3, [sp, #0xa0]
00424fec: mov r0, r5
00424ff0: strb r2, [sp, #0xa3]
00424ff4: mov r1, r4
00424ff8: ldr r2, [sp, #0x24]
00424ffc: blx r6
00425000: ldrsb r3, [sp, #0x90]
00425004: cmn r3, #1
00425008: beq #0x4253e4
0042500c: ldrsb r3, [sp, #0x65]
00425010: ldr r1, [pc, #0x42c]
00425014: ldr r2, [sp, #8]
00425018: cmp r3, #5
0042501c: ldreq r7, [sp, #0x68]
00425020: ldr r3, [r2, r1]
00425024: movne r7, #0
00425028: mov r4, #0
0042502c: add r3, r3, #8
00425030: cmp r7, #0
00425034: str r1, [sp, #0x28]
00425038: str r4, [sp, #0x40]
0042503c: str r4, [sp, #0x44]
00425040: str r3, [sp, #0x3c]
00425044: str r4, [sp, #0x48]
00425048: beq #0x4253f4
0042504c: add r0, sp, #0x58
00425050: ldr r1, [r7, #0x50]
00425054: bl #0x42257c
00425058: ldr r3, [r7, #0x50]
0042505c: cmp r3, r4
00425060: addle r3, sp, #0x3c
00425064: strle r3, [sp, #0x14]
00425068: ble #0x42517c
0042506c: add ip, sp, #0x3c
00425070: movw r0, #0x5555
00425074: add sb, sp, #0x4c
00425078: orr r0, r0, r0, lsl #14
0042507c: add r1, ip, #0xc
00425080: str ip, [sp, #0x14]
00425084: str r0, [sp, #0x18]
00425088: add sl, sp, #0x70
0042508c: mov r5, r4
00425090: add fp, sb, #4
00425094: str r1, [sp, #0x34]
00425098: mov r1, #0
0042509c: mov r0, #0xc
004250a0: bl #0x310570
004250a4: strb r5, [r0]
004250a8: strb r5, [r0, #1]
004250ac: ldr r2, [sp, #0x58]
004250b0: mov r3, r0
004250b4: mov r0, r4
004250b8: str r3, [r2, r4, lsl #2]
004250bc: ldr r3, [sp, #0x58]
004250c0: mov r2, #2
004250c4: ldr r6, [r3, r4, lsl #2]
004250c8: strb r2, [sp, #0x4d]
004250cc: strb r5, [sp, #0x4c]
004250d0: bl #0x30ed30
004250d4: strd r0, r1, [sp, #0x70]
004250d8: ldm sl, {r2, r3}
004250dc: mov r0, sb
004250e0: stm fp, {r2, r3}
004250e4: ldr r3, [r7]
004250e8: ldr r8, [r3, #0x20]
004250ec: bl #0x420a84
004250f0: mov r2, r6
004250f4: mov r1, r0
004250f8: mov r0, r7
004250fc: blx r8
00425100: cmp r0, #0
00425104: beq #0x425164
00425108: ldr r8, [sp, #0x44]
0042510c: ldr r3, [sp, #0x48]
00425110: cmp r8, r3
00425114: beq #0x425288
00425118: mov r3, #0
0042511c: str r3, [r8]
00425120: str r5, [r8, #8]
00425124: str r5, [r8, #4]
00425128: ldr r8, [sp, #0x44]
0042512c: add r8, r8, #0xc
00425130: str r8, [sp, #0x44]
00425134: mov r0, r6
00425138: bl #0x797a54
0042513c: bl #0x30e6a0
00425140: str r0, [r8, #-0xc]
00425144: mov r0, r6
00425148: bl #0x797a54
0042514c: bl #0x30ea24
00425150: sub r8, r8, #0xc
00425154: str r0, [r8, #4]
00425158: mov r0, r6
0042515c: bl #0x796fb4
00425160: str r0, [r8, #8]
00425164: mov r0, sb
00425168: bl #0x797124
0042516c: ldr r3, [r7, #0x50]
00425170: add r4, r4, #1
00425174: cmp r4, r3
00425178: blt #0x425098
0042517c: ldr r2, [sp, #8]
00425180: ldr r1, [sp, #0x20]
00425184: ldr r3, [r2, r1]
00425188: ldr r2, [sp, #0x2c]
0042518c: ldr r1, [sp, #0xc]
00425190: ldr r0, [r3, #0x34]
00425194: ldr r3, [sp, #0x14]
00425198: bl #0x509aec
0042519c: ldr r2, [sp, #0x5c]
004251a0: ldr r0, [sp, #0x58]
004251a4: rsb r3, r0, r2
004251a8: lsrs r3, r3, #2
004251ac: beq #0x4251e8
004251b0: mov r4, #0
004251b4: ldr r5, [r0, r4, lsl #2]
004251b8: cmp r5, #0
004251bc: beq #0x4251d8
004251c0: mov r0, r5
004251c4: bl #0x797124
004251c8: mov r0, r5
004251cc: bl #0x310440
004251d0: ldr r0, [sp, #0x58]
004251d4: ldr r2, [sp, #0x5c]
004251d8: add r4, r4, #1
004251dc: rsb r3, r0, r2
004251e0: cmp r4, r3, asr #2
004251e4: blo #0x4251b4
004251e8: cmp r2, r0
004251ec: strne r0, [sp, #0x5c]
004251f0: cmp r0, #0
004251f4: beq #0x425210
004251f8: ldr r1, [sp, #0x60]
004251fc: rsb r1, r0, r1
00425200: bic r1, r1, #3
00425204: cmp r1, #0x80
00425208: bhi #0x4253d0
0042520c: bl #0x708f00
00425210: ldr r1, [sp, #0x1c]
00425214: ldr r2, [pc, #0x22c]
00425218: ldr r3, [sp, #0x8c]
0042521c: ldr r0, [r1, #4]
00425220: ldr r1, [pc, #0x224]
00425224: add r2, pc, r2
00425228: add r1, pc, r1
0042522c: bl #0x7ab730
00425230: ldr r2, [sp, #0x28]
00425234: ldr r4, [sp, #8]
00425238: ldr ip, [sp, #0x14]
0042523c: ldr r3, [r4, r2]
00425240: add r0, ip, #4
00425244: add r3, r3, #8
00425248: str r3, [sp, #0x3c]
0042524c: bl #0x3fab18
00425250: ldr r0, [sp, #0x24]
00425254: bl #0x797124
00425258: ldr r0, [sp, #0xc]
0042525c: bl #0x318254
00425260: mov r0, #1
00425264: ldr r2, [sp, #8]
00425268: ldr r1, [sp, #0x10]
0042526c: ldr r3, [r2, r1]
00425270: ldr r2, [sp, #0xa4]
00425274: ldr r3, [r3]
00425278: cmp r2, r3
0042527c: bne #0x425430
00425280: add sp, sp, #0xac
00425284: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00425288: ldr r3, [sp, #0x40]
0042528c: ldr ip, [sp, #0x18]
00425290: rsb r3, r3, r8
00425294: asr r3, r3, #2
00425298: add r2, r3, r3, lsl #2
0042529c: add r2, r2, r2, lsl #4
004252a0: add r2, r2, r2, lsl #8
004252a4: add r2, r2, r2, lsl #16
004252a8: add r2, r3, r2, lsl #1
004252ac: cmp r2, #1
004252b0: addhs r3, r2, r2
004252b4: addlo r3, r2, #1
004252b8: cmp r3, ip
004252bc: bls #0x4253d8
004252c0: movw r3, #0x5555
004252c4: orr r3, r3, r3, lsl #14
004252c8: mov r1, r3
004252cc: mov r2, sl
004252d0: ldr r0, [sp, #0x34]
004252d4: str r3, [sp, #0x70]
004252d8: bl #0x3fac54
004252dc: ldr lr, [sp, #0x40]
004252e0: mov r3, r0
004252e4: rsb r8, lr, r8
004252e8: asr r8, r8, #2
004252ec: add r2, r8, r8, lsl #2
004252f0: add r2, r2, r2, lsl #4
004252f4: add r2, r2, r2, lsl #8
004252f8: add r2, r2, r2, lsl #16
004252fc: add r2, r8, r2, lsl #1
00425300: cmp r2, #0
00425304: str r2, [sp, #0x30]
00425308: movle r2, r0
0042530c: ble #0x425358
00425310: ldr ip, [sp, #0x30]
00425314: mov r2, #0
00425318: ldr r1, [lr, r2]
0042531c: add r0, lr, r2
00425320: add r0, r0, #4
00425324: str r1, [r3, r2]
00425328: ldr r8, [r0], #4
0042532c: add r1, r3, r2
00425330: add r1, r1, #4
00425334: str r8, [r1], #4
00425338: ldr r0, [r0]
0042533c: subs ip, ip, #1
00425340: add r2, r2, #0xc
00425344: str r0, [r1]
00425348: bne #0x425318
0042534c: ldr r0, [sp, #0x30]
00425350: mov r1, #0xc
00425354: mla r2, r1, r0, r3
00425358: str r5, [r2, #8]
0042535c: str r5, [r2, #4]
00425360: mov r8, r2
00425364: mov r2, #0
00425368: str r2, [r8], #0xc
0042536c: ldr r0, [sp, #0x40]
00425370: ldr r2, [sp, #0x48]
00425374: cmp r0, #0
00425378: beq #0x4253b4
0042537c: rsb r2, r0, r2
00425380: asr r2, r2, #2
00425384: mov ip, #0xc
00425388: add r1, r2, r2, lsl #2
0042538c: add r1, r1, r1, lsl #4
00425390: add r1, r1, r1, lsl #8
00425394: add r1, r1, r1, lsl #16
00425398: add r1, r2, r1, lsl #1
0042539c: mul r1, ip, r1
004253a0: cmp r1, #0x80
004253a4: bhi #0x425420
004253a8: str r3, [sp, #4]
004253ac: bl #0x708f00
004253b0: ldr r3, [sp, #4]
004253b4: ldr r2, [sp, #0x70]
004253b8: mov r0, #0xc
004253bc: str r3, [sp, #0x40]
004253c0: mla r3, r0, r2, r3
004253c4: str r8, [sp, #0x44]
004253c8: str r3, [sp, #0x48]
004253cc: b #0x425134
004253d0: bl #0x310440
004253d4: b #0x425210
004253d8: cmp r2, r3
004253dc: bls #0x4252c8
004253e0: b #0x4252c0
004253e4: ldr r0, [sp, #0x9c]
004253e8: ldr r1, [sp, #0x98]
004253ec: bl #0x752b38
004253f0: b #0x42500c
004253f4: ldr ip, [sp, #8]
004253f8: ldr r4, [sp, #0x20]
004253fc: add r0, sp, #0x3c
00425400: str r0, [sp, #0x14]
00425404: ldr r3, [ip, r4]
00425408: ldr r2, [sp, #0x2c]
0042540c: ldr r1, [sp, #0xc]
00425410: ldr r0, [r3, #0x34]
00425414: ldr r3, [sp, #0x14]
00425418: bl #0x509aec
0042541c: b #0x425210
00425420: str r3, [sp, #4]
00425424: bl #0x310440
00425428: ldr r3, [sp, #4]
0042542c: b #0x4253b4
00425430: bl #0x30e310
00425434: subseq pc, r6, r0, lsl #23
00425438: andeq r4, r0, ip, lsr #1
0042543c: strdeq r3, r4, [r0], -r4
00425440: subeq r4, sl, ip, asr #4
00425444: andeq r4, r0, r8, lsl #1
00425448: subeq r3, sl, ip, lsr #26
0042544c: ldrdeq sp, lr, [sb], #-0xf8

# 0x31f668 _ZN11Application13ShowStatubBarEb
0031f668: bx lr

# 0x431c38 _ZN11MenuManagerC1Ev
00431c38: push {r4, r5, r6, r7, r8, sb, sl, lr}
00431c3c: ldr r6, [pc, #0x238]
00431c40: ldr r2, [pc, #0x238]
00431c44: ldr r3, [pc, #0x238]
00431c48: add r6, pc, r6
00431c4c: ldr sb, [r6, r2]
00431c50: ldr r3, [r6, r3]
00431c54: sub sp, sp, #0x80
00431c58: ldr r2, [sb]
00431c5c: add r1, r3, #0x28
00431c60: add r3, r3, #8
00431c64: mov r4, r0
00431c68: str r1, [r0, #4]
00431c6c: str r3, [r0]
00431c70: add r0, r0, #0x20
00431c74: mov r5, #0
00431c78: str r2, [sp, #0x7c]
00431c7c: bl #0x431428
00431c80: mov r2, #0
00431c84: mov r3, r4
00431c88: str r2, [r4, #0x58]
00431c8c: str r2, [r4, #0x54]
00431c90: str r2, [r4, #0x50]
00431c94: mov r7, #1
00431c98: str r5, [r4, #0x5c]
00431c9c: str r5, [r4, #0x60]
00431ca0: str r5, [r4, #0x64]
00431ca4: str r5, [r4, #0x68]
00431ca8: str r5, [r4, #0x6c]
00431cac: str r5, [r4, #0x74]
00431cb0: strb r5, [r3, #0x70]!
00431cb4: str r3, [r4, #0x7c]
00431cb8: str r3, [r4, #0x78]
00431cbc: strb r7, [r4, #0xc4]
00431cc0: add r0, r4, #0xcc
00431cc4: str r5, [r4, #0x80]
00431cc8: strb r5, [r4, #0x88]
00431ccc: str r5, [r4, #0xac]
00431cd0: str r5, [r4, #0xb0]
00431cd4: str r5, [r4, #0xbc]
00431cd8: str r5, [r4, #0xc0]
00431cdc: str r5, [r4, #0xcc]
00431ce0: str r5, [r4, #0xd0]
00431ce4: str r5, [r4, #0xd4]
00431ce8: str r5, [r4, #0xd8]
00431cec: str r5, [r4, #0xdc]
00431cf0: str r5, [r4, #0xe0]
00431cf4: str r5, [r4, #0xe4]
00431cf8: str r5, [r4, #0xe8]
00431cfc: str r5, [r4, #0xec]
00431d00: str r5, [r4, #0xf0]
00431d04: bl #0x431948
00431d08: mvn r3, #0
00431d0c: str r3, [r4, #0x108]
00431d10: ldr r3, [pc, #0x170]
00431d14: str r5, [r4, #0x10c]
00431d18: strb r5, [r4, #0x110]
00431d1c: ldr r0, [r6, r3]
00431d20: bl #0x761170
00431d24: ldr r3, [pc, #0x160]
00431d28: mov r2, #0x3f800000
00431d2c: strb r7, [sp, #0x20]
00431d30: ldr r3, [r6, r3]
00431d34: str r2, [sp, #0x24]
00431d38: str r5, [sp, #4]
00431d3c: ldr r3, [r3, #0x10]
00431d40: str r5, [sp, #8]
00431d44: str r5, [sp, #0x10]
00431d48: str r5, [sp, #0x14]
00431d4c: str r5, [sp, #0xc]
00431d50: str r5, [sp, #0x18]
00431d54: str r5, [sp, #0x1c]
00431d58: ldr r1, [r3, #0x10]
00431d5c: ldr r3, [pc, #0x12c]
00431d60: add r0, sp, #4
00431d64: str r1, [sp, #4]
00431d68: ldr r2, [r6, r3]
00431d6c: mov r3, #0x200
00431d70: str r3, [sp, #0x10]
00431d74: str r2, [sp, #8]
00431d78: str r3, [sp, #0x14]
00431d7c: strb r5, [sp, #0x20]
00431d80: bl #0x7a9814
00431d84: ldr r3, [pc, #0x108]
00431d88: add sl, sp, #0x64
00431d8c: add r8, sp, #0x4c
00431d90: ldr r6, [r6, r3]
00431d94: add r7, sp, #0x34
00431d98: mov r0, r6
00431d9c: bl #0x337888
00431da0: ldr r1, [pc, #0xf0]
00431da4: add r2, sp, #0x30
00431da8: mov r0, sl
00431dac: add r1, pc, r1
00431db0: bl #0x3140ec
00431db4: mov r1, sl
00431db8: mov r2, r5
00431dbc: mov r0, r6
00431dc0: bl #0x337ddc
00431dc4: mov r0, sl
00431dc8: bl #0x318254
00431dcc: mov r0, r6
00431dd0: bl #0x337888
00431dd4: ldr r1, [pc, #0xc0]
00431dd8: add r2, sp, #0x2c
00431ddc: mov r0, r8
00431de0: add r1, pc, r1
00431de4: bl #0x3140ec
00431de8: mov r1, r8
00431dec: mov r2, r5
00431df0: mov r0, r6
00431df4: bl #0x337ddc
00431df8: mov r0, r8
00431dfc: bl #0x318254
00431e00: mov r0, r6
00431e04: bl #0x337888
00431e08: ldr r1, [pc, #0x90]
00431e0c: add r2, sp, #0x28
00431e10: mov r0, r7
00431e14: add r1, pc, r1
00431e18: bl #0x3140ec
00431e1c: mov r2, r5
00431e20: mov r0, r6
00431e24: mov r1, r7
00431e28: bl #0x337ddc
00431e2c: mov r0, r7
00431e30: bl #0x318254
00431e34: mov r1, #8
00431e38: mov r0, #0x164
00431e3c: bl #0x310570
00431e40: mov r6, r0
00431e44: bl #0x437e24
00431e48: str r6, [r4, #0xf4]
00431e4c: str r5, [r4, #0x104]
00431e50: str r5, [r4, #0xf8]
00431e54: str r5, [r4, #0xfc]
00431e58: str r5, [r4, #0x100]
00431e5c: ldr r2, [sp, #0x7c]
00431e60: ldr r3, [sb]
00431e64: mov r0, r4
00431e68: cmp r2, r3
00431e6c: bne #0x431e78
00431e70: add sp, sp, #0x80
00431e74: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00431e78: bl #0x30e310
00431e7c: subseq r2, r6, r8, asr #28
00431e80: andeq r4, r0, ip, lsr #1
00431e84: strdeq r3, r4, [r0], -ip
00431e88: andeq r2, r0, ip, lsl #13
00431e8c: strdeq r3, r4, [r0], -r4
00431e90: ldrdeq r3, r4, [r0], -r0
00431e94: andeq r0, r0, r4, lsl #17
00431e98: subeq lr, r8, r4, lsr #1
00431e9c: umaaleq lr, r8, r0, r0
00431ea0: subeq lr, r8, ip, ror r0

# 0x31fe38 _ZSt19__destroy_range_auxINSt4priv15_Deque_iteratorI19CharMenuTutorialMsgSt16_Nonconst_traitsIS2_EEES2_EvT_S6_PT0_RKSt12__false_type
0031fe38: push {r4, r5, r6, lr}
0031fe3c: ldr r5, [r0]
0031fe40: mov r4, r0
0031fe44: mov r6, r1
0031fe48: ldr r3, [r6]
0031fe4c: add r0, r5, #0x1c
0031fe50: cmp r5, r3
0031fe54: beq #0x31feac
0031fe58: bl #0x3139ac
0031fe5c: add r0, r5, #4
0031fe60: bl #0x3139ac
0031fe64: ldr r5, [r4]
0031fe68: ldr r3, [r4, #8]
0031fe6c: add r5, r5, #0x34
0031fe70: cmp r5, r3
0031fe74: str r5, [r4]
0031fe78: bne #0x31fe48
0031fe7c: ldr r3, [r4, #0xc]
0031fe80: add r2, r3, #4
0031fe84: str r2, [r4, #0xc]
0031fe88: ldr r5, [r3, #4]
0031fe8c: add r3, r5, #0x68
0031fe90: str r3, [r4, #8]
0031fe94: str r5, [r4, #4]
0031fe98: str r5, [r4]
0031fe9c: ldr r3, [r6]
0031fea0: add r0, r5, #0x1c
0031fea4: cmp r5, r3
0031fea8: bne #0x31fe58
0031feac: pop {r4, r5, r6, pc}

# 0x460428 _ZN38Script_SkipAllCharMenuTutorialMessages7ExecuteEbi
00460428: ldr r3, [pc, #0x3c]
0046042c: ldr r2, [pc, #0x3c]
00460430: push {r4, r5, r6, lr}
00460434: add r3, pc, r3
00460438: ldr r4, [r3, r2]
0046043c: ldr r2, [r4, #0x14]
00460440: ldr r3, [r4, #4]
00460444: cmp r2, r3
00460448: beq #0x460468
0046044c: add r5, r4, #4
00460450: mov r0, r5
00460454: bl #0x441d58
00460458: ldr r2, [r4, #0x14]
0046045c: ldr r3, [r4, #4]
00460460: cmp r2, r3
00460464: bne #0x460450
00460468: pop {r4, r5, r6, pc}
0046046c: subseq r4, r3, ip, asr r6
00460470: andeq r4, r0, r0, lsl #18

# 0x329e78 _ZN18MenuMessageManagerI19CharMenuTutorialMsgLi1EED1Ev
00329e78: ldr r3, [pc, #0x3c]
00329e7c: ldr r2, [pc, #0x3c]
00329e80: push {r4, r5, r6, lr}
00329e84: add r3, pc, r3
00329e88: ldr r2, [r3, r2]
00329e8c: mov r4, r0
00329e90: mov r6, r0
00329e94: add r2, r2, #8
00329e98: add r5, r0, #4
00329e9c: str r2, [r4], #0x2c
00329ea0: sub r4, r4, #0x28
00329ea4: mov r0, r4
00329ea8: bl #0x329e10
00329eac: cmp r5, r4
00329eb0: bne #0x329ea0
00329eb4: mov r0, r6
00329eb8: pop {r4, r5, r6, pc}
00329ebc: rsbeq sl, r6, ip, lsl #24
00329ec0: andeq r1, r0, r8, ror r1

# 0x4c7a8c _ZN7Structs27SkipCharMenuTutorialMessageD1Ev
004c7a8c: ldr r3, [pc, #0x24]
004c7a90: ldr r2, [pc, #0x24]
004c7a94: push {r4, lr}
004c7a98: add r3, pc, r3
004c7a9c: ldr r2, [r3, r2]
004c7aa0: mov r4, r0
004c7aa4: add r2, r2, #8
004c7aa8: str r2, [r0]
004c7aac: bl #0x4c6c60
004c7ab0: mov r0, r4
004c7ab4: pop {r4, pc}
004c7ab8: strdeq ip, sp, [ip], #-0xf8
004c7abc: andeq r0, r0, r0, lsl #24

# 0x4fff54 _ZN7Structs30EnqueueCharMenuTutorialMessage4readEP11IStreamBase
004fff54: push {r4, r5, r6, lr}
004fff58: mov r4, r0
004fff5c: sub sp, sp, #8
004fff60: mov r5, r1
004fff64: bl #0x4ff828
004fff68: mov r0, r5
004fff6c: add r1, r4, #8
004fff70: bl #0x3df1a0
004fff74: mov r3, #1
004fff78: cmp r3, #0
004fff7c: str r3, [sp, #4]
004fff80: bne #0x4fffc4
004fff84: add r3, r4, #9
004fff88: add r2, r4, #0xa
004fff8c: ldrb r0, [r2, #1]
004fff90: ldrb r1, [r3, #-1]
004fff94: cmp r3, r2
004fff98: eor r1, r0, r1
004fff9c: strb r1, [r3, #-1]
004fffa0: ldrb r0, [r2, #1]
004fffa4: eor r1, r1, r0
004fffa8: strb r1, [r2, #1]
004fffac: ldrb r0, [r3, #-1]
004fffb0: sub r2, r2, #1
004fffb4: eor r1, r1, r0
004fffb8: strb r1, [r3, #-1]
004fffbc: add r3, r3, #1
004fffc0: blo #0x4fff8c
004fffc4: ldr r0, [r4, #0xc]
004fffc8: cmp r0, #0
004fffcc: beq #0x4fffd4
004fffd0: bl #0x310440
004fffd4: ldr r0, [r4, #8]
004fffd8: mov r1, #1
004fffdc: mov r6, #0
004fffe0: add r0, r0, r1
004fffe4: bl #0x31056c
004fffe8: ldr r2, [r4, #8]
004fffec: mov r1, r0
004ffff0: str r0, [r4, #0xc]
004ffff4: mov r3, r6
004ffff8: mov r0, r5
004ffffc: bl #0x317454
00500000: ldr r3, [r4, #8]
00500004: ldr r2, [r4, #0xc]
00500008: mov r0, r5
0050000c: add r1, r4, #0x10
00500010: strb r6, [r2, r3]
00500014: bl #0x3df1a0
00500018: mov r3, #1
0050001c: cmp r3, r6
00500020: str r3, [sp, #4]
00500024: bne #0x500068
00500028: add r3, r4, #0x11
0050002c: add r2, r4, #0x12
00500030: ldrb r0, [r2, #1]
00500034: ldrb r1, [r3, #-1]
00500038: cmp r3, r2
0050003c: eor r1, r0, r1
00500040: strb r1, [r3, #-1]
00500044: ldrb r0, [r2, #1]
00500048: eor r1, r1, r0
0050004c: strb r1, [r2, #1]
00500050: ldrb r0, [r3, #-1]
00500054: sub r2, r2, #1
00500058: eor r1, r1, r0
0050005c: strb r1, [r3, #-1]
00500060: add r3, r3, #1
00500064: blo #0x500030
00500068: ldr r0, [r4, #0x14]
0050006c: cmp r0, #0
00500070: beq #0x500078
00500074: bl #0x310440
00500078: ldr r0, [r4, #0x10]
0050007c: mov r1, #1
00500080: mov r6, #0
00500084: add r0, r0, r1
00500088: bl #0x31056c
0050008c: ldr r2, [r4, #0x10]
00500090: mov r1, r0
00500094: str r0, [r4, #0x14]
00500098: mov r3, r6
0050009c: mov r0, r5
005000a0: bl #0x317454
005000a4: ldr r3, [r4, #0x10]
005000a8: ldr r2, [r4, #0x14]
005000ac: mov r0, r5
005000b0: add r1, r4, #0x18
005000b4: strb r6, [r2, r3]
005000b8: bl #0x459090
005000bc: mov r3, #1
005000c0: cmp r3, r6
005000c4: str r3, [sp, #4]
005000c8: bne #0x50010c
005000cc: add r3, r4, #0x1a
005000d0: add r4, r4, #0x19
005000d4: ldrb r1, [r3, #1]
005000d8: ldrb r2, [r4, #-1]
005000dc: cmp r3, r4
005000e0: eor r2, r1, r2
005000e4: strb r2, [r4, #-1]
005000e8: ldrb r1, [r3, #1]
005000ec: eor r2, r2, r1
005000f0: strb r2, [r3, #1]
005000f4: ldrb r1, [r4, #-1]
005000f8: sub r3, r3, #1
005000fc: eor r2, r2, r1
00500100: strb r2, [r4, #-1]
00500104: add r4, r4, #1
00500108: bhi #0x5000d4
0050010c: add sp, sp, #8
00500110: pop {r4, r5, r6, pc}

# 0x329d8c _ZNSt4priv11_Deque_baseI19CharMenuTutorialMsgSaIS1_EED2Ev
00329d8c: push {r4, r5, r6, lr}
00329d90: mov r6, r0
00329d94: ldr r0, [r0, #0x20]
00329d98: cmp r0, #0
00329d9c: beq #0x329df4
00329da0: ldr r5, [r6, #0x1c]
00329da4: ldr r4, [r6, #0xc]
00329da8: add r5, r5, #4
00329dac: cmp r4, r5
00329db0: bhs #0x329e08
00329db4: ldr r0, [r4]
00329db8: mov r1, #0x68
00329dbc: add r4, r4, #4
00329dc0: cmp r0, #0
00329dc4: beq #0x329dcc
00329dc8: bl #0x708f00
00329dcc: cmp r5, r4
00329dd0: bhi #0x329db4
00329dd4: ldr r0, [r6, #0x20]
00329dd8: ldr r1, [r6, #0x24]
00329ddc: cmp r0, #0
00329de0: beq #0x329df4
00329de4: lsl r1, r1, #2
00329de8: cmp r1, #0x80
00329dec: bhi #0x329dfc
00329df0: bl #0x708f00
00329df4: mov r0, r6
00329df8: pop {r4, r5, r6, pc}
00329dfc: bl #0x310440
00329e00: mov r0, r6
00329e04: pop {r4, r5, r6, pc}
00329e08: ldr r1, [r6, #0x24]
00329e0c: b #0x329de4

# 0x4d1340 _ZN7Structs30EnqueueCharMenuTutorialMessageD2Ev
004d1340: push {r4, lr}
004d1344: ldr r3, [pc, #0x44]
004d1348: ldr r2, [pc, #0x44]
004d134c: mov r4, r0
004d1350: add r3, pc, r3
004d1354: ldr r0, [r0, #0xc]
004d1358: ldr r2, [r3, r2]
004d135c: cmp r0, #0
004d1360: add r2, r2, #8
004d1364: str r2, [r4]
004d1368: beq #0x4d1370
004d136c: bl #0x310440
004d1370: ldr r0, [r4, #0x14]
004d1374: cmp r0, #0
004d1378: beq #0x4d1380
004d137c: bl #0x310440
004d1380: mov r0, r4
004d1384: bl #0x4c6c60
004d1388: mov r0, r4
004d138c: pop {r4, pc}
004d1390: subeq r3, ip, r0, asr #14
004d1394: andeq r3, r0, r8, asr #18

# 0x4319cc _ZN11MenuManagerC2Ev
004319cc: push {r4, r5, r6, r7, r8, sb, sl, lr}
004319d0: ldr r6, [pc, #0x238]
004319d4: ldr r2, [pc, #0x238]
004319d8: ldr r3, [pc, #0x238]
004319dc: add r6, pc, r6
004319e0: ldr sb, [r6, r2]
004319e4: ldr r3, [r6, r3]
004319e8: sub sp, sp, #0x80
004319ec: ldr r2, [sb]
004319f0: add r1, r3, #0x28
004319f4: add r3, r3, #8
004319f8: mov r4, r0
004319fc: str r1, [r0, #4]
00431a00: str r3, [r0]
00431a04: add r0, r0, #0x20
00431a08: mov r5, #0
00431a0c: str r2, [sp, #0x7c]
00431a10: bl #0x431428
00431a14: mov r2, #0
00431a18: mov r3, r4
00431a1c: str r2, [r4, #0x58]
00431a20: str r2, [r4, #0x54]
00431a24: str r2, [r4, #0x50]
00431a28: mov r7, #1
00431a2c: str r5, [r4, #0x5c]
00431a30: str r5, [r4, #0x60]
00431a34: str r5, [r4, #0x64]
00431a38: str r5, [r4, #0x68]
00431a3c: str r5, [r4, #0x6c]
00431a40: str r5, [r4, #0x74]
00431a44: strb r5, [r3, #0x70]!
00431a48: str r3, [r4, #0x7c]
00431a4c: str r3, [r4, #0x78]
00431a50: strb r7, [r4, #0xc4]
00431a54: add r0, r4, #0xcc
00431a58: str r5, [r4, #0x80]
00431a5c: strb r5, [r4, #0x88]
00431a60: str r5, [r4, #0xac]
00431a64: str r5, [r4, #0xb0]
00431a68: str r5, [r4, #0xbc]
00431a6c: str r5, [r4, #0xc0]
00431a70: str r5, [r4, #0xcc]
00431a74: str r5, [r4, #0xd0]
00431a78: str r5, [r4, #0xd4]
00431a7c: str r5, [r4, #0xd8]
00431a80: str r5, [r4, #0xdc]
00431a84: str r5, [r4, #0xe0]
00431a88: str r5, [r4, #0xe4]
00431a8c: str r5, [r4, #0xe8]
00431a90: str r5, [r4, #0xec]
00431a94: str r5, [r4, #0xf0]
00431a98: bl #0x431948
00431a9c: mvn r3, #0
00431aa0: str r3, [r4, #0x108]
00431aa4: ldr r3, [pc, #0x170]
00431aa8: str r5, [r4, #0x10c]
00431aac: strb r5, [r4, #0x110]
00431ab0: ldr r0, [r6, r3]
00431ab4: bl #0x761170
00431ab8: ldr r3, [pc, #0x160]
00431abc: mov r2, #0x3f800000
00431ac0: strb r7, [sp, #0x20]
00431ac4: ldr r3, [r6, r3]
00431ac8: str r2, [sp, #0x24]
00431acc: str r5, [sp, #4]
00431ad0: ldr r3, [r3, #0x10]
00431ad4: str r5, [sp, #8]
00431ad8: str r5, [sp, #0x10]
00431adc: str r5, [sp, #0x14]
00431ae0: str r5, [sp, #0xc]
00431ae4: str r5, [sp, #0x18]
00431ae8: str r5, [sp, #0x1c]
00431aec: ldr r1, [r3, #0x10]
00431af0: ldr r3, [pc, #0x12c]
00431af4: add r0, sp, #4
00431af8: str r1, [sp, #4]
00431afc: ldr r2, [r6, r3]
00431b00: mov r3, #0x200
00431b04: str r3, [sp, #0x10]
00431b08: str r2, [sp, #8]
00431b0c: str r3, [sp, #0x14]
00431b10: strb r5, [sp, #0x20]
00431b14: bl #0x7a9814
00431b18: ldr r3, [pc, #0x108]
00431b1c: add sl, sp, #0x64
00431b20: add r8, sp, #0x4c
00431b24: ldr r6, [r6, r3]
00431b28: add r7, sp, #0x34
00431b2c: mov r0, r6
00431b30: bl #0x337888
00431b34: ldr r1, [pc, #0xf0]
00431b38: add r2, sp, #0x30
00431b3c: mov r0, sl
00431b40: add r1, pc, r1
00431b44: bl #0x3140ec
00431b48: mov r1, sl
00431b4c: mov r2, r5
00431b50: mov r0, r6
00431b54: bl #0x337ddc
00431b58: mov r0, sl
00431b5c: bl #0x318254
00431b60: mov r0, r6
00431b64: bl #0x337888
00431b68: ldr r1, [pc, #0xc0]
00431b6c: add r2, sp, #0x2c
00431b70: mov r0, r8
00431b74: add r1, pc, r1
00431b78: bl #0x3140ec
00431b7c: mov r1, r8
00431b80: mov r2, r5
00431b84: mov r0, r6
00431b88: bl #0x337ddc
00431b8c: mov r0, r8
00431b90: bl #0x318254
00431b94: mov r0, r6
00431b98: bl #0x337888
00431b9c: ldr r1, [pc, #0x90]
00431ba0: add r2, sp, #0x28
00431ba4: mov r0, r7
00431ba8: add r1, pc, r1
00431bac: bl #0x3140ec
00431bb0: mov r2, r5
00431bb4: mov r0, r6
00431bb8: mov r1, r7
00431bbc: bl #0x337ddc
00431bc0: mov r0, r7
00431bc4: bl #0x318254
00431bc8: mov r1, #8
00431bcc: mov r0, #0x164
00431bd0: bl #0x310570
00431bd4: mov r6, r0
00431bd8: bl #0x437e24
00431bdc: str r6, [r4, #0xf4]
00431be0: str r5, [r4, #0x104]
00431be4: str r5, [r4, #0xf8]
00431be8: str r5, [r4, #0xfc]
00431bec: str r5, [r4, #0x100]
00431bf0: ldr r2, [sp, #0x7c]
00431bf4: ldr r3, [sb]
00431bf8: mov r0, r4
00431bfc: cmp r2, r3
00431c00: bne #0x431c0c
00431c04: add sp, sp, #0x80
00431c08: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00431c0c: bl #0x30e310
00431c10: ldrheq r3, [r6], #-4
00431c14: andeq r4, r0, ip, lsr #1
00431c18: strdeq r3, r4, [r0], -ip
00431c1c: andeq r2, r0, ip, lsl #13
00431c20: strdeq r3, r4, [r0], -r4
00431c24: ldrdeq r3, r4, [r0], -r0
00431c28: andeq r0, r0, r4, lsl #17
00431c2c: subeq lr, r8, r0, lsl r3
00431c30: strdeq lr, pc, [r8], #-0x2c
00431c34: subeq lr, r8, r8, ror #5

# 0x44996c _Z19NativeGetNumPotionsRKN7gameswf7fn_callE
0044996c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00449970: ldr r4, [pc, #0x184]
00449974: ldr r8, [pc, #0x184]
00449978: ldr r2, [r0, #0xc]
0044997c: add r4, pc, r4
00449980: ldr r1, [r4, r8]
00449984: sub sp, sp, #0x50
00449988: ldr r3, [r0, #0x14]
0044998c: ldr r1, [r1]
00449990: str r1, [sp, #0x4c]
00449994: ldr r2, [r2]
00449998: mov r1, #0xc
0044999c: mla r3, r1, r3, r2
004499a0: mov r1, #0
004499a4: ldrsb r2, [r3, #1]
004499a8: cmp r2, #5
004499ac: ldreq r5, [r3, #4]
004499b0: ldr r3, [pc, #0x14c]
004499b4: mov r2, #1
004499b8: movne r5, #0
004499bc: ldr r3, [r4, r3]
004499c0: ldr r0, [r3, #0x40]
004499c4: bl #0x36e478
004499c8: ldr r6, [r0, #0x660]
004499cc: cmp r6, #0
004499d0: beq #0x449abc
004499d4: ldr r1, [pc, #0x12c]
004499d8: ldr r3, [r5]
004499dc: add sb, sp, #0x38
004499e0: add r1, pc, r1
004499e4: mov r0, sb
004499e8: ldr sl, [r3, #0x1c]
004499ec: bl #0x413a7c
004499f0: add r0, r6, #0x37c
004499f4: bl #0x3fc690
004499f8: mov r3, #0
004499fc: strb r3, [sp, #0xc]
00449a00: mov r3, #2
00449a04: strb r3, [sp, #0xd]
00449a08: bl #0x30ed30
00449a0c: strd r0, r1, [sp, #0x18]
00449a10: ldr r3, [sp, #0x18]
00449a14: add r7, sp, #0xc
00449a18: mov r1, sb
00449a1c: str r3, [sp, #0x10]
00449a20: ldr r3, [sp, #0x1c]
00449a24: mov r2, r7
00449a28: mov r0, r5
00449a2c: str r3, [r7, #8]
00449a30: blx sl
00449a34: mov r0, r7
00449a38: bl #0x797124
00449a3c: ldrsb r3, [sp, #0x38]
00449a40: cmn r3, #1
00449a44: beq #0x449ad8
00449a48: ldr r1, [pc, #0xbc]
00449a4c: ldr r3, [r5]
00449a50: add sl, sp, #0x24
00449a54: add r1, pc, r1
00449a58: mov r0, sl
00449a5c: ldr r7, [r3, #0x1c]
00449a60: bl #0x413a7c
00449a64: ldrb r0, [r6, #0x3a8]
00449a68: mov r3, #0
00449a6c: strb r3, [sp]
00449a70: sxtb r0, r0
00449a74: mov r3, #2
00449a78: strb r3, [sp, #1]
00449a7c: bl #0x30ed30
00449a80: strd r0, r1, [sp, #0x18]
00449a84: ldr r3, [sp, #0x18]
00449a88: mov r1, sl
00449a8c: mov r2, sp
00449a90: str r3, [sp, #4]
00449a94: ldr r3, [sp, #0x1c]
00449a98: mov r0, r5
00449a9c: mov r6, sp
00449aa0: str r3, [sp, #8]
00449aa4: blx r7
00449aa8: mov r0, sp
00449aac: bl #0x797124
00449ab0: ldrsb r3, [sp, #0x24]
00449ab4: cmn r3, #1
00449ab8: beq #0x449ae8
00449abc: ldr r3, [r4, r8]
00449ac0: ldr r2, [sp, #0x4c]
00449ac4: ldr r3, [r3]
00449ac8: cmp r2, r3
00449acc: bne #0x449af8
00449ad0: add sp, sp, #0x50
00449ad4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00449ad8: ldr r0, [sp, #0x44]
00449adc: ldr r1, [sp, #0x40]
00449ae0: bl #0x752b38
00449ae4: b #0x449a48
00449ae8: ldr r0, [sp, #0x30]
00449aec: ldr r1, [sp, #0x2c]
00449af0: bl #0x752b38
00449af4: b #0x449abc
00449af8: bl #0x30e310
00449afc: subseq fp, r4, r4, lsl r1
00449b00: andeq r4, r0, ip, lsr #1
00449b04: strdeq r3, r4, [r0], -r4
00449b08: subeq r2, r8, r0, asr #26
00449b0c: ldrdeq r2, r3, [r8], #-0xcc

# 0x456488 _Z27GetNewScriptCmdDataInstanceIN7Structs27SkipCharMenuTutorialMessageEEPNS0_9ScriptCmdEv
00456488: push {r4, lr}
0045648c: mov r1, #0
00456490: mov r0, #8
00456494: bl #0x310570
00456498: ldr r4, [pc, #0x1c]
0045649c: ldr r3, [pc, #0x1c]
004564a0: mov r1, #0
004564a4: add r4, pc, r4
004564a8: ldr r3, [r4, r3]
004564ac: str r1, [r0, #4]
004564b0: add r3, r3, #8
004564b4: str r3, [r0]
004564b8: pop {r4, pc}
004564bc: subseq lr, r3, ip, ror #11
004564c0: andeq r0, r0, r0, lsl #24

# 0x45644c _Z27GetNewScriptCmdDataInstanceIN7Structs31SkipAllCharMenuTutorialMessagesEEPNS0_9ScriptCmdEv
0045644c: push {r4, lr}
00456450: mov r1, #0
00456454: mov r0, #8
00456458: bl #0x310570
0045645c: ldr r4, [pc, #0x1c]
00456460: ldr r3, [pc, #0x1c]
00456464: mov r1, #0
00456468: add r4, pc, r4
0045646c: ldr r3, [r4, r3]
00456470: str r1, [r0, #4]
00456474: add r3, r3, #8
00456478: str r3, [r0]
0045647c: pop {r4, pc}
00456480: subseq lr, r3, r8, lsr #12
00456484: andeq r2, r0, r4, asr #29

# 0x45b674 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EE18_M_push_back_aux_vERKS0_
0045b674: push {r4, r5, r6, r7, r8, sb, sl, lr}
0045b678: ldr sl, [r0, #0x1c]
0045b67c: ldr r2, [r0, #0x20]
0045b680: ldr r3, [r0, #0x24]
0045b684: mov r5, r1
0045b688: rsb r1, r2, sl
0045b68c: sub r1, r3, r1, asr #2
0045b690: cmp r1, #1
0045b694: mov r4, r0
0045b698: bls #0x45b6d8
0045b69c: add r0, r4, #0x24
0045b6a0: bl #0x45b654
0045b6a4: str r0, [sl, #4]
0045b6a8: mov r1, r5
0045b6ac: ldr r0, [r4, #0x10]
0045b6b0: bl #0x45a780
0045b6b4: ldr r3, [r4, #0x1c]
0045b6b8: add r2, r3, #4
0045b6bc: str r2, [r4, #0x1c]
0045b6c0: ldr r3, [r3, #4]
0045b6c4: add r2, r3, #0x68
0045b6c8: str r3, [r4, #0x10]
0045b6cc: str r2, [r4, #0x18]
0045b6d0: str r3, [r4, #0x14]
0045b6d4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045b6d8: ldr r1, [r0, #0xc]
0045b6dc: rsb r7, r1, sl
0045b6e0: asr r7, r7, #2
0045b6e4: add r7, r7, #1
0045b6e8: add sb, r7, #1
0045b6ec: cmp r3, sb, lsl #1
0045b6f0: bls #0x45b758
0045b6f4: rsb r6, sb, r3
0045b6f8: lsr r6, r6, #1
0045b6fc: add r6, r2, r6, lsl #2
0045b700: cmp r1, r6
0045b704: bhi #0x45b7dc
0045b708: add sl, sl, #4
0045b70c: rsb r2, r1, sl
0045b710: cmp r2, #0
0045b714: ble #0x45b724
0045b718: add r0, r6, r7, lsl #2
0045b71c: rsb r0, r2, r0
0045b720: bl #0x30df38
0045b724: str r6, [r4, #0xc]
0045b728: ldr r3, [r6]
0045b72c: sub r7, r7, #1
0045b730: add sl, r6, r7, lsl #2
0045b734: add r2, r3, #0x68
0045b738: str r2, [r4, #8]
0045b73c: str r3, [r4, #4]
0045b740: str sl, [r4, #0x1c]
0045b744: ldr r3, [r6, r7, lsl #2]
0045b748: add r2, r3, #0x68
0045b74c: str r2, [r4, #0x18]
0045b750: str r3, [r4, #0x14]
0045b754: b #0x45b69c
0045b758: cmp r3, #0
0045b75c: movne r2, r3
0045b760: moveq r2, #1
0045b764: add r8, r3, #2
0045b768: add r8, r8, r2
0045b76c: mov r1, r8
0045b770: mov r2, #0
0045b774: add r0, r0, #0x20
0045b778: bl #0x3293c8
0045b77c: ldr r2, [r4, #0x1c]
0045b780: ldr r1, [r4, #0xc]
0045b784: rsb r6, sb, r8
0045b788: lsr r6, r6, #1
0045b78c: add r2, r2, #4
0045b790: subs r2, r2, r1
0045b794: mov sl, r0
0045b798: add r6, r0, r6, lsl #2
0045b79c: beq #0x45b7a8
0045b7a0: mov r0, r6
0045b7a4: bl #0x30df38
0045b7a8: ldr r0, [r4, #0x20]
0045b7ac: ldr r1, [r4, #0x24]
0045b7b0: cmp r0, #0
0045b7b4: beq #0x45b7c8
0045b7b8: lsl r1, r1, #2
0045b7bc: cmp r1, #0x80
0045b7c0: bhi #0x45b7d4
0045b7c4: bl #0x708f00
0045b7c8: str sl, [r4, #0x20]
0045b7cc: str r8, [r4, #0x24]
0045b7d0: b #0x45b724
0045b7d4: bl #0x310440
0045b7d8: b #0x45b7c8
0045b7dc: add r2, sl, #4
0045b7e0: subs r2, r2, r1
0045b7e4: beq #0x45b724
0045b7e8: mov r0, r6
0045b7ec: bl #0x30df38
0045b7f0: b #0x45b724

# 0x433b44 _ZN19CharMenuTutorialMsgC1EiRKSsS1_
00433b44: push {r4, r5, r6, lr}
00433b48: mov r4, r0
00433b4c: mov r5, r3
00433b50: str r1, [r0], #4
00433b54: mov r1, r2
00433b58: bl #0x32b918
00433b5c: mov r1, r5
00433b60: add r0, r4, #0x1c
00433b64: bl #0x32b918
00433b68: mov r0, r4
00433b6c: pop {r4, r5, r6, pc}

# 0x45787c _Z27GetNewScriptCmdImplInstanceI37Script_EnqueueCharMenuTutorialMessageEP13ScriptCmdImplv
0045787c: push {r4, lr}
00457880: mov r1, #0
00457884: mov r0, #0x10
00457888: bl #0x310570
0045788c: ldr r4, [pc, #0x28]
00457890: ldr r2, [pc, #0x28]
00457894: mov r1, #0
00457898: add r4, pc, r4
0045789c: ldr r2, [r4, r2]
004578a0: str r1, [r0, #0xc]
004578a4: strb r1, [r0, #4]
004578a8: add r2, r2, #8
004578ac: str r2, [r0]
004578b0: mvn r2, #0
004578b4: str r2, [r0, #8]
004578b8: pop {r4, pc}
004578bc: ldrsheq sp, [r3], #-0x18
004578c0: andeq r3, r0, r8, ror #6

# 0x4d1324 _ZN7Structs30EnqueueCharMenuTutorialMessageD0Ev
004d1324: push {r4, lr}
004d1328: mov r4, r0
004d132c: bl #0x4d12cc
004d1330: mov r0, r4
004d1334: bl #0x310440
004d1338: mov r0, r4
004d133c: pop {r4, pc}

# 0x329e10 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EED1Ev
00329e10: push {r4, r5, r6, r7, r8, sb, sl, lr}
00329e14: ldr ip, [r0, #0x18]
00329e18: ldr lr, [r0, #0x1c]
00329e1c: ldm r0, {r5, r6, r7, sb}
00329e20: ldr sl, [r0, #0x10]
00329e24: ldr r8, [r0, #0x14]
00329e28: sub sp, sp, #0x28
00329e2c: mov r4, r0
00329e30: add r1, sp, #4
00329e34: mov r2, #0
00329e38: add r3, sp, #0x24
00329e3c: add r0, sp, #0x14
00329e40: str lr, [sp, #0x10]
00329e44: str ip, [sp, #0xc]
00329e48: str sb, [sp, #0x20]
00329e4c: str r7, [sp, #0x1c]
00329e50: str r6, [sp, #0x18]
00329e54: str r5, [sp, #0x14]
00329e58: str r8, [sp, #8]
00329e5c: str sl, [sp, #4]
00329e60: bl #0x31fe38
00329e64: mov r0, r4
00329e68: bl #0x329d8c
00329e6c: mov r0, r4
00329e70: add sp, sp, #0x28
00329e74: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x455960 _ZNK38Script_SkipAllCharMenuTutorialMessages10IsBlockingEv
00455960: mov r0, #0
00455964: bx lr

# 0x4d12cc _ZN7Structs30EnqueueCharMenuTutorialMessageD1Ev
004d12cc: push {r4, lr}
004d12d0: ldr r3, [pc, #0x44]
004d12d4: ldr r2, [pc, #0x44]
004d12d8: mov r4, r0
004d12dc: add r3, pc, r3
004d12e0: ldr r0, [r0, #0xc]
004d12e4: ldr r2, [r3, r2]
004d12e8: cmp r0, #0
004d12ec: add r2, r2, #8
004d12f0: str r2, [r4]
004d12f4: beq #0x4d12fc
004d12f8: bl #0x310440
004d12fc: ldr r0, [r4, #0x14]
004d1300: cmp r0, #0
004d1304: beq #0x4d130c
004d1308: bl #0x310440
004d130c: mov r0, r4
004d1310: bl #0x4c6c60
004d1314: mov r0, r4
004d1318: pop {r4, pc}
004d131c: strheq r3, [ip], #-0x74
004d1320: andeq r3, r0, r8, asr #18

# 0x44de4c _Z21NativeGetParsedStringRKN7gameswf7fn_callE
0044de4c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044de50: ldr r4, [pc, #0x408]
0044de54: ldr r8, [pc, #0x408]
0044de58: ldr r2, [r0, #0x10]
0044de5c: add r4, pc, r4
0044de60: ldr r3, [r4, r8]
0044de64: sub sp, sp, #0xac
0044de68: cmp r2, #2
0044de6c: ldr r3, [r3]
0044de70: mov r7, r0
0044de74: str r3, [sp, #0xa4]
0044de78: beq #0x44de98
0044de7c: ldr r3, [r4, r8]
0044de80: ldr r2, [sp, #0xa4]
0044de84: ldr r3, [r3]
0044de88: cmp r2, r3
0044de8c: bne #0x44e25c
0044de90: add sp, sp, #0xac
0044de94: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044de98: ldr r3, [r0, #0xc]
0044de9c: ldr r2, [r0, #0x14]
0044dea0: mov r5, #0xc
0044dea4: ldr r3, [r3]
0044dea8: mla r0, r5, r2, r3
0044deac: ldrb r1, [r0, #1]
0044deb0: sub r1, r1, #3
0044deb4: uxtb r1, r1
0044deb8: cmp r1, #1
0044debc: bhi #0x44de7c
0044dec0: sub r2, r2, #1
0044dec4: mla r3, r5, r2, r3
0044dec8: ldrsb r3, [r3, #1]
0044decc: cmp r3, #5
0044ded0: bne #0x44de7c
0044ded4: bl #0x796f5c
0044ded8: ldr r3, [r7, #0xc]
0044dedc: ldr r2, [r7, #0x14]
0044dee0: mov sl, r0
0044dee4: ldr r3, [r3]
0044dee8: sub r2, r2, #1
0044deec: mla r5, r5, r2, r3
0044def0: ldrsb r3, [r5, #1]
0044def4: cmp r3, #5
0044def8: movne r0, #0
0044defc: ldreq r0, [r5, #4]
0044df00: bl #0x439ce8
0044df04: cmp sl, #0
0044df08: mov r6, r0
0044df0c: beq #0x44de7c
0044df10: ldr r0, [pc, #0x350]
0044df14: ldr r1, [pc, #0x350]
0044df18: add r2, sp, #0x78
0044df1c: ldr r3, [r4, r0]
0044df20: str r0, [sp, #0x18]
0044df24: str r1, [sp, #0x1c]
0044df28: ldr r0, [r3, #0x34]
0044df2c: mov r1, sl
0044df30: str r2, [sp, #0x10]
0044df34: bl #0x508c74
0044df38: str r0, [sp, #0x28]
0044df3c: ldr r0, [sp, #0x10]
0044df40: mov r1, #0x10
0044df44: mov r5, #0
0044df48: str r0, [sp, #0x88]
0044df4c: str r0, [sp, #0x8c]
0044df50: bl #0x31167c
0044df54: ldr sl, [sp, #0x1c]
0044df58: ldr r2, [sp, #0x88]
0044df5c: add r0, sp, #0x5c
0044df60: ldr r3, [r4, sl]
0044df64: strb r5, [r2]
0044df68: str r5, [sp, #0x44]
0044df6c: add r3, r3, #8
0044df70: str r3, [sp, #0x40]
0044df74: str r5, [sp, #0x48]
0044df78: str r5, [sp, #0x4c]
0044df7c: ldr r1, [r6, #0x50]
0044df80: cmp r1, #1
0044df84: movlt r1, #1
0044df88: bl #0x42257c
0044df8c: ldr r3, [r6, #0x50]
0044df90: cmp r3, r5
0044df94: addle fp, sp, #0x40
0044df98: strle fp, [sp, #0x14]
0044df9c: ble #0x44e0d4
0044dfa0: add r0, sp, #0x40
0044dfa4: add r2, sp, #0x50
0044dfa8: add r1, sp, #0x90
0044dfac: add r3, r2, #4
0044dfb0: add sl, r0, #4
0044dfb4: str r0, [sp, #0x14]
0044dfb8: add fp, sp, #0x70
0044dfbc: add r0, r1, #1
0044dfc0: str r7, [sp, #0x34]
0044dfc4: str r1, [sp, #0x24]
0044dfc8: str sl, [sp, #0x2c]
0044dfcc: add sb, r3, #4
0044dfd0: str fp, [sp, #0x30]
0044dfd4: str r0, [sp, #0x20]
0044dfd8: str r4, [sp, #0x38]
0044dfdc: str r8, [sp, #0x3c]
0044dfe0: mov r7, r2
0044dfe4: str r3, [sp, #4]
0044dfe8: b #0x44e004
0044dfec: cmp r3, #2
0044dff0: beq #0x44e1a8
0044dff4: ldr r3, [r6, #0x50]
0044dff8: add r5, r5, #1
0044dffc: cmp r5, r3
0044e000: bge #0x44e0c8
0044e004: mov r1, #0
0044e008: mov r0, #0xc
0044e00c: bl #0x310570
0044e010: mov r8, #0
0044e014: strb r8, [r0]
0044e018: strb r8, [r0, #1]
0044e01c: ldr r2, [sp, #0x5c]
0044e020: mov r3, r0
0044e024: mov r1, r5
0044e028: str r3, [r2, r5, lsl #2]
0044e02c: ldr r2, [sp, #0x5c]
0044e030: ldr r3, [r6]
0044e034: mov r0, r6
0044e038: ldr r4, [r2, r5, lsl #2]
0044e03c: lsl sl, r5, #2
0044e040: mov r2, r4
0044e044: mov lr, pc
0044e048: ldr pc, [r3, #0x28]
0044e04c: ldr r1, [sp, #0x48]
0044e050: ldr r3, [sp, #0x4c]
0044e054: mov r2, #0
0044e058: str r8, [sp, #0x58]
0044e05c: cmp r1, r3
0044e060: str r2, [sp, #0x50]
0044e064: str r8, [sp, #0x54]
0044e068: beq #0x44e194
0044e06c: ldr r2, [r7]
0044e070: mov r3, r1
0044e074: str r2, [r3], #4
0044e078: ldr fp, [sp, #4]
0044e07c: ldr r2, [fp]
0044e080: str r2, [r1, #4]
0044e084: ldr r2, [sb]
0044e088: str r2, [r3, #4]
0044e08c: ldr r8, [sp, #0x48]
0044e090: add r8, r8, #0xc
0044e094: str r8, [sp, #0x48]
0044e098: ldr r3, [sp, #0x5c]
0044e09c: sub r8, r8, #0xc
0044e0a0: ldr r2, [r3, sl]
0044e0a4: ldrb r3, [r2, #1]
0044e0a8: sub r1, r3, #3
0044e0ac: uxtb r1, r1
0044e0b0: cmp r1, #1
0044e0b4: bhi #0x44dfec
0044e0b8: mov r0, r4
0044e0bc: bl #0x796f5c
0044e0c0: str r0, [r8, #8]
0044e0c4: b #0x44dff4
0044e0c8: ldr r7, [sp, #0x34]
0044e0cc: ldr r4, [sp, #0x38]
0044e0d0: ldr r8, [sp, #0x3c]
0044e0d4: ldr sl, [sp, #0x18]
0044e0d8: ldr r2, [sp, #0x28]
0044e0dc: ldr r1, [sp, #0x10]
0044e0e0: ldr r3, [r4, sl]
0044e0e4: ldr r0, [r3, #0x34]
0044e0e8: ldr r3, [sp, #0x14]
0044e0ec: bl #0x509aec
0044e0f0: ldr r1, [sp, #0x8c]
0044e0f4: ldr r0, [r7]
0044e0f8: bl #0x797350
0044e0fc: ldr r1, [sp, #0x60]
0044e100: ldr r3, [sp, #0x5c]
0044e104: rsb r2, r3, r1
0044e108: lsrs r2, r2, #2
0044e10c: beq #0x44e148
0044e110: mov r5, #0
0044e114: ldr r6, [r3, r5, lsl #2]
0044e118: cmp r6, #0
0044e11c: beq #0x44e138
0044e120: mov r0, r6
0044e124: bl #0x797124
0044e128: mov r0, r6
0044e12c: bl #0x310440
0044e130: ldr r3, [sp, #0x5c]
0044e134: ldr r1, [sp, #0x60]
0044e138: add r5, r5, #1
0044e13c: rsb r2, r3, r1
0044e140: cmp r5, r2, asr #2
0044e144: blo #0x44e114
0044e148: cmp r1, r3
0044e14c: strne r3, [sp, #0x60]
0044e150: cmp r3, #0
0044e154: beq #0x44e16c
0044e158: ldr r2, [sp, #0x64]
0044e15c: mov r0, r3
0044e160: rsb r3, r3, r2
0044e164: bic r1, r3, #3
0044e168: bl #0x31bb44
0044e16c: ldr fp, [sp, #0x1c]
0044e170: ldr r1, [sp, #0x14]
0044e174: ldr r3, [r4, fp]
0044e178: add r0, r1, #4
0044e17c: add r3, r3, #8
0044e180: str r3, [sp, #0x40]
0044e184: bl #0x3fab18
0044e188: ldr r0, [sp, #0x10]
0044e18c: bl #0x3139ac
0044e190: b #0x44de7c
0044e194: ldr r0, [sp, #0x2c]
0044e198: mov r2, r7
0044e19c: bl #0x43f414
0044e1a0: ldr r8, [sp, #0x48]
0044e1a4: b #0x44e098
0044e1a8: ldmib r2, {r2, r3}
0044e1ac: ldr r1, [sp, #0x30]
0044e1b0: str r3, [sp, #0x74]
0044e1b4: str r2, [sp, #0x70]
0044e1b8: ldrd r0, r1, [r1]
0044e1bc: mov r2, r0
0044e1c0: mov r3, r1
0044e1c4: strd r0, r1, [sp, #8]
0044e1c8: strd r0, r1, [sp, #0x68]
0044e1cc: bl #0x30e2bc
0044e1d0: subs sl, r0, #0
0044e1d4: bne #0x44dff4
0044e1d8: mov r0, r4
0044e1dc: bl #0x797a54
0044e1e0: bl #0x30ea24
0044e1e4: str r0, [r8, #4]
0044e1e8: mov r0, r4
0044e1ec: bl #0x797a54
0044e1f0: bl #0x30e6a0
0044e1f4: str r0, [r8]
0044e1f8: ldr r3, [sp, #0xa0]
0044e1fc: mvn r2, #0
0044e200: mov r0, r4
0044e204: bfi r3, r2, #0, #0x18
0044e208: lsr r2, r3, #0x18
0044e20c: str r3, [sp, #0xa0]
0044e210: bfi r2, sl, #0, #1
0044e214: mov r3, #1
0044e218: ldr r1, [sp, #0x24]
0044e21c: strb r3, [sp, #0x90]
0044e220: strb r2, [sp, #0xa3]
0044e224: strb sl, [sp, #0x91]
0044e228: bl #0x797b7c
0044e22c: ldrsb r3, [sp, #0x90]
0044e230: cmn r3, #1
0044e234: ldrne r3, [sp, #0x20]
0044e238: ldreq r3, [sp, #0x9c]
0044e23c: str r3, [r8, #8]
0044e240: ldrsb r3, [sp, #0x90]
0044e244: cmn r3, #1
0044e248: bne #0x44dff4
0044e24c: ldr r0, [sp, #0x9c]
0044e250: ldr r1, [sp, #0x98]
0044e254: bl #0x752b38
0044e258: b #0x44dff4
0044e25c: bl #0x30e310
0044e260: subseq r6, r4, r4, lsr ip
0044e264: andeq r4, r0, ip, lsr #1
0044e268: strdeq r3, r4, [r0], -r4
0044e26c: andeq r4, r0, r8, lsl #1

# 0x457834 _Z27GetNewScriptCmdImplInstanceI34Script_SkipCharMenuTutorialMessageEP13ScriptCmdImplv
00457834: push {r4, lr}
00457838: mov r1, #0
0045783c: mov r0, #0x10
00457840: bl #0x310570
00457844: ldr r4, [pc, #0x28]
00457848: ldr r2, [pc, #0x28]
0045784c: mov r1, #0
00457850: add r4, pc, r4
00457854: ldr r2, [r4, r2]
00457858: str r1, [r0, #0xc]
0045785c: strb r1, [r0, #4]
00457860: add r2, r2, #8
00457864: str r2, [r0]
00457868: mvn r2, #0
0045786c: str r2, [r0, #8]
00457870: pop {r4, pc}
00457874: subseq sp, r3, r0, asr #4
00457878: andeq r2, r0, r4, asr r3

# 0x460b1c _ZN37Script_EnqueueCharMenuTutorialMessage7ExecuteEbi
00460b1c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00460b20: ldr r4, [pc, #0xf8]
00460b24: ldr r3, [pc, #0xf8]
00460b28: ldr r5, [pc, #0xf8]
00460b2c: add r4, pc, r4
00460b30: ldr sl, [r4, r3]
00460b34: sub sp, sp, #0x70
00460b38: add r8, sp, #0x54
00460b3c: ldr r3, [sl]
00460b40: add r5, pc, r5
00460b44: add r7, sp, #0x3c
00460b48: str r3, [sp, #0x6c]
00460b4c: ldr r6, [r0, #0xc]
00460b50: mov r1, r5
00460b54: add r2, sp, #4
00460b58: mov r0, r8
00460b5c: bl #0x3140ec
00460b60: mov r1, r5
00460b64: mov r2, sp
00460b68: add r5, sp, #8
00460b6c: mov r0, r7
00460b70: bl #0x3140ec
00460b74: mov r2, r8
00460b78: mov r3, r7
00460b7c: mvn r1, #0
00460b80: mov r0, r5
00460b84: bl #0x433b44
00460b88: mov r0, r7
00460b8c: bl #0x318254
00460b90: mov r0, r8
00460b94: bl #0x318254
00460b98: ldr r3, [r6, #0x18]
00460b9c: add r7, r5, #4
00460ba0: add r8, r5, #0x1c
00460ba4: str r3, [sp, #8]
00460ba8: ldr sb, [r6, #0xc]
00460bac: mov r0, sb
00460bb0: bl #0x30de54
00460bb4: mov r1, sb
00460bb8: add r2, sb, r0
00460bbc: mov r0, r7
00460bc0: bl #0x3109e0
00460bc4: ldr r6, [r6, #0x14]
00460bc8: mov r0, r6
00460bcc: bl #0x30de54
00460bd0: mov r1, r6
00460bd4: add r2, r6, r0
00460bd8: mov r0, r8
00460bdc: bl #0x3109e0
00460be0: ldr r0, [pc, #0x44]
00460be4: mov r1, r5
00460be8: ldr r0, [r4, r0]
00460bec: add r0, r0, #4
00460bf0: bl #0x45b7f4
00460bf4: mov r0, r8
00460bf8: bl #0x318254
00460bfc: mov r0, r7
00460c00: bl #0x318254
00460c04: ldr r2, [sp, #0x6c]
00460c08: ldr r3, [sl]
00460c0c: cmp r2, r3
00460c10: bne #0x460c1c
00460c14: add sp, sp, #0x70
00460c18: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00460c1c: bl #0x30e310
00460c20: subseq r3, r3, r4, ror #30
00460c24: andeq r4, r0, ip, lsr #1
00460c28: subeq sl, r6, r8, asr #25
00460c2c: andeq r4, r0, r0, lsl #18

# 0x455950 _ZNK37Script_EnqueueCharMenuTutorialMessage10IsBlockingEv
00455950: mov r0, #0
00455954: bx lr

# 0x421aac _ZN8MenuBase16FS_GetNumPotionsEPKcS1_Pv
00421aac: ldr r3, [pc, #0x94]
00421ab0: push {r4, r5, lr}
00421ab4: mov r5, r2
00421ab8: ldr r2, [pc, #0x8c]
00421abc: add r3, pc, r3
00421ac0: sub sp, sp, #0x1c
00421ac4: ldr r0, [r3, r2]
00421ac8: mov r1, #0
00421acc: mov r2, #1
00421ad0: ldr r0, [r0, #0x40]
00421ad4: bl #0x36e478
00421ad8: ldr r0, [r0, #0x660]
00421adc: cmp r0, #0
00421ae0: beq #0x421b40
00421ae4: add r0, r0, #0x37c
00421ae8: bl #0x3fc690
00421aec: mov r3, #0
00421af0: strb r3, [sp, #4]
00421af4: mov r3, #2
00421af8: strb r3, [sp, #5]
00421afc: bl #0x30ed30
00421b00: strd r0, r1, [sp, #0x10]
00421b04: ldr ip, [sp, #0x10]
00421b08: ldr r1, [pc, #0x40]
00421b0c: ldr r2, [pc, #0x40]
00421b10: ldr r0, [r5, #4]
00421b14: str ip, [sp, #8]
00421b18: ldr ip, [sp, #0x14]
00421b1c: add r4, sp, #4
00421b20: add r1, pc, r1
00421b24: add r2, pc, r2
00421b28: mov r3, r4
00421b2c: str ip, [r4, #8]
00421b30: bl #0x7ab5d4
00421b34: mov r0, r4
00421b38: bl #0x797124
00421b3c: mov r0, #1
00421b40: add sp, sp, #0x1c
00421b44: pop {r4, r5, pc}
00421b48: ldrsbeq r2, [r7], #-0xf4
00421b4c: strdeq r3, r4, [r0], -r4
00421b50: subeq r1, sl, r0, ror #13
00421b54: subeq r7, sl, ip, lsr #8

# 0x442838 _Z33NativeSkipCharMenuTutorialMessageRKN7gameswf7fn_callE
00442838: push {r4, lr}
0044283c: ldr r4, [pc, #0x40]
00442840: ldr r3, [pc, #0x40]
00442844: add r4, pc, r4
00442848: ldr r0, [r4, r3]
0044284c: ldr r2, [r0, #0x14]
00442850: ldr r3, [r0, #4]
00442854: cmp r2, r3
00442858: beq #0x442880
0044285c: add r0, r0, #4
00442860: bl #0x441d58
00442864: ldr r3, [pc, #0x20]
00442868: ldr r3, [r4, r3]
0044286c: ldr r0, [r3]
00442870: cmp r0, #0
00442874: beq #0x442880
00442878: pop {r4, lr}
0044287c: b #0x442734
00442880: pop {r4, pc}
00442884: subseq r2, r5, ip, asr #4
00442888: andeq r4, r0, r0, lsl #18
0044288c: andeq r2, r0, r8, ror r5

# 0x44c7a0 _Z32NativeGetCharMenuTutorialMessageRKN7gameswf7fn_callE
0044c7a0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044c7a4: ldr r4, [pc, #0x290]
0044c7a8: ldr sb, [pc, #0x290]
0044c7ac: ldr r2, [r0, #0xc]
0044c7b0: add r4, pc, r4
0044c7b4: ldr r1, [r4, sb]
0044c7b8: sub sp, sp, #0xd4
0044c7bc: ldr r3, [r0, #0x14]
0044c7c0: ldr r1, [r1]
0044c7c4: mov r8, r0
0044c7c8: ldr r5, [pc, #0x274]
0044c7cc: str r1, [sp, #0xcc]
0044c7d0: ldr r2, [r2]
0044c7d4: mov r1, #0xc
0044c7d8: add r6, sp, #0x78
0044c7dc: mla r3, r1, r3, r2
0044c7e0: add r5, pc, r5
0044c7e4: ldrsb r2, [r3, #1]
0044c7e8: add r7, sp, #0x60
0044c7ec: cmp r2, #5
0044c7f0: ldreq r0, [r3, #4]
0044c7f4: movne r0, #0
0044c7f8: bl #0x439cb4
0044c7fc: mov r1, r5
0044c800: add r2, sp, #0x28
0044c804: mov sl, r0
0044c808: mov r0, r6
0044c80c: bl #0x3140ec
0044c810: mov r1, r5
0044c814: add r2, sp, #0x24
0044c818: mov r0, r7
0044c81c: add r5, sp, #0x2c
0044c820: bl #0x3140ec
0044c824: mov r2, r6
0044c828: mvn r1, #0
0044c82c: mov r3, r7
0044c830: mov r0, r5
0044c834: bl #0x433b44
0044c838: mov r0, r7
0044c83c: bl #0x3139ac
0044c840: mov r0, r6
0044c844: bl #0x3139ac
0044c848: ldr r3, [pc, #0x1f8]
0044c84c: ldr r3, [r4, r3]
0044c850: ldr r2, [r3, #0x14]
0044c854: ldr r6, [r3, #4]
0044c858: cmp r2, r6
0044c85c: moveq r1, #0
0044c860: beq #0x44c9d0
0044c864: mov r3, r6
0044c868: ldr r2, [r3], #4
0044c86c: add r0, r5, #4
0044c870: cmp r0, r3
0044c874: str r2, [sp, #0x2c]
0044c878: beq #0x44c888
0044c87c: ldr r1, [r6, #0x18]
0044c880: ldr r2, [r6, #0x14]
0044c884: bl #0x3109e0
0044c888: add r0, r5, #0x1c
0044c88c: add r3, r6, #0x1c
0044c890: cmp r0, r3
0044c894: beq #0x44c8a4
0044c898: ldr r2, [r6, #0x2c]
0044c89c: ldr r1, [r6, #0x30]
0044c8a0: bl #0x3109e0
0044c8a4: ldr r1, [pc, #0x1a0]
0044c8a8: ldr r3, [sl]
0044c8ac: add fp, sp, #0xb8
0044c8b0: add r1, pc, r1
0044c8b4: mov r0, fp
0044c8b8: ldr r7, [r3, #0x1c]
0044c8bc: bl #0x413a7c
0044c8c0: ldr r3, [pc, #0x188]
0044c8c4: ldr r1, [sp, #0x2c]
0044c8c8: add r6, sp, #0x18
0044c8cc: ldr r3, [r4, r3]
0044c8d0: ldr r0, [r3, #0x34]
0044c8d4: bl #0x508edc
0044c8d8: mov r3, #0
0044c8dc: mov r1, r0
0044c8e0: mov r0, r6
0044c8e4: strb r3, [sp, #0x19]
0044c8e8: strb r3, [sp, #0x18]
0044c8ec: bl #0x797350
0044c8f0: mov r1, fp
0044c8f4: mov r2, r6
0044c8f8: mov r0, sl
0044c8fc: blx r7
0044c900: mov r0, r6
0044c904: bl #0x797124
0044c908: ldrsb r3, [sp, #0xb8]
0044c90c: cmn r3, #1
0044c910: beq #0x44ca28
0044c914: ldr r1, [pc, #0x138]
0044c918: ldr r3, [sl]
0044c91c: add fp, sp, #0xa4
0044c920: add r6, sp, #0xc
0044c924: add r1, pc, r1
0044c928: mov r0, fp
0044c92c: ldr r7, [r3, #0x1c]
0044c930: bl #0x413a7c
0044c934: mov r3, #0
0044c938: ldr r1, [sp, #0x44]
0044c93c: mov r0, r6
0044c940: strb r3, [sp, #0xd]
0044c944: strb r3, [sp, #0xc]
0044c948: bl #0x797350
0044c94c: mov r1, fp
0044c950: mov r2, r6
0044c954: mov r0, sl
0044c958: blx r7
0044c95c: mov r0, r6
0044c960: bl #0x797124
0044c964: ldrsb r3, [sp, #0xa4]
0044c968: cmn r3, #1
0044c96c: beq #0x44ca18
0044c970: ldr r1, [pc, #0xe0]
0044c974: ldr r3, [sl]
0044c978: add fp, sp, #0x90
0044c97c: add r1, pc, r1
0044c980: mov r0, fp
0044c984: ldr r7, [r3, #0x1c]
0044c988: bl #0x413a7c
0044c98c: mov r3, #0
0044c990: ldr r1, [sp, #0x5c]
0044c994: mov r0, sp
0044c998: strb r3, [sp, #1]
0044c99c: strb r3, [sp]
0044c9a0: bl #0x797350
0044c9a4: mov r1, fp
0044c9a8: mov r2, sp
0044c9ac: mov r0, sl
0044c9b0: blx r7
0044c9b4: mov r0, sp
0044c9b8: bl #0x797124
0044c9bc: ldrsb r3, [sp, #0x90]
0044c9c0: mov r6, sp
0044c9c4: cmn r3, #1
0044c9c8: beq #0x44ca04
0044c9cc: mov r1, #1
0044c9d0: ldr r0, [r8]
0044c9d4: bl #0x797230
0044c9d8: add r0, r5, #0x1c
0044c9dc: bl #0x3139ac
0044c9e0: add r0, r5, #4
0044c9e4: bl #0x3139ac
0044c9e8: ldr r3, [r4, sb]
0044c9ec: ldr r2, [sp, #0xcc]
0044c9f0: ldr r3, [r3]
0044c9f4: cmp r2, r3
0044c9f8: bne #0x44ca38
0044c9fc: add sp, sp, #0xd4
0044ca00: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044ca04: ldr r1, [sp, #0x98]
0044ca08: ldr r0, [sp, #0x9c]
0044ca0c: bl #0x752b38
0044ca10: mov r1, #1
0044ca14: b #0x44c9d0
0044ca18: ldr r0, [sp, #0xb0]
0044ca1c: ldr r1, [sp, #0xac]
0044ca20: bl #0x752b38
0044ca24: b #0x44c970
0044ca28: ldr r0, [sp, #0xc4]
0044ca2c: ldr r1, [sp, #0xc0]
0044ca30: bl #0x752b38
0044ca34: b #0x44c914
0044ca38: bl #0x30e310
0044ca3c: subseq r8, r4, r0, ror #5
0044ca40: andeq r4, r0, ip, lsr #1
0044ca44: subeq pc, r7, r8, lsr #32
0044ca48: andeq r4, r0, r0, lsl #18
0044ca4c: strdeq r0, r1, [r8], #-0
0044ca50: strdeq r3, r4, [r0], -r4
0044ca54: subeq r0, r8, ip, asr r0
0044ca58: subeq r0, r8, r4, lsl r0

# 0x4c7ac4 _ZN7Structs31SkipAllCharMenuTutorialMessagesD2Ev
004c7ac4: ldr r3, [pc, #0x24]
004c7ac8: ldr r2, [pc, #0x24]
004c7acc: push {r4, lr}
004c7ad0: add r3, pc, r3
004c7ad4: ldr r2, [r3, r2]
004c7ad8: mov r4, r0
004c7adc: add r2, r2, #8
004c7ae0: str r2, [r0]
004c7ae4: bl #0x4c6c60
004c7ae8: mov r0, r4
004c7aec: pop {r4, pc}
004c7af0: subeq ip, ip, r0, asr #31
004c7af4: andeq r2, r0, r4, asr #29

# 0x4cdda8 _ZN7Structs27SkipCharMenuTutorialMessageD0Ev
004cdda8: push {r4, lr}
004cddac: mov r4, r0
004cddb0: bl #0x4c7a8c
004cddb4: mov r0, r4
004cddb8: bl #0x310440
004cddbc: mov r0, r4
004cddc0: pop {r4, pc}

# 0x4564c4 _Z27GetNewScriptCmdDataInstanceIN7Structs30EnqueueCharMenuTutorialMessageEEPNS0_9ScriptCmdEv
004564c4: push {r4, lr}
004564c8: mov r1, #0
004564cc: mov r0, #0x1c
004564d0: bl #0x310570
004564d4: ldr r4, [pc, #0x20]
004564d8: ldr r2, [pc, #0x20]
004564dc: mov r1, #0
004564e0: add r4, pc, r4
004564e4: ldr r2, [r4, r2]
004564e8: str r1, [r0, #0x14]
004564ec: str r1, [r0, #0xc]
004564f0: add r2, r2, #8
004564f4: str r2, [r0]
004564f8: pop {r4, pc}
004564fc: ldrheq lr, [r3], #-0x50
00456500: andeq r3, r0, r8, asr #18

# 0x433b70 _ZN19CharMenuTutorialMsgC2EiRKSsS1_
00433b70: push {r4, r5, r6, lr}
00433b74: mov r4, r0
00433b78: mov r5, r3
00433b7c: str r1, [r0], #4
00433b80: mov r1, r2
00433b84: bl #0x32b918
00433b88: mov r1, r5
00433b8c: add r0, r4, #0x1c
00433b90: bl #0x32b918
00433b94: mov r0, r4
00433b98: pop {r4, r5, r6, pc}

# 0x4562dc _ZNKSt4priv20_Deque_iterator_baseI19CharMenuTutorialMsgE11_M_subtractERKS2_
004562dc: push {r4, r5}
004562e0: ldm r0, {r2, r3}
004562e4: ldr r5, [r1, #8]
004562e8: ldr r4, [r1]
004562ec: rsb r3, r3, r2
004562f0: movw ip, #0x4ec5
004562f4: movt ip, #0xc4ec
004562f8: asr r3, r3, #2
004562fc: mul r3, ip, r3
00456300: ldr r0, [r0, #0xc]
00456304: ldr r1, [r1, #0xc]
00456308: rsb r2, r4, r5
0045630c: asr r2, r2, #2
00456310: mla ip, ip, r2, r3
00456314: rsb r0, r1, r0
00456318: asr r0, r0, #2
0045631c: sub r0, r0, #1
00456320: add r0, ip, r0, lsl #1
00456324: pop {r4, r5}
00456328: bx lr

# 0x439cb4 _ZN7gameswf7cast_toINS_9as_objectEEEPT_PNS_19as_object_interfaceE
00439cb4: push {r4, lr}
00439cb8: subs r4, r0, #0
00439cbc: beq #0x439ce0
00439cc0: ldr r3, [r4]
00439cc4: mov r1, #0
00439cc8: mov lr, pc
00439ccc: ldr pc, [r3, #8]
00439cd0: cmp r0, #0
00439cd4: beq #0x439ce0
00439cd8: mov r0, r4
00439cdc: pop {r4, pc}
00439ce0: mov r0, #0
00439ce4: pop {r4, pc}

# 0x3293c8 _ZNSaIP19CharMenuTutorialMsgE8allocateEjPKv
003293c8: str lr, [sp, #-4]!
003293cc: cmn r1, #0xc0000001
003293d0: sub sp, sp, #0xc
003293d4: bhi #0x329410
003293d8: cmp r1, #0
003293dc: moveq r0, r1
003293e0: bne #0x3293ec
003293e4: add sp, sp, #0xc
003293e8: ldm sp!, {pc}
003293ec: lsl r0, r1, #2
003293f0: cmp r0, #0x80
003293f4: str r0, [sp, #4]
003293f8: bhi #0x329408
003293fc: add r0, sp, #4
00329400: bl #0x708ec0
00329404: b #0x3293e4
00329408: bl #0x310454
0032940c: b #0x3293e4
00329410: ldr r0, [pc, #0xc]
00329414: add r0, pc, r0
00329418: bl #0x30e0c4
0032941c: mov r0, #1
00329420: bl #0x30de48
00329424: subseq r5, sb, ip, asr r0

# 0x455958 _ZNK34Script_SkipCharMenuTutorialMessage10IsBlockingEv
00455958: mov r0, #0
0045595c: bx lr

# 0x4ff89c _ZN7Structs31SkipAllCharMenuTutorialMessages4readEP11IStreamBase
004ff89c: b #0x4ff828

# 0x4cdd8c _ZN7Structs31SkipAllCharMenuTutorialMessagesD0Ev
004cdd8c: push {r4, lr}
004cdd90: mov r4, r0
004cdd94: bl #0x4c7af8
004cdd98: mov r0, r4
004cdd9c: bl #0x310440
004cdda0: mov r0, r4
004cdda4: pop {r4, pc}

# 0x4577ec _Z27GetNewScriptCmdImplInstanceI38Script_SkipAllCharMenuTutorialMessagesEP13ScriptCmdImplv
004577ec: push {r4, lr}
004577f0: mov r1, #0
004577f4: mov r0, #0x10
004577f8: bl #0x310570
004577fc: ldr r4, [pc, #0x28]
00457800: ldr r2, [pc, #0x28]
00457804: mov r1, #0
00457808: add r4, pc, r4
0045780c: ldr r2, [r4, r2]
00457810: str r1, [r0, #0xc]
00457814: strb r1, [r0, #4]
00457818: add r2, r2, #8
0045781c: str r2, [r0]
00457820: mvn r2, #0
00457824: str r2, [r0, #8]
00457828: pop {r4, pc}
0045782c: subseq sp, r3, r8, lsl #5
00457830: muleq r0, ip, r0

# 0x4c7ac0 _ZN7Structs27SkipCharMenuTutorialMessage8finalizeEv
004c7ac0: b #0x4c6c68

# 0x45a780 _ZN19CharMenuTutorialMsgC1ERKS_
0045a780: push {r4, r5, r6, lr}
0045a784: ldr r3, [r1]
0045a788: mov r4, r0
0045a78c: mov r5, r1
0045a790: str r3, [r0], #4
0045a794: str r0, [r4, #0x14]
0045a798: str r0, [r4, #0x18]
0045a79c: ldr r2, [r5, #0x14]
0045a7a0: ldr r1, [r1, #0x18]
0045a7a4: bl #0x3116e8
0045a7a8: add r0, r4, #0x1c
0045a7ac: str r0, [r4, #0x2c]
0045a7b0: str r0, [r4, #0x30]
0045a7b4: ldr r2, [r5, #0x2c]
0045a7b8: ldr r1, [r5, #0x30]
0045a7bc: bl #0x3116e8
0045a7c0: mov r0, r4
0045a7c4: pop {r4, r5, r6, pc}

# 0x441d58 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EE9pop_frontEv
00441d58: push {r4, r5, r6, lr}
00441d5c: ldr r5, [r0]
00441d60: mov r4, r0
00441d64: add r0, r5, #0x1c
00441d68: bl #0x3139ac
00441d6c: add r0, r5, #4
00441d70: bl #0x3139ac
00441d74: ldr r2, [r4, #8]
00441d78: ldr r3, [r4]
00441d7c: sub r2, r2, #0x34
00441d80: cmp r3, r2
00441d84: beq #0x441d94
00441d88: add r3, r3, #0x34
00441d8c: str r3, [r4]
00441d90: pop {r4, r5, r6, pc}
00441d94: ldr r0, [r4, #4]
00441d98: cmp r0, #0
00441d9c: beq #0x441da8
00441da0: mov r1, #0x68
00441da4: bl #0x708f00
00441da8: ldr r3, [r4, #0xc]
00441dac: add r2, r3, #4
00441db0: str r2, [r4, #0xc]
00441db4: ldr r3, [r3, #4]
00441db8: add r2, r3, #0x68
00441dbc: str r3, [r4]
00441dc0: str r2, [r4, #8]
00441dc4: str r3, [r4, #4]
00441dc8: pop {r4, r5, r6, pc}

# 0x4d1280 _ZN7Structs30EnqueueCharMenuTutorialMessage8finalizeEv
004d1280: push {r4, lr}
004d1284: mov r4, r0
004d1288: ldr r0, [r0, #0xc]
004d128c: cmp r0, #0
004d1290: beq #0x4d12a4
004d1294: bl #0x310440
004d1298: mov r3, #0
004d129c: str r3, [r4, #8]
004d12a0: str r3, [r4, #0xc]
004d12a4: ldr r0, [r4, #0x14]
004d12a8: cmp r0, #0
004d12ac: beq #0x4d12c0
004d12b0: bl #0x310440
004d12b4: mov r3, #0
004d12b8: str r3, [r4, #0x10]
004d12bc: str r3, [r4, #0x14]
004d12c0: mov r0, r4
004d12c4: pop {r4, lr}
004d12c8: b #0x4c6c68

# 0x3fc690 _ZNK13ItemInventory13GetNumPotionsEv
003fc690: ldr r0, [r0, #0x24]
003fc694: cmp r0, #0
003fc698: ldrshne r0, [r0, #0x50]
003fc69c: bx lr

# 0x45b7f4 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EE9push_backERKS0_
0045b7f4: push {r4, lr}
0045b7f8: ldr r2, [r0, #0x18]
0045b7fc: ldr r3, [r0, #0x10]
0045b800: mov r4, r0
0045b804: sub r2, r2, #0x34
0045b808: cmp r3, r2
0045b80c: beq #0x45b828
0045b810: mov r0, r3
0045b814: bl #0x45a780
0045b818: ldr r3, [r4, #0x10]
0045b81c: add r3, r3, #0x34
0045b820: str r3, [r4, #0x10]
0045b824: pop {r4, pc}
0045b828: pop {r4, lr}
0045b82c: b #0x45b674

# 0x446978 _Z25NativeGetStringNumPotionsRKN7gameswf7fn_callE
00446978: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044697c: ldr r4, [pc, #0x160]
00446980: ldr r8, [pc, #0x160]
00446984: ldr r3, [r0, #0xc]
00446988: add r4, pc, r4
0044698c: ldr r2, [r4, r8]
00446990: sub sp, sp, #0x44
00446994: ldr r0, [r0, #0x14]
00446998: ldr r2, [r2]
0044699c: str r2, [sp, #0x3c]
004469a0: ldr r3, [r3]
004469a4: mov r2, #0xc
004469a8: mla r2, r2, r0, r3
004469ac: sub r0, r0, #1
004469b0: ldrsb r1, [r2, #1]
004469b4: cmp r1, #5
004469b8: ldreq r7, [r2, #4]
004469bc: mov r2, #0xc
004469c0: mla r0, r2, r0, r3
004469c4: movne r7, #0
004469c8: bl #0x797a54
004469cc: bl #0x30ea24
004469d0: mov r1, #0
004469d4: bl #0x43c388
004469d8: subs sb, r0, #0
004469dc: beq #0x446ab4
004469e0: add r5, sp, #0x10
004469e4: mov r0, r5
004469e8: mov r1, #0x10
004469ec: str r5, [sp, #0x20]
004469f0: str r5, [sp, #0x24]
004469f4: bl #0x31167c
004469f8: ldr r2, [sp, #0x20]
004469fc: ldr r3, [pc, #0xe8]
00446a00: mov r6, #0
00446a04: strb r6, [r2]
00446a08: ldr r3, [r4, r3]
00446a0c: ldr r2, [pc, #0xdc]
00446a10: ldr r1, [pc, #0xdc]
00446a14: ldr r0, [r3, #0x2c]
00446a18: add r2, pc, r2
00446a1c: add r1, pc, r1
00446a20: ldr sl, [r3, #0x34]
00446a24: bl #0x4c4bdc
00446a28: mov r1, r0
00446a2c: mov r0, sl
00446a30: bl #0x508edc
00446a34: mov fp, r0
00446a38: add r0, sb, #0x37c
00446a3c: bl #0x3fc690
00446a40: mov r2, fp
00446a44: mov r3, r0
00446a48: mov r1, r5
00446a4c: mov r0, sl
00446a50: bl #0x508ef4
00446a54: ldr r1, [pc, #0x9c]
00446a58: ldr r3, [r7]
00446a5c: add fp, sp, #0x28
00446a60: add sl, sp, #4
00446a64: add r1, pc, r1
00446a68: mov r0, fp
00446a6c: ldr sb, [r3, #0x1c]
00446a70: bl #0x413a7c
00446a74: mov r0, sl
00446a78: ldr r1, [sp, #0x24]
00446a7c: strb r6, [sp, #5]
00446a80: strb r6, [sp, #4]
00446a84: bl #0x797350
00446a88: mov r1, fp
00446a8c: mov r2, sl
00446a90: mov r0, r7
00446a94: blx sb
00446a98: mov r0, sl
00446a9c: bl #0x797124
00446aa0: ldrsb r3, [sp, #0x28]
00446aa4: cmn r3, #1
00446aa8: beq #0x446ad0
00446aac: mov r0, r5
00446ab0: bl #0x3139ac
00446ab4: ldr r3, [r4, r8]
00446ab8: ldr r2, [sp, #0x3c]
00446abc: ldr r3, [r3]
00446ac0: cmp r2, r3
00446ac4: bne #0x446ae0
00446ac8: add sp, sp, #0x44
00446acc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00446ad0: ldr r0, [sp, #0x34]
00446ad4: ldr r1, [sp, #0x30]
00446ad8: bl #0x752b38
00446adc: b #0x446aac
00446ae0: bl #0x30e310
00446ae4: subseq lr, r4, r8, lsl #2
00446ae8: andeq r4, r0, ip, lsr #1
00446aec: strdeq r3, r4, [r0], -r4
00446af0: subeq r5, r8, r8, lsl r7
00446af4: subeq r8, r7, ip, lsl #4
00446af8: subeq r5, r8, r4, ror #13

# 0x4603d0 _ZN34Script_SkipCharMenuTutorialMessage7ExecuteEbi
004603d0: push {r4, lr}
004603d4: ldr r4, [pc, #0x40]
004603d8: ldr r3, [pc, #0x40]
004603dc: add r4, pc, r4
004603e0: ldr r0, [r4, r3]
004603e4: ldr r2, [r0, #0x14]
004603e8: ldr r3, [r0, #4]
004603ec: cmp r2, r3
004603f0: beq #0x460418
004603f4: add r0, r0, #4
004603f8: bl #0x441d58
004603fc: ldr r3, [pc, #0x20]
00460400: ldr r3, [r4, r3]
00460404: ldr r0, [r3]
00460408: cmp r0, #0
0046040c: beq #0x460418
00460410: pop {r4, lr}
00460414: b #0x45a0fc
00460418: pop {r4, pc}
0046041c: ldrheq r4, [r3], #-0x64
00460420: andeq r4, r0, r0, lsl #18
00460424: andeq r2, r0, r8, ror r5
