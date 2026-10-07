
# _ZN9Character20SG_GetGameDifficultyEv
003bb8e4: movw r3, #0x14e8
003bb8e8: ldr r2, [r0, r3]
003bb8ec: ldr r3, [pc, #0x1c]
003bb8f0: cmp r2, #0
003bb8f4: add r3, pc, r3
003bb8f8: mvneq r0, #0
003bb8fc: bxeq lr
003bb900: ldr r2, [pc, #0xc]
003bb904: ldr r3, [r3, r2]
003bb908: ldr r0, [r3]
003bb90c: bx lr

# _ZN13PlayerManager13IsLocalPlayerEPK9Character
0036effc: subs r3, r1, #0
0036f000: push {r4, lr}
0036f004: beq #0x36f020
0036f008: mov r2, #0
0036f00c: bl #0x36eea8
0036f010: ldr r3, [r0]
0036f014: mov lr, pc
0036f018: ldr pc, [r3, #0x50]
0036f01c: pop {r4, pc}
0036f020: mov r0, r3
0036f024: pop {r4, pc}

# _ZN9Character17INV_TransmuteItemEjb
003a4bec: push {r4, r5, r6, lr}
003a4bf0: mov r4, r0
003a4bf4: add r0, r0, #0x37c
003a4bf8: mov r5, r2
003a4bfc: bl #0x3fc61c
003a4c00: mov r2, r5
003a4c04: mov r1, r0
003a4c08: mov r0, r4
003a4c0c: pop {r4, r5, r6, lr}
003a4c10: b #0x3a4a3c

# _ZN9Character17INV_TransmuteItemEP12ItemInstanceb
003a4a3c: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a4a40: ldr r4, [pc, #0x14c]
003a4a44: subs r7, r1, #0
003a4a48: mov r6, r0
003a4a4c: add r4, pc, r4
003a4a50: mov sb, r2
003a4a54: moveq r5, r7
003a4a58: beq #0x3a4b14
003a4a5c: add r1, r0, #0xff0
003a4a60: add r8, r0, #0x560
003a4a64: add r1, r1, #4
003a4a68: mov r2, #0xc5
003a4a6c: mov r0, r8
003a4a70: ldr fp, [r7, #0x54]
003a4a74: bl #0x3dedb4
003a4a78: ldr sl, [pc, #0x118]
003a4a7c: ldr r1, [pc, #0x118]
003a4a80: ldr r2, [pc, #0x118]
003a4a84: ldr r3, [r4, sl]
003a4a88: mov r5, r0
003a4a8c: add r1, pc, r1
003a4a90: ldr r0, [r3, #0x2c]
003a4a94: add r2, pc, r2
003a4a98: bl #0x4c4bdc
003a4a9c: lsl fp, fp, #8
003a4aa0: add r5, r5, #0x100
003a4aa4: mul r5, fp, r5
003a4aa8: asr r5, r5, #8
003a4aac: mul r5, r0, r5
003a4ab0: asr r5, r5, #0x10
003a4ab4: cmp r5, #1
003a4ab8: movlt r5, #1
003a4abc: cmp sb, #0
003a4ac0: bne #0x3a4b14
003a4ac4: ldrsh r3, [r7, #0x50]
003a4ac8: cmp r3, #1
003a4acc: ble #0x3a4b74
003a4ad0: mov r0, r7
003a4ad4: mvn r1, #0
003a4ad8: bl #0x3fa17c
003a4adc: add sb, r6, #0x37c
003a4ae0: mov r0, sb
003a4ae4: mov r1, r5
003a4ae8: bl #0x3fe164
003a4aec: mov r0, r8
003a4af0: mov r1, #0xd5
003a4af4: mov r2, #1
003a4af8: bl #0x3e0798
003a4afc: ldr r3, [r4, sl]
003a4b00: mov r1, r6
003a4b04: ldr r0, [r3, #0x40]
003a4b08: bl #0x36effc
003a4b0c: cmp r0, #0
003a4b10: bne #0x3a4b1c
003a4b14: mov r0, r5
003a4b18: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a4b1c: ldr r3, [pc, #0x80]
003a4b20: mov r0, r8
003a4b24: mov r1, #0xd5
003a4b28: ldr r3, [r4, r3]
003a4b2c: mov r2, #0
003a4b30: ldr r4, [r3]
003a4b34: bl #0x3df6e0
003a4b38: cmp r0, #0x12c
003a4b3c: blt #0x3a4b14
003a4b40: mov r0, r6
003a4b44: ldr r3, [r6]
003a4b48: mov lr, pc
003a4b4c: ldr pc, [r3, #0x28]
003a4b50: cmp r0, #0
003a4b54: beq #0x3a4b14
003a4b58: ldr r0, [pc, #0x48]
003a4b5c: add r0, pc, r0
003a4b60: bl #0x3a3f70
003a4b64: mov r1, r0
003a4b68: mov r0, r4
003a4b6c: bl #0x3813b8
003a4b70: b #0x3a4b14
003a4b74: add sb, r6, #0x37c
003a4b78: mov r1, r7
003a4b7c: mov r0, sb
003a4b80: bl #0x3fc63c
003a4b84: mov r1, r0
003a4b88: mov r0, sb
003a4b8c: bl #0x3fe448
003a4b90: b #0x3a4ae0
003a4b94: subseq r0, pc, r4, asr #32
003a4b98: strdeq r3, r4, [r0], -r4
003a4b9c: subseq ip, r1, r4, asr #25
003a4ba0: subseq lr, r1, r4, ror #13
003a4ba4: andeq r1, r0, r0, ror sp
003a4ba8: subseq lr, r1, r4, lsr r6

# _Z9GetOnlinev
007fd794: b #0x7fd744

# _ZN9Character20SG_GetGameDifficultyEv
003bb8e4: movw r3, #0x14e8
003bb8e8: ldr r2, [r0, r3]
003bb8ec: ldr r3, [pc, #0x1c]
003bb8f0: cmp r2, #0
003bb8f4: add r3, pc, r3
003bb8f8: mvneq r0, #0
003bb8fc: bxeq lr
003bb900: ldr r2, [pc, #0xc]
003bb904: ldr r3, [r3, r2]
003bb908: ldr r0, [r3]
003bb90c: bx lr

# _ZN13PlayerManager13IsLocalPlayerEPK9Character
0036effc: subs r3, r1, #0
0036f000: push {r4, lr}
0036f004: beq #0x36f020
0036f008: mov r2, #0
0036f00c: bl #0x36eea8
0036f010: ldr r3, [r0]
0036f014: mov lr, pc
0036f018: ldr pc, [r3, #0x50]
0036f01c: pop {r4, pc}
0036f020: mov r0, r3
0036f024: pop {r4, pc}

# _ZN9Character17INV_TransmuteItemEjb
003a4bec: push {r4, r5, r6, lr}
003a4bf0: mov r4, r0
003a4bf4: add r0, r0, #0x37c
003a4bf8: mov r5, r2
003a4bfc: bl #0x3fc61c
003a4c00: mov r2, r5
003a4c04: mov r1, r0
003a4c08: mov r0, r4
003a4c0c: pop {r4, r5, r6, lr}
003a4c10: b #0x3a4a3c

# _ZN9Character17INV_TransmuteItemEP12ItemInstanceb
003a4a3c: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a4a40: ldr r4, [pc, #0x14c]
003a4a44: subs r7, r1, #0
003a4a48: mov r6, r0
003a4a4c: add r4, pc, r4
003a4a50: mov sb, r2
003a4a54: moveq r5, r7
003a4a58: beq #0x3a4b14
003a4a5c: add r1, r0, #0xff0
003a4a60: add r8, r0, #0x560
003a4a64: add r1, r1, #4
003a4a68: mov r2, #0xc5
003a4a6c: mov r0, r8
003a4a70: ldr fp, [r7, #0x54]
003a4a74: bl #0x3dedb4
003a4a78: ldr sl, [pc, #0x118]
003a4a7c: ldr r1, [pc, #0x118]
003a4a80: ldr r2, [pc, #0x118]
003a4a84: ldr r3, [r4, sl]
003a4a88: mov r5, r0
003a4a8c: add r1, pc, r1
003a4a90: ldr r0, [r3, #0x2c]
003a4a94: add r2, pc, r2
003a4a98: bl #0x4c4bdc
003a4a9c: lsl fp, fp, #8
003a4aa0: add r5, r5, #0x100
003a4aa4: mul r5, fp, r5
003a4aa8: asr r5, r5, #8
003a4aac: mul r5, r0, r5
003a4ab0: asr r5, r5, #0x10
003a4ab4: cmp r5, #1
003a4ab8: movlt r5, #1
003a4abc: cmp sb, #0
003a4ac0: bne #0x3a4b14
003a4ac4: ldrsh r3, [r7, #0x50]
003a4ac8: cmp r3, #1
003a4acc: ble #0x3a4b74
003a4ad0: mov r0, r7
003a4ad4: mvn r1, #0
003a4ad8: bl #0x3fa17c
003a4adc: add sb, r6, #0x37c
003a4ae0: mov r0, sb
003a4ae4: mov r1, r5
003a4ae8: bl #0x3fe164
003a4aec: mov r0, r8
003a4af0: mov r1, #0xd5
003a4af4: mov r2, #1
003a4af8: bl #0x3e0798
003a4afc: ldr r3, [r4, sl]
003a4b00: mov r1, r6
003a4b04: ldr r0, [r3, #0x40]
003a4b08: bl #0x36effc
003a4b0c: cmp r0, #0
003a4b10: bne #0x3a4b1c
003a4b14: mov r0, r5
003a4b18: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a4b1c: ldr r3, [pc, #0x80]
003a4b20: mov r0, r8
003a4b24: mov r1, #0xd5
003a4b28: ldr r3, [r4, r3]
003a4b2c: mov r2, #0
003a4b30: ldr r4, [r3]
003a4b34: bl #0x3df6e0
003a4b38: cmp r0, #0x12c
003a4b3c: blt #0x3a4b14
003a4b40: mov r0, r6
003a4b44: ldr r3, [r6]
003a4b48: mov lr, pc
003a4b4c: ldr pc, [r3, #0x28]
003a4b50: cmp r0, #0
003a4b54: beq #0x3a4b14
003a4b58: ldr r0, [pc, #0x48]
003a4b5c: add r0, pc, r0
003a4b60: bl #0x3a3f70
003a4b64: mov r1, r0
003a4b68: mov r0, r4
003a4b6c: bl #0x3813b8
003a4b70: b #0x3a4b14
003a4b74: add sb, r6, #0x37c
003a4b78: mov r1, r7
003a4b7c: mov r0, sb
003a4b80: bl #0x3fc63c
003a4b84: mov r1, r0
003a4b88: mov r0, sb
003a4b8c: bl #0x3fe448
003a4b90: b #0x3a4ae0
003a4b94: subseq r0, pc, r4, asr #32
003a4b98: strdeq r3, r4, [r0], -r4
003a4b9c: subseq ip, r1, r4, asr #25
003a4ba0: subseq lr, r1, r4, ror #13
003a4ba4: andeq r1, r0, r0, ror sp
003a4ba8: subseq lr, r1, r4, lsr r6

# _Z9GetOnlinev
007fd794: b #0x7fd744
