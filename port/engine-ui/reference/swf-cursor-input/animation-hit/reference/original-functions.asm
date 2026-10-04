
# _ZN7gameswf10scene_node15get_local_mouseEPNS_9characterERfS3_
00777514: push     {r4, r5, r6, r7, lr}
00777518: mov      r5, r0
0077751c: sub      sp, sp, #0x14
00777520: mov      r4, r1
00777524: mov      r6, r2
00777528: mov      r7, r3
0077752c: bl       #0x777350
00777530: ldr      r2, [r5, #0x260]
00777534: ldr      r3, [r5, #0x264]
00777538: mov      r1, #0x41000000
0077753c: str      r2, [sp, #8]
00777540: ldr      r0, [sp, #8]
00777544: add      r1, r1, #0xa00000
00777548: str      r3, [sp, #0xc]
0077754c: bl       #0x30ed6c
00777550: mov      r1, #0x41000000
00777554: str      r0, [sp, #8]
00777558: add      r1, r1, #0xa00000
0077755c: ldr      r0, [sp, #0xc]
00777560: bl       #0x30ed6c
00777564: ldr      r3, [sp, #8]
00777568: str      r0, [sp, #0xc]
0077756c: add      r5, r4, #0x3c
00777570: str      r3, [sp]
00777574: ldr      r3, [sp, #0xc]
00777578: mov      r0, r5
0077757c: str      r3, [sp, #4]
00777580: bl       #0x386144
00777584: ldr      r3, [r4, #0x40]
00777588: cmp      r3, #0
0077758c: beq      #0x7775ac
00777590: mov      r0, r5
00777594: bl       #0x386144
00777598: ldr      r0, [r4, #0x40]
0077759c: bl       #0x753f74
007775a0: mov      r1, sp
007775a4: add      r2, sp, #8
007775a8: bl       #0x753d7c
007775ac: ldr      r3, [sp, #4]
007775b0: ldr      r2, [sp]
007775b4: str      r2, [r6]
007775b8: str      r3, [r7]
007775bc: add      sp, sp, #0x14
007775c0: pop      {r4, r5, r6, r7, pc}

# _ZN8RenderFX17CollectCharactersEPN7gameswf9characterEPKci
007a8acc: push     {r4, r5, r6, r7, r8, lr}
007a8ad0: tst      r3, #1
007a8ad4: ldrbne   r4, [r1, #0x9b]
007a8ad8: mov      r6, r3
007a8adc: mov      r8, r0
007a8ae0: ldr      r3, [r1]
007a8ae4: mov      r0, r1
007a8ae8: mov      r5, r1
007a8aec: mov      r1, #2
007a8af0: moveq    r4, #1
007a8af4: mov      r7, r2
007a8af8: mov      lr, pc
007a8afc: ldr      pc, [r3, #8]
007a8b00: cmp      r0, #0
007a8b04: beq      #0x7a8b10
007a8b08: tst      r6, #2
007a8b0c: bne      #0x7a8bc4
007a8b10: cmp      r4, #0
007a8b14: beq      #0x7a8bc0
007a8b18: cmp      r7, #0
007a8b1c: beq      #0x7a8b44
007a8b20: ldr      r0, [r5, #0x44]
007a8b24: mov      r1, r7
007a8b28: ldrsb    r3, [r0]
007a8b2c: cmn      r3, #1
007a8b30: addne    r0, r0, #1
007a8b34: ldreq    r0, [r0, #0xc]
007a8b38: bl       #0x30ebd4
007a8b3c: cmp      r0, #0
007a8b40: beq      #0x7a8b6c
007a8b44: tst      r6, #4
007a8b48: bne      #0x7a8bd4
007a8b4c: ldr      r3, [r8, #8]
007a8b50: ldr      r2, [r8, #0xc]
007a8b54: add      r4, r3, #1
007a8b58: cmp      r4, r2
007a8b5c: bgt      #0x7a8bf4
007a8b60: ldr      r2, [r8, #4]
007a8b64: str      r5, [r2, r3, lsl #2]
007a8b68: str      r4, [r8, #8]
007a8b6c: ldr      r3, [r5]
007a8b70: mov      r0, r5
007a8b74: mov      r1, #2
007a8b78: mov      lr, pc
007a8b7c: ldr      pc, [r3, #8]
007a8b80: cmp      r0, #0
007a8b84: beq      #0x7a8bc0
007a8b88: ldr      r3, [r5, #0xac]
007a8b8c: cmp      r3, #0
007a8b90: ble      #0x7a8bc0
007a8b94: mov      r4, #0
007a8b98: ldr      r3, [r5, #0xa8]
007a8b9c: mov      r0, r8
007a8ba0: mov      r2, r7
007a8ba4: ldr      r1, [r3, r4, lsl #2]
007a8ba8: mov      r3, r6
007a8bac: bl       #0x7a8acc
007a8bb0: ldr      r3, [r5, #0xac]
007a8bb4: add      r4, r4, #1
007a8bb8: cmp      r4, r3
007a8bbc: blt      #0x7a8b98
007a8bc0: pop      {r4, r5, r6, r7, r8, pc}
007a8bc4: ldrb     r3, [r5, #0xea]
007a8bc8: cmp      r3, #0
007a8bcc: bne      #0x7a8b10
007a8bd0: pop      {r4, r5, r6, r7, r8, pc}
007a8bd4: ldr      r2, [r5, #0x44]
007a8bd8: ldrsb    r3, [r2]
007a8bdc: cmn      r3, #1
007a8be0: ldreq    r3, [r2, #4]
007a8be4: sub      r3, r3, #1
007a8be8: cmp      r3, #0
007a8bec: beq      #0x7a8b6c
007a8bf0: b        #0x7a8b4c
007a8bf4: add      r0, r8, #4
007a8bf8: add      r1, r4, r4, asr #1
007a8bfc: bl       #0x413e14
007a8c00: ldr      r3, [r8, #8]
007a8c04: b        #0x7a8b60

# _ZNK7gameswf6matrix20transform_by_inverseEPNS_5pointERKS1_
00753d7c: push     {r4, r5, r6, r7, lr}
00753d80: sub      sp, sp, #0x1c
00753d84: mov      ip, #0
00753d88: add      r3, sp, #8
00753d8c: str      ip, [r3], #4
00753d90: str      ip, [r3], #4
00753d94: str      ip, [r3], #4
00753d98: mov      r7, r0
00753d9c: mov      r4, r2
00753da0: mov      lr, #0x3f800000
00753da4: str      ip, [r3]
00753da8: mov      r5, r1
00753dac: mov      r0, sp
00753db0: mov      r1, r7
00753db4: str      lr, [sp, #0x10]
00753db8: str      ip, [sp, #4]
00753dbc: str      lr, [sp]
00753dc0: bl       #0x795adc
00753dc4: ldr      r1, [r4]
00753dc8: ldr      r0, [sp]
00753dcc: bl       #0x30ed6c
00753dd0: ldr      r1, [r4, #4]
00753dd4: mov      r6, r0
00753dd8: ldr      r0, [sp, #4]
00753ddc: bl       #0x30ed6c
00753de0: mov      r1, r0
00753de4: mov      r0, r6
00753de8: bl       #0x30eba4
00753dec: ldr      r1, [sp, #8]
00753df0: bl       #0x30eba4
00753df4: str      r0, [r5]
00753df8: ldr      r1, [r4]
00753dfc: ldr      r0, [sp, #0xc]
00753e00: bl       #0x30ed6c
00753e04: ldr      r1, [r4, #4]
00753e08: mov      r6, r0
00753e0c: ldr      r0, [sp, #0x10]
00753e10: bl       #0x30ed6c
00753e14: mov      r1, r0
00753e18: mov      r0, r6
00753e1c: bl       #0x30eba4
00753e20: ldr      r1, [sp, #0x14]
00753e24: bl       #0x30eba4
00753e28: str      r0, [r5, #4]
00753e2c: add      sp, sp, #0x1c
00753e30: pop      {r4, r5, r6, r7, pc}

# _ZN8RenderFX9GotoFrameEPN7gameswf9characterEPKcb
007ab924: push     {r4, r5, r6, r7, r8, sl, lr}
007ab928: ldr      r4, [pc, #0xcc]
007ab92c: ldr      r6, [pc, #0xcc]
007ab930: mov      r8, r3
007ab934: add      r4, pc, r4
007ab938: ldr      r0, [r4, r6]
007ab93c: sub      sp, sp, #0x1c
007ab940: subs     r5, r1, #0
007ab944: ldr      r3, [r0]
007ab948: mov      r7, r2
007ab94c: str      r3, [sp, #0x14]
007ab950: beq      #0x7ab9c8
007ab954: ldr      r2, [r5]
007ab958: mov      r0, r5
007ab95c: mov      r1, #2
007ab960: mov      lr, pc
007ab964: ldr      pc, [r2, #8]
007ab968: cmp      r0, #0
007ab96c: beq      #0x7ab9c8
007ab970: ldr      r3, [r5]
007ab974: mov      r1, r7
007ab978: mov      r0, sp
007ab97c: ldr      r7, [r3, #0x9c]
007ab980: bl       #0x413a7c
007ab984: mov      r0, r5
007ab988: mov      r1, sp
007ab98c: blx      r7
007ab990: ldrsb    r3, [sp]
007ab994: mov      sl, sp
007ab998: mov      r7, r0
007ab99c: cmn      r3, #1
007ab9a0: beq      #0x7ab9e8
007ab9a4: cmp      r7, #0
007ab9a8: beq      #0x7ab9c8
007ab9ac: mov      r0, r5
007ab9b0: eor      r1, r8, #1
007ab9b4: ldr      r3, [r5]
007ab9b8: mov      lr, pc
007ab9bc: ldr      pc, [r3, #0x94]
007ab9c0: mov      r0, #1
007ab9c4: b        #0x7ab9cc
007ab9c8: mov      r0, #0
007ab9cc: ldr      r3, [r4, r6]
007ab9d0: ldr      r2, [sp, #0x14]
007ab9d4: ldr      r3, [r3]
007ab9d8: cmp      r2, r3
007ab9dc: bne      #0x7ab9f8
007ab9e0: add      sp, sp, #0x1c
007ab9e4: pop      {r4, r5, r6, r7, r8, sl, pc}
007ab9e8: ldr      r0, [sp, #0xc]
007ab9ec: ldr      r1, [sp, #8]
007ab9f0: bl       #0x752b38
007ab9f4: b        #0x7ab9a4
007ab9f8: bl       #0x30e310
007ab9fc: andseq   sb, lr, ip, asr r1
007aba00: andeq    r4, r0, ip, lsr #1

# _ZN7gameswf6matrix11set_inverseERKS0_
00795adc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00795ae0: ldr      r6, [r1, #0x10]
00795ae4: mov      r4, r1
00795ae8: mov      r5, r0
00795aec: ldr      r1, [r1]
00795af0: mov      r0, r6
00795af4: bl       #0x30ed6c
00795af8: ldr      r1, [r4, #0xc]
00795afc: mov      r7, r0
00795b00: ldr      r0, [r4, #4]
00795b04: bl       #0x30ed6c
00795b08: mov      r1, r0
00795b0c: mov      r0, r7
00795b10: bl       #0x30e3ac
00795b14: mov      r1, #0
00795b18: mov      r7, r0
00795b1c: bl       #0x30df8c
00795b20: cmp      r0, #0
00795b24: bne      #0x795c70
00795b28: mov      r1, r7
00795b2c: mov      r0, #0x3f800000
00795b30: bl       #0x30ec94
00795b34: mov      r1, r6
00795b38: mov      r7, r0
00795b3c: bl       #0x30ed6c
00795b40: mvn      r1, #0x800000
00795b44: mov      sl, r0
00795b48: bl       #0x30e4b4
00795b4c: cmp      r0, #0
00795b50: bne      #0x795d80
00795b54: mov      sl, #0
00795b58: str      sl, [r5]
00795b5c: ldr      r1, [r4]
00795b60: mov      r0, r7
00795b64: bl       #0x30ed6c
00795b68: mvn      r1, #0x800000
00795b6c: mov      r6, r0
00795b70: bl       #0x30e4b4
00795b74: cmp      r0, #0
00795b78: bne      #0x795d64
00795b7c: mov      r6, #0
00795b80: str      r6, [r5, #0x10]
00795b84: ldr      r0, [r4, #4]
00795b88: mov      r1, r7
00795b8c: add      r0, r0, #0x80000000
00795b90: bl       #0x30ed6c
00795b94: mvn      r1, #0x800000
00795b98: mov      r8, r0
00795b9c: bl       #0x30e4b4
00795ba0: cmp      r0, #0
00795ba4: bne      #0x795d48
00795ba8: mov      r8, #0
00795bac: str      r8, [r5, #4]
00795bb0: ldr      r0, [r4, #0xc]
00795bb4: mov      r1, r7
00795bb8: add      r0, r0, #0x80000000
00795bbc: bl       #0x30ed6c
00795bc0: mvn      r1, #0x800000
00795bc4: mov      r7, r0
00795bc8: bl       #0x30e4b4
00795bcc: cmp      r0, #0
00795bd0: bne      #0x795d2c
00795bd4: mov      r7, #0
00795bd8: str      r7, [r5, #0xc]
00795bdc: mov      r0, sl
00795be0: ldr      r1, [r4, #8]
00795be4: bl       #0x30ed6c
00795be8: ldr      r1, [r4, #0x14]
00795bec: mov      sl, r0
00795bf0: mov      r0, r8
00795bf4: bl       #0x30ed6c
00795bf8: mov      r1, r0
00795bfc: mov      r0, sl
00795c00: bl       #0x30eba4
00795c04: add      r8, r0, #0x80000000
00795c08: mov      r0, r8
00795c0c: mvn      r1, #0x800000
00795c10: bl       #0x30e4b4
00795c14: cmp      r0, #0
00795c18: bne      #0x795d10
00795c1c: mov      r8, #0
00795c20: str      r8, [r5, #8]
00795c24: ldr      r1, [r4, #8]
00795c28: mov      r0, r7
00795c2c: bl       #0x30ed6c
00795c30: ldr      r1, [r4, #0x14]
00795c34: mov      r7, r0
00795c38: mov      r0, r6
00795c3c: bl       #0x30ed6c
00795c40: mov      r1, r0
00795c44: mov      r0, r7
00795c48: bl       #0x30eba4
00795c4c: add      r4, r0, #0x80000000
00795c50: mov      r0, r4
00795c54: mvn      r1, #0x800000
00795c58: bl       #0x30e4b4
00795c5c: cmp      r0, #0
00795c60: bne      #0x795cf0
00795c64: mov      r4, #0
00795c68: str      r4, [r5, #0x14]
00795c6c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00795c70: mov      r2, #0
00795c74: add      r3, r5, #8
00795c78: str      r2, [r5, #4]
00795c7c: str      r2, [r3], #4
00795c80: str      r2, [r3], #4
00795c84: str      r2, [r3], #4
00795c88: mov      r1, #0x3f800000
00795c8c: str      r2, [r3]
00795c90: str      r1, [r5, #0x10]
00795c94: str      r1, [r5]
00795c98: ldr      r6, [r4, #8]
00795c9c: mvn      r1, #0x800000
00795ca0: add      r6, r6, #0x80000000
00795ca4: mov      r0, r6
00795ca8: bl       #0x30e4b4
00795cac: cmp      r0, #0
00795cb0: beq      #0x795ccc
00795cb4: mvn      r1, #0x80000000
00795cb8: mov      r0, r6
00795cbc: sub      r1, r1, #0x800000
00795cc0: bl       #0x30e9ac
00795cc4: cmp      r0, #0
00795cc8: bne      #0x795cd0
00795ccc: mov      r6, #0
00795cd0: str      r6, [r5, #8]
00795cd4: ldr      r4, [r4, #0x14]
00795cd8: mvn      r1, #0x800000
00795cdc: add      r4, r4, #0x80000000
00795ce0: mov      r0, r4
00795ce4: bl       #0x30e4b4
00795ce8: cmp      r0, #0
00795cec: beq      #0x795c64
00795cf0: mvn      r1, #0x80000000
00795cf4: mov      r0, r4
00795cf8: sub      r1, r1, #0x800000
00795cfc: bl       #0x30e9ac
00795d00: cmp      r0, #0
00795d04: beq      #0x795c64
00795d08: str      r4, [r5, #0x14]
00795d0c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00795d10: mvn      r1, #0x80000000
00795d14: mov      r0, r8
00795d18: sub      r1, r1, #0x800000
00795d1c: bl       #0x30e9ac
00795d20: cmp      r0, #0
00795d24: bne      #0x795c20
00795d28: b        #0x795c1c
00795d2c: mvn      r1, #0x80000000
00795d30: mov      r0, r7
00795d34: sub      r1, r1, #0x800000
00795d38: bl       #0x30e9ac
00795d3c: cmp      r0, #0
00795d40: bne      #0x795bd8
00795d44: b        #0x795bd4
00795d48: mvn      r1, #0x80000000
00795d4c: mov      r0, r8
00795d50: sub      r1, r1, #0x800000
00795d54: bl       #0x30e9ac
00795d58: cmp      r0, #0
00795d5c: bne      #0x795bac
00795d60: b        #0x795ba8
00795d64: mvn      r1, #0x80000000
00795d68: mov      r0, r6
00795d6c: sub      r1, r1, #0x800000
00795d70: bl       #0x30e9ac
00795d74: cmp      r0, #0
00795d78: bne      #0x795b80
00795d7c: b        #0x795b7c
00795d80: mvn      r1, #0x80000000
00795d84: mov      r0, sl
00795d88: sub      r1, r1, #0x800000
00795d8c: bl       #0x30e9ac
00795d90: cmp      r0, #0
00795d94: bne      #0x795b58
00795d98: b        #0x795b54
