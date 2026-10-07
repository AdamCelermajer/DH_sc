
# _ZN10PlayerInfo17SetCharacterLevelEi
00370e48: push     {r4, r5, r6, r7, lr}
00370e4c: ldr      r4, [pc, #0x90]
00370e50: ldr      r3, [pc, #0x90]
00370e54: sub      sp, sp, #0x2c
00370e58: add      r4, pc, r4
00370e5c: ldr      r2, [sp, #0x20]
00370e60: ldr      r3, [r4, r3]
00370e64: mvn      ip, #0
00370e68: cmp      r1, r2
00370e6c: mov      r6, #0
00370e70: mov      r2, #0
00370e74: add      r3, r3, #8
00370e78: mov      lr, #0x10
00370e7c: mov      r7, #0
00370e80: strd     r6, r7, [sp, #8]
00370e84: str      lr, [sp, #4]
00370e88: str      ip, [sp, #0x14]
00370e8c: strb     r2, [sp, #0x1c]
00370e90: str      r3, [sp]
00370e94: mov      r5, r0
00370e98: str      ip, [sp, #0x10]
00370e9c: str      r2, [sp, #0x18]
00370ea0: moveq    r6, sp
00370ea4: beq      #0x370eb8
00370ea8: mov      r0, sp
00370eac: mov      r6, sp
00370eb0: str      r1, [sp, #0x20]
00370eb4: bl       #0x814f84
00370eb8: ldr      r2, [pc, #0x2c]
00370ebc: ldr      r3, [r5, #0x310]
00370ec0: add      r0, r5, #0x310
00370ec4: ldr      r2, [r4, r2]
00370ec8: add      r1, r6, #0x20
00370ecc: add      r2, r2, #8
00370ed0: str      r2, [sp]
00370ed4: mov      lr, pc
00370ed8: ldr      pc, [r3, #0x1c]
00370edc: add      sp, sp, #0x2c
00370ee0: pop      {r4, r5, r6, r7, pc}
00370ee4: rsbeq    r3, r2, r8, lsr ip
00370ee8: andeq    r2, r0, r4, lsl #19
00370eec: andeq    r3, r0, ip, lsr r5

# _ZN13PlayerManager21GetPlayerByInternalIDEib
0036dfb0: cmn      r1, #1
0036dfb4: push     {r4, r5, r6, lr}
0036dfb8: mov      r4, r1
0036dfbc: mov      r5, r0
0036dfc0: mov      r6, r2
0036dfc4: beq      #0x36e040
0036dfc8: bl       #0x7fd794
0036dfcc: ldrb     r3, [r0, #5]
0036dfd0: cmp      r3, #0
0036dfd4: bne      #0x36e048
0036dfd8: ldr      r0, [r5, #0x694]
0036dfdc: add      r1, r5, #0x690
0036dfe0: cmp      r0, #0
0036dfe4: movne    r2, r1
0036dfe8: bne      #0x36dff4
0036dfec: b        #0x36e038
0036dff0: mov      r0, r3
0036dff4: ldr      r3, [r0, #0x10]
0036dff8: cmp      r4, r3
0036dffc: ldrgt    r3, [r0, #0xc]
0036e000: ldrle    r3, [r0, #8]
0036e004: movgt    r0, r2
0036e008: mov      r2, r0
0036e00c: cmp      r3, #0
0036e010: bne      #0x36dff0
0036e014: cmp      r1, r0
0036e018: beq      #0x36e094
0036e01c: ldr      r3, [r0, #0x10]
0036e020: cmp      r4, r3
0036e024: blt      #0x36e038
0036e028: cmp      r1, r0
0036e02c: beq      #0x36e094
0036e030: add      r0, r0, #0x18
0036e034: pop      {r4, r5, r6, pc}
0036e038: mov      r0, r1
0036e03c: b        #0x36e028
0036e040: add      r0, r0, #8
0036e044: pop      {r4, r5, r6, pc}
0036e048: bl       #0x320e98
0036e04c: ldrb     r3, [r0, #0x24]
0036e050: cmp      r3, #0
0036e054: beq      #0x36dfd8
0036e058: bl       #0x800f8c
0036e05c: ldr      r3, [r0]
0036e060: mov      lr, pc
0036e064: ldr      pc, [r3, #0x64]
0036e068: cmp      r0, #0
0036e06c: beq      #0x36dfd8
0036e070: bl       #0x8100dc
0036e074: bl       #0x8100e0
0036e078: cmp      r0, #0
0036e07c: beq      #0x36dfd8
0036e080: mov      r0, r5
0036e084: mov      r1, r4
0036e088: mov      r2, r6
0036e08c: pop      {r4, r5, r6, lr}
0036e090: b        #0x36dec4
0036e094: add      r0, r5, #8
0036e098: pop      {r4, r5, r6, pc}

# _ZN13PlayerManager17_ManageCharactersEv
0037280c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00372810: ldr      r8, [pc, #0xeac]
00372814: ldr      r1, [pc, #0xeac]
00372818: ldr      r2, [pc, #0xeac]
0037281c: add      r8, pc, r8
00372820: ldr      r3, [r8, r1]
00372824: sub      sp, sp, #0x1d4
00372828: mov      r7, r0
0037282c: ldr      r3, [r3]
00372830: ldr      r0, [r8, r2]
00372834: str      r1, [sp, #0x1c]
00372838: str      r2, [sp, #0x18]
0037283c: str      r3, [sp, #0x1cc]
00372840: bl       #0x31f594
00372844: str      r0, [sp, #0x24]
00372848: mov      r0, r7
0037284c: bl       #0x36d7a8
00372850: subs     fp, r0, #0
00372854: ble      #0x373a94
00372858: ldr      r3, [pc, #0xe70]
0037285c: ldr      lr, [pc, #0xe70]
00372860: mov      r0, #0
00372864: add      r3, pc, r3
00372868: str      r3, [sp, #0x30]
0037286c: ldr      r3, [pc, #0xe64]
00372870: mov      r1, #1
00372874: str      lr, [sp, #0x2c]
00372878: add      r3, pc, r3
0037287c: str      r3, [sp, #0x34]
00372880: ldr      r3, [pc, #0xe54]
00372884: str      r0, [sp, #0x20]
00372888: mov      r6, r0
0037288c: add      r3, pc, r3
00372890: str      r3, [sp, #0x38]
00372894: ldr      r3, [pc, #0xe44]
00372898: str      r0, [sp, #0xc]
0037289c: str      r1, [sp, #0x10]
003728a0: add      r3, pc, r3
003728a4: str      r3, [sp, #0x3c]
003728a8: mov      r1, r6
003728ac: mov      r2, #0
003728b0: mov      r0, r7
003728b4: bl       #0x36e744
003728b8: ldr      r3, [r0]
003728bc: mov      r4, r0
003728c0: ldr      r5, [r0, #0x660]
003728c4: mov      lr, pc
003728c8: ldr      pc, [r3, #0x5c]
003728cc: cmp      r0, #0
003728d0: beq      #0x372ec0
003728d4: ldrb     r3, [r4, #0x66c]
003728d8: cmp      r3, #0
003728dc: beq      #0x372f50
003728e0: ldr      sl, [r4, #0x664]
003728e4: cmn      sl, #1
003728e8: beq      #0x372b6c
003728ec: ldr      sb, [r4, #0x680]
003728f0: cmp      sb, #0
003728f4: beq      #0x3739d8
003728f8: bl       #0x32bd08
003728fc: ldrb     r3, [r0, #0x11]
00372900: cmp      r3, #0
00372904: beq      #0x372f8c
00372908: add      r2, sp, #0x1b4
0037290c: mov      r0, r2
00372910: add      r1, r4, #0x2d0
00372914: str      r2, [sp, #0x14]
00372918: bl       #0x32b918
0037291c: bl       #0x32bd08
00372920: add      sl, sp, #0x19c
00372924: mov      r1, r0
00372928: mov      r0, sl
0037292c: bl       #0x4995f4
00372930: ldr      r0, [sp, #0x1c8]
00372934: ldr      r1, [sp, #0x1b0]
00372938: ldr      r2, [sp, #0x1c4]
0037293c: ldr      r3, [sp, #0x1ac]
00372940: rsb      r2, r0, r2
00372944: rsb      r3, r1, r3
00372948: cmp      r2, r3
0037294c: beq      #0x373618
00372950: mov      r0, sl
00372954: bl       #0x318254
00372958: ldr      r0, [sp, #0x14]
0037295c: bl       #0x318254
00372960: bl       #0x32bd08
00372964: add      sl, sp, #0x184
00372968: mov      r1, r0
0037296c: mov      r0, sl
00372970: bl       #0x4995f4
00372974: mov      r0, r4
00372978: mov      r1, sl
0037297c: bl       #0x371ccc
00372980: mov      r0, sl
00372984: bl       #0x318254
00372988: cmp      r5, #0
0037298c: beq      #0x373028
00372990: ldr      ip, [sp, #0x18]
00372994: ldr      r0, [r8, ip]
00372998: bl       #0x31f594
0037299c: subs     sl, r0, #0
003729a0: beq      #0x3729b4
003729a4: ldr      r3, [sl, #0x130]
003729a8: cmp      r3, #0x26
003729ac: ldrbeq   r3, [sl, #0x144]
003729b0: beq      #0x3729b8
003729b4: mov      r3, #0
003729b8: ldrb     r2, [r4, #0x4e5]
003729bc: cmp      r2, r3
003729c0: beq      #0x372a78
003729c4: ldr      ip, [pc, #0xd18]
003729c8: ldrb     r1, [sp, #0xa5]
003729cc: mov      r2, #0xb0000000
003729d0: ldr      ip, [r8, ip]
003729d4: cmp      r1, r3
003729d8: asr      r2, r2, #0x16
003729dc: mov      r0, #0
003729e0: mov      r1, #0
003729e4: add      lr, sp, #0x1d0
003729e8: mvn      sb, #0
003729ec: strd     r0, r1, [lr, r2]
003729f0: add      ip, ip, #8
003729f4: mov      r2, #1
003729f8: mov      r0, #0
003729fc: mov      r1, #0
00372a00: str      sb, [sp, #0x9c]
00372a04: str      sb, [sp, #0x98]
00372a08: str      r2, [sp, #0x8c]
00372a0c: strb     r0, [sp, #0xa4]
00372a10: str      ip, [sp, #0x88]
00372a14: str      r1, [sp, #0xa0]
00372a18: addeq    sb, sp, #0x88
00372a1c: beq      #0x372a30
00372a20: add      sb, sp, #0x88
00372a24: mov      r0, sb
00372a28: strb     r3, [sp, #0xa5]
00372a2c: bl       #0x814f84
00372a30: ldr      r3, [pc, #0xcb0]
00372a34: add      r0, r4, #0x4c0
00372a38: add      r1, sb, #0x1d
00372a3c: ldr      r3, [r8, r3]
00372a40: add      r0, r0, #8
00372a44: add      r3, r3, #8
00372a48: str      r3, [sp, #0x88]
00372a4c: ldr      r3, [r4, #0x4c8]
00372a50: mov      lr, pc
00372a54: ldr      pc, [r3, #0x1c]
00372a58: ldr      r3, [pc, #0xc8c]
00372a5c: ldr      r2, [pc, #0xc8c]
00372a60: mov      r1, #0x7d0
00372a64: ldr      r3, [r8, r3]
00372a68: add      r2, pc, r2
00372a6c: str      r1, [r2]
00372a70: add      r3, r3, #8
00372a74: str      r3, [sp, #0x88]
00372a78: cmp      sl, #0
00372a7c: beq      #0x372a90
00372a80: ldr      sl, [sl, #0x130]
00372a84: cmp      sl, #0x23
00372a88: movle    sl, #0
00372a8c: movgt    sl, #1
00372a90: ldrb     r3, [r4, #0x525]
00372a94: cmp      r3, sl
00372a98: beq      #0x372b4c
00372a9c: bl       #0x7fd794
00372aa0: ldrb     r3, [r0, #5]
00372aa4: cmp      r3, #0
00372aa8: bne      #0x3735e4
00372aac: ldr      r0, [pc, #0xc30]
00372ab0: ldrb     r2, [sp, #0x85]
00372ab4: mov      r3, #0xa8000000
00372ab8: ldr      r0, [r8, r0]
00372abc: asr      r3, r3, #0x16
00372ac0: mov      r1, #0
00372ac4: add      sb, r0, #8
00372ac8: add      ip, sp, #0x1d0
00372acc: mov      r0, #0
00372ad0: cmp      r2, sl
00372ad4: mvn      lr, #0
00372ad8: mov      r2, #0
00372adc: strd     r0, r1, [ip, r3]
00372ae0: mov      r3, #1
00372ae4: str      sb, [sp, #0x68]
00372ae8: str      r3, [sp, #0x6c]
00372aec: str      lr, [sp, #0x7c]
00372af0: strb     r2, [sp, #0x84]
00372af4: str      lr, [sp, #0x78]
00372af8: str      r2, [sp, #0x80]
00372afc: addeq    sb, sp, #0x68
00372b00: beq      #0x372b14
00372b04: add      sb, sp, #0x68
00372b08: mov      r0, sb
00372b0c: strb     sl, [sp, #0x85]
00372b10: bl       #0x814f84
00372b14: ldr      r3, [pc, #0xbcc]
00372b18: add      r0, r4, #0x500
00372b1c: add      r0, r0, #8
00372b20: ldr      r3, [r8, r3]
00372b24: add      r1, sb, #0x1d
00372b28: add      r3, r3, #8
00372b2c: str      r3, [sp, #0x68]
00372b30: ldr      r3, [r4, #0x508]
00372b34: mov      lr, pc
00372b38: ldr      pc, [r3, #0x1c]
00372b3c: ldr      r3, [pc, #0xba8]
00372b40: ldr      r3, [r8, r3]
00372b44: add      r3, r3, #8
00372b48: str      r3, [sp, #0x68]
00372b4c: bl       #0x7fd794
00372b50: ldrb     r3, [r0, #5]
00372b54: cmp      r3, #0
00372b58: bne      #0x3730b4
00372b5c: bl       #0x7fd794
00372b60: ldrb     r3, [r0, #5]
00372b64: cmp      r3, #0
00372b68: bne      #0x373120
00372b6c: ldrb     r3, [r7, #0x6c9]
00372b70: cmp      r3, #0
00372b74: beq      #0x372ea0
00372b78: cmp      r5, #0
00372b7c: beq      #0x37305c
00372b80: bl       #0x7fd794
00372b84: ldrb     r3, [r0, #5]
00372b88: cmp      r3, #0
00372b8c: beq      #0x372ea0
00372b90: ldrb     r3, [r4, #0x66c]
00372b94: cmp      r3, #0
00372b98: beq      #0x3731cc
00372b9c: mov      r1, r4
00372ba0: mov      r0, r7
00372ba4: bl       #0x370020
00372ba8: ldr      r1, [r5, #0x39c]
00372bac: ldr      r3, [r4, #0x498]
00372bb0: cmp      r3, r1
00372bb4: beq      #0x372bc0
00372bb8: mov      r0, r4
00372bbc: bl       #0x36fe04
00372bc0: add      sl, r5, #0x37c
00372bc4: mov      r0, sl
00372bc8: ldr      sb, [r4, #0x358]
00372bcc: bl       #0x3fc690
00372bd0: cmp      r0, sb
00372bd4: beq      #0x372bec
00372bd8: mov      r0, sl
00372bdc: bl       #0x3fc690
00372be0: mov      r1, r0
00372be4: mov      r0, r4
00372be8: bl       #0x36fcb0
00372bec: add      sb, r5, #0x560
00372bf0: mov      r0, sb
00372bf4: mov      r1, #0x21
00372bf8: mov      r2, #0
00372bfc: ldr      sl, [r4, #0x448]
00372c00: bl       #0x3df6e0
00372c04: cmp      r0, sl
00372c08: beq      #0x372c28
00372c0c: mov      r1, #0x21
00372c10: mov      r0, sb
00372c14: mov      r2, #0
00372c18: bl       #0x3df6e0
00372c1c: mov      r1, r0
00372c20: mov      r0, r4
00372c24: bl       #0x36fd58
00372c28: mov      r0, sb
00372c2c: mov      r1, #0x24
00372c30: mov      r2, #0
00372c34: ldr      sl, [r4, #0x470]
00372c38: bl       #0x3df6e0
00372c3c: cmp      r0, sl
00372c40: beq      #0x372cf8
00372c44: mov      r1, #0x24
00372c48: mov      r2, #0
00372c4c: mov      r0, sb
00372c50: bl       #0x3df6e0
00372c54: ldr      ip, [pc, #0xa98]
00372c58: ldr      r1, [sp, #0x60]
00372c5c: mov      r2, #0x9e000000
00372c60: ldr      ip, [r8, ip]
00372c64: cmp      r0, r1
00372c68: asr      r2, r2, #0x16
00372c6c: mov      r1, #0
00372c70: mov      r3, r0
00372c74: add      lr, sp, #0x1d0
00372c78: mov      r0, #0
00372c7c: mvn      sl, #0
00372c80: strd     r0, r1, [lr, r2]
00372c84: add      ip, ip, #8
00372c88: mov      r2, #0x20
00372c8c: mov      r0, #0
00372c90: mov      r1, #0
00372c94: str      sl, [sp, #0x54]
00372c98: str      sl, [sp, #0x50]
00372c9c: str      r2, [sp, #0x44]
00372ca0: strb     r0, [sp, #0x5c]
00372ca4: str      ip, [sp, #0x40]
00372ca8: str      r1, [sp, #0x58]
00372cac: addeq    sl, sp, #0x40
00372cb0: beq      #0x372cc4
00372cb4: add      sl, sp, #0x40
00372cb8: mov      r0, sl
00372cbc: str      r3, [sp, #0x60]
00372cc0: bl       #0x814f84
00372cc4: ldr      r3, [pc, #0xa2c]
00372cc8: add      r1, sl, #0x20
00372ccc: add      r0, r4, #0x450
00372cd0: ldr      r3, [r8, r3]
00372cd4: add      r3, r3, #8
00372cd8: str      r3, [sp, #0x40]
00372cdc: ldr      r3, [r4, #0x450]
00372ce0: mov      lr, pc
00372ce4: ldr      pc, [r3, #0x1c]
00372ce8: ldr      r3, [pc, #0x9fc]
00372cec: ldr      r3, [r8, r3]
00372cf0: add      r3, r3, #8
00372cf4: str      r3, [sp, #0x40]
00372cf8: mov      r0, sb
00372cfc: mov      r1, #0x13
00372d00: mov      r2, #0
00372d04: ldr      sl, [r4, #0x330]
00372d08: bl       #0x3df6e0
00372d0c: cmp      r0, sl
00372d10: beq      #0x372d30
00372d14: mov      r1, #0x13
00372d18: mov      r0, sb
00372d1c: mov      r2, #0
00372d20: bl       #0x3df6e0
00372d24: mov      r1, r0
00372d28: mov      r0, r4
00372d2c: bl       #0x370e48
00372d30: mov      r0, r5
00372d34: ldr      sl, [r4, #0x380]
00372d38: bl       #0x3bb7fc
00372d3c: cmp      r0, sl
00372d40: beq      #0x372d58
00372d44: mov      r0, r5
00372d48: bl       #0x3bb7fc
00372d4c: mov      r1, r0
00372d50: mov      r0, r4
00372d54: bl       #0x370ef0
00372d58: mov      r1, #0
00372d5c: mov      r0, r5
00372d60: bl       #0x3bbe68
00372d64: mov      r1, #1
00372d68: strb     r0, [sp, #0xc4]
00372d6c: mov      r0, r5
00372d70: bl       #0x3bbe68
00372d74: mov      r1, #2
00372d78: strb     r0, [sp, #0xc5]
00372d7c: mov      r0, r5
00372d80: bl       #0x3bbe68
00372d84: movw     sl, #0x14e8
00372d88: strb     r0, [sp, #0xc6]
00372d8c: add      r1, sp, #0xc4
00372d90: add      r0, r4, #0x3d8
00372d94: mov      r2, #3
00372d98: bl       #0x370258
00372d9c: ldr      r3, [r5, sl]
00372da0: cmp      r3, #0
00372da4: beq      #0x372dd8
00372da8: ldr      r2, [r3, #0x84]
00372dac: cmp      r2, #0x1e
00372db0: bls      #0x372dd8
00372db4: ldr      r2, [pc, #0x964]
00372db8: ldr      r2, [r8, r2]
00372dbc: ldr      r2, [r2]
00372dc0: cmp      r2, #2
00372dc4: moveq    r2, #0
00372dc8: streq    r2, [r2]
00372dcc: beq      #0x372dd8
00372dd0: cmp      r2, #1
00372dd4: beq      #0x373ad0
00372dd8: add      r2, sp, #0xd4
00372ddc: str      r4, [sp, #0x28]
00372de0: mov      sl, #0
00372de4: str      r2, [sp, #0x14]
00372de8: movw     sb, #0x14e8
00372dec: mov      r4, r2
00372df0: b        #0x372e04
00372df4: add      sl, sl, #1
00372df8: cmp      sl, #0x1e
00372dfc: beq      #0x372e50
00372e00: ldr      r3, [r5, sb]
00372e04: cmp      r3, #0
00372e08: beq      #0x372df4
00372e0c: ldr      r3, [r3, #0x84]
00372e10: cmp      r3, sl
00372e14: bls      #0x372df4
00372e18: mov      r0, r5
00372e1c: mov      r1, sl
00372e20: bl       #0x3bbed0
00372e24: cmp      r0, #0
00372e28: mvnlt    r3, #0
00372e2c: strblt   r3, [r4, sl]
00372e30: blt      #0x372df4
00372e34: mov      r1, sl
00372e38: mov      r0, r5
00372e3c: bl       #0x3bbed0
00372e40: strb     r0, [r4, sl]
00372e44: add      sl, sl, #1
00372e48: cmp      sl, #0x1e
00372e4c: bne      #0x372e00
00372e50: ldr      r4, [sp, #0x28]
00372e54: mov      r2, sl
00372e58: ldr      r1, [sp, #0x14]
00372e5c: add      r0, r4, #0x400
00372e60: bl       #0x3702a8
00372e64: add      sl, r4, #0x4c0
00372e68: add      sl, sl, #8
00372e6c: mov      r0, sl
00372e70: bl       #0x814f70
00372e74: cmp      r0, #0
00372e78: beq      #0x372ea0
00372e7c: ldr      r3, [r5]
00372e80: mov      r0, r5
00372e84: ldrb     r1, [r4, #0x4e5]
00372e88: mov      lr, pc
00372e8c: ldr      pc, [r3, #0x40]
00372e90: ldr      r2, [sp, #0x24]
00372e94: ldr      r3, [r2, #0x130]
00372e98: cmp      r3, #0x26
00372e9c: beq      #0x3738ec
00372ea0: ldrb     r3, [r4, #0x4e5]
00372ea4: ldr      r1, [sp, #0x10]
00372ea8: ldr      r2, [sp, #0xc]
00372eac: cmp      r3, #0
00372eb0: moveq    r1, r3
00372eb4: movne    r2, #1
00372eb8: str      r1, [sp, #0x10]
00372ebc: str      r2, [sp, #0xc]
00372ec0: add      r6, r6, #1
00372ec4: cmp      r6, fp
00372ec8: bne      #0x3728a8
00372ecc: bl       #0x7fd794
00372ed0: ldrb     r3, [r0, #5]
00372ed4: cmp      r3, #0
00372ed8: bne      #0x373734
00372edc: bl       #0x7fd794
00372ee0: ldrb     r3, [r0, #5]
00372ee4: cmp      r3, #0
00372ee8: bne      #0x3732a8
00372eec: mov      r4, #0
00372ef0: bl       #0x7fd794
00372ef4: ldrb     r3, [r0, #5]
00372ef8: cmp      r3, #0
00372efc: bne      #0x373638
00372f00: cmp      r4, #0
00372f04: bne      #0x373698
00372f08: ldrb     r3, [r7, #0x6c9]
00372f0c: cmp      r3, #0
00372f10: bne      #0x372f30
00372f14: ldr      r3, [r7, #0x6c4]
00372f18: cmp      r3, #0
00372f1c: ble      #0x372f30
00372f20: mov      r0, r7
00372f24: bl       #0x370e00
00372f28: mov      r0, r7
00372f2c: bl       #0x3721b8
00372f30: ldr      r0, [sp, #0x1c]
00372f34: ldr      r2, [sp, #0x1cc]
00372f38: ldr      r3, [r8, r0]
00372f3c: ldr      r3, [r3]
00372f40: cmp      r2, r3
00372f44: bne      #0x373b84
00372f48: add      sp, sp, #0x1d4
00372f4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00372f50: add      r0, r4, #0x500
00372f54: add      r0, r0, #8
00372f58: bl       #0x814f70
00372f5c: cmp      r0, #0
00372f60: beq      #0x372b6c
00372f64: ldrb     r3, [r4, #0x525]
00372f68: cmp      r3, #0
00372f6c: beq      #0x372b6c
00372f70: mov      r0, r7
00372f74: bl       #0x36f074
00372f78: ldr      lr, [sp, #0x20]
00372f7c: cmp      r0, #0
00372f80: movne    lr, #1
00372f84: str      lr, [sp, #0x20]
00372f88: b        #0x372b6c
00372f8c: bl       #0x320e98
00372f90: ldrb     r3, [r0, #0x28]
00372f94: cmp      r3, #0
00372f98: beq      #0x37317c
00372f9c: bl       #0x81a6ac
00372fa0: add      r3, sp, #0xd4
00372fa4: ldr      r1, [r0, #4]
00372fa8: add      r2, sp, #0xd0
00372fac: mov      r0, r3
00372fb0: add      sl, sp, #0x16c
00372fb4: str      r3, [sp, #0x14]
00372fb8: bl       #0x3140ec
00372fbc: add      r1, r4, #0x2d0
00372fc0: mov      r0, sl
00372fc4: bl       #0x32b918
00372fc8: ldr      r0, [sp, #0x180]
00372fcc: ldr      r1, [sp, #0xe8]
00372fd0: ldr      r2, [sp, #0x17c]
00372fd4: ldr      r3, [sp, #0xe4]
00372fd8: rsb      r2, r0, r2
00372fdc: rsb      r3, r1, r3
00372fe0: cmp      r2, r3
00372fe4: beq      #0x3738d4
00372fe8: mov      r0, sl
00372fec: add      sl, sp, #0x154
00372ff0: bl       #0x318254
00372ff4: add      r2, sp, #0xcc
00372ff8: ldr      r1, [sp, #0xe8]
00372ffc: mov      r0, sl
00373000: bl       #0x3140ec
00373004: mov      r0, r4
00373008: mov      r1, sl
0037300c: bl       #0x371ccc
00373010: mov      r0, sl
00373014: bl       #0x318254
00373018: ldr      r0, [sp, #0x14]
0037301c: bl       #0x318254
00373020: cmp      r5, #0
00373024: bne      #0x372990
00373028: ldr      r1, [sb, #0x34]
0037302c: ldr      r3, [r4, #0x380]
00373030: cmp      r3, r1
00373034: beq      #0x373040
00373038: mov      r0, r4
0037303c: bl       #0x370ef0
00373040: ldr      r1, [sb, #0x30]
00373044: ldr      r3, [r4, #0x330]
00373048: cmp      r3, r1
0037304c: beq      #0x372990
00373050: mov      r0, r4
00373054: bl       #0x370e48
00373058: b        #0x372990
0037305c: ldr      lr, [sp, #0x24]
00373060: cmp      lr, #0
00373064: beq      #0x372ec0
00373068: ldr      r3, [r4, #0x380]
0037306c: cmn      r3, #1
00373070: beq      #0x372ec0
00373074: bl       #0x7fd794
00373078: ldrb     r3, [r0, #5]
0037307c: cmp      r3, #0
00373080: bne      #0x373a64
00373084: ldr      r1, [r4, #0x670]
00373088: mov      r0, r7
0037308c: bl       #0x372220
00373090: ldrb     r3, [r4, #0x4e5]
00373094: ldr      r1, [sp, #0x10]
00373098: ldr      r2, [sp, #0xc]
0037309c: cmp      r3, #0
003730a0: moveq    r1, r3
003730a4: movne    r2, #1
003730a8: str      r1, [sp, #0x10]
003730ac: str      r2, [sp, #0xc]
003730b0: b        #0x372ec0
003730b4: bl       #0x320e98
003730b8: ldrb     r3, [r0, #0x24]
003730bc: cmp      r3, #0
003730c0: beq      #0x372b5c
003730c4: bl       #0x800f8c
003730c8: ldr      r3, [r0]
003730cc: mov      lr, pc
003730d0: ldr      pc, [r3, #0x64]
003730d4: cmp      r0, #0
003730d8: beq      #0x372b5c
003730dc: bl       #0x8100dc
003730e0: bl       #0x8100e0
003730e4: cmp      r0, #0
003730e8: beq      #0x372b5c
003730ec: ldrb     r3, [r4, #0x525]
003730f0: cmp      r3, #0
003730f4: bne      #0x372b5c
003730f8: mov      r0, r7
003730fc: bl       #0x36e09c
00373100: ldrb     r3, [r0, #0x4e5]
00373104: cmp      r3, #0
00373108: beq      #0x372b5c
0037310c: ldrb     r3, [r7, #0x71a]
00373110: cmp      r3, #0
00373114: moveq    r3, #1
00373118: strbeq   r3, [r7, #0x71a]
0037311c: b        #0x372b5c
00373120: bl       #0x320e98
00373124: ldrb     r3, [r0, #0x24]
00373128: cmp      r3, #0
0037312c: beq      #0x372b6c
00373130: bl       #0x800f8c
00373134: ldr      r3, [r0]
00373138: mov      lr, pc
0037313c: ldr      pc, [r3, #0x64]
00373140: cmp      r0, #0
00373144: beq      #0x372b6c
00373148: bl       #0x8100dc
0037314c: bl       #0x8100e0
00373150: cmp      r0, #0
00373154: beq      #0x372b6c
00373158: ldrb     r3, [r4, #0x525]
0037315c: cmp      r3, #0
00373160: beq      #0x372b6c
00373164: ldrb     r3, [r4, #0x545]
00373168: cmp      r3, #0
0037316c: bne      #0x372b6c
00373170: mov      r0, r4
00373174: bl       #0x3709b4
00373178: b        #0x372b6c
0037317c: add      sl, sp, #0x13c
00373180: add      r1, r4, #0x2d0
00373184: mov      r0, sl
00373188: bl       #0x32b918
0037318c: ldr      r1, [sb, #0x2c]
00373190: mov      r0, sl
00373194: bl       #0x313c48
00373198: mov      r3, r0
0037319c: mov      r0, sl
003731a0: str      r3, [sp, #8]
003731a4: bl       #0x318254
003731a8: ldr      r3, [sp, #8]
003731ac: cmp      r3, #0
003731b0: bne      #0x372988
003731b4: add      sl, sp, #0x124
003731b8: add      r2, sp, #0xc8
003731bc: mov      r0, sl
003731c0: ldr      r1, [sb, #0x2c]
003731c4: bl       #0x3140ec
003731c8: b        #0x372974
003731cc: mov      r1, r4
003731d0: mov      r0, r7
003731d4: bl       #0x36d850
003731d8: mov      r0, r5
003731dc: bl       #0x3bb7e8
003731e0: cmp      r0, #0
003731e4: addeq    r3, r4, #0x2d0
003731e8: beq      #0x373a00
003731ec: add      r3, r4, #0x2d0
003731f0: add      sl, sp, #0x10c
003731f4: mov      r1, r3
003731f8: mov      r0, sl
003731fc: str      r3, [sp, #8]
00373200: bl       #0x32b918
00373204: mov      r0, r5
00373208: bl       #0x3bb7e8
0037320c: mov      r1, r0
00373210: mov      r0, sl
00373214: bl       #0x313c48
00373218: mov      sb, r0
0037321c: eor      sb, sb, #1
00373220: mov      r0, sl
00373224: bl       #0x318254
00373228: tst      sb, #0xff
0037322c: ldr      r3, [sp, #8]
00373230: bne      #0x373a00
00373234: mov      r0, r5
00373238: ldr      sl, [r4, #0x380]
0037323c: bl       #0x3bb7fc
00373240: cmp      r0, sl
00373244: beq      #0x373308
00373248: ldr      ip, [sp, #0x18]
0037324c: ldr      sl, [r8, ip]
00373250: mov      r0, sl
00373254: bl       #0x31f594
00373258: cmp      r0, #0
0037325c: beq      #0x373308
00373260: mov      r0, sl
00373264: bl       #0x31f594
00373268: ldrb     r3, [r0, #0x144]
0037326c: cmp      r3, #0
00373270: beq      #0x373308
00373274: ldr      sl, [r4, #0x380]
00373278: add      sb, r5, #0x560
0037327c: mov      r0, sb
00373280: mov      r1, sl
00373284: bl       #0x3e0854
00373288: mov      r0, r5
0037328c: mov      r1, sl
00373290: bl       #0x3bb814
00373294: add      r0, r5, #0x3c8
00373298: bl       #0x3d8cfc
0037329c: mov      lr, #1
003732a0: str      lr, [sp, #0x28]
003732a4: b        #0x373314
003732a8: bl       #0x7fd794
003732ac: bl       #0x7fd5b4
003732b0: cmp      r0, #0
003732b4: beq      #0x372eec
003732b8: ldr      r0, [sp, #0xc]
003732bc: cmp      r0, #0
003732c0: beq      #0x372eec
003732c4: ldr      r1, [sp, #0x10]
003732c8: cmp      r1, #0
003732cc: bne      #0x372eec
003732d0: ldr      r2, [sp, #0x18]
003732d4: ldr      r4, [pc, #0x420]
003732d8: ldr      r0, [r8, r2]
003732dc: add      r4, pc, r4
003732e0: ldr      r5, [r4]
003732e4: bl       #0x31f66c
003732e8: rsb      r0, r0, r5
003732ec: cmp      r0, #0
003732f0: movle    r3, #0x7d0
003732f4: str      r0, [r4]
003732f8: strle    r3, [r4]
003732fc: movle    r4, #1
00373300: bgt      #0x372eec
00373304: b        #0x372ef0
00373308: mov      ip, #0
0037330c: str      ip, [sp, #0x28]
00373310: add      sb, r5, #0x560
00373314: ldr      r1, [r4, #0x498]
00373318: ldr      r3, [r5, #0x39c]
0037331c: cmp      r1, r3
00373320: addeq    sl, r5, #0x37c
00373324: beq      #0x373334
00373328: add      sl, r5, #0x37c
0037332c: mov      r0, sl
00373330: bl       #0x3fdfd8
00373334: ldr      r3, [r4, #0x358]
00373338: mov      r0, sl
0037333c: str      r3, [sp, #8]
00373340: bl       #0x3fc690
00373344: ldr      r3, [sp, #8]
00373348: cmp      r0, r3
0037334c: beq      #0x37335c
00373350: mov      r0, sl
00373354: ldr      r1, [r4, #0x358]
00373358: bl       #0x3ffc40
0037335c: mov      r0, sb
00373360: mov      r1, #0x21
00373364: mov      r2, #0
00373368: ldr      sl, [r4, #0x448]
0037336c: bl       #0x3df6e0
00373370: cmp      r0, sl
00373374: beq      #0x373388
00373378: mov      r0, sb
0037337c: mov      r1, #0x21
00373380: ldr      r2, [r4, #0x448]
00373384: bl       #0x3e0808
00373388: mov      r0, sb
0037338c: mov      r1, #0x24
00373390: mov      r2, #0
00373394: ldr      sl, [r4, #0x470]
00373398: bl       #0x3df6e0
0037339c: cmp      r0, sl
003733a0: beq      #0x3733d4
003733a4: ldr      r3, [r5]
003733a8: mov      r0, r5
003733ac: mov      lr, pc
003733b0: ldr      pc, [r3, #0x34]
003733b4: mov      r1, #0x24
003733b8: mov      r2, #0
003733bc: mov      r0, sb
003733c0: bl       #0x3df6e0
003733c4: mov      r0, sb
003733c8: mov      r1, #0x24
003733cc: ldr      r2, [r4, #0x470]
003733d0: bl       #0x3e0808
003733d4: ldr      sl, [r4, #0x660]
003733d8: mov      r0, r5
003733dc: ldr      r2, [sl, #0x93c]
003733e0: ldr      r3, [sl, #0x5b8]
003733e4: add      r3, r2, r3
003733e8: asr      r3, r3, #8
003733ec: str      r3, [sp, #0x14]
003733f0: bl       #0x3bb828
003733f4: ldr      r1, [r4, #0x330]
003733f8: mov      r3, r0
003733fc: cmp      r0, r1
00373400: beq      #0x373410
00373404: mov      r0, r5
00373408: bl       #0x3bb840
0037340c: ldr      r3, [r4, #0x330]
00373410: ldr      r0, [sp, #0x14]
00373414: cmp      r0, r3
00373418: beq      #0x373448
0037341c: ldr      r2, [sl, #0x5b8]
00373420: mov      r1, #0x13
00373424: mov      r0, sb
00373428: rsb      r2, r2, r3, lsl #8
0037342c: asr      r2, r2, #8
00373430: bl       #0x3e0808
00373434: ldr      r2, [r4, #0x330]
00373438: ldr      r1, [sp, #0x14]
0037343c: rsb      r2, r1, r2
00373440: cmp      r2, #1
00373444: beq      #0x373978
00373448: add      r0, r4, #0x3d8
0037344c: bl       #0x814f70
00373450: cmp      r0, #0
00373454: beq      #0x3738a8
00373458: ldr      r1, [r4, #0x3f8]
0037345c: cmp      r1, #0
00373460: beq      #0x3739d0
00373464: ldr      r2, [r4, #0x3fc]
00373468: cmp      r2, #0
0037346c: ble      #0x3739d0
00373470: add      sb, sp, #0xc4
00373474: mov      r0, sb
00373478: bl       #0x30e868
0037347c: mov      sl, #0
00373480: ldrsb    r2, [sb, sl]
00373484: mov      r1, sl
00373488: mov      r0, r5
0037348c: cmn      r2, #1
00373490: mvnlt    lr, #0
00373494: strblt   lr, [sb, sl]
00373498: mvnlt    r2, #0
0037349c: add      sl, sl, #1
003734a0: bl       #0x3bbe54
003734a4: cmp      sl, #3
003734a8: bne      #0x373480
003734ac: add      r0, r4, #0x400
003734b0: bl       #0x814f70
003734b4: cmp      r0, #0
003734b8: beq      #0x3738c4
003734bc: movw     r3, #0x14e8
003734c0: ldr      r3, [r5, r3]
003734c4: cmp      r3, #0
003734c8: beq      #0x3734fc
003734cc: ldr      r3, [r3, #0x84]
003734d0: cmp      r3, #0x1e
003734d4: bls      #0x3734fc
003734d8: ldr      r3, [pc, #0x240]
003734dc: ldr      r3, [r8, r3]
003734e0: ldr      r3, [r3]
003734e4: cmp      r3, #2
003734e8: moveq    r3, #0
003734ec: streq    r3, [r3]
003734f0: beq      #0x3734fc
003734f4: cmp      r3, #1
003734f8: beq      #0x373b04
003734fc: ldr      r1, [r4, #0x420]
00373500: cmp      r1, #0
00373504: beq      #0x37351c
00373508: ldr      r2, [r4, #0x424]
0037350c: cmp      r2, #0
00373510: ble      #0x37351c
00373514: add      r0, sp, #0xd4
00373518: bl       #0x30e868
0037351c: mov      sl, #0
00373520: add      r3, sp, #0xd4
00373524: str      r4, [sp, #0x14]
00373528: mov      r1, sl
0037352c: movw     sb, #0x14e8
00373530: mov      r4, r3
00373534: ldr      r3, [r5, sb]
00373538: cmp      r3, #0
0037353c: beq      #0x373568
00373540: ldr      r3, [r3, #0x84]
00373544: cmp      sl, r3
00373548: bhs      #0x373568
0037354c: ldrsb    r2, [r4, sl]
00373550: cmp      r2, #0
00373554: blt      #0x373568
00373558: mov      r1, sl
0037355c: mov      r0, r5
00373560: bl       #0x3bbebc
00373564: mov      r1, #1
00373568: add      sl, sl, #1
0037356c: cmp      sl, #0x1e
00373570: bne      #0x373534
00373574: cmp      r1, #0
00373578: ldr      r4, [sp, #0x14]
0037357c: bne      #0x37396c
00373580: ldrb     r3, [r4, #0x4e5]
00373584: cmp      r3, #0
00373588: ldrne    r1, [sp, #0x2c]
0037358c: moveq    sb, r3
00373590: ldrne    r2, [r8, r1]
00373594: ldrbne   sb, [r2, #0x30]
00373598: ldrb     r2, [r5, #0x80]
0037359c: eorne    sb, sb, #1
003735a0: cmp      r2, sb
003735a4: addne    sl, r5, #0x4f0
003735a8: addne    sl, sl, #0xc
003735ac: beq      #0x373a28
003735b0: mov      r0, sl
003735b4: mov      r1, #0
003735b8: bl       #0x3c1a00
003735bc: add      sl, r4, #0x4c0
003735c0: mov      r0, r5
003735c4: mov      r1, sb
003735c8: add      sl, sl, #8
003735cc: ldr      r3, [r5]
003735d0: mov      lr, pc
003735d4: ldr      pc, [r3, #0x40]
003735d8: mov      r0, sl
003735dc: bl       #0x814f70
003735e0: b        #0x372e6c
003735e4: bl       #0x320e98
003735e8: ldrb     r3, [r0, #0x24]
003735ec: cmp      r3, #0
003735f0: beq      #0x372aac
003735f4: bl       #0x800f8c
003735f8: ldr      r3, [r0]
003735fc: mov      lr, pc
00373600: ldr      pc, [r3, #0x64]
00373604: cmp      r0, #0
00373608: beq      #0x372aac
0037360c: bl       #0x8100dc
00373610: bl       #0x8100e0
00373614: b        #0x372aac
00373618: bl       #0x30e5e0
0037361c: cmp      r0, #0
00373620: bne      #0x372950
00373624: mov      r0, sl
00373628: bl       #0x318254
0037362c: ldr      r0, [sp, #0x14]
00373630: bl       #0x318254
00373634: b        #0x372988
00373638: ldr      r3, [sp, #0x18]
0037363c: ldr      r5, [r8, r3]
00373640: mov      r0, r5
00373644: bl       #0x31f594
00373648: cmp      r0, #0
0037364c: beq      #0x372f00
00373650: mov      r1, #0
00373654: mov      r0, r7
00373658: mov      r2, r1
0037365c: bl       #0x36e478
00373660: ldrb     r3, [r0, #0x4e5]
00373664: cmp      r3, #0
00373668: bne      #0x372f00
0037366c: ldr      r6, [pc, #0x8c]
00373670: mov      r0, r5
00373674: add      r6, pc, r6
00373678: ldr      r5, [r6, #4]
0037367c: bl       #0x31f66c
00373680: rsb      r0, r0, r5
00373684: cmp      r0, #0
00373688: movwle   r3, #0x1388
0037368c: str      r0, [r6, #4]
00373690: strle    r3, [r6, #4]
00373694: bgt      #0x372f00
00373698: bl       #0x8100dc
0037369c: bl       #0x8103f4
003736a0: mov      r1, #0
003736a4: mov      r2, r1
003736a8: mov      r0, r7
003736ac: bl       #0x36e478
003736b0: bl       #0x81347c
003736b4: bl       #0x8151e8
003736b8: mov      r3, #0
003736bc: str      r3, [r0, #0x2c]
003736c0: b        #0x372f08
003736c4: rsbeq    r2, r2, r4, ror r2
003736c8: andeq    r4, r0, ip, lsr #1
003736cc: strdeq   r3, r4, [r0], -r4
003736d0: subseq   lr, r4, r4, asr #30
003736d4: andeq    r1, r0, r0, lsr #20
003736d8: subseq   lr, r4, r0, asr #30

# _ZNK9Character8GetLevelEv
003bd120: add      r0, r0, #0x560
003bd124: mov      r1, #0x13
003bd128: mov      r2, #0
003bd12c: b        #0x3df6e0
