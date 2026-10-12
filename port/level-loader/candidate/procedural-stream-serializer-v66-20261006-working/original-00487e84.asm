# _ZN3rnd15RandomGeneratorC1Ev 0x487e84 size=344
00487e84: push {r4, r5, lr}
00487e88: mov r4, r0
00487e8c: sub sp, sp, #0xc
00487e90: add r0, r0, #8
00487e94: mov r5, #0
00487e98: bl #0x487dd8 ; _ZN7Array2dIPN3rnd4TileEEC1Ev
00487e9c: mov r2, r4
00487ea0: mov r3, r4
00487ea4: str r5, [r4, #0x40]
00487ea8: strb r5, [r2, #0x3c]!
00487eac: str r2, [r4, #0x48]
00487eb0: str r2, [r4, #0x44]
00487eb4: str r5, [r4, #0x4c]
00487eb8: str r5, [r4, #0x58]
00487ebc: strb r5, [r3, #0x54]!
00487ec0: str r3, [r4, #0x60]
00487ec4: str r3, [r4, #0x5c]
00487ec8: str r5, [r4, #0x64]
00487ecc: mov r1, r4
00487ed0: add r0, r4, #0x6c
00487ed4: bl #0x48dff8 ; _ZN3rnd8RootRuleC1ERNS_15RandomGeneratorE
00487ed8: add r3, r4, #0xfc
00487edc: mov r0, r3
00487ee0: str r3, [r4, #0x10c]
00487ee4: str r3, [r4, #0x110]
00487ee8: mov r1, #0x10
00487eec: bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00487ef0: ldr r2, [r4, #0x10c]
00487ef4: add r3, r4, #0x12c
00487ef8: mov r0, r3
00487efc: strb r5, [r2]
00487f00: mov r1, #0x10
00487f04: str r5, [r4, #0x114]
00487f08: str r5, [r4, #0x118]
00487f0c: str r5, [r4, #0x11c]
00487f10: str r5, [r4, #0x120]
00487f14: str r5, [r4, #0x124]
00487f18: str r5, [r4, #0x128]
00487f1c: str r3, [r4, #0x13c]
00487f20: str r3, [r4, #0x140]
00487f24: bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00487f28: ldr r2, [r4, #0x13c]
00487f2c: add r3, r4, #0x144
00487f30: mov r0, r3
00487f34: strb r5, [r2]
00487f38: mov r1, #0x10
00487f3c: str r3, [r4, #0x154]
00487f40: str r3, [r4, #0x158]
00487f44: bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00487f48: ldr r3, [r4, #0x154]
00487f4c: ldr r1, [pc, #0x80]
00487f50: add r2, sp, #4
00487f54: strb r5, [r3]
00487f58: add r1, pc, r1
00487f5c: add r0, r4, #0x15c
00487f60: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00487f64: add r3, r4, #0x174
00487f68: mov r0, r3
00487f6c: str r3, [r4, #0x184]
00487f70: str r3, [r4, #0x188]
00487f74: mov r1, #0x10
00487f78: bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00487f7c: ldr r3, [r4, #0x184]
00487f80: mov r1, r5
00487f84: mov r0, #0x318
00487f88: strb r5, [r3]
00487f8c: str r5, [r4, #0x18c]
00487f90: bl #0x310570 ; _Znwj15MemoryHintState
00487f94: mov r1, #4
00487f98: mov r5, r0
00487f9c: bl #0x3f51a8 ; _ZN11LevelConfigC1EN10ObjectBase6GO_IDSE
00487fa0: ldr r3, [pc, #0x30]
00487fa4: str r5, [r4, #0x18c]
00487fa8: add r3, pc, r3
00487fac: str r3, [r5, #0x20]
00487fb0: ldr r0, [r4, #0x18c]
00487fb4: add r0, r0, #4
00487fb8: bl #0x513d78 ; _ZN11PropertyMap14InitPropertiesEv
00487fbc: ldr r0, [r4, #0x18c]
00487fc0: add r0, r0, #4
00487fc4: bl #0x5136ec ; _ZN11PropertyMap21LoadDefaultPropertiesEv
00487fc8: mov r0, r4
00487fcc: add sp, sp, #0xc
00487fd0: pop {r4, r5, pc}
00487fd4: subeq lr, r3, r0, lsl #25
00487fd8: subeq r8, r3, r8, asr #9
