00312e00: ldr r2, [pc, #0x7c]
00312e04: ldr r3, [pc, #0x7c]
00312e08: push {r4, r5, r6, r7}
00312e0c: add r2, pc, r2
00312e10: ldr r0, [r2, r3]
00312e14: ldr r3, [pc, #0x70]
00312e18: ldr r1, [pc, #0x70]
00312e1c: mov r7, #0x3f000000
00312e20: ldr r6, [r2, r3]
00312e24: ldr r3, [pc, #0x68]
00312e28: mov r4, #0x3f800000
00312e2c: add r1, pc, r1
00312e30: ldr r5, [r2, r3]
00312e34: ldr r3, [pc, #0x5c]
00312e38: str r7, [r1, #8]
00312e3c: str r4, [r0, #8]
00312e40: ldr ip, [r2, r3]
00312e44: mov r3, #0
00312e48: str r3, [r6, #8]
00312e4c: str r3, [r5, #8]
00312e50: str r3, [ip, #8]
00312e54: str r3, [r0, #4]
00312e58: str r7, [r1]
00312e5c: str r7, [r1, #4]
00312e60: str r3, [r6]
00312e64: str r3, [r6, #4]
00312e68: str r4, [r5]
00312e6c: str r3, [r5, #4]
00312e70: str r3, [ip]
00312e74: str r4, [ip, #4]
00312e78: str r3, [r0]
00312e7c: pop {r4, r5, r6, r7}
00312e80: bx lr
00312e84: rsbeq r1, r8, r4, lsl #25
00312e88: andeq r4, r0, r0, asr #6
00312e8c: andeq r3, r0, ip, lsr #30
00312e90: rsbeq ip, r8, r4, lsl sl
00312e94: andeq r3, r0, r8, lsr r2
00312e98: ldrdeq r4, r5, [r0], -r8
