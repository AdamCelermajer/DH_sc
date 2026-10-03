
# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6
