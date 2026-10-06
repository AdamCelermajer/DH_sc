
# _ZN6CharAI13_SkillCleanUpEv 003d8ae0 size 72
003d8ae0: push {r4, r5, r6, lr}
003d8ae4: ldr r3, [r0, #0xb4]
003d8ae8: ldr r6, [r0, #0xb8]
003d8aec: mov r5, r0
003d8af0: rsb r6, r3, r6
003d8af4: asrs r6, r6, #2
003d8af8: beq #0x3d8b24
003d8afc: mov r4, #0
003d8b00: b #0x3d8b08
003d8b04: ldr r3, [r5, #0xb4]
003d8b08: ldr r0, [r3, r4, lsl #2]
003d8b0c: add r4, r4, #1
003d8b10: cmp r0, #0
003d8b14: beq #0x3d8b1c
003d8b18: bl #0x3daafc
003d8b1c: cmp r4, r6
003d8b20: bne #0x3d8b04
003d8b24: pop {r4, r5, r6, pc}

# _ZNK12CMsgDropLoot11GetDataSizeEv 0031f48c size 12
0031f48c: ldr r0, [r0, #0x64]
0031f490: add r0, r0, #0x18
0031f494: bx lr

# _ZN12CMsgDropLootD0Ev 003243c0 size 60
003243c0: ldr r3, [pc, #0x2c]
003243c4: ldr r2, [pc, #0x2c]
003243c8: push {r4, lr}
003243cc: add r3, pc, r3
003243d0: ldr r2, [r3, r2]
003243d4: mov r4, r0
003243d8: add r2, r2, #8
003243dc: str r2, [r0]
003243e0: bl #0x80a194
003243e4: mov r0, r4
003243e8: bl #0x310440
003243ec: mov r0, r4
003243f0: pop {r4, pc}
003243f4: rsbeq r0, r7, r4, asr #13
003243f8: andeq r0, r0, r0, asr #26

# _ZN7Structs8DropLootD0Ev 004d1f68 size 28
004d1f68: push {r4, lr}
004d1f6c: mov r4, r0
004d1f70: bl #0x4d1f10
004d1f74: mov r0, r4
004d1f78: bl #0x310440
004d1f7c: mov r0, r4
004d1f80: pop {r4, pc}

# _ZN7Structs8DropLoot8finalizeEv 004d1ec4 size 76
004d1ec4: push {r4, lr}
004d1ec8: mov r4, r0
004d1ecc: ldr r0, [r0, #0xc]
004d1ed0: cmp r0, #0
004d1ed4: beq #0x4d1ee8
004d1ed8: bl #0x310440
004d1edc: mov r3, #0
004d1ee0: str r3, [r4, #8]
004d1ee4: str r3, [r4, #0xc]
004d1ee8: ldr r0, [r4, #0x14]
004d1eec: cmp r0, #0
004d1ef0: beq #0x4d1f04
004d1ef4: bl #0x310440
004d1ef8: mov r3, #0
004d1efc: str r3, [r4, #0x10]
004d1f00: str r3, [r4, #0x14]
004d1f04: mov r0, r4
004d1f08: pop {r4, lr}
004d1f0c: b #0x4c6c68

# _ZN6CharAI13_SpellCleanUpEv 003d8a98 size 72
003d8a98: push {r4, r5, r6, lr}
003d8a9c: ldr r3, [r0, #0xc0]
003d8aa0: ldr r6, [r0, #0xc4]
003d8aa4: mov r5, r0
003d8aa8: rsb r6, r3, r6
003d8aac: asrs r6, r6, #2
003d8ab0: beq #0x3d8adc
003d8ab4: mov r4, #0
003d8ab8: b #0x3d8ac0
003d8abc: ldr r3, [r5, #0xc0]
003d8ac0: ldr r0, [r3, r4, lsl #2]
003d8ac4: add r4, r4, #1
003d8ac8: cmp r0, #0
003d8acc: beq #0x3d8ad4
003d8ad0: bl #0x3daafc
003d8ad4: cmp r4, r6
003d8ad8: bne #0x3d8abc
003d8adc: pop {r4, r5, r6, pc}

# _ZN12CMsgDropLoot9WriteDataER12NetBitStream 003204a4 size 88
003204a4: push {r4, r5, r6, r7, r8, lr}
003204a8: mov r7, r0
003204ac: ldr r3, [r7], #0x50
003204b0: mov r5, r1
003204b4: mov r4, r0
003204b8: mov lr, pc
003204bc: ldr pc, [r3, #8]
003204c0: mov r1, r7
003204c4: mov r6, r0
003204c8: mov r2, #0x18
003204cc: mov r0, r5
003204d0: bl #0x80eda8
003204d4: ldr r1, [r4, #0x68]
003204d8: cmp r1, #0
003204dc: beq #0x3204f4
003204e0: ldr r2, [r4, #0x64]
003204e4: cmp r2, #0
003204e8: ble #0x3204f4
003204ec: mov r0, r5
003204f0: bl #0x80eda8
003204f4: mov r0, r6
003204f8: pop {r4, r5, r6, r7, r8, pc}

# _ZN10AISDefault6OnDiedEP10GameObject 003dbe90 size 4
003dbe90: bx lr

# _ZNK12EventManager10RaiseAsyncERK6IEvent 00339090 size 4
00339090: b #0x338ebc

# _ZN7Structs8DropLootD1Ev 004d1f10 size 88
004d1f10: push {r4, lr}
004d1f14: ldr r3, [pc, #0x44]
004d1f18: ldr r2, [pc, #0x44]
004d1f1c: mov r4, r0
004d1f20: add r3, pc, r3
004d1f24: ldr r0, [r0, #0xc]
004d1f28: ldr r2, [r3, r2]
004d1f2c: cmp r0, #0
004d1f30: add r2, r2, #8
004d1f34: str r2, [r4]
004d1f38: beq #0x4d1f40
004d1f3c: bl #0x310440
004d1f40: ldr r0, [r4, #0x14]
004d1f44: cmp r0, #0
004d1f48: beq #0x4d1f50
004d1f4c: bl #0x310440
004d1f50: mov r0, r4
004d1f54: bl #0x4c6c60
004d1f58: mov r0, r4
004d1f5c: pop {r4, pc}
004d1f60: subeq r2, ip, r0, ror fp
004d1f64: andeq r1, r0, r0, lsl #14

# _ZN12CMsgDropLootD1Ev 0031ffa8 size 52
0031ffa8: ldr r3, [pc, #0x24]
0031ffac: ldr r2, [pc, #0x24]
0031ffb0: push {r4, lr}
0031ffb4: add r3, pc, r3
0031ffb8: ldr r2, [r3, r2]
0031ffbc: mov r4, r0
0031ffc0: add r2, r2, #8
0031ffc4: str r2, [r0]
0031ffc8: bl #0x80a194
0031ffcc: mov r0, r4
0031ffd0: pop {r4, pc}

# _ZN9AISPlayer9OnDeAggroEP9Character 003dde48 size 668
003dde48: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dde4c: ldr r4, [pc, #0x264]
003dde50: ldr r6, [pc, #0x264]
003dde54: ldr r7, [pc, #0x264]
003dde58: add r4, pc, r4
003dde5c: ldr r3, [r4, r6]
003dde60: sub sp, sp, #0x4c
003dde64: mov sb, r1
003dde68: ldr r3, [r3]
003dde6c: mov r5, r0
003dde70: add r8, sp, #0x2c
003dde74: str r3, [sp, #0x44]
003dde78: bl #0x3dbea8
003dde7c: ldr sl, [r4, r7]
003dde80: mov r0, sl
003dde84: bl #0x337888
003dde88: ldr r1, [pc, #0x234]
003dde8c: add r2, sp, #0x10
003dde90: mov r0, r8
003dde94: add r1, pc, r1
003dde98: bl #0x3140ec
003dde9c: mov r0, sl
003ddea0: mov r1, r8
003ddea4: bl #0x337a88
003ddea8: mov sl, r0
003ddeac: ldr r0, [sp, #0x40]
003ddeb0: cmp r0, r8
003ddeb4: beq #0x3dded4
003ddeb8: cmp r0, #0
003ddebc: beq #0x3dded4
003ddec0: ldr r1, [sp, #0x2c]
003ddec4: rsb r1, r0, r1
003ddec8: cmp r1, #0x80
003ddecc: bhi #0x3de054
003dded0: bl #0x708f00
003dded4: cmp sl, #0
003dded8: bne #0x3de030
003ddedc: ldr r3, [r5, #0xd0]
003ddee0: sub r3, r3, #1
003ddee4: str r3, [r5, #0xd0]
003ddee8: bl #0x7fd794
003ddeec: ldrb r3, [r0, #5]
003ddef0: cmp r3, #0
003ddef4: bne #0x3de010
003ddef8: ldr r8, [pc, #0x1c8]
003ddefc: ldr r3, [pc, #0x1c8]
003ddf00: ldr fp, [r4, r8]
003ddf04: ldr r3, [r4, r3]
003ddf08: mov r0, fp
003ddf0c: ldr r8, [r3]
003ddf10: bl #0x31f594
003ddf14: mov r0, sb
003ddf18: ldr sl, [r5, #0xd4]
003ddf1c: bl #0x3a3024
003ddf20: ldr r3, [r0, #0x14]
003ddf24: rsb sl, r3, sl
003ddf28: str sl, [r5, #0xd4]
003ddf2c: ldrb r3, [r8, #0x31]
003ddf30: cmp r3, #0
003ddf34: bne #0x3ddf64
003ddf38: cmp sl, #0
003ddf3c: bne #0x3ddf64
003ddf40: ldr r1, [pc, #0x188]
003ddf44: mov r0, r8
003ddf48: mov sb, #1
003ddf4c: add r1, pc, r1
003ddf50: bl #0x369514
003ddf54: ldrb r3, [r8, #0x32]
003ddf58: strb sb, [r8, #0x31]
003ddf5c: cmp r3, #0
003ddf60: bne #0x3de084
003ddf64: ldr r3, [r5, #0xd0]
003ddf68: cmp r3, #0
003ddf6c: beq #0x3de05c
003ddf70: ldr r8, [r4, r7]
003ddf74: add r7, sp, #0x14
003ddf78: mov r0, r8
003ddf7c: bl #0x337888
003ddf80: ldr r1, [pc, #0x14c]
003ddf84: add r2, sp, #0xc
003ddf88: mov r0, r7
003ddf8c: add r1, pc, r1
003ddf90: bl #0x3140ec
003ddf94: mov r0, r8
003ddf98: mov r1, r7
003ddf9c: bl #0x337a88
003ddfa0: mov r8, r0
003ddfa4: ldr r0, [sp, #0x28]
003ddfa8: cmp r0, r7
003ddfac: beq #0x3ddfcc
003ddfb0: cmp r0, #0
003ddfb4: beq #0x3ddfcc
003ddfb8: ldr r1, [sp, #0x14]
003ddfbc: rsb r1, r0, r1
003ddfc0: cmp r1, #0x80
003ddfc4: bhi #0x3de07c
003ddfc8: bl #0x708f00
003ddfcc: cmp r8, #0
003ddfd0: beq #0x3ddff4
003ddfd4: ldr r0, [pc, #0xfc]
003ddfd8: ldr r1, [pc, #0xfc]
003ddfdc: ldr r3, [r5, #0xd4]
003ddfe0: ldr r0, [r4, r0]
003ddfe4: add r1, pc, r1
003ddfe8: ldr r2, [r5, #0xd0]
003ddfec: add r0, r0, #0xa8
003ddff0: bl #0x30e004
003ddff4: ldr r3, [r4, r6]
003ddff8: ldr r2, [sp, #0x44]
003ddffc: ldr r3, [r3]
003de000: cmp r2, r3
003de004: bne #0x3de0b4
003de008: add sp, sp, #0x4c
003de00c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003de010: ldr r8, [pc, #0xb0]
003de014: ldr r1, [r5, #0x98]
003de018: ldr r3, [r4, r8]
003de01c: ldr r0, [r3, #0x40]
003de020: bl #0x36effc
003de024: cmp r0, #0
003de028: beq #0x3ddff4
003de02c: b #0x3ddefc
003de030: ldr r0, [pc, #0xa0]
003de034: ldr r1, [pc, #0xa4]
003de038: ldr r2, [r5, #0xd0]
003de03c: ldr r0, [r4, r0]
003de040: add r1, pc, r1
003de044: ldr r3, [r5, #0xd4]
003de048: add r0, r0, #0xa8
003de04c: bl #0x30e004
003de050: b #0x3ddedc
003de054: bl #0x310440
003de058: b #0x3dded4
003de05c: ldrb r3, [r8, #0x32]
003de060: cmp r3, #0
003de064: bne #0x3ddf70
003de068: ldr r3, [r5, #0xc4]
003de06c: ldr r2, [r5, #0xc8]
003de070: cmp r3, r2
003de074: strne r3, [r5, #0xc8]
003de078: b #0x3ddf70
003de07c: bl #0x310440
003de080: b #0x3ddfcc
003de084: mov r0, fp
003de088: bl #0x31f594
003de08c: ldr r1, [r0, #0x120]
003de090: cmp r1, #0
003de094: blt #0x3ddf64
003de098: mov ip, #0x7d0
003de09c: mov r2, sb
003de0a0: mov r3, sl
003de0a4: mov r0, r8
003de0a8: str ip, [sp]
003de0ac: bl #0x36bd78
003de0b0: b #0x3ddf64
003de0b4: bl #0x30e310
003de0b8: subseq r6, fp, r8, lsr ip
003de0bc: andeq r4, r0, ip, lsr #1
003de0c0: andeq r0, r0, r4, lsl #17
003de0c4: subeq r7, lr, r4, lsl sp
003de0c8: strdeq r3, r4, [r0], -r4
003de0cc: andeq r0, r0, r4, lsr #27
003de0d0: ldrdeq r3, r4, [lr], #-0xc4
003de0d4: subeq r7, lr, ip, lsl ip
003de0d8: andeq r1, r0, r0, asr #19
003de0dc: umaaleq r7, lr, r4, ip
003de0e0: subeq r7, lr, r0, lsl #24

# _ZN6CharAI10AI_SetDeadEv 003d6cdc size 140
003d6cdc: mov r1, #0
003d6ce0: push {r4, lr}
003d6ce4: mov r2, r1
003d6ce8: mov r4, r0
003d6cec: bl #0x3d6890
003d6cf0: mov r0, r4
003d6cf4: bl #0x3d49c4
003d6cf8: ldr r0, [r4, #4]
003d6cfc: mov r1, #0
003d6d00: mov r2, r1
003d6d04: add r0, r0, #0x4f0
003d6d08: mov r3, #1
003d6d0c: add r0, r0, #0xc
003d6d10: bl #0x3c58c8
003d6d14: ldr r0, [r4, #4]
003d6d18: ldr r1, [r4, #0x10]
003d6d1c: add r0, r0, #0x3b4
003d6d20: bl #0x3db2d8
003d6d24: ldr r0, [r4, #4]
003d6d28: ldr r1, [r4, #0x14]
003d6d2c: add r0, r0, #0x3b4
003d6d30: bl #0x3db2d8
003d6d34: mvn r3, #0
003d6d38: str r3, [r4, #0x14]
003d6d3c: str r3, [r4, #0x10]
003d6d40: mov r0, r4
003d6d44: bl #0x3d5fa8
003d6d48: mov r0, r4
003d6d4c: mov r1, #0
003d6d50: bl #0x3d6abc
003d6d54: mov r0, r4
003d6d58: bl #0x3d8ae0
003d6d5c: mov r0, r4
003d6d60: pop {r4, lr}
003d6d64: b #0x3d8a98

# _ZN12CMsgDropLoot9ResetDataEv 0031fb84 size 60
0031fb84: push {r4, r5, r6, lr}
0031fb88: mov r4, r0
0031fb8c: ldr r0, [r0, #0x68]
0031fb90: mov r5, #0
0031fb94: strb r5, [r4, #0x50]
0031fb98: cmp r0, r5
0031fb9c: str r5, [r4, #0x54]
0031fba0: str r5, [r4, #0x58]
0031fba4: str r5, [r4, #0x5c]
0031fba8: str r5, [r4, #0x60]
0031fbac: str r5, [r4, #0x64]
0031fbb0: beq #0x31fbbc
0031fbb4: bl #0x310440
0031fbb8: str r5, [r4, #0x68]
0031fbbc: pop {r4, r5, r6, pc}

# _ZN12CMsgDropLoot8ReadDataER12NetBitStream 003203bc size 120
003203bc: push {r4, r5, r6, r7, r8, lr}
003203c0: mov r7, r0
003203c4: ldr r3, [r7], #0x50
003203c8: mov r5, r1
003203cc: mov r4, r0
003203d0: mov lr, pc
003203d4: ldr pc, [r3, #8]
003203d8: mov r1, r7
003203dc: mov r6, r0
003203e0: mov r2, #0x18
003203e4: mov r0, r5
003203e8: bl #0x80ec28
003203ec: ldr r0, [r4, #0x68]
003203f0: cmp r0, #0
003203f4: beq #0x320404
003203f8: bl #0x310440
003203fc: mov r3, #0
00320400: str r3, [r4, #0x68]
00320404: ldr r0, [r4, #0x64]
00320408: cmp r0, #0
0032040c: ble #0x32042c
00320410: mov r1, #2
00320414: bl #0x31056c
00320418: ldr r2, [r4, #0x64]
0032041c: mov r1, r0
00320420: str r0, [r4, #0x68]
00320424: mov r0, r5
00320428: bl #0x80ec28
0032042c: mov r0, r6
00320430: pop {r4, r5, r6, r7, r8, pc}

# _ZN10AISDefault9OnDeAggroEP9Character 003dbea8 size 4
003dbea8: bx lr

# _ZN6CharAI17AI_SyncLastTargetEv 003d49c4 size 12
003d49c4: ldr r3, [r0, #0x40]
003d49c8: str r3, [r0, #0x44]
003d49cc: bx lr

# _Z27GetNewScriptCmdImplInstanceI15Script_DropLootEP13ScriptCmdImplv 00457fec size 72
00457fec: push {r4, lr}
00457ff0: mov r1, #0
00457ff4: mov r0, #0x10
00457ff8: bl #0x310570
00457ffc: ldr r4, [pc, #0x28]
00458000: ldr r2, [pc, #0x28]
00458004: mov r1, #0
00458008: add r4, pc, r4
0045800c: ldr r2, [r4, r2]
00458010: str r1, [r0, #0xc]
00458014: strb r1, [r0, #4]
00458018: add r2, r2, #8
0045801c: str r2, [r0]
00458020: mvn r2, #0
00458024: str r2, [r0, #8]
00458028: pop {r4, pc}
0045802c: subseq ip, r3, r8, lsl #21
00458030: andeq r3, r0, r8, ror #19

# _ZNK9Character8DropLootEP10GameObject 003a5ae4 size 52
003a5ae4: push {r4, r5, lr}
003a5ae8: mov r4, r1
003a5aec: sub sp, sp, #0xc
003a5af0: mov r5, r0
003a5af4: bl #0x3a2fcc
003a5af8: mov ip, #0
003a5afc: mov r1, r5
003a5b00: mov r2, r4
003a5b04: mvn r3, #0
003a5b08: str ip, [sp]
003a5b0c: bl #0x3ecba0
003a5b10: add sp, sp, #0xc
003a5b14: pop {r4, r5, pc}

# _ZN6CharAI6OnDiedEP10GameObject 003d1000 size 80
003d1000: push {r4, r5, r6, lr}
003d1004: mov r4, r0
003d1008: ldr r0, [r0, #0x34]
003d100c: mov r5, r1
003d1010: cmp r0, #0
003d1014: beq #0x3d1024
003d1018: ldr r1, [r4, #4]
003d101c: mov r2, r5
003d1020: bl #0x3d2628
003d1024: ldr r3, [r4, #0x1c]
003d1028: cmp r3, #0
003d102c: beq #0x3d1044
003d1030: mov r0, r3
003d1034: mov r1, r5
003d1038: ldr r3, [r3]
003d103c: mov lr, pc
003d1040: ldr pc, [r3, #0x24]
003d1044: mov r0, r4
003d1048: pop {r4, r5, r6, lr}
003d104c: b #0x3d6cdc

# _ZN6CharAIC2Ev 003cebf0 size 352
003cebf0: ldr r3, [pc, #0x14c]
003cebf4: ldr r2, [pc, #0x14c]
003cebf8: push {r4, r5, lr}
003cebfc: add r3, pc, r3
003cec00: ldr r2, [r3, r2]
003cec04: mov r4, r0
003cec08: mov r1, #0
003cec0c: add r2, r2, #8
003cec10: str r2, [r4]
003cec14: ldr r2, [pc, #0x130]
003cec18: mov r0, #1
003cec1c: mvn ip, #0
003cec20: mov r5, r4
003cec24: strb r0, [r4, #0x55]
003cec28: str r1, [r4, #8]
003cec2c: str r1, [r4, #0xc]
003cec30: strb r1, [r4, #0x18]
003cec34: str r1, [r4, #0x1c]
003cec38: str r1, [r4, #0x20]
003cec3c: strb r1, [r4, #0x24]
003cec40: str r1, [r4, #0x28]
003cec44: strb r1, [r4, #0x2c]
003cec48: str r1, [r4, #0x30]
003cec4c: str r1, [r4, #0x34]
003cec50: str r1, [r4, #0x3c]
003cec54: str r1, [r4, #0x40]
003cec58: str r1, [r4, #0x44]
003cec5c: strb r1, [r4, #0x49]
003cec60: strb r0, [r4, #0x4a]
003cec64: strb r0, [r4, #0x4b]
003cec68: strb r1, [r4, #0x4c]
003cec6c: strb r0, [r4, #0x4d]
003cec70: str r1, [r4, #0x50]
003cec74: strb r0, [r4, #0x54]
003cec78: str r1, [r4, #0x58]
003cec7c: mov r0, r4
003cec80: str r1, [r4, #0x60]
003cec84: str ip, [r4, #0x10]
003cec88: str ip, [r4, #0x14]
003cec8c: str ip, [r4, #0x38]
003cec90: strb r1, [r5, #0x5c]!
003cec94: str r5, [r4, #0x68]
003cec98: str r5, [r4, #0x64]
003cec9c: str r1, [r4, #0x6c]
003ceca0: str r1, [r4, #0x80]
003ceca4: strb r1, [r0, #0x7c]!
003ceca8: ldr r5, [r3, r2]
003cecac: mov r2, r4
003cecb0: str r0, [r4, #0x88]
003cecb4: str r0, [r4, #0x84]
003cecb8: str r1, [r4, #0x8c]
003cecbc: str r1, [r4, #0x98]
003cecc0: add r0, r4, #0xac
003cecc4: strb r1, [r2, #0x94]!
003cecc8: str r2, [r4, #0xa0]
003ceccc: str r0, [r4, #0xb0]
003cecd0: str ip, [r4, #0xcc]
003cecd4: strb r1, [r4, #0xd1]
003cecd8: str r2, [r4, #0x9c]
003cecdc: str r1, [r4, #0xa4]
003cece0: str r0, [r4, #0xac]
003cece4: str r1, [r4, #0xb4]
003cece8: str r1, [r4, #0xb8]
003cecec: str r1, [r4, #0xbc]
003cecf0: str r1, [r4, #0xc0]
003cecf4: str r1, [r4, #0xc4]
003cecf8: str r1, [r4, #0xc8]
003cecfc: strb r1, [r4, #0xd0]
003ced00: ldr r1, [r5, #0x18]
003ced04: ldr r2, [r5, #0x10]
003ced08: sub sp, sp, #0xc
003ced0c: sub r3, r1, #4
003ced10: cmp r2, r3
003ced14: str r4, [sp, #4]
003ced18: beq #0x3ced38
003ced1c: str r4, [r2]
003ced20: ldr r3, [r5, #0x10]
003ced24: add r3, r3, #4
003ced28: str r3, [r5, #0x10]
003ced2c: mov r0, r4
003ced30: add sp, sp, #0xc
003ced34: pop {r4, r5, pc}
003ced38: add r0, sp, #4
003ced3c: bl #0x3ce810
003ced40: b #0x3ced2c

# _ZNK15Script_DropLoot10IsBlockingEv 004557e8 size 8
004557e8: mov r0, #0
004557ec: bx lr

# _ZN6CharAI16AI_ClearAllAggroEv 003d5fa8 size 480
003d5fa8: push {r4, r5, r6, r7, r8, sl, lr}
003d5fac: mov r3, #0
003d5fb0: sub sp, sp, #0x14
003d5fb4: mov r5, r0
003d5fb8: ldr r1, [r5, #0x8c]
003d5fbc: mov r0, sp
003d5fc0: str r3, [sp, #8]
003d5fc4: str r3, [sp]
003d5fc8: str r3, [sp, #4]
003d5fcc: ldr r4, [r5, #0x84]
003d5fd0: bl #0x3d5e14
003d5fd4: mov sl, sp
003d5fd8: add r6, r5, #0x7c
003d5fdc: add r7, r5, #4
003d5fe0: add r8, sp, #0xc
003d5fe4: cmp r4, r6
003d5fe8: beq #0x3d60a4
003d5fec: ldr r0, [r4, #0x10]
003d5ff0: ldr r3, [r0, #0x460]
003d5ff4: cmp r3, #0
003d5ff8: beq #0x3d6058
003d5ffc: add r0, r0, #0x450
003d6000: add r0, r0, #0xc
003d6004: ldr ip, [r7]
003d6008: mov r1, r0
003d600c: b #0x3d6014
003d6010: mov r3, r2
003d6014: ldr r2, [r3, #0x10]
003d6018: cmp r2, ip
003d601c: ldrlo r2, [r3, #0xc]
003d6020: ldrhs r2, [r3, #8]
003d6024: movlo r3, r1
003d6028: mov r1, r3
003d602c: cmp r2, #0
003d6030: bne #0x3d6010
003d6034: cmp r0, r3
003d6038: beq #0x3d6058
003d603c: ldr r1, [r5, #4]
003d6040: ldr r2, [r3, #0x10]
003d6044: cmp r1, r2
003d6048: blo #0x3d6058
003d604c: mov r1, r8
003d6050: str r3, [sp, #0xc]
003d6054: bl #0x3d5d9c
003d6058: ldmib sp, {r1, r3}
003d605c: cmp r1, r3
003d6060: beq #0x3d614c
003d6064: ldr r3, [r4, #0x10]
003d6068: str r3, [r1]
003d606c: ldr r3, [sp, #4]
003d6070: add r3, r3, #4
003d6074: str r3, [sp, #4]
003d6078: ldr r2, [r4, #0xc]
003d607c: cmp r2, #0
003d6080: bne #0x3d608c
003d6084: b #0x3d6118
003d6088: mov r2, r3
003d608c: ldr r3, [r2, #8]
003d6090: cmp r3, #0
003d6094: bne #0x3d6088
003d6098: mov r4, r2
003d609c: cmp r4, r6
003d60a0: bne #0x3d5fec
003d60a4: ldr r3, [r5, #0x8c]
003d60a8: cmp r3, #0
003d60ac: bne #0x3d615c
003d60b0: ldm sp, {r0, r3}
003d60b4: rsb r3, r0, r3
003d60b8: lsrs r3, r3, #2
003d60bc: beq #0x3d60f0
003d60c0: mov r4, #0
003d60c4: ldr r3, [r0, r4, lsl #2]
003d60c8: ldr r1, [r5, #4]
003d60cc: add r4, r4, #1
003d60d0: add r0, r3, #0x3c8
003d60d4: ldr r3, [r3, #0x3c8]
003d60d8: mov lr, pc
003d60dc: ldr pc, [r3, #0x3c]
003d60e0: ldm sp, {r0, r3}
003d60e4: rsb r3, r0, r3
003d60e8: cmp r4, r3, asr #2
003d60ec: blo #0x3d60c4
003d60f0: cmp r0, #0
003d60f4: beq #0x3d6110
003d60f8: ldr r1, [sp, #8]
003d60fc: rsb r1, r0, r1
003d6100: bic r1, r1, #3
003d6104: cmp r1, #0x80
003d6108: bhi #0x3d6180
003d610c: bl #0x708f00
003d6110: add sp, sp, #0x14
003d6114: pop {r4, r5, r6, r7, r8, sl, pc}
003d6118: ldr r3, [r4, #4]
003d611c: ldr r1, [r3, #0xc]
003d6120: cmp r4, r1
003d6124: bne #0x3d6140
003d6128: mov r4, r3
003d612c: ldr r3, [r3, #4]
003d6130: ldr r2, [r3, #0xc]
003d6134: cmp r2, r4
003d6138: beq #0x3d6128
003d613c: ldr r2, [r4, #0xc]
003d6140: cmp r3, r2
003d6144: movne r4, r3
003d6148: b #0x3d5fe4
003d614c: mov r0, sp
003d6150: add r2, r4, #0x10
003d6154: bl #0x3d5ee0
003d6158: b #0x3d6078
003d615c: mov r0, r4
003d6160: ldr r1, [r5, #0x80]
003d6164: bl #0x3cd34c
003d6168: mov r3, #0
003d616c: str r4, [r5, #0x88]
003d6170: str r3, [r5, #0x8c]
003d6174: str r4, [r5, #0x84]
003d6178: str r3, [r5, #0x80]
003d617c: b #0x3d60b0
003d6180: bl #0x310440
003d6184: b #0x3d6110

# _ZN6CharAI24AI_ClearAllAggroTowardMeEb 003d6abc size 544
003d6abc: push {r4, r5, r6, r7, r8, sb, sl, lr}
003d6ac0: mov r3, #0
003d6ac4: sub sp, sp, #0x10
003d6ac8: mov r5, r0
003d6acc: mov r7, r1
003d6ad0: mov r0, sp
003d6ad4: ldr r1, [r5, #0xa4]
003d6ad8: str r3, [sp, #8]
003d6adc: str r3, [sp]
003d6ae0: str r3, [sp, #4]
003d6ae4: ldr r4, [r5, #0x9c]
003d6ae8: bl #0x3d5e14
003d6aec: mov sb, sp
003d6af0: add r6, r5, #0x94
003d6af4: add r8, r5, #4
003d6af8: add sl, sp, #0xc
003d6afc: cmp r4, r6
003d6b00: beq #0x3d6bd4
003d6b04: cmp r7, #0
003d6b08: beq #0x3d6c64
003d6b0c: ldr r0, [r4, #0x10]
003d6b10: ldr r2, [r5, #4]
003d6b14: ldr r3, [r0, #0x408]
003d6b18: cmp r2, r3
003d6b1c: beq #0x3d6c48
003d6b20: ldr r3, [r0, #0x448]
003d6b24: cmp r3, #0
003d6b28: beq #0x3d6b88
003d6b2c: add r0, r0, #0x440
003d6b30: add r0, r0, #4
003d6b34: ldr ip, [r8]
003d6b38: mov r1, r0
003d6b3c: b #0x3d6b44
003d6b40: mov r3, r2
003d6b44: ldr r2, [r3, #0x10]
003d6b48: cmp r2, ip
003d6b4c: ldrlo r2, [r3, #0xc]
003d6b50: ldrhs r2, [r3, #8]
003d6b54: movlo r3, r1
003d6b58: mov r1, r3
003d6b5c: cmp r2, #0
003d6b60: bne #0x3d6b40
003d6b64: cmp r0, r3
003d6b68: beq #0x3d6b88
003d6b6c: ldr r1, [r5, #4]
003d6b70: ldr r2, [r3, #0x10]
003d6b74: cmp r1, r2
003d6b78: blo #0x3d6b88
003d6b7c: mov r1, sl
003d6b80: str r3, [sp, #0xc]
003d6b84: bl #0x3d5d9c
003d6b88: ldmib sp, {r1, r3}
003d6b8c: cmp r1, r3
003d6b90: beq #0x3d6ca0
003d6b94: ldr r3, [r4, #0x10]
003d6b98: str r3, [r1]
003d6b9c: ldr r3, [sp, #4]
003d6ba0: add r3, r3, #4
003d6ba4: str r3, [sp, #4]
003d6ba8: ldr r2, [r4, #0xc]
003d6bac: cmp r2, #0
003d6bb0: bne #0x3d6bbc
003d6bb4: b #0x3d6c6c
003d6bb8: mov r2, r3
003d6bbc: ldr r3, [r2, #8]
003d6bc0: cmp r3, #0
003d6bc4: bne #0x3d6bb8
003d6bc8: mov r4, r2
003d6bcc: cmp r4, r6
003d6bd0: bne #0x3d6b04
003d6bd4: ldr r3, [r5, #0xa4]
003d6bd8: cmp r3, #0
003d6bdc: bne #0x3d6cb0
003d6be0: ldm sp, {r0, r3}
003d6be4: rsb r3, r0, r3
003d6be8: lsrs r3, r3, #2
003d6bec: beq #0x3d6c20
003d6bf0: mov r4, #0
003d6bf4: ldr r3, [r5, #4]
003d6bf8: ldr r1, [r0, r4, lsl #2]
003d6bfc: add r4, r4, #1
003d6c00: add r0, r3, #0x3c8
003d6c04: ldr r3, [r3, #0x3c8]
003d6c08: mov lr, pc
003d6c0c: ldr pc, [r3, #0x3c]
003d6c10: ldm sp, {r0, r3}
003d6c14: rsb r3, r0, r3
003d6c18: cmp r4, r3, asr #2
003d6c1c: blo #0x3d6bf4
003d6c20: cmp r0, #0
003d6c24: beq #0x3d6c40
003d6c28: ldr r1, [sp, #8]
003d6c2c: rsb r1, r0, r1
003d6c30: bic r1, r1, #3
003d6c34: cmp r1, #0x80
003d6c38: bhi #0x3d6cd4
003d6c3c: bl #0x708f00
003d6c40: add sp, sp, #0x10
003d6c44: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003d6c48: mov r1, #0
003d6c4c: add r0, r0, #0x3c8
003d6c50: mov r2, r1
003d6c54: bl #0x3d6890
003d6c58: ldr r0, [r4, #0x10]
003d6c5c: add r0, r0, #0x3c8
003d6c60: bl #0x3d49c4
003d6c64: ldr r0, [r4, #0x10]
003d6c68: b #0x3d6b20
003d6c6c: ldr r3, [r4, #4]
003d6c70: ldr r1, [r3, #0xc]
003d6c74: cmp r4, r1
003d6c78: bne #0x3d6c94
003d6c7c: mov r4, r3
003d6c80: ldr r3, [r3, #4]
003d6c84: ldr r2, [r3, #0xc]
003d6c88: cmp r2, r4
003d6c8c: beq #0x3d6c7c
003d6c90: ldr r2, [r4, #0xc]
003d6c94: cmp r3, r2
003d6c98: movne r4, r3
003d6c9c: b #0x3d6afc
003d6ca0: mov r0, sp
003d6ca4: add r2, r4, #0x10
003d6ca8: bl #0x3d5ee0
003d6cac: b #0x3d6ba8
003d6cb0: mov r0, r4
003d6cb4: ldr r1, [r5, #0x98]
003d6cb8: bl #0x3cd34c
003d6cbc: mov r3, #0
003d6cc0: str r4, [r5, #0xa0]
003d6cc4: str r3, [r5, #0xa4]
003d6cc8: str r4, [r5, #0x9c]
003d6ccc: str r3, [r5, #0x98]
003d6cd0: b #0x3d6be0
003d6cd4: bl #0x310440
003d6cd8: b #0x3d6c40

# _ZN6CharAI13ClearAllAggroEv 003cd384 size 456
003cd384: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003cd388: ldr sl, [pc, #0x1b4]
003cd38c: ldr sb, [pc, #0x1b4]
003cd390: sub sp, sp, #0x2c
003cd394: add sl, pc, sl
003cd398: ldr r3, [sl, sb]
003cd39c: mov r6, #0
003cd3a0: ldr r8, [r3, #0x38]
003cd3a4: ldr r5, [r8, #0x60]!
003cd3a8: cmp r8, r5
003cd3ac: beq #0x3cd400
003cd3b0: ldr r4, [r5, #8]
003cd3b4: cmp r4, #0
003cd3b8: addne r4, r4, #0x3c8
003cd3bc: ldr r3, [r4, #0x8c]
003cd3c0: cmp r3, #0
003cd3c4: beq #0x3cd3e8
003cd3c8: add r7, r4, #0x7c
003cd3cc: mov r0, r7
003cd3d0: ldr r1, [r4, #0x80]
003cd3d4: bl #0x3cd34c
003cd3d8: str r7, [r4, #0x88]
003cd3dc: str r7, [r4, #0x84]
003cd3e0: str r6, [r4, #0x80]
003cd3e4: str r6, [r4, #0x8c]
003cd3e8: ldr r3, [r4, #0xa4]
003cd3ec: cmp r3, #0
003cd3f0: bne #0x3cd4ec
003cd3f4: ldr r5, [r5]
003cd3f8: cmp r8, r5
003cd3fc: bne #0x3cd3b0
003cd400: ldr r3, [sl, sb]
003cd404: add r6, sp, #0x1c
003cd408: mov r0, r6
003cd40c: ldr r3, [r3, #0x38]
003cd410: add r8, sp, #8
003cd414: add sb, r8, #4
003cd418: ldr r4, [r3, #4]!
003cd41c: add fp, sb, #4
003cd420: mov r7, r6
003cd424: str r3, [sp, #4]
003cd428: bl #0x33f50c
003cd42c: ldr r3, [sp, #4]
003cd430: mov r0, r8
003cd434: cmp r3, r4
003cd438: beq #0x3cd4e4
003cd43c: ldr r3, [r4, #8]
003cd440: subs r1, r3, #0
003cd444: beq #0x3cd4d0
003cd448: bl #0x33dd2c
003cd44c: ldr r2, [r8]
003cd450: mov r3, r6
003cd454: mov r1, #0
003cd458: str r2, [r3], #4
003cd45c: ldr r2, [sb]
003cd460: mov r0, r7
003cd464: str r2, [r6, #4]
003cd468: ldr r2, [fp]
003cd46c: mov r6, r7
003cd470: str r2, [r3, #4]
003cd474: bl #0x33fdc0
003cd478: cmp r0, #0
003cd47c: mov r0, r7
003cd480: beq #0x3cd4d0
003cd484: bl #0x33ff54
003cd488: subs r5, r0, #0
003cd48c: beq #0x3cd4d0
003cd490: ldr r3, [r5, #0x454]
003cd494: cmp r3, #0
003cd498: beq #0x3cd4c4
003cd49c: add sl, r5, #0x440
003cd4a0: add sl, sl, #4
003cd4a4: mov r0, sl
003cd4a8: ldr r1, [r5, #0x448]
003cd4ac: bl #0x3cd34c
003cd4b0: mov r3, #0
003cd4b4: str sl, [r5, #0x450]
003cd4b8: str sl, [r5, #0x44c]
003cd4bc: str r3, [r5, #0x448]
003cd4c0: str r3, [r5, #0x454]
003cd4c4: ldr r3, [r5, #0x46c]
003cd4c8: cmp r3, #0
003cd4cc: bne #0x3cd514
003cd4d0: ldr r4, [r4]
003cd4d4: ldr r3, [sp, #4]
003cd4d8: mov r0, r8
003cd4dc: cmp r3, r4
003cd4e0: bne #0x3cd43c
003cd4e4: add sp, sp, #0x2c
003cd4e8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003cd4ec: add r7, r4, #0x94
003cd4f0: mov r0, r7
003cd4f4: ldr r1, [r4, #0x98]
003cd4f8: bl #0x3cd34c
003cd4fc: str r7, [r4, #0xa0]
003cd500: str r6, [r4, #0xa4]
003cd504: str r7, [r4, #0x9c]
003cd508: str r6, [r4, #0x98]
003cd50c: ldr r5, [r5]
003cd510: b #0x3cd3f8
003cd514: add sl, r5, #0x450
003cd518: add sl, sl, #0xc
003cd51c: mov r0, sl
003cd520: ldr r1, [r5, #0x460]
003cd524: bl #0x3cd34c
003cd528: mov r3, #0
003cd52c: str sl, [r5, #0x468]
003cd530: str r3, [r5, #0x46c]
003cd534: str sl, [r5, #0x464]
003cd538: str r3, [r5, #0x460]
003cd53c: ldr r4, [r4]
003cd540: b #0x3cd4d4
003cd544: ldrsheq r7, [ip], #-0x6c
003cd548: strdeq r3, r4, [r0], -r4

# _ZN6CharAI9GroupInfo6OnDiedEP9CharacterP10GameObject 003d2628 size 420
003d2628: push {r4, r5, r6, r7, r8, lr}
003d262c: ldr r3, [r1, #0x400]
003d2630: mov r5, r0
003d2634: mov r6, r2
003d2638: cmp r3, #2
003d263c: beq #0x3d2654
003d2640: cmp r3, #1
003d2644: beq #0x3d26e0
003d2648: cmp r3, #3
003d264c: beq #0x3d2758
003d2650: pop {r4, r5, r6, r7, r8, pc}
003d2654: ldr r3, [r0, #0xc]
003d2658: ldr r7, [r0, #0x10]
003d265c: mov r2, #1
003d2660: strb r2, [r0, #0x28]
003d2664: rsb r7, r3, r7
003d2668: asrs r7, r7, #2
003d266c: beq #0x3d269c
003d2670: mov r4, #0
003d2674: b #0x3d267c
003d2678: ldr r3, [r5, #0xc]
003d267c: ldr r3, [r3, r4, lsl #2]
003d2680: mov r1, r6
003d2684: add r4, r4, #1
003d2688: ldr r0, [r3, #0x378]
003d268c: mov r2, #0
003d2690: bl #0x40570c
003d2694: cmp r4, r7
003d2698: bne #0x3d2678
003d269c: ldr r3, [r5, #0x18]
003d26a0: ldr r7, [r5, #0x1c]
003d26a4: rsb r7, r3, r7
003d26a8: asrs r7, r7, #2
003d26ac: beq #0x3d2650
003d26b0: mov r4, #0
003d26b4: b #0x3d26bc
003d26b8: ldr r3, [r5, #0x18]
003d26bc: ldr r3, [r3, r4, lsl #2]
003d26c0: mov r1, r6
003d26c4: add r4, r4, #1
003d26c8: ldr r0, [r3, #0x378]
003d26cc: mov r2, #0
003d26d0: bl #0x40570c
003d26d4: cmp r4, r7
003d26d8: bne #0x3d26b8
003d26dc: pop {r4, r5, r6, r7, r8, pc}
003d26e0: ldrb r4, [r0, #0x28]
003d26e4: cmp r4, #0
003d26e8: bne #0x3d2650
003d26ec: ldr r7, [r0, #0x10]
003d26f0: ldr r2, [r0, #0xc]
003d26f4: strb r3, [r0, #0x28]
003d26f8: rsb r7, r2, r7
003d26fc: asrs r7, r7, #2
003d2700: beq #0x3d2650
003d2704: mov r6, r3
003d2708: b #0x3d271c
003d270c: add r4, r4, #1
003d2710: cmp r4, r7
003d2714: strb r6, [r5, #0x28]
003d2718: beq #0x3d2754
003d271c: cmp r6, #0
003d2720: beq #0x3d270c
003d2724: ldr r3, [r5, #0xc]
003d2728: ldr r3, [r3, r4, lsl #2]
003d272c: add r4, r4, #1
003d2730: mov r0, r3
003d2734: ldr r3, [r3]
003d2738: mov lr, pc
003d273c: ldr pc, [r3, #0x34]
003d2740: cmp r0, #0
003d2744: moveq r6, #0
003d2748: cmp r4, r7
003d274c: strb r6, [r5, #0x28]
003d2750: bne #0x3d271c
003d2754: pop {r4, r5, r6, r7, r8, pc}
003d2758: ldr r4, [r0, #0x24]
003d275c: cmp r4, #0
003d2760: bne #0x3d2650
003d2764: ldr r7, [r0, #0x1c]
003d2768: ldr r3, [r0, #0x18]
003d276c: rsb r7, r3, r7
003d2770: asrs r7, r7, #2
003d2774: moveq r6, #1
003d2778: beq #0x3d27c4
003d277c: mov r6, #1
003d2780: b #0x3d2790
003d2784: add r4, r4, #1
003d2788: cmp r4, r7
003d278c: beq #0x3d27c4
003d2790: cmp r6, #0
003d2794: beq #0x3d2784
003d2798: ldr r3, [r5, #0x18]
003d279c: ldr r3, [r3, r4, lsl #2]
003d27a0: add r4, r4, #1
003d27a4: mov r0, r3
003d27a8: ldr r3, [r3]
003d27ac: mov lr, pc
003d27b0: ldr pc, [r3, #0x34]
003d27b4: cmp r0, #0
003d27b8: moveq r6, #0
003d27bc: cmp r4, r7
003d27c0: bne #0x3d2790
003d27c4: str r6, [r5, #0x24]
003d27c8: pop {r4, r5, r6, r7, r8, pc}

# _Z14GetNewInstanceI12CMsgDropLootEP8CMessageb 00328d08 size 40
00328d08: push {r4, r5, r6, lr}
00328d0c: mov r1, #2
00328d10: mov r5, r0
00328d14: mov r0, #0x6c
00328d18: bl #0x310570
00328d1c: mov r1, r5
00328d20: mov r4, r0
00328d24: bl #0x328c84
00328d28: mov r0, r4
00328d2c: pop {r4, r5, r6, pc}

# _ZN6CharAI9OnDeAggroEP9Character 003d2014 size 180
003d2014: push {r4, r5, r6, r7, r8, sl, lr}
003d2018: ldr r4, [pc, #0x98]
003d201c: ldr r6, [pc, #0x98]
003d2020: ldr r2, [pc, #0x98]
003d2024: add r4, pc, r4
003d2028: ldr r3, [r4, r6]
003d202c: ldr r7, [r4, r2]
003d2030: sub sp, sp, #0x24
003d2034: ldr r3, [r3]
003d2038: mov r8, r0
003d203c: mov r0, r7
003d2040: str r3, [sp, #0x1c]
003d2044: mov sl, r1
003d2048: bl #0x337888
003d204c: ldr r1, [pc, #0x70]
003d2050: add r5, sp, #4
003d2054: mov r2, sp
003d2058: add r1, pc, r1
003d205c: mov r0, r5
003d2060: bl #0x3140ec
003d2064: mov r1, r5
003d2068: mov r0, r7
003d206c: bl #0x337a88
003d2070: mov r0, r5
003d2074: bl #0x3139ac
003d2078: ldr r3, [r8, #0x1c]
003d207c: cmp r3, #0
003d2080: beq #0x3d2098
003d2084: mov r0, r3
003d2088: mov r1, sl
003d208c: ldr r3, [r3]
003d2090: mov lr, pc
003d2094: ldr pc, [r3, #0x3c]
003d2098: ldr r3, [r4, r6]
003d209c: ldr r2, [sp, #0x1c]
003d20a0: ldr r3, [r3]
003d20a4: cmp r2, r3
003d20a8: bne #0x3d20b4
003d20ac: add sp, sp, #0x24
003d20b0: pop {r4, r5, r6, r7, r8, sl, pc}
003d20b4: bl #0x30e310
003d20b8: subseq r2, ip, ip, ror #20
003d20bc: andeq r4, r0, ip, lsr #1
003d20c0: andeq r0, r0, r4, lsl #17
003d20c4: umaaleq r3, pc, r0, r4

# _ZN15Script_DropLoot7ExecuteEbi 0045e374 size 324
0045e374: push {r4, r5, r6, r7, r8, sb, sl, lr}
0045e378: ldr r4, [pc, #0x124]
0045e37c: ldr sb, [pc, #0x124]
0045e380: ldr r1, [pc, #0x124]
0045e384: add r4, pc, r4
0045e388: ldr r3, [r4, sb]
0045e38c: ldr r6, [r4, r1]
0045e390: sub sp, sp, #0x40
0045e394: ldr r3, [r3]
0045e398: mov sl, r2
0045e39c: add r5, sp, #0x24
0045e3a0: str r3, [sp, #0x3c]
0045e3a4: ldr r7, [r0, #0xc]
0045e3a8: mov r0, r6
0045e3ac: bl #0x337888
0045e3b0: ldr r1, [pc, #0xf8]
0045e3b4: add r2, sp, #0x20
0045e3b8: mov r0, r5
0045e3bc: add r1, pc, r1
0045e3c0: ldr r8, [pc, #0xec]
0045e3c4: bl #0x3140ec
0045e3c8: mov r1, r5
0045e3cc: mov r0, r6
0045e3d0: bl #0x337a88
0045e3d4: mov r0, r5
0045e3d8: bl #0x318254
0045e3dc: ldr r3, [r4, r8]
0045e3e0: add r6, sp, #0x14
0045e3e4: ldr r2, [r7, #0xc]
0045e3e8: ldr r1, [r3, #0x38]
0045e3ec: mov r5, #0
0045e3f0: mov r3, sl
0045e3f4: mov r0, r6
0045e3f8: str r5, [sp]
0045e3fc: str r5, [sp, #4]
0045e400: bl #0x34aca0
0045e404: mov r1, r5
0045e408: mov r0, r6
0045e40c: bl #0x33fdc0
0045e410: subs r5, r0, #0
0045e414: bne #0x45e480
0045e418: ldr r3, [r4, r8]
0045e41c: add r6, sp, #8
0045e420: ldr r2, [r7, #0x14]
0045e424: ldr r1, [r3, #0x38]
0045e428: mov r7, #0
0045e42c: mov r3, sl
0045e430: mov r0, r6
0045e434: str r7, [sp]
0045e438: str r7, [sp, #4]
0045e43c: bl #0x34aca0
0045e440: mov r1, r7
0045e444: mov r0, r6
0045e448: bl #0x33fdc0
0045e44c: subs r1, r0, #0
0045e450: bne #0x45e490
0045e454: cmp r5, #0
0045e458: beq #0x45e464
0045e45c: mov r0, r5
0045e460: bl #0x3a5ae4
0045e464: ldr r3, [r4, sb]
0045e468: ldr r2, [sp, #0x3c]
0045e46c: ldr r3, [r3]
0045e470: cmp r2, r3
0045e474: bne #0x45e4a0
0045e478: add sp, sp, #0x40
0045e47c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045e480: mov r0, r6
0045e484: bl #0x33ff54
0045e488: mov r5, r0
0045e48c: b #0x45e418
0045e490: mov r0, r6
0045e494: bl #0x33fee4
0045e498: mov r1, r0
0045e49c: b #0x45e454
0045e4a0: bl #0x30e310
0045e4a4: subseq r6, r3, ip, lsl #14
0045e4a8: andeq r4, r0, ip, lsr #1
0045e4ac: andeq r0, r0, r4, lsl #17
0045e4b0: subeq lr, r6, r4, asr #25
0045e4b4: strdeq r3, r4, [r0], -r4

# _ZN10ItemObject13DropLootTableEiPK10GameObjectS2_ib 003ecba0 size 296
003ecba0: push {r4, r5, r6, r7, r8, sl, lr}
003ecba4: subs r5, r2, #0
003ecba8: sub sp, sp, #0x54
003ecbac: mov r8, r0
003ecbb0: mov r6, r1
003ecbb4: mov r7, r3
003ecbb8: moveq r4, r5
003ecbbc: beq #0x3ecbe8
003ecbc0: add r4, sp, #0x44
003ecbc4: mov r0, r4
003ecbc8: mov r1, r5
003ecbcc: bl #0x33dd70
003ecbd0: mov r0, r4
003ecbd4: mov r1, #0
003ecbd8: bl #0x33ff8c
003ecbdc: subs r4, r0, #0
003ecbe0: bne #0x3ecc1c
003ecbe4: mov r4, #0
003ecbe8: cmp r6, #0
003ecbec: beq #0x3ecc14
003ecbf0: add sl, sp, #0x38
003ecbf4: mov r1, r6
003ecbf8: mov r0, sl
003ecbfc: bl #0x33dd70
003ecc00: mov r0, sl
003ecc04: mov r1, #0
003ecc08: bl #0x33ff8c
003ecc0c: subs r3, r0, #0
003ecc10: bne #0x3ecc2c
003ecc14: mov sl, #0
003ecc18: b #0x3ecc9c
003ecc1c: ldr r3, [r4, #0xf4]
003ecc20: cmp r3, #0
003ecc24: beq #0x3ecbe8
003ecc28: b #0x3ecbe4
003ecc2c: ldr r2, [r3, #0xf4]
003ecc30: cmp r2, #0
003ecc34: bne #0x3ecc14
003ecc38: mov sl, r3
003ecc3c: bl #0x3a3158
003ecc40: cmp r0, #0
003ecc44: beq #0x3ecc8c
003ecc48: mov r0, sp
003ecc4c: bl #0x3ff200
003ecc50: mov r0, sp
003ecc54: mov r1, r8
003ecc58: mov r2, r5
003ecc5c: mov r3, r7
003ecc60: bl #0x3ecae4
003ecc64: mov r0, sp
003ecc68: mov r1, r6
003ecc6c: mov r2, r5
003ecc70: mov r3, r7
003ecc74: bl #0x3ec8a0
003ecc78: mov r0, sp
003ecc7c: mov r4, sp
003ecc80: bl #0x3ff460
003ecc84: add sp, sp, #0x54
003ecc88: pop {r4, r5, r6, r7, r8, sl, pc}
003ecc8c: mov r0, sl
003ecc90: bl #0x3a3144
003ecc94: cmp r0, #0
003ecc98: bne #0x3ecc48
003ecc9c: cmp r4, #0
003ecca0: beq #0x3ecc84
003ecca4: ldr r2, [r4]
003ecca8: mov r0, r4
003eccac: mov lr, pc
003eccb0: ldr pc, [r2, #0x28]
003eccb4: cmp r0, #0
003eccb8: bne #0x3ecc48
003eccbc: cmp sl, r4
003eccc0: bne #0x3ecc84
003eccc4: b #0x3ecc48

# _Z27GetNewScriptCmdDataInstanceIN7Structs8DropLootEEPNS0_9ScriptCmdEv 00456afc size 64
00456afc: push {r4, lr}
00456b00: mov r1, #0
00456b04: mov r0, #0x18
00456b08: bl #0x310570
00456b0c: ldr r4, [pc, #0x20]
00456b10: ldr r2, [pc, #0x20]
00456b14: mov r1, #0
00456b18: add r4, pc, r4
00456b1c: ldr r2, [r4, r2]
00456b20: str r1, [r0, #0x14]
00456b24: str r1, [r0, #0xc]
00456b28: add r2, r2, #8
00456b2c: str r2, [r0]
00456b30: pop {r4, pc}
00456b34: subseq sp, r3, r8, ror pc
00456b38: andeq r1, r0, r0, lsl #14

# _ZN10ItemObject13DropLootTableEiPK10GameObjectS2_PK9Characteri 003eccc8 size 124
003eccc8: push {r4, r5, r6, r7, r8, lr}
003ecccc: subs r5, r2, #0
003eccd0: sub sp, sp, #0x48
003eccd4: mov r8, r0
003eccd8: mov r7, r1
003eccdc: mov r6, r3
003ecce0: beq #0x3ecd00
003ecce4: add r4, sp, #0x3c
003ecce8: mov r1, r5
003eccec: mov r0, r4
003eccf0: bl #0x33dd70
003eccf4: mov r0, r4
003eccf8: mov r1, #0
003eccfc: bl #0x33ff8c
003ecd00: add r4, sp, #4
003ecd04: mov r0, r4
003ecd08: bl #0x3ff200
003ecd0c: mov r0, r4
003ecd10: mov r1, r8
003ecd14: mov r2, r5
003ecd18: ldr r3, [sp, #0x60]
003ecd1c: bl #0x3ecae4
003ecd20: mov r0, r4
003ecd24: mov r1, r7
003ecd28: mov r2, r5
003ecd2c: mov r3, r6
003ecd30: bl #0x3ec974
003ecd34: mov r0, r4
003ecd38: bl #0x3ff460
003ecd3c: add sp, sp, #0x48
003ecd40: pop {r4, r5, r6, r7, r8, pc}

# _ZN7Structs8DropLoot4readEP11IStreamBase 00500bb0 size 356
00500bb0: push {r4, r5, r6, lr}
00500bb4: mov r4, r0
00500bb8: sub sp, sp, #8
00500bbc: mov r5, r1
00500bc0: bl #0x4ff828
00500bc4: mov r0, r5
00500bc8: add r1, r4, #8
00500bcc: bl #0x3df1a0
00500bd0: mov r3, #1
00500bd4: cmp r3, #0
00500bd8: str r3, [sp, #4]
00500bdc: bne #0x500c20
00500be0: add r3, r4, #9
00500be4: add r2, r4, #0xa
00500be8: ldrb r0, [r2, #1]
00500bec: ldrb r1, [r3, #-1]
00500bf0: cmp r3, r2
00500bf4: eor r1, r0, r1
00500bf8: strb r1, [r3, #-1]
00500bfc: ldrb r0, [r2, #1]
00500c00: eor r1, r1, r0
00500c04: strb r1, [r2, #1]
00500c08: ldrb r0, [r3, #-1]
00500c0c: sub r2, r2, #1
00500c10: eor r1, r1, r0
00500c14: strb r1, [r3, #-1]
00500c18: add r3, r3, #1
00500c1c: blo #0x500be8
00500c20: ldr r0, [r4, #0xc]
00500c24: cmp r0, #0
00500c28: beq #0x500c30
00500c2c: bl #0x310440
00500c30: ldr r0, [r4, #8]
00500c34: mov r1, #1
00500c38: mov r6, #0
00500c3c: add r0, r0, r1
00500c40: bl #0x31056c
00500c44: ldr r2, [r4, #8]
00500c48: mov r1, r0
00500c4c: str r0, [r4, #0xc]
00500c50: mov r3, r6
00500c54: mov r0, r5
00500c58: bl #0x317454
00500c5c: ldr r3, [r4, #8]
00500c60: ldr r2, [r4, #0xc]
00500c64: mov r0, r5
00500c68: add r1, r4, #0x10
00500c6c: strb r6, [r2, r3]
00500c70: bl #0x3df1a0
00500c74: mov r3, #1
00500c78: cmp r3, r6
00500c7c: str r3, [sp, #4]
00500c80: bne #0x500cc4
00500c84: add r3, r4, #0x11
00500c88: add r2, r4, #0x12
00500c8c: ldrb r0, [r2, #1]
00500c90: ldrb r1, [r3, #-1]
00500c94: cmp r3, r2
00500c98: eor r1, r0, r1
00500c9c: strb r1, [r3, #-1]
00500ca0: ldrb r0, [r2, #1]
00500ca4: eor r1, r1, r0
00500ca8: strb r1, [r2, #1]
00500cac: ldrb r0, [r3, #-1]
00500cb0: sub r2, r2, #1
00500cb4: eor r1, r1, r0
00500cb8: strb r1, [r3, #-1]
00500cbc: add r3, r3, #1
00500cc0: blo #0x500c8c
00500cc4: ldr r0, [r4, #0x14]
00500cc8: cmp r0, #0
00500ccc: beq #0x500cd4
00500cd0: bl #0x310440
00500cd4: ldr r0, [r4, #0x10]
00500cd8: mov r1, #1
00500cdc: mov r6, #0
00500ce0: add r0, r0, r1
00500ce4: bl #0x31056c
00500ce8: ldr r2, [r4, #0x10]
00500cec: mov r1, r0
00500cf0: str r0, [r4, #0x14]
00500cf4: mov r3, r6
00500cf8: mov r0, r5
00500cfc: bl #0x317454
00500d00: ldr r3, [r4, #0x10]
00500d04: ldr r2, [r4, #0x14]
00500d08: strb r6, [r2, r3]
00500d0c: add sp, sp, #8
00500d10: pop {r4, r5, r6, pc}

# _ZN12CMsgDropLoot13SetPropertiesEv 00327908 size 52
00327908: ldr r1, [pc, #0x28]
0032790c: push {r4, lr}
00327910: add r1, pc, r1
00327914: mov r4, r0
00327918: add r2, r1, #0xc
0032791c: add r0, r0, #0x14
00327920: bl #0x3109e0
00327924: mov r3, #0
00327928: strb r3, [r4, #0x33]
0032792c: mov r3, #1
00327930: str r3, [r4, #0x2c]
00327934: pop {r4, pc}
00327938: subseq r7, sb, r8, ror r5

# _ZN12CMsgDropLoot10GetDataPtrEv 0031f484 size 8
0031f484: add r0, r0, #0x50
0031f488: bx lr

# _ZN11AISExternal6OnDiedEP10GameObject 003dd440 size 80
003dd440: push {r4, r5, r6, lr}
003dd444: sub sp, sp, #8
003dd448: mov r5, r0
003dd44c: mov r6, r1
003dd450: mov r0, sp
003dd454: bl #0x3192b4
003dd458: mov r0, sp
003dd45c: mov r1, r6
003dd460: bl #0x386f28
003dd464: ldr r1, [pc, #0x20]
003dd468: mov r0, r5
003dd46c: mov r2, sp
003dd470: add r1, pc, r1
003dd474: bl #0x37c41c
003dd478: mov r0, sp
003dd47c: mov r4, sp
003dd480: bl #0x319228
003dd484: add sp, sp, #8
003dd488: pop {r4, r5, r6, pc}
003dd48c: subeq r8, lr, r8, lsl r7

# _ZN12CMsgDropLootC1Eb 00328c84 size 132
00328c84: push {r4, r5, r6, lr}
00328c88: ldr r5, [pc, #0x6c]
00328c8c: mov r2, r1
00328c90: ldr r6, [pc, #0x68]
00328c94: add r5, pc, r5
00328c98: mov r1, r5
00328c9c: mov r4, r0
00328ca0: bl #0x80a540
00328ca4: ldr r3, [pc, #0x58]
00328ca8: add r6, pc, r6
00328cac: mov r1, r5
00328cb0: ldr r3, [r6, r3]
00328cb4: mov r5, #0
00328cb8: strb r5, [r4, #0x50]
00328cbc: add r3, r3, #8
00328cc0: str r3, [r4]
00328cc4: str r5, [r4, #0x54]
00328cc8: str r5, [r4, #0x58]
00328ccc: str r5, [r4, #0x5c]
00328cd0: str r5, [r4, #0x60]
00328cd4: str r5, [r4, #0x64]
00328cd8: str r5, [r4, #0x68]
00328cdc: add r0, r4, #0x14
00328ce0: add r2, r1, #0xc
00328ce4: bl #0x3109e0
00328ce8: mov r3, #1
00328cec: str r3, [r4, #0x2c]
00328cf0: strb r5, [r4, #0x33]
00328cf4: mov r0, r4
00328cf8: pop {r4, r5, r6, pc}
00328cfc: ldrsheq r6, [sb], #-0x14
00328d00: rsbeq fp, r6, r8, ror #27
00328d04: andeq r0, r0, r0, asr #26

# _ZN7Structs8DropLootD2Ev 004d1f84 size 88
004d1f84: push {r4, lr}
004d1f88: ldr r3, [pc, #0x44]
004d1f8c: ldr r2, [pc, #0x44]
004d1f90: mov r4, r0
004d1f94: add r3, pc, r3
004d1f98: ldr r0, [r0, #0xc]
004d1f9c: ldr r2, [r3, r2]
004d1fa0: cmp r0, #0
004d1fa4: add r2, r2, #8
004d1fa8: str r2, [r4]
004d1fac: beq #0x4d1fb4
004d1fb0: bl #0x310440
004d1fb4: ldr r0, [r4, #0x14]
004d1fb8: cmp r0, #0
004d1fbc: beq #0x4d1fc4
004d1fc0: bl #0x310440
004d1fc4: mov r0, r4
004d1fc8: bl #0x4c6c60
004d1fcc: mov r0, r4
004d1fd0: pop {r4, pc}
004d1fd4: strdeq r2, r3, [ip], #-0xac
004d1fd8: andeq r1, r0, r0, lsl #14
