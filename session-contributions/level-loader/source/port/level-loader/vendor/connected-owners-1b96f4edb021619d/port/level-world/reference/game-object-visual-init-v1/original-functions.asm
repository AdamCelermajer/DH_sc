# _ZN10GameObject11SetPositionERK7Point3DIfEb 0x393db4 220
00393db4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00393db8: ldr      r5, [r0, #0x2e0]
00393dbc: mov      r4, r0
00393dc0: mov      r6, r1
00393dc4: cmp      r5, #0
00393dc8: mov      r7, r2
00393dcc: beq      #0x393e2c // 
00393dd0: ldr      r1, [r0, #0x164]
00393dd4: ldr      r0, [r6, #4]
00393dd8: bl       #0x30e3ac // 
00393ddc: ldr      r1, [r4, #0x168]
00393de0: mov      sl, r0
00393de4: ldr      r0, [r6, #8]
00393de8: bl       #0x30e3ac // 
00393dec: ldr      r1, [r4, #0x160]
00393df0: mov      r8, r0
00393df4: ldr      r0, [r6]
00393df8: bl       #0x30e3ac // 
00393dfc: mov      r1, r0
00393e00: ldr      r0, [r5, #0xc]
00393e04: bl       #0x30eba4 // 
00393e08: mov      r1, sl
00393e0c: str      r0, [r5, #0xc]
00393e10: ldr      r0, [r5, #0x10]
00393e14: bl       #0x30eba4 // 
00393e18: mov      r1, r8
00393e1c: str      r0, [r5, #0x10]
00393e20: ldr      r0, [r5, #0x14]
00393e24: bl       #0x30eba4 // 
00393e28: str      r0, [r5, #0x14]
00393e2c: ldr      r3, [r6]
00393e30: mov      r0, r4
00393e34: str      r3, [r4, #0x160]
00393e38: ldr      r3, [r6, #4]
00393e3c: str      r3, [r4, #0x164]
00393e40: ldr      r3, [r6, #8]
00393e44: str      r3, [r4, #0x168]
00393e48: bl       #0x38aac8 // _ZN10GameObject18UpdateAbsoluteAABBEv
00393e4c: ldr      r0, [r4, #0x2dc]
00393e50: cmp      r0, #0
00393e54: beq      #0x393e64 // 
00393e58: ldr      r1, [r4, #0x160]
00393e5c: ldr      r2, [r4, #0x164]
00393e60: bl       #0x46ea80 // _ZN14PhysicalObject11setPositionEff
00393e64: ldr      r0, [r4, #0x2d8]
00393e68: cmp      r0, #0
00393e6c: beq      #0x393e74 // 
00393e70: bl       #0x470cb8 // _ZN12VisualObject12SyncPositionEv
00393e74: cmp      r7, #0
00393e78: bne      #0x393e80 // 
00393e7c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393e80: mov      r0, r4
00393e84: mov      r1, r6
00393e88: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393e8c: b        #0x393600 // _ZN10GameObject14SetDestinationERK7Point3DIfE
# _ZN10GameObject16LoadVisualObjectEv 0x394eb0 16
00394eb0: ldr      r2, [r0, #0x2bc]
00394eb4: ldr      r1, [r0, #0x2a4]
00394eb8: mov      r3, #1
00394ebc: b        #0x394d34 // _ZN10GameObject15SetVisualObjectEPKcS1_b
# _ZN12VisualObjectC1EP10GameObjectRKSsS3_ 0x472a0c 592
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
00472af4: bl       #0x50a564 // _ZN12AssetManager15GetAssetManagerEv
00472af8: ldr      ip, [r8, #0x10]
00472afc: ldr      r2, [r8, #0x14]
00472b00: ldr      r1, [sl, #0x14]
00472b04: mov      r3, r5
00472b08: cmp      ip, r2
00472b0c: moveq    r2, r5
00472b10: mvn      ip, #0x80000000
00472b14: str      ip, [sp]
00472b18: bl       #0x50a504 // _ZN12AssetManager13loadSceneNodeEPKcS1_bi
00472b1c: cmp      r0, r5
00472b20: str      r0, [r4, #8]
00472b24: beq      #0x472c40 // 
00472b28: mov      r1, r7
00472b2c: mov      r0, r4
00472b30: bl       #0x47295c // _ZN12VisualObject9SetParentEP10GameObject
00472b34: ldr      r0, [r4, #8]
00472b38: bl       #0x35c854 // _ZN13RootSceneNode18RefreshBoundingBoxEv
00472b3c: mov      r0, r4
00472b40: bl       #0x4718f0 // _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
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
00472b74: bl       #0x350ee0 // _ZN12SceneManager13ForceRegisterEv
00472b78: ldr      r3, [r5, #0x10]
00472b7c: ldr      r2, [pc, #0xd4]
00472b80: ldr      r1, [r4, #8]
00472b84: ldr      r0, [r3, #0x1c]
00472b88: add      r2, pc, r2
00472b8c: mov      r3, #1
00472b90: bl       #0x35a0e4 // _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
00472b94: subs     r2, r0, #0
00472b98: beq      #0x472c08 // 
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
00472bcc: beq      #0x472bec // 
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
00472c0c: bl       #0x47211c // _ZN12VisualObject11CalcMeshBoxEv
00472c10: mov      r0, r4
00472c14: bl       #0x470a54 // _ZN12VisualObject12ApplyMeshBoxEv
00472c18: mov      r1, #0
00472c1c: mov      r0, #8
00472c20: bl       #0x310570 // _Znwj15MemoryHintState
00472c24: ldr      r1, [r4, #8]
00472c28: mov      r5, r0
00472c2c: mov      r2, #0
00472c30: bl       #0x474d30 // _ZN14AnimControllerC1EP13RootSceneNodeb
00472c34: mov      r0, r4
00472c38: mov      r1, r5
00472c3c: bl       #0x470a84 // _ZN12VisualObject17SetAnimControllerEP14AnimController
00472c40: mov      r0, r4
00472c44: add      sp, sp, #0xc
00472c48: pop      {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq   r2, r2, r4, ror r0
00472c50: muleq    r0, r4, lr
00472c54: strdeq   r3, r4, [r0], -r4
00472c58: subeq    sl, r5, r0, lsr #22
# _ZN10ObjectBase8InitPostEv 0x33ec0c 148
0033ec0c: push     {r4, r5, r6, r7, r8, lr}
0033ec10: mov      r8, r0
0033ec14: add      r0, r0, #0x8c
0033ec18: bl       #0x33eb28 // _ZN13ConditionData4InitEv
0033ec1c: add      r0, r8, #0xb0
0033ec20: bl       #0x33eb28 // _ZN13ConditionData4InitEv
0033ec24: ldr      r5, [r8, #0xe8]
0033ec28: ldr      r2, [r8, #0xe4]
0033ec2c: ldr      r3, [pc, #0x60]
0033ec30: cmp      r5, r2
0033ec34: add      r3, pc, r3
0033ec38: beq      #0x33ec88 // 
0033ec3c: ldr      r2, [pc, #0x54]
0033ec40: ldr      r2, [r3, r2]
0033ec44: ldr      r6, [r2]
0033ec48: cmp      r6, #0
0033ec4c: beq      #0x33ec8c // 
0033ec50: ldr      r2, [pc, #0x44]
0033ec54: mov      r4, #0
0033ec58: ldr      r3, [r3, r2]
0033ec5c: ldr      r7, [r3]
0033ec60: b        #0x33ec70 // 
0033ec64: add      r4, r4, #1
0033ec68: cmp      r4, r6
0033ec6c: beq      #0x33ec8c // 
0033ec70: ldr      r1, [r7, r4, lsl #2]
0033ec74: mov      r0, r5
0033ec78: bl       #0x30e31c // 
0033ec7c: cmp      r0, #0
0033ec80: bne      #0x33ec64 // 
0033ec84: str      r4, [r8, #0xec]
0033ec88: pop      {r4, r5, r6, r7, r8, pc}
0033ec8c: mvn      r4, #0
0033ec90: b        #0x33ec84 // 
0033ec94: rsbeq    r5, r5, ip, asr lr
0033ec98: andeq    r3, r0, r0, lsr r5
0033ec9c: andeq    r4, r0, r0, lsl #17
# _ZN12VisualObject12ApplyMeshBoxEv 0x470a54 48
00470a54: push     {r4, lr}
00470a58: ldr      r3, [r0, #4]
00470a5c: mov      r1, r0
00470a60: cmp      r3, #0
00470a64: beq      #0x470a80 // 
00470a68: mov      r0, r3
00470a6c: ldrb     r2, [r1, #0x28]
00470a70: ldr      r3, [r3]
00470a74: add      r1, r1, #0x10
00470a78: mov      lr, pc
00470a7c: ldr      pc, [r3, #0x9c]
00470a80: pop      {r4, pc}
# _ZN12VisualObject11CalcMeshBoxEv 0x47211c 1516
0047211c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120: ldr      r2, [pc, #0x5d4]
00472124: ldr      r3, [r0, #0xc]
00472128: sub      sp, sp, #0x64
0047212c: add      r2, pc, r2
00472130: cmp      r3, #0
00472134: str      r2, [sp, #8]
00472138: mov      r4, r0
0047213c: beq      #0x472408 // 
00472140: mov      r0, r3
00472144: ldr      r3, [r3]
00472148: mov      lr, pc
0047214c: ldr      pc, [r3, #0x30]
00472150: ldr      r1, [r0]
00472154: mov      r3, r0
00472158: ldr      r2, [r4, #0xc]
0047215c: str      r1, [r4, #0x10]
00472160: ldr      r1, [r0, #4]
00472164: mov      r0, r2
00472168: str      r1, [r4, #0x14]
0047216c: ldr      r3, [r3, #8]
00472170: str      r3, [r4, #0x18]
00472174: ldr      r3, [r2]
00472178: mov      lr, pc
0047217c: ldr      pc, [r3, #0x30]
00472180: ldr      r2, [r0, #0xc]
00472184: mov      r3, r0
00472188: ldr      r0, [r4, #0xc]
0047218c: str      r2, [r4, #0x1c]
00472190: ldr      r2, [r3, #0x10]
00472194: str      r2, [r4, #0x20]
00472198: ldr      r3, [r3, #0x14]
0047219c: str      r3, [r4, #0x24]
004721a0: bl       #0x597290 // _ZNK6glitch5scene10ISceneNode9getParentEv
004721a4: ldr      r3, [r0]
004721a8: mov      lr, pc
004721ac: ldr      pc, [r3, #0x90]
004721b0: mov      r3, r0
004721b4: ldr      r1, [r0]
004721b8: ldr      r0, [r4, #0x10]
004721bc: ldr      r6, [r3, #4]
004721c0: ldr      r5, [r3, #8]
004721c4: bl       #0x30ed6c // 
004721c8: mov      r1, r6
004721cc: str      r0, [r4, #0x10]
004721d0: ldr      r0, [r4, #0x14]
004721d4: bl       #0x30ed6c // 
004721d8: mov      r1, r5
004721dc: str      r0, [r4, #0x14]
004721e0: ldr      r0, [r4, #0x18]
004721e4: bl       #0x30ed6c // 
004721e8: str      r0, [r4, #0x18]
004721ec: ldr      r0, [r4, #0xc]
004721f0: bl       #0x597290 // _ZNK6glitch5scene10ISceneNode9getParentEv
004721f4: ldr      r3, [r0]
004721f8: mov      lr, pc
004721fc: ldr      pc, [r3, #0x90]
00472200: mov      r3, r0
00472204: ldr      r1, [r0]
00472208: ldr      r0, [r4, #0x1c]
0047220c: ldr      r6, [r3, #4]
00472210: ldr      r5, [r3, #8]
00472214: bl       #0x30ed6c // 
00472218: mov      r1, r6
0047221c: str      r0, [r4, #0x1c]
00472220: ldr      r0, [r4, #0x20]
00472224: bl       #0x30ed6c // 
00472228: mov      r1, r5
0047222c: str      r0, [r4, #0x20]
00472230: ldr      r0, [r4, #0x24]
00472234: bl       #0x30ed6c // 
00472238: str      r0, [r4, #0x24]
0047223c: ldr      r3, [r4, #8]
00472240: add      r5, sp, #0x10
00472244: mov      r6, #0
00472248: mov      r0, r3
0047224c: ldr      r3, [r3]
00472250: mov      lr, pc
00472254: ldr      pc, [r3, #0x40]
00472258: mov      r2, #0x41
0047225c: mov      r1, r0
00472260: mov      r0, r5
00472264: strb     r6, [sp, #0x50]
00472268: bl       #0x30e868 // 
0047226c: mov      r3, #0
00472270: mov      r1, r5
00472274: add      r0, r4, #0x10
00472278: str      r3, [sp, #0x48]
0047227c: str      r3, [sp, #0x40]
00472280: str      r3, [sp, #0x44]
00472284: strb     r6, [sp, #0x50]
00472288: bl       #0x312da8 // _ZN7Point3DIfE9transformERKN6glitch4core8CMatrix4IfEE
0047228c: mov      r1, r5
00472290: add      r0, r4, #0x1c
00472294: bl       #0x312da8 // _ZN7Point3DIfE9transformERKN6glitch4core8CMatrix4IfEE
00472298: ldr      r7, [r4, #0x1c]
0047229c: ldr      r5, [r4, #0x10]
004722a0: mov      r0, r7
004722a4: mov      r1, r5
004722a8: bl       #0x30e70c // 
004722ac: mov      r1, r5
004722b0: cmp      r0, r6
004722b4: mov      r0, r7
004722b8: movne    fp, r7
004722bc: moveq    fp, r5
004722c0: bl       #0x30e2f8 // 
004722c4: cmp      r0, #0
004722c8: ldr      r6, [r4, #0x20]
004722cc: moveq    r7, r5
004722d0: ldr      r5, [r4, #0x14]
004722d4: mov      r0, r6
004722d8: str      r7, [r4, #0x1c]
004722dc: mov      r1, r5
004722e0: str      fp, [r4, #0x10]
004722e4: bl       #0x30e70c // 
004722e8: mov      r1, r5
004722ec: cmp      r0, #0
004722f0: mov      r0, r6
004722f4: movne    sb, r6
004722f8: moveq    sb, r5
004722fc: bl       #0x30e2f8 // 
00472300: cmp      r0, #0
00472304: ldr      r8, [r4, #0x18]
00472308: moveq    r6, r5
0047230c: ldr      r5, [r4, #0x24]
00472310: mov      r1, r8
00472314: str      r6, [r4, #0x20]
00472318: mov      r0, r5
0047231c: str      sb, [r4, #0x14]
00472320: bl       #0x30e70c // 
00472324: mov      r1, r8
00472328: cmp      r0, #0
0047232c: mov      r0, r5
00472330: movne    sl, r5
00472334: moveq    sl, r8
00472338: bl       #0x30e2f8 // 
0047233c: cmp      r0, #0
00472340: moveq    r5, r8
00472344: mov      r1, fp
00472348: str      r5, [r4, #0x24]
0047234c: mov      r0, r7
00472350: str      sl, [r4, #0x18]
00472354: bl       #0x30e3ac // 
00472358: mov      r1, #0x3f000000
0047235c: bl       #0x30ed6c // 
00472360: mov      r1, sb
00472364: mov      r7, r0
00472368: mov      r0, r6
0047236c: bl       #0x30e3ac // 
00472370: mov      r1, #0x3f000000
00472374: bl       #0x30ed6c // 
00472378: mov      r1, sl
0047237c: mov      r8, r0
00472380: mov      r0, r5
00472384: bl       #0x30e3ac // 
00472388: mov      r1, #0x3f000000
0047238c: bl       #0x30ed6c // 
00472390: ldr      ip, [sp, #8]
00472394: ldr      r3, [pc, #0x364]
00472398: mov      r6, r0
0047239c: mov      r1, r7
004723a0: ldr      r5, [ip, r3]
004723a4: ldr      r0, [r5]
004723a8: bl       #0x30e3ac // 
004723ac: str      r0, [r4, #0x10]
004723b0: ldr      r1, [r5]
004723b4: mov      r0, r7
004723b8: bl       #0x30eba4 // 
004723bc: str      r0, [r4, #0x1c]
004723c0: ldr      r0, [r5, #4]
004723c4: mov      r1, r8
004723c8: bl       #0x30e3ac // 
004723cc: str      r0, [r4, #0x14]
004723d0: ldr      r1, [r5, #4]
004723d4: mov      r0, r8
004723d8: bl       #0x30eba4 // 
004723dc: str      r0, [r4, #0x20]
004723e0: ldr      r0, [r5, #8]
004723e4: mov      r1, r6
004723e8: bl       #0x30e3ac // 
004723ec: str      r0, [r4, #0x18]
004723f0: ldr      r1, [r5, #8]
004723f4: mov      r0, r6
004723f8: bl       #0x30eba4 // 
004723fc: str      r0, [r4, #0x24]
00472400: add      sp, sp, #0x64
00472404: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408: ldr      ip, [sp, #8]
0047240c: ldr      r0, [pc, #0x2f0]
00472410: mvn      r1, #0x80000000
00472414: sub      r1, r1, #0x800000
00472418: ldr      r5, [ip, r0]
0047241c: mvn      r2, #0x800000
00472420: str      r1, [r4, #0x18]
00472424: str      r1, [r4, #0x10]
00472428: str      r1, [r4, #0x14]
0047242c: str      r2, [r4, #0x24]
00472430: str      r2, [r4, #0x1c]
00472434: str      r2, [r4, #0x20]
00472438: ldr      r2, [r5, #0x10]
0047243c: str      r3, [sp, #0x5c]
00472440: str      r3, [sp, #0x54]
00472444: str      r3, [sp, #0x58]
00472448: ldr      r3, [r2, #0x1c]
0047244c: add      r6, sp, #0x54
00472450: movw     r1, #0x6164
00472454: mov      r0, r3
00472458: ldr      ip, [r3]
0047245c: movt     r1, #0x7365
00472460: ldr      r3, [r4, #8]
00472464: mov      r2, r6
00472468: mov      lr, pc
0047246c: ldr      pc, [ip, #0x20]
00472470: ldr      r0, [sp, #0x54]
00472474: ldr      r3, [sp, #0x58]
00472478: rsb      r3, r0, r3
0047247c: asrs     r3, r3, #2
00472480: str      r3, [sp, #0xc]
00472484: beq      #0x472690 // 
00472488: ldr      r2, [sp, #0xc]
0047248c: cmp      r2, #0
00472490: beq      #0x472680 // 
00472494: mov      r5, #0
00472498: b        #0x4724a0 // 
0047249c: ldr      r0, [sp, #0x54]
004724a0: ldr      r3, [r0, r5, lsl #2]
004724a4: mov      r0, r3
004724a8: ldr      r3, [r3]
004724ac: mov      lr, pc
004724b0: ldr      pc, [r3, #0x30]
004724b4: ldr      r3, [sp, #0x54]
004724b8: ldr      sl, [r0, #8]
004724bc: ldr      fp, [r0]
004724c0: ldr      r3, [r3, r5, lsl #2]
004724c4: ldr      sb, [r0, #4]
004724c8: mov      r0, r3
004724cc: ldr      r3, [r3]
004724d0: mov      lr, pc
004724d4: ldr      pc, [r3, #0x30]
004724d8: ldr      r2, [sp, #0x54]
004724dc: mov      r3, r0
004724e0: ldr      r6, [r0, #0x14]
004724e4: ldr      r8, [r0, #0xc]
004724e8: ldr      r0, [r2, r5, lsl #2]
004724ec: ldr      r7, [r3, #0x10]
004724f0: bl       #0x597290 // _ZNK6glitch5scene10ISceneNode9getParentEv
004724f4: cmp      r0, #0
004724f8: beq      #0x4725bc // 
004724fc: ldr      r3, [sp, #0x54]
00472500: ldr      r0, [r3, r5, lsl #2]
00472504: bl       #0x597290 // _ZNK6glitch5scene10ISceneNode9getParentEv
00472508: ldr      r3, [r0]
0047250c: mov      lr, pc
00472510: ldr      pc, [r3, #0x90]
00472514: mov      r2, r0
00472518: ldr      r3, [r2, #4]
0047251c: ldr      r2, [r2, #8]
00472520: ldr      r1, [r0]
00472524: mov      r0, fp
00472528: stm      sp, {r2, r3}
0047252c: bl       #0x30ed6c // 
00472530: ldr      r3, [sp, #4]
00472534: mov      fp, r0
00472538: mov      r0, sb
0047253c: mov      r1, r3
00472540: bl       #0x30ed6c // 
00472544: ldr      r2, [sp]
00472548: mov      sb, r0
0047254c: mov      r0, sl
00472550: mov      r1, r2
00472554: bl       #0x30ed6c // 
00472558: ldr      r3, [sp, #0x54]
0047255c: mov      sl, r0
00472560: ldr      r0, [r3, r5, lsl #2]
00472564: bl       #0x597290 // _ZNK6glitch5scene10ISceneNode9getParentEv
00472568: ldr      r3, [r0]
0047256c: mov      lr, pc
00472570: ldr      pc, [r3, #0x90]
00472574: mov      r2, r0
00472578: ldr      r3, [r2, #4]
0047257c: ldr      r2, [r2, #8]
00472580: ldr      r1, [r0]
00472584: mov      r0, r8
00472588: stm      sp, {r2, r3}
0047258c: bl       #0x30ed6c // 
00472590: ldr      r3, [sp, #4]
00472594: mov      r8, r0
00472598: mov      r0, r7
0047259c: mov      r1, r3
004725a0: bl       #0x30ed6c // 
004725a4: ldr      r2, [sp]
004725a8: mov      r7, r0
004725ac: mov      r0, r6
004725b0: mov      r1, r2
004725b4: bl       #0x30ed6c // 
004725b8: mov      r6, r0
004725bc: ldr      r3, [r4, #0x10]
004725c0: mov      r1, fp
004725c4: add      r5, r5, #1
004725c8: mov      r0, r3
004725cc: str      r3, [sp, #4]
004725d0: bl       #0x30e2f8 // 
004725d4: cmp      r0, #0
004725d8: ldr      r3, [sp, #4]
004725dc: movne    r3, fp
004725e0: ldr      fp, [r4, #0x14]
004725e4: str      r3, [r4, #0x10]
004725e8: mov      r1, sb
004725ec: mov      r0, fp
004725f0: bl       #0x30e2f8 // 
004725f4: cmp      r0, #0
004725f8: movne    fp, sb
004725fc: ldr      sb, [r4, #0x18]
00472600: mov      r1, sl
00472604: str      fp, [r4, #0x14]
00472608: mov      r0, sb
0047260c: bl       #0x30e2f8 // 
00472610: cmp      r0, #0
00472614: movne    sb, sl
00472618: ldr      sl, [r4, #0x1c]
0047261c: mov      r1, r8
00472620: str      sb, [r4, #0x18]
00472624: mov      r0, sl
00472628: bl       #0x30e70c // 
0047262c: cmp      r0, #0
00472630: movne    sl, r8
00472634: ldr      r8, [r4, #0x20]
00472638: mov      r1, r7
0047263c: str      sl, [r4, #0x1c]
00472640: mov      r0, r8
00472644: bl       #0x30e70c // 
00472648: cmp      r0, #0
0047264c: movne    r8, r7
00472650: ldr      r7, [r4, #0x24]
00472654: str      r8, [r4, #0x20]
00472658: mov      r1, r6
0047265c: mov      r0, r7
00472660: bl       #0x30e70c // 
00472664: ldr      r3, [sp, #0xc]
00472668: cmp      r0, #0
0047266c: movne    r7, r6
00472670: cmp      r5, r3
00472674: str      r7, [r4, #0x24]
00472678: bne      #0x47249c // 
0047267c: ldr      r0, [sp, #0x54]
00472680: cmp      r0, #0
00472684: beq      #0x47223c // 
00472688: bl       #0x310450 // _Z10GlitchFreePv
0047268c: b        #0x47223c // 
00472690: ldr      r3, [r5, #0x10]
00472694: movw     r1, #0x6164
00472698: movt     r1, #0x6d65
0047269c: ldr      ip, [r3, #0x1c]
004726a0: mov      r2, r6
004726a4: ldr      r3, [r4, #8]
004726a8: mov      r0, ip
004726ac: ldr      ip, [ip]
004726b0: mov      lr, pc
004726b4: ldr      pc, [ip, #0x20]
004726b8: ldr      r0, [sp, #0x54]
004726bc: ldr      r3, [sp, #0x58]
004726c0: rsb      r3, r0, r3
004726c4: asrs     r3, r3, #2
004726c8: str      r3, [sp, #0xc]
004726cc: bne      #0x472488 // 
004726d0: mov      r3, #0
004726d4: cmp      r0, #0
004726d8: str      r3, [r4, #0x24]
004726dc: str      r3, [r4, #0x10]
004726e0: str      r3, [r4, #0x14]
004726e4: str      r3, [r4, #0x18]
004726e8: str      r3, [r4, #0x1c]
004726ec: str      r3, [r4, #0x20]
004726f0: beq      #0x472400 // 
004726f4: bl       #0x310450 // _Z10GlitchFreePv
004726f8: b        #0x472400 // 
004726fc: subseq   r2, r2, r4, ror #18
00472700: andeq    r3, r0, ip, lsr #30
00472704: strdeq   r3, r4, [r0], -r4
# _ZN12VisualObject9SetParentEP10GameObject 0x47295c 176
0047295c: push     {r4, r5, r6, lr}
00472960: ldr      r4, [pc, #0x9c]
00472964: cmp      r1, #0
00472968: str      r1, [r0, #4]
0047296c: mov      r5, r0
00472970: add      r4, pc, r4
00472974: beq      #0x4729c8 // 
00472978: ldrb     r3, [r1, #0x84]
0047297c: cmp      r3, #0
00472980: bne      #0x4729a4 // 
00472984: mov      r0, r5
00472988: bl       #0x38ba74 // _ZN12VisualObject4SyncEv
0047298c: ldr      r3, [pc, #0x74]
00472990: ldr      r0, [r5, #8]
00472994: mov      r1, #1
00472998: ldr      r2, [r4, r3]
0047299c: pop      {r4, r5, r6, lr}
004729a0: b        #0x50e484 // _Z22RecursiveSetBoolOnNodePN6glitch5scene10ISceneNodeEbPFvS2_bE
004729a4: mov      r0, r1
004729a8: ldr      r3, [r1]
004729ac: mov      lr, pc
004729b0: ldr      pc, [r3, #0x80]
004729b4: cmp      r0, #0
004729b8: beq      #0x4729cc // 
004729bc: ldr      r3, [r5, #4]
004729c0: cmp      r3, #0
004729c4: bne      #0x472984 // 
004729c8: pop      {r4, r5, r6, pc}
004729cc: mov      r0, r5
004729d0: bl       #0x38ba74 // _ZN12VisualObject4SyncEv
004729d4: ldr      r6, [r5, #8]
004729d8: ldr      r3, [r6]
004729dc: mov      r0, r6
004729e0: ldr      r4, [r3, #0xa4]
004729e4: mov      lr, pc
004729e8: ldr      pc, [r3, #0xa0]
004729ec: mov      r1, r0
004729f0: mov      r0, r6
004729f4: blx      r4
004729f8: ldr      r0, [r5, #8]
004729fc: pop      {r4, r5, r6, lr}
00472a00: b        #0x50f220 // _Z14OptimizeStaticPN6glitch5scene10ISceneNodeE
00472a04: subseq   r2, r2, r0, lsr #2
00472a08: andeq    r3, r0, r4, ror r0
# _ZN12VisualObject4SyncEv 0x38ba74 32
0038ba74: push     {r4, lr}
0038ba78: mov      r4, r0
0038ba7c: bl       #0x470cb8 // _ZN12VisualObject12SyncPositionEv
0038ba80: mov      r0, r4
0038ba84: bl       #0x472948 // _ZN12VisualObject12SyncRotationEv
0038ba88: mov      r0, r4
0038ba8c: pop      {r4, lr}
0038ba90: b        #0x472860 // _ZN12VisualObject11SyncScalingEv
# _ZN10GameObject15SetRelativeAABBERK4aabbIfEb 0x38b110 280
0038b110: push     {r4, r5, r6, r7, r8, lr}
0038b114: ldr      r5, [r1]
0038b118: mov      r3, r1
0038b11c: mov      r4, r0
0038b120: str      r5, [r0, #0x144]
0038b124: ldr      r7, [r1, #4]
0038b128: mov      r1, r5
0038b12c: str      r7, [r0, #0x148]
0038b130: ldr      r2, [r3, #8]
0038b134: str      r2, [r0, #0x14c]
0038b138: ldr      r0, [r3, #0xc]
0038b13c: str      r0, [r4, #0x150]
0038b140: ldr      r6, [r3, #0x10]
0038b144: str      r6, [r4, #0x154]
0038b148: ldr      r3, [r3, #0x14]
0038b14c: str      r3, [r4, #0x158]
0038b150: bl       #0x30e3ac // 
0038b154: mov      r1, #0
0038b158: mov      r8, r0
0038b15c: bl       #0x30df8c // 
0038b160: cmp      r0, #0
0038b164: beq      #0x38b188 // 
0038b168: mov      r1, r7
0038b16c: mov      r0, r6
0038b170: bl       #0x30e3ac // 
0038b174: mov      r1, #0
0038b178: bl       #0x30df8c // 
0038b17c: cmp      r0, #0
0038b180: movne    r3, #1
0038b184: strbne   r3, [r4, #0x2f9]
0038b188: mov      r1, #0x41000000
0038b18c: mov      r0, r8
0038b190: add      r1, r1, #0x200000
0038b194: bl       #0x30e70c // 
0038b198: cmp      r0, #0
0038b19c: beq      #0x38b1c8 // 
0038b1a0: mov      r1, #0x40000000
0038b1a4: add      r1, r1, #0xa00000
0038b1a8: mov      r0, r5
0038b1ac: bl       #0x30e3ac // 
0038b1b0: mov      r1, #0x40000000
0038b1b4: str      r0, [r4, #0x144]
0038b1b8: add      r1, r1, #0xa00000
0038b1bc: ldr      r0, [r4, #0x150]
0038b1c0: bl       #0x30eba4 // 
0038b1c4: str      r0, [r4, #0x150]
0038b1c8: ldr      r5, [r4, #0x148]
0038b1cc: ldr      r0, [r4, #0x154]
0038b1d0: mov      r1, r5
0038b1d4: bl       #0x30e3ac // 
0038b1d8: mov      r1, #0x41000000
0038b1dc: add      r1, r1, #0x200000
0038b1e0: bl       #0x30e70c // 
0038b1e4: cmp      r0, #0
0038b1e8: beq      #0x38b214 // 
0038b1ec: mov      r1, #0x40000000
0038b1f0: add      r1, r1, #0xa00000
0038b1f4: mov      r0, r5
0038b1f8: bl       #0x30e3ac // 
0038b1fc: mov      r1, #0x40000000
0038b200: str      r0, [r4, #0x148]
0038b204: add      r1, r1, #0xa00000
0038b208: ldr      r0, [r4, #0x154]
0038b20c: bl       #0x30eba4 // 
0038b210: str      r0, [r4, #0x154]
0038b214: mov      r0, r4
0038b218: bl       #0x38aac8 // _ZN10GameObject18UpdateAbsoluteAABBEv
0038b21c: mov      r0, r4
0038b220: pop      {r4, r5, r6, r7, r8, lr}
0038b224: b        #0x393ea0 // _ZN10GameObject14UpdatePFObjectEv
# _ZN10GameObject18UpdateAbsoluteAABBEv 0x38aac8 152
0038aac8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov      r4, r0
0038aad0: ldr      sl, [r4, #0x148]
0038aad4: ldr      r0, [r0, #0x144]
0038aad8: ldr      r8, [r4, #0x14c]
0038aadc: ldr      r7, [r4, #0x150]
0038aae0: ldr      r6, [r4, #0x154]
0038aae4: ldr      r5, [r4, #0x158]
0038aae8: ldr      r1, [r4, #0x160]
0038aaec: str      r0, [r4, #0x12c]
0038aaf0: str      sl, [r4, #0x130]
0038aaf4: str      r8, [r4, #0x134]
0038aaf8: str      r7, [r4, #0x138]
0038aafc: str      r6, [r4, #0x13c]
0038ab00: str      r5, [r4, #0x140]
0038ab04: bl       #0x30eba4 // 
0038ab08: ldr      r1, [r4, #0x164]
0038ab0c: str      r0, [r4, #0x12c]
0038ab10: mov      r0, sl
0038ab14: bl       #0x30eba4 // 
0038ab18: ldr      r1, [r4, #0x168]
0038ab1c: str      r0, [r4, #0x130]
0038ab20: mov      r0, r8
0038ab24: bl       #0x30eba4 // 
0038ab28: ldr      r1, [r4, #0x160]
0038ab2c: str      r0, [r4, #0x134]
0038ab30: mov      r0, r7
0038ab34: bl       #0x30eba4 // 
0038ab38: ldr      r1, [r4, #0x164]
0038ab3c: str      r0, [r4, #0x138]
0038ab40: mov      r0, r6
0038ab44: bl       #0x30eba4 // 
0038ab48: ldr      r1, [r4, #0x168]
0038ab4c: str      r0, [r4, #0x13c]
0038ab50: mov      r0, r5
0038ab54: bl       #0x30eba4 // 
0038ab58: str      r0, [r4, #0x140]
0038ab5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
# _ZN10GameObject9InitFinalEv 0x38cd48 408
0038cd48: push     {r4, r5, r6, r7, r8, lr}
0038cd4c: ldr      r5, [pc, #0x178]
0038cd50: ldr      r6, [pc, #0x178]
0038cd54: sub      sp, sp, #0x28
0038cd58: add      r5, pc, r5
0038cd5c: ldr      r3, [r5, r6]
0038cd60: mov      r4, r0
0038cd64: ldr      r3, [r3]
0038cd68: str      r3, [sp, #0x24]
0038cd6c: bl       #0x38bd64 // _ZN10GameObject21CheckSpawnProbabilityEv
0038cd70: ldr      r3, [r4, #0x274]
0038cd74: cmp      r0, r3
0038cd78: bge      #0x38ce94 // 
0038cd7c: ldrb     r3, [r4, #0x81]
0038cd80: cmp      r3, #0
0038cd84: bne      #0x38ce94 // 
0038cd88: ldrb     r3, [r4, #0x2ed]
0038cd8c: cmp      r3, #0
0038cd90: bne      #0x38ceb0 // 
0038cd94: ldr      r1, [r4, #0x144]
0038cd98: ldr      r0, [r4, #0x150]
0038cd9c: bl       #0x30e3ac // 
0038cda0: ldr      r1, [r4, #0x148]
0038cda4: mov      r7, r0
0038cda8: ldr      r0, [r4, #0x154]
0038cdac: bl       #0x30e3ac // 
0038cdb0: mov      r8, r0
0038cdb4: mov      r1, r8
0038cdb8: mov      r0, r7
0038cdbc: bl       #0x30e70c // 
0038cdc0: cmp      r0, #0
0038cdc4: ldr      r0, [pc, #0x108]
0038cdc8: movne    r7, r8
0038cdcc: ldrb     r2, [r4, #0x84]
0038cdd0: add      r1, r4, #0x1c8
0038cdd4: add      r3, r4, #0x160
0038cdd8: ldr      r0, [r5, r0]
0038cddc: str      r7, [sp]
0038cde0: str      r4, [sp, #4]
0038cde4: bl       #0x526b18 // _ZN7PFWorld10InitObjectER8PFObjectbRK7Point3DIfEfPv
0038cde8: ldr      r7, [r4, #0x44]
0038cdec: mov      r0, r7
0038cdf0: bl       #0x30de54 // 
0038cdf4: mov      r1, r7
0038cdf8: add      r2, r7, r0
0038cdfc: add      r0, r4, #0x254
0038ce00: bl       #0x3109e0 // _ZNSs9_M_assignEPKcS0_
0038ce04: ldr      r0, [r4, #0x2d8]
0038ce08: cmp      r0, #0
0038ce0c: beq      #0x38ce8c // 
0038ce10: bl       #0x38ba74 // _ZN12VisualObject4SyncEv
0038ce14: ldr      r3, [pc, #0xbc]
0038ce18: add      r7, sp, #0xc
0038ce1c: ldr      r2, [r4, #0x288]
0038ce20: ldr      r3, [r5, r3]
0038ce24: ldr      r1, [r4, #0x28c]
0038ce28: mov      r0, r7
0038ce2c: ldr      r3, [r3, #0x10]
0038ce30: ldr      r8, [r3, #0x1c]
0038ce34: str      r7, [sp, #0x1c]
0038ce38: str      r7, [sp, #0x20]
0038ce3c: add      r8, r8, #0x294
0038ce40: bl       #0x3116e8 // _ZNSs19_M_range_initializeEPKcS0_
0038ce44: mov      r0, r8
0038ce48: mov      r1, r7
0038ce4c: bl       #0x40c3cc // _ZN15LightSetManager21GetLightSetIdFromNameESs
0038ce50: mov      r8, r0
0038ce54: mov      r0, r7
0038ce58: bl       #0x318254 // _ZNSsD1Ev
0038ce5c: ldr      r3, [r4, #0x2d8]
0038ce60: str      r8, [r3, #0x40]
0038ce64: ldr      r3, [r4, #0x2d8]
0038ce68: cmp      r3, #0
0038ce6c: beq      #0x38ce8c // 
0038ce70: ldr      r0, [r3, #8]
0038ce74: cmp      r0, #0
0038ce78: beq      #0x38ce8c // 
0038ce7c: ldr      r1, [pc, #0x58]
0038ce80: add      r1, pc, r1
0038ce84: bl       #0x5984f4 // _ZN6glitch5scene10ISceneNode20getSceneNodeFromNameEPKc
0038ce88: str      r0, [r4, #0x180]
0038ce8c: mov      r0, r4
0038ce90: bl       #0x393ea0 // _ZN10GameObject14UpdatePFObjectEv
0038ce94: ldr      r3, [r5, r6]
0038ce98: ldr      r2, [sp, #0x24]
0038ce9c: ldr      r3, [r3]
0038cea0: cmp      r2, r3
0038cea4: bne      #0x38cec8 // 
0038cea8: add      sp, sp, #0x28
0038ceac: pop      {r4, r5, r6, r7, r8, pc}
0038ceb0: ldr      r3, [r4]
0038ceb4: mov      r0, r4
0038ceb8: mov      r1, #1
0038cebc: mov      lr, pc
0038cec0: ldr      pc, [r3, #0x40]
0038cec4: b        #0x38cd94 // 
0038cec8: bl       #0x30e310 // 
0038cecc: rsbeq    r7, r0, r8, lsr sp
0038ced0: andeq    r4, r0, ip, lsr #1
0038ced4: andeq    r1, r0, r4, lsl #4
0038ced8: strdeq   r3, r4, [r0], -r4
0038cedc: ldrsheq  r5, [r3], #-0x58
# _ZN10GameObject15SetVisualObjectEP12VisualObject 0x394338 64
00394338: push     {r4, r5, r6, lr}
0039433c: ldr      r3, [r0, #0x2d8]
00394340: mov      r4, r0
00394344: mov      r5, r1
00394348: cmp      r3, r1
0039434c: beq      #0x394374 // 
00394350: cmp      r3, #0
00394354: beq      #0x394370 // 
00394358: mov      r0, r3
0039435c: ldr      r3, [r3]
00394360: mov      lr, pc
00394364: ldr      pc, [r3, #4]
00394368: mov      r3, #0
0039436c: str      r3, [r4, #0x2d8]
00394370: str      r5, [r4, #0x2d8]
00394374: pop      {r4, r5, r6, pc}
# _ZN10GameObject15SetVisualObjectEPKcS1_b 0x394d34 380
00394d34: cmp      r3, #0
00394d38: push     {r4, r5, r6, r7, r8, lr}
00394d3c: mov      r4, r0
00394d40: mov      r5, r1
00394d44: mov      r6, r2
00394d48: beq      #0x394dec // 
00394d4c: cmp      r1, #0
00394d50: add      r8, r0, #0x290
00394d54: beq      #0x394e6c // 
00394d58: mov      r0, r5
00394d5c: bl       #0x30de54 // 
00394d60: mov      r1, r5
00394d64: add      r2, r5, r0
00394d68: mov      r0, r8
00394d6c: bl       #0x3109e0 // _ZNSs9_M_assignEPKcS0_
00394d70: cmp      r6, #0
00394d74: cmpne    r5, #0
00394d78: add      r7, r4, #0x2a8
00394d7c: beq      #0x394e28 // 
00394d80: mov      r0, r6
00394d84: bl       #0x30de54 // 
00394d88: add      r2, r6, r0
00394d8c: mov      r1, r6
00394d90: mov      r0, r7
00394d94: bl       #0x3109e0 // _ZNSs9_M_assignEPKcS0_
00394d98: ldr      r2, [r4, #0x2a0]
00394d9c: ldr      r3, [r4, #0x2a4]
00394da0: cmp      r2, r3
00394da4: beq      #0x394e50 // 
00394da8: mov      r1, #0
00394dac: mov      r0, #0xac
00394db0: bl       #0x310570 // _Znwj15MemoryHintState
00394db4: mov      r3, r7
00394db8: mov      r5, r0
00394dbc: mov      r1, r4
00394dc0: mov      r2, r8
00394dc4: bl       #0x472a0c // _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00394dc8: ldr      r3, [r5, #8]
00394dcc: cmp      r3, #0
00394dd0: beq      #0x394e94 // 
00394dd4: mov      r0, r4
00394dd8: mov      r1, r5
00394ddc: bl       #0x394338 // _ZN10GameObject15SetVisualObjectEP12VisualObject
00394de0: ldr      r3, [r5, #8]
00394de4: str      r4, [r3, #0x204]
00394de8: pop      {r4, r5, r6, r7, r8, pc}
00394dec: cmp      r1, #0
00394df0: beq      #0x394e68 // 
00394df4: mov      r0, r1
00394df8: ldr      r1, [r4, #0x2a4]
00394dfc: bl       #0x30e31c // 
00394e00: cmp      r0, #0
00394e04: bne      #0x394e60 // 
00394e08: cmp      r6, #0
00394e0c: beq      #0x394e24 // 
00394e10: mov      r0, r6
00394e14: ldr      r1, [r4, #0x2bc]
00394e18: bl       #0x30e31c // 
00394e1c: cmp      r0, #0
00394e20: bne      #0x394e60 // 
00394e24: pop      {r4, r5, r6, r7, r8, pc}
00394e28: ldr      r6, [pc, #0x78]
00394e2c: mov      r0, r7
00394e30: add      r6, pc, r6
00394e34: mov      r2, r6
00394e38: mov      r1, r6
00394e3c: bl       #0x3109e0 // _ZNSs9_M_assignEPKcS0_
00394e40: ldr      r2, [r4, #0x2a0]
00394e44: ldr      r3, [r4, #0x2a4]
00394e48: cmp      r2, r3
00394e4c: bne      #0x394da8 // 
00394e50: mov      r0, r4
00394e54: mov      r1, #0
00394e58: pop      {r4, r5, r6, r7, r8, lr}
00394e5c: b        #0x394338 // _ZN10GameObject15SetVisualObjectEP12VisualObject
00394e60: add      r8, r4, #0x290
00394e64: b        #0x394d58 // 
00394e68: add      r8, r0, #0x290
00394e6c: ldr      r5, [pc, #0x38]
00394e70: mov      r0, r8
00394e74: add      r7, r4, #0x2a8
00394e78: add      r5, pc, r5
00394e7c: mov      r2, r5
00394e80: mov      r1, r5
00394e84: bl       #0x3109e0 // _ZNSs9_M_assignEPKcS0_
00394e88: mov      r6, r5
00394e8c: mov      r2, r5
00394e90: b        #0x394d8c // 
00394e94: mov      r0, r5
00394e98: ldr      r3, [r5]
00394e9c: mov      lr, pc
00394ea0: ldr      pc, [r3, #4]
00394ea4: pop      {r4, r5, r6, r7, r8, pc}
00394ea8: ldrsbeq  r6, [r3], #-0x98
# _ZN14AnimControllerC1EP13RootSceneNodeb 0x474d30 276
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
00474d60: beq      #0x474dc8 // 
00474d64: ldr      r3, [r1]
00474d68: cmp      r6, #0
00474d6c: ldr      r3, [r3, #-0xc]
00474d70: add      r1, r1, r3
00474d74: ldr      r3, [r1, #4]
00474d78: add      r3, r3, #1
00474d7c: str      r3, [r1, #4]
00474d80: bne      #0x474db0 // 
00474d84: ldr      r3, [pc, #0x9c]
00474d88: mov      r0, r5
00474d8c: mov      r2, r5
00474d90: ldr      r1, [r4, r3]
00474d94: ldr      r3, [pc, #0x90]
00474d98: str      r5, [sp]
00474d9c: ldr      r3, [r4, r3]
00474da0: bl       #0x474cac // _ZN14AnimController17SetCallbacksOnAllEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474da4: mov      r0, r5
00474da8: add      sp, sp, #8
00474dac: pop      {r4, r5, r6, pc}
00474db0: ldr      r3, [r5, #4]
00474db4: mov      r0, r3
00474db8: ldr      r3, [r3]
00474dbc: mov      lr, pc
00474dc0: ldr      pc, [r3, #0x74]
00474dc4: b        #0x474da4 // 
00474dc8: ldr      r3, [pc, #0x60]
00474dcc: ldr      r3, [r4, r3]
00474dd0: ldr      r3, [r3]
00474dd4: cmp      r3, #2
00474dd8: streq    r1, [r1]
00474ddc: beq      #0x474d64 // 
00474de0: cmp      r3, #1
00474de4: bne      #0x474d64 // 
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
00474e14: bl       #0x30e004 // 
00474e18: ldr      r1, [r5, #4]
00474e1c: b        #0x474d64 // 
00474e20: subseq   pc, r1, r0, asr sp
00474e24: andeq    r4, r0, r0, lsl r8
00474e28: andeq    r3, r0, r0, ror r7
00474e2c: andeq    r2, r0, r0, lsl #18
00474e30: andeq    r3, r0, r0, asr #19
00474e34: andeq    r1, r0, r0, asr #19
00474e38: ldrdeq   sb, sl, [r4], #-0x5c
00474e3c: subeq    r8, r5, r8, lsr r8
00474e40: subeq    r8, r5, ip, asr sb
# _ZN10GameObject8InitPostEv 0x38be5c 560
0038be5c: push     {r4, r5, r6, r7, r8, lr}
0038be60: mov      r4, r0
0038be64: bl       #0x33ec0c // _ZN10ObjectBase8InitPostEv
0038be68: mov      r0, r4
0038be6c: bl       #0x38bd64 // _ZN10GameObject21CheckSpawnProbabilityEv
0038be70: ldr      r3, [r4, #0x274]
0038be74: ldr      r5, [pc, #0x204]
0038be78: cmp      r0, r3
0038be7c: add      r5, pc, r5
0038be80: bge      #0x38c02c // 
0038be84: ldr      r0, [r4, #0x120]
0038be88: mov      r3, #0
0038be8c: movw     r1, #0xb717
0038be90: str      r3, [r4, #0x2dc]
0038be94: movt     r1, #0x38d1
0038be98: bic      r0, r0, #0x80000000
0038be9c: bl       #0x30e70c // 
0038bea0: cmp      r0, #0
0038bea4: ldr      r0, [r4, #0x124]
0038bea8: movne    r3, #0x3f800000
0038beac: movw     r1, #0xb717
0038beb0: strne    r3, [r4, #0x120]
0038beb4: movt     r1, #0x38d1
0038beb8: bic      r0, r0, #0x80000000
0038bebc: bl       #0x30e70c // 
0038bec0: cmp      r0, #0
0038bec4: ldr      r0, [r4, #0x128]
0038bec8: movne    r3, #0x3f800000
0038becc: movw     r1, #0xb717
0038bed0: strne    r3, [r4, #0x124]
0038bed4: movt     r1, #0x38d1
0038bed8: bic      r0, r0, #0x80000000
0038bedc: bl       #0x30e70c // 
0038bee0: cmp      r0, #0
0038bee4: movne    r3, #0x3f800000
0038bee8: movw     r1, #0xfa35
0038beec: strne    r3, [r4, #0x128]
0038bef0: ldr      r0, [r4, #0x16c]
0038bef4: movt     r1, #0x3c8e
0038bef8: bl       #0x30ed6c // 
0038befc: movw     r1, #0xfa35
0038bf00: str      r0, [r4, #0x16c]
0038bf04: movt     r1, #0x3c8e
0038bf08: ldr      r0, [r4, #0x170]
0038bf0c: bl       #0x30ed6c // 
0038bf10: movw     r1, #0xfa35
0038bf14: str      r0, [r4, #0x170]
0038bf18: movt     r1, #0x3c8e
0038bf1c: ldr      r0, [r4, #0x174]
0038bf20: bl       #0x30ed6c // 
0038bf24: mov      r2, #1
0038bf28: str      r0, [r4, #0x178]
0038bf2c: str      r0, [r4, #0x174]
0038bf30: add      r1, r4, #0x160
0038bf34: mov      r0, r4
0038bf38: bl       #0x393db4 // _ZN10GameObject11SetPositionERK7Point3DIfEb
0038bf3c: ldr      r0, [r4, #0x144]
0038bf40: ldr      r1, [r4, #0x120]
0038bf44: bl       #0x30ed6c // 
0038bf48: ldr      r1, [r4, #0x124]
0038bf4c: str      r0, [r4, #0x144]
0038bf50: ldr      r0, [r4, #0x148]
0038bf54: bl       #0x30ed6c // 
0038bf58: ldr      r1, [r4, #0x128]
0038bf5c: str      r0, [r4, #0x148]
0038bf60: ldr      r0, [r4, #0x14c]
0038bf64: bl       #0x30ed6c // 
0038bf68: ldr      r1, [r4, #0x120]
0038bf6c: str      r0, [r4, #0x14c]
0038bf70: ldr      r0, [r4, #0x150]
0038bf74: bl       #0x30ed6c // 
0038bf78: ldr      r1, [r4, #0x124]
0038bf7c: str      r0, [r4, #0x150]
0038bf80: ldr      r0, [r4, #0x154]
0038bf84: bl       #0x30ed6c // 
0038bf88: ldr      r1, [r4, #0x128]
0038bf8c: str      r0, [r4, #0x154]
0038bf90: ldr      r0, [r4, #0x158]
0038bf94: bl       #0x30ed6c // 
0038bf98: str      r0, [r4, #0x158]
0038bf9c: mov      r0, r4
0038bfa0: bl       #0x38aac8 // _ZN10GameObject18UpdateAbsoluteAABBEv
0038bfa4: ldr      r0, [r4, #0x2d8]
0038bfa8: cmp      r0, #0
0038bfac: beq      #0x38c040 // 
0038bfb0: bl       #0x38ba74 // _ZN12VisualObject4SyncEv
0038bfb4: ldrb     r3, [r4, #0x15c]
0038bfb8: ldr      r7, [r4, #0x36c]
0038bfbc: cmp      r3, #0
0038bfc0: movne    r3, #0
0038bfc4: strbne   r3, [r4, #0x28]
0038bfc8: ldr      r3, [r4, #0x368]
0038bfcc: cmp      r3, r7
0038bfd0: beq      #0x38c02c // 
0038bfd4: ldr      r3, [pc, #0xa8]
0038bfd8: ldr      r3, [r5, r3]
0038bfdc: ldr      r8, [r3]
0038bfe0: cmp      r8, #0
0038bfe4: beq      #0x38c030 // 
0038bfe8: ldr      r3, [pc, #0x98]
0038bfec: mov      r6, #0
0038bff0: ldr      r3, [r5, r3]
0038bff4: ldr      r5, [r3]
0038bff8: b        #0x38c008 // 
0038bffc: add      r6, r6, #1
0038c000: cmp      r6, r8
0038c004: beq      #0x38c030 // 
0038c008: ldr      r1, [r5, r6, lsl #2]
0038c00c: mov      r0, r7
0038c010: bl       #0x30e31c // 
0038c014: cmp      r0, #0
0038c018: bne      #0x38bffc // 
0038c01c: uxth     r6, r6
0038c020: mov      r3, #0x370
0038c024: strh     r6, [r4, r3]
0038c028: pop      {r4, r5, r6, r7, r8, pc}
0038c02c: pop      {r4, r5, r6, r7, r8, pc}
0038c030: movw     r6, #0xffff
0038c034: mov      r3, #0x370
0038c038: strh     r6, [r4, r3]
0038c03c: pop      {r4, r5, r6, r7, r8, pc}
0038c040: bl       #0x38174c // _ZN6Device17IsHighPerformanceEv
0038c044: cmp      r0, #0
0038c048: bne      #0x38c068 // 
0038c04c: ldrb     r3, [r4, #0x60]
0038c050: cmp      r3, #0
0038c054: beq      #0x38c068 // 
0038c058: ldrb     r3, [r4, #0x10c]
0038c05c: cmp      r3, #0
0038c060: strne    r0, [r4, #0x2d8]
0038c064: bne      #0x38bfb4 // 
0038c068: mov      r0, r4
0038c06c: bl       #0x394eb0 // _ZN10GameObject16LoadVisualObjectEv
0038c070: ldr      r0, [r4, #0x2d8]
0038c074: cmp      r0, #0
0038c078: beq      #0x38bfb4 // 
0038c07c: b        #0x38bfb0 // 
0038c080: rsbeq    r8, r0, r4, lsl ip
0038c084: andeq    r3, r0, r8, lsr sp
0038c088: andeq    r3, r0, r8, lsr #19
