# 0x33de64 _ZN18SimpleTypePropertyIbE14IsDefaultValueEPv
0033de64: ldr r2, [r0, #4]
0033de68: ldrb r3, [r0, #0x20]
0033de6c: ldrb r0, [r1, r2]
0033de70: cmp r0, r3
0033de74: movne r0, #0
0033de78: moveq r0, #1
0033de7c: bx lr

# 0x33de80 _ZN18SimpleTypePropertyIbE17SetToDefaultValueEPv
0033de80: ldrb r2, [r0, #0x20]
0033de84: ldr r3, [r0, #4]
0033de88: strb r2, [r1, r3]
0033de8c: bx lr

# 0x33e580 _ZN18SimpleTypePropertyIbE25SetDefaultValueFromStringEPKc
0033e580: mov r3, r0
0033e584: mov r2, #0
0033e588: strb r2, [r3, #0x20]!
0033e58c: mov r0, r1
0033e590: mov r1, r3
0033e594: b #0x30f020

# 0x33e598 _ZN18SimpleTypePropertyIbE10FromStringEPvPKc
0033e598: ldr r3, [r0, #4]
0033e59c: mov r0, #0
0033e5a0: strb r0, [r1, r3]
0033e5a4: add r1, r1, r3
0033e5a8: mov r0, r2
0033e5ac: b #0x30f020

# 0x33e5b0 _ZN18SimpleTypePropertyIbE8ToStringEPv
0033e5b0: push {r4, r5, r6, r7, lr}
0033e5b4: mov r4, r0
0033e5b8: sub sp, sp, #0xc
0033e5bc: mov r0, #0x100
0033e5c0: mov r7, r2
0033e5c4: mov r6, r1
0033e5c8: bl #0x310454
0033e5cc: ldr r1, [r6, #4]
0033e5d0: mov r5, r0
0033e5d4: add r1, r7, r1
0033e5d8: bl #0x30eeb4
0033e5dc: mov r0, r4
0033e5e0: mov r1, r5
0033e5e4: add r2, sp, #4
0033e5e8: bl #0x3140ec
0033e5ec: mov r0, r4
0033e5f0: add sp, sp, #0xc
0033e5f4: pop {r4, r5, r6, r7, pc}

# 0x33eed8 _ZN18SimpleTypePropertyIbE5CloneEv
0033eed8: push {r4, r5, r6, r7, r8, lr}
0033eedc: mov r1, #0
0033eee0: mov r5, r0
0033eee4: mov r0, #0x24
0033eee8: bl #0x310570
0033eeec: ldr r7, [pc, #0x7c]
0033eef0: ldr r3, [pc, #0x7c]
0033eef4: mov r6, r0
0033eef8: add r7, pc, r7
0033eefc: ldr r3, [r7, r3]
0033ef00: mov r4, r0
0033ef04: mov r1, #0x10
0033ef08: add r3, r3, #8
0033ef0c: str r3, [r6], #8
0033ef10: str r6, [r0, #0x18]
0033ef14: str r6, [r0, #0x1c]
0033ef18: mov r0, r6
0033ef1c: bl #0x31167c
0033ef20: ldr r3, [pc, #0x50]
0033ef24: ldr r2, [r4, #0x18]
0033ef28: mov r1, #0
0033ef2c: ldr r3, [r7, r3]
0033ef30: strb r1, [r2]
0033ef34: add r2, r5, #8
0033ef38: add r3, r3, #8
0033ef3c: str r3, [r4]
0033ef40: ldr r3, [r5, #4]
0033ef44: cmp r6, r2
0033ef48: str r3, [r4, #4]
0033ef4c: beq #0x33ef60
0033ef50: mov r0, r6
0033ef54: ldr r1, [r5, #0x1c]
0033ef58: ldr r2, [r5, #0x18]
0033ef5c: bl #0x3109e0
0033ef60: ldrb r3, [r5, #0x20]
0033ef64: mov r0, r4
0033ef68: strb r3, [r4, #0x20]
0033ef6c: pop {r4, r5, r6, r7, r8, pc}
0033ef70: mlseq r5, r8, fp, r5
0033ef74: andeq r2, r0, r0, lsr r3
0033ef78: andeq r3, r0, ip, asr #28

# 0x38ae00 _ZN18SimpleTypePropertyIiE14IsDefaultValueEPv
0038ae00: ldr r2, [r0, #4]
0038ae04: ldr r3, [r0, #0x20]
0038ae08: ldr r0, [r1, r2]
0038ae0c: cmp r0, r3
0038ae10: movne r0, #0
0038ae14: moveq r0, #1
0038ae18: bx lr

# 0x38ae1c _ZN18SimpleTypePropertyIiE17SetToDefaultValueEPv
0038ae1c: ldr r2, [r0, #0x20]
0038ae20: ldr r3, [r0, #4]
0038ae24: str r2, [r1, r3]
0038ae28: bx lr

# 0x38ba94 _ZN18SimpleTypePropertyIiE25SetDefaultValueFromStringEPKc
0038ba94: mov r3, r0
0038ba98: mov r2, #0
0038ba9c: str r2, [r3, #0x20]!
0038baa0: mov r0, r1
0038baa4: mov r1, r3
0038baa8: b #0x30f03c

# 0x38baac _ZN18SimpleTypePropertyIiE10FromStringEPvPKc
0038baac: ldr r3, [r0, #4]
0038bab0: mov r0, #0
0038bab4: str r0, [r1, r3]
0038bab8: add r1, r1, r3
0038babc: mov r0, r2
0038bac0: b #0x30f03c

# 0x38bac4 _ZN18SimpleTypePropertyIiE8ToStringEPv
0038bac4: push {r4, r5, r6, r7, lr}
0038bac8: mov r4, r0
0038bacc: sub sp, sp, #0xc
0038bad0: mov r0, #0x100
0038bad4: mov r7, r2
0038bad8: mov r6, r1
0038badc: bl #0x310454
0038bae0: ldr r1, [r6, #4]
0038bae4: mov r5, r0
0038bae8: add r1, r7, r1
0038baec: bl #0x30eea0
0038baf0: mov r0, r4
0038baf4: mov r1, r5
0038baf8: add r2, sp, #4
0038bafc: bl #0x3140ec
0038bb00: mov r0, r4
0038bb04: add sp, sp, #0xc
0038bb08: pop {r4, r5, r6, r7, pc}

# 0x38c08c _ZN18SimpleTypePropertyIiE5CloneEv
0038c08c: push {r4, r5, r6, r7, r8, lr}
0038c090: mov r1, #0
0038c094: mov r5, r0
0038c098: mov r0, #0x24
0038c09c: bl #0x310570
0038c0a0: ldr r7, [pc, #0x7c]
0038c0a4: ldr r3, [pc, #0x7c]
0038c0a8: mov r6, r0
0038c0ac: add r7, pc, r7
0038c0b0: ldr r3, [r7, r3]
0038c0b4: mov r4, r0
0038c0b8: mov r1, #0x10
0038c0bc: add r3, r3, #8
0038c0c0: str r3, [r6], #8
0038c0c4: str r6, [r0, #0x18]
0038c0c8: str r6, [r0, #0x1c]
0038c0cc: mov r0, r6
0038c0d0: bl #0x31167c
0038c0d4: ldr r3, [pc, #0x50]
0038c0d8: ldr r2, [r4, #0x18]
0038c0dc: mov r1, #0
0038c0e0: ldr r3, [r7, r3]
0038c0e4: strb r1, [r2]
0038c0e8: add r2, r5, #8
0038c0ec: add r3, r3, #8
0038c0f0: str r3, [r4]
0038c0f4: ldr r3, [r5, #4]
0038c0f8: cmp r6, r2
0038c0fc: str r3, [r4, #4]
0038c100: beq #0x38c114
0038c104: mov r0, r6
0038c108: ldr r1, [r5, #0x1c]
0038c10c: ldr r2, [r5, #0x18]
0038c110: bl #0x3109e0
0038c114: ldr r3, [r5, #0x20]
0038c118: mov r0, r4
0038c11c: str r3, [r4, #0x20]
0038c120: pop {r4, r5, r6, r7, r8, pc}
0038c124: rsbeq r8, r0, r4, ror #19
0038c128: andeq r2, r0, r0, lsr r3
0038c12c: muleq r0, r0, r5

# 0x3f03d8 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE14IsDefaultValueEPv
003f03d8: mov r3, r0
003f03dc: ldr r0, [r0, #4]
003f03e0: add r0, r1, r0
003f03e4: add r1, r3, #0x20
003f03e8: b #0x3f0344

# 0x3f3858 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE8ToStringEPv
003f3858: push {r4, r5, r6, r7, lr}
003f385c: mov r4, r0
003f3860: sub sp, sp, #0xc
003f3864: mov r0, #0x100
003f3868: mov r7, r2
003f386c: mov r6, r1
003f3870: bl #0x310454
003f3874: ldr r1, [r6, #4]
003f3878: mov r5, r0
003f387c: add r1, r7, r1
003f3880: bl #0x30ee60
003f3884: mov r0, r4
003f3888: mov r1, r5
003f388c: add r2, sp, #4
003f3890: bl #0x3140ec
003f3894: mov r0, r4
003f3898: add sp, sp, #0xc
003f389c: pop {r4, r5, r6, r7, pc}

# 0x3f8eb4 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE17SetToDefaultValueEPv
003f8eb4: mov r3, r0
003f8eb8: ldr r0, [r0, #4]
003f8ebc: add r0, r1, r0
003f8ec0: add r1, r3, #0x20
003f8ec4: b #0x3f8c40

# 0x3f8ec8 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE5CloneEv
003f8ec8: push {r4, r5, r6, r7, r8, lr}
003f8ecc: mov r1, #0
003f8ed0: mov r5, r0
003f8ed4: mov r0, #0x2c
003f8ed8: bl #0x310570
003f8edc: ldr r7, [pc, #0x8c]
003f8ee0: ldr r3, [pc, #0x8c]
003f8ee4: mov r6, r0
003f8ee8: add r7, pc, r7
003f8eec: ldr r3, [r7, r3]
003f8ef0: mov r4, r0
003f8ef4: mov r1, #0x10
003f8ef8: add r3, r3, #8
003f8efc: str r3, [r6], #8
003f8f00: str r6, [r0, #0x18]
003f8f04: str r6, [r0, #0x1c]
003f8f08: mov r0, r6
003f8f0c: bl #0x31167c
003f8f10: ldr r2, [pc, #0x60]
003f8f14: ldr r1, [r4, #0x18]
003f8f18: mov r3, #0
003f8f1c: ldr r2, [r7, r2]
003f8f20: strb r3, [r1]
003f8f24: str r3, [r4, #0x28]
003f8f28: add r2, r2, #8
003f8f2c: str r2, [r4]
003f8f30: str r3, [r4, #0x20]
003f8f34: str r3, [r4, #0x24]
003f8f38: ldr r3, [r5, #4]
003f8f3c: add r2, r5, #8
003f8f40: cmp r6, r2
003f8f44: str r3, [r4, #4]
003f8f48: beq #0x3f8f5c
003f8f4c: mov r0, r6
003f8f50: ldr r1, [r5, #0x1c]
003f8f54: ldr r2, [r5, #0x18]
003f8f58: bl #0x3109e0
003f8f5c: add r1, r5, #0x20
003f8f60: add r0, r4, #0x20
003f8f64: bl #0x3f8c40
003f8f68: mov r0, r4
003f8f6c: pop {r4, r5, r6, r7, r8, pc}
003f8f70: subseq fp, sb, r8, lsr #23
003f8f74: andeq r2, r0, r0, lsr r3
003f8f78: muleq r0, r8, r6

# 0x3f8f7c _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE25SetDefaultValueFromStringEPKc
003f8f7c: push {r4, r5, r6, lr}
003f8f80: sub sp, sp, #0x10
003f8f84: add r4, r0, #0x20
003f8f88: add r5, sp, #4
003f8f8c: mov r3, #0
003f8f90: mov r6, r1
003f8f94: mov r0, r4
003f8f98: mov r1, r5
003f8f9c: str r3, [sp, #0xc]
003f8fa0: str r3, [sp, #4]
003f8fa4: str r3, [sp, #8]
003f8fa8: bl #0x3f8c40
003f8fac: mov r0, r5
003f8fb0: bl #0x34611c
003f8fb4: mov r0, r6
003f8fb8: mov r1, r4
003f8fbc: bl #0x30f010
003f8fc0: add sp, sp, #0x10
003f8fc4: pop {r4, r5, r6, pc}

# 0x3f8fc8 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE10FromStringEPvPKc
003f8fc8: push {r4, r5, r6, lr}
003f8fcc: ldr r4, [r0, #4]
003f8fd0: sub sp, sp, #0x10
003f8fd4: add r5, sp, #4
003f8fd8: add r4, r1, r4
003f8fdc: mov r3, #0
003f8fe0: mov r1, r5
003f8fe4: mov r0, r4
003f8fe8: mov r6, r2
003f8fec: str r3, [sp, #0xc]
003f8ff0: str r3, [sp, #4]
003f8ff4: str r3, [sp, #8]
003f8ff8: bl #0x3f8c40
003f8ffc: mov r0, r5
003f9000: bl #0x34611c
003f9004: mov r0, r6
003f9008: mov r1, r4
003f900c: bl #0x30f010
003f9010: add sp, sp, #0x10
003f9014: pop {r4, r5, r6, pc}
