
# _ZN14CharProperties10_LoadClassERN7Structs19CharacterPropertiesEib
003e2e20: ldr      ip, [pc, #0x1e0]
003e2e24: cmp      r2, #0
003e2e28: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003e2e2c: add      ip, pc, ip
003e2e30: mov      r8, r0
003e2e34: mov      sl, r1
003e2e38: mov      sb, r3
003e2e3c: blt      #0x3e2ee0
003e2e40: ldr      r3, [pc, #0x1c4]
003e2e44: ldr      r3, [ip, r3]
003e2e48: ldr      r3, [r3]
003e2e4c: cmp      r2, r3
003e2e50: bge      #0x3e2ee0
003e2e54: ldr      r3, [pc, #0x1b4]
003e2e58: mov      r7, #0xc
003e2e5c: ldr      r3, [ip, r3]
003e2e60: ldr      r3, [r3]
003e2e64: mla      r7, r7, r2, r3
003e2e68: ldr      r2, [r7, #4]
003e2e6c: cmp      r2, #0
003e2e70: beq      #0x3e2ee0
003e2e74: mov      r4, #0
003e2e78: mov      r6, r4
003e2e7c: ldr      r5, [r7, #8]
003e2e80: add      r5, r5, r4
003e2e84: ldr      r3, [r5, #8]
003e2e88: cmp      r3, #9
003e2e8c: addls    pc, pc, r3, lsl #2
003e2e90: b        #0x3e2ed0
003e2e94: b        #0x3e2fdc
003e2e98: b        #0x3e2fb0
003e2e9c: b        #0x3e2f88
003e2ea0: b        #0x3e2ed0
003e2ea4: b        #0x3e2f60
003e2ea8: b        #0x3e2f34
003e2eac: b        #0x3e2ee4
003e2eb0: b        #0x3e2ef8
003e2eb4: b        #0x3e2f20
003e2eb8: b        #0x3e2ebc
003e2ebc: mov      r2, r5
003e2ec0: mov      r0, r8
003e2ec4: mov      r1, sl
003e2ec8: bl       #0x3e2bd0
003e2ecc: ldr      r2, [r7, #4]
003e2ed0: add      r6, r6, #1
003e2ed4: cmp      r2, r6
003e2ed8: add      r4, r4, #0x18
003e2edc: bhi      #0x3e2e7c
003e2ee0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003e2ee4: mov      r0, r8
003e2ee8: mov      r1, sl
003e2eec: mov      r2, r5
003e2ef0: mov      r3, sb
003e2ef4: bl       #0x3e2c10
003e2ef8: mov      r2, r5
003e2efc: mov      r0, r8
003e2f00: mov      r1, sl
003e2f04: bl       #0x3e2bdc
003e2f08: ldr      r2, [r7, #4]
003e2f0c: add      r6, r6, #1
003e2f10: add      r4, r4, #0x18
003e2f14: cmp      r2, r6
003e2f18: bhi      #0x3e2e7c
003e2f1c: b        #0x3e2ee0
003e2f20: mov      r0, r8
003e2f24: mov      r1, sl
003e2f28: mov      r2, r5
003e2f2c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003e2f30: b        #0x3e2bcc
003e2f34: mov      r2, r5
003e2f38: mov      r0, r8
003e2f3c: mov      r1, sl
003e2f40: mov      r3, sb
003e2f44: bl       #0x3e2c78
003e2f48: ldr      r2, [r7, #4]
003e2f4c: add      r6, r6, #1
003e2f50: add      r4, r4, #0x18
003e2f54: cmp      r2, r6
003e2f58: bhi      #0x3e2e7c
003e2f5c: b        #0x3e2ee0
003e2f60: mov      r2, r5
003e2f64: mov      r0, r8
003e2f68: mov      r1, sl
003e2f6c: bl       #0x3e2cc0
003e2f70: ldr      r2, [r7, #4]
003e2f74: add      r6, r6, #1
003e2f78: add      r4, r4, #0x18
003e2f7c: cmp      r2, r6
003e2f80: bhi      #0x3e2e7c
003e2f84: b        #0x3e2ee0
003e2f88: mov      r2, r5
003e2f8c: mov      r0, r8
003e2f90: mov      r1, sl
003e2f94: bl       #0x3e2d04
003e2f98: ldr      r2, [r7, #4]
003e2f9c: add      r6, r6, #1
003e2fa0: add      r4, r4, #0x18
003e2fa4: cmp      r2, r6
003e2fa8: bhi      #0x3e2e7c
003e2fac: b        #0x3e2ee0
003e2fb0: mov      r2, r5
003e2fb4: mov      r0, r8
003e2fb8: mov      r1, sl
003e2fbc: mov      r3, sb
003e2fc0: bl       #0x3e2d4c
003e2fc4: ldr      r2, [r7, #4]
003e2fc8: add      r6, r6, #1
003e2fcc: add      r4, r4, #0x18
003e2fd0: cmp      r2, r6
003e2fd4: bhi      #0x3e2e7c
003e2fd8: b        #0x3e2ee0
003e2fdc: mov      r2, r5
003e2fe0: mov      r0, r8
003e2fe4: mov      r1, sl
003e2fe8: mov      r3, sb
003e2fec: bl       #0x3e3014
003e2ff0: ldr      r2, [r7, #4]
003e2ff4: add      r6, r6, #1
003e2ff8: add      r4, r4, #0x18
003e2ffc: cmp      r2, r6
003e3000: bhi      #0x3e2e7c
003e3004: b        #0x3e2ee0
003e3008: subseq   r1, fp, r4, ror #24
003e300c: andeq    r3, r0, r8, ror #10
003e3010: muleq    r0, r0, r4

# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003def34: ldr      r3, [pc, #0x40]
003def38: ldr      r2, [pc, #0x40]
003def3c: push     {r4, r5, r6, r7, r8, lr}
003def40: add      r3, pc, r3
003def44: mov      r8, r0
003def48: ldr      r7, [r3, r2]
003def4c: mov      r6, r1
003def50: mov      r4, #0
003def54: mov      r1, r4
003def58: mov      r0, r8
003def5c: ldr      r5, [r7, r4, lsl #2]
003def60: bl       #0x3def10
003def64: add      r4, r4, #1
003def68: add      r5, r5, #4
003def6c: cmp      r4, #0xe0
003def70: str      r0, [r6, r5]
003def74: bne      #0x3def54
003def78: pop      {r4, r5, r6, r7, r8, pc}
003def7c: subseq   r5, fp, r0, asr fp
003def80: andeq    r2, r0, r8, lsr #5
