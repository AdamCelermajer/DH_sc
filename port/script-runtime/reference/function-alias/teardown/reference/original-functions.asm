
# _ZN9LuaScriptD2Ev
0037bec0: push     {r4, r5, r6, lr}
0037bec4: ldr      r5, [pc, #0x12c]
0037bec8: ldr      r3, [pc, #0x12c]
0037becc: ldr      r2, [r0, #0x90]
0037bed0: add      r5, pc, r5
0037bed4: ldr      r3, [r5, r3]
0037bed8: cmp      r2, #0
0037bedc: mov      r4, r0
0037bee0: add      r3, r3, #8
0037bee4: str      r3, [r0]
0037bee8: bne      #0x37bfc8
0037beec: add      r3, r4, #0x68
0037bef0: ldr      r0, [r3, #0x14]
0037bef4: cmp      r0, r3
0037bef8: beq      #0x37bf18
0037befc: cmp      r0, #0
0037bf00: beq      #0x37bf18
0037bf04: ldr      r1, [r4, #0x68]
0037bf08: rsb      r1, r0, r1
0037bf0c: cmp      r1, #0x80
0037bf10: bhi      #0x37bff0
0037bf14: bl       #0x708f00
0037bf18: ldr      r3, [r4, #0x5c]
0037bf1c: cmp      r3, #0
0037bf20: beq      #0x37bf48
0037bf24: add      r6, r4, #0x4c
0037bf28: mov      r0, r6
0037bf2c: ldr      r1, [r4, #0x50]
0037bf30: bl       #0x37bd7c
0037bf34: mov      r3, #0
0037bf38: str      r6, [r4, #0x58]
0037bf3c: str      r3, [r4, #0x5c]
0037bf40: str      r6, [r4, #0x54]
0037bf44: str      r3, [r4, #0x50]
0037bf48: ldr      r3, [r4, #0x44]
0037bf4c: cmp      r3, #0
0037bf50: beq      #0x37bf78
0037bf54: add      r6, r4, #0x34
0037bf58: mov      r0, r6
0037bf5c: ldr      r1, [r4, #0x38]
0037bf60: bl       #0x37bd7c
0037bf64: mov      r3, #0
0037bf68: str      r6, [r4, #0x40]
0037bf6c: str      r3, [r4, #0x44]
0037bf70: str      r6, [r4, #0x3c]
0037bf74: str      r3, [r4, #0x38]
0037bf78: ldr      r3, [r4, #0x2c]
0037bf7c: cmp      r3, #0
0037bf80: beq      #0x37bfa8
0037bf84: add      r6, r4, #0x1c
0037bf88: mov      r0, r6
0037bf8c: ldr      r1, [r4, #0x20]
0037bf90: bl       #0x37bcc0
0037bf94: mov      r3, #0
0037bf98: str      r6, [r4, #0x28]
0037bf9c: str      r3, [r4, #0x2c]
0037bfa0: str      r6, [r4, #0x24]
0037bfa4: str      r3, [r4, #0x20]
0037bfa8: ldr      r3, [pc, #0x50]
0037bfac: add      r0, r4, #4
0037bfb0: ldr      r3, [r5, r3]
0037bfb4: add      r3, r3, #8
0037bfb8: str      r3, [r4, #0x10]
0037bfbc: bl       #0x31b180
0037bfc0: mov      r0, r4
0037bfc4: pop      {r4, r5, r6, pc}
0037bfc8: add      r6, r0, #0x80
0037bfcc: mov      r0, r6
0037bfd0: ldr      r1, [r4, #0x84]
0037bfd4: bl       #0x37bcf8
0037bfd8: mov      r3, #0
0037bfdc: str      r6, [r4, #0x8c]
0037bfe0: str      r3, [r4, #0x90]
0037bfe4: str      r6, [r4, #0x88]
0037bfe8: str      r3, [r4, #0x84]
0037bfec: b        #0x37beec
0037bff0: bl       #0x310440
0037bff4: b        #0x37bf18
0037bff8: rsbeq    r8, r1, r0, asr #23
0037bffc: andeq    r1, r0, r4, ror r6
0037c000: andeq    r3, r0, r8, asr r6

# _ZN9LuaScriptD1Ev
0037c004: push     {r4, r5, r6, lr}
0037c008: ldr      r5, [pc, #0x12c]
0037c00c: ldr      r3, [pc, #0x12c]
0037c010: ldr      r2, [r0, #0x90]
0037c014: add      r5, pc, r5
0037c018: ldr      r3, [r5, r3]
0037c01c: cmp      r2, #0
0037c020: mov      r4, r0
0037c024: add      r3, r3, #8
0037c028: str      r3, [r0]
0037c02c: bne      #0x37c10c
0037c030: add      r3, r4, #0x68
0037c034: ldr      r0, [r3, #0x14]
0037c038: cmp      r0, r3
0037c03c: beq      #0x37c05c
0037c040: cmp      r0, #0
0037c044: beq      #0x37c05c
0037c048: ldr      r1, [r4, #0x68]
0037c04c: rsb      r1, r0, r1
0037c050: cmp      r1, #0x80
0037c054: bhi      #0x37c134
0037c058: bl       #0x708f00
0037c05c: ldr      r3, [r4, #0x5c]
0037c060: cmp      r3, #0
0037c064: beq      #0x37c08c
0037c068: add      r6, r4, #0x4c
0037c06c: mov      r0, r6
0037c070: ldr      r1, [r4, #0x50]
0037c074: bl       #0x37bd7c
0037c078: mov      r3, #0
0037c07c: str      r6, [r4, #0x58]
0037c080: str      r3, [r4, #0x5c]
0037c084: str      r6, [r4, #0x54]
0037c088: str      r3, [r4, #0x50]
0037c08c: ldr      r3, [r4, #0x44]
0037c090: cmp      r3, #0
0037c094: beq      #0x37c0bc
0037c098: add      r6, r4, #0x34
0037c09c: mov      r0, r6
0037c0a0: ldr      r1, [r4, #0x38]
0037c0a4: bl       #0x37bd7c
0037c0a8: mov      r3, #0
0037c0ac: str      r6, [r4, #0x40]
0037c0b0: str      r3, [r4, #0x44]
0037c0b4: str      r6, [r4, #0x3c]
0037c0b8: str      r3, [r4, #0x38]
0037c0bc: ldr      r3, [r4, #0x2c]
0037c0c0: cmp      r3, #0
0037c0c4: beq      #0x37c0ec
0037c0c8: add      r6, r4, #0x1c
0037c0cc: mov      r0, r6
0037c0d0: ldr      r1, [r4, #0x20]
0037c0d4: bl       #0x37bcc0
0037c0d8: mov      r3, #0
0037c0dc: str      r6, [r4, #0x28]
0037c0e0: str      r3, [r4, #0x2c]
0037c0e4: str      r6, [r4, #0x24]
0037c0e8: str      r3, [r4, #0x20]
0037c0ec: ldr      r3, [pc, #0x50]
0037c0f0: add      r0, r4, #4
0037c0f4: ldr      r3, [r5, r3]
0037c0f8: add      r3, r3, #8
0037c0fc: str      r3, [r4, #0x10]
0037c100: bl       #0x31b180
0037c104: mov      r0, r4
0037c108: pop      {r4, r5, r6, pc}
0037c10c: add      r6, r0, #0x80
0037c110: mov      r0, r6
0037c114: ldr      r1, [r4, #0x84]
0037c118: bl       #0x37bcf8
0037c11c: mov      r3, #0
0037c120: str      r6, [r4, #0x8c]
0037c124: str      r3, [r4, #0x90]
0037c128: str      r6, [r4, #0x88]
0037c12c: str      r3, [r4, #0x84]
0037c130: b        #0x37c030
0037c134: bl       #0x310440
0037c138: b        #0x37c05c
0037c13c: rsbeq    r8, r1, ip, ror sl
0037c140: andeq    r1, r0, r4, ror r6
0037c144: andeq    r3, r0, r8, asr r6
