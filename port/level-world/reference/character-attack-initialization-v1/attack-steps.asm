_ZN12CharAnimator17ANIM_SkipNextStepEv 003c9464 size 8
003c9464: ldr r1, [r0, #0x2c]
003c9468: b #0x3c9444
_ZN6CharAI17AI_SyncLastTargetEv 003d49c4 size 12
003d49c4: ldr r3, [r0, #0x40]
003d49c8: str r3, [r0, #0x44]
003d49cc: bx lr
_ZN6CharAIC2Ev 003cebf0 size 352
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
_ZN6CharAI23_OnAnimStepBegin_AttackEv 003d4044 size 220
003d4044: push {r4, r5, r6, lr}
003d4048: ldr r3, [r0, #4]
003d404c: mov r4, r0
003d4050: add r0, r3, #0x490
003d4054: add r0, r0, #0xc
003d4058: ldr r5, [r3, #0x4c8]
003d405c: bl #0x3c932c
003d4060: mov r6, r0
003d4064: ldr r0, [r4, #4]
003d4068: add r0, r0, #0x490
003d406c: add r0, r0, #0xc
003d4070: bl #0x3c934c
003d4074: cmp r5, #0
003d4078: beq #0x3d40c8
003d407c: cmp r5, #1
003d4080: beq #0x3d408c
003d4084: mov r0, #1
003d4088: pop {r4, r5, r6, pc}
003d408c: cmp r6, #0
003d4090: bne #0x3d40e4
003d4094: ldr r3, [r4, #4]
003d4098: strb r5, [r4, #0x79]
003d409c: ldr r1, [r3, #0x408]
003d40a0: ldr r0, [r3, #0x378]
003d40a4: bl #0x4052bc
003d40a8: mov r0, r4
003d40ac: strb r6, [r4, #0x7a]
003d40b0: ldr r3, [r4]
003d40b4: ldr r1, [r4, #0x74]
003d40b8: mov lr, pc
003d40bc: ldr pc, [r3, #0xa4]
003d40c0: mov r0, #1
003d40c4: pop {r4, r5, r6, pc}
003d40c8: ldr r0, [r4, #4]
003d40cc: str r6, [r4, #0x74]
003d40d0: mov r2, r6
003d40d4: mov r1, #0x1a
003d40d8: bl #0x3a4d5c
003d40dc: mov r0, #1
003d40e0: pop {r4, r5, r6, pc}
003d40e4: ldr r3, [r4, #4]
003d40e8: sub r0, r0, #1
003d40ec: cmp r6, r0
003d40f0: movne r6, #0
003d40f4: moveq r6, #1
003d40f8: strb r6, [r4, #0x79]
003d40fc: ldr r1, [r3, #0x408]
003d4100: ldr r0, [r3, #0x378]
003d4104: bl #0x4052bc
003d4108: cmp r6, #0
003d410c: mov r3, #0
003d4110: strb r3, [r4, #0x7a]
003d4114: mov r0, #1
003d4118: strbne r5, [r4, #0x7a]
003d411c: pop {r4, r5, r6, pc}
_ZNK9Character14HasComboAttackEv 003a346c size 132
003a346c: push {r4, r5, r6, lr}
003a3470: ldr r4, [pc, #0x68]
003a3474: ldr r3, [pc, #0x68]
003a3478: add r4, pc, r4
003a347c: ldr r3, [r4, r3]
003a3480: ldr r5, [r3]
003a3484: bl #0x3a3228
003a3488: mov r3, #0xa0
003a348c: mla r5, r3, r0, r5
003a3490: ldr r3, [r5, #4]
003a3494: cmp r3, #0
003a3498: blt #0x3a34d8
003a349c: ldr r2, [pc, #0x44]
003a34a0: ldr r2, [r4, r2]
003a34a4: ldr r2, [r2]
003a34a8: cmp r3, r2
003a34ac: bge #0x3a34d8
003a34b0: ldr r2, [pc, #0x34]
003a34b4: mov r1, #0x14
003a34b8: ldr r2, [r4, r2]
003a34bc: ldr r2, [r2]
003a34c0: mla r3, r1, r3, r2
003a34c4: ldr r0, [r3, #0x10]
003a34c8: cmp r0, #1
003a34cc: movne r0, #0
003a34d0: moveq r0, #1
003a34d4: pop {r4, r5, r6, pc}
003a34d8: mov r0, #0
003a34dc: pop {r4, r5, r6, pc}
003a34e0: subseq r1, pc, r8, lsl r6
003a34e4: andeq r4, r0, r4, asr #16
003a34e8: andeq r2, r0, r8, asr #20
003a34ec: andeq r3, r0, ip, ror ip
_ZN6CharAI21_OnAnimStepEnd_AttackEv 003d3e44 size 436
003d3e44: push {r4, r5, r6, r7, r8, lr}
003d3e48: mov r4, r0
003d3e4c: ldr r0, [r0, #4]
003d3e50: bl #0x3a346c
003d3e54: cmp r0, #0
003d3e58: bne #0x3d3e64
003d3e5c: mov r0, #1
003d3e60: pop {r4, r5, r6, r7, r8, pc}
003d3e64: ldr r3, [r4, #4]
003d3e68: add r0, r3, #0x490
003d3e6c: add r0, r0, #0xc
003d3e70: ldr r5, [r3, #0x4c8]
003d3e74: bl #0x3c932c
003d3e78: mov r7, r0
003d3e7c: ldr r0, [r4, #4]
003d3e80: add r0, r0, #0x490
003d3e84: add r0, r0, #0xc
003d3e88: bl #0x3c934c
003d3e8c: cmp r5, #0
003d3e90: mov r6, r0
003d3e94: bne #0x3d3ed0
003d3e98: ldrb r1, [r4, #0x78]
003d3e9c: sub r3, r0, #1
003d3ea0: cmp r7, r3
003d3ea4: strb r5, [r4, #0x78]
003d3ea8: eor r1, r1, #1
003d3eac: beq #0x3d3f20
003d3eb0: cmp r1, #0
003d3eb4: bne #0x3d3f68
003d3eb8: ldr r0, [r4, #4]
003d3ebc: mov r1, #0x1b
003d3ec0: mov r2, #0
003d3ec4: bl #0x3a4d5c
003d3ec8: mov r0, #1
003d3ecc: pop {r4, r5, r6, r7, r8, pc}
003d3ed0: cmp r5, #1
003d3ed4: bne #0x3d3e5c
003d3ed8: ldr r3, [r4, #0x40]
003d3edc: cmp r3, #0
003d3ee0: moveq r0, r5
003d3ee4: beq #0x3d3fc8
003d3ee8: mov r0, r3
003d3eec: ldr r3, [r3]
003d3ef0: mov lr, pc
003d3ef4: ldr pc, [r3, #0x34]
003d3ef8: ldr r3, [r4, #0x40]
003d3efc: cmp r3, #0
003d3f00: beq #0x3d3fc8
003d3f04: mov r2, #0
003d3f08: sub r6, r6, #2
003d3f0c: cmp r7, r6
003d3f10: beq #0x3d3fa4
003d3f14: mov r3, #0
003d3f18: strb r3, [r4, #0x78]
003d3f1c: b #0x3d3e5c
003d3f20: cmp r1, #0
003d3f24: beq #0x3d3f54
003d3f28: mov r0, r4
003d3f2c: bl #0x3d8d70
003d3f30: ldr r0, [r4, #4]
003d3f34: mov r1, #0x1b
003d3f38: mov r2, #0
003d3f3c: bl #0x3a4d5c
003d3f40: ldr r0, [r4, #4]
003d3f44: mov r1, #0x1c
003d3f48: mov r2, #0
003d3f4c: bl #0x3a4d5c
003d3f50: b #0x3d3e5c
003d3f54: ldr r0, [r4, #4]
003d3f58: add r0, r0, #0x490
003d3f5c: add r0, r0, #0xc
003d3f60: bl #0x3c9484
003d3f64: b #0x3d3f30
003d3f68: ldr r3, [r4, #4]
003d3f6c: mov r0, r3
003d3f70: ldr r3, [r3]
003d3f74: mov lr, pc
003d3f78: ldr pc, [r3, #0x124]
003d3f7c: cmp r0, #0
003d3f80: bne #0x3d3eb8
003d3f84: mov r0, r4
003d3f88: bl #0x3d8d70
003d3f8c: ldr r0, [r4, #4]
003d3f90: mov r1, r6
003d3f94: add r0, r0, #0x490
003d3f98: add r0, r0, #0xc
003d3f9c: bl #0x3c9484
003d3fa0: b #0x3d3eb8
003d3fa4: cmp r0, #0
003d3fa8: bne #0x3d3fec
003d3fac: cmp r2, #0
003d3fb0: bne #0x3d3f14
003d3fb4: ldr r0, [r4, #4]
003d3fb8: add r0, r0, #0x490
003d3fbc: add r0, r0, #0xc
003d3fc0: bl #0x3c9464
003d3fc4: b #0x3d3e5c
003d3fc8: ldr r2, [r4, #4]
003d3fcc: movw r3, #0x14a8
003d3fd0: ldrsb r3, [r2, r3]
003d3fd4: cmp r3, #8
003d3fd8: moveq r3, #0
003d3fdc: moveq r2, #1
003d3fe0: beq #0x3d3f08
003d3fe4: mov r3, #0
003d3fe8: b #0x3d3f04
003d3fec: cmp r3, #0
003d3ff0: bne #0x3d3f14
003d3ff4: b #0x3d3fac
_ZN12CharAnimator12ANIM_SetStepEj 003c9484 size 8
003c9484: ldr r2, [r0, #0x2c]
003c9488: b #0x3c946c
_ZN6CharAI21_ClearNonStickyTargetEv 003d8d70 size 44
003d8d70: push {r4, lr}
003d8d74: ldrb r1, [r0, #0x4b]
003d8d78: mov r4, r0
003d8d7c: cmp r1, #0
003d8d80: beq #0x3d8d88
003d8d84: pop {r4, pc}
003d8d88: mov r2, r1
003d8d8c: bl #0x3d6890
003d8d90: mov r0, r4
003d8d94: pop {r4, lr}
003d8d98: b #0x3d49c4
