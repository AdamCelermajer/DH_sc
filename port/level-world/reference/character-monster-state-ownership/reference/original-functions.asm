
# _ZN6CharAI13_UpdateTargetEv
003cb908: push     {r4, r5, r6, lr}
003cb90c: mov      r4, r0
003cb910: ldr      r0, [r0, #4]
003cb914: add      r0, r0, #0x4f0
003cb918: add      r0, r0, #0xc
003cb91c: bl       #0x3c0230
003cb920: cmp      r0, #0
003cb924: beq      #0x3cb92c
003cb928: pop      {r4, r5, r6, pc}
003cb92c: ldr      r0, [r4, #4]
003cb930: add      r0, r0, #0x4f0
003cb934: add      r0, r0, #0xc
003cb938: bl       #0x3c01c0
003cb93c: cmp      r0, #0
003cb940: bne      #0x3cb928
003cb944: ldr      r3, [r4, #0x40]
003cb948: cmp      r3, #0
003cb94c: beq      #0x3cb928
003cb950: mov      r0, r3
003cb954: ldr      r1, [r4, #4]
003cb958: ldr      r3, [r3]
003cb95c: mov      lr, pc
003cb960: ldr      pc, [r3, #0x88]
003cb964: subs     r2, r0, #0
003cb968: beq      #0x3cbaa4
003cb96c: ldr      r3, [r4, #0x40]
003cb970: cmp      r3, #0
003cb974: beq      #0x3cb928
003cb978: ldr      r0, [r4, #4]
003cb97c: bl       #0x3a2fec
003cb980: ldr      r3, [r4, #0x40]
003cb984: mov      r0, r3
003cb988: ldr      r3, [r3]
003cb98c: mov      lr, pc
003cb990: ldr      pc, [r3, #0x34]
003cb994: ldrb     r3, [r4, #0x48]
003cb998: eor      r0, r0, #1
003cb99c: uxtb     r5, r0
003cb9a0: cmp      r3, #0
003cb9a4: bne      #0x3cba6c
003cb9a8: cmp      r5, #0
003cb9ac: bne      #0x3cbaf8
003cb9b0: ldr      r1, [r4, #0x40]
003cb9b4: strb     r5, [r4, #0x48]
003cb9b8: cmp      r1, #0
003cb9bc: beq      #0x3cb928
003cb9c0: mov      r0, r4
003cb9c4: bl       #0x3d4ed8
003cb9c8: ldrb     r3, [r4, #0x49]
003cb9cc: mov      r5, r0
003cb9d0: cmp      r3, #0
003cb9d4: bne      #0x3cba88
003cb9d8: cmp      r0, #0
003cb9dc: bne      #0x3cbae4
003cb9e0: ldr      r3, [r4, #0x40]
003cb9e4: strb     r5, [r4, #0x49]
003cb9e8: cmp      r3, #0
003cb9ec: beq      #0x3cb928
003cb9f0: cmp      r5, #0
003cb9f4: beq      #0x3cb928
003cb9f8: mov      r0, r3
003cb9fc: ldr      r1, [r4, #4]
003cba00: ldr      r3, [r3]
003cba04: mov      lr, pc
003cba08: ldr      pc, [r3, #0x88]
003cba0c: cmp      r0, #0
003cba10: beq      #0x3cb928
003cba14: ldr      r3, [r4, #4]
003cba18: mov      r0, r3
003cba1c: ldr      r3, [r3]
003cba20: mov      lr, pc
003cba24: ldr      pc, [r3, #0x124]
003cba28: cmp      r0, #0
003cba2c: beq      #0x3cbabc
003cba30: mov      r0, r4
003cba34: ldr      r1, [r4, #0x40]
003cba38: bl       #0x3d63d8
003cba3c: cmp      r0, #0
003cba40: bne      #0x3cbb0c
003cba44: mov      r0, r4
003cba48: ldr      r1, [r4, #0x40]
003cba4c: bl       #0x3d6604
003cba50: cmp      r0, #0
003cba54: beq      #0x3cbad0
003cba58: ldr      r2, [r4, #0x40]
003cba5c: ldr      r0, [r4, #4]
003cba60: mov      r1, #0xf
003cba64: pop      {r4, r5, r6, lr}
003cba68: b        #0x3a4d5c
003cba6c: cmp      r5, #0
003cba70: bne      #0x3cb9b0
003cba74: ldr      r0, [r4, #4]
003cba78: mov      r1, #0xa
003cba7c: ldr      r2, [r4, #0x40]
003cba80: bl       #0x3a4d5c
003cba84: b        #0x3cb9b0
003cba88: cmp      r0, #0
003cba8c: bne      #0x3cb9e0
003cba90: ldr      r0, [r4, #4]
003cba94: mov      r1, #0xc
003cba98: ldr      r2, [r4, #0x40]
003cba9c: bl       #0x3a4d5c
003cbaa0: b        #0x3cb9e0
003cbaa4: ldr      r0, [r4, #4]
003cbaa8: mov      r1, #0xc
003cbaac: str      r2, [r4, #0x40]
003cbab0: str      r2, [r4, #0x44]
003cbab4: pop      {r4, r5, r6, lr}
003cbab8: b        #0x3a4d5c
003cbabc: mov      r0, r4
003cbac0: ldr      r1, [r4, #0x40]
003cbac4: bl       #0x3d6188
003cbac8: cmp      r0, #0
003cbacc: bne      #0x3cbb20
003cbad0: ldr      r2, [r4, #0x40]
003cbad4: ldr      r0, [r4, #4]
003cbad8: mov      r1, #0xe
003cbadc: pop      {r4, r5, r6, lr}
003cbae0: b        #0x3a4d5c
003cbae4: ldr      r0, [r4, #4]
003cbae8: mov      r1, #0xd
003cbaec: ldr      r2, [r4, #0x40]
003cbaf0: bl       #0x3a4d5c
003cbaf4: b        #0x3cb9e0
003cbaf8: ldr      r0, [r4, #4]
003cbafc: mov      r1, #0xb
003cbb00: ldr      r2, [r4, #0x40]
003cbb04: bl       #0x3a4d5c
003cbb08: b        #0x3cb9b0
003cbb0c: ldr      r2, [r4, #0x40]
003cbb10: ldr      r0, [r4, #4]
003cbb14: mov      r1, #0x10
003cbb18: pop      {r4, r5, r6, lr}
003cbb1c: b        #0x3a4d5c
003cbb20: ldr      r2, [r4, #0x40]
003cbb24: ldr      r0, [r4, #4]
003cbb28: mov      r1, #0x11
003cbb2c: pop      {r4, r5, r6, lr}
003cbb30: b        #0x3a4d5c

# _ZN6CSDead6OnInitEiP9CharacterP16CharStateMachine
003c8920: push     {r4, r5, r6, lr}
003c8924: add      r5, r2, #0x4f0
003c8928: add      r5, r5, #0xc
003c892c: sub      sp, sp, #0x18
003c8930: mov      r4, #0
003c8934: mov      r6, r1
003c8938: mov      r0, r5
003c893c: mov      r2, #0x2e
003c8940: mov      r3, #2
003c8944: str      r4, [sp, #0x10]
003c8948: str      r4, [sp, #0x14]
003c894c: str      r4, [sp]
003c8950: str      r4, [sp, #4]
003c8954: bl       #0x3c7b18
003c8958: mov      r0, r5
003c895c: mov      r1, r6
003c8960: movw     r2, #0xc359
003c8964: mov      r3, #0x10
003c8968: str      r4, [sp, #4]
003c896c: str      r4, [sp, #8]
003c8970: str      r4, [sp, #0xc]
003c8974: str      r4, [sp]
003c8978: bl       #0x3c7b18
003c897c: add      sp, sp, #0x18
003c8980: pop      {r4, r5, r6, pc}

# _ZN16CharStateMachine9_GetStateEi
003c184c: push     {r4, r5, lr}
003c1850: sub      sp, sp, #0xc
003c1854: mov      r5, r0
003c1858: mov      r4, r1
003c185c: bl       #0x3c0084
003c1860: ldr      r3, [pc, #0xb8]
003c1864: cmp      r0, #0
003c1868: add      r3, pc, r3
003c186c: bne      #0x3c1890
003c1870: ldr      r2, [pc, #0xac]
003c1874: ldr      r2, [r3, r2]
003c1878: ldr      r2, [r2]
003c187c: cmp      r2, #2
003c1880: streq    r0, [r0]
003c1884: beq      #0x3c1890
003c1888: cmp      r2, #1
003c188c: beq      #0x3c18ec
003c1890: ldr      r3, [r5, #0xc]
003c1894: add      r5, r5, #8
003c1898: cmp      r3, #0
003c189c: beq      #0x3c18e0
003c18a0: mov      r1, r5
003c18a4: b        #0x3c18ac
003c18a8: mov      r3, r2
003c18ac: ldr      r2, [r3, #0x10]
003c18b0: cmp      r4, r2
003c18b4: ldrgt    r2, [r3, #0xc]
003c18b8: ldrle    r2, [r3, #8]
003c18bc: movgt    r3, r1
003c18c0: mov      r1, r3
003c18c4: cmp      r2, #0
003c18c8: bne      #0x3c18a8
003c18cc: cmp      r5, r3
003c18d0: beq      #0x3c18e0
003c18d4: ldr      r2, [r3, #0x10]
003c18d8: cmp      r4, r2
003c18dc: movge    r5, r3
003c18e0: add      r0, r5, #0x14
003c18e4: add      sp, sp, #0xc
003c18e8: pop      {r4, r5, pc}
003c18ec: ldr      r0, [pc, #0x34]
003c18f0: ldr      r1, [pc, #0x34]
003c18f4: ldr      r2, [pc, #0x34]
003c18f8: ldr      r0, [r3, r0]
003c18fc: ldr      r3, [pc, #0x30]
003c1900: mov      ip, #0x97
003c1904: add      r1, pc, r1
003c1908: add      r2, pc, r2
003c190c: add      r3, pc, r3
003c1910: add      r0, r0, #0xa8
003c1914: str      ip, [sp]
003c1918: bl       #0x30e004
003c191c: b        #0x3c1890
003c1920: subseq   r3, sp, r8, lsr #4
003c1924: andeq    r3, r0, r0, asr #19
003c1928: andeq    r1, r0, r0, asr #19
003c192c: ldrdeq   ip, sp, [pc], #-0xa4
003c1930: subseq   r3, r0, r8, asr #6
003c1934: ldrsbeq  r3, [r0], #-0x2c

# _ZN9Character8InitPostEv
003b4d60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr      r5, [pc, #0x828]
003b4d68: ldr      r6, [pc, #0x828]
003b4d6c: movw     r3, #0x1394
003b4d70: add      r5, pc, r5
003b4d74: ldr      r2, [r5, r6]
003b4d78: ldrb     r7, [r0, r3]
003b4d7c: sub      sp, sp, #0xfc
003b4d80: ldr      r2, [r2]
003b4d84: cmp      r7, #0
003b4d88: mov      r4, r0
003b4d8c: str      r2, [sp, #0xf4]
003b4d90: beq      #0x3b4db0
003b4d94: ldr      r3, [r5, r6]
003b4d98: ldr      r2, [sp, #0xf4]
003b4d9c: ldr      r3, [r3]
003b4da0: cmp      r2, r3
003b4da4: bne      #0x3b5590
003b4da8: add      sp, sp, #0xfc
003b4dac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov      r2, #1
003b4db4: strb     r2, [r0, r3]
003b4db8: bl       #0x38bd64
003b4dbc: ldr      r3, [r4, #0x274]
003b4dc0: cmp      r0, r3
003b4dc4: bge      #0x3b4d94
003b4dc8: ldr      fp, [pc, #0x7cc]
003b4dcc: add      r8, sp, #0xdc
003b4dd0: ldr      sl, [r5, fp]
003b4dd4: mov      r0, sl
003b4dd8: bl       #0x337888
003b4ddc: ldr      r1, [pc, #0x7bc]
003b4de0: add      r2, sp, #0x48
003b4de4: mov      r0, r8
003b4de8: add      r1, pc, r1
003b4dec: bl       #0x3140ec
003b4df0: mov      r1, r8
003b4df4: mov      r0, sl
003b4df8: bl       #0x337a88
003b4dfc: mov      r0, r8
003b4e00: bl       #0x318254
003b4e04: movw     r3, #0x13fc
003b4e08: ldr      r2, [r4, r3]
003b4e0c: movw     r3, #0x13f8
003b4e10: ldr      r3, [r4, r3]
003b4e14: cmp      r3, r2
003b4e18: beq      #0x3b4e6c
003b4e1c: ldr      r1, [pc, #0x780]
003b4e20: add      r8, sp, #0x24
003b4e24: ldr      r3, [r4, #0x64]
003b4e28: ldr      r1, [r5, r1]
003b4e2c: mov      r0, r8
003b4e30: ldr      r1, [r1, #0x38]
003b4e34: str      r7, [sp]
003b4e38: str      r7, [sp, #4]
003b4e3c: bl       #0x34aca0
003b4e40: mov      r0, r8
003b4e44: mov      r1, r7
003b4e48: bl       #0x33fdc0
003b4e4c: cmp      r0, #0
003b4e50: beq      #0x3b4e6c
003b4e54: mov      r0, r8
003b4e58: bl       #0x33fee4
003b4e5c: subs     r1, r0, #0
003b4e60: beq      #0x3b4e6c
003b4e64: ldr      r0, [r4, #0x378]
003b4e68: bl       #0x405540
003b4e6c: mov      r0, r4
003b4e70: movw     r7, #0x13c8
003b4e74: bl       #0x3b3d38
003b4e78: ldrsh    r3, [r4, r7]
003b4e7c: cmn      r3, #1
003b4e80: movweq   r1, #0xffff
003b4e84: beq      #0x3b4ec4
003b4e88: ldr      sl, [r5, fp]
003b4e8c: add      r8, sp, #0xc4
003b4e90: mov      r0, sl
003b4e94: bl       #0x337888
003b4e98: ldr      r1, [pc, #0x708]
003b4e9c: add      r2, sp, #0x44
003b4ea0: mov      r0, r8
003b4ea4: add      r1, pc, r1
003b4ea8: bl       #0x3140ec
003b4eac: mov      r1, r8
003b4eb0: mov      r0, sl
003b4eb4: bl       #0x337a88
003b4eb8: mov      r0, r8
003b4ebc: bl       #0x318254
003b4ec0: ldrh     r1, [r4, r7]
003b4ec4: add      r7, r4, #0x560
003b4ec8: sxth     r1, r1
003b4ecc: mov      r0, r7
003b4ed0: bl       #0x3df2a4
003b4ed4: mov      r0, r7
003b4ed8: mov      r1, #1
003b4edc: bl       #0x3e0810
003b4ee0: mov      r0, r4
003b4ee4: bl       #0x3a54d4
003b4ee8: subs     r8, r0, #0
003b4eec: beq      #0x3b4f04
003b4ef0: bl       #0x30de54
003b4ef4: mov      r1, r8
003b4ef8: add      r2, r8, r0
003b4efc: add      r0, r4, #0x290
003b4f00: bl       #0x3109e0
003b4f04: ldr      sl, [r5, fp]
003b4f08: add      r8, sp, #0xac
003b4f0c: mov      r0, sl
003b4f10: bl       #0x337888
003b4f14: ldr      r1, [pc, #0x690]
003b4f18: add      r2, sp, #0x40
003b4f1c: mov      r0, r8
003b4f20: add      r1, pc, r1
003b4f24: bl       #0x3140ec
003b4f28: mov      r1, r8
003b4f2c: mov      r0, sl
003b4f30: bl       #0x337a88
003b4f34: mov      r0, r8
003b4f38: bl       #0x318254
003b4f3c: ldr      r0, [r4, #0x59c]
003b4f40: bl       #0x30e964
003b4f44: movw     r1, #0x74bc
003b4f48: movt     r1, #0x3c13
003b4f4c: bl       #0x30ed6c
003b4f50: str      r0, [r4, #0x120]
003b4f54: ldr      r0, [r4, #0x5a0]
003b4f58: bl       #0x30e964
003b4f5c: movw     r1, #0x74bc
003b4f60: movt     r1, #0x3c13
003b4f64: bl       #0x30ed6c
003b4f68: str      r0, [r4, #0x124]
003b4f6c: ldr      r0, [r4, #0x5a4]
003b4f70: bl       #0x30e964
003b4f74: movw     r1, #0xd70a
003b4f78: movt     r1, #0x3c23
003b4f7c: bl       #0x30ed6c
003b4f80: str      r0, [r4, #0x128]
003b4f84: mov      r0, r4
003b4f88: bl       #0x38be5c
003b4f8c: mov      r0, r4
003b4f90: bl       #0x38ab60
003b4f94: subs     r1, r0, #0
003b4f98: beq      #0x3b5488
003b4f9c: mov      r1, #2
003b4fa0: mov      r0, r4
003b4fa4: bl       #0x3bc4d0
003b4fa8: ldr      r3, [pc, #0x600]
003b4fac: mov      r0, r4
003b4fb0: ldr      r3, [r5, r3]
003b4fb4: ldr      r8, [r3]
003b4fb8: bl       #0x3a2fec
003b4fbc: mov      r3, #0x44
003b4fc0: mla      r8, r3, r0, r8
003b4fc4: ldr      r3, [r4]
003b4fc8: mov      r0, r4
003b4fcc: mov      lr, pc
003b4fd0: ldr      pc, [r3, #0x28]
003b4fd4: cmp      r0, #0
003b4fd8: bne      #0x3b4ff4
003b4fdc: ldrb     r3, [r8, #0x10]
003b4fe0: cmp      r3, #0
003b4fe4: beq      #0x3b4ff4
003b4fe8: mov      r3, #1
003b4fec: strb     r3, [r4, #0x3ec]
003b4ff0: b        #0x3b5014
003b4ff4: ldr      r3, [r4]
003b4ff8: mov      r0, r4
003b4ffc: mov      lr, pc
003b5000: ldr      pc, [r3, #0x28]
003b5004: cmp      r0, #0
003b5008: bne      #0x3b54a4
003b500c: add      r0, r4, #0x3c8
003b5010: bl       #0x3cf1f0
003b5014: movw     r3, #0x1488
003b5018: ldr      r2, [pc, #0x594]
003b501c: ldr      r1, [r4, r3]
003b5020: ldr      r3, [r8, #0x30]
003b5024: str      r2, [sp, #0xc]
003b5028: mov      r2, r4
003b502c: add      r1, r1, r3
003b5030: ldr      r3, [sp, #0xc]
003b5034: ldr      sb, [pc, #0x57c]
003b5038: add      sl, sp, #0x94
003b503c: ldr      r0, [r5, r3]
003b5040: bl       #0x495430
003b5044: movw     r3, #0x1484
003b5048: str      r0, [r4, r3]
003b504c: mov      r0, r4
003b5050: bl       #0x3b4738
003b5054: ldr      r8, [r5, fp]
003b5058: add      sb, pc, sb
003b505c: mov      r0, r8
003b5060: bl       #0x337888
003b5064: add      r2, sp, #0x3c
003b5068: mov      r0, sl
003b506c: mov      r1, sb
003b5070: bl       #0x3140ec
003b5074: mov      r1, sl
003b5078: mov      r0, r8
003b507c: bl       #0x337a88
003b5080: mov      r0, sl
003b5084: bl       #0x318254
003b5088: add      r0, r4, #0x490
003b508c: add      r0, r0, #0xc
003b5090: bl       #0x3c9f4c
003b5094: add      sl, sp, #0x7c
003b5098: mov      r0, r8
003b509c: bl       #0x337888
003b50a0: add      r2, sp, #0x38
003b50a4: mov      r0, sl
003b50a8: mov      r1, sb
003b50ac: bl       #0x3140ec
003b50b0: mov      r1, sl
003b50b4: mov      r0, r8
003b50b8: bl       #0x337a88
003b50bc: mov      r0, sl
003b50c0: bl       #0x318254
003b50c4: mov      r0, r4
003b50c8: bl       #0x3b3b00
003b50cc: add      sl, sp, #0x64
003b50d0: mov      r0, r8
003b50d4: bl       #0x337888
003b50d8: add      r2, sp, #0x34
003b50dc: mov      r1, sb
003b50e0: mov      r0, sl
003b50e4: bl       #0x3140ec
003b50e8: mov      r1, sl
003b50ec: mov      r0, r8
003b50f0: bl       #0x337a88
003b50f4: mov      r0, sl
003b50f8: bl       #0x318254
003b50fc: ldr      r3, [r4]
003b5100: mov      r0, r4
003b5104: mov      lr, pc
003b5108: ldr      pc, [r3, #0x28]
003b510c: cmp      r0, #0
003b5110: beq      #0x3b538c
003b5114: ldr      r3, [r4, #0x2d8]
003b5118: cmp      r3, #0
003b511c: beq      #0x3b512c
003b5120: ldr      r0, [r3, #8]
003b5124: mov      r1, #0
003b5128: bl       #0x59719c
003b512c: ldr      r2, [pc, #0x470]
003b5130: mov      r0, r4
003b5134: mov      r1, #4
003b5138: str      r2, [sp, #0x10]
003b513c: bl       #0x3bc4d0
003b5140: ldr      r3, [sp, #0x10]
003b5144: ldr      r0, [r5, r3]
003b5148: bl       #0x31f594
003b514c: cmp      r0, #0
003b5150: beq      #0x3b5160
003b5154: ldr      r1, [r0, #0x118]
003b5158: mov      r0, r4
003b515c: bl       #0x3bb950
003b5160: ldr      r2, [sp, #0x10]
003b5164: mov      r1, r4
003b5168: ldr      r3, [r5, r2]
003b516c: ldr      r0, [r3, #0x40]
003b5170: bl       #0x36effc
003b5174: cmp      r0, #0
003b5178: bne      #0x3b54dc
003b517c: movw     r3, #0x13c8
003b5180: ldrsh    r1, [r4, r3]
003b5184: mov      r0, r7
003b5188: bl       #0x3df2a4
003b518c: mov      r0, r7
003b5190: bl       #0x3df480
003b5194: mov      r0, r7
003b5198: mov      r1, #1
003b519c: bl       #0x3e0810
003b51a0: ldr      r2, [sp, #0x10]
003b51a4: mov      r1, r4
003b51a8: ldr      r3, [r5, r2]
003b51ac: ldr      r0, [r3, #0x40]
003b51b0: bl       #0x36effc
003b51b4: cmp      r0, #0
003b51b8: bne      #0x3b54d0
003b51bc: movw     r3, #0xc9ff
003b51c0: movt     r3, #0x3b9a
003b51c4: str      r3, [r4, #0x3a4]
003b51c8: mov      r2, #0
003b51cc: mov      r0, r7
003b51d0: mov      r1, #0xc2
003b51d4: bl       #0x3df6e0
003b51d8: ldr      r2, [sp, #0xc]
003b51dc: bic      r0, r0, r0, asr #31
003b51e0: strb     r0, [r4, #0x3a8]
003b51e4: ldr      r3, [r5, r2]
003b51e8: ldr      r2, [r3, #0x1c]
003b51ec: ldr      r3, [r3, #0x20]
003b51f0: rsb      r3, r2, r3
003b51f4: asr      r3, r3, #3
003b51f8: add      r2, r3, r3, lsl #2
003b51fc: add      r2, r2, r2, lsl #4
003b5200: add      r2, r2, r2, lsl #8
003b5204: add      r2, r2, r2, lsl #16
003b5208: add      r3, r3, r2, lsl #1
003b520c: cmp      r3, #0
003b5210: beq      #0x3b538c
003b5214: mov      r0, #0x24
003b5218: mov      r1, #0
003b521c: bl       #0x31056c
003b5220: movw     r3, #0x1494
003b5224: str      r0, [r4, r3]
003b5228: ldr      r3, [pc, #0x38c]
003b522c: mov      sb, r0
003b5230: ldr      r3, [r5, r3]
003b5234: ldr      sl, [r3]
003b5238: cmp      sl, #0
003b523c: beq      #0x3b550c
003b5240: ldr      r3, [pc, #0x378]
003b5244: ldr      r2, [pc, #0x378]
003b5248: str      r7, [sp, #0x18]
003b524c: ldr      r3, [r5, r3]
003b5250: add      r2, pc, r2
003b5254: mov      r8, #0
003b5258: ldr      r3, [r3]
003b525c: str      r0, [sp, #0x14]
003b5260: mov      sb, r2
003b5264: mov      r7, r3
003b5268: b        #0x3b5278
003b526c: add      r8, r8, #1
003b5270: cmp      r8, sl
003b5274: beq      #0x3b5504
003b5278: mov      r0, sb
003b527c: ldr      r1, [r7, r8, lsl #2]
003b5280: bl       #0x30e31c
003b5284: cmp      r0, #0
003b5288: bne      #0x3b526c
003b528c: ldr      sb, [sp, #0x14]
003b5290: ldr      r7, [sp, #0x18]
003b5294: mov      r1, r8
003b5298: ldr      r3, [sp, #0xc]
003b529c: str      r7, [sp, #0x14]
003b52a0: str      fp, [sp, #0x18]
003b52a4: ldr      r2, [r5, r3]
003b52a8: ldr      r3, [pc, #0x318]
003b52ac: str      r6, [sp, #0x1c]
003b52b0: mov      r8, #0
003b52b4: movw     sl, #0x1494
003b52b8: mov      r7, r1
003b52bc: mov      r6, r2
003b52c0: mov      fp, r3
003b52c4: b        #0x3b52cc
003b52c8: ldr      sb, [r4, sl]
003b52cc: mov      r0, r6
003b52d0: add      r1, r7, r8
003b52d4: mov      r2, #0
003b52d8: bl       #0x495430
003b52dc: str      r0, [sb, r8, lsl #2]
003b52e0: ldr      r3, [r4, sl]
003b52e4: ldr      r3, [r3, r8, lsl #2]
003b52e8: cmp      r3, #0
003b52ec: beq      #0x3b5350
003b52f0: ldr      r2, [r5, fp]
003b52f4: mov      r0, r3
003b52f8: mov      r1, #0
003b52fc: ldr      lr, [r2]
003b5300: ldr      ip, [r2, #4]
003b5304: ldr      r2, [r2, #8]
003b5308: str      lr, [r3, #0x34]
003b530c: str      ip, [r3, #0x38]
003b5310: str      r2, [r3, #0x3c]
003b5314: bl       #0x492aa0
003b5318: ldr      r3, [r4, sl]
003b531c: mov      r1, #0
003b5320: ldr      r0, [r3, r8, lsl #2]
003b5324: bl       #0x492ef0
003b5328: ldr      r3, [r4, sl]
003b532c: ldr      r0, [r3, r8, lsl #2]
003b5330: bl       #0x49267c
003b5334: ldr      r3, [r0]
003b5338: mov      lr, pc
003b533c: ldr      pc, [r3, #0x44]
003b5340: mov      r1, #1
003b5344: ldr      r3, [r0]
003b5348: mov      lr, pc
003b534c: ldr      pc, [r3, #0x40]
003b5350: add      r8, r8, #1
003b5354: cmp      r8, #9
003b5358: bne      #0x3b52c8
003b535c: ldr      r2, [sp, #0x10]
003b5360: mov      r1, r4
003b5364: ldr      r7, [sp, #0x14]
003b5368: ldr      r3, [r5, r2]
003b536c: ldr      fp, [sp, #0x18]
003b5370: ldr      r6, [sp, #0x1c]
003b5374: ldr      r0, [r3, #0x40]
003b5378: bl       #0x36effc
003b537c: cmp      r0, #0
003b5380: bne      #0x3b5514
003b5384: mov      r0, r4
003b5388: bl       #0x3a41a0
003b538c: add      r8, r4, #0xff0
003b5390: add      r8, r8, #4
003b5394: mov      r1, r8
003b5398: mov      r2, #0xd2
003b539c: mov      r0, r7
003b53a0: bl       #0x3dedb4
003b53a4: bl       #0x30e964
003b53a8: mov      r3, #0x1440
003b53ac: str      r0, [r4, r3]
003b53b0: mov      r2, #0xd3
003b53b4: mov      r1, r8
003b53b8: mov      r0, r7
003b53bc: bl       #0x3dedb4
003b53c0: bl       #0x30e964
003b53c4: movw     r3, #0x1444
003b53c8: str      r0, [r4, r3]
003b53cc: add      r1, r4, #0x160
003b53d0: mov      r0, r4
003b53d4: bl       #0x3a58f4
003b53d8: add      r1, r4, #0x1440
003b53dc: mov      r0, r4
003b53e0: add      r1, r1, #0x10
003b53e4: mov      r2, #1
003b53e8: bl       #0x393db4
003b53ec: ldr      ip, [r4, #0x16c]
003b53f0: ldr      r0, [r4, #0x2d8]
003b53f4: ldr      r1, [r4, #0x170]
003b53f8: ldr      r2, [r4, #0x174]
003b53fc: movw     r3, #0x145c
003b5400: str      ip, [r4, r3]
003b5404: movw     r3, #0x1460
003b5408: str      r1, [r4, r3]
003b540c: cmp      r0, #0
003b5410: movw     r3, #0x1464
003b5414: str      r2, [r4, r3]
003b5418: beq      #0x3b5420
003b541c: bl       #0x470a54
003b5420: mov      r1, #0
003b5424: mov      r2, #1
003b5428: mov      r0, r4
003b542c: bl       #0x3a59ac
003b5430: mov      r0, r4
003b5434: bl       #0x3b3a70
003b5438: ldrb     r1, [r4, #0x3ec]
003b543c: cmp      r1, #0
003b5440: beq      #0x3b54c4
003b5444: mov      r0, r4
003b5448: bl       #0x3d37d0
003b544c: ldr      r7, [r5, fp]
003b5450: add      r4, sp, #0x4c
003b5454: mov      r0, r7
003b5458: bl       #0x337888
003b545c: ldr      r1, [pc, #0x168]
003b5460: add      r2, sp, #0x30
003b5464: mov      r0, r4
003b5468: add      r1, pc, r1
003b546c: bl       #0x3140ec
003b5470: mov      r0, r7
003b5474: mov      r1, r4
003b5478: bl       #0x337a88
003b547c: mov      r0, r4
003b5480: bl       #0x318254
003b5484: b        #0x3b4d94
003b5488: mov      r0, r4
003b548c: ldr      r3, [r4]
003b5490: mov      lr, pc
003b5494: ldr      pc, [r3, #0x40]
003b5498: mov      r0, r4
003b549c: bl       #0x33ddb4
003b54a0: b        #0x3b4d94
003b54a4: ldr      r3, [pc, #0xf8]
003b54a8: mov      r1, r4
003b54ac: ldr      r3, [r5, r3]
003b54b0: ldr      r0, [r3, #0x40]
003b54b4: bl       #0x36effc
003b54b8: cmp      r0, #0
003b54bc: bne      #0x3b500c
003b54c0: b        #0x3b4fe8
003b54c4: add      r0, r4, #0x3c8
003b54c8: bl       #0x3ce7c0
003b54cc: b        #0x3b5444
003b54d0: mov      r0, r4
003b54d4: bl       #0x3b3a90
003b54d8: b        #0x3b51bc
003b54dc: mov      r0, r4
003b54e0: bl       #0x3b395c
003b54e4: mov      r0, r7
003b54e8: bl       #0x3defac
003b54ec: movw     r3, #0x14e8
003b54f0: ldr      r0, [r4, r3]
003b54f4: cmp      r0, #0
003b54f8: beq      #0x3b517c
003b54fc: bl       #0x4679e8
003b5500: b        #0x3b517c
003b5504: ldr      sb, [sp, #0x14]
003b5508: ldr      r7, [sp, #0x18]
003b550c: mvn      r1, #0
003b5510: b        #0x3b5298
003b5514: ldr      r3, [sp, #0xc]
003b5518: mov      r2, #0
003b551c: movw     r8, #0x149c
003b5520: ldr      r0, [r5, r3]
003b5524: ldr      r3, [pc, #0xa4]
003b5528: ldr      r3, [r5, r3]
003b552c: ldr      r3, [r3]
003b5530: ldr      r1, [r3, #0x88]
003b5534: bl       #0x495430
003b5538: cmp      r0, #0
003b553c: str      r0, [r4, r8]
003b5540: beq      #0x3b5384
003b5544: mov      r2, #0
003b5548: str      r2, [r0, #0x3c]
003b554c: str      r2, [r0, #0x34]
003b5550: str      r2, [r0, #0x38]
003b5554: mov      r1, #0
003b5558: bl       #0x492aa0
003b555c: mov      r1, #0
003b5560: ldr      r0, [r4, r8]
003b5564: bl       #0x492ef0
003b5568: ldr      r0, [r4, r8]
003b556c: bl       #0x49267c
003b5570: ldr      r3, [r0]
003b5574: mov      lr, pc
003b5578: ldr      pc, [r3, #0x44]
003b557c: mov      r1, #1
003b5580: ldr      r3, [r0]
003b5584: mov      lr, pc
003b5588: ldr      pc, [r3, #0x40]
003b558c: b        #0x3b5384
003b5590: bl       #0x30e310
003b5594: subseq   pc, sp, r0, lsr #26
003b5598: andeq    r4, r0, ip, lsr #1
003b559c: andeq    r0, r0, r4, lsl #17
003b55a0: subseq   pc, r0, r8, lsr #32
003b55a4: strdeq   r3, r4, [r0], -r4
003b55a8: subseq   lr, r0, ip, ror #30
003b55ac: ldrsheq  lr, [r0], #-0xe0
003b55b0: andeq    r0, r0, r8, asr r7
003b55b4: andeq    r1, r0, r8, lsl #22
003b55b8: ldrheq   lr, [r0], #-0xd8
003b55bc: andeq    r0, r0, r4, asr #13
003b55c0: muleq    r0, r4, r2
003b55c4: subseq   lr, r0, r0, ror #24
003b55c8: andeq    r3, r0, ip, lsr #30
003b55cc: subseq   lr, r0, r8, lsr #19
003b55d0: andeq    r3, r0, r8, asr #5

# _ZN8CSLimbus6OnInitEiP9CharacterP16CharStateMachine
003c7cc0: ldr      ip, [pc, #0x3c]
003c7cc4: push     {r4, lr}
003c7cc8: ldr      r3, [pc, #0x38]
003c7ccc: add      ip, pc, ip
003c7cd0: add      r0, r2, #0x4f0
003c7cd4: ldr      r4, [ip, r3]
003c7cd8: sub      sp, sp, #0x10
003c7cdc: mov      lr, #0
003c7ce0: add      r0, r0, #0xc
003c7ce4: mov      r2, #0x2f
003c7ce8: mov      r3, #1
003c7cec: stm      sp, {r4, lr}
003c7cf0: str      r4, [sp, #8]
003c7cf4: str      lr, [sp, #0xc]
003c7cf8: bl       #0x3c7b18
003c7cfc: add      sp, sp, #0x10
003c7d00: pop      {r4, pc}
003c7d04: subseq   ip, ip, r4, asr #27
003c7d08: strheq   r1, [r0], -r0

# _ZN16CharStateMachine12SetCharacterEP9Character
003c1600: push     {r4, r5, lr}
003c1604: ldr      r3, [pc, #0x70]
003c1608: subs     r4, r1, #0
003c160c: sub      sp, sp, #0xc
003c1610: mov      r5, r0
003c1614: add      r3, pc, r3
003c1618: beq      #0x3c1628
003c161c: str      r4, [r5, #4]
003c1620: add      sp, sp, #0xc
003c1624: pop      {r4, r5, pc}
003c1628: ldr      r2, [pc, #0x50]
003c162c: ldr      r2, [r3, r2]
003c1630: ldr      r2, [r2]
003c1634: cmp      r2, #2
003c1638: streq    r4, [r4]
003c163c: beq      #0x3c161c
003c1640: cmp      r2, #1
003c1644: bne      #0x3c161c
003c1648: ldr      r0, [pc, #0x34]
003c164c: ldr      r1, [pc, #0x34]
003c1650: ldr      r2, [pc, #0x34]
003c1654: ldr      r0, [r3, r0]
003c1658: ldr      r3, [pc, #0x30]
003c165c: mov      ip, #0xe1
003c1660: add      r1, pc, r1
003c1664: add      r2, pc, r2
003c1668: add      r3, pc, r3
003c166c: add      r0, r0, #0xa8
003c1670: str      ip, [sp]
003c1674: bl       #0x30e004
003c1678: b        #0x3c161c
003c167c: subseq   r3, sp, ip, ror r4
003c1680: andeq    r3, r0, r0, asr #19
003c1684: andeq    r1, r0, r0, asr #19
003c1688: subeq    ip, pc, r8, ror sp
003c168c: subseq   r0, r3, r4, lsr #20
003c1690: subseq   r3, r0, r0, lsl #11

# _ZN16CharStateMachineC2Ev
003c1b58: ldr      ip, [pc, #0x84]
003c1b5c: str      r4, [sp, #-4]!
003c1b60: ldr      r4, [pc, #0x80]
003c1b64: add      ip, pc, ip
003c1b68: mov      r2, #0
003c1b6c: ldr      r4, [ip, r4]
003c1b70: mov      r1, r0
003c1b74: str      r2, [r0, #4]
003c1b78: add      r4, r4, #8
003c1b7c: str      r4, [r0]
003c1b80: str      r2, [r0, #0xc]
003c1b84: mvn      r4, #0
003c1b88: strb     r2, [r1, #8]!
003c1b8c: str      r1, [r0, #0x14]
003c1b90: str      r4, [r0, #0x28]
003c1b94: str      r2, [r0, #0x5c]
003c1b98: str      r1, [r0, #0x10]
003c1b9c: str      r2, [r0, #0x18]
003c1ba0: str      r2, [r0, #0x20]
003c1ba4: str      r2, [r0, #0x24]
003c1ba8: str      r2, [r0, #0x60]
003c1bac: str      r2, [r0, #0x2c]
003c1bb0: str      r2, [r0, #0x30]
003c1bb4: str      r2, [r0, #0x34]
003c1bb8: str      r2, [r0, #0x38]
003c1bbc: str      r2, [r0, #0x3c]
003c1bc0: str      r2, [r0, #0x40]
003c1bc4: str      r2, [r0, #0x44]
003c1bc8: str      r2, [r0, #0x48]
003c1bcc: str      r2, [r0, #0x4c]
003c1bd0: str      r2, [r0, #0x50]
003c1bd4: str      r2, [r0, #0x54]
003c1bd8: str      r2, [r0, #0x58]
003c1bdc: ldm      sp!, {r4}
003c1be0: bx       lr
003c1be4: subseq   r2, sp, ip, lsr #30
003c1be8: strdeq   r2, r3, [r0], -r8

# _ZN8CSAttack6OnInitEiP9CharacterP16CharStateMachine
003c8284: push     {r4, r5, r6, r7, r8, lr}
003c8288: add      r5, r2, #0x4f0
003c828c: add      r5, r5, #0xc
003c8290: sub      sp, sp, #0x58
003c8294: mov      r4, #0
003c8298: mov      r6, r1
003c829c: mov      r0, r5
003c82a0: mov      r2, #0x22
003c82a4: mov      r3, #3
003c82a8: str      r4, [sp, #0x50]
003c82ac: str      r4, [sp, #0x54]
003c82b0: str      r4, [sp]
003c82b4: str      r4, [sp, #4]
003c82b8: bl       #0x3c7b18
003c82bc: mov      r0, r5
003c82c0: mov      r1, r6
003c82c4: movw     r2, #0xc358
003c82c8: mov      r3, #0xc
003c82cc: str      r4, [sp, #0x48]
003c82d0: str      r4, [sp, #0x4c]
003c82d4: str      r4, [sp]
003c82d8: str      r4, [sp, #4]
003c82dc: bl       #0x3c7b18
003c82e0: mov      r0, r5
003c82e4: mov      r1, r6
003c82e8: movw     r2, #0xc355
003c82ec: mov      r3, #6
003c82f0: str      r4, [sp, #0x40]
003c82f4: str      r4, [sp, #0x44]
003c82f8: str      r4, [sp]
003c82fc: str      r4, [sp, #4]
003c8300: ldr      r8, [pc, #0x120]
003c8304: bl       #0x3c7b18
003c8308: mov      r0, r5
003c830c: mov      r1, r6
003c8310: movw     r2, #0xc356
003c8314: mov      r3, #7
003c8318: str      r4, [sp, #0x38]
003c831c: str      r4, [sp, #0x3c]
003c8320: str      r4, [sp]
003c8324: str      r4, [sp, #4]
003c8328: bl       #0x3c7b18
003c832c: ldr      r3, [pc, #0xf8]
003c8330: add      r8, pc, r8
003c8334: mov      r0, r5
003c8338: ldr      ip, [r8, r3]
003c833c: mov      r1, r6
003c8340: movw     r2, #0xc35a
003c8344: mov      r3, #0xb
003c8348: str      ip, [sp]
003c834c: str      ip, [sp, #0x30]
003c8350: str      r4, [sp, #0x34]
003c8354: str      r4, [sp, #4]
003c8358: bl       #0x3c7b18
003c835c: ldr      r3, [pc, #0xcc]
003c8360: mov      r0, r5
003c8364: mov      r1, r6
003c8368: ldr      r7, [r8, r3]
003c836c: movw     r2, #0xc35b
003c8370: mov      r3, #0xa
003c8374: str      r7, [sp, #0x28]
003c8378: str      r4, [sp, #0x2c]
003c837c: str      r7, [sp]
003c8380: str      r4, [sp, #4]
003c8384: bl       #0x3c7b18
003c8388: mov      r0, r5
003c838c: mov      r1, r6
003c8390: movw     r2, #0xc35c
003c8394: mov      r3, #9
003c8398: str      r7, [sp, #0x20]
003c839c: str      r4, [sp, #0x24]
003c83a0: str      r7, [sp]
003c83a4: str      r4, [sp, #4]
003c83a8: bl       #0x3c7b18
003c83ac: mov      r0, r5
003c83b0: mov      r1, r6
003c83b4: movw     r2, #0xc35d
003c83b8: mov      r3, #8
003c83bc: str      r7, [sp]
003c83c0: str      r7, [sp, #0x18]
003c83c4: str      r4, [sp, #0x1c]
003c83c8: str      r4, [sp, #4]
003c83cc: bl       #0x3c7b18
003c83d0: ldr      r3, [pc, #0x5c]
003c83d4: mov      r0, r5
003c83d8: mov      r1, r6
003c83dc: ldr      ip, [r8, r3]
003c83e0: movw     r2, #0xc351
003c83e4: mov      r3, #4
003c83e8: str      ip, [sp]
003c83ec: str      ip, [sp, #0x10]
003c83f0: str      r4, [sp, #0x14]
003c83f4: str      r4, [sp, #4]
003c83f8: bl       #0x3c7b18
003c83fc: mov      r0, r5
003c8400: mov      r1, r6
003c8404: movw     r2, #0xc357
003c8408: mov      r3, #0xf
003c840c: str      r4, [sp, #4]
003c8410: str      r4, [sp, #8]
003c8414: str      r4, [sp, #0xc]
003c8418: str      r4, [sp]
003c841c: bl       #0x3c7b18
003c8420: add      sp, sp, #0x58
003c8424: pop      {r4, r5, r6, r7, r8, pc}
003c8428: subseq   ip, ip, r0, ror #14
003c842c: andeq    r2, r0, r4, lsl #29
003c8430: andeq    r3, r0, ip, asr #9
003c8434: andeq    r3, r0, r4, lsl #7

# _ZN16CharStateMachineC1Ev
003c1ac4: ldr      ip, [pc, #0x84]
003c1ac8: str      r4, [sp, #-4]!
003c1acc: ldr      r4, [pc, #0x80]
003c1ad0: add      ip, pc, ip
003c1ad4: mov      r2, #0
003c1ad8: ldr      r4, [ip, r4]
003c1adc: mov      r1, r0
003c1ae0: str      r2, [r0, #4]
003c1ae4: add      r4, r4, #8
003c1ae8: str      r4, [r0]
003c1aec: str      r2, [r0, #0xc]
003c1af0: mvn      r4, #0
003c1af4: strb     r2, [r1, #8]!
003c1af8: str      r1, [r0, #0x14]
003c1afc: str      r4, [r0, #0x28]
003c1b00: str      r2, [r0, #0x5c]
003c1b04: str      r1, [r0, #0x10]
003c1b08: str      r2, [r0, #0x18]
003c1b0c: str      r2, [r0, #0x20]
003c1b10: str      r2, [r0, #0x24]
003c1b14: str      r2, [r0, #0x60]
003c1b18: str      r2, [r0, #0x2c]
003c1b1c: str      r2, [r0, #0x30]
003c1b20: str      r2, [r0, #0x34]
003c1b24: str      r2, [r0, #0x38]
003c1b28: str      r2, [r0, #0x3c]
003c1b2c: str      r2, [r0, #0x40]
003c1b30: str      r2, [r0, #0x44]
003c1b34: str      r2, [r0, #0x48]
003c1b38: str      r2, [r0, #0x4c]
003c1b3c: str      r2, [r0, #0x50]
003c1b40: str      r2, [r0, #0x54]
003c1b44: str      r2, [r0, #0x58]
003c1b48: ldm      sp!, {r4}
003c1b4c: bx       lr
003c1b50: subseq   r2, sp, r0, asr #31
003c1b54: strdeq   r2, r3, [r0], -r8

# _ZN9Character9InitFinalEv
003b4978: push     {r4, r5, r6, r7, r8, lr}
003b497c: ldr      r4, [pc, #0x22c]
003b4980: ldr      r6, [pc, #0x22c]
003b4984: movw     r3, #0x1395
003b4988: add      r4, pc, r4
003b498c: ldr      r2, [r4, r6]
003b4990: ldrb     r1, [r0, r3]
003b4994: sub      sp, sp, #0x48
003b4998: ldr      r2, [r2]
003b499c: cmp      r1, #0
003b49a0: mov      r5, r0
003b49a4: str      r2, [sp, #0x44]
003b49a8: beq      #0x3b49c8
003b49ac: ldr      r3, [r4, r6]
003b49b0: ldr      r2, [sp, #0x44]
003b49b4: ldr      r3, [r3]
003b49b8: cmp      r2, r3
003b49bc: bne      #0x3b4bac
003b49c0: add      sp, sp, #0x48
003b49c4: pop      {r4, r5, r6, r7, r8, pc}
003b49c8: mov      r2, #1
003b49cc: strb     r2, [r0, r3]
003b49d0: bl       #0x38bd64
003b49d4: ldr      r3, [r5, #0x274]
003b49d8: cmp      r0, r3
003b49dc: bge      #0x3b49ac
003b49e0: mov      r0, r5
003b49e4: bl       #0x38cd48
003b49e8: mov      r0, r5
003b49ec: bl       #0x3a3094
003b49f0: cmp      r0, #0
003b49f4: beq      #0x3b4b98
003b49f8: ldr      r3, [pc, #0x1b8]
003b49fc: mov      r1, #0
003b4a00: mov      r2, #1
003b4a04: ldr      r3, [r4, r3]
003b4a08: ldr      r0, [r3, #0x40]
003b4a0c: bl       #0x36e478
003b4a10: ldr      r7, [r0, #0x660]
003b4a14: mov      r3, #0
003b4a18: str      r3, [sp, #8]
003b4a1c: cmp      r7, #0
003b4a20: str      r3, [sp]
003b4a24: str      r3, [sp, #4]
003b4a28: beq      #0x3b4a88
003b4a2c: mov      r1, sp
003b4a30: mov      r0, r7
003b4a34: bl       #0x393ae4
003b4a38: mov      r0, r7
003b4a3c: bl       #0x3935dc
003b4a40: ldr      r1, [r0]
003b4a44: mov      r7, r0
003b4a48: ldr      r0, [sp]
003b4a4c: bl       #0x30eba4
003b4a50: str      r0, [sp]
003b4a54: ldr      r1, [r7, #4]
003b4a58: ldr      r0, [sp, #4]
003b4a5c: bl       #0x30eba4
003b4a60: str      r0, [sp, #4]
003b4a64: ldr      r1, [r7, #8]
003b4a68: ldr      r0, [sp, #8]
003b4a6c: bl       #0x30eba4
003b4a70: mov      r1, sp
003b4a74: str      r0, [sp, #8]
003b4a78: mov      r2, #1
003b4a7c: mov      r0, r5
003b4a80: mov      r8, sp
003b4a84: bl       #0x393db4
003b4a88: ldr      r3, [r5, #0x2d8]
003b4a8c: cmp      r3, #0
003b4a90: beq      #0x3b4af8
003b4a94: ldr      r3, [r5]
003b4a98: mov      r0, r5
003b4a9c: mov      lr, pc
003b4aa0: ldr      pc, [r3, #0x28]
003b4aa4: cmp      r0, #0
003b4aa8: beq      #0x3b4b7c
003b4aac: ldr      r3, [pc, #0x104]
003b4ab0: ldr      r1, [pc, #0x104]
003b4ab4: add      r7, sp, #0x2c
003b4ab8: ldr      r3, [r4, r3]
003b4abc: add      r1, pc, r1
003b4ac0: add      r2, sp, #0x10
003b4ac4: ldr      r3, [r3, #0x10]
003b4ac8: mov      r0, r7
003b4acc: ldr      r8, [r3, #0x1c]
003b4ad0: bl       #0x3140ec
003b4ad4: add      r8, r8, #0x294
003b4ad8: mov      r0, r8
003b4adc: mov      r1, r7
003b4ae0: bl       #0x40c3cc
003b4ae4: mov      r8, r0
003b4ae8: mov      r0, r7
003b4aec: bl       #0x318254
003b4af0: ldr      r3, [r5, #0x2d8]
003b4af4: str      r8, [r3, #0x40]
003b4af8: add      r7, r5, #0x3c8
003b4afc: mov      r0, r7
003b4b00: ldr      r3, [r5, #0x3c8]
003b4b04: mov      lr, pc
003b4b08: ldr      pc, [r3, #0x10]
003b4b0c: ldr      r3, [r5]
003b4b10: mov      r0, r5
003b4b14: mov      lr, pc
003b4b18: ldr      pc, [r3, #0x28]
003b4b1c: cmp      r0, #0
003b4b20: beq      #0x3b49ac
003b4b24: ldr      r3, [pc, #0x8c]
003b4b28: mov      r1, r5
003b4b2c: ldr      r3, [r4, r3]
003b4b30: ldr      r0, [r3, #0x40]
003b4b34: bl       #0x36effc
003b4b38: cmp      r0, #0
003b4b3c: beq      #0x3b49ac
003b4b40: mov      r0, r7
003b4b44: add      r7, r5, #0x560
003b4b48: bl       #0x3d8894
003b4b4c: mov      r0, r7
003b4b50: mov      r1, #1
003b4b54: bl       #0x3e0810
003b4b58: mov      r0, r7
003b4b5c: mov      r1, #0xc2
003b4b60: mov      r2, #0
003b4b64: bl       #0x3df6e0
003b4b68: bic      r0, r0, r0, asr #31
003b4b6c: strb     r0, [r5, #0x3a8]
003b4b70: mov      r0, r5
003b4b74: bl       #0x3bc4a8
003b4b78: b        #0x3b49ac
003b4b7c: ldr      r3, [pc, #0x34]
003b4b80: ldr      r1, [pc, #0x38]
003b4b84: add      r7, sp, #0x14
003b4b88: ldr      r3, [r4, r3]
003b4b8c: add      r1, pc, r1
003b4b90: add      r2, sp, #0xc
003b4b94: b        #0x3b4ac4
003b4b98: mov      r0, r5
003b4b9c: bl       #0x3a307c
003b4ba0: cmp      r0, #0
003b4ba4: beq      #0x3b4a88
003b4ba8: b        #0x3b49f8
003b4bac: bl       #0x30e310
003b4bb0: subseq   r0, lr, r8, lsl #2
003b4bb4: andeq    r4, r0, ip, lsr #1
003b4bb8: strdeq   r3, r4, [r0], -r4
003b4bbc: subseq   pc, r0, ip, ror r3

# _ZN6CSIdle6OnInitEiP9CharacterP16CharStateMachine
003c7e60: push     {r4, r5, r6, r7, r8, sl, lr}
003c7e64: add      r5, r2, #0x4f0
003c7e68: add      r5, r5, #0xc
003c7e6c: sub      sp, sp, #0x7c
003c7e70: mov      r4, #0
003c7e74: mov      r6, r1
003c7e78: mov      sl, r2
003c7e7c: mov      r0, r5
003c7e80: mov      r2, #0x23
003c7e84: mov      r3, #3
003c7e88: str      r4, [sp, #0x70]
003c7e8c: str      r4, [sp, #0x74]
003c7e90: str      r4, [sp]
003c7e94: str      r4, [sp, #4]
003c7e98: bl       #0x3c7b18
003c7e9c: mov      r0, r5
003c7ea0: mov      r1, r6
003c7ea4: mov      r2, #0x22
003c7ea8: mov      r3, #3
003c7eac: str      r4, [sp, #0x68]
003c7eb0: str      r4, [sp, #0x6c]
003c7eb4: str      r4, [sp]
003c7eb8: str      r4, [sp, #4]
003c7ebc: ldr      r8, [pc, #0x1d8]
003c7ec0: bl       #0x3c7b18
003c7ec4: mov      r0, r5
003c7ec8: mov      r1, r6
003c7ecc: movw     r2, #0xc358
003c7ed0: mov      r3, #0xc
003c7ed4: str      r4, [sp, #0x60]
003c7ed8: str      r4, [sp, #0x64]
003c7edc: str      r4, [sp]
003c7ee0: str      r4, [sp, #4]
003c7ee4: bl       #0x3c7b18
003c7ee8: ldr      r3, [pc, #0x1b0]
003c7eec: add      r8, pc, r8
003c7ef0: mov      r0, r5
003c7ef4: ldr      ip, [r8, r3]
003c7ef8: mov      r1, r6
003c7efc: movw     r2, #0xc35a
003c7f00: mov      r3, #0xb
003c7f04: str      ip, [sp]
003c7f08: str      ip, [sp, #0x58]
003c7f0c: str      r4, [sp, #0x5c]
003c7f10: str      r4, [sp, #4]
003c7f14: bl       #0x3c7b18
003c7f18: ldr      r3, [pc, #0x184]
003c7f1c: mov      r0, r5
003c7f20: mov      r1, r6
003c7f24: ldr      r7, [r8, r3]
003c7f28: movw     r2, #0xc35b
003c7f2c: mov      r3, #0xa
003c7f30: str      r7, [sp, #0x50]
003c7f34: str      r4, [sp, #0x54]
003c7f38: str      r7, [sp]
003c7f3c: str      r4, [sp, #4]
003c7f40: bl       #0x3c7b18
003c7f44: mov      r0, r5
003c7f48: mov      r1, r6
003c7f4c: movw     r2, #0xc35c
003c7f50: mov      r3, #9
003c7f54: str      r7, [sp, #0x48]
003c7f58: str      r4, [sp, #0x4c]
003c7f5c: str      r7, [sp]
003c7f60: str      r4, [sp, #4]
003c7f64: bl       #0x3c7b18
003c7f68: mov      r0, r5
003c7f6c: mov      r1, r6
003c7f70: movw     r2, #0xc35d
003c7f74: mov      r3, #8
003c7f78: str      r7, [sp]
003c7f7c: str      r7, [sp, #0x40]
003c7f80: str      r4, [sp, #0x44]
003c7f84: str      r4, [sp, #4]
003c7f88: bl       #0x3c7b18
003c7f8c: mov      r0, r5
003c7f90: mov      r1, r6
003c7f94: movw     r2, #0xc356
003c7f98: mov      r3, #7
003c7f9c: str      r4, [sp, #0x38]
003c7fa0: str      r4, [sp, #0x3c]
003c7fa4: str      r4, [sp]
003c7fa8: str      r4, [sp, #4]
003c7fac: bl       #0x3c7b18
003c7fb0: mov      r0, r5
003c7fb4: mov      r1, r6
003c7fb8: movw     r2, #0xc355
003c7fbc: mov      r3, #6
003c7fc0: str      r4, [sp, #0x30]
003c7fc4: str      r4, [sp, #0x34]
003c7fc8: str      r4, [sp]
003c7fcc: str      r4, [sp, #4]
003c7fd0: bl       #0x3c7b18
003c7fd4: ldr      r3, [pc, #0xcc]
003c7fd8: mov      r0, r5
003c7fdc: mov      r1, r6
003c7fe0: ldr      ip, [r8, r3]
003c7fe4: movw     r2, #0xc354
003c7fe8: mov      r3, #5
003c7fec: str      ip, [sp]
003c7ff0: str      ip, [sp, #0x28]
003c7ff4: str      r4, [sp, #0x2c]
003c7ff8: str      r4, [sp, #4]
003c7ffc: bl       #0x3c7b18
003c8000: mov      r0, r5
003c8004: mov      r1, r6
003c8008: movw     r2, #0xc351
003c800c: mov      r3, #4
003c8010: str      r4, [sp, #0x20]
003c8014: str      r4, [sp, #0x24]
003c8018: str      r4, [sp]
003c801c: str      r4, [sp, #4]
003c8020: bl       #0x3c7b18
003c8024: mov      r0, r5
003c8028: mov      r1, r6
003c802c: movw     r2, #0xc352
003c8030: mov      r3, #3
003c8034: str      r4, [sp, #0x18]
003c8038: str      r4, [sp, #0x1c]
003c803c: str      r4, [sp]
003c8040: str      r4, [sp, #4]
003c8044: bl       #0x3c7b18
003c8048: mov      r0, r5
003c804c: mov      r1, r6
003c8050: movw     r2, #0xc353
003c8054: mov      r3, #0xd
003c8058: str      r4, [sp, #0x10]
003c805c: str      r4, [sp, #0x14]
003c8060: str      r4, [sp]
003c8064: str      r4, [sp, #4]
003c8068: bl       #0x3c7b18
003c806c: mov      r0, r5
003c8070: mov      r1, r6
003c8074: movw     r2, #0xc357
003c8078: mov      r3, #0xf
003c807c: str      r4, [sp, #8]
003c8080: str      r4, [sp, #0xc]
003c8084: str      r4, [sp]
003c8088: str      r4, [sp, #4]
003c808c: bl       #0x3c7b18
003c8090: strb     r4, [sl, #0x538]
003c8094: add      sp, sp, #0x7c
003c8098: pop      {r4, r5, r6, r7, r8, sl, pc}
003c809c: subseq   ip, ip, r4, lsr #23
003c80a0: andeq    r2, r0, r4, lsl #29
003c80a4: andeq    r3, r0, ip, asr #9
003c80a8: ldrdeq   r0, r1, [r0], -r4

# _ZN16CharStateMachine13RegisterStateEi
003c7318: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c731c: ldr      r4, [pc, #0x178]
003c7320: ldr      r5, [pc, #0x178]
003c7324: ldr      r3, [r0, #0xc]
003c7328: add      r4, pc, r4
003c732c: ldr      r2, [r4, r5]
003c7330: sub      sp, sp, #0x2c
003c7334: cmp      r3, #0
003c7338: ldr      r2, [r2]
003c733c: mov      r7, r0
003c7340: str      r1, [sp, #4]
003c7344: add      r8, r0, #8
003c7348: str      r2, [sp, #0x24]
003c734c: beq      #0x3c73b4
003c7350: mov      r0, r8
003c7354: b        #0x3c7360
003c7358: mov      r0, r3
003c735c: mov      r3, r2
003c7360: ldr      r2, [r3, #0x10]
003c7364: cmp      r2, r1
003c7368: ldrlt    r2, [r3, #0xc]
003c736c: ldrge    r2, [r3, #8]
003c7370: movlt    r3, r0
003c7374: cmp      r2, #0
003c7378: bne      #0x3c7358
003c737c: cmp      r8, r3
003c7380: beq      #0x3c73c0
003c7384: ldr      r2, [r3, #0x10]
003c7388: cmp      r1, r2
003c738c: blt      #0x3c73b4
003c7390: cmp      r8, r3
003c7394: beq      #0x3c73c0
003c7398: ldr      r3, [r4, r5]
003c739c: ldr      r2, [sp, #0x24]
003c73a0: ldr      r3, [r3]
003c73a4: cmp      r2, r3
003c73a8: bne      #0x3c7498
003c73ac: add      sp, sp, #0x2c
003c73b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c73b4: mov      r3, r8
003c73b8: cmp      r8, r3
003c73bc: bne      #0x3c7398
003c73c0: ldr      sl, [pc, #0xdc]
003c73c4: ldr      r1, [sp, #4]
003c73c8: mov      r3, #0
003c73cc: add      sl, pc, sl
003c73d0: b        #0x3c73e0
003c73d4: add      r3, r3, #1
003c73d8: cmp      r3, #0x14
003c73dc: beq      #0x3c7398
003c73e0: ldr      r2, [sl, r3, lsl #3]
003c73e4: lsl      sb, r3, #3
003c73e8: cmp      r1, r2
003c73ec: bne      #0x3c73d4
003c73f0: add      fp, sp, #4
003c73f4: mov      r1, fp
003c73f8: mov      r0, r8
003c73fc: bl       #0x3c71e0
003c7400: ldr      r3, [sp, #4]
003c7404: mov      r1, fp
003c7408: add      sl, sl, sb
003c740c: str      r3, [r0]
003c7410: mov      r0, r8
003c7414: bl       #0x3c71e0
003c7418: str      r0, [sp]
003c741c: mov      lr, pc
003c7420: ldr      pc, [sl, #4]
003c7424: ldr      r3, [sp]
003c7428: mov      r1, fp
003c742c: add      r6, sp, #0xc
003c7430: str      r0, [r3, #4]
003c7434: mov      r0, r8
003c7438: bl       #0x3c71e0
003c743c: ldr      ip, [r0, #4]
003c7440: ldr      r1, [sp, #4]
003c7444: ldr      r2, [r7, #4]
003c7448: mov      r3, r7
003c744c: mov      r0, ip
003c7450: ldr      ip, [ip]
003c7454: mov      lr, pc
003c7458: ldr      pc, [ip, #8]
003c745c: ldr      r3, [pc, #0x44]
003c7460: ldr      r7, [r4, r3]
003c7464: mov      r0, r7
003c7468: bl       #0x337888
003c746c: ldr      r1, [pc, #0x38]
003c7470: add      r2, sp, #8
003c7474: mov      r0, r6
003c7478: add      r1, pc, r1
003c747c: bl       #0x3140ec
003c7480: mov      r0, r7
003c7484: mov      r1, r6
003c7488: bl       #0x337a88
003c748c: mov      r0, r6
003c7490: bl       #0x318254
003c7494: b        #0x3c7398
003c7498: bl       #0x30e310
003c749c: subseq   sp, ip, r8, ror #14
003c74a0: andeq    r4, r0, ip, lsr #1
003c74a4: subseq   pc, sb, ip, ror #5
003c74a8: andeq    r0, r0, r4, lsl #17
003c74ac: strheq   sp, [pc], #-0xa8

# _ZN9Character7InitAllEv
003b35f0: push     {r4, lr}
003b35f4: mov      r4, r0
003b35f8: ldr      r3, [r0]
003b35fc: mov      lr, pc
003b3600: ldr      pc, [r3, #0x1c]
003b3604: mov      r0, r4
003b3608: ldr      r3, [r4]
003b360c: mov      lr, pc
003b3610: ldr      pc, [r3, #0x58]
003b3614: pop      {r4, pc}

# _ZN16CharStateMachine16SM_RegisterEventEiiiM9CharacterFbiPviRiE
003c7b18: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c7b1c: ldr      r4, [r0, #0xc]
003c7b20: sub      sp, sp, #0x30
003c7b24: mov      r5, r2
003c7b28: cmp      r4, #0
003c7b2c: mov      sl, r3
003c7b30: ldr      r7, [sp, #0x50]
003c7b34: ldr      r8, [sp, #0x54]
003c7b38: add      r0, r0, #8
003c7b3c: beq      #0x3c7c48
003c7b40: mov      r2, r0
003c7b44: b        #0x3c7b4c
003c7b48: mov      r4, r3
003c7b4c: ldr      r3, [r4, #0x10]
003c7b50: cmp      r1, r3
003c7b54: ldrgt    r3, [r4, #0xc]
003c7b58: ldrle    r3, [r4, #8]
003c7b5c: movgt    r4, r2
003c7b60: mov      r2, r4
003c7b64: cmp      r3, #0
003c7b68: bne      #0x3c7b48
003c7b6c: cmp      r0, r4
003c7b70: beq      #0x3c7c40
003c7b74: ldr      r3, [r4, #0x10]
003c7b78: cmp      r1, r3
003c7b7c: blt      #0x3c7c48
003c7b80: cmp      r0, r4
003c7b84: beq      #0x3c7c40
003c7b88: ldr      ip, [r4, #0x20]
003c7b8c: add      r6, r4, #0x1c
003c7b90: cmp      ip, #0
003c7b94: moveq    ip, r6
003c7b98: beq      #0x3c7bc8
003c7b9c: mov      r2, r6
003c7ba0: b        #0x3c7ba8
003c7ba4: mov      ip, r3
003c7ba8: ldr      r3, [ip, #0x10]
003c7bac: cmp      r3, r5
003c7bb0: ldrlt    r3, [ip, #0xc]
003c7bb4: ldrge    r3, [ip, #8]
003c7bb8: movlt    ip, r2
003c7bbc: mov      r2, ip
003c7bc0: cmp      r3, #0
003c7bc4: bne      #0x3c7ba4
003c7bc8: cmp      r6, ip
003c7bcc: beq      #0x3c7c88
003c7bd0: ldr      r2, [ip, #0x10]
003c7bd4: mov      r3, ip
003c7bd8: cmp      r2, r5
003c7bdc: bgt      #0x3c7c88
003c7be0: str      r7, [r3, #0x14]
003c7be4: str      r8, [r3, #0x18]
003c7be8: ldr      ip, [r4, #0x20]
003c7bec: cmp      ip, #0
003c7bf0: moveq    ip, r6
003c7bf4: beq      #0x3c7c24
003c7bf8: mov      r2, r6
003c7bfc: b        #0x3c7c04
003c7c00: mov      ip, r3
003c7c04: ldr      r3, [ip, #0x10]
003c7c08: cmp      r3, r5
003c7c0c: ldrlt    r3, [ip, #0xc]
003c7c10: ldrge    r3, [ip, #8]
003c7c14: movlt    ip, r2
003c7c18: mov      r2, ip
003c7c1c: cmp      r3, #0
003c7c20: bne      #0x3c7c00
003c7c24: cmp      r6, ip
003c7c28: beq      #0x3c7c50
003c7c2c: ldr      r2, [ip, #0x10]
003c7c30: mov      r3, ip
003c7c34: cmp      r2, r5
003c7c38: bgt      #0x3c7c50
003c7c3c: str      sl, [r3, #0x1c]
003c7c40: add      sp, sp, #0x30
003c7c44: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c7c48: mov      r4, r0
003c7c4c: b        #0x3c7b80
003c7c50: mov      lr, #0
003c7c54: mov      r3, sp
003c7c58: mov      r1, r6
003c7c5c: add      r0, sp, #0x20
003c7c60: add      r2, sp, #0x24
003c7c64: mvn      r4, #0
003c7c68: str      r5, [sp]
003c7c6c: str      r4, [sp, #0xc]
003c7c70: str      lr, [sp, #4]
003c7c74: str      ip, [sp, #0x24]
003c7c78: str      lr, [sp, #8]
003c7c7c: bl       #0x3c77a4
003c7c80: ldr      r3, [sp, #0x20]
003c7c84: b        #0x3c7c3c
003c7c88: mov      lr, #0
003c7c8c: add      r3, sp, #0x10
003c7c90: add      r0, sp, #0x28
003c7c94: mov      r1, r6
003c7c98: add      r2, sp, #0x2c
003c7c9c: mvn      sb, #0
003c7ca0: str      sb, [sp, #0x1c]
003c7ca4: str      lr, [sp, #0x14]
003c7ca8: str      ip, [sp, #0x2c]
003c7cac: str      r5, [sp, #0x10]
003c7cb0: str      lr, [sp, #0x18]
003c7cb4: bl       #0x3c77a4
003c7cb8: ldr      r3, [sp, #0x28]
003c7cbc: b        #0x3c7be0

# _ZN6CSMove6OnInitEiP9CharacterP16CharStateMachine
003c80ac: push     {r4, r5, r6, r7, r8, lr}
003c80b0: add      r5, r2, #0x4f0
003c80b4: add      r5, r5, #0xc
003c80b8: sub      sp, sp, #0x60
003c80bc: mov      r4, #0
003c80c0: mov      r6, r1
003c80c4: mov      r0, r5
003c80c8: mov      r2, #0x3f
003c80cc: mov      r3, #3
003c80d0: str      r4, [sp, #0x58]
003c80d4: str      r4, [sp, #0x5c]
003c80d8: str      r4, [sp]
003c80dc: str      r4, [sp, #4]
003c80e0: ldr      r8, [pc, #0x18c]
003c80e4: bl       #0x3c7b18
003c80e8: mov      r0, r5
003c80ec: mov      r1, r6
003c80f0: movw     r2, #0xc358
003c80f4: mov      r3, #0xc
003c80f8: str      r4, [sp, #0x50]
003c80fc: str      r4, [sp, #0x54]
003c8100: str      r4, [sp]
003c8104: str      r4, [sp, #4]
003c8108: bl       #0x3c7b18
003c810c: ldr      r3, [pc, #0x164]
003c8110: add      r8, pc, r8
003c8114: mov      r0, r5
003c8118: ldr      ip, [r8, r3]
003c811c: mov      r1, r6
003c8120: movw     r2, #0xc35a
003c8124: mov      r3, #0xb
003c8128: str      ip, [sp]
003c812c: str      ip, [sp, #0x48]
003c8130: str      r4, [sp, #0x4c]
003c8134: str      r4, [sp, #4]
003c8138: bl       #0x3c7b18
003c813c: ldr      r3, [pc, #0x138]
003c8140: mov      r0, r5
003c8144: mov      r1, r6
003c8148: ldr      r7, [r8, r3]
003c814c: movw     r2, #0xc35b
003c8150: mov      r3, #0xa
003c8154: str      r7, [sp, #0x40]
003c8158: str      r4, [sp, #0x44]
003c815c: str      r7, [sp]
003c8160: str      r4, [sp, #4]
003c8164: bl       #0x3c7b18
003c8168: mov      r0, r5
003c816c: mov      r1, r6
003c8170: movw     r2, #0xc35c
003c8174: mov      r3, #9
003c8178: str      r7, [sp, #0x38]
003c817c: str      r4, [sp, #0x3c]
003c8180: str      r7, [sp]
003c8184: str      r4, [sp, #4]
003c8188: bl       #0x3c7b18
003c818c: mov      r0, r5
003c8190: mov      r1, r6
003c8194: movw     r2, #0xc35d
003c8198: mov      r3, #8
003c819c: str      r7, [sp]
003c81a0: str      r7, [sp, #0x30]
003c81a4: str      r4, [sp, #0x34]
003c81a8: str      r4, [sp, #4]
003c81ac: bl       #0x3c7b18
003c81b0: mov      r0, r5
003c81b4: mov      r1, r6
003c81b8: movw     r2, #0xc356
003c81bc: mov      r3, #7
003c81c0: str      r4, [sp, #0x28]
003c81c4: str      r4, [sp, #0x2c]
003c81c8: str      r4, [sp]
003c81cc: str      r4, [sp, #4]
003c81d0: bl       #0x3c7b18
003c81d4: mov      r0, r5
003c81d8: mov      r1, r6
003c81dc: movw     r2, #0xc355
003c81e0: mov      r3, #6
003c81e4: str      r4, [sp, #0x20]
003c81e8: str      r4, [sp, #0x24]
003c81ec: str      r4, [sp]
003c81f0: str      r4, [sp, #4]
003c81f4: bl       #0x3c7b18
003c81f8: ldr      r3, [pc, #0x80]
003c81fc: mov      r0, r5
003c8200: mov      r1, r6
003c8204: ldr      ip, [r8, r3]
003c8208: movw     r2, #0xc354
003c820c: mov      r3, #5
003c8210: str      ip, [sp]
003c8214: str      ip, [sp, #0x18]
003c8218: str      r4, [sp, #0x1c]
003c821c: str      r4, [sp, #4]
003c8220: bl       #0x3c7b18
003c8224: mov      r0, r5
003c8228: mov      r1, r6
003c822c: movw     r2, #0xc353
003c8230: mov      r3, #0xd
003c8234: str      r4, [sp, #0x10]
003c8238: str      r4, [sp, #0x14]
003c823c: str      r4, [sp]
003c8240: str      r4, [sp, #4]
003c8244: bl       #0x3c7b18
003c8248: mov      r0, r5
003c824c: mov      r1, r6
003c8250: movw     r2, #0xc357
003c8254: mov      r3, #0xf
003c8258: str      r4, [sp, #4]
003c825c: str      r4, [sp, #8]
003c8260: str      r4, [sp, #0xc]
003c8264: str      r4, [sp]
003c8268: bl       #0x3c7b18
003c826c: add      sp, sp, #0x60
003c8270: pop      {r4, r5, r6, r7, r8, pc}
003c8274: subseq   ip, ip, r0, lsl #19
003c8278: andeq    r2, r0, r4, lsl #29
003c827c: andeq    r3, r0, ip, asr #9
003c8280: ldrdeq   r0, r1, [r0], -r4

# _ZN6CharAI13AI_ScriptInitEv
003cfde4: push     {r4, r5, r6, r7, lr}
003cfde8: ldr      r3, [r0, #4]
003cfdec: sub      sp, sp, #0xc
003cfdf0: mov      r4, r0
003cfdf4: mov      r0, r3
003cfdf8: ldr      r3, [r3]
003cfdfc: mov      lr, pc
003cfe00: ldr      pc, [r3, #0x34]
003cfe04: ldr      r5, [pc, #0x110]
003cfe08: cmp      r0, #0
003cfe0c: add      r5, pc, r5
003cfe10: bne      #0x3cfed0
003cfe14: ldr      r1, [r4, #0x10]
003cfe18: cmn      r1, #1
003cfe1c: beq      #0x3cfe2c
003cfe20: ldr      r0, [r4, #4]
003cfe24: add      r0, r0, #0x3b4
003cfe28: bl       #0x3db2d8
003cfe2c: ldr      r6, [pc, #0xec]
003cfe30: ldr      r1, [pc, #0xec]
003cfe34: ldr      r2, [pc, #0xec]
003cfe38: ldr      r3, [r5, r6]
003cfe3c: add      r1, pc, r1
003cfe40: add      r2, pc, r2
003cfe44: ldr      r0, [r3, #0x2c]
003cfe48: ldr      r7, [r4, #4]
003cfe4c: bl       #0x4c4bdc
003cfe50: add      r7, r7, #0x3b4
003cfe54: mov      r1, r0
003cfe58: mov      ip, #0
003cfe5c: mov      r0, r7
003cfe60: mvn      r2, #0
003cfe64: mov      r3, #0x33
003cfe68: str      ip, [sp]
003cfe6c: bl       #0x3dbe24
003cfe70: ldr      r1, [r4, #0x14]
003cfe74: str      r0, [r4, #0x10]
003cfe78: cmn      r1, #1
003cfe7c: beq      #0x3cfe8c
003cfe80: ldr      r0, [r4, #4]
003cfe84: add      r0, r0, #0x3b4
003cfe88: bl       #0x3db2d8
003cfe8c: ldr      r3, [r5, r6]
003cfe90: ldr      r1, [pc, #0x94]
003cfe94: ldr      r2, [pc, #0x94]
003cfe98: ldr      r0, [r3, #0x2c]
003cfe9c: add      r1, pc, r1
003cfea0: add      r2, pc, r2
003cfea4: ldr      r5, [r4, #4]
003cfea8: bl       #0x4c4bdc
003cfeac: add      r5, r5, #0x3b4
003cfeb0: mov      r1, r0
003cfeb4: mov      ip, #0
003cfeb8: mov      r0, r5
003cfebc: mvn      r2, #0
003cfec0: mov      r3, #0x34
003cfec4: str      ip, [sp]
003cfec8: bl       #0x3dbe24
003cfecc: str      r0, [r4, #0x14]
003cfed0: ldr      r3, [r4, #0x1c]
003cfed4: cmp      r3, #0
003cfed8: beq      #0x3cff14
003cfedc: mov      r0, r3
003cfee0: ldr      r3, [r3]
003cfee4: mov      lr, pc
003cfee8: ldr      pc, [r3, #8]
003cfeec: ldr      r3, [r4, #0x1c]
003cfef0: mov      r0, r3
003cfef4: ldr      r3, [r3]
003cfef8: mov      lr, pc
003cfefc: ldr      pc, [r3, #0xc]
003cff00: ldr      r3, [r4, #0x1c]
003cff04: mov      r0, r3
003cff08: ldr      r3, [r3]
003cff0c: mov      lr, pc
003cff10: ldr      pc, [r3, #0x10]
003cff14: add      sp, sp, #0xc
003cff18: pop      {r4, r5, r6, r7, pc}
003cff1c: subseq   r4, ip, r4, lsl #25
003cff20: strdeq   r3, r4, [r0], -r4
003cff24: subeq    r1, pc, r4, lsl sb
003cff28: subeq    r5, pc, r8, lsr r6
003cff2c: strheq   r1, [pc], #-0x84
003cff30: subeq    r5, pc, r0, ror #11
