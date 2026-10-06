# 0x65d2b4 _ZN6glitch7collada10CSceneNodeC2ERKNS0_16CColladaDatabaseEPNS0_5SNodeE
0065d2b4: push {r4, r5, r6, r7, lr}
0065d2b8: mov r5, r2
0065d2bc: sub sp, sp, #0x2c
0065d2c0: mvn r2, #0
0065d2c4: mov r4, r1
0065d2c8: add r1, r1, #4
0065d2cc: mov r6, r0
0065d2d0: mov r7, r3
0065d2d4: bl #0x583b34
0065d2d8: ldr r2, [r5]
0065d2dc: ldr r3, [pc, #0xfc]
0065d2e0: str r2, [r6, #0x14c]
0065d2e4: ldr r1, [r5, #4]
0065d2e8: cmp r2, #0
0065d2ec: add r3, pc, r3
0065d2f0: str r1, [r6, #0x150]
0065d2f4: beq #0x65d308
0065d2f8: ldr r1, [r2, #4]
0065d2fc: cmp r1, #0
0065d300: addne r1, r1, #1
0065d304: strne r1, [r2, #4]
0065d308: ldr r2, [pc, #0xd4]
0065d30c: cmp r7, #0
0065d310: ldr r2, [r3, r2]
0065d314: add r2, r2, #4
0065d318: str r2, [r6, #0x148]
0065d31c: ldr r3, [r4]
0065d320: str r3, [r6]
0065d324: ldr r3, [r3, #-0x1c]
0065d328: ldr r2, [r4, #0x1c]
0065d32c: str r2, [r6, r3]
0065d330: ldr r3, [r6]
0065d334: ldr r2, [r4, #0x20]
0065d338: ldr r3, [r3, #-0xc]
0065d33c: str r2, [r6, r3]
0065d340: str r7, [r6, #0x154]
0065d344: beq #0x65d3d4
0065d348: ldr r1, [r7, #4]
0065d34c: mov r0, r6
0065d350: bl #0x598a04
0065d354: ldr r3, [r6, #0x154]
0065d358: mov r0, r6
0065d35c: add r1, sp, #0x1c
0065d360: ldr r2, [r3, #0xc]
0065d364: str r2, [sp, #0x1c]
0065d368: ldr r2, [r3, #0x10]
0065d36c: str r2, [sp, #0x20]
0065d370: ldr r3, [r3, #0x14]
0065d374: str r3, [sp, #0x24]
0065d378: bl #0x59712c
0065d37c: ldr r3, [r6, #0x154]
0065d380: mov r0, r6
0065d384: mov r1, sp
0065d388: ldr r2, [r3, #0x18]
0065d38c: str r2, [sp]
0065d390: ldr r2, [r3, #0x1c]
0065d394: str r2, [sp, #4]
0065d398: ldr r2, [r3, #0x20]
0065d39c: str r2, [sp, #8]
0065d3a0: ldr r3, [r3, #0x24]
0065d3a4: str r3, [sp, #0xc]
0065d3a8: bl #0x5970f4
0065d3ac: ldr r3, [r6, #0x154]
0065d3b0: mov r0, r6
0065d3b4: add r1, sp, #0x10
0065d3b8: ldr r2, [r3, #0x28]
0065d3bc: str r2, [sp, #0x10]
0065d3c0: ldr r2, [r3, #0x2c]
0065d3c4: str r2, [sp, #0x14]
0065d3c8: ldr r3, [r3, #0x30]
0065d3cc: str r3, [sp, #0x18]
0065d3d0: bl #0x5970c4
0065d3d4: mov r0, r6
0065d3d8: add sp, sp, #0x2c
0065d3dc: pop {r4, r5, r6, r7, pc}
0065d3e0: eorseq r7, r3, r4, lsr #15
0065d3e4: strheq r1, [r0], -r4

# 0x65c074 _ZN6glitch7collada14CRootSceneNodeD1Ev
0065c074: push {r4, r5, r6, r7, r8, lr}
0065c078: ldr r5, [pc, #0x1b8]
0065c07c: ldr r3, [pc, #0x1b8]
0065c080: mov r4, r0
0065c084: add r5, pc, r5
0065c088: ldr r3, [r5, r3]
0065c08c: add r7, r0, #0x1b4
0065c090: add r2, r3, #0x124
0065c094: add r3, r3, #0x1c
0065c098: str r3, [r0]
0065c09c: str r2, [r0, #0x1bc]
0065c0a0: bl #0x5987e8
0065c0a4: ldr r0, [r4, #0x1b4]
0065c0a8: cmp r0, r7
0065c0ac: bne #0x65c0b8
0065c0b0: b #0x65c0cc
0065c0b4: mov r0, r6
0065c0b8: ldr r6, [r0]
0065c0bc: bl #0x310450
0065c0c0: cmp r6, r7
0065c0c4: bne #0x65c0b4
0065c0c8: mov r0, r7
0065c0cc: str r0, [r4, #0x1b4]
0065c0d0: str r0, [r7, #4]
0065c0d4: ldr r3, [r4, #0x1a0]
0065c0d8: cmp r3, #0
0065c0dc: bne #0x65c210
0065c0e0: ldr r0, [r4, #0x188]
0065c0e4: add r7, r4, #0x188
0065c0e8: cmp r0, r7
0065c0ec: bne #0x65c0f8
0065c0f0: b #0x65c10c
0065c0f4: mov r0, r6
0065c0f8: ldr r6, [r0]
0065c0fc: bl #0x310450
0065c100: cmp r6, r7
0065c104: bne #0x65c0f4
0065c108: mov r0, r7
0065c10c: str r0, [r4, #0x188]
0065c110: str r0, [r7, #4]
0065c114: add r0, r4, #0x180
0065c118: bl #0x65b03c
0065c11c: add r0, r4, #0x178
0065c120: bl #0x65bf5c
0065c124: ldr r0, [r4, #0x170]
0065c128: add r7, r4, #0x170
0065c12c: cmp r0, r7
0065c130: bne #0x65c13c
0065c134: b #0x65c150
0065c138: mov r0, r6
0065c13c: ldr r6, [r0]
0065c140: bl #0x310450
0065c144: cmp r6, r7
0065c148: bne #0x65c138
0065c14c: mov r0, r7
0065c150: str r0, [r4, #0x170]
0065c154: str r0, [r7, #4]
0065c158: ldr r0, [r4, #0x168]
0065c15c: add r7, r4, #0x168
0065c160: cmp r0, r7
0065c164: bne #0x65c170
0065c168: b #0x65c184
0065c16c: mov r0, r6
0065c170: ldr r6, [r0]
0065c174: bl #0x310450
0065c178: cmp r6, r7
0065c17c: bne #0x65c16c
0065c180: mov r0, r7
0065c184: str r0, [r4, #0x168]
0065c188: str r0, [r7, #4]
0065c18c: ldr r0, [r4, #0x160]
0065c190: add r7, r4, #0x160
0065c194: cmp r0, r7
0065c198: bne #0x65c1a4
0065c19c: b #0x65c1b8
0065c1a0: mov r0, r6
0065c1a4: ldr r6, [r0]
0065c1a8: bl #0x310450
0065c1ac: cmp r6, r7
0065c1b0: bne #0x65c1a0
0065c1b4: mov r0, r7
0065c1b8: str r0, [r4, #0x160]
0065c1bc: str r0, [r7, #4]
0065c1c0: ldr r0, [r4, #0x158]
0065c1c4: add r7, r4, #0x158
0065c1c8: cmp r0, r7
0065c1cc: bne #0x65c1d8
0065c1d0: b #0x65c1ec
0065c1d4: mov r0, r6
0065c1d8: ldr r6, [r0]
0065c1dc: bl #0x310450
0065c1e0: cmp r6, r7
0065c1e4: bne #0x65c1d4
0065c1e8: mov r0, r7
0065c1ec: ldr r1, [pc, #0x4c]
0065c1f0: str r0, [r4, #0x158]
0065c1f4: str r0, [r7, #4]
0065c1f8: ldr r1, [r5, r1]
0065c1fc: mov r0, r4
0065c200: add r1, r1, #4
0065c204: bl #0x65af4c
0065c208: mov r0, r4
0065c20c: pop {r4, r5, r6, r7, r8, pc}
0065c210: add r6, r4, #0x190
0065c214: mov r0, r6
0065c218: ldr r1, [r4, #0x194]
0065c21c: bl #0x65c038
0065c220: mov r3, #0
0065c224: str r6, [r4, #0x19c]
0065c228: str r3, [r4, #0x1a0]
0065c22c: str r6, [r4, #0x198]
0065c230: str r3, [r4, #0x194]
0065c234: b #0x65c0e0
0065c238: eorseq r8, r3, ip, lsl #20
0065c23c: andeq r1, r0, ip, lsl #11
0065c240: andeq r3, r0, r4, asr ip

# 0x597f68 _ZN6glitch5scene10ISceneNode21removeBindedAnimatorsEv
00597f68: push {r4, r5, r6, lr}
00597f6c: mov r6, r0
00597f70: mov r5, r0
00597f74: ldr r4, [r6, #0x104]!
00597f78: b #0x597fac
00597f7c: ldr r3, [r4, #8]
00597f80: mov r1, r5
00597f84: mov r0, r3
00597f88: ldr r3, [r3]
00597f8c: mov lr, pc
00597f90: ldr pc, [r3, #0x2c]
00597f94: ldr r3, [r4, #8]
00597f98: ldr r2, [r3]
00597f9c: ldr r0, [r2, #-0xc]
00597fa0: add r0, r3, r0
00597fa4: bl #0x31d584
00597fa8: ldr r4, [r4]
00597fac: cmp r6, r4
00597fb0: bne #0x597f7c
00597fb4: ldr r0, [r5, #0x104]
00597fb8: cmp r4, r0
00597fbc: bne #0x597fc8
00597fc0: b #0x597fd8
00597fc4: mov r0, r6
00597fc8: ldr r6, [r0]
00597fcc: bl #0x310450
00597fd0: cmp r6, r4
00597fd4: bne #0x597fc4
00597fd8: str r4, [r5, #0x108]
00597fdc: str r4, [r5, #0x104]
00597fe0: pop {r4, r5, r6, pc}

# 0x598b84 _ZN6glitch5scene10ISceneNodeD1Ev
00598b84: ldr r2, [pc, #0x10c]
00598b88: ldr r3, [pc, #0x10c]
00598b8c: push {r4, r5, r6, lr}
00598b90: add r2, pc, r2
00598b94: ldr r3, [r2, r3]
00598b98: mov r5, r0
00598b9c: add r2, r3, #0x120
00598ba0: add r3, r3, #0x1c
00598ba4: str r3, [r0]
00598ba8: str r2, [r0, #0x130]
00598bac: bl #0x5987e8
00598bb0: ldr r0, [r5, #0x114]
00598bb4: cmp r0, #0
00598bb8: beq #0x598bc0
00598bbc: bl #0x31d584
00598bc0: ldr r0, [r5, #0x104]
00598bc4: add r6, r5, #0x104
00598bc8: cmp r6, r0
00598bcc: bne #0x598bd8
00598bd0: b #0x598be8
00598bd4: mov r0, r4
00598bd8: ldr r4, [r0]
00598bdc: bl #0x310450
00598be0: cmp r6, r4
00598be4: bne #0x598bd4
00598be8: str r6, [r5, #0x104]
00598bec: str r6, [r6, #4]
00598bf0: ldr r0, [r5, #0xfc]
00598bf4: add r6, r5, #0xfc
00598bf8: cmp r0, r6
00598bfc: bne #0x598c08
00598c00: b #0x598c1c
00598c04: mov r0, r4
00598c08: ldr r4, [r0]
00598c0c: bl #0x310450
00598c10: cmp r4, r6
00598c14: bne #0x598c04
00598c18: mov r0, r6
00598c1c: str r0, [r5, #0xfc]
00598c20: add ip, r5, #0xf0
00598c24: str r0, [r6, #4]
00598c28: ldr r3, [ip, #4]
00598c2c: add r0, r5, #0xf4
00598c30: cmp r3, r0
00598c34: beq #0x598c5c
00598c38: mov r1, #0
00598c3c: b #0x598c44
00598c40: mov r3, r2
00598c44: ldr r2, [r3]
00598c48: str r1, [r3, #4]
00598c4c: str r1, [r3]
00598c50: cmp r0, r2
00598c54: bne #0x598c40
00598c58: mov r3, r0
00598c5c: str r3, [ip, #8]
00598c60: str r3, [ip, #4]
00598c64: mov r3, #0
00598c68: str r3, [r5, #0xf0]
00598c6c: add r3, r5, #0xc
00598c70: ldr r0, [r3, #0x14]
00598c74: cmp r0, r3
00598c78: beq #0x598c88
00598c7c: cmp r0, #0
00598c80: beq #0x598c88
00598c84: bl #0x310450
00598c88: mov r0, r5
00598c8c: bl #0x6a1194
00598c90: mov r0, r5
00598c94: pop {r4, r5, r6, pc}
00598c98: eorseq fp, pc, r0, lsl #30
00598c9c: andeq r2, r0, r0, lsl #3

# 0x5990c0 _ZN6glitch5scene10ISceneNodeC2EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
005990c0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005990c4: mov r5, #0
005990c8: sub sp, sp, #0x14
005990cc: mov r8, r1
005990d0: str r5, [r0, #4]
005990d4: str r5, [r0, #8]
005990d8: mov r4, r0
005990dc: mov sl, r3
005990e0: ldr r7, [sp, #0x3c]
005990e4: str r2, [sp, #0xc]
005990e8: bl #0x6a118c
005990ec: ldr r2, [r8]
005990f0: add r3, r4, #0xc
005990f4: mov r0, r3
005990f8: str r2, [r4]
005990fc: ldr r1, [r8, #4]
00599100: ldr r2, [r2, #-0x1c]
00599104: mov sb, #0x40
00599108: mov r6, #0x3f800000
0059910c: str r1, [r4, r2]
00599110: ldr r2, [r4]
00599114: ldr r1, [r8, #8]
00599118: mov r8, #1
0059911c: ldr r2, [r2, #-0xc]
00599120: str r1, [r4, r2]
00599124: str r3, [r4, #0x1c]
00599128: str r3, [r4, #0x20]
0059912c: bl #0x598ee0
00599130: ldr r3, [r4, #0x1c]
00599134: mov r1, r5
00599138: mov r2, sb
0059913c: strb r5, [r3]
00599140: add r0, r4, #0x24
00599144: strb r5, [r4, #0x64]
00599148: bl #0x30e460
0059914c: mov r2, sb
00599150: mov r1, r5
00599154: str r6, [r4, #0x24]
00599158: str r6, [r4, #0x38]
0059915c: str r6, [r4, #0x4c]
00599160: str r6, [r4, #0x60]
00599164: strb r8, [r4, #0x64]
00599168: strb r5, [r4, #0xa8]
0059916c: add r0, r4, #0x68
00599170: bl #0x30e460
00599174: str r6, [r4, #0x68]
00599178: str r6, [r4, #0x7c]
0059917c: str r6, [r4, #0x90]
00599180: str r6, [r4, #0xa4]
00599184: strb r8, [r4, #0xa8]
00599188: ldr r3, [sl]
0059918c: add r2, r4, #0xb8
00599190: str r2, [sp, #4]
00599194: str r3, [r4, #0xac]
00599198: ldr r3, [sl, #4]
0059919c: mov ip, #0xbf000000
005991a0: add ip, ip, #0x800000
005991a4: str r3, [r4, #0xb0]
005991a8: ldr r3, [sl, #8]
005991ac: add lr, r4, #0xfc
005991b0: add sb, r4, #0xf4
005991b4: str r3, [r4, #0xb4]
005991b8: ldr fp, [sp, #0x38]
005991bc: add sl, r4, #0x104
005991c0: ldm fp, {r0, r1, r2, r3}
005991c4: ldr fp, [sp, #4]
005991c8: stm fp, {r0, r1, r2, r3}
005991cc: ldr r3, [r7]
005991d0: mov r0, r4
005991d4: mov r1, r5
005991d8: str r3, [r4, #0xc8]
005991dc: ldr r3, [r7, #4]
005991e0: str r3, [r4, #0xcc]
005991e4: ldr r3, [r7, #8]
005991e8: str ip, [r4, #0xdc]
005991ec: str ip, [r4, #0xd4]
005991f0: str r3, [r4, #0xd0]
005991f4: str ip, [r4, #0xd8]
005991f8: str r6, [r4, #0xe8]
005991fc: str r6, [r4, #0xe0]
00599200: str r6, [r4, #0xe4]
00599204: str r5, [r4, #0xec]
00599208: str r5, [r4, #0xf0]
0059920c: str sb, [r4, #0xf4]
00599210: str sb, [r4, #0xf8]
00599214: str lr, [r4, #0x100]
00599218: str sl, [r4, #0x108]
0059921c: ldr r2, [sp, #0xc]
00599220: movw r3, #0x60f
00599224: str r3, [r4, #0x11c]
00599228: mov r3, #0
0059922c: str r2, [r4, #0x10c]
00599230: strb r8, [r4, #0x121]
00599234: str r3, [r4, #0x128]
00599238: str lr, [r4, #0xfc]
0059923c: str sl, [r4, #0x104]
00599240: str r5, [r4, #0x110]
00599244: str r5, [r4, #0x114]
00599248: str r5, [r4, #0x118]
0059924c: strb r8, [r4, #0x120]
00599250: str r5, [r4, #0x124]
00599254: str r5, [r4, #0x12c]
00599258: bl #0x597c60
0059925c: mov r0, r4
00599260: add sp, sp, #0x14
00599264: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# 0x35d764 _ZN13RootSceneNodeD2Ev
0035d764: push {r4, r5, r6, lr}
0035d768: ldr r3, [r1]
0035d76c: mov r5, r1
0035d770: mov r4, r0
0035d774: str r3, [r0]
0035d778: ldr r3, [r3, #-0x1c]
0035d77c: ldr r2, [r1, #0x34]
0035d780: str r2, [r0, r3]
0035d784: ldr r3, [r0]
0035d788: ldr r2, [r1, #0x38]
0035d78c: ldr r3, [r3, #-0xc]
0035d790: str r2, [r0, r3]
0035d794: ldr r3, [r0, #0x1f0]
0035d798: cmp r3, #0
0035d79c: beq #0x35d7b8
0035d7a0: ldr r2, [r3]
0035d7a4: ldr r0, [r2, #-0xc]
0035d7a8: add r0, r3, r0
0035d7ac: bl #0x31d584
0035d7b0: mov r3, #0
0035d7b4: str r3, [r4, #0x1f0]
0035d7b8: ldr r3, [r4, #0x1f4]
0035d7bc: cmp r3, #0
0035d7c0: beq #0x35d7dc
0035d7c4: ldr r2, [r3]
0035d7c8: ldr r0, [r2, #-0xc]
0035d7cc: add r0, r3, r0
0035d7d0: bl #0x31d584
0035d7d4: mov r3, #0
0035d7d8: str r3, [r4, #0x1f4]
0035d7dc: ldr r3, [r4, #0x1f8]
0035d7e0: cmp r3, #0
0035d7e4: beq #0x35d800
0035d7e8: ldr r2, [r3]
0035d7ec: ldr r0, [r2, #-0xc]
0035d7f0: add r0, r3, r0
0035d7f4: bl #0x31d584
0035d7f8: mov r3, #0
0035d7fc: str r3, [r4, #0x1f8]
0035d800: add r0, r4, #0x1d4
0035d804: bl #0x3139ac
0035d808: add r0, r4, #0x1bc
0035d80c: bl #0x3139ac
0035d810: mov r0, r4
0035d814: add r1, r5, #4
0035d818: bl #0x65c260
0035d81c: mov r0, r4
0035d820: pop {r4, r5, r6, pc}

# 0x473884 _ZN12VisualObjectD1Ev
00473884: push {r4, r5, r6, lr}
00473888: ldr r5, [pc, #0x204]
0047388c: ldr r3, [pc, #0x204]
00473890: mov r4, r0
00473894: add r5, pc, r5
00473898: ldr r3, [r5, r3]
0047389c: mov r1, #0
004738a0: add r3, r3, #8
004738a4: str r3, [r0]
004738a8: bl #0x470a84
004738ac: ldr r3, [r4, #0xc]
004738b0: cmp r3, #0
004738b4: beq #0x4738d0
004738b8: ldr r2, [r3]
004738bc: ldr r0, [r2, #-0xc]
004738c0: add r0, r3, r0
004738c4: bl #0x31d584
004738c8: mov r3, #0
004738cc: str r3, [r4, #0xc]
004738d0: ldr r3, [r4, #8]
004738d4: cmp r3, #0
004738d8: beq #0x47391c
004738dc: mov r0, r3
004738e0: ldr r3, [r3]
004738e4: mov lr, pc
004738e8: ldr pc, [r3, #0x74]
004738ec: ldr r3, [r4, #8]
004738f0: mov r0, r3
004738f4: ldr r3, [r3]
004738f8: mov lr, pc
004738fc: ldr pc, [r3, #0x68]
00473900: ldr r3, [r4, #8]
00473904: ldr r2, [r3]
00473908: ldr r0, [r2, #-0xc]
0047390c: add r0, r3, r0
00473910: bl #0x31d584
00473914: mov r3, #0
00473918: str r3, [r4, #8]
0047391c: ldr r3, [r4, #0x30]
00473920: cmp r3, #0
00473924: beq #0x473968
00473928: mov r0, r3
0047392c: ldr r3, [r3]
00473930: mov lr, pc
00473934: ldr pc, [r3, #0x74]
00473938: ldr r3, [r4, #0x30]
0047393c: mov r0, r3
00473940: ldr r3, [r3]
00473944: mov lr, pc
00473948: ldr pc, [r3, #0x68]
0047394c: ldr r3, [r4, #0x30]
00473950: ldr r2, [r3]
00473954: ldr r0, [r2, #-0xc]
00473958: add r0, r3, r0
0047395c: bl #0x31d584
00473960: mov r3, #0
00473964: str r3, [r4, #0x30]
00473968: ldr r3, [r4, #0x34]
0047396c: cmp r3, #0
00473970: beq #0x4739b4
00473974: mov r0, r3
00473978: ldr r3, [r3]
0047397c: mov lr, pc
00473980: ldr pc, [r3, #0x74]
00473984: ldr r3, [r4, #0x34]
00473988: mov r0, r3
0047398c: ldr r3, [r3]
00473990: mov lr, pc
00473994: ldr pc, [r3, #0x68]
00473998: ldr r3, [r4, #0x34]
0047399c: ldr r2, [r3]
004739a0: ldr r0, [r2, #-0xc]
004739a4: add r0, r3, r0
004739a8: bl #0x31d584
004739ac: mov r3, #0
004739b0: str r3, [r4, #0x34]
004739b4: ldr r3, [pc, #0xe0]
004739b8: ldr r3, [r5, r3]
004739bc: ldr r3, [r3, #0x10]
004739c0: ldr r0, [r3, #0x1c]
004739c4: bl #0x350ee0
004739c8: ldr r0, [r4, #0x9c]
004739cc: add r3, r4, #0x9c
004739d0: cmp r0, #0
004739d4: beq #0x4739f0
004739d8: ldr r1, [r3, #8]
004739dc: rsb r1, r0, r1
004739e0: bic r1, r1, #3
004739e4: cmp r1, #0x80
004739e8: bhi #0x473a70
004739ec: bl #0x708f00
004739f0: ldr r0, [r4, #0x8c]
004739f4: add r3, r4, #0x8c
004739f8: cmp r0, #0
004739fc: beq #0x473a18
00473a00: ldr r1, [r3, #8]
00473a04: rsb r1, r0, r1
00473a08: bic r1, r1, #3
00473a0c: cmp r1, #0x80
00473a10: bhi #0x473a8c
00473a14: bl #0x708f00
00473a18: ldr r0, [r4, #0x80]
00473a1c: add r3, r4, #0x80
00473a20: cmp r0, #0
00473a24: beq #0x473a40
00473a28: ldr r1, [r3, #8]
00473a2c: rsb r1, r0, r1
00473a30: bic r1, r1, #3
00473a34: cmp r1, #0x80
00473a38: bhi #0x473a84
00473a3c: bl #0x708f00
00473a40: ldr r0, [r4, #0x44]
00473a44: add r3, r4, #0x44
00473a48: cmp r0, #0
00473a4c: beq #0x473a68
00473a50: ldr r1, [r3, #0x10]
00473a54: rsb r1, r0, r1
00473a58: bic r1, r1, #3
00473a5c: cmp r1, #0x80
00473a60: bhi #0x473a78
00473a64: bl #0x708f00
00473a68: mov r0, r4
00473a6c: pop {r4, r5, r6, pc}
00473a70: bl #0x310440
00473a74: b #0x4739f0
00473a78: bl #0x310440
00473a7c: mov r0, r4
00473a80: pop {r4, r5, r6, pc}
00473a84: bl #0x310440
00473a88: b #0x473a40
00473a8c: bl #0x310440
00473a90: b #0x473a18
00473a94: ldrsheq r1, [r2], #-0x1c
00473a98: muleq r0, r4, lr
00473a9c: strdeq r3, r4, [r0], -r4

# 0x598864 _ZN6glitch5scene10ISceneNode8addChildEPS1_
00598864: cmp r1, r0
00598868: cmpne r1, #0
0059886c: push {r4, r5, r6, lr}
00598870: mov r5, r0
00598874: mov r4, r1
00598878: bne #0x598880
0059887c: pop {r4, r5, r6, pc}
00598880: ldr r3, [r1]
00598884: mov r0, r1
00598888: ldr r3, [r3, #-0xc]
0059888c: add r3, r1, r3
00598890: ldr r2, [r3, #4]
00598894: add r2, r2, #1
00598898: str r2, [r3, #4]
0059889c: ldr r3, [r1]
005988a0: mov lr, pc
005988a4: ldr pc, [r3, #0x68]
005988a8: ldr r2, [r5, #0xf8]
005988ac: add r3, r4, #4
005988b0: add r1, r5, #0xf4
005988b4: str r2, [r4, #8]
005988b8: str r3, [r2]
005988bc: str r3, [r5, #0xf8]
005988c0: str r1, [r4, #4]
005988c4: ldr r3, [r5, #0xf0]
005988c8: mov r0, r4
005988cc: mov r1, r5
005988d0: add r3, r3, #1
005988d4: str r3, [r5, #0xf0]
005988d8: bl #0x5971e0
005988dc: ldr r0, [r5, #0x110]
005988e0: cmp r0, #0
005988e4: beq #0x5988ec
005988e8: bl #0x5890b4
005988ec: ldr r1, [r5, #0x11c]
005988f0: mov r0, r4
005988f4: ldr r3, [r4]
005988f8: and r1, r1, #1
005988fc: mov lr, pc
00598900: ldr pc, [r3, #0xec]
00598904: pop {r4, r5, r6, pc}

# 0x5987e8 _ZN6glitch5scene10ISceneNode9removeAllEv
005987e8: push {r4, r5, r6, r7, r8, lr}
005987ec: mov r6, r0
005987f0: ldr r3, [r6, #0xf4]!
005987f4: mov r7, r0
005987f8: cmp r3, r6
005987fc: beq #0x59883c
00598800: mov r4, #0
00598804: b #0x59880c
00598808: mov r3, r5
0059880c: sub r2, r3, #4
00598810: ldr r5, [r3]
00598814: str r4, [r3, #4]
00598818: str r4, [r3]
0059881c: str r4, [r2, #0xec]
00598820: ldr r3, [r3, #-4]
00598824: ldr r0, [r3, #-0xc]
00598828: add r0, r2, r0
0059882c: bl #0x31d584
00598830: cmp r6, r5
00598834: bne #0x598808
00598838: mov r3, r6
0059883c: ldr r0, [r7, #0x110]
00598840: mov r2, #0
00598844: str r3, [r7, #0xf8]
00598848: cmp r0, #0
0059884c: str r2, [r7, #0xf0]
00598850: str r3, [r7, #0xf4]
00598854: beq #0x598860
00598858: pop {r4, r5, r6, r7, r8, lr}
0059885c: b #0x5890b4
00598860: pop {r4, r5, r6, r7, r8, pc}

# 0x597fe4 _ZN6glitch5scene10ISceneNode20removeBindedAnimatorEPNS0_18ISceneNodeAnimatorE
00597fe4: push {r4, lr}
00597fe8: mov r2, r0
00597fec: mov r3, r1
00597ff0: ldr r4, [r0, #0x104]!
00597ff4: b #0x598008
00597ff8: ldr r1, [r4, #8]
00597ffc: cmp r1, r3
00598000: beq #0x598014
00598004: ldr r4, [r4]
00598008: cmp r0, r4
0059800c: bne #0x597ff8
00598010: pop {r4, pc}
00598014: mov r1, r2
00598018: mov r0, r3
0059801c: ldr r3, [r3]
00598020: mov lr, pc
00598024: ldr pc, [r3, #0x2c]
00598028: ldr r3, [r4, #8]
0059802c: ldr r2, [r3]
00598030: ldr r0, [r2, #-0xc]
00598034: add r0, r3, r0
00598038: bl #0x31d584
0059803c: ldr r3, [r4]
00598040: ldr r2, [r4, #4]
00598044: mov r0, r4
00598048: str r3, [r2]
0059804c: str r2, [r3, #4]
00598050: pop {r4, lr}
00598054: b #0x310450

# 0x596ce0 _ZN6glitch5scene10ISceneNode8onDeleteEv
00596ce0: push {r4, lr}
00596ce4: mov r4, r0
00596ce8: ldr r3, [r0]
00596cec: mov lr, pc
00596cf0: ldr pc, [r3, #0x80]
00596cf4: mov r0, r4
00596cf8: ldr r3, [r4]
00596cfc: mov lr, pc
00596d00: ldr pc, [r3, #0x74]
00596d04: pop {r4, pc}

# 0x35c4e0 _ZN6glitch7collada10CSceneNodeD0Ev
0035c4e0: push {r4, lr}
0035c4e4: mov r4, r0
0035c4e8: bl #0x35c444
0035c4ec: mov r0, r4
0035c4f0: bl #0x310440
0035c4f4: mov r0, r4
0035c4f8: pop {r4, pc}

# 0x597094 _ZNK6glitch5scene10ISceneNode12getAnimatorsEv
00597094: add r0, r0, #0xfc
00597098: bx lr

# 0x598ca0 _ZN6glitch5scene10ISceneNodeD0Ev
00598ca0: push {r4, lr}
00598ca4: mov r4, r0
00598ca8: bl #0x598b84
00598cac: mov r0, r4
00598cb0: bl #0x30e2b0
00598cb4: mov r0, r4
00598cb8: pop {r4, pc}

# 0x599268 _ZN6glitch5scene10ISceneNodeC1EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
00599268: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059926c: ldr r6, [pc, #0x1c0]
00599270: ldr lr, [pc, #0x1c0]
00599274: ldr ip, [pc, #0x1c0]
00599278: add r6, pc, r6
0059927c: ldr lr, [r6, lr]
00599280: ldr ip, [r6, ip]
00599284: mov sl, #1
00599288: ldr r5, [lr, #0xc]
0059928c: add ip, ip, #8
00599290: str sl, [r0, #0x134]
00599294: str r5, [r0]
00599298: str ip, [r0, #0x130]
0059929c: ldr ip, [r5, #-0xc]
005992a0: ldr lr, [lr, #0x10]
005992a4: mov r5, #0
005992a8: sub sp, sp, #0xc
005992ac: str lr, [r0, ip]
005992b0: str r5, [r0, #4]
005992b4: str r5, [r0, #8]
005992b8: mov r4, r0
005992bc: mov r7, r2
005992c0: str r3, [sp]
005992c4: ldr r8, [sp, #0x30]
005992c8: str r1, [sp, #4]
005992cc: bl #0x6a118c
005992d0: ldr r2, [pc, #0x168]
005992d4: add r1, r4, #0xc
005992d8: mov r0, r1
005992dc: ldr r2, [r6, r2]
005992e0: str r1, [r4, #0x1c]
005992e4: str r1, [r4, #0x20]
005992e8: add r1, r2, #0x120
005992ec: add r2, r2, #0x1c
005992f0: str r2, [r4]
005992f4: str r1, [r4, #0x130]
005992f8: bl #0x598ee0
005992fc: ldr r2, [r4, #0x1c]
00599300: mov sb, #0x40
00599304: mov r6, #0x3f800000
00599308: strb r5, [r2]
0059930c: mov r1, r5
00599310: mov r2, sb
00599314: strb r5, [r4, #0x64]
00599318: add r0, r4, #0x24
0059931c: bl #0x30e460
00599320: mov r2, sb
00599324: mov r1, r5
00599328: str r6, [r4, #0x24]
0059932c: str r6, [r4, #0x38]
00599330: str r6, [r4, #0x4c]
00599334: str r6, [r4, #0x60]
00599338: strb sl, [r4, #0x64]
0059933c: strb r5, [r4, #0xa8]
00599340: add r0, r4, #0x68
00599344: bl #0x30e460
00599348: str r6, [r4, #0x68]
0059934c: str r6, [r4, #0x7c]
00599350: str r6, [r4, #0x90]
00599354: str r6, [r4, #0xa4]
00599358: strb sl, [r4, #0xa8]
0059935c: ldr r2, [r7]
00599360: add fp, r4, #0xb8
00599364: mov ip, #0xbf000000
00599368: str r2, [r4, #0xac]
0059936c: ldr r2, [r7, #4]
00599370: add ip, ip, #0x800000
00599374: add lr, r4, #0xfc
00599378: str r2, [r4, #0xb0]
0059937c: ldr r2, [r7, #8]
00599380: add sb, r4, #0xf4
00599384: add r7, r4, #0x104
00599388: str r2, [r4, #0xb4]
0059938c: ldr r3, [sp]
00599390: ldm r3, {r0, r1, r2, r3}
00599394: stm fp, {r0, r1, r2, r3}
00599398: ldr r3, [r8]
0059939c: mov r0, r4
005993a0: mov r1, r5
005993a4: str r3, [r4, #0xc8]
005993a8: ldr r3, [r8, #4]
005993ac: str r3, [r4, #0xcc]
005993b0: ldr r3, [r8, #8]
005993b4: str ip, [r4, #0xdc]
005993b8: str ip, [r4, #0xd4]
005993bc: str r3, [r4, #0xd0]
005993c0: str ip, [r4, #0xd8]
005993c4: str r6, [r4, #0xe8]
005993c8: str r6, [r4, #0xe0]
005993cc: str r6, [r4, #0xe4]
005993d0: str r5, [r4, #0xec]
005993d4: str r5, [r4, #0xf0]
005993d8: str sb, [r4, #0xf4]
005993dc: str sb, [r4, #0xf8]
005993e0: str lr, [r4, #0x100]
005993e4: str r7, [r4, #0x108]
005993e8: ldr r3, [sp, #4]
005993ec: strb sl, [r4, #0x121]
005993f0: str lr, [r4, #0xfc]
005993f4: str r3, [r4, #0x10c]
005993f8: movw r3, #0x60f
005993fc: str r3, [r4, #0x11c]
00599400: mov r3, #0
00599404: str r3, [r4, #0x128]
00599408: str r7, [r4, #0x104]
0059940c: str r5, [r4, #0x110]
00599410: str r5, [r4, #0x114]
00599414: str r5, [r4, #0x118]
00599418: strb sl, [r4, #0x120]
0059941c: str r5, [r4, #0x124]
00599420: str r5, [r4, #0x12c]
00599424: bl #0x597c60
00599428: mov r0, r4
0059942c: add sp, sp, #0xc
00599430: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00599434: eorseq fp, pc, r8, lsl r8
00599438: andeq r4, r0, r4, lsr #23
0059943c: andeq r2, r0, r4, asr #22
00599440: andeq r2, r0, r0, lsl #3

# 0x65b000 _ZN6glitch7collada14CRootSceneNode18removeMorphingMeshEPNS0_13CMorphingMeshE
0065b000: ldr ip, [r0, #0x168]!
0065b004: b #0x65b018
0065b008: ldr r3, [ip, #8]
0065b00c: cmp r3, r1
0065b010: beq #0x65b024
0065b014: ldr ip, [ip]
0065b018: cmp r0, ip
0065b01c: bne #0x65b008
0065b020: bx lr
0065b024: ldr r3, [ip]
0065b028: ldr r2, [ip, #4]
0065b02c: mov r0, ip
0065b030: str r3, [r2]
0065b034: str r2, [r3, #4]
0065b038: b #0x310450

# 0x473aa0 _ZN12VisualObjectD0Ev
00473aa0: push {r4, lr}
00473aa4: mov r4, r0
00473aa8: bl #0x473884
00473aac: mov r0, r4
00473ab0: bl #0x310440
00473ab4: mov r0, r4
00473ab8: pop {r4, pc}

# 0x65b844 _ZN6glitch7collada14CRootSceneNodeC2ERKNS0_16CColladaDatabaseE
0065b844: push {r4, r5, r6, r7, r8, sb, sl, lr}
0065b848: mov r3, #0
0065b84c: mov r5, r1
0065b850: add r1, r1, #4
0065b854: mov r4, r0
0065b858: bl #0x65d2b4
0065b85c: ldr r2, [r5]
0065b860: mov r1, #0
0065b864: mov r3, r4
0065b868: str r2, [r4]
0065b86c: ldr r0, [r5, #0x28]
0065b870: ldr r2, [r2, #-0x1c]
0065b874: add lr, r4, #0x170
0065b878: add ip, r4, #0x178
0065b87c: str r0, [r4, r2]
0065b880: ldr r2, [r4]
0065b884: ldr sl, [r5, #0x2c]
0065b888: add r0, r4, #0x180
0065b88c: ldr r8, [r2, #-0xc]
0065b890: add r7, r4, #0x158
0065b894: add r2, r4, #0x188
0065b898: add r6, r4, #0x160
0065b89c: add r5, r4, #0x168
0065b8a0: str sl, [r4, r8]
0065b8a4: str r0, [r4, #0x184]
0065b8a8: str r2, [r4, #0x18c]
0065b8ac: str r0, [r4, #0x180]
0065b8b0: str r2, [r4, #0x188]
0065b8b4: str r7, [r4, #0x15c]
0065b8b8: add r2, r4, #0x1b4
0065b8bc: str r6, [r4, #0x164]
0065b8c0: str r5, [r4, #0x16c]
0065b8c4: str lr, [r4, #0x174]
0065b8c8: str ip, [r4, #0x17c]
0065b8cc: str r7, [r4, #0x158]
0065b8d0: str r6, [r4, #0x160]
0065b8d4: str r5, [r4, #0x168]
0065b8d8: str lr, [r4, #0x170]
0065b8dc: str ip, [r4, #0x178]
0065b8e0: str r1, [r4, #0x194]
0065b8e4: mov r0, #1
0065b8e8: strb r1, [r3, #0x190]!
0065b8ec: str r3, [r4, #0x19c]
0065b8f0: str r0, [r4, #0x1ac]
0065b8f4: str r2, [r4, #0x1b8]
0065b8f8: mov r0, r4
0065b8fc: str r3, [r4, #0x198]
0065b900: str r1, [r4, #0x1a0]
0065b904: strb r1, [r4, #0x1a8]
0065b908: str r2, [r4, #0x1b4]
0065b90c: bl #0x59719c
0065b910: mov r0, r4
0065b914: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x65c260 _ZN6glitch7collada14CRootSceneNodeD2Ev
0065c260: push {r4, r5, r6, r7, r8, lr}
0065c264: ldr r3, [r1]
0065c268: mov r4, r0
0065c26c: add r7, r0, #0x1b4
0065c270: str r3, [r0]
0065c274: ldr r3, [r3, #-0x1c]
0065c278: ldr r2, [r1, #0x28]
0065c27c: mov r5, r1
0065c280: str r2, [r0, r3]
0065c284: ldr r3, [r0]
0065c288: ldr r2, [r1, #0x2c]
0065c28c: ldr r3, [r3, #-0xc]
0065c290: str r2, [r0, r3]
0065c294: bl #0x5987e8
0065c298: ldr r0, [r4, #0x1b4]
0065c29c: cmp r0, r7
0065c2a0: bne #0x65c2ac
0065c2a4: b #0x65c2c0
0065c2a8: mov r0, r6
0065c2ac: ldr r6, [r0]
0065c2b0: bl #0x310450
0065c2b4: cmp r6, r7
0065c2b8: bne #0x65c2a8
0065c2bc: mov r0, r7
0065c2c0: str r0, [r4, #0x1b4]
0065c2c4: str r0, [r7, #4]
0065c2c8: ldr r3, [r4, #0x1a0]
0065c2cc: cmp r3, #0
0065c2d0: bne #0x65c3fc
0065c2d4: ldr r0, [r4, #0x188]
0065c2d8: add r7, r4, #0x188
0065c2dc: cmp r0, r7
0065c2e0: bne #0x65c2ec
0065c2e4: b #0x65c300
0065c2e8: mov r0, r6
0065c2ec: ldr r6, [r0]
0065c2f0: bl #0x310450
0065c2f4: cmp r6, r7
0065c2f8: bne #0x65c2e8
0065c2fc: mov r0, r7
0065c300: str r0, [r4, #0x188]
0065c304: str r0, [r7, #4]
0065c308: add r0, r4, #0x180
0065c30c: bl #0x65b03c
0065c310: add r0, r4, #0x178
0065c314: bl #0x65bf5c
0065c318: ldr r0, [r4, #0x170]
0065c31c: add r7, r4, #0x170
0065c320: cmp r0, r7
0065c324: bne #0x65c330
0065c328: b #0x65c344
0065c32c: mov r0, r6
0065c330: ldr r6, [r0]
0065c334: bl #0x310450
0065c338: cmp r6, r7
0065c33c: bne #0x65c32c
0065c340: mov r0, r7
0065c344: str r0, [r4, #0x170]
0065c348: str r0, [r7, #4]
0065c34c: ldr r0, [r4, #0x168]
0065c350: add r7, r4, #0x168
0065c354: cmp r0, r7
0065c358: bne #0x65c364
0065c35c: b #0x65c378
0065c360: mov r0, r6
0065c364: ldr r6, [r0]
0065c368: bl #0x310450
0065c36c: cmp r6, r7
0065c370: bne #0x65c360
0065c374: mov r0, r7
0065c378: str r0, [r4, #0x168]
0065c37c: str r0, [r7, #4]
0065c380: ldr r0, [r4, #0x160]
0065c384: add r7, r4, #0x160
0065c388: cmp r0, r7
0065c38c: bne #0x65c398
0065c390: b #0x65c3ac
0065c394: mov r0, r6
0065c398: ldr r6, [r0]
0065c39c: bl #0x310450
0065c3a0: cmp r6, r7
0065c3a4: bne #0x65c394
0065c3a8: mov r0, r7
0065c3ac: str r0, [r4, #0x160]
0065c3b0: str r0, [r7, #4]
0065c3b4: ldr r0, [r4, #0x158]
0065c3b8: add r7, r4, #0x158
0065c3bc: cmp r0, r7
0065c3c0: bne #0x65c3cc
0065c3c4: b #0x65c3e0
0065c3c8: mov r0, r6
0065c3cc: ldr r6, [r0]
0065c3d0: bl #0x310450
0065c3d4: cmp r6, r7
0065c3d8: bne #0x65c3c8
0065c3dc: mov r0, r7
0065c3e0: str r0, [r4, #0x158]
0065c3e4: add r1, r5, #4
0065c3e8: str r0, [r7, #4]
0065c3ec: mov r0, r4
0065c3f0: bl #0x65af4c
0065c3f4: mov r0, r4
0065c3f8: pop {r4, r5, r6, r7, r8, pc}
0065c3fc: add r6, r4, #0x190
0065c400: mov r0, r6
0065c404: ldr r1, [r4, #0x194]
0065c408: bl #0x65c038
0065c40c: mov r3, #0
0065c410: str r6, [r4, #0x19c]
0065c414: str r3, [r4, #0x1a0]
0065c418: str r6, [r4, #0x198]
0065c41c: str r3, [r4, #0x194]
0065c420: b #0x65c2d4

# 0x5986e8 _ZN6glitch5scene10ISceneNode14removeAnimatorEPNS0_18ISceneNodeAnimatorE
005986e8: push {r4, r5, r6, lr}
005986ec: mov r2, r0
005986f0: mov r5, r0
005986f4: mov r3, r1
005986f8: ldr r4, [r2, #0xfc]!
005986fc: b #0x598710
00598700: ldr r1, [r4, #8]
00598704: cmp r1, r3
00598708: beq #0x59871c
0059870c: ldr r4, [r4]
00598710: cmp r2, r4
00598714: bne #0x598700
00598718: pop {r4, r5, r6, pc}
0059871c: mov r0, r3
00598720: mov r1, r5
00598724: ldr r3, [r3]
00598728: mov lr, pc
0059872c: ldr pc, [r3, #0x2c]
00598730: ldr r3, [r4, #8]
00598734: ldr r2, [r3]
00598738: ldr r0, [r2, #-0xc]
0059873c: add r0, r3, r0
00598740: bl #0x31d584
00598744: ldr r3, [r4]
00598748: ldr r2, [r4, #4]
0059874c: mov r0, r4
00598750: str r3, [r2]
00598754: str r2, [r3, #4]
00598758: bl #0x310450
0059875c: ldr r0, [r5, #0x110]
00598760: cmp r0, #0
00598764: beq #0x598718
00598768: pop {r4, r5, r6, lr}
0059876c: b #0x5890b4

# 0x598658 _ZN6glitch5scene10ISceneNode15removeAnimatorsEv
00598658: push {r4, r5, r6, lr}
0059865c: mov r6, r0
00598660: mov r5, r0
00598664: ldr r4, [r6, #0xfc]!
00598668: b #0x59869c
0059866c: ldr r3, [r4, #8]
00598670: mov r1, r5
00598674: mov r0, r3
00598678: ldr r3, [r3]
0059867c: mov lr, pc
00598680: ldr pc, [r3, #0x2c]
00598684: ldr r3, [r4, #8]
00598688: ldr r2, [r3]
0059868c: ldr r0, [r2, #-0xc]
00598690: add r0, r3, r0
00598694: bl #0x31d584
00598698: ldr r4, [r4]
0059869c: cmp r6, r4
005986a0: bne #0x59866c
005986a4: ldr r0, [r5, #0xfc]
005986a8: cmp r4, r0
005986ac: bne #0x5986b8
005986b0: b #0x5986c8
005986b4: mov r0, r6
005986b8: ldr r6, [r0]
005986bc: bl #0x310450
005986c0: cmp r4, r6
005986c4: bne #0x5986b4
005986c8: ldr r0, [r5, #0x110]
005986cc: str r4, [r5, #0x100]
005986d0: str r4, [r5, #0xfc]
005986d4: cmp r0, #0
005986d8: beq #0x5986e4
005986dc: pop {r4, r5, r6, lr}
005986e0: b #0x5890b4
005986e4: pop {r4, r5, r6, pc}

# 0x35c444 _ZN6glitch7collada10CSceneNodeD1Ev
0035c444: push {r4, r5, r6, lr}
0035c448: ldr r5, [pc, #0x64]
0035c44c: ldr r3, [pc, #0x64]
0035c450: mov r4, r0
0035c454: add r5, pc, r5
0035c458: ldr r3, [r5, r3]
0035c45c: add r0, r0, #0x14c
0035c460: add r2, r3, #0x124
0035c464: add r3, r3, #0x1c
0035c468: str r3, [r4]
0035c46c: str r2, [r4, #0x158]
0035c470: bl #0x619474
0035c474: ldr r3, [pc, #0x40]
0035c478: mov r0, r4
0035c47c: ldr r1, [r5, r3]
0035c480: ldr r3, [r1, #4]
0035c484: ldr ip, [r1, #0x14]
0035c488: ldr r2, [r1, #0x18]
0035c48c: str r3, [r4]
0035c490: ldr r3, [r3, #-0x1c]
0035c494: add r1, r1, #8
0035c498: str ip, [r4, r3]
0035c49c: ldr r3, [r4]
0035c4a0: ldr r3, [r3, #-0xc]
0035c4a4: str r2, [r4, r3]
0035c4a8: bl #0x598cbc
0035c4ac: mov r0, r4
0035c4b0: pop {r4, r5, r6, pc}
0035c4b4: rsbeq r8, r3, ip, lsr r6
0035c4b8: andeq r3, r0, ip, ror r4
0035c4bc: andeq r4, r0, r4, asr #32

# 0x35d67c _ZN13RootSceneNodeD1Ev
0035d67c: push {r4, r5, r6, lr}
0035d680: ldr r5, [pc, #0xb4]
0035d684: ldr r3, [pc, #0xb4]
0035d688: ldr r2, [r0, #0x1f0]
0035d68c: add r5, pc, r5
0035d690: ldr r3, [r5, r3]
0035d694: cmp r2, #0
0035d698: mov r4, r0
0035d69c: add r1, r3, #0x124
0035d6a0: add r3, r3, #0x1c
0035d6a4: str r3, [r0]
0035d6a8: str r1, [r0, #0x20c]
0035d6ac: beq #0x35d6c8
0035d6b0: ldr r3, [r2]
0035d6b4: ldr r0, [r3, #-0xc]
0035d6b8: add r0, r2, r0
0035d6bc: bl #0x31d584
0035d6c0: mov r3, #0
0035d6c4: str r3, [r4, #0x1f0]
0035d6c8: ldr r3, [r4, #0x1f4]
0035d6cc: cmp r3, #0
0035d6d0: beq #0x35d6ec
0035d6d4: ldr r2, [r3]
0035d6d8: ldr r0, [r2, #-0xc]
0035d6dc: add r0, r3, r0
0035d6e0: bl #0x31d584
0035d6e4: mov r3, #0
0035d6e8: str r3, [r4, #0x1f4]
0035d6ec: ldr r3, [r4, #0x1f8]
0035d6f0: cmp r3, #0
0035d6f4: beq #0x35d710
0035d6f8: ldr r2, [r3]
0035d6fc: ldr r0, [r2, #-0xc]
0035d700: add r0, r3, r0
0035d704: bl #0x31d584
0035d708: mov r3, #0
0035d70c: str r3, [r4, #0x1f8]
0035d710: add r0, r4, #0x1d4
0035d714: bl #0x3139ac
0035d718: add r0, r4, #0x1bc
0035d71c: bl #0x3139ac
0035d720: ldr r1, [pc, #0x1c]
0035d724: mov r0, r4
0035d728: ldr r1, [r5, r1]
0035d72c: add r1, r1, #4
0035d730: bl #0x65c260
0035d734: mov r0, r4
0035d738: pop {r4, r5, r6, pc}
0035d73c: rsbeq r7, r3, r4, lsl #8
0035d740: andeq r3, r0, r8, lsl #18
0035d744: strdeq r1, r2, [r0], -r4

# 0x35d824 _ZN13RootSceneNodeC1ERKN6glitch7collada16CColladaDatabaseE
0035d824: push {r4, r5, r6, lr}
0035d828: ldr r5, [pc, #0xd0]
0035d82c: ldr r3, [pc, #0xd0]
0035d830: ldr r2, [pc, #0xd0]
0035d834: add r5, pc, r5
0035d838: ldr r3, [r5, r3]
0035d83c: ldr r2, [r5, r2]
0035d840: mov r6, #1
0035d844: ldr ip, [r3, #0x3c]
0035d848: add r2, r2, #8
0035d84c: str r2, [r0, #0x20c]
0035d850: str r6, [r0, #0x210]
0035d854: str ip, [r0]
0035d858: ldr lr, [r3, #0x40]
0035d85c: ldr ip, [ip, #-0xc]
0035d860: mov r2, r1
0035d864: add r1, r3, #4
0035d868: str lr, [r0, ip]
0035d86c: mov r4, r0
0035d870: bl #0x65b844
0035d874: ldr r3, [pc, #0x90]
0035d878: add r2, r4, #0x1bc
0035d87c: mov r0, r2
0035d880: ldr r3, [r5, r3]
0035d884: str r2, [r4, #0x1cc]
0035d888: str r2, [r4, #0x1d0]
0035d88c: add r2, r3, #0x124
0035d890: add r3, r3, #0x1c
0035d894: str r3, [r4]
0035d898: str r2, [r4, #0x20c]
0035d89c: mov r1, #0x10
0035d8a0: bl #0x31167c
0035d8a4: ldr r2, [r4, #0x1cc]
0035d8a8: mov r5, #0
0035d8ac: add r3, r4, #0x1d4
0035d8b0: strb r5, [r2]
0035d8b4: mov r0, r3
0035d8b8: str r3, [r4, #0x1e4]
0035d8bc: str r3, [r4, #0x1e8]
0035d8c0: mov r1, #0x10
0035d8c4: bl #0x31167c
0035d8c8: ldr r3, [r4, #0x1e4]
0035d8cc: mov r0, r4
0035d8d0: strb r5, [r3]
0035d8d4: strb r6, [r4, #0x208]
0035d8d8: strb r5, [r4, #0x20a]
0035d8dc: strb r5, [r4, #0x1ec]
0035d8e0: str r5, [r4, #0x1f0]
0035d8e4: str r5, [r4, #0x1f4]
0035d8e8: str r5, [r4, #0x1f8]
0035d8ec: str r5, [r4, #0x1fc]
0035d8f0: strb r6, [r4, #0x200]
0035d8f4: str r5, [r4, #0x204]
0035d8f8: strb r5, [r4, #0x209]
0035d8fc: pop {r4, r5, r6, pc}
0035d900: rsbeq r7, r3, ip, asr r2
0035d904: strdeq r1, r2, [r0], -r4
0035d908: andeq r2, r0, r4, asr #22
0035d90c: andeq r3, r0, r8, lsl #18

# 0x65add4 _ZN6glitch7collada14CRootSceneNode14removeMaterialERKN5boost13intrusive_ptrINS_5video9CMaterialEEE
0065add4: bx lr

# 0x597004 _ZN6glitch5scene10ISceneNode11removeChildEPS1_
00597004: ldr r3, [r1, #0xec]
00597008: push {r4, lr}
0059700c: cmp r3, r0
00597010: beq #0x59701c
00597014: mov r0, #0
00597018: pop {r4, pc}
0059701c: ldr r2, [r1, #4]
00597020: add ip, r1, #4
00597024: cmp r2, #0
00597028: ldrne r0, [r1, #8]
0059702c: strne r2, [r0]
00597030: strne r0, [r2, #4]
00597034: ldr lr, [r3, #0xf0]
00597038: mov r2, #0
0059703c: sub r0, ip, #4
00597040: sub lr, lr, #1
00597044: str lr, [r3, #0xf0]
00597048: str r2, [r1, #8]
0059704c: str r2, [r1, #4]
00597050: str r2, [r0, #0xec]
00597054: ldr r3, [ip, #-4]
00597058: ldr r3, [r3, #-0xc]
0059705c: add r0, r0, r3
00597060: bl #0x31d584
00597064: mov r0, #1
00597068: pop {r4, pc}

# 0x65b734 _ZN6glitch7collada14CRootSceneNodeC1ERKNS0_16CColladaDatabaseE
0065b734: push {r4, r5, r6, r7, r8, sb, sl, lr}
0065b738: ldr r5, [pc, #0xf4]
0065b73c: ldr r3, [pc, #0xf4]
0065b740: ldr r2, [pc, #0xf4]
0065b744: add r5, pc, r5
0065b748: ldr r3, [r5, r3]
0065b74c: ldr r2, [r5, r2]
0065b750: mov r6, #1
0065b754: ldr ip, [r3, #0x30]
0065b758: add r2, r2, #8
0065b75c: str r2, [r0, #0x1bc]
0065b760: str r6, [r0, #0x1c0]
0065b764: str ip, [r0]
0065b768: ldr lr, [r3, #0x34]
0065b76c: ldr ip, [ip, #-0xc]
0065b770: mov r2, r1
0065b774: add r1, r3, #4
0065b778: str lr, [r0, ip]
0065b77c: mov r3, #0
0065b780: mov r4, r0
0065b784: bl #0x65d2b4
0065b788: ldr r2, [pc, #0xb0]
0065b78c: mov r1, #0
0065b790: mov r3, r4
0065b794: ldr r2, [r5, r2]
0065b798: add lr, r4, #0x178
0065b79c: add ip, r4, #0x180
0065b7a0: add r0, r4, #0x188
0065b7a4: add sl, r4, #0x158
0065b7a8: add r8, r4, #0x160
0065b7ac: add sb, r2, #0x124
0065b7b0: add r7, r4, #0x168
0065b7b4: add r5, r4, #0x170
0065b7b8: add r2, r2, #0x1c
0065b7bc: str r2, [r4]
0065b7c0: str r0, [r4, #0x18c]
0065b7c4: str r0, [r4, #0x188]
0065b7c8: add r2, r4, #0x1b4
0065b7cc: str sb, [r4, #0x1bc]
0065b7d0: str sl, [r4, #0x15c]
0065b7d4: str r8, [r4, #0x164]
0065b7d8: str r7, [r4, #0x16c]
0065b7dc: str r5, [r4, #0x174]
0065b7e0: str lr, [r4, #0x17c]
0065b7e4: str ip, [r4, #0x184]
0065b7e8: str sl, [r4, #0x158]
0065b7ec: str r8, [r4, #0x160]
0065b7f0: str r7, [r4, #0x168]
0065b7f4: str r5, [r4, #0x170]
0065b7f8: str lr, [r4, #0x178]
0065b7fc: str ip, [r4, #0x180]
0065b800: str r1, [r4, #0x194]
0065b804: strb r1, [r3, #0x190]!
0065b808: mov r0, r4
0065b80c: str r3, [r4, #0x19c]
0065b810: str r6, [r4, #0x1ac]
0065b814: str r2, [r4, #0x1b8]
0065b818: str r3, [r4, #0x198]
0065b81c: str r1, [r4, #0x1a0]
0065b820: strb r1, [r4, #0x1a8]
0065b824: str r2, [r4, #0x1b4]
0065b828: bl #0x59719c
0065b82c: mov r0, r4
0065b830: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0065b834: eorseq sb, r3, ip, asr #6
0065b838: andeq r3, r0, r4, asr ip
0065b83c: andeq r2, r0, r4, asr #22
0065b840: andeq r1, r0, ip, lsl #11

# 0x473abc _ZN12VisualObjectD2Ev
00473abc: push {r4, r5, r6, lr}
00473ac0: ldr r5, [pc, #0x204]
00473ac4: ldr r3, [pc, #0x204]
00473ac8: mov r4, r0
00473acc: add r5, pc, r5
00473ad0: ldr r3, [r5, r3]
00473ad4: mov r1, #0
00473ad8: add r3, r3, #8
00473adc: str r3, [r0]
00473ae0: bl #0x470a84
00473ae4: ldr r3, [r4, #0xc]
00473ae8: cmp r3, #0
00473aec: beq #0x473b08
00473af0: ldr r2, [r3]
00473af4: ldr r0, [r2, #-0xc]
00473af8: add r0, r3, r0
00473afc: bl #0x31d584
00473b00: mov r3, #0
00473b04: str r3, [r4, #0xc]
00473b08: ldr r3, [r4, #8]
00473b0c: cmp r3, #0
00473b10: beq #0x473b54
00473b14: mov r0, r3
00473b18: ldr r3, [r3]
00473b1c: mov lr, pc
00473b20: ldr pc, [r3, #0x74]
00473b24: ldr r3, [r4, #8]
00473b28: mov r0, r3
00473b2c: ldr r3, [r3]
00473b30: mov lr, pc
00473b34: ldr pc, [r3, #0x68]
00473b38: ldr r3, [r4, #8]
00473b3c: ldr r2, [r3]
00473b40: ldr r0, [r2, #-0xc]
00473b44: add r0, r3, r0
00473b48: bl #0x31d584
00473b4c: mov r3, #0
00473b50: str r3, [r4, #8]
00473b54: ldr r3, [r4, #0x30]
00473b58: cmp r3, #0
00473b5c: beq #0x473ba0
00473b60: mov r0, r3
00473b64: ldr r3, [r3]
00473b68: mov lr, pc
00473b6c: ldr pc, [r3, #0x74]
00473b70: ldr r3, [r4, #0x30]
00473b74: mov r0, r3
00473b78: ldr r3, [r3]
00473b7c: mov lr, pc
00473b80: ldr pc, [r3, #0x68]
00473b84: ldr r3, [r4, #0x30]
00473b88: ldr r2, [r3]
00473b8c: ldr r0, [r2, #-0xc]
00473b90: add r0, r3, r0
00473b94: bl #0x31d584
00473b98: mov r3, #0
00473b9c: str r3, [r4, #0x30]
00473ba0: ldr r3, [r4, #0x34]
00473ba4: cmp r3, #0
00473ba8: beq #0x473bec
00473bac: mov r0, r3
00473bb0: ldr r3, [r3]
00473bb4: mov lr, pc
00473bb8: ldr pc, [r3, #0x74]
00473bbc: ldr r3, [r4, #0x34]
00473bc0: mov r0, r3
00473bc4: ldr r3, [r3]
00473bc8: mov lr, pc
00473bcc: ldr pc, [r3, #0x68]
00473bd0: ldr r3, [r4, #0x34]
00473bd4: ldr r2, [r3]
00473bd8: ldr r0, [r2, #-0xc]
00473bdc: add r0, r3, r0
00473be0: bl #0x31d584
00473be4: mov r3, #0
00473be8: str r3, [r4, #0x34]
00473bec: ldr r3, [pc, #0xe0]
00473bf0: ldr r3, [r5, r3]
00473bf4: ldr r3, [r3, #0x10]
00473bf8: ldr r0, [r3, #0x1c]
00473bfc: bl #0x350ee0
00473c00: ldr r0, [r4, #0x9c]
00473c04: add r3, r4, #0x9c
00473c08: cmp r0, #0
00473c0c: beq #0x473c28
00473c10: ldr r1, [r3, #8]
00473c14: rsb r1, r0, r1
00473c18: bic r1, r1, #3
00473c1c: cmp r1, #0x80
00473c20: bhi #0x473ca8
00473c24: bl #0x708f00
00473c28: ldr r0, [r4, #0x8c]
00473c2c: add r3, r4, #0x8c
00473c30: cmp r0, #0
00473c34: beq #0x473c50
00473c38: ldr r1, [r3, #8]
00473c3c: rsb r1, r0, r1
00473c40: bic r1, r1, #3
00473c44: cmp r1, #0x80
00473c48: bhi #0x473cc4
00473c4c: bl #0x708f00
00473c50: ldr r0, [r4, #0x80]
00473c54: add r3, r4, #0x80
00473c58: cmp r0, #0
00473c5c: beq #0x473c78
00473c60: ldr r1, [r3, #8]
00473c64: rsb r1, r0, r1
00473c68: bic r1, r1, #3
00473c6c: cmp r1, #0x80
00473c70: bhi #0x473cbc
00473c74: bl #0x708f00
00473c78: ldr r0, [r4, #0x44]
00473c7c: add r3, r4, #0x44
00473c80: cmp r0, #0
00473c84: beq #0x473ca0
00473c88: ldr r1, [r3, #0x10]
00473c8c: rsb r1, r0, r1
00473c90: bic r1, r1, #3
00473c94: cmp r1, #0x80
00473c98: bhi #0x473cb0
00473c9c: bl #0x708f00
00473ca0: mov r0, r4
00473ca4: pop {r4, r5, r6, pc}
00473ca8: bl #0x310440
00473cac: b #0x473c28
00473cb0: bl #0x310440
00473cb4: mov r0, r4
00473cb8: pop {r4, r5, r6, pc}
00473cbc: bl #0x310440
00473cc0: b #0x473c78
00473cc4: bl #0x310440
00473cc8: b #0x473c50
00473ccc: subseq r0, r2, r4, asr #31
00473cd0: muleq r0, r4, lr
00473cd4: strdeq r3, r4, [r0], -r4

# 0x65c244 _ZN6glitch7collada14CRootSceneNodeD0Ev
0065c244: push {r4, lr}
0065c248: mov r4, r0
0065c24c: bl #0x65c074
0065c250: mov r0, r4
0065c254: bl #0x30e2b0
0065c258: mov r0, r4
0065c25c: pop {r4, pc}

# 0x65d150 _ZN6glitch7collada10CSceneNodeC1ERKNS0_16CColladaDatabaseEPNS0_5SNodeE
0065d150: push {r4, r5, r6, r7, lr}
0065d154: ldr r5, [pc, #0x144]
0065d158: ldr r3, [pc, #0x144]
0065d15c: ldr ip, [pc, #0x144]
0065d160: add r5, pc, r5
0065d164: ldr r3, [r5, r3]
0065d168: ldr ip, [r5, ip]
0065d16c: mov r6, #1
0065d170: ldr lr, [r3, #0x24]
0065d174: add ip, ip, #8
0065d178: str r6, [r0, #0x15c]
0065d17c: str lr, [r0]
0065d180: str ip, [r0, #0x158]
0065d184: ldr ip, [lr, #-0xc]
0065d188: ldr lr, [r3, #0x28]
0065d18c: mov r7, r1
0065d190: sub sp, sp, #0x2c
0065d194: add r1, r3, #4
0065d198: mov r6, r2
0065d19c: str lr, [r0, ip]
0065d1a0: mvn r2, #0
0065d1a4: mov r4, r0
0065d1a8: bl #0x583b34
0065d1ac: ldr r3, [r7]
0065d1b0: str r3, [r4, #0x14c]
0065d1b4: ldr r2, [r7, #4]
0065d1b8: cmp r3, #0
0065d1bc: str r2, [r4, #0x150]
0065d1c0: beq #0x65d1d4
0065d1c4: ldr r2, [r3, #4]
0065d1c8: cmp r2, #0
0065d1cc: addne r2, r2, #1
0065d1d0: strne r2, [r3, #4]
0065d1d4: ldr r2, [pc, #0xd0]
0065d1d8: ldr r3, [pc, #0xd0]
0065d1dc: cmp r6, #0
0065d1e0: ldr r2, [r5, r2]
0065d1e4: ldr r3, [r5, r3]
0065d1e8: str r6, [r4, #0x154]
0065d1ec: add r2, r2, #4
0065d1f0: add r1, r3, #0x124
0065d1f4: add r3, r3, #0x1c
0065d1f8: str r2, [r4, #0x148]
0065d1fc: str r3, [r4]
0065d200: str r1, [r4, #0x158]
0065d204: beq #0x65d294
0065d208: ldr r1, [r6, #4]
0065d20c: mov r0, r4
0065d210: bl #0x598a04
0065d214: ldr r3, [r4, #0x154]
0065d218: mov r0, r4
0065d21c: add r1, sp, #0x1c
0065d220: ldr r2, [r3, #0xc]
0065d224: str r2, [sp, #0x1c]
0065d228: ldr r2, [r3, #0x10]
0065d22c: str r2, [sp, #0x20]
0065d230: ldr r3, [r3, #0x14]
0065d234: str r3, [sp, #0x24]
0065d238: bl #0x59712c
0065d23c: ldr r3, [r4, #0x154]
0065d240: mov r0, r4
0065d244: mov r1, sp
0065d248: ldr r2, [r3, #0x18]
0065d24c: str r2, [sp]
0065d250: ldr r2, [r3, #0x1c]
0065d254: str r2, [sp, #4]
0065d258: ldr r2, [r3, #0x20]
0065d25c: str r2, [sp, #8]
0065d260: ldr r3, [r3, #0x24]
0065d264: str r3, [sp, #0xc]
0065d268: bl #0x5970f4
0065d26c: ldr r3, [r4, #0x154]
0065d270: mov r0, r4
0065d274: add r1, sp, #0x10
0065d278: ldr r2, [r3, #0x28]
0065d27c: str r2, [sp, #0x10]
0065d280: ldr r2, [r3, #0x2c]
0065d284: str r2, [sp, #0x14]
0065d288: ldr r3, [r3, #0x30]
0065d28c: str r3, [sp, #0x18]
0065d290: bl #0x5970c4
0065d294: mov r0, r4
0065d298: add sp, sp, #0x2c
0065d29c: pop {r4, r5, r6, r7, pc}
0065d2a0: eorseq r7, r3, r0, lsr sb
0065d2a4: andeq r4, r0, r4, asr #32
0065d2a8: andeq r2, r0, r4, asr #22
0065d2ac: strheq r1, [r0], -r4
0065d2b0: andeq r3, r0, ip, ror r4

# 0x59706c _ZN6glitch5scene10ISceneNode6removeEv
0059706c: push {r4, lr}
00597070: ldr r3, [r0, #0xec]
00597074: mov r1, r0
00597078: cmp r3, #0
0059707c: beq #0x597090
00597080: mov r0, r3
00597084: ldr r3, [r3]
00597088: mov lr, pc
0059708c: ldr pc, [r3, #0x60]
00597090: pop {r4, pc}

# 0x31d584 _ZNK6glitch17IReferenceCounted4dropEv
0031d584: push {r4, lr}
0031d588: ldr r3, [r0, #4]
0031d58c: mov r4, r0
0031d590: sub r3, r3, #1
0031d594: cmp r3, #0
0031d598: str r3, [r0, #4]
0031d59c: beq #0x31d5a8
0031d5a0: mov r0, #0
0031d5a4: pop {r4, pc}
0031d5a8: ldr r3, [r0]
0031d5ac: mov lr, pc
0031d5b0: ldr pc, [r3, #8]
0031d5b4: mov r0, r4
0031d5b8: ldr r3, [r4]
0031d5bc: mov lr, pc
0031d5c0: ldr pc, [r3, #4]
0031d5c4: mov r0, #1
0031d5c8: pop {r4, pc}

# 0x35d910 _ZN13RootSceneNodeC2ERKN6glitch7collada16CColladaDatabaseE
0035d910: push {r4, r5, r6, lr}
0035d914: mov r5, r1
0035d918: add r1, r1, #4
0035d91c: mov r4, r0
0035d920: bl #0x65b844
0035d924: ldr r2, [r5]
0035d928: add r3, r4, #0x1bc
0035d92c: mov r0, r3
0035d930: str r2, [r4]
0035d934: ldr ip, [r5, #0x34]
0035d938: ldr r2, [r2, #-0x1c]
0035d93c: mov r1, #0x10
0035d940: mov r6, #0
0035d944: str ip, [r4, r2]
0035d948: ldr r2, [r4]
0035d94c: ldr ip, [r5, #0x38]
0035d950: ldr r2, [r2, #-0xc]
0035d954: str ip, [r4, r2]
0035d958: str r3, [r4, #0x1cc]
0035d95c: str r3, [r4, #0x1d0]
0035d960: bl #0x31167c
0035d964: ldr r2, [r4, #0x1cc]
0035d968: add r3, r4, #0x1d4
0035d96c: mov r0, r3
0035d970: strb r6, [r2]
0035d974: mov r1, #0x10
0035d978: str r3, [r4, #0x1e4]
0035d97c: str r3, [r4, #0x1e8]
0035d980: bl #0x31167c
0035d984: ldr r2, [r4, #0x1e4]
0035d988: mov r3, #1
0035d98c: mov r0, r4
0035d990: strb r6, [r2]
0035d994: strb r3, [r4, #0x208]
0035d998: strb r6, [r4, #0x20a]
0035d99c: strb r6, [r4, #0x1ec]
0035d9a0: str r6, [r4, #0x1f0]
0035d9a4: str r6, [r4, #0x1f4]
0035d9a8: str r6, [r4, #0x1f8]
0035d9ac: str r6, [r4, #0x1fc]
0035d9b0: strb r3, [r4, #0x200]
0035d9b4: str r6, [r4, #0x204]
0035d9b8: strb r6, [r4, #0x209]
0035d9bc: pop {r4, r5, r6, pc}

# 0x35d748 _ZN13RootSceneNodeD0Ev
0035d748: push {r4, lr}
0035d74c: mov r4, r0
0035d750: bl #0x35d67c
0035d754: mov r0, r4
0035d758: bl #0x310440
0035d75c: mov r0, r4
0035d760: pop {r4, pc}
