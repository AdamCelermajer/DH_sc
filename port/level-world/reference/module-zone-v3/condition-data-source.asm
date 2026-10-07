
# _ZN13ConditionData4InitEv
0033eb28: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033eb2c: ldr r5, [r0, #0x18]
0033eb30: ldr r3, [r0, #0x14]
0033eb34: ldr r8, [pc, #0xbc]
0033eb38: mov sl, r0
0033eb3c: cmp r3, r5
0033eb40: add r8, pc, r8
0033eb44: beq #0x33ebec
0033eb48: ldr r1, [pc, #0xac]
0033eb4c: mov r0, r5
0033eb50: add r1, pc, r1
0033eb54: bl #0x30e31c
0033eb58: cmp r0, #0
0033eb5c: beq #0x33ebec
0033eb60: ldr r3, [pc, #0x98]
0033eb64: ldr r3, [r8, r3]
0033eb68: ldr r6, [r3]
0033eb6c: cmp r6, #0
0033eb70: beq #0x33ebec
0033eb74: ldr r3, [pc, #0x88]
0033eb78: mov r4, #0
0033eb7c: ldr r3, [r8, r3]
0033eb80: ldr r7, [r3]
0033eb84: b #0x33eb94
0033eb88: add r4, r4, #1
0033eb8c: cmp r4, r6
0033eb90: beq #0x33ebf0
0033eb94: ldr r1, [r7, r4, lsl #2]
0033eb98: mov r0, r5
0033eb9c: bl #0x30e31c
0033eba0: cmp r0, #0
0033eba4: bne #0x33eb88
0033eba8: cmn r4, #1
0033ebac: beq #0x33ebf4
0033ebb0: mov r1, r0
0033ebb4: mov r0, #0xc
0033ebb8: bl #0x310570
0033ebbc: mov r5, r0
0033ebc0: bl #0x4786f0
0033ebc4: ldr r3, [pc, #0x3c]
0033ebc8: str r5, [sl, #0x1c]
0033ebcc: mov r0, r5
0033ebd0: ldr r3, [r8, r3]
0033ebd4: ldr r3, [r3]
0033ebd8: add r4, r3, r4, lsl #4
0033ebdc: ldr r2, [r4, #4]
0033ebe0: ldr r1, [r4, #8]
0033ebe4: pop {r4, r5, r6, r7, r8, sb, sl, lr}
0033ebe8: b #0x478914
0033ebec: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033ebf0: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033ebf4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033ebf8: rsbeq r5, r5, r0, asr pc
0033ebfc: subseq fp, sb, r0, asr fp
0033ec00: andeq r2, r0, r4, ror #30
0033ec04: andeq r3, r0, r4, asr r5
0033ec08: andeq r2, r0, r0, asr #16

# _ZN13ConditionDataD1Ev
0033e7f8: ldr r3, [pc, #0x30]
0033e7fc: ldr r2, [pc, #0x30]
0033e800: push {r4, r5, r6, lr}
0033e804: add r3, pc, r3
0033e808: ldr r2, [r3, r2]
0033e80c: mov r4, r0
0033e810: mov r5, r0
0033e814: add r2, r2, #8
0033e818: str r2, [r4], #4
0033e81c: bl #0x33e7c8
0033e820: mov r0, r4
0033e824: bl #0x3139ac
0033e828: mov r0, r5
0033e82c: pop {r4, r5, r6, pc}
0033e830: rsbeq r6, r5, ip, lsl #5
0033e834: andeq r2, r0, r8, asr lr

# _ZN13ConditionData5ClearEv
0033e7c8: push {r4, r5, r6, lr}
0033e7cc: ldr r4, [r0, #0x1c]
0033e7d0: mov r5, r0
0033e7d4: cmp r4, #0
0033e7d8: beq #0x33e7f4
0033e7dc: mov r0, r4
0033e7e0: bl #0x478eac
0033e7e4: mov r0, r4
0033e7e8: bl #0x310440
0033e7ec: mov r3, #0
0033e7f0: str r3, [r5, #0x1c]
0033e7f4: pop {r4, r5, r6, pc}

# _ZN13ConditionDataD0Ev
0033eacc: push {r4, lr}
0033ead0: mov r4, r0
0033ead4: bl #0x33e7f8
0033ead8: mov r0, r4
0033eadc: bl #0x310440
0033eae0: mov r0, r4
0033eae4: pop {r4, pc}

# _ZN13ConditionDataD2Ev
0033eae8: ldr r3, [pc, #0x30]
0033eaec: ldr r2, [pc, #0x30]
0033eaf0: push {r4, r5, r6, lr}
0033eaf4: add r3, pc, r3
0033eaf8: ldr r2, [r3, r2]
0033eafc: mov r4, r0
0033eb00: mov r5, r0
0033eb04: add r2, r2, #8
0033eb08: str r2, [r4], #4
0033eb0c: bl #0x33e7c8
0033eb10: mov r0, r4
0033eb14: bl #0x3139ac
0033eb18: mov r0, r5
0033eb1c: pop {r4, r5, r6, pc}
0033eb20: mlseq r5, ip, pc, r5
0033eb24: andeq r2, r0, r8, asr lr

# _ZN13ConditionDataC2Ev
0033edd8: ldr r2, [pc, #0x4c]
0033eddc: ldr ip, [pc, #0x4c]
0033ede0: mov r3, r0
0033ede4: add r2, pc, r2
0033ede8: ldr ip, [r2, ip]
0033edec: push {r4, lr}
0033edf0: add ip, ip, #8
0033edf4: mov r4, r0
0033edf8: str ip, [r3], #4
0033edfc: mov r0, r3
0033ee00: str r3, [r4, #0x14]
0033ee04: str r3, [r4, #0x18]
0033ee08: mov r1, #0x10
0033ee0c: bl #0x31167c
0033ee10: ldr r2, [r4, #0x14]
0033ee14: mov r3, #0
0033ee18: mov r0, r4
0033ee1c: strb r3, [r2]
0033ee20: strb r3, [r4, #0x20]
0033ee24: str r3, [r4, #0x1c]
0033ee28: pop {r4, pc}
0033ee2c: rsbeq r5, r5, ip, lsr #25
0033ee30: andeq r2, r0, r8, asr lr

# _ZN13ConditionDataC1Ev
0033ed7c: ldr r2, [pc, #0x4c]
0033ed80: ldr ip, [pc, #0x4c]
0033ed84: mov r3, r0
0033ed88: add r2, pc, r2
0033ed8c: ldr ip, [r2, ip]
0033ed90: push {r4, lr}
0033ed94: add ip, ip, #8
0033ed98: mov r4, r0
0033ed9c: str ip, [r3], #4
0033eda0: mov r0, r3
0033eda4: str r3, [r4, #0x14]
0033eda8: str r3, [r4, #0x18]
0033edac: mov r1, #0x10
0033edb0: bl #0x31167c
0033edb4: ldr r2, [r4, #0x14]
0033edb8: mov r3, #0
0033edbc: mov r0, r4
0033edc0: strb r3, [r2]
0033edc4: strb r3, [r4, #0x20]
0033edc8: str r3, [r4, #0x1c]
0033edcc: pop {r4, pc}
0033edd0: rsbeq r5, r5, r8, lsl #26
0033edd4: andeq r2, r0, r8, asr lr

# _ZN13ConditionData11SetAsTestedEb
0033dd24: strb r1, [r0, #0x20]
0033dd28: bx lr

# _ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310: push {r4, r5, r6, r7, r8, lr}
0033f314: ldr r6, [pc, #0x198]
0033f318: ldr r2, [pc, #0x198]
0033f31c: ldr r3, [pc, #0x198]
0033f320: add r6, pc, r6
0033f324: ldr r2, [r6, r2]
0033f328: ldr r3, [r6, r3]
0033f32c: mov r4, r0
0033f330: add r2, r2, #8
0033f334: add r0, r3, #8
0033f338: add r3, r4, #8
0033f33c: str r2, [r4]
0033f340: str r0, [r4, #4]
0033f344: mov r7, r1
0033f348: mov r0, r3
0033f34c: str r3, [r4, #0x18]
0033f350: str r3, [r4, #0x1c]
0033f354: mov r1, #0x10
0033f358: bl #0x31167c
0033f35c: ldr r2, [pc, #0x15c]
0033f360: ldr r1, [r4, #0x18]
0033f364: mov r5, #0
0033f368: ldr r2, [r6, r2]
0033f36c: strb r5, [r1]
0033f370: add r3, r4, #0x30
0033f374: add r1, r2, #0x74
0033f378: add r0, r2, #8
0033f37c: add r2, r2, #0x68
0033f380: stm r4, {r0, r2}
0033f384: str r1, [r4, #0x24]
0033f388: mov r0, r3
0033f38c: str r5, [r4, #0x20]
0033f390: strb r5, [r4, #0x28]
0033f394: strb r5, [r4, #0x29]
0033f398: str r5, [r4, #0x2c]
0033f39c: str r3, [r4, #0x40]
0033f3a0: str r3, [r4, #0x44]
0033f3a4: mov r1, #0x10
0033f3a8: bl #0x31167c
0033f3ac: ldr r2, [r4, #0x40]
0033f3b0: add r3, r4, #0x48
0033f3b4: mov r0, r3
0033f3b8: strb r5, [r2]
0033f3bc: mov r1, #0x10
0033f3c0: str r3, [r4, #0x58]
0033f3c4: str r3, [r4, #0x5c]
0033f3c8: bl #0x31167c
0033f3cc: ldr r2, [r4, #0x58]
0033f3d0: add r3, r4, #0x68
0033f3d4: mvn r6, #0
0033f3d8: strb r5, [r2]
0033f3dc: mov r1, #0x10
0033f3e0: mov r0, r3
0033f3e4: strb r5, [r4, #0x60]
0033f3e8: str r3, [r4, #0x78]
0033f3ec: str r3, [r4, #0x7c]
0033f3f0: str r6, [r4, #0x64]
0033f3f4: bl #0x31167c
0033f3f8: ldr r3, [r4, #0x78]
0033f3fc: add r0, r4, #0x8c
0033f400: strb r5, [r3]
0033f404: mov r3, #1
0033f408: strb r3, [r4, #0x8a]
0033f40c: strb r5, [r4, #0x81]
0033f410: strb r5, [r4, #0x84]
0033f414: strb r5, [r4, #0x85]
0033f418: strb r5, [r4, #0x86]
0033f41c: strb r5, [r4, #0x88]
0033f420: strb r5, [r4, #0x89]
0033f424: bl #0x33ed7c
0033f428: add r0, r4, #0xb0
0033f42c: bl #0x33ed7c
0033f430: add r3, r4, #0xd4
0033f434: mov r0, r3
0033f438: str r3, [r4, #0xe4]
0033f43c: str r3, [r4, #0xe8]
0033f440: mov r1, #0x10
0033f444: bl #0x31167c
0033f448: ldr r3, [r4, #0xe4]
0033f44c: mov r1, r5
0033f450: mov r0, #0xc
0033f454: strb r5, [r3]
0033f458: mov r3, #0
0033f45c: str r3, [r4, #0x114]
0033f460: strb r5, [r4, #0xf0]
0033f464: strb r5, [r4, #0xf1]
0033f468: strb r5, [r4, #0xf8]
0033f46c: str r5, [r4, #0xfc]
0033f470: str r5, [r4, #0x100]
0033f474: str r5, [r4, #0x104]
0033f478: strb r5, [r4, #0x10c]
0033f47c: strb r5, [r4, #0x118]
0033f480: strb r5, [r4, #0x119]
0033f484: str r5, [r4, #0x11c]
0033f488: str r7, [r4, #0xf4]
0033f48c: str r6, [r4, #0x110]
0033f490: str r6, [r4, #0xec]
0033f494: str r6, [r4, #0x108]
0033f498: bl #0x310570
0033f49c: mov r5, r0
0033f4a0: bl #0x33f50c
0033f4a4: str r5, [r4, #0x2c]
0033f4a8: mov r0, r4
0033f4ac: str r4, [r5, #4]
0033f4b0: pop {r4, r5, r6, r7, r8, pc}
0033f4b4: rsbeq r5, r5, r0, ror r7
0033f4b8: andeq r1, r0, ip, lsl #1
0033f4bc: ldrdeq r3, r4, [r0], -ip
0033f4c0: andeq r3, r0, r4, lsl #23
