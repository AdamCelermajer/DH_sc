_ZN6glitch2io17CTextureAttributeC1EPKcRKN5boost13intrusive_ptrINS_5video8ITextureEEEPNS6_12IVideoDriverEb
00566ea0: push {r4, r5, r6, r7, r8, sb, sl, lr}
00566ea4: ldr r6, [pc, #0xc4]
00566ea8: ldr ip, [pc, #0xc4]
00566eac: mov r4, r0
00566eb0: add r6, pc, r6
00566eb4: ldr ip, [r6, ip]
00566eb8: mov r5, r0
00566ebc: mov r0, #1
00566ec0: add ip, ip, #8
00566ec4: str r0, [r4, #4]
00566ec8: str ip, [r5], #8
00566ecc: mov r7, r1
00566ed0: str r5, [r4, #0x18]
00566ed4: str r5, [r4, #0x1c]
00566ed8: mov r0, r5
00566edc: mov r1, #0x10
00566ee0: mov r8, r3
00566ee4: mov sl, r2
00566ee8: ldrb sb, [sp, #0x20]
00566eec: bl #0x3209a8
00566ef0: ldr r3, [pc, #0x80]
00566ef4: ldr r1, [r4, #0x18]
00566ef8: mov r2, #0
00566efc: ldr r3, [r6, r3]
00566f00: strb r2, [r1]
00566f04: cmp r8, #0
00566f08: add r3, r3, #8
00566f0c: str r3, [r4]
00566f10: str r2, [r4, #0x24]
00566f14: strb sb, [r4, #0x20]
00566f18: str r8, [r4, #0x28]
00566f1c: ldrne r3, [r8, #4]
00566f20: mov r0, r7
00566f24: addne r3, r3, #1
00566f28: strne r3, [r8, #4]
00566f2c: bl #0x30de54
00566f30: mov r1, r7
00566f34: add r2, r7, r0
00566f38: mov r0, r5
00566f3c: bl #0x320b88
00566f40: ldr r3, [sl]
00566f44: cmp r3, #0
00566f48: ldrne r2, [r3, #4]
00566f4c: addne r2, r2, #1
00566f50: strne r2, [r3, #4]
00566f54: ldr r0, [r4, #0x24]
00566f58: str r3, [r4, #0x24]
00566f5c: cmp r0, #0
00566f60: beq #0x566f68
00566f64: bl #0x31d584
00566f68: mov r0, r4
00566f6c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00566f70: subeq sp, r2, r0, ror #23
00566f74: andeq r2, r0, r4, asr #24
00566f78: andeq r0, r0, r0, ror ip

_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture19generateMipmapsImplEv
005b280c: push {r4, r5, r6, lr}
005b2810: mov r4, r0
005b2814: ldr r0, [r0, #0x34]
005b2818: ldr r3, [r4, #0x38]
005b281c: mov r2, r4
005b2820: ldr r5, [r0, #0x4c]
005b2824: and r3, r3, #3
005b2828: sub r5, r5, #1
005b282c: mov r1, r5
005b2830: bl #0x5b26f0
005b2834: ldr r6, [r4, #0x34]
005b2838: ldr r3, [r6, #0x268]
005b283c: cmp r5, r3
005b2840: beq #0x5b2854
005b2844: add r0, r5, #0x8400
005b2848: add r0, r0, #0xc0
005b284c: bl #0x30e1e4
005b2850: str r5, [r6, #0x268]
005b2854: ldr r3, [r4, #0x38]
005b2858: ldr r5, [pc, #0x78]
005b285c: ubfx r2, r3, #0xc, #3
005b2860: add r5, pc, r5
005b2864: cmp r2, #1
005b2868: and r3, r3, #3
005b286c: add r2, r5, #0xa4
005b2870: ldr r6, [r2, r3, lsl #2]
005b2874: ble #0x5b28a0
005b2878: mov r0, r6
005b287c: bl #0x30e370
005b2880: ldrb r3, [r4, #0x3f]
005b2884: tst r3, #2
005b2888: bne #0x5b289c
005b288c: ldrh r3, [r4, #0x40]
005b2890: orr r3, r3, #2
005b2894: strh r3, [r4, #0x40]
005b2898: pop {r4, r5, r6, pc}
005b289c: pop {r4, r5, r6, pc}
005b28a0: movw r1, #0x2801
005b28a4: mov r2, #0x2700
005b28a8: mov r0, r6
005b28ac: bl #0x30e910
005b28b0: mov r0, r6
005b28b4: bl #0x30e370
005b28b8: ldr r3, [r4, #0x38]
005b28bc: add r5, r5, #0xb4
005b28c0: mov r0, r6
005b28c4: ubfx r3, r3, #0xc, #3
005b28c8: ldr r2, [r5, r3, lsl #2]
005b28cc: movw r1, #0x2801
005b28d0: bl #0x30e910
005b28d4: b #0x5b2880
005b28d8: ldrsbteq sp, [r2], -r4

_ZN6glitch5video11CNullDriver8CTextureC1EPKcPS1_RKNS0_12STextureDescE
005b9558: push {r4, r5, r6, lr}
005b955c: ldr r4, [pc, #0x20]
005b9560: mov r5, r0
005b9564: bl #0x5fe6e0
005b9568: ldr r3, [pc, #0x18]
005b956c: add r4, pc, r4
005b9570: mov r0, r5
005b9574: ldr r3, [r4, r3]
005b9578: add r3, r3, #8
005b957c: str r3, [r5]
005b9580: pop {r4, r5, r6, pc}
005b9584: eorseq fp, sp, r4, lsr #10
005b9588: andeq r2, r0, r0, ror r1

_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18createRenderTargetERKN5boost13intrusive_ptrINS0_8ITextureEEEj
005b388c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005b3890: ldr r4, [pc, #0x168]
005b3894: ldr r5, [pc, #0x168]
005b3898: mov sl, r1
005b389c: add r4, pc, r4
005b38a0: ldr ip, [r4, r5]
005b38a4: mov r8, r2
005b38a8: ldr r2, [r2]
005b38ac: ldr r1, [ip]
005b38b0: sub sp, sp, #0x9c
005b38b4: mov r6, #0x14
005b38b8: str r1, [sp, #0x94]
005b38bc: ldr fp, [r2, #0x38]
005b38c0: mov r7, r0
005b38c4: mov sb, r3
005b38c8: ubfx fp, fp, #4, #6
005b38cc: mla r6, r6, fp, sl
005b38d0: add r6, r6, #0x4a0
005b38d4: add r6, r6, #8
005b38d8: ldrh r2, [r6, #6]
005b38dc: cmp fp, r2
005b38e0: beq #0x5b3990
005b38e4: cmp fp, #0x27
005b38e8: beq #0x5b3974
005b38ec: mov r0, #0
005b38f0: bl #0x5ed944
005b38f4: ldrh r2, [r6, #6]
005b38f8: ldr r8, [r0, fp, lsl #2]
005b38fc: cmp r2, #0x27
005b3900: beq #0x5b3984
005b3904: mov r0, #0
005b3908: str r2, [sp, #0xc]
005b390c: bl #0x5ed944
005b3910: ldr r2, [sp, #0xc]
005b3914: ldr ip, [r0, r2, lsl #2]
005b3918: ldr r2, [pc, #0xe8]
005b391c: add r6, sp, #0x14
005b3920: mov r3, r8
005b3924: add r2, pc, r2
005b3928: mov r1, #0x7f
005b392c: mov r0, r6
005b3930: str ip, [sp]
005b3934: bl #0x30e244
005b3938: ldr r0, [pc, #0xcc]
005b393c: mov r1, r6
005b3940: mov r2, #3
005b3944: add r0, pc, r0
005b3948: bl #0x60ace8
005b394c: mov r3, #0
005b3950: str r3, [r7]
005b3954: ldr r3, [r4, r5]
005b3958: ldr r2, [sp, #0x94]
005b395c: mov r0, r7
005b3960: ldr r3, [r3]
005b3964: cmp r2, r3
005b3968: bne #0x5b39fc
005b396c: add sp, sp, #0x9c
005b3970: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005b3974: ldr r8, [pc, #0x94]
005b3978: cmp r2, #0x27
005b397c: add r8, pc, r8
005b3980: bne #0x5b3904
005b3984: ldr ip, [pc, #0x88]
005b3988: add ip, pc, ip
005b398c: b #0x5b3918
005b3990: mov r1, #0
005b3994: mov r0, #0x5c
005b3998: bl #0x5341ac
005b399c: mov r1, sl
005b39a0: mov r6, r0
005b39a4: bl #0x6ddbec
005b39a8: ldr ip, [pc, #0x68]
005b39ac: ldr r3, [r6, #4]
005b39b0: mov r1, #0
005b39b4: ldr ip, [r4, ip]
005b39b8: add r3, r3, #1
005b39bc: str r3, [r6, #4]
005b39c0: add ip, ip, #8
005b39c4: str ip, [r6]
005b39c8: mov r3, r1
005b39cc: str sb, [sp]
005b39d0: mov r2, r8
005b39d4: mov r0, r6
005b39d8: mov lr, pc
005b39dc: ldr pc, [ip, #0x20]
005b39e0: str r6, [r7]
005b39e4: ldr r3, [r6, #4]
005b39e8: mov r0, r6
005b39ec: add r3, r3, #1
005b39f0: str r3, [r6, #4]
005b39f4: bl #0x31d584
005b39f8: b #0x5b3954
005b39fc: bl #0x30e310
005b3a00: ldrshteq r1, [lr], -r4
005b3a04: andeq r4, r0, ip, lsr #1
005b3a08: eorseq ip, r2, r4, ror ip
005b3a0c: eorseq ip, r2, r4, ror ip
005b3a10: eorseq r2, r1, r4, ror #21
005b3a14: ldrsbteq r2, [r1], -r8
005b3a18: andeq r4, r0, r8, ror sl

_ZN6glitch5video12IVideoDriver13createTextureEPKcRKNS0_12STextureDescE
005aa5f8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005aa5fc: ldr r8, [r3]
005aa600: mov r5, r1
005aa604: ldr r1, [r1, #0x9c]
005aa608: add ip, r8, #6
005aa60c: and ip, ip, #0x1f
005aa610: mov lr, #1
005aa614: ands ip, r1, lr, lsl ip
005aa618: ldr r7, [pc, #0x25c]
005aa61c: sub sp, sp, #0x24
005aa620: mov r4, r0
005aa624: add r7, pc, r7
005aa628: bne #0x5aa670
005aa62c: uxth r3, r8
005aa630: cmp r3, #0xff
005aa634: beq #0x5aa6b8
005aa638: mov r0, ip
005aa63c: str r2, [sp, #0x10]
005aa640: bl #0x5fda68
005aa644: ldr r2, [sp, #0x10]
005aa648: ldr r3, [r0, r8, lsl #2]
005aa64c: ldr r1, [pc, #0x22c]
005aa650: mov r0, #3
005aa654: add r1, pc, r1
005aa658: bl #0x60b034
005aa65c: mov r3, #0
005aa660: str r3, [r4]
005aa664: mov r0, r4
005aa668: add sp, sp, #0x24
005aa66c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005aa670: ldr r0, [r3, #0x10]
005aa674: cmp r0, #0
005aa678: beq #0x5aa6c4
005aa67c: ldr r6, [r3, #0x14]
005aa680: cmp r6, #0
005aa684: ldreq sl, [r3, #0x18]
005aa688: beq #0x5aa6cc
005aa68c: ldr sl, [r3, #0x18]
005aa690: cmp sl, #0
005aa694: beq #0x5aa6cc
005aa698: tst r1, #0x10
005aa69c: bne #0x5aa6f0
005aa6a0: cmp r0, r6
005aa6a4: beq #0x5aa868
005aa6a8: ldr r1, [pc, #0x1d4]
005aa6ac: mov r3, r0
005aa6b0: add r1, pc, r1
005aa6b4: b #0x5aa6d8
005aa6b8: ldr r3, [pc, #0x1c8]
005aa6bc: add r3, pc, r3
005aa6c0: b #0x5aa64c
005aa6c4: ldr sl, [r3, #0x18]
005aa6c8: ldr r6, [r3, #0x14]
005aa6cc: ldr r1, [pc, #0x1b8]
005aa6d0: mov r3, r0
005aa6d4: add r1, pc, r1
005aa6d8: mov r0, #3
005aa6dc: stm sp, {r6, sl}
005aa6e0: bl #0x60b034
005aa6e4: mov r3, #0
005aa6e8: str r3, [r4]
005aa6ec: b #0x5aa664
005aa6f0: cmp r8, #3
005aa6f4: beq #0x5aa73c
005aa6f8: tst r1, #0x20
005aa6fc: bne #0x5aa73c
005aa700: sub r1, r0, #1
005aa704: tst r1, r0
005aa708: beq #0x5aa71c
005aa70c: ldr r1, [pc, #0x17c]
005aa710: mov r3, r0
005aa714: add r1, pc, r1
005aa718: b #0x5aa6d8
005aa71c: sub r1, r6, #1
005aa720: tst r1, r6
005aa724: bne #0x5aa70c
005aa728: cmp r8, #1
005aa72c: bne #0x5aa73c
005aa730: sub r1, sl, #1
005aa734: tst r1, sl
005aa738: bne #0x5aa70c
005aa73c: ldr sb, [pc, #0x150]
005aa740: ldr r8, [r3, #4]
005aa744: mov fp, #0x28
005aa748: ldr r1, [r7, sb]
005aa74c: str r2, [sp, #0x10]
005aa750: str r3, [sp, #0x14]
005aa754: mla fp, fp, r8, r1
005aa758: str r8, [sp, #0x1c]
005aa75c: ldrb ip, [fp, #0x24]
005aa760: mov r1, ip
005aa764: str ip, [sp, #0x18]
005aa768: bl #0x30eb2c
005aa76c: cmp r1, #0
005aa770: ldr r2, [sp, #0x10]
005aa774: ldr r3, [sp, #0x14]
005aa778: beq #0x5aa7d0
005aa77c: uxth r1, r8
005aa780: cmp r1, #0x27
005aa784: bne #0x5aa828
005aa788: ldr r3, [pc, #0x108]
005aa78c: add r3, pc, r3
005aa790: ldr r1, [r7, sb]
005aa794: ldr ip, [sp, #0x1c]
005aa798: mov r0, #0x28
005aa79c: ldr r5, [sp, #0x18]
005aa7a0: mla r0, r0, ip, r1
005aa7a4: ldr r1, [pc, #0xf0]
005aa7a8: ldrb ip, [r0, #0x26]
005aa7ac: ldrb lr, [r0, #0x25]
005aa7b0: add r1, pc, r1
005aa7b4: mov r0, #3
005aa7b8: stm sp, {r5, lr}
005aa7bc: str ip, [sp, #8]
005aa7c0: bl #0x60b034
005aa7c4: mov r3, #0
005aa7c8: str r3, [r4]
005aa7cc: b #0x5aa664
005aa7d0: mov r0, r6
005aa7d4: ldrb r1, [fp, #0x25]
005aa7d8: str r2, [sp, #0x10]
005aa7dc: str r3, [sp, #0x14]
005aa7e0: bl #0x30eb2c
005aa7e4: cmp r1, #0
005aa7e8: ldr r2, [sp, #0x10]
005aa7ec: ldr r3, [sp, #0x14]
005aa7f0: bne #0x5aa77c
005aa7f4: mov r0, sl
005aa7f8: ldrb r1, [fp, #0x26]
005aa7fc: bl #0x30eb2c
005aa800: cmp r1, #0
005aa804: ldr r2, [sp, #0x10]
005aa808: ldr r3, [sp, #0x14]
005aa80c: bne #0x5aa77c
005aa810: mov r1, r5
005aa814: ldr ip, [r5]
005aa818: mov r0, r4
005aa81c: mov lr, pc
005aa820: ldr pc, [ip, #0x204]
005aa824: b #0x5aa664
005aa828: mov r0, #0
005aa82c: str r2, [sp, #0x10]
005aa830: str r3, [sp, #0x14]
005aa834: bl #0x5ed944
005aa838: ldr r3, [sp, #0x14]
005aa83c: ldr r1, [r7, sb]
005aa840: ldr r2, [sp, #0x10]
005aa844: ldr r3, [r3, #4]
005aa848: str r3, [sp, #0x1c]
005aa84c: ldr r5, [sp, #0x1c]
005aa850: ldr r3, [r0, r8, lsl #2]
005aa854: mov r0, #0x28
005aa858: mla r1, r0, r5, r1
005aa85c: ldrb r1, [r1, #0x24]
005aa860: str r1, [sp, #0x18]
005aa864: b #0x5aa790
005aa868: cmp r8, #1
005aa86c: bne #0x5aa6f0
005aa870: cmp sl, r0
005aa874: bne #0x5aa6a8
005aa878: b #0x5aa6f8
005aa87c: eorseq sl, lr, ip, ror #8
005aa880: eorseq r5, r3, r4, asr r7
005aa884: eorseq r5, r3, r0, ror r7
005aa888: eorseq fp, r1, r4, lsr #27
005aa88c: eorseq r5, r3, r4, lsl #14
005aa890: eorseq r5, r3, ip, asr r7
005aa894: andeq r1, r0, r4, lsr pc
005aa898: ldrsbteq fp, [r1], -r4
005aa89c: eorseq r5, r3, r8, lsl r7

_ZN6glitch5video15CTextureManagerC2EPNS0_12IVideoDriverE
005eaed8: push {r4, r5, lr}
005eaedc: sub sp, sp, #0x2c
005eaee0: mov r4, r0
005eaee4: mov r5, r1
005eaee8: bl #0x5e840c
005eaeec: str r5, [r4, #0x28]
005eaef0: ldr r3, [r5, #0xd4]
005eaef4: add r5, r4, #0x30
005eaef8: ldr r3, [r3, #0x34]
005eaefc: cmp r3, #0
005eaf00: str r3, [r4, #0x2c]
005eaf04: ldrne r2, [r3, #4]
005eaf08: addne r2, r2, #1
005eaf0c: strne r2, [r3, #4]
005eaf10: mov r3, #0
005eaf14: mov r2, #0x43
005eaf18: str r3, [r4, #0x64]
005eaf1c: str r3, [r4, #0x30]
005eaf20: str r3, [r4, #0x34]
005eaf24: str r3, [r4, #0x38]
005eaf28: str r3, [r4, #0x3c]
005eaf2c: str r3, [r4, #0x40]
005eaf30: str r3, [r4, #0x44]
005eaf34: str r3, [r4, #0x68]
005eaf38: str r3, [r4, #0x6c]
005eaf3c: str r3, [r4, #0x70]
005eaf40: str r3, [r4, #0x48]
005eaf44: str r3, [r4, #0x4c]
005eaf48: str r3, [r4, #0x50]
005eaf4c: str r3, [r4, #0x54]
005eaf50: str r3, [r4, #0x58]
005eaf54: str r3, [r4, #0x5c]
005eaf58: str r3, [r4, #0x60]
005eaf5c: str r2, [r4, #0x74]
005eaf60: bl #0x603480
005eaf64: cmp r0, #0
005eaf68: str r0, [sp, #0x24]
005eaf6c: ldrne r3, [r0, #4]
005eaf70: addne r3, r3, #1
005eaf74: strne r3, [r0, #4]
005eaf78: ldr r1, [r4, #0x34]
005eaf7c: ldr r3, [r4, #0x38]
005eaf80: cmp r1, r3
005eaf84: beq #0x5eb324
005eaf88: ldr r3, [sp, #0x24]
005eaf8c: cmp r3, #0
005eaf90: str r3, [r1]
005eaf94: ldrne r2, [r3, #4]
005eaf98: addne r2, r2, #1
005eaf9c: strne r2, [r3, #4]
005eafa0: ldr r3, [r4, #0x34]
005eafa4: add r3, r3, #4
005eafa8: str r3, [r4, #0x34]
005eafac: ldr r0, [sp, #0x24]
005eafb0: cmp r0, #0
005eafb4: beq #0x5eafbc
005eafb8: bl #0x31d584
005eafbc: bl #0x604bc0
005eafc0: cmp r0, #0
005eafc4: str r0, [sp, #0x20]
005eafc8: ldrne r3, [r0, #4]
005eafcc: addne r3, r3, #1
005eafd0: strne r3, [r0, #4]
005eafd4: ldr r1, [r4, #0x34]
005eafd8: ldr r3, [r4, #0x38]
005eafdc: cmp r1, r3
005eafe0: beq #0x5eb2d4
005eafe4: ldr r3, [sp, #0x20]
005eafe8: cmp r3, #0
005eafec: str r3, [r1]
005eaff0: ldrne r2, [r3, #4]
005eaff4: addne r2, r2, #1
005eaff8: strne r2, [r3, #4]
005eaffc: ldr r3, [r4, #0x34]
005eb000: add r3, r3, #4
005eb004: str r3, [r4, #0x34]
005eb008: ldr r0, [sp, #0x20]
005eb00c: cmp r0, #0
005eb010: beq #0x5eb018
005eb014: bl #0x31d584
005eb018: bl #0x60632c
005eb01c: cmp r0, #0
005eb020: str r0, [sp, #0x1c]
005eb024: ldrne r3, [r0, #4]
005eb028: addne r3, r3, #1
005eb02c: strne r3, [r0, #4]
005eb030: ldr r1, [r4, #0x34]
005eb034: ldr r3, [r4, #0x38]
005eb038: cmp r1, r3
005eb03c: beq #0x5eb2c4
005eb040: ldr r3, [sp, #0x1c]
005eb044: cmp r3, #0
005eb048: str r3, [r1]
005eb04c: ldrne r2, [r3, #4]
005eb050: addne r2, r2, #1
005eb054: strne r2, [r3, #4]
005eb058: ldr r3, [r4, #0x34]
005eb05c: add r3, r3, #4
005eb060: str r3, [r4, #0x34]
005eb064: ldr r0, [sp, #0x1c]
005eb068: cmp r0, #0
005eb06c: beq #0x5eb074
005eb070: bl #0x31d584
005eb074: bl #0x602dc4
005eb078: cmp r0, #0
005eb07c: str r0, [sp, #0x18]
005eb080: ldrne r3, [r0, #4]
005eb084: addne r3, r3, #1
005eb088: strne r3, [r0, #4]
005eb08c: ldr r1, [r4, #0x34]
005eb090: ldr r3, [r4, #0x38]
005eb094: cmp r1, r3
005eb098: beq #0x5eb2f4
005eb09c: ldr r3, [sp, #0x18]
005eb0a0: cmp r3, #0
005eb0a4: str r3, [r1]
005eb0a8: ldrne r2, [r3, #4]
005eb0ac: addne r2, r2, #1
005eb0b0: strne r2, [r3, #4]
005eb0b4: ldr r3, [r4, #0x34]
005eb0b8: add r3, r3, #4
005eb0bc: str r3, [r4, #0x34]
005eb0c0: ldr r0, [sp, #0x18]
005eb0c4: cmp r0, #0
005eb0c8: beq #0x5eb0d0
005eb0cc: bl #0x31d584
005eb0d0: bl #0x60500c
005eb0d4: cmp r0, #0
005eb0d8: str r0, [sp, #0x14]
005eb0dc: ldrne r3, [r0, #4]
005eb0e0: addne r3, r3, #1
005eb0e4: strne r3, [r0, #4]
005eb0e8: ldr r1, [r4, #0x34]
005eb0ec: ldr r3, [r4, #0x38]
005eb0f0: cmp r1, r3
005eb0f4: beq #0x5eb304
005eb0f8: ldr r3, [sp, #0x14]
005eb0fc: cmp r3, #0
005eb100: str r3, [r1]
005eb104: ldrne r2, [r3, #4]
005eb108: addne r2, r2, #1
005eb10c: strne r2, [r3, #4]
005eb110: ldr r3, [r4, #0x34]
005eb114: add r3, r3, #4
005eb118: str r3, [r4, #0x34]
005eb11c: ldr r0, [sp, #0x14]
005eb120: cmp r0, #0
005eb124: beq #0x5eb12c
005eb128: bl #0x31d584
005eb12c: bl #0x604344
005eb130: cmp r0, #0
005eb134: str r0, [sp, #0x10]
005eb138: ldrne r3, [r0, #4]
005eb13c: addne r3, r3, #1
005eb140: strne r3, [r0, #4]
005eb144: ldr r1, [r4, #0x34]
005eb148: ldr r3, [r4, #0x38]
005eb14c: cmp r1, r3
005eb150: beq #0x5eb314
005eb154: ldr r3, [sp, #0x10]
005eb158: cmp r3, #0
005eb15c: str r3, [r1]
005eb160: ldrne r2, [r3, #4]
005eb164: addne r2, r2, #1
005eb168: strne r2, [r3, #4]
005eb16c: ldr r3, [r4, #0x34]
005eb170: add r3, r3, #4
005eb174: str r3, [r4, #0x34]
005eb178: ldr r0, [sp, #0x10]
005eb17c: cmp r0, #0
005eb180: beq #0x5eb188
005eb184: bl #0x31d584
005eb188: bl #0x6057c0
005eb18c: cmp r0, #0
005eb190: str r0, [sp, #0xc]
005eb194: ldrne r3, [r0, #4]
005eb198: addne r3, r3, #1
005eb19c: strne r3, [r0, #4]
005eb1a0: ldr r1, [r4, #0x34]
005eb1a4: ldr r3, [r4, #0x38]
005eb1a8: cmp r1, r3
005eb1ac: beq #0x5eb2e4
005eb1b0: ldr r3, [sp, #0xc]
005eb1b4: cmp r3, #0
005eb1b8: str r3, [r1]
005eb1bc: ldrne r2, [r3, #4]
005eb1c0: addne r2, r2, #1
005eb1c4: strne r2, [r3, #4]
005eb1c8: ldr r3, [r4, #0x34]
005eb1cc: add r3, r3, #4
005eb1d0: str r3, [r4, #0x34]
005eb1d4: ldr r0, [sp, #0xc]
005eb1d8: cmp r0, #0
005eb1dc: beq #0x5eb1e4
005eb1e0: bl #0x31d584
005eb1e4: bl #0x606d10
005eb1e8: ldr r1, [r4, #0x40]
005eb1ec: ldr r3, [r4, #0x44]
005eb1f0: add r5, r4, #0x3c
005eb1f4: str r0, [sp, #8]
005eb1f8: cmp r1, r3
005eb1fc: beq #0x5eb26c
005eb200: str r0, [r1]
005eb204: ldr r3, [r4, #0x40]
005eb208: add r3, r3, #4
005eb20c: str r3, [r4, #0x40]
005eb210: bl #0x60762c
005eb214: ldr r1, [r4, #0x40]
005eb218: ldr r3, [r4, #0x44]
005eb21c: str r0, [sp, #4]
005eb220: cmp r1, r3
005eb224: beq #0x5eb290
005eb228: str r0, [r1]
005eb22c: ldr r3, [r4, #0x40]
005eb230: add r3, r3, #4
005eb234: str r3, [r4, #0x40]
005eb238: bl #0x6072c0
005eb23c: ldr r1, [r4, #0x40]
005eb240: ldr r3, [r4, #0x44]
005eb244: str r0, [sp]
005eb248: cmp r1, r3
005eb24c: beq #0x5eb2b4
005eb250: str r0, [r1]
005eb254: ldr r3, [r4, #0x40]
005eb258: add r3, r3, #4
005eb25c: str r3, [r4, #0x40]
005eb260: mov r0, r4
005eb264: add sp, sp, #0x2c
005eb268: pop {r4, r5, pc}
005eb26c: mov r0, r5
005eb270: add r2, sp, #8
005eb274: bl #0x5ea990
005eb278: bl #0x60762c
005eb27c: ldr r1, [r4, #0x40]
005eb280: ldr r3, [r4, #0x44]
005eb284: str r0, [sp, #4]
005eb288: cmp r1, r3
005eb28c: bne #0x5eb228
005eb290: mov r0, r5
005eb294: add r2, sp, #4
005eb298: bl #0x5ea990
005eb29c: bl #0x6072c0
005eb2a0: ldr r1, [r4, #0x40]
005eb2a4: ldr r3, [r4, #0x44]
005eb2a8: str r0, [sp]
005eb2ac: cmp r1, r3
005eb2b0: bne #0x5eb250
005eb2b4: mov r0, r5
005eb2b8: mov r2, sp
005eb2bc: bl #0x5ea990
005eb2c0: b #0x5eb260
005eb2c4: mov r0, r5
005eb2c8: add r2, sp, #0x1c
005eb2cc: bl #0x5e8fdc
005eb2d0: b #0x5eb064
005eb2d4: mov r0, r5
005eb2d8: add r2, sp, #0x20
005eb2dc: bl #0x5e8fdc
005eb2e0: b #0x5eb008
005eb2e4: mov r0, r5
005eb2e8: add r2, sp, #0xc
005eb2ec: bl #0x5e8fdc
005eb2f0: b #0x5eb1d4
005eb2f4: mov r0, r5
005eb2f8: add r2, sp, #0x18
005eb2fc: bl #0x5e8fdc
005eb300: b #0x5eb0c0
005eb304: mov r0, r5
005eb308: add r2, sp, #0x14
005eb30c: bl #0x5e8fdc
005eb310: b #0x5eb11c
005eb314: mov r0, r5
005eb318: add r2, sp, #0x10
005eb31c: bl #0x5e8fdc
005eb320: b #0x5eb178
005eb324: mov r0, r5
005eb328: add r2, sp, #0x24
005eb32c: bl #0x5e8fdc
005eb330: b #0x5eafac

_ZN6glitch5video11CNullDriver17createTextureImplEPKcRKNS0_12STextureDescE
005b958c: push {r4, r5, r6, r7, r8, lr}
005b9590: mov r5, r0
005b9594: mov r6, r1
005b9598: mov r0, #0x54
005b959c: mov r1, #0
005b95a0: mov r8, r2
005b95a4: mov r7, r3
005b95a8: bl #0x5341ac
005b95ac: mov r3, r7
005b95b0: mov r1, r8
005b95b4: mov r2, r6
005b95b8: mov r4, r0
005b95bc: bl #0x5b9558
005b95c0: cmp r4, #0
005b95c4: str r4, [r5]
005b95c8: ldrne r3, [r4, #4]
005b95cc: mov r0, r5
005b95d0: addne r3, r3, #1
005b95d4: strne r3, [r4, #4]
005b95d8: pop {r4, r5, r6, r7, r8, pc}

_ZN6glitch5video11CNullDriver8CTextureC2EPKcPS1_RKNS0_12STextureDescE
005b95dc: push {r4, r5, r6, lr}
005b95e0: ldr r4, [pc, #0x20]
005b95e4: mov r5, r0
005b95e8: bl #0x5fe6e0
005b95ec: ldr r3, [pc, #0x18]
005b95f0: add r4, pc, r4
005b95f4: mov r0, r5
005b95f8: ldr r3, [r4, r3]
005b95fc: add r3, r3, #8
005b9600: str r3, [r5]
005b9604: pop {r4, r5, r6, pc}
005b9608: eorseq fp, sp, r0, lsr #9
005b960c: andeq r2, r0, r0, ror r1

_ZN6glitch5video12IVideoDriverC2EPNS_7IDeviceEPNS0_14IShaderManagerEPNS0_24CMaterialRendererManagerEPNS0_15CTextureManagerEPNS0_31CGlobalMaterialParameterManagerERKN5boost13intrusive_ptrINS0_6CLightEEE
005aaf1c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005aaf20: ldr r7, [pc, #0x3b0]
005aaf24: ldr ip, [pc, #0x3b0]
005aaf28: ldr fp, [pc, #0x3b0]
005aaf2c: add r7, pc, r7
005aaf30: ldr ip, [r7, ip]
005aaf34: ldr lr, [r7, fp]
005aaf38: mov r4, r0
005aaf3c: mov r5, #1
005aaf40: add r0, ip, #8
005aaf44: mov ip, r4
005aaf48: ldr lr, [lr]
005aaf4c: str r5, [r4, #4]
005aaf50: str r0, [ip], #8
005aaf54: sub sp, sp, #0x11c
005aaf58: str ip, [r4, #0x18]
005aaf5c: str ip, [r4, #0x1c]
005aaf60: str r3, [sp]
005aaf64: ldr r3, [sp, #0x140]
005aaf68: mov r0, ip
005aaf6c: mov r8, r2
005aaf70: str lr, [sp, #0x114]
005aaf74: mov r6, r1
005aaf78: ldr sl, [sp, #0x148]
005aaf7c: str r3, [sp, #4]
005aaf80: ldr sb, [sp, #0x144]
005aaf84: bl #0x5aaf18
005aaf88: ldr r2, [r4, #0x18]
005aaf8c: mov r5, #0
005aaf90: add r3, r4, #0x20
005aaf94: strb r5, [r2]
005aaf98: mov r0, r3
005aaf9c: str r3, [r4, #0x30]
005aafa0: str r3, [r4, #0x34]
005aafa4: bl #0x5aaf18
005aafa8: ldr r3, [r4, #0x30]
005aafac: add r0, r4, #0x50
005aafb0: strb r5, [r3]
005aafb4: mvn r3, #0
005aafb8: strh r5, [r4, #0x3e]
005aafbc: strh r5, [r4, #0x3a]
005aafc0: strh r5, [r4, #0x3c]
005aafc4: strh r3, [r4, #0x38]
005aafc8: ldr r3, [sl]
005aafcc: mov sl, #0
005aafd0: cmp r3, r5
005aafd4: str r3, [r4, #0x40]
005aafd8: ldrne r2, [r3]
005aafdc: mov r5, #8
005aafe0: addne r2, r2, #1
005aafe4: strne r2, [r3]
005aafe8: str sl, [r4, #0x44]
005aafec: str sl, [r4, #0x48]
005aaff0: str r5, [r4, #0x4c]
005aaff4: bl #0x6da980
005aaff8: mov r3, #0x10
005aaffc: str r3, [r4, #0x88]
005ab000: mov r3, #0x400
005ab004: str r3, [r4, #0x10c]
005ab008: str r6, [r4, #0xd4]
005ab00c: ldr r3, [sp]
005ab010: mvn r2, #0
005ab014: movw r0, #0x136
005ab018: str r3, [r4, #0xdc]
005ab01c: ldr r3, [sp, #4]
005ab020: add r1, r4, #0x14c
005ab024: strb r2, [r4, #0xf8]
005ab028: str r3, [r4, #0xe0]
005ab02c: strb r2, [r4, #0xf9]
005ab030: str r8, [r4, #0xd8]
005ab034: str sb, [r4, #0xe4]
005ab038: str sl, [r4, #0x78]
005ab03c: str sl, [r4, #0x7c]
005ab040: str sl, [r4, #0x80]
005ab044: str sl, [r4, #0x84]
005ab048: str sl, [r4, #0xa0]
005ab04c: str sl, [r4, #0xa4]
005ab050: str sl, [r4, #0xa8]
005ab054: str sl, [r4, #0xac]
005ab058: str sl, [r4, #0xb0]
005ab05c: str sl, [r4, #0xb4]
005ab060: str sl, [r4, #0xb8]
005ab064: str sl, [r4, #0xbc]
005ab068: str sl, [r4, #0xc0]
005ab06c: str sl, [r4, #0xc4]
005ab070: str sl, [r4, #0xc8]
005ab074: str sl, [r4, #0xcc]
005ab078: str sl, [r4, #0xd0]
005ab07c: str sl, [r4, #0xe8]
005ab080: str sl, [r4, #0xec]
005ab084: str sl, [r4, #0xf0]
005ab088: str sl, [r4, #0xf4]
005ab08c: str sl, [r4, #0x104]
005ab090: str sl, [r4, #0x108]
005ab094: str sl, [r4, #0x110]
005ab098: str sl, [r4, #0x114]
005ab09c: strh r2, [r4, r0]
005ab0a0: mov r2, #0x40
005ab0a4: str sl, [r4, #0x118]
005ab0a8: str sl, [r4, #0x120]
005ab0ac: str sl, [r4, #0x124]
005ab0b0: str sl, [r4, #0x128]
005ab0b4: str sl, [r4, #0x12c]
005ab0b8: str sl, [r4, #0x130]
005ab0bc: strb sl, [r4, #0x135]
005ab0c0: str sl, [r4, #0x138]
005ab0c4: str sl, [r4, #0x13c]
005ab0c8: str sl, [r4, #0x140]
005ab0cc: str sl, [r4, #0x144]
005ab0d0: str sl, [r4, #0x148]
005ab0d4: str sl, [r4, #0x14c]
005ab0d8: str sl, [r1, #4]
005ab0dc: str r2, [r4, #0x9c]
005ab0e0: str sl, [r4, #0x15c]
005ab0e4: str sl, [r4, #0x154]
005ab0e8: str sl, [r4, #0x158]
005ab0ec: mov r0, r8
005ab0f0: ldr r3, [r8]
005ab0f4: mov r1, r4
005ab0f8: mov r2, #1
005ab0fc: add r8, r4, #0xfa
005ab100: mov lr, pc
005ab104: ldr pc, [r3, #8]
005ab108: mov r1, #0xff
005ab10c: mov r2, r5
005ab110: mov r0, r8
005ab114: bl #0x30e460
005ab118: ldr r1, [r4, #0x40]
005ab11c: cmp r1, sl
005ab120: beq #0x5ab234
005ab124: ldr r1, [r4, #0xdc]
005ab128: cmp r1, #0
005ab12c: beq #0x5ab284
005ab130: ldr r1, [r4, #0xe0]
005ab134: cmp r1, #0
005ab138: beq #0x5ab2ac
005ab13c: ldr r1, [r4, #0xe4]
005ab140: cmp r1, #0
005ab144: beq #0x5ab20c
005ab148: ldr r1, [pc, #0x194]
005ab14c: ldr r2, [pc, #0x194]
005ab150: add r6, sp, #0x14
005ab154: add r1, pc, r1
005ab158: add r2, pc, r2
005ab15c: mov r0, r6
005ab160: bl #0x30eae4
005ab164: ldr r0, [r4, #0xe4]
005ab168: mov r1, r6
005ab16c: bl #0x5bb378
005ab170: movw r3, #0xffff
005ab174: cmp r0, r3
005ab178: strh r0, [r4, #0x38]
005ab17c: beq #0x5ab1dc
005ab180: ldr sl, [pc, #0x164]
005ab184: ldr sb, [pc, #0x164]
005ab188: mov r5, #0
005ab18c: add sl, pc, sl
005ab190: add sb, pc, sb
005ab194: mov r3, r5
005ab198: mov r1, sl
005ab19c: mov r2, sb
005ab1a0: mov r0, r6
005ab1a4: bl #0x30eae4
005ab1a8: ldr r0, [r4, #0xe4]
005ab1ac: mov r1, r6
005ab1b0: bl #0x5bb378
005ab1b4: add r5, r5, #1
005ab1b8: cmp r5, #4
005ab1bc: strh r0, [r8], #2
005ab1c0: bne #0x5ab194
005ab1c4: ldr r1, [pc, #0x128]
005ab1c8: ldr r0, [r4, #0xe4]
005ab1cc: add r1, pc, r1
005ab1d0: bl #0x5bb378
005ab1d4: movw r3, #0x136
005ab1d8: strh r0, [r4, r3]
005ab1dc: ldr r2, [r4, #0xdc]
005ab1e0: ldr r3, [r7, fp]
005ab1e4: mov r1, #0
005ab1e8: str r1, [r4, #0x108]
005ab1ec: str r2, [r4, #0x104]
005ab1f0: ldr r2, [sp, #0x114]
005ab1f4: ldr r3, [r3]
005ab1f8: mov r0, r4
005ab1fc: cmp r2, r3
005ab200: bne #0x5ab2d4
005ab204: add sp, sp, #0x11c
005ab208: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005ab20c: mov r0, #0x3c
005ab210: bl #0x5341ac
005ab214: mov r1, r4
005ab218: mov r5, r0
005ab21c: bl #0x5b9e50
005ab220: ldr r3, [r4, #0x138]
005ab224: str r5, [r4, #0xe4]
005ab228: orr r3, r3, #0x20
005ab22c: str r3, [r4, #0x138]
005ab230: b #0x5ab1dc
005ab234: add r5, sp, #0x10
005ab238: mov r0, r5
005ab23c: bl #0x59feb8
005ab240: ldr r3, [sp, #0x10]
005ab244: add r0, sp, #0x118
005ab248: str r3, [sp, #0xc]
005ab24c: cmp r3, sl
005ab250: ldrne r2, [r3]
005ab254: addne r2, r2, #1
005ab258: strne r2, [r3]
005ab25c: ldrne r3, [sp, #0xc]
005ab260: ldr r2, [r4, #0x40]
005ab264: str r3, [r4, #0x40]
005ab268: str r2, [r0, #-0x10c]!
005ab26c: bl #0x5aa3c8
005ab270: mov r0, r5
005ab274: bl #0x5aa3c8
005ab278: mov r0, r4
005ab27c: bl #0x5a8ae8
005ab280: b #0x5ab124
005ab284: mov r0, #0x98
005ab288: bl #0x5341ac
005ab28c: mov r1, r4
005ab290: mov r5, r0
005ab294: bl #0x5d8f24
005ab298: ldr r3, [r4, #0x138]
005ab29c: str r5, [r4, #0xdc]
005ab2a0: orr r3, r3, #0x10
005ab2a4: str r3, [r4, #0x138]
005ab2a8: b #0x5ab130
005ab2ac: mov r0, #0x78
005ab2b0: bl #0x5341ac
005ab2b4: mov r1, r4
005ab2b8: mov r5, r0
005ab2bc: bl #0x5eaa7c
005ab2c0: ldr r3, [r4, #0x138]
005ab2c4: str r5, [r4, #0xe0]
005ab2c8: orr r3, r3, #0x20
005ab2cc: str r3, [r4, #0x138]
005ab2d0: b #0x5ab13c
005ab2d4: bl #0x30e310
005ab2d8: eorseq sb, lr, r4, ror #22
005ab2dc: andeq r2, r0, r4, lsl #31
005ab2e0: andeq r4, r0, ip, lsr #1
005ab2e4: eorseq r4, r3, r4, lsr #28
005ab2e8: eorseq r4, r3, r8, lsr #28
005ab2ec: eorseq r4, r3, r4, lsl #28
005ab2f0: eorseq r4, r3, r0, lsr #28
005ab2f4: eorseq r4, r3, ip, asr #27

_ZN6glitch5video19CCommonGLDriverBase14initExtensionsEPKh
006dd734: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006dd738: ldr r6, [pc, #0x134]
006dd73c: ldr r8, [pc, #0x134]
006dd740: sub sp, sp, #0x410
006dd744: add r6, pc, r6
006dd748: ldr r3, [r6, r8]
006dd74c: sub sp, sp, #4
006dd750: subs r5, r1, #0
006dd754: ldr r3, [r3]
006dd758: mov sl, r0
006dd75c: str r3, [sp, #0x40c]
006dd760: beq #0x6dd850
006dd764: mov r0, r5
006dd768: bl #0x30de54
006dd76c: add r0, r0, #1
006dd770: bl #0x5345f4
006dd774: mov r7, r0
006dd778: ldr r0, [pc, #0xfc]
006dd77c: mov r1, #1
006dd780: add r0, pc, r0
006dd784: bl #0x60aca0
006dd788: ldrb r3, [r5]
006dd78c: cmp r3, #0
006dd790: beq #0x6dd840
006dd794: ldr fp, [pc, #0xe4]
006dd798: add sb, sp, #0x10
006dd79c: sub sb, sb, #4
006dd7a0: add fp, pc, fp
006dd7a4: add r4, r7, #1
006dd7a8: mov r2, r7
006dd7ac: b #0x6dd7c0
006dd7b0: ldrb r3, [r5, #1]!
006dd7b4: add r4, r4, #1
006dd7b8: cmp r3, #0
006dd7bc: beq #0x6dd840
006dd7c0: strb r3, [r4, #-1]
006dd7c4: ldrb r3, [r5]
006dd7c8: cmp r3, #0x20
006dd7cc: bne #0x6dd7b0
006dd7d0: mov r3, #0
006dd7d4: strb r3, [r4, #-1]
006dd7d8: mov r0, r2
006dd7dc: str r2, [sp, #4]
006dd7e0: bl #0x6dd6dc
006dd7e4: movw ip, #0xffff
006dd7e8: cmp r0, ip
006dd7ec: ldr r2, [sp, #4]
006dd7f0: beq #0x6dd814
006dd7f4: lsr r3, r0, #5
006dd7f8: add r3, r3, #0x1ec
006dd7fc: add r3, r3, #2
006dd800: ldr r1, [sl, r3, lsl #2]
006dd804: and r0, r0, #0x1f
006dd808: mov ip, #1
006dd80c: orr r0, r1, ip, lsl r0
006dd810: str r0, [sl, r3, lsl #2]
006dd814: mov r1, fp
006dd818: mov r0, sb
006dd81c: bl #0x30eae4
006dd820: mov r0, sb
006dd824: mov r1, #1
006dd828: bl #0x60aca0
006dd82c: ldrb r3, [r5, #1]!
006dd830: mov r2, r4
006dd834: add r4, r4, #1
006dd838: cmp r3, #0
006dd83c: bne #0x6dd7c0
006dd840: cmp r7, #0
006dd844: beq #0x6dd850
006dd848: mov r0, r7
006dd84c: bl #0x534688
006dd850: ldr r3, [r6, r8]
006dd854: ldr r2, [sp, #0x40c]
006dd858: ldr r3, [r3]
006dd85c: cmp r2, r3
006dd860: bne #0x6dd870
006dd864: add sp, sp, #0x14
006dd868: add sp, sp, #0x400
006dd86c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006dd870: bl #0x30e310
006dd874: eoreq r7, fp, ip, asr #6
006dd878: andeq r4, r0, ip, lsr #1

_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17createTextureImplEPKcRKNS0_12STextureDescE
005b5f6c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005b5f70: sub sp, sp, #0x3c
005b5f74: add sl, sp, #0x18
005b5f78: mov r4, r3
005b5f7c: mov ip, sl
005b5f80: mov lr, r3
005b5f84: mov r5, r0
005b5f88: mov r6, r1
005b5f8c: mov r8, r2
005b5f90: ldm r4!, {r0, r1, r2, r3}
005b5f94: stm ip!, {r0, r1, r2, r3}
005b5f98: ldm r4, {r0, r1, r2, r3}
005b5f9c: stm ip!, {r0, r1, r2}
005b5fa0: ldr r7, [sp, #0x28]
005b5fa4: ldr r4, [pc, #0x380]
005b5fa8: strh r3, [ip], #2
005b5fac: sub r2, r7, #1
005b5fb0: tst r2, r7
005b5fb4: lsr r3, r3, #0x10
005b5fb8: strb r3, [ip]
005b5fbc: add r4, pc, r4
005b5fc0: bne #0x5b5fd4
005b5fc4: ldr r3, [sp, #0x2c]
005b5fc8: sub r2, r3, #1
005b5fcc: tst r2, r3
005b5fd0: beq #0x5b61c4
005b5fd4: mov sb, #0
005b5fd8: ldr r3, [r6, #0x7ec]
005b5fdc: tst r3, #8
005b5fe0: beq #0x5b6050
005b5fe4: ldr fp, [sp, #0x18]
005b5fe8: cmp fp, #0
005b5fec: beq #0x5b6050
005b5ff0: cmp fp, #3
005b5ff4: beq #0x5b6050
005b5ff8: cmp sb, #0
005b5ffc: bne #0x5b6050
005b6000: uxth r3, fp
005b6004: cmp r3, #0xff
005b6008: beq #0x5b62d8
005b600c: mov r0, sb
005b6010: bl #0x5fda68
005b6014: ldr r7, [sp, #0x28]
005b6018: ldr r3, [r0, fp, lsl #2]
005b601c: ldr ip, [sp, #0x2c]
005b6020: ldr r1, [pc, #0x308]
005b6024: mov r2, r8
005b6028: str ip, [sp, #4]
005b602c: ldr ip, [sp, #0x30]
005b6030: add r1, pc, r1
005b6034: mov r0, #3
005b6038: str r7, [sp]
005b603c: str ip, [sp, #8]
005b6040: bl #0x60b034
005b6044: mov r3, #0
005b6048: str r3, [r5]
005b604c: b #0x5b616c
005b6050: ldr r7, [sp, #0x1c]
005b6054: ldr r2, [pc, #0x2d8]
005b6058: mov r3, #0x28
005b605c: mul r3, r3, r7
005b6060: ldr r2, [r4, r2]
005b6064: ldr r3, [r2, r3]
005b6068: tst r3, #0x30
005b606c: bne #0x5b6178
005b6070: ldrb r2, [sp, #0x35]
005b6074: cmp r2, #0
005b6078: beq #0x5b623c
005b607c: mov r3, #0x14
005b6080: mla r7, r3, r7, r6
005b6084: add r7, r7, #0x4a0
005b6088: add r7, r7, #8
005b608c: ldrh fp, [r7, #6]
005b6090: ldr r7, [lr, #4]
005b6094: str fp, [sp, #0x1c]
005b6098: cmp fp, r7
005b609c: beq #0x5b6110
005b60a0: cmp fp, #0x27
005b60a4: beq #0x5b61ec
005b60a8: uxth r3, r7
005b60ac: cmp r3, #0x27
005b60b0: beq #0x5b628c
005b60b4: mov r0, #0
005b60b8: bl #0x5ed944
005b60bc: ldrb r2, [sp, #0x35]
005b60c0: ldr r3, [r0, r7, lsl #2]
005b60c4: ldr fp, [sp, #0x1c]
005b60c8: cmp r2, #0
005b60cc: bne #0x5b6254
005b60d0: ldr r7, [pc, #0x260]
005b60d4: add r7, pc, r7
005b60d8: uxth r2, fp
005b60dc: cmp r2, #0x27
005b60e0: beq #0x5b6298
005b60e4: mov r0, #0
005b60e8: str r3, [sp, #0x14]
005b60ec: bl #0x5ed944
005b60f0: ldr r3, [sp, #0x14]
005b60f4: ldr ip, [r0, fp, lsl #2]
005b60f8: ldr r1, [pc, #0x23c]
005b60fc: mov r0, #2
005b6100: mov r2, r8
005b6104: add r1, pc, r1
005b6108: stm sp, {r7, ip}
005b610c: bl #0x60b034
005b6110: ldr r7, [sp, #0x20]
005b6114: cmp r7, #2
005b6118: beq #0x5b6260
005b611c: cmp r7, #3
005b6120: beq #0x5b6224
005b6124: cmp r7, #0
005b6128: bne #0x5b62a4
005b612c: mov r1, #0
005b6130: mov r0, #0x5c
005b6134: bl #0x5341ac
005b6138: mov r3, sl
005b613c: mov r1, r8
005b6140: mov r2, r6
005b6144: mov r7, r0
005b6148: bl #0x6dded8
005b614c: ldr r3, [pc, #0x1ec]
005b6150: ldr r3, [r4, r3]
005b6154: add r3, r3, #8
005b6158: str r3, [r7]
005b615c: str r7, [r5]
005b6160: ldr r3, [r7, #4]
005b6164: add r3, r3, #1
005b6168: str r3, [r7, #4]
005b616c: mov r0, r5
005b6170: add sp, sp, #0x3c
005b6174: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005b6178: ldr r3, [sp, #0x18]
005b617c: cmp r3, #0
005b6180: beq #0x5b6070
005b6184: cmp r3, #2
005b6188: beq #0x5b6070
005b618c: uxth r3, r7
005b6190: cmp r3, #0x27
005b6194: beq #0x5b6320
005b6198: mov r0, #0
005b619c: bl #0x5ed944
005b61a0: ldr r3, [r0, r7, lsl #2]
005b61a4: ldr r1, [pc, #0x198]
005b61a8: mov r2, r8
005b61ac: mov r0, #3
005b61b0: add r1, pc, r1
005b61b4: bl #0x60b034
005b61b8: mov r3, #0
005b61bc: str r3, [r5]
005b61c0: b #0x5b616c
005b61c4: ldr r3, [sp, #0x18]
005b61c8: cmp r3, #1
005b61cc: movne sb, #1
005b61d0: bne #0x5b5fd8
005b61d4: ldr r3, [sp, #0x30]
005b61d8: sub r2, r3, #1
005b61dc: tst r2, r3
005b61e0: movne sb, #0
005b61e4: moveq sb, #1
005b61e8: b #0x5b5fd8
005b61ec: uxth r3, r7
005b61f0: cmp r3, #0x27
005b61f4: beq #0x5b62e4
005b61f8: mov r0, #0
005b61fc: bl #0x5ed944
005b6200: ldr r3, [r0, r7, lsl #2]
005b6204: ldr r1, [pc, #0x13c]
005b6208: mov r2, r8
005b620c: mov r0, #3
005b6210: add r1, pc, r1
005b6214: bl #0x60b034
005b6218: mov r3, #0
005b621c: str r3, [r5]
005b6220: b #0x5b616c
005b6224: cmp sb, #0
005b6228: beq #0x5b62f0
005b622c: mov r0, #0
005b6230: bl #0x5fda78
005b6234: ldr r3, [r0, r7, lsl #2]
005b6238: b #0x5b62b8
005b623c: mov r3, #0x14
005b6240: mla r7, r3, r7, r6
005b6244: add r7, r7, #0x4a0
005b6248: add r7, r7, #8
005b624c: ldrh fp, [r7, #4]
005b6250: b #0x5b6090
005b6254: ldr r7, [pc, #0xf0]
005b6258: add r7, pc, r7
005b625c: b #0x5b60d8
005b6260: mov r0, #0
005b6264: bl #0x5fda78
005b6268: ldr r1, [pc, #0xe0]
005b626c: ldr r3, [r0, #8]
005b6270: mov r2, r8
005b6274: add r1, pc, r1
005b6278: mov r0, #3
005b627c: bl #0x60b034
005b6280: mov r3, #0
005b6284: str r3, [r5]
005b6288: b #0x5b616c
005b628c: ldr r3, [pc, #0xc0]
005b6290: add r3, pc, r3
005b6294: b #0x5b60c8
005b6298: ldr ip, [pc, #0xb8]
005b629c: add ip, pc, ip
005b62a0: b #0x5b60f8
005b62a4: uxth r3, r7
005b62a8: cmp r3, #0xff
005b62ac: bne #0x5b622c
005b62b0: ldr r3, [pc, #0xa4]
005b62b4: add r3, pc, r3
005b62b8: ldr r1, [pc, #0xa0]
005b62bc: mov r0, #2
005b62c0: mov r2, r8
005b62c4: add r1, pc, r1
005b62c8: bl #0x60b034
005b62cc: mov r3, #0
005b62d0: str r3, [sp, #0x20]
005b62d4: b #0x5b612c
005b62d8: ldr r3, [pc, #0x84]
005b62dc: add r3, pc, r3
005b62e0: b #0x5b601c
005b62e4: ldr r3, [pc, #0x7c]
005b62e8: add r3, pc, r3
005b62ec: b #0x5b6204
005b62f0: ldr ip, [sp, #0x2c]
005b62f4: ldr r1, [pc, #0x70]
005b62f8: mov r0, r7
005b62fc: str ip, [sp]
005b6300: ldr ip, [sp, #0x30]
005b6304: add r1, pc, r1
005b6308: mov r2, r8
005b630c: ldr r3, [sp, #0x28]
005b6310: str ip, [sp, #4]
005b6314: bl #0x60b034
005b6318: str sb, [r5]
005b631c: b #0x5b616c
005b6320: ldr r3, [pc, #0x48]
005b6324: add r3, pc, r3
005b6328: b #0x5b61a4
005b632c: ldrsbteq lr, [sp], -r4
005b6330: eorseq sl, r2, r0, asr #13
005b6334: andeq r1, r0, r4, lsr pc
005b6338: eorseq r6, r3, r4, asr #24
005b633c: eorseq sl, r2, r4, lsr #13
005b6340: andeq r0, r0, r8, ror #18
005b6344: eorseq sl, r2, r8, ror r5
005b6348: eorseq sl, r2, r8, asr r5
005b634c: eorseq sl, r2, r0, asr #10
005b6350: eorseq sl, r2, ip, ror r5
005b6354: ldrsbteq r0, [r1], -r0
005b6358: eorseq r0, r1, r4, asr #3
005b635c: eorseq r0, r1, ip, lsr #3
005b6360: eorseq sl, r2, r4, lsr #11
005b6364: eorseq r0, r1, r4, lsl #3
005b6368: eorseq r0, r1, r8, ror r1
005b636c: eorseq sl, r2, r4, lsl r5
005b6370: eorseq r0, r1, ip, lsr r1

_ZN6glitch5video15CTextureManagerC1EPNS0_12IVideoDriverE
005eaa7c: push {r4, r5, lr}
005eaa80: sub sp, sp, #0x2c
005eaa84: mov r4, r0
005eaa88: mov r5, r1
005eaa8c: bl #0x5e840c
005eaa90: str r5, [r4, #0x28]
005eaa94: ldr r3, [r5, #0xd4]
005eaa98: add r5, r4, #0x30
005eaa9c: ldr r3, [r3, #0x34]
005eaaa0: cmp r3, #0
005eaaa4: str r3, [r4, #0x2c]
005eaaa8: ldrne r2, [r3, #4]
005eaaac: addne r2, r2, #1
005eaab0: strne r2, [r3, #4]
005eaab4: mov r3, #0
005eaab8: mov r2, #0x43
005eaabc: str r3, [r4, #0x64]
005eaac0: str r3, [r4, #0x30]
005eaac4: str r3, [r4, #0x34]
005eaac8: str r3, [r4, #0x38]
005eaacc: str r3, [r4, #0x3c]
005eaad0: str r3, [r4, #0x40]
005eaad4: str r3, [r4, #0x44]
005eaad8: str r3, [r4, #0x68]
005eaadc: str r3, [r4, #0x6c]
005eaae0: str r3, [r4, #0x70]
005eaae4: str r3, [r4, #0x48]
005eaae8: str r3, [r4, #0x4c]
005eaaec: str r3, [r4, #0x50]
005eaaf0: str r3, [r4, #0x54]
005eaaf4: str r3, [r4, #0x58]
005eaaf8: str r3, [r4, #0x5c]
005eaafc: str r3, [r4, #0x60]
005eab00: str r2, [r4, #0x74]
005eab04: bl #0x603480
005eab08: cmp r0, #0
005eab0c: str r0, [sp, #0x24]
005eab10: ldrne r3, [r0, #4]
005eab14: addne r3, r3, #1
005eab18: strne r3, [r0, #4]
005eab1c: ldr r1, [r4, #0x34]
005eab20: ldr r3, [r4, #0x38]
005eab24: cmp r1, r3
005eab28: beq #0x5eaec8
005eab2c: ldr r3, [sp, #0x24]
005eab30: cmp r3, #0
005eab34: str r3, [r1]
005eab38: ldrne r2, [r3, #4]
005eab3c: addne r2, r2, #1
005eab40: strne r2, [r3, #4]
005eab44: ldr r3, [r4, #0x34]
005eab48: add r3, r3, #4
005eab4c: str r3, [r4, #0x34]
005eab50: ldr r0, [sp, #0x24]
005eab54: cmp r0, #0
005eab58: beq #0x5eab60
005eab5c: bl #0x31d584
005eab60: bl #0x604bc0
005eab64: cmp r0, #0
005eab68: str r0, [sp, #0x20]
005eab6c: ldrne r3, [r0, #4]
005eab70: addne r3, r3, #1
005eab74: strne r3, [r0, #4]
005eab78: ldr r1, [r4, #0x34]
005eab7c: ldr r3, [r4, #0x38]
005eab80: cmp r1, r3
005eab84: beq #0x5eae78
005eab88: ldr r3, [sp, #0x20]
005eab8c: cmp r3, #0
005eab90: str r3, [r1]
005eab94: ldrne r2, [r3, #4]
005eab98: addne r2, r2, #1
005eab9c: strne r2, [r3, #4]
005eaba0: ldr r3, [r4, #0x34]
005eaba4: add r3, r3, #4
005eaba8: str r3, [r4, #0x34]
005eabac: ldr r0, [sp, #0x20]
005eabb0: cmp r0, #0
005eabb4: beq #0x5eabbc
005eabb8: bl #0x31d584
005eabbc: bl #0x60632c
005eabc0: cmp r0, #0
005eabc4: str r0, [sp, #0x1c]
005eabc8: ldrne r3, [r0, #4]
005eabcc: addne r3, r3, #1
005eabd0: strne r3, [r0, #4]
005eabd4: ldr r1, [r4, #0x34]
005eabd8: ldr r3, [r4, #0x38]
005eabdc: cmp r1, r3
005eabe0: beq #0x5eae68
005eabe4: ldr r3, [sp, #0x1c]
005eabe8: cmp r3, #0
005eabec: str r3, [r1]
005eabf0: ldrne r2, [r3, #4]
005eabf4: addne r2, r2, #1
005eabf8: strne r2, [r3, #4]
005eabfc: ldr r3, [r4, #0x34]
005eac00: add r3, r3, #4
005eac04: str r3, [r4, #0x34]
005eac08: ldr r0, [sp, #0x1c]
005eac0c: cmp r0, #0
005eac10: beq #0x5eac18
005eac14: bl #0x31d584
005eac18: bl #0x602dc4
005eac1c: cmp r0, #0
005eac20: str r0, [sp, #0x18]
005eac24: ldrne r3, [r0, #4]
005eac28: addne r3, r3, #1
005eac2c: strne r3, [r0, #4]
005eac30: ldr r1, [r4, #0x34]
005eac34: ldr r3, [r4, #0x38]
005eac38: cmp r1, r3
005eac3c: beq #0x5eae98
005eac40: ldr r3, [sp, #0x18]
005eac44: cmp r3, #0
005eac48: str r3, [r1]
005eac4c: ldrne r2, [r3, #4]
005eac50: addne r2, r2, #1
005eac54: strne r2, [r3, #4]
005eac58: ldr r3, [r4, #0x34]
005eac5c: add r3, r3, #4
005eac60: str r3, [r4, #0x34]
005eac64: ldr r0, [sp, #0x18]
005eac68: cmp r0, #0
005eac6c: beq #0x5eac74
005eac70: bl #0x31d584
005eac74: bl #0x60500c
005eac78: cmp r0, #0
005eac7c: str r0, [sp, #0x14]
005eac80: ldrne r3, [r0, #4]
005eac84: addne r3, r3, #1
005eac88: strne r3, [r0, #4]
005eac8c: ldr r1, [r4, #0x34]
005eac90: ldr r3, [r4, #0x38]
005eac94: cmp r1, r3
005eac98: beq #0x5eaea8
005eac9c: ldr r3, [sp, #0x14]
005eaca0: cmp r3, #0
005eaca4: str r3, [r1]
005eaca8: ldrne r2, [r3, #4]
005eacac: addne r2, r2, #1
005eacb0: strne r2, [r3, #4]
005eacb4: ldr r3, [r4, #0x34]
005eacb8: add r3, r3, #4
005eacbc: str r3, [r4, #0x34]
005eacc0: ldr r0, [sp, #0x14]
005eacc4: cmp r0, #0
005eacc8: beq #0x5eacd0
005eaccc: bl #0x31d584
005eacd0: bl #0x604344
005eacd4: cmp r0, #0
005eacd8: str r0, [sp, #0x10]
005eacdc: ldrne r3, [r0, #4]
005eace0: addne r3, r3, #1
005eace4: strne r3, [r0, #4]
005eace8: ldr r1, [r4, #0x34]
005eacec: ldr r3, [r4, #0x38]
005eacf0: cmp r1, r3
005eacf4: beq #0x5eaeb8
005eacf8: ldr r3, [sp, #0x10]
005eacfc: cmp r3, #0
005ead00: str r3, [r1]
005ead04: ldrne r2, [r3, #4]
005ead08: addne r2, r2, #1
005ead0c: strne r2, [r3, #4]
005ead10: ldr r3, [r4, #0x34]
005ead14: add r3, r3, #4
005ead18: str r3, [r4, #0x34]
005ead1c: ldr r0, [sp, #0x10]
005ead20: cmp r0, #0
005ead24: beq #0x5ead2c
005ead28: bl #0x31d584
005ead2c: bl #0x6057c0
005ead30: cmp r0, #0
005ead34: str r0, [sp, #0xc]
005ead38: ldrne r3, [r0, #4]
005ead3c: addne r3, r3, #1
005ead40: strne r3, [r0, #4]
005ead44: ldr r1, [r4, #0x34]
005ead48: ldr r3, [r4, #0x38]
005ead4c: cmp r1, r3
005ead50: beq #0x5eae88
005ead54: ldr r3, [sp, #0xc]
005ead58: cmp r3, #0
005ead5c: str r3, [r1]
005ead60: ldrne r2, [r3, #4]
005ead64: addne r2, r2, #1
005ead68: strne r2, [r3, #4]
005ead6c: ldr r3, [r4, #0x34]
005ead70: add r3, r3, #4
005ead74: str r3, [r4, #0x34]
005ead78: ldr r0, [sp, #0xc]
005ead7c: cmp r0, #0
005ead80: beq #0x5ead88
005ead84: bl #0x31d584
005ead88: bl #0x606d10
005ead8c: ldr r1, [r4, #0x40]
005ead90: ldr r3, [r4, #0x44]
005ead94: add r5, r4, #0x3c
005ead98: str r0, [sp, #8]
005ead9c: cmp r1, r3
005eada0: beq #0x5eae10
005eada4: str r0, [r1]
005eada8: ldr r3, [r4, #0x40]
005eadac: add r3, r3, #4
005eadb0: str r3, [r4, #0x40]
005eadb4: bl #0x60762c
005eadb8: ldr r1, [r4, #0x40]
005eadbc: ldr r3, [r4, #0x44]
005eadc0: str r0, [sp, #4]
005eadc4: cmp r1, r3
005eadc8: beq #0x5eae34
005eadcc: str r0, [r1]
005eadd0: ldr r3, [r4, #0x40]
005eadd4: add r3, r3, #4
005eadd8: str r3, [r4, #0x40]
005eaddc: bl #0x6072c0
005eade0: ldr r1, [r4, #0x40]
005eade4: ldr r3, [r4, #0x44]
005eade8: str r0, [sp]
005eadec: cmp r1, r3
005eadf0: beq #0x5eae58
005eadf4: str r0, [r1]
005eadf8: ldr r3, [r4, #0x40]
005eadfc: add r3, r3, #4
005eae00: str r3, [r4, #0x40]
005eae04: mov r0, r4
005eae08: add sp, sp, #0x2c
005eae0c: pop {r4, r5, pc}
005eae10: mov r0, r5
005eae14: add r2, sp, #8
005eae18: bl #0x5ea990
005eae1c: bl #0x60762c
005eae20: ldr r1, [r4, #0x40]
005eae24: ldr r3, [r4, #0x44]
005eae28: str r0, [sp, #4]
005eae2c: cmp r1, r3
005eae30: bne #0x5eadcc
005eae34: mov r0, r5
005eae38: add r2, sp, #4
005eae3c: bl #0x5ea990
005eae40: bl #0x6072c0
005eae44: ldr r1, [r4, #0x40]
005eae48: ldr r3, [r4, #0x44]
005eae4c: str r0, [sp]
005eae50: cmp r1, r3
005eae54: bne #0x5eadf4
005eae58: mov r0, r5
005eae5c: mov r2, sp
005eae60: bl #0x5ea990
005eae64: b #0x5eae04
005eae68: mov r0, r5
005eae6c: add r2, sp, #0x1c
005eae70: bl #0x5e8fdc
005eae74: b #0x5eac08
005eae78: mov r0, r5
005eae7c: add r2, sp, #0x20
005eae80: bl #0x5e8fdc
005eae84: b #0x5eabac
005eae88: mov r0, r5
005eae8c: add r2, sp, #0xc
005eae90: bl #0x5e8fdc
005eae94: b #0x5ead78
005eae98: mov r0, r5
005eae9c: add r2, sp, #0x18
005eaea0: bl #0x5e8fdc
005eaea4: b #0x5eac64
005eaea8: mov r0, r5
005eaeac: add r2, sp, #0x14
005eaeb0: bl #0x5e8fdc
005eaeb4: b #0x5eacc0
005eaeb8: mov r0, r5
005eaebc: add r2, sp, #0x10
005eaec0: bl #0x5e8fdc
005eaec4: b #0x5ead1c
005eaec8: mov r0, r5
005eaecc: add r2, sp, #0x24
005eaed0: bl #0x5e8fdc
005eaed4: b #0x5eab50

_ZN6glitch5video11CNullDriver8CTexture19generateMipmapsImplEv
005b8dd4: bx lr

_ZN6glitch5video19CCommonGLDriverBase12CTextureBaseC2EPKcPS1_RKNS0_12STextureDescE
006dded8: push {r4, r5, r6, lr}
006ddedc: ldr r5, [pc, #0x2c]
006ddee0: mov r4, r0
006ddee4: bl #0x5fe6e0
006ddee8: ldr r3, [pc, #0x24]
006ddeec: add r5, pc, r5
006ddef0: mov r2, #0
006ddef4: ldr r3, [r5, r3]
006ddef8: strb r2, [r4, #0x58]
006ddefc: str r2, [r4, #0x54]
006ddf00: add r3, r3, #8
006ddf04: str r3, [r4]
006ddf08: mov r0, r4
006ddf0c: pop {r4, r5, r6, pc}
006ddf10: eoreq r6, fp, r4, lsr #23
006ddf14: andeq r3, r0, r0, lsl ip

_ZN6glitch5video19CCommonGLDriverBase12CTextureBaseC1EPKcPS1_RKNS0_12STextureDescE
006dde98: push {r4, r5, r6, lr}
006dde9c: ldr r5, [pc, #0x2c]
006ddea0: mov r4, r0
006ddea4: bl #0x5fe6e0
006ddea8: ldr r3, [pc, #0x24]
006ddeac: add r5, pc, r5
006ddeb0: mov r2, #0
006ddeb4: ldr r3, [r5, r3]
006ddeb8: strb r2, [r4, #0x58]
006ddebc: str r2, [r4, #0x54]
006ddec0: add r3, r3, #8
006ddec4: str r3, [r4]
006ddec8: mov r0, r4
006ddecc: pop {r4, r5, r6, pc}
006dded0: eoreq r6, fp, r4, ror #23
006dded4: andeq r3, r0, r0, lsl ip

_ZN6glitch5video11CNullDriver18createRenderTargetERKN5boost13intrusive_ptrINS0_8ITextureEEEj
005b90d8: mov r2, #0
005b90dc: str r2, [r0]
005b90e0: bx lr

_ZN6glitch5video12IVideoDriverC1EPNS_7IDeviceEPNS0_14IShaderManagerEPNS0_24CMaterialRendererManagerEPNS0_15CTextureManagerEPNS0_31CGlobalMaterialParameterManagerERKN5boost13intrusive_ptrINS0_6CLightEEE
005ab2f8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005ab2fc: ldr r7, [pc, #0x3b0]
005ab300: ldr ip, [pc, #0x3b0]
005ab304: ldr fp, [pc, #0x3b0]
005ab308: add r7, pc, r7
005ab30c: ldr ip, [r7, ip]
005ab310: ldr lr, [r7, fp]
005ab314: mov r4, r0
005ab318: mov r5, #1
005ab31c: add r0, ip, #8
005ab320: mov ip, r4
005ab324: ldr lr, [lr]
005ab328: str r5, [r4, #4]
005ab32c: str r0, [ip], #8
005ab330: sub sp, sp, #0x11c
005ab334: str ip, [r4, #0x18]
005ab338: str ip, [r4, #0x1c]
005ab33c: str r3, [sp]
005ab340: ldr r3, [sp, #0x140]
005ab344: mov r0, ip
005ab348: mov r8, r2
005ab34c: str lr, [sp, #0x114]
005ab350: mov r6, r1
005ab354: ldr sl, [sp, #0x148]
005ab358: str r3, [sp, #4]
005ab35c: ldr sb, [sp, #0x144]
005ab360: bl #0x5aaf18
005ab364: ldr r2, [r4, #0x18]
005ab368: mov r5, #0
005ab36c: add r3, r4, #0x20
005ab370: strb r5, [r2]
005ab374: mov r0, r3
005ab378: str r3, [r4, #0x30]
005ab37c: str r3, [r4, #0x34]
005ab380: bl #0x5aaf18
005ab384: ldr r3, [r4, #0x30]
005ab388: add r0, r4, #0x50
005ab38c: strb r5, [r3]
005ab390: mvn r3, #0
005ab394: strh r5, [r4, #0x3e]
005ab398: strh r5, [r4, #0x3a]
005ab39c: strh r5, [r4, #0x3c]
005ab3a0: strh r3, [r4, #0x38]
005ab3a4: ldr r3, [sl]
005ab3a8: mov sl, #0
005ab3ac: cmp r3, r5
005ab3b0: str r3, [r4, #0x40]
005ab3b4: ldrne r2, [r3]
005ab3b8: mov r5, #8
005ab3bc: addne r2, r2, #1
005ab3c0: strne r2, [r3]
005ab3c4: str sl, [r4, #0x44]
005ab3c8: str sl, [r4, #0x48]
005ab3cc: str r5, [r4, #0x4c]
005ab3d0: bl #0x6da980
005ab3d4: mov r3, #0x10
005ab3d8: str r3, [r4, #0x88]
005ab3dc: mov r3, #0x400
005ab3e0: str r3, [r4, #0x10c]
005ab3e4: str r6, [r4, #0xd4]
005ab3e8: ldr r3, [sp]
005ab3ec: mvn r2, #0
005ab3f0: movw r0, #0x136
005ab3f4: str r3, [r4, #0xdc]
005ab3f8: ldr r3, [sp, #4]
005ab3fc: add r1, r4, #0x14c
005ab400: strb r2, [r4, #0xf8]
005ab404: str r3, [r4, #0xe0]
005ab408: strb r2, [r4, #0xf9]
005ab40c: str r8, [r4, #0xd8]
005ab410: str sb, [r4, #0xe4]
005ab414: str sl, [r4, #0x78]
005ab418: str sl, [r4, #0x7c]
005ab41c: str sl, [r4, #0x80]
005ab420: str sl, [r4, #0x84]
005ab424: str sl, [r4, #0xa0]
005ab428: str sl, [r4, #0xa4]
005ab42c: str sl, [r4, #0xa8]
005ab430: str sl, [r4, #0xac]
005ab434: str sl, [r4, #0xb0]
005ab438: str sl, [r4, #0xb4]
005ab43c: str sl, [r4, #0xb8]
005ab440: str sl, [r4, #0xbc]
005ab444: str sl, [r4, #0xc0]
005ab448: str sl, [r4, #0xc4]
005ab44c: str sl, [r4, #0xc8]
005ab450: str sl, [r4, #0xcc]
005ab454: str sl, [r4, #0xd0]
005ab458: str sl, [r4, #0xe8]
005ab45c: str sl, [r4, #0xec]
005ab460: str sl, [r4, #0xf0]
005ab464: str sl, [r4, #0xf4]
005ab468: str sl, [r4, #0x104]
005ab46c: str sl, [r4, #0x108]
005ab470: str sl, [r4, #0x110]
005ab474: str sl, [r4, #0x114]
005ab478: strh r2, [r4, r0]
005ab47c: mov r2, #0x40
005ab480: str sl, [r4, #0x118]
005ab484: str sl, [r4, #0x120]
005ab488: str sl, [r4, #0x124]
005ab48c: str sl, [r4, #0x128]
005ab490: str sl, [r4, #0x12c]
005ab494: str sl, [r4, #0x130]
005ab498: strb sl, [r4, #0x135]
005ab49c: str sl, [r4, #0x138]
005ab4a0: str sl, [r4, #0x13c]
005ab4a4: str sl, [r4, #0x140]
005ab4a8: str sl, [r4, #0x144]
005ab4ac: str sl, [r4, #0x148]
005ab4b0: str sl, [r4, #0x14c]
005ab4b4: str sl, [r1, #4]
005ab4b8: str r2, [r4, #0x9c]
005ab4bc: str sl, [r4, #0x15c]
005ab4c0: str sl, [r4, #0x154]
005ab4c4: str sl, [r4, #0x158]
005ab4c8: mov r0, r8
005ab4cc: ldr r3, [r8]
005ab4d0: mov r1, r4
005ab4d4: mov r2, #1
005ab4d8: add r8, r4, #0xfa
005ab4dc: mov lr, pc
005ab4e0: ldr pc, [r3, #8]
005ab4e4: mov r1, #0xff
005ab4e8: mov r2, r5
005ab4ec: mov r0, r8
005ab4f0: bl #0x30e460
005ab4f4: ldr r1, [r4, #0x40]
005ab4f8: cmp r1, sl
005ab4fc: beq #0x5ab610
005ab500: ldr r1, [r4, #0xdc]
005ab504: cmp r1, #0
005ab508: beq #0x5ab660
005ab50c: ldr r1, [r4, #0xe0]
005ab510: cmp r1, #0
005ab514: beq #0x5ab688
005ab518: ldr r1, [r4, #0xe4]
005ab51c: cmp r1, #0
005ab520: beq #0x5ab5e8
005ab524: ldr r1, [pc, #0x194]
005ab528: ldr r2, [pc, #0x194]
005ab52c: add r6, sp, #0x14
005ab530: add r1, pc, r1
005ab534: add r2, pc, r2
005ab538: mov r0, r6
005ab53c: bl #0x30eae4
005ab540: ldr r0, [r4, #0xe4]
005ab544: mov r1, r6
005ab548: bl #0x5bb378
005ab54c: movw r3, #0xffff
005ab550: cmp r0, r3
005ab554: strh r0, [r4, #0x38]
005ab558: beq #0x5ab5b8
005ab55c: ldr sl, [pc, #0x164]
005ab560: ldr sb, [pc, #0x164]
005ab564: mov r5, #0
005ab568: add sl, pc, sl
005ab56c: add sb, pc, sb
005ab570: mov r3, r5
005ab574: mov r1, sl
005ab578: mov r2, sb
005ab57c: mov r0, r6
005ab580: bl #0x30eae4
005ab584: ldr r0, [r4, #0xe4]
005ab588: mov r1, r6
005ab58c: bl #0x5bb378
005ab590: add r5, r5, #1
005ab594: cmp r5, #4
005ab598: strh r0, [r8], #2
005ab59c: bne #0x5ab570
005ab5a0: ldr r1, [pc, #0x128]
005ab5a4: ldr r0, [r4, #0xe4]
005ab5a8: add r1, pc, r1
005ab5ac: bl #0x5bb378
005ab5b0: movw r3, #0x136
005ab5b4: strh r0, [r4, r3]
005ab5b8: ldr r2, [r4, #0xdc]
005ab5bc: ldr r3, [r7, fp]
005ab5c0: mov r1, #0
005ab5c4: str r1, [r4, #0x108]
005ab5c8: str r2, [r4, #0x104]
005ab5cc: ldr r2, [sp, #0x114]
005ab5d0: ldr r3, [r3]
005ab5d4: mov r0, r4
005ab5d8: cmp r2, r3
005ab5dc: bne #0x5ab6b0
005ab5e0: add sp, sp, #0x11c
005ab5e4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005ab5e8: mov r0, #0x3c
005ab5ec: bl #0x5341ac
005ab5f0: mov r1, r4
005ab5f4: mov r5, r0
005ab5f8: bl #0x5b9e50
005ab5fc: ldr r3, [r4, #0x138]
005ab600: str r5, [r4, #0xe4]
005ab604: orr r3, r3, #0x20
005ab608: str r3, [r4, #0x138]
005ab60c: b #0x5ab5b8
005ab610: add r5, sp, #0x10
005ab614: mov r0, r5
005ab618: bl #0x59feb8
005ab61c: ldr r3, [sp, #0x10]
005ab620: add r0, sp, #0x118
005ab624: str r3, [sp, #0xc]
005ab628: cmp r3, sl
005ab62c: ldrne r2, [r3]
005ab630: addne r2, r2, #1
005ab634: strne r2, [r3]
005ab638: ldrne r3, [sp, #0xc]
005ab63c: ldr r2, [r4, #0x40]
005ab640: str r3, [r4, #0x40]
005ab644: str r2, [r0, #-0x10c]!
005ab648: bl #0x5aa3c8
005ab64c: mov r0, r5
005ab650: bl #0x5aa3c8
005ab654: mov r0, r4
005ab658: bl #0x5a8ae8
005ab65c: b #0x5ab500
005ab660: mov r0, #0x98
005ab664: bl #0x5341ac
005ab668: mov r1, r4
005ab66c: mov r5, r0
005ab670: bl #0x5d8f24
005ab674: ldr r3, [r4, #0x138]
005ab678: str r5, [r4, #0xdc]
005ab67c: orr r3, r3, #0x10
005ab680: str r3, [r4, #0x138]
005ab684: b #0x5ab50c
005ab688: mov r0, #0x78
005ab68c: bl #0x5341ac
005ab690: mov r1, r4
005ab694: mov r5, r0
005ab698: bl #0x5eaa7c
005ab69c: ldr r3, [r4, #0x138]
005ab6a0: str r5, [r4, #0xe0]
005ab6a4: orr r3, r3, #0x20
005ab6a8: str r3, [r4, #0x138]
005ab6ac: b #0x5ab518
005ab6b0: bl #0x30e310
005ab6b4: eorseq sb, lr, r8, lsl #15
005ab6b8: andeq r2, r0, r4, lsl #31
005ab6bc: andeq r4, r0, ip, lsr #1
005ab6c0: eorseq r4, r3, r8, asr #20
005ab6c4: eorseq r4, r3, ip, asr #20
005ab6c8: eorseq r4, r3, r8, lsr #20
005ab6cc: eorseq r4, r3, r4, asr #20
005ab6d0: ldrshteq r4, [r3], -r0
