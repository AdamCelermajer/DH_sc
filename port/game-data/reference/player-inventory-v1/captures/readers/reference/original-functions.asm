
# _ZN11IStreamBase6readAsERSs
00461da8: push     {r4, r5, lr}
00461dac: sub      sp, sp, #0x14
00461db0: mov      r4, r1
00461db4: add      r1, sp, #0xc
00461db8: mov      r5, r0
00461dbc: bl       #0x38b758
00461dc0: ldr      r1, [sp, #0xc]
00461dc4: ldr      r3, [pc, #0xac]
00461dc8: cmp      r1, #0
00461dcc: add      r3, pc, r3
00461dd0: ble      #0x461e04
00461dd4: sub      r1, r1, #1
00461dd8: mov      r0, r4
00461ddc: bl       #0x461ca8
00461de0: ldr      r2, [sp, #0xc]
00461de4: mov      r0, r5
00461de8: ldr      r1, [r4, #0x14]
00461dec: asr      r3, r2, #0x1f
00461df0: ldr      ip, [r5]
00461df4: mov      lr, pc
00461df8: ldr      pc, [ip, #0x18]
00461dfc: add      sp, sp, #0x14
00461e00: pop      {r4, r5, pc}
00461e04: ldr      r2, [pc, #0x70]
00461e08: ldr      r2, [r3, r2]
00461e0c: ldr      r2, [r2]
00461e10: cmp      r2, #2
00461e14: moveq    r3, #0
00461e18: streq    r3, [r3]
00461e1c: beq      #0x461e28
00461e20: cmp      r2, #1
00461e24: beq      #0x461e38
00461e28: mov      r0, r4
00461e2c: mov      r1, #0
00461e30: bl       #0x461ca8
00461e34: b        #0x461dfc
00461e38: ldr      r0, [pc, #0x40]
00461e3c: ldr      r1, [pc, #0x40]
00461e40: ldr      r2, [pc, #0x40]
00461e44: ldr      r0, [r3, r0]
00461e48: ldr      r3, [pc, #0x3c]
00461e4c: add      r1, pc, r1
00461e50: mov      ip, #0x57
00461e54: add      r0, r0, #0xa8
00461e58: add      r2, pc, r2
00461e5c: add      r3, pc, r3
00461e60: str      ip, [sp]
00461e64: bl       #0x30e004
00461e68: ldr      r1, [sp, #0xc]
00461e6c: cmp      r1, #0
00461e70: ble      #0x461e28
00461e74: b        #0x461dd4
00461e78: subseq   r2, r3, r4, asr #25
00461e7c: andeq    r3, r0, r0, asr #19
00461e80: andeq    r1, r0, r0, asr #19
00461e84: subeq    ip, r5, ip, lsl #11
00461e88: subeq    fp, r6, r8, ror #5
00461e8c: strheq   ip, [r5], #-0x6c

# _ZN11IStreamBase6readAsIjEEvRT_
00313b48: str      lr, [sp, #-4]!
00313b4c: mov      r3, #0
00313b50: sub      sp, sp, #0xc
00313b54: ldr      ip, [r0]
00313b58: mov      r2, #4
00313b5c: mov      lr, pc
00313b60: ldr      pc, [ip, #0x18]
00313b64: ldr      r3, [pc, #0x74]
00313b68: cmp      r0, #4
00313b6c: add      r3, pc, r3
00313b70: beq      #0x313ba0
00313b74: ldr      r2, [pc, #0x68]
00313b78: ldr      r2, [r3, r2]
00313b7c: ldr      r2, [r2]
00313b80: cmp      r2, #2
00313b84: moveq    r3, #0
00313b88: streq    r3, [r3]
00313b8c: beq      #0x313b98
00313b90: cmp      r2, #1
00313b94: beq      #0x313bac
00313b98: add      sp, sp, #0xc
00313b9c: ldm      sp!, {pc}
00313ba0: cmp      r1, #0
00313ba4: beq      #0x313b98
00313ba8: b        #0x313b74
00313bac: ldr      r0, [pc, #0x34]
00313bb0: ldr      r1, [pc, #0x34]
00313bb4: ldr      r2, [pc, #0x34]
00313bb8: ldr      r0, [r3, r0]
00313bbc: ldr      r3, [pc, #0x30]
00313bc0: mov      ip, #0x45
00313bc4: add      r1, pc, r1
00313bc8: add      r2, pc, r2
00313bcc: add      r3, pc, r3
00313bd0: add      r0, r0, #0xa8
00313bd4: str      ip, [sp]
00313bd8: bl       #0x30e004
00313bdc: b        #0x313b98
00313be0: rsbeq    r0, r8, r4, lsr #30
00313be4: andeq    r3, r0, r0, asr #19
00313be8: andeq    r1, r0, r0, asr #19
00313bec: subseq   sl, sl, r4, lsl r8
00313bf0: subseq   sl, sl, r8, lsr sb
00313bf4: subseq   sl, sl, ip, asr #18

# _ZN11IStreamBase6readAsIiEEvRT_
0038b758: str      lr, [sp, #-4]!
0038b75c: mov      r3, #0
0038b760: sub      sp, sp, #0xc
0038b764: ldr      ip, [r0]
0038b768: mov      r2, #4
0038b76c: mov      lr, pc
0038b770: ldr      pc, [ip, #0x18]
0038b774: ldr      r3, [pc, #0x74]
0038b778: cmp      r0, #4
0038b77c: add      r3, pc, r3
0038b780: beq      #0x38b7b0
0038b784: ldr      r2, [pc, #0x68]
0038b788: ldr      r2, [r3, r2]
0038b78c: ldr      r2, [r2]
0038b790: cmp      r2, #2
0038b794: moveq    r3, #0
0038b798: streq    r3, [r3]
0038b79c: beq      #0x38b7a8
0038b7a0: cmp      r2, #1
0038b7a4: beq      #0x38b7bc
0038b7a8: add      sp, sp, #0xc
0038b7ac: ldm      sp!, {pc}
0038b7b0: cmp      r1, #0
0038b7b4: beq      #0x38b7a8
0038b7b8: b        #0x38b784
0038b7bc: ldr      r0, [pc, #0x34]
0038b7c0: ldr      r1, [pc, #0x34]
0038b7c4: ldr      r2, [pc, #0x34]
0038b7c8: ldr      r0, [r3, r0]
0038b7cc: ldr      r3, [pc, #0x30]
0038b7d0: mov      ip, #0x45
0038b7d4: add      r1, pc, r1
0038b7d8: add      r2, pc, r2
0038b7dc: add      r3, pc, r3
0038b7e0: add      r0, r0, #0xa8
0038b7e4: str      ip, [sp]
0038b7e8: bl       #0x30e004
0038b7ec: b        #0x38b7a8
0038b7f0: rsbeq    sb, r0, r4, lsl r3
0038b7f4: andeq    r3, r0, r0, asr #19
0038b7f8: andeq    r1, r0, r0, asr #19
0038b7fc: subseq   r2, r3, r4, lsl #24
0038b800: subseq   r2, r3, r8, lsr #26
0038b804: subseq   r2, r3, ip, lsr sp

# _ZN11IStreamBase6readAsItEEvRT_
00469070: str      lr, [sp, #-4]!
00469074: mov      r3, #0
00469078: sub      sp, sp, #0xc
0046907c: ldr      ip, [r0]
00469080: mov      r2, #2
00469084: mov      lr, pc
00469088: ldr      pc, [ip, #0x18]
0046908c: ldr      r3, [pc, #0x74]
00469090: cmp      r0, #2
00469094: add      r3, pc, r3
00469098: beq      #0x4690c8
0046909c: ldr      r2, [pc, #0x68]
004690a0: ldr      r2, [r3, r2]
004690a4: ldr      r2, [r2]
004690a8: cmp      r2, #2
004690ac: moveq    r3, #0
004690b0: streq    r3, [r3]
004690b4: beq      #0x4690c0
004690b8: cmp      r2, #1
004690bc: beq      #0x4690d4
004690c0: add      sp, sp, #0xc
004690c4: ldm      sp!, {pc}
004690c8: cmp      r1, #0
004690cc: beq      #0x4690c0
004690d0: b        #0x46909c
004690d4: ldr      r0, [pc, #0x34]
004690d8: ldr      r1, [pc, #0x34]
004690dc: ldr      r2, [pc, #0x34]
004690e0: ldr      r0, [r3, r0]
004690e4: ldr      r3, [pc, #0x30]
004690e8: mov      ip, #0x45
004690ec: add      r1, pc, r1
004690f0: add      r2, pc, r2
004690f4: add      r3, pc, r3
004690f8: add      r0, r0, #0xa8
004690fc: str      ip, [sp]
00469100: bl       #0x30e004
00469104: b        #0x4690c0
00469108: ldrsheq  fp, [r2], #-0x9c
0046910c: andeq    r3, r0, r0, asr #19
00469110: andeq    r1, r0, r0, asr #19
00469114: subeq    r5, r5, ip, ror #5
00469118: subeq    r5, r5, r0, lsl r4
0046911c: subeq    r5, r5, r4, lsr #8

# _ZN12StreamReader6readAsIjEET_P11IStreamBase
00313a90: str      lr, [sp, #-4]!
00313a94: sub      sp, sp, #0x14
00313a98: mov      r3, #0
00313a9c: ldr      ip, [r0]
00313aa0: add      r1, sp, #0xc
00313aa4: mov      r2, #4
00313aa8: mov      lr, pc
00313aac: ldr      pc, [ip, #0x18]
00313ab0: ldr      r3, [pc, #0x78]
00313ab4: cmp      r0, #4
00313ab8: add      r3, pc, r3
00313abc: beq      #0x313af0
00313ac0: ldr      r2, [pc, #0x6c]
00313ac4: ldr      r2, [r3, r2]
00313ac8: ldr      r2, [r2]
00313acc: cmp      r2, #2
00313ad0: moveq    r3, #0
00313ad4: streq    r3, [r3]
00313ad8: beq      #0x313ae4
00313adc: cmp      r2, #1
00313ae0: beq      #0x313afc
00313ae4: ldr      r0, [sp, #0xc]
00313ae8: add      sp, sp, #0x14
00313aec: ldm      sp!, {pc}
00313af0: cmp      r1, #0
00313af4: beq      #0x313ae4
00313af8: b        #0x313ac0
00313afc: ldr      r0, [pc, #0x34]
00313b00: ldr      r1, [pc, #0x34]
00313b04: ldr      r2, [pc, #0x34]
00313b08: ldr      r0, [r3, r0]
00313b0c: ldr      r3, [pc, #0x30]
00313b10: mov      ip, #0x44
00313b14: add      r1, pc, r1
00313b18: add      r2, pc, r2
00313b1c: add      r3, pc, r3
00313b20: add      r0, r0, #0xa8
00313b24: str      ip, [sp]
00313b28: bl       #0x30e004
00313b2c: b        #0x313ae4

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE5eraseENS_17_Rb_tree_iteratorIS5_S9_EE
00467c18: push     {r4, lr}
00467c1c: mov      r4, r0
00467c20: add      r2, r4, #8
00467c24: ldr      r0, [r1]
00467c28: add      r3, r4, #0xc
00467c2c: add      r1, r4, #4
00467c30: bl       #0x336004
00467c34: cmp      r0, #0
00467c38: beq      #0x467c44
00467c3c: mov      r1, #0x18
00467c40: bl       #0x708f00
00467c44: ldr      r3, [r4, #0x10]
00467c48: sub      r3, r3, #1
00467c4c: str      r3, [r4, #0x10]
00467c50: pop      {r4, pc}
