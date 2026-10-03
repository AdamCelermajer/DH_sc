
# _ZN14PhysicalObject5unpinEv
0046eae0: push     {r4, lr}
0046eae4: ldrb     r3, [r0, #0x27]
0046eae8: mov      r4, r0
0046eaec: cmp      r3, #0
0046eaf0: beq      #0x46eb1c
0046eaf4: mov      r3, #0
0046eaf8: strb     r3, [r0, #0x27]
0046eafc: ldr      r0, [r0, #0x14]
0046eb00: bl       #0x7e1818
0046eb04: ldr      r3, [r4, #0x14]
0046eb08: mov      r1, #0
0046eb0c: ldrh     r2, [r3]
0046eb10: str      r1, [r3, #0x8c]
0046eb14: bic      r2, r2, #8
0046eb18: strh     r2, [r3]
0046eb1c: pop      {r4, pc}

# _ZN6b2Body7SetMassEPK10b2MassData
007e1b28: push     {r4, r5, r6, r7, r8, lr}
007e1b2c: ldr      r2, [r0, #0x58]
007e1b30: mov      r3, #0x19000
007e1b34: add      r3, r3, #0x1d4
007e1b38: ldrb     r3, [r2, r3]
007e1b3c: mov      r4, r0
007e1b40: mov      r5, r1
007e1b44: cmp      r3, #0
007e1b48: bne      #0x7e1ce8
007e1b4c: mov      r1, #0
007e1b50: str      r1, [r0, #0x78]
007e1b54: str      r1, [r0, #0x7c]
007e1b58: str      r1, [r0, #0x80]
007e1b5c: ldr      r6, [r5]
007e1b60: str      r6, [r0, #0x74]
007e1b64: mov      r0, r6
007e1b68: bl       #0x30e2f8
007e1b6c: cmp      r0, #0
007e1b70: beq      #0x7e1b84
007e1b74: mov      r1, r6
007e1b78: mov      r0, #0x3f800000
007e1b7c: bl       #0x30ec94
007e1b80: str      r0, [r4, #0x78]
007e1b84: ldrh     r3, [r4]
007e1b88: mov      r1, #0
007e1b8c: tst      r3, #0x40
007e1b90: ldreq    r6, [r5, #0xc]
007e1b94: ldrne    r6, [r4, #0x7c]
007e1b98: streq    r6, [r4, #0x7c]
007e1b9c: mov      r0, r6
007e1ba0: bl       #0x30e2f8
007e1ba4: cmp      r0, #0
007e1ba8: bne      #0x7e1d10
007e1bac: ldr      r3, [r5, #4]
007e1bb0: ldr      r1, [r4, #0xc]
007e1bb4: str      r3, [r4, #0x1c]
007e1bb8: ldr      r3, [r5, #8]
007e1bbc: ldr      r6, [r4, #0x1c]
007e1bc0: str      r3, [r4, #0x20]
007e1bc4: mov      r0, r6
007e1bc8: bl       #0x30ed6c
007e1bcc: ldr      r5, [r4, #0x20]
007e1bd0: mov      r7, r0
007e1bd4: ldr      r1, [r4, #0x14]
007e1bd8: mov      r0, r5
007e1bdc: bl       #0x30ed6c
007e1be0: mov      r1, r0
007e1be4: mov      r0, r7
007e1be8: bl       #0x30eba4
007e1bec: ldr      r1, [r4, #0x10]
007e1bf0: mov      r7, r0
007e1bf4: mov      r0, r6
007e1bf8: bl       #0x30ed6c
007e1bfc: ldr      r1, [r4, #0x18]
007e1c00: mov      r6, r0
007e1c04: mov      r0, r5
007e1c08: bl       #0x30ed6c
007e1c0c: mov      r1, r0
007e1c10: mov      r0, r6
007e1c14: bl       #0x30eba4
007e1c18: ldr      r1, [r4, #4]
007e1c1c: mov      r6, r0
007e1c20: mov      r0, r7
007e1c24: bl       #0x30eba4
007e1c28: ldr      r1, [r4, #8]
007e1c2c: mov      r5, r0
007e1c30: mov      r0, r6
007e1c34: bl       #0x30eba4
007e1c38: str      r5, [r4, #0x2c]
007e1c3c: str      r0, [r4, #0x30]
007e1c40: ldr      r5, [r4, #0x64]
007e1c44: ldr      r2, [r4, #0x2c]
007e1c48: ldr      r3, [r4, #0x30]
007e1c4c: cmp      r5, #0
007e1c50: str      r2, [r4, #0x24]
007e1c54: str      r3, [r4, #0x28]
007e1c58: beq      #0x7e1c80
007e1c5c: add      r6, r4, #0x1c
007e1c60: ldr      r3, [r5]
007e1c64: mov      r0, r5
007e1c68: mov      r1, r6
007e1c6c: mov      lr, pc
007e1c70: ldr      pc, [r3, #0x1c]
007e1c74: ldr      r5, [r5, #8]
007e1c78: cmp      r5, #0
007e1c7c: bne      #0x7e1c60
007e1c80: ldr      r0, [r4, #0x78]
007e1c84: mov      r1, #0
007e1c88: bl       #0x30df8c
007e1c8c: cmp      r0, #0
007e1c90: ldrh     r5, [r4, #2]
007e1c94: bne      #0x7e1cec
007e1c98: mov      r3, #1
007e1c9c: strh     r3, [r4, #2]
007e1ca0: mov      r3, #1
007e1ca4: sxth     r5, r5
007e1ca8: cmp      r5, r3
007e1cac: beq      #0x7e1ce8
007e1cb0: ldr      r5, [r4, #0x64]
007e1cb4: cmp      r5, #0
007e1cb8: beq      #0x7e1ce8
007e1cbc: mov      r6, #0x19000
007e1cc0: add      r6, r6, #0x1d8
007e1cc4: add      r7, r4, #4
007e1cc8: ldr      r3, [r4, #0x58]
007e1ccc: mov      r0, r5
007e1cd0: mov      r2, r7
007e1cd4: ldr      r1, [r3, r6]
007e1cd8: bl       #0x7e61b0
007e1cdc: ldr      r5, [r5, #8]
007e1ce0: cmp      r5, #0
007e1ce4: bne      #0x7e1cc8
007e1ce8: pop      {r4, r5, r6, r7, r8, pc}
007e1cec: ldr      r0, [r4, #0x80]
007e1cf0: mov      r1, #0
007e1cf4: bl       #0x30df8c
007e1cf8: cmp      r0, #0
007e1cfc: movne    r3, #0
007e1d00: strhne   r3, [r4, #2]
007e1d04: movne    r3, #0
007e1d08: bne      #0x7e1ca4
007e1d0c: b        #0x7e1c98
007e1d10: mov      r1, r6
007e1d14: mov      r0, #0x3f800000
007e1d18: bl       #0x30ec94
007e1d1c: str      r0, [r4, #0x80]
007e1d20: b        #0x7e1bac

# _ZN14PhysicalObject3pinEv
0046eb20: str      lr, [sp, #-4]!
0046eb24: ldrb     r3, [r0, #0x27]
0046eb28: sub      sp, sp, #0x14
0046eb2c: cmp      r3, #0
0046eb30: bne      #0x46eb68
0046eb34: ldr      r2, [r0, #0x14]
0046eb38: mov      r3, #1
0046eb3c: strb     r3, [r0, #0x27]
0046eb40: ldr      r1, [r2, #0x1c]
0046eb44: mov      r0, r2
0046eb48: mov      r3, #0
0046eb4c: str      r1, [sp, #4]
0046eb50: ldr      r2, [r2, #0x20]
0046eb54: mov      r1, sp
0046eb58: str      r3, [sp, #0xc]
0046eb5c: str      r2, [sp, #8]
0046eb60: str      r3, [sp]
0046eb64: bl       #0x7e1b28
0046eb68: add      sp, sp, #0x14
0046eb6c: ldm      sp!, {pc}

# _ZN6b2Body17SetMassFromShapesEv
007e1818: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007e181c: ldr      r2, [r0, #0x58]
007e1820: mov      r3, #0x19000
007e1824: add      r3, r3, #0x1d4
007e1828: ldrb     r2, [r2, r3]
007e182c: ldr      r3, [pc, #0x2ec]
007e1830: sub      sp, sp, #0x10
007e1834: cmp      r2, #0
007e1838: mov      r4, r0
007e183c: add      r3, pc, r3
007e1840: bne      #0x7e1aa8
007e1844: ldr      r1, [pc, #0x2d8]
007e1848: ldr      r5, [r0, #0x64]
007e184c: mov      r2, #0
007e1850: ldr      r3, [r3, r1]
007e1854: str      r2, [r0, #0x80]
007e1858: str      r2, [r0, #0x74]
007e185c: str      r2, [r0, #0x78]
007e1860: str      r2, [r0, #0x7c]
007e1864: cmp      r5, #0
007e1868: ldr      r8, [r3]
007e186c: ldr      r7, [r3, #4]
007e1870: mov      r6, r5
007e1874: beq      #0x7e1948
007e1878: mov      sb, sp
007e187c: ldr      r3, [r5]
007e1880: mov      r0, r5
007e1884: mov      r1, sp
007e1888: mov      lr, pc
007e188c: ldr      pc, [r3, #0x10]
007e1890: ldr      r1, [sp]
007e1894: ldr      r0, [r4, #0x74]
007e1898: bl       #0x30eba4
007e189c: ldr      r6, [sp]
007e18a0: str      r0, [r4, #0x74]
007e18a4: ldr      r1, [sp, #4]
007e18a8: mov      sl, r0
007e18ac: mov      r0, r6
007e18b0: bl       #0x30ed6c
007e18b4: mov      r1, r0
007e18b8: mov      r0, r8
007e18bc: bl       #0x30eba4
007e18c0: ldr      r1, [sp, #8]
007e18c4: mov      r8, r0
007e18c8: mov      r0, r6
007e18cc: bl       #0x30ed6c
007e18d0: mov      r1, r0
007e18d4: mov      r0, r7
007e18d8: bl       #0x30eba4
007e18dc: ldr      r1, [sp, #0xc]
007e18e0: mov      r7, r0
007e18e4: ldr      r0, [r4, #0x7c]
007e18e8: bl       #0x30eba4
007e18ec: str      r0, [r4, #0x7c]
007e18f0: ldr      r5, [r5, #8]
007e18f4: cmp      r5, #0
007e18f8: bne      #0x7e187c
007e18fc: mov      r0, sl
007e1900: mov      r1, #0
007e1904: bl       #0x30e2f8
007e1908: cmp      r0, #0
007e190c: beq      #0x7e1944
007e1910: mov      r1, sl
007e1914: mov      r0, #0x3f800000
007e1918: bl       #0x30ec94
007e191c: mov      r5, r0
007e1920: str      r0, [r4, #0x78]
007e1924: mov      r1, r5
007e1928: mov      r0, r8
007e192c: bl       #0x30ed6c
007e1930: mov      r1, r5
007e1934: mov      r8, r0
007e1938: mov      r0, r7
007e193c: bl       #0x30ed6c
007e1940: mov      r7, r0
007e1944: ldr      r6, [r4, #0x64]
007e1948: ldr      r5, [r4, #0x7c]
007e194c: mov      r1, #0
007e1950: mov      r0, r5
007e1954: bl       #0x30e2f8
007e1958: cmp      r0, #0
007e195c: bne      #0x7e1ab0
007e1960: mov      r3, #0
007e1964: str      r3, [r4, #0x80]
007e1968: str      r3, [r4, #0x7c]
007e196c: str      r7, [r4, #0x20]
007e1970: str      r8, [r4, #0x1c]
007e1974: ldr      r1, [r4, #0xc]
007e1978: mov      r0, r8
007e197c: bl       #0x30ed6c
007e1980: ldr      r1, [r4, #0x14]
007e1984: mov      r5, r0
007e1988: mov      r0, r7
007e198c: bl       #0x30ed6c
007e1990: mov      r1, r0
007e1994: mov      r0, r5
007e1998: bl       #0x30eba4
007e199c: ldr      r1, [r4, #0x10]
007e19a0: mov      r5, r0
007e19a4: mov      r0, r8
007e19a8: bl       #0x30ed6c
007e19ac: ldr      r1, [r4, #0x18]
007e19b0: mov      r8, r0
007e19b4: mov      r0, r7
007e19b8: bl       #0x30ed6c
007e19bc: mov      r1, r0
007e19c0: mov      r0, r8
007e19c4: bl       #0x30eba4
007e19c8: ldr      r1, [r4, #4]
007e19cc: mov      r7, r0
007e19d0: mov      r0, r5
007e19d4: bl       #0x30eba4
007e19d8: ldr      r1, [r4, #8]
007e19dc: mov      r5, r0
007e19e0: mov      r0, r7
007e19e4: bl       #0x30eba4
007e19e8: str      r5, [r4, #0x2c]
007e19ec: str      r0, [r4, #0x30]
007e19f0: ldr      r2, [r4, #0x2c]
007e19f4: ldr      r3, [r4, #0x30]
007e19f8: subs     r5, r6, #0
007e19fc: str      r2, [r4, #0x24]
007e1a00: str      r3, [r4, #0x28]
007e1a04: beq      #0x7e1a2c
007e1a08: add      r6, r4, #0x1c
007e1a0c: ldr      r3, [r5]
007e1a10: mov      r0, r5
007e1a14: mov      r1, r6
007e1a18: mov      lr, pc
007e1a1c: ldr      pc, [r3, #0x1c]
007e1a20: ldr      r5, [r5, #8]
007e1a24: cmp      r5, #0
007e1a28: bne      #0x7e1a0c
007e1a2c: ldr      r0, [r4, #0x78]
007e1a30: mov      r1, #0
007e1a34: bl       #0x30df8c
007e1a38: cmp      r0, #0
007e1a3c: ldrh     r5, [r4, #2]
007e1a40: beq      #0x7e1b10
007e1a44: ldr      r0, [r4, #0x80]
007e1a48: mov      r1, #0
007e1a4c: bl       #0x30df8c
007e1a50: cmp      r0, #0
007e1a54: movne    r3, #0
007e1a58: strhne   r3, [r4, #2]
007e1a5c: movne    r3, #0
007e1a60: beq      #0x7e1b10
007e1a64: sxth     r5, r5
007e1a68: cmp      r5, r3
007e1a6c: beq      #0x7e1aa8
007e1a70: ldr      r5, [r4, #0x64]
007e1a74: cmp      r5, #0
007e1a78: beq      #0x7e1aa8
007e1a7c: mov      r6, #0x19000
007e1a80: add      r6, r6, #0x1d8
007e1a84: add      r7, r4, #4
007e1a88: ldr      r3, [r4, #0x58]
007e1a8c: mov      r0, r5
007e1a90: mov      r2, r7
007e1a94: ldr      r1, [r3, r6]
007e1a98: bl       #0x7e61b0
007e1a9c: ldr      r5, [r5, #8]
007e1aa0: cmp      r5, #0
007e1aa4: bne      #0x7e1a88
007e1aa8: add      sp, sp, #0x10
007e1aac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007e1ab0: ldrh     r3, [r4]
007e1ab4: tst      r3, #0x40
007e1ab8: bne      #0x7e1960
007e1abc: mov      r1, r8
007e1ac0: mov      r0, r8
007e1ac4: bl       #0x30ed6c
007e1ac8: mov      r1, r7
007e1acc: mov      sl, r0
007e1ad0: mov      r0, r7
007e1ad4: bl       #0x30ed6c
007e1ad8: mov      r1, r0
007e1adc: mov      r0, sl
007e1ae0: bl       #0x30eba4
007e1ae4: ldr      r1, [r4, #0x74]
007e1ae8: bl       #0x30ed6c
007e1aec: mov      r1, r0
007e1af0: mov      r0, r5
007e1af4: bl       #0x30e3ac
007e1af8: mov      r1, r0
007e1afc: str      r0, [r4, #0x7c]
007e1b00: mov      r0, #0x3f800000
007e1b04: bl       #0x30ec94
007e1b08: str      r0, [r4, #0x80]
007e1b0c: b        #0x7e196c
007e1b10: mov      r3, #1
007e1b14: strh     r3, [r4, #2]
007e1b18: mov      r3, #1
007e1b1c: b        #0x7e1a64
007e1b20: andseq   r3, fp, r4, asr r2
007e1b24: andeq    r0, r0, r0, asr #18
