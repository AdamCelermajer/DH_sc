
# _ZNK9Character14CanRangeAttackEv
003a4d3c: movw     r3, #0x1078
003a4d40: ldr      r3, [r0, r3]
003a4d44: cmn      r3, #1
003a4d48: beq      #0x3a4d54
003a4d4c: mov      r0, #1
003a4d50: bx       lr
003a4d54: add      r0, r0, #0x37c
003a4d58: b        #0x400014

# _ZNK9Character14CanRangeAttackERiS0_S0_
003a4cd0: push     {r4, r5, r6, r7}
003a4cd4: movw     r4, #0x1078
003a4cd8: mov      ip, r0
003a4cdc: ldr      r0, [r0, r4]
003a4ce0: mov      r6, r1
003a4ce4: mov      r5, r2
003a4ce8: cmn      r0, #1
003a4cec: mov      r7, r3
003a4cf0: beq      #0x3a4d28
003a4cf4: movw     r3, #0x1070
003a4cf8: ldr      r3, [ip, r3]
003a4cfc: mov      r0, #1
003a4d00: asr      r3, r3, #8
003a4d04: str      r3, [r1]
003a4d08: movw     r3, #0x1074
003a4d0c: ldr      r3, [ip, r3]
003a4d10: asr      r3, r3, #8
003a4d14: str      r3, [r2]
003a4d18: ldr      r3, [ip, r4]
003a4d1c: str      r3, [r7]
003a4d20: pop      {r4, r5, r6, r7}
003a4d24: bx       lr
003a4d28: add      r0, ip, #0x37c
003a4d2c: pop      {r4, r5, r6, r7}
003a4d30: b        #0x3ffebc
