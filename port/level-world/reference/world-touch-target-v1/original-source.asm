# 0x320e44 _ZN11Application14GetSavedOptionEPKc
00320e44: push {r4, r5, r6, lr}
00320e48: ldr r4, [r0, #0x4c]
00320e4c: mov r5, r1
00320e50: mov r0, r4
00320e54: bl #0x46d4a8
00320e58: cmp r0, #0
00320e5c: bne #0x320e64
00320e60: pop {r4, r5, r6, pc}
00320e64: mov r0, r4
00320e68: mov r1, r5
00320e6c: pop {r4, r5, r6, lr}
00320e70: b #0x46d474

# 0x320e74 _ZN11Application11IsUsingDPadEv
00320e74: ldr r1, [pc, #0x14]
00320e78: push {r4, lr}
00320e7c: add r1, pc, r1
00320e80: bl #0x320e44
00320e84: subs r0, r0, #0
00320e88: movne r0, #1
00320e8c: pop {r4, pc}
00320e90: subseq sp, sb, r4, asr #27

# 0x328f40 _ZN11Application10ResetTouchEv
00328f40: push {r4, r5, r6, r7, lr}
00328f44: ldr r3, [r0, #0x20]
00328f48: sub sp, sp, #0x14
00328f4c: mov r6, r0
00328f50: cmp r3, #0
00328f54: beq #0x328fd8
00328f58: mvn r2, #0
00328f5c: strh r2, [sp, #0xc]
00328f60: strh r2, [sp, #0xe]
00328f64: add r5, sp, #4
00328f68: mov r1, r3
00328f6c: mov r0, r5
00328f70: ldr r3, [r3]
00328f74: mov lr, pc
00328f78: ldr pc, [r3, #0x4c]
00328f7c: add r7, sp, #0xc
00328f80: ldr r4, [sp, #4]
00328f84: b #0x328fa8
00328f88: ldr r3, [r6, #0x20]
00328f8c: ldr r2, [r4, #8]
00328f90: mov r1, r7
00328f94: mov r0, r3
00328f98: ldr r3, [r3]
00328f9c: mov lr, pc
00328fa0: ldr pc, [r3, #0x28]
00328fa4: ldr r4, [r4]
00328fa8: cmp r4, r5
00328fac: bne #0x328f88
00328fb0: ldr r0, [sp, #4]
00328fb4: cmp r0, r5
00328fb8: bne #0x328fc4
00328fbc: b #0x328fd8
00328fc0: mov r0, r4
00328fc4: ldr r4, [r0]
00328fc8: mov r1, #0xc
00328fcc: bl #0x708f00
00328fd0: cmp r4, r5
00328fd4: bne #0x328fc0
00328fd8: add sp, sp, #0x14
00328fdc: pop {r4, r5, r6, r7, pc}

# 0x33a99c _ZN18EvTouchScreenPressD1Ev
0033a99c: bx lr

# 0x33a9a0 _ZN17EvTouchScreenMoveD1Ev
0033a9a0: bx lr

# 0x33a9a4 _ZNK15TouchScreenBase13_IsQueueEmptyEv
0033a9a4: ldr r3, [r0, #0x1a4]
0033a9a8: ldr r0, [r0, #0x1a0]
0033a9ac: cmp r0, r3
0033a9b0: movne r0, #0
0033a9b4: moveq r0, #1
0033a9b8: bx lr

# 0x33a9bc _ZN15TouchScreenBase11_AddToQueueENS_10TouchEventERK7Point2DIfEl
0033a9bc: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033a9c0: ldr r5, [r0, #0x1a4]
0033a9c4: mov r7, r2
0033a9c8: mov r8, r3
0033a9cc: add r2, r5, #1
0033a9d0: cmp r2, #0xf
0033a9d4: movhi r3, #0
0033a9d8: str r2, [r0, #0x1a4]
0033a9dc: strhi r3, [r0, #0x1a4]
0033a9e0: mov r4, r0
0033a9e4: mov r6, r1
0033a9e8: bl #0x33a9a4
0033a9ec: cmp r0, #0
0033a9f0: bne #0x33aad0
0033a9f4: ldr ip, [r4, #0x1a0]
0033a9f8: cmp ip, r5
0033a9fc: ldreq lr, [r4, #0x194]
0033aa00: beq #0x33aa90
0033aa04: ldr r0, [r4, #0x194]
0033aa08: mov r2, r5
0033aa0c: mov sl, #0xc
0033aa10: mov lr, r0
0033aa14: b #0x33aa20
0033aa18: cmp r2, ip
0033aa1c: beq #0x33aa90
0033aa20: cmp r2, #0
0033aa24: subne r2, r2, #1
0033aa28: mulne r3, sl, r2
0033aa2c: moveq r3, #0xb4
0033aa30: add r1, r0, r3
0033aa34: ldr r1, [r1, #8]
0033aa38: moveq r2, #0xf
0033aa3c: cmp r1, r8
0033aa40: bne #0x33aa18
0033aa44: ldr r1, [r0, r3]
0033aa48: cmp r1, #2
0033aa4c: beq #0x33aa90
0033aa50: cmp r1, #0
0033aa54: beq #0x33aa90
0033aa58: cmp r1, #1
0033aa5c: bne #0x33aa18
0033aa60: str r5, [r4, #0x1a4]
0033aa64: str r6, [r0, r3]
0033aa68: ldr r4, [r4, #0x194]
0033aa6c: ldr r0, [r7]
0033aa70: add r4, r4, r3
0033aa74: bl #0x30e4cc
0033aa78: uxth r5, r0
0033aa7c: ldr r0, [r7, #4]
0033aa80: bl #0x30e4cc
0033aa84: strh r5, [r4, #4]
0033aa88: strh r0, [r4, #6]
0033aa8c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033aa90: mov r3, #0xc
0033aa94: mul r5, r3, r5
0033aa98: str r6, [lr, r5]
0033aa9c: ldr r0, [r7]
0033aaa0: bl #0x30e4cc
0033aaa4: uxth sl, r0
0033aaa8: ldr r0, [r7, #4]
0033aaac: bl #0x30e4cc
0033aab0: ldr r6, [r4, #0x194]
0033aab4: add r6, r6, r5
0033aab8: strh r0, [r6, #6]
0033aabc: strh sl, [r6, #4]
0033aac0: ldr r3, [r4, #0x194]
0033aac4: add r5, r3, r5
0033aac8: str r8, [r5, #8]
0033aacc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033aad0: str r5, [r4, #0x1a4]
0033aad4: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x33aad8 _ZNK15TouchScreenBase12getDimensionEv
0033aad8: push {r4, r5, r6, r7, r8, lr}
0033aadc: mov r4, r0
0033aae0: ldr r3, [r1]
0033aae4: mov r0, r1
0033aae8: mov r5, r1
0033aaec: mov lr, pc
0033aaf0: ldr pc, [r3, #8]
0033aaf4: ldr r3, [r5]
0033aaf8: uxth r7, r0
0033aafc: mov r0, r5
0033ab00: mov lr, pc
0033ab04: ldr pc, [r3, #0x10]
0033ab08: ldr r3, [r5]
0033ab0c: uxth r6, r0
0033ab10: mov r0, r5
0033ab14: mov lr, pc
0033ab18: ldr pc, [r3, #0xc]
0033ab1c: ldr r3, [r5]
0033ab20: uxth r8, r0
0033ab24: mov r0, r5
0033ab28: mov lr, pc
0033ab2c: ldr pc, [r3, #0x14]
0033ab30: ldr r3, [pc, #0x7c]
0033ab34: ldr r2, [pc, #0x7c]
0033ab38: sxth ip, r7
0033ab3c: add r3, pc, r3
0033ab40: ldr r2, [r3, r2]
0033ab44: sxth r1, r8
0033ab48: uxth r0, r0
0033ab4c: add r2, r2, #8
0033ab50: cmp ip, r1
0033ab54: str r2, [r4]
0033ab58: strh r7, [r4, #4]
0033ab5c: strh r6, [r4, #6]
0033ab60: strh r8, [r4, #8]
0033ab64: strh r0, [r4, #0xa]
0033ab68: bgt #0x33ab7c
0033ab6c: uxthlt r3, r7
0033ab70: uxthge r8, r7
0033ab74: uxthlt r7, r8
0033ab78: uxthlt r8, r3
0033ab7c: sxth r2, r6
0033ab80: sxth r3, r0
0033ab84: cmp r2, r3
0033ab88: bgt #0x33ab9c
0033ab8c: uxthlt r3, r6
0033ab90: uxthge r0, r6
0033ab94: uxthlt r6, r0
0033ab98: uxthlt r0, r3
0033ab9c: strh r0, [r4, #6]
0033aba0: strh r8, [r4, #4]
0033aba4: strh r7, [r4, #8]
0033aba8: strh r6, [r4, #0xa]
0033abac: mov r0, r4
0033abb0: pop {r4, r5, r6, r7, r8, pc}
0033abb4: rsbeq sb, r5, r4, asr pc
0033abb8: andeq r3, r0, r4, lsl #27

# 0x33abbc _ZNK15TouchScreenBase12getDimensionER3RctIsE
0033abbc: push {r4, r5, r6, r7, r8, lr}
0033abc0: ldr r3, [r0]
0033abc4: mov r5, r1
0033abc8: mov r4, r0
0033abcc: mov lr, pc
0033abd0: ldr pc, [r3, #8]
0033abd4: ldr r3, [r4]
0033abd8: uxth r8, r0
0033abdc: mov r0, r4
0033abe0: mov lr, pc
0033abe4: ldr pc, [r3, #0x10]
0033abe8: ldr r3, [r4]
0033abec: uxth r7, r0
0033abf0: mov r0, r4
0033abf4: mov lr, pc
0033abf8: ldr pc, [r3, #0xc]
0033abfc: ldr r3, [r4]
0033ac00: uxth r6, r0
0033ac04: mov r0, r4
0033ac08: mov lr, pc
0033ac0c: ldr pc, [r3, #0x14]
0033ac10: strh r8, [r5, #4]
0033ac14: strh r0, [r5, #0xa]
0033ac18: strh r7, [r5, #6]
0033ac1c: strh r6, [r5, #8]
0033ac20: pop {r4, r5, r6, r7, r8, pc}

# 0x33ac24 _ZN15TouchScreenBase10touchBeganERK7Point2DIsEl
0033ac24: push {r4, r5, r6, r7, lr}
0033ac28: ldr r3, [r0, #0x190]
0033ac2c: add lr, r2, #1
0033ac30: mov r5, r2
0033ac34: cmp r3, r2
0033ac38: mov r3, #0x30
0033ac3c: mla r3, r3, r2, r0
0033ac40: strlt lr, [r0, #0x190]
0033ac44: ldrb r2, [r3, #0x28]
0033ac48: sub sp, sp, #0xc
0033ac4c: mov r4, r0
0033ac50: cmp r2, #0
0033ac54: mov r6, r1
0033ac58: bne #0x33ac84
0033ac5c: mov r1, #6
0033ac60: mul r1, r1, r5
0033ac64: ldrh ip, [r6, #2]
0033ac68: ldrh r7, [r6]
0033ac6c: add r1, r1, #1
0033ac70: lsl r1, r1, #3
0033ac74: add r0, r0, r1
0033ac78: strh r7, [r4, r1]
0033ac7c: strh ip, [r0, #2]
0033ac80: str r2, [r3, #0x24]
0033ac84: mov r3, #6
0033ac88: mul r3, r3, r5
0033ac8c: add r2, r5, r5, lsl #1
0033ac90: add r3, r3, #1
0033ac94: add r3, r4, r3, lsl #3
0033ac98: ldrh r1, [r3, #4]
0033ac9c: add r2, r2, #1
0033aca0: lsl r0, r2, #4
0033aca4: strh r1, [r4, r0]
0033aca8: ldrh r7, [r3, #6]
0033acac: add r0, r4, r0
0033acb0: mov ip, #0x30
0033acb4: strh r7, [r0, #2]
0033acb8: ldrh r0, [r6]
0033acbc: mla r1, ip, r5, r4
0033acc0: strh r0, [r3, #4]
0033acc4: ldrh r7, [r6, #2]
0033acc8: mov r0, #1
0033accc: add r2, r4, r2, lsl #4
0033acd0: strh r7, [r3, #6]
0033acd4: mul ip, ip, lr
0033acd8: strb r0, [r1, #0x28]
0033acdc: strb r0, [r1, #0x20]
0033ace0: mov r3, #0x188
0033ace4: ldrd r0, r1, [r4, r3]
0033ace8: strd r0, r1, [r2, #8]
0033acec: mov r7, #0
0033acf0: str r7, [r4, ip]
0033acf4: ldrsh r0, [r6]
0033acf8: bl #0x30e964
0033acfc: str r0, [sp]
0033ad00: ldrsh r0, [r6, #2]
0033ad04: bl #0x30e964
0033ad08: mov r1, r7
0033ad0c: str r0, [sp, #4]
0033ad10: mov r3, r5
0033ad14: mov r0, r4
0033ad18: mov r2, sp
0033ad1c: bl #0x33a9bc
0033ad20: add sp, sp, #0xc
0033ad24: pop {r4, r5, r6, r7, pc}

# 0x33ad28 _ZN15TouchScreenBase10touchMovedERK7Point2DIsEl
0033ad28: push {r4, r5, r6, r7, r8, sb, lr}
0033ad2c: mov r3, #0x30
0033ad30: mov r5, r2
0033ad34: mla r2, r3, r2, r0
0033ad38: mov r6, r1
0033ad3c: ldrb r1, [r2, #0x28]
0033ad40: sub sp, sp, #0xc
0033ad44: mov r4, r0
0033ad48: cmp r1, #0
0033ad4c: beq #0x33ae08
0033ad50: mov r0, #6
0033ad54: mul r0, r0, r5
0033ad58: add ip, r5, r5, lsl #1
0033ad5c: add r0, r0, #1
0033ad60: lsl r0, r0, #3
0033ad64: add r1, r4, r0
0033ad68: ldrh r7, [r1, #4]
0033ad6c: add ip, ip, #1
0033ad70: lsl lr, ip, #4
0033ad74: strh r7, [r4, lr]
0033ad78: ldrh r7, [r1, #6]
0033ad7c: add lr, r4, lr
0033ad80: add ip, r4, ip, lsl #4
0033ad84: strh r7, [lr, #2]
0033ad88: ldrh lr, [r6]
0033ad8c: mla r3, r5, r3, r3
0033ad90: strh lr, [r1, #4]
0033ad94: ldrh r7, [r6, #2]
0033ad98: mov lr, #0x188
0033ad9c: strh r7, [r1, #6]
0033ada0: ldrd r8, sb, [r4, lr]
0033ada4: strd r8, sb, [ip, #8]
0033ada8: mov ip, #1
0033adac: str ip, [r4, r3]
0033adb0: ldrsh r0, [r4, r0]
0033adb4: ldrsh r3, [r1, #4]
0033adb8: rsb lr, r3, r0
0033adbc: eor ip, lr, lr, asr #31
0033adc0: sub ip, ip, lr, asr #31
0033adc4: cmp ip, #0xb
0033adc8: ble #0x33ae08
0033adcc: ldrsh ip, [r1, #6]
0033add0: ldrsh r1, [r1, #2]
0033add4: rsb ip, ip, r1
0033add8: eor r1, ip, ip, asr #31
0033addc: sub r1, r1, ip, asr #31
0033ade0: cmp r1, #4
0033ade4: bgt #0x33ae08
0033ade8: add r2, r2, #0x20
0033adec: ldr r1, [r2, #4]
0033adf0: cmp r1, #0
0033adf4: bne #0x33ae08
0033adf8: cmp r0, r3
0033adfc: movge r3, #1
0033ae00: movlt r3, #2
0033ae04: str r3, [r2, #4]
0033ae08: ldrsh r0, [r6]
0033ae0c: bl #0x30e964
0033ae10: str r0, [sp]
0033ae14: ldrsh r0, [r6, #2]
0033ae18: bl #0x30e964
0033ae1c: mov r3, r5
0033ae20: str r0, [sp, #4]
0033ae24: mov r1, #1
0033ae28: mov r0, r4
0033ae2c: mov r2, sp
0033ae30: bl #0x33a9bc
0033ae34: add sp, sp, #0xc
0033ae38: pop {r4, r5, r6, r7, r8, sb, pc}

# 0x33ae3c _ZN15TouchScreenBase14touchCancelledERK7Point2DIsEl
0033ae3c: push {r4, r5, r6, lr}
0033ae40: mov r3, #0x30
0033ae44: mov r5, r2
0033ae48: mla r2, r2, r3, r3
0033ae4c: mla r3, r3, r5, r0
0033ae50: mov r4, r0
0033ae54: mvn r0, #0
0033ae58: str r0, [r3, #0x2c]
0033ae5c: mov r0, #0
0033ae60: strb r0, [r3, #0x28]
0033ae64: mov r3, #2
0033ae68: str r3, [r4, r2]
0033ae6c: ldr r3, [r4, #0x190]
0033ae70: sub sp, sp, #8
0033ae74: mov r6, r1
0033ae78: sub r3, r3, #1
0033ae7c: cmp r3, r5
0033ae80: streq r5, [r4, #0x190]
0033ae84: ldrsh r0, [r1]
0033ae88: bl #0x30e964
0033ae8c: str r0, [sp]
0033ae90: ldrsh r0, [r6, #2]
0033ae94: bl #0x30e964
0033ae98: mov r3, r5
0033ae9c: str r0, [sp, #4]
0033aea0: mov r1, #2
0033aea4: mov r0, r4
0033aea8: mov r2, sp
0033aeac: bl #0x33a9bc
0033aeb0: add sp, sp, #8
0033aeb4: pop {r4, r5, r6, pc}

# 0x33aeb8 _ZN15TouchScreenBase10touchEndedERK7Point2DIsEl
0033aeb8: b #0x33ae3c

# 0x33aebc _ZNK15TouchScreenBase15isRegionPressedERKN6glitch4core4rectIfEE
0033aebc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0033aec0: ldr r3, [r0, #0x190]
0033aec4: sub sp, sp, #0xc
0033aec8: mov r8, r0
0033aecc: cmp r3, #0
0033aed0: mov sb, r1
0033aed4: ble #0x33afec
0033aed8: mov r2, #0x30
0033aedc: mla r2, r2, r3, r0
0033aee0: mov r4, r0
0033aee4: str r2, [sp, #4]
0033aee8: ldr r6, [r0, #0x1a8]
0033aeec: ldr r7, [r0, #0x1b0]
0033aef0: ldr fp, [r1]
0033aef4: cmp r6, #2
0033aef8: ldrh r0, [r4, #0xc]
0033aefc: ldrh r5, [r4, #0xe]
0033af00: beq #0x33b008
0033af04: cmp r6, #3
0033af08: beq #0x33aff4
0033af0c: cmp r6, #1
0033af10: bne #0x33af24
0033af14: movw r1, #0x1ae
0033af18: ldrh r3, [r8, r1]
0033af1c: rsb r5, r5, r3
0033af20: uxth r5, r5
0033af24: sxth r0, r0
0033af28: bl #0x30e964
0033af2c: mov r1, r7
0033af30: bl #0x30ed6c
0033af34: bl #0x30e4cc
0033af38: sxth r0, r0
0033af3c: bl #0x30e964
0033af40: mov r1, fp
0033af44: mov sl, r0
0033af48: bl #0x30e4b4
0033af4c: cmp r0, #0
0033af50: beq #0x33afdc
0033af54: sxth r0, r5
0033af58: bl #0x30e964
0033af5c: mov r1, r7
0033af60: bl #0x30ed6c
0033af64: bl #0x30e4cc
0033af68: sxth r0, r0
0033af6c: bl #0x30e964
0033af70: ldr r1, [sb, #4]
0033af74: mov r5, r0
0033af78: bl #0x30e4b4
0033af7c: cmp r0, #0
0033af80: beq #0x33afdc
0033af84: mov r0, sl
0033af88: ldr r1, [sb, #8]
0033af8c: bl #0x30e9ac
0033af90: cmp r0, #0
0033af94: beq #0x33afdc
0033af98: mov r0, r5
0033af9c: ldr r1, [sb, #0xc]
0033afa0: bl #0x30e9ac
0033afa4: cmp r0, #0
0033afa8: beq #0x33afdc
0033afac: ldrb r3, [r4, #0x20]
0033afb0: cmp r3, #0
0033afb4: beq #0x33afdc
0033afb8: ldrb r3, [r4, #0x28]
0033afbc: cmp r3, #0
0033afc0: beq #0x33afdc
0033afc4: ldr r3, [r4, #0x30]
0033afc8: cmp r3, #0
0033afcc: bne #0x33afdc
0033afd0: mov r0, #1
0033afd4: add sp, sp, #0xc
0033afd8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0033afdc: ldr r3, [sp, #4]
0033afe0: add r4, r4, #0x30
0033afe4: cmp r4, r3
0033afe8: bne #0x33aef4
0033afec: mov r0, #0
0033aff0: b #0x33afd4
0033aff4: mov r2, #0x1ac
0033aff8: ldrh r3, [r8, r2]
0033affc: rsb r0, r0, r3
0033b000: uxth r0, r0
0033b004: b #0x33af24
0033b008: mov r3, #0x1ac
0033b00c: movw r1, #0x1ae
0033b010: ldrh r2, [r8, r3]
0033b014: ldrh r3, [r8, r1]
0033b018: rsb r0, r0, r2
0033b01c: rsb r5, r5, r3
0033b020: uxth r0, r0
0033b024: uxth r5, r5
0033b028: b #0x33af24

# 0x33b02c _ZNK15TouchScreenBase27getTouchIDInitiatedInRegionERKN6glitch4core4rectIfEE
0033b02c: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033b030: ldr sl, [r0, #0x190]
0033b034: mov r7, r1
0033b038: cmp sl, #0
0033b03c: ble #0x33b0e0
0033b040: ldr r8, [r1]
0033b044: mov r4, r0
0033b048: mov r5, #0
0033b04c: ldrsh r0, [r4, #8]
0033b050: bl #0x30e964
0033b054: mov r1, r8
0033b058: mov r6, r0
0033b05c: bl #0x30e4b4
0033b060: cmp r0, #0
0033b064: ldrh r0, [r4, #0xa]
0033b068: beq #0x33b0d0
0033b06c: sxth r0, r0
0033b070: bl #0x30e964
0033b074: ldr r1, [r7, #4]
0033b078: mov sb, r0
0033b07c: bl #0x30e4b4
0033b080: cmp r0, #0
0033b084: mov r0, r6
0033b088: beq #0x33b0d0
0033b08c: ldr r1, [r7, #8]
0033b090: bl #0x30e9ac
0033b094: cmp r0, #0
0033b098: mov r0, sb
0033b09c: beq #0x33b0d0
0033b0a0: ldr r1, [r7, #0xc]
0033b0a4: bl #0x30e9ac
0033b0a8: cmp r0, #0
0033b0ac: beq #0x33b0d0
0033b0b0: ldrb r3, [r4, #0x20]
0033b0b4: cmp r3, #0
0033b0b8: beq #0x33b0d0
0033b0bc: ldrb r3, [r4, #0x28]
0033b0c0: cmp r3, #0
0033b0c4: beq #0x33b0d0
0033b0c8: mov r0, r5
0033b0cc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033b0d0: add r5, r5, #1
0033b0d4: cmp r5, sl
0033b0d8: add r4, r4, #0x30
0033b0dc: bne #0x33b04c
0033b0e0: mvn r5, #0
0033b0e4: b #0x33b0c8

# 0x33b0e8 _ZNK15TouchScreenBase18getTouchIDInRegionERKN6glitch4core4rectIfEE
0033b0e8: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033b0ec: ldr sl, [r0, #0x190]
0033b0f0: mov r7, r1
0033b0f4: cmp sl, #0
0033b0f8: ble #0x33b19c
0033b0fc: ldr r8, [r1]
0033b100: mov r4, r0
0033b104: mov r5, #0
0033b108: ldrsh r0, [r4, #0xc]
0033b10c: bl #0x30e964
0033b110: mov r1, r8
0033b114: mov r6, r0
0033b118: bl #0x30e4b4
0033b11c: cmp r0, #0
0033b120: ldrh r0, [r4, #0xe]
0033b124: beq #0x33b18c
0033b128: sxth r0, r0
0033b12c: bl #0x30e964
0033b130: ldr r1, [r7, #4]
0033b134: mov sb, r0
0033b138: bl #0x30e4b4
0033b13c: cmp r0, #0
0033b140: mov r0, r6
0033b144: beq #0x33b18c
0033b148: ldr r1, [r7, #8]
0033b14c: bl #0x30e9ac
0033b150: cmp r0, #0
0033b154: mov r0, sb
0033b158: beq #0x33b18c
0033b15c: ldr r1, [r7, #0xc]
0033b160: bl #0x30e9ac
0033b164: cmp r0, #0
0033b168: beq #0x33b18c
0033b16c: ldrb r3, [r4, #0x20]
0033b170: cmp r3, #0
0033b174: beq #0x33b18c
0033b178: ldrb r3, [r4, #0x28]
0033b17c: cmp r3, #0
0033b180: beq #0x33b18c
0033b184: mov r0, r5
0033b188: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033b18c: add r5, r5, #1
0033b190: cmp r5, sl
0033b194: add r4, r4, #0x30
0033b198: bne #0x33b108
0033b19c: mvn r5, #0
0033b1a0: b #0x33b184

# 0x33b1a4 _ZN15TouchScreenBase13getTouchPointEl
0033b1a4: mov r3, #0x30
0033b1a8: mla r3, r3, r1, r0
0033b1ac: ldrb r2, [r3, #0x28]
0033b1b0: cmp r2, #0
0033b1b4: bne #0x33b1c0
0033b1b8: mov r0, #0
0033b1bc: bx lr
0033b1c0: ldrb r3, [r3, #0x20]
0033b1c4: cmp r3, #0
0033b1c8: beq #0x33b1b8
0033b1cc: mov r3, #6
0033b1d0: mul r1, r3, r1
0033b1d4: add r0, r0, r1, lsl #3
0033b1d8: add r0, r0, #0xc
0033b1dc: bx lr

# 0x33b1e0 _ZN15TouchScreenBase18getFirstTouchPointEl
0033b1e0: mov r3, #0x30
0033b1e4: mla r3, r3, r1, r0
0033b1e8: ldrb r2, [r3, #0x28]
0033b1ec: cmp r2, #0
0033b1f0: bne #0x33b1fc
0033b1f4: mov r0, #0
0033b1f8: bx lr
0033b1fc: ldrb r3, [r3, #0x20]
0033b200: cmp r3, #0
0033b204: beq #0x33b1f4
0033b208: mov r3, #6
0033b20c: mul r1, r3, r1
0033b210: add r1, r1, #1
0033b214: add r0, r0, r1, lsl #3
0033b218: bx lr

# 0x33b21c _ZNK15TouchScreenBase20getTouchDisplacementEl
0033b21c: str r4, [sp, #-4]!
0033b220: mov r3, #0x30
0033b224: mla r3, r3, r2, r1
0033b228: ldrb ip, [r3, #0x28]
0033b22c: cmp ip, #0
0033b230: beq #0x33b240
0033b234: ldrb r3, [r3, #0x20]
0033b238: cmp r3, #0
0033b23c: bne #0x33b254
0033b240: mov r3, #0
0033b244: strh r3, [r0]
0033b248: strh r3, [r0, #2]
0033b24c: ldm sp!, {r4}
0033b250: bx lr
0033b254: mov ip, #6
0033b258: mul ip, ip, r2
0033b25c: add ip, ip, #1
0033b260: lsl ip, ip, #3
0033b264: add r3, r1, ip
0033b268: ldrh r4, [r1, ip]
0033b26c: ldrh ip, [r3, #4]
0033b270: cmp ip, r4
0033b274: ldrhne r3, [r3, #6]
0033b278: beq #0x33b2a8
0033b27c: add r2, r2, r2, lsl #1
0033b280: add r2, r2, #1
0033b284: lsl r2, r2, #4
0033b288: add r4, r1, r2
0033b28c: ldrh r1, [r1, r2]
0033b290: ldrh r2, [r4, #2]
0033b294: rsb ip, r1, ip
0033b298: rsb r3, r2, r3
0033b29c: strh ip, [r0]
0033b2a0: strh r3, [r0, #2]
0033b2a4: b #0x33b24c
0033b2a8: ldrh r4, [r3, #2]
0033b2ac: ldrh r3, [r3, #6]
0033b2b0: cmp r3, r4
0033b2b4: bne #0x33b27c
0033b2b8: b #0x33b240

# 0x33b2bc _ZNK15TouchScreenBase17getSwipeDirectionEl
0033b2bc: mov r3, #0x30
0033b2c0: mla r3, r3, r1, r0
0033b2c4: ldrb r2, [r3, #0x28]
0033b2c8: cmp r2, #0
0033b2cc: beq #0x33b2e4
0033b2d0: ldrb r2, [r3, #0x20]
0033b2d4: add r3, r3, #0x20
0033b2d8: cmp r2, #0
0033b2dc: ldrne r0, [r3, #4]
0033b2e0: bxne lr
0033b2e4: mov r0, #0
0033b2e8: bx lr

# 0x33b2ec _ZN15TouchScreenBase8hasTouchEv
0033b2ec: mov r3, #0
0033b2f0: ldrb r2, [r0, #0x28]
0033b2f4: add r3, r3, #1
0033b2f8: cmp r2, #0
0033b2fc: beq #0x33b318
0033b300: ldr r2, [r0, #0x30]
0033b304: cmp r2, #0
0033b308: bne #0x33b318
0033b30c: ldrb r2, [r0, #0x20]
0033b310: cmp r2, #0
0033b314: bne #0x33b32c
0033b318: cmp r3, #8
0033b31c: add r0, r0, #0x30
0033b320: bne #0x33b2f0
0033b324: mov r0, #0
0033b328: bx lr
0033b32c: mov r0, #1
0033b330: bx lr

# 0x33b334 _ZN15TouchScreenBase6updateEd
0033b334: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033b338: mov sl, #0x188
0033b33c: mov r7, r0
0033b340: strd r2, r3, [r0, sl]
0033b344: mov r4, r0
0033b348: mov r5, #1
0033b34c: mvn sb, #0
0033b350: b #0x33b3a0
0033b354: ldrd r0, r1, [r4, #0x18]
0033b358: bl #0x30eb44
0033b35c: ldrd r2, r3, [r7, sl]
0033b360: bl #0x30e760
0033b364: cmp r0, #0
0033b368: beq #0x33b3bc
0033b36c: ldrb r3, [r4, #0x28]
0033b370: cmp r3, #0
0033b374: beq #0x33b390
0033b378: strb r6, [r4, #0x28]
0033b37c: str sb, [r4, #0x2c]
0033b380: ldr r3, [r7, #0x190]
0033b384: cmp r3, r8
0033b388: subeq r3, r3, #1
0033b38c: streq r3, [r7, #0x190]
0033b390: add r5, r5, #1
0033b394: cmp r5, #9
0033b398: add r4, r4, #0x30
0033b39c: beq #0x33b3d8
0033b3a0: ldrb r6, [r4, #0x20]
0033b3a4: mov r3, #0x3fc00000
0033b3a8: mov r2, #0
0033b3ac: cmp r6, #0
0033b3b0: add r3, r3, #0x200000
0033b3b4: sub r8, r5, #1
0033b3b8: beq #0x33b354
0033b3bc: ldrb r3, [r4, #0x28]
0033b3c0: add r4, r4, #0x30
0033b3c4: cmp r3, #0
0033b3c8: strne r5, [r7, #0x190]
0033b3cc: add r5, r5, #1
0033b3d0: cmp r5, #9
0033b3d4: bne #0x33b3a0
0033b3d8: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x33b3dc _ZN15TouchScreenBase5getIDEl
0033b3dc: str r4, [sp, #-4]!
0033b3e0: mov r3, r0
0033b3e4: mov r2, r0
0033b3e8: mvn r4, #0
0033b3ec: mov r0, #0
0033b3f0: ldr ip, [r2, #0x2c]
0033b3f4: cmp ip, r1
0033b3f8: beq #0x33b430
0033b3fc: ldrb ip, [r2, #0x28]
0033b400: add r2, r2, #0x30
0033b404: cmp ip, #0
0033b408: bne #0x33b414
0033b40c: cmn r4, #1
0033b410: moveq r4, r0
0033b414: add r0, r0, #1
0033b418: cmp r0, #8
0033b41c: bne #0x33b3f0
0033b420: mov r2, #0x30
0033b424: mla r3, r2, r4, r3
0033b428: mov r0, r4
0033b42c: str r1, [r3, #0x2c]
0033b430: ldm sp!, {r4}
0033b434: bx lr

# 0x33b43c _ZN17EvTouchScreenMoveD0Ev
0033b43c: ldr r3, [pc, #0x24]
0033b440: ldr r2, [pc, #0x24]
0033b444: push {r4, lr}
0033b448: add r3, pc, r3
0033b44c: ldr r2, [r3, r2]
0033b450: mov r4, r0
0033b454: add r2, r2, #8
0033b458: str r2, [r0]
0033b45c: bl #0x310440
0033b460: mov r0, r4
0033b464: pop {r4, pc}
0033b468: rsbeq sb, r5, r8, asr #12
0033b46c: strheq r0, [r0], -r0

# 0x33b470 _ZN18EvTouchScreenPressD0Ev
0033b470: ldr r3, [pc, #0x24]
0033b474: ldr r2, [pc, #0x24]
0033b478: push {r4, lr}
0033b47c: add r3, pc, r3
0033b480: ldr r2, [r3, r2]
0033b484: mov r4, r0
0033b488: add r2, r2, #8
0033b48c: str r2, [r0]
0033b490: bl #0x310440
0033b494: mov r0, r4
0033b498: pop {r4, pc}
0033b49c: rsbeq sb, r5, r4, lsl r6
0033b4a0: strheq r0, [r0], -r0

# 0x33b4d8 _ZN15TouchScreenBase5clearEv
0033b4d8: push {r4, lr}
0033b4dc: mov r4, r0
0033b4e0: ldr r0, [pc, #0x24]
0033b4e4: add r0, pc, r0
0033b4e8: bl #0x324114
0033b4ec: mov r3, #0
0033b4f0: mov r2, r3
0033b4f4: add r3, r3, #1
0033b4f8: cmp r3, #8
0033b4fc: strb r2, [r4, #0x28]
0033b500: add r4, r4, #0x30
0033b504: bne #0x33b4f4
0033b508: pop {r4, pc}
0033b50c: subseq r4, r8, ip, lsr #22

# 0x33b510 _ZN15TouchScreenBase13_PopFromQueueEv
0033b510: push {r4, lr}
0033b514: sub sp, sp, #8
0033b518: mov r4, r0
0033b51c: bl #0x33a9a4
0033b520: ldr r3, [pc, #0x80]
0033b524: cmp r0, #0
0033b528: add r3, pc, r3
0033b52c: beq #0x33b554
0033b530: ldr r2, [pc, #0x74]
0033b534: ldr r2, [r3, r2]
0033b538: ldr r2, [r2]
0033b53c: cmp r2, #2
0033b540: moveq r3, #0
0033b544: streq r3, [r3]
0033b548: beq #0x33b554
0033b54c: cmp r2, #1
0033b550: beq #0x33b574
0033b554: ldr r3, [r4, #0x1a0]
0033b558: add r3, r3, #1
0033b55c: cmp r3, #0xf
0033b560: str r3, [r4, #0x1a0]
0033b564: movhi r3, #0
0033b568: strhi r3, [r4, #0x1a0]
0033b56c: add sp, sp, #8
0033b570: pop {r4, pc}
0033b574: ldr r0, [pc, #0x34]
0033b578: ldr r1, [pc, #0x34]
0033b57c: ldr r2, [pc, #0x34]
0033b580: ldr r0, [r3, r0]
0033b584: ldr r3, [pc, #0x30]
0033b588: mov ip, #0x6c
0033b58c: add r1, pc, r1
0033b590: add r2, pc, r2
0033b594: add r3, pc, r3
0033b598: add r0, r0, #0xa8
0033b59c: str ip, [sp]
0033b5a0: bl #0x30e004
0033b5a4: b #0x33b554
0033b5a8: rsbeq sb, r5, r8, ror #10
0033b5ac: andeq r3, r0, r0, asr #19
0033b5b0: andeq r1, r0, r0, asr #19
0033b5b4: subseq r2, r8, ip, asr #28
0033b5b8: ldrheq r4, [r8], #-0xa8
0033b5bc: subseq r4, r8, ip, asr #21

# 0x33b5c0 _ZNK15TouchScreenBase20_GetNextEventInQueueEv
0033b5c0: push {r4, lr}
0033b5c4: sub sp, sp, #8
0033b5c8: mov r4, r0
0033b5cc: bl #0x33a9a4
0033b5d0: ldr r3, [pc, #0x78]
0033b5d4: cmp r0, #0
0033b5d8: add r3, pc, r3
0033b5dc: beq #0x33b604
0033b5e0: ldr r2, [pc, #0x6c]
0033b5e4: ldr r2, [r3, r2]
0033b5e8: ldr r2, [r2]
0033b5ec: cmp r2, #2
0033b5f0: moveq r3, #0
0033b5f4: streq r3, [r3]
0033b5f8: beq #0x33b604
0033b5fc: cmp r2, #1
0033b600: beq #0x33b61c
0033b604: ldr r3, [r4, #0x194]
0033b608: ldr r2, [r4, #0x1a0]
0033b60c: mov r0, #0xc
0033b610: mla r0, r0, r2, r3
0033b614: add sp, sp, #8
0033b618: pop {r4, pc}
0033b61c: ldr r0, [pc, #0x34]
0033b620: ldr r1, [pc, #0x34]
0033b624: ldr r2, [pc, #0x34]
0033b628: ldr r0, [r3, r0]
0033b62c: ldr r3, [pc, #0x30]
0033b630: mov ip, #0x35
0033b634: add r1, pc, r1
0033b638: add r2, pc, r2
0033b63c: add r3, pc, r3
0033b640: add r0, r0, #0xa8
0033b644: str ip, [sp]
0033b648: bl #0x30e004
0033b64c: b #0x33b604
0033b650: strhteq sb, [r5], #-0x48
0033b654: andeq r3, r0, r0, asr #19
0033b658: andeq r1, r0, r0, asr #19
0033b65c: subseq r2, r8, r4, lsr #27
0033b660: subseq r4, r8, r0, lsl sl
0033b664: subseq r4, r8, r4, lsr #20

# 0x33b668 _GLOBAL__I_.._.._sources_Core_IO_TouchScreen_TouchScreenBase.cpp
0033b668: push {r4, r5, r6, lr}
0033b66c: ldr r4, [pc, #0x64]
0033b670: ldr r2, [pc, #0x64]
0033b674: ldr r3, [pc, #0x64]
0033b678: add r4, pc, r4
0033b67c: ldr r1, [r4, r2]
0033b680: add r3, pc, r3
0033b684: mov r2, #0x3f000000
0033b688: ldr r0, [r1]
0033b68c: str r2, [r3, #8]
0033b690: str r2, [r3]
0033b694: tst r0, #1
0033b698: str r2, [r3, #4]
0033b69c: beq #0x33b6a4
0033b6a0: pop {r4, r5, r6, pc}
0033b6a4: mov r3, #1
0033b6a8: str r3, [r1]
0033b6ac: ldr r3, [pc, #0x30]
0033b6b0: ldr r5, [r4, r3]
0033b6b4: mov r0, r5
0033b6b8: bl #0x32d79c
0033b6bc: ldr r3, [pc, #0x24]
0033b6c0: mov r0, r5
0033b6c4: ldr r1, [r4, r3]
0033b6c8: ldr r3, [pc, #0x1c]
0033b6cc: ldr r2, [r4, r3]
0033b6d0: pop {r4, r5, r6, lr}
0033b6d4: b #0x30e304
0033b6d8: rsbeq sb, r5, r8, lsl r4
0033b6dc: andeq r0, r0, ip, lsr #31
0033b6e0: rsbeq r6, r6, ip, lsr r7
0033b6e4: strdeq r3, r4, [r0], -r4
0033b6e8: andeq r0, r0, r0, asr #17
0033b6ec: muleq r0, r0, r8

# 0x33bd2c _ZNK15TouchScreenBase14getTouchIDListEv
0033bd2c: push {r4, r5, r6, lr}
0033bd30: mov r4, r0
0033bd34: sub sp, sp, #8
0033bd38: str r0, [r4]
0033bd3c: str r0, [r4, #4]
0033bd40: mov r6, r1
0033bd44: mov r5, #0
0033bd48: b #0x33bd58
0033bd4c: add r5, r5, #1
0033bd50: cmp r5, #8
0033bd54: beq #0x33bd94
0033bd58: ldrb r3, [r6, #0x28]
0033bd5c: add r6, r6, #0x30
0033bd60: cmp r3, #0
0033bd64: beq #0x33bd4c
0033bd68: mov r0, r4
0033bd6c: bl #0x33b6f0
0033bd70: str r5, [r0, #8]
0033bd74: ldr r3, [r4, #4]
0033bd78: add r5, r5, #1
0033bd7c: cmp r5, #8
0033bd80: str r4, [r0]
0033bd84: str r3, [r0, #4]
0033bd88: str r0, [r3]
0033bd8c: str r0, [r4, #4]
0033bd90: bne #0x33bd58
0033bd94: mov r0, r4
0033bd98: add r1, sp, #4
0033bd9c: bl #0x33b710
0033bda0: mov r0, r4
0033bda4: add sp, sp, #8
0033bda8: pop {r4, r5, r6, pc}

# 0x33bdac _ZNSt6vectorIN15TouchScreenBase12_QueuedEventESaIS1_EED1Ev
0033bdac: push {r4, lr}
0033bdb0: mov r4, r0
0033bdb4: ldr r0, [r0]
0033bdb8: cmp r0, #0
0033bdbc: beq #0x33bdf4
0033bdc0: ldr r3, [r4, #8]
0033bdc4: rsb r3, r0, r3
0033bdc8: asr r3, r3, #2
0033bdcc: add r1, r3, r3, lsl #2
0033bdd0: add r1, r1, r1, lsl #4
0033bdd4: add r1, r1, r1, lsl #8
0033bdd8: add r1, r1, r1, lsl #16
0033bddc: add r3, r3, r1, lsl #1
0033bde0: mov r1, #0xc
0033bde4: mul r1, r1, r3
0033bde8: cmp r1, #0x80
0033bdec: bhi #0x33bdfc
0033bdf0: bl #0x708f00
0033bdf4: mov r0, r4
0033bdf8: pop {r4, pc}
0033bdfc: bl #0x310440
0033be00: mov r0, r4
0033be04: pop {r4, pc}

# 0x33be08 _ZN15TouchScreenBaseD1Ev
0033be08: ldr r3, [pc, #0x30]
0033be0c: ldr r2, [pc, #0x30]
0033be10: push {r4, r5, r6, lr}
0033be14: add r3, pc, r3
0033be18: ldr r2, [r3, r2]
0033be1c: mov r4, r0
0033be20: mov r5, r0
0033be24: add r2, r2, #8
0033be28: str r2, [r4], #0x194
0033be2c: bl #0x33b4d8
0033be30: mov r0, r4
0033be34: bl #0x33bdac
0033be38: mov r0, r5
0033be3c: pop {r4, r5, r6, pc}
0033be40: rsbeq r8, r5, ip, ror ip
0033be44: andeq r3, r0, r8, lsl sl

# 0x33be48 _ZN15TouchScreenBaseD0Ev
0033be48: push {r4, lr}
0033be4c: mov r4, r0
0033be50: bl #0x33be08
0033be54: mov r0, r4
0033be58: bl #0x310440
0033be5c: mov r0, r4
0033be60: pop {r4, pc}

# 0x33be64 _ZN15TouchScreenBaseD2Ev
0033be64: ldr r3, [pc, #0x30]
0033be68: ldr r2, [pc, #0x30]
0033be6c: push {r4, r5, r6, lr}
0033be70: add r3, pc, r3
0033be74: ldr r2, [r3, r2]
0033be78: mov r4, r0
0033be7c: mov r5, r0
0033be80: add r2, r2, #8
0033be84: str r2, [r4], #0x194
0033be88: bl #0x33b4d8
0033be8c: mov r0, r4
0033be90: bl #0x33bdac
0033be94: mov r0, r5
0033be98: pop {r4, r5, r6, pc}
0033be9c: rsbeq r8, r5, r0, lsr #24
0033bea0: andeq r3, r0, r8, lsl sl

# 0x33bea4 _ZNSaIN15TouchScreenBase12_QueuedEventEE11_M_allocateEjRj
0033bea4: push {r4, lr}
0033bea8: movw r3, #0x5555
0033beac: orr r3, r3, r3, lsl #14
0033beb0: cmp r1, r3
0033beb4: sub sp, sp, #8
0033beb8: mov r4, r2
0033bebc: bhi #0x33bf14
0033bec0: cmp r1, #0
0033bec4: moveq r0, r1
0033bec8: bne #0x33bed4
0033becc: add sp, sp, #8
0033bed0: pop {r4, pc}
0033bed4: mov r0, #0xc
0033bed8: mul r0, r0, r1
0033bedc: cmp r0, #0x80
0033bee0: str r0, [sp, #4]
0033bee4: bhi #0x33bf0c
0033bee8: add r0, sp, #4
0033beec: bl #0x708ec0
0033bef0: ldr r2, [sp, #4]
0033bef4: movw r3, #0xaaab
0033bef8: movt r3, #0xaaaa
0033befc: umull r1, r3, r3, r2
0033bf00: lsr r3, r3, #3
0033bf04: str r3, [r4]
0033bf08: b #0x33becc
0033bf0c: bl #0x310454
0033bf10: b #0x33bef0
0033bf14: ldr r0, [pc, #0xc]
0033bf18: add r0, pc, r0
0033bf1c: bl #0x30e0c4
0033bf20: mov r0, #1
0033bf24: bl #0x30de48
0033bf28: subseq r2, r8, r8, asr r5

# 0x33bf2c _ZNSt6vectorIN15TouchScreenBase12_QueuedEventESaIS1_EE20_M_allocate_and_copyIPS1_EES5_RjT_S7_
0033bf2c: push {r4, r5, r6, lr}
0033bf30: mov r5, r3
0033bf34: mov r4, r2
0033bf38: rsb r5, r4, r5
0033bf3c: mov r2, r1
0033bf40: add r0, r0, #8
0033bf44: ldr r1, [r1]
0033bf48: bl #0x33bea4
0033bf4c: asr r3, r5, #2
0033bf50: add r5, r3, r3, lsl #2
0033bf54: add r5, r5, r5, lsl #4
0033bf58: add r5, r5, r5, lsl #8
0033bf5c: add r5, r5, r5, lsl #16
0033bf60: add r5, r3, r5, lsl #1
0033bf64: cmp r5, #0
0033bf68: ble #0x33bfa4
0033bf6c: add r4, r4, #0xc
0033bf70: add r3, r0, #0xc
0033bf74: ldr r2, [r4, #-0xc]
0033bf78: subs r5, r5, #1
0033bf7c: str r2, [r3, #-0xc]
0033bf80: ldrh r2, [r4, #-8]
0033bf84: strh r2, [r3, #-8]
0033bf88: ldrh r2, [r4, #-6]
0033bf8c: strh r2, [r3, #-6]
0033bf90: ldr r2, [r4, #-4]
0033bf94: add r4, r4, #0xc
0033bf98: str r2, [r3, #-4]
0033bf9c: add r3, r3, #0xc
0033bfa0: bne #0x33bf74
0033bfa4: pop {r4, r5, r6, pc}

# 0x33bfa8 _ZNSt6vectorIN15TouchScreenBase12_QueuedEventESaIS1_EE7reserveEj.clone.2
0033bfa8: push {r4, r5, r6, lr}
0033bfac: ldr r2, [r0]
0033bfb0: ldr r3, [r0, #8]
0033bfb4: mov r1, #0x10
0033bfb8: sub sp, sp, #8
0033bfbc: rsb r3, r2, r3
0033bfc0: asr r3, r3, #2
0033bfc4: mov r4, r0
0033bfc8: add ip, r3, r3, lsl #2
0033bfcc: mla ip, ip, r1, ip
0033bfd0: str r1, [sp, #4]
0033bfd4: add ip, ip, ip, lsl #8
0033bfd8: add ip, ip, ip, lsl #16
0033bfdc: add r3, r3, ip, lsl #1
0033bfe0: cmp r3, #0xf
0033bfe4: bhi #0x33c0c0
0033bfe8: ldr r3, [r0, #4]
0033bfec: cmp r2, #0
0033bff0: rsb ip, r2, r3
0033bff4: asr ip, ip, #2
0033bff8: add r5, ip, ip, lsl #2
0033bffc: mla r5, r5, r1, r5
0033c000: add r5, r5, r5, lsl #8
0033c004: add r5, r5, r5, lsl #16
0033c008: add r5, ip, r5, lsl #1
0033c00c: beq #0x33c0d0
0033c010: add r1, sp, #4
0033c014: bl #0x33bf2c
0033c018: ldr r3, [r4, #4]
0033c01c: mov r6, r0
0033c020: ldr r0, [r4]
0033c024: cmp r3, r0
0033c028: beq #0x33c068
0033c02c: sub r2, r3, #0xc
0033c030: rsb r2, r0, r2
0033c034: lsr r2, r2, #2
0033c038: add r1, r2, r2, lsl #2
0033c03c: add r1, r1, r1, lsl #5
0033c040: add r1, r2, r1, lsl #1
0033c044: add r1, r1, r1, lsl #5
0033c048: lsl ip, r1, #0xf
0033c04c: rsb r1, r1, ip
0033c050: add r2, r2, r1, lsl #1
0033c054: bic r2, r2, #0xc0000000
0033c058: mvn r1, #0xb
0033c05c: mul r2, r1, r2
0033c060: add r2, r2, r1
0033c064: add r3, r3, r2
0033c068: cmp r3, #0
0033c06c: ldr r2, [r4, #8]
0033c070: beq #0x33c0a4
0033c074: rsb r3, r3, r2
0033c078: asr r3, r3, #2
0033c07c: add r1, r3, r3, lsl #2
0033c080: add r1, r1, r1, lsl #4
0033c084: add r1, r1, r1, lsl #8
0033c088: add r1, r1, r1, lsl #16
0033c08c: add r3, r3, r1, lsl #1
0033c090: mov r1, #0xc
0033c094: mul r1, r1, r3
0033c098: cmp r1, #0x80
0033c09c: bhi #0x33c0c8
0033c0a0: bl #0x708f00
0033c0a4: ldr r2, [sp, #4]
0033c0a8: mov r3, #0xc
0033c0ac: mla r5, r3, r5, r6
0033c0b0: mla r3, r3, r2, r6
0033c0b4: str r5, [r4, #4]
0033c0b8: str r3, [r4, #8]
0033c0bc: str r6, [r4]
0033c0c0: add sp, sp, #8
0033c0c4: pop {r4, r5, r6, pc}
0033c0c8: bl #0x310440
0033c0cc: b #0x33c0a4
0033c0d0: add r0, r0, #8
0033c0d4: add r2, sp, #4
0033c0d8: bl #0x33bea4
0033c0dc: mov r6, r0
0033c0e0: b #0x33c0a4

# 0x33c0e4 _ZNSt6vectorIN15TouchScreenBase12_QueuedEventESaIS1_EE22_M_insert_overflow_auxEPS1_RKS1_RKSt12__false_typejb.clone.5
0033c0e4: push {r4, r5, r6, r7, lr}
0033c0e8: mov r4, r0
0033c0ec: ldm r0, {r0, ip}
0033c0f0: mov r6, r1
0033c0f4: movw r3, #0x5555
0033c0f8: rsb r0, r0, ip
0033c0fc: asr r0, r0, #2
0033c100: orr r3, r3, r3, lsl #14
0033c104: add r1, r0, r0, lsl #2
0033c108: sub sp, sp, #0xc
0033c10c: add r1, r1, r1, lsl #4
0033c110: mov r5, r2
0033c114: add r1, r1, r1, lsl #8
0033c118: add r1, r1, r1, lsl #16
0033c11c: add r0, r0, r1, lsl #1
0033c120: cmp r0, #1
0033c124: addhs r1, r0, r0
0033c128: addlo r1, r0, #1
0033c12c: cmp r1, r3
0033c130: bhi #0x33c284
0033c134: cmp r0, r1
0033c138: bhi #0x33c284
0033c13c: add r2, sp, #8
0033c140: str r1, [r2, #-4]!
0033c144: add r0, r4, #8
0033c148: bl #0x33bea4
0033c14c: ldr r2, [r4]
0033c150: mov r7, r0
0033c154: rsb r6, r2, r6
0033c158: asr r6, r6, #2
0033c15c: add r3, r6, r6, lsl #2
0033c160: add r3, r3, r3, lsl #4
0033c164: add r3, r3, r3, lsl #8
0033c168: add r3, r3, r3, lsl #16
0033c16c: add r6, r6, r3, lsl #1
0033c170: cmp r6, #0
0033c174: movle r3, r0
0033c178: ble #0x33c1c0
0033c17c: add r2, r2, #0xc
0033c180: add r3, r0, #0xc
0033c184: mov r1, r6
0033c188: ldr r0, [r2, #-0xc]
0033c18c: subs r1, r1, #1
0033c190: str r0, [r3, #-0xc]
0033c194: ldrh r0, [r2, #-8]
0033c198: strh r0, [r3, #-8]
0033c19c: ldrh r0, [r2, #-6]
0033c1a0: strh r0, [r3, #-6]
0033c1a4: ldr r0, [r2, #-4]
0033c1a8: add r2, r2, #0xc
0033c1ac: str r0, [r3, #-4]
0033c1b0: add r3, r3, #0xc
0033c1b4: bne #0x33c188
0033c1b8: mov r3, #0xc
0033c1bc: mla r3, r3, r6, r7
0033c1c0: ldr r2, [r5]
0033c1c4: add r6, r3, #0xc
0033c1c8: str r2, [r3]
0033c1cc: ldrh r2, [r5, #4]
0033c1d0: strh r2, [r3, #4]
0033c1d4: ldrh r0, [r5, #6]
0033c1d8: strh r0, [r3, #6]
0033c1dc: ldr r2, [r5, #8]
0033c1e0: str r2, [r3, #8]
0033c1e4: ldm r4, {r0, r3}
0033c1e8: cmp r3, r0
0033c1ec: beq #0x33c22c
0033c1f0: sub r2, r3, #0xc
0033c1f4: rsb r2, r0, r2
0033c1f8: lsr r2, r2, #2
0033c1fc: add r1, r2, r2, lsl #2
0033c200: add r1, r1, r1, lsl #5
0033c204: add r1, r2, r1, lsl #1
0033c208: add r1, r1, r1, lsl #5
0033c20c: lsl ip, r1, #0xf
0033c210: rsb r1, r1, ip
0033c214: add r2, r2, r1, lsl #1
0033c218: bic r2, r2, #0xc0000000
0033c21c: mvn r1, #0xb
0033c220: mul r2, r1, r2
0033c224: add r2, r2, r1
0033c228: add r3, r3, r2
0033c22c: cmp r3, #0
0033c230: ldr r2, [r4, #8]
0033c234: beq #0x33c268
0033c238: rsb r3, r3, r2
0033c23c: asr r3, r3, #2
0033c240: add r1, r3, r3, lsl #2
0033c244: add r1, r1, r1, lsl #4
0033c248: add r1, r1, r1, lsl #8
0033c24c: add r1, r1, r1, lsl #16
0033c250: add r3, r3, r1, lsl #1
0033c254: mov r1, #0xc
0033c258: mul r1, r1, r3
0033c25c: cmp r1, #0x80
0033c260: bhi #0x33c290
0033c264: bl #0x708f00
0033c268: ldr r3, [sp, #4]
0033c26c: mov r2, #0xc
0033c270: str r7, [r4]
0033c274: mla r7, r2, r3, r7
0033c278: stmib r4, {r6, r7}
0033c27c: add sp, sp, #0xc
0033c280: pop {r4, r5, r6, r7, pc}
0033c284: movw r1, #0x5555
0033c288: orr r1, r1, r1, lsl #14
0033c28c: b #0x33c13c
0033c290: bl #0x310440
0033c294: b #0x33c268

# 0x33c298 _ZN15TouchScreenBaseC2Ess
0033c298: ldr r3, [pc, #0x130]
0033c29c: ldr ip, [pc, #0x130]
0033c2a0: push {r4, r5, r6, r7, r8, sl, lr}
0033c2a4: add r3, pc, r3
0033c2a8: ldr ip, [r3, ip]
0033c2ac: mov r4, r0
0033c2b0: sub sp, sp, #0x14
0033c2b4: add ip, ip, #8
0033c2b8: str ip, [r0], #0x38
0033c2bc: add lr, r4, #0x1b8
0033c2c0: mov ip, #0
0033c2c4: mov r6, #0
0033c2c8: strh r6, [r0, #-0x30]
0033c2cc: strh r6, [r0, #-0x2e]
0033c2d0: strh r6, [r0, #-0x2c]
0033c2d4: strh r6, [r0, #-0x2a]
0033c2d8: strh r6, [r0, #-0x28]
0033c2dc: strh r6, [r0, #-0x26]
0033c2e0: strb ip, [r0, #-0x18]
0033c2e4: strb r6, [r0, #-0x10]
0033c2e8: add r0, r0, #0x30
0033c2ec: cmp r0, lr
0033c2f0: bne #0x33c2c4
0033c2f4: mov r3, #0x1ac
0033c2f8: strh r1, [r4, r3]
0033c2fc: movw r3, #0x1ae
0033c300: strh r2, [r4, r3]
0033c304: mov r0, #0
0033c308: mov r1, #0
0033c30c: mov r3, #0x188
0033c310: strd r0, r1, [r4, r3]
0033c314: add sl, r4, #0x194
0033c318: mov r3, #0x3f800000
0033c31c: str r3, [r4, #0x1b0]
0033c320: str r6, [r4, #0x190]
0033c324: str r6, [r4, #0x194]
0033c328: str r6, [r4, #0x198]
0033c32c: str r6, [r4, #0x19c]
0033c330: str r6, [r4, #0x1a0]
0033c334: str r6, [r4, #0x1a4]
0033c338: str r6, [r4, #0x1a8]
0033c33c: mov r0, sl
0033c340: add r7, sp, #4
0033c344: bl #0x33bfa8
0033c348: mov r5, r6
0033c34c: add r8, r7, #8
0033c350: b #0x33c38c
0033c354: ldr r3, [sp, #4]
0033c358: add r6, r6, #1
0033c35c: cmp r6, #0x10
0033c360: str r3, [r1]
0033c364: ldrh r3, [sp, #8]
0033c368: strh r3, [r1, #4]
0033c36c: ldrh r3, [sp, #0xa]
0033c370: strh r3, [r1, #6]
0033c374: ldr r3, [sp, #0xc]
0033c378: str r3, [r1, #8]
0033c37c: ldr r3, [r4, #0x198]
0033c380: add r3, r3, #0xc
0033c384: str r3, [r4, #0x198]
0033c388: beq #0x33c3c4
0033c38c: ldr r1, [r4, #0x198]
0033c390: ldr r3, [r4, #0x19c]
0033c394: str r5, [r7]
0033c398: str r5, [r8]
0033c39c: cmp r1, r3
0033c3a0: strh r5, [sp, #8]
0033c3a4: strh r5, [sp, #0xa]
0033c3a8: bne #0x33c354
0033c3ac: mov r0, sl
0033c3b0: mov r2, r7
0033c3b4: add r6, r6, #1
0033c3b8: bl #0x33c0e4
0033c3bc: cmp r6, #0x10
0033c3c0: bne #0x33c38c
0033c3c4: mov r0, r4
0033c3c8: add sp, sp, #0x14
0033c3cc: pop {r4, r5, r6, r7, r8, sl, pc}
0033c3d0: rsbeq r8, r5, ip, ror #15
0033c3d4: andeq r3, r0, r8, lsl sl

# 0x33c3d8 _ZN15TouchScreenBaseC1Ess
0033c3d8: ldr r3, [pc, #0x130]
0033c3dc: ldr ip, [pc, #0x130]
0033c3e0: push {r4, r5, r6, r7, r8, sl, lr}
0033c3e4: add r3, pc, r3
0033c3e8: ldr ip, [r3, ip]
0033c3ec: mov r4, r0
0033c3f0: sub sp, sp, #0x14
0033c3f4: add ip, ip, #8
0033c3f8: str ip, [r0], #0x38
0033c3fc: add lr, r4, #0x1b8
0033c400: mov ip, #0
0033c404: mov r6, #0
0033c408: strh r6, [r0, #-0x30]
0033c40c: strh r6, [r0, #-0x2e]
0033c410: strh r6, [r0, #-0x2c]
0033c414: strh r6, [r0, #-0x2a]
0033c418: strh r6, [r0, #-0x28]
0033c41c: strh r6, [r0, #-0x26]
0033c420: strb ip, [r0, #-0x18]
0033c424: strb r6, [r0, #-0x10]
0033c428: add r0, r0, #0x30
0033c42c: cmp r0, lr
0033c430: bne #0x33c404
0033c434: mov r3, #0x1ac
0033c438: strh r1, [r4, r3]
0033c43c: movw r3, #0x1ae
0033c440: strh r2, [r4, r3]
0033c444: mov r0, #0
0033c448: mov r1, #0
0033c44c: mov r3, #0x188
0033c450: strd r0, r1, [r4, r3]
0033c454: add sl, r4, #0x194
0033c458: mov r3, #0x3f800000
0033c45c: str r3, [r4, #0x1b0]
0033c460: str r6, [r4, #0x190]
0033c464: str r6, [r4, #0x194]
0033c468: str r6, [r4, #0x198]
0033c46c: str r6, [r4, #0x19c]
0033c470: str r6, [r4, #0x1a0]
0033c474: str r6, [r4, #0x1a4]
0033c478: str r6, [r4, #0x1a8]
0033c47c: mov r0, sl
0033c480: add r7, sp, #4
0033c484: bl #0x33bfa8
0033c488: mov r5, r6
0033c48c: add r8, r7, #8
0033c490: b #0x33c4cc
0033c494: ldr r3, [sp, #4]
0033c498: add r6, r6, #1
0033c49c: cmp r6, #0x10
0033c4a0: str r3, [r1]
0033c4a4: ldrh r3, [sp, #8]
0033c4a8: strh r3, [r1, #4]
0033c4ac: ldrh r3, [sp, #0xa]
0033c4b0: strh r3, [r1, #6]
0033c4b4: ldr r3, [sp, #0xc]
0033c4b8: str r3, [r1, #8]
0033c4bc: ldr r3, [r4, #0x198]
0033c4c0: add r3, r3, #0xc
0033c4c4: str r3, [r4, #0x198]
0033c4c8: beq #0x33c504
0033c4cc: ldr r1, [r4, #0x198]
0033c4d0: ldr r3, [r4, #0x19c]
0033c4d4: str r5, [r7]
0033c4d8: str r5, [r8]
0033c4dc: cmp r1, r3
0033c4e0: strh r5, [sp, #8]
0033c4e4: strh r5, [sp, #0xa]
0033c4e8: bne #0x33c494
0033c4ec: mov r0, sl
0033c4f0: mov r2, r7
0033c4f4: add r6, r6, #1
0033c4f8: bl #0x33c0e4
0033c4fc: cmp r6, #0x10
0033c500: bne #0x33c4cc
0033c504: mov r0, r4
0033c508: add sp, sp, #0x14
0033c50c: pop {r4, r5, r6, r7, r8, sl, pc}
0033c510: rsbeq r8, r5, ip, lsr #13
0033c514: andeq r3, r0, r8, lsl sl

# 0x33c568 _ZN15TouchScreenBase13ProcessEventsEv
0033c568: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0033c56c: ldr r5, [pc, #0x6c0]
0033c570: ldr r2, [pc, #0x6c0]
0033c574: sub sp, sp, #0x11c
0033c578: add r5, pc, r5
0033c57c: ldr r3, [r5, r2]
0033c580: str r2, [sp, #0x2c]
0033c584: ldr r2, [pc, #0x6b0]
0033c588: ldr ip, [r3]
0033c58c: ldr r3, [pc, #0x6ac]
0033c590: mov r8, r0
0033c594: ldr r0, [pc, #0x6a8]
0033c598: ldr r1, [r5, r3]
0033c59c: ldr r3, [pc, #0x6a4]
0033c5a0: add r2, pc, r2
0033c5a4: ldr r7, [r1, #0x14]
0033c5a8: add r3, pc, r3
0033c5ac: add r3, r3, #0x1d
0033c5b0: str r3, [sp, #0x28]
0033c5b4: ldr r3, [pc, #0x690]
0033c5b8: ldr r6, [pc, #0x690]
0033c5bc: add r2, r2, #0x1d
0033c5c0: str ip, [sp, #0x114]
0033c5c4: str r2, [sp, #0x24]
0033c5c8: str r3, [sp, #0x20]
0033c5cc: str r0, [sp, #0x18]
0033c5d0: mov r0, r8
0033c5d4: bl #0x33a9a4
0033c5d8: cmp r0, #0
0033c5dc: bne #0x33c74c
0033c5e0: mov r0, r8
0033c5e4: bl #0x33b5c0
0033c5e8: mov sl, r0
0033c5ec: mov r0, r8
0033c5f0: bl #0x33b510
0033c5f4: ldr r3, [r8, #0x1a8]
0033c5f8: ldrh r0, [sl, #4]
0033c5fc: ldrh fp, [sl, #6]
0033c600: cmp r3, #2
0033c604: beq #0x33c76c
0033c608: cmp r3, #3
0033c60c: beq #0x33c898
0033c610: cmp r3, #1
0033c614: bne #0x33c628
0033c618: movw r3, #0x1ae
0033c61c: ldrh r3, [r8, r3]
0033c620: rsb fp, fp, r3
0033c624: uxth fp, fp
0033c628: ldr r4, [r8, #0x1b0]
0033c62c: sxth r0, r0
0033c630: bl #0x30e964
0033c634: mov r1, r4
0033c638: bl #0x30ed6c
0033c63c: bl #0x30e4cc
0033c640: uxth sb, r0
0033c644: sxth r0, fp
0033c648: bl #0x30e964
0033c64c: mov r1, r0
0033c650: mov r0, r4
0033c654: bl #0x30ed6c
0033c658: bl #0x30e4cc
0033c65c: ldr r3, [sl]
0033c660: uxth r0, r0
0033c664: str r0, [sp, #4]
0033c668: cmp r3, #1
0033c66c: beq #0x33c850
0033c670: cmp r3, #2
0033c674: beq #0x33c790
0033c678: cmp r3, #0
0033c67c: bne #0x33c5d0
0033c680: ldr r2, [sp, #0x18]
0033c684: add r4, sp, #0xfc
0033c688: ldr fp, [r5, r2]
0033c68c: mov r0, fp
0033c690: bl #0x337888
0033c694: mov r0, r4
0033c698: ldr r1, [sp, #0x28]
0033c69c: str r4, [sp, #0x10c]
0033c6a0: str r4, [sp, #0x110]
0033c6a4: bl #0x33c518
0033c6a8: mov r0, fp
0033c6ac: mov r1, r4
0033c6b0: bl #0x337a88
0033c6b4: mov fp, r0
0033c6b8: ldr r0, [sp, #0x110]
0033c6bc: cmp r0, r4
0033c6c0: beq #0x33c6e0
0033c6c4: cmp r0, #0
0033c6c8: beq #0x33c6e0
0033c6cc: ldr r1, [sp, #0xfc]
0033c6d0: rsb r1, r0, r1
0033c6d4: cmp r1, #0x80
0033c6d8: bhi #0x33cc10
0033c6dc: bl #0x708f00
0033c6e0: cmp fp, #0
0033c6e4: bne #0x33c8ac
0033c6e8: ldr r3, [pc, #0x564]
0033c6ec: str r3, [sp, #0xc]
0033c6f0: ldr r0, [sp, #0xc]
0033c6f4: ldr r3, [sl, #8]
0033c6f8: mov ip, #4
0033c6fc: ldr r2, [r5, r0]
0033c700: add r1, sp, #0x84
0033c704: mov r0, r7
0033c708: add r2, r2, #8
0033c70c: str r2, [sp, #0x84]
0033c710: ldr r2, [sp, #4]
0033c714: str r3, [sp, #0x90]
0033c718: mov r3, #1
0033c71c: str ip, [sp, #0x88]
0033c720: strh r2, [sp, #0x8e]
0033c724: strb r3, [sp, #0x94]
0033c728: strh sb, [sp, #0x8c]
0033c72c: bl #0x338ebc
0033c730: ldr r3, [r5, r6]
0033c734: mov r0, r8
0033c738: add r3, r3, #8
0033c73c: str r3, [sp, #0x84]
0033c740: bl #0x33a9a4
0033c744: cmp r0, #0
0033c748: beq #0x33c5e0
0033c74c: ldr r0, [sp, #0x2c]
0033c750: ldr r2, [sp, #0x114]
0033c754: ldr r3, [r5, r0]
0033c758: ldr r3, [r3]
0033c75c: cmp r2, r3
0033c760: bne #0x33cc30
0033c764: add sp, sp, #0x11c
0033c768: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0033c76c: mov r3, #0x1ac
0033c770: ldrh r2, [r8, r3]
0033c774: movw r3, #0x1ae
0033c778: ldrh r3, [r8, r3]
0033c77c: rsb r0, r0, r2
0033c780: uxth r0, r0
0033c784: rsb fp, fp, r3
0033c788: uxth fp, fp
0033c78c: b #0x33c628
0033c790: ldr r3, [sp, #0x18]
0033c794: add r4, sp, #0xe4
0033c798: ldr fp, [r5, r3]
0033c79c: mov r0, fp
0033c7a0: bl #0x337888
0033c7a4: mov r0, r4
0033c7a8: ldr r1, [sp, #0x24]
0033c7ac: str r4, [sp, #0xf4]
0033c7b0: str r4, [sp, #0xf8]
0033c7b4: bl #0x33c518
0033c7b8: mov r0, fp
0033c7bc: mov r1, r4
0033c7c0: bl #0x337a88
0033c7c4: mov fp, r0
0033c7c8: ldr r0, [sp, #0xf8]
0033c7cc: cmp r0, r4
0033c7d0: beq #0x33c7f0
0033c7d4: cmp r0, #0
0033c7d8: beq #0x33c7f0
0033c7dc: ldr r1, [sp, #0xe4]
0033c7e0: rsb r1, r0, r1
0033c7e4: cmp r1, #0x80
0033c7e8: bhi #0x33cc08
0033c7ec: bl #0x708f00
0033c7f0: cmp fp, #0
0033c7f4: bne #0x33ca5c
0033c7f8: ldr r0, [pc, #0x454]
0033c7fc: str r0, [sp, #0xc]
0033c800: ldr r3, [sp, #0xc]
0033c804: mov ip, #4
0033c808: mov r0, r7
0033c80c: ldr r2, [r5, r3]
0033c810: ldr r3, [sl, #8]
0033c814: add r1, sp, #0x34
0033c818: add r2, r2, #8
0033c81c: str r2, [sp, #0x34]
0033c820: ldr r2, [sp, #4]
0033c824: str r3, [sp, #0x40]
0033c828: mov r3, #0
0033c82c: strb r3, [sp, #0x44]
0033c830: str ip, [sp, #0x38]
0033c834: strh sb, [sp, #0x3c]
0033c838: strh r2, [sp, #0x3e]
0033c83c: bl #0x338ebc
0033c840: ldr r3, [r5, r6]
0033c844: add r3, r3, #8
0033c848: str r3, [sp, #0x34]
0033c84c: b #0x33c5d0
0033c850: ldr r3, [sp, #0x20]
0033c854: mov ip, #5
0033c858: mov r0, r7
0033c85c: ldr r2, [r5, r3]
0033c860: ldr r3, [sl, #8]
0033c864: add r1, sp, #0xd4
0033c868: add r2, r2, #8
0033c86c: str r2, [sp, #0xd4]
0033c870: ldr r2, [sp, #4]
0033c874: str r3, [sp, #0xe0]
0033c878: str ip, [sp, #0xd8]
0033c87c: strh sb, [sp, #0xdc]
0033c880: strh r2, [sp, #0xde]
0033c884: bl #0x338ebc
0033c888: ldr r3, [r5, r6]
0033c88c: add r3, r3, #8
0033c890: str r3, [sp, #0xd4]
0033c894: b #0x33c5d0
0033c898: mov r3, #0x1ac
0033c89c: ldrh r3, [r8, r3]
0033c8a0: rsb r0, r0, r3
0033c8a4: uxth r0, r0
0033c8a8: b #0x33c628
0033c8ac: ldr r0, [sp, #4]
0033c8b0: sxth fp, sb
0033c8b4: add r1, fp, #0x63
0033c8b8: sub fp, fp, #0x64
0033c8bc: sxth r0, r0
0033c8c0: cmp fp, r1
0033c8c4: ldr r4, [sl, #8]
0033c8c8: str r0, [sp, #0x10]
0033c8cc: bgt #0x33cc18
0033c8d0: ldr r3, [pc, #0x37c]
0033c8d4: add r2, sp, #0xc0
0033c8d8: str sb, [sp, #8]
0033c8dc: str r3, [sp, #0xc]
0033c8e0: ldr r3, [r5, r3]
0033c8e4: str sl, [sp, #0x14]
0033c8e8: str r8, [sp, #0x1c]
0033c8ec: add r3, r3, #8
0033c8f0: mov r8, r1
0033c8f4: mov sl, r2
0033c8f8: mov sb, r3
0033c8fc: asr r2, r4, #0x1f
0033c900: mov r0, r7
0033c904: lsr r2, r2, #0x1d
0033c908: add r3, r4, r2
0033c90c: and r3, r3, #7
0033c910: rsb r3, r2, r3
0033c914: ldr r2, [sp, #4]
0033c918: str r3, [sp, #0xcc]
0033c91c: mov r3, #4
0033c920: str r3, [sp, #0xc4]
0033c924: mov r1, sl
0033c928: mov r3, #1
0033c92c: strh fp, [sp, #0xc8]
0033c930: strb r3, [sp, #0xd0]
0033c934: str sb, [sp, #0xc0]
0033c938: strh r2, [sp, #0xca]
0033c93c: bl #0x338ebc
0033c940: ldr r3, [r5, r6]
0033c944: add fp, fp, #0x32
0033c948: cmp fp, r8
0033c94c: add r3, r3, #8
0033c950: str r3, [sp, #0xc0]
0033c954: add r4, r4, #1
0033c958: ble #0x33c8fc
0033c95c: ldr sb, [sp, #8]
0033c960: ldr sl, [sp, #0x14]
0033c964: ldr r8, [sp, #0x1c]
0033c968: ldr r0, [sp, #0x10]
0033c96c: ldr r2, [sp, #0x10]
0033c970: add r0, r0, #0x63
0033c974: sub fp, r2, #0x64
0033c978: cmp fp, r0
0033c97c: str r0, [sp, #8]
0033c980: bgt #0x33ca0c
0033c984: ldr r0, [sp, #0xc]
0033c988: add r2, sp, #0xac
0033c98c: str sl, [sp, #0x10]
0033c990: ldr r3, [r5, r0]
0033c994: str r8, [sp, #0x14]
0033c998: mov r8, r2
0033c99c: add r3, r3, #8
0033c9a0: mov sl, r3
0033c9a4: asr r2, r4, #0x1f
0033c9a8: mov r0, r7
0033c9ac: lsr r2, r2, #0x1d
0033c9b0: add r3, r4, r2
0033c9b4: and r3, r3, #7
0033c9b8: rsb r3, r2, r3
0033c9bc: str r3, [sp, #0xb8]
0033c9c0: mov r2, #1
0033c9c4: mov r3, #4
0033c9c8: mov r1, r8
0033c9cc: strh fp, [sp, #0xb6]
0033c9d0: str r3, [sp, #0xb0]
0033c9d4: str sl, [sp, #0xac]
0033c9d8: strh sb, [sp, #0xb4]
0033c9dc: strb r2, [sp, #0xbc]
0033c9e0: bl #0x338ebc
0033c9e4: ldr r3, [r5, r6]
0033c9e8: ldr r0, [sp, #8]
0033c9ec: add fp, fp, #0x32
0033c9f0: add r3, r3, #8
0033c9f4: cmp fp, r0
0033c9f8: str r3, [sp, #0xac]
0033c9fc: add r4, r4, #1
0033ca00: ble #0x33c9a4
0033ca04: ldr sl, [sp, #0x10]
0033ca08: ldr r8, [sp, #0x14]
0033ca0c: ldr r2, [sp, #0xc]
0033ca10: mov r0, r7
0033ca14: add r1, sp, #0x98
0033ca18: ldr r3, [r5, r2]
0033ca1c: mov r2, #4
0033ca20: str r2, [sp, #0x9c]
0033ca24: add r3, r3, #8
0033ca28: str r3, [sp, #0x98]
0033ca2c: mov r3, #0
0033ca30: str r3, [sp, #0xa4]
0033ca34: mov r3, #1
0033ca38: strb r3, [sp, #0xa8]
0033ca3c: mov r3, #0x19
0033ca40: strh r3, [sp, #0xa0]
0033ca44: strh r3, [sp, #0xa2]
0033ca48: bl #0x338ebc
0033ca4c: ldr r3, [r5, r6]
0033ca50: add r3, r3, #8
0033ca54: str r3, [sp, #0x98]
0033ca58: b #0x33c6f0
0033ca5c: ldr r2, [sp, #4]
0033ca60: sxth fp, sb
0033ca64: add r1, fp, #0x63
0033ca68: sub fp, fp, #0x64
0033ca6c: sxth r2, r2
0033ca70: cmp fp, r1
0033ca74: ldr r4, [sl, #8]
0033ca78: str r2, [sp, #0x10]
0033ca7c: bgt #0x33cc24
0033ca80: ldr r0, [pc, #0x1cc]
0033ca84: add r2, sp, #0x70
0033ca88: str sb, [sp, #8]
0033ca8c: ldr r3, [r5, r0]
0033ca90: str sl, [sp, #0x14]
0033ca94: str r8, [sp, #0x1c]
0033ca98: add r3, r3, #8
0033ca9c: str r0, [sp, #0xc]
0033caa0: mov r8, r1
0033caa4: mov sl, r2
0033caa8: mov sb, r3
0033caac: asr r2, r4, #0x1f
0033cab0: mov r0, r7
0033cab4: lsr r2, r2, #0x1d
0033cab8: add r3, r4, r2
0033cabc: and r3, r3, #7
0033cac0: rsb r3, r2, r3
0033cac4: ldr r2, [sp, #4]
0033cac8: str r3, [sp, #0x7c]
0033cacc: mov r3, #4
0033cad0: str r3, [sp, #0x74]
0033cad4: mov r1, sl
0033cad8: mov r3, #0
0033cadc: strh fp, [sp, #0x78]
0033cae0: strb r3, [sp, #0x80]
0033cae4: str sb, [sp, #0x70]
0033cae8: strh r2, [sp, #0x7a]
0033caec: bl #0x338ebc
0033caf0: ldr r3, [r5, r6]
0033caf4: add fp, fp, #0x32
0033caf8: cmp fp, r8
0033cafc: add r3, r3, #8
0033cb00: str r3, [sp, #0x70]
0033cb04: add r4, r4, #1
0033cb08: ble #0x33caac
0033cb0c: ldr sb, [sp, #8]
0033cb10: ldr sl, [sp, #0x14]
0033cb14: ldr r8, [sp, #0x1c]
0033cb18: ldr r0, [sp, #0x10]
0033cb1c: ldr r2, [sp, #0x10]
0033cb20: add r0, r0, #0x63
0033cb24: sub fp, r2, #0x64
0033cb28: cmp fp, r0
0033cb2c: str r0, [sp, #8]
0033cb30: bgt #0x33cbbc
0033cb34: ldr r0, [sp, #0xc]
0033cb38: add r2, sp, #0x5c
0033cb3c: str sl, [sp, #0x10]
0033cb40: ldr r3, [r5, r0]
0033cb44: str r8, [sp, #0x14]
0033cb48: mov r8, r2
0033cb4c: add r3, r3, #8
0033cb50: mov sl, r3
0033cb54: asr r2, r4, #0x1f
0033cb58: mov r0, r7
0033cb5c: lsr r2, r2, #0x1d
0033cb60: add r3, r4, r2
0033cb64: and r3, r3, #7
0033cb68: rsb r3, r2, r3
0033cb6c: str r3, [sp, #0x68]
0033cb70: mov r2, #0
0033cb74: mov r3, #4
0033cb78: mov r1, r8
0033cb7c: strh fp, [sp, #0x66]
0033cb80: str r3, [sp, #0x60]
0033cb84: str sl, [sp, #0x5c]
0033cb88: strh sb, [sp, #0x64]
0033cb8c: strb r2, [sp, #0x6c]
0033cb90: bl #0x338ebc
0033cb94: ldr r3, [r5, r6]
0033cb98: ldr r0, [sp, #8]
0033cb9c: add fp, fp, #0x32
0033cba0: add r3, r3, #8
0033cba4: cmp fp, r0
0033cba8: str r3, [sp, #0x5c]
0033cbac: add r4, r4, #1
0033cbb0: ble #0x33cb54
0033cbb4: ldr sl, [sp, #0x10]
0033cbb8: ldr r8, [sp, #0x14]
0033cbbc: ldr r2, [sp, #0xc]
0033cbc0: mov r3, #0
0033cbc4: mov r0, r7
0033cbc8: ldr ip, [r5, r2]
0033cbcc: mov r2, #4
0033cbd0: str r2, [sp, #0x4c]
0033cbd4: add ip, ip, #8
0033cbd8: mov r2, #0x19
0033cbdc: add r1, sp, #0x48
0033cbe0: strb r3, [sp, #0x58]
0033cbe4: str r3, [sp, #0x54]
0033cbe8: str ip, [sp, #0x48]
0033cbec: strh r2, [sp, #0x50]
0033cbf0: strh r2, [sp, #0x52]
0033cbf4: bl #0x338ebc
0033cbf8: ldr r3, [r5, r6]
0033cbfc: add r3, r3, #8
0033cc00: str r3, [sp, #0x48]
0033cc04: b #0x33c800
0033cc08: bl #0x310440
0033cc0c: b #0x33c7f0
0033cc10: bl #0x310440
0033cc14: b #0x33c6e0
0033cc18: ldr r2, [pc, #0x34]
0033cc1c: str r2, [sp, #0xc]
0033cc20: b #0x33c968
0033cc24: ldr r3, [pc, #0x28]
0033cc28: str r3, [sp, #0xc]
0033cc2c: b #0x33cb18
0033cc30: bl #0x30e310
0033cc34: rsbeq r8, r5, r8, lsl r5
0033cc38: andeq r4, r0, ip, lsr #1
0033cc3c: subseq r3, r8, r0, lsr #22
0033cc40: strdeq r3, r4, [r0], -r4
0033cc44: andeq r0, r0, r4, lsl #17
0033cc48: subseq r3, r8, r8, lsl fp
0033cc4c: andeq r0, r0, r8, ror #23
0033cc50: strheq r0, [r0], -r0
0033cc54: andeq r3, r0, ip, lsr #6

# 0x33cc58 _ZNK17TouchScreenIPhone12getLeftBoundEv
0033cc58: mov r3, #0x1b4
0033cc5c: ldrsh r0, [r0, r3]
0033cc60: bx lr

# 0x33cc64 _ZNK17TouchScreenIPhone13getRightBoundEv
0033cc64: movw r3, #0x1b6
0033cc68: ldrsh r0, [r0, r3]
0033cc6c: bx lr

# 0x33cc70 _ZNK17TouchScreenIPhone11getTopBoundEv
0033cc70: mov r3, #0x1b8
0033cc74: ldrsh r0, [r0, r3]
0033cc78: bx lr

# 0x33cc7c _ZNK17TouchScreenIPhone14getBottomBoundEv
0033cc7c: movw r3, #0x1ba
0033cc80: ldrsh r0, [r0, r3]
0033cc84: bx lr

# 0x33cc88 _GLOBAL__I_.._.._sources_Core_IO_TouchScreen_TouchScreenIPhone.cpp
0033cc88: ldr r3, [pc, #0x14]
0033cc8c: mov r2, #0x3f000000
0033cc90: add r3, pc, r3
0033cc94: str r2, [r3, #8]
0033cc98: str r2, [r3]
0033cc9c: str r2, [r3, #4]
0033cca0: bx lr
0033cca4: rsbeq r5, r6, r8, lsr r1

# 0x33cca8 _ZNK17TouchScreenIPhone18getTouchIDInRegionERKN6glitch4core4rectIfEE
0033cca8: b #0x33b0e8

# 0x33ccac _ZNK17TouchScreenIPhone15isRegionPressedERKN6glitch4core4rectIfEE
0033ccac: b #0x33aebc

# 0x33ccb0 _ZN17TouchScreenIPhoneD1Ev
0033ccb0: ldr r3, [pc, #0x24]
0033ccb4: ldr r2, [pc, #0x24]
0033ccb8: push {r4, lr}
0033ccbc: add r3, pc, r3
0033ccc0: ldr r2, [r3, r2]
0033ccc4: mov r4, r0
0033ccc8: add r2, r2, #8
0033cccc: str r2, [r0]
0033ccd0: bl #0x33be64
0033ccd4: mov r0, r4
0033ccd8: pop {r4, pc}

# 0x33cce4 _ZN17TouchScreenIPhoneD0Ev
0033cce4: push {r4, lr}
0033cce8: mov r4, r0
0033ccec: bl #0x33ccb0
0033ccf0: mov r0, r4
0033ccf4: bl #0x310440
0033ccf8: mov r0, r4
0033ccfc: pop {r4, pc}

# 0x33cd00 _ZN17TouchScreenIPhoneD2Ev
0033cd00: ldr r3, [pc, #0x24]
0033cd04: ldr r2, [pc, #0x24]
0033cd08: push {r4, lr}
0033cd0c: add r3, pc, r3
0033cd10: ldr r2, [r3, r2]
0033cd14: mov r4, r0
0033cd18: add r2, r2, #8
0033cd1c: str r2, [r0]
0033cd20: bl #0x33be64
0033cd24: mov r0, r4
0033cd28: pop {r4, pc}
0033cd2c: rsbeq r7, r5, r4, lsl #27
0033cd30: andeq r1, r0, r8, asr #25

# 0x33cd34 _ZN17TouchScreenIPhoneC1Efffff
0033cd34: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033cd38: mov r4, r0
0033cd3c: mov r0, r3
0033cd40: mov sl, r1
0033cd44: mov r8, r2
0033cd48: bl #0x30e4cc
0033cd4c: uxth r7, r0
0033cd50: ldr r0, [sp, #0x20]
0033cd54: bl #0x30e4cc
0033cd58: uxth r6, r0
0033cd5c: sxth r1, r7
0033cd60: sxth r2, r6
0033cd64: mov r0, r4
0033cd68: ldr r5, [pc, #0x54]
0033cd6c: bl #0x33c298
0033cd70: ldr r3, [pc, #0x50]
0033cd74: add r5, pc, r5
0033cd78: mov r0, sl
0033cd7c: ldr r3, [r5, r3]
0033cd80: add r3, r3, #8
0033cd84: str r3, [r4]
0033cd88: bl #0x30e4cc
0033cd8c: mov r3, #0x1b4
0033cd90: strh r0, [r4, r3]
0033cd94: movw r3, #0x1b6
0033cd98: strh r7, [r4, r3]
0033cd9c: mov r0, r8
0033cda0: bl #0x30e4cc
0033cda4: mov r3, #0x1b8
0033cda8: strh r0, [r4, r3]
0033cdac: movw r3, #0x1ba
0033cdb0: strh r6, [r4, r3]
0033cdb4: ldr r3, [sp, #0x24]
0033cdb8: mov r0, r4
0033cdbc: str r3, [r4, #0x1b0]
0033cdc0: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033cdc4: rsbeq r7, r5, ip, lsl sp
0033cdc8: andeq r1, r0, r8, asr #25

# 0x33cdcc _ZN17TouchScreenIPhoneC2Efffff
0033cdcc: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033cdd0: mov r4, r0
0033cdd4: mov r0, r3
0033cdd8: mov sl, r1
0033cddc: mov r8, r2
0033cde0: bl #0x30e4cc
0033cde4: uxth r7, r0
0033cde8: ldr r0, [sp, #0x20]
0033cdec: bl #0x30e4cc
0033cdf0: uxth r6, r0
0033cdf4: sxth r1, r7
0033cdf8: sxth r2, r6
0033cdfc: mov r0, r4
0033ce00: ldr r5, [pc, #0x54]
0033ce04: bl #0x33c298
0033ce08: ldr r3, [pc, #0x50]
0033ce0c: add r5, pc, r5
0033ce10: mov r0, sl
0033ce14: ldr r3, [r5, r3]
0033ce18: add r3, r3, #8
0033ce1c: str r3, [r4]
0033ce20: bl #0x30e4cc
0033ce24: mov r3, #0x1b4
0033ce28: strh r0, [r4, r3]
0033ce2c: movw r3, #0x1b6
0033ce30: strh r7, [r4, r3]
0033ce34: mov r0, r8
0033ce38: bl #0x30e4cc
0033ce3c: mov r3, #0x1b8
0033ce40: strh r0, [r4, r3]
0033ce44: movw r3, #0x1ba
0033ce48: strh r6, [r4, r3]
0033ce4c: ldr r3, [sp, #0x24]
0033ce50: mov r0, r4
0033ce54: str r3, [r4, #0x1b0]
0033ce58: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033ce5c: rsbeq r7, r5, r4, lsl #25
0033ce60: andeq r1, r0, r8, asr #25

# 0x33ceb4 _ZN17TouchScreenIPhone10touchEndedEffdiffl
0033ceb4: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033ceb8: ldr r4, [pc, #0xb4]
0033cebc: ldr r3, [pc, #0xb4]
0033cec0: sub sp, sp, #0x20
0033cec4: add r4, pc, r4
0033cec8: ldr r5, [r4, r3]
0033cecc: mov r7, r1
0033ced0: ldr r1, [sp, #0x54]
0033ced4: ldr r3, [r5]
0033ced8: mov sb, r2
0033cedc: mov r6, r0
0033cee0: str r3, [sp, #0x1c]
0033cee4: bl #0x33b3dc
0033cee8: ldr r3, [pc, #0x8c]
0033ceec: mov sl, r0
0033cef0: add r8, sp, #4
0033cef4: ldr r4, [r4, r3]
0033cef8: mov r0, r4
0033cefc: bl #0x337888
0033cf00: ldr r1, [pc, #0x78]
0033cf04: mov r0, r8
0033cf08: str r8, [sp, #0x14]
0033cf0c: add r1, pc, r1
0033cf10: add r1, r1, #0x12
0033cf14: str r8, [sp, #0x18]
0033cf18: bl #0x33ce64
0033cf1c: mov r1, r8
0033cf20: mov r0, r4
0033cf24: bl #0x337a88
0033cf28: mov r0, r8
0033cf2c: bl #0x3139ac
0033cf30: mov r0, r7
0033cf34: bl #0x30e4cc
0033cf38: strh r0, [sp]
0033cf3c: mov r0, sb
0033cf40: bl #0x30e4cc
0033cf44: mov r2, sl
0033cf48: strh r0, [sp, #2]
0033cf4c: mov r1, sp
0033cf50: mov r0, r6
0033cf54: bl #0x33aeb8
0033cf58: ldr r2, [sp, #0x1c]
0033cf5c: ldr r3, [r5]
0033cf60: cmp r2, r3
0033cf64: bne #0x33cf70
0033cf68: add sp, sp, #0x20
0033cf6c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033cf70: bl #0x30e310
0033cf74: rsbeq r7, r5, ip, asr #23
0033cf78: andeq r4, r0, ip, lsr #1
0033cf7c: andeq r0, r0, r4, lsl #17
0033cf80: ldrsbeq r3, [r8], #-0x14

# 0x33cf84 _ZN17TouchScreenIPhone14touchCancelledEffdiffl
0033cf84: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033cf88: ldr r4, [pc, #0xb4]
0033cf8c: ldr r3, [pc, #0xb4]
0033cf90: sub sp, sp, #0x20
0033cf94: add r4, pc, r4
0033cf98: ldr r5, [r4, r3]
0033cf9c: mov r7, r1
0033cfa0: ldr r1, [sp, #0x54]
0033cfa4: ldr r3, [r5]
0033cfa8: mov sb, r2
0033cfac: mov r6, r0
0033cfb0: str r3, [sp, #0x1c]
0033cfb4: bl #0x33b3dc
0033cfb8: ldr r3, [pc, #0x8c]
0033cfbc: mov sl, r0
0033cfc0: add r8, sp, #4
0033cfc4: ldr r4, [r4, r3]
0033cfc8: mov r0, r4
0033cfcc: bl #0x337888
0033cfd0: ldr r1, [pc, #0x78]
0033cfd4: mov r0, r8
0033cfd8: str r8, [sp, #0x14]
0033cfdc: add r1, pc, r1
0033cfe0: add r1, r1, #0x12
0033cfe4: str r8, [sp, #0x18]
0033cfe8: bl #0x33ce64
0033cfec: mov r1, r8
0033cff0: mov r0, r4
0033cff4: bl #0x337a88
0033cff8: mov r0, r8
0033cffc: bl #0x3139ac
0033d000: mov r0, r7
0033d004: bl #0x30e4cc
0033d008: strh r0, [sp]
0033d00c: mov r0, sb
0033d010: bl #0x30e4cc
0033d014: mov r2, sl
0033d018: strh r0, [sp, #2]
0033d01c: mov r1, sp
0033d020: mov r0, r6
0033d024: bl #0x33ae3c
0033d028: ldr r2, [sp, #0x1c]
0033d02c: ldr r3, [r5]
0033d030: cmp r2, r3
0033d034: bne #0x33d040
0033d038: add sp, sp, #0x20
0033d03c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033d040: bl #0x30e310

# 0x33d054 _ZN17TouchScreenIPhone10touchBeganEffdiffl
0033d054: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033d058: ldr r4, [pc, #0xb4]
0033d05c: ldr r3, [pc, #0xb4]
0033d060: sub sp, sp, #0x20
0033d064: add r4, pc, r4
0033d068: ldr r5, [r4, r3]
0033d06c: mov r7, r1
0033d070: ldr r1, [sp, #0x54]
0033d074: ldr r3, [r5]
0033d078: mov sb, r2
0033d07c: mov r6, r0
0033d080: str r3, [sp, #0x1c]
0033d084: bl #0x33b3dc
0033d088: ldr r3, [pc, #0x8c]
0033d08c: mov sl, r0
0033d090: add r8, sp, #4
0033d094: ldr r4, [r4, r3]
0033d098: mov r0, r4
0033d09c: bl #0x337888
0033d0a0: ldr r1, [pc, #0x78]
0033d0a4: mov r0, r8
0033d0a8: str r8, [sp, #0x14]
0033d0ac: add r1, pc, r1
0033d0b0: add r1, r1, #0x12
0033d0b4: str r8, [sp, #0x18]
0033d0b8: bl #0x33ce64
0033d0bc: mov r1, r8
0033d0c0: mov r0, r4
0033d0c4: bl #0x337a88
0033d0c8: mov r0, r8
0033d0cc: bl #0x3139ac
0033d0d0: mov r0, r7
0033d0d4: bl #0x30e4cc
0033d0d8: strh r0, [sp]
0033d0dc: mov r0, sb
0033d0e0: bl #0x30e4cc
0033d0e4: mov r2, sl
0033d0e8: strh r0, [sp, #2]
0033d0ec: mov r1, sp
0033d0f0: mov r0, r6
0033d0f4: bl #0x33ac24
0033d0f8: ldr r2, [sp, #0x1c]
0033d0fc: ldr r3, [r5]
0033d100: cmp r2, r3
0033d104: bne #0x33d110
0033d108: add sp, sp, #0x20
0033d10c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033d110: bl #0x30e310
0033d114: rsbeq r7, r5, ip, lsr #20
0033d118: andeq r4, r0, ip, lsr #1
0033d11c: andeq r0, r0, r4, lsl #17
0033d120: subseq r3, r8, r4, lsr r0

# 0x33d124 _ZN17TouchScreenIPhone10touchMovedEffdiffl
0033d124: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033d128: ldr r4, [pc, #0xb4]
0033d12c: ldr r3, [pc, #0xb4]
0033d130: sub sp, sp, #0x20
0033d134: add r4, pc, r4
0033d138: ldr r5, [r4, r3]
0033d13c: mov r7, r1
0033d140: ldr r1, [sp, #0x54]
0033d144: ldr r3, [r5]
0033d148: mov sb, r2
0033d14c: mov r6, r0
0033d150: str r3, [sp, #0x1c]
0033d154: bl #0x33b3dc
0033d158: ldr r3, [pc, #0x8c]
0033d15c: mov sl, r0
0033d160: add r8, sp, #4
0033d164: ldr r4, [r4, r3]
0033d168: mov r0, r4
0033d16c: bl #0x337888
0033d170: ldr r1, [pc, #0x78]
0033d174: mov r0, r8
0033d178: str r8, [sp, #0x14]
0033d17c: add r1, pc, r1
0033d180: add r1, r1, #0x12
0033d184: str r8, [sp, #0x18]
0033d188: bl #0x33ce64
0033d18c: mov r1, r8
0033d190: mov r0, r4
0033d194: bl #0x337a88
0033d198: mov r0, r8
0033d19c: bl #0x3139ac
0033d1a0: mov r0, r7
0033d1a4: bl #0x30e4cc
0033d1a8: strh r0, [sp]
0033d1ac: mov r0, sb
0033d1b0: bl #0x30e4cc
0033d1b4: mov r2, sl
0033d1b8: strh r0, [sp, #2]
0033d1bc: mov r1, sp
0033d1c0: mov r0, r6
0033d1c4: bl #0x33ad28
0033d1c8: ldr r2, [sp, #0x1c]
0033d1cc: ldr r3, [r5]
0033d1d0: cmp r2, r3
0033d1d4: bne #0x33d1e0
0033d1d8: add sp, sp, #0x20
0033d1dc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033d1e0: bl #0x30e310
0033d1e4: rsbeq r7, r5, ip, asr sb
0033d1e8: andeq r4, r0, ip, lsr #1
0033d1ec: andeq r0, r0, r4, lsl #17
0033d1f0: subseq r2, r8, r4, ror #30

# 0x33d1f4 _ZNK14TouchScreenPS312getLeftBoundEv
0033d1f4: mov r0, #0
0033d1f8: bx lr

# 0x33d1fc _ZNK14TouchScreenPS313getRightBoundEv
0033d1fc: mov r0, #0
0033d200: bx lr

# 0x33d204 _ZNK14TouchScreenPS311getTopBoundEv
0033d204: mov r0, #0
0033d208: bx lr

# 0x33d20c _ZNK14TouchScreenPS314getBottomBoundEv
0033d20c: mov r0, #0
0033d210: bx lr

# 0x33d214 _ZThn436_N14TouchScreenPS37onEventERKN6glitch6SEventE
0033d214: sub r0, r0, #0x1b4
0033d218: b #0x33d21c

# 0x33d21c _ZN14TouchScreenPS37onEventERKN6glitch6SEventE
0033d21c: push {r4, lr}
0033d220: ldr r4, [r1]
0033d224: sub sp, sp, #0x10
0033d228: mov r3, r1
0033d22c: cmp r4, #1
0033d230: beq #0x33d240
0033d234: mov r0, #0
0033d238: add sp, sp, #0x10
0033d23c: pop {r4, pc}
0033d240: ldr r2, [r1, #0x14]
0033d244: cmp r2, #3
0033d248: beq #0x33d2d0
0033d24c: cmp r2, #6
0033d250: beq #0x33d290
0033d254: cmp r2, #0
0033d258: bne #0x33d234
0033d25c: ldr lr, [pc, #0xa8]
0033d260: add r1, sp, #0xc
0033d264: add lr, pc, lr
0033d268: strb r4, [lr]
0033d26c: ldr ip, [r0]
0033d270: ldrh lr, [r3, #0xc]
0033d274: ldrh r3, [r3, #8]
0033d278: ldr ip, [ip, #0x20]
0033d27c: strh lr, [sp, #0xe]
0033d280: strh r3, [sp, #0xc]
0033d284: blx ip
0033d288: mov r0, r4
0033d28c: b #0x33d238
0033d290: ldr r2, [pc, #0x78]
0033d294: add r2, pc, r2
0033d298: ldrb r2, [r2]
0033d29c: cmp r2, #0
0033d2a0: beq #0x33d234
0033d2a4: ldr ip, [r0]
0033d2a8: ldrh r2, [r1, #0xc]
0033d2ac: ldrh r3, [r3, #8]
0033d2b0: ldr ip, [ip, #0x24]
0033d2b4: add r1, sp, #4
0033d2b8: strh r2, [sp, #6]
0033d2bc: strh r3, [sp, #4]
0033d2c0: mov r2, #0
0033d2c4: blx ip
0033d2c8: mov r0, r4
0033d2cc: b #0x33d238
0033d2d0: ldr r1, [pc, #0x3c]
0033d2d4: mov lr, #0
0033d2d8: mov r2, lr
0033d2dc: add r1, pc, r1
0033d2e0: strb lr, [r1]
0033d2e4: ldr ip, [r0]
0033d2e8: ldrh lr, [r3, #0xc]
0033d2ec: ldrh r3, [r3, #8]
0033d2f0: ldr ip, [ip, #0x28]
0033d2f4: add r1, sp, #8
0033d2f8: strh r3, [sp, #8]
0033d2fc: strh lr, [sp, #0xa]
0033d300: blx ip
0033d304: mov r0, r4
0033d308: b #0x33d238
0033d30c: rsbeq r4, r6, r0, ror fp
0033d310: rsbeq r4, r6, r0, asr #22

# 0x33d318 _GLOBAL__I_.._.._sources_Core_IO_TouchScreen_TouchScreenPS3.cpp
0033d318: ldr r3, [pc, #0x14]
0033d31c: mov r2, #0x3f000000
0033d320: add r3, pc, r3
0033d324: str r2, [r3, #0xc]
0033d328: str r2, [r3, #4]
0033d32c: str r2, [r3, #8]
0033d330: bx lr
0033d334: strhteq r4, [r6], #-0xa4

# 0x33d338 _ZThn436_N14TouchScreenPS3D1Ev
0033d338: sub r0, r0, #0x1b4
0033d33c: b #0x33d340

# 0x33d340 _ZN14TouchScreenPS3D1Ev
0033d340: ldr r3, [pc, #0x34]
0033d344: ldr r1, [pc, #0x34]
0033d348: ldr r2, [pc, #0x34]
0033d34c: add r3, pc, r3
0033d350: ldr r1, [r3, r1]
0033d354: ldr r2, [r3, r2]
0033d358: push {r4, lr}
0033d35c: add r1, r1, #8
0033d360: add r2, r2, #8
0033d364: mov r4, r0
0033d368: str r1, [r0]
0033d36c: str r2, [r0, #0x1b4]
0033d370: bl #0x33be64
0033d374: mov r0, r4
0033d378: pop {r4, pc}
0033d37c: rsbeq r7, r5, r4, asr #14
0033d380: andeq r1, r0, ip, lsl #4
0033d384: andeq r2, r0, ip, asr #14

# 0x33d388 _ZThn436_N14TouchScreenPS3D0Ev
0033d388: sub r0, r0, #0x1b4
0033d38c: b #0x33d390

# 0x33d390 _ZN14TouchScreenPS3D0Ev
0033d390: push {r4, lr}
0033d394: mov r4, r0
0033d398: bl #0x33d340
0033d39c: mov r0, r4
0033d3a0: bl #0x310440
0033d3a4: mov r0, r4
0033d3a8: pop {r4, pc}

# 0x33d3ac _ZN14TouchScreenPS3D2Ev
0033d3ac: ldr r3, [pc, #0x34]
0033d3b0: ldr r1, [pc, #0x34]
0033d3b4: ldr r2, [pc, #0x34]
0033d3b8: add r3, pc, r3
0033d3bc: ldr r1, [r3, r1]
0033d3c0: ldr r2, [r3, r2]
0033d3c4: push {r4, lr}
0033d3c8: add r1, r1, #8
0033d3cc: add r2, r2, #8
0033d3d0: mov r4, r0
0033d3d4: str r1, [r0]
0033d3d8: str r2, [r0, #0x1b4]
0033d3dc: bl #0x33be64
0033d3e0: mov r0, r4
0033d3e4: pop {r4, pc}

# 0x33d3f4 _ZN14TouchScreenPS3C1Ev
0033d3f4: mov r1, #0
0033d3f8: push {r4, r5, r6, lr}
0033d3fc: mov r2, r1
0033d400: ldr r5, [pc, #0x30]
0033d404: mov r4, r0
0033d408: bl #0x33c298
0033d40c: ldr r3, [pc, #0x28]
0033d410: add r5, pc, r5
0033d414: mov r2, #0
0033d418: ldr r3, [r5, r3]
0033d41c: str r2, [r4, #0x190]
0033d420: mov r0, r4
0033d424: add r2, r3, #0x68
0033d428: add r3, r3, #8
0033d42c: str r3, [r4]
0033d430: str r2, [r4, #0x1b4]
0033d434: pop {r4, r5, r6, pc}
0033d438: rsbeq r7, r5, r0, lsl #13
0033d43c: andeq r1, r0, ip, lsl #4

# 0x33d440 _ZN14TouchScreenPS3C2Ev
0033d440: mov r1, #0
0033d444: push {r4, r5, r6, lr}
0033d448: mov r2, r1
0033d44c: ldr r5, [pc, #0x30]
0033d450: mov r4, r0
0033d454: bl #0x33c298
0033d458: ldr r3, [pc, #0x28]
0033d45c: add r5, pc, r5
0033d460: mov r2, #0
0033d464: ldr r3, [r5, r3]
0033d468: str r2, [r4, #0x190]
0033d46c: mov r0, r4
0033d470: add r2, r3, #0x68
0033d474: add r3, r3, #8
0033d478: str r3, [r4]
0033d47c: str r2, [r4, #0x1b4]
0033d480: pop {r4, r5, r6, pc}
0033d484: rsbeq r7, r5, r4, lsr r6
0033d488: andeq r1, r0, ip, lsl #4

# 0x33d48c _ZNK16TouchScreenWin3212getLeftBoundEv
0033d48c: mov r0, #0
0033d490: bx lr

# 0x33d494 _ZNK16TouchScreenWin3213getRightBoundEv
0033d494: mov r0, #0
0033d498: bx lr

# 0x33d49c _ZNK16TouchScreenWin3211getTopBoundEv
0033d49c: mov r0, #0
0033d4a0: bx lr

# 0x33d4a4 _ZNK16TouchScreenWin3214getBottomBoundEv
0033d4a4: mov r0, #0
0033d4a8: bx lr

# 0x33d4ac _ZThn436_N16TouchScreenWin327onEventERKN6glitch6SEventE
0033d4ac: sub r0, r0, #0x1b4
0033d4b0: b #0x33d4b4

# 0x33d4b4 _ZN16TouchScreenWin327onEventERKN6glitch6SEventE
0033d4b4: push {r4, lr}
0033d4b8: ldr r4, [r1]
0033d4bc: sub sp, sp, #0x10
0033d4c0: mov r3, r1
0033d4c4: cmp r4, #1
0033d4c8: beq #0x33d4d8
0033d4cc: mov r0, #0
0033d4d0: add sp, sp, #0x10
0033d4d4: pop {r4, pc}
0033d4d8: ldr r2, [r1, #0x14]
0033d4dc: cmp r2, #3
0033d4e0: beq #0x33d568
0033d4e4: cmp r2, #6
0033d4e8: beq #0x33d528
0033d4ec: cmp r2, #0
0033d4f0: bne #0x33d4cc
0033d4f4: ldr lr, [pc, #0xa8]
0033d4f8: add r1, sp, #0xc
0033d4fc: add lr, pc, lr
0033d500: strb r4, [lr]
0033d504: ldr ip, [r0]
0033d508: ldrh lr, [r3, #0xc]
0033d50c: ldrh r3, [r3, #8]
0033d510: ldr ip, [ip, #0x20]
0033d514: strh lr, [sp, #0xe]
0033d518: strh r3, [sp, #0xc]
0033d51c: blx ip
0033d520: mov r0, r4
0033d524: b #0x33d4d0
0033d528: ldr r2, [pc, #0x78]
0033d52c: add r2, pc, r2
0033d530: ldrb r2, [r2]
0033d534: cmp r2, #0
0033d538: beq #0x33d4cc
0033d53c: ldr ip, [r0]
0033d540: ldrh r2, [r1, #0xc]
0033d544: ldrh r3, [r3, #8]
0033d548: ldr ip, [ip, #0x24]
0033d54c: add r1, sp, #4
0033d550: strh r2, [sp, #6]
0033d554: strh r3, [sp, #4]
0033d558: mov r2, #0
0033d55c: blx ip
0033d560: mov r0, r4
0033d564: b #0x33d4d0
0033d568: ldr r1, [pc, #0x3c]
0033d56c: mov lr, #0
0033d570: mov r2, lr
0033d574: add r1, pc, r1
0033d578: strb lr, [r1]
0033d57c: ldr ip, [r0]
0033d580: ldrh lr, [r3, #0xc]
0033d584: ldrh r3, [r3, #8]
0033d588: ldr ip, [ip, #0x28]
0033d58c: add r1, sp, #8
0033d590: strh r3, [sp, #8]
0033d594: strh lr, [sp, #0xa]
0033d598: blx ip
0033d59c: mov r0, r4
0033d5a0: b #0x33d4d0
0033d5a4: rsbeq r4, r6, r8, ror #17
0033d5a8: strhteq r4, [r6], #-0x88
0033d5ac: rsbeq r4, r6, r0, ror r8

# 0x33d5b0 _GLOBAL__I_.._.._sources_Core_IO_TouchScreen_TouchScreenWin32.cpp
0033d5b0: ldr r3, [pc, #0x14]
0033d5b4: mov r2, #0x3f000000
0033d5b8: add r3, pc, r3
0033d5bc: str r2, [r3, #0xc]
0033d5c0: str r2, [r3, #4]
0033d5c4: str r2, [r3, #8]
0033d5c8: bx lr
0033d5cc: rsbeq r4, r6, ip, lsr #16

# 0x33d5d0 _ZThn436_N16TouchScreenWin32D1Ev
0033d5d0: sub r0, r0, #0x1b4
0033d5d4: b #0x33d5d8

# 0x33d5d8 _ZN16TouchScreenWin32D1Ev
0033d5d8: ldr r3, [pc, #0x34]
0033d5dc: ldr r1, [pc, #0x34]
0033d5e0: ldr r2, [pc, #0x34]
0033d5e4: add r3, pc, r3
0033d5e8: ldr r1, [r3, r1]
0033d5ec: ldr r2, [r3, r2]
0033d5f0: push {r4, lr}
0033d5f4: add r1, r1, #8
0033d5f8: add r2, r2, #8
0033d5fc: mov r4, r0
0033d600: str r1, [r0]
0033d604: str r2, [r0, #0x1b4]
0033d608: bl #0x33be64
0033d60c: mov r0, r4
0033d610: pop {r4, pc}
0033d614: rsbeq r7, r5, ip, lsr #9
0033d618: ldrdeq r2, r3, [r0], -r0
0033d61c: andeq r2, r0, ip, asr #14

# 0x33d620 _ZThn436_N16TouchScreenWin32D0Ev
0033d620: sub r0, r0, #0x1b4
0033d624: b #0x33d628

# 0x33d628 _ZN16TouchScreenWin32D0Ev
0033d628: push {r4, lr}
0033d62c: mov r4, r0
0033d630: bl #0x33d5d8
0033d634: mov r0, r4
0033d638: bl #0x310440
0033d63c: mov r0, r4
0033d640: pop {r4, pc}

# 0x33d644 _ZN16TouchScreenWin32D2Ev
0033d644: ldr r3, [pc, #0x34]
0033d648: ldr r1, [pc, #0x34]
0033d64c: ldr r2, [pc, #0x34]
0033d650: add r3, pc, r3
0033d654: ldr r1, [r3, r1]
0033d658: ldr r2, [r3, r2]
0033d65c: push {r4, lr}
0033d660: add r1, r1, #8
0033d664: add r2, r2, #8
0033d668: mov r4, r0
0033d66c: str r1, [r0]
0033d670: str r2, [r0, #0x1b4]
0033d674: bl #0x33be64
0033d678: mov r0, r4
0033d67c: pop {r4, pc}
0033d680: rsbeq r7, r5, r0, asr #8
0033d684: ldrdeq r2, r3, [r0], -r0
0033d688: andeq r2, r0, ip, asr #14

# 0x33d68c _ZN16TouchScreenWin32C1Ess
0033d68c: push {r4, r5, r6, lr}
0033d690: ldr r5, [pc, #0x30]
0033d694: mov r4, r0
0033d698: bl #0x33c298
0033d69c: ldr r3, [pc, #0x28]
0033d6a0: add r5, pc, r5
0033d6a4: mov r2, #0
0033d6a8: ldr r3, [r5, r3]
0033d6ac: str r2, [r4, #0x190]
0033d6b0: mov r0, r4
0033d6b4: add r2, r3, #0x68
0033d6b8: add r3, r3, #8
0033d6bc: str r3, [r4]
0033d6c0: str r2, [r4, #0x1b4]
0033d6c4: pop {r4, r5, r6, pc}

# 0x33d6d0 _ZN16TouchScreenWin32C2Ess
0033d6d0: push {r4, r5, r6, lr}
0033d6d4: ldr r5, [pc, #0x30]
0033d6d8: mov r4, r0
0033d6dc: bl #0x33c298
0033d6e0: ldr r3, [pc, #0x28]
0033d6e4: add r5, pc, r5
0033d6e8: mov r2, #0
0033d6ec: ldr r3, [r5, r3]
0033d6f0: str r2, [r4, #0x190]
0033d6f4: mov r0, r4
0033d6f8: add r2, r3, #0x68
0033d6fc: add r3, r3, #8
0033d700: str r3, [r4]
0033d704: str r2, [r4, #0x1b4]
0033d708: pop {r4, r5, r6, pc}
0033d70c: rsbeq r7, r5, ip, lsr #7
0033d710: ldrdeq r2, r3, [r0], -r0

# 0x3824a0 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKl9TouchDataENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE5eraseENS_17_Rb_tree_iteratorIS6_SA_EE
003824a0: push {r4, lr}
003824a4: mov r4, r0
003824a8: add r2, r4, #8
003824ac: ldr r0, [r1]
003824b0: add r3, r4, #0xc
003824b4: add r1, r4, #4
003824b8: bl #0x336004
003824bc: cmp r0, #0
003824c0: beq #0x3824cc
003824c4: mov r1, #0x1c
003824c8: bl #0x708f00
003824cc: ldr r3, [r4, #0x10]
003824d0: sub r3, r3, #1
003824d4: str r3, [r4, #0x10]
003824d8: pop {r4, pc}

# 0x3824dc _ZNSaINSt4priv13_Rb_tree_nodeISt4pairIKl9TouchDataEEEE8allocateEjPKv.clone.2
003824dc: str lr, [sp, #-4]!
003824e0: sub sp, sp, #0xc
003824e4: add r0, sp, #8
003824e8: mov r3, #0x1c
003824ec: str r3, [r0, #-4]!
003824f0: bl #0x708ec0
003824f4: add sp, sp, #0xc
003824f8: ldm sp!, {pc}

# 0x3824fc _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKl9TouchDataENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS6_SE_SE_
003824fc: cmp r1, r2
00382500: push {r4, r5, r6, r7, r8, lr}
00382504: mov r4, r1
00382508: mov r7, r2
0038250c: mov r5, r0
00382510: mov r8, r3
00382514: beq #0x3825e4
00382518: ldr r3, [sp, #0x1c]
0038251c: cmp r3, #0
00382520: beq #0x38258c
00382524: mov r0, r4
00382528: bl #0x3824dc
0038252c: ldr r2, [r8]
00382530: mov r3, #0
00382534: mov r6, r0
00382538: str r2, [r0, #0x10]
0038253c: ldr r2, [r8, #4]
00382540: str r2, [r0, #0x14]
00382544: ldr r2, [r8, #8]
00382548: str r3, [r0, #0xc]
0038254c: str r3, [r0, #8]
00382550: str r2, [r0, #0x18]
00382554: str r0, [r7, #0xc]
00382558: ldr r3, [r4, #0xc]
0038255c: cmp r7, r3
00382560: beq #0x3825dc
00382564: mov r0, r6
00382568: str r7, [r6, #4]
0038256c: add r1, r4, #4
00382570: bl #0x313760
00382574: ldr r3, [r4, #0x10]
00382578: mov r0, r5
0038257c: add r3, r3, #1
00382580: str r3, [r4, #0x10]
00382584: str r6, [r5]
00382588: pop {r4, r5, r6, r7, r8, pc}
0038258c: ldr r3, [sp, #0x18]
00382590: cmp r3, #0
00382594: beq #0x382624
00382598: mov r0, r4
0038259c: bl #0x3824dc
003825a0: ldr r2, [r8]
003825a4: mov r3, #0
003825a8: mov r6, r0
003825ac: str r2, [r0, #0x10]
003825b0: ldr r2, [r8, #4]
003825b4: str r2, [r0, #0x14]
003825b8: ldr r2, [r8, #8]
003825bc: str r3, [r0, #0xc]
003825c0: str r3, [r0, #8]
003825c4: str r2, [r0, #0x18]
003825c8: str r0, [r7, #8]
003825cc: ldr r3, [r4, #8]
003825d0: cmp r7, r3
003825d4: streq r0, [r4, #8]
003825d8: b #0x382564
003825dc: str r6, [r4, #0xc]
003825e0: b #0x382564
003825e4: mov r0, r1
003825e8: bl #0x3824dc
003825ec: ldr r2, [r8]
003825f0: mov r3, #0
003825f4: mov r6, r0
003825f8: str r2, [r0, #0x10]
003825fc: ldr r2, [r8, #4]
00382600: str r2, [r0, #0x14]
00382604: ldr r2, [r8, #8]
00382608: str r3, [r0, #0xc]
0038260c: str r3, [r0, #8]
00382610: str r2, [r0, #0x18]
00382614: str r0, [r4, #8]
00382618: str r0, [r4, #4]
0038261c: str r0, [r4, #0xc]
00382620: b #0x382564
00382624: ldr r2, [r8]
00382628: ldr r3, [r7, #0x10]
0038262c: cmp r2, r3
00382630: bge #0x382524
00382634: b #0x382598

# 0x382638 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKl9TouchDataENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE13insert_uniqueERKS6_
00382638: push {r4, r5, r6, lr}
0038263c: ldr ip, [r1, #4]
00382640: sub sp, sp, #0x10
00382644: mov r4, r0
00382648: cmp ip, #0
0038264c: mov r3, r2
00382650: moveq ip, r1
00382654: beq #0x3826b0
00382658: ldr r6, [r2]
0038265c: b #0x382664
00382660: mov ip, r2
00382664: ldr r0, [ip, #0x10]
00382668: mov r5, #1
0038266c: cmp r0, r6
00382670: ldrgt r2, [ip, #8]
00382674: ldrle r2, [ip, #0xc]
00382678: movle r5, #0
0038267c: cmp r2, #0
00382680: bne #0x382660
00382684: cmp r5, #0
00382688: moveq r5, ip
0038268c: bne #0x3826b0
00382690: cmp r6, r0
00382694: movle r3, #0
00382698: strle r5, [r4]
0038269c: strble r3, [r4, #4]
003826a0: bgt #0x382718
003826a4: mov r0, r4
003826a8: add sp, sp, #0x10
003826ac: pop {r4, r5, r6, pc}
003826b0: ldr r2, [r1, #8]
003826b4: cmp ip, r2
003826b8: beq #0x382798
003826bc: ldrb r2, [ip]
003826c0: cmp r2, #0
003826c4: bne #0x3826d8
003826c8: ldr r2, [ip, #4]
003826cc: ldr r2, [r2, #4]
003826d0: cmp ip, r2
003826d4: beq #0x382784
003826d8: ldr r0, [ip, #8]
003826dc: cmp r0, #0
003826e0: bne #0x3826ec
003826e4: b #0x382744
003826e8: mov r0, r2
003826ec: ldr r2, [r0, #0xc]
003826f0: cmp r2, #0
003826f4: bne #0x3826e8
003826f8: ldr r6, [r3]
003826fc: mov r5, r0
00382700: ldr r0, [r0, #0x10]
00382704: cmp r6, r0
00382708: movle r3, #0
0038270c: strle r5, [r4]
00382710: strble r3, [r4, #4]
00382714: ble #0x3826a4
00382718: mov r2, ip
0038271c: add r0, sp, #8
00382720: mov ip, #0
00382724: str ip, [sp, #4]
00382728: str ip, [sp]
0038272c: bl #0x3824fc
00382730: ldr r3, [sp, #8]
00382734: mov r2, #1
00382738: strb r2, [r4, #4]
0038273c: str r3, [r4]
00382740: b #0x3826a4
00382744: ldr r2, [ip, #4]
00382748: ldr r0, [r2, #8]
0038274c: cmp ip, r0
00382750: movne r5, r2
00382754: ldrne r6, [r3]
00382758: ldrne r0, [r2, #0x10]
0038275c: beq #0x382768
00382760: b #0x382690
00382764: mov r2, r5
00382768: ldr r5, [r2, #4]
0038276c: ldr r0, [r5, #8]
00382770: cmp r0, r2
00382774: beq #0x382764
00382778: ldr r6, [r3]
0038277c: ldr r0, [r5, #0x10]
00382780: b #0x382690
00382784: ldr r2, [ip, #0xc]
00382788: ldr r6, [r3]
0038278c: mov r5, r2
00382790: ldr r0, [r2, #0x10]
00382794: b #0x382690
00382798: mov r2, ip
0038279c: mov lr, #0
003827a0: add r0, sp, #0xc
003827a4: stm sp, {ip, lr}
003827a8: bl #0x3824fc
003827ac: ldr r3, [sp, #0xc]
003827b0: mov r2, #1
003827b4: strb r2, [r4, #4]
003827b8: str r3, [r4]
003827bc: b #0x3826a4

# 0x3827c0 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKl9TouchDataENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE13insert_uniqueENS_17_Rb_tree_iteratorIS6_SA_EERKS6_
003827c0: push {r4, r5, r6, r7, r8, sl, lr}
003827c4: ldr r4, [r2]
003827c8: ldr r2, [r1, #8]
003827cc: sub sp, sp, #0x2c
003827d0: mov r5, r1
003827d4: cmp r4, r2
003827d8: mov r7, r0
003827dc: mov r6, r3
003827e0: beq #0x382950
003827e4: cmp r4, r1
003827e8: beq #0x3829d0
003827ec: ldrb r3, [r4]
003827f0: cmp r3, #0
003827f4: beq #0x3828e4
003827f8: ldr ip, [r4, #8]
003827fc: cmp ip, #0
00382800: bne #0x38280c
00382804: b #0x382904
00382808: mov ip, r3
0038280c: ldr r3, [ip, #0xc]
00382810: cmp r3, #0
00382814: bne #0x382808
00382818: ldr r2, [r6]
0038281c: ldr r0, [r4, #0x10]
00382820: cmp r2, r0
00382824: movge r1, #0
00382828: movlt r1, #1
0038282c: cmp r1, #0
00382830: bne #0x3828a4
00382834: ldr r8, [r4, #0xc]
00382838: cmp r8, #0
0038283c: beq #0x382a38
00382840: mov ip, r8
00382844: b #0x38284c
00382848: mov ip, r3
0038284c: ldr r3, [ip, #8]
00382850: cmp r3, #0
00382854: bne #0x382848
00382858: cmp r1, #0
0038285c: bne #0x382934
00382860: cmp r2, r0
00382864: ble #0x3829f8
00382868: cmp r5, ip
0038286c: beq #0x38287c
00382870: ldr r3, [ip, #0x10]
00382874: cmp r2, r3
00382878: bge #0x382934
0038287c: cmp r8, #0
00382880: bne #0x3829b0
00382884: mov r1, r5
00382888: mov r2, r4
0038288c: mov r3, r6
00382890: mov r0, r7
00382894: str r8, [sp]
00382898: str r4, [sp, #4]
0038289c: bl #0x3824fc
003828a0: b #0x3828d8
003828a4: ldr r3, [ip, #0x10]
003828a8: cmp r2, r3
003828ac: ble #0x382834
003828b0: ldr lr, [ip, #0xc]
003828b4: cmp lr, #0
003828b8: beq #0x382a18
003828bc: mov ip, #0
003828c0: mov r1, r5
003828c4: mov r2, r4
003828c8: mov r3, r6
003828cc: mov r0, r7
003828d0: stm sp, {r4, ip}
003828d4: bl #0x3824fc
003828d8: mov r0, r7
003828dc: add sp, sp, #0x2c
003828e0: pop {r4, r5, r6, r7, r8, sl, pc}
003828e4: ldr r3, [r4, #4]
003828e8: ldr r3, [r3, #4]
003828ec: cmp r4, r3
003828f0: ldreq ip, [r4, #0xc]
003828f4: beq #0x382818
003828f8: ldr ip, [r4, #8]
003828fc: cmp ip, #0
00382900: bne #0x38280c
00382904: ldr ip, [r4, #4]
00382908: ldr r3, [ip, #8]
0038290c: cmp r4, r3
00382910: beq #0x38291c
00382914: b #0x382818
00382918: mov ip, r3
0038291c: ldr r3, [ip, #4]
00382920: ldr r2, [r3, #8]
00382924: cmp r2, ip
00382928: beq #0x382918
0038292c: mov ip, r3
00382930: b #0x382818
00382934: mov r1, r5
00382938: mov r2, r6
0038293c: add r0, sp, #8
00382940: bl #0x382638
00382944: ldr r3, [sp, #8]
00382948: str r3, [r7]
0038294c: b #0x3828d8
00382950: ldr r2, [r1, #0x10]
00382954: cmp r2, #0
00382958: beq #0x382aa8
0038295c: ldr r2, [r3]
00382960: ldr ip, [r4, #0x10]
00382964: cmp r2, ip
00382968: blt #0x382ac0
0038296c: ble #0x3829f8
00382970: ldr lr, [r4, #0xc]
00382974: cmp lr, #0
00382978: beq #0x382a70
0038297c: mov ip, lr
00382980: b #0x382988
00382984: mov ip, r3
00382988: ldr r3, [ip, #8]
0038298c: cmp r3, #0
00382990: bne #0x382984
00382994: cmp r5, ip
00382998: beq #0x382b10
0038299c: ldr r3, [ip, #0x10]
003829a0: cmp r2, r3
003829a4: bge #0x382ad4
003829a8: cmp lr, #0
003829ac: beq #0x382af0
003829b0: mov lr, #0
003829b4: mov r1, r5
003829b8: mov r2, ip
003829bc: mov r3, r6
003829c0: mov r0, r7
003829c4: stm sp, {ip, lr}
003829c8: bl #0x3824fc
003829cc: b #0x3828d8
003829d0: ldr r2, [r4, #0xc]
003829d4: ldr ip, [r3]
003829d8: ldr lr, [r2, #0x10]
003829dc: cmp lr, ip
003829e0: bge #0x382a00
003829e4: mov ip, #0
003829e8: str ip, [sp]
003829ec: str r4, [sp, #4]
003829f0: bl #0x3824fc
003829f4: b #0x3828d8
003829f8: str r4, [r7]
003829fc: b #0x3828d8
00382a00: mov r2, r3
00382a04: add r0, sp, #0x10
00382a08: bl #0x382638
00382a0c: ldr r3, [sp, #0x10]
00382a10: str r3, [r7]
00382a14: b #0x3828d8
00382a18: mov r1, r5
00382a1c: mov r2, ip
00382a20: mov r3, r6
00382a24: mov r0, r7
00382a28: str lr, [sp]
00382a2c: str ip, [sp, #4]
00382a30: bl #0x3824fc
00382a34: b #0x3828d8
00382a38: ldr r3, [r4, #4]
00382a3c: ldr ip, [r3, #0xc]
00382a40: cmp r4, ip
00382a44: movne ip, r4
00382a48: bne #0x382a60
00382a4c: mov ip, r3
00382a50: ldr r3, [r3, #4]
00382a54: ldr sl, [r3, #0xc]
00382a58: cmp ip, sl
00382a5c: beq #0x382a4c
00382a60: ldr sl, [ip, #0xc]
00382a64: cmp r3, sl
00382a68: movne ip, r3
00382a6c: b #0x382858
00382a70: ldr r3, [r4, #4]
00382a74: ldr r1, [r3, #0xc]
00382a78: cmp r4, r1
00382a7c: movne ip, r4
00382a80: bne #0x382a98
00382a84: mov ip, r3
00382a88: ldr r3, [r3, #4]
00382a8c: ldr r1, [r3, #0xc]
00382a90: cmp r1, ip
00382a94: beq #0x382a84
00382a98: ldr r1, [ip, #0xc]
00382a9c: cmp r3, r1
00382aa0: movne ip, r3
00382aa4: b #0x382994
00382aa8: mov r2, r3
00382aac: add r0, sp, #0x20
00382ab0: bl #0x382638
00382ab4: ldr r3, [sp, #0x20]
00382ab8: str r3, [r7]
00382abc: b #0x3828d8
00382ac0: mov ip, #0
00382ac4: mov r2, r4
00382ac8: stm sp, {r4, ip}
00382acc: bl #0x3824fc
00382ad0: b #0x3828d8
00382ad4: mov r1, r5
00382ad8: mov r2, r6
00382adc: add r0, sp, #0x18
00382ae0: bl #0x382638
00382ae4: ldr r3, [sp, #0x18]
00382ae8: str r3, [r7]
00382aec: b #0x3828d8
00382af0: mov r1, r5
00382af4: mov r2, r4
00382af8: mov r3, r6
00382afc: mov r0, r7
00382b00: str lr, [sp]
00382b04: str r4, [sp, #4]
00382b08: bl #0x3824fc
00382b0c: b #0x3828d8
00382b10: mov ip, #0
00382b14: mov r1, r5
00382b18: mov r2, r4
00382b1c: mov r3, r6
00382b20: mov r0, r7
00382b24: str ip, [sp]
00382b28: str r4, [sp, #4]
00382b2c: bl #0x3824fc
00382b30: b #0x3828d8

# 0x382b34 _ZNSt3mapIl9TouchDataSt4lessIlESaISt4pairIKlS0_EEEixIlEERS0_RKT_
00382b34: push {r4, lr}
00382b38: ldr ip, [r0, #4]
00382b3c: sub sp, sp, #0x18
00382b40: cmp ip, #0
00382b44: beq #0x382bd0
00382b48: ldr r4, [r1]
00382b4c: mov r2, r0
00382b50: b #0x382b58
00382b54: mov ip, r3
00382b58: ldr r3, [ip, #0x10]
00382b5c: cmp r4, r3
00382b60: ldrgt r3, [ip, #0xc]
00382b64: ldrle r3, [ip, #8]
00382b68: movgt ip, r2
00382b6c: mov r2, ip
00382b70: cmp r3, #0
00382b74: bne #0x382b54
00382b78: cmp r0, ip
00382b7c: beq #0x382b90
00382b80: ldr r2, [ip, #0x10]
00382b84: mov r3, ip
00382b88: cmp r2, r4
00382b8c: ble #0x382bc4
00382b90: mov r1, r0
00382b94: add r3, sp, #4
00382b98: str ip, [sp, #0x10]
00382b9c: add r0, sp, #0x14
00382ba0: mov ip, #0
00382ba4: add r2, sp, #0x10
00382ba8: str r4, [sp, #4]
00382bac: strh ip, [sp, #0xe]
00382bb0: strh ip, [sp, #0xc]
00382bb4: strh ip, [sp, #0xa]
00382bb8: strh ip, [sp, #8]
00382bbc: bl #0x3827c0
00382bc0: ldr r3, [sp, #0x14]
00382bc4: add r0, r3, #0x14
00382bc8: add sp, sp, #0x18
00382bcc: pop {r4, pc}
00382bd0: ldr r4, [r1]
00382bd4: mov ip, r0
00382bd8: b #0x382b78

# 0x383084 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKl9TouchDataENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00383084: push {r4, r5, r6, lr}
00383088: subs r4, r1, #0
0038308c: mov r6, r0
00383090: beq #0x3830b8
00383094: ldr r1, [r4, #0xc]
00383098: mov r0, r6
0038309c: bl #0x383084
003830a0: ldr r5, [r4, #8]
003830a4: mov r0, r4
003830a8: mov r1, #0x1c
003830ac: bl #0x708f00
003830b0: subs r4, r5, #0
003830b4: bne #0x383094
003830b8: pop {r4, r5, r6, pc}

# 0x38ab68 _ZNK10GameObject10IsTouchingERK7Point3DIfE
0038ab68: push {r4, r5, r6, lr}
0038ab6c: ldr r5, [r1]
0038ab70: mov r6, r1
0038ab74: mov r4, r0
0038ab78: mov r1, r5
0038ab7c: ldr r0, [r0, #0x12c]
0038ab80: bl #0x30e9ac
0038ab84: cmp r0, #0
0038ab88: beq #0x38ac04
0038ab8c: mov r0, r5
0038ab90: ldr r1, [r4, #0x138]
0038ab94: bl #0x30e9ac
0038ab98: cmp r0, #0
0038ab9c: beq #0x38ac04
0038aba0: ldr r5, [r6, #4]
0038aba4: ldr r0, [r4, #0x130]
0038aba8: mov r1, r5
0038abac: bl #0x30e9ac
0038abb0: cmp r0, #0
0038abb4: beq #0x38ac04
0038abb8: mov r0, r5
0038abbc: ldr r1, [r4, #0x13c]
0038abc0: bl #0x30e9ac
0038abc4: cmp r0, #0
0038abc8: beq #0x38ac04
0038abcc: ldr r5, [r6, #8]
0038abd0: ldr r0, [r4, #0x134]
0038abd4: mov r1, r5
0038abd8: bl #0x30e9ac
0038abdc: cmp r0, #0
0038abe0: beq #0x38ac04
0038abe4: mov r0, r5
0038abe8: ldr r1, [r4, #0x140]
0038abec: bl #0x30e9ac
0038abf0: cmp r0, #0
0038abf4: mov r0, #0
0038abf8: movne r0, #1
0038abfc: uxtb r0, r0
0038ac00: pop {r4, r5, r6, pc}
0038ac04: mov r0, #0
0038ac08: pop {r4, r5, r6, pc}

# 0x38ac0c _ZNK10GameObject8IsNearbyERK7Point3DIfEf
0038ac0c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038ac10: mov r3, r0
0038ac14: ldr r4, [r0, #0x12c]
0038ac18: mov r5, r2
0038ac1c: mov r0, r2
0038ac20: ldr r2, [r3, #0x140]
0038ac24: sub sp, sp, #0xc
0038ac28: mov r7, r1
0038ac2c: mov r1, #0xc2000000
0038ac30: str r2, [sp, #4]
0038ac34: add r1, r1, #0xc80000
0038ac38: ldr sl, [r3, #0x130]
0038ac3c: ldr fp, [r3, #0x134]
0038ac40: ldr r8, [r3, #0x138]
0038ac44: ldr sb, [r3, #0x13c]
0038ac48: bl #0x30ed6c
0038ac4c: mov r6, r0
0038ac50: mov r1, r6
0038ac54: mov r0, r4
0038ac58: bl #0x30eba4
0038ac5c: ldr r4, [r7]
0038ac60: mov r1, r4
0038ac64: bl #0x30e9ac
0038ac68: cmp r0, #0
0038ac6c: beq #0x38aca0
0038ac70: mov r1, #0x42000000
0038ac74: mov r0, r5
0038ac78: add r1, r1, #0xc80000
0038ac7c: bl #0x30ed6c
0038ac80: mov r5, r0
0038ac84: mov r1, r5
0038ac88: mov r0, r8
0038ac8c: bl #0x30eba4
0038ac90: mov r1, r4
0038ac94: bl #0x30e4b4
0038ac98: cmp r0, #0
0038ac9c: bne #0x38acac
0038aca0: mov r0, #0
0038aca4: add sp, sp, #0xc
0038aca8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038acac: ldr r4, [r7, #4]
0038acb0: mov r1, r6
0038acb4: mov r0, sl
0038acb8: bl #0x30eba4
0038acbc: mov r1, r4
0038acc0: bl #0x30e9ac
0038acc4: cmp r0, #0
0038acc8: beq #0x38aca0
0038accc: mov r1, r5
0038acd0: mov r0, sb
0038acd4: bl #0x30eba4
0038acd8: mov r1, r4
0038acdc: bl #0x30e4b4
0038ace0: cmp r0, #0
0038ace4: beq #0x38aca0
0038ace8: ldr r4, [r7, #8]
0038acec: mov r1, r6
0038acf0: mov r0, fp
0038acf4: bl #0x30eba4
0038acf8: mov r1, r4
0038acfc: bl #0x30e9ac
0038ad00: cmp r0, #0
0038ad04: beq #0x38aca0
0038ad08: mov r1, r5
0038ad0c: ldr r0, [sp, #4]
0038ad10: bl #0x30eba4
0038ad14: mov r1, r4
0038ad18: bl #0x30e4b4
0038ad1c: cmp r0, #0
0038ad20: mov r0, #0
0038ad24: movne r0, #1
0038ad28: uxtb r0, r0
0038ad2c: b #0x38aca4

# 0x38b518 _ZNK10GameObject10IsTouchingEPKS_
0038b518: push {r4, r5, lr}
0038b51c: ldr r3, [pc, #0xf4]
0038b520: subs r4, r1, #0
0038b524: sub sp, sp, #0xc
0038b528: mov r5, r0
0038b52c: add r3, pc, r3
0038b530: beq #0x38b5c4
0038b534: ldr r0, [r5, #0x12c]
0038b538: ldr r1, [r4, #0x138]
0038b53c: bl #0x30e9ac
0038b540: cmp r0, #0
0038b544: beq #0x38b55c
0038b548: ldr r0, [r5, #0x138]
0038b54c: ldr r1, [r4, #0x12c]
0038b550: bl #0x30e4b4
0038b554: cmp r0, #0
0038b558: bne #0x38b568
0038b55c: mov r0, #0
0038b560: add sp, sp, #0xc
0038b564: pop {r4, r5, pc}
0038b568: ldr r0, [r5, #0x130]
0038b56c: ldr r1, [r4, #0x13c]
0038b570: bl #0x30e9ac
0038b574: cmp r0, #0
0038b578: beq #0x38b55c
0038b57c: ldr r0, [r5, #0x13c]
0038b580: ldr r1, [r4, #0x130]
0038b584: bl #0x30e4b4
0038b588: cmp r0, #0
0038b58c: beq #0x38b55c
0038b590: ldr r0, [r5, #0x134]
0038b594: ldr r1, [r4, #0x140]
0038b598: bl #0x30e9ac
0038b59c: cmp r0, #0
0038b5a0: beq #0x38b55c
0038b5a4: ldr r0, [r5, #0x140]
0038b5a8: ldr r1, [r4, #0x134]
0038b5ac: bl #0x30e4b4
0038b5b0: cmp r0, #0
0038b5b4: mov r0, #0
0038b5b8: movne r0, #1
0038b5bc: uxtb r0, r0
0038b5c0: b #0x38b560
0038b5c4: ldr r2, [pc, #0x50]
0038b5c8: ldr r2, [r3, r2]
0038b5cc: ldr r2, [r2]
0038b5d0: cmp r2, #2
0038b5d4: streq r4, [r4]
0038b5d8: beq #0x38b534
0038b5dc: cmp r2, #1
0038b5e0: bne #0x38b534
0038b5e4: ldr r0, [pc, #0x34]
0038b5e8: ldr r1, [pc, #0x34]
0038b5ec: ldr r2, [pc, #0x34]
0038b5f0: ldr r0, [r3, r0]
0038b5f4: ldr r3, [pc, #0x30]
0038b5f8: movw ip, #0x17b
0038b5fc: add r1, pc, r1
0038b600: add r2, pc, r2
0038b604: add r3, pc, r3
0038b608: add r0, r0, #0xa8
0038b60c: str ip, [sp]
0038b610: bl #0x30e004
0038b614: b #0x38b534
0038b618: rsbeq sb, r0, r4, ror #10
0038b61c: andeq r3, r0, r0, asr #19
0038b620: andeq r1, r0, r0, asr #19
0038b624: ldrsbeq r2, [r3], #-0xdc

# 0x38b630 _ZNK10GameObject14IsHostTouchingEv
0038b630: push {r4, r5, r6, r7, r8, lr}
0038b634: mov r7, r0
0038b638: bl #0x7fd794
0038b63c: ldrb r3, [r0, #5]
0038b640: ldr r5, [pc, #0x84]
0038b644: cmp r3, #0
0038b648: add r5, pc, r5
0038b64c: beq #0x38b6a8
0038b650: ldr r6, [pc, #0x78]
0038b654: ldr r3, [r5, r6]
0038b658: ldr r0, [r3, #0x40]
0038b65c: ldr r3, [r0, #0x6c4]
0038b660: cmp r3, #0
0038b664: ble #0x38b6c4
0038b668: mov r4, #0
0038b66c: mov r1, r4
0038b670: mov r2, #1
0038b674: bl #0x36e744
0038b678: mov r8, r0
0038b67c: bl #0x80f23c
0038b680: cmp r0, #0
0038b684: add r4, r4, #1
0038b688: beq #0x38b6b0
0038b68c: ldr r3, [r8, #0x660]
0038b690: mov r0, r7
0038b694: subs r1, r3, #0
0038b698: beq #0x38b6b0
0038b69c: bl #0x38b518
0038b6a0: cmp r0, #0
0038b6a4: beq #0x38b6b0
0038b6a8: mov r0, #1
0038b6ac: pop {r4, r5, r6, r7, r8, pc}
0038b6b0: ldr r3, [r5, r6]
0038b6b4: ldr r0, [r3, #0x40]
0038b6b8: ldr r3, [r0, #0x6c4]
0038b6bc: cmp r4, r3
0038b6c0: blt #0x38b66c
0038b6c4: mov r0, #0
0038b6c8: pop {r4, r5, r6, r7, r8, pc}
0038b6cc: rsbeq sb, r0, r8, asr #8
0038b6d0: strdeq r3, r4, [r0], -r4

# 0x38b6d4 _ZNK10GameObject20GetNumPlayerTouchingEv
0038b6d4: push {r4, r5, r6, r7, r8, lr}
0038b6d8: ldr r5, [pc, #0x70]
0038b6dc: ldr r6, [pc, #0x70]
0038b6e0: mov r7, r0
0038b6e4: add r5, pc, r5
0038b6e8: ldr r3, [r5, r6]
0038b6ec: ldr r0, [r3, #0x40]
0038b6f0: ldr r3, [r0, #0x6c4]
0038b6f4: cmp r3, #0
0038b6f8: movle r8, #0
0038b6fc: ble #0x38b748
0038b700: mov r4, #0
0038b704: mov r8, r4
0038b708: mov r1, r4
0038b70c: mov r2, #1
0038b710: bl #0x36e744
0038b714: ldr r3, [r0, #0x660]
0038b718: add r4, r4, #1
0038b71c: mov r0, r7
0038b720: subs r1, r3, #0
0038b724: beq #0x38b734
0038b728: bl #0x38b518
0038b72c: cmp r0, #0
0038b730: addne r8, r8, #1
0038b734: ldr r3, [r5, r6]
0038b738: ldr r0, [r3, #0x40]
0038b73c: ldr r3, [r0, #0x6c4]
0038b740: cmp r4, r3
0038b744: blt #0x38b708
0038b748: mov r0, r8
0038b74c: pop {r4, r5, r6, r7, r8, pc}
0038b750: rsbeq sb, r0, ip, lsr #7
0038b754: strdeq r3, r4, [r0], -r4

# 0x390548 _ZN10GameObject21_SetTargetListSortingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390548: push {r4, r5, r6, r7, r8, lr}
0039054c: ldr r3, [r0, #4]
00390550: mov r5, r2
00390554: ldr r4, [pc, #0xb8]
00390558: ldm r3, {r1, r2}
0039055c: add r4, pc, r4
00390560: rsb r3, r1, r2
00390564: asr r3, r3, #4
00390568: add r2, r3, r3, lsl #3
0039056c: add r2, r2, r2, lsl #6
00390570: add r2, r3, r2, lsl #3
00390574: add r2, r2, r2, lsl #15
00390578: add r3, r3, r2, lsl #3
0039057c: cmp r3, #0
00390580: bne #0x390588
00390584: pop {r4, r5, r6, r7, r8, pc}
00390588: ldr r3, [r1, #4]
0039058c: cmp r3, #3
00390590: bne #0x390584
00390594: mov r1, #0
00390598: bl #0x37baf8
0039059c: bl #0x31bbf0
003905a0: bl #0x30e4cc
003905a4: ldr r2, [r5, #0x314]
003905a8: ldr r3, [r5, #0x304]
003905ac: mov r7, r0
003905b0: cmp r2, r3
003905b4: beq #0x3905d4
003905b8: add r6, r5, #0x304
003905bc: mov r0, r6
003905c0: bl #0x38fb18
003905c4: ldr r2, [r5, #0x314]
003905c8: ldr r3, [r5, #0x304]
003905cc: cmp r2, r3
003905d0: bne #0x3905bc
003905d4: cmp r7, #1
003905d8: beq #0x390604
003905dc: cmp r7, #2
003905e0: beq #0x3905f4
003905e4: ldr r3, [pc, #0x2c]
003905e8: ldr r3, [r4, r3]
003905ec: str r3, [r5, #0x32c]
003905f0: b #0x390584
003905f4: ldr r3, [pc, #0x20]
003905f8: ldr r3, [r4, r3]
003905fc: str r3, [r5, #0x32c]
00390600: pop {r4, r5, r6, r7, r8, pc}
00390604: ldr r3, [pc, #0x14]
00390608: ldr r3, [r4, r3]
0039060c: str r3, [r5, #0x32c]
00390610: pop {r4, r5, r6, r7, r8, pc}
00390614: rsbeq r4, r0, r4, lsr r5
00390618: andeq r2, r0, r4, ror ip
0039061c: ldrdeq r1, r2, [r0], -r4
00390620: andeq r4, r0, r4, asr #21

# 0x390624 _ZN10GameObject26_SetTargetListObjectFilterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390624: str lr, [sp, #-4]!
00390628: ldr r3, [r0, #4]
0039062c: sub sp, sp, #0xc
00390630: ldr r1, [r3, #4]
00390634: ldr ip, [r3]
00390638: rsb r3, ip, r1
0039063c: asr r3, r3, #4
00390640: add r1, r3, r3, lsl #3
00390644: add r1, r1, r1, lsl #6
00390648: add r1, r3, r1, lsl #3
0039064c: add r1, r1, r1, lsl #15
00390650: add r3, r3, r1, lsl #3
00390654: cmp r3, #0
00390658: bne #0x390664
0039065c: add sp, sp, #0xc
00390660: ldm sp!, {pc}
00390664: ldr r3, [ip, #4]
00390668: cmp r3, #3
0039066c: bne #0x39065c
00390670: mov r1, #0
00390674: str r2, [sp, #4]
00390678: bl #0x37baf8
0039067c: bl #0x31bbf0
00390680: bl #0x30e4cc
00390684: ldr r2, [sp, #4]
00390688: str r0, [r2, #0x33c]
0039068c: b #0x39065c

# 0x390690 _ZN10GameObject29_SetTargetListCharacterFilterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390690: str lr, [sp, #-4]!
00390694: ldr r3, [r0, #4]
00390698: sub sp, sp, #0xc
0039069c: ldr r1, [r3, #4]
003906a0: ldr ip, [r3]
003906a4: rsb r3, ip, r1
003906a8: asr r3, r3, #4
003906ac: add r1, r3, r3, lsl #3
003906b0: add r1, r1, r1, lsl #6
003906b4: add r1, r3, r1, lsl #3
003906b8: add r1, r1, r1, lsl #15
003906bc: add r3, r3, r1, lsl #3
003906c0: cmp r3, #0
003906c4: bne #0x3906d0
003906c8: add sp, sp, #0xc
003906cc: ldm sp!, {pc}
003906d0: ldr r3, [ip, #4]
003906d4: cmp r3, #3
003906d8: bne #0x3906c8
003906dc: mov r1, #0
003906e0: str r2, [sp, #4]
003906e4: bl #0x37baf8
003906e8: bl #0x31bbf0
003906ec: bl #0x30e4cc
003906f0: ldr r2, [sp, #4]
003906f4: str r0, [r2, #0x338]
003906f8: b #0x3906c8

# 0x3a9340 _ZN9CharacterC2EN10ObjectBase6GO_IDSE
003a9340: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a9344: add ip, r0, #0x374
003a9348: sub sp, sp, #0x3c
003a934c: mov r4, r0
003a9350: str ip, [sp, #0xc]
003a9354: bl #0x38c398
003a9358: ldr ip, [sp, #0xc]
003a935c: add r5, r4, #0x4f0
003a9360: add r5, r5, #0xc
003a9364: mov r0, ip
003a9368: bl #0x404db8
003a936c: add r0, r4, #0x3b4
003a9370: str r0, [sp, #0x20]
003a9374: add r0, r4, #0x37c
003a9378: bl #0x3ff330
003a937c: add r2, r4, #0x490
003a9380: add r1, r4, #0x3c8
003a9384: add r2, r2, #0xc
003a9388: ldr r0, [sp, #0x20]
003a938c: str r1, [sp, #0x1c]
003a9390: str r2, [sp, #0x14]
003a9394: bl #0x3dbb0c
003a9398: ldr r0, [sp, #0x1c]
003a939c: bl #0x3cebf0
003a93a0: ldr r0, [sp, #0x14]
003a93a4: bl #0x3c8ff4
003a93a8: add r3, r4, #0x560
003a93ac: mov r0, r5
003a93b0: str r3, [sp, #0x18]
003a93b4: ldr sb, [pc, #0x50c]
003a93b8: bl #0x3c1b58
003a93bc: ldr r0, [sp, #0x18]
003a93c0: bl #0x3df084
003a93c4: ldr lr, [pc, #0x500]
003a93c8: add sb, pc, sb
003a93cc: mov r8, #0
003a93d0: ldr lr, [sb, lr]
003a93d4: mov fp, #1
003a93d8: mvn r6, #0
003a93dc: add sl, lr, #0x324
003a93e0: str sl, [sp, #0x34]
003a93e4: add sl, lr, #0x180
003a93e8: str sl, [sp, #0x10]
003a93ec: add sl, lr, #0x1f4
003a93f0: str sl, [sp, #0x24]
003a93f4: add sl, lr, #0x220
003a93f8: str sl, [sp, #0x28]
003a93fc: add sl, lr, #0x230
003a9400: str sl, [sp, #0x2c]
003a9404: add r0, lr, #8
003a9408: add r1, lr, #0x15c
003a940c: add r2, lr, #0x168
003a9410: add sl, lr, #0x304
003a9414: str sl, [sp, #0x30]
003a9418: stm r4, {r0, r1}
003a941c: str r2, [r4, #0x24]
003a9420: ldr r0, [sp, #0x10]
003a9424: add lr, lr, #0x314
003a9428: add r7, r4, #0x1380
003a942c: str r0, [r4, #0x374]
003a9430: ldr r1, [sp, #0x24]
003a9434: add r3, r7, #0x18
003a9438: movw sl, #0x13a8
003a943c: str r1, [r4, #0x37c]
003a9440: ldr r2, [sp, #0x28]
003a9444: add r7, r7, #0x30
003a9448: str r2, [r4, #0x3b4]
003a944c: ldr r0, [sp, #0x2c]
003a9450: str r0, [r4, #0x3c8]
003a9454: ldr r1, [sp, #0x30]
003a9458: str lr, [r4, #0x4fc]
003a945c: mov r0, r3
003a9460: str r1, [r4, #0x49c]
003a9464: ldr r2, [sp, #0x34]
003a9468: mov r1, #0x10
003a946c: str r2, [r4, #0x560]
003a9470: movw r2, #0x1394
003a9474: strb r8, [r4, r2]
003a9478: movw r2, #0x1395
003a947c: strb r8, [r4, r2]
003a9480: movw r2, #0x1396
003a9484: strb fp, [r4, r2]
003a9488: movw r2, #0x1397
003a948c: strb r6, [r4, r2]
003a9490: movw r2, #0x13ac
003a9494: str r3, [r4, r2]
003a9498: str r3, [r4, sl]
003a949c: bl #0x31167c
003a94a0: ldr r3, [r4, sl]
003a94a4: mov sl, #0x13c0
003a94a8: mov r0, r7
003a94ac: strb r8, [r3]
003a94b0: movw r3, #0x13c4
003a94b4: str r7, [r4, r3]
003a94b8: mov r1, #0x10
003a94bc: str r7, [r4, sl]
003a94c0: bl #0x31167c
003a94c4: ldr r2, [r4, sl]
003a94c8: add r7, r4, sl
003a94cc: add r3, r7, #0xc
003a94d0: strb r8, [r2]
003a94d4: movw r2, #0x13c8
003a94d8: strh r6, [r4, r2]
003a94dc: movw r2, #0x13ca
003a94e0: strh r6, [r4, r2]
003a94e4: movw sl, #0x13dc
003a94e8: movw r2, #0x13e0
003a94ec: str r3, [r4, r2]
003a94f0: mov r0, r3
003a94f4: str r3, [r4, sl]
003a94f8: mov r1, #0x10
003a94fc: bl #0x31167c
003a9500: ldr r3, [r4, sl]
003a9504: add r7, r7, #0x28
003a9508: movw sl, #0x13f8
003a950c: strb r8, [r3]
003a9510: movw r3, #0x13e4
003a9514: strb fp, [r4, r3]
003a9518: movw r3, #0x13fc
003a951c: str r7, [r4, r3]
003a9520: mov r0, r7
003a9524: str r7, [r4, sl]
003a9528: mov r1, #0x10
003a952c: bl #0x31167c
003a9530: ldr r3, [r4, sl]
003a9534: add r7, r4, #0x1400
003a9538: movw sl, #0x1410
003a953c: strb r8, [r3]
003a9540: movw r3, #0x1414
003a9544: str r7, [r4, r3]
003a9548: mov r0, r7
003a954c: str r7, [r4, sl]
003a9550: mov r1, #0x10
003a9554: bl #0x31167c
003a9558: ldr r3, [r4, sl]
003a955c: add r7, r7, #0x18
003a9560: movw sl, #0x1428
003a9564: strb r8, [r3]
003a9568: movw r3, #0x142c
003a956c: str r7, [r4, r3]
003a9570: mov r0, r7
003a9574: str r7, [r4, sl]
003a9578: mov r1, #0x10
003a957c: bl #0x31167c
003a9580: ldr r2, [r4, sl]
003a9584: mov r3, #0
003a9588: mov r1, #0xbf000000
003a958c: strb r8, [r2]
003a9590: movw r2, #0x14a8
003a9594: strb r6, [r4, r2]
003a9598: movw r2, #0x1430
003a959c: strb fp, [r4, r2]
003a95a0: movw r2, #0x1434
003a95a4: str r8, [r4, r2]
003a95a8: movw r2, #0x1438
003a95ac: str r8, [r4, r2]
003a95b0: movw r2, #0x1448
003a95b4: strb fp, [r4, r2]
003a95b8: movw r2, #0x1449
003a95bc: strb r8, [r4, r2]
003a95c0: movw r2, #0x144c
003a95c4: str r8, [r4, r2]
003a95c8: movw r2, #0x1450
003a95cc: str r3, [r4, r2]
003a95d0: movw r2, #0x1454
003a95d4: str r3, [r4, r2]
003a95d8: movw r2, #0x1458
003a95dc: str r3, [r4, r2]
003a95e0: movw r2, #0x145c
003a95e4: str r3, [r4, r2]
003a95e8: movw r2, #0x1460
003a95ec: str r3, [r4, r2]
003a95f0: movw r2, #0x1464
003a95f4: str r3, [r4, r2]
003a95f8: movw r2, #0x1468
003a95fc: str r3, [r4, r2]
003a9600: movw r2, #0x146c
003a9604: str r3, [r4, r2]
003a9608: movw r2, #0x1470
003a960c: str r3, [r4, r2]
003a9610: movw r2, #0x1474
003a9614: str r3, [r4, r2]
003a9618: movw r2, #0x1478
003a961c: str r3, [r4, r2]
003a9620: movw r2, #0x147c
003a9624: str r3, [r4, r2]
003a9628: mov r2, #0x1480
003a962c: strb r8, [r4, r2]
003a9630: movw r2, #0x1481
003a9634: strb r8, [r4, r2]
003a9638: movw r2, #0x1484
003a963c: str r8, [r4, r2]
003a9640: movw r2, #0x1488
003a9644: str r8, [r4, r2]
003a9648: movw r2, #0x148c
003a964c: str r8, [r4, r2]
003a9650: movw r2, #0x1490
003a9654: str r8, [r4, r2]
003a9658: movw r2, #0x1494
003a965c: str r8, [r4, r2]
003a9660: movw r2, #0x1498
003a9664: str r6, [r4, r2]
003a9668: movw r2, #0x149c
003a966c: str r8, [r4, r2]
003a9670: movw r2, #0x14a0
003a9674: str r8, [r4, r2]
003a9678: movw r2, #0x14a4
003a967c: str r8, [r4, r2]
003a9680: movw r2, #0x14aa
003a9684: strh r8, [r4, r2]
003a9688: movw r2, #0x14ac
003a968c: strb r8, [r4, r2]
003a9690: movw r2, #0x14d8
003a9694: str r3, [r4, r2]
003a9698: add r1, r1, #0x800000
003a969c: movw r2, #0x14fc
003a96a0: str r1, [r4, r2]
003a96a4: movw r2, #0x1504
003a96a8: str r6, [r4, r2]
003a96ac: movw r2, #0x14ad
003a96b0: strb r8, [r4, r2]
003a96b4: movw r2, #0x14b0
003a96b8: str r3, [r4, r2]
003a96bc: movw r2, #0x14b4
003a96c0: str r3, [r4, r2]
003a96c4: movw r2, #0x14b8
003a96c8: str r3, [r4, r2]
003a96cc: movw r2, #0x14bc
003a96d0: str r3, [r4, r2]
003a96d4: mov r2, #0x14c0
003a96d8: str r3, [r4, r2]
003a96dc: movw r2, #0x14c4
003a96e0: str r3, [r4, r2]
003a96e4: movw r3, #0x14c8
003a96e8: strb r8, [r4, r3]
003a96ec: movw r3, #0x14ca
003a96f0: strh r6, [r4, r3]
003a96f4: movw r3, #0x14cc
003a96f8: str r8, [r4, r3]
003a96fc: movw r3, #0x14d0
003a9700: strh r8, [r4, r3]
003a9704: movw r3, #0x14d4
003a9708: str r8, [r4, r3]
003a970c: movw r3, #0x14dc
003a9710: strb r8, [r4, r3]
003a9714: movw r3, #0x14e4
003a9718: strb r8, [r4, r3]
003a971c: movw r3, #0x14e5
003a9720: strb r8, [r4, r3]
003a9724: movw r3, #0x14e8
003a9728: str r8, [r4, r3]
003a972c: movw r3, #0x14ec
003a9730: str r8, [r4, r3]
003a9734: add r7, r4, #0x1500
003a9738: movw r3, #0x14f0
003a973c: add r0, r4, #0x1a40
003a9740: strb r8, [r4, r3]
003a9744: add r0, r0, #8
003a9748: mov r3, #0x1500
003a974c: add r7, r7, #8
003a9750: str r6, [r4, r3]
003a9754: str r0, [sp, #0x10]
003a9758: mov r0, r7
003a975c: bl #0x3a6a24
003a9760: ldr r0, [sp, #0x10]
003a9764: bl #0x3a6a24
003a9768: add r0, r4, #0x304
003a976c: mov r1, r4
003a9770: strb fp, [r4, #0x28]
003a9774: bl #0x4a191c
003a9778: strb fp, [r4, #0x1c4]
003a977c: strb fp, [r4, #0x85]
003a9780: mov r0, #0x10
003a9784: mov r1, r8
003a9788: bl #0x310570
003a978c: ldr r3, [pc, #0x13c]
003a9790: ldr ip, [sp, #0xc]
003a9794: mov r6, r0
003a9798: ldr r3, [sb, r3]
003a979c: cmp ip, r8
003a97a0: strb r8, [r6, #0xa]
003a97a4: add r3, r3, #8
003a97a8: str r8, [r0, #0xc]
003a97ac: stm r0, {r3, ip}
003a97b0: strb r8, [r6, #8]
003a97b4: strb r8, [r6, #9]
003a97b8: beq #0x3a986c
003a97bc: mov r0, ip
003a97c0: mov r1, r6
003a97c4: bl #0x404e10
003a97c8: ldr r3, [r4, #0x378]
003a97cc: ldr r0, [sp, #0x20]
003a97d0: mov r1, r4
003a97d4: str r4, [r3, #0xc]
003a97d8: bl #0x3db480
003a97dc: ldr r0, [sp, #0x1c]
003a97e0: mov r1, r4
003a97e4: bl #0x3cb7c0
003a97e8: ldr r0, [sp, #0x14]
003a97ec: mov r1, r4
003a97f0: bl #0x3c9890
003a97f4: mov r0, r5
003a97f8: mov r1, r4
003a97fc: bl #0x3c1600
003a9800: ldr r0, [sp, #0x18]
003a9804: mov r1, r4
003a9808: bl #0x3dec0c
003a980c: mov r6, #0
003a9810: str r4, [r4, #0x380]
003a9814: mov r1, r6
003a9818: mov r0, r5
003a981c: add r6, r6, #1
003a9820: bl #0x3c7318
003a9824: cmp r6, #0x14
003a9828: bne #0x3a9814
003a982c: mov r1, #0
003a9830: movw r2, #0x14e0
003a9834: str r1, [r4, r2]
003a9838: mvn r3, #0
003a983c: movw r2, #0x14f4
003a9840: str r3, [r4, r2]
003a9844: str r7, [r4, #0x100]
003a9848: ldr sl, [sp, #0x10]
003a984c: movw r2, #0x14f8
003a9850: mov r0, r4
003a9854: str sl, [r4, #0x104]
003a9858: str r3, [r4, r2]
003a985c: mov r3, #1
003a9860: strb r3, [r4, #0xf8]
003a9864: add sp, sp, #0x3c
003a9868: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a986c: ldr r3, [pc, #0x60]
003a9870: ldr r3, [sb, r3]
003a9874: ldr r3, [r3]
003a9878: cmp r3, #2
003a987c: streq ip, [r4, #0x374]
003a9880: beq #0x3a97bc
003a9884: cmp r3, #1
003a9888: bne #0x3a97bc
003a988c: ldr r0, [pc, #0x44]
003a9890: ldr r1, [pc, #0x44]
003a9894: ldr r2, [pc, #0x44]
003a9898: ldr r0, [sb, r0]
003a989c: ldr r3, [pc, #0x40]
003a98a0: mov lr, #0x44
003a98a4: add r1, pc, r1
003a98a8: add r0, r0, #0xa8
003a98ac: add r2, pc, r2
003a98b0: add r3, pc, r3
003a98b4: str ip, [sp, #0xc]
003a98b8: str lr, [sp]
003a98bc: bl #0x30e004
003a98c0: ldr ip, [sp, #0xc]
003a98c4: b #0x3a97bc
003a98c8: subseq fp, lr, r8, asr #13
003a98cc: andeq r2, r0, r8, lsl #28
003a98d0: andeq r2, r0, r4, lsr #21
003a98d4: andeq r3, r0, r0, asr #19
003a98d8: andeq r1, r0, r0, asr #19
003a98dc: subseq r4, r1, r4, lsr fp
003a98e0: subseq sb, r1, r4, lsl ip
003a98e4: subseq sb, r1, r0, lsr #24

# 0x3abb9c _ZN9Character22UpdateObjectOfInterestEv
003abb9c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003abba0: movw r5, #0x14a4
003abba4: ldr r3, [r0, r5]
003abba8: ldr r6, [pc, #0x2cc]
003abbac: mov r1, #0
003abbb0: cmp r3, #0
003abbb4: movw r2, #0x14ad
003abbb8: strb r1, [r0, r2]
003abbbc: sub sp, sp, #0x74
003abbc0: mov r4, r0
003abbc4: add r6, pc, r6
003abbc8: beq #0x3abbf4
003abbcc: mov r0, r3
003abbd0: mov r1, r4
003abbd4: ldr r3, [r3]
003abbd8: mov lr, pc
003abbdc: ldr pc, [r3, #0x8c]
003abbe0: cmp r0, #0
003abbe4: bne #0x3abc28
003abbe8: mov r2, #0
003abbec: movw r3, #0x14a4
003abbf0: str r2, [r4, r3]
003abbf4: ldr r7, [pc, #0x284]
003abbf8: movw sl, #0x14aa
003abbfc: ldrh r5, [r4, sl]
003abc00: ldr r0, [r6, r7]
003abc04: bl #0x31f66c
003abc08: rsb r5, r0, r5
003abc0c: uxth r5, r5
003abc10: sxth r3, r5
003abc14: cmp r3, #0
003abc18: strh r5, [r4, sl]
003abc1c: ble #0x3abc48
003abc20: add sp, sp, #0x74
003abc24: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003abc28: ldr r3, [r4, r5]
003abc2c: ldrb r3, [r3, #0x81]
003abc30: cmp r3, #0
003abc34: beq #0x3abbf4
003abc38: mov r2, #0
003abc3c: movw r3, #0x14a4
003abc40: str r2, [r4, r3]
003abc44: b #0x3abbf4
003abc48: mvn r0, #0
003abc4c: movw r3, #0x14a8
003abc50: strb r0, [r4, r3]
003abc54: mov r3, #0x1f4
003abc58: strh r3, [r4, sl]
003abc5c: mov r2, #0
003abc60: movw r3, #0x14ac
003abc64: strb r2, [r4, r3]
003abc68: mov r8, #1
003abc6c: movw r1, #0x14a4
003abc70: add r5, sp, #8
003abc74: ldr fp, [r4, r1]
003abc78: mov r3, r2
003abc7c: str r2, [r4, r1]
003abc80: mov r0, r5
003abc84: mov r2, r8
003abc88: mov r1, r4
003abc8c: str r8, [sp]
003abc90: bl #0x4a2730
003abc94: mov r3, #0x59
003abc98: str r3, [sp, #0x3c]
003abc9c: ldr r2, [sp, #0x18]
003abca0: ldr r3, [sp, #8]
003abca4: str r8, [sp, #0x40]
003abca8: cmp r2, r3
003abcac: beq #0x3abcc8
003abcb0: mov r0, r5
003abcb4: bl #0x38fb18
003abcb8: ldr r3, [sp, #8]
003abcbc: ldr r2, [sp, #0x18]
003abcc0: cmp r2, r3
003abcc4: bne #0x3abcb0
003abcc8: ldr r3, [pc, #0x1b4]
003abccc: mov r0, r4
003abcd0: ldr r3, [r6, r3]
003abcd4: str r3, [sp, #0x30]
003abcd8: bl #0x3935dc
003abcdc: ldr r7, [r6, r7]
003abce0: ldr r1, [pc, #0x1a0]
003abce4: ldr r2, [pc, #0x1a0]
003abce8: mov r8, r0
003abcec: add r1, pc, r1
003abcf0: add r2, pc, r2
003abcf4: ldr r0, [r7, #0x2c]
003abcf8: bl #0x4c4bdc
003abcfc: bl #0x30e964
003abd00: ldr r3, [pc, #0x188]
003abd04: ldr lr, [r7, #0x38]
003abd08: mov r2, r0
003abd0c: ldr r3, [r6, r3]
003abd10: add ip, lr, #0x80
003abd14: str ip, [sp, #0x60]
003abd18: add r3, r3, #8
003abd1c: str r3, [sp, #0x5c]
003abd20: ldr lr, [lr, #0x80]
003abd24: movw r3, #0xfdb
003abd28: str ip, [sp, #0x68]
003abd2c: mov ip, #0
003abd30: movt r3, #0x40c9
003abd34: str ip, [sp, #0x6c]
003abd38: mov r1, r8
003abd3c: add ip, sp, #0x5c
003abd40: mov r0, r5
003abd44: str lr, [sp, #0x64]
003abd48: str ip, [sp]
003abd4c: bl #0x4a33c8
003abd50: ldr r3, [pc, #0x13c]
003abd54: ldr r7, [sp, #8]
003abd58: ldr r2, [sp, #0x18]
003abd5c: ldr r3, [r6, r3]
003abd60: cmp r2, r7
003abd64: add r3, r3, #8
003abd68: str r3, [sp, #0x5c]
003abd6c: movwne r6, #0x14a4
003abd70: movwne sb, #0x14a8
003abd74: movwne sl, #0x14ac
003abd78: bne #0x3abde8
003abd7c: b #0x3abe68
003abd80: str r3, [r4, r6]
003abd84: ldr r3, [r7]
003abd88: mov r1, r4
003abd8c: mov r0, r3
003abd90: ldr r3, [r3]
003abd94: mov lr, pc
003abd98: ldr pc, [r3, #0x90]
003abd9c: uxtb r0, r0
003abda0: strb r0, [r4, sb]
003abda4: ldr r3, [r7, #0xc]
003abda8: sxtb r0, r0
003abdac: and r3, r3, #1
003abdb0: cmp r3, #0
003abdb4: strb r3, [r4, sl]
003abdb8: bne #0x3abe74
003abdbc: cmp r0, #0
003abdc0: cmpne r0, #2
003abdc4: beq #0x3abe74
003abdc8: cmp r0, #1
003abdcc: beq #0x3abe34
003abdd0: mov r0, r5
003abdd4: bl #0x38fb18
003abdd8: ldr r7, [sp, #8]
003abddc: ldr r3, [sp, #0x18]
003abde0: cmp r3, r7
003abde4: beq #0x3abe68
003abde8: ldr r3, [r7]
003abdec: movw r8, #0x14a4
003abdf0: cmp r4, r3
003abdf4: beq #0x3abdd0
003abdf8: ldr r2, [r4, r6]
003abdfc: cmp r2, #0
003abe00: beq #0x3abd80
003abe04: ldr r2, [r7, #0xc]
003abe08: tst r2, #1
003abe0c: bne #0x3abd80
003abe10: mov r0, r3
003abe14: mov r1, r4
003abe18: ldr r3, [r3]
003abe1c: mov lr, pc
003abe20: ldr pc, [r3, #0x90]
003abe24: cmp r0, #1
003abe28: bne #0x3abdd0
003abe2c: ldr r3, [r7]
003abe30: b #0x3abd80
003abe34: ldr r3, [r4, r6]
003abe38: ldr r2, [r3, #0x3bc]
003abe3c: cmp r4, r2
003abe40: bne #0x3abdd0
003abe44: cmp r3, #0
003abe48: beq #0x3abe5c
003abe4c: cmp fp, r3
003abe50: movne r2, #1
003abe54: movwne r3, #0x14ad
003abe58: strbne r2, [r4, r3]
003abe5c: mov r0, r5
003abe60: bl #0x38d18c
003abe64: b #0x3abc20
003abe68: movw r3, #0x14a4
003abe6c: ldr r3, [r4, r3]
003abe70: b #0x3abe44
003abe74: ldr r3, [r4, r8]
003abe78: b #0x3abe44
003abe7c: subseq r8, lr, ip, asr #29
003abe80: strdeq r3, r4, [r0], -r4
003abe84: ldrdeq r1, r2, [r0], -r4
003abe88: subseq r5, r1, r4, ror #20

# 0x3ad430 _ZNK9Character13CTRLIsAllowedEv
003ad430: push {r4, lr}
003ad434: add r4, r0, #0x4f0
003ad438: add r4, r4, #0xc
003ad43c: mov r0, r4
003ad440: mov r1, #1
003ad444: bl #0x3c034c
003ad448: cmp r0, #0
003ad44c: beq #0x3ad458
003ad450: mov r0, #0
003ad454: pop {r4, pc}
003ad458: mov r0, r4
003ad45c: mov r1, #1
003ad460: bl #0x3c0378
003ad464: eor r0, r0, #1
003ad468: uxtb r0, r0
003ad46c: pop {r4, pc}

# 0x3ad5ac _ZN9Character11ForceUseOOIEP10GameObject
003ad5ac: push {r4, r5, r6, lr}
003ad5b0: subs r5, r1, #0
003ad5b4: mov r4, r0
003ad5b8: beq #0x3ad5c8
003ad5bc: ldr r1, [r0, #0x408]
003ad5c0: cmp r1, #0
003ad5c4: beq #0x3ad5cc
003ad5c8: pop {r4, r5, r6, pc}
003ad5cc: add r6, r0, #0x4f0
003ad5d0: add r6, r6, #0xc
003ad5d4: mov r0, r6
003ad5d8: bl #0x3c0260
003ad5dc: subs r1, r0, #0
003ad5e0: beq #0x3ad600
003ad5e4: mov r1, r5
003ad5e8: add r0, r4, #0x3c8
003ad5ec: mov r2, #0
003ad5f0: bl #0x3d6890
003ad5f4: mov r3, #1
003ad5f8: strb r3, [r4, #0x412]
003ad5fc: b #0x3ad5c8
003ad600: mov r0, r6
003ad604: bl #0x3c029c
003ad608: cmp r0, #0
003ad60c: beq #0x3ad5c8
003ad610: b #0x3ad5e4

# 0x3ad614 _ZN9Character6UseOOIEv
003ad614: push {r4, r5, r6, lr}
003ad618: movw r3, #0x14a4
003ad61c: ldr r3, [r0, r3]
003ad620: mov r4, r0
003ad624: cmp r3, #0
003ad628: beq #0x3ad638
003ad62c: ldr r1, [r0, #0x408]
003ad630: cmp r1, #0
003ad634: beq #0x3ad63c
003ad638: pop {r4, r5, r6, pc}
003ad63c: add r5, r0, #0x4f0
003ad640: add r5, r5, #0xc
003ad644: mov r0, r5
003ad648: bl #0x3c0260
003ad64c: subs r1, r0, #0
003ad650: beq #0x3ad674
003ad654: movw r3, #0x14a4
003ad658: ldr r1, [r4, r3]
003ad65c: add r0, r4, #0x3c8
003ad660: mov r2, #0
003ad664: bl #0x3d6890
003ad668: mov r3, #1
003ad66c: strb r3, [r4, #0x412]
003ad670: b #0x3ad638
003ad674: mov r0, r5
003ad678: bl #0x3c029c
003ad67c: cmp r0, #0
003ad680: beq #0x3ad638
003ad684: b #0x3ad654

# 0x3ad688 _ZThn884_N9Character11Ctrl_UseOOIEP10GameObject
003ad688: sub r0, r0, #0x374
003ad68c: b #0x3ad690

# 0x3ad690 _ZN9Character11Ctrl_UseOOIEP10GameObject
003ad690: push {r4, r5, r6, lr}
003ad694: ldr r3, [r0]
003ad698: mov r4, r0
003ad69c: mov r5, r1
003ad6a0: mov lr, pc
003ad6a4: ldr pc, [r3, #0x34]
003ad6a8: cmp r0, #0
003ad6ac: bne #0x3ad6e4
003ad6b0: cmp r5, #0
003ad6b4: beq #0x3ad6c8
003ad6b8: mov r0, r4
003ad6bc: mov r1, r5
003ad6c0: pop {r4, r5, r6, lr}
003ad6c4: b #0x3ad5ac
003ad6c8: movw r3, #0x14a4
003ad6cc: ldr r3, [r4, r3]
003ad6d0: cmp r3, #0
003ad6d4: beq #0x3ad6e4
003ad6d8: mov r0, r4
003ad6dc: pop {r4, r5, r6, lr}
003ad6e0: b #0x3ad614
003ad6e4: pop {r4, r5, r6, pc}

# 0x3addc0 _ZThn884_N9Character10Ctrl_ClickERK7Point3DIfEb
003addc0: sub r0, r0, #0x374
003addc4: b #0x3addc8

# 0x3addc8 _ZN9Character10Ctrl_ClickERK7Point3DIfEb
003addc8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003addcc: ldr r6, [pc, #0x674]
003addd0: ldr sl, [pc, #0x674]
003addd4: sub sp, sp, #0xbc
003addd8: add r6, pc, r6
003adddc: ldr r3, [r6, sl]
003adde0: mov r7, r1
003adde4: str r2, [sp, #0x18]
003adde8: ldr r3, [r3]
003addec: mov r8, r0
003addf0: str r3, [sp, #0xb4]
003addf4: bl #0x3ad430
003addf8: cmp r0, #0
003addfc: bne #0x3ade1c
003ade00: ldr r3, [r6, sl]
003ade04: ldr r2, [sp, #0xb4]
003ade08: ldr r3, [r3]
003ade0c: cmp r2, r3
003ade10: bne #0x3ae444
003ade14: add sp, sp, #0xbc
003ade18: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ade1c: ldr r1, [pc, #0x62c]
003ade20: add r4, r8, #0x4f0
003ade24: add r4, r4, #0xc
003ade28: ldr r5, [r6, r1]
003ade2c: str r1, [sp, #0x20]
003ade30: mov r0, r5
003ade34: bl #0x320e74
003ade38: str r0, [sp, #0x14]
003ade3c: mov r0, r4
003ade40: bl #0x3c0334
003ade44: cmp r0, #0
003ade48: beq #0x3adecc
003ade4c: ldr r2, [sp, #0x18]
003ade50: cmp r2, #0
003ade54: beq #0x3ade74
003ade58: movw r3, #0x14c8
003ade5c: mov r2, #0
003ade60: strb r2, [r8, r3]
003ade64: mvn r1, #0
003ade68: movw r3, #0x14ca
003ade6c: strh r1, [r8, r3]
003ade70: b #0x3ade00
003ade74: mov r2, #1
003ade78: movw r3, #0x14c8
003ade7c: strb r2, [r8, r3]
003ade80: ldr r1, [r7]
003ade84: movw r3, #0x14b0
003ade88: movw r0, #0x14bc
003ade8c: str r1, [r8, r3]
003ade90: ldr r2, [r7, #4]
003ade94: movw r3, #0x14b4
003ade98: str r2, [r8, r3]
003ade9c: ldr r3, [r7, #8]
003adea0: str r1, [r8, r0]
003adea4: mov r1, #0x14c0
003adea8: str r2, [r8, r1]
003adeac: movw r2, #0x14c4
003adeb0: str r3, [r8, r2]
003adeb4: mvn r1, #0
003adeb8: movw r2, #0x14ca
003adebc: strh r1, [r8, r2]
003adec0: movw r2, #0x14b8
003adec4: str r3, [r8, r2]
003adec8: b #0x3ade00
003adecc: mov r0, r4
003aded0: bl #0x3c02e8
003aded4: cmp r0, #0
003aded8: bne #0x3ade4c
003adedc: movw r3, #0x14c8
003adee0: strb r0, [r8, r3]
003adee4: mvn r1, #0
003adee8: movw r3, #0x14ca
003adeec: strh r1, [r8, r3]
003adef0: ldr sb, [r5, #0x38]
003adef4: add r2, r8, #0x3c8
003adef8: str r0, [sp, #0x24]
003adefc: str r2, [sp, #0x1c]
003adf00: ldr r5, [sb, #0x60]!
003adf04: ldr r1, [pc, #0x548]
003adf08: movw r3, #0x23f0
003adf0c: movt r3, #0x4974
003adf10: cmp r5, sb
003adf14: str r3, [sp, #0x30]
003adf18: str r1, [sp, #0x34]
003adf1c: str r6, [sp, #0x28]
003adf20: str sl, [sp, #0x2c]
003adf24: beq #0x3ae034
003adf28: ldr r4, [r5, #8]
003adf2c: mov r0, r4
003adf30: bl #0x3935dc
003adf34: ldr r2, [r0, #8]
003adf38: cmp r4, r8
003adf3c: str r2, [sp, #8]
003adf40: ldr r6, [r0]
003adf44: ldr r0, [r0, #4]
003adf48: ldr sl, [r7]
003adf4c: ldr fp, [r7, #4]
003adf50: str r0, [sp, #0x10]
003adf54: ldr r3, [r7, #8]
003adf58: str r3, [sp, #0xc]
003adf5c: beq #0x3ae028
003adf60: ldr r3, [r4]
003adf64: mov r0, r4
003adf68: mov r1, r8
003adf6c: mov lr, pc
003adf70: ldr pc, [r3, #0x88]
003adf74: cmp r0, #0
003adf78: beq #0x3ae028
003adf7c: ldr r1, [sp, #0x14]
003adf80: cmp r1, #0
003adf84: bne #0x3ae09c
003adf88: ldr r0, [sp, #0x1c]
003adf8c: mov r1, r4
003adf90: bl #0x3d5a98
003adf94: cmp r0, #0
003adf98: bne #0x3ae028
003adf9c: ldr r1, [sp, #8]
003adfa0: ldr r0, [sp, #0xc]
003adfa4: bl #0x30e3ac
003adfa8: ldr r1, [sp, #0x10]
003adfac: mov r3, r0
003adfb0: mov r0, fp
003adfb4: str r3, [sp]
003adfb8: bl #0x30e3ac
003adfbc: mov r1, r6
003adfc0: mov fp, r0
003adfc4: mov r0, sl
003adfc8: bl #0x30e3ac
003adfcc: mov r1, r0
003adfd0: bl #0x30ed6c
003adfd4: mov r1, fp
003adfd8: mov r6, r0
003adfdc: mov r0, fp
003adfe0: bl #0x30ed6c
003adfe4: mov r1, r0
003adfe8: mov r0, r6
003adfec: bl #0x30eba4
003adff0: ldr r3, [sp]
003adff4: mov r6, r0
003adff8: mov r1, r3
003adffc: mov r0, r3
003ae000: bl #0x30ed6c
003ae004: mov r1, r0
003ae008: mov r0, r6
003ae00c: bl #0x30eba4
003ae010: mov r6, r0
003ae014: mov r1, r6
003ae018: ldr r0, [sp, #0x30]
003ae01c: bl #0x30e2f8
003ae020: cmp r0, #0
003ae024: bne #0x3ae190
003ae028: ldr r5, [r5]
003ae02c: cmp r5, sb
003ae030: bne #0x3adf28
003ae034: add r2, sp, #0x24
003ae038: ldm r2, {r2, r6, sl}
003ae03c: cmp r2, #0
003ae040: beq #0x3ae0b8
003ae044: ldr r3, [pc, #0x40c]
003ae048: add r4, sp, #0x9c
003ae04c: ldr r5, [r6, r3]
003ae050: mov r0, r5
003ae054: bl #0x337888
003ae058: ldr r1, [pc, #0x3fc]
003ae05c: add r2, sp, #0x50
003ae060: mov r0, r4
003ae064: add r1, pc, r1
003ae068: bl #0x3140ec
003ae06c: mov r1, r4
003ae070: mov r0, r5
003ae074: bl #0x337a88
003ae078: mov r0, r4
003ae07c: bl #0x3139ac
003ae080: mov r3, #1
003ae084: strb r3, [r8, #0x413]
003ae088: ldr r0, [sp, #0x1c]
003ae08c: ldr r1, [sp, #0x24]
003ae090: mov r2, #0
003ae094: bl #0x3d6890
003ae098: b #0x3ade00
003ae09c: ldr r0, [sp, #0x1c]
003ae0a0: mov r1, r4
003ae0a4: bl #0x3d574c
003ae0a8: cmp r0, #0
003ae0ac: bne #0x3adf88
003ae0b0: ldr r5, [r5]
003ae0b4: b #0x3ae02c
003ae0b8: ldr r3, [sp, #0x18]
003ae0bc: cmp r3, #0
003ae0c0: beq #0x3ae1d8
003ae0c4: ldr r1, [sp, #0x14]
003ae0c8: cmp r1, #0
003ae0cc: bne #0x3ae1d8
003ae0d0: ldr r2, [sp, #0x20]
003ae0d4: str r1, [sp, #0xc]
003ae0d8: movw r1, #0x23f0
003ae0dc: ldr r3, [r6, r2]
003ae0e0: ldr r2, [pc, #0x378]
003ae0e4: movt r1, #0x4974
003ae0e8: ldr r3, [r3, #0x38]
003ae0ec: add r2, pc, r2
003ae0f0: str r2, [sp, #8]
003ae0f4: ldr r2, [pc, #0x358]
003ae0f8: str r1, [sp, #0x10]
003ae0fc: ldr r4, [r3, #0x14]
003ae100: add fp, r3, #0xc
003ae104: add sb, sp, #0x38
003ae108: str r2, [sp, #0x24]
003ae10c: cmp r4, fp
003ae110: beq #0x3ae2dc
003ae114: ldr r1, [r4, #0x2c]
003ae118: cmp r1, #0
003ae11c: beq #0x3ae16c
003ae120: mov r0, sb
003ae124: bl #0x33dd2c
003ae128: mov r0, sb
003ae12c: bl #0x33fee4
003ae130: subs r5, r0, #0
003ae134: beq #0x3ae16c
003ae138: cmp r8, r5
003ae13c: beq #0x3ae16c
003ae140: ldr r3, [r5]
003ae144: mov r1, r8
003ae148: mov lr, pc
003ae14c: ldr pc, [r3, #0x88]
003ae150: cmp r0, #0
003ae154: beq #0x3ae16c
003ae158: ldr r0, [r5, #0x5c]
003ae15c: ldr r1, [sp, #8]
003ae160: bl #0x30e31c
003ae164: cmp r0, #0
003ae168: bne #0x3ae338
003ae16c: ldr r2, [r4, #0xc]
003ae170: cmp r2, #0
003ae174: beq #0x3ae290
003ae178: mov r4, r2
003ae17c: ldr r3, [r4, #8]
003ae180: cmp r3, #0
003ae184: beq #0x3ae10c
003ae188: mov r4, r3
003ae18c: b #0x3ae17c
003ae190: mov r1, r4
003ae194: ldr r0, [sp, #0x1c]
003ae198: bl #0x3d574c
003ae19c: ldr r2, [sp, #0x34]
003ae1a0: ldr r1, [sp, #0x28]
003ae1a4: cmp r0, #0
003ae1a8: mov r0, r4
003ae1ac: ldr r3, [r1, r2]
003ae1b0: mov r1, r7
003ae1b4: ldr r3, [r3]
003ae1b8: ldrne r2, [r3, #0x2c]
003ae1bc: ldreq r2, [r3, #0x50]
003ae1c0: bl #0x38ac0c
003ae1c4: cmp r0, #0
003ae1c8: strne r6, [sp, #0x30]
003ae1cc: strne r4, [sp, #0x24]
003ae1d0: ldr r5, [r5]
003ae1d4: b #0x3ae02c
003ae1d8: ldr sb, [pc, #0x278]
003ae1dc: add r4, sp, #0x6c
003ae1e0: ldr r5, [r6, sb]
003ae1e4: mov r0, r5
003ae1e8: bl #0x337888
003ae1ec: ldr r1, [pc, #0x270]
003ae1f0: add r2, sp, #0x48
003ae1f4: mov r0, r4
003ae1f8: add r1, pc, r1
003ae1fc: bl #0x3140ec
003ae200: mov r0, r5
003ae204: mov r1, r4
003ae208: bl #0x337a88
003ae20c: cmp r0, #0
003ae210: beq #0x3ae2c4
003ae214: mov r0, r4
003ae218: bl #0x3139ac
003ae21c: ldr r5, [r6, sb]
003ae220: add r4, sp, #0x54
003ae224: mov r0, r5
003ae228: bl #0x337888
003ae22c: ldr r1, [pc, #0x234]
003ae230: add r2, sp, #0x44
003ae234: mov r0, r4
003ae238: add r1, pc, r1
003ae23c: bl #0x3140ec
003ae240: mov r1, r4
003ae244: mov r0, r5
003ae248: bl #0x337a88
003ae24c: mov r0, r4
003ae250: bl #0x3139ac
003ae254: mov r1, #0
003ae258: mov r2, r1
003ae25c: ldr r0, [sp, #0x1c]
003ae260: bl #0x3d6890
003ae264: ldr r0, [sp, #0x1c]
003ae268: bl #0x3d49c4
003ae26c: ldr r2, [sp, #0x18]
003ae270: cmp r2, #0
003ae274: bne #0x3ae42c
003ae278: mov r0, r8
003ae27c: mov r1, r7
003ae280: ldr r3, [r8]
003ae284: mov lr, pc
003ae288: ldr pc, [r3, #0xe4]
003ae28c: b #0x3ade00
003ae290: ldr r3, [r4, #4]
003ae294: ldr r1, [r3, #0xc]
003ae298: cmp r4, r1
003ae29c: bne #0x3ae2b8
003ae2a0: mov r4, r3
003ae2a4: ldr r3, [r3, #4]
003ae2a8: ldr r2, [r3, #0xc]
003ae2ac: cmp r2, r4
003ae2b0: beq #0x3ae2a0
003ae2b4: ldr r2, [r4, #0xc]
003ae2b8: cmp r2, r3
003ae2bc: movne r4, r3
003ae2c0: b #0x3ae10c
003ae2c4: ldr r1, [sp, #0x14]
003ae2c8: cmp r1, #0
003ae2cc: beq #0x3ae214
003ae2d0: mov r0, r4
003ae2d4: bl #0x3139ac
003ae2d8: b #0x3ade00
003ae2dc: ldr r3, [sp, #0xc]
003ae2e0: cmp r3, #0
003ae2e4: beq #0x3ae1d8
003ae2e8: ldr r3, [pc, #0x168]
003ae2ec: add r4, sp, #0x84
003ae2f0: ldr r5, [r6, r3]
003ae2f4: mov r0, r5
003ae2f8: bl #0x337888
003ae2fc: ldr r1, [pc, #0x168]
003ae300: add r2, sp, #0x4c
003ae304: mov r0, r4
003ae308: add r1, pc, r1
003ae30c: bl #0x3140ec
003ae310: mov r1, r4
003ae314: mov r0, r5
003ae318: bl #0x337a88
003ae31c: mov r0, r4
003ae320: bl #0x3139ac
003ae324: ldr r0, [sp, #0x1c]
003ae328: ldr r1, [sp, #0xc]
003ae32c: mov r2, #0
003ae330: bl #0x3d6890
003ae334: b #0x3ade00
003ae338: mov r0, r5
003ae33c: bl #0x3935dc
003ae340: ldr r1, [r0]
003ae344: mov r3, r0
003ae348: ldr r0, [r7]
003ae34c: str r3, [sp]
003ae350: bl #0x30e3ac
003ae354: ldr r3, [sp]
003ae358: mov r2, r0
003ae35c: ldr r0, [r7, #4]
003ae360: ldr r1, [r3, #4]
003ae364: str r2, [sp, #4]
003ae368: bl #0x30e3ac
003ae36c: ldr r3, [sp]
003ae370: mov ip, r0
003ae374: ldr r0, [r7, #8]
003ae378: ldr r1, [r3, #8]
003ae37c: str ip, [sp]
003ae380: bl #0x30e3ac
003ae384: ldr r2, [sp, #4]
003ae388: str r0, [sp, #0x20]
003ae38c: mov r1, r2
003ae390: mov r0, r2
003ae394: bl #0x30ed6c
003ae398: ldr ip, [sp]
003ae39c: mov r3, r0
003ae3a0: str r3, [sp]
003ae3a4: mov r1, ip
003ae3a8: mov r0, ip
003ae3ac: bl #0x30ed6c
003ae3b0: ldr r3, [sp]
003ae3b4: mov r1, r0
003ae3b8: mov r0, r3
003ae3bc: bl #0x30eba4
003ae3c0: mov r3, r0
003ae3c4: ldr r0, [sp, #0x20]
003ae3c8: str r3, [sp]
003ae3cc: mov r1, r0
003ae3d0: bl #0x30ed6c
003ae3d4: ldr r3, [sp]
003ae3d8: mov r1, r0
003ae3dc: mov r0, r3
003ae3e0: bl #0x30eba4
003ae3e4: str r0, [sp, #0x20]
003ae3e8: ldr r1, [sp, #0x20]
003ae3ec: ldr r0, [sp, #0x10]
003ae3f0: bl #0x30e2f8
003ae3f4: cmp r0, #0
003ae3f8: beq #0x3ae16c
003ae3fc: ldr r1, [sp, #0x24]
003ae400: mov r0, r5
003ae404: ldr r3, [r6, r1]
003ae408: mov r1, r7
003ae40c: ldr r3, [r3]
003ae410: ldr r2, [r3, #0x54]
003ae414: bl #0x38ac0c
003ae418: cmp r0, #0
003ae41c: ldrne r2, [sp, #0x20]
003ae420: strne r5, [sp, #0xc]
003ae424: strne r2, [sp, #0x10]
003ae428: b #0x3ae16c
003ae42c: mov r0, r8
003ae430: mov r1, r7
003ae434: ldr r3, [r8]
003ae438: mov lr, pc
003ae43c: ldr pc, [r3, #0xec]
003ae440: b #0x3ade00
003ae444: bl #0x30e310
003ae448: ldrheq r6, [lr], #-0xc8
003ae44c: andeq r4, r0, ip, lsr #1
003ae450: strdeq r3, r4, [r0], -r4
003ae454: andeq r3, r0, r8, asr #5
003ae458: andeq r0, r0, r4, lsl #17
003ae45c: subseq r5, r1, r4, asr r7
003ae460: subseq r2, r1, ip, asr #7
003ae464: ldrsbeq r0, [r1], #-0xf0
003ae468: subseq r5, r1, r0, lsl #11
003ae46c: ldrheq r5, [r1], #-0x40

# 0x3b8f38 _ZN9Character10_SetTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b8f38: push {r4, lr}
003b8f3c: ldr r3, [r0, #4]
003b8f40: ldm r3, {r0, r1}
003b8f44: rsb r3, r0, r1
003b8f48: asr r3, r3, #4
003b8f4c: add r1, r3, r3, lsl #3
003b8f50: add r1, r1, r1, lsl #6
003b8f54: add r1, r3, r1, lsl #3
003b8f58: add r1, r1, r1, lsl #15
003b8f5c: add r3, r3, r1, lsl #3
003b8f60: cmp r3, #0
003b8f64: bne #0x3b8f6c
003b8f68: pop {r4, pc}
003b8f6c: ldr r3, [r0, #4]
003b8f70: cmp r3, #2
003b8f74: beq #0x3b8f80
003b8f78: cmp r3, #7
003b8f7c: bne #0x3b8f68
003b8f80: add r4, r2, #0x3c8
003b8f84: bl #0x31b5a0
003b8f88: mov r2, #0
003b8f8c: mov r1, r0
003b8f90: mov r0, r4
003b8f94: pop {r4, lr}
003b8f98: b #0x3d6890

# 0x3c02e8 _ZNK16CharStateMachine15SM_IsUsingSkillEv
003c02e8: push {r4, lr}
003c02ec: bl #0x3c01ac
003c02f0: cmp r0, #6
003c02f4: movne r0, #0
003c02f8: moveq r0, #1
003c02fc: pop {r4, pc}

# 0x3c0334 _ZNK16CharStateMachine12SM_IsCastingEv
003c0334: push {r4, lr}
003c0338: bl #0x3c01ac
003c033c: cmp r0, #7
003c0340: movne r0, #0
003c0344: moveq r0, #1
003c0348: pop {r4, pc}

# 0x3cebf0 _ZN6CharAIC2Ev
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

# 0x3d49c4 _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr r3, [r0, #0x40]
003d49c8: str r3, [r0, #0x44]
003d49cc: bx lr

# 0x3d574c _ZNK6CharAI10AI_IsEnemyEPK10GameObject
003d574c: push {r4, r5, r6, r7, r8, lr}
003d5750: ldr r4, [pc, #0x2fc]
003d5754: subs r7, r1, #0
003d5758: sub sp, sp, #0x18
003d575c: mov r5, r0
003d5760: add r4, pc, r4
003d5764: beq #0x3d59d0
003d5768: add r6, sp, #0xc
003d576c: mov r0, r6
003d5770: mov r1, r7
003d5774: bl #0x33dd70
003d5778: mov r0, r6
003d577c: mov r1, #0
003d5780: bl #0x33ff8c
003d5784: subs r6, r0, #0
003d5788: bne #0x3d57bc
003d578c: cmp r7, #0
003d5790: beq #0x3d57b0
003d5794: ldr r3, [r7]
003d5798: mov r0, r7
003d579c: ldr r1, [r5, #4]
003d57a0: mov lr, pc
003d57a4: ldr pc, [r3, #0x88]
003d57a8: cmp r0, #0
003d57ac: bne #0x3d58e4
003d57b0: mov r0, #0
003d57b4: add sp, sp, #0x18
003d57b8: pop {r4, r5, r6, r7, r8, pc}
003d57bc: ldr r8, [r6, #0xf4]
003d57c0: cmp r8, #0
003d57c4: bne #0x3d578c
003d57c8: bl #0x3a3180
003d57cc: cmp r0, #0
003d57d0: blt #0x3d597c
003d57d4: mov r0, r6
003d57d8: bl #0x3a3180
003d57dc: ldr r7, [pc, #0x274]
003d57e0: ldr r3, [r4, r7]
003d57e4: ldr r3, [r3]
003d57e8: cmp r0, r3
003d57ec: blt #0x3d5814
003d57f0: ldr r3, [pc, #0x264]
003d57f4: ldr r3, [r4, r3]
003d57f8: ldr r3, [r3]
003d57fc: cmp r3, #2
003d5800: moveq r3, #0
003d5804: streq r3, [r3]
003d5808: beq #0x3d5814
003d580c: cmp r3, #1
003d5810: beq #0x3d5a20
003d5814: ldr r0, [r5, #4]
003d5818: bl #0x3a3180
003d581c: cmp r0, #0
003d5820: blt #0x3d5924
003d5824: ldr r0, [r5, #4]
003d5828: bl #0x3a3180
003d582c: ldr r3, [r4, r7]
003d5830: ldr r3, [r3]
003d5834: cmp r0, r3
003d5838: blt #0x3d5860
003d583c: ldr r3, [pc, #0x218]
003d5840: ldr r3, [r4, r3]
003d5844: ldr r3, [r3]
003d5848: cmp r3, #2
003d584c: moveq r3, #0
003d5850: streq r3, [r3]
003d5854: beq #0x3d5860
003d5858: cmp r3, #1
003d585c: beq #0x3d59ec
003d5860: ldr r3, [r5, #4]
003d5864: mov r0, r3
003d5868: ldr r3, [r3]
003d586c: mov lr, pc
003d5870: ldr pc, [r3, #0x28]
003d5874: cmp r0, #0
003d5878: bne #0x3d5908
003d587c: ldr r3, [pc, #0x1dc]
003d5880: ldr r0, [r5, #4]
003d5884: ldr r3, [r4, r3]
003d5888: ldr r4, [r3]
003d588c: bl #0x3a3180
003d5890: mov r3, #0xc
003d5894: mla r4, r3, r0, r4
003d5898: mov r0, r6
003d589c: bl #0x3a3180
003d58a0: ldr ip, [r4, #4]
003d58a4: cmp ip, #0
003d58a8: beq #0x3d57b0
003d58ac: ldr r2, [r4, #8]
003d58b0: ldr r3, [r2, #4]
003d58b4: cmp r0, r3
003d58b8: movne r3, #0
003d58bc: bne #0x3d58d0
003d58c0: b #0x3d59e0
003d58c4: ldr r1, [r2, #4]
003d58c8: cmp r0, r1
003d58cc: beq #0x3d59e0
003d58d0: add r3, r3, #1
003d58d4: cmp r3, ip
003d58d8: add r2, r2, #0xc
003d58dc: bne #0x3d58c4
003d58e0: b #0x3d57b0
003d58e4: mov r0, r7
003d58e8: ldr r1, [r5, #4]
003d58ec: ldr r3, [r7]
003d58f0: mov lr, pc
003d58f4: ldr pc, [r3, #0x90]
003d58f8: cmp r0, #8
003d58fc: movne r0, #0
003d5900: moveq r0, #1
003d5904: b #0x3d57b4
003d5908: ldr r3, [r6]
003d590c: mov r0, r6
003d5910: mov lr, pc
003d5914: ldr pc, [r3, #0x28]
003d5918: cmp r0, #0
003d591c: bne #0x3d57b0
003d5920: b #0x3d587c
003d5924: ldr r3, [pc, #0x130]
003d5928: ldr r3, [r4, r3]
003d592c: ldr r3, [r3]
003d5930: cmp r3, #2
003d5934: moveq r3, #0
003d5938: streq r3, [r3]
003d593c: beq #0x3d5824
003d5940: cmp r3, #1
003d5944: bne #0x3d5824
003d5948: ldr r0, [pc, #0x114]
003d594c: ldr r1, [pc, #0x114]
003d5950: ldr r2, [pc, #0x114]
003d5954: ldr r0, [r4, r0]
003d5958: ldr r3, [pc, #0x110]
003d595c: movw ip, #0x109
003d5960: add r1, pc, r1
003d5964: add r2, pc, r2
003d5968: add r3, pc, r3
003d596c: add r0, r0, #0xa8
003d5970: str ip, [sp]
003d5974: bl #0x30e004
003d5978: b #0x3d5824
003d597c: ldr r3, [pc, #0xd8]
003d5980: ldr r3, [r4, r3]
003d5984: ldr r3, [r3]
003d5988: cmp r3, #2
003d598c: streq r8, [r8]
003d5990: beq #0x3d57d4
003d5994: cmp r3, #1
003d5998: bne #0x3d57d4
003d599c: ldr r0, [pc, #0xc0]
003d59a0: ldr r1, [pc, #0xcc]
003d59a4: ldr r2, [pc, #0xcc]
003d59a8: ldr r0, [r4, r0]
003d59ac: ldr r3, [pc, #0xc8]
003d59b0: movw ip, #0x107
003d59b4: add r1, pc, r1
003d59b8: add r2, pc, r2
003d59bc: add r3, pc, r3
003d59c0: add r0, r0, #0xa8
003d59c4: str ip, [sp]
003d59c8: bl #0x30e004
003d59cc: b #0x3d57d4
003d59d0: ldr r7, [r0, #0x40]
003d59d4: cmp r7, #0
003d59d8: beq #0x3d57b0
003d59dc: b #0x3d5768
003d59e0: ldr r0, [r2, #8]
003d59e4: lsr r0, r0, #0x1f
003d59e8: b #0x3d57b4
003d59ec: ldr r0, [pc, #0x70]
003d59f0: ldr r1, [pc, #0x88]
003d59f4: ldr r2, [pc, #0x88]
003d59f8: ldr r0, [r4, r0]
003d59fc: ldr r3, [pc, #0x84]
003d5a00: movw ip, #0x10a
003d5a04: add r1, pc, r1
003d5a08: add r2, pc, r2
003d5a0c: add r3, pc, r3
003d5a10: add r0, r0, #0xa8
003d5a14: str ip, [sp]
003d5a18: bl #0x30e004
003d5a1c: b #0x3d5860
003d5a20: ldr r0, [pc, #0x3c]
003d5a24: ldr r1, [pc, #0x60]
003d5a28: ldr r2, [pc, #0x60]
003d5a2c: ldr r0, [r4, r0]
003d5a30: ldr r3, [pc, #0x5c]
003d5a34: mov ip, #0x108
003d5a38: add r1, pc, r1
003d5a3c: add r2, pc, r2
003d5a40: add r3, pc, r3
003d5a44: add r0, r0, #0xa8
003d5a48: str ip, [sp]
003d5a4c: bl #0x30e004
003d5a50: b #0x3d5814
003d5a54: subseq pc, fp, r0, lsr r3
003d5a58: andeq r2, r0, r4, asr #4
003d5a5c: andeq r3, r0, r0, asr #19
003d5a60: andeq r4, r0, ip, lsr #12
003d5a64: andeq r1, r0, r0, asr #19
003d5a68: subeq r8, lr, r8, ror sl
003d5a6c: subeq pc, lr, r4, lsl sp
003d5a70: subeq pc, lr, r8, asr ip
003d5a74: subeq r8, lr, r4, lsr #20
003d5a78: subeq pc, lr, r0, ror #24
003d5a7c: subeq pc, lr, r4, lsl #24
003d5a80: ldrdeq r8, sb, [lr], #-0x94
003d5a84: umaaleq pc, lr, r0, ip
003d5a88: strheq pc, [lr], #-0xb4
003d5a8c: subeq r8, lr, r0, lsr #19

# 0x3d5a98 _ZNK6CharAI12AI_IsNeutralEPK10GameObject
003d5a98: push {r4, r5, r6, r7, lr}
003d5a9c: ldr r4, [pc, #0x27c]
003d5aa0: cmp r1, #0
003d5aa4: sub sp, sp, #0x1c
003d5aa8: mov r5, r0
003d5aac: add r4, pc, r4
003d5ab0: beq #0x3d5ca8
003d5ab4: add r6, sp, #0xc
003d5ab8: mov r0, r6
003d5abc: bl #0x33dd70
003d5ac0: mov r0, r6
003d5ac4: mov r1, #0
003d5ac8: bl #0x33ff8c
003d5acc: subs r6, r0, #0
003d5ad0: bne #0x3d5ae0
003d5ad4: mov r0, #1
003d5ad8: add sp, sp, #0x1c
003d5adc: pop {r4, r5, r6, r7, pc}
003d5ae0: ldr r7, [r6, #0xf4]
003d5ae4: cmp r7, #0
003d5ae8: bne #0x3d5ad4
003d5aec: bl #0x3a3180
003d5af0: cmp r0, #0
003d5af4: blt #0x3d5c54
003d5af8: mov r0, r6
003d5afc: bl #0x3a3180
003d5b00: ldr r7, [pc, #0x21c]
003d5b04: ldr r3, [r4, r7]
003d5b08: ldr r3, [r3]
003d5b0c: cmp r0, r3
003d5b10: blt #0x3d5b38
003d5b14: ldr r3, [pc, #0x20c]
003d5b18: ldr r3, [r4, r3]
003d5b1c: ldr r3, [r3]
003d5b20: cmp r3, #2
003d5b24: moveq r3, #0
003d5b28: streq r3, [r3]
003d5b2c: beq #0x3d5b38
003d5b30: cmp r3, #1
003d5b34: beq #0x3d5cec
003d5b38: ldr r0, [r5, #4]
003d5b3c: bl #0x3a3180
003d5b40: cmp r0, #0
003d5b44: blt #0x3d5bfc
003d5b48: ldr r0, [r5, #4]
003d5b4c: bl #0x3a3180
003d5b50: ldr r3, [r4, r7]
003d5b54: ldr r3, [r3]
003d5b58: cmp r0, r3
003d5b5c: blt #0x3d5b84
003d5b60: ldr r3, [pc, #0x1c0]
003d5b64: ldr r3, [r4, r3]
003d5b68: ldr r3, [r3]
003d5b6c: cmp r3, #2
003d5b70: moveq r3, #0
003d5b74: streq r3, [r3]
003d5b78: beq #0x3d5b84
003d5b7c: cmp r3, #1
003d5b80: beq #0x3d5cb8
003d5b84: ldr r3, [pc, #0x1a0]
003d5b88: ldr r0, [r5, #4]
003d5b8c: ldr r3, [r4, r3]
003d5b90: ldr r4, [r3]
003d5b94: bl #0x3a3180
003d5b98: mov r3, #0xc
003d5b9c: mla r4, r3, r0, r4
003d5ba0: mov r0, r6
003d5ba4: bl #0x3a3180
003d5ba8: ldr ip, [r4, #4]
003d5bac: cmp ip, #0
003d5bb0: beq #0x3d5ad4
003d5bb4: ldr r2, [r4, #8]
003d5bb8: ldr r3, [r2, #4]
003d5bbc: cmp r0, r3
003d5bc0: movne r3, #0
003d5bc4: bne #0x3d5bd8
003d5bc8: b #0x3d5bec
003d5bcc: ldr r1, [r2, #4]
003d5bd0: cmp r0, r1
003d5bd4: beq #0x3d5bec
003d5bd8: add r3, r3, #1
003d5bdc: cmp r3, ip
003d5be0: add r2, r2, #0xc
003d5be4: bne #0x3d5bcc
003d5be8: b #0x3d5ad4
003d5bec: ldr r0, [r2, #8]
003d5bf0: rsbs r0, r0, #1
003d5bf4: movlo r0, #0
003d5bf8: b #0x3d5ad8
003d5bfc: ldr r3, [pc, #0x124]
003d5c00: ldr r3, [r4, r3]
003d5c04: ldr r3, [r3]
003d5c08: cmp r3, #2
003d5c0c: moveq r3, #0
003d5c10: streq r3, [r3]
003d5c14: beq #0x3d5b48
003d5c18: cmp r3, #1
003d5c1c: bne #0x3d5b48
003d5c20: ldr r0, [pc, #0x108]
003d5c24: ldr r1, [pc, #0x108]
003d5c28: ldr r2, [pc, #0x108]
003d5c2c: ldr r0, [r4, r0]
003d5c30: ldr r3, [pc, #0x104]
003d5c34: mov ip, #0xe4
003d5c38: add r1, pc, r1
003d5c3c: add r2, pc, r2
003d5c40: add r3, pc, r3
003d5c44: add r0, r0, #0xa8
003d5c48: str ip, [sp]
003d5c4c: bl #0x30e004
003d5c50: b #0x3d5b48
003d5c54: ldr r3, [pc, #0xcc]
003d5c58: ldr r3, [r4, r3]
003d5c5c: ldr r3, [r3]
003d5c60: cmp r3, #2
003d5c64: streq r7, [r7]
003d5c68: beq #0x3d5af8
003d5c6c: cmp r3, #1
003d5c70: bne #0x3d5af8
003d5c74: ldr r0, [pc, #0xb4]
003d5c78: ldr r1, [pc, #0xc0]
003d5c7c: ldr r2, [pc, #0xc0]
003d5c80: ldr r0, [r4, r0]
003d5c84: ldr r3, [pc, #0xbc]
003d5c88: mov ip, #0xe2
003d5c8c: add r1, pc, r1
003d5c90: add r2, pc, r2
003d5c94: add r3, pc, r3
003d5c98: add r0, r0, #0xa8
003d5c9c: str ip, [sp]
003d5ca0: bl #0x30e004
003d5ca4: b #0x3d5af8
003d5ca8: ldr r1, [r0, #0x40]
003d5cac: cmp r1, #0
003d5cb0: beq #0x3d5ad4
003d5cb4: b #0x3d5ab4
003d5cb8: ldr r0, [pc, #0x70]
003d5cbc: ldr r1, [pc, #0x88]
003d5cc0: ldr r2, [pc, #0x88]
003d5cc4: ldr r0, [r4, r0]
003d5cc8: ldr r3, [pc, #0x84]
003d5ccc: mov ip, #0xe5
003d5cd0: add r1, pc, r1
003d5cd4: add r2, pc, r2
003d5cd8: add r3, pc, r3
003d5cdc: add r0, r0, #0xa8
003d5ce0: str ip, [sp]
003d5ce4: bl #0x30e004
003d5ce8: b #0x3d5b84
003d5cec: ldr r0, [pc, #0x3c]
003d5cf0: ldr r1, [pc, #0x60]
003d5cf4: ldr r2, [pc, #0x60]
003d5cf8: ldr r0, [r4, r0]
003d5cfc: ldr r3, [pc, #0x5c]
003d5d00: mov ip, #0xe3
003d5d04: add r1, pc, r1
003d5d08: add r2, pc, r2
003d5d0c: add r3, pc, r3
003d5d10: add r0, r0, #0xa8
003d5d14: str ip, [sp]
003d5d18: bl #0x30e004
003d5d1c: b #0x3d5b38
003d5d20: subseq lr, fp, r4, ror #31
003d5d24: andeq r2, r0, r4, asr #4
003d5d28: andeq r3, r0, r0, asr #19
003d5d2c: andeq r4, r0, ip, lsr #12
003d5d30: andeq r1, r0, r0, asr #19
003d5d34: subeq r8, lr, r0, lsr #15
003d5d38: subeq pc, lr, ip, lsr sl
003d5d3c: subeq pc, lr, r0, lsl #19
003d5d40: subeq r8, lr, ip, asr #14
003d5d44: subeq pc, lr, r8, lsl #19
003d5d48: subeq pc, lr, ip, lsr #18
003d5d4c: subeq r8, lr, r8, lsl #14
003d5d50: subeq pc, lr, r4, asr #19
003d5d54: subeq pc, lr, r8, ror #17
003d5d58: ldrdeq r8, sb, [lr], #-0x64
003d5d5c: subeq pc, lr, r0, lsr sb
003d5d60: strheq pc, [lr], #-0x84

# 0x3d6890 _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr r5, [pc, #0x204]
003d6898: ldr r7, [pc, #0x204]
003d689c: sub sp, sp, #0x78
003d68a0: add r5, pc, r5
003d68a4: ldr r3, [r5, r7]
003d68a8: mov r4, r0
003d68ac: cmp r2, #0
003d68b0: ldr r3, [r3]
003d68b4: mov r6, r1
003d68b8: str r1, [r4, #0x3c]
003d68bc: str r3, [sp, #0x74]
003d68c0: bne #0x3d6a20
003d68c4: ldr r3, [r0, #0x40]
003d68c8: ldr sb, [pc, #0x1d8]
003d68cc: add r8, sp, #0x5c
003d68d0: cmp r3, r1
003d68d4: ldrne r1, [r0, #4]
003d68d8: ldr sl, [r5, sb]
003d68dc: movwne r3, #0x14d0
003d68e0: strhne r2, [r1, r3]
003d68e4: mov r0, sl
003d68e8: bl #0x337888
003d68ec: ldr r1, [pc, #0x1b8]
003d68f0: add r2, sp, #0x10
003d68f4: mov r0, r8
003d68f8: add r1, pc, r1
003d68fc: bl #0x3140ec
003d6900: mov r0, sl
003d6904: mov r1, r8
003d6908: bl #0x337a88
003d690c: mov sl, r0
003d6910: ldr r0, [sp, #0x70]
003d6914: cmp r0, r8
003d6918: beq #0x3d6938
003d691c: cmp r0, #0
003d6920: beq #0x3d6938
003d6924: ldr r1, [sp, #0x5c]
003d6928: rsb r1, r0, r1
003d692c: cmp r1, #0x80
003d6930: bhi #0x3d6a4c
003d6934: bl #0x708f00
003d6938: cmp sl, #0
003d693c: beq #0x3d69a4
003d6940: ldr r3, [r4, #0x40]
003d6944: cmp r3, r6
003d6948: beq #0x3d69a4
003d694c: subs r3, r3, #0
003d6950: movne r3, #1
003d6954: subs r2, r6, #0
003d6958: movne r2, #1
003d695c: tst r2, r3
003d6960: bne #0x3d6a28
003d6964: cmp r3, #0
003d6968: beq #0x3d6a18
003d696c: ldr sl, [r5, sb]
003d6970: add r8, sp, #0x2c
003d6974: mov r0, sl
003d6978: bl #0x337888
003d697c: ldr r1, [pc, #0x12c]
003d6980: add r2, sp, #8
003d6984: mov r0, r8
003d6988: add r1, pc, r1
003d698c: bl #0x3140ec
003d6990: mov r0, sl
003d6994: mov r1, r8
003d6998: bl #0x337a88
003d699c: mov r0, r8
003d69a0: bl #0x318254
003d69a4: cmp r6, #0
003d69a8: str r6, [r4, #0x40]
003d69ac: beq #0x3d69fc
003d69b0: ldr r0, [r4, #4]
003d69b4: bl #0x3a2fec
003d69b8: ldr r2, [r4, #0x44]
003d69bc: ldr r3, [r4, #0x40]
003d69c0: cmp r3, r2
003d69c4: movne r2, #0
003d69c8: strbne r2, [r4, #0x4c]
003d69cc: movne r2, r3
003d69d0: str r2, [r4, #0x44]
003d69d4: mov r0, r3
003d69d8: ldr r3, [r3]
003d69dc: mov lr, pc
003d69e0: ldr pc, [r3, #0x34]
003d69e4: eor r0, r0, #1
003d69e8: strb r0, [r4, #0x48]
003d69ec: ldr r1, [r4, #0x40]
003d69f0: mov r0, r4
003d69f4: bl #0x3d4ed8
003d69f8: strb r0, [r4, #0x49]
003d69fc: ldr r3, [r5, r7]
003d6a00: ldr r2, [sp, #0x74]
003d6a04: ldr r3, [r3]
003d6a08: cmp r2, r3
003d6a0c: bne #0x3d6a9c
003d6a10: add sp, sp, #0x78
003d6a14: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp r2, #0
003d6a1c: bne #0x3d6a5c
003d6a20: str r6, [r4, #0x40]
003d6a24: b #0x3d69fc
003d6a28: ldr sl, [r5, sb]
003d6a2c: add r8, sp, #0x44
003d6a30: mov r0, sl
003d6a34: bl #0x337888
003d6a38: ldr r1, [pc, #0x74]
003d6a3c: add r2, sp, #0xc
003d6a40: mov r0, r8
003d6a44: add r1, pc, r1
003d6a48: b #0x3d698c
003d6a4c: bl #0x310440
003d6a50: cmp sl, #0
003d6a54: beq #0x3d69a4
003d6a58: b #0x3d6940
003d6a5c: ldr sl, [r5, sb]
003d6a60: add r8, sp, #0x14
003d6a64: mov r0, sl
003d6a68: bl #0x337888
003d6a6c: ldr r1, [pc, #0x44]
003d6a70: add r2, sp, #4
003d6a74: mov r0, r8
003d6a78: add r1, pc, r1
003d6a7c: bl #0x3140ec
003d6a80: mov r1, r8
003d6a84: mov r0, sl
003d6a88: bl #0x337a88
003d6a8c: mov r0, r8
003d6a90: bl #0x318254
003d6a94: str r6, [r4, #0x40]
003d6a98: b #0x3d69b0
003d6a9c: bl #0x30e310
003d6aa0: ldrsheq lr, [fp], #-0x10
003d6aa4: andeq r4, r0, ip, lsr #1
003d6aa8: andeq r0, r0, r4, lsl #17
003d6aac: subeq lr, lr, r0, ror #27
003d6ab0: subeq lr, lr, r8, ror #26
003d6ab4: subeq lr, lr, ip, lsr #25
003d6ab8: subeq lr, lr, r8, ror ip

# 0x3ec474 _ZN10ItemObject17_DoAutoPickupHackEPS_PK10GameObjectS3_
003ec474: push {r4, r5, r6, r7, r8, lr}
003ec478: mov r1, #0
003ec47c: mov r4, r0
003ec480: add r0, r0, #0x374
003ec484: mov r5, r2
003ec488: bl #0x3fc61c
003ec48c: ldr r7, [pc, #0x60]
003ec490: cmp r0, #0
003ec494: cmpne r4, #0
003ec498: add r7, pc, r7
003ec49c: bne #0x3ec4a4
003ec4a0: pop {r4, r5, r6, r7, r8, pc}
003ec4a4: bl #0x3f9e34
003ec4a8: ldr r3, [pc, #0x48]
003ec4ac: ldr r1, [pc, #0x48]
003ec4b0: ldr r2, [pc, #0x48]
003ec4b4: ldr r3, [r7, r3]
003ec4b8: mov r6, r0
003ec4bc: add r1, pc, r1
003ec4c0: add r2, pc, r2
003ec4c4: ldr r0, [r3, #0x2c]
003ec4c8: bl #0x4c4bdc
003ec4cc: cmp r6, r0
003ec4d0: bne #0x3ec4a0
003ec4d4: cmp r5, #0
003ec4d8: beq #0x3ec4a0
003ec4dc: mov r0, r4
003ec4e0: mov r1, r5
003ec4e4: ldr r3, [r4]
003ec4e8: mov lr, pc
003ec4ec: ldr pc, [r3, #0x98]
003ec4f0: pop {r4, r5, r6, r7, r8, pc}
003ec4f4: ldrsheq r8, [sl], #-0x58
003ec4f8: strdeq r3, r4, [r0], -r4
003ec4fc: subeq sb, sp, r4, asr #27
003ec500: ldrdeq sb, sl, [sp], #-0xd0

# 0x3ee3ac _ZN19QE_PickedUpLiftableD1Ev
003ee3ac: bx lr

# 0x3ee5fc _ZN14LiftableObject6PickUpEP10GameObject
003ee5fc: push {r4, r5, r6, r7, r8, sl, lr}
003ee600: ldr r3, [r0, #0x390]
003ee604: ldr r4, [pc, #0x304]
003ee608: sub sp, sp, #0x34
003ee60c: cmp r3, #0
003ee610: mov r5, r0
003ee614: mov r6, r1
003ee618: add r4, pc, r4
003ee61c: beq #0x3ee644
003ee620: ldr r3, [pc, #0x2ec]
003ee624: ldr r3, [r4, r3]
003ee628: ldr r3, [r3]
003ee62c: cmp r3, #2
003ee630: moveq r3, #0
003ee634: streq r3, [r3]
003ee638: beq #0x3ee644
003ee63c: cmp r3, #1
003ee640: beq #0x3ee7e0
003ee644: cmp r6, #0
003ee648: beq #0x3ee78c
003ee64c: ldr r3, [r5, #0x390]
003ee650: cmp r3, #0
003ee654: beq #0x3ee664
003ee658: mov r0, #0
003ee65c: add sp, sp, #0x34
003ee660: pop {r4, r5, r6, r7, r8, sl, pc}
003ee664: cmp r6, #0
003ee668: beq #0x3ee658
003ee66c: ldr r8, [r6, #0x2d8]
003ee670: ldr r7, [r5, #0x2d8]
003ee674: cmp r8, #0
003ee678: beq #0x3ee868
003ee67c: cmp r7, #0
003ee680: beq #0x3ee814
003ee684: cmp r8, #0
003ee688: cmpne r7, #0
003ee68c: beq #0x3ee658
003ee690: ldr r1, [pc, #0x280]
003ee694: mov r0, r8
003ee698: add r1, pc, r1
003ee69c: bl #0x470a18
003ee6a0: ldr r3, [pc, #0x274]
003ee6a4: subs sl, r0, #0
003ee6a8: ldreq sl, [r8, #8]
003ee6ac: ldr r1, [r4, r3]
003ee6b0: mov r0, r7
003ee6b4: bl #0x470c24
003ee6b8: mov r3, #0x3f800000
003ee6bc: mov r0, r7
003ee6c0: add r1, sp, #0x24
003ee6c4: str r3, [sp, #0x2c]
003ee6c8: str r3, [sp, #0x24]
003ee6cc: str r3, [sp, #0x28]
003ee6d0: ldr r8, [pc, #0x248]
003ee6d4: bl #0x4727ac
003ee6d8: ldr r1, [r7, #8]
003ee6dc: ldr r3, [sl]
003ee6e0: mov r0, sl
003ee6e4: mov lr, pc
003ee6e8: ldr pc, [r3, #0x5c]
003ee6ec: ldr r0, [r5, #0x2dc]
003ee6f0: bl #0x46eb70
003ee6f4: ldr r0, [r5, #0x2dc]
003ee6f8: bl #0x46eae0
003ee6fc: str r6, [r5, #0x390]
003ee700: ldr r0, [r4, r8]
003ee704: bl #0x31f594
003ee708: subs r6, r0, #0
003ee70c: beq #0x3ee8bc
003ee710: ldr r3, [r5]
003ee714: mov r0, r5
003ee718: mov lr, pc
003ee71c: ldr pc, [r3, #0xc8]
003ee720: ldr r3, [r4, r8]
003ee724: ldr r1, [pc, #0x1f8]
003ee728: ldr r2, [pc, #0x1f8]
003ee72c: mov r7, r0
003ee730: add r1, pc, r1
003ee734: ldr r0, [r3, #0x2c]
003ee738: add r2, pc, r2
003ee73c: ldr r5, [r5, #0x64]
003ee740: bl #0x4c4bdc
003ee744: ldr r2, [pc, #0x1e0]
003ee748: add r1, sp, #0x30
003ee74c: mov r3, #0
003ee750: ldr r2, [r4, r2]
003ee754: str r0, [sp, #0xc]
003ee758: mov r0, r6
003ee75c: add r2, r2, #8
003ee760: str r2, [r1, #-0x28]!
003ee764: mvn r2, #0
003ee768: str r5, [sp, #0x14]
003ee76c: strb r3, [sp, #0x19]
003ee770: str r2, [sp, #0x1c]
003ee774: str r7, [sp, #0x20]
003ee778: str r3, [sp, #0x10]
003ee77c: strb r3, [sp, #0x18]
003ee780: bl #0x339090
003ee784: mov r0, #1
003ee788: b #0x3ee65c
003ee78c: ldr r3, [pc, #0x180]
003ee790: ldr r3, [r4, r3]
003ee794: ldr r3, [r3]
003ee798: cmp r3, #2
003ee79c: streq r6, [r6]
003ee7a0: beq #0x3ee64c
003ee7a4: cmp r3, #1
003ee7a8: bne #0x3ee64c
003ee7ac: ldr r0, [pc, #0x17c]
003ee7b0: ldr r1, [pc, #0x17c]
003ee7b4: ldr r2, [pc, #0x17c]
003ee7b8: ldr r0, [r4, r0]
003ee7bc: ldr r3, [pc, #0x178]
003ee7c0: mov ip, #0xbb
003ee7c4: add r1, pc, r1
003ee7c8: add r2, pc, r2
003ee7cc: add r3, pc, r3
003ee7d0: add r0, r0, #0xa8
003ee7d4: str ip, [sp]
003ee7d8: bl #0x30e004
003ee7dc: b #0x3ee64c
003ee7e0: ldr r0, [pc, #0x148]
003ee7e4: ldr r1, [pc, #0x154]
003ee7e8: ldr r2, [pc, #0x154]
003ee7ec: ldr r0, [r4, r0]
003ee7f0: ldr r3, [pc, #0x150]
003ee7f4: mov ip, #0xba
003ee7f8: add r1, pc, r1
003ee7fc: add r2, pc, r2
003ee800: add r3, pc, r3
003ee804: add r0, r0, #0xa8
003ee808: str ip, [sp]
003ee80c: bl #0x30e004
003ee810: b #0x3ee644
003ee814: ldr r3, [pc, #0xf8]
003ee818: ldr r3, [r4, r3]
003ee81c: ldr r3, [r3]
003ee820: cmp r3, #2
003ee824: streq r7, [r7]
003ee828: beq #0x3ee684
003ee82c: cmp r3, #1
003ee830: bne #0x3ee684
003ee834: ldr r0, [pc, #0xf4]
003ee838: ldr r1, [pc, #0x10c]
003ee83c: ldr r2, [pc, #0x10c]
003ee840: ldr r0, [r4, r0]
003ee844: ldr r3, [pc, #0x108]
003ee848: mov ip, #0xc8
003ee84c: add r1, pc, r1
003ee850: add r2, pc, r2
003ee854: add r3, pc, r3
003ee858: add r0, r0, #0xa8
003ee85c: str ip, [sp]
003ee860: bl #0x30e004
003ee864: b #0x3ee684
003ee868: ldr r3, [pc, #0xa4]
003ee86c: ldr r3, [r4, r3]
003ee870: ldr r3, [r3]
003ee874: cmp r3, #2
003ee878: streq r8, [r8]
003ee87c: beq #0x3ee67c
003ee880: cmp r3, #1
003ee884: bne #0x3ee67c
003ee888: ldr r0, [pc, #0xa0]
003ee88c: ldr r1, [pc, #0xc4]
003ee890: ldr r2, [pc, #0xc4]
003ee894: ldr r0, [r4, r0]
003ee898: ldr r3, [pc, #0xc0]
003ee89c: mov ip, #0xc7
003ee8a0: add r1, pc, r1
003ee8a4: add r2, pc, r2
003ee8a8: add r3, pc, r3
003ee8ac: add r0, r0, #0xa8
003ee8b0: str ip, [sp]
003ee8b4: bl #0x30e004
003ee8b8: b #0x3ee67c
003ee8bc: ldr r3, [pc, #0x50]
003ee8c0: ldr r3, [r4, r3]
003ee8c4: ldr r3, [r3]
003ee8c8: cmp r3, #2
003ee8cc: streq r6, [r6]
003ee8d0: beq #0x3ee710
003ee8d4: cmp r3, #1
003ee8d8: bne #0x3ee710
003ee8dc: ldr r0, [pc, #0x4c]
003ee8e0: ldr r1, [pc, #0x7c]
003ee8e4: ldr r2, [pc, #0x7c]
003ee8e8: ldr r0, [r4, r0]
003ee8ec: ldr r3, [pc, #0x78]
003ee8f0: mov ip, #0xdf
003ee8f4: add r1, pc, r1
003ee8f8: add r2, pc, r2
003ee8fc: add r3, pc, r3
003ee900: add r0, r0, #0xa8
003ee904: str ip, [sp]
003ee908: bl #0x30e004
003ee90c: b #0x3ee710
003ee910: subseq r6, sl, r8, ror r4
003ee914: andeq r3, r0, r0, asr #19
003ee918: subeq r7, sp, r0, lsl #18
003ee91c: andeq r3, r0, ip, lsr #30
003ee920: strdeq r3, r4, [r0], -r4
003ee924: subeq r4, sp, r8, lsr r2
003ee928: subeq r7, sp, r0, lsl sp
003ee92c: andeq r1, r0, ip, lsl r7
003ee930: andeq r1, r0, r0, asr #19
003ee934: subeq pc, ip, r4, lsl ip
003ee938: subeq r7, sp, r0, ror #24
003ee93c: strdeq r7, r8, [sp], #-0xb4
003ee940: subeq pc, ip, r0, ror #23
003ee944: subeq r7, sp, ip, lsl ip
003ee948: subeq r7, sp, r0, asr #23
003ee94c: subeq pc, ip, ip, lsl #23
003ee950: strdeq r7, r8, [sp], #-0xb0
003ee954: subeq r7, sp, ip, ror #22
003ee958: subeq pc, ip, r8, lsr fp
003ee95c: umaaleq r7, sp, r4, fp
003ee960: subeq r7, sp, r8, lsl fp
003ee964: subeq pc, ip, r4, ror #21
003ee968: subseq r1, r2, r0, rrx
003ee96c: subeq r7, sp, r4, asr #21

# 0x3ee970 _ZN19QE_PickedUpLiftableD0Ev
003ee970: ldr r3, [pc, #0x24]
003ee974: ldr r2, [pc, #0x24]
003ee978: push {r4, lr}
003ee97c: add r3, pc, r3
003ee980: ldr r2, [r3, r2]
003ee984: mov r4, r0
003ee988: add r2, r2, #8
003ee98c: str r2, [r0]
003ee990: bl #0x310440
003ee994: mov r0, r4
003ee998: pop {r4, pc}
003ee99c: subseq r6, sl, r4, lsl r1
003ee9a0: strheq r0, [r0], -r0

# 0x3f9e34 _ZNK12ItemInstance13GetPickUpTypeEv
003f9e34: push {r4, lr}
003f9e38: ldrsh r3, [r0, #0x58]
003f9e3c: cmn r3, #1
003f9e40: beq #0x3f9e4c
003f9e44: mov r0, r3
003f9e48: pop {r4, pc}
003f9e4c: bl #0x3f9e08
003f9e50: ldr r0, [r0, #0xc]
003f9e54: pop {r4, pc}

# 0x404d60 _ZN14v2Controllable10Ctrl_ClickERK7Point3DIfEb
00404d60: bx lr

# 0x404da8 _ZN14v2Controllable11Ctrl_UseOOIEP10GameObject
00404da8: bx lr

# 0x4051a8 _ZN12v2Controller9Cmd_ClickERK7Point3DIfEb
004051a8: push {r4, lr}
004051ac: ldrb ip, [r0, #9]
004051b0: ldr r3, [pc, #0x44]
004051b4: cmp ip, #0
004051b8: add r3, pc, r3
004051bc: bne #0x4051e4
004051c0: ldr ip, [pc, #0x38]
004051c4: ldr r3, [r3, ip]
004051c8: ldrb r3, [r3]
004051cc: cmp r3, #0
004051d0: beq #0x4051d8
004051d4: pop {r4, pc}
004051d8: ldrb r3, [r0, #8]
004051dc: cmp r3, #0
004051e0: bne #0x4051d4
004051e4: ldr r3, [r0, #4]
004051e8: mov r0, r3
004051ec: ldr r3, [r3]
004051f0: mov lr, pc
004051f4: ldr pc, [r3, #8]
004051f8: pop {r4, pc}
004051fc: ldrsbeq pc, [r8], #-0x88
00405200: andeq r3, r0, r0, asr r6

# 0x405260 _ZN12v2Controller10Cmd_LookAtERK7Point3DIfE
00405260: push {r4, lr}
00405264: ldrb r2, [r0, #9]
00405268: ldr r3, [pc, #0x44]
0040526c: cmp r2, #0
00405270: add r3, pc, r3
00405274: bne #0x40529c
00405278: ldr r2, [pc, #0x38]
0040527c: ldr r3, [r3, r2]
00405280: ldrb r3, [r3]
00405284: cmp r3, #0
00405288: beq #0x405290
0040528c: pop {r4, pc}
00405290: ldrb r3, [r0, #8]
00405294: cmp r3, #0
00405298: bne #0x40528c
0040529c: ldr r3, [r0, #4]
004052a0: mov r0, r3
004052a4: ldr r3, [r3]
004052a8: mov lr, pc
004052ac: ldr pc, [r3, #0x10]
004052b0: pop {r4, pc}
004052b4: subseq pc, r8, r0, lsr #16
004052b8: andeq r3, r0, r0, asr r6

# 0x405318 _ZN12v2Controller10Cmd_WarpToERK7Point3DIfE
00405318: push {r4, lr}
0040531c: ldrb r2, [r0, #9]
00405320: ldr r3, [pc, #0x44]
00405324: cmp r2, #0
00405328: add r3, pc, r3
0040532c: bne #0x405354
00405330: ldr r2, [pc, #0x38]
00405334: ldr r3, [r3, r2]
00405338: ldrb r3, [r3]
0040533c: cmp r3, #0
00405340: beq #0x405348
00405344: pop {r4, pc}
00405348: ldrb r3, [r0, #8]
0040534c: cmp r3, #0
00405350: bne #0x405344
00405354: ldr r3, [r0, #4]
00405358: mov r0, r3
0040535c: ldr r3, [r3]
00405360: mov lr, pc
00405364: ldr pc, [r3, #0x18]
00405368: pop {r4, pc}
0040536c: subseq pc, r8, r8, ror #14
00405370: andeq r3, r0, r0, asr r6

# 0x4054e4 _ZN12v2Controller10Cmd_MoveToERK7Point3DIfE
004054e4: push {r4, lr}
004054e8: ldrb r2, [r0, #9]
004054ec: ldr r3, [pc, #0x44]
004054f0: cmp r2, #0
004054f4: add r3, pc, r3
004054f8: bne #0x405520
004054fc: ldr r2, [pc, #0x38]
00405500: ldr r3, [r3, r2]
00405504: ldrb r3, [r3]
00405508: cmp r3, #0
0040550c: beq #0x405514
00405510: pop {r4, pc}
00405514: ldrb r3, [r0, #8]
00405518: cmp r3, #0
0040551c: bne #0x405510
00405520: ldr r3, [r0, #4]
00405524: mov r0, r3
00405528: ldr r3, [r3]
0040552c: mov lr, pc
00405530: ldr pc, [r3, #0x2c]
00405534: pop {r4, pc}

# 0x4057fc _ZN12v2Controller10Cmd_UseOOIEP10GameObject
004057fc: push {r4, r5, r6, r7, r8, lr}
00405800: ldrb r2, [r0, #9]
00405804: ldr r3, [pc, #0x13c]
00405808: mov r4, r0
0040580c: cmp r2, #0
00405810: mov r5, r1
00405814: add r3, pc, r3
00405818: bne #0x405840
0040581c: ldr r2, [pc, #0x128]
00405820: ldr r3, [r3, r2]
00405824: ldrb r3, [r3]
00405828: cmp r3, #0
0040582c: beq #0x405834
00405830: pop {r4, r5, r6, r7, r8, pc}
00405834: ldrb r3, [r0, #8]
00405838: cmp r3, #0
0040583c: bne #0x405830
00405840: bl #0x7fd794
00405844: ldrb r3, [r0, #5]
00405848: cmp r3, #0
0040584c: beq #0x405904
00405850: ldrb r3, [r4, #0xa]
00405854: cmp r3, #0
00405858: beq #0x405904
0040585c: ldr r7, [r4, #0xc]
00405860: cmp r7, #0
00405864: beq #0x405904
00405868: cmp r5, #0
0040586c: movne r6, r5
00405870: beq #0x405934
00405874: ldr r3, [r6]
00405878: mov r0, r6
0040587c: mov lr, pc
00405880: ldr pc, [r3, #0x24]
00405884: cmp r0, #0
00405888: bne #0x405920
0040588c: ldr r3, [r6]
00405890: mov r0, r6
00405894: ldr r1, [r4, #0xc]
00405898: mov lr, pc
0040589c: ldr pc, [r3, #0x90]
004058a0: cmp r0, #1
004058a4: beq #0x405904
004058a8: ldr r3, [r6]
004058ac: mov r0, r6
004058b0: ldr r1, [r4, #0xc]
004058b4: mov lr, pc
004058b8: ldr pc, [r3, #0x90]
004058bc: cmp r0, #8
004058c0: beq #0x405904
004058c4: bl #0x80b1bc
004058c8: mov r8, r0
004058cc: ldr r0, [pc, #0x7c]
004058d0: mov r1, #1
004058d4: ldr r6, [r6, #0x108]
004058d8: add r0, pc, r0
004058dc: ldrb r7, [r7, #0x108]
004058e0: bl #0x80a244
004058e4: uxth r6, r6
004058e8: mov r3, #5
004058ec: mov r1, r0
004058f0: strb r7, [r0, #0x54]
004058f4: strb r3, [r0, #0x50]
004058f8: strh r6, [r0, #0x52]
004058fc: mov r0, r8
00405900: bl #0x80e2a4
00405904: ldr r3, [r4, #4]
00405908: mov r1, r5
0040590c: mov r0, r3
00405910: ldr r3, [r3]
00405914: mov lr, pc
00405918: ldr pc, [r3, #0x50]
0040591c: pop {r4, r5, r6, r7, r8, pc}
00405920: mov r0, r6
00405924: bl #0x3a30c4
00405928: cmp r0, #0
0040592c: bne #0x405904
00405930: b #0x40588c
00405934: movw r3, #0x14a4
00405938: ldr r6, [r7, r3]
0040593c: cmp r6, #0
00405940: beq #0x405904
00405944: b #0x405874
00405948: subseq pc, r8, ip, ror r2
0040594c: andeq r3, r0, r0, asr r6
00405950: subeq sb, fp, r0, lsr r6

# 0x405f48 _ZN15v2EmuController8_onEventEPK13EvMouseButtonPK12EventManager
00405f48: push {r4, r5, r6, r7, r8, lr}
00405f4c: ldr r3, [r1, #8]
00405f50: ldr r7, [pc, #0x160]
00405f54: sub sp, sp, #0x40
00405f58: cmp r3, #0
00405f5c: mov r8, r1
00405f60: mov r4, r0
00405f64: add r7, pc, r7
00405f68: bne #0x405fe4
00405f6c: ldrb r3, [r1, #0xc]
00405f70: cmp r3, #0
00405f74: bne #0x405fe4
00405f78: ldr r3, [r0, #0x18]
00405f7c: and r2, r3, #3
00405f80: cmp r2, #3
00405f84: beq #0x40604c
00405f88: tst r3, #1
00405f8c: bne #0x405ff0
00405f90: tst r3, #2
00405f94: beq #0x405fe4
00405f98: ldrsh r0, [r1, #0x10]
00405f9c: mov r3, #0
00405fa0: str r3, [sp, #0xc]
00405fa4: str r3, [sp, #4]
00405fa8: str r3, [sp, #8]
00405fac: bl #0x30e964
00405fb0: mov r6, r0
00405fb4: ldrsh r0, [r8, #0xe]
00405fb8: bl #0x30e964
00405fbc: ldr r3, [pc, #0xf8]
00405fc0: add r5, sp, #4
00405fc4: str r0, [sp, #0x28]
00405fc8: add r1, sp, #0x28
00405fcc: ldr r0, [r7, r3]
00405fd0: mov r2, r5
00405fd4: str r6, [sp, #0x2c]
00405fd8: bl #0x525884
00405fdc: cmp r0, #0
00405fe0: bne #0x4060a8
00405fe4: mov r0, #0
00405fe8: add sp, sp, #0x40
00405fec: pop {r4, r5, r6, r7, r8, pc}
00405ff0: ldrsh r0, [r1, #0x10]
00405ff4: mov r3, #0
00405ff8: str r3, [sp, #0x18]
00405ffc: str r3, [sp, #0x10]
00406000: str r3, [sp, #0x14]
00406004: bl #0x30e964
00406008: mov r6, r0
0040600c: ldrsh r0, [r8, #0xe]
00406010: bl #0x30e964
00406014: ldr r3, [pc, #0xa0]
00406018: add r5, sp, #0x10
0040601c: str r0, [sp, #0x30]
00406020: add r1, sp, #0x30
00406024: ldr r0, [r7, r3]
00406028: mov r2, r5
0040602c: str r6, [sp, #0x34]
00406030: bl #0x525884
00406034: cmp r0, #0
00406038: beq #0x405fe4
0040603c: mov r0, r4
00406040: mov r1, r5
00406044: bl #0x405318
00406048: b #0x405fe4
0040604c: ldrsh r0, [r1, #0x10]
00406050: mov r3, #0
00406054: str r3, [sp, #0x24]
00406058: str r3, [sp, #0x1c]
0040605c: str r3, [sp, #0x20]
00406060: bl #0x30e964
00406064: mov r6, r0
00406068: ldrsh r0, [r8, #0xe]
0040606c: bl #0x30e964
00406070: ldr r3, [pc, #0x44]
00406074: add r5, sp, #0x1c
00406078: str r0, [sp, #0x38]
0040607c: add r1, sp, #0x38
00406080: ldr r0, [r7, r3]
00406084: mov r2, r5
00406088: str r6, [sp, #0x3c]
0040608c: bl #0x525884
00406090: cmp r0, #0
00406094: beq #0x405fe4
00406098: mov r0, r4
0040609c: mov r1, r5
004060a0: bl #0x4054e4
004060a4: b #0x405fe4
004060a8: mov r0, r4
004060ac: mov r1, r5
004060b0: bl #0x405260
004060b4: b #0x405fe4
004060b8: subseq lr, r8, ip, lsr #22
004060bc: andeq r1, r0, r4, lsl #4

# 0x408b80 _ZThn16_N17v2MixedController11Ctrl_UseOOIEP10GameObject
00408b80: sub r0, r0, #0x10
00408b84: b #0x408b88

# 0x408b88 _ZN17v2MixedController11Ctrl_UseOOIEP10GameObject
00408b88: b #0x4057fc

# 0x408c60 _ZThn16_N17v2MixedController10Ctrl_ClickERK7Point3DIfEb
00408c60: sub r0, r0, #0x10
00408c64: b #0x408c68

# 0x408c68 _ZN17v2MixedController10Ctrl_ClickERK7Point3DIfEb
00408c68: b #0x4051a8

# 0x4119c4 _ZN12CameraTarget9SetTargetEP10GameObjecti
004119c4: ldr r3, [pc, #0xa0]
004119c8: push {r4, r5, r6, lr}
004119cc: subs r5, r1, #0
004119d0: mov r4, r0
004119d4: add r3, pc, r3
004119d8: mov r6, r2
004119dc: beq #0x411a2c
004119e0: cmp r2, #0
004119e4: ble #0x411a30
004119e8: ldr r0, [r0, #0xc]
004119ec: cmp r0, #0
004119f0: beq #0x411a60
004119f4: bl #0x3943b8
004119f8: ldr r3, [r0]
004119fc: str r3, [r4, #0x10]
00411a00: ldr r3, [r0, #4]
00411a04: str r3, [r4, #0x14]
00411a08: ldr r3, [r0, #8]
00411a0c: str r6, [r4, #0x20]
00411a10: str r6, [r4, #0x1c]
00411a14: str r3, [r4, #0x18]
00411a18: mov r3, #0
00411a1c: str r5, [r4, #0xc]
00411a20: str r3, [r4, #0x40]
00411a24: str r3, [r4, #0x38]
00411a28: str r3, [r4, #0x3c]
00411a2c: pop {r4, r5, r6, pc}
00411a30: ldr r1, [pc, #0x38]
00411a34: mov r2, #0
00411a38: ldr r3, [r3, r1]
00411a3c: ldr r1, [r3]
00411a40: str r1, [r0, #0x10]
00411a44: ldr r1, [r3, #4]
00411a48: str r1, [r0, #0x14]
00411a4c: ldr r3, [r3, #8]
00411a50: str r2, [r0, #0x20]
00411a54: str r2, [r0, #0x1c]
00411a58: str r3, [r0, #0x18]
00411a5c: b #0x411a18
00411a60: ldr r2, [pc, #8]
00411a64: ldr r0, [r3, r2]
00411a68: b #0x4119f8
00411a6c: ldrheq r3, [r8], #-0xc
00411a70: andeq r3, r0, ip, lsr #30

# 0x411b38 _ZN12CameraTarget9SetTargetEPKci
00411b38: push {r4, r5, r6, r7, lr}
00411b3c: ldr ip, [pc, #0x70]
00411b40: mov r3, r1
00411b44: ldr r1, [pc, #0x6c]
00411b48: add ip, pc, ip
00411b4c: sub sp, sp, #0x1c
00411b50: ldr r1, [ip, r1]
00411b54: add r5, sp, #0xc
00411b58: mov r4, #0
00411b5c: ldr r1, [r1, #0x38]
00411b60: mov r7, r0
00411b64: mov r6, r2
00411b68: mov r0, r5
00411b6c: mov r2, r3
00411b70: mvn r3, #0
00411b74: str r4, [sp]
00411b78: str r4, [sp, #4]
00411b7c: bl #0x34aca0
00411b80: mov r0, r5
00411b84: mov r1, r4
00411b88: bl #0x33fdc0
00411b8c: cmp r0, r4
00411b90: beq #0x411bac
00411b94: mov r0, r5
00411b98: bl #0x33fee4
00411b9c: mov r2, r6
00411ba0: mov r1, r0
00411ba4: mov r0, r7
00411ba8: bl #0x4119c4
00411bac: add sp, sp, #0x1c
00411bb0: pop {r4, r5, r6, r7, pc}
00411bb4: subseq r2, r8, r8, asr #30
00411bb8: strdeq r3, r4, [r0], -r4

# 0x418bec _ZN11HUDControls5TouchEv
00418bec: ldr r3, [pc, #0x4c]
00418bf0: ldr r2, [pc, #0x4c]
00418bf4: push {r4, lr}
00418bf8: add r3, pc, r3
00418bfc: ldr r2, [r3, r2]
00418c00: mov r1, #0
00418c04: str r1, [r0, #0x18]
00418c08: strb r1, [r0, #8]
00418c0c: strb r1, [r0, #9]
00418c10: strb r1, [r0, #0xa]
00418c14: str r1, [r0, #0x14]
00418c18: ldr r0, [r2, #0x40]
00418c1c: mov r2, r1
00418c20: bl #0x36e478
00418c24: ldr r3, [r0, #0x660]
00418c28: cmp r3, #0
00418c2c: beq #0x418c3c
00418c30: ldr r0, [r3, #0x378]
00418c34: pop {r4, lr}
00418c38: b #0x40559c
00418c3c: pop {r4, pc}

# 0x41a780 _ZN11HUDControls6UpdateEv
0041a780: push {r4, r5, r6, r7, r8, sb, sl, lr}
0041a784: ldr r4, [pc, #0x25c]
0041a788: ldr r3, [pc, #0x25c]
0041a78c: sub sp, sp, #0x20
0041a790: add r4, pc, r4
0041a794: ldr r3, [r4, r3]
0041a798: mov r5, r0
0041a79c: ldrb r3, [r3, #0x30]
0041a7a0: cmp r3, #0
0041a7a4: beq #0x41a910
0041a7a8: ldr r3, [pc, #0x240]
0041a7ac: mov r2, #1
0041a7b0: ldr r3, [r4, r3]
0041a7b4: strb r2, [r3]
0041a7b8: ldr r6, [pc, #0x234]
0041a7bc: mov r3, #0
0041a7c0: strb r3, [r5, #0x84]
0041a7c4: ldr r0, [r4, r6]
0041a7c8: bl #0x31f594
0041a7cc: cmp r0, #0
0041a7d0: beq #0x41a7e0
0041a7d4: ldrb r3, [r0, #0x198]
0041a7d8: cmp r3, #0
0041a7dc: beq #0x41a920
0041a7e0: ldr r3, [r5, #0x658]
0041a7e4: cmp r3, #0
0041a7e8: beq #0x41a908
0041a7ec: ldrb r3, [r5, #8]
0041a7f0: cmp r3, #0
0041a7f4: beq #0x41a940
0041a7f8: ldr r3, [r4, r6]
0041a7fc: mov r1, #0
0041a800: mov r2, r1
0041a804: ldr r0, [r3, #0x40]
0041a808: bl #0x36e478
0041a80c: ldr r6, [r0, #0x660]
0041a810: cmp r6, #0
0041a814: beq #0x41a908
0041a818: ldrb r3, [r5, #9]
0041a81c: cmp r3, #0
0041a820: beq #0x41a840
0041a824: movw r3, #0x14a4
0041a828: ldr r1, [r6, r3]
0041a82c: cmp r1, #0
0041a830: beq #0x41a9d8
0041a834: ldr r0, [r6, #0x378]
0041a838: mov r1, #0
0041a83c: bl #0x4057fc
0041a840: ldrb r3, [r5, #0xa]
0041a844: cmp r3, #0
0041a848: bne #0x41a978
0041a84c: ldrb r7, [r5, #9]
0041a850: cmp r7, #0
0041a854: bne #0x41a908
0041a858: ldr r0, [r5, #0x7c]
0041a85c: cmp r0, #0
0041a860: ble #0x41a908
0041a864: ldr r5, [r5, #0x80]
0041a868: cmp r5, #0
0041a86c: ble #0x41a908
0041a870: bl #0x30e964
0041a874: mov r8, r0
0041a878: mov r0, r5
0041a87c: bl #0x30e964
0041a880: ldr r2, [pc, #0x170]
0041a884: mov r3, #0
0041a888: str r0, [sp, #0x1c]
0041a88c: add r1, sp, #0x18
0041a890: ldr r0, [r4, r2]
0041a894: mov r2, sp
0041a898: str r3, [sp, #8]
0041a89c: str r8, [sp, #0x18]
0041a8a0: str r3, [sp]
0041a8a4: str r3, [sp, #4]
0041a8a8: bl #0x525884
0041a8ac: cmp r0, #0
0041a8b0: mov r5, sp
0041a8b4: beq #0x41a908
0041a8b8: movw r4, #0x149c
0041a8bc: ldr r0, [r6, r4]
0041a8c0: ldr r1, [sp]
0041a8c4: ldr r2, [sp, #4]
0041a8c8: cmp r0, #0
0041a8cc: ldr r3, [sp, #8]
0041a8d0: beq #0x41a8fc
0041a8d4: str r1, [r0, #0x34]
0041a8d8: str r2, [r0, #0x38]
0041a8dc: str r3, [r0, #0x3c]
0041a8e0: mov r1, r7
0041a8e4: bl #0x492aa0
0041a8e8: ldr r0, [r6, r4]
0041a8ec: cmp r0, #0
0041a8f0: beq #0x41a8fc
0041a8f4: mov r1, #1
0041a8f8: bl #0x492ef0
0041a8fc: ldr r0, [r6, #0x378]
0041a900: mov r1, sp
0041a904: bl #0x4054e4
0041a908: add sp, sp, #0x20
0041a90c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0041a910: ldr r2, [pc, #0xd8]
0041a914: ldr r2, [r4, r2]
0041a918: strb r3, [r2]
0041a91c: b #0x41a7b8
0041a920: ldrb r2, [r5, #9]
0041a924: cmp r2, #0
0041a928: strbne r3, [r5, #9]
0041a92c: ldrb r3, [r5, #0xa]
0041a930: cmp r3, #0
0041a934: movne r3, #0
0041a938: strbne r3, [r5, #0xa]
0041a93c: b #0x41a908
0041a940: mov r0, r5
0041a944: bl #0x419b4c
0041a948: mvn r3, #0
0041a94c: str r3, [r5, #0x80]
0041a950: str r3, [r5, #0x7c]
0041a954: ldr r3, [r4, r6]
0041a958: mov r1, #0
0041a95c: mov r2, r1
0041a960: ldr r0, [r3, #0x40]
0041a964: bl #0x36e478
0041a968: ldr r6, [r0, #0x660]
0041a96c: cmp r6, #0
0041a970: bne #0x41a818
0041a974: b #0x41a908
0041a978: mov r0, r6
0041a97c: bl #0x3ad430
0041a980: cmp r0, #0
0041a984: beq #0x41a84c
0041a988: ldr r7, [r5, #0x668]
0041a98c: ldr r1, [r5, #0x660]
0041a990: ldr sb, [r6, #0x378]
0041a994: mov r0, r7
0041a998: bl #0x30ed6c
0041a99c: ldr r1, [r5, #0x664]
0041a9a0: mov sl, r0
0041a9a4: mov r0, r7
0041a9a8: bl #0x30ed6c
0041a9ac: mov r1, r7
0041a9b0: mov r8, r0
0041a9b4: ldr r0, [r5, #0x65c]
0041a9b8: bl #0x30ed6c
0041a9bc: add r1, sp, #0xc
0041a9c0: str r0, [sp, #0xc]
0041a9c4: mov r0, sb
0041a9c8: str sl, [sp, #0x10]
0041a9cc: str r8, [sp, #0x14]
0041a9d0: bl #0x405374
0041a9d4: b #0x41a84c
0041a9d8: strb r1, [r6, #0x413]
0041a9dc: ldr r0, [r6, #0x378]
0041a9e0: bl #0x405b04
0041a9e4: b #0x41a840
0041a9e8: subseq sl, r7, r0, lsl #6
0041a9ec: andeq r1, r0, r0, lsr #20
0041a9f0: strdeq r3, r4, [r0], -r8
0041a9f4: strdeq r3, r4, [r0], -r4
0041a9f8: andeq r1, r0, r4, lsl #4

# 0x43665c _ZNSaINSt4priv13_Rb_tree_nodeISt4pairIKlN12MenuWorldMap12InputHandler9TouchDataEEEEE8allocateEjPKv.clone.4
0043665c: str lr, [sp, #-4]!
00436660: sub sp, sp, #0xc
00436664: add r0, sp, #8
00436668: mov r3, #0x1c
0043666c: str r3, [r0, #-4]!
00436670: bl #0x708ec0
00436674: add sp, sp, #0xc
00436678: ldm sp!, {pc}

# 0x43667c _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKlN12MenuWorldMap12InputHandler9TouchDataEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS8_SG_SG_
0043667c: cmp r1, r2
00436680: push {r4, r5, r6, r7, r8, lr}
00436684: mov r4, r1
00436688: mov r7, r2
0043668c: mov r5, r0
00436690: mov r8, r3
00436694: beq #0x436764
00436698: ldr r3, [sp, #0x1c]
0043669c: cmp r3, #0
004366a0: beq #0x43670c
004366a4: mov r0, r4
004366a8: bl #0x43665c
004366ac: ldr r2, [r8]
004366b0: mov r3, #0
004366b4: mov r6, r0
004366b8: str r2, [r0, #0x10]
004366bc: ldr r2, [r8, #4]
004366c0: str r2, [r0, #0x14]
004366c4: ldr r2, [r8, #8]
004366c8: str r3, [r0, #0xc]
004366cc: str r3, [r0, #8]
004366d0: str r2, [r0, #0x18]
004366d4: str r0, [r7, #0xc]
004366d8: ldr r3, [r4, #0xc]
004366dc: cmp r7, r3
004366e0: beq #0x43675c
004366e4: mov r0, r6
004366e8: str r7, [r6, #4]
004366ec: add r1, r4, #4
004366f0: bl #0x313760
004366f4: ldr r3, [r4, #0x10]
004366f8: mov r0, r5
004366fc: add r3, r3, #1
00436700: str r3, [r4, #0x10]
00436704: str r6, [r5]
00436708: pop {r4, r5, r6, r7, r8, pc}
0043670c: ldr r3, [sp, #0x18]
00436710: cmp r3, #0
00436714: beq #0x4367a4
00436718: mov r0, r4
0043671c: bl #0x43665c
00436720: ldr r2, [r8]
00436724: mov r3, #0
00436728: mov r6, r0
0043672c: str r2, [r0, #0x10]
00436730: ldr r2, [r8, #4]
00436734: str r2, [r0, #0x14]
00436738: ldr r2, [r8, #8]
0043673c: str r3, [r0, #0xc]
00436740: str r3, [r0, #8]
00436744: str r2, [r0, #0x18]
00436748: str r0, [r7, #8]
0043674c: ldr r3, [r4, #8]
00436750: cmp r7, r3
00436754: streq r0, [r4, #8]
00436758: b #0x4366e4
0043675c: str r6, [r4, #0xc]
00436760: b #0x4366e4
00436764: mov r0, r1
00436768: bl #0x43665c
0043676c: ldr r2, [r8]
00436770: mov r3, #0
00436774: mov r6, r0
00436778: str r2, [r0, #0x10]
0043677c: ldr r2, [r8, #4]
00436780: str r2, [r0, #0x14]
00436784: ldr r2, [r8, #8]
00436788: str r3, [r0, #0xc]
0043678c: str r3, [r0, #8]
00436790: str r2, [r0, #0x18]
00436794: str r0, [r4, #8]
00436798: str r0, [r4, #4]
0043679c: str r0, [r4, #0xc]
004367a0: b #0x4366e4
004367a4: ldr r2, [r8]
004367a8: ldr r3, [r7, #0x10]
004367ac: cmp r2, r3
004367b0: bge #0x4366a4
004367b4: b #0x436718

# 0x4367b8 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKlN12MenuWorldMap12InputHandler9TouchDataEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE13insert_uniqueERKS8_
004367b8: push {r4, r5, r6, lr}
004367bc: ldr ip, [r1, #4]
004367c0: sub sp, sp, #0x10
004367c4: mov r4, r0
004367c8: cmp ip, #0
004367cc: mov r3, r2
004367d0: moveq ip, r1
004367d4: beq #0x436830
004367d8: ldr r6, [r2]
004367dc: b #0x4367e4
004367e0: mov ip, r2
004367e4: ldr r0, [ip, #0x10]
004367e8: mov r5, #1
004367ec: cmp r0, r6
004367f0: ldrgt r2, [ip, #8]
004367f4: ldrle r2, [ip, #0xc]
004367f8: movle r5, #0
004367fc: cmp r2, #0
00436800: bne #0x4367e0
00436804: cmp r5, #0
00436808: moveq r5, ip
0043680c: bne #0x436830
00436810: cmp r6, r0
00436814: movle r3, #0
00436818: strle r5, [r4]
0043681c: strble r3, [r4, #4]
00436820: bgt #0x436898
00436824: mov r0, r4
00436828: add sp, sp, #0x10
0043682c: pop {r4, r5, r6, pc}
00436830: ldr r2, [r1, #8]
00436834: cmp ip, r2
00436838: beq #0x436918
0043683c: ldrb r2, [ip]
00436840: cmp r2, #0
00436844: bne #0x436858
00436848: ldr r2, [ip, #4]
0043684c: ldr r2, [r2, #4]
00436850: cmp ip, r2
00436854: beq #0x436904
00436858: ldr r0, [ip, #8]
0043685c: cmp r0, #0
00436860: bne #0x43686c
00436864: b #0x4368c4
00436868: mov r0, r2
0043686c: ldr r2, [r0, #0xc]
00436870: cmp r2, #0
00436874: bne #0x436868
00436878: ldr r6, [r3]
0043687c: mov r5, r0
00436880: ldr r0, [r0, #0x10]
00436884: cmp r6, r0
00436888: movle r3, #0
0043688c: strle r5, [r4]
00436890: strble r3, [r4, #4]
00436894: ble #0x436824
00436898: mov r2, ip
0043689c: add r0, sp, #8
004368a0: mov ip, #0
004368a4: str ip, [sp, #4]
004368a8: str ip, [sp]
004368ac: bl #0x43667c
004368b0: ldr r3, [sp, #8]
004368b4: mov r2, #1
004368b8: strb r2, [r4, #4]
004368bc: str r3, [r4]
004368c0: b #0x436824
004368c4: ldr r2, [ip, #4]
004368c8: ldr r0, [r2, #8]
004368cc: cmp ip, r0
004368d0: movne r5, r2
004368d4: ldrne r6, [r3]
004368d8: ldrne r0, [r2, #0x10]
004368dc: beq #0x4368e8
004368e0: b #0x436810
004368e4: mov r2, r5
004368e8: ldr r5, [r2, #4]
004368ec: ldr r0, [r5, #8]
004368f0: cmp r0, r2
004368f4: beq #0x4368e4
004368f8: ldr r6, [r3]
004368fc: ldr r0, [r5, #0x10]
00436900: b #0x436810
00436904: ldr r2, [ip, #0xc]
00436908: ldr r6, [r3]
0043690c: mov r5, r2
00436910: ldr r0, [r2, #0x10]
00436914: b #0x436810
00436918: mov r2, ip
0043691c: mov lr, #0
00436920: add r0, sp, #0xc
00436924: stm sp, {ip, lr}
00436928: bl #0x43667c
0043692c: ldr r3, [sp, #0xc]
00436930: mov r2, #1
00436934: strb r2, [r4, #4]
00436938: str r3, [r4]
0043693c: b #0x436824

# 0x436940 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKlN12MenuWorldMap12InputHandler9TouchDataEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE13insert_uniqueENS_17_Rb_tree_iteratorIS8_SC_EERKS8_
00436940: push {r4, r5, r6, r7, r8, sl, lr}
00436944: ldr r4, [r2]
00436948: ldr r2, [r1, #8]
0043694c: sub sp, sp, #0x2c
00436950: mov r5, r1
00436954: cmp r4, r2
00436958: mov r7, r0
0043695c: mov r6, r3
00436960: beq #0x436ad0
00436964: cmp r4, r1
00436968: beq #0x436b50
0043696c: ldrb r3, [r4]
00436970: cmp r3, #0
00436974: beq #0x436a64
00436978: ldr ip, [r4, #8]
0043697c: cmp ip, #0
00436980: bne #0x43698c
00436984: b #0x436a84
00436988: mov ip, r3
0043698c: ldr r3, [ip, #0xc]
00436990: cmp r3, #0
00436994: bne #0x436988
00436998: ldr r2, [r6]
0043699c: ldr r0, [r4, #0x10]
004369a0: cmp r2, r0
004369a4: movge r1, #0
004369a8: movlt r1, #1
004369ac: cmp r1, #0
004369b0: bne #0x436a24
004369b4: ldr r8, [r4, #0xc]
004369b8: cmp r8, #0
004369bc: beq #0x436bb8
004369c0: mov ip, r8
004369c4: b #0x4369cc
004369c8: mov ip, r3
004369cc: ldr r3, [ip, #8]
004369d0: cmp r3, #0
004369d4: bne #0x4369c8
004369d8: cmp r1, #0
004369dc: bne #0x436ab4
004369e0: cmp r2, r0
004369e4: ble #0x436b78
004369e8: cmp r5, ip
004369ec: beq #0x4369fc
004369f0: ldr r3, [ip, #0x10]
004369f4: cmp r2, r3
004369f8: bge #0x436ab4
004369fc: cmp r8, #0
00436a00: bne #0x436b30
00436a04: mov r1, r5
00436a08: mov r2, r4
00436a0c: mov r3, r6
00436a10: mov r0, r7
00436a14: str r8, [sp]
00436a18: str r4, [sp, #4]
00436a1c: bl #0x43667c
00436a20: b #0x436a58
00436a24: ldr r3, [ip, #0x10]
00436a28: cmp r2, r3
00436a2c: ble #0x4369b4
00436a30: ldr lr, [ip, #0xc]
00436a34: cmp lr, #0
00436a38: beq #0x436b98
00436a3c: mov ip, #0
00436a40: mov r1, r5
00436a44: mov r2, r4
00436a48: mov r3, r6
00436a4c: mov r0, r7
00436a50: stm sp, {r4, ip}
00436a54: bl #0x43667c
00436a58: mov r0, r7
00436a5c: add sp, sp, #0x2c
00436a60: pop {r4, r5, r6, r7, r8, sl, pc}
00436a64: ldr r3, [r4, #4]
00436a68: ldr r3, [r3, #4]
00436a6c: cmp r4, r3
00436a70: ldreq ip, [r4, #0xc]
00436a74: beq #0x436998
00436a78: ldr ip, [r4, #8]
00436a7c: cmp ip, #0
00436a80: bne #0x43698c
00436a84: ldr ip, [r4, #4]
00436a88: ldr r3, [ip, #8]
00436a8c: cmp r4, r3
00436a90: beq #0x436a9c
00436a94: b #0x436998
00436a98: mov ip, r3
00436a9c: ldr r3, [ip, #4]
00436aa0: ldr r2, [r3, #8]
00436aa4: cmp r2, ip
00436aa8: beq #0x436a98
00436aac: mov ip, r3
00436ab0: b #0x436998
00436ab4: mov r1, r5
00436ab8: mov r2, r6
00436abc: add r0, sp, #8
00436ac0: bl #0x4367b8
00436ac4: ldr r3, [sp, #8]
00436ac8: str r3, [r7]
00436acc: b #0x436a58
00436ad0: ldr r2, [r1, #0x10]
00436ad4: cmp r2, #0
00436ad8: beq #0x436c28
00436adc: ldr r2, [r3]
00436ae0: ldr ip, [r4, #0x10]
00436ae4: cmp r2, ip
00436ae8: blt #0x436c40
00436aec: ble #0x436b78
00436af0: ldr lr, [r4, #0xc]
00436af4: cmp lr, #0
00436af8: beq #0x436bf0
00436afc: mov ip, lr
00436b00: b #0x436b08
00436b04: mov ip, r3
00436b08: ldr r3, [ip, #8]
00436b0c: cmp r3, #0
00436b10: bne #0x436b04
00436b14: cmp r5, ip
00436b18: beq #0x436c90
00436b1c: ldr r3, [ip, #0x10]
00436b20: cmp r2, r3
00436b24: bge #0x436c54
00436b28: cmp lr, #0
00436b2c: beq #0x436c70
00436b30: mov lr, #0
00436b34: mov r1, r5
00436b38: mov r2, ip
00436b3c: mov r3, r6
00436b40: mov r0, r7
00436b44: stm sp, {ip, lr}
00436b48: bl #0x43667c
00436b4c: b #0x436a58
00436b50: ldr r2, [r4, #0xc]
00436b54: ldr ip, [r3]
00436b58: ldr lr, [r2, #0x10]
00436b5c: cmp lr, ip
00436b60: bge #0x436b80
00436b64: mov ip, #0
00436b68: str ip, [sp]
00436b6c: str r4, [sp, #4]
00436b70: bl #0x43667c
00436b74: b #0x436a58
00436b78: str r4, [r7]
00436b7c: b #0x436a58
00436b80: mov r2, r3
00436b84: add r0, sp, #0x10
00436b88: bl #0x4367b8
00436b8c: ldr r3, [sp, #0x10]
00436b90: str r3, [r7]
00436b94: b #0x436a58
00436b98: mov r1, r5
00436b9c: mov r2, ip
00436ba0: mov r3, r6
00436ba4: mov r0, r7
00436ba8: str lr, [sp]
00436bac: str ip, [sp, #4]
00436bb0: bl #0x43667c
00436bb4: b #0x436a58
00436bb8: ldr r3, [r4, #4]
00436bbc: ldr ip, [r3, #0xc]
00436bc0: cmp r4, ip
00436bc4: movne ip, r4
00436bc8: bne #0x436be0
00436bcc: mov ip, r3
00436bd0: ldr r3, [r3, #4]
00436bd4: ldr sl, [r3, #0xc]
00436bd8: cmp ip, sl
00436bdc: beq #0x436bcc
00436be0: ldr sl, [ip, #0xc]
00436be4: cmp r3, sl
00436be8: movne ip, r3
00436bec: b #0x4369d8
00436bf0: ldr r3, [r4, #4]
00436bf4: ldr r1, [r3, #0xc]
00436bf8: cmp r4, r1
00436bfc: movne ip, r4
00436c00: bne #0x436c18
00436c04: mov ip, r3
00436c08: ldr r3, [r3, #4]
00436c0c: ldr r1, [r3, #0xc]
00436c10: cmp r1, ip
00436c14: beq #0x436c04
00436c18: ldr r1, [ip, #0xc]
00436c1c: cmp r3, r1
00436c20: movne ip, r3
00436c24: b #0x436b14
00436c28: mov r2, r3
00436c2c: add r0, sp, #0x20
00436c30: bl #0x4367b8
00436c34: ldr r3, [sp, #0x20]
00436c38: str r3, [r7]
00436c3c: b #0x436a58
00436c40: mov ip, #0
00436c44: mov r2, r4
00436c48: stm sp, {r4, ip}
00436c4c: bl #0x43667c
00436c50: b #0x436a58
00436c54: mov r1, r5
00436c58: mov r2, r6
00436c5c: add r0, sp, #0x18
00436c60: bl #0x4367b8
00436c64: ldr r3, [sp, #0x18]
00436c68: str r3, [r7]
00436c6c: b #0x436a58
00436c70: mov r1, r5
00436c74: mov r2, r4
00436c78: mov r3, r6
00436c7c: mov r0, r7
00436c80: str lr, [sp]
00436c84: str r4, [sp, #4]
00436c88: bl #0x43667c
00436c8c: b #0x436a58
00436c90: mov ip, #0
00436c94: mov r1, r5
00436c98: mov r2, r4
00436c9c: mov r3, r6
00436ca0: mov r0, r7
00436ca4: str ip, [sp]
00436ca8: str r4, [sp, #4]
00436cac: bl #0x43667c
00436cb0: b #0x436a58

# 0x436cb4 _ZNSt3mapIlN12MenuWorldMap12InputHandler9TouchDataESt4lessIlESaISt4pairIKlS2_EEEixIlEERS2_RKT_
00436cb4: push {r4, lr}
00436cb8: ldr ip, [r0, #4]
00436cbc: sub sp, sp, #0x18
00436cc0: cmp ip, #0
00436cc4: beq #0x436d50
00436cc8: ldr r4, [r1]
00436ccc: mov r2, r0
00436cd0: b #0x436cd8
00436cd4: mov ip, r3
00436cd8: ldr r3, [ip, #0x10]
00436cdc: cmp r4, r3
00436ce0: ldrgt r3, [ip, #0xc]
00436ce4: ldrle r3, [ip, #8]
00436ce8: movgt ip, r2
00436cec: mov r2, ip
00436cf0: cmp r3, #0
00436cf4: bne #0x436cd4
00436cf8: cmp r0, ip
00436cfc: beq #0x436d10
00436d00: ldr r2, [ip, #0x10]
00436d04: mov r3, ip
00436d08: cmp r2, r4
00436d0c: ble #0x436d44
00436d10: mov r1, r0
00436d14: add r3, sp, #4
00436d18: str ip, [sp, #0x10]
00436d1c: add r0, sp, #0x14
00436d20: mov ip, #0
00436d24: add r2, sp, #0x10
00436d28: str r4, [sp, #4]
00436d2c: strh ip, [sp, #0xe]
00436d30: strh ip, [sp, #0xc]
00436d34: strh ip, [sp, #0xa]
00436d38: strh ip, [sp, #8]
00436d3c: bl #0x436940
00436d40: ldr r3, [sp, #0x14]
00436d44: add r0, r3, #0x14
00436d48: add sp, sp, #0x18
00436d4c: pop {r4, pc}
00436d50: ldr r4, [r1]
00436d54: mov ip, r0
00436d58: b #0x436cf8

# 0x436d5c _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKlN12MenuWorldMap12InputHandler9TouchDataEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00436d5c: push {r4, r5, r6, lr}
00436d60: subs r4, r1, #0
00436d64: mov r6, r0
00436d68: beq #0x436d90
00436d6c: ldr r1, [r4, #0xc]
00436d70: mov r0, r6
00436d74: bl #0x436d5c
00436d78: ldr r5, [r4, #8]
00436d7c: mov r0, r4
00436d80: mov r1, #0x1c
00436d84: bl #0x708f00
00436d88: subs r4, r5, #0
00436d8c: bne #0x436d6c
00436d90: pop {r4, r5, r6, pc}

# 0x436e04 _ZNSt4priv8_Rb_treeIlSt4lessIlESt4pairIKlN12MenuWorldMap12InputHandler9TouchDataEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE5eraseENS_17_Rb_tree_iteratorIS8_SC_EE
00436e04: push {r4, lr}
00436e08: mov r4, r0
00436e0c: add r2, r4, #8
00436e10: ldr r0, [r1]
00436e14: add r3, r4, #0xc
00436e18: add r1, r4, #4
00436e1c: bl #0x336004
00436e20: cmp r0, #0
00436e24: beq #0x436e30
00436e28: mov r1, #0x1c
00436e2c: bl #0x708f00
00436e30: ldr r3, [r4, #0x10]
00436e34: sub r3, r3, #1
00436e38: str r3, [r4, #0x10]
00436e3c: pop {r4, pc}

# 0x439ca4 _Z17NativeTouchToMoveRKN7gameswf7fn_callE
00439ca4: bx lr

# 0x47a898 _ZNK30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableE12GetPositionsER13Vector3DFList
0047a898: bx lr

# 0x47d0e4 _Z24GetObjectiveImplInstanceI30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableEEP9Objectivev
0047d0e4: push {r4, r5, r6, lr}
0047d0e8: mov r1, #0
0047d0ec: mov r0, #0x28
0047d0f0: bl #0x310570
0047d0f4: ldr r5, [pc, #0x38]
0047d0f8: mov r4, r0
0047d0fc: bl #0x47be00
0047d100: ldr r3, [pc, #0x30]
0047d104: add r5, pc, r5
0047d108: mov r2, #0
0047d10c: ldr r3, [r5, r3]
0047d110: str r2, [r4, #0x20]
0047d114: mov r0, r4
0047d118: add r2, r3, #0x4c
0047d11c: add r3, r3, #8
0047d120: str r3, [r4]
0047d124: mvn r3, #0
0047d128: str r2, [r4, #0x18]
0047d12c: str r3, [r4, #0x24]
0047d130: pop {r4, r5, r6, pc}
0047d134: subseq r7, r1, ip, lsl #19
0047d138: andeq r3, r0, r8, asr pc

# 0x47d37c _ZThn24_N30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableED1Ev
0047d37c: sub r0, r0, #0x18
0047d380: b #0x47d384

# 0x47d384 _ZN30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableED1Ev
0047d384: ldr r3, [pc, #0x34]
0047d388: ldr r1, [pc, #0x34]
0047d38c: ldr r2, [pc, #0x34]
0047d390: add r3, pc, r3
0047d394: ldr r1, [r3, r1]
0047d398: ldr r2, [r3, r2]
0047d39c: push {r4, lr}
0047d3a0: add r1, r1, #8
0047d3a4: add r2, r2, #8
0047d3a8: mov r4, r0
0047d3ac: str r1, [r0]
0047d3b0: str r2, [r0, #0x18]
0047d3b4: bl #0x47a1e4
0047d3b8: mov r0, r4
0047d3bc: pop {r4, pc}
0047d3c0: subseq r7, r1, r0, lsl #14
0047d3c4: muleq r0, r0, sl
0047d3c8: andeq r0, r0, r0, asr #22

# 0x47d730 _ZThn24_N30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableED0Ev
0047d730: sub r0, r0, #0x18
0047d734: b #0x47d738

# 0x47d738 _ZN30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableED0Ev
0047d738: ldr r3, [pc, #0x3c]
0047d73c: ldr r1, [pc, #0x3c]
0047d740: ldr r2, [pc, #0x3c]
0047d744: add r3, pc, r3
0047d748: ldr r1, [r3, r1]
0047d74c: ldr r2, [r3, r2]
0047d750: push {r4, lr}
0047d754: add r1, r1, #8
0047d758: add r2, r2, #8
0047d75c: mov r4, r0
0047d760: str r1, [r0]
0047d764: str r2, [r0, #0x18]
0047d768: bl #0x47a1e4
0047d76c: mov r0, r4
0047d770: bl #0x310440
0047d774: mov r0, r4
0047d778: pop {r4, pc}
0047d77c: subseq r7, r1, ip, asr #6
0047d780: muleq r0, r0, sl
0047d784: andeq r0, r0, r0, asr #22

# 0x47df64 _ZNK30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableE37DBG_TraceDetailedObjectiveInformationEP7__sFILE
0047df64: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0047df68: mov r7, r0
0047df6c: ldr r0, [pc, #0x1b8]
0047df70: sub sp, sp, #0xc
0047df74: mov r3, r1
0047df78: mov r5, r1
0047df7c: mov r2, #0x1a
0047df80: mov r1, #1
0047df84: add r0, pc, r0
0047df88: ldr r4, [pc, #0x1a0]
0047df8c: ldr r6, [r7, #0xc]
0047df90: bl #0x30e598
0047df94: ldr r3, [pc, #0x198]
0047df98: add r4, pc, r4
0047df9c: ldr r1, [pc, #0x194]
0047dfa0: ldr r8, [r4, r3]
0047dfa4: ldr r2, [r6, #4]
0047dfa8: add r1, pc, r1
0047dfac: ldr r0, [r8, #0x2c]
0047dfb0: bl #0x4c4b08
0047dfb4: ldr r1, [pc, #0x180]
0047dfb8: mov r2, r0
0047dfbc: mov r0, r5
0047dfc0: add r1, pc, r1
0047dfc4: bl #0x30e004
0047dfc8: ldr r3, [r6, #4]
0047dfcc: cmp r3, #5
0047dfd0: beq #0x47e048
0047dfd4: ldr r1, [pc, #0x164]
0047dfd8: mov r0, r5
0047dfdc: ldr r2, [r6, #0x20]
0047dfe0: add r1, pc, r1
0047dfe4: bl #0x30e004
0047dfe8: ldr r3, [r6, #0x24]
0047dfec: cmp r3, #0
0047dff0: blt #0x47e01c
0047dff4: ldr r2, [pc, #0x148]
0047dff8: ldr r2, [r4, r2]
0047dffc: ldr r2, [r2]
0047e000: cmp r3, r2
0047e004: bhs #0x47e01c
0047e008: ldr r2, [pc, #0x138]
0047e00c: ldr r2, [r4, r2]
0047e010: ldr r2, [r2]
0047e014: ldr r2, [r2, r3, lsl #2]
0047e018: b #0x47e024
0047e01c: ldr r2, [pc, #0x128]
0047e020: add r2, pc, r2
0047e024: ldr r1, [pc, #0x124]
0047e028: mov r0, r5
0047e02c: add r1, pc, r1
0047e030: bl #0x30e004
0047e034: mov r0, r7
0047e038: mov r1, r5
0047e03c: add sp, sp, #0xc
0047e040: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0047e044: b #0x47aab4
0047e048: ldr sb, [r8, #0x38]
0047e04c: ldr fp, [r6, #0x20]
0047e050: ldr r8, [sb, #0x60]!
0047e054: cmp sb, r8
0047e058: beq #0x47e07c
0047e05c: ldr sl, [r8, #8]
0047e060: mov r0, sl
0047e064: bl #0x3b3d38
0047e068: cmp fp, r0
0047e06c: beq #0x47e0c0
0047e070: ldr r8, [r8]
0047e074: cmp sb, r8
0047e078: bne #0x47e05c
0047e07c: ldr r3, [r6, #0x20]
0047e080: cmp r3, #0
0047e084: blt #0x47e120
0047e088: ldr r2, [pc, #0xc4]
0047e08c: ldr r2, [r4, r2]
0047e090: ldr r2, [r2]
0047e094: cmp r3, r2
0047e098: bhs #0x47e120
0047e09c: ldr r2, [pc, #0xb4]
0047e0a0: ldr r2, [r4, r2]
0047e0a4: ldr r2, [r2]
0047e0a8: ldr r2, [r2, r3, lsl #2]
0047e0ac: ldr r1, [pc, #0xa8]
0047e0b0: mov r0, r5
0047e0b4: add r1, pc, r1
0047e0b8: bl #0x30e004
0047e0bc: b #0x47dfe8
0047e0c0: cmp sl, #0
0047e0c4: beq #0x47e07c
0047e0c8: ldr r3, [r6, #0x20]
0047e0cc: cmp r3, #0
0047e0d0: blt #0x47e114
0047e0d4: ldr r2, [pc, #0x78]
0047e0d8: ldr r2, [r4, r2]
0047e0dc: ldr r2, [r2]
0047e0e0: cmp r3, r2
0047e0e4: bhs #0x47e114
0047e0e8: ldr r2, [pc, #0x68]
0047e0ec: ldr r2, [r4, r2]
0047e0f0: ldr r2, [r2]
0047e0f4: ldr r2, [r2, r3, lsl #2]
0047e0f8: ldr r1, [pc, #0x60]
0047e0fc: ldr ip, [sl, #0x44]
0047e100: mov r0, r5
0047e104: add r1, pc, r1
0047e108: str ip, [sp]
0047e10c: bl #0x30e004
0047e110: b #0x47dfe8
0047e114: ldr r2, [pc, #0x48]
0047e118: add r2, pc, r2
0047e11c: b #0x47e0f8
0047e120: ldr r2, [pc, #0x40]
0047e124: add r2, pc, r2
0047e128: b #0x47e0ac
0047e12c: strheq pc, [r4], #-0xe4
0047e130: ldrsheq r6, [r1], #-0xa8
0047e134: strdeq r3, r4, [r0], -r4
0047e138: subeq r4, r4, r0, asr #19
0047e13c: subeq pc, r4, r8, asr #27
0047e140: subeq pc, r4, r0, asr #27
0047e144: andeq r1, r0, r0, asr #17
0047e148: andeq r3, r0, ip, asr fp
0047e14c: strdeq r1, r2, [r4], #-0x70
0047e150: subeq pc, r4, ip, lsl #27
0047e154: andeq r4, r0, r4, lsl #4
0047e158: andeq r3, r0, r8, lsl #24
0047e15c: subeq pc, r4, ip, asr #27
0047e160: subeq pc, r4, r4, asr sp
0047e164: strdeq r1, r2, [r4], #-0x68
0047e168: subeq r1, r4, ip, ror #13

# 0x47eb90 _ZN30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableE7CompileEv
0047eb90: push {r4, lr}
0047eb94: ldr r2, [r0, #0xc]
0047eb98: mov r4, r0
0047eb9c: ldr r3, [r2, #0x20]
0047eba0: cmn r3, #1
0047eba4: str r3, [r0, #0x24]
0047eba8: beq #0x47ebc4
0047ebac: mov r3, #1
0047ebb0: strb r3, [r0, #8]
0047ebb4: ldr r3, [r2, #0x28]
0047ebb8: ldr r2, [r0, #0x20]
0047ebbc: cmp r2, r3
0047ebc0: bge #0x47ebc8
0047ebc4: pop {r4, pc}
0047ebc8: bl #0x47ba10
0047ebcc: mov r0, r4
0047ebd0: ldr r3, [r4]
0047ebd4: mov lr, pc
0047ebd8: ldr pc, [r3, #0x1c]
0047ebdc: pop {r4, pc}

# 0x47f5a0 _ZN30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableE11handleEventEPK6IEventPK12EventManager
0047f5a0: push {r4, lr}
0047f5a4: ldr r3, [r1, #0x18]
0047f5a8: ldr r2, [r0, #0x24]
0047f5ac: mov r4, r0
0047f5b0: ldr r0, [r0, #0xc]
0047f5b4: cmp r2, r3
0047f5b8: beq #0x47f5c4
0047f5bc: mov r0, #0
0047f5c0: pop {r4, pc}
0047f5c4: ldrb r3, [r1, #0x11]
0047f5c8: cmp r3, #0
0047f5cc: bne #0x47f61c
0047f5d0: ldr r3, [r4, #0x20]
0047f5d4: add r3, r3, #1
0047f5d8: str r3, [r4, #0x20]
0047f5dc: mov r3, #1
0047f5e0: strb r3, [r1, #0x10]
0047f5e4: ldr r3, [r4, #0x20]
0047f5e8: str r3, [r1, #0x14]
0047f5ec: ldr r3, [r4, #0x20]
0047f5f0: ldr r2, [r0, #0x28]
0047f5f4: cmp r2, r3
0047f5f8: bgt #0x47f5bc
0047f5fc: mov r0, r4
0047f600: bl #0x47ba10
0047f604: mov r0, r4
0047f608: ldr r3, [r4]
0047f60c: mov lr, pc
0047f610: ldr pc, [r3, #0x1c]
0047f614: mov r0, #0
0047f618: pop {r4, pc}
0047f61c: ldr r3, [r1, #0x14]
0047f620: ldr r2, [r4, #0x20]
0047f624: cmp r2, r3
0047f628: strlt r3, [r4, #0x20]
0047f62c: blt #0x47f5f0
0047f630: b #0x47f5bc

# 0x4cf570 _ZN7Structs23v2QuestPickedUpLiftable8finalizeEv
004cf570: push {r4, lr}
004cf574: mov r4, r0
004cf578: ldr r0, [r0, #0x14]
004cf57c: cmp r0, #0
004cf580: beq #0x4cf594
004cf584: bl #0x310440
004cf588: mov r3, #0
004cf58c: str r3, [r4, #0x10]
004cf590: str r3, [r4, #0x14]
004cf594: ldr r0, [r4, #0x1c]
004cf598: cmp r0, #0
004cf59c: beq #0x4cf5b0
004cf5a0: bl #0x310440
004cf5a4: mov r3, #0
004cf5a8: str r3, [r4, #0x18]
004cf5ac: str r3, [r4, #0x1c]
004cf5b0: mov r0, r4
004cf5b4: pop {r4, lr}
004cf5b8: b #0x4cf4d8

# 0x4cf884 _ZN7Structs23v2QuestPickedUpLiftableD1Ev
004cf884: push {r4, lr}
004cf888: ldr r3, [pc, #0x44]
004cf88c: ldr r2, [pc, #0x44]
004cf890: mov r4, r0
004cf894: add r3, pc, r3
004cf898: ldr r0, [r0, #0x14]
004cf89c: ldr r2, [r3, r2]
004cf8a0: cmp r0, #0
004cf8a4: add r2, r2, #8
004cf8a8: str r2, [r4]
004cf8ac: beq #0x4cf8b4
004cf8b0: bl #0x310440
004cf8b4: ldr r0, [r4, #0x1c]
004cf8b8: cmp r0, #0
004cf8bc: beq #0x4cf8c4
004cf8c0: bl #0x310440
004cf8c4: mov r0, r4
004cf8c8: bl #0x4cf760
004cf8cc: mov r0, r4
004cf8d0: pop {r4, pc}
004cf8d4: strdeq r5, r6, [ip], #-0x1c
004cf8d8: andeq r1, r0, r4, lsr #2

# 0x4cf8dc _ZN7Structs23v2QuestPickedUpLiftableD0Ev
004cf8dc: push {r4, lr}
004cf8e0: mov r4, r0
004cf8e4: bl #0x4cf884
004cf8e8: mov r0, r4
004cf8ec: bl #0x310440
004cf8f0: mov r0, r4
004cf8f4: pop {r4, pc}

# 0x4cf8f8 _ZN7Structs23v2QuestPickedUpLiftableD2Ev
004cf8f8: push {r4, lr}
004cf8fc: ldr r3, [pc, #0x44]
004cf900: ldr r2, [pc, #0x44]
004cf904: mov r4, r0
004cf908: add r3, pc, r3
004cf90c: ldr r0, [r0, #0x14]
004cf910: ldr r2, [r3, r2]
004cf914: cmp r0, #0
004cf918: add r2, r2, #8
004cf91c: str r2, [r4]
004cf920: beq #0x4cf928
004cf924: bl #0x310440
004cf928: ldr r0, [r4, #0x1c]
004cf92c: cmp r0, #0
004cf930: beq #0x4cf938
004cf934: bl #0x310440
004cf938: mov r0, r4
004cf93c: bl #0x4cf760
004cf940: mov r0, r4
004cf944: pop {r4, pc}
004cf948: subeq r5, ip, r8, lsl #3
004cf94c: andeq r1, r0, r4, lsr #2

# 0x504804 _ZN7Structs23v2QuestPickedUpLiftable4readEP11IStreamBase
00504804: b #0x504588

# 0x50ed8c _Z24GetWorldPosFromScreenPosRK7Point2DIfER7Point3DIfEf
0050ed8c: ldr r3, [pc, #0x118]
0050ed90: ldr r2, [pc, #0x118]
0050ed94: push {r4, r5, r6, r7, r8, sl, lr}
0050ed98: add r3, pc, r3
0050ed9c: ldr r2, [r3, r2]
0050eda0: sub sp, sp, #0x34
0050eda4: mov r6, r0
0050eda8: ldr r2, [r2, #0x10]
0050edac: ldr r0, [r0]
0050edb0: mov r4, r1
0050edb4: ldr r7, [r2, #0x1c]
0050edb8: bl #0x30e4cc
0050edbc: mov r5, r0
0050edc0: ldr r0, [r6, #4]
0050edc4: bl #0x30e4cc
0050edc8: ldr r1, [r7, #0x2c]
0050edcc: add r2, sp, #0x28
0050edd0: mov r3, #0
0050edd4: ldr ip, [r1]
0050edd8: ldr ip, [ip, #0x14]
0050eddc: str r0, [sp, #0x2c]
0050ede0: str r5, [sp, #0x28]
0050ede4: add r0, sp, #4
0050ede8: blx ip
0050edec: ldr r1, [sp, #8]
0050edf0: ldr r0, [sp, #0x14]
0050edf4: bl #0x30e3ac
0050edf8: ldr r1, [sp, #0xc]
0050edfc: mov r6, r0
0050ee00: ldr r0, [sp, #0x18]
0050ee04: bl #0x30e3ac
0050ee08: ldr r1, [sp, #4]
0050ee0c: mov r5, r0
0050ee10: ldr r0, [sp, #0x10]
0050ee14: bl #0x30e3ac
0050ee18: str r0, [sp, #0x1c]
0050ee1c: add r0, sp, #0x1c
0050ee20: str r6, [sp, #0x20]
0050ee24: str r5, [sp, #0x24]
0050ee28: bl #0x35e8e0
0050ee2c: ldr r5, [sp, #0xc]
0050ee30: ldr r7, [r0, #8]
0050ee34: mov r3, r0
0050ee38: ldr r8, [r0, #4]
0050ee3c: mov r1, r7
0050ee40: mov r0, r5
0050ee44: ldr sl, [r3]
0050ee48: bl #0x30ec94
0050ee4c: mov r1, r8
0050ee50: mov r6, r0
0050ee54: bl #0x30ed6c
0050ee58: mov r1, r0
0050ee5c: ldr r0, [sp, #8]
0050ee60: bl #0x30e3ac
0050ee64: mov r1, sl
0050ee68: mov r8, r0
0050ee6c: mov r0, r6
0050ee70: bl #0x30ed6c
0050ee74: mov r1, r0
0050ee78: ldr r0, [sp, #4]
0050ee7c: bl #0x30e3ac
0050ee80: mov r1, r7
0050ee84: str r0, [r4]
0050ee88: str r8, [r4, #4]
0050ee8c: mov r0, r6
0050ee90: bl #0x30ed6c
0050ee94: mov r1, r0
0050ee98: mov r0, r5
0050ee9c: bl #0x30e3ac
0050eea0: str r0, [r4, #8]
0050eea4: add sp, sp, #0x34
0050eea8: pop {r4, r5, r6, r7, r8, sl, pc}
0050eeac: strdeq r5, r6, [r8], #-0xc8
0050eeb0: strdeq r3, r4, [r0], -r4

# 0x51b874 _ZN7PFFloor14GetCollisionAtERK7Point3DIfES3_RS1_
0051b874: ldr ip, [pc, #0xe8]
0051b878: push {r4, r5, r6, r7, r8, sl, lr}
0051b87c: ldr lr, [pc, #0xe4]
0051b880: add ip, pc, ip
0051b884: ldr r6, [r1]
0051b888: ldr lr, [ip, lr]
0051b88c: ldr r4, [r2]
0051b890: ldr r5, [r1, #8]
0051b894: ldr r8, [lr, #0x10]
0051b898: ldr r7, [r1, #4]
0051b89c: ldr sl, [r2, #8]
0051b8a0: ldr r1, [r2, #4]
0051b8a4: ldr r2, [r8, #0x1c]
0051b8a8: sub sp, sp, #0x54
0051b8ac: mov lr, #0
0051b8b0: str lr, [sp, #0x28]
0051b8b4: str r4, [sp, #0x38]
0051b8b8: str lr, [sp, #0x44]
0051b8bc: str lr, [sp, #0x48]
0051b8c0: str lr, [sp, #0x4c]
0051b8c4: str lr, [sp, #8]
0051b8c8: str lr, [sp, #0xc]
0051b8cc: str lr, [sp, #0x10]
0051b8d0: str lr, [sp, #0x14]
0051b8d4: str lr, [sp, #0x18]
0051b8d8: str lr, [sp, #0x1c]
0051b8dc: str lr, [sp, #0x20]
0051b8e0: str lr, [sp, #0x24]
0051b8e4: str r6, [sp, #0x2c]
0051b8e8: str r7, [sp, #0x30]
0051b8ec: str r5, [sp, #0x34]
0051b8f0: str r1, [sp, #0x3c]
0051b8f4: str sl, [sp, #0x40]
0051b8f8: ldr r6, [r2, #0x2c]
0051b8fc: ldr r2, [r0, #0x40]
0051b900: mov r4, r3
0051b904: ldr r1, [r6]
0051b908: ldr r3, [r2]
0051b90c: mov r0, r2
0051b910: ldr r5, [r1, #0xc]
0051b914: mov lr, pc
0051b918: ldr pc, [r3, #0xb0]
0051b91c: add r3, sp, #8
0051b920: mov r2, r0
0051b924: str r3, [sp]
0051b928: mov r0, r6
0051b92c: add r1, sp, #0x2c
0051b930: add r3, sp, #0x44
0051b934: blx r5
0051b938: cmp r0, #0
0051b93c: beq #0x51b95c
0051b940: ldr r2, [sp, #0x48]
0051b944: ldr r3, [sp, #0x4c]
0051b948: ldr r1, [sp, #0x44]
0051b94c: mov r0, #1
0051b950: str r2, [r4, #4]
0051b954: str r1, [r4]
0051b958: str r3, [r4, #8]
0051b95c: add sp, sp, #0x54
0051b960: pop {r4, r5, r6, r7, r8, sl, pc}
0051b964: subeq sb, r7, r0, lsl r2
0051b968: strdeq r3, r4, [r0], -r4

# 0x5212e0 _ZN6PFRoom14GetCollisionAtERK7Point3DIfES3_RS1_b
005212e0: push {r4, r5, r6, r7, r8, lr}
005212e4: ldrb r4, [sp, #0x18]
005212e8: mov r5, r0
005212ec: mov r8, r1
005212f0: cmp r4, #0
005212f4: mov r6, r2
005212f8: mov r7, r3
005212fc: beq #0x52135c
00521300: ldr r3, [r0, #0x30]
00521304: ldr r2, [r0, #0x34]
00521308: rsb r2, r3, r2
0052130c: lsrs r2, r2, #2
00521310: movne r4, #0
00521314: bne #0x521334
00521318: mov r0, #0
0052131c: pop {r4, r5, r6, r7, r8, pc}
00521320: ldr r3, [r5, #0x30]
00521324: ldr r2, [r5, #0x34]
00521328: rsb r2, r3, r2
0052132c: cmp r4, r2, asr #2
00521330: bhs #0x521318
00521334: ldr r0, [r3, r4, lsl #2]
00521338: mov r1, r8
0052133c: mov r2, r6
00521340: mov r3, r7
00521344: bl #0x51b874
00521348: cmp r0, #0
0052134c: add r4, r4, #1
00521350: beq #0x521320
00521354: mov r0, #1
00521358: pop {r4, r5, r6, r7, r8, pc}
0052135c: ldr r3, [r0, #0x30]
00521360: ldr r1, [r0, #0x34]
00521364: rsb r2, r3, r1
00521368: lsrs r2, r2, #2
0052136c: beq #0x521318
00521370: ldr r0, [r3, r4, lsl #2]
00521374: ldr r2, [r0, #0x24]
00521378: tst r2, #0x3000000
0052137c: beq #0x5213a0
00521380: add r4, r4, #1
00521384: rsb r2, r3, r1
00521388: cmp r4, r2, asr #2
0052138c: bhs #0x521318
00521390: ldr r0, [r3, r4, lsl #2]
00521394: ldr r2, [r0, #0x24]
00521398: tst r2, #0x3000000
0052139c: bne #0x521380
005213a0: mov r1, r8
005213a4: mov r2, r6
005213a8: mov r3, r7
005213ac: bl #0x51b874
005213b0: cmp r0, #0
005213b4: bne #0x521354
005213b8: ldr r3, [r5, #0x30]
005213bc: ldr r1, [r5, #0x34]
005213c0: b #0x521380

# 0x525800 _ZN7PFWorld14GetCollisionAtERK7Point3DIfES3_RS1_b
00525800: push {r4, r5, r6, r7, r8, sl, lr}
00525804: mov r5, r0
00525808: ldr ip, [r5, #0xc]
0052580c: ldr r0, [r0, #8]
00525810: sub sp, sp, #0xc
00525814: mov sl, r1
00525818: rsb ip, r0, ip
0052581c: lsrs ip, ip, #2
00525820: mov r8, r2
00525824: mov r6, r3
00525828: ldrb r7, [sp, #0x28]
0052582c: beq #0x525878
00525830: mov r4, #0
00525834: b #0x52584c
00525838: ldr r0, [r5, #8]
0052583c: ldr r3, [r5, #0xc]
00525840: rsb r3, r0, r3
00525844: cmp r4, r3, asr #2
00525848: bhs #0x525878
0052584c: ldr r0, [r0, r4, lsl #2]
00525850: mov r1, sl
00525854: mov r2, r8
00525858: mov r3, r6
0052585c: str r7, [sp]
00525860: bl #0x5212e0
00525864: cmp r0, #0
00525868: add r4, r4, #1
0052586c: beq #0x525838
00525870: mov r0, #1
00525874: b #0x52587c
00525878: mov r0, #0
0052587c: add sp, sp, #0xc
00525880: pop {r4, r5, r6, r7, r8, sl, pc}

# 0x525884 _ZN7PFWorld22TranslateScreenToWorldERK7Point2DIfER7Point3DIfE
00525884: ldr r3, [pc, #0xb0]
00525888: push {r4, r5, r6, r7, r8, lr}
0052588c: mov r7, r1
00525890: ldr r1, [pc, #0xa8]
00525894: add r3, pc, r3
00525898: sub sp, sp, #0x40
0052589c: ldr r1, [r3, r1]
005258a0: mov r5, r0
005258a4: ldr r0, [r7]
005258a8: ldr r1, [r1, #0x10]
005258ac: mov r4, r2
005258b0: ldr r8, [r1, #0x1c]
005258b4: bl #0x30e4cc
005258b8: mov r6, r0
005258bc: ldr r0, [r7, #4]
005258c0: bl #0x30e4cc
005258c4: ldr r1, [r8, #0x2c]
005258c8: add r2, sp, #0x38
005258cc: mov r3, #0
005258d0: ldr ip, [r1]
005258d4: ldr ip, [ip, #0x14]
005258d8: str r0, [sp, #0x3c]
005258dc: str r6, [sp, #0x38]
005258e0: add r0, sp, #8
005258e4: blx ip
005258e8: ldr ip, [sp, #8]
005258ec: mov r0, r5
005258f0: mov r3, r4
005258f4: str ip, [sp, #0x2c]
005258f8: ldr ip, [sp, #0xc]
005258fc: add r1, sp, #0x2c
00525900: add r2, sp, #0x20
00525904: str ip, [sp, #0x30]
00525908: ldr ip, [sp, #0x10]
0052590c: str ip, [sp, #0x34]
00525910: ldr ip, [sp, #0x14]
00525914: str ip, [sp, #0x20]
00525918: ldr ip, [sp, #0x18]
0052591c: str ip, [sp, #0x24]
00525920: ldr ip, [sp, #0x1c]
00525924: str ip, [sp, #0x28]
00525928: mov ip, #0
0052592c: str ip, [sp]
00525930: bl #0x525800
00525934: add sp, sp, #0x40
00525938: pop {r4, r5, r6, r7, r8, pc}

# 0x52f11c appOnTouch
0052f11c: push {r4, r5, r6, r7, lr}
0052f120: mov r6, r0
0052f124: ldr r0, [pc, #0x10c]
0052f128: mov r5, r1
0052f12c: sub sp, sp, #0x14
0052f130: add r0, pc, r0
0052f134: mov r4, r2
0052f138: ldr r7, [sp, #0x34]
0052f13c: bl #0x324114
0052f140: sub r2, r5, #0x12c
0052f144: sub r2, r2, #1
0052f148: cmp r2, #0xc6
0052f14c: movhi r1, #0
0052f150: movls r1, #1
0052f154: ldr r3, [pc, #0xe0]
0052f158: cmp r4, #0xfa
0052f15c: movle r1, #0
0052f160: cmp r1, #0
0052f164: add r3, pc, r3
0052f168: beq #0x52f174
0052f16c: cmp r4, #0x190
0052f170: blt #0x52f1dc
0052f174: ldr r2, [pc, #0xc4]
0052f178: ldr r2, [r3, r2]
0052f17c: ldr r2, [r2]
0052f180: cmp r2, #0
0052f184: beq #0x52f1d4
0052f188: ldr r1, [pc, #0xb4]
0052f18c: ldr r3, [r3, r1]
0052f190: ldrb r3, [r3]
0052f194: cmp r3, #0
0052f198: bne #0x52f1d4
0052f19c: cmp r6, #1
0052f1a0: beq #0x52f1f0
0052f1a4: cmp r6, #2
0052f1a8: beq #0x52f214
0052f1ac: cmp r6, #0
0052f1b0: bne #0x52f1d4
0052f1b4: ldr r0, [r2, #0x20]
0052f1b8: add r1, sp, #0xc
0052f1bc: mov r2, r7
0052f1c0: ldr r3, [r0]
0052f1c4: ldr r3, [r3, #0x28]
0052f1c8: strh r5, [sp, #0xc]
0052f1cc: strh r4, [sp, #0xe]
0052f1d0: blx r3
0052f1d4: add sp, sp, #0x14
0052f1d8: pop {r4, r5, r6, r7, pc}
0052f1dc: ldr r2, [pc, #0x64]
0052f1e0: mov r1, #0
0052f1e4: ldr r2, [r3, r2]
0052f1e8: strb r1, [r2]
0052f1ec: b #0x52f174
0052f1f0: ldr r0, [r2, #0x20]
0052f1f4: add r1, sp, #8
0052f1f8: mov r2, r7
0052f1fc: ldr r3, [r0]
0052f200: ldr r3, [r3, #0x20]
0052f204: strh r5, [sp, #8]
0052f208: strh r4, [sp, #0xa]
0052f20c: blx r3
0052f210: b #0x52f1d4
0052f214: ldr r0, [r2, #0x20]
0052f218: add r1, sp, #4
0052f21c: mov r2, r7
0052f220: ldr r3, [r0]
0052f224: ldr r3, [r3, #0x24]
0052f228: strh r5, [sp, #4]
0052f22c: strh r4, [sp, #6]
0052f230: blx r3
0052f234: b #0x52f1d4
0052f238: eorseq sp, sl, r8, lsr lr
0052f23c: subeq r5, r6, ip, lsr #18
0052f240: andeq r4, r0, r4, lsr #13
0052f244: strheq r3, [r0], -r8
0052f248: andeq r1, r0, r4, lsr #29

# 0x533440 Java_com_gameloft_android_GAND_GloftD2SS_GameGLSurfaceView_nativeOnTouch
00533440: ldr r1, [pc, #0x8c]
00533444: push {r4, r5, r6, lr}
00533448: mov r6, r2
0053344c: ldr r2, [pc, #0x84]
00533450: add r1, pc, r1
00533454: mov r5, r3
00533458: ldr r2, [r1, r2]
0053345c: sub sp, sp, #0x18
00533460: ldr r4, [sp, #0x30]
00533464: ldr r3, [r2]
00533468: cmp r3, #1
0053346c: beq #0x5334cc
00533470: mov r1, #0
00533474: add r0, sp, #0x10
00533478: bl #0x30e724
0053347c: ldr r2, [sp, #0x14]
00533480: movw r3, #0x4dd3
00533484: movt r3, #0x1062
00533488: smull r1, r3, r3, r2
0053348c: mov ip, #1
00533490: str ip, [sp]
00533494: ldr ip, [sp, #0x38]
00533498: asr r2, r2, #0x1f
0053349c: rsb r2, r2, r3, asr #6
005334a0: ldr r3, [sp, #0x10]
005334a4: str ip, [sp, #4]
005334a8: ldr ip, [sp, #0x3c]
005334ac: mov r1, #0x3e8
005334b0: mla r3, r1, r3, r2
005334b4: mov r0, r6
005334b8: mov r1, r5
005334bc: ldr r2, [sp, #0x28]
005334c0: str ip, [sp, #8]
005334c4: str r4, [sp, #0xc]
005334c8: bl #0x52f11c
005334cc: add sp, sp, #0x18
005334d0: pop {r4, r5, r6, pc}
005334d4: subeq r1, r6, r0, asr #12
005334d8: andeq r2, r0, r0, ror #24

# 0x6c59c0 _ZN6glitch5scene22CSceneCollisionManager27getRayFromScreenCoordinatesENS_4core10position2dIiEEPNS0_16ICameraSceneNodeE
006c59c0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006c59c4: mov ip, #0
006c59c8: str ip, [r0, #0x14]
006c59cc: str ip, [r0]
006c59d0: str ip, [r0, #4]
006c59d4: str ip, [r0, #8]
006c59d8: str ip, [r0, #0xc]
006c59dc: str ip, [r0, #0x10]
006c59e0: mov sl, r1
006c59e4: ldr r1, [r1, #8]
006c59e8: sub sp, sp, #0x44
006c59ec: mov r4, r0
006c59f0: cmp r1, #0
006c59f4: mov sb, r2
006c59f8: mov r7, r3
006c59fc: beq #0x6c5c3c
006c5a00: cmp r3, #0
006c5a04: beq #0x6c5d30
006c5a08: ldr r3, [r7]
006c5a0c: mov r0, r7
006c5a10: mov lr, pc
006c5a14: ldr pc, [r3, #0x144]
006c5a18: add ip, r0, #0x2c
006c5a1c: add r8, r0, #0xc
006c5a20: add fp, r0, #0x5c
006c5a24: mov r5, #0
006c5a28: mov r2, ip
006c5a2c: mov r6, r0
006c5a30: mov r1, fp
006c5a34: add r3, sp, #0x34
006c5a38: mov r0, r8
006c5a3c: str ip, [sp, #4]
006c5a40: str r5, [sp, #0x34]
006c5a44: str r5, [sp, #0x38]
006c5a48: str r5, [sp, #0x3c]
006c5a4c: bl #0x3415d8
006c5a50: add r2, r6, #0x3c
006c5a54: add r3, sp, #0x28
006c5a58: mov r1, fp
006c5a5c: mov r0, r8
006c5a60: str r5, [sp, #0x28]
006c5a64: str r5, [sp, #0x2c]
006c5a68: str r5, [sp, #0x30]
006c5a6c: bl #0x3415d8
006c5a70: ldr r1, [sp, #0x34]
006c5a74: ldr r0, [sp, #0x28]
006c5a78: bl #0x30e3ac
006c5a7c: ldr r1, [sp, #0x38]
006c5a80: str r0, [sp, #0xc]
006c5a84: ldr r0, [sp, #0x2c]
006c5a88: bl #0x30e3ac
006c5a8c: ldr r1, [sp, #0x3c]
006c5a90: str r0, [sp, #0x10]
006c5a94: ldr r0, [sp, #0x30]
006c5a98: bl #0x30e3ac
006c5a9c: ldr ip, [sp, #4]
006c5aa0: add r3, sp, #0x1c
006c5aa4: str r0, [sp, #0x14]
006c5aa8: mov r2, ip
006c5aac: mov r0, r8
006c5ab0: add r1, r6, #0x4c
006c5ab4: str r5, [sp, #0x24]
006c5ab8: str r5, [sp, #0x1c]
006c5abc: str r5, [sp, #0x20]
006c5ac0: bl #0x3415d8
006c5ac4: ldr r1, [sp, #0x34]
006c5ac8: ldr r0, [sp, #0x1c]
006c5acc: bl #0x30e3ac
006c5ad0: ldr r1, [sp, #0x38]
006c5ad4: str r0, [sp, #8]
006c5ad8: ldr r0, [sp, #0x20]
006c5adc: bl #0x30e3ac
006c5ae0: ldr r1, [sp, #0x3c]
006c5ae4: mov fp, r0
006c5ae8: ldr r0, [sp, #0x24]
006c5aec: bl #0x30e3ac
006c5af0: ldr r3, [sl, #0xc]
006c5af4: mov r8, r0
006c5af8: ldr r0, [sb]
006c5afc: ldr r3, [r3, #0xcc]
006c5b00: ldr ip, [r3, #-4]
006c5b04: ldr r1, [ip, #0x1c]
006c5b08: ldr r3, [ip, #0x14]
006c5b0c: ldr r2, [ip, #0x18]
006c5b10: ldr r5, [ip, #0x20]
006c5b14: rsb r3, r3, r1
006c5b18: str r3, [sp, #4]
006c5b1c: rsb r5, r2, r5
006c5b20: bl #0x30e964
006c5b24: ldr r3, [sp, #4]
006c5b28: mov sl, r0
006c5b2c: mov r0, r3
006c5b30: bl #0x30e964
006c5b34: mov r1, r0
006c5b38: mov r0, sl
006c5b3c: bl #0x30ec94
006c5b40: mov sl, r0
006c5b44: ldr r0, [sb, #4]
006c5b48: bl #0x30e964
006c5b4c: mov sb, r0
006c5b50: mov r0, r5
006c5b54: bl #0x30e964
006c5b58: mov r1, r0
006c5b5c: mov r0, sb
006c5b60: bl #0x30ec94
006c5b64: ldr r3, [r7]
006c5b68: mov r5, r0
006c5b6c: mov r0, r7
006c5b70: mov lr, pc
006c5b74: ldr pc, [r3, #0x150]
006c5b78: cmp r0, #0
006c5b7c: bne #0x6c5c48
006c5b80: ldr r3, [r6]
006c5b84: str r3, [r4]
006c5b88: ldr r3, [r6, #4]
006c5b8c: str r3, [r4, #4]
006c5b90: ldr r3, [r6, #8]
006c5b94: str r3, [r4, #8]
006c5b98: ldr r1, [sp, #0x10]
006c5b9c: mov r0, sl
006c5ba0: bl #0x30ed6c
006c5ba4: ldr r1, [sp, #0x38]
006c5ba8: bl #0x30eba4
006c5bac: mov r1, fp
006c5bb0: mov r6, r0
006c5bb4: mov r0, r5
006c5bb8: bl #0x30ed6c
006c5bbc: mov r1, r0
006c5bc0: mov r0, r6
006c5bc4: bl #0x30eba4
006c5bc8: ldr r1, [sp, #0x14]
006c5bcc: mov r6, r0
006c5bd0: mov r0, sl
006c5bd4: bl #0x30ed6c
006c5bd8: ldr r1, [sp, #0x3c]
006c5bdc: bl #0x30eba4
006c5be0: mov r1, r8
006c5be4: mov r7, r0
006c5be8: mov r0, r5
006c5bec: bl #0x30ed6c
006c5bf0: mov r1, r0
006c5bf4: mov r0, r7
006c5bf8: bl #0x30eba4
006c5bfc: ldr r1, [sp, #0xc]
006c5c00: mov r7, r0
006c5c04: mov r0, sl
006c5c08: bl #0x30ed6c
006c5c0c: ldr r1, [sp, #0x34]
006c5c10: bl #0x30eba4
006c5c14: ldr r1, [sp, #8]
006c5c18: mov r8, r0
006c5c1c: mov r0, r5
006c5c20: bl #0x30ed6c
006c5c24: mov r1, r0
006c5c28: mov r0, r8
006c5c2c: bl #0x30eba4
006c5c30: str r6, [r4, #0x10]
006c5c34: str r0, [r4, #0xc]
006c5c38: str r7, [r4, #0x14]
006c5c3c: mov r0, r4
006c5c40: add sp, sp, #0x44
006c5c44: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006c5c48: mov r1, #0x3f000000
006c5c4c: mov r0, sl
006c5c50: bl #0x30e3ac
006c5c54: mov r1, #0x3f000000
006c5c58: mov sb, r0
006c5c5c: mov r0, r5
006c5c60: bl #0x30e3ac
006c5c64: ldr r1, [sp, #0x10]
006c5c68: mov r7, r0
006c5c6c: mov r0, sb
006c5c70: bl #0x30ed6c
006c5c74: ldr r1, [r6, #4]
006c5c78: bl #0x30eba4
006c5c7c: mov r1, fp
006c5c80: mov r3, r0
006c5c84: mov r0, r7
006c5c88: str r3, [sp, #4]
006c5c8c: bl #0x30ed6c
006c5c90: ldr r3, [sp, #4]
006c5c94: mov r1, r0
006c5c98: mov r0, r3
006c5c9c: bl #0x30eba4
006c5ca0: ldr r1, [sp, #0x14]
006c5ca4: mov r2, r0
006c5ca8: mov r0, sb
006c5cac: str r2, [sp]
006c5cb0: bl #0x30ed6c
006c5cb4: ldr r1, [r6, #8]
006c5cb8: bl #0x30eba4
006c5cbc: mov r1, r8
006c5cc0: mov r3, r0
006c5cc4: mov r0, r7
006c5cc8: str r3, [sp, #4]
006c5ccc: bl #0x30ed6c
006c5cd0: ldr r3, [sp, #4]
006c5cd4: mov r1, r0
006c5cd8: mov r0, r3
006c5cdc: bl #0x30eba4
006c5ce0: ldr r1, [sp, #0xc]
006c5ce4: mov r3, r0
006c5ce8: mov r0, sb
006c5cec: str r3, [sp, #4]
006c5cf0: bl #0x30ed6c
006c5cf4: ldr r1, [r6]
006c5cf8: bl #0x30eba4
006c5cfc: ldr r1, [sp, #8]
006c5d00: mov r6, r0
006c5d04: mov r0, r7
006c5d08: bl #0x30ed6c
006c5d0c: mov r1, r0
006c5d10: mov r0, r6
006c5d14: bl #0x30eba4
006c5d18: ldr r2, [sp]
006c5d1c: str r0, [r4]
006c5d20: str r2, [r4, #4]
006c5d24: ldr r3, [sp, #4]
006c5d28: str r3, [r4, #8]
006c5d2c: b #0x6c5b98
006c5d30: ldr r7, [r1, #0xe4]
006c5d34: cmp r7, #0
006c5d38: beq #0x6c5c3c
006c5d3c: b #0x6c5a08

# 0x6c5d40 _ZN6glitch5scene22CSceneCollisionManager15getPickedNodeBBEPNS0_10ISceneNodeERKNS_4core6line3dIfEEibRfRS3_
006c5d40: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006c5d44: sub sp, sp, #0x124
006c5d48: ldrb r8, [sp, #0x148]
006c5d4c: add r4, sp, #0x5c
006c5d50: mov r7, r2
006c5d54: str r3, [sp, #0x20]
006c5d58: mov ip, #0
006c5d5c: str r4, [sp, #0x3c]
006c5d60: str r0, [sp, #0x40]
006c5d64: add r3, r4, #0xc
006c5d68: add r2, r4, #0x6c
006c5d6c: str ip, [r3, #-0xc]
006c5d70: str ip, [r3, #-8]
006c5d74: str ip, [r3, #-4]
006c5d78: add r3, r3, #0xc
006c5d7c: cmp r3, r2
006c5d80: bne #0x6c5d6c
006c5d84: mov r6, r1
006c5d88: ldr r4, [r6, #0xf4]!
006c5d8c: cmp r4, r6
006c5d90: beq #0x6c60d8
006c5d94: add ip, sp, #0xbc
006c5d98: str ip, [sp, #0x50]
006c5d9c: ldr ip, [sp, #0x3c]
006c5da0: add r0, sp, #0x100
006c5da4: add r2, sp, #0x11c
006c5da8: add r3, sp, #0x118
006c5dac: add ip, ip, #8
006c5db0: str r0, [sp, #0x44]
006c5db4: str r2, [sp, #0x48]
006c5db8: str r3, [sp, #0x4c]
006c5dbc: str ip, [sp, #0x54]
006c5dc0: cmp r4, #0
006c5dc4: moveq r5, r4
006c5dc8: subne r5, r4, #4
006c5dcc: ldr r3, [r5, #0x11c]
006c5dd0: tst r3, #1
006c5dd4: beq #0x6c60cc
006c5dd8: cmp r8, #0
006c5ddc: bne #0x6c6118
006c5de0: ldr r0, [sp, #0x20]
006c5de4: cmp r0, #0
006c5de8: bne #0x6c6134
006c5dec: mov r2, #0x40
006c5df0: mov r1, #0
006c5df4: ldr r0, [sp, #0x50]
006c5df8: bl #0x30e460
006c5dfc: mov r3, #0x3f800000
006c5e00: mov ip, #1
006c5e04: strb ip, [sp, #0xfc]
006c5e08: str r3, [sp, #0xbc]
006c5e0c: str r3, [sp, #0xd0]
006c5e10: str r3, [sp, #0xe4]
006c5e14: str r3, [sp, #0xf8]
006c5e18: ldr r3, [r5]
006c5e1c: mov r0, r5
006c5e20: mov lr, pc
006c5e24: ldr pc, [r3, #0x38]
006c5e28: ldr r1, [sp, #0x50]
006c5e2c: bl #0x3232c0
006c5e30: cmp r0, #0
006c5e34: beq #0x6c60cc
006c5e38: ldr r2, [sp, #0xbc]
006c5e3c: ldr fp, [r7]
006c5e40: ldr sb, [r7, #4]
006c5e44: mov r1, r2
006c5e48: mov r0, fp
006c5e4c: str r2, [sp, #0x14]
006c5e50: bl #0x30ed6c
006c5e54: ldr r1, [sp, #0xcc]
006c5e58: mov r3, r0
006c5e5c: mov r0, sb
006c5e60: ldr sl, [r7, #8]
006c5e64: str r3, [sp, #0x1c]
006c5e68: bl #0x30ed6c
006c5e6c: ldr r3, [sp, #0x1c]
006c5e70: mov r1, r0
006c5e74: mov r0, r3
006c5e78: bl #0x30eba4
006c5e7c: ldr r1, [sp, #0xdc]
006c5e80: mov r3, r0
006c5e84: mov r0, sl
006c5e88: str r3, [sp, #0x1c]
006c5e8c: bl #0x30ed6c
006c5e90: ldr r3, [sp, #0x1c]
006c5e94: mov r1, r0
006c5e98: mov r0, r3
006c5e9c: bl #0x30eba4
006c5ea0: ldr r1, [sp, #0xec]
006c5ea4: bl #0x30eba4
006c5ea8: ldr ip, [sp, #0xc0]
006c5eac: str r0, [sp, #0x100]
006c5eb0: mov r0, fp
006c5eb4: mov r1, ip
006c5eb8: str ip, [sp, #0x18]
006c5ebc: bl #0x30ed6c
006c5ec0: ldr r1, [sp, #0xd0]
006c5ec4: mov r3, r0
006c5ec8: mov r0, sb
006c5ecc: str r3, [sp, #0x1c]
006c5ed0: bl #0x30ed6c
006c5ed4: ldr r3, [sp, #0x1c]
006c5ed8: mov r1, r0
006c5edc: mov r0, r3
006c5ee0: bl #0x30eba4
006c5ee4: ldr r1, [sp, #0xe0]
006c5ee8: mov r3, r0
006c5eec: mov r0, sl
006c5ef0: str r3, [sp, #0x1c]
006c5ef4: bl #0x30ed6c
006c5ef8: ldr r3, [sp, #0x1c]
006c5efc: mov r1, r0
006c5f00: mov r0, r3
006c5f04: bl #0x30eba4
006c5f08: ldr r1, [sp, #0xf0]
006c5f0c: bl #0x30eba4
006c5f10: ldr r1, [sp, #0xc4]
006c5f14: str r0, [sp, #0x104]
006c5f18: mov r0, fp
006c5f1c: bl #0x30ed6c
006c5f20: ldr r1, [sp, #0xd4]
006c5f24: mov fp, r0
006c5f28: mov r0, sb
006c5f2c: bl #0x30ed6c
006c5f30: mov r1, r0
006c5f34: mov r0, fp
006c5f38: bl #0x30eba4
006c5f3c: ldr r1, [sp, #0xe4]
006c5f40: mov sb, r0
006c5f44: mov r0, sl
006c5f48: bl #0x30ed6c
006c5f4c: mov r1, r0
006c5f50: mov r0, sb
006c5f54: bl #0x30eba4
006c5f58: ldr r1, [sp, #0xf4]
006c5f5c: bl #0x30eba4
006c5f60: ldr r2, [sp, #0x14]
006c5f64: ldr fp, [r7, #0xc]
006c5f68: ldr sb, [r7, #0x10]
006c5f6c: mov r1, r2
006c5f70: ldr sl, [r7, #0x14]
006c5f74: str r0, [sp, #0x108]
006c5f78: mov r0, fp
006c5f7c: bl #0x30ed6c
006c5f80: ldr r1, [sp, #0xcc]
006c5f84: mov r3, r0
006c5f88: mov r0, sb
006c5f8c: str r3, [sp, #0x1c]
006c5f90: bl #0x30ed6c
006c5f94: ldr r3, [sp, #0x1c]
006c5f98: mov r1, r0
006c5f9c: mov r0, r3
006c5fa0: bl #0x30eba4
006c5fa4: ldr r1, [sp, #0xdc]
006c5fa8: mov r3, r0
006c5fac: mov r0, sl
006c5fb0: str r3, [sp, #0x1c]
006c5fb4: bl #0x30ed6c
006c5fb8: ldr r3, [sp, #0x1c]
006c5fbc: mov r1, r0
006c5fc0: mov r0, r3
006c5fc4: bl #0x30eba4
006c5fc8: mov r1, r0
006c5fcc: ldr r0, [sp, #0xec]
006c5fd0: bl #0x30eba4
006c5fd4: ldr ip, [sp, #0x18]
006c5fd8: str r0, [sp, #0x10c]
006c5fdc: mov r0, fp
006c5fe0: mov r1, ip
006c5fe4: bl #0x30ed6c
006c5fe8: ldr r1, [sp, #0xd0]
006c5fec: mov r3, r0
006c5ff0: mov r0, sb
006c5ff4: str r3, [sp, #0x1c]
006c5ff8: bl #0x30ed6c
006c5ffc: ldr r3, [sp, #0x1c]
006c6000: mov r1, r0
006c6004: mov r0, r3
006c6008: bl #0x30eba4
006c600c: ldr r1, [sp, #0xe0]
006c6010: mov r3, r0
006c6014: mov r0, sl
006c6018: str r3, [sp, #0x1c]
006c601c: bl #0x30ed6c
006c6020: ldr r3, [sp, #0x1c]
006c6024: mov r1, r0
006c6028: mov r0, r3
006c602c: bl #0x30eba4
006c6030: mov r1, r0
006c6034: ldr r0, [sp, #0xf0]
006c6038: bl #0x30eba4
006c603c: ldr r1, [sp, #0xc4]
006c6040: str r0, [sp, #0x110]
006c6044: mov r0, fp
006c6048: bl #0x30ed6c
006c604c: ldr r1, [sp, #0xd4]
006c6050: mov fp, r0
006c6054: mov r0, sb
006c6058: bl #0x30ed6c
006c605c: mov r1, r0
006c6060: mov r0, fp
006c6064: bl #0x30eba4
006c6068: ldr r1, [sp, #0xe4]
006c606c: mov sb, r0
006c6070: mov r0, sl
006c6074: bl #0x30ed6c
006c6078: mov r1, r0
006c607c: mov r0, sb
006c6080: bl #0x30eba4
006c6084: mov r1, r0
006c6088: ldr r0, [sp, #0xf4]
006c608c: bl #0x30eba4
006c6090: str r0, [sp, #0x114]
006c6094: ldr r3, [r5]
006c6098: mov r0, r5
006c609c: mov lr, pc
006c60a0: ldr pc, [r3, #0x30]
006c60a4: add r1, sp, #0x44
006c60a8: ldm r1, {r1, r2, r3}
006c60ac: mov sl, r0
006c60b0: bl #0x585b08
006c60b4: cmp r0, #0
006c60b8: bne #0x6c6154
006c60bc: ldr r3, [r5, #0x11c]
006c60c0: and r3, r3, #1
006c60c4: cmp r3, #0
006c60c8: bne #0x6c60e0
006c60cc: ldr r4, [r4]
006c60d0: cmp r6, r4
006c60d4: bne #0x6c5dc0
006c60d8: add sp, sp, #0x124
006c60dc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006c60e0: ldr ip, [sp, #0x14c]
006c60e4: mov r1, r5
006c60e8: ldr r0, [sp, #0x40]
006c60ec: str ip, [sp, #4]
006c60f0: ldr ip, [sp, #0x150]
006c60f4: mov r2, r7
006c60f8: ldr r3, [sp, #0x20]
006c60fc: str r8, [sp]
006c6100: str ip, [sp, #8]
006c6104: bl #0x6c5d40
006c6108: ldr r4, [r4]
006c610c: cmp r6, r4
006c6110: bne #0x6c5dc0
006c6114: b #0x6c60d8
006c6118: mov r0, r5
006c611c: bl #0x5971bc
006c6120: cmp r0, #0
006c6124: bne #0x6c60bc
006c6128: ldr r0, [sp, #0x20]
006c612c: cmp r0, #0
006c6130: beq #0x6c5dec
006c6134: ldr r3, [r5]
006c6138: mov r0, r5
006c613c: mov lr, pc
006c6140: ldr pc, [r3, #0x4c]
006c6144: ldr r2, [sp, #0x20]
006c6148: tst r0, r2
006c614c: bne #0x6c5dec
006c6150: b #0x6c60bc
006c6154: mov r0, sl
006c6158: ldr r1, [sp, #0x3c]
006c615c: bl #0x585638
006c6160: ldr r1, [sp, #0x100]
006c6164: ldr r3, [sp, #0x104]
006c6168: ldr r0, [sp, #0x108]
006c616c: ldr ip, [sp, #0x3c]
006c6170: ldr sl, [sp, #0x54]
006c6174: mov r2, #0
006c6178: str r7, [sp, #0x34]
006c617c: str r8, [sp, #0x38]
006c6180: str r0, [sp, #0x24]
006c6184: add sb, ip, #0x68
006c6188: mov fp, r1
006c618c: str r4, [sp, #0x28]
006c6190: str r6, [sp, #0x2c]
006c6194: str r5, [sp, #0x30]
006c6198: mov r7, r2
006c619c: mov r8, r3
006c61a0: ldr r0, [sl, #-8]
006c61a4: mov r1, fp
006c61a8: bl #0x30e3ac
006c61ac: mov r1, r8
006c61b0: mov r5, r0
006c61b4: ldr r0, [sl, #-4]
006c61b8: bl #0x30e3ac
006c61bc: ldr r1, [sp, #0x24]
006c61c0: mov r6, r0
006c61c4: ldr r0, [sl]
006c61c8: bl #0x30e3ac
006c61cc: mov r1, r5
006c61d0: mov r4, r0
006c61d4: mov r0, r5
006c61d8: bl #0x30ed6c
006c61dc: mov r1, r6
006c61e0: mov r5, r0
006c61e4: mov r0, r6
006c61e8: bl #0x30ed6c
006c61ec: mov r1, r0
006c61f0: mov r0, r5
006c61f4: bl #0x30eba4
006c61f8: mov r1, r4
006c61fc: mov r5, r0
006c6200: mov r0, r4
006c6204: bl #0x30ed6c
006c6208: mov r1, r0
006c620c: mov r0, r5
006c6210: bl #0x30eba4
006c6214: mov r4, r0
006c6218: mov r1, r4
006c621c: mov r0, r7
006c6220: bl #0x30e70c
006c6224: add sl, sl, #0xc
006c6228: cmp r0, #0
006c622c: movne r7, r4
006c6230: cmp sl, sb
006c6234: bne #0x6c61a0
006c6238: ldr r3, [sp, #0x14c]
006c623c: mov r2, r7
006c6240: mov r1, r2
006c6244: ldr r0, [r3]
006c6248: str r2, [sp, #0x14]
006c624c: bl #0x30e2f8
006c6250: cmp r0, #0
006c6254: ldr r5, [sp, #0x30]
006c6258: ldrne ip, [sp, #0x150]
006c625c: ldr r4, [sp, #0x28]
006c6260: ldr r6, [sp, #0x2c]
006c6264: ldr r7, [sp, #0x34]
006c6268: ldr r8, [sp, #0x38]
006c626c: ldr r2, [sp, #0x14]
006c6270: strne r5, [ip]
006c6274: ldrne r0, [sp, #0x14c]
006c6278: strne r2, [r0]
006c627c: b #0x6c60bc

# 0x7eb480 _ZN12b2MouseJoint9SetTargetERK6b2Vec2
007eb480: ldr r3, [r0, #0x34]
007eb484: ldrh r2, [r3]
007eb488: tst r2, #8
007eb48c: bicne r2, r2, #8
007eb490: strhne r2, [r3]
007eb494: movne r2, #0
007eb498: strne r2, [r3, #0x8c]
007eb49c: ldr r3, [r1]
007eb4a0: str r3, [r0, #0x4c]
007eb4a4: ldr r3, [r1, #4]
007eb4a8: str r3, [r0, #0x50]
007eb4ac: bx lr
