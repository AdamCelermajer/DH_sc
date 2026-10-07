
# _ZN14CharProperties19LoadGearsPropertiesEv
003df480: push     {r4, r5, r6, r7, r8, lr}
003df484: mov      r5, r0
003df488: ldr      r0, [r5, #4]
003df48c: mov      r7, #0
003df490: add      r0, r0, #0x37c
003df494: bl       #0x3ffd20
003df498: cmp      r7, r0
003df49c: bhs      #0x3df544
003df4a0: ldr      r0, [r5, #4]
003df4a4: mov      r1, r7
003df4a8: add      r0, r0, #0x37c
003df4ac: bl       #0x3ffe3c
003df4b0: subs     r6, r0, #0
003df4b4: beq      #0x3df52c
003df4b8: bl       #0x3f9e00
003df4bc: mov      r8, r0
003df4c0: ldr      r0, [r5, #4]
003df4c4: mov      r1, r7
003df4c8: mov      r4, #0
003df4cc: add      r0, r0, #0x37c
003df4d0: bl       #0x40022c
003df4d4: mov      r1, r8
003df4d8: mov      r2, r0
003df4dc: mov      r0, r5
003df4e0: bl       #0x3e3154
003df4e4: b        #0x3df514
003df4e8: bl       #0x3fa038
003df4ec: mov      r8, r0
003df4f0: ldr      r0, [r5, #4]
003df4f4: mov      r1, r7
003df4f8: add      r4, r4, #1
003df4fc: add      r0, r0, #0x37c
003df500: bl       #0x40022c
003df504: mov      r1, r8
003df508: mov      r2, r0
003df50c: mov      r0, r5
003df510: bl       #0x3e32b4
003df514: mov      r0, r6
003df518: bl       #0x3f9e80
003df51c: cmp      r4, r0
003df520: mov      r1, r4
003df524: mov      r0, r6
003df528: blo      #0x3df4e8
003df52c: ldr      r0, [r5, #4]
003df530: add      r7, r7, #1
003df534: add      r0, r0, #0x37c
003df538: bl       #0x3ffd20
003df53c: cmp      r7, r0
003df540: blo      #0x3df4a0
003df544: pop      {r4, r5, r6, r7, r8, pc}

# _ZN9Character12ValidateHPMPEv
003bd140: push     {r4, r5, r6, lr}
003bd144: add      r5, r0, #0xff0
003bd148: add      r4, r0, #0x560
003bd14c: add      r5, r5, #4
003bd150: mov      r1, r5
003bd154: mov      r2, #0x24
003bd158: mov      r0, r4
003bd15c: bl       #0x3dedb4
003bd160: mov      r1, r5
003bd164: mov      r6, r0
003bd168: mov      r2, #0x26
003bd16c: mov      r0, r4
003bd170: bl       #0x3dedb4
003bd174: mov      r1, #0x24
003bd178: cmp      r0, r6
003bd17c: movlt    r2, r0
003bd180: movge    r2, r6
003bd184: mov      r0, r4
003bd188: bl       #0x3e07a0
003bd18c: mov      r1, r5
003bd190: mov      r0, r4
003bd194: mov      r2, #0x29
003bd198: bl       #0x3dedb4
003bd19c: mov      r1, r5
003bd1a0: mov      r6, r0
003bd1a4: mov      r2, #0x2b
003bd1a8: mov      r0, r4
003bd1ac: bl       #0x3dedb4
003bd1b0: mov      r1, #0x29
003bd1b4: cmp      r0, r6
003bd1b8: movlt    r2, r0
003bd1bc: movge    r2, r6
003bd1c0: mov      r0, r4
003bd1c4: pop      {r4, r5, r6, lr}
003bd1c8: b        #0x3e07a0

# _ZN14CharProperties20ResetGearsPropertiesEv
003defac: add      r1, r0, #0x710
003defb0: b        #0x3def34

# _ZN9Character14INV_UpdateSkinEv
003a999c: ldr      r1, [pc, #0x340]
003a99a0: ldr      r2, [pc, #0x340]
003a99a4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a99a8: add      r1, pc, r1
003a99ac: ldr      r3, [r1, r2]
003a99b0: sub      sp, sp, #0x64
003a99b4: add      sb, sp, #0x44
003a99b8: ldr      r3, [r3]
003a99bc: str      r1, [sp, #8]
003a99c0: mov      r6, r0
003a99c4: mov      r1, #0x10
003a99c8: mov      r0, sb
003a99cc: str      r3, [sp, #0x5c]
003a99d0: str      r2, [sp, #0x14]
003a99d4: str      sb, [sp, #0x54]
003a99d8: str      sb, [sp, #0x58]
003a99dc: bl       #0x31167c
003a99e0: ldr      r3, [sp, #0x54]
003a99e4: mov      r4, #0
003a99e8: strb     r4, [r3]
003a99ec: ldr      r3, [r6, #0x2d8]
003a99f0: cmp      r3, r4
003a99f4: beq      #0x3a9b78
003a99f8: ldr      r3, [pc, #0x2ec]
003a99fc: add      r7, r6, #0x37c
003a9a00: mvn      r5, #0
003a9a04: str      r3, [sp, #0xc]
003a9a08: ldr      r3, [pc, #0x2e0]
003a9a0c: mov      fp, sb
003a9a10: add      r3, pc, r3
003a9a14: str      r3, [sp, #0x18]
003a9a18: ldr      r3, [pc, #0x2d4]
003a9a1c: ldr      r1, [sp, #0x18]
003a9a20: add      r3, pc, r3
003a9a24: str      r3, [sp, #0x1c]
003a9a28: ldr      r2, [sp, #0x1c]
003a9a2c: ldr      r3, [pc, #0x2c4]
003a9a30: add      r1, r1, #7
003a9a34: add      r2, r2, #0xd
003a9a38: add      r3, pc, r3
003a9a3c: str      r3, [sp, #0x10]
003a9a40: str      r1, [sp, #0x20]
003a9a44: str      r2, [sp, #0x24]
003a9a48: b        #0x3a9adc
003a9a4c: mov      r1, r8
003a9a50: ldr      r0, [r6, #0x2d8]
003a9a54: bl       #0x470e5c
003a9a58: ldr      r2, [sp, #0x58]
003a9a5c: mov      r1, r0
003a9a60: mov      sl, r0
003a9a64: ldr      r0, [r6, #0x2d8]
003a9a68: bl       #0x474568
003a9a6c: cmn      r0, #1
003a9a70: mov      r3, r0
003a9a74: beq      #0x3a9c28
003a9a78: ldr      r2, [sp, #8]
003a9a7c: ldr      r1, [sp, #0xc]
003a9a80: add      r8, sp, #0x2c
003a9a84: str      r3, [sp, #4]
003a9a88: ldr      sb, [r2, r1]
003a9a8c: mov      r0, sb
003a9a90: bl       #0x337888
003a9a94: add      r2, sp, #0x28
003a9a98: ldr      r1, [sp, #0x10]
003a9a9c: mov      r0, r8
003a9aa0: bl       #0x3140ec
003a9aa4: mov      r1, r8
003a9aa8: mov      r0, sb
003a9aac: bl       #0x337a88
003a9ab0: mov      r0, r8
003a9ab4: bl       #0x318254
003a9ab8: ldr      r3, [sp, #4]
003a9abc: mov      r1, sl
003a9ac0: ldr      r0, [r6, #0x2d8]
003a9ac4: mov      r2, r3
003a9ac8: bl       #0x470e18
003a9acc: add      r4, r4, #1
003a9ad0: cmp      r4, #9
003a9ad4: add      r5, r5, #1
003a9ad8: beq      #0x3a9b74
003a9adc: mov      r1, r4
003a9ae0: mov      r0, r7
003a9ae4: bl       #0x3ffe3c
003a9ae8: mov      r1, r4
003a9aec: mov      sl, r0
003a9af0: mov      r0, r7
003a9af4: bl       #0x3ffd54
003a9af8: subs     r8, r0, #0
003a9afc: beq      #0x3a9acc
003a9b00: cmp      sl, #0
003a9b04: beq      #0x3a9ba4
003a9b08: mov      r0, sl
003a9b0c: bl       #0x3f9e08
003a9b10: ldr      sl, [r0, #0x50]
003a9b14: mov      r0, sl
003a9b18: bl       #0x30de54
003a9b1c: mov      r1, sl
003a9b20: add      r2, sl, r0
003a9b24: mov      r0, fp
003a9b28: bl       #0x3109e0
003a9b2c: cmp      r5, #1
003a9b30: bhi      #0x3a9a4c
003a9b34: ldr      r8, [sp, #0x58]
003a9b38: ldr      r1, [pc, #0x1bc]
003a9b3c: mov      r0, r8
003a9b40: add      r1, pc, r1
003a9b44: bl       #0x30ebd4
003a9b48: cmp      r0, #0
003a9b4c: movne    r3, #0
003a9b50: beq      #0x3a9c68
003a9b54: mov      r2, r4
003a9b58: mov      r1, r8
003a9b5c: ldr      r0, [r6, #0x2d8]
003a9b60: add      r4, r4, #1
003a9b64: bl       #0x473cd8
003a9b68: cmp      r4, #9
003a9b6c: add      r5, r5, #1
003a9b70: bne      #0x3a9adc
003a9b74: mov      sb, fp
003a9b78: mov      r0, sb
003a9b7c: bl       #0x318254
003a9b80: ldr      r2, [sp, #8]
003a9b84: ldr      r1, [sp, #0x14]
003a9b88: ldr      r3, [r2, r1]
003a9b8c: ldr      r2, [sp, #0x5c]
003a9b90: ldr      r3, [r3]
003a9b94: cmp      r2, r3
003a9b98: bne      #0x3a9ce0
003a9b9c: add      sp, sp, #0x64
003a9ba0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a9ba4: bl       #0x30de54
003a9ba8: mov      r1, r8
003a9bac: add      r2, r8, r0
003a9bb0: mov      r0, fp
003a9bb4: bl       #0x3109e0
003a9bb8: mov      r0, fp
003a9bbc: ldr      r1, [sp, #0x18]
003a9bc0: ldr      r2, [sp, #0x20]
003a9bc4: bl       #0x310804
003a9bc8: cmp      r5, #1
003a9bcc: bls      #0x3a9bf8
003a9bd0: mov      r1, r8
003a9bd4: ldr      r0, [r6, #0x2d8]
003a9bd8: bl       #0x470e5c
003a9bdc: ldr      r2, [sp, #0x58]
003a9be0: mov      r1, r0
003a9be4: mov      sl, r0
003a9be8: ldr      r0, [r6, #0x2d8]
003a9bec: bl       #0x474568
003a9bf0: mov      r3, r0
003a9bf4: b        #0x3a9a78
003a9bf8: ldr      r1, [pc, #0x100]
003a9bfc: ldr      r0, [sp, #0x58]
003a9c00: add      r1, pc, r1
003a9c04: bl       #0x30ebd4
003a9c08: cmp      r0, #0
003a9c0c: moveq    r3, r4
003a9c10: movne    r3, #0
003a9c14: mov      r1, sl
003a9c18: ldr      r0, [r6, #0x2d8]
003a9c1c: mov      r2, r4
003a9c20: bl       #0x473cd8
003a9c24: b        #0x3a9acc
003a9c28: mov      r0, r8
003a9c2c: bl       #0x30de54
003a9c30: mov      r1, r8
003a9c34: add      r2, r8, r0
003a9c38: mov      r0, fp
003a9c3c: bl       #0x3109e0
003a9c40: ldr      r1, [sp, #0x1c]
003a9c44: ldr      r2, [sp, #0x24]
003a9c48: mov      r0, fp
003a9c4c: bl       #0x310804
003a9c50: ldr      r0, [r6, #0x2d8]
003a9c54: mov      r1, sl
003a9c58: ldr      r2, [sp, #0x58]
003a9c5c: bl       #0x474568
003a9c60: mov      r3, r0
003a9c64: b        #0x3a9a78
003a9c68: ldr      r1, [pc, #0x94]
003a9c6c: mov      r0, r8
003a9c70: add      r1, pc, r1
003a9c74: bl       #0x30ebd4
003a9c78: cmp      r0, #0
003a9c7c: beq      #0x3a9c88
003a9c80: mov      r3, #2
003a9c84: b        #0x3a9b54
003a9c88: ldr      r1, [pc, #0x78]
003a9c8c: mov      r0, r8
003a9c90: add      r1, pc, r1
003a9c94: bl       #0x30ebd4
003a9c98: subs     r0, r0, #0
003a9c9c: movne    r0, #1
003a9ca0: cmp      r4, #2
003a9ca4: movne    r0, #0
003a9ca8: cmp      r0, #0
003a9cac: moveq    r3, r4
003a9cb0: beq      #0x3a9b54
003a9cb4: ldr      r1, [pc, #0x50]
003a9cb8: mov      r0, r8
003a9cbc: add      r1, pc, r1
003a9cc0: bl       #0x30ebd4
003a9cc4: cmp      r0, #0
003a9cc8: beq      #0x3a9c80
003a9ccc: mov      r3, #0x4c
003a9cd0: strb     r3, [r0]
003a9cd4: ldr      r8, [sp, #0x58]
003a9cd8: mov      r3, #2
003a9cdc: b        #0x3a9b54
003a9ce0: bl       #0x30e310
003a9ce4: subseq   fp, lr, r8, ror #1
003a9ce8: andeq    r4, r0, ip, lsr #1
003a9cec: andeq    r0, r0, r4, lsl #17
003a9cf0: subseq   sb, r1, r8, lsr fp
003a9cf4: subseq   sb, r1, r8, lsl fp
003a9cf8: subseq   sb, r1, r8, lsl fp
003a9cfc: ldrsbeq  sb, [r1], #-0x98
003a9d00: subseq   sb, r1, r8, lsl sb
003a9d04: ldrheq   sb, [r1], #-0x80

# _ZN14CharProperties16RecalcPropertiesEb
003e0810: cmp      r1, #0
003e0814: push     {r4, r5, r6, lr}
003e0818: mov      r4, r0
003e081c: bne      #0x3e0840
003e0820: mov      r5, #0
003e0824: mov      r1, r5
003e0828: mov      r0, r4
003e082c: add      r5, r5, #1
003e0830: bl       #0x3dfe60
003e0834: cmp      r5, #0xe0
003e0838: bne      #0x3e0824
003e083c: pop      {r4, r5, r6, pc}
003e0840: add      r1, r0, #8
003e0844: ldr      r2, [r0, #0x74]
003e0848: mov      r3, #0
003e084c: bl       #0x3e2e20
003e0850: b        #0x3e0820
