
# _ZN13ConditionData11SetAsTestedEb 0033dd24 size8
0033dd24: strb r1, [r0, #0x20]
0033dd28: bx lr

# _ZN11IStreamBase6readAsIbEEvRT_ 0033e040 size176
0033e040: str lr, [sp, #-4]!
0033e044: mov r3, #0
0033e048: sub sp, sp, #0xc
0033e04c: ldr ip, [r0]
0033e050: mov r2, #1
0033e054: mov lr, pc
0033e058: ldr pc, [ip, #0x18]
0033e05c: ldr r3, [pc, #0x74]
0033e060: cmp r0, #1
0033e064: add r3, pc, r3
0033e068: beq #0x33e098
0033e06c: ldr r2, [pc, #0x68]
0033e070: ldr r2, [r3, r2]
0033e074: ldr r2, [r2]
0033e078: cmp r2, #2
0033e07c: moveq r3, #0
0033e080: streq r3, [r3]
0033e084: beq #0x33e090
0033e088: cmp r2, #1
0033e08c: beq #0x33e0a4
0033e090: add sp, sp, #0xc
0033e094: ldm sp!, {pc}
0033e098: cmp r1, #0
0033e09c: beq #0x33e090
0033e0a0: b #0x33e06c
0033e0a4: ldr r0, [pc, #0x34]
0033e0a8: ldr r1, [pc, #0x34]
0033e0ac: ldr r2, [pc, #0x34]
0033e0b0: ldr r0, [r3, r0]
0033e0b4: ldr r3, [pc, #0x30]
0033e0b8: mov ip, #0x45
0033e0bc: add r1, pc, r1
0033e0c0: add r2, pc, r2
0033e0c4: add r3, pc, r3
0033e0c8: add r0, r0, #0xa8
0033e0cc: str ip, [sp]
0033e0d0: bl #0x30e004
0033e0d4: b #0x33e090
0033e0d8: rsbeq r6, r5, ip, lsr #20
0033e0dc: andeq r3, r0, r0, asr #19
0033e0e0: andeq r1, r0, r0, asr #19
0033e0e4: subseq r0, r8, ip, lsl r3
0033e0e8: subseq r0, r8, r0, asr #8
0033e0ec: subseq r0, r8, r4, asr r4

# _ZN11IStreamBase7writeAsIbEEvRKT_ 0033e138 size176
0033e138: str lr, [sp, #-4]!
0033e13c: mov r3, #0
0033e140: sub sp, sp, #0xc
0033e144: ldr ip, [r0]
0033e148: mov r2, #1
0033e14c: mov lr, pc
0033e150: ldr pc, [ip, #0x1c]
0033e154: ldr r3, [pc, #0x74]
0033e158: cmp r0, #1
0033e15c: add r3, pc, r3
0033e160: beq #0x33e190
0033e164: ldr r2, [pc, #0x68]
0033e168: ldr r2, [r3, r2]
0033e16c: ldr r2, [r2]
0033e170: cmp r2, #2
0033e174: moveq r3, #0
0033e178: streq r3, [r3]
0033e17c: beq #0x33e188
0033e180: cmp r2, #1
0033e184: beq #0x33e19c
0033e188: add sp, sp, #0xc
0033e18c: ldm sp!, {pc}
0033e190: cmp r1, #0
0033e194: beq #0x33e188
0033e198: b #0x33e164
0033e19c: ldr r0, [pc, #0x34]
0033e1a0: ldr r1, [pc, #0x34]
0033e1a4: ldr r2, [pc, #0x34]
0033e1a8: ldr r0, [r3, r0]
0033e1ac: ldr r3, [pc, #0x30]
0033e1b0: mov ip, #0x4d
0033e1b4: add r1, pc, r1
0033e1b8: add r2, pc, r2
0033e1bc: add r3, pc, r3
0033e1c0: add r0, r0, #0xa8
0033e1c4: str ip, [sp]
0033e1c8: bl #0x30e004
0033e1cc: b #0x33e188
0033e1d0: rsbeq r6, r5, r4, lsr sb
0033e1d4: andeq r3, r0, r0, asr #19
0033e1d8: andeq r1, r0, r0, asr #19
0033e1dc: subseq r0, r8, r4, lsr #4
0033e1e0: subseq r0, r8, r8, ror #5
0033e1e4: subseq r0, r8, ip, asr r3

# _ZNK9Character9IsMonsterEv 003a3064 size24
003a3064: push {r4, lr}
003a3068: bl #0x3a3054
003a306c: cmp r0, #4
003a3070: movne r0, #0
003a3074: moveq r0, #1
003a3078: pop {r4, pc}

# _ZN11IStreamBase6readAsIN7Structs19CharacterPropertiesEEEvRT_ 003a3970 size176
003a3970: str lr, [sp, #-4]!
003a3974: mov r3, #0
003a3978: sub sp, sp, #0xc
003a397c: ldr ip, [r0]
003a3980: mov r2, #0x384
003a3984: mov lr, pc
003a3988: ldr pc, [ip, #0x18]
003a398c: ldr r3, [pc, #0x74]
003a3990: cmp r0, #0x384
003a3994: add r3, pc, r3
003a3998: beq #0x3a39c8
003a399c: ldr r2, [pc, #0x68]
003a39a0: ldr r2, [r3, r2]
003a39a4: ldr r2, [r2]
003a39a8: cmp r2, #2
003a39ac: moveq r3, #0
003a39b0: streq r3, [r3]
003a39b4: beq #0x3a39c0
003a39b8: cmp r2, #1
003a39bc: beq #0x3a39d4
003a39c0: add sp, sp, #0xc
003a39c4: ldm sp!, {pc}
003a39c8: cmp r1, #0
003a39cc: beq #0x3a39c0
003a39d0: b #0x3a399c
003a39d4: ldr r0, [pc, #0x34]
003a39d8: ldr r1, [pc, #0x34]
003a39dc: ldr r2, [pc, #0x34]
003a39e0: ldr r0, [r3, r0]
003a39e4: ldr r3, [pc, #0x30]
003a39e8: mov ip, #0x45
003a39ec: add r1, pc, r1
003a39f0: add r2, pc, r2
003a39f4: add r3, pc, r3
003a39f8: add r0, r0, #0xa8
003a39fc: str ip, [sp]
003a3a00: bl #0x30e004
003a3a04: b #0x3a39c0
003a3a08: ldrsheq r1, [pc], #-0xc
003a3a0c: andeq r3, r0, r0, asr #19
003a3a10: andeq r1, r0, r0, asr #19
003a3a14: subseq sl, r1, ip, ror #19
003a3a18: subseq sl, r1, r0, lsl fp
003a3a1c: subseq sl, r1, r4, lsr #22

# _ZN11IStreamBase7writeAsIN7Structs19CharacterPropertiesEEEvRKT_ 003a3ad0 size176
003a3ad0: str lr, [sp, #-4]!
003a3ad4: mov r3, #0
003a3ad8: sub sp, sp, #0xc
003a3adc: ldr ip, [r0]
003a3ae0: mov r2, #0x384
003a3ae4: mov lr, pc
003a3ae8: ldr pc, [ip, #0x1c]
003a3aec: ldr r3, [pc, #0x74]
003a3af0: cmp r0, #0x384
003a3af4: add r3, pc, r3
003a3af8: beq #0x3a3b28
003a3afc: ldr r2, [pc, #0x68]
003a3b00: ldr r2, [r3, r2]
003a3b04: ldr r2, [r2]
003a3b08: cmp r2, #2
003a3b0c: moveq r3, #0
003a3b10: streq r3, [r3]
003a3b14: beq #0x3a3b20
003a3b18: cmp r2, #1
003a3b1c: beq #0x3a3b34
003a3b20: add sp, sp, #0xc
003a3b24: ldm sp!, {pc}
003a3b28: cmp r1, #0
003a3b2c: beq #0x3a3b20
003a3b30: b #0x3a3afc
003a3b34: ldr r0, [pc, #0x34]
003a3b38: ldr r1, [pc, #0x34]
003a3b3c: ldr r2, [pc, #0x34]
003a3b40: ldr r0, [r3, r0]
003a3b44: ldr r3, [pc, #0x30]
003a3b48: mov ip, #0x4d
003a3b4c: add r1, pc, r1
003a3b50: add r2, pc, r2
003a3b54: add r3, pc, r3
003a3b58: add r0, r0, #0xa8
003a3b5c: str ip, [sp]
003a3b60: bl #0x30e004
003a3b64: b #0x3a3b20

# _ZN11IStreamBase6readAsI7Point3DIfEEEvRT_ 003a3a20 size176
003a3a20: str lr, [sp, #-4]!
003a3a24: mov r3, #0
003a3a28: sub sp, sp, #0xc
003a3a2c: ldr ip, [r0]
003a3a30: mov r2, #0xc
003a3a34: mov lr, pc
003a3a38: ldr pc, [ip, #0x18]
003a3a3c: ldr r3, [pc, #0x74]
003a3a40: cmp r0, #0xc
003a3a44: add r3, pc, r3
003a3a48: beq #0x3a3a78
003a3a4c: ldr r2, [pc, #0x68]
003a3a50: ldr r2, [r3, r2]
003a3a54: ldr r2, [r2]
003a3a58: cmp r2, #2
003a3a5c: moveq r3, #0
003a3a60: streq r3, [r3]
003a3a64: beq #0x3a3a70
003a3a68: cmp r2, #1
003a3a6c: beq #0x3a3a84
003a3a70: add sp, sp, #0xc
003a3a74: ldm sp!, {pc}
003a3a78: cmp r1, #0
003a3a7c: beq #0x3a3a70
003a3a80: b #0x3a3a4c
003a3a84: ldr r0, [pc, #0x34]
003a3a88: ldr r1, [pc, #0x34]
003a3a8c: ldr r2, [pc, #0x34]
003a3a90: ldr r0, [r3, r0]
003a3a94: ldr r3, [pc, #0x30]
003a3a98: mov ip, #0x45
003a3a9c: add r1, pc, r1
003a3aa0: add r2, pc, r2
003a3aa4: add r3, pc, r3
003a3aa8: add r0, r0, #0xa8
003a3aac: str ip, [sp]
003a3ab0: bl #0x30e004
003a3ab4: b #0x3a3a70
003a3ab8: subseq r1, pc, ip, asr #32
003a3abc: andeq r3, r0, r0, asr #19
003a3ac0: andeq r1, r0, r0, asr #19
003a3ac4: subseq sl, r1, ip, lsr sb
003a3ac8: subseq sl, r1, r0, ror #20
003a3acc: subseq sl, r1, r4, ror sl

# _ZN11IStreamBase7writeAsI7Point3DIfEEEvRKT_ 003a3b80 size176
003a3b80: str lr, [sp, #-4]!
003a3b84: mov r3, #0
003a3b88: sub sp, sp, #0xc
003a3b8c: ldr ip, [r0]
003a3b90: mov r2, #0xc
003a3b94: mov lr, pc
003a3b98: ldr pc, [ip, #0x1c]
003a3b9c: ldr r3, [pc, #0x74]
003a3ba0: cmp r0, #0xc
003a3ba4: add r3, pc, r3
003a3ba8: beq #0x3a3bd8
003a3bac: ldr r2, [pc, #0x68]
003a3bb0: ldr r2, [r3, r2]
003a3bb4: ldr r2, [r2]
003a3bb8: cmp r2, #2
003a3bbc: moveq r3, #0
003a3bc0: streq r3, [r3]
003a3bc4: beq #0x3a3bd0
003a3bc8: cmp r2, #1
003a3bcc: beq #0x3a3be4
003a3bd0: add sp, sp, #0xc
003a3bd4: ldm sp!, {pc}
003a3bd8: cmp r1, #0
003a3bdc: beq #0x3a3bd0
003a3be0: b #0x3a3bac
003a3be4: ldr r0, [pc, #0x34]
003a3be8: ldr r1, [pc, #0x34]
003a3bec: ldr r2, [pc, #0x34]
003a3bf0: ldr r0, [r3, r0]
003a3bf4: ldr r3, [pc, #0x30]
003a3bf8: mov ip, #0x4d
003a3bfc: add r1, pc, r1
003a3c00: add r2, pc, r2
003a3c04: add r3, pc, r3
003a3c08: add r0, r0, #0xa8
003a3c0c: str ip, [sp]
003a3c10: bl #0x30e004
003a3c14: b #0x3a3bd0
003a3c18: subseq r0, pc, ip, ror #29
003a3c1c: andeq r3, r0, r0, asr #19
003a3c20: andeq r1, r0, r0, asr #19
003a3c24: ldrsbeq sl, [r1], #-0x7c
003a3c28: subseq sl, r1, r0, lsr #17
003a3c2c: subseq sl, r1, r4, lsl sb

# _ZN14CharProperties20PROPS_RemoveAllBuffsEv 003e0af8 size372
003e0af8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e0afc: ldr r2, [pc, #0x160]
003e0b00: sub sp, sp, #0x34
003e0b04: add r3, r0, #0xe10
003e0b08: add r2, pc, r2
003e0b0c: str r2, [sp, #8]
003e0b10: ldr r2, [pc, #0x150]
003e0b14: add r3, r3, #8
003e0b18: str r3, [sp, #4]
003e0b1c: ldr sl, [r0, #0xe20]
003e0b20: mov r7, r0
003e0b24: add sb, sp, #0x20
003e0b28: add r4, sp, #0x10
003e0b2c: str r2, [sp, #0xc]
003e0b30: ldr r3, [sp, #4]
003e0b34: cmp r3, sl
003e0b38: beq #0x3e0bec
003e0b3c: add r5, sl, #0x34
003e0b40: ldm r5, {r0, r1, r2, r3}
003e0b44: stm sb, {r0, r1, r2, r3}
003e0b48: add r0, sl, #0x44
003e0b4c: mov r1, sb
003e0b50: bl #0x3de870
003e0b54: subs r8, r0, #0
003e0b58: beq #0x3e0ba8
003e0b5c: mov r6, #0
003e0b60: ldm r5, {r0, r1, r2, r3}
003e0b64: stm r4, {r0, r1, r2, r3}
003e0b68: mov r1, r6
003e0b6c: mov r0, r4
003e0b70: bl #0x3de8b4
003e0b74: ldr r3, [sp, #0x10]
003e0b78: ldr r0, [r7, #4]
003e0b7c: add r6, r6, #1
003e0b80: ldr fp, [r3]
003e0b84: add r0, r0, #0x3b4
003e0b88: ldr r1, [fp, #0x388]
003e0b8c: bl #0x3db2d8
003e0b90: mov r0, fp
003e0b94: bl #0x4c5740
003e0b98: mov r0, fp
003e0b9c: bl #0x310440
003e0ba0: cmp r6, r8
003e0ba4: bne #0x3e0b60
003e0ba8: ldr r2, [sp, #8]
003e0bac: ldr r3, [sp, #0xc]
003e0bb0: add r1, sl, #0x18
003e0bb4: ldr r0, [r2, r3]
003e0bb8: bl #0x494978
003e0bbc: ldr r2, [sl, #0xc]
003e0bc0: cmp r2, #0
003e0bc4: bne #0x3e0bd0
003e0bc8: b #0x3e0c30
003e0bcc: mov r2, r3
003e0bd0: ldr r3, [r2, #8]
003e0bd4: cmp r3, #0
003e0bd8: bne #0x3e0bcc
003e0bdc: ldr r3, [sp, #4]
003e0be0: mov sl, r2
003e0be4: cmp r3, sl
003e0be8: bne #0x3e0b3c
003e0bec: ldr r3, [r7, #0xe28]
003e0bf0: cmp r3, #0
003e0bf4: beq #0x3e0c1c
003e0bf8: ldr r0, [sp, #4]
003e0bfc: ldr r1, [r7, #0xe1c]
003e0c00: bl #0x3e0ab8
003e0c04: ldr r2, [sp, #4]
003e0c08: mov r3, #0
003e0c0c: str r3, [r7, #0xe28]
003e0c10: str r2, [r7, #0xe24]
003e0c14: str r2, [r7, #0xe20]
003e0c18: str r3, [r7, #0xe1c]
003e0c1c: mov r0, r7
003e0c20: mov r1, #1
003e0c24: bl #0x3e0810
003e0c28: add sp, sp, #0x34
003e0c2c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e0c30: ldr r3, [sl, #4]
003e0c34: ldr r1, [r3, #0xc]
003e0c38: cmp r1, sl
003e0c3c: bne #0x3e0c58
003e0c40: mov sl, r3
003e0c44: ldr r3, [r3, #4]
003e0c48: ldr r2, [r3, #0xc]
003e0c4c: cmp r2, sl
003e0c50: beq #0x3e0c40
003e0c54: ldr r2, [sl, #0xc]
003e0c58: cmp r3, r2
003e0c5c: movne sl, r3
003e0c60: b #0x3e0b30
003e0c64: subseq r3, fp, r8, lsl #31
003e0c68: andeq r1, r0, r8, lsl #22

# _ZN9Character6ReviveEP10GameObjectb 003a59ac size312
003a59ac: push {r4, r5, r6, r7, lr}
003a59b0: movw r3, #0x1449
003a59b4: ldrb r3, [r0, r3]
003a59b8: ldr r5, [pc, #0x11c]
003a59bc: sub sp, sp, #0x24
003a59c0: cmp r3, #0
003a59c4: mov r4, r0
003a59c8: mov r6, r2
003a59cc: add r5, pc, r5
003a59d0: bne #0x3a5a44
003a59d4: mov r2, #1
003a59d8: movw r3, #0x1448
003a59dc: strb r2, [r4, r3]
003a59e0: mov r7, #0
003a59e4: movw r3, #0x1449
003a59e8: strb r7, [r4, r3]
003a59ec: mov r0, r4
003a59f0: bl #0x3b3a70
003a59f4: strb r7, [r4, #0x118]
003a59f8: bl #0x7fd794
003a59fc: ldrb r3, [r0, #5]
003a5a00: cmp r3, r7
003a5a04: mvnne r3, #0
003a5a08: strne r3, [r4, #0x110]
003a5a0c: movne r3, #0
003a5a10: strne r3, [r4, #0x114]
003a5a14: cmp r6, #0
003a5a18: bne #0x3a5ad0
003a5a1c: ldr r3, [r4]
003a5a20: mov r0, r4
003a5a24: mov lr, pc
003a5a28: ldr pc, [r3, #0x28]
003a5a2c: cmp r0, #0
003a5a30: bne #0x3a5a54
003a5a34: add r0, r4, #0x3c8
003a5a38: bl #0x3d8894
003a5a3c: add sp, sp, #0x24
003a5a40: pop {r4, r5, r6, r7, pc}
003a5a44: mov r1, #3
003a5a48: mov r2, #0
003a5a4c: bl #0x3a4d5c
003a5a50: b #0x3a59d4
003a5a54: bl #0x7fd794
003a5a58: ldrb ip, [r0, #5]
003a5a5c: cmp ip, #0
003a5a60: bne #0x3a5a34
003a5a64: ldr r3, [pc, #0x74]
003a5a68: add r2, sp, #0x1c
003a5a6c: ldr r0, [r5, r3]
003a5a70: movw r3, #0x147c
003a5a74: ldr lr, [r4, r3]
003a5a78: movw r3, #0x1474
003a5a7c: ldr r7, [r4, r3]
003a5a80: movw r3, #0x1478
003a5a84: ldr r6, [r4, r3]
003a5a88: add r5, sp, #0x10
003a5a8c: mov r3, ip
003a5a90: mov r1, r5
003a5a94: str r7, [sp, #0x10]
003a5a98: str r6, [sp, #0x14]
003a5a9c: str lr, [sp, #0x1c]
003a5aa0: str lr, [sp, #0x18]
003a5aa4: str ip, [sp]
003a5aa8: str ip, [sp, #4]
003a5aac: str ip, [sp, #8]
003a5ab0: bl #0x525508
003a5ab4: ldr r3, [sp, #0x1c]
003a5ab8: mov r1, r5
003a5abc: mov r0, r4
003a5ac0: mov r2, #1
003a5ac4: str r3, [sp, #0x18]
003a5ac8: bl #0x393db4
003a5acc: b #0x3a5a34
003a5ad0: mov r0, r4
003a5ad4: bl #0x3b4088
003a5ad8: b #0x3a5a1c
003a5adc: subseq pc, lr, r4, asr #1
003a5ae0: andeq r1, r0, r4, lsl #4

# _ZN6CharAI16AI_ScriptCleanUpEv 003cfd7c size104
003cfd7c: push {r4, lr}
003cfd80: mov r4, r0
003cfd84: ldr r0, [r0, #4]
003cfd88: ldr r1, [r4, #0x10]
003cfd8c: add r0, r0, #0x3b4
003cfd90: bl #0x3db2d8
003cfd94: ldr r0, [r4, #4]
003cfd98: ldr r1, [r4, #0x14]
003cfd9c: add r0, r0, #0x3b4
003cfda0: bl #0x3db2d8
003cfda4: ldr r2, [r4, #0x1c]
003cfda8: mvn r3, #0
003cfdac: str r3, [r4, #0x14]
003cfdb0: cmp r2, #0
003cfdb4: str r3, [r4, #0x10]
003cfdb8: beq #0x3cfde0
003cfdbc: mov r0, r4
003cfdc0: bl #0x3d8ae0
003cfdc4: mov r0, r4
003cfdc8: bl #0x3d8a98
003cfdcc: ldr r3, [r4, #0x1c]
003cfdd0: mov r0, r3
003cfdd4: ldr r3, [r3]
003cfdd8: mov lr, pc
003cfddc: ldr pc, [r3, #0x14]
003cfde0: pop {r4, pc}

# _ZN6CharAI13AI_ScriptInitEv 003cfde4 size336
003cfde4: push {r4, r5, r6, r7, lr}
003cfde8: ldr r3, [r0, #4]
003cfdec: sub sp, sp, #0xc
003cfdf0: mov r4, r0
003cfdf4: mov r0, r3
003cfdf8: ldr r3, [r3]
003cfdfc: mov lr, pc
003cfe00: ldr pc, [r3, #0x34]
003cfe04: ldr r5, [pc, #0x110]
003cfe08: cmp r0, #0
003cfe0c: add r5, pc, r5
003cfe10: bne #0x3cfed0
003cfe14: ldr r1, [r4, #0x10]
003cfe18: cmn r1, #1
003cfe1c: beq #0x3cfe2c
003cfe20: ldr r0, [r4, #4]
003cfe24: add r0, r0, #0x3b4
003cfe28: bl #0x3db2d8
003cfe2c: ldr r6, [pc, #0xec]
003cfe30: ldr r1, [pc, #0xec]
003cfe34: ldr r2, [pc, #0xec]
003cfe38: ldr r3, [r5, r6]
003cfe3c: add r1, pc, r1
003cfe40: add r2, pc, r2
003cfe44: ldr r0, [r3, #0x2c]
003cfe48: ldr r7, [r4, #4]
003cfe4c: bl #0x4c4bdc
003cfe50: add r7, r7, #0x3b4
003cfe54: mov r1, r0
003cfe58: mov ip, #0
003cfe5c: mov r0, r7
003cfe60: mvn r2, #0
003cfe64: mov r3, #0x33
003cfe68: str ip, [sp]
003cfe6c: bl #0x3dbe24
003cfe70: ldr r1, [r4, #0x14]
003cfe74: str r0, [r4, #0x10]
003cfe78: cmn r1, #1
003cfe7c: beq #0x3cfe8c
003cfe80: ldr r0, [r4, #4]
003cfe84: add r0, r0, #0x3b4
003cfe88: bl #0x3db2d8
003cfe8c: ldr r3, [r5, r6]
003cfe90: ldr r1, [pc, #0x94]
003cfe94: ldr r2, [pc, #0x94]
003cfe98: ldr r0, [r3, #0x2c]
003cfe9c: add r1, pc, r1
003cfea0: add r2, pc, r2
003cfea4: ldr r5, [r4, #4]
003cfea8: bl #0x4c4bdc
003cfeac: add r5, r5, #0x3b4
003cfeb0: mov r1, r0
003cfeb4: mov ip, #0
003cfeb8: mov r0, r5
003cfebc: mvn r2, #0
003cfec0: mov r3, #0x34
003cfec4: str ip, [sp]
003cfec8: bl #0x3dbe24
003cfecc: str r0, [r4, #0x14]
003cfed0: ldr r3, [r4, #0x1c]
003cfed4: cmp r3, #0
003cfed8: beq #0x3cff14
003cfedc: mov r0, r3
003cfee0: ldr r3, [r3]
003cfee4: mov lr, pc
003cfee8: ldr pc, [r3, #8]
003cfeec: ldr r3, [r4, #0x1c]
003cfef0: mov r0, r3
003cfef4: ldr r3, [r3]
003cfef8: mov lr, pc
003cfefc: ldr pc, [r3, #0xc]
003cff00: ldr r3, [r4, #0x1c]
003cff04: mov r0, r3
003cff08: ldr r3, [r3]
003cff0c: mov lr, pc
003cff10: ldr pc, [r3, #0x10]
003cff14: add sp, sp, #0xc
003cff18: pop {r4, r5, r6, r7, pc}
003cff1c: subseq r4, ip, r4, lsl #25
003cff20: strdeq r3, r4, [r0], -r4
003cff24: subeq r1, pc, r4, lsl sb
003cff28: subeq r5, pc, r8, lsr r6
003cff2c: strheq r1, [pc], #-0x84
003cff30: subeq r5, pc, r0, ror #11

# _ZN6CharAI17AI_SyncLastTargetEv 003d49c4 size12
003d49c4: ldr r3, [r0, #0x40]
003d49c8: str r3, [r0, #0x44]
003d49cc: bx lr

# _ZN16CharStateMachine15SM_SetIdleStateEb 003c1a00 size20
003c1a00: strb r1, [r0, #0x3c]
003c1a04: mvn r2, #0
003c1a08: mov r1, #3
003c1a0c: mov r3, #0
003c1a10: b #0x3c1938

# _ZNK16CharStateMachine11SM_GetStateEv 003c01ac size20
003c01ac: ldr r3, [r0, #0x20]
003c01b0: cmp r3, #0
003c01b4: mvneq r0, #0
003c01b8: ldrne r0, [r3]
003c01bc: bx lr

# _ZNK16CharStateMachine13SM_IsInLimbusEv 003c01c0 size20
003c01c0: push {r4, lr}
003c01c4: bl #0x3c01ac
003c01c8: rsbs r0, r0, #1
003c01cc: movlo r0, #0
003c01d0: pop {r4, pc}

# _ZNK16CharStateMachine15SM_IsInPreSpawnEv 003c01d4 size24
003c01d4: push {r4, lr}
003c01d8: bl #0x3c01ac
003c01dc: cmp r0, #0x11
003c01e0: movne r0, #0
003c01e4: moveq r0, #1
003c01e8: pop {r4, pc}

# _ZN12VisualObject14SyncVisibilityEv 004713d0 size108
004713d0: push {r4, r5, r6, lr}
004713d4: ldr r4, [r0, #4]
004713d8: mov r5, r0
004713dc: cmp r4, #0
004713e0: beq #0x471438
004713e4: ldrb r3, [r4, #0x80]
004713e8: cmp r3, #0
004713ec: bne #0x471400
004713f0: mov r1, #0
004713f4: mov r0, r5
004713f8: pop {r4, r5, r6, lr}
004713fc: b #0x471368
00471400: ldr r3, [r4]
00471404: mov r0, r4
00471408: mov lr, pc
0047140c: ldr pc, [r3, #0xc4]
00471410: cmp r0, #0
00471414: beq #0x471430
00471418: ldrb r3, [r4, #0x2ee]
0047141c: cmp r3, #0
00471420: beq #0x471430
00471424: ldrb r3, [r4, #0x2f0]
00471428: cmp r3, #0
0047142c: beq #0x4713f0
00471430: mov r1, #1
00471434: b #0x4713f4
00471438: pop {r4, r5, r6, pc}

# _ZN12VisualObject4SyncEv 0038ba74 size32
0038ba74: push {r4, lr}
0038ba78: mov r4, r0
0038ba7c: bl #0x470cb8
0038ba80: mov r0, r4
0038ba84: bl #0x472948
0038ba88: mov r0, r4
0038ba8c: pop {r4, lr}
0038ba90: b #0x472860
