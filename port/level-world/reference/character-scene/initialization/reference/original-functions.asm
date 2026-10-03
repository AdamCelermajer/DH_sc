
# _ZNK9Character11GetCharTypeEv
003a3054: push     {r4, lr}
003a3058: bl       #0x3a3024
003a305c: ldr      r0, [r0, #0x38]
003a3060: pop      {r4, pc}

# _ZN12VisualObject17SetAnimControllerEP14AnimController
00470a84: push     {r4, r5, r6, lr}
00470a88: ldr      r3, [r0, #0x38]
00470a8c: mov      r4, r0
00470a90: mov      r5, r1
00470a94: cmp      r3, r1
00470a98: beq      #0x470ac0
00470a9c: cmp      r3, #0
00470aa0: beq      #0x470abc
00470aa4: mov      r0, r3
00470aa8: ldr      r3, [r3]
00470aac: mov      lr, pc
00470ab0: ldr      pc, [r3, #4]
00470ab4: mov      r3, #0
00470ab8: str      r3, [r4, #0x38]
00470abc: str      r5, [r4, #0x38]
00470ac0: pop      {r4, r5, r6, pc}

# _ZNK9Character11GetCharAIIdEv
003a2fec: ldr      r0, [r0, #0xffc]
003a2ff0: ldr      r3, [pc, #0x24]
003a2ff4: cmp      r0, #0
003a2ff8: add      r3, pc, r3
003a2ffc: blt      #0x3a3014
003a3000: ldr      r2, [pc, #0x18]
003a3004: ldr      r3, [r3, r2]
003a3008: ldr      r3, [r3]
003a300c: cmp      r0, r3
003a3010: bxlt     lr
003a3014: mov      r0, #8
003a3018: bx       lr

# _ZN9Character18SetInitialPositionERK7Point3DIfE
003a58f4: push     {r4, r5, lr}
003a58f8: ldr      r3, [r1]
003a58fc: movw     r2, #0x1450
003a5900: ldr      lr, [pc, #0x64]
003a5904: str      r3, [r0, r2]
003a5908: ldr      r2, [r1, #4]
003a590c: movw     r3, #0x1454
003a5910: add      lr, pc, lr
003a5914: str      r2, [r0, r3]
003a5918: ldr      r3, [pc, #0x50]
003a591c: mov      r4, r0
003a5920: sub      sp, sp, #0x1c
003a5924: ldr      r0, [lr, r3]
003a5928: ldr      r3, [r1, #8]
003a592c: mov      ip, #0
003a5930: movw     r5, #0x1458
003a5934: add      r1, r4, #0x1440
003a5938: str      r3, [r4, r5]
003a593c: add      r1, r1, #0x10
003a5940: mov      r3, ip
003a5944: add      r2, sp, #0x14
003a5948: str      ip, [sp]
003a594c: str      ip, [sp, #4]
003a5950: str      ip, [sp, #8]
003a5954: bl       #0x525508
003a5958: cmp      r0, #0
003a595c: ldrne    r3, [sp, #0x14]
003a5960: strne    r3, [r4, r5]
003a5964: add      sp, sp, #0x1c
003a5968: pop      {r4, r5, pc}
003a596c: subseq   pc, lr, r0, lsl #3
003a5970: andeq    r1, r0, r4, lsl #4

# _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00472a0c: push     {r4, r5, r6, r7, r8, sl, lr}
00472a10: ldr      r6, [pc, #0x234]
00472a14: ldr      r5, [pc, #0x234]
00472a18: mov      ip, #0xbf000000
00472a1c: add      r6, pc, r6
00472a20: ldr      r5, [r6, r5]
00472a24: mov      lr, #0
00472a28: add      ip, ip, #0x800000
00472a2c: mov      r7, r1
00472a30: add      r1, r5, #8
00472a34: mov      r5, #0
00472a38: mov      r8, r3
00472a3c: sub      sp, sp, #0xc
00472a40: str      r1, [r0]
00472a44: str      lr, [r0, #0x24]
00472a48: str      ip, [r0, #0x74]
00472a4c: str      lr, [r0, #0x10]
00472a50: str      lr, [r0, #0x14]
00472a54: str      lr, [r0, #0x18]
00472a58: str      lr, [r0, #0x1c]
00472a5c: str      lr, [r0, #0x20]
00472a60: str      ip, [r0, #0x58]
00472a64: str      ip, [r0, #0x5c]
00472a68: str      ip, [r0, #0x60]
00472a6c: str      ip, [r0, #0x64]
00472a70: str      ip, [r0, #0x68]
00472a74: str      r7, [r0, #4]
00472a78: str      r5, [r0, #8]
00472a7c: str      r5, [r0, #0xc]
00472a80: strb     r5, [r0, #0x28]
00472a84: str      r5, [r0, #0x2c]
00472a88: str      r5, [r0, #0x30]
00472a8c: str      r5, [r0, #0x34]
00472a90: str      r5, [r0, #0x38]
00472a94: strb     r5, [r0, #0x3c]
00472a98: str      r5, [r0, #0x40]
00472a9c: str      r5, [r0, #0x44]
00472aa0: str      r5, [r0, #0x48]
00472aa4: str      r5, [r0, #0x4c]
00472aa8: str      r5, [r0, #0x50]
00472aac: str      r5, [r0, #0x54]
00472ab0: strb     r5, [r0, #0x6c]
00472ab4: strb     r5, [r0, #0x7c]
00472ab8: strb     r5, [r0, #0x7d]
00472abc: strb     r5, [r0, #0x7e]
00472ac0: strb     r5, [r0, #0x7f]
00472ac4: str      r5, [r0, #0x80]
00472ac8: str      r5, [r0, #0x84]
00472acc: str      r5, [r0, #0x88]
00472ad0: str      r5, [r0, #0x8c]
00472ad4: str      r5, [r0, #0x90]
00472ad8: str      r5, [r0, #0x94]
00472adc: str      r5, [r0, #0x9c]
00472ae0: str      r5, [r0, #0xa0]
00472ae4: str      r5, [r0, #0xa4]
00472ae8: strb     r5, [r0, #0xa9]
00472aec: mov      sl, r2
00472af0: mov      r4, r0
00472af4: bl       #0x50a564
00472af8: ldr      ip, [r8, #0x10]
00472afc: ldr      r2, [r8, #0x14]
00472b00: ldr      r1, [sl, #0x14]
00472b04: mov      r3, r5
00472b08: cmp      ip, r2
00472b0c: moveq    r2, r5
00472b10: mvn      ip, #0x80000000
00472b14: str      ip, [sp]
00472b18: bl       #0x50a504
00472b1c: cmp      r0, r5
00472b20: str      r0, [r4, #8]
00472b24: beq      #0x472c40
00472b28: mov      r1, r7
00472b2c: mov      r0, r4
00472b30: bl       #0x47295c
00472b34: ldr      r0, [r4, #8]
00472b38: bl       #0x35c854
00472b3c: mov      r0, r4
00472b40: bl       #0x4718f0
00472b44: ldr      r3, [pc, #0x108]
00472b48: ldr      r1, [r4, #8]
00472b4c: ldr      r5, [r6, r3]
00472b50: ldr      r3, [r5, #0x10]
00472b54: ldr      r3, [r3, #0x1c]
00472b58: ldr      r3, [r3, #4]
00472b5c: mov      r0, r3
00472b60: ldr      r3, [r3]
00472b64: mov      lr, pc
00472b68: ldr      pc, [r3, #0x5c]
00472b6c: ldr      r3, [r5, #0x10]
00472b70: ldr      r0, [r3, #0x1c]
00472b74: bl       #0x350ee0
00472b78: ldr      r3, [r5, #0x10]
00472b7c: ldr      r2, [pc, #0xd4]
00472b80: ldr      r1, [r4, #8]
00472b84: ldr      r0, [r3, #0x1c]
00472b88: add      r2, pc, r2
00472b8c: mov      r3, #1
00472b90: bl       #0x35a0e4
00472b94: subs     r2, r0, #0
00472b98: beq      #0x472c08
00472b9c: mov      r3, #1
00472ba0: strb     r3, [r4, #0x28]
00472ba4: ldr      r3, [r5, #0x10]
00472ba8: movw     r1, #0x6164
00472bac: movt     r1, #0x6d65
00472bb0: ldr      r3, [r3, #0x1c]
00472bb4: mov      r0, r3
00472bb8: ldr      r3, [r3]
00472bbc: mov      lr, pc
00472bc0: ldr      pc, [r3, #0x1c]
00472bc4: cmp      r0, #0
00472bc8: str      r0, [r4, #0xc]
00472bcc: beq      #0x472bec
00472bd0: ldr      r3, [r0]
00472bd4: ldr      r3, [r3, #-0xc]
00472bd8: add      r0, r0, r3
00472bdc: ldr      r3, [r0, #4]
00472be0: add      r3, r3, #1
00472be4: str      r3, [r0, #4]
00472be8: ldr      r0, [r4, #0xc]
00472bec: mov      r1, #0
00472bf0: strb     r1, [r0, #0x138]
00472bf4: ldr      r3, [r4, #0xc]
00472bf8: mov      r0, r3
00472bfc: ldr      r3, [r3]
00472c00: mov      lr, pc
00472c04: ldr      pc, [r3, #0x48]
00472c08: mov      r0, r4
00472c0c: bl       #0x47211c
00472c10: mov      r0, r4
00472c14: bl       #0x470a54
00472c18: mov      r1, #0
00472c1c: mov      r0, #8
00472c20: bl       #0x310570
00472c24: ldr      r1, [r4, #8]
00472c28: mov      r5, r0
00472c2c: mov      r2, #0
00472c30: bl       #0x474d30
00472c34: mov      r0, r4
00472c38: mov      r1, r5
00472c3c: bl       #0x470a84
00472c40: mov      r0, r4
00472c44: add      sp, sp, #0xc
00472c48: pop      {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq   r2, r2, r4, ror r0
00472c50: muleq    r0, r4, lr
00472c54: strdeq   r3, r4, [r0], -r4
00472c58: subeq    sl, r5, r0, lsr #22

# _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
004718f0: push     {r4, r5, r6, lr}
004718f4: ldr      r3, [r0, #8]
004718f8: ldr      r5, [pc, #0xf0]
004718fc: sub      sp, sp, #0x18
00471900: cmp      r3, #0
00471904: mov      r4, r0
00471908: add      r5, pc, r5
0047190c: beq      #0x471990
00471910: ldr      r2, [pc, #0xdc]
00471914: mov      r6, #0
00471918: str      r6, [sp, #0xc]
0047191c: ldr      r2, [r5, r2]
00471920: str      r6, [sp, #0x10]
00471924: str      r6, [sp, #0x14]
00471928: ldr      r2, [r2, #0x10]
0047192c: movw     r1, #0x6164
00471930: movt     r1, #0x4d65
00471934: ldr      ip, [r2, #0x1c]
00471938: add      r2, sp, #0xc
0047193c: mov      r0, ip
00471940: ldr      ip, [ip]
00471944: mov      lr, pc
00471948: ldr      pc, [ip, #0x20]
0047194c: ldr      r0, [sp, #0xc]
00471950: ldr      r1, [sp, #0x10]
00471954: rsb      r1, r0, r1
00471958: asrs     r1, r1, #2
0047195c: beq      #0x47197c
00471960: mov      r2, #1
00471964: ldr      r3, [r0, r6, lsl #2]
00471968: add      r6, r6, #1
0047196c: cmp      r6, r1
00471970: str      r3, [r4, #0x2c]
00471974: strb     r2, [r4, #0x7f]
00471978: blo      #0x471964
0047197c: cmp      r0, #0
00471980: beq      #0x471988
00471984: bl       #0x310450
00471988: add      sp, sp, #0x18
0047198c: pop      {r4, r5, r6, pc}
00471990: ldr      r2, [pc, #0x60]
00471994: ldr      r2, [r5, r2]
00471998: ldr      r2, [r2]
0047199c: cmp      r2, #2
004719a0: beq      #0x4719e4
004719a4: cmp      r2, #1
004719a8: bne      #0x471910
004719ac: ldr      r0, [pc, #0x48]
004719b0: ldr      r1, [pc, #0x48]
004719b4: ldr      r2, [pc, #0x48]
004719b8: ldr      r0, [r5, r0]
004719bc: ldr      r3, [pc, #0x44]
004719c0: movw     ip, #0x3ff
004719c4: add      r1, pc, r1
004719c8: add      r3, pc, r3
004719cc: add      r0, r0, #0xa8
004719d0: add      r2, pc, r2
004719d4: str      ip, [sp]
004719d8: bl       #0x30e004
004719dc: ldr      r3, [r4, #8]
004719e0: b        #0x471910
004719e4: str      r3, [r3]
004719e8: ldr      r3, [r0, #8]
004719ec: b        #0x471910
004719f0: subseq   r3, r2, r8, lsl #3
004719f4: strdeq   r3, r4, [r0], -r4
004719f8: andeq    r3, r0, r0, asr #19
004719fc: andeq    r1, r0, r0, asr #19
00471a00: subeq    ip, r4, r4, lsl sl
00471a04: subeq    fp, r5, r0, ror ip
00471a08: subeq    fp, r5, r0, lsl #25

# _ZN9Character7SG_LoadEi
003bc4d0: movw     r3, #0x14e8
003bc4d4: ldr      r0, [r0, r3]
003bc4d8: cmp      r0, #0
003bc4dc: bxeq     lr
003bc4e0: b        #0x465430

# _ZNK9Character9GetCharAIEv
003a3024: ldr      r3, [pc, #0x20]
003a3028: ldr      r2, [pc, #0x20]
003a302c: push     {r4, lr}
003a3030: add      r3, pc, r3
003a3034: ldr      r2, [r3, r2]
003a3038: ldr      r4, [r2]
003a303c: bl       #0x3a2fec
003a3040: mov      r3, #0x44
003a3044: mla      r0, r3, r0, r4
003a3048: pop      {r4, pc}
003a304c: subseq   r1, pc, r0, ror #20
003a3050: andeq    r0, r0, r8, asr r7

# _ZN14AnimControllerC1EP13RootSceneNodeb
00474d30: push     {r4, r5, r6, lr}
00474d34: ldr      r4, [pc, #0xe4]
00474d38: ldr      r3, [pc, #0xe4]
00474d3c: cmp      r1, #0
00474d40: add      r4, pc, r4
00474d44: ldr      r3, [r4, r3]
00474d48: sub      sp, sp, #8
00474d4c: mov      r5, r0
00474d50: add      r3, r3, #8
00474d54: str      r3, [r0]
00474d58: mov      r6, r2
00474d5c: str      r1, [r0, #4]
00474d60: beq      #0x474dc8
00474d64: ldr      r3, [r1]
00474d68: cmp      r6, #0
00474d6c: ldr      r3, [r3, #-0xc]
00474d70: add      r1, r1, r3
00474d74: ldr      r3, [r1, #4]
00474d78: add      r3, r3, #1
00474d7c: str      r3, [r1, #4]
00474d80: bne      #0x474db0
00474d84: ldr      r3, [pc, #0x9c]
00474d88: mov      r0, r5
00474d8c: mov      r2, r5
00474d90: ldr      r1, [r4, r3]
00474d94: ldr      r3, [pc, #0x90]
00474d98: str      r5, [sp]
00474d9c: ldr      r3, [r4, r3]
00474da0: bl       #0x474cac
00474da4: mov      r0, r5
00474da8: add      sp, sp, #8
00474dac: pop      {r4, r5, r6, pc}
00474db0: ldr      r3, [r5, #4]
00474db4: mov      r0, r3
00474db8: ldr      r3, [r3]
00474dbc: mov      lr, pc
00474dc0: ldr      pc, [r3, #0x74]
00474dc4: b        #0x474da4
00474dc8: ldr      r3, [pc, #0x60]
00474dcc: ldr      r3, [r4, r3]
00474dd0: ldr      r3, [r3]
00474dd4: cmp      r3, #2
00474dd8: streq    r1, [r1]
00474ddc: beq      #0x474d64
00474de0: cmp      r3, #1
00474de4: bne      #0x474d64
00474de8: ldr      r0, [pc, #0x44]
00474dec: ldr      r1, [pc, #0x44]
00474df0: ldr      r2, [pc, #0x44]
00474df4: ldr      r0, [r4, r0]
00474df8: ldr      r3, [pc, #0x40]
00474dfc: add      r1, pc, r1
00474e00: mov      ip, #0x1c
00474e04: add      r0, r0, #0xa8
00474e08: add      r2, pc, r2
00474e0c: add      r3, pc, r3
00474e10: str      ip, [sp]
00474e14: bl       #0x30e004
00474e18: ldr      r1, [r5, #4]
00474e1c: b        #0x474d64
00474e20: subseq   pc, r1, r0, asr sp
00474e24: andeq    r4, r0, r0, lsl r8
00474e28: andeq    r3, r0, r0, ror r7
00474e2c: andeq    r2, r0, r0, lsl #18
00474e30: andeq    r3, r0, r0, asr #19
00474e34: andeq    r1, r0, r0, asr #19
00474e38: ldrdeq   sb, sl, [r4], #-0x5c
00474e3c: subeq    r8, r5, r8, lsr r8
00474e40: subeq    r8, r5, ip, asr sb
