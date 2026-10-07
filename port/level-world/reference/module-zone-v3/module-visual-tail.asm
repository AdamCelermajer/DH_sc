
# _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
004718f0: push {r4, r5, r6, lr}
004718f4: ldr r3, [r0, #8]
004718f8: ldr r5, [pc, #0xf0]
004718fc: sub sp, sp, #0x18
00471900: cmp r3, #0
00471904: mov r4, r0
00471908: add r5, pc, r5
0047190c: beq #0x471990
00471910: ldr r2, [pc, #0xdc]
00471914: mov r6, #0
00471918: str r6, [sp, #0xc]
0047191c: ldr r2, [r5, r2]
00471920: str r6, [sp, #0x10]
00471924: str r6, [sp, #0x14]
00471928: ldr r2, [r2, #0x10]
0047192c: movw r1, #0x6164
00471930: movt r1, #0x4d65
00471934: ldr ip, [r2, #0x1c]
00471938: add r2, sp, #0xc
0047193c: mov r0, ip
00471940: ldr ip, [ip]
00471944: mov lr, pc
00471948: ldr pc, [ip, #0x20]
0047194c: ldr r0, [sp, #0xc]
00471950: ldr r1, [sp, #0x10]
00471954: rsb r1, r0, r1
00471958: asrs r1, r1, #2
0047195c: beq #0x47197c
00471960: mov r2, #1
00471964: ldr r3, [r0, r6, lsl #2]
00471968: add r6, r6, #1
0047196c: cmp r6, r1
00471970: str r3, [r4, #0x2c]
00471974: strb r2, [r4, #0x7f]
00471978: blo #0x471964
0047197c: cmp r0, #0
00471980: beq #0x471988
00471984: bl #0x310450
00471988: add sp, sp, #0x18
0047198c: pop {r4, r5, r6, pc}
00471990: ldr r2, [pc, #0x60]
00471994: ldr r2, [r5, r2]
00471998: ldr r2, [r2]
0047199c: cmp r2, #2
004719a0: beq #0x4719e4
004719a4: cmp r2, #1
004719a8: bne #0x471910
004719ac: ldr r0, [pc, #0x48]
004719b0: ldr r1, [pc, #0x48]
004719b4: ldr r2, [pc, #0x48]
004719b8: ldr r0, [r5, r0]
004719bc: ldr r3, [pc, #0x44]
004719c0: movw ip, #0x3ff
004719c4: add r1, pc, r1
004719c8: add r3, pc, r3
004719cc: add r0, r0, #0xa8
004719d0: add r2, pc, r2
004719d4: str ip, [sp]
004719d8: bl #0x30e004
004719dc: ldr r3, [r4, #8]
004719e0: b #0x471910
004719e4: str r3, [r3]
004719e8: ldr r3, [r0, #8]
004719ec: b #0x471910
004719f0: subseq r3, r2, r8, lsl #3
004719f4: strdeq r3, r4, [r0], -r4
004719f8: andeq r3, r0, r0, asr #19
004719fc: andeq r1, r0, r0, asr #19
00471a00: subeq ip, r4, r4, lsl sl
00471a04: subeq fp, r5, r0, ror ip
00471a08: subeq fp, r5, r0, lsl #25

# _ZN12SceneManager13ForceRegisterEv
00350ee0: mov r3, #1
00350ee4: strb r3, [r0, #0x448]
00350ee8: strb r3, [r0, #0x289]
00350eec: bx lr

# _ZN12VisualObject12ApplyMeshBoxEv
00470a54: push {r4, lr}
00470a58: ldr r3, [r0, #4]
00470a5c: mov r1, r0
00470a60: cmp r3, #0
00470a64: beq #0x470a80
00470a68: mov r0, r3
00470a6c: ldrb r2, [r1, #0x28]
00470a70: ldr r3, [r3]
00470a74: add r1, r1, #0x10
00470a78: mov lr, pc
00470a7c: ldr pc, [r3, #0x9c]
00470a80: pop {r4, pc}

# _ZN12VisualObject17SetAnimControllerEP14AnimController
00470a84: push {r4, r5, r6, lr}
00470a88: ldr r3, [r0, #0x38]
00470a8c: mov r4, r0
00470a90: mov r5, r1
00470a94: cmp r3, r1
00470a98: beq #0x470ac0
00470a9c: cmp r3, #0
00470aa0: beq #0x470abc
00470aa4: mov r0, r3
00470aa8: ldr r3, [r3]
00470aac: mov lr, pc
00470ab0: ldr pc, [r3, #4]
00470ab4: mov r3, #0
00470ab8: str r3, [r4, #0x38]
00470abc: str r5, [r4, #0x38]
00470ac0: pop {r4, r5, r6, pc}

# _ZN14AnimControllerC1EP13RootSceneNodeb
00474d30: push {r4, r5, r6, lr}
00474d34: ldr r4, [pc, #0xe4]
00474d38: ldr r3, [pc, #0xe4]
00474d3c: cmp r1, #0
00474d40: add r4, pc, r4
00474d44: ldr r3, [r4, r3]
00474d48: sub sp, sp, #8
00474d4c: mov r5, r0
00474d50: add r3, r3, #8
00474d54: str r3, [r0]
00474d58: mov r6, r2
00474d5c: str r1, [r0, #4]
00474d60: beq #0x474dc8
00474d64: ldr r3, [r1]
00474d68: cmp r6, #0
00474d6c: ldr r3, [r3, #-0xc]
00474d70: add r1, r1, r3
00474d74: ldr r3, [r1, #4]
00474d78: add r3, r3, #1
00474d7c: str r3, [r1, #4]
00474d80: bne #0x474db0
00474d84: ldr r3, [pc, #0x9c]
00474d88: mov r0, r5
00474d8c: mov r2, r5
00474d90: ldr r1, [r4, r3]
00474d94: ldr r3, [pc, #0x90]
00474d98: str r5, [sp]
00474d9c: ldr r3, [r4, r3]
00474da0: bl #0x474cac
00474da4: mov r0, r5
00474da8: add sp, sp, #8
00474dac: pop {r4, r5, r6, pc}
00474db0: ldr r3, [r5, #4]
00474db4: mov r0, r3
00474db8: ldr r3, [r3]
00474dbc: mov lr, pc
00474dc0: ldr pc, [r3, #0x74]
00474dc4: b #0x474da4
00474dc8: ldr r3, [pc, #0x60]
00474dcc: ldr r3, [r4, r3]
00474dd0: ldr r3, [r3]
00474dd4: cmp r3, #2
00474dd8: streq r1, [r1]
00474ddc: beq #0x474d64
00474de0: cmp r3, #1
00474de4: bne #0x474d64
00474de8: ldr r0, [pc, #0x44]
00474dec: ldr r1, [pc, #0x44]
00474df0: ldr r2, [pc, #0x44]
00474df4: ldr r0, [r4, r0]
00474df8: ldr r3, [pc, #0x40]
00474dfc: add r1, pc, r1
00474e00: mov ip, #0x1c
00474e04: add r0, r0, #0xa8
00474e08: add r2, pc, r2
00474e0c: add r3, pc, r3
00474e10: str ip, [sp]
00474e14: bl #0x30e004
00474e18: ldr r1, [r5, #4]
00474e1c: b #0x474d64
00474e20: subseq pc, r1, r0, asr sp
00474e24: andeq r4, r0, r0, lsl r8
00474e28: andeq r3, r0, r0, ror r7
00474e2c: andeq r2, r0, r0, lsl #18
00474e30: andeq r3, r0, r0, asr #19
00474e34: andeq r1, r0, r0, asr #19
00474e38: ldrdeq sb, sl, [r4], #-0x5c
00474e3c: subeq r8, r5, r8, lsr r8
00474e40: subeq r8, r5, ip, asr sb

# _ZN12VisualObject9SetParentEP10GameObject
0047295c: push {r4, r5, r6, lr}
00472960: ldr r4, [pc, #0x9c]
00472964: cmp r1, #0
00472968: str r1, [r0, #4]
0047296c: mov r5, r0
00472970: add r4, pc, r4
00472974: beq #0x4729c8
00472978: ldrb r3, [r1, #0x84]
0047297c: cmp r3, #0
00472980: bne #0x4729a4
00472984: mov r0, r5
00472988: bl #0x38ba74
0047298c: ldr r3, [pc, #0x74]
00472990: ldr r0, [r5, #8]
00472994: mov r1, #1
00472998: ldr r2, [r4, r3]
0047299c: pop {r4, r5, r6, lr}
004729a0: b #0x50e484
004729a4: mov r0, r1
004729a8: ldr r3, [r1]
004729ac: mov lr, pc
004729b0: ldr pc, [r3, #0x80]
004729b4: cmp r0, #0
004729b8: beq #0x4729cc
004729bc: ldr r3, [r5, #4]
004729c0: cmp r3, #0
004729c4: bne #0x472984
004729c8: pop {r4, r5, r6, pc}
004729cc: mov r0, r5
004729d0: bl #0x38ba74
004729d4: ldr r6, [r5, #8]
004729d8: ldr r3, [r6]
004729dc: mov r0, r6
004729e0: ldr r4, [r3, #0xa4]
004729e4: mov lr, pc
004729e8: ldr pc, [r3, #0xa0]
004729ec: mov r1, r0
004729f0: mov r0, r6
004729f4: blx r4
004729f8: ldr r0, [r5, #8]
004729fc: pop {r4, r5, r6, lr}
00472a00: b #0x50f220
00472a04: subseq r2, r2, r0, lsr #2
00472a08: andeq r3, r0, r4, ror r0

# _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
0035a0e4: push {r4, r5, r6, r7, r8, sl, lr}
0035a0e8: ldr r4, [pc, #0x90]
0035a0ec: ldr r6, [pc, #0x90]
0035a0f0: cmp r2, #0
0035a0f4: cmpne r1, #0
0035a0f8: add r4, pc, r4
0035a0fc: ldr ip, [r4, r6]
0035a100: mov r5, r1
0035a104: sub sp, sp, #0x24
0035a108: ldr ip, [ip]
0035a10c: moveq r1, #0
0035a110: movne r1, #1
0035a114: mov sl, r0
0035a118: mov r8, r3
0035a11c: str ip, [sp, #0x1c]
0035a120: moveq r5, r1
0035a124: beq #0x35a15c
0035a128: add r7, sp, #4
0035a12c: mov r1, r2
0035a130: mov r0, r7
0035a134: mov r2, sp
0035a138: bl #0x3140ec
0035a13c: mov r1, r5
0035a140: mov r0, sl
0035a144: mov r2, r7
0035a148: mov r3, r8
0035a14c: bl #0x352b74
0035a150: mov r5, r0
0035a154: mov r0, r7
0035a158: bl #0x318254
0035a15c: ldr r3, [r4, r6]
0035a160: ldr r2, [sp, #0x1c]
0035a164: mov r0, r5
0035a168: ldr r3, [r3]
0035a16c: cmp r2, r3
0035a170: bne #0x35a17c
0035a174: add sp, sp, #0x24
0035a178: pop {r4, r5, r6, r7, r8, sl, pc}
0035a17c: bl #0x30e310
0035a180: mlseq r3, r8, sb, sl
0035a184: andeq r4, r0, ip, lsr #1

# _ZN12VisualObject11SetPositionERK7Point3DIfE
00470c24: push {r4, lr}
00470c28: mov r4, r0
00470c2c: ldr r0, [r0, #8]
00470c30: sub sp, sp, #0x10
00470c34: cmp r0, #0
00470c38: beq #0x470c7c
00470c3c: ldr r3, [r0]
00470c40: ldr lr, [r1]
00470c44: ldr ip, [r1, #4]
00470c48: ldr r2, [r1, #8]
00470c4c: ldr r3, [r3, #0xa4]
00470c50: add r1, sp, #4
00470c54: str lr, [sp, #4]
00470c58: str ip, [sp, #8]
00470c5c: str r2, [sp, #0xc]
00470c60: blx r3
00470c64: ldr r3, [r4, #8]
00470c68: mov r1, #0
00470c6c: mov r0, r3
00470c70: ldr r3, [r3]
00470c74: mov lr, pc
00470c78: ldr pc, [r3, #0xb8]
00470c7c: add sp, sp, #0x10
00470c80: pop {r4, pc}

# _ZN12VisualObject11SetRotationERK7Point3DIfE
00472874: push {r4, r5, r6, lr}
00472878: ldr r3, [r0, #8]
0047287c: sub sp, sp, #0x10
00472880: mov r4, r0
00472884: cmp r3, #0
00472888: beq #0x472900
0047288c: ldr r2, [r1]
00472890: ldr r3, [r1, #8]
00472894: mov r0, sp
00472898: add r2, r2, #0x80000000
0047289c: ldr r1, [r1, #4]
004728a0: add r3, r3, #0x80000000
004728a4: bl #0x35c9d8
004728a8: ldr r3, [r4, #8]
004728ac: mov r5, sp
004728b0: mov r0, r3
004728b4: ldr r3, [r3]
004728b8: mov lr, pc
004728bc: ldr pc, [r3, #0x98]
004728c0: ldr r1, [sp]
004728c4: mov r6, r0
004728c8: ldr r0, [r0]
004728cc: bl #0x30df8c
004728d0: cmp r0, #0
004728d4: bne #0x472908
004728d8: ldr r3, [r4, #8]
004728dc: mov r1, sp
004728e0: mov r0, r3
004728e4: ldr r3, [r3]
004728e8: mov lr, pc
004728ec: ldr pc, [r3, #0x9c]
004728f0: mov r0, r4
004728f4: bl #0x47211c
004728f8: mov r0, r4
004728fc: bl #0x470a54
00472900: add sp, sp, #0x10
00472904: pop {r4, r5, r6, pc}
00472908: ldr r0, [r6, #4]
0047290c: ldr r1, [sp, #4]
00472910: bl #0x30df8c
00472914: cmp r0, #0
00472918: beq #0x4728d8
0047291c: ldr r0, [r6, #8]
00472920: ldr r1, [sp, #8]
00472924: bl #0x30df8c
00472928: cmp r0, #0
0047292c: beq #0x4728d8
00472930: ldr r0, [r6, #0xc]
00472934: ldr r1, [sp, #0xc]
00472938: bl #0x30df8c
0047293c: cmp r0, #0
00472940: bne #0x472900
00472944: b #0x4728d8

# _ZN12VisualObject10SetScalingERK7Point3DIfE
004727ac: push {r4, r5, r6, r7, r8, lr}
004727b0: ldr r3, [r0, #8]
004727b4: sub sp, sp, #0x10
004727b8: mov r5, r0
004727bc: cmp r3, #0
004727c0: mov r4, r1
004727c4: beq #0x47282c
004727c8: mov r0, r3
004727cc: ldr r3, [r3]
004727d0: mov lr, pc
004727d4: ldr pc, [r3, #0x90]
004727d8: ldr r7, [r4]
004727dc: ldr r1, [r0]
004727e0: mov r6, r0
004727e4: mov r0, r7
004727e8: bl #0x30df8c
004727ec: cmp r0, #0
004727f0: ldr r8, [r4, #8]
004727f4: ldr r4, [r4, #4]
004727f8: bne #0x472834
004727fc: ldr r0, [r5, #8]
00472800: add r1, sp, #4
00472804: ldr r3, [r0]
00472808: ldr r3, [r3, #0x94]
0047280c: str r7, [sp, #4]
00472810: str r4, [sp, #8]
00472814: str r8, [sp, #0xc]
00472818: blx r3
0047281c: mov r0, r5
00472820: bl #0x47211c
00472824: mov r0, r5
00472828: bl #0x470a54
0047282c: add sp, sp, #0x10
00472830: pop {r4, r5, r6, r7, r8, pc}
00472834: mov r0, r4
00472838: ldr r1, [r6, #4]
0047283c: bl #0x30df8c
00472840: cmp r0, #0
00472844: beq #0x4727fc
00472848: ldr r1, [r6, #8]
0047284c: mov r0, r8
00472850: bl #0x30df8c
00472854: cmp r0, #0
00472858: bne #0x47282c
0047285c: b #0x4727fc

# _ZN13RootSceneNode22updateAbsolutePositionEb
0035c27c: ldr r3, [pc, #0x18]
0035c280: ldr r2, [pc, #0x18]
0035c284: add r3, pc, r3
0035c288: ldr r2, [r3, r2]
0035c28c: ldr r3, [r2]
0035c290: add r3, r3, #1
0035c294: str r3, [r2]
0035c298: b #0x597c60
0035c29c: rsbeq r8, r3, ip, lsl #16
0035c2a0: ldrdeq r2, r3, [r0], -r4

# _ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb
00597c60: push {r4, r5, r6, lr}
00597c64: ldr r3, [r0, #0xec]
00597c68: mov r4, r0
00597c6c: mov r5, r1
00597c70: cmp r3, #0
00597c74: beq #0x597d18
00597c78: ldr r2, [r3, #0x11c]
00597c7c: tst r2, #0x20
00597c80: bne #0x597cd0
00597c84: ldr r2, [r0, #0x11c]
00597c88: tst r2, #0x5e
00597c8c: bne #0x597cd0
00597c90: cmp r5, #0
00597c94: ldrne r5, [r4, #0xf4]!
00597c98: bne #0x597cc4
00597c9c: b #0x597ccc
00597ca0: cmp r5, #0
00597ca4: moveq r3, r5
00597ca8: subne r3, r5, #4
00597cac: mov r0, r3
00597cb0: mov r1, #1
00597cb4: ldr r3, [r3]
00597cb8: mov lr, pc
00597cbc: ldr pc, [r3, #0xb8]
00597cc0: ldr r5, [r5]
00597cc4: cmp r4, r5
00597cc8: bne #0x597ca0
00597ccc: pop {r4, r5, r6, pc}
00597cd0: mov r0, r3
00597cd4: ldr r3, [r3]
00597cd8: mov lr, pc
00597cdc: ldr pc, [r3, #0x38]
00597ce0: ldr r3, [r4]
00597ce4: mov r6, r0
00597ce8: mov r0, r4
00597cec: mov lr, pc
00597cf0: ldr pc, [r3, #0x40]
00597cf4: add r2, r4, #0x24
00597cf8: mov r1, r0
00597cfc: mov r0, r6
00597d00: bl #0x597884
00597d04: ldr r3, [r4, #0x11c]
00597d08: orr r3, r3, #0x120
00597d0c: bic r3, r3, #0x50
00597d10: str r3, [r4, #0x11c]
00597d14: b #0x597c90
00597d18: ldr r3, [r0, #0x11c]
00597d1c: tst r3, #0x5e
00597d20: beq #0x597c90
00597d24: mov r6, r0
00597d28: ldr r3, [r6], #0x24
00597d2c: mov lr, pc
00597d30: ldr pc, [r3, #0x40]
00597d34: mov r2, #0x41
00597d38: mov r1, r0
00597d3c: mov r0, r6
00597d40: bl #0x30e868
00597d44: ldr r3, [r4, #0x11c]
00597d48: orr r3, r3, #0x120
00597d4c: bic r3, r3, #0x50
00597d50: str r3, [r4, #0x11c]
00597d54: b #0x597c90

# _ZN6glitch5scene10ISceneNode8setScaleERKNS_4core8vector3dIfEE
005970c4: ldr r3, [r1]
005970c8: ldr r2, [r0, #0x11c]
005970cc: str r3, [r0, #0xc8]
005970d0: ldr r3, [r1, #4]
005970d4: orr r2, r2, #2
005970d8: str r3, [r0, #0xcc]
005970dc: ldr r3, [r1, #8]
005970e0: str r2, [r0, #0x11c]
005970e4: str r3, [r0, #0xd0]
005970e8: bx lr

# _ZN6glitch5scene10ISceneNode11setRotationERKNS_4core10quaternionE
005970f4: ldr r3, [r1]
005970f8: ldr r2, [r0, #0x11c]
005970fc: str r3, [r0, #0xb8]
00597100: ldr r3, [r1, #4]
00597104: orr r2, r2, #4
00597108: str r3, [r0, #0xbc]
0059710c: ldr r3, [r1, #8]
00597110: str r3, [r0, #0xc0]
00597114: ldr r3, [r1, #0xc]
00597118: str r2, [r0, #0x11c]
0059711c: str r3, [r0, #0xc4]
00597120: bx lr

# _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0059712c: ldr r3, [r1]
00597130: ldr r2, [r0, #0x11c]
00597134: str r3, [r0, #0xac]
00597138: ldr r3, [r1, #4]
0059713c: orr r2, r2, #8
00597140: str r3, [r0, #0xb0]
00597144: ldr r3, [r1, #8]
00597148: str r2, [r0, #0x11c]
0059714c: str r3, [r0, #0xb4]
00597150: bx lr

# _ZNK10GameObject11IsUpdatableEv
0038aac0: mov r0, #1
0038aac4: bx lr

# _ZN5Decor8InitPostEv
00388a98: push {r4, r5, r6, lr}
00388a9c: mov r4, r0
00388aa0: bl #0x38be5c
00388aa4: ldr r0, [r4, #0x2d8]
00388aa8: ldr r5, [pc, #0x68]
00388aac: cmp r0, #0
00388ab0: add r5, pc, r5
00388ab4: beq #0x388b14
00388ab8: bl #0x470a54
00388abc: ldr r3, [r4, #0x2d8]
00388ac0: ldrb r3, [r3, #0x28]
00388ac4: cmp r3, #0
00388ac8: bne #0x388ad8
00388acc: mov r0, r4
00388ad0: pop {r4, r5, r6, lr}
00388ad4: b #0x388730
00388ad8: ldr r3, [pc, #0x3c]
00388adc: mov r1, #0
00388ae0: mov r0, #0x28
00388ae4: ldr r3, [r5, r3]
00388ae8: ldr r6, [r3, #0x44]
00388aec: bl #0x310570
00388af0: mov r1, r6
00388af4: mov r5, r0
00388af8: mov r2, r4
00388afc: bl #0x388a2c
00388b00: mov r0, r4
00388b04: mov r1, r5
00388b08: mov r2, #0
00388b0c: bl #0x394bf8
00388b10: b #0x388acc
00388b14: pop {r4, r5, r6, pc}
00388b18: rsbeq fp, r0, r0, ror #31
00388b1c: strdeq r3, r4, [r0], -r4

# _ZN5Decor12LoadFloorMapEv
00388730: push {r4, lr}
00388734: ldr r2, [r0, #0x2d8]
00388738: ldr r3, [pc, #0x68]
0038873c: mov r4, r0
00388740: cmp r2, #0
00388744: add r3, pc, r3
00388748: beq #0x388758
0038874c: ldrb r1, [r0, #0x375]
00388750: cmp r1, #0
00388754: bne #0x38875c
00388758: pop {r4, pc}
0038875c: ldr r1, [r2, #8]
00388760: ldr r2, [r0, #0x64]
00388764: ldr r0, [pc, #0x40]
00388768: ldr r0, [r3, r0]
0038876c: ldr r3, [r4, #0x44]
00388770: bl #0x523c14
00388774: cmp r0, #0
00388778: beq #0x38879c
0038877c: ldrb r3, [r4, #0x376]
00388780: add r1, r4, #0x12c
00388784: cmp r3, #0
00388788: ldr r3, [r0, #0x24]
0038878c: orrne r3, r3, #1
00388790: biceq r3, r3, #1
00388794: str r3, [r0, #0x24]
00388798: bl #0x388218
0038879c: mov r3, #0
003887a0: strb r3, [r4, #0x375]
003887a4: pop {r4, pc}
003887a8: rsbeq ip, r0, ip, asr #6
003887ac: andeq r1, r0, r4, lsl #4
