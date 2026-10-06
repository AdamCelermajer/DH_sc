_ZN6glitch5video16COpenGLES2DriverC2EPNS_7IDeviceE
005b5e00: push {r4, r5, r6, lr}
005b5e04: ldr r4, [pc, #0x20]
005b5e08: mov r5, r0
005b5e0c: bl #0x5b5c40
005b5e10: ldr r3, [pc, #0x18]
005b5e14: add r4, pc, r4
005b5e18: mov r0, r5
005b5e1c: ldr r3, [r4, r3]
005b5e20: add r3, r3, #8
005b5e24: str r3, [r5]
005b5e28: pop {r4, r5, r6, pc}
005b5e2c: eorseq lr, sp, ip, ror ip
005b5e30: andeq r2, r0, ip, ror #28

_ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEC2EPNS_7IDeviceE
005b5c40: push {r4, r5, r6, r7, r8, sb, sl, lr}
005b5c44: mov r8, r0
005b5c48: mov r5, r1
005b5c4c: mov r0, #0x84
005b5c50: mov r1, #0
005b5c54: bl #0x5341ac
005b5c58: mov r4, r0
005b5c5c: ldr r6, [pc, #0x108]
005b5c60: bl #0x6e04ec
005b5c64: mov r1, r5
005b5c68: mov r2, r4
005b5c6c: mov r0, r8
005b5c70: bl #0x6de5f8
005b5c74: ldr r2, [pc, #0xf4]
005b5c78: add r6, pc, r6
005b5c7c: mov r7, #0
005b5c80: ldr r2, [r6, r2]
005b5c84: mov r3, r8
005b5c88: add r4, r8, #0x820
005b5c8c: add r2, r2, #8
005b5c90: str r7, [r8, #0x7f8]
005b5c94: str r2, [r8]
005b5c98: str r7, [r8, #0x7fc]
005b5c9c: str r7, [r8, #0x804]
005b5ca0: strb r7, [r3, #0x800]!
005b5ca4: str r3, [r8, #0x80c]
005b5ca8: str r3, [r8, #0x808]
005b5cac: mov r0, r4
005b5cb0: str r7, [r8, #0x810]
005b5cb4: bl #0x6df9b8
005b5cb8: ldr r3, [pc, #0xb4]
005b5cbc: add sl, r4, #0xd4
005b5cc0: mov r5, #0x3f800000
005b5cc4: ldr r3, [r6, r3]
005b5cc8: str r7, [r8, #0x824]
005b5ccc: add r4, r4, #8
005b5cd0: add r3, r3, #8
005b5cd4: str r3, [r8]
005b5cd8: mov r6, #1
005b5cdc: strb r7, [r4, #0x40]
005b5ce0: mov r0, r4
005b5ce4: mov r1, #0
005b5ce8: mov r2, #0x40
005b5cec: bl #0x30e460
005b5cf0: str r5, [r4]
005b5cf4: str r5, [r4, #0x14]
005b5cf8: str r5, [r4, #0x28]
005b5cfc: str r5, [r4, #0x3c]
005b5d00: strb r6, [r4, #0x40]
005b5d04: add r4, r4, #0x44
005b5d08: cmp r4, sl
005b5d0c: bne #0x5b5cdc
005b5d10: add r4, r8, #0x8f0
005b5d14: add r4, r4, #4
005b5d18: add r6, r4, #0x4c0
005b5d1c: add r6, r6, #8
005b5d20: mov sl, #0
005b5d24: mov r7, #1
005b5d28: strb sl, [r4, #0x40]
005b5d2c: mov r0, r4
005b5d30: mov r1, #0
005b5d34: mov r2, #0x40
005b5d38: bl #0x30e460
005b5d3c: str r5, [r4]
005b5d40: str r5, [r4, #0x14]
005b5d44: str r5, [r4, #0x28]
005b5d48: str r5, [r4, #0x3c]
005b5d4c: strb r7, [r4, #0x40]
005b5d50: add r4, r4, #0x44
005b5d54: cmp r4, r6
005b5d58: bne #0x5b5d28
005b5d5c: mvn r3, #0
005b5d60: str r3, [r8, #0xdbc]
005b5d64: mov r0, r8
005b5d68: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b5d6c: eorseq lr, sp, r8, lsl lr
005b5d70: andeq r4, r0, r4, ror r5
005b5d74: andeq r1, r0, r8, lsl r4

_ZN6glitch5video6CImageC2ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvjjbb
00602938: push {r4, r5, r6, r7, r8, sb, sl, lr}
0060293c: ldr lr, [pc, #0x13c]
00602940: ldr r5, [pc, #0x13c]
00602944: mov ip, #0
00602948: add lr, pc, lr
0060294c: ldr r5, [lr, r5]
00602950: str ip, [r0, #4]
00602954: str ip, [r0, #8]
00602958: add r5, r5, #8
0060295c: str r5, [r0]
00602960: str ip, [r0, #0xc]
00602964: ldr r7, [r2]
00602968: sub sp, sp, #8
0060296c: ldrb r6, [sp, #0x30]
00602970: ldr r5, [sp, #0x28]
00602974: str r7, [r0, #0x10]
00602978: ldr r8, [r2, #4]
0060297c: ldrb r2, [sp, #0x34]
00602980: mov r7, r1
00602984: str r8, [r0, #0x14]
00602988: ldr r1, [sp, #0x2c]
0060298c: cmp r6, ip
00602990: mov r4, r0
00602994: str r1, [r0, #0x24]
00602998: strb r2, [r0, #0x29]
0060299c: mov r8, r3
006029a0: str r5, [r0, #0x1c]
006029a4: str r7, [r0, #0x20]
006029a8: strb ip, [r0, #0x28]
006029ac: beq #0x6029e0
006029b0: movw r3, #0xf00d
006029b4: movt r3, #0xbad
006029b8: str r3, [r0, #8]
006029bc: mov r1, ip
006029c0: bl #0x601988
006029c4: ldr r3, [r4, #0x24]
006029c8: str r8, [r4, #8]
006029cc: cmp r3, #0
006029d0: bne #0x602a04
006029d4: mov r0, r4
006029d8: add sp, sp, #8
006029dc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
006029e0: mov r1, r6
006029e4: bl #0x601988
006029e8: mov r1, r8
006029ec: mov r2, r5
006029f0: ldr r0, [r4, #8]
006029f4: bl #0x30e868
006029f8: ldr r3, [r4, #0x24]
006029fc: cmp r3, #0
00602a00: beq #0x6029d4
00602a04: add r3, r3, #1
00602a08: lsl r0, r3, #2
00602a0c: mov r1, #0
00602a10: bl #0x5341a8
00602a14: ldr r5, [r4, #0x24]
00602a18: str r0, [r4, #0xc]
00602a1c: ldr r6, [r4, #8]
00602a20: cmp r5, #0
00602a24: ldr r8, [r4, #0x10]
00602a28: ldr sl, [r4, #0x14]
00602a2c: beq #0x602a74
00602a30: mov r5, #0
00602a34: mov sb, r5
00602a38: uxtb r3, r5
00602a3c: mov r0, r7
00602a40: mov r1, r8
00602a44: mov r2, sl
00602a48: str sb, [sp]
00602a4c: bl #0x5edbcc
00602a50: ldr r3, [r4, #0xc]
00602a54: add r6, r6, r0
00602a58: str r6, [r3, r5, lsl #2]
00602a5c: ldr r3, [r4, #0x24]
00602a60: add r5, r5, #1
00602a64: cmp r3, r5
00602a68: bhi #0x602a38
00602a6c: ldr r0, [r4, #0xc]
00602a70: lsl r5, r5, #2
00602a74: mov r3, #0
00602a78: str r3, [r0, r5]
00602a7c: b #0x6029d4
00602a80: eorseq r2, sb, r8, asr #2
00602a84: andeq r0, r0, r0, lsl r6

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

_ZN6glitch5video18ICodeShaderManager20initAdditionalConfigEPKc
006e0a3c: push {r4, r5, r6, r7, r8, lr}
006e0a40: ldr r3, [r0, #0x80]
006e0a44: mov r4, r0
006e0a48: mov r7, r1
006e0a4c: cmn r3, #1
006e0a50: beq #0x6e0a58
006e0a54: pop {r4, r5, r6, r7, r8, pc}
006e0a58: ldr r3, [r0, #0x2c]
006e0a5c: ldr r3, [r3, #0xd4]
006e0a60: ldr r5, [r3, #0x34]
006e0a64: cmp r5, #0
006e0a68: ldrne r3, [r5, #4]
006e0a6c: mov r0, r5
006e0a70: addne r3, r3, #1
006e0a74: strne r3, [r5, #4]
006e0a78: ldr r3, [r5]
006e0a7c: mov lr, pc
006e0a80: ldr pc, [r3, #0xc]
006e0a84: subs r6, r0, #0
006e0a88: beq #0x6e0b2c
006e0a8c: ldr r3, [r6]
006e0a90: mov lr, pc
006e0a94: ldr pc, [r3, #0x20]
006e0a98: mov r1, #0
006e0a9c: str r0, [r4, #0x80]
006e0aa0: add r0, r0, #1
006e0aa4: bl #0x5341a8
006e0aa8: mov r1, r0
006e0aac: ldr r0, [r4, #0x7c]
006e0ab0: str r1, [r4, #0x7c]
006e0ab4: cmp r0, #0
006e0ab8: beq #0x6e0ac4
006e0abc: bl #0x30e0b8
006e0ac0: ldr r1, [r4, #0x7c]
006e0ac4: ldr r2, [r4, #0x80]
006e0ac8: ldr r3, [r6]
006e0acc: mov r0, r6
006e0ad0: mov lr, pc
006e0ad4: ldr pc, [r3, #0xc]
006e0ad8: mov r0, r6
006e0adc: bl #0x31d584
006e0ae0: ldr r3, [r4, #0x80]
006e0ae4: ldr r2, [r4, #0x7c]
006e0ae8: mov r1, #0
006e0aec: strb r1, [r2, r3]
006e0af0: ldr r3, [r4, #0x7c]
006e0af4: ldr r1, [r4, #0x80]
006e0af8: add r1, r3, r1
006e0afc: cmp r1, r3
006e0b00: beq #0x6e0b20
006e0b04: mov r0, #0xa
006e0b08: ldrsb r2, [r3]
006e0b0c: cmp r2, #0x5e
006e0b10: strbeq r0, [r3]
006e0b14: add r3, r3, #1
006e0b18: cmp r3, r1
006e0b1c: bne #0x6e0b08
006e0b20: mov r0, r5
006e0b24: pop {r4, r5, r6, r7, r8, lr}
006e0b28: b #0x31d584
006e0b2c: ldr r4, [pc, #0x28]
006e0b30: add r4, pc, r4
006e0b34: ldrb r3, [r4]
006e0b38: cmp r3, #0
006e0b3c: beq #0x6e0b20
006e0b40: ldr r1, [pc, #0x18]
006e0b44: mov r2, r7
006e0b48: mov r0, #2
006e0b4c: add r1, pc, r1
006e0b50: bl #0x60b034
006e0b54: strb r6, [r4]
006e0b58: b #0x6e0b20
006e0b5c: eoreq ip, fp, r0, ror #31

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

_ZN6glitch5video12IVideoDriver9setOptionEjb
005a90b0: cmp r2, #0
005a90b4: push {r4, lr}
005a90b8: bne #0x5a90e4
005a90bc: ldr r2, [r0, #0x88]
005a90c0: tst r1, #0x100
005a90c4: bic r1, r2, r1
005a90c8: str r1, [r0, #0x88]
005a90cc: bne #0x5a90d4
005a90d0: pop {r4, pc}
005a90d4: ldr r3, [r0]
005a90d8: mov lr, pc
005a90dc: ldr pc, [r3, #0x1fc]
005a90e0: pop {r4, pc}
005a90e4: ldr r2, [r0, #0x88]
005a90e8: orr r1, r2, r1
005a90ec: str r1, [r0, #0x88]
005a90f0: pop {r4, pc}

_ZN6glitch5video12IVideoDriver4initEtth
005aab60: ldr ip, [pc, #0x32c]
005aab64: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005aab68: ldr lr, [pc, #0x328]
005aab6c: add ip, pc, ip
005aab70: mov r7, r2
005aab74: ldr r2, [ip, lr]
005aab78: sub sp, sp, #0x6c
005aab7c: str ip, [sp, #0x10]
005aab80: ldr r2, [r2]
005aab84: str lr, [sp, #0x2c]
005aab88: cmp r7, #0
005aab8c: strb r3, [r0, #0x135]
005aab90: strh r1, [r0, #0x3e]
005aab94: str r2, [sp, #0x64]
005aab98: mov r8, r0
005aab9c: strh r7, [r0, #0x3c]
005aaba0: ldr r4, [r0, #0xe4]
005aaba4: addeq r5, sp, #0x44
005aaba8: bne #0x5aad94
005aabac: ldr r1, [pc, #0x2e8]
005aabb0: mov r2, #0x1c
005aabb4: mov r3, #0x11
005aabb8: add r1, pc, r1
005aabbc: mov sb, #1
005aabc0: mov fp, #0xff
005aabc4: mov r0, r4
005aabc8: stm sp, {sb, fp}
005aabcc: bl #0x5bc374
005aabd0: movw r3, #0x136
005aabd4: strh r0, [r8, r3]
005aabd8: mov r1, r0
005aabdc: mov r0, r4
005aabe0: bl #0x5b9d2c
005aabe4: ldr r3, [pc, #0x2b4]
005aabe8: ldr sl, [pc, #0x2b4]
005aabec: add lr, sp, #0x40
005aabf0: add r3, pc, r3
005aabf4: str r3, [sp, #0x20]
005aabf8: ldr r3, [pc, #0x2a8]
005aabfc: add r1, sp, #0x3c
005aac00: add r2, sp, #0x34
005aac04: add r3, pc, r3
005aac08: str r3, [sp, #0x24]
005aac0c: ldr r3, [pc, #0x298]
005aac10: add r8, r8, #0xfa
005aac14: add sl, pc, sl
005aac18: add r3, pc, r3
005aac1c: str r3, [sp, #0x28]
005aac20: mov r6, #0
005aac24: str lr, [sp, #0x1c]
005aac28: str r1, [sp, #0x18]
005aac2c: str r2, [sp, #0x14]
005aac30: mov r3, r6
005aac34: mov r1, sl
005aac38: ldr r2, [sp, #0x20]
005aac3c: mov r0, r5
005aac40: bl #0x30eae4
005aac44: mov r2, #0x1f
005aac48: mov r3, #0x10
005aac4c: mov r1, r5
005aac50: mov r0, r4
005aac54: stm sp, {sb, fp}
005aac58: bl #0x5bc374
005aac5c: mov r1, r0
005aac60: strh r0, [r8]
005aac64: mov r0, r4
005aac68: bl #0x5b9d2c
005aac6c: mov r7, #0
005aac70: mvn ip, #0x7f
005aac74: ldrh r1, [r8], #2
005aac78: mov r2, r7
005aac7c: ldr r3, [sp, #0x1c]
005aac80: strb ip, [sp, #0x42]
005aac84: mov r0, r4
005aac88: mvn ip, #0
005aac8c: strb ip, [sp, #0x40]
005aac90: strb ip, [sp, #0x43]
005aac94: strb r7, [sp, #0x41]
005aac98: bl #0x5c47fc
005aac9c: mov r3, r6
005aaca0: mov r1, sl
005aaca4: ldr r2, [sp, #0x24]
005aaca8: mov r0, r5
005aacac: bl #0x30eae4
005aacb0: mov r1, r5
005aacb4: mov r2, #0x1d
005aacb8: mov r3, #5
005aacbc: mov r0, r4
005aacc0: stm sp, {sb, fp}
005aacc4: bl #0x5bc374
005aacc8: mov ip, r0
005aaccc: mov lr, #0x3f800000
005aacd0: mov r2, r7
005aacd4: ldr r3, [sp, #0x18]
005aacd8: mov r1, ip
005aacdc: mov r0, r4
005aace0: str lr, [sp, #0x3c]
005aace4: str ip, [sp, #0xc]
005aace8: bl #0x5c4bb0
005aacec: ldr ip, [sp, #0xc]
005aacf0: mov r0, r4
005aacf4: mov r1, ip
005aacf8: bl #0x5b9d2c
005aacfc: mov r3, r6
005aad00: mov r1, sl
005aad04: ldr r2, [sp, #0x28]
005aad08: mov r0, r5
005aad0c: bl #0x30eae4
005aad10: mov r1, r5
005aad14: mov r2, #0x1e
005aad18: mov r3, #6
005aad1c: mov r0, r4
005aad20: stm sp, {sb, fp}
005aad24: bl #0x5bc374
005aad28: mov lr, #0
005aad2c: mov ip, r0
005aad30: mov r1, ip
005aad34: mov r2, r7
005aad38: ldr r3, [sp, #0x14]
005aad3c: str lr, [sp, #0x34]
005aad40: mov r0, r4
005aad44: mov lr, #0x3f800000
005aad48: str lr, [sp, #0x38]
005aad4c: str ip, [sp, #0xc]
005aad50: bl #0x5c4b04
005aad54: ldr ip, [sp, #0xc]
005aad58: mov r0, r4
005aad5c: add r6, r6, #1
005aad60: mov r1, ip
005aad64: bl #0x5b9d2c
005aad68: cmp r6, #4
005aad6c: bne #0x5aac30
005aad70: ldr r2, [sp, #0x10]
005aad74: ldr r1, [sp, #0x2c]
005aad78: ldr r3, [r2, r1]
005aad7c: ldr r2, [sp, #0x64]
005aad80: ldr r3, [r3]
005aad84: cmp r2, r3
005aad88: bne #0x5aae90
005aad8c: add sp, sp, #0x6c
005aad90: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005aad94: ldr sb, [pc, #0x114]
005aad98: ldr r1, [pc, #0x114]
005aad9c: add r5, sp, #0x44
005aada0: add sb, pc, sb
005aada4: add r1, pc, r1
005aada8: mov r2, sb
005aadac: mov r0, r5
005aadb0: bl #0x30eae4
005aadb4: mov r2, #0x12
005aadb8: mov r3, r2
005aadbc: mov r6, #0
005aadc0: mov r1, r5
005aadc4: mov fp, #1
005aadc8: mov r0, r4
005aadcc: str fp, [sp]
005aadd0: str r6, [sp, #4]
005aadd4: bl #0x5bc374
005aadd8: add sl, r8, #0x40
005aaddc: mov r1, r0
005aade0: strh r0, [r8, #0x38]
005aade4: mov r0, r4
005aade8: bl #0x5b9d2c
005aadec: mov r2, r6
005aadf0: mov r0, r4
005aadf4: ldrh r1, [r8, #0x38]
005aadf8: mov r3, sl
005aadfc: bl #0x5be68c
005aae00: cmp r7, fp
005aae04: bls #0x5aabac
005aae08: ldr r3, [pc, #0xa8]
005aae0c: str r8, [sp, #0x14]
005aae10: mov r6, fp
005aae14: add r3, pc, r3
005aae18: mov r8, r3
005aae1c: mov r3, r6
005aae20: mov r1, r8
005aae24: mov r2, sb
005aae28: mov r0, r5
005aae2c: bl #0x30eae4
005aae30: mov r2, #0x12
005aae34: uxtb ip, r6
005aae38: mov r3, r2
005aae3c: mov r1, r5
005aae40: str ip, [sp, #4]
005aae44: mov r0, r4
005aae48: mov ip, #1
005aae4c: str ip, [sp]
005aae50: bl #0x5bc374
005aae54: mov fp, r0
005aae58: mov r1, fp
005aae5c: mov r0, r4
005aae60: bl #0x5b9d2c
005aae64: add r6, r6, #1
005aae68: mov r3, sl
005aae6c: mov r0, r4
005aae70: mov r1, fp
005aae74: mov r2, #0
005aae78: bl #0x5be68c
005aae7c: uxth r3, r6
005aae80: cmp r7, r3
005aae84: bhi #0x5aae1c
005aae88: ldr r8, [sp, #0x14]
005aae8c: b #0x5aabac
005aae90: bl #0x30e310
005aae94: eorseq sb, lr, r4, lsr #30
005aae98: andeq r4, r0, ip, lsr #1
005aae9c: eorseq r5, r3, r0, ror #7
005aaea0: eorseq r5, r3, r0, asr #7
005aaea4: eorseq r5, r3, ip, ror r3
005aaea8: ldrhteq r5, [r3], -ip
005aaeac: ldrhteq r5, [r3], -r8
005aaeb0: eorseq r5, r3, r0, ror #3
005aaeb4: ldrsbteq r5, [r3], -r4
005aaeb8: eorseq r5, r3, ip, ror r1

_ZN6glitch5video6CImageC2ERKN5boost13intrusive_ptrIS1_EERKNS_4core10position2dIiEERKNS7_11dimension2dIiEE
00601c24: push {r4, r5, r6, r7, r8, sl, lr}
00601c28: ldr r6, [pc, #0xfc]
00601c2c: ldr r7, [pc, #0xfc]
00601c30: mov r5, #0
00601c34: add r6, pc, r6
00601c38: ldr r7, [r6, r7]
00601c3c: mov ip, #1
00601c40: str r5, [r0, #4]
00601c44: add r7, r7, #8
00601c48: str r7, [r0]
00601c4c: mov r7, #0x27
00601c50: str r7, [r0, #0x20]
00601c54: str r5, [r0, #8]
00601c58: str r5, [r0, #0xc]
00601c5c: str r5, [r0, #0x10]
00601c60: str r5, [r0, #0x14]
00601c64: str r5, [r0, #0x18]
00601c68: str r5, [r0, #0x1c]
00601c6c: str r5, [r0, #0x24]
00601c70: strb r5, [r0, #0x28]
00601c74: strb ip, [r0, #0x29]
00601c78: mov r7, r1
00601c7c: ldr r1, [r1]
00601c80: sub sp, sp, #0x1c
00601c84: mov r4, r0
00601c88: cmp r1, r5
00601c8c: mov r8, r2
00601c90: mov sl, r3
00601c94: beq #0x601d20
00601c98: ldr r3, [r1, #0x20]
00601c9c: mov r1, ip
00601ca0: str r3, [r0, #0x20]
00601ca4: ldr r3, [sl]
00601ca8: str r3, [r0, #0x10]
00601cac: ldr r3, [sl, #4]
00601cb0: str r3, [r0, #0x14]
00601cb4: ldr r3, [r7]
00601cb8: ldrb r3, [r3, #0x28]
00601cbc: strb r3, [r0, #0x28]
00601cc0: bl #0x601988
00601cc4: ldr r2, [pc, #0x68]
00601cc8: ldr r3, [r7]
00601ccc: ldr r0, [r4, #0x20]
00601cd0: ldr r1, [r6, r2]
00601cd4: mov ip, #0x28
00601cd8: ldr r2, [r3, #0x18]
00601cdc: mla r1, ip, r0, r1
00601ce0: ldr r3, [r3, #8]
00601ce4: ldr ip, [r8, #4]
00601ce8: ldrb r1, [r1, #0x15]
00601cec: ldr r8, [r8]
00601cf0: mla r3, ip, r2, r3
00601cf4: ldr r6, [r4, #8]
00601cf8: ldr ip, [sl, #4]
00601cfc: ldr lr, [r4, #0x18]
00601d00: ldr r7, [sl]
00601d04: mla r1, r8, r1, r3
00601d08: mov r3, r0
00601d0c: stm sp, {r6, lr}
00601d10: str r7, [sp, #8]
00601d14: str ip, [sp, #0xc]
00601d18: str r5, [sp, #0x10]
00601d1c: bl #0x5f95ac
00601d20: mov r0, r4
00601d24: add sp, sp, #0x1c
00601d28: pop {r4, r5, r6, r7, r8, sl, pc}
00601d2c: eorseq r2, sb, ip, asr lr
00601d30: andeq r0, r0, r0, lsl r6
00601d34: andeq r1, r0, r4, lsr pc

_ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEE
00602110: ldr ip, [pc, #0x64]
00602114: push {r4, r5, r6, lr}
00602118: ldr lr, [pc, #0x60]
0060211c: add ip, pc, ip
00602120: mov r3, #0
00602124: ldr lr, [ip, lr]
00602128: str r3, [r0, #4]
0060212c: str r3, [r0, #8]
00602130: add lr, lr, #8
00602134: str lr, [r0]
00602138: str r3, [r0, #0xc]
0060213c: ldr r5, [r2]
00602140: mov lr, #1
00602144: mov r4, r0
00602148: str r5, [r0, #0x10]
0060214c: ldr r2, [r2, #4]
00602150: str r1, [r0, #0x20]
00602154: strb r3, [r0, #0x28]
00602158: str r2, [r0, #0x14]
0060215c: str r3, [r0, #0x18]
00602160: str r3, [r0, #0x1c]
00602164: str r3, [r0, #0x24]
00602168: strb lr, [r0, #0x29]
0060216c: mov r1, lr
00602170: bl #0x601988
00602174: mov r0, r4
00602178: pop {r4, r5, r6, pc}
0060217c: eorseq r2, sb, r4, ror sb
00602180: andeq r0, r0, r0, lsl r6

_ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEb
00602028: push {r4, r5, r6, lr}
0060202c: ldr lr, [pc, #0x60]
00602030: ldr r5, [pc, #0x60]
00602034: mov ip, #0
00602038: add lr, pc, lr
0060203c: ldr r5, [lr, r5]
00602040: str ip, [r0, #4]
00602044: str ip, [r0, #8]
00602048: add r5, r5, #8
0060204c: str r5, [r0]
00602050: str ip, [r0, #0xc]
00602054: ldr r6, [r2]
00602058: mov r5, #1
0060205c: mov r4, r0
00602060: str r6, [r0, #0x10]
00602064: ldr r2, [r2, #4]
00602068: str r1, [r0, #0x20]
0060206c: str ip, [r0, #0x24]
00602070: str r2, [r0, #0x14]
00602074: strb r3, [r0, #0x28]
00602078: str ip, [r0, #0x18]
0060207c: str ip, [r0, #0x1c]
00602080: strb r5, [r0, #0x29]
00602084: mov r1, r5
00602088: bl #0x601988
0060208c: mov r0, r4
00602090: pop {r4, r5, r6, pc}
00602094: eorseq r2, sb, r8, asr sl
00602098: andeq r0, r0, r0, lsl r6

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

_ZNK6glitch5video9CMaterial24updateParametersHashCodeEh
005c5d88: push {r4, r5, r6, r7, r8, sb, sl, fp}
005c5d8c: sub sp, sp, #0x10
005c5d90: str r0, [sp]
005c5d94: ldr r3, [r0, #4]
005c5d98: str r1, [sp, #4]
005c5d9c: add fp, r0, #0x20
005c5da0: ldr r2, [r3, #0x18]
005c5da4: ldr r0, [sp, #4]
005c5da8: mov r1, #0xc
005c5dac: ldr sb, [pc, #0x1e4]
005c5db0: mla r2, r1, r0, r2
005c5db4: add sb, pc, sb
005c5db8: ldr r1, [r2, #8]
005c5dbc: ldr r2, [r1, #0x20]
005c5dc0: ldr r6, [r1, #0x24]
005c5dc4: ldrh r8, [r2, #0x36]
005c5dc8: ldrh r0, [r2, #0x2e]
005c5dcc: ldrh r1, [r2, #0x2c]
005c5dd0: ldrh r2, [r2, #0x34]
005c5dd4: add r8, r8, r0
005c5dd8: uxth r8, r8
005c5ddc: rsb r8, r1, r8
005c5de0: rsb r8, r2, r8
005c5de4: uxth r8, r8
005c5de8: add r8, r6, r8, lsl #1
005c5dec: cmp r8, r6
005c5df0: moveq r1, #0
005c5df4: moveq r5, r1
005c5df8: beq #0x5c5ea0
005c5dfc: ldr r1, [pc, #0x198]
005c5e00: ldr r2, [pc, #0x198]
005c5e04: mov ip, #0xd
005c5e08: str r1, [sp, #8]
005c5e0c: mov r1, #0
005c5e10: str r2, [sp, #0xc]
005c5e14: mov r5, r1
005c5e18: ldrh r2, [r6]
005c5e1c: tst r2, #0x8000
005c5e20: bne #0x5c5e94
005c5e24: ldrh r0, [r3, #0xe]
005c5e28: cmp r0, r2
005c5e2c: ldrhi r0, [r3, #0x20]
005c5e30: movls r2, #0
005c5e34: addhi r2, r0, r2, lsl #4
005c5e38: ldrh r0, [r2, #4]
005c5e3c: ldr sl, [r2, #8]
005c5e40: cmp r0, #2
005c5e44: beq #0x5c5edc
005c5e48: cmp r0, #0xb
005c5e4c: beq #0x5c5e94
005c5e50: cmp r0, #0xf
005c5e54: beq #0x5c5e94
005c5e58: ldrb r0, [r2, #6]
005c5e5c: cmp r0, #0xb
005c5e60: beq #0x5c5f10
005c5e64: ldr r7, [sp, #8]
005c5e68: ldr r2, [r2, #0xc]
005c5e6c: ldr r4, [sb, r7]
005c5e70: add r2, fp, r2
005c5e74: ldrb r0, [r4, r0]
005c5e78: mla r0, sl, r0, r2
005c5e7c: cmp r2, r0
005c5e80: beq #0x5c5e94
005c5e84: ldrb r4, [r2], #1
005c5e88: cmp r2, r0
005c5e8c: mla r1, ip, r1, r4
005c5e90: bne #0x5c5e84
005c5e94: add r6, r6, #2
005c5e98: cmp r8, r6
005c5e9c: bne #0x5c5e18
005c5ea0: ldm sp, {r0, r7}
005c5ea4: lsl r5, r5, #0x14
005c5ea8: ldr r2, [r0, #0x18]
005c5eac: and r1, r1, #0xff
005c5eb0: lsr r5, r5, #0x14
005c5eb4: ldr r3, [r2, r7, lsl #2]
005c5eb8: bic r3, r3, #0xff0000
005c5ebc: bic r3, r3, #0xf000
005c5ec0: bic r3, r3, #0xff
005c5ec4: orr r3, r1, r3
005c5ec8: orr r5, r3, r5, lsl #12
005c5ecc: str r5, [r2, r7, lsl #2]
005c5ed0: add sp, sp, #0x10
005c5ed4: pop {r4, r5, r6, r7, r8, sb, sl, fp}
005c5ed8: bx lr
005c5edc: ldr r2, [r2, #0xc]
005c5ee0: add r2, fp, r2
005c5ee4: add sl, r2, sl, lsl #2
005c5ee8: cmp r2, sl
005c5eec: beq #0x5c5e94
005c5ef0: ldrb r0, [r2], #1
005c5ef4: cmp r2, sl
005c5ef8: mla r5, ip, r5, r0
005c5efc: bne #0x5c5ef0
005c5f00: add r6, r6, #2
005c5f04: cmp r8, r6
005c5f08: bne #0x5c5e18
005c5f0c: b #0x5c5ea0
005c5f10: ldr r7, [r2, #0xc]
005c5f14: add r7, fp, r7
005c5f18: add sl, r7, sl, lsl #2
005c5f1c: cmp r7, sl
005c5f20: beq #0x5c5e94
005c5f24: mov r4, r3
005c5f28: ldr r0, [r7]
005c5f2c: cmp r0, #0
005c5f30: beq #0x5c5f6c
005c5f34: mov r3, #0
005c5f38: ldrb r2, [r0, r3]
005c5f3c: add r3, r3, #1
005c5f40: cmp r3, #0x44
005c5f44: mla r1, ip, r1, r2
005c5f48: bne #0x5c5f38
005c5f4c: add r7, r7, #4
005c5f50: cmp sl, r7
005c5f54: bne #0x5c5f28
005c5f58: add r6, r6, #2
005c5f5c: cmp r8, r6
005c5f60: mov r3, r4
005c5f64: bne #0x5c5e18
005c5f68: b #0x5c5ea0
005c5f6c: ldr r2, [sp, #0xc]
005c5f70: ldr r3, [sb, r2]
005c5f74: ldrb r2, [r0, r3]
005c5f78: add r0, r0, #1
005c5f7c: cmp r0, #0x44
005c5f80: mla r1, ip, r1, r2
005c5f84: bne #0x5c5f74
005c5f88: add r7, r7, #4
005c5f8c: cmp sl, r7
005c5f90: bne #0x5c5f28
005c5f94: b #0x5c5f58
005c5f98: ldrsbteq lr, [ip], -ip
005c5f9c: andeq r1, r0, r0, asr #11
005c5fa0: andeq r2, r0, r0, lsr r8

_ZNK6glitch5video8ITexture12setDataDirtyEb
005fdae8: ldrb r3, [r0, #0x3f]
005fdaec: push {r4, r5, r6, r7}
005fdaf0: tst r3, #2
005fdaf4: beq #0x5fdb70
005fdaf8: ldr r3, [r0, #0x2c]
005fdafc: cmp r3, #0
005fdb00: beq #0x5fdbd8
005fdb04: ldr r7, [r0, #0x38]
005fdb08: ldrh r3, [r0, #0x40]
005fdb0c: ldrb r1, [r0, #0x3e]
005fdb10: and r7, r7, #3
005fdb14: orr r3, r3, #1
005fdb18: cmp r7, #2
005fdb1c: mov r2, #0
005fdb20: strh r3, [r0, #0x40]
005fdb24: moveq r7, #6
005fdb28: movne r7, #1
005fdb2c: mov r3, r2
005fdb30: mov r6, #1
005fdb34: ldr r4, [r0, #0x30]
005fdb38: add r1, r1, #1
005fdb3c: lsr ip, r3, #5
005fdb40: add r1, r4, r1, lsl #2
005fdb44: ldr r4, [r1, ip, lsl #2]
005fdb48: and r5, r3, #0x1f
005fdb4c: add r2, r2, #1
005fdb50: orr r4, r4, r6, lsl r5
005fdb54: str r4, [r1, ip, lsl #2]
005fdb58: ldrb r1, [r0, #0x3e]
005fdb5c: cmp r2, r7
005fdb60: add r3, r3, r1
005fdb64: blt #0x5fdb34
005fdb68: pop {r4, r5, r6, r7}
005fdb6c: bx lr
005fdb70: ldr r3, [r0, #0x2c]
005fdb74: cmp r3, #0
005fdb78: beq #0x5fdbe4
005fdb7c: ldr r1, [r0, #0x38]
005fdb80: ldrb r2, [r0, #0x3e]
005fdb84: ldr r3, [r0, #0x30]
005fdb88: and r1, r1, #3
005fdb8c: cmp r1, #2
005fdb90: moveq r1, #6
005fdb94: movne r1, #1
005fdb98: mul r1, r2, r1
005fdb9c: ldrh ip, [r0, #0x40]
005fdba0: add r1, r1, #0x1f
005fdba4: add r2, r2, #1
005fdba8: lsr r1, r1, #5
005fdbac: add r3, r3, r2, lsl #2
005fdbb0: add r1, r3, r1, lsl #2
005fdbb4: orr r2, ip, #1
005fdbb8: cmp r1, r3
005fdbbc: strh r2, [r0, #0x40]
005fdbc0: beq #0x5fdb68
005fdbc4: mvn r2, #0
005fdbc8: str r2, [r3], #4
005fdbcc: cmp r1, r3
005fdbd0: bne #0x5fdbc8
005fdbd4: b #0x5fdb68
005fdbd8: cmp r1, #0
005fdbdc: beq #0x5fdb68
005fdbe0: b #0x5fdb04
005fdbe4: cmp r1, #0
005fdbe8: beq #0x5fdb68
005fdbec: b #0x5fdb7c

_ZN6glitch5video6CImageC2ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEb
0060209c: push {r4, r5, r6, lr}
006020a0: ldr lr, [pc, #0x60]
006020a4: ldr r5, [pc, #0x60]
006020a8: mov ip, #0
006020ac: add lr, pc, lr
006020b0: ldr r5, [lr, r5]
006020b4: str ip, [r0, #4]
006020b8: str ip, [r0, #8]
006020bc: add r5, r5, #8
006020c0: str r5, [r0]
006020c4: str ip, [r0, #0xc]
006020c8: ldr r6, [r2]
006020cc: mov r5, #1
006020d0: mov r4, r0
006020d4: str r6, [r0, #0x10]
006020d8: ldr r2, [r2, #4]
006020dc: str r1, [r0, #0x20]
006020e0: str ip, [r0, #0x24]
006020e4: str r2, [r0, #0x14]
006020e8: strb r3, [r0, #0x28]
006020ec: str ip, [r0, #0x18]
006020f0: str ip, [r0, #0x1c]
006020f4: strb r5, [r0, #0x29]
006020f8: mov r1, r5
006020fc: bl #0x601988
00602100: mov r0, r4
00602104: pop {r4, r5, r6, pc}
00602108: eorseq r2, sb, r4, ror #19
0060210c: andeq r0, r0, r0, lsl r6

_ZNK6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture10updateDataEb
005afff0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005afff4: ldr r2, [pc, #0x440]
005afff8: ldrb r3, [r0, #0x3f]
005afffc: sub sp, sp, #0x4c
005b0000: add r2, pc, r2
005b0004: tst r3, #2
005b0008: str r2, [sp, #0x30]
005b000c: ldrbeq ip, [r0, #0x3e]
005b0010: movne r3, #1
005b0014: ldrbne sb, [r0, #0x3e]
005b0018: streq ip, [sp, #0x40]
005b001c: strne r3, [sp, #0x40]
005b0020: ldr r2, [r0, #0x2c]
005b0024: ldr r3, [r0, #0x30]
005b0028: ldr r4, [r0, #0x38]
005b002c: movne r8, sb
005b0030: moveq r8, ip
005b0034: moveq sb, #1
005b0038: add r8, r8, #1
005b003c: cmp r2, #0
005b0040: mov r5, r0
005b0044: mov fp, r1
005b0048: add r8, r3, r8, lsl #2
005b004c: ubfx r4, r4, #4, #6
005b0050: ldr sl, [r0, #0x34]
005b0054: beq #0x5b0094
005b0058: mov r0, r4
005b005c: ldr r1, [r5, #0x20]
005b0060: bl #0x5edaec
005b0064: ldr r6, [r5, #0x34]
005b0068: tst r0, #1
005b006c: and r0, r0, #3
005b0070: ldr r3, [r6, #0x26c]
005b0074: movne r7, #1
005b0078: rsbeq r7, r0, #4
005b007c: cmp r7, r3
005b0080: beq #0x5b0094
005b0084: movw r0, #0xcf5
005b0088: mov r1, r7
005b008c: bl #0x30e16c
005b0090: str r7, [r6, #0x26c]
005b0094: bl #0x30e1a8
005b0098: mov r3, #0x14
005b009c: mla r3, r3, r4, sl
005b00a0: mov r1, #0
005b00a4: str r3, [sp, #0x34]
005b00a8: ldr r2, [r5, #0x38]
005b00ac: ldr r3, [pc, #0x38c]
005b00b0: ldr ip, [sp, #0x34]
005b00b4: and r2, r2, #3
005b00b8: cmp r2, #2
005b00bc: add r3, pc, r3
005b00c0: add ip, ip, #0x4b0
005b00c4: moveq r2, #6
005b00c8: movne r2, #1
005b00cc: add ip, ip, #4
005b00d0: add r3, r3, #0xa4
005b00d4: str r1, [sp, #0x20]
005b00d8: str r2, [sp, #0x44]
005b00dc: str ip, [sp, #0x3c]
005b00e0: str r3, [sp, #0x38]
005b00e4: str r1, [sp, #0x24]
005b00e8: mov r4, r1
005b00ec: ldr r2, [sp, #0x40]
005b00f0: cmp r2, #0
005b00f4: beq #0x5b025c
005b00f8: ldr r3, [sp, #0x34]
005b00fc: sub sl, r2, #1
005b0100: ldr ip, [pc, #0x33c]
005b0104: uxtb sl, sl
005b0108: add r3, r3, #0x4b0
005b010c: add sl, sl, #1
005b0110: mov r6, #0
005b0114: add r3, r3, #8
005b0118: lsl sl, sl, #2
005b011c: str r3, [sp, #0x2c]
005b0120: mov r7, r6
005b0124: str ip, [sp, #0x28]
005b0128: ldr r3, [r8]
005b012c: mov r2, #1
005b0130: ands r3, r3, r2, lsl r4
005b0134: beq #0x5b0238
005b0138: ldr lr, [r5, #0x2c]
005b013c: cmp lr, #0
005b0140: beq #0x5b0168
005b0144: ldrb r3, [r5, #0x3f]
005b0148: tst r3, #2
005b014c: beq #0x5b027c
005b0150: ldr r3, [r5, #0x30]
005b0154: ldr r1, [sp, #0x20]
005b0158: ldm r3, {r2, r3}
005b015c: rsb r2, r2, r3
005b0160: mul r2, r2, r1
005b0164: add lr, lr, r2
005b0168: ldr r3, [r5, #0x20]
005b016c: ldr r1, [r5, #0x24]
005b0170: ldr r2, [r5, #0x38]
005b0174: asr r3, r3, r7
005b0178: asr r1, r1, r7
005b017c: and r0, r2, #3
005b0180: cmp r3, #1
005b0184: movlt r3, #1
005b0188: cmp r1, #1
005b018c: movlt r1, #1
005b0190: cmp r0, #1
005b0194: beq #0x5b0224
005b0198: cmp r0, #2
005b019c: ldreq ip, [sp, #0x24]
005b01a0: ldrne ip, [sp, #0x38]
005b01a4: ubfx r2, r2, #4, #6
005b01a8: addeq r0, ip, #0x8500
005b01ac: ldrne r0, [ip, r0, lsl #2]
005b01b0: mov ip, #0x28
005b01b4: mul ip, ip, r2
005b01b8: ldr r2, [sp, #0x28]
005b01bc: str ip, [sp, #0x18]
005b01c0: ldr ip, [sp, #0x30]
005b01c4: addeq r0, r0, #0x15
005b01c8: ldr r2, [ip, r2]
005b01cc: ldr ip, [sp, #0x18]
005b01d0: ldr ip, [r2, ip]
005b01d4: str ip, [sp, #0x1c]
005b01d8: ands ip, ip, #8
005b01dc: beq #0x5b02a0
005b01e0: cmp fp, #0
005b01e4: beq #0x5b02e4
005b01e8: ldr r2, [sp, #0x34]
005b01ec: ldr ip, [r5, #0x30]
005b01f0: ldr r2, [r2, #0x4b0]
005b01f4: str r1, [sp]
005b01f8: mov r1, #0
005b01fc: str r1, [sp, #4]
005b0200: str r2, [sp, #0x1c]
005b0204: add r1, ip, r6
005b0208: ldr r1, [r1, #4]
005b020c: ldr ip, [ip, r6]
005b0210: str lr, [sp, #0xc]
005b0214: rsb ip, ip, r1
005b0218: mov r1, r7
005b021c: str ip, [sp, #8]
005b0220: bl #0x30ed3c
005b0224: bl #0x30e1a8
005b0228: cmp r0, #0
005b022c: ldrbne r3, [r5, #0x3f]
005b0230: orrne r3, r3, #0x10
005b0234: strbne r3, [r5, #0x3f]
005b0238: add r4, r4, sb
005b023c: cmp r4, #0x1f
005b0240: movhi r3, #0
005b0244: add r6, r6, #4
005b0248: strhi r3, [r8], #4
005b024c: subhi r4, r4, #0x20
005b0250: cmp r6, sl
005b0254: add r7, r7, #1
005b0258: bne #0x5b0128
005b025c: ldr ip, [sp, #0x24]
005b0260: ldr r1, [sp, #0x44]
005b0264: add ip, ip, #1
005b0268: cmp ip, r1
005b026c: str ip, [sp, #0x24]
005b0270: bge #0x5b0360
005b0274: str ip, [sp, #0x20]
005b0278: b #0x5b00ec
005b027c: ldr r3, [r5, #0x30]
005b0280: ldrb r1, [r5, #0x3e]
005b0284: ldr ip, [sp, #0x20]
005b0288: ldr r2, [r3, r6]
005b028c: ldr r3, [r3, r1, lsl #2]
005b0290: add r3, r3, #0x7f
005b0294: bic r3, r3, #0x7f
005b0298: mla r2, r3, ip, r2
005b029c: b #0x5b0164
005b02a0: cmp fp, #0
005b02a4: beq #0x5b0328
005b02a8: ldr r2, [sp, #0x34]
005b02ac: ldr r2, [r2, #0x4b0]
005b02b0: str ip, [sp, #4]
005b02b4: ldr ip, [sp, #0x3c]
005b02b8: str r2, [sp, #0x1c]
005b02bc: str r1, [sp]
005b02c0: ldr r1, [ip]
005b02c4: str r1, [sp, #8]
005b02c8: ldr r1, [sp, #0x2c]
005b02cc: ldr ip, [r1]
005b02d0: mov r1, r7
005b02d4: str lr, [sp, #0x10]
005b02d8: str ip, [sp, #0xc]
005b02dc: bl #0x30e010
005b02e0: b #0x5b0224
005b02e4: ldr r2, [sp, #0x34]
005b02e8: str r1, [sp, #4]
005b02ec: str r3, [sp]
005b02f0: ldr r3, [r2, #0x4b0]
005b02f4: ldr r2, [r5, #0x30]
005b02f8: mov r1, r7
005b02fc: str r3, [sp, #8]
005b0300: add r3, r2, r6
005b0304: ldr ip, [r2, r6]
005b0308: ldr r3, [r3, #4]
005b030c: mov r2, fp
005b0310: str lr, [sp, #0x10]
005b0314: rsb ip, ip, r3
005b0318: mov r3, fp
005b031c: str ip, [sp, #0xc]
005b0320: bl #0x30dda0
005b0324: b #0x5b0224
005b0328: ldr r2, [sp, #0x3c]
005b032c: str r1, [sp, #4]
005b0330: str r3, [sp]
005b0334: ldr r3, [r2]
005b0338: mov r1, r7
005b033c: mov r2, fp
005b0340: str r3, [sp, #8]
005b0344: ldr r3, [sp, #0x2c]
005b0348: ldr ip, [r3]
005b034c: mov r3, fp
005b0350: str lr, [sp, #0x10]
005b0354: str ip, [sp, #0xc]
005b0358: bl #0x30eb50
005b035c: b #0x5b0224
005b0360: cmp r4, #0
005b0364: movne r3, #0
005b0368: strne r3, [r8]
005b036c: ldrh r2, [r5, #0x40]
005b0370: ldrb r3, [r5, #0x3f]
005b0374: bic r2, r2, #3
005b0378: tst r3, #0x10
005b037c: strh r2, [r5, #0x40]
005b0380: bne #0x5b03e0
005b0384: ldrb r2, [r5, #0x3e]
005b0388: cmp r2, #1
005b038c: bls #0x5b03e0
005b0390: tst r3, #2
005b0394: beq #0x5b03e0
005b0398: ldr r3, [r5, #0x2c]
005b039c: cmp r3, #0
005b03a0: beq #0x5b0410
005b03a4: ldr r1, [sp, #0x30]
005b03a8: ldr r3, [r5, #0x38]
005b03ac: ldr r2, [pc, #0x90]
005b03b0: ubfx r3, r3, #4, #6
005b03b4: ldr r2, [r1, r2]
005b03b8: mov r1, #0x28
005b03bc: mul r3, r1, r3
005b03c0: ldr r3, [r2, r3]
005b03c4: tst r3, #8
005b03c8: beq #0x5b03ec
005b03cc: ldr r1, [pc, #0x74]
005b03d0: ldr r2, [r5, #0x1c]
005b03d4: mov r0, #2
005b03d8: add r1, pc, r1
005b03dc: bl #0x60b034
005b03e0: mov r0, #1
005b03e4: add sp, sp, #0x4c
005b03e8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005b03ec: ldr r3, [r5, #0x34]
005b03f0: ldr r3, [r3, #0x9c]
005b03f4: tst r3, #4
005b03f8: beq #0x5b03e0
005b03fc: mov r0, r5
005b0400: ldr r3, [r5]
005b0404: mov lr, pc
005b0408: ldr pc, [r3, #0x20]
005b040c: b #0x5b03e0
005b0410: ldr r3, [r5, #0x38]
005b0414: ldr r2, [pc, #0x28]
005b0418: ldr ip, [sp, #0x30]
005b041c: ubfx r3, r3, #4, #6
005b0420: mov r1, #0x28
005b0424: ldr r2, [ip, r2]
005b0428: mul r3, r1, r3
005b042c: ldr r3, [r2, r3]
005b0430: tst r3, #8
005b0434: beq #0x5b03e0
005b0438: b #0x5b03cc
005b043c: mlaseq lr, r0, sl, r4
005b0440: eorseq pc, r2, r8, ror pc
005b0444: andeq r1, r0, r4, lsr pc
005b0448: eorseq r0, r3, r0, lsr #32

_ZN6glitch5video8ITextureC1EPKcPNS0_12IVideoDriverERKNS0_12STextureDescE
005fe404: ldr ip, [pc, #0x2cc]
005fe408: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005fe40c: ldr lr, [pc, #0x2c8]
005fe410: add ip, pc, ip
005fe414: mov r6, r0
005fe418: ldr lr, [ip, lr]
005fe41c: sub sp, sp, #0x24
005fe420: mov r4, #0
005fe424: add lr, lr, #8
005fe428: str r4, [r6, #4]
005fe42c: mov r4, r2
005fe430: str lr, [r0], #8
005fe434: add r2, sp, #0x1c
005fe438: mov r5, r3
005fe43c: bl #0x32603c
005fe440: ldr r3, [r5, #0x10]
005fe444: str r3, [r6, #0x20]
005fe448: ldr r3, [r5, #0x14]
005fe44c: str r3, [r6, #0x24]
005fe450: ldr r3, [r5]
005fe454: cmp r3, #1
005fe458: ldreq r2, [r5, #0x18]
005fe45c: mov r3, #0
005fe460: movne r2, #1
005fe464: str r3, [r6, #0x38]
005fe468: str r3, [r6, #0x2c]
005fe46c: str r3, [r6, #0x30]
005fe470: mvn r3, #0
005fe474: str r2, [r6, #0x28]
005fe478: str r4, [r6, #0x34]
005fe47c: strh r3, [r6, #0x3c]
005fe480: ldrb r0, [r5, #0x1c]
005fe484: cmp r0, #0
005fe488: moveq r2, #1
005fe48c: beq #0x5fe524
005fe490: ldr r3, [r5, #0x10]
005fe494: cmp r3, #0
005fe498: mvneq r2, #0
005fe49c: beq #0x5fe4b0
005fe4a0: mvn r2, #0
005fe4a4: lsrs r3, r3, #1
005fe4a8: add r2, r2, #1
005fe4ac: bne #0x5fe4a4
005fe4b0: ldr r3, [r5, #0x14]
005fe4b4: str r2, [sp, #0x18]
005fe4b8: cmp r3, #0
005fe4bc: mvneq r1, #0
005fe4c0: beq #0x5fe4d4
005fe4c4: mvn r1, #0
005fe4c8: lsrs r3, r3, #1
005fe4cc: add r1, r1, #1
005fe4d0: bne #0x5fe4c8
005fe4d4: ldr r3, [r5, #0x18]
005fe4d8: str r1, [sp, #0x14]
005fe4dc: cmp r3, #0
005fe4e0: mvneq r0, #0
005fe4e4: beq #0x5fe4f8
005fe4e8: mvn r0, #0
005fe4ec: lsrs r3, r3, #1
005fe4f0: add r0, r0, #1
005fe4f4: bne #0x5fe4ec
005fe4f8: cmp r1, r2
005fe4fc: movhi r2, r1
005fe500: addhi r3, sp, #0x14
005fe504: addls r3, sp, #0x18
005fe508: cmp r2, r0
005fe50c: str r0, [sp, #0x10]
005fe510: addlo r3, sp, #0x10
005fe514: ldr r2, [r3]
005fe518: add r2, r2, #1
005fe51c: uxtb r2, r2
005fe520: sub r0, r2, #1
005fe524: strb r2, [r6, #0x3e]
005fe528: ldrb r1, [r5, #0x1d]
005fe52c: mov r3, #0
005fe530: str r3, [r6, #0x4c]
005fe534: cmp r1, #0
005fe538: moveq ip, r1
005fe53c: movne ip, #4
005fe540: strb ip, [r6, #0x3f]
005fe544: movw ip, #0x1ffd
005fe548: strh ip, [r6, #0x40]
005fe54c: mov r1, #0
005fe550: mov ip, #0x3f800000
005fe554: str ip, [r6, #0x44]
005fe558: str r3, [r6, #0x48]
005fe55c: strb r1, [r6, #0x43]
005fe560: strb r1, [r6, #0x42]
005fe564: ldr r1, [r5]
005fe568: ldr r3, [r6, #0x38]
005fe56c: and r1, r1, #3
005fe570: bic r3, r3, #3
005fe574: orr r3, r1, r3
005fe578: str r3, [r6, #0x38]
005fe57c: ldr r1, [r5, #8]
005fe580: bic r3, r3, #0xc
005fe584: and r1, r1, #3
005fe588: orr r3, r3, r1, lsl #2
005fe58c: str r3, [r6, #0x38]
005fe590: ldr r1, [r5, #0xc]
005fe594: bic r3, r3, #0xc00
005fe598: and r1, r1, #3
005fe59c: orr r3, r3, r1, lsl #10
005fe5a0: str r3, [r6, #0x38]
005fe5a4: ldr r1, [r5, #4]
005fe5a8: bic r3, r3, #0x3f0
005fe5ac: and r1, r1, #0x3f
005fe5b0: orr r3, r3, r1, lsl #4
005fe5b4: str r3, [r6, #0x38]
005fe5b8: ldrb r1, [r5, #0x1c]
005fe5bc: bic r3, r3, #0x3f000
005fe5c0: cmp r1, #0
005fe5c4: movne r1, #0x3000
005fe5c8: moveq r1, #0x1000
005fe5cc: orr r3, r3, r1
005fe5d0: orr r3, r3, #0x8000
005fe5d4: bic r3, r3, #0xff00000
005fe5d8: bic r3, r3, #0xc0000
005fe5dc: tst r3, #0x70000000
005fe5e0: str r3, [r6, #0x38]
005fe5e4: bicne r3, r3, #0x70000000
005fe5e8: strne r3, [r6, #0x38]
005fe5ec: subne r0, r2, #1
005fe5f0: bl #0x30e964
005fe5f4: ldr r3, [r6, #0x38]
005fe5f8: ldrb sl, [r6, #0x3e]
005fe5fc: str r0, [r6, #0x50]
005fe600: and r0, r3, #3
005fe604: cmp r0, #2
005fe608: moveq r0, #6
005fe60c: movne r0, #1
005fe610: mul r0, sl, r0
005fe614: add r2, sl, #1
005fe618: add r0, r0, #0x1f
005fe61c: add r0, r2, r0, lsr #5
005fe620: mov r1, #0
005fe624: lsl r0, r0, #2
005fe628: bl #0x5341a8
005fe62c: mov r8, r0
005fe630: ldr r0, [r6, #0x30]
005fe634: str r8, [r6, #0x30]
005fe638: cmp r0, #0
005fe63c: beq #0x5fe648
005fe640: bl #0x30e0b8
005fe644: ldr r8, [r6, #0x30]
005fe648: ldmib r5, {r3, fp}
005fe64c: cmp fp, #1
005fe650: movne fp, #0
005fe654: moveq fp, #1
005fe658: str r3, [sp, #0xc]
005fe65c: cmp sl, #0
005fe660: ldr sb, [r5, #0x18]
005fe664: moveq r7, sl
005fe668: beq #0x5fe6bc
005fe66c: mov r4, #0
005fe670: mov r7, r4
005fe674: mov ip, r4
005fe678: str r7, [r8, r4, lsl #2]
005fe67c: ldr r1, [r5, #0x10]
005fe680: ldr r2, [r5, #0x14]
005fe684: ldr r0, [sp, #0xc]
005fe688: mov r3, sb
005fe68c: str ip, [sp]
005fe690: str fp, [sp, #4]
005fe694: bl #0x5edbec
005fe698: add r4, r4, #1
005fe69c: uxtb ip, r4
005fe6a0: cmp ip, sl
005fe6a4: add r7, r7, r0
005fe6a8: blo #0x5fe678
005fe6ac: sub sl, sl, #1
005fe6b0: uxtb sl, sl
005fe6b4: add sl, sl, #1
005fe6b8: add r8, r8, sl, lsl #2
005fe6bc: mov r0, r6
005fe6c0: str r7, [r8]
005fe6c4: mov r1, #1
005fe6c8: bl #0x5fdae8
005fe6cc: mov r0, r6
005fe6d0: add sp, sp, #0x24
005fe6d4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005fe6d8: eorseq r6, sb, r0, lsl #13
005fe6dc: andeq r3, r0, r4, lsl #10

_ZN6glitch5video18ICodeShaderManagerC2Ev
006e0cf0: push {r4, r5, r6, lr}
006e0cf4: ldr r5, [pc, #0x38]
006e0cf8: mov r4, r0
006e0cfc: bl #0x5e5770
006e0d00: ldr r3, [pc, #0x30]
006e0d04: add r5, pc, r5
006e0d08: mov r0, r4
006e0d0c: ldr r3, [r5, r3]
006e0d10: add r3, r3, #8
006e0d14: str r3, [r0], #0x54
006e0d18: bl #0x6e0804
006e0d1c: mov r3, #0
006e0d20: str r3, [r4, #0x7c]
006e0d24: mvn r3, #0
006e0d28: str r3, [r4, #0x80]
006e0d2c: mov r0, r4
006e0d30: pop {r4, r5, r6, pc}
006e0d34: eoreq r3, fp, ip, lsl #27
006e0d38: andeq r4, r0, r4, ror #3

_ZN6glitch7collada6CImageC1ERKNS0_16CColladaDatabaseERNS0_6SImageE
0060e1d4: push {r4, lr}
0060e1d8: ldr r3, [r1]
0060e1dc: mov r4, r0
0060e1e0: ldr r0, [pc, #0x98]
0060e1e4: str r3, [r4, #0xc]
0060e1e8: ldr r1, [r1, #4]
0060e1ec: cmp r3, #0
0060e1f0: add r0, pc, r0
0060e1f4: str r1, [r4, #0x10]
0060e1f8: beq #0x60e20c
0060e1fc: ldr r1, [r3, #4]
0060e200: cmp r1, #0
0060e204: addne r1, r1, #1
0060e208: strne r1, [r3, #4]
0060e20c: ldr r1, [pc, #0x70]
0060e210: ldr r3, [pc, #0x70]
0060e214: str r2, [r4, #0x18]
0060e218: ldr r1, [r0, r1]
0060e21c: ldr r3, [r0, r3]
0060e220: add r1, r1, #4
0060e224: add r3, r3, #8
0060e228: str r1, [r4, #8]
0060e22c: str r3, [r4]
0060e230: mov r1, #1
0060e234: mov r3, #0
0060e238: str r3, [r4, #0x14]
0060e23c: str r1, [r4, #4]
0060e240: ldr r3, [r2]
0060e244: str r3, [r4, #8]
0060e248: ldr r3, [r2, #0x10]
0060e24c: cmp r3, #0
0060e250: streq r3, [r4, #0x14]
0060e254: beq #0x60e278
0060e258: ldr r2, [r3, #4]
0060e25c: add r2, r2, r1
0060e260: str r2, [r3, #4]
0060e264: ldr r0, [r4, #0x14]
0060e268: str r3, [r4, #0x14]
0060e26c: cmp r0, #0
0060e270: beq #0x60e278
0060e274: bl #0x31d584
0060e278: mov r0, r4
0060e27c: pop {r4, pc}
0060e280: eorseq r6, r8, r0, lsr #17
0060e284: strheq r1, [r0], -r4
0060e288: andeq r3, r0, r8, lsl #27

_ZN6glitch5video6CImageC2ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEE
00602184: ldr ip, [pc, #0x64]
00602188: push {r4, r5, r6, lr}
0060218c: ldr lr, [pc, #0x60]
00602190: add ip, pc, ip
00602194: mov r3, #0
00602198: ldr lr, [ip, lr]
0060219c: str r3, [r0, #4]
006021a0: str r3, [r0, #8]
006021a4: add lr, lr, #8
006021a8: str lr, [r0]
006021ac: str r3, [r0, #0xc]
006021b0: ldr r5, [r2]
006021b4: mov lr, #1
006021b8: mov r4, r0
006021bc: str r5, [r0, #0x10]
006021c0: ldr r2, [r2, #4]
006021c4: str r1, [r0, #0x20]
006021c8: strb r3, [r0, #0x28]
006021cc: str r2, [r0, #0x14]
006021d0: str r3, [r0, #0x18]
006021d4: str r3, [r0, #0x1c]
006021d8: str r3, [r0, #0x24]
006021dc: strb lr, [r0, #0x29]
006021e0: mov r1, lr
006021e4: bl #0x601988
006021e8: mov r0, r4
006021ec: pop {r4, r5, r6, pc}
006021f0: eorseq r2, sb, r0, lsl #18
006021f4: andeq r0, r0, r0, lsl r6

_ZN6glitch5video8ITexture4bindEb
005fde9c: push {r4, lr}
005fdea0: ldrb r3, [r0, #0x3f]
005fdea4: sub sp, sp, #8
005fdea8: mov r4, r0
005fdeac: tst r3, #8
005fdeb0: bne #0x5fdf18
005fdeb4: ldr r3, [r4]
005fdeb8: mov r0, r4
005fdebc: mov lr, pc
005fdec0: ldr pc, [r3, #0xc]
005fdec4: cmp r0, #0
005fdec8: beq #0x5fdf10
005fdecc: ldr r0, [r4, #0x34]
005fded0: ldr r3, [r0, #0x9c]
005fded4: tst r3, #0x1000000
005fded8: beq #0x5fdf10
005fdedc: ldr r3, [r0, #0x138]
005fdee0: tst r3, #6
005fdee4: bne #0x5fdf10
005fdee8: ldr r3, [r4, #4]
005fdeec: add r1, sp, #8
005fdef0: str r4, [r1, #-4]!
005fdef4: add r3, r3, #1
005fdef8: str r3, [r4, #4]
005fdefc: bl #0x5acfd0
005fdf00: ldr r0, [sp, #4]
005fdf04: cmp r0, #0
005fdf08: beq #0x5fdf10
005fdf0c: bl #0x31d584
005fdf10: add sp, sp, #8
005fdf14: pop {r4, pc}
005fdf18: ldrh r3, [r0, #0x40]
005fdf1c: tst r3, #1
005fdf20: beq #0x5fdf10
005fdf24: b #0x5fdeb4

_ZN6glitch5video19CCommonGLDriverBaseC2EPNS_7IDeviceEPNS0_14IShaderManagerE
006de5f8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006de5fc: sub sp, sp, #0x1c
006de600: mov ip, #0
006de604: add lr, sp, #0x18
006de608: str ip, [lr, #-4]!
006de60c: mov r3, ip
006de610: str lr, [sp, #8]
006de614: str ip, [sp]
006de618: str ip, [sp, #4]
006de61c: mov r4, r0
006de620: bl #0x5aaf1c
006de624: ldr r0, [sp, #0x14]
006de628: ldr r5, [pc, #0x20c]
006de62c: cmp r0, #0
006de630: add r5, pc, r5
006de634: beq #0x6de64c
006de638: ldr r3, [r0]
006de63c: sub r3, r3, #1
006de640: cmp r3, #0
006de644: str r3, [r0]
006de648: beq #0x6de808
006de64c: ldr r2, [pc, #0x1ec]
006de650: mov r1, #0
006de654: mov r3, #0
006de658: ldr r2, [r5, r2]
006de65c: strb r1, [r4, #0x160]
006de660: add r0, r4, #0x164
006de664: add r2, r2, #8
006de668: str r2, [r4]
006de66c: add r2, r4, #0x1c4
006de670: str r3, [r0]
006de674: str r3, [r0, #4]
006de678: str r3, [r0, #0xc]
006de67c: str r3, [r0, #0x10]
006de680: str r3, [r0, #0x14]
006de684: add r0, r0, #0x18
006de688: cmp r0, r2
006de68c: bne #0x6de670
006de690: bl #0x6dcac0
006de694: add r0, r4, #0x254
006de698: bl #0x6dcd90
006de69c: mov r3, #0xa
006de6a0: mov r6, #0x3f800000
006de6a4: str r3, [r4, #0x284]
006de6a8: add r5, r4, #0x288
006de6ac: add sl, r4, #0x354
006de6b0: mov sb, #0
006de6b4: mov fp, #1
006de6b8: mov r7, #0
006de6bc: strb sb, [r5, #0x40]
006de6c0: mov r0, r5
006de6c4: mov r1, r7
006de6c8: mov r2, #0x40
006de6cc: bl #0x30e460
006de6d0: str r6, [r5]
006de6d4: str r6, [r5, #0x14]
006de6d8: str r6, [r5, #0x28]
006de6dc: str r6, [r5, #0x3c]
006de6e0: strb fp, [r5, #0x40]
006de6e4: add r5, r5, #0x44
006de6e8: cmp r5, sl
006de6ec: mov r8, #1
006de6f0: bne #0x6de6b8
006de6f4: mov r1, r7
006de6f8: mov r2, #0x40
006de6fc: strb r7, [r4, #0x394]
006de700: mov r0, r5
006de704: bl #0x30e460
006de708: mov r1, r7
006de70c: mov r2, #0x40
006de710: str r6, [r4, #0x354]
006de714: str r6, [r4, #0x368]
006de718: str r6, [r4, #0x37c]
006de71c: str r6, [r4, #0x390]
006de720: strb r8, [r4, #0x394]
006de724: strb r7, [r4, #0x3d8]
006de728: add r0, r4, #0x398
006de72c: bl #0x30e460
006de730: mov r1, r7
006de734: mov r2, #0x40
006de738: str r6, [r4, #0x398]
006de73c: str r6, [r4, #0x3ac]
006de740: str r6, [r4, #0x3c0]
006de744: str r6, [r4, #0x3d4]
006de748: strb r8, [r4, #0x3d8]
006de74c: strb r7, [r4, #0x41c]
006de750: add r0, r4, #0x3dc
006de754: bl #0x30e460
006de758: mov r3, #0
006de75c: add r2, r4, #0x7c0
006de760: str r3, [r4, #0x4a8]
006de764: str r6, [r4, #0x418]
006de768: strb r8, [r4, #0x41c]
006de76c: str r6, [r4, #0x3dc]
006de770: str r6, [r4, #0x3f0]
006de774: str r6, [r4, #0x404]
006de778: strb r7, [r4, #0x4a0]
006de77c: strb r7, [r4, #0x4a1]
006de780: str r7, [r4, #0x4a4]
006de784: add r2, r2, #0xc
006de788: mov r1, r7
006de78c: add r3, r4, #0x4c0
006de790: mov r0, #0x27
006de794: strh r0, [r3, #-0x14]
006de798: strh r0, [r3, #-0x12]
006de79c: str r1, [r3, #-0x10]
006de7a0: str r1, [r3, #-0xc]
006de7a4: str r1, [r3, #-8]
006de7a8: str r1, [r3, #-4]
006de7ac: add r3, r3, #0x14
006de7b0: cmp r3, r2
006de7b4: bne #0x6de790
006de7b8: str r1, [r4, #0x7b8]
006de7bc: str r1, [r4, #0x7bc]
006de7c0: str r1, [r4, #0x7c0]
006de7c4: str r1, [r4, #0x7c4]
006de7c8: str r1, [r4, #0x7c8]
006de7cc: str r1, [r4, #0x7cc]
006de7d0: str r1, [r4, #0x7d0]
006de7d4: str r1, [r4, #0x7d4]
006de7d8: str r1, [r4, #0x7d8]
006de7dc: str r1, [r4, #0x7dc]
006de7e0: str r1, [r4, #0x7e0]
006de7e4: str r1, [r4, #0x7e4]
006de7e8: str r1, [r4, #0x7e8]
006de7ec: str r1, [r4, #0x7ec]
006de7f0: add r0, r4, #0x420
006de7f4: mov r2, #0x80
006de7f8: bl #0x30e460
006de7fc: mov r0, r4
006de800: add sp, sp, #0x1c
006de804: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006de808: ldrb r3, [r0, #0x54]
006de80c: cmp r3, #0
006de810: bne #0x6de82c
006de814: ldr r3, [pc, #0x28]
006de818: ldr r2, [r0, #0x50]
006de81c: ldr r3, [r5, r3]
006de820: ldr r1, [r3]
006de824: str r1, [r2]
006de828: str r2, [r3]
006de82c: mov r3, #0
006de830: str r3, [r0, #0x50]
006de834: bl #0x30e2b0
006de838: b #0x6de64c
006de83c: eoreq r6, fp, r0, ror #8
006de840: andeq r3, r0, ip, ror #26
006de844: andeq r3, r0, r0, asr #25

_ZN6glitch5video8ITexture7setDataEPvbb
005fdf74: push {r4, r5, r6, r7, r8, lr}
005fdf78: mov r4, r0
005fdf7c: ldr r0, [r0, #0x2c]
005fdf80: mov r5, r1
005fdf84: mov r6, r2
005fdf88: cmp r1, r0
005fdf8c: mov r7, r3
005fdf90: beq #0x5fe084
005fdf94: cmp r0, #0
005fdf98: beq #0x5fe004
005fdf9c: ldrb r3, [r4, #0x3f]
005fdfa0: tst r3, #1
005fdfa4: bne #0x5fe000
005fdfa8: cmp r5, #0
005fdfac: str r5, [r4, #0x2c]
005fdfb0: movne r5, #1
005fdfb4: beq #0x5fe018
005fdfb8: ldrb ip, [r4, #0x3e]
005fdfbc: cmp r6, #0
005fdfc0: orrne r3, r3, #1
005fdfc4: andeq r3, r3, #0xfe
005fdfc8: cmp ip, #1
005fdfcc: strb r3, [r4, #0x3f]
005fdfd0: bls #0x5fe064
005fdfd4: cmp r7, #0
005fdfd8: beq #0x5fe064
005fdfdc: and r1, r3, #2
005fdfe0: uxtb r1, r1
005fdfe4: cmp r1, #0
005fdfe8: beq #0x5fe098
005fdfec: orr r3, r3, #2
005fdff0: cmp r5, #0
005fdff4: strb r3, [r4, #0x3f]
005fdff8: bne #0x5fe074
005fdffc: pop {r4, r5, r6, r7, r8, pc}
005fe000: bl #0x30e0b8
005fe004: cmp r5, #0
005fe008: str r5, [r4, #0x2c]
005fe00c: ldrb r3, [r4, #0x3f]
005fe010: movne r5, #1
005fe014: bne #0x5fdfb8
005fe018: orr r3, r3, #1
005fe01c: tst r3, #8
005fe020: strb r3, [r4, #0x3f]
005fe024: ldrh r3, [r4, #0x40]
005fe028: ldrb r2, [r4, #0x3e]
005fe02c: bicne r3, r3, #1
005fe030: lslne r3, r3, #0x10
005fe034: lsrne r3, r3, #0x10
005fe038: strhne r3, [r4, #0x40]
005fe03c: bic r3, r3, #2
005fe040: cmp r2, #1
005fe044: strh r3, [r4, #0x40]
005fe048: bls #0x5fe0bc
005fe04c: cmp r7, #0
005fe050: beq #0x5fe0bc
005fe054: ldrb r3, [r4, #0x3f]
005fe058: orr r3, r3, #2
005fe05c: strb r3, [r4, #0x3f]
005fe060: pop {r4, r5, r6, r7, r8, pc}
005fe064: bic r3, r3, #2
005fe068: cmp r5, #0
005fe06c: strb r3, [r4, #0x3f]
005fe070: beq #0x5fdffc
005fe074: mov r0, r4
005fe078: mov r1, #0
005fe07c: pop {r4, r5, r6, r7, r8, lr}
005fe080: b #0x5fdae8
005fe084: cmp r1, #0
005fe088: beq #0x5fe0cc
005fe08c: ldrb r3, [r4, #0x3f]
005fe090: mov r5, #0
005fe094: b #0x5fdfb8
005fe098: ldr r3, [r4, #0x30]
005fe09c: add r2, ip, #0x1f
005fe0a0: asr r2, r2, #5
005fe0a4: add r0, ip, #1
005fe0a8: add r0, r3, r0, lsl #2
005fe0ac: lsl r2, r2, #2
005fe0b0: bl #0x30e460
005fe0b4: ldrb r3, [r4, #0x3f]
005fe0b8: b #0x5fdfec
005fe0bc: ldrb r3, [r4, #0x3f]
005fe0c0: bic r3, r3, #2
005fe0c4: strb r3, [r4, #0x3f]
005fe0c8: b #0x5fdffc
005fe0cc: ldrb r3, [r4, #0x3f]
005fe0d0: b #0x5fe018

_ZN6glitch5video12IVideoDriver31initImplementationDependentDataEv
005ae5f0: push {r4, r5, r6, r7, r8, lr}
005ae5f4: mov r2, #0
005ae5f8: sub sp, sp, #0xb8
005ae5fc: mov r3, #1
005ae600: mov r4, r0
005ae604: str r3, [sp, #8]
005ae608: str r2, [sp]
005ae60c: str r2, [sp, #4]
005ae610: add r5, sp, #0xb4
005ae614: ldr ip, [r0]
005ae618: mov r1, r4
005ae61c: mov r3, #4
005ae620: mov r0, r5
005ae624: mov lr, pc
005ae628: ldr pc, [ip, #0x78]
005ae62c: ldr r1, [r4, #0xc0]
005ae630: ldr r3, [r4, #0xc4]
005ae634: cmp r1, r3
005ae638: beq #0x5aef6c
005ae63c: ldr r3, [sp, #0xb4]
005ae640: cmp r3, #0
005ae644: str r3, [r1]
005ae648: ldrne r2, [r3, #4]
005ae64c: addne r2, r2, #1
005ae650: strne r2, [r3, #4]
005ae654: ldr r3, [r4, #0xc0]
005ae658: add r3, r3, #4
005ae65c: str r3, [r4, #0xc0]
005ae660: ldr r0, [sp, #0xb4]
005ae664: cmp r0, #0
005ae668: beq #0x5ae670
005ae66c: bl #0x31d584
005ae670: mov r2, #0
005ae674: mov r3, #1
005ae678: str r2, [sp]
005ae67c: stmib sp, {r2, r3}
005ae680: add r0, sp, #0xb0
005ae684: mov r3, #4
005ae688: ldr ip, [r4]
005ae68c: mov r1, r4
005ae690: mov lr, pc
005ae694: ldr pc, [ip, #0x78]
005ae698: ldr r3, [sp, #0xb0]
005ae69c: cmp r3, #0
005ae6a0: ldrne r2, [r3, #4]
005ae6a4: addne r2, r2, #1
005ae6a8: strne r2, [r3, #4]
005ae6ac: ldr r0, [r4, #0xb0]
005ae6b0: str r3, [r4, #0xb0]
005ae6b4: cmp r0, #0
005ae6b8: beq #0x5ae6c0
005ae6bc: bl #0x31d584
005ae6c0: ldr r0, [sp, #0xb0]
005ae6c4: cmp r0, #0
005ae6c8: beq #0x5ae6d0
005ae6cc: bl #0x31d584
005ae6d0: mov r2, #0
005ae6d4: mov r3, #1
005ae6d8: str r2, [sp]
005ae6dc: stmib sp, {r2, r3}
005ae6e0: add r0, sp, #0xac
005ae6e4: mov r3, #4
005ae6e8: ldr ip, [r4]
005ae6ec: mov r1, r4
005ae6f0: mov lr, pc
005ae6f4: ldr pc, [ip, #0x78]
005ae6f8: ldr r3, [sp, #0xac]
005ae6fc: cmp r3, #0
005ae700: ldrne r2, [r3, #4]
005ae704: addne r2, r2, #1
005ae708: strne r2, [r3, #4]
005ae70c: ldr r0, [r4, #0xb4]
005ae710: str r3, [r4, #0xb4]
005ae714: cmp r0, #0
005ae718: beq #0x5ae720
005ae71c: bl #0x31d584
005ae720: ldr r0, [sp, #0xac]
005ae724: cmp r0, #0
005ae728: beq #0x5ae730
005ae72c: bl #0x31d584
005ae730: mov r2, #1
005ae734: mov r3, #0
005ae738: str r2, [sp, #8]
005ae73c: str r3, [sp, #4]
005ae740: str r3, [sp]
005ae744: add r0, sp, #0xa8
005ae748: mov r3, #4
005ae74c: ldr ip, [r4]
005ae750: mov r1, r4
005ae754: mov lr, pc
005ae758: ldr pc, [ip, #0x78]
005ae75c: ldr r3, [sp, #0xa8]
005ae760: cmp r3, #0
005ae764: ldrne r2, [r3, #4]
005ae768: addne r2, r2, #1
005ae76c: strne r2, [r3, #4]
005ae770: ldr r0, [r4, #0xb8]
005ae774: str r3, [r4, #0xb8]
005ae778: cmp r0, #0
005ae77c: beq #0x5ae784
005ae780: bl #0x31d584
005ae784: ldr r0, [sp, #0xa8]
005ae788: cmp r0, #0
005ae78c: beq #0x5ae794
005ae790: bl #0x31d584
005ae794: ldr r3, [r4, #0x9c]
005ae798: tst r3, #0x1000000
005ae79c: bne #0x5aecfc
005ae7a0: mov r2, #0x40000
005ae7a4: add r0, sp, #0x94
005ae7a8: mov r1, #0
005ae7ac: bl #0x5a1404
005ae7b0: ldr r3, [sp, #0x94]
005ae7b4: cmp r3, #0
005ae7b8: ldrne r2, [r3]
005ae7bc: addne r2, r2, #1
005ae7c0: strne r2, [r3]
005ae7c4: ldr r5, [r4, #0xa4]
005ae7c8: str r3, [r4, #0xa4]
005ae7cc: cmp r5, #0
005ae7d0: beq #0x5ae7e8
005ae7d4: ldr r3, [r5]
005ae7d8: sub r3, r3, #1
005ae7dc: cmp r3, #0
005ae7e0: str r3, [r5]
005ae7e4: beq #0x5aece8
005ae7e8: ldr r5, [sp, #0x94]
005ae7ec: cmp r5, #0
005ae7f0: beq #0x5ae808
005ae7f4: ldr r3, [r5]
005ae7f8: sub r3, r3, #1
005ae7fc: cmp r3, #0
005ae800: str r3, [r5]
005ae804: beq #0x5aecd4
005ae808: ldr r3, [r4, #0xb0]
005ae80c: ldr r0, [r4, #0xa4]
005ae810: cmp r3, #0
005ae814: str r3, [sp, #0x74]
005ae818: ldrne r2, [r3, #4]
005ae81c: add r1, r0, #0x14
005ae820: addne r2, r2, #1
005ae824: strne r2, [r3, #4]
005ae828: mov r3, #4
005ae82c: str r3, [sp, #0x78]
005ae830: mov r3, #6
005ae834: str r3, [sp, #0x7c]
005ae838: mov r3, #3
005ae83c: strh r3, [sp, #0x80]
005ae840: add r2, sp, #0x74
005ae844: mov r3, #0x10
005ae848: strh r3, [sp, #0x82]
005ae84c: bl #0x5abbcc
005ae850: ldr r0, [sp, #0x74]
005ae854: cmp r0, #0
005ae858: beq #0x5ae860
005ae85c: bl #0x31d584
005ae860: ldr r3, [r4, #0xb0]
005ae864: ldr r0, [r4, #0xa4]
005ae868: cmp r3, #0
005ae86c: str r3, [sp, #0x64]
005ae870: ldrne r2, [r3, #4]
005ae874: add r1, r0, #0x24
005ae878: addne r2, r2, #1
005ae87c: strne r2, [r3, #4]
005ae880: mov r3, #0
005ae884: str r3, [sp, #0x68]
005ae888: mov r3, #1
005ae88c: str r3, [sp, #0x6c]
005ae890: mov r3, #4
005ae894: strh r3, [sp, #0x70]
005ae898: add r2, sp, #0x64
005ae89c: mov r3, #0x10
005ae8a0: strh r3, [sp, #0x72]
005ae8a4: bl #0x5abbcc
005ae8a8: ldr r0, [sp, #0x64]
005ae8ac: cmp r0, #0
005ae8b0: beq #0x5ae8b8
005ae8b4: bl #0x31d584
005ae8b8: mov r2, #0x40000
005ae8bc: add r0, sp, #0x90
005ae8c0: mov r1, #0
005ae8c4: bl #0x5a1404
005ae8c8: ldr r3, [sp, #0x90]
005ae8cc: cmp r3, #0
005ae8d0: ldrne r2, [r3]
005ae8d4: addne r2, r2, #1
005ae8d8: strne r2, [r3]
005ae8dc: ldr r5, [r4, #0xa8]
005ae8e0: str r3, [r4, #0xa8]
005ae8e4: cmp r5, #0
005ae8e8: beq #0x5ae900
005ae8ec: ldr r3, [r5]
005ae8f0: sub r3, r3, #1
005ae8f4: cmp r3, #0
005ae8f8: str r3, [r5]
005ae8fc: beq #0x5aecc0
005ae900: ldr r5, [sp, #0x90]
005ae904: cmp r5, #0
005ae908: beq #0x5ae920
005ae90c: ldr r3, [r5]
005ae910: sub r3, r3, #1
005ae914: cmp r3, #0
005ae918: str r3, [r5]
005ae91c: beq #0x5aecac
005ae920: ldr r3, [r4, #0xb0]
005ae924: ldr r0, [r4, #0xa8]
005ae928: cmp r3, #0
005ae92c: str r3, [sp, #0x54]
005ae930: ldrne r2, [r3, #4]
005ae934: add r1, r0, #0x14
005ae938: addne r2, r2, #1
005ae93c: strne r2, [r3, #4]
005ae940: mov r3, #0
005ae944: str r3, [sp, #0x58]
005ae948: mov r3, #6
005ae94c: str r3, [sp, #0x5c]
005ae950: mov r3, #3
005ae954: strh r3, [sp, #0x60]
005ae958: add r2, sp, #0x54
005ae95c: mov r3, #0xc
005ae960: strh r3, [sp, #0x62]
005ae964: bl #0x5abbcc
005ae968: ldr r0, [sp, #0x54]
005ae96c: cmp r0, #0
005ae970: beq #0x5ae978
005ae974: bl #0x31d584
005ae978: ldr r3, [r4, #0xb4]
005ae97c: ldr r0, [r4, #0xa8]
005ae980: cmp r3, #0
005ae984: str r3, [sp, #0x44]
005ae988: ldrne r2, [r3, #4]
005ae98c: add r1, r0, #0x24
005ae990: addne r2, r2, #1
005ae994: strne r2, [r3, #4]
005ae998: mov r3, #0
005ae99c: str r3, [sp, #0x48]
005ae9a0: mov r3, #1
005ae9a4: str r3, [sp, #0x4c]
005ae9a8: add r2, sp, #0x44
005ae9ac: mov r3, #4
005ae9b0: strh r3, [sp, #0x50]
005ae9b4: strh r3, [sp, #0x52]
005ae9b8: bl #0x5abbcc
005ae9bc: ldr r0, [sp, #0x44]
005ae9c0: cmp r0, #0
005ae9c4: beq #0x5ae9cc
005ae9c8: bl #0x31d584
005ae9cc: mov r2, #0x40000
005ae9d0: add r0, sp, #0x8c
005ae9d4: mov r1, #1
005ae9d8: bl #0x5a1404
005ae9dc: ldr r3, [sp, #0x8c]
005ae9e0: cmp r3, #0
005ae9e4: ldrne r2, [r3]
005ae9e8: addne r2, r2, #1
005ae9ec: strne r2, [r3]
005ae9f0: ldr r5, [r4, #0xac]
005ae9f4: str r3, [r4, #0xac]
005ae9f8: cmp r5, #0
005ae9fc: beq #0x5aea14
005aea00: ldr r3, [r5]
005aea04: sub r3, r3, #1
005aea08: cmp r3, #0
005aea0c: str r3, [r5]
005aea10: beq #0x5aec98
005aea14: ldr r5, [sp, #0x8c]
005aea18: cmp r5, #0
005aea1c: beq #0x5aea34
005aea20: ldr r3, [r5]
005aea24: sub r3, r3, #1
005aea28: cmp r3, #0
005aea2c: str r3, [r5]
005aea30: beq #0x5aec84
005aea34: ldr r3, [r4, #0xb0]
005aea38: ldr r0, [r4, #0xac]
005aea3c: cmp r3, #0
005aea40: str r3, [sp, #0x34]
005aea44: ldrne r2, [r3, #4]
005aea48: add r1, r0, #0x14
005aea4c: addne r2, r2, #1
005aea50: strne r2, [r3, #4]
005aea54: mov r3, #0xc
005aea58: str r3, [sp, #0x38]
005aea5c: mov r3, #6
005aea60: str r3, [sp, #0x3c]
005aea64: mov r3, #3
005aea68: strh r3, [sp, #0x40]
005aea6c: add r2, sp, #0x34
005aea70: mov r3, #0x18
005aea74: strh r3, [sp, #0x42]
005aea78: bl #0x5abbcc
005aea7c: ldr r0, [sp, #0x34]
005aea80: cmp r0, #0
005aea84: beq #0x5aea8c
005aea88: bl #0x31d584
005aea8c: ldr r3, [r4, #0xb0]
005aea90: ldr r0, [r4, #0xac]
005aea94: cmp r3, #0
005aea98: str r3, [sp, #0x24]
005aea9c: ldrne r2, [r3, #4]
005aeaa0: add r1, r0, #0x24
005aeaa4: addne r2, r2, #1
005aeaa8: strne r2, [r3, #4]
005aeaac: mov r3, #0
005aeab0: str r3, [sp, #0x28]
005aeab4: mov r3, #6
005aeab8: str r3, [sp, #0x2c]
005aeabc: mov r3, #2
005aeac0: strh r3, [sp, #0x30]
005aeac4: add r2, sp, #0x24
005aeac8: mov r3, #0x18
005aeacc: strh r3, [sp, #0x32]
005aead0: bl #0x5abbcc
005aead4: ldr r0, [sp, #0x24]
005aead8: cmp r0, #0
005aeadc: beq #0x5aeae4
005aeae0: bl #0x31d584
005aeae4: ldr r3, [r4, #0xb0]
005aeae8: ldr r0, [r4, #0xac]
005aeaec: cmp r3, #0
005aeaf0: str r3, [sp, #0x14]
005aeaf4: ldrne r2, [r3, #4]
005aeaf8: add r1, r0, #0x34
005aeafc: addne r2, r2, #1
005aeb00: strne r2, [r3, #4]
005aeb04: mov r3, #8
005aeb08: str r3, [sp, #0x18]
005aeb0c: mov r3, #1
005aeb10: str r3, [sp, #0x1c]
005aeb14: mov r3, #4
005aeb18: strh r3, [sp, #0x20]
005aeb1c: add r2, sp, #0x14
005aeb20: mov r3, #0x18
005aeb24: strh r3, [sp, #0x22]
005aeb28: bl #0x5abbcc
005aeb2c: ldr r0, [sp, #0x14]
005aeb30: cmp r0, #0
005aeb34: beq #0x5aeb3c
005aeb38: bl #0x31d584
005aeb3c: ldr r3, [r4, #0xd4]
005aeb40: mov r1, #0
005aeb44: mov r0, #0x70
005aeb48: ldr r6, [r3, #0xa8]
005aeb4c: ldr r7, [r3, #0xa4]
005aeb50: bl #0x5341ac
005aeb54: mov ip, #4
005aeb58: mov r3, r4
005aeb5c: mov r1, r7
005aeb60: mov r2, r6
005aeb64: str ip, [sp]
005aeb68: mvn ip, #0
005aeb6c: mov r5, r0
005aeb70: str ip, [sp, #4]
005aeb74: bl #0x6b8cb4
005aeb78: cmp r5, #0
005aeb7c: str r5, [sp, #0x88]
005aeb80: ldrne r3, [r5, #4]
005aeb84: add r6, r4, #0x110
005aeb88: addne r3, r3, #1
005aeb8c: strne r3, [r5, #4]
005aeb90: ldr r1, [r4, #0x114]
005aeb94: ldr r3, [r4, #0x118]
005aeb98: cmp r1, r3
005aeb9c: beq #0x5aef7c
005aeba0: ldr r3, [sp, #0x88]
005aeba4: cmp r3, #0
005aeba8: str r3, [r1]
005aebac: ldrne r2, [r3, #4]
005aebb0: addne r2, r2, #1
005aebb4: strne r2, [r3, #4]
005aebb8: ldr r3, [r4, #0x114]
005aebbc: add r3, r3, #4
005aebc0: str r3, [r4, #0x114]
005aebc4: ldr r0, [sp, #0x88]
005aebc8: cmp r0, #0
005aebcc: beq #0x5aebd4
005aebd0: bl #0x31d584
005aebd4: ldr r3, [r4, #0xd4]
005aebd8: mov r1, #0
005aebdc: mov r0, #0x70
005aebe0: ldr r7, [r3, #0xa8]
005aebe4: ldr r8, [r3, #0xa4]
005aebe8: bl #0x5341ac
005aebec: mov ip, #4
005aebf0: mov r3, r4
005aebf4: mov r1, r8
005aebf8: str ip, [sp]
005aebfc: mov r2, r7
005aec00: mvn ip, #0
005aec04: mov r5, r0
005aec08: str ip, [sp, #4]
005aec0c: bl #0x6b8cb4
005aec10: cmp r5, #0
005aec14: str r5, [sp, #0x84]
005aec18: ldrne r3, [r5, #4]
005aec1c: addne r3, r3, #1
005aec20: strne r3, [r5, #4]
005aec24: ldr r1, [r4, #0x114]
005aec28: ldr r3, [r4, #0x118]
005aec2c: cmp r1, r3
005aec30: beq #0x5aef8c
005aec34: ldr r3, [sp, #0x84]
005aec38: cmp r3, #0
005aec3c: str r3, [r1]
005aec40: ldrne r2, [r3, #4]
005aec44: addne r2, r2, #1
005aec48: strne r2, [r3, #4]
005aec4c: ldr r3, [r4, #0x114]
005aec50: add r3, r3, #4
005aec54: str r3, [r4, #0x114]
005aec58: ldr r0, [sp, #0x84]
005aec5c: cmp r0, #0
005aec60: beq #0x5aec68
005aec64: bl #0x31d584
005aec68: ldr r3, [r4, #0x110]
005aec6c: mov r2, #0
005aec70: str r2, [r4, #0x11c]
005aec74: ldr r3, [r3]
005aec78: str r3, [r4, #0x120]
005aec7c: add sp, sp, #0xb8
005aec80: pop {r4, r5, r6, r7, r8, pc}
005aec84: mov r0, r5
005aec88: bl #0x5a0a1c
005aec8c: mov r0, r5
005aec90: bl #0x30e2b0
005aec94: b #0x5aea34
005aec98: mov r0, r5
005aec9c: bl #0x5a0a1c
005aeca0: mov r0, r5
005aeca4: bl #0x30e2b0
005aeca8: b #0x5aea14
005aecac: mov r0, r5
005aecb0: bl #0x5a0a1c
005aecb4: mov r0, r5
005aecb8: bl #0x30e2b0
005aecbc: b #0x5ae920
005aecc0: mov r0, r5
005aecc4: bl #0x5a0a1c
005aecc8: mov r0, r5
005aeccc: bl #0x30e2b0
005aecd0: b #0x5ae900
005aecd4: mov r0, r5
005aecd8: bl #0x5a0a1c
005aecdc: mov r0, r5
005aece0: bl #0x30e2b0
005aece4: b #0x5ae808
005aece8: mov r0, r5
005aecec: bl #0x5a0a1c
005aecf0: mov r0, r5
005aecf4: bl #0x30e2b0
005aecf8: b #0x5ae7e8
005aecfc: mov r1, #0
005aed00: mov r0, #0xc
005aed04: bl #0x5341a8
005aed08: mov r3, #0
005aed0c: str r3, [r0, #8]
005aed10: str r3, [r0]
005aed14: str r3, [r0, #4]
005aed18: mov r3, #0xc
005aed1c: str r3, [sp]
005aed20: mov r3, #1
005aed24: mov r2, #0
005aed28: stmib sp, {r0, r3}
005aed2c: add r5, sp, #0xa4
005aed30: mov r3, r2
005aed34: ldr ip, [r4]
005aed38: mov r0, r5
005aed3c: mov r1, r4
005aed40: mov lr, pc
005aed44: ldr pc, [ip, #0x78]
005aed48: ldr r3, [sp, #0xa4]
005aed4c: ldrb r2, [r3, #0x12]
005aed50: tst r2, #8
005aed54: bne #0x5aef24
005aed58: ldrb r2, [r3, #0x11]
005aed5c: cmp r2, #4
005aed60: beq #0x5aed78
005aed64: mov r0, r3
005aed68: mov r1, #1
005aed6c: ldr r3, [r3]
005aed70: mov lr, pc
005aed74: ldr pc, [r3, #0xc]
005aed78: add r0, sp, #0xa0
005aed7c: mov r1, #0
005aed80: bl #0x5a135c
005aed84: ldr r3, [sp, #0xa0]
005aed88: cmp r3, #0
005aed8c: ldrne r2, [r3]
005aed90: addne r2, r2, #1
005aed94: strne r2, [r3]
005aed98: ldr r6, [r4, #0x140]
005aed9c: str r3, [r4, #0x140]
005aeda0: cmp r6, #0
005aeda4: beq #0x5aedbc
005aeda8: ldr r3, [r6]
005aedac: sub r3, r3, #1
005aedb0: cmp r3, #0
005aedb4: str r3, [r6]
005aedb8: beq #0x5aef44
005aedbc: ldr r6, [sp, #0xa0]
005aedc0: cmp r6, #0
005aedc4: beq #0x5aeddc
005aedc8: ldr r3, [r6]
005aedcc: sub r3, r3, #1
005aedd0: cmp r3, #0
005aedd4: str r3, [r6]
005aedd8: beq #0x5aef30
005aeddc: mov r1, r5
005aede0: mvn r2, #0
005aede4: ldr r0, [r4, #0x140]
005aede8: bl #0x5a15c0
005aedec: ldr r3, [r4, #0x140]
005aedf0: mov r1, #1
005aedf4: mov r2, #0
005aedf8: str r1, [r3, #8]
005aedfc: ldr r3, [r4, #0x140]
005aee00: strb r2, [sp, #0x9f]
005aee04: strb r2, [sp, #0x9c]
005aee08: cmp r3, r2
005aee0c: strb r2, [sp, #0x9d]
005aee10: strb r1, [sp, #0x9e]
005aee14: str r3, [sp, #0x98]
005aee18: ldrne r2, [r3]
005aee1c: mov r0, #0x24
005aee20: addne r2, r2, r1
005aee24: strne r2, [r3]
005aee28: mov r1, #0
005aee2c: bl #0x5341ac
005aee30: add r3, sp, #0x9c
005aee34: mov ip, #1
005aee38: add r1, sp, #0x98
005aee3c: mov r2, #2
005aee40: mov r5, r0
005aee44: str ip, [sp]
005aee48: bl #0x5a0834
005aee4c: cmp r5, #0
005aee50: ldrne r3, [r5]
005aee54: addne r3, r3, #1
005aee58: strne r3, [r5]
005aee5c: ldr r0, [r4, #0x144]
005aee60: str r5, [r4, #0x144]
005aee64: cmp r0, #0
005aee68: beq #0x5aee84
005aee6c: ldr r3, [r0]
005aee70: sub r3, r3, #1
005aee74: cmp r3, #0
005aee78: str r3, [r0]
005aee7c: bne #0x5aee84
005aee80: bl #0x30e2b0
005aee84: ldr r5, [sp, #0x98]
005aee88: cmp r5, #0
005aee8c: beq #0x5aeea4
005aee90: ldr r3, [r5]
005aee94: sub r3, r3, #1
005aee98: cmp r3, #0
005aee9c: str r3, [r5]
005aeea0: beq #0x5aef58
005aeea4: mov r5, r4
005aeea8: add r6, r4, #0xc
005aeeac: ldr r2, [r4, #0x144]
005aeeb0: cmp r2, #0
005aeeb4: ldrne r3, [r2]
005aeeb8: addne r3, r3, #1
005aeebc: strne r3, [r2]
005aeec0: ldr r3, [r5, #0x148]
005aeec4: str r2, [r5, #0x148]
005aeec8: add r5, r5, #4
005aeecc: cmp r3, #0
005aeed0: mov r0, r3
005aeed4: beq #0x5aeef0
005aeed8: ldr r2, [r3]
005aeedc: sub r2, r2, #1
005aeee0: cmp r2, #0
005aeee4: str r2, [r3]
005aeee8: bne #0x5aeef0
005aeeec: bl #0x30e2b0
005aeef0: cmp r5, r6
005aeef4: bne #0x5aeeac
005aeef8: mov r0, r4
005aeefc: ldr r3, [r4]
005aef00: mov r1, #0x2000
005aef04: mov r2, #1
005aef08: mov lr, pc
005aef0c: ldr pc, [r3, #0xa0]
005aef10: ldr r0, [sp, #0xa4]
005aef14: cmp r0, #0
005aef18: beq #0x5ae7a0
005aef1c: bl #0x31d584
005aef20: b #0x5ae7a0
005aef24: tst r2, #2
005aef28: beq #0x5aed78
005aef2c: b #0x5aed58
005aef30: mov r0, r6
005aef34: bl #0x5a0a1c
005aef38: mov r0, r6
005aef3c: bl #0x30e2b0
005aef40: b #0x5aeddc
005aef44: mov r0, r6
005aef48: bl #0x5a0a1c
005aef4c: mov r0, r6
005aef50: bl #0x30e2b0
005aef54: b #0x5aedbc
005aef58: mov r0, r5
005aef5c: bl #0x5a0a1c
005aef60: mov r0, r5
005aef64: bl #0x30e2b0
005aef68: b #0x5aeea4
005aef6c: mov r2, r5
005aef70: add r0, r4, #0xbc
005aef74: bl #0x5ab9a4
005aef78: b #0x5ae660
005aef7c: mov r0, r6
005aef80: add r2, sp, #0x88
005aef84: bl #0x5ab808
005aef88: b #0x5aebc4
005aef8c: mov r0, r6
005aef90: add r2, sp, #0x84
005aef94: bl #0x5ab808
005aef98: b #0x5aec58

_ZNK6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture16updateParametersEv
005afd40: push {r4, r5, r6, r7, lr}
005afd44: mov r4, r0
005afd48: ldr r0, [pc, #0x278]
005afd4c: ldr r2, [r4, #0x38]
005afd50: ldrh r3, [r4, #0x40]
005afd54: ldr r1, [pc, #0x270]
005afd58: add r0, pc, r0
005afd5c: add r0, r0, #0xa4
005afd60: tst r3, #4
005afd64: and ip, r2, #3
005afd68: sub sp, sp, #0xc
005afd6c: ldr r5, [r0, ip, lsl #2]
005afd70: add r1, pc, r1
005afd74: beq #0x5afda8
005afd78: ldrb r3, [r4, #0x3f]
005afd7c: tst r3, #2
005afd80: bne #0x5afe7c
005afd84: ubfx r2, r2, #0xc, #3
005afd88: ldr r3, [pc, #0x240]
005afd8c: mov r0, r5
005afd90: movw r1, #0x2801
005afd94: add r3, pc, r3
005afd98: add r3, r3, #0xb4
005afd9c: ldr r2, [r3, r2, lsl #2]
005afda0: bl #0x30e910
005afda4: ldrh r3, [r4, #0x40]
005afda8: tst r3, #8
005afdac: bne #0x5aff9c
005afdb0: tst r3, #0x10
005afdb4: bne #0x5aff70
005afdb8: tst r3, #0x20
005afdbc: bne #0x5aff44
005afdc0: tst r3, #0x40
005afdc4: beq #0x5afe74
005afdc8: ldr r2, [r4, #0x34]
005afdcc: ldr r1, [r2, #0x9c]
005afdd0: tst r1, #0x80
005afdd4: bne #0x5afe4c
005afdd8: tst r3, #0x80
005afddc: beq #0x5afdec
005afde0: ldr r1, [r2, #0x9c]
005afde4: tst r1, #0x20000
005afde8: bne #0x5afef8
005afdec: ldr r2, [r2, #0x7ec]
005afdf0: tst r2, #0x80000
005afdf4: beq #0x5afe34
005afdf8: tst r3, #0x400
005afdfc: beq #0x5afe34
005afe00: ldr r3, [r4, #0x38]
005afe04: ubfx r3, r3, #0xc, #3
005afe08: cmp r3, #3
005afe0c: bgt #0x5aff30
005afe10: mov r1, #0x3f000000
005afe14: ldr r0, [r4, #0x50]
005afe18: bl #0x30eba4
005afe1c: bl #0x30e4cc
005afe20: mov r2, r0
005afe24: mov r0, r5
005afe28: movw r1, #0x813d
005afe2c: bl #0x30e910
005afe30: ldrh r3, [r4, #0x40]
005afe34: movw r2, #0xe003
005afe38: movt r2, #0
005afe3c: and r2, r3, r2
005afe40: strh r2, [r4, #0x40]
005afe44: add sp, sp, #0xc
005afe48: pop {r4, r5, r6, r7, pc}
005afe4c: ldr r3, [pc, #0x180]
005afe50: ldr r2, [r4, #0x38]
005afe54: mov r0, r5
005afe58: add r3, pc, r3
005afe5c: add r3, r3, #0xcc
005afe60: ubfx r2, r2, #0x15, #3
005afe64: ldr r2, [r3, r2, lsl #2]
005afe68: movw r1, #0x2803
005afe6c: bl #0x30e910
005afe70: ldrh r3, [r4, #0x40]
005afe74: ldr r2, [r4, #0x34]
005afe78: b #0x5afdd8
005afe7c: ldr r0, [pc, #0x154]
005afe80: ubfx r3, r2, #4, #6
005afe84: ldr r1, [r1, r0]
005afe88: mov r0, #0x28
005afe8c: mul r3, r0, r3
005afe90: ldr r3, [r1, r3]
005afe94: tst r3, #8
005afe98: beq #0x5afd84
005afe9c: mov r0, #0
005afea0: ldr r6, [r4, #0x1c]
005afea4: bl #0x5fdaa0
005afea8: ldr r1, [pc, #0x12c]
005afeac: ldr r3, [pc, #0x12c]
005afeb0: ldr ip, [r0]
005afeb4: mov r2, r6
005afeb8: add r3, pc, r3
005afebc: add r1, pc, r1
005afec0: mov r0, #3
005afec4: str ip, [sp]
005afec8: bl #0x60b034
005afecc: ldr r3, [r4, #0x38]
005afed0: ubfx r2, r3, #0xc, #3
005afed4: cmp r2, #0
005afed8: beq #0x5afd88
005afedc: ldrh r2, [r4, #0x40]
005afee0: bic r3, r3, #0x7000
005afee4: str r3, [r4, #0x38]
005afee8: orr r3, r2, #4
005afeec: strh r3, [r4, #0x40]
005afef0: mov r2, #0
005afef4: b #0x5afd88
005afef8: ldr r6, [r2, #0x4a8]
005afefc: ldr r7, [r4, #0x44]
005aff00: mov r0, r6
005aff04: mov r1, r7
005aff08: bl #0x30e70c
005aff0c: cmp r0, #0
005aff10: moveq r6, r7
005aff14: mov r2, r6
005aff18: mov r0, r5
005aff1c: movw r1, #0x84fe
005aff20: bl #0x30e25c
005aff24: ldr r2, [r4, #0x34]
005aff28: ldrh r3, [r4, #0x40]
005aff2c: b #0x5afdec
005aff30: ldr r0, [r4, #0x50]
005aff34: bl #0x30e514
005aff38: bl #0x30e4cc
005aff3c: mov r2, r0
005aff40: b #0x5afe24
005aff44: ldr r3, [pc, #0x98]
005aff48: ldr r2, [r4, #0x38]
005aff4c: mov r0, r5
005aff50: add r3, pc, r3
005aff54: add r3, r3, #0xcc
005aff58: ubfx r2, r2, #0x15, #3
005aff5c: ldr r2, [r3, r2, lsl #2]
005aff60: movw r1, #0x2803
005aff64: bl #0x30e910
005aff68: ldrh r3, [r4, #0x40]
005aff6c: b #0x5afdc0
005aff70: ldr r3, [pc, #0x70]
005aff74: ldr r2, [r4, #0x38]
005aff78: mov r0, r5
005aff7c: add r3, pc, r3
005aff80: add r3, r3, #0xcc
005aff84: ubfx r2, r2, #0x12, #3
005aff88: ldr r2, [r3, r2, lsl #2]
005aff8c: movw r1, #0x2802
005aff90: bl #0x30e910
005aff94: ldrh r3, [r4, #0x40]
005aff98: b #0x5afdb8
005aff9c: ldr r3, [pc, #0x48]
005affa0: ldr r2, [r4, #0x38]
005affa4: mov r0, r5
005affa8: add r3, pc, r3
005affac: add r3, r3, #0xb4
005affb0: ubfx r2, r2, #0xf, #3
005affb4: ldr r2, [r3, r2, lsl #2]
005affb8: mov r1, #0x2800
005affbc: bl #0x30e910
005affc0: ldrh r3, [r4, #0x40]
005affc4: b #0x5afdb0
005affc8: ldrsbteq r0, [r3], -ip
005affcc: eorseq r4, lr, r0, lsr #26
005affd0: eorseq r0, r3, r0, lsr #5
005affd4: ldrsbteq r0, [r3], -ip
005affd8: andeq r1, r0, r4, lsr pc
005affdc: eorseq r0, r3, r4, asr #9
005affe0: eorseq r0, r3, r8, lsr #10
005affe4: eorseq r0, r3, r4, ror #1
005affe8: ldrhteq r0, [r3], -r8
005affec: eorseq r0, r3, ip, lsl #1

_ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvPS8_bb
006021f8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006021fc: ldr sb, [pc, #0x224]
00602200: ldr ip, [pc, #0x224]
00602204: mov r5, #0
00602208: add sb, pc, sb
0060220c: ldr ip, [sb, ip]
00602210: str r5, [r0, #4]
00602214: str r5, [r0, #8]
00602218: add ip, ip, #8
0060221c: str ip, [r0]
00602220: str r5, [r0, #0xc]
00602224: ldr lr, [r2]
00602228: sub sp, sp, #0xc
0060222c: ldrb r6, [sp, #0x34]
00602230: ldrb ip, [sp, #0x38]
00602234: str lr, [r0, #0x10]
00602238: ldr r2, [r2, #4]
0060223c: mov r4, r0
00602240: strb ip, [r0, #0x29]
00602244: str r2, [r0, #0x14]
00602248: str r5, [r0, #0x18]
0060224c: str r5, [r0, #0x1c]
00602250: cmp r6, r5
00602254: str r1, [r4, #0x20]
00602258: str r5, [r0, #0x24]
0060225c: strb r5, [r0, #0x28]
00602260: mov r7, r1
00602264: mov r8, r3
00602268: ldr sl, [sp, #0x30]
0060226c: bne #0x60239c
00602270: cmp sl, #0
00602274: beq #0x602404
00602278: mov r3, #1
0060227c: mov r1, r3
00602280: strb r3, [r0, #0x28]
00602284: bl #0x601988
00602288: ldr r3, [r4, #0x14]
0060228c: ldr r2, [r4, #0x18]
00602290: mov r1, r8
00602294: ldr r0, [r4, #8]
00602298: mul r2, r2, r3
0060229c: bl #0x30e868
006022a0: ldr r3, [pc, #0x188]
006022a4: mov fp, #0x28
006022a8: mov r5, r6
006022ac: str r6, [r4, #0x24]
006022b0: mul fp, fp, r7
006022b4: ldr r6, [r4, #0x10]
006022b8: ldr r7, [r4, #0x14]
006022bc: str r3, [sp, #4]
006022c0: ldr r1, [sl, r5]
006022c4: mov r8, r5
006022c8: cmp r1, #0
006022cc: cmpeq r6, #1
006022d0: moveq r3, #0
006022d4: movne r3, #1
006022d8: beq #0x602334
006022dc: cmp r6, #1
006022e0: lsrhi r6, r6, #1
006022e4: ldr r3, [sp, #4]
006022e8: cmp r7, #1
006022ec: lsrhi r7, r7, #1
006022f0: ldr r2, [sb, r3]
006022f4: ldr r3, [r4, #0xc]
006022f8: add r8, r8, #1
006022fc: add r2, r2, fp
00602300: ldrb r2, [r2, #0x16]
00602304: ldr r0, [r3, r5]
00602308: add r5, r5, #4
0060230c: mul r2, r2, r6
00602310: mul r2, r7, r2
00602314: lsr r2, r2, #3
00602318: bl #0x30e868
0060231c: ldr r1, [sl, r5]
00602320: cmp r1, #0
00602324: cmpeq r6, #1
00602328: moveq r3, #0
0060232c: movne r3, #1
00602330: bne #0x6022dc
00602334: cmp r7, #1
00602338: bne #0x6022e4
0060233c: ldr r2, [r4, #0x24]
00602340: str r8, [r4, #0x24]
00602344: cmp r2, r8
00602348: bls #0x602390
0060234c: ldr r2, [r4, #0xc]
00602350: mov r6, r3
00602354: ldr r0, [r2, r5]
00602358: str r3, [sp]
0060235c: bl #0x30e2b0
00602360: ldr r2, [r4, #0xc]
00602364: ldr r3, [sp]
00602368: str r3, [r2, r5]
0060236c: ldr r3, [r4, #0xc]
00602370: ldr r0, [r3, r5]
00602374: bl #0x30e2b0
00602378: ldr r3, [r4, #0xc]
0060237c: str r6, [r3, r5]
00602380: b #0x60236c
00602384: cmp r3, #1
00602388: bne #0x6023e8
0060238c: strb r3, [r4, #0x28]
00602390: mov r0, r4
00602394: add sp, sp, #0xc
00602398: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0060239c: movw r3, #0xf00d
006023a0: movt r3, #0xbad
006023a4: str r3, [r0, #0xc]
006023a8: str r3, [r0, #8]
006023ac: mov r1, #1
006023b0: bl #0x601988
006023b4: cmp sl, r5
006023b8: str r8, [r4, #8]
006023bc: str sl, [r4, #0xc]
006023c0: str r5, [r4, #0x24]
006023c4: beq #0x602390
006023c8: ldr r2, [r4, #0x10]
006023cc: ldr r3, [r4, #0x14]
006023d0: ldr r1, [sl, r5]
006023d4: cmp r1, #0
006023d8: cmpeq r2, #1
006023dc: beq #0x602384
006023e0: cmp r2, #1
006023e4: lsrhi r2, r2, #1
006023e8: ldr r1, [r4, #0x24]
006023ec: cmp r3, #1
006023f0: lsrhi r3, r3, #1
006023f4: add r1, r1, #1
006023f8: add r5, r5, #4
006023fc: str r1, [r4, #0x24]
00602400: b #0x6023d0
00602404: mov r1, #1
00602408: bl #0x601988
0060240c: ldr r3, [r4, #0x14]
00602410: ldr r2, [r4, #0x18]
00602414: mov r1, r8
00602418: ldr r0, [r4, #8]
0060241c: mul r2, r2, r3
00602420: bl #0x30e868
00602424: b #0x602390
00602428: eorseq r2, sb, r8, lsl #17
0060242c: andeq r0, r0, r0, lsl r6
00602430: andeq r1, r0, r4, lsr pc

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

_ZN6glitch5video6CImageC2ENS0_14E_PIXEL_FORMATERKN5boost13intrusive_ptrIS1_EE
00601eb0: ldr r3, [pc, #0x168]
00601eb4: ldr ip, [pc, #0x168]
00601eb8: push {r4, r5, r6, r7, lr}
00601ebc: add r3, pc, r3
00601ec0: ldr ip, [r3, ip]
00601ec4: mov r5, #0
00601ec8: mov r6, #1
00601ecc: add ip, ip, #8
00601ed0: str ip, [r0]
00601ed4: str r1, [r0, #0x20]
00601ed8: str r5, [r0, #4]
00601edc: str r5, [r0, #8]
00601ee0: str r5, [r0, #0xc]
00601ee4: str r5, [r0, #0x10]
00601ee8: str r5, [r0, #0x14]
00601eec: str r5, [r0, #0x18]
00601ef0: str r5, [r0, #0x1c]
00601ef4: str r5, [r0, #0x24]
00601ef8: strb r5, [r0, #0x28]
00601efc: strb r6, [r0, #0x29]
00601f00: mov r7, r2
00601f04: ldr r2, [r2]
00601f08: sub sp, sp, #0x1c
00601f0c: mov r4, r0
00601f10: cmp r2, r5
00601f14: beq #0x602014
00601f18: ldr r3, [r2, #0x10]
00601f1c: mov r1, r6
00601f20: str r3, [r0, #0x10]
00601f24: ldr r3, [r2, #0x14]
00601f28: str r3, [r0, #0x14]
00601f2c: ldr r3, [r7]
00601f30: ldrb r3, [r3, #0x28]
00601f34: strb r3, [r0, #0x28]
00601f38: bl #0x601988
00601f3c: ldr r1, [r7]
00601f40: ldr r6, [r4, #0x18]
00601f44: ldr r7, [r4, #8]
00601f48: ldr lr, [r4, #0x10]
00601f4c: ldr ip, [r4, #0x14]
00601f50: ldr r2, [r1, #0x18]
00601f54: ldr r0, [r1, #0x20]
00601f58: ldr r3, [r4, #0x20]
00601f5c: ldr r1, [r1, #8]
00601f60: str r7, [sp]
00601f64: stmib sp, {r6, lr}
00601f68: str ip, [sp, #0xc]
00601f6c: str r5, [sp, #0x10]
00601f70: bl #0x5f95ac
00601f74: cmp r0, r5
00601f78: bne #0x602014
00601f7c: ldr r0, [r4, #8]
00601f80: mov r3, #0x27
00601f84: str r3, [r4, #0x20]
00601f88: cmp r0, r5
00601f8c: beq #0x601f94
00601f90: bl #0x30e0b8
00601f94: ldr r3, [r4, #0xc]
00601f98: mov r6, #0
00601f9c: str r6, [r4, #8]
00601fa0: cmp r3, r6
00601fa4: beq #0x602000
00601fa8: ldrb r2, [r4, #0x28]
00601fac: cmp r2, r6
00601fb0: beq #0x601ff8
00601fb4: ldr r0, [r3]
00601fb8: cmp r0, r6
00601fbc: beq #0x601ff8
00601fc0: mov r5, #4
00601fc4: mov r7, r6
00601fc8: bl #0x30e0b8
00601fcc: ldr r3, [r4, #0xc]
00601fd0: add r2, r5, #4
00601fd4: str r7, [r3, r6]
00601fd8: ldr r3, [r4, #0xc]
00601fdc: mov r6, r5
00601fe0: ldr r0, [r3, r5]
00601fe4: mov r5, r2
00601fe8: cmp r0, #0
00601fec: bne #0x601fc8
00601ff0: cmp r3, #0
00601ff4: beq #0x602000
00601ff8: mov r0, r3
00601ffc: bl #0x30e0b8
00602000: mov r3, #0
00602004: str r3, [r4, #0x1c]
00602008: strb r3, [r4, #0x29]
0060200c: str r3, [r4, #0x14]
00602010: str r3, [r4, #0x10]
00602014: mov r0, r4
00602018: add sp, sp, #0x1c
0060201c: pop {r4, r5, r6, r7, pc}
00602020: ldrsbteq r2, [sb], -r4
00602024: andeq r0, r0, r0, lsl r6

_ZN6glitch5video16COpenGLES2DriverC1EPNS_7IDeviceE
005b5d78: push {r4, r5, r6, lr}
005b5d7c: ldr r4, [pc, #0x20]
005b5d80: mov r5, r0
005b5d84: bl #0x5b5c40
005b5d88: ldr r3, [pc, #0x18]
005b5d8c: add r4, pc, r4
005b5d90: mov r0, r5
005b5d94: ldr r3, [r4, r3]
005b5d98: add r3, r3, #8
005b5d9c: str r3, [r5]
005b5da0: pop {r4, r5, r6, pc}
005b5da4: eorseq lr, sp, r4, lsl #26
005b5da8: andeq r2, r0, ip, ror #28

_ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvbb
00602670: push {r4, r5, r6, r7, r8, lr}
00602674: ldr lr, [pc, #0xa8]
00602678: ldr r5, [pc, #0xa8]
0060267c: mov ip, #0
00602680: add lr, pc, lr
00602684: ldr r5, [lr, r5]
00602688: str ip, [r0, #4]
0060268c: str ip, [r0, #8]
00602690: add r5, r5, #8
00602694: str r5, [r0]
00602698: str ip, [r0, #0xc]
0060269c: ldr r7, [r2]
006026a0: ldrb r6, [sp, #0x18]
006026a4: ldrb r5, [sp, #0x1c]
006026a8: str r7, [r0, #0x10]
006026ac: ldr r2, [r2, #4]
006026b0: cmp r6, ip
006026b4: strb r5, [r0, #0x29]
006026b8: mov r4, r0
006026bc: str r1, [r0, #0x20]
006026c0: str r2, [r0, #0x14]
006026c4: strb ip, [r0, #0x28]
006026c8: mov r5, r3
006026cc: str ip, [r0, #0x18]
006026d0: str ip, [r0, #0x1c]
006026d4: str ip, [r0, #0x24]
006026d8: bne #0x602704
006026dc: mov r1, #1
006026e0: bl #0x601988
006026e4: ldr r3, [r4, #0x14]
006026e8: ldr r2, [r4, #0x18]
006026ec: mov r1, r5
006026f0: ldr r0, [r4, #8]
006026f4: mul r2, r2, r3
006026f8: bl #0x30e868
006026fc: mov r0, r4
00602700: pop {r4, r5, r6, r7, r8, pc}
00602704: movw r3, #0xf00d
00602708: movt r3, #0xbad
0060270c: str r3, [r0, #8]
00602710: mov r1, #1
00602714: bl #0x601988
00602718: str r5, [r4, #8]
0060271c: mov r0, r4
00602720: pop {r4, r5, r6, r7, r8, pc}
00602724: eorseq r2, sb, r0, lsl r4
00602728: andeq r0, r0, r0, lsl r6

_ZN6glitch5video6CImageC2ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvPS8_bb
00602434: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00602438: ldr sb, [pc, #0x224]
0060243c: ldr ip, [pc, #0x224]
00602440: mov r5, #0
00602444: add sb, pc, sb
00602448: ldr ip, [sb, ip]
0060244c: str r5, [r0, #4]
00602450: str r5, [r0, #8]
00602454: add ip, ip, #8
00602458: str ip, [r0]
0060245c: str r5, [r0, #0xc]
00602460: ldr lr, [r2]
00602464: sub sp, sp, #0xc
00602468: ldrb r6, [sp, #0x34]
0060246c: ldrb ip, [sp, #0x38]
00602470: str lr, [r0, #0x10]
00602474: ldr r2, [r2, #4]
00602478: mov r4, r0
0060247c: strb ip, [r0, #0x29]
00602480: str r2, [r0, #0x14]
00602484: str r5, [r0, #0x18]
00602488: str r5, [r0, #0x1c]
0060248c: cmp r6, r5
00602490: str r1, [r4, #0x20]
00602494: str r5, [r0, #0x24]
00602498: strb r5, [r0, #0x28]
0060249c: mov r7, r1
006024a0: mov r8, r3
006024a4: ldr sl, [sp, #0x30]
006024a8: bne #0x6025d8
006024ac: cmp sl, #0
006024b0: beq #0x602640
006024b4: mov r3, #1
006024b8: mov r1, r3
006024bc: strb r3, [r0, #0x28]
006024c0: bl #0x601988
006024c4: ldr r3, [r4, #0x14]
006024c8: ldr r2, [r4, #0x18]
006024cc: mov r1, r8
006024d0: ldr r0, [r4, #8]
006024d4: mul r2, r2, r3
006024d8: bl #0x30e868
006024dc: ldr r3, [pc, #0x188]
006024e0: mov fp, #0x28
006024e4: mov r5, r6
006024e8: str r6, [r4, #0x24]
006024ec: mul fp, fp, r7
006024f0: ldr r6, [r4, #0x10]
006024f4: ldr r7, [r4, #0x14]
006024f8: str r3, [sp, #4]
006024fc: ldr r1, [sl, r5]
00602500: mov r8, r5
00602504: cmp r1, #0
00602508: cmpeq r6, #1
0060250c: moveq r3, #0
00602510: movne r3, #1
00602514: beq #0x602570
00602518: cmp r6, #1
0060251c: lsrhi r6, r6, #1
00602520: ldr r3, [sp, #4]
00602524: cmp r7, #1
00602528: lsrhi r7, r7, #1
0060252c: ldr r2, [sb, r3]
00602530: ldr r3, [r4, #0xc]
00602534: add r8, r8, #1
00602538: add r2, r2, fp
0060253c: ldrb r2, [r2, #0x16]
00602540: ldr r0, [r3, r5]
00602544: add r5, r5, #4
00602548: mul r2, r2, r6
0060254c: mul r2, r7, r2
00602550: lsr r2, r2, #3
00602554: bl #0x30e868
00602558: ldr r1, [sl, r5]
0060255c: cmp r1, #0
00602560: cmpeq r6, #1
00602564: moveq r3, #0
00602568: movne r3, #1
0060256c: bne #0x602518
00602570: cmp r7, #1
00602574: bne #0x602520
00602578: ldr r2, [r4, #0x24]
0060257c: str r8, [r4, #0x24]
00602580: cmp r2, r8
00602584: bls #0x6025cc
00602588: ldr r2, [r4, #0xc]
0060258c: mov r6, r3
00602590: ldr r0, [r2, r5]
00602594: str r3, [sp]
00602598: bl #0x30e2b0
0060259c: ldr r2, [r4, #0xc]
006025a0: ldr r3, [sp]
006025a4: str r3, [r2, r5]
006025a8: ldr r3, [r4, #0xc]
006025ac: ldr r0, [r3, r5]
006025b0: bl #0x30e2b0
006025b4: ldr r3, [r4, #0xc]
006025b8: str r6, [r3, r5]
006025bc: b #0x6025a8
006025c0: cmp r3, #1
006025c4: bne #0x602624
006025c8: strb r3, [r4, #0x28]
006025cc: mov r0, r4
006025d0: add sp, sp, #0xc
006025d4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006025d8: movw r3, #0xf00d
006025dc: movt r3, #0xbad
006025e0: str r3, [r0, #0xc]
006025e4: str r3, [r0, #8]
006025e8: mov r1, #1
006025ec: bl #0x601988
006025f0: cmp sl, r5
006025f4: str r8, [r4, #8]
006025f8: str sl, [r4, #0xc]
006025fc: str r5, [r4, #0x24]
00602600: beq #0x6025cc
00602604: ldr r2, [r4, #0x10]
00602608: ldr r3, [r4, #0x14]
0060260c: ldr r1, [sl, r5]
00602610: cmp r1, #0
00602614: cmpeq r2, #1
00602618: beq #0x6025c0
0060261c: cmp r2, #1
00602620: lsrhi r2, r2, #1
00602624: ldr r1, [r4, #0x24]
00602628: cmp r3, #1
0060262c: lsrhi r3, r3, #1
00602630: add r1, r1, #1
00602634: add r5, r5, #4
00602638: str r1, [r4, #0x24]
0060263c: b #0x60260c
00602640: mov r1, #1
00602644: bl #0x601988
00602648: ldr r3, [r4, #0x14]
0060264c: ldr r2, [r4, #0x18]
00602650: mov r1, r8
00602654: ldr r0, [r4, #8]
00602658: mul r2, r2, r3
0060265c: bl #0x30e868
00602660: b #0x6025cc
00602664: eorseq r2, sb, ip, asr #12
00602668: andeq r0, r0, r0, lsl r6
0060266c: andeq r1, r0, r4, lsr pc

_ZNK6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture6updateEb
005b044c: push {r4, r5, r6, lr}
005b0450: ldrh r3, [r0, #0x40]
005b0454: mov r4, r0
005b0458: mov r5, r1
005b045c: bic r2, r3, #3
005b0460: lsl r2, r2, #0x13
005b0464: lsr r2, r2, #0x13
005b0468: cmp r2, #0
005b046c: bne #0x5b048c
005b0470: ands r0, r3, #1
005b0474: bne #0x5b047c
005b0478: pop {r4, r5, r6, pc}
005b047c: mov r0, r4
005b0480: mov r1, r5
005b0484: pop {r4, r5, r6, lr}
005b0488: b #0x5afff0
005b048c: bl #0x5afd40
005b0490: ldrh r3, [r4, #0x40]
005b0494: b #0x5b0470

_ZN6glitch5video18ICodeShaderManagerC1Ev
006e0ca4: push {r4, r5, r6, lr}
006e0ca8: ldr r5, [pc, #0x38]
006e0cac: mov r4, r0
006e0cb0: bl #0x5e5770
006e0cb4: ldr r3, [pc, #0x30]
006e0cb8: add r5, pc, r5
006e0cbc: mov r0, r4
006e0cc0: ldr r3, [r5, r3]
006e0cc4: add r3, r3, #8
006e0cc8: str r3, [r0], #0x54
006e0ccc: bl #0x6e0804
006e0cd0: mov r3, #0
006e0cd4: str r3, [r4, #0x7c]
006e0cd8: mvn r3, #0
006e0cdc: str r3, [r4, #0x80]
006e0ce0: mov r0, r4
006e0ce4: pop {r4, r5, r6, pc}

_ZN6glitch5video6CImageC2ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvbb
0060272c: push {r4, r5, r6, r7, r8, lr}
00602730: ldr lr, [pc, #0xa8]
00602734: ldr r5, [pc, #0xa8]
00602738: mov ip, #0
0060273c: add lr, pc, lr
00602740: ldr r5, [lr, r5]
00602744: str ip, [r0, #4]
00602748: str ip, [r0, #8]
0060274c: add r5, r5, #8
00602750: str r5, [r0]
00602754: str ip, [r0, #0xc]
00602758: ldr r7, [r2]
0060275c: ldrb r6, [sp, #0x18]
00602760: ldrb r5, [sp, #0x1c]
00602764: str r7, [r0, #0x10]
00602768: ldr r2, [r2, #4]
0060276c: cmp r6, ip
00602770: strb r5, [r0, #0x29]
00602774: mov r4, r0
00602778: str r1, [r0, #0x20]
0060277c: str r2, [r0, #0x14]
00602780: strb ip, [r0, #0x28]
00602784: mov r5, r3
00602788: str ip, [r0, #0x18]
0060278c: str ip, [r0, #0x1c]
00602790: str ip, [r0, #0x24]
00602794: bne #0x6027c0
00602798: mov r1, #1
0060279c: bl #0x601988
006027a0: ldr r3, [r4, #0x14]
006027a4: ldr r2, [r4, #0x18]
006027a8: mov r1, r5
006027ac: ldr r0, [r4, #8]
006027b0: mul r2, r2, r3
006027b4: bl #0x30e868
006027b8: mov r0, r4
006027bc: pop {r4, r5, r6, r7, r8, pc}
006027c0: movw r3, #0xf00d
006027c4: movt r3, #0xbad
006027c8: str r3, [r0, #8]
006027cc: mov r1, #1
006027d0: bl #0x601988
006027d4: str r5, [r4, #8]
006027d8: mov r0, r4
006027dc: pop {r4, r5, r6, r7, r8, pc}
006027e0: eorseq r2, sb, r4, asr r3
006027e4: andeq r0, r0, r0, lsl r6

_ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17genericDriverInitERKNS_4core11dimension2dIiEEb
005b3afc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005b3b00: ldr r5, [pc, #0xfb8]
005b3b04: ldr r2, [pc, #0xfb8]
005b3b08: sub sp, sp, #0x44
005b3b0c: add r5, pc, r5
005b3b10: ldr r3, [r5, r2]
005b3b14: mov r4, r0
005b3b18: movw r0, #0x1f02
005b3b1c: ldr r3, [r3]
005b3b20: stmib sp, {r1, r2}
005b3b24: str r3, [sp, #0x3c]
005b3b28: bl #0x30e280
005b3b2c: ldr r3, [pc, #0xf94]
005b3b30: ldr r3, [r5, r3]
005b3b34: ldr r2, [r3]
005b3b38: ldrsb r3, [r0]
005b3b3c: cmn r3, #1
005b3b40: beq #0x5b3b54
005b3b44: uxtab r3, r2, r3
005b3b48: ldrb r3, [r3, #1]
005b3b4c: tst r3, #4
005b3b50: bne #0x5b3b5c
005b3b54: add r0, r0, #1
005b3b58: b #0x5b3b38
005b3b5c: ldr r1, [pc, #0xf68]
005b3b60: add r3, sp, #0x1c
005b3b64: add r2, sp, #0x20
005b3b68: mov ip, #0
005b3b6c: add r1, pc, r1
005b3b70: str ip, [sp, #0x1c]
005b3b74: str ip, [sp, #0x20]
005b3b78: bl #0x30e274
005b3b7c: cmp r0, #0
005b3b80: ldrgt r3, [sp, #0x20]
005b3b84: movgt r2, #0x64
005b3b88: ldrle r3, [sp, #0x20]
005b3b8c: mulgt r3, r2, r3
005b3b90: ldr r2, [sp, #0x1c]
005b3b94: strgt r3, [sp, #0x20]
005b3b98: add r3, r3, r2
005b3b9c: cmp r3, #0x64
005b3ba0: str r3, [r4, #0x4a4]
005b3ba4: bls #0x5b4b5c
005b3ba8: ldr r0, [pc, #0xf20]
005b3bac: mov r1, #1
005b3bb0: add r0, pc, r0
005b3bb4: bl #0x60aca0
005b3bb8: movw r0, #0x1f03
005b3bbc: bl #0x30e280
005b3bc0: mov r1, r0
005b3bc4: mov r0, r4
005b3bc8: bl #0x6dd734
005b3bcc: ldr r3, [r4, #0x7d0]
005b3bd0: tst r3, #0x20
005b3bd4: bne #0x5b4b8c
005b3bd8: add r1, sp, #0x40
005b3bdc: mov r3, #0
005b3be0: str r3, [r1, #-0x28]!
005b3be4: movw r0, #0x8872
005b3be8: bl #0x30e568
005b3bec: ldr r3, [sp, #0x18]
005b3bf0: ldr r1, [r4, #0x9c]
005b3bf4: movw r0, #0xd57
005b3bf8: cmp r3, #8
005b3bfc: movhs r3, #8
005b3c00: cmp r3, #1
005b3c04: orr r2, r1, #1
005b3c08: str r2, [r4, #0x9c]
005b3c0c: orrhi r2, r1, #3
005b3c10: strhi r2, [r4, #0x9c]
005b3c14: orr r2, r2, #0x800
005b3c18: str r3, [r4, #0x4c]
005b3c1c: orr r2, r2, #4
005b3c20: add r1, sp, #0x40
005b3c24: mov r3, #0
005b3c28: str r3, [r1, #-0x2c]!
005b3c2c: str r2, [r4, #0x9c]
005b3c30: bl #0x30e568
005b3c34: ldr r2, [r4, #0x9c]
005b3c38: ldr r0, [r4, #0x7b8]
005b3c3c: orr r2, r2, #0x18
005b3c40: tst r0, #0x4000000
005b3c44: str r2, [r4, #0x9c]
005b3c48: beq #0x5b4b3c
005b3c4c: ldr r3, [r4, #0x7e8]
005b3c50: ldr r1, [r4, #0x7ec]
005b3c54: orr r2, r2, #0x20
005b3c58: str r2, [r4, #0x9c]
005b3c5c: ldr ip, [r4, #0x7c0]
005b3c60: tst ip, #0x800000
005b3c64: beq #0x5b4b2c
005b3c68: orr r2, r2, #0x80
005b3c6c: str r2, [r4, #0x9c]
005b3c70: orr r2, r2, #0x100
005b3c74: tst r0, #0x40000000
005b3c78: str r2, [r4, #0x9c]
005b3c7c: beq #0x5b4b1c
005b3c80: orr r2, r2, #0x200
005b3c84: str r2, [r4, #0x9c]
005b3c88: ldr lr, [r4, #0x7d0]
005b3c8c: orr r0, r2, #0x1000
005b3c90: str r0, [r4, #0x9c]
005b3c94: tst lr, #0x20
005b3c98: orrne r0, r2, #0x21000
005b3c9c: strne r0, [r4, #0x9c]
005b3ca0: mov r2, #1
005b3ca4: orr r0, r0, #0x12c0000
005b3ca8: tst r3, #0x100
005b3cac: str r0, [r4, #0x9c]
005b3cb0: strb r2, [r4, #0x4a1]
005b3cb4: bne #0x5b491c
005b3cb8: tst r1, #0x200000
005b3cbc: moveq r6, #5
005b3cc0: bne #0x5b491c
005b3cc4: movw ip, #0x4ae
005b3cc8: strh r6, [r4, ip]
005b3ccc: mov r0, #0
005b3cd0: movw ip, #0x4ac
005b3cd4: strh r0, [r4, ip]
005b3cd8: movw r2, #0x1909
005b3cdc: movw ip, #0x1401
005b3ce0: tst r3, #0x100
005b3ce4: str r2, [r4, #0x4b4]
005b3ce8: str ip, [r4, #0x4b8]
005b3cec: str r0, [r4, #0x4bc]
005b3cf0: str r2, [r4, #0x4b0]
005b3cf4: bne #0x5b4914
005b3cf8: tst r1, #0x200000
005b3cfc: moveq r6, #5
005b3d00: bne #0x5b4914
005b3d04: movw ip, #0x4c2
005b3d08: strh r6, [r4, ip]
005b3d0c: mov r2, #0
005b3d10: mov ip, #0x4c0
005b3d14: strh r2, [r4, ip]
005b3d18: mov r6, #2
005b3d1c: movw ip, #0x4d4
005b3d20: strh r6, [r4, ip]
005b3d24: mov r7, #0xe
005b3d28: movw ip, #0x4d6
005b3d2c: strh r7, [r4, ip]
005b3d30: movw r0, #0x1906
005b3d34: movw ip, #0x1401
005b3d38: tst r3, #0x100
005b3d3c: str r0, [r4, #0x4dc]
005b3d40: str ip, [r4, #0x4e0]
005b3d44: str r2, [r4, #0x4e4]
005b3d48: str r2, [r4, #0x4c4]
005b3d4c: str r2, [r4, #0x4c8]
005b3d50: str r2, [r4, #0x4cc]
005b3d54: str r2, [r4, #0x4d0]
005b3d58: str r0, [r4, #0x4d8]
005b3d5c: bne #0x5b490c
005b3d60: tst r1, #0x200000
005b3d64: moveq ip, #7
005b3d68: bne #0x5b490c
005b3d6c: movw r0, #0x4ea
005b3d70: strh ip, [r4, r0]
005b3d74: mov r8, #4
005b3d78: movw r0, #0x4e8
005b3d7c: strh r8, [r4, r0]
005b3d80: mvn r2, #0
005b3d84: mov r0, #0
005b3d88: tst r3, #0x100
005b3d8c: str r2, [r4, #0x4f4]
005b3d90: str r0, [r4, #0x4f8]
005b3d94: str r2, [r4, #0x4ec]
005b3d98: str r2, [r4, #0x4f0]
005b3d9c: bne #0x5b4904
005b3da0: tst r1, #0x200000
005b3da4: moveq sl, #7
005b3da8: bne #0x5b4904
005b3dac: movw r8, #0x4fe
005b3db0: strh sl, [r4, r8]
005b3db4: movw r8, #0x4fc
005b3db8: mov sl, #4
005b3dbc: strh sl, [r4, r8]
005b3dc0: movw r7, #0x190a
005b3dc4: mov sl, #5
005b3dc8: mov r8, #0x510
005b3dcc: str r7, [r4, #0x504]
005b3dd0: strh sl, [r4, r8]
005b3dd4: movw r8, #0x512
005b3dd8: strh sl, [r4, r8]
005b3ddc: movw r8, #0x8363
005b3de0: str r8, [r4, #0x51c]
005b3de4: movw r8, #0x8d62
005b3de8: str r8, [r4, #0x520]
005b3dec: mov sl, #7
005b3df0: movw r8, #0x524
005b3df4: strh sl, [r4, r8]
005b3df8: movw r8, #0x526
005b3dfc: strh sl, [r4, r8]
005b3e00: movw r8, #0x538
005b3e04: strh sl, [r4, r8]
005b3e08: movw r8, #0x53a
005b3e0c: strh sl, [r4, r8]
005b3e10: movw r8, #0x8033
005b3e14: str r8, [r4, #0x544]
005b3e18: movw r8, #0x8056
005b3e1c: str r8, [r4, #0x548]
005b3e20: mov sl, #9
005b3e24: movw r8, #0x54c
005b3e28: strh sl, [r4, r8]
005b3e2c: movw r8, #0x54e
005b3e30: strh sl, [r4, r8]
005b3e34: mov r2, #0
005b3e38: mov r8, #0x560
005b3e3c: str r2, [r4, #0x55c]
005b3e40: strh sl, [r4, r8]
005b3e44: movw r8, #0x562
005b3e48: strh sl, [r4, r8]
005b3e4c: movw ip, #0x1908
005b3e50: movw r0, #0x1907
005b3e54: str r2, [r4, #0x50c]
005b3e58: str r2, [r4, #0x528]
005b3e5c: str r2, [r4, #0x52c]
005b3e60: str r2, [r4, #0x530]
005b3e64: str r2, [r4, #0x534]
005b3e68: str r2, [r4, #0x550]
005b3e6c: str r2, [r4, #0x554]
005b3e70: str r2, [r4, #0x558]
005b3e74: movw r6, #0x1401
005b3e78: movw r2, #0x8034
005b3e7c: str r7, [r4, #0x500]
005b3e80: str r6, [r4, #0x508]
005b3e84: str r0, [r4, #0x514]
005b3e88: str r0, [r4, #0x518]
005b3e8c: str ip, [r4, #0x53c]
005b3e90: str ip, [r4, #0x540]
005b3e94: str ip, [r4, #0x564]
005b3e98: ands sb, r3, #0x100
005b3e9c: str ip, [r4, #0x568]
005b3ea0: str r2, [r4, #0x56c]
005b3ea4: movw r2, #0x8057
005b3ea8: str r2, [r4, #0x570]
005b3eac: bne #0x5b49e4
005b3eb0: tst r1, #0x200000
005b3eb4: movne ip, #0xe
005b3eb8: moveq ip, #5
005b3ebc: movw r2, #0x576
005b3ec0: strh ip, [r4, r2]
005b3ec4: movw r2, #0x574
005b3ec8: mov ip, #0xa
005b3ecc: strh ip, [r4, r2]
005b3ed0: str r0, [r4, #0x57c]
005b3ed4: str r6, [r4, #0x580]
005b3ed8: str sb, [r4, #0x584]
005b3edc: str r0, [r4, #0x578]
005b3ee0: tst r3, #0x100
005b3ee4: movne ip, #0xa
005b3ee8: bne #0x5b3ef8
005b3eec: tst r1, #0x200000
005b3ef0: movne ip, #0xe
005b3ef4: moveq ip, #5
005b3ef8: movw r0, #0x58a
005b3efc: strh ip, [r4, r0]
005b3f00: mov r8, #0xa
005b3f04: movw r0, #0x588
005b3f08: strh r8, [r4, r0]
005b3f0c: mov sl, #0xe
005b3f10: movw r0, #0x59c
005b3f14: strh sl, [r4, r0]
005b3f18: mov r2, #0
005b3f1c: tst r3, #0x10000000
005b3f20: movw r0, #0x59e
005b3f24: strh sl, [r4, r0]
005b3f28: movne ip, #0xd
005b3f2c: str r2, [r4, #0x5ac]
005b3f30: str r2, [r4, #0x58c]
005b3f34: str r2, [r4, #0x590]
005b3f38: str r2, [r4, #0x594]
005b3f3c: str r2, [r4, #0x598]
005b3f40: str r2, [r4, #0x5a0]
005b3f44: str r2, [r4, #0x5a4]
005b3f48: str r2, [r4, #0x5a8]
005b3f4c: movne r2, #1
005b3f50: bne #0x5b3f68
005b3f54: and r2, r1, #0x440000
005b3f58: cmp r2, #0
005b3f5c: movne r2, #1
005b3f60: movne ip, #0xd
005b3f64: moveq ip, #0xe
005b3f68: tst r3, #0x100
005b3f6c: bne #0x5b48e4
005b3f70: tst r1, #0x200000
005b3f74: moveq r7, #6
005b3f78: bne #0x5b48e4
005b3f7c: tst r1, #0x40000
005b3f80: bne #0x5b48f0
005b3f84: cmp r2, #0
005b3f88: movwne r8, #0x80e1
005b3f8c: beq #0x5b4a10
005b3f90: movw r2, #0x80e1
005b3f94: movw r0, #0x1401
005b3f98: mov r6, #0x5b0
005b3f9c: strh ip, [r4, r6]
005b3fa0: movw ip, #0x5b2
005b3fa4: strh r7, [r4, ip]
005b3fa8: tst r3, #0x100
005b3fac: str r2, [r4, #0x5b8]
005b3fb0: mov r2, #0
005b3fb4: str r8, [r4, #0x5b4]
005b3fb8: str r0, [r4, #0x5bc]
005b3fbc: str r2, [r4, #0x5c0]
005b3fc0: beq #0x5b4a20
005b3fc4: movw r0, #0x5c4
005b3fc8: mov ip, #0xe
005b3fcc: strh ip, [r4, r0]
005b3fd0: add r0, r0, #2
005b3fd4: strh ip, [r4, r0]
005b3fd8: movw r0, #0x1401
005b3fdc: movw r2, #0x1908
005b3fe0: str r0, [r4, #0x5d0]
005b3fe4: movw r0, #0x8058
005b3fe8: str r2, [r4, #0x5cc]
005b3fec: str r0, [r4, #0x5d4]
005b3ff0: str r2, [r4, #0x5c8]
005b3ff4: tst r3, #0x100
005b3ff8: bne #0x5b4950
005b3ffc: tst r1, #0x200000
005b4000: moveq r7, #7
005b4004: bne #0x5b4950
005b4008: ands r0, r3, #0x200000
005b400c: movw ip, #0x5da
005b4010: strh r7, [r4, ip]
005b4014: mov r2, #0
005b4018: movne r6, #0x10
005b401c: moveq r6, #0xe
005b4020: movw ip, #0x5d8
005b4024: mov r7, #0xe
005b4028: tst r3, #0x100
005b402c: strh r7, [r4, ip]
005b4030: str r2, [r4, #0x5e8]
005b4034: str r2, [r4, #0x5dc]
005b4038: str r2, [r4, #0x5e0]
005b403c: str r2, [r4, #0x5e4]
005b4040: bne #0x5b493c
005b4044: tst r1, #0x200000
005b4048: moveq r7, #9
005b404c: bne #0x5b493c
005b4050: cmp r0, #0
005b4054: ldr ip, [r4, #0x7dc]
005b4058: movw r8, #0x5ec
005b405c: strh r6, [r4, r8]
005b4060: moveq r2, r0
005b4064: movwne r2, #0x1908
005b4068: movw r6, #0x5ee
005b406c: strh r7, [r4, r6]
005b4070: moveq r0, r2
005b4074: str r2, [r4, #0x5f0]
005b4078: movw r2, #0x1908
005b407c: movwne r0, #0x8368
005b4080: str r2, [r4, #0x5f4]
005b4084: tst ip, #2
005b4088: mov r2, #0
005b408c: str r0, [r4, #0x5f8]
005b4090: str r2, [r4, #0x5fc]
005b4094: bne #0x5b4930
005b4098: ands r6, lr, #0x10000
005b409c: moveq r7, #5
005b40a0: bne #0x5b4930
005b40a4: mov r0, #0x600
005b40a8: strh r7, [r4, r0]
005b40ac: mov r2, #0
005b40b0: movw r0, #0x602
005b40b4: mov r8, #5
005b40b8: tst ip, #2
005b40bc: strh r8, [r4, r0]
005b40c0: str r6, [r4, #0x604]
005b40c4: str r2, [r4, #0x610]
005b40c8: str r2, [r4, #0x608]
005b40cc: str r2, [r4, #0x60c]
005b40d0: bne #0x5b4944
005b40d4: ands lr, lr, #0x10000
005b40d8: moveq r6, #9
005b40dc: bne #0x5b4944
005b40e0: movw r0, #0x614
005b40e4: strh r6, [r4, r0]
005b40e8: mov r2, #0
005b40ec: movw r0, #0x616
005b40f0: mov sl, #9
005b40f4: tst r3, #0x100
005b40f8: strh sl, [r4, r0]
005b40fc: str lr, [r4, #0x618]
005b4100: str r2, [r4, #0x624]
005b4104: str r2, [r4, #0x61c]
005b4108: str r2, [r4, #0x620]
005b410c: bne #0x5b4924
005b4110: tst r1, #0x200000
005b4114: moveq lr, #7
005b4118: streq lr, [sp, #0xc]
005b411c: bne #0x5b4924
005b4120: and r2, r3, #0x100000
005b4124: cmp r2, #0
005b4128: movw lr, #0x87ee
005b412c: moveq lr, r2
005b4130: str lr, [r4, #0x67c]
005b4134: and r0, r3, #0x40000000
005b4138: movw r6, #0x8c92
005b413c: movw lr, #0x8c93
005b4140: moveq r6, r2
005b4144: moveq lr, r2
005b4148: movne sb, #0x15
005b414c: moveq sb, #0xe
005b4150: movne sl, #0x16
005b4154: moveq sl, #0xe
005b4158: movne r8, #0x17
005b415c: moveq r8, #0xe
005b4160: movw r2, #0x8c01
005b4164: cmp r0, #0
005b4168: moveq r2, r0
005b416c: str r2, [r4, #0x690]
005b4170: str lr, [r4, #0x668]
005b4174: ldr lr, [sp, #0xc]
005b4178: movw fp, #0x62a
005b417c: movne r7, #0x18
005b4180: moveq r7, #0xe
005b4184: strh lr, [r4, fp]
005b4188: mov fp, #0x650
005b418c: strh sb, [r4, fp]
005b4190: movw sb, #0x664
005b4194: strh sl, [r4, sb]
005b4198: movw sl, #0x678
005b419c: strh r8, [r4, sl]
005b41a0: movw r8, #0x68c
005b41a4: strh r7, [r4, r8]
005b41a8: mov r8, #0xe
005b41ac: movw r7, #0x628
005b41b0: strh r8, [r4, r7]
005b41b4: mov sl, #0x14
005b41b8: movw r7, #0x63c
005b41bc: strh sl, [r4, r7]
005b41c0: mov lr, #0xc
005b41c4: movw r7, #0x63e
005b41c8: strh lr, [r4, r7]
005b41cc: movw r7, #0x83f3
005b41d0: str r7, [r4, #0x640]
005b41d4: movw r7, #0x652
005b41d8: strh r8, [r4, r7]
005b41dc: movw lr, #0x67a
005b41e0: str r6, [r4, #0x654]
005b41e4: movw r6, #0x666
005b41e8: mov r2, #0
005b41ec: strh r8, [r4, r6]
005b41f0: movne sb, #0x19
005b41f4: moveq sb, #0xe
005b41f8: strh r8, [r4, lr]
005b41fc: mov r6, #5
005b4200: movw lr, #0x68e
005b4204: mov fp, #0x6a0
005b4208: strh r6, [r4, lr]
005b420c: movne sl, #0x1a
005b4210: moveq sl, #0xe
005b4214: str r2, [r4, #0x62c]
005b4218: str r2, [r4, #0x630]
005b421c: str r2, [r4, #0x634]
005b4220: str r2, [r4, #0x638]
005b4224: str r2, [r4, #0x644]
005b4228: str r2, [r4, #0x648]
005b422c: str r2, [r4, #0x64c]
005b4230: str r2, [r4, #0x658]
005b4234: str r2, [r4, #0x65c]
005b4238: str r2, [r4, #0x660]
005b423c: str r2, [r4, #0x66c]
005b4240: str r2, [r4, #0x670]
005b4244: str r2, [r4, #0x674]
005b4248: str r2, [r4, #0x680]
005b424c: str r2, [r4, #0x684]
005b4250: str r2, [r4, #0x688]
005b4254: ldr lr, [r4, #0x7bc]
005b4258: str r2, [r4, #0x694]
005b425c: strh sb, [r4, fp]
005b4260: movw sb, #0x6b4
005b4264: strh sl, [r4, sb]
005b4268: movne r8, #0x1b
005b426c: moveq r8, #0xe
005b4270: movw sl, #0x6c8
005b4274: movw r7, #0x8c03
005b4278: strh r8, [r4, sl]
005b427c: moveq r7, r0
005b4280: movw r8, #0x6a2
005b4284: mov sl, #7
005b4288: movw r6, #0x8c02
005b428c: strh sl, [r4, r8]
005b4290: moveq r6, r0
005b4294: str r7, [r4, #0x6a4]
005b4298: movne r0, #0x8c00
005b429c: movw r7, #0x6b6
005b42a0: mov r8, #5
005b42a4: strh r8, [r4, r7]
005b42a8: tst lr, #2
005b42ac: str r0, [r4, #0x6b8]
005b42b0: movw r0, #0x6ca
005b42b4: strh sl, [r4, r0]
005b42b8: str r6, [r4, #0x6cc]
005b42bc: str r2, [r4, #0x6d8]
005b42c0: str r2, [r4, #0x698]
005b42c4: str r2, [r4, #0x69c]
005b42c8: str r2, [r4, #0x6a8]
005b42cc: str r2, [r4, #0x6ac]
005b42d0: str r2, [r4, #0x6b0]
005b42d4: str r2, [r4, #0x6bc]
005b42d8: str r2, [r4, #0x6c0]
005b42dc: str r2, [r4, #0x6c4]
005b42e0: str r2, [r4, #0x6d0]
005b42e4: str r2, [r4, #0x6d4]
005b42e8: beq #0x5b4300
005b42ec: tst lr, #1
005b42f0: bne #0x5b4bc0
005b42f4: ldr r2, [r4, #0x7d8]
005b42f8: tst r2, #0x8000
005b42fc: bne #0x5b4bc0
005b4300: ands r2, r3, #0x10000
005b4304: movne r7, #0x1c
005b4308: beq #0x5b4bb0
005b430c: tst r3, #0x100
005b4310: movne r8, #0xa
005b4314: bne #0x5b4324
005b4318: tst r1, #0x200000
005b431c: movne r8, #0xe
005b4320: moveq r8, #5
005b4324: cmp r2, #0
005b4328: movw sl, #0x6dc
005b432c: strh r7, [r4, sl]
005b4330: movwne r2, #0x1907
005b4334: movw r7, #0x6de
005b4338: strh r8, [r4, r7]
005b433c: moveq r6, r2
005b4340: moveq r0, r2
005b4344: movwne r6, #0x881b
005b4348: movwne r0, #0x8d61
005b434c: str r2, [r4, #0x6e4]
005b4350: tst lr, #2
005b4354: mov r2, #0
005b4358: str r6, [r4, #0x6e0]
005b435c: str r0, [r4, #0x6e8]
005b4360: str r2, [r4, #0x6ec]
005b4364: beq #0x5b437c
005b4368: tst lr, #1
005b436c: bne #0x5b4bcc
005b4370: ldr r2, [r4, #0x7d8]
005b4374: tst r2, #0x8000
005b4378: bne #0x5b4bcc
005b437c: ands r2, r3, #0x10000
005b4380: movne sb, #0x1d
005b4384: beq #0x5b4ba0
005b4388: tst r3, #0x100
005b438c: movne r8, #0xa
005b4390: bne #0x5b43a0
005b4394: tst r1, #0x200000
005b4398: movne r8, #0xe
005b439c: moveq r8, #7
005b43a0: cmp r2, #0
005b43a4: mov sl, #0x6f0
005b43a8: strh sb, [r4, sl]
005b43ac: movwne r2, #0x1908
005b43b0: movw sl, #0x6f2
005b43b4: strh r8, [r4, sl]
005b43b8: moveq r7, r2
005b43bc: moveq r6, r2
005b43c0: movwne r7, #0x881a
005b43c4: movwne r6, #0x8d61
005b43c8: str r2, [r4, #0x6f8]
005b43cc: ands r0, lr, #2
005b43d0: mov r2, #0
005b43d4: str r2, [r4, #0x700]
005b43d8: str r7, [r4, #0x6f4]
005b43dc: str r6, [r4, #0x6fc]
005b43e0: movne r8, #0x1f
005b43e4: andne r2, r3, #0x8000
005b43e8: bne #0x5b43f8
005b43ec: ands r2, r3, #0x8000
005b43f0: movne r8, #0x1f
005b43f4: beq #0x5b4c08
005b43f8: tst r3, #0x100
005b43fc: movne r7, #0xa
005b4400: bne #0x5b4410
005b4404: tst r1, #0x200000
005b4408: movne r7, #0xe
005b440c: moveq r7, #5
005b4410: cmp r2, #0
005b4414: movw r6, #0x8815
005b4418: moveq r6, #0
005b441c: cmp r0, #0
005b4420: bne #0x5b4970
005b4424: cmp r2, #0
005b4428: moveq r0, r2
005b442c: bne #0x5b4970
005b4430: movw sl, #0x704
005b4434: strh r8, [r4, sl]
005b4438: movw r8, #0x706
005b443c: strh r7, [r4, r8]
005b4440: ands lr, lr, #2
005b4444: str r2, [r4, #0x70c]
005b4448: mov r2, #0
005b444c: str r6, [r4, #0x708]
005b4450: str r0, [r4, #0x710]
005b4454: str r2, [r4, #0x714]
005b4458: bne #0x5b4464
005b445c: tst r3, #0x8000
005b4460: beq #0x5b4bf8
005b4464: mov r7, #0x1f
005b4468: tst r3, #0x100
005b446c: bne #0x5b4968
005b4470: tst r1, #0x200000
005b4474: moveq r6, #7
005b4478: bne #0x5b4968
005b447c: cmp lr, #0
005b4480: bne #0x5b4958
005b4484: tst r3, #0x8000
005b4488: moveq r8, lr
005b448c: moveq r0, lr
005b4490: bne #0x5b4958
005b4494: movw r1, #0x718
005b4498: strh r7, [r4, r1]
005b449c: ands r2, r3, #0x400000
005b44a0: movw r1, #0x71a
005b44a4: strh r6, [r4, r1]
005b44a8: movwne r2, #0x1902
005b44ac: str r8, [r4, #0x71c]
005b44b0: mov r1, #0
005b44b4: moveq r8, #0x27
005b44b8: movne r8, #0x20
005b44bc: movw r7, #0x72c
005b44c0: str lr, [r4, #0x720]
005b44c4: str r0, [r4, #0x724]
005b44c8: str r1, [r4, #0x728]
005b44cc: moveq r6, r2
005b44d0: strh r8, [r4, r7]
005b44d4: moveq lr, r2
005b44d8: movne r6, r2
005b44dc: movwne lr, #0x1403
005b44e0: movw r7, #0x72e
005b44e4: tst r3, #0x400000
005b44e8: mov sl, #0x20
005b44ec: strh sl, [r4, r7]
005b44f0: movne r0, #0x20
005b44f4: moveq r0, #0x27
005b44f8: str r2, [r4, #0x734]
005b44fc: ands r1, r3, #4
005b4500: movw r2, #0x81a5
005b4504: str r6, [r4, #0x730]
005b4508: str lr, [r4, #0x738]
005b450c: str r2, [r4, #0x73c]
005b4510: beq #0x5b4a5c
005b4514: mov r1, #0x740
005b4518: strh r0, [r4, r1]
005b451c: mov lr, #0x21
005b4520: movw r1, #0x742
005b4524: strh lr, [r4, r1]
005b4528: mov r2, #0
005b452c: movw r1, #0x81a6
005b4530: str r2, [r4, #0x74c]
005b4534: str r1, [r4, #0x750]
005b4538: str r2, [r4, #0x744]
005b453c: str r2, [r4, #0x748]
005b4540: ands r2, r3, #0x400000
005b4544: movne r7, #0x22
005b4548: moveq r7, #0x27
005b454c: ands r1, r3, #8
005b4550: movne r6, #0x22
005b4554: bne #0x5b4564
005b4558: tst r3, #4
005b455c: movne r6, #0x21
005b4560: moveq r6, #0x20
005b4564: cmp r2, #0
005b4568: movwne r2, #0x1902
005b456c: moveq lr, r2
005b4570: moveq r0, r2
005b4574: movne lr, r2
005b4578: movwne r0, #0x1405
005b457c: cmp r1, #0
005b4580: beq #0x5b4af8
005b4584: movw r1, #0x754
005b4588: strh r7, [r4, r1]
005b458c: movw r1, #0x756
005b4590: strh r6, [r4, r1]
005b4594: str r2, [r4, #0x75c]
005b4598: movw r2, #0x81a7
005b459c: str lr, [r4, #0x758]
005b45a0: str r0, [r4, #0x760]
005b45a4: str r2, [r4, #0x764]
005b45a8: tst ip, #0x10
005b45ac: beq #0x5b4a84
005b45b0: movw r1, #0x768
005b45b4: mov r0, #0x27
005b45b8: strh r0, [r4, r1]
005b45bc: mov r6, #0x23
005b45c0: add r1, r1, #2
005b45c4: strh r6, [r4, r1]
005b45c8: mov r2, #0
005b45cc: movw r1, #0x88f0
005b45d0: str r2, [r4, #0x774]
005b45d4: str r1, [r4, #0x778]
005b45d8: str r2, [r4, #0x76c]
005b45dc: str r2, [r4, #0x770]
005b45e0: tst r3, #0x200
005b45e4: bne #0x5b49b0
005b45e8: tst r3, #0x400
005b45ec: movne r0, #0x25
005b45f0: beq #0x5b4be8
005b45f4: movw r1, #0x77e
005b45f8: strh r0, [r4, r1]
005b45fc: mov r2, #0
005b4600: movw r1, #0x77c
005b4604: mov r7, #0x27
005b4608: strh r7, [r4, r1]
005b460c: str r2, [r4, #0x78c]
005b4610: str r2, [r4, #0x780]
005b4614: str r2, [r4, #0x784]
005b4618: str r2, [r4, #0x788]
005b461c: tst r3, #0x400
005b4620: bne #0x5b497c
005b4624: tst r3, #0x800
005b4628: movne r1, #0x26
005b462c: beq #0x5b4bd8
005b4630: movw r2, #0x792
005b4634: strh r1, [r4, r2]
005b4638: mov r3, #0
005b463c: mov r2, #0x790
005b4640: mov r8, #0x27
005b4644: strh r8, [r4, r2]
005b4648: str r3, [r4, #0x7a0]
005b464c: str r3, [r4, #0x794]
005b4650: str r3, [r4, #0x798]
005b4654: str r3, [r4, #0x79c]
005b4658: mov lr, #0x27
005b465c: movw r2, #0x7a4
005b4660: strh lr, [r4, r2]
005b4664: mov r0, #0x26
005b4668: movw r2, #0x7a6
005b466c: strh r0, [r4, r2]
005b4670: ldr r0, [pc, #0x45c]
005b4674: mov r3, #0
005b4678: movw r2, #0x8d48
005b467c: str r3, [r4, #0x7b0]
005b4680: str r3, [r4, #0x7a8]
005b4684: str r3, [r4, #0x7ac]
005b4688: str r2, [r4, #0x7b4]
005b468c: mov r1, #1
005b4690: add r0, pc, r0
005b4694: bl #0x60aca0
005b4698: movw r0, #0x1f02
005b469c: bl #0x30e280
005b46a0: mov r7, r0
005b46a4: bl #0x30de54
005b46a8: add r6, r4, #8
005b46ac: add r2, r7, r0
005b46b0: mov r1, r7
005b46b4: mov r0, r6
005b46b8: bl #0x320b88
005b46bc: ldr r3, [r4, #0x18]
005b46c0: ldr r1, [r4, #0x1c]
005b46c4: subs r2, r3, r1
005b46c8: bne #0x5b4c18
005b46cc: add r7, sp, #0x24
005b46d0: add r1, r2, #8
005b46d4: mov r0, r7
005b46d8: str r7, [sp, #0x34]
005b46dc: str r7, [sp, #0x38]
005b46e0: bl #0x3209a8
005b46e4: ldr r3, [sp, #0x34]
005b46e8: mov r2, #0
005b46ec: strb r2, [r3]
005b46f0: ldr r3, [sp, #0x38]
005b46f4: cmp r3, r7
005b46f8: ldrne r2, [sp, #0x24]
005b46fc: ldr r3, [sp, #0x34]
005b4700: addeq r2, r7, #0x10
005b4704: rsb r2, r3, r2
005b4708: cmp r2, #7
005b470c: bls #0x5b4cc4
005b4710: mov r2, #0x4f
005b4714: strb r2, [r3]
005b4718: ldr r1, [pc, #0x3b8]
005b471c: ldr r0, [sp, #0x34]
005b4720: mov r2, #6
005b4724: add r1, pc, r1
005b4728: add r0, r0, #1
005b472c: add r1, r1, #1
005b4730: bl #0x30e868
005b4734: ldr r3, [sp, #0x34]
005b4738: mov r2, #0
005b473c: strb r2, [r3, #7]
005b4740: ldr r3, [sp, #0x34]
005b4744: add r3, r3, #7
005b4748: str r3, [sp, #0x34]
005b474c: mov r0, r7
005b4750: ldr r1, [r4, #0x1c]
005b4754: ldr r2, [r4, #0x18]
005b4758: bl #0x320a4c
005b475c: cmp r6, r7
005b4760: beq #0x5b4774
005b4764: mov r0, r6
005b4768: ldr r1, [sp, #0x38]
005b476c: ldr r2, [sp, #0x34]
005b4770: bl #0x320b88
005b4774: ldr r0, [sp, #0x38]
005b4778: cmp r0, r7
005b477c: beq #0x5b478c
005b4780: cmp r0, #0
005b4784: beq #0x5b478c
005b4788: bl #0x310450
005b478c: ldr r1, [r4, #0x1c]
005b4790: ldr r0, [pc, #0x344]
005b4794: mov r2, #1
005b4798: add r0, pc, r0
005b479c: bl #0x60ace8
005b47a0: movw r0, #0x1f01
005b47a4: bl #0x30e280
005b47a8: mov r7, r0
005b47ac: mov r0, #0x1f00
005b47b0: bl #0x30e280
005b47b4: cmp r0, #0
005b47b8: cmpne r7, #0
005b47bc: mov r6, r0
005b47c0: bne #0x5b4c80
005b47c4: mov r0, r4
005b47c8: bl #0x5af034
005b47cc: uxth r1, r0
005b47d0: cmp r1, #7
005b47d4: ldrb r3, [sp, #0x14]
005b47d8: movls r2, r1
005b47dc: movhi r2, #8
005b47e0: mov r0, r4
005b47e4: bl #0x5aab60
005b47e8: mov r1, #0
005b47ec: mov r0, #0x38
005b47f0: bl #0x5341ac
005b47f4: ldr r2, [sp, #4]
005b47f8: mov r1, r4
005b47fc: mov r6, r0
005b4800: bl #0x5b1bd8
005b4804: cmp r6, #0
005b4808: str r6, [sp, #0x10]
005b480c: ldrne r3, [r6, #4]
005b4810: mov r0, r4
005b4814: add r1, sp, #0x10
005b4818: addne r3, r3, #1
005b481c: strne r3, [r6, #4]
005b4820: ldr r3, [r4]
005b4824: mov lr, pc
005b4828: ldr pc, [r3, #0x8c]
005b482c: movw r0, #0xd05
005b4830: mov r1, #1
005b4834: bl #0x30e16c
005b4838: mov r0, r4
005b483c: ldr r1, [sp, #4]
005b4840: bl #0x5b1ab4
005b4844: cmp r0, #0
005b4848: moveq r4, r0
005b484c: beq #0x5b48b0
005b4850: ldr r8, [pc, #0x288]
005b4854: mov r7, r4
005b4858: mov r6, #0
005b485c: add r8, pc, r8
005b4860: add r8, r8, #0x110
005b4864: ldr r0, [r8, r6]
005b4868: cmp r0, #0
005b486c: bne #0x5b4c74
005b4870: add r6, r6, #4
005b4874: cmp r6, #0x14
005b4878: add r7, r7, #4
005b487c: bne #0x5b4864
005b4880: mov r0, r4
005b4884: ldr r3, [r4]
005b4888: mov lr, pc
005b488c: ldr pc, [r3, #0x74]
005b4890: ldr r3, [r4]
005b4894: mov r0, r4
005b4898: mov r1, #1
005b489c: mov lr, pc
005b48a0: ldr pc, [r3, #0xa8]
005b48a4: mov r0, r4
005b48a8: bl #0x5ae5f0
005b48ac: mov r4, #1
005b48b0: ldr r0, [sp, #0x10]
005b48b4: cmp r0, #0
005b48b8: beq #0x5b48c0
005b48bc: bl #0x31d584
005b48c0: ldr r1, [sp, #8]
005b48c4: ldr r2, [sp, #0x3c]
005b48c8: mov r0, r4
005b48cc: ldr r3, [r5, r1]
005b48d0: ldr r3, [r3]
005b48d4: cmp r2, r3
005b48d8: bne #0x5b4d9c
005b48dc: add sp, sp, #0x44
005b48e0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005b48e4: tst r1, #0x40000
005b48e8: mov r7, #0xe
005b48ec: beq #0x5b3f84
005b48f0: cmp r2, #0
005b48f4: movweq r8, #0x1908
005b48f8: beq #0x5b4a14
005b48fc: movw r8, #0x1908
005b4900: b #0x5b3f90
005b4904: mov sl, #0xe
005b4908: b #0x5b3dac
005b490c: mov ip, #0xe
005b4910: b #0x5b3d6c
005b4914: mov r6, #0xe
005b4918: b #0x5b3d04
005b491c: mov r6, #0xe
005b4920: b #0x5b3cc4
005b4924: mov r0, #0xe
005b4928: str r0, [sp, #0xc]
005b492c: b #0x5b4120
005b4930: mov r7, #0x11
005b4934: movw r6, #0x83f0
005b4938: b #0x5b40a4
005b493c: mov r7, #0xe
005b4940: b #0x5b4050
005b4944: mov r6, #0x12
005b4948: movw lr, #0x83f1
005b494c: b #0x5b40e0
005b4950: mov r7, #0xe
005b4954: b #0x5b4008
005b4958: movw lr, #0x1908
005b495c: movw r8, #0x8814
005b4960: movw r0, #0x1406
005b4964: b #0x5b4494
005b4968: mov r6, #0xe
005b496c: b #0x5b447c
005b4970: movw r2, #0x1907
005b4974: movw r0, #0x1406
005b4978: b #0x5b4430
005b497c: mov r2, #0x790
005b4980: mov sl, #0x27
005b4984: strh sl, [r4, r2]
005b4988: mov ip, #0x25
005b498c: movw r2, #0x792
005b4990: strh ip, [r4, r2]
005b4994: mov r3, #0
005b4998: movw r2, #0x8d47
005b499c: str r3, [r4, #0x79c]
005b49a0: str r2, [r4, #0x7a0]
005b49a4: str r3, [r4, #0x794]
005b49a8: str r3, [r4, #0x798]
005b49ac: b #0x5b4658
005b49b0: movw r1, #0x77c
005b49b4: mov r7, #0x27
005b49b8: strh r7, [r4, r1]
005b49bc: mov r8, #0x24
005b49c0: movw r1, #0x77e
005b49c4: strh r8, [r4, r1]
005b49c8: mov r2, #0
005b49cc: movw r1, #0x8d46
005b49d0: str r2, [r4, #0x788]
005b49d4: str r1, [r4, #0x78c]
005b49d8: str r2, [r4, #0x780]
005b49dc: str r2, [r4, #0x784]
005b49e0: b #0x5b461c
005b49e4: movw r2, #0x574
005b49e8: mov r7, #0xa
005b49ec: strh r7, [r4, r2]
005b49f0: movw r2, #0x576
005b49f4: strh r7, [r4, r2]
005b49f8: movw r2, #0x8051
005b49fc: str r0, [r4, #0x57c]
005b4a00: str r6, [r4, #0x580]
005b4a04: str r2, [r4, #0x584]
005b4a08: str r0, [r4, #0x578]
005b4a0c: b #0x5b3ee0
005b4a10: mov r8, r2
005b4a14: mov r2, #0
005b4a18: mov r0, r2
005b4a1c: b #0x5b3f98
005b4a20: ands r2, r1, #0x200000
005b4a24: bne #0x5b3fc4
005b4a28: movw ip, #0x5c4
005b4a2c: mov sl, #0xe
005b4a30: strh sl, [r4, ip]
005b4a34: mov r6, #7
005b4a38: movw ip, #0x5c6
005b4a3c: strh r6, [r4, ip]
005b4a40: movw r0, #0x1908
005b4a44: movw ip, #0x1401
005b4a48: str r0, [r4, #0x5cc]
005b4a4c: str ip, [r4, #0x5d0]
005b4a50: str r2, [r4, #0x5d4]
005b4a54: str r0, [r4, #0x5c8]
005b4a58: b #0x5b3ff4
005b4a5c: mov r2, #0x740
005b4a60: strh r0, [r4, r2]
005b4a64: mov r6, #0x20
005b4a68: movw r2, #0x742
005b4a6c: strh r6, [r4, r2]
005b4a70: str r1, [r4, #0x750]
005b4a74: str r1, [r4, #0x744]
005b4a78: str r1, [r4, #0x748]
005b4a7c: str r1, [r4, #0x74c]
005b4a80: b #0x5b4540
005b4a84: ldr r2, [r4, #0x7d4]
005b4a88: tst r2, #0x10
005b4a8c: bne #0x5b45b0
005b4a90: ands r2, r3, #0x800000
005b4a94: bne #0x5b45b0
005b4a98: movw r1, #0x768
005b4a9c: mov r7, #0x27
005b4aa0: strh r7, [r4, r1]
005b4aa4: movw r1, #0x76a
005b4aa8: strh r7, [r4, r1]
005b4aac: str r2, [r4, #0x778]
005b4ab0: str r2, [r4, #0x76c]
005b4ab4: str r2, [r4, #0x770]
005b4ab8: str r2, [r4, #0x774]
005b4abc: b #0x5b45e0
005b4ac0: eorseq r0, lr, r4, lsl #31
005b4ac4: andeq r4, r0, ip, lsr #1
005b4ac8: ldrdeq r1, r2, [r0], -ip
005b4acc: ldrhteq sp, [r5], -r4
005b4ad0: eorseq ip, r2, r0, asr #20
005b4ad4: ldrsbteq fp, [r2], -r0
005b4ad8: eorseq fp, r2, r4, asr pc
005b4adc: eorseq fp, r2, r8, ror #29
005b4ae0: ldrsbteq fp, [r2], -r8
005b4ae4: ldrhteq fp, [r2], -ip
005b4ae8: eorseq fp, r2, r4, asr #20
005b4aec: eorseq fp, r2, r4, lsl #20
005b4af0: eorseq fp, r2, r0, lsl #20
005b4af4: eorseq fp, r2, ip, ror #18
005b4af8: movw r8, #0x754
005b4afc: strh r7, [r4, r8]
005b4b00: movw r7, #0x756
005b4b04: strh r6, [r4, r7]
005b4b08: str lr, [r4, #0x758]
005b4b0c: str r2, [r4, #0x75c]
005b4b10: str r0, [r4, #0x760]
005b4b14: str r1, [r4, #0x764]
005b4b18: b #0x5b45a8
005b4b1c: ldr r0, [r4, #0x7d4]
005b4b20: tst r0, #0x80
005b4b24: beq #0x5b3c88
005b4b28: b #0x5b3c80
005b4b2c: tst r3, #0x1000
005b4b30: ldreq r2, [r4, #0x9c]
005b4b34: beq #0x5b3c70
005b4b38: b #0x5b3c68
005b4b3c: ldr r3, [r4, #0x7e8]
005b4b40: tst r3, #0x20000
005b4b44: ldrne r1, [r4, #0x7ec]
005b4b48: bne #0x5b3c54
005b4b4c: ldr r1, [r4, #0x7ec]
005b4b50: tst r1, #8
005b4b54: beq #0x5b3c5c
005b4b58: b #0x5b3c54
005b4b5c: ldr r0, [pc, #-0x80]
005b4b60: mov r1, #2
005b4b64: add r0, pc, r0
005b4b68: bl #0x60aca0
005b4b6c: movw r0, #0x1f03
005b4b70: bl #0x30e280
005b4b74: mov r1, r0
005b4b78: mov r0, r4
005b4b7c: bl #0x6dd734
005b4b80: ldr r3, [r4, #0x7d0]
005b4b84: tst r3, #0x20
005b4b88: beq #0x5b3bd8
005b4b8c: add r1, r4, #0x4a0
005b4b90: add r1, r1, #8
005b4b94: movw r0, #0x84ff
005b4b98: bl #0x30ec10
005b4b9c: b #0x5b3bd8
005b4ba0: tst r3, #0x8000
005b4ba4: movne sb, #0x1f
005b4ba8: moveq sb, #0xe
005b4bac: b #0x5b4388
005b4bb0: tst r3, #0x8000
005b4bb4: movne r7, #0x1e
005b4bb8: moveq r7, #0xe
005b4bbc: b #0x5b430c
005b4bc0: mov r7, #0x1c
005b4bc4: and r2, r3, #0x10000
005b4bc8: b #0x5b430c
005b4bcc: mov sb, #0x1d
005b4bd0: and r2, r3, #0x10000
005b4bd4: b #0x5b4388
005b4bd8: tst r3, #0x200
005b4bdc: movne r1, #0x24
005b4be0: moveq r1, #0x27
005b4be4: b #0x5b4630
005b4be8: tst r3, #0x800
005b4bec: movne r0, #0x26
005b4bf0: moveq r0, #0x27
005b4bf4: b #0x5b45f4
005b4bf8: tst r3, #0x10000
005b4bfc: movne r7, #0x1d
005b4c00: moveq r7, #0xe
005b4c04: b #0x5b4468
005b4c08: tst r3, #0x10000
005b4c0c: movne r8, #0x1d
005b4c10: moveq r8, #0xe
005b4c14: b #0x5b43f8
005b4c18: cmp r2, #6
005b4c1c: bls #0x5b46cc
005b4c20: cmp r3, r1
005b4c24: moveq r0, r3
005b4c28: beq #0x5b4c5c
005b4c2c: ldr sb, [pc, #-0x14c]
005b4c30: add r8, r1, #1
005b4c34: add sb, pc, sb
005b4c38: add sl, sb, #7
005b4c3c: add sb, sb, #2
005b4c40: ldrsb r0, [r8, #-1]
005b4c44: cmp r0, #0x4f
005b4c48: beq #0x5b4d50
005b4c4c: cmp r8, r3
005b4c50: mov r0, r8
005b4c54: add r8, r8, #1
005b4c58: bne #0x5b4c40
005b4c5c: cmp r3, r0
005b4c60: beq #0x5b46cc
005b4c64: rsb r0, r1, r0
005b4c68: cmn r0, #1
005b4c6c: bne #0x5b4790
005b4c70: b #0x5b46cc
005b4c74: ldr r1, [r7, #0x254]
005b4c78: bl #0x30ddf4
005b4c7c: b #0x5b4870
005b4c80: ldr r0, [pc, #-0x19c]
005b4c84: mov r1, r7
005b4c88: mov r2, #1
005b4c8c: add r0, pc, r0
005b4c90: bl #0x60ace8
005b4c94: ldr r0, [pc, #-0x1ac]
005b4c98: mov r1, r6
005b4c9c: mov r2, #1
005b4ca0: add r0, pc, r0
005b4ca4: bl #0x60ace8
005b4ca8: mov r0, r6
005b4cac: bl #0x30de54
005b4cb0: mov r1, r6
005b4cb4: add r2, r6, r0
005b4cb8: add r0, r4, #0x20
005b4cbc: bl #0x320b88
005b4cc0: b #0x5b47c4
005b4cc4: mov r1, #7
005b4cc8: mov r0, r7
005b4ccc: bl #0x320358
005b4cd0: mov r1, #0
005b4cd4: mov sb, r0
005b4cd8: bl #0x310568
005b4cdc: ldr r1, [sp, #0x38]
005b4ce0: ldr r8, [sp, #0x34]
005b4ce4: mov sl, r0
005b4ce8: cmp r1, r8
005b4cec: moveq r0, r0
005b4cf0: beq #0x5b4d04
005b4cf4: rsb r8, r1, r8
005b4cf8: mov r2, r8
005b4cfc: bl #0x30e868
005b4d00: add r0, r0, r8
005b4d04: ldr r1, [pc, #-0x218]
005b4d08: mov r2, #7
005b4d0c: add r1, pc, r1
005b4d10: bl #0x30e868
005b4d14: mov r3, #0
005b4d18: strb r3, [r0, #7]
005b4d1c: ldr r3, [sp, #0x38]
005b4d20: add r8, r0, #7
005b4d24: cmp r3, r7
005b4d28: beq #0x5b4d3c
005b4d2c: cmp r3, #0
005b4d30: beq #0x5b4d3c
005b4d34: mov r0, r3
005b4d38: bl #0x310450
005b4d3c: add sb, sl, sb
005b4d40: str sb, [sp, #0x24]
005b4d44: str r8, [sp, #0x34]
005b4d48: str sl, [sp, #0x38]
005b4d4c: b #0x5b474c
005b4d50: cmp r8, r3
005b4d54: mov r0, r8
005b4d58: beq #0x5b4c5c
005b4d5c: mov ip, sb
005b4d60: ldrsb r7, [r0]
005b4d64: ldrsb lr, [ip, #-1]
005b4d68: cmp r7, lr
005b4d6c: bne #0x5b4d8c
005b4d70: cmp ip, sl
005b4d74: beq #0x5b4d94
005b4d78: add r0, r0, #1
005b4d7c: cmp r0, r3
005b4d80: add ip, ip, #1
005b4d84: bne #0x5b4d60
005b4d88: b #0x5b4c5c
005b4d8c: add r8, r8, #1
005b4d90: b #0x5b4c40
005b4d94: sub r0, r8, #1
005b4d98: b #0x5b4c5c
005b4d9c: bl #0x30e310

_ZN6glitch5video15CTextureManager22createTextureFromImageEPKcRKN5boost13intrusive_ptrINS0_6CImageEEENS0_16E_TEXTURE_LAYOUTE
005ec460: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005ec464: ldr ip, [r3]
005ec468: sub sp, sp, #0x5c
005ec46c: mov lr, #1
005ec470: mov r4, #0xc
005ec474: mov sl, r3
005ec478: mov r3, #0
005ec47c: str r4, [sp, #0x38]
005ec480: str lr, [sp, #0x44]
005ec484: str lr, [sp, #0x48]
005ec488: str r3, [sp, #0x40]
005ec48c: strb r3, [sp, #0x50]
005ec490: str lr, [sp, #0x4c]
005ec494: strb r3, [sp, #0x52]
005ec498: str r3, [sp, #0x34]
005ec49c: str r3, [sp, #0x3c]
005ec4a0: strb r3, [sp, #0x51]
005ec4a4: ldr r3, [ip, #0x20]
005ec4a8: str r1, [sp, #0x28]
005ec4ac: str r0, [sp, #0x2c]
005ec4b0: str r3, [sp, #0x38]
005ec4b4: ldr r3, [ip, #0x10]
005ec4b8: mov r5, r2
005ec4bc: ldr r2, [sp, #0x28]
005ec4c0: str r3, [sp, #0x44]
005ec4c4: ldr r3, [ip, #0x14]
005ec4c8: ldr r6, [sp, #0x80]
005ec4cc: str r3, [sp, #0x48]
005ec4d0: ldrb r4, [ip, #0x28]
005ec4d4: cmp r4, #0
005ec4d8: ldrne r1, [sp, #0x28]
005ec4dc: ldreq r3, [r1, #0x74]
005ec4e0: ldrne r3, [r1, #0x74]
005ec4e4: ldr r1, [r2, #0x28]
005ec4e8: ubfxne r4, r3, #6, #1
005ec4ec: ldr r2, [r1, #0x88]
005ec4f0: tst r2, #0x10
005ec4f4: moveq r2, r4
005ec4f8: movne r2, #1
005ec4fc: tst r3, #0x20
005ec500: movne r3, #3
005ec504: strb r2, [sp, #0x50]
005ec508: strne r3, [sp, #0x40]
005ec50c: bne #0x5ec51c
005ec510: tst r3, #0x10
005ec514: movne r3, #1
005ec518: strne r3, [sp, #0x40]
005ec51c: cmp r6, #1
005ec520: beq #0x5ec808
005ec524: cmp r6, #0
005ec528: bne #0x5ec7a8
005ec52c: add r0, sp, #0x54
005ec530: add r3, sp, #0x34
005ec534: mov r2, r5
005ec538: bl #0x5aa5f8
005ec53c: ldr r3, [sp, #0x54]
005ec540: cmp r3, #0
005ec544: mov r0, r3
005ec548: beq #0x5ec7fc
005ec54c: ldr r2, [sp, #0x44]
005ec550: cmp r2, #0
005ec554: mvneq r1, #0
005ec558: beq #0x5ec56c
005ec55c: mvn r1, #0
005ec560: asrs r2, r2, #1
005ec564: add r1, r1, #1
005ec568: bne #0x5ec560
005ec56c: ldr r2, [sp, #0x48]
005ec570: cmp r2, #0
005ec574: mvneq ip, #0
005ec578: beq #0x5ec58c
005ec57c: mvn ip, #0
005ec580: asrs r2, r2, #1
005ec584: add ip, ip, #1
005ec588: bne #0x5ec580
005ec58c: ldr r2, [sl]
005ec590: cmp ip, r1
005ec594: movge r1, ip
005ec598: movlt r1, r1
005ec59c: ldr lr, [r2, #0x24]
005ec5a0: cmp r1, lr
005ec5a4: ldr r1, [r2, #8]
005ec5a8: movne r6, #1
005ec5ac: eoreq r6, r4, #1
005ec5b0: cmp r1, #0
005ec5b4: str r1, [sp, #0x24]
005ec5b8: beq #0x5eca38
005ec5bc: ldr ip, [r3, #0x38]
005ec5c0: ldr r1, [r2, #0x20]
005ec5c4: ubfx r2, ip, #4, #6
005ec5c8: cmp r2, r1
005ec5cc: beq #0x5ec850
005ec5d0: cmp r6, #0
005ec5d4: beq #0x5ec8bc
005ec5d8: ldr r3, [r0, #0x30]
005ec5dc: ldr r2, [r3]
005ec5e0: ldr r0, [r3, #4]
005ec5e4: rsb r0, r2, r0
005ec5e8: mov r1, #0
005ec5ec: bl #0x5341a8
005ec5f0: ldr r2, [sp, #0x24]
005ec5f4: mov r1, r0
005ec5f8: mov r3, r6
005ec5fc: subs r4, r0, r2
005ec600: movne r4, #1
005ec604: mov r2, r4
005ec608: ldr r0, [sp, #0x54]
005ec60c: bl #0x5fdf74
005ec610: cmp r4, #0
005ec614: beq #0x5ec994
005ec618: ldr r3, [sp, #0x54]
005ec61c: ldr r2, [sl]
005ec620: ldr r0, [r3, #0x38]
005ec624: ldr r7, [r2, #0x20]
005ec628: mov r4, r3
005ec62c: ubfx r0, r0, #4, #6
005ec630: cmp r0, r7
005ec634: beq #0x5ec9ec
005ec638: uxth r3, r7
005ec63c: cmp r3, #0x27
005ec640: bne #0x5ec9d0
005ec644: ldr r4, [pc, #0x43c]
005ec648: mov r7, r0
005ec64c: add r4, pc, r4
005ec650: cmp r7, #0x27
005ec654: bne #0x5ec9c0
005ec658: ldr ip, [pc, #0x42c]
005ec65c: add ip, pc, ip
005ec660: ldr r1, [pc, #0x428]
005ec664: mov r3, r4
005ec668: mov r2, r5
005ec66c: add r1, pc, r1
005ec670: mov r0, #2
005ec674: str ip, [sp]
005ec678: bl #0x60b034
005ec67c: ldr r3, [sp, #0x54]
005ec680: ldr r2, [sl]
005ec684: mov r4, r3
005ec688: cmp r6, #0
005ec68c: ldr sb, [r2, #0xc]
005ec690: ldr r7, [r3, #0x24]
005ec694: ldr r6, [r3, #0x20]
005ec698: beq #0x5ec904
005ec69c: mov r1, #1
005ec6a0: str r1, [sp, #0x1c]
005ec6a4: mov r5, #0
005ec6a8: str sl, [sp, #0x20]
005ec6ac: b #0x5ec794
005ec6b0: ldr r3, [r4, #4]
005ec6b4: add r3, r3, #1
005ec6b8: str r3, [r4, #4]
005ec6bc: ldr r0, [sp, #0x54]
005ec6c0: cmp r0, #0
005ec6c4: beq #0x5ec79c
005ec6c8: mov r1, #4
005ec6cc: mov r2, #0
005ec6d0: mov r3, r5
005ec6d4: bl #0x5fe0d4
005ec6d8: mov r8, r0
005ec6dc: ldr r0, [sp, #0x54]
005ec6e0: ldr ip, [sp, #0x20]
005ec6e4: cmp r5, #0
005ec6e8: mov r1, r5
005ec6ec: ldr r3, [ip]
005ec6f0: ldrne fp, [sb, #-4]
005ec6f4: ldreq fp, [sp, #0x24]
005ec6f8: ldr r3, [r3, #0x20]
005ec6fc: ldr sl, [r0, #0x38]
005ec700: str r3, [sp, #0x18]
005ec704: bl #0x5ea504
005ec708: ldr r3, [sp, #0x18]
005ec70c: ubfx sl, sl, #4, #6
005ec710: str r0, [sp, #4]
005ec714: mov r2, #0
005ec718: mov r0, r3
005ec71c: mov r1, fp
005ec720: mov r3, sl
005ec724: str r8, [sp]
005ec728: str r6, [sp, #8]
005ec72c: str r7, [sp, #0xc]
005ec730: str r2, [sp, #0x10]
005ec734: bl #0x5f95ac
005ec738: cmp r0, #0
005ec73c: beq #0x5ec960
005ec740: asr r6, r6, #1
005ec744: asr r7, r7, #1
005ec748: cmp r6, #1
005ec74c: movlt r6, #1
005ec750: cmp r7, #1
005ec754: movlt r7, #1
005ec758: cmp r8, #0
005ec75c: beq #0x5ec768
005ec760: mov r0, r4
005ec764: bl #0x5fdc0c
005ec768: cmp r4, #0
005ec76c: beq #0x5ec778
005ec770: mov r0, r4
005ec774: bl #0x31d584
005ec778: ldr r3, [sp, #0x1c]
005ec77c: add r5, r5, #1
005ec780: uxtb r5, r5
005ec784: cmp r5, r3
005ec788: add sb, sb, #4
005ec78c: bhs #0x5ec994
005ec790: ldr r4, [sp, #0x54]
005ec794: cmp r4, #0
005ec798: bne #0x5ec6b0
005ec79c: mov r0, #0
005ec7a0: mov r8, r0
005ec7a4: b #0x5ec6e0
005ec7a8: uxth r3, r6
005ec7ac: cmp r3, #0xff
005ec7b0: beq #0x5ec844
005ec7b4: mov r0, #0
005ec7b8: bl #0x5fda78
005ec7bc: ldr r3, [r0, r6, lsl #2]
005ec7c0: ldr r1, [pc, #0x2cc]
005ec7c4: mov r0, #2
005ec7c8: mov r2, r5
005ec7cc: add r1, pc, r1
005ec7d0: bl #0x60b034
005ec7d4: ldr ip, [sp, #0x28]
005ec7d8: add r0, sp, #0x54
005ec7dc: add r3, sp, #0x34
005ec7e0: ldr r1, [ip, #0x28]
005ec7e4: mov r2, r5
005ec7e8: bl #0x5aa5f8
005ec7ec: ldr r3, [sp, #0x54]
005ec7f0: cmp r3, #0
005ec7f4: mov r0, r3
005ec7f8: bne #0x5ec54c
005ec7fc: ldr r2, [sp, #0x2c]
005ec800: str r3, [r2]
005ec804: b #0x5ec954
005ec808: ldrb r3, [ip, #0x28]
005ec80c: cmp r3, #0
005ec810: streq r6, [sp, #0x3c]
005ec814: beq #0x5ec52c
005ec818: mov r0, #0
005ec81c: bl #0x5fda78
005ec820: ldr r1, [pc, #0x270]
005ec824: ldr r3, [r0, #4]
005ec828: mov r2, r5
005ec82c: add r1, pc, r1
005ec830: mov r0, #2
005ec834: bl #0x60b034
005ec838: ldr r3, [sp, #0x28]
005ec83c: ldr r1, [r3, #0x28]
005ec840: b #0x5ec52c
005ec844: ldr r3, [pc, #0x250]
005ec848: add r3, pc, r3
005ec84c: b #0x5ec7c0
005ec850: ldr ip, [sp, #0x28]
005ec854: ldr r1, [ip, #0x74]
005ec858: tst r1, #1
005ec85c: bne #0x5ec5d0
005ec860: tst r1, #2
005ec864: beq #0x5ec5d0
005ec868: ldr r1, [r3, #0x20]
005ec86c: mov r0, r2
005ec870: bl #0x5edaec
005ec874: ldr r3, [sl]
005ec878: ldr r3, [r3, #0x18]
005ec87c: cmp r3, r0
005ec880: ldrne r0, [sp, #0x54]
005ec884: bne #0x5ec5d0
005ec888: ldr r0, [sp, #0x54]
005ec88c: ldrb r3, [r0, #0x3e]
005ec890: cmp r3, #1
005ec894: bls #0x5eca38
005ec898: cmp r6, #0
005ec89c: bne #0x5eca38
005ec8a0: bl #0x5e80e8
005ec8a4: ldr r3, [sl]
005ec8a8: ldr r3, [r3, #0x1c]
005ec8ac: cmp r0, r3
005ec8b0: ldreq r0, [sp, #0x54]
005ec8b4: beq #0x5eca38
005ec8b8: ldr r0, [sp, #0x54]
005ec8bc: ldr r3, [r0, #0x38]
005ec8c0: ldrb r2, [r0, #0x3f]
005ec8c4: and r3, r3, #3
005ec8c8: cmp r3, #2
005ec8cc: moveq r3, #5
005ec8d0: movne r3, #0
005ec8d4: tst r2, #2
005ec8d8: ldrne r1, [r0, #0x30]
005ec8dc: ldreq r2, [r0, #0x30]
005ec8e0: ldrbeq r1, [r0, #0x3e]
005ec8e4: ldrne r2, [r1]
005ec8e8: ldrne r1, [r1, #4]
005ec8ec: ldreq r2, [r2, r1, lsl #2]
005ec8f0: rsbne r2, r2, r1
005ec8f4: add r0, r2, #0x7f
005ec8f8: bic r0, r0, #0x7f
005ec8fc: mla r0, r0, r3, r2
005ec900: b #0x5ec5e8
005ec904: ldrb r2, [r3, #0x3e]
005ec908: cmp r2, #0
005ec90c: str r2, [sp, #0x1c]
005ec910: bne #0x5ec6a4
005ec914: ldr ip, [sp, #0x28]
005ec918: ldr r2, [ip, #0x74]
005ec91c: tst r2, #2
005ec920: bne #0x5ec9a8
005ec924: ldr r1, [sp, #0x2c]
005ec928: cmp r3, #0
005ec92c: str r3, [r1]
005ec930: beq #0x5ec944
005ec934: ldr r2, [r3, #4]
005ec938: add r2, r2, #1
005ec93c: str r2, [r3, #4]
005ec940: ldr r3, [sp, #0x54]
005ec944: cmp r3, #0
005ec948: beq #0x5ec954
005ec94c: mov r0, r3
005ec950: bl #0x31d584
005ec954: ldr r0, [sp, #0x2c]
005ec958: add sp, sp, #0x5c
005ec95c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005ec960: ldr r1, [sp, #0x2c]
005ec964: mov r2, #0
005ec968: cmp r8, #0
005ec96c: str r2, [r1]
005ec970: beq #0x5ec97c
005ec974: mov r0, r4
005ec978: bl #0x5fdc0c
005ec97c: cmp r4, #0
005ec980: beq #0x5ec940
005ec984: mov r0, r4
005ec988: bl #0x31d584
005ec98c: ldr r3, [sp, #0x54]
005ec990: b #0x5ec944
005ec994: ldr ip, [sp, #0x28]
005ec998: ldr r3, [sp, #0x54]
005ec99c: ldr r2, [ip, #0x74]
005ec9a0: tst r2, #2
005ec9a4: beq #0x5ec924
005ec9a8: eor r2, r2, #1
005ec9ac: mov r0, r3
005ec9b0: and r1, r2, #1
005ec9b4: bl #0x5fde9c
005ec9b8: ldr r3, [sp, #0x54]
005ec9bc: b #0x5ec924
005ec9c0: mov r0, #0
005ec9c4: bl #0x5ed944
005ec9c8: ldr ip, [r0, r7, lsl #2]
005ec9cc: b #0x5ec660
005ec9d0: mov r0, #0
005ec9d4: bl #0x5ed944
005ec9d8: ldr r3, [sp, #0x54]
005ec9dc: ldr r4, [r0, r7, lsl #2]
005ec9e0: ldr r7, [r3, #0x38]
005ec9e4: ubfx r7, r7, #4, #6
005ec9e8: b #0x5ec650
005ec9ec: ldr ip, [sp, #0x28]
005ec9f0: ldr r1, [ip, #0x74]
005ec9f4: tst r1, #1
005ec9f8: bne #0x5ec688
005ec9fc: ldr r1, [r3, #0x20]
005eca00: bl #0x5edaec
005eca04: ldr r2, [sl]
005eca08: ldr r3, [r2, #0x18]
005eca0c: cmp r3, r0
005eca10: beq #0x5eca50
005eca14: ldr r1, [pc, #0x84]
005eca18: mov r2, r5
005eca1c: mov r0, #2
005eca20: add r1, pc, r1
005eca24: bl #0x60b034
005eca28: ldr r3, [sp, #0x54]
005eca2c: ldr r2, [sl]
005eca30: mov r4, r3
005eca34: b #0x5ec688
005eca38: mov r3, r6
005eca3c: ldr r1, [sp, #0x24]
005eca40: mov r2, #0
005eca44: bl #0x5fdf74
005eca48: ldr r3, [sp, #0x54]
005eca4c: b #0x5ec914
005eca50: ldr r3, [sp, #0x54]
005eca54: ldrb r1, [r3, #0x3e]
005eca58: mov r4, r3
005eca5c: cmp r1, #1
005eca60: bls #0x5ec688
005eca64: mov r0, r3
005eca68: bl #0x5e80e8
005eca6c: ldr r2, [sl]
005eca70: ldr r3, [r2, #0x1c]
005eca74: cmp r0, r3
005eca78: ldreq r3, [sp, #0x54]
005eca7c: moveq r4, r3
005eca80: bne #0x5eca14
005eca84: b #0x5ec688
005eca88: eoreq sb, sp, r4, lsl lr
005eca8c: eoreq sb, sp, r4, lsl #28

_ZN6glitch5video6CImageC1ERKN5boost13intrusive_ptrIS1_EERKNS_4core10position2dIiEERKNS7_11dimension2dIiEE
00601b10: push {r4, r5, r6, r7, r8, sl, lr}
00601b14: ldr r6, [pc, #0xfc]
00601b18: ldr r7, [pc, #0xfc]
00601b1c: mov r5, #0
00601b20: add r6, pc, r6
00601b24: ldr r7, [r6, r7]
00601b28: mov ip, #1
00601b2c: str r5, [r0, #4]
00601b30: add r7, r7, #8
00601b34: str r7, [r0]
00601b38: mov r7, #0x27
00601b3c: str r7, [r0, #0x20]
00601b40: str r5, [r0, #8]
00601b44: str r5, [r0, #0xc]
00601b48: str r5, [r0, #0x10]
00601b4c: str r5, [r0, #0x14]
00601b50: str r5, [r0, #0x18]
00601b54: str r5, [r0, #0x1c]
00601b58: str r5, [r0, #0x24]
00601b5c: strb r5, [r0, #0x28]
00601b60: strb ip, [r0, #0x29]
00601b64: mov r7, r1
00601b68: ldr r1, [r1]
00601b6c: sub sp, sp, #0x1c
00601b70: mov r4, r0
00601b74: cmp r1, r5
00601b78: mov r8, r2
00601b7c: mov sl, r3
00601b80: beq #0x601c0c
00601b84: ldr r3, [r1, #0x20]
00601b88: mov r1, ip
00601b8c: str r3, [r0, #0x20]
00601b90: ldr r3, [sl]
00601b94: str r3, [r0, #0x10]
00601b98: ldr r3, [sl, #4]
00601b9c: str r3, [r0, #0x14]
00601ba0: ldr r3, [r7]
00601ba4: ldrb r3, [r3, #0x28]
00601ba8: strb r3, [r0, #0x28]
00601bac: bl #0x601988
00601bb0: ldr r2, [pc, #0x68]
00601bb4: ldr r3, [r7]
00601bb8: ldr r0, [r4, #0x20]
00601bbc: ldr r1, [r6, r2]
00601bc0: mov ip, #0x28
00601bc4: ldr r2, [r3, #0x18]
00601bc8: mla r1, ip, r0, r1
00601bcc: ldr r3, [r3, #8]
00601bd0: ldr ip, [r8, #4]
00601bd4: ldrb r1, [r1, #0x15]
00601bd8: ldr r8, [r8]
00601bdc: mla r3, ip, r2, r3
00601be0: ldr r6, [r4, #8]
00601be4: ldr ip, [sl, #4]
00601be8: ldr lr, [r4, #0x18]
00601bec: ldr r7, [sl]
00601bf0: mla r1, r8, r1, r3
00601bf4: mov r3, r0
00601bf8: stm sp, {r6, lr}
00601bfc: str r7, [sp, #8]
00601c00: str ip, [sp, #0xc]
00601c04: str r5, [sp, #0x10]
00601c08: bl #0x5f95ac
00601c0c: mov r0, r4
00601c10: add sp, sp, #0x1c
00601c14: pop {r4, r5, r6, r7, r8, sl, pc}
00601c18: eorseq r2, sb, r0, ror pc
00601c1c: andeq r0, r0, r0, lsl r6
00601c20: andeq r1, r0, r4, lsr pc

_ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKN5boost13intrusive_ptrIS1_EE
00601d38: ldr r3, [pc, #0x168]
00601d3c: ldr ip, [pc, #0x168]
00601d40: push {r4, r5, r6, r7, lr}
00601d44: add r3, pc, r3
00601d48: ldr ip, [r3, ip]
00601d4c: mov r5, #0
00601d50: mov r6, #1
00601d54: add ip, ip, #8
00601d58: str ip, [r0]
00601d5c: str r1, [r0, #0x20]
00601d60: str r5, [r0, #4]
00601d64: str r5, [r0, #8]
00601d68: str r5, [r0, #0xc]
00601d6c: str r5, [r0, #0x10]
00601d70: str r5, [r0, #0x14]
00601d74: str r5, [r0, #0x18]
00601d78: str r5, [r0, #0x1c]
00601d7c: str r5, [r0, #0x24]
00601d80: strb r5, [r0, #0x28]
00601d84: strb r6, [r0, #0x29]
00601d88: mov r7, r2
00601d8c: ldr r2, [r2]
00601d90: sub sp, sp, #0x1c
00601d94: mov r4, r0
00601d98: cmp r2, r5
00601d9c: beq #0x601e9c
00601da0: ldr r3, [r2, #0x10]
00601da4: mov r1, r6
00601da8: str r3, [r0, #0x10]
00601dac: ldr r3, [r2, #0x14]
00601db0: str r3, [r0, #0x14]
00601db4: ldr r3, [r7]
00601db8: ldrb r3, [r3, #0x28]
00601dbc: strb r3, [r0, #0x28]
00601dc0: bl #0x601988
00601dc4: ldr r1, [r7]
00601dc8: ldr r6, [r4, #0x18]
00601dcc: ldr r7, [r4, #8]
00601dd0: ldr lr, [r4, #0x10]
00601dd4: ldr ip, [r4, #0x14]
00601dd8: ldr r2, [r1, #0x18]
00601ddc: ldr r0, [r1, #0x20]
00601de0: ldr r3, [r4, #0x20]
00601de4: ldr r1, [r1, #8]
00601de8: str r7, [sp]
00601dec: stmib sp, {r6, lr}
00601df0: str ip, [sp, #0xc]
00601df4: str r5, [sp, #0x10]
00601df8: bl #0x5f95ac
00601dfc: cmp r0, r5
00601e00: bne #0x601e9c
00601e04: ldr r0, [r4, #8]
00601e08: mov r3, #0x27
00601e0c: str r3, [r4, #0x20]
00601e10: cmp r0, r5
00601e14: beq #0x601e1c
00601e18: bl #0x30e0b8
00601e1c: ldr r3, [r4, #0xc]
00601e20: mov r6, #0
00601e24: str r6, [r4, #8]
00601e28: cmp r3, r6
00601e2c: beq #0x601e88
00601e30: ldrb r2, [r4, #0x28]
00601e34: cmp r2, r6
00601e38: beq #0x601e80
00601e3c: ldr r0, [r3]
00601e40: cmp r0, r6
00601e44: beq #0x601e80
00601e48: mov r5, #4
00601e4c: mov r7, r6
00601e50: bl #0x30e0b8
00601e54: ldr r3, [r4, #0xc]
00601e58: add r2, r5, #4
00601e5c: str r7, [r3, r6]
00601e60: ldr r3, [r4, #0xc]
00601e64: mov r6, r5
00601e68: ldr r0, [r3, r5]
00601e6c: mov r5, r2
00601e70: cmp r0, #0
00601e74: bne #0x601e50
00601e78: cmp r3, #0
00601e7c: beq #0x601e88
00601e80: mov r0, r3
00601e84: bl #0x30e0b8
00601e88: mov r3, #0
00601e8c: str r3, [r4, #0x1c]
00601e90: strb r3, [r4, #0x29]
00601e94: str r3, [r4, #0x14]
00601e98: str r3, [r4, #0x10]
00601e9c: mov r0, r4
00601ea0: add sp, sp, #0x1c
00601ea4: pop {r4, r5, r6, r7, pc}
00601ea8: eorseq r2, sb, ip, asr #26
00601eac: andeq r0, r0, r0, lsl r6

_ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvjjbb
006027e8: push {r4, r5, r6, r7, r8, sb, sl, lr}
006027ec: ldr lr, [pc, #0x13c]
006027f0: ldr r5, [pc, #0x13c]
006027f4: mov ip, #0
006027f8: add lr, pc, lr
006027fc: ldr r5, [lr, r5]
00602800: str ip, [r0, #4]
00602804: str ip, [r0, #8]
00602808: add r5, r5, #8
0060280c: str r5, [r0]
00602810: str ip, [r0, #0xc]
00602814: ldr r7, [r2]
00602818: sub sp, sp, #8
0060281c: ldrb r6, [sp, #0x30]
00602820: ldr r5, [sp, #0x28]
00602824: str r7, [r0, #0x10]
00602828: ldr r8, [r2, #4]
0060282c: ldrb r2, [sp, #0x34]
00602830: mov r7, r1
00602834: str r8, [r0, #0x14]
00602838: ldr r1, [sp, #0x2c]
0060283c: cmp r6, ip
00602840: mov r4, r0
00602844: str r1, [r0, #0x24]
00602848: strb r2, [r0, #0x29]
0060284c: mov r8, r3
00602850: str r5, [r0, #0x1c]
00602854: str r7, [r0, #0x20]
00602858: strb ip, [r0, #0x28]
0060285c: beq #0x602890
00602860: movw r3, #0xf00d
00602864: movt r3, #0xbad
00602868: str r3, [r0, #8]
0060286c: mov r1, ip
00602870: bl #0x601988
00602874: ldr r3, [r4, #0x24]
00602878: str r8, [r4, #8]
0060287c: cmp r3, #0
00602880: bne #0x6028b4
00602884: mov r0, r4
00602888: add sp, sp, #8
0060288c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00602890: mov r1, r6
00602894: bl #0x601988
00602898: mov r1, r8
0060289c: mov r2, r5
006028a0: ldr r0, [r4, #8]
006028a4: bl #0x30e868
006028a8: ldr r3, [r4, #0x24]
006028ac: cmp r3, #0
006028b0: beq #0x602884
006028b4: add r3, r3, #1
006028b8: lsl r0, r3, #2
006028bc: mov r1, #0
006028c0: bl #0x5341a8
006028c4: ldr r5, [r4, #0x24]
006028c8: str r0, [r4, #0xc]
006028cc: ldr r6, [r4, #8]
006028d0: cmp r5, #0
006028d4: ldr r8, [r4, #0x10]
006028d8: ldr sl, [r4, #0x14]
006028dc: beq #0x602924
006028e0: mov r5, #0
006028e4: mov sb, r5
006028e8: uxtb r3, r5
006028ec: mov r0, r7
006028f0: mov r1, r8
006028f4: mov r2, sl
006028f8: str sb, [sp]
006028fc: bl #0x5edbcc
00602900: ldr r3, [r4, #0xc]
00602904: add r6, r6, r0
00602908: str r6, [r3, r5, lsl #2]
0060290c: ldr r3, [r4, #0x24]
00602910: add r5, r5, #1
00602914: cmp r3, r5
00602918: bhi #0x6028e8
0060291c: ldr r0, [r4, #0xc]
00602920: lsl r5, r5, #2
00602924: mov r3, #0
00602928: str r3, [r0, r5]
0060292c: b #0x602884
00602930: mlaseq sb, r8, r2, r2
00602934: andeq r0, r0, r0, lsl r6
