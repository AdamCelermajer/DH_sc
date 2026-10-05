# _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb 00394bf8 316
00394bf8: push {r4, r5, r6, r7, r8, sb, sl, lr}
00394bfc: ldr r4, [pc, #0x120]
00394c00: ldr r7, [pc, #0x120]
00394c04: ldr ip, [pc, #0x120]
00394c08: add r4, pc, r4
00394c0c: ldr r3, [r4, r7]
00394c10: ldr r8, [r4, ip]
00394c14: sub sp, sp, #0x20
00394c18: ldr r3, [r3]
00394c1c: add r5, sp, #4
00394c20: mov sl, r0
00394c24: mov r0, r8
00394c28: str r3, [sp, #0x1c]
00394c2c: mov sb, r2
00394c30: mov r6, r1
00394c34: bl #0x337888
00394c38: mov r0, r5
00394c3c: mov r1, #0xd
00394c40: str r5, [sp, #0x14]
00394c44: str r5, [sp, #0x18]
00394c48: bl #0x31167c
00394c4c: ldr r1, [pc, #0xdc]
00394c50: mov r2, #0xc
00394c54: ldr r0, [sp, #0x18]
00394c58: add r1, pc, r1
00394c5c: bl #0x30e868
00394c60: add r3, r0, #0xc
00394c64: str r3, [sp, #0x14]
00394c68: mov r3, #0
00394c6c: strb r3, [r0, #0xc]
00394c70: mov r1, r5
00394c74: mov r0, r8
00394c78: bl #0x337a88
00394c7c: mov r8, r0
00394c80: mov r0, r5
00394c84: bl #0x3139ac
00394c88: cmp r8, #0
00394c8c: beq #0x394cc4
00394c90: cmp r6, #0
00394c94: beq #0x394ca8
00394c98: mov r0, r6
00394c9c: ldr r3, [r6]
00394ca0: mov lr, pc
00394ca4: ldr pc, [r3, #4]
00394ca8: ldr r3, [r4, r7]
00394cac: ldr r2, [sp, #0x1c]
00394cb0: ldr r3, [r3]
00394cb4: cmp r2, r3
00394cb8: bne #0x394d20
00394cbc: add sp, sp, #0x20
00394cc0: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00394cc4: ldr r3, [sl, #0x2dc]
00394cc8: cmp r3, r6
00394ccc: beq #0x394d00
00394cd0: cmp r3, #0
00394cd4: beq #0x394cec
00394cd8: mov r0, r3
00394cdc: ldr r3, [r3]
00394ce0: mov lr, pc
00394ce4: ldr pc, [r3, #4]
00394ce8: str r8, [sl, #0x2dc]
00394cec: cmp r6, #0
00394cf0: str r6, [sl, #0x2dc]
00394cf4: beq #0x394d00
00394cf8: cmp sb, #0
00394cfc: bne #0x394d0c
00394d00: mov r0, sl
00394d04: bl #0x393ea0
00394d08: b #0x394ca8
00394d0c: mov r0, r6
00394d10: bl #0x46eb20
00394d14: mov r0, sl
00394d18: bl #0x393ea0
00394d1c: b #0x394ca8
00394d20: bl #0x30e310
00394d24: subseq pc, pc, r8, lsl #29
00394d28: andeq r4, r0, ip, lsr #1
00394d2c: andeq r0, r0, r4, lsl #17
00394d30: subseq sp, r2, r8, lsr #24
# _ZN11COnlineImplC1Ev 00825000 52
00825000: push {r4, r5, r6, lr}
00825004: ldr r4, [pc, #0x20]
00825008: mov r5, r0
0082500c: bl #0x7fd838
00825010: ldr r3, [pc, #0x18]
00825014: add r4, pc, r4
00825018: mov r0, r5
0082501c: ldr r3, [r4, r3]
00825020: add r3, r3, #8
00825024: str r3, [r5]
00825028: pop {r4, r5, r6, pc}
0082502c: andseq pc, r6, ip, ror sl
00825030: andeq r1, r0, r8, lsr #19
# _ZN11AISExternal11OnInitFinalEv 003dce44 16
003dce44: ldr r1, [pc, #4]
003dce48: add r1, pc, r1
003dce4c: b #0x37c514
003dce50: subeq r8, lr, r0, lsr #25
# _ZN14PhysicalObjectC1EP13PhysicalWorldP10GameObjectbbbbstti 0046ef68 904
0046ef68: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ef6c: ldr r5, [pc, #0x360]
0046ef70: ldr r4, [pc, #0x360]
0046ef74: sub sp, sp, #0xb4
0046ef78: add r5, pc, r5
0046ef7c: str r4, [sp, #0x10]
0046ef80: ldr lr, [r5, r4]
0046ef84: ldr ip, [pc, #0x350]
0046ef88: ldr r4, [pc, #0x350]
0046ef8c: ldrb sb, [sp, #0xd8]
0046ef90: ldr ip, [r5, ip]
0046ef94: ldr r4, [r5, r4]
0046ef98: ldr lr, [lr]
0046ef9c: mov r7, #0
0046efa0: str r4, [sp, #0xc]
0046efa4: add ip, ip, #8
0046efa8: mov r4, r0
0046efac: mov sl, #0
0046efb0: str r1, [r0, #4]
0046efb4: str ip, [r0]
0046efb8: str r2, [r4, #8]
0046efbc: str sl, [r0, #0xc]
0046efc0: strb sb, [r0, #0x10]
0046efc4: str r7, [r0, #0x14]
0046efc8: str r7, [r0, #0x18]
0046efcc: str r7, [r0, #0x1c]
0046efd0: strb r7, [r0, #0x26]
0046efd4: strb r7, [r0, #0x27]
0046efd8: ldrb ip, [sp, #0xdc]
0046efdc: mov r6, r2
0046efe0: str lr, [sp, #0xac]
0046efe4: ldrh r2, [sp, #0xe8]
0046efe8: ldrb lr, [sp, #0xe0]
0046efec: str r3, [sp, #0x1c]
0046eff0: ldrh r3, [sp, #0xec]
0046eff4: ldr r0, [sp, #0xc]
0046eff8: str ip, [sp, #0x20]
0046effc: str lr, [sp, #0x24]
0046f000: str r3, [sp, #0x18]
0046f004: ldrsh r8, [sp, #0xe4]
0046f008: str r2, [sp, #0x14]
0046f00c: bl #0x337888
0046f010: ldr r1, [pc, #0x2cc]
0046f014: add fp, sp, #0x94
0046f018: add r2, sp, #0x90
0046f01c: add r1, pc, r1
0046f020: mov r0, fp
0046f024: bl #0x3140ec
0046f028: mov r1, fp
0046f02c: ldr r0, [sp, #0xc]
0046f030: bl #0x337a88
0046f034: mov r3, r0
0046f038: mov r0, fp
0046f03c: str r3, [sp, #8]
0046f040: bl #0x3139ac
0046f044: ldr r3, [sp, #8]
0046f048: mvn r2, #0x298
0046f04c: sub r2, r2, #1
0046f050: cmp r3, r7
0046f054: movne r8, r2
0046f058: cmp r6, r7
0046f05c: beq #0x46f1d0
0046f060: cmp sb, r7
0046f064: bne #0x46f1f4
0046f068: ldr r3, [pc, #0x278]
0046f06c: mov r2, #1
0046f070: ldr r1, [r6, #0x12c]
0046f074: ldr r3, [r5, r3]
0046f078: ldr r0, [r6, #0x138]
0046f07c: str r2, [sp, #0x30]
0046f080: add ip, r3, #8
0046f084: movw r3, #0xcccd
0046f088: movt r3, #0x3e4c
0046f08c: strh r2, [sp, #0x46]
0046f090: mvn r2, #0
0046f094: str ip, [sp, #0x2c]
0046f098: strh r2, [sp, #0x48]
0046f09c: str r3, [sp, #0x38]
0046f0a0: str sl, [sp, #0x40]
0046f0a4: str sb, [sp, #0x8c]
0046f0a8: str sb, [sp, #0x34]
0046f0ac: str sl, [sp, #0x3c]
0046f0b0: strh sb, [sp, #0x4a]
0046f0b4: strb sb, [sp, #0x44]
0046f0b8: bl #0x30e3ac
0046f0bc: movw r1, #0xd70a
0046f0c0: movt r1, #0x3c23
0046f0c4: bl #0x30ed6c
0046f0c8: ldr r1, [r6, #0x130]
0046f0cc: mov sb, r0
0046f0d0: ldr r0, [r6, #0x13c]
0046f0d4: bl #0x30e3ac
0046f0d8: movw r1, #0xd70a
0046f0dc: movt r1, #0x3c23
0046f0e0: bl #0x30ed6c
0046f0e4: movw r1, #0xd70a
0046f0e8: mov r7, r0
0046f0ec: movt r1, #0x3c23
0046f0f0: ldr r0, [r6, #0x160]
0046f0f4: bl #0x30ed6c
0046f0f8: movw r1, #0xd70a
0046f0fc: movt r1, #0x3c23
0046f100: mov sl, r0
0046f104: ldr r0, [r6, #0x164]
0046f108: bl #0x30ed6c
0046f10c: mov r1, #0x3f000000
0046f110: mov r6, r0
0046f114: mov r0, sb
0046f118: bl #0x30ed6c
0046f11c: mov r1, #0x3f000000
0046f120: mov r3, r0
0046f124: mov r0, r7
0046f128: str r3, [sp, #8]
0046f12c: bl #0x30ed6c
0046f130: ldr r3, [sp, #8]
0046f134: add fp, sp, #0x2c
0046f138: mov r2, r0
0046f13c: mov r1, r3
0046f140: mov r0, fp
0046f144: bl #0x7e44b8
0046f148: mov r1, r7
0046f14c: mov r0, sb
0046f150: bl #0x30e70c
0046f154: cmp r0, #0
0046f158: moveq r7, sb
0046f15c: mov r0, r7
0046f160: mov r1, #0x3f000000
0046f164: bl #0x30ed6c
0046f168: ldr r3, [pc, #0x17c]
0046f16c: str r0, [r4, #0xc]
0046f170: mov ip, fp
0046f174: ldr r3, [r5, r3]
0046f178: add r3, r3, #8
0046f17c: str r3, [sp, #0x2c]
0046f180: strh r8, [r4, #0x24]
0046f184: ldr lr, [sp, #0x14]
0046f188: mov r1, ip
0046f18c: mov r3, r6
0046f190: strh lr, [r4, #0x20]
0046f194: ldr r2, [sp, #0x18]
0046f198: mov r0, r4
0046f19c: strh r2, [r4, #0x22]
0046f1a0: ldr lr, [sp, #0x20]
0046f1a4: strh r8, [ip, #0x1e]
0046f1a8: mov r2, sl
0046f1ac: strb lr, [ip, #0x18]
0046f1b0: ldr lr, [sp, #0x14]
0046f1b4: strh lr, [ip, #0x1a]
0046f1b8: ldr lr, [sp, #0x18]
0046f1bc: strh lr, [ip, #0x1c]
0046f1c0: ldr ip, [sp, #0x1c]
0046f1c4: ldr lr, [sp, #0x24]
0046f1c8: stm sp, {ip, lr}
0046f1cc: bl #0x46edf0
0046f1d0: ldr ip, [sp, #0x10]
0046f1d4: ldr r2, [sp, #0xac]
0046f1d8: mov r0, r4
0046f1dc: ldr r3, [r5, ip]
0046f1e0: ldr r3, [r3]
0046f1e4: cmp r2, r3
0046f1e8: bne #0x46f2d0
0046f1ec: add sp, sp, #0xb4
0046f1f0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f1f4: movw r3, #0xcccd
0046f1f8: ldr r1, [r6, #0x12c]
0046f1fc: ldr r0, [r6, #0x138]
0046f200: movt r3, #0x3e4c
0046f204: mov ip, #1
0046f208: mvn lr, #0
0046f20c: str r3, [sp, #0x38]
0046f210: strh ip, [sp, #0x46]
0046f214: strh lr, [sp, #0x48]
0046f218: strb r7, [sp, #0x44]
0046f21c: str r7, [sp, #0x30]
0046f220: str sl, [sp, #0x50]
0046f224: str r7, [sp, #0x34]
0046f228: str sl, [sp, #0x3c]
0046f22c: str sl, [sp, #0x40]
0046f230: strh r7, [sp, #0x4a]
0046f234: str sl, [sp, #0x4c]
0046f238: bl #0x30e3ac
0046f23c: movw r1, #0xd70a
0046f240: movt r1, #0x3c23
0046f244: bl #0x30ed6c
0046f248: ldr r1, [r6, #0x130]
0046f24c: mov sb, r0
0046f250: ldr r0, [r6, #0x13c]
0046f254: bl #0x30e3ac
0046f258: movw r1, #0xd70a
0046f25c: movt r1, #0x3c23
0046f260: bl #0x30ed6c
0046f264: movw r1, #0xd70a
0046f268: mov r7, r0
0046f26c: movt r1, #0x3c23
0046f270: ldr r0, [r6, #0x160]
0046f274: bl #0x30ed6c
0046f278: movw r1, #0xd70a
0046f27c: movt r1, #0x3c23
0046f280: mov sl, r0
0046f284: ldr r0, [r6, #0x164]
0046f288: bl #0x30ed6c
0046f28c: mov r1, r7
0046f290: mov r6, r0
0046f294: mov r0, sb
0046f298: bl #0x30e70c
0046f29c: cmp r0, #0
0046f2a0: moveq r7, sb
0046f2a4: mov r0, r7
0046f2a8: mov r1, #0x3f000000
0046f2ac: bl #0x30ed6c
0046f2b0: ldr r3, [pc, #0x34]
0046f2b4: add ip, sp, #0xb0
0046f2b8: str r0, [r4, #0xc]
0046f2bc: ldr r3, [r5, r3]
0046f2c0: str r0, [sp, #0x54]
0046f2c4: add r3, r3, #8
0046f2c8: str r3, [ip, #-0x84]!
0046f2cc: b #0x46f180
0046f2d0: bl #0x30e310
0046f2d4: subseq r5, r2, r8, lsl fp
0046f2d8: andeq r4, r0, ip, lsr #1
0046f2dc: andeq r2, r0, r4, lsl #9
0046f2e0: andeq r0, r0, r4, lsl #17
0046f2e4: subeq lr, r5, ip, asr #11
0046f2e8: ldrdeq r4, r5, [r0], -ip
0046f2ec: andeq r3, r0, r8, lsl r8
# _ZN7COnline11GetInstanceEv 007fd744 80
007fd744: ldr r3, [pc, #0x40]
007fd748: ldr r2, [pc, #0x40]
007fd74c: push {r4, r5, r6, lr}
007fd750: add r3, pc, r3
007fd754: ldr r4, [r3, r2]
007fd758: ldr r5, [r4]
007fd75c: cmp r5, #0
007fd760: beq #0x7fd76c
007fd764: mov r0, r5
007fd768: pop {r4, r5, r6, pc}
007fd76c: mov r1, #2
007fd770: mov r0, #0x48
007fd774: bl #0x310570
007fd778: mov r5, r0
007fd77c: bl #0x825000
007fd780: str r5, [r4]
007fd784: mov r0, r5
007fd788: pop {r4, r5, r6, pc}
007fd78c: andseq r7, sb, r0, asr #6
007fd790: andeq r1, r0, ip, lsr #16
# _ZN6CharAI11OnInitFinalEv 003d0ba4 36
003d0ba4: push {r4, lr}
003d0ba8: ldr r3, [r0, #0x1c]
003d0bac: cmp r3, #0
003d0bb0: beq #0x3d0bc4
003d0bb4: mov r0, r3
003d0bb8: ldr r3, [r3]
003d0bbc: mov lr, pc
003d0bc0: ldr pc, [r3, #0x10]
003d0bc4: pop {r4, pc}
# _ZN12VisualObject12ApplyMeshBoxEv 00470a54 48
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
# _ZN12VisualObject11CalcMeshBoxEv 0047211c 1516
0047211c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120: ldr r2, [pc, #0x5d4]
00472124: ldr r3, [r0, #0xc]
00472128: sub sp, sp, #0x64
0047212c: add r2, pc, r2
00472130: cmp r3, #0
00472134: str r2, [sp, #8]
00472138: mov r4, r0
0047213c: beq #0x472408
00472140: mov r0, r3
00472144: ldr r3, [r3]
00472148: mov lr, pc
0047214c: ldr pc, [r3, #0x30]
00472150: ldr r1, [r0]
00472154: mov r3, r0
00472158: ldr r2, [r4, #0xc]
0047215c: str r1, [r4, #0x10]
00472160: ldr r1, [r0, #4]
00472164: mov r0, r2
00472168: str r1, [r4, #0x14]
0047216c: ldr r3, [r3, #8]
00472170: str r3, [r4, #0x18]
00472174: ldr r3, [r2]
00472178: mov lr, pc
0047217c: ldr pc, [r3, #0x30]
00472180: ldr r2, [r0, #0xc]
00472184: mov r3, r0
00472188: ldr r0, [r4, #0xc]
0047218c: str r2, [r4, #0x1c]
00472190: ldr r2, [r3, #0x10]
00472194: str r2, [r4, #0x20]
00472198: ldr r3, [r3, #0x14]
0047219c: str r3, [r4, #0x24]
004721a0: bl #0x597290
004721a4: ldr r3, [r0]
004721a8: mov lr, pc
004721ac: ldr pc, [r3, #0x90]
004721b0: mov r3, r0
004721b4: ldr r1, [r0]
004721b8: ldr r0, [r4, #0x10]
004721bc: ldr r6, [r3, #4]
004721c0: ldr r5, [r3, #8]
004721c4: bl #0x30ed6c
004721c8: mov r1, r6
004721cc: str r0, [r4, #0x10]
004721d0: ldr r0, [r4, #0x14]
004721d4: bl #0x30ed6c
004721d8: mov r1, r5
004721dc: str r0, [r4, #0x14]
004721e0: ldr r0, [r4, #0x18]
004721e4: bl #0x30ed6c
004721e8: str r0, [r4, #0x18]
004721ec: ldr r0, [r4, #0xc]
004721f0: bl #0x597290
004721f4: ldr r3, [r0]
004721f8: mov lr, pc
004721fc: ldr pc, [r3, #0x90]
00472200: mov r3, r0
00472204: ldr r1, [r0]
00472208: ldr r0, [r4, #0x1c]
0047220c: ldr r6, [r3, #4]
00472210: ldr r5, [r3, #8]
00472214: bl #0x30ed6c
00472218: mov r1, r6
0047221c: str r0, [r4, #0x1c]
00472220: ldr r0, [r4, #0x20]
00472224: bl #0x30ed6c
00472228: mov r1, r5
0047222c: str r0, [r4, #0x20]
00472230: ldr r0, [r4, #0x24]
00472234: bl #0x30ed6c
00472238: str r0, [r4, #0x24]
0047223c: ldr r3, [r4, #8]
00472240: add r5, sp, #0x10
00472244: mov r6, #0
00472248: mov r0, r3
0047224c: ldr r3, [r3]
00472250: mov lr, pc
00472254: ldr pc, [r3, #0x40]
00472258: mov r2, #0x41
0047225c: mov r1, r0
00472260: mov r0, r5
00472264: strb r6, [sp, #0x50]
00472268: bl #0x30e868
0047226c: mov r3, #0
00472270: mov r1, r5
00472274: add r0, r4, #0x10
00472278: str r3, [sp, #0x48]
0047227c: str r3, [sp, #0x40]
00472280: str r3, [sp, #0x44]
00472284: strb r6, [sp, #0x50]
00472288: bl #0x312da8
0047228c: mov r1, r5
00472290: add r0, r4, #0x1c
00472294: bl #0x312da8
00472298: ldr r7, [r4, #0x1c]
0047229c: ldr r5, [r4, #0x10]
004722a0: mov r0, r7
004722a4: mov r1, r5
004722a8: bl #0x30e70c
004722ac: mov r1, r5
004722b0: cmp r0, r6
004722b4: mov r0, r7
004722b8: movne fp, r7
004722bc: moveq fp, r5
004722c0: bl #0x30e2f8
004722c4: cmp r0, #0
004722c8: ldr r6, [r4, #0x20]
004722cc: moveq r7, r5
004722d0: ldr r5, [r4, #0x14]
004722d4: mov r0, r6
004722d8: str r7, [r4, #0x1c]
004722dc: mov r1, r5
004722e0: str fp, [r4, #0x10]
004722e4: bl #0x30e70c
004722e8: mov r1, r5
004722ec: cmp r0, #0
004722f0: mov r0, r6
004722f4: movne sb, r6
004722f8: moveq sb, r5
004722fc: bl #0x30e2f8
00472300: cmp r0, #0
00472304: ldr r8, [r4, #0x18]
00472308: moveq r6, r5
0047230c: ldr r5, [r4, #0x24]
00472310: mov r1, r8
00472314: str r6, [r4, #0x20]
00472318: mov r0, r5
0047231c: str sb, [r4, #0x14]
00472320: bl #0x30e70c
00472324: mov r1, r8
00472328: cmp r0, #0
0047232c: mov r0, r5
00472330: movne sl, r5
00472334: moveq sl, r8
00472338: bl #0x30e2f8
0047233c: cmp r0, #0
00472340: moveq r5, r8
00472344: mov r1, fp
00472348: str r5, [r4, #0x24]
0047234c: mov r0, r7
00472350: str sl, [r4, #0x18]
00472354: bl #0x30e3ac
00472358: mov r1, #0x3f000000
0047235c: bl #0x30ed6c
00472360: mov r1, sb
00472364: mov r7, r0
00472368: mov r0, r6
0047236c: bl #0x30e3ac
00472370: mov r1, #0x3f000000
00472374: bl #0x30ed6c
00472378: mov r1, sl
0047237c: mov r8, r0
00472380: mov r0, r5
00472384: bl #0x30e3ac
00472388: mov r1, #0x3f000000
0047238c: bl #0x30ed6c
00472390: ldr ip, [sp, #8]
00472394: ldr r3, [pc, #0x364]
00472398: mov r6, r0
0047239c: mov r1, r7
004723a0: ldr r5, [ip, r3]
004723a4: ldr r0, [r5]
004723a8: bl #0x30e3ac
004723ac: str r0, [r4, #0x10]
004723b0: ldr r1, [r5]
004723b4: mov r0, r7
004723b8: bl #0x30eba4
004723bc: str r0, [r4, #0x1c]
004723c0: ldr r0, [r5, #4]
004723c4: mov r1, r8
004723c8: bl #0x30e3ac
004723cc: str r0, [r4, #0x14]
004723d0: ldr r1, [r5, #4]
004723d4: mov r0, r8
004723d8: bl #0x30eba4
004723dc: str r0, [r4, #0x20]
004723e0: ldr r0, [r5, #8]
004723e4: mov r1, r6
004723e8: bl #0x30e3ac
004723ec: str r0, [r4, #0x18]
004723f0: ldr r1, [r5, #8]
004723f4: mov r0, r6
004723f8: bl #0x30eba4
004723fc: str r0, [r4, #0x24]
00472400: add sp, sp, #0x64
00472404: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408: ldr ip, [sp, #8]
0047240c: ldr r0, [pc, #0x2f0]
00472410: mvn r1, #0x80000000
00472414: sub r1, r1, #0x800000
00472418: ldr r5, [ip, r0]
0047241c: mvn r2, #0x800000
00472420: str r1, [r4, #0x18]
00472424: str r1, [r4, #0x10]
00472428: str r1, [r4, #0x14]
0047242c: str r2, [r4, #0x24]
00472430: str r2, [r4, #0x1c]
00472434: str r2, [r4, #0x20]
00472438: ldr r2, [r5, #0x10]
0047243c: str r3, [sp, #0x5c]
00472440: str r3, [sp, #0x54]
00472444: str r3, [sp, #0x58]
00472448: ldr r3, [r2, #0x1c]
0047244c: add r6, sp, #0x54
00472450: movw r1, #0x6164
00472454: mov r0, r3
00472458: ldr ip, [r3]
0047245c: movt r1, #0x7365
00472460: ldr r3, [r4, #8]
00472464: mov r2, r6
00472468: mov lr, pc
0047246c: ldr pc, [ip, #0x20]
00472470: ldr r0, [sp, #0x54]
00472474: ldr r3, [sp, #0x58]
00472478: rsb r3, r0, r3
0047247c: asrs r3, r3, #2
00472480: str r3, [sp, #0xc]
00472484: beq #0x472690
00472488: ldr r2, [sp, #0xc]
0047248c: cmp r2, #0
00472490: beq #0x472680
00472494: mov r5, #0
00472498: b #0x4724a0
0047249c: ldr r0, [sp, #0x54]
004724a0: ldr r3, [r0, r5, lsl #2]
004724a4: mov r0, r3
004724a8: ldr r3, [r3]
004724ac: mov lr, pc
004724b0: ldr pc, [r3, #0x30]
004724b4: ldr r3, [sp, #0x54]
004724b8: ldr sl, [r0, #8]
004724bc: ldr fp, [r0]
004724c0: ldr r3, [r3, r5, lsl #2]
004724c4: ldr sb, [r0, #4]
004724c8: mov r0, r3
004724cc: ldr r3, [r3]
004724d0: mov lr, pc
004724d4: ldr pc, [r3, #0x30]
004724d8: ldr r2, [sp, #0x54]
004724dc: mov r3, r0
004724e0: ldr r6, [r0, #0x14]
004724e4: ldr r8, [r0, #0xc]
004724e8: ldr r0, [r2, r5, lsl #2]
004724ec: ldr r7, [r3, #0x10]
004724f0: bl #0x597290
004724f4: cmp r0, #0
004724f8: beq #0x4725bc
004724fc: ldr r3, [sp, #0x54]
00472500: ldr r0, [r3, r5, lsl #2]
00472504: bl #0x597290
00472508: ldr r3, [r0]
0047250c: mov lr, pc
00472510: ldr pc, [r3, #0x90]
00472514: mov r2, r0
00472518: ldr r3, [r2, #4]
0047251c: ldr r2, [r2, #8]
00472520: ldr r1, [r0]
00472524: mov r0, fp
00472528: stm sp, {r2, r3}
0047252c: bl #0x30ed6c
00472530: ldr r3, [sp, #4]
00472534: mov fp, r0
00472538: mov r0, sb
0047253c: mov r1, r3
00472540: bl #0x30ed6c
00472544: ldr r2, [sp]
00472548: mov sb, r0
0047254c: mov r0, sl
00472550: mov r1, r2
00472554: bl #0x30ed6c
00472558: ldr r3, [sp, #0x54]
0047255c: mov sl, r0
00472560: ldr r0, [r3, r5, lsl #2]
00472564: bl #0x597290
00472568: ldr r3, [r0]
0047256c: mov lr, pc
00472570: ldr pc, [r3, #0x90]
00472574: mov r2, r0
00472578: ldr r3, [r2, #4]
0047257c: ldr r2, [r2, #8]
00472580: ldr r1, [r0]
00472584: mov r0, r8
00472588: stm sp, {r2, r3}
0047258c: bl #0x30ed6c
00472590: ldr r3, [sp, #4]
00472594: mov r8, r0
00472598: mov r0, r7
0047259c: mov r1, r3
004725a0: bl #0x30ed6c
004725a4: ldr r2, [sp]
004725a8: mov r7, r0
004725ac: mov r0, r6
004725b0: mov r1, r2
004725b4: bl #0x30ed6c
004725b8: mov r6, r0
004725bc: ldr r3, [r4, #0x10]
004725c0: mov r1, fp
004725c4: add r5, r5, #1
004725c8: mov r0, r3
004725cc: str r3, [sp, #4]
004725d0: bl #0x30e2f8
004725d4: cmp r0, #0
004725d8: ldr r3, [sp, #4]
004725dc: movne r3, fp
004725e0: ldr fp, [r4, #0x14]
004725e4: str r3, [r4, #0x10]
004725e8: mov r1, sb
004725ec: mov r0, fp
004725f0: bl #0x30e2f8
004725f4: cmp r0, #0
004725f8: movne fp, sb
004725fc: ldr sb, [r4, #0x18]
00472600: mov r1, sl
00472604: str fp, [r4, #0x14]
00472608: mov r0, sb
0047260c: bl #0x30e2f8
00472610: cmp r0, #0
00472614: movne sb, sl
00472618: ldr sl, [r4, #0x1c]
0047261c: mov r1, r8
00472620: str sb, [r4, #0x18]
00472624: mov r0, sl
00472628: bl #0x30e70c
0047262c: cmp r0, #0
00472630: movne sl, r8
00472634: ldr r8, [r4, #0x20]
00472638: mov r1, r7
0047263c: str sl, [r4, #0x1c]
00472640: mov r0, r8
00472644: bl #0x30e70c
00472648: cmp r0, #0
0047264c: movne r8, r7
00472650: ldr r7, [r4, #0x24]
00472654: str r8, [r4, #0x20]
00472658: mov r1, r6
0047265c: mov r0, r7
00472660: bl #0x30e70c
00472664: ldr r3, [sp, #0xc]
00472668: cmp r0, #0
0047266c: movne r7, r6
00472670: cmp r5, r3
00472674: str r7, [r4, #0x24]
00472678: bne #0x47249c
0047267c: ldr r0, [sp, #0x54]
00472680: cmp r0, #0
00472684: beq #0x47223c
00472688: bl #0x310450
0047268c: b #0x47223c
00472690: ldr r3, [r5, #0x10]
00472694: movw r1, #0x6164
00472698: movt r1, #0x6d65
0047269c: ldr ip, [r3, #0x1c]
004726a0: mov r2, r6
004726a4: ldr r3, [r4, #8]
004726a8: mov r0, ip
004726ac: ldr ip, [ip]
004726b0: mov lr, pc
004726b4: ldr pc, [ip, #0x20]
004726b8: ldr r0, [sp, #0x54]
004726bc: ldr r3, [sp, #0x58]
004726c0: rsb r3, r0, r3
004726c4: asrs r3, r3, #2
004726c8: str r3, [sp, #0xc]
004726cc: bne #0x472488
004726d0: mov r3, #0
004726d4: cmp r0, #0
004726d8: str r3, [r4, #0x24]
004726dc: str r3, [r4, #0x10]
004726e0: str r3, [r4, #0x14]
004726e4: str r3, [r4, #0x18]
004726e8: str r3, [r4, #0x1c]
004726ec: str r3, [r4, #0x20]
004726f0: beq #0x472400
004726f4: bl #0x310450
004726f8: b #0x472400
004726fc: subseq r2, r2, r4, ror #18
00472700: andeq r3, r0, ip, lsr #30
00472704: strdeq r3, r4, [r0], -r4
# _ZN9Character9InitFinalEv 003b4978 588
003b4978: push {r4, r5, r6, r7, r8, lr}
003b497c: ldr r4, [pc, #0x22c]
003b4980: ldr r6, [pc, #0x22c]
003b4984: movw r3, #0x1395
003b4988: add r4, pc, r4
003b498c: ldr r2, [r4, r6]
003b4990: ldrb r1, [r0, r3]
003b4994: sub sp, sp, #0x48
003b4998: ldr r2, [r2]
003b499c: cmp r1, #0
003b49a0: mov r5, r0
003b49a4: str r2, [sp, #0x44]
003b49a8: beq #0x3b49c8
003b49ac: ldr r3, [r4, r6]
003b49b0: ldr r2, [sp, #0x44]
003b49b4: ldr r3, [r3]
003b49b8: cmp r2, r3
003b49bc: bne #0x3b4bac
003b49c0: add sp, sp, #0x48
003b49c4: pop {r4, r5, r6, r7, r8, pc}
003b49c8: mov r2, #1
003b49cc: strb r2, [r0, r3]
003b49d0: bl #0x38bd64
003b49d4: ldr r3, [r5, #0x274]
003b49d8: cmp r0, r3
003b49dc: bge #0x3b49ac
003b49e0: mov r0, r5
003b49e4: bl #0x38cd48
003b49e8: mov r0, r5
003b49ec: bl #0x3a3094
003b49f0: cmp r0, #0
003b49f4: beq #0x3b4b98
003b49f8: ldr r3, [pc, #0x1b8]
003b49fc: mov r1, #0
003b4a00: mov r2, #1
003b4a04: ldr r3, [r4, r3]
003b4a08: ldr r0, [r3, #0x40]
003b4a0c: bl #0x36e478
003b4a10: ldr r7, [r0, #0x660]
003b4a14: mov r3, #0
003b4a18: str r3, [sp, #8]
003b4a1c: cmp r7, #0
003b4a20: str r3, [sp]
003b4a24: str r3, [sp, #4]
003b4a28: beq #0x3b4a88
003b4a2c: mov r1, sp
003b4a30: mov r0, r7
003b4a34: bl #0x393ae4
003b4a38: mov r0, r7
003b4a3c: bl #0x3935dc
003b4a40: ldr r1, [r0]
003b4a44: mov r7, r0
003b4a48: ldr r0, [sp]
003b4a4c: bl #0x30eba4
003b4a50: str r0, [sp]
003b4a54: ldr r1, [r7, #4]
003b4a58: ldr r0, [sp, #4]
003b4a5c: bl #0x30eba4
003b4a60: str r0, [sp, #4]
003b4a64: ldr r1, [r7, #8]
003b4a68: ldr r0, [sp, #8]
003b4a6c: bl #0x30eba4
003b4a70: mov r1, sp
003b4a74: str r0, [sp, #8]
003b4a78: mov r2, #1
003b4a7c: mov r0, r5
003b4a80: mov r8, sp
003b4a84: bl #0x393db4
003b4a88: ldr r3, [r5, #0x2d8]
003b4a8c: cmp r3, #0
003b4a90: beq #0x3b4af8
003b4a94: ldr r3, [r5]
003b4a98: mov r0, r5
003b4a9c: mov lr, pc
003b4aa0: ldr pc, [r3, #0x28]
003b4aa4: cmp r0, #0
003b4aa8: beq #0x3b4b7c
003b4aac: ldr r3, [pc, #0x104]
003b4ab0: ldr r1, [pc, #0x104]
003b4ab4: add r7, sp, #0x2c
003b4ab8: ldr r3, [r4, r3]
003b4abc: add r1, pc, r1
003b4ac0: add r2, sp, #0x10
003b4ac4: ldr r3, [r3, #0x10]
003b4ac8: mov r0, r7
003b4acc: ldr r8, [r3, #0x1c]
003b4ad0: bl #0x3140ec
003b4ad4: add r8, r8, #0x294
003b4ad8: mov r0, r8
003b4adc: mov r1, r7
003b4ae0: bl #0x40c3cc
003b4ae4: mov r8, r0
003b4ae8: mov r0, r7
003b4aec: bl #0x318254
003b4af0: ldr r3, [r5, #0x2d8]
003b4af4: str r8, [r3, #0x40]
003b4af8: add r7, r5, #0x3c8
003b4afc: mov r0, r7
003b4b00: ldr r3, [r5, #0x3c8]
003b4b04: mov lr, pc
003b4b08: ldr pc, [r3, #0x10]
003b4b0c: ldr r3, [r5]
003b4b10: mov r0, r5
003b4b14: mov lr, pc
003b4b18: ldr pc, [r3, #0x28]
003b4b1c: cmp r0, #0
003b4b20: beq #0x3b49ac
003b4b24: ldr r3, [pc, #0x8c]
003b4b28: mov r1, r5
003b4b2c: ldr r3, [r4, r3]
003b4b30: ldr r0, [r3, #0x40]
003b4b34: bl #0x36effc
003b4b38: cmp r0, #0
003b4b3c: beq #0x3b49ac
003b4b40: mov r0, r7
003b4b44: add r7, r5, #0x560
003b4b48: bl #0x3d8894
003b4b4c: mov r0, r7
003b4b50: mov r1, #1
003b4b54: bl #0x3e0810
003b4b58: mov r0, r7
003b4b5c: mov r1, #0xc2
003b4b60: mov r2, #0
003b4b64: bl #0x3df6e0
003b4b68: bic r0, r0, r0, asr #31
003b4b6c: strb r0, [r5, #0x3a8]
003b4b70: mov r0, r5
003b4b74: bl #0x3bc4a8
003b4b78: b #0x3b49ac
003b4b7c: ldr r3, [pc, #0x34]
003b4b80: ldr r1, [pc, #0x38]
003b4b84: add r7, sp, #0x14
003b4b88: ldr r3, [r4, r3]
003b4b8c: add r1, pc, r1
003b4b90: add r2, sp, #0xc
003b4b94: b #0x3b4ac4
003b4b98: mov r0, r5
003b4b9c: bl #0x3a307c
003b4ba0: cmp r0, #0
003b4ba4: beq #0x3b4a88
003b4ba8: b #0x3b49f8
003b4bac: bl #0x30e310
003b4bb0: subseq r0, lr, r8, lsl #2
003b4bb4: andeq r4, r0, ip, lsr #1
003b4bb8: strdeq r3, r4, [r0], -r4
003b4bbc: subseq pc, r0, ip, ror r3
# _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti 0046f2f0 904
0046f2f0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046f2f4: ldr r5, [pc, #0x360]
0046f2f8: ldr r4, [pc, #0x360]
0046f2fc: sub sp, sp, #0xb4
0046f300: add r5, pc, r5
0046f304: str r4, [sp, #0x10]
0046f308: ldr lr, [r5, r4]
0046f30c: ldr ip, [pc, #0x350]
0046f310: ldr r4, [pc, #0x350]
0046f314: ldrb sb, [sp, #0xd8]
0046f318: ldr ip, [r5, ip]
0046f31c: ldr r4, [r5, r4]
0046f320: ldr lr, [lr]
0046f324: mov r7, #0
0046f328: str r4, [sp, #0xc]
0046f32c: add ip, ip, #8
0046f330: mov r4, r0
0046f334: mov sl, #0
0046f338: str r1, [r0, #4]
0046f33c: str ip, [r0]
0046f340: str r2, [r4, #8]
0046f344: str sl, [r0, #0xc]
0046f348: strb sb, [r0, #0x10]
0046f34c: str r7, [r0, #0x14]
0046f350: str r7, [r0, #0x18]
0046f354: str r7, [r0, #0x1c]
0046f358: strb r7, [r0, #0x26]
0046f35c: strb r7, [r0, #0x27]
0046f360: ldrb ip, [sp, #0xdc]
0046f364: mov r6, r2
0046f368: str lr, [sp, #0xac]
0046f36c: ldrh r2, [sp, #0xe8]
0046f370: ldrb lr, [sp, #0xe0]
0046f374: str r3, [sp, #0x1c]
0046f378: ldrh r3, [sp, #0xec]
0046f37c: ldr r0, [sp, #0xc]
0046f380: str ip, [sp, #0x20]
0046f384: str lr, [sp, #0x24]
0046f388: str r3, [sp, #0x18]
0046f38c: ldrsh r8, [sp, #0xe4]
0046f390: str r2, [sp, #0x14]
0046f394: bl #0x337888
0046f398: ldr r1, [pc, #0x2cc]
0046f39c: add fp, sp, #0x94
0046f3a0: add r2, sp, #0x90
0046f3a4: add r1, pc, r1
0046f3a8: mov r0, fp
0046f3ac: bl #0x3140ec
0046f3b0: mov r1, fp
0046f3b4: ldr r0, [sp, #0xc]
0046f3b8: bl #0x337a88
0046f3bc: mov r3, r0
0046f3c0: mov r0, fp
0046f3c4: str r3, [sp, #8]
0046f3c8: bl #0x3139ac
0046f3cc: ldr r3, [sp, #8]
0046f3d0: mvn r2, #0x298
0046f3d4: sub r2, r2, #1
0046f3d8: cmp r3, r7
0046f3dc: movne r8, r2
0046f3e0: cmp r6, r7
0046f3e4: beq #0x46f558
0046f3e8: cmp sb, r7
0046f3ec: bne #0x46f57c
0046f3f0: ldr r3, [pc, #0x278]
0046f3f4: mov r2, #1
0046f3f8: ldr r1, [r6, #0x12c]
0046f3fc: ldr r3, [r5, r3]
0046f400: ldr r0, [r6, #0x138]
0046f404: str r2, [sp, #0x30]
0046f408: add ip, r3, #8
0046f40c: movw r3, #0xcccd
0046f410: movt r3, #0x3e4c
0046f414: strh r2, [sp, #0x46]
0046f418: mvn r2, #0
0046f41c: str ip, [sp, #0x2c]
0046f420: strh r2, [sp, #0x48]
0046f424: str r3, [sp, #0x38]
0046f428: str sl, [sp, #0x40]
0046f42c: str sb, [sp, #0x8c]
0046f430: str sb, [sp, #0x34]
0046f434: str sl, [sp, #0x3c]
0046f438: strh sb, [sp, #0x4a]
0046f43c: strb sb, [sp, #0x44]
0046f440: bl #0x30e3ac
0046f444: movw r1, #0xd70a
0046f448: movt r1, #0x3c23
0046f44c: bl #0x30ed6c
0046f450: ldr r1, [r6, #0x130]
0046f454: mov sb, r0
0046f458: ldr r0, [r6, #0x13c]
0046f45c: bl #0x30e3ac
0046f460: movw r1, #0xd70a
0046f464: movt r1, #0x3c23
0046f468: bl #0x30ed6c
0046f46c: movw r1, #0xd70a
0046f470: mov r7, r0
0046f474: movt r1, #0x3c23
0046f478: ldr r0, [r6, #0x160]
0046f47c: bl #0x30ed6c
0046f480: movw r1, #0xd70a
0046f484: movt r1, #0x3c23
0046f488: mov sl, r0
0046f48c: ldr r0, [r6, #0x164]
0046f490: bl #0x30ed6c
0046f494: mov r1, #0x3f000000
0046f498: mov r6, r0
0046f49c: mov r0, sb
0046f4a0: bl #0x30ed6c
0046f4a4: mov r1, #0x3f000000
0046f4a8: mov r3, r0
0046f4ac: mov r0, r7
0046f4b0: str r3, [sp, #8]
0046f4b4: bl #0x30ed6c
0046f4b8: ldr r3, [sp, #8]
0046f4bc: add fp, sp, #0x2c
0046f4c0: mov r2, r0
0046f4c4: mov r1, r3
0046f4c8: mov r0, fp
0046f4cc: bl #0x7e44b8
0046f4d0: mov r1, r7
0046f4d4: mov r0, sb
0046f4d8: bl #0x30e70c
0046f4dc: cmp r0, #0
0046f4e0: moveq r7, sb
0046f4e4: mov r0, r7
0046f4e8: mov r1, #0x3f000000
0046f4ec: bl #0x30ed6c
0046f4f0: ldr r3, [pc, #0x17c]
0046f4f4: str r0, [r4, #0xc]
0046f4f8: mov ip, fp
0046f4fc: ldr r3, [r5, r3]
0046f500: add r3, r3, #8
0046f504: str r3, [sp, #0x2c]
0046f508: strh r8, [r4, #0x24]
0046f50c: ldr lr, [sp, #0x14]
0046f510: mov r1, ip
0046f514: mov r3, r6
0046f518: strh lr, [r4, #0x20]
0046f51c: ldr r2, [sp, #0x18]
0046f520: mov r0, r4
0046f524: strh r2, [r4, #0x22]
0046f528: ldr lr, [sp, #0x20]
0046f52c: strh r8, [ip, #0x1e]
0046f530: mov r2, sl
0046f534: strb lr, [ip, #0x18]
0046f538: ldr lr, [sp, #0x14]
0046f53c: strh lr, [ip, #0x1a]
0046f540: ldr lr, [sp, #0x18]
0046f544: strh lr, [ip, #0x1c]
0046f548: ldr ip, [sp, #0x1c]
0046f54c: ldr lr, [sp, #0x24]
0046f550: stm sp, {ip, lr}
0046f554: bl #0x46edf0
0046f558: ldr ip, [sp, #0x10]
0046f55c: ldr r2, [sp, #0xac]
0046f560: mov r0, r4
0046f564: ldr r3, [r5, ip]
0046f568: ldr r3, [r3]
0046f56c: cmp r2, r3
0046f570: bne #0x46f658
0046f574: add sp, sp, #0xb4
0046f578: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f57c: movw r3, #0xcccd
0046f580: ldr r1, [r6, #0x12c]
0046f584: ldr r0, [r6, #0x138]
0046f588: movt r3, #0x3e4c
0046f58c: mov ip, #1
0046f590: mvn lr, #0
0046f594: str r3, [sp, #0x38]
0046f598: strh ip, [sp, #0x46]
0046f59c: strh lr, [sp, #0x48]
0046f5a0: strb r7, [sp, #0x44]
0046f5a4: str r7, [sp, #0x30]
0046f5a8: str sl, [sp, #0x50]
0046f5ac: str r7, [sp, #0x34]
0046f5b0: str sl, [sp, #0x3c]
0046f5b4: str sl, [sp, #0x40]
0046f5b8: strh r7, [sp, #0x4a]
0046f5bc: str sl, [sp, #0x4c]
0046f5c0: bl #0x30e3ac
0046f5c4: movw r1, #0xd70a
0046f5c8: movt r1, #0x3c23
0046f5cc: bl #0x30ed6c
0046f5d0: ldr r1, [r6, #0x130]
0046f5d4: mov sb, r0
0046f5d8: ldr r0, [r6, #0x13c]
0046f5dc: bl #0x30e3ac
0046f5e0: movw r1, #0xd70a
0046f5e4: movt r1, #0x3c23
0046f5e8: bl #0x30ed6c
0046f5ec: movw r1, #0xd70a
0046f5f0: mov r7, r0
0046f5f4: movt r1, #0x3c23
0046f5f8: ldr r0, [r6, #0x160]
0046f5fc: bl #0x30ed6c
0046f600: movw r1, #0xd70a
0046f604: movt r1, #0x3c23
0046f608: mov sl, r0
0046f60c: ldr r0, [r6, #0x164]
0046f610: bl #0x30ed6c
0046f614: mov r1, r7
0046f618: mov r6, r0
0046f61c: mov r0, sb
0046f620: bl #0x30e70c
0046f624: cmp r0, #0
0046f628: moveq r7, sb
0046f62c: mov r0, r7
0046f630: mov r1, #0x3f000000
0046f634: bl #0x30ed6c
0046f638: ldr r3, [pc, #0x34]
0046f63c: add ip, sp, #0xb0
0046f640: str r0, [r4, #0xc]
0046f644: ldr r3, [r5, r3]
0046f648: str r0, [sp, #0x54]
0046f64c: add r3, r3, #8
0046f650: str r3, [ip, #-0x84]!
0046f654: b #0x46f508
0046f658: bl #0x30e310
# _ZN10GameObjectC1EN10ObjectBase6GO_IDSE 0038c130 616
0038c130: push {r4, r5, r6, r7, lr}
0038c134: ldr r6, [pc, #0x254]
0038c138: sub sp, sp, #0xc
0038c13c: mov r4, r0
0038c140: bl #0x33f310
0038c144: ldr r2, [pc, #0x248]
0038c148: add r6, pc, r6
0038c14c: mov r3, #0
0038c150: ldr r2, [r6, r2]
0038c154: mov r5, #0
0038c158: str r3, [r4, #0x120]
0038c15c: add r1, r2, #0xe4
0038c160: add r0, r2, #8
0038c164: add r2, r2, #0xd8
0038c168: str r2, [r4, #4]
0038c16c: str r1, [r4, #0x24]
0038c170: str r0, [r4]
0038c174: str r3, [r4, #0x124]
0038c178: str r3, [r4, #0x128]
0038c17c: str r3, [r4, #0x12c]
0038c180: str r3, [r4, #0x130]
0038c184: str r3, [r4, #0x134]
0038c188: str r3, [r4, #0x138]
0038c18c: str r3, [r4, #0x13c]
0038c190: str r3, [r4, #0x140]
0038c194: str r3, [r4, #0x144]
0038c198: str r3, [r4, #0x148]
0038c19c: str r3, [r4, #0x14c]
0038c1a0: str r3, [r4, #0x150]
0038c1a4: str r3, [r4, #0x154]
0038c1a8: str r3, [r4, #0x158]
0038c1ac: str r3, [r4, #0x160]
0038c1b0: str r3, [r4, #0x164]
0038c1b4: str r3, [r4, #0x168]
0038c1b8: str r3, [r4, #0x16c]
0038c1bc: str r3, [r4, #0x170]
0038c1c0: str r3, [r4, #0x174]
0038c1c4: str r3, [r4, #0x178]
0038c1c8: str r3, [r4, #0x184]
0038c1cc: str r3, [r4, #0x188]
0038c1d0: str r3, [r4, #0x18c]
0038c1d4: str r3, [r4, #0x190]
0038c1d8: str r3, [r4, #0x194]
0038c1dc: strb r5, [r4, #0x15c]
0038c1e0: str r5, [r4, #0x180]
0038c1e4: add r0, r4, #0x1c8
0038c1e8: str r3, [r4, #0x198]
0038c1ec: str r3, [r4, #0x1c0]
0038c1f0: str r3, [r4, #0x19c]
0038c1f4: str r3, [r4, #0x1a0]
0038c1f8: str r3, [r4, #0x1a4]
0038c1fc: str r3, [r4, #0x1a8]
0038c200: str r3, [r4, #0x1ac]
0038c204: str r3, [r4, #0x1b0]
0038c208: strb r5, [r4, #0x1b4]
0038c20c: strb r5, [r4, #0x1b5]
0038c210: str r3, [r4, #0x1b8]
0038c214: str r3, [r4, #0x1bc]
0038c218: strb r5, [r4, #0x1c4]
0038c21c: bl #0x524644
0038c220: add r3, r4, #0x278
0038c224: mvn r7, #0
0038c228: mov r2, #0x64
0038c22c: str r2, [r4, #0x274]
0038c230: mov r0, r3
0038c234: str r3, [r4, #0x288]
0038c238: str r3, [r4, #0x28c]
0038c23c: str r5, [r4, #0x26c]
0038c240: str r7, [r4, #0x270]
0038c244: mov r1, #0x10
0038c248: bl #0x31167c
0038c24c: ldr r2, [r4, #0x288]
0038c250: add r3, r4, #0x290
0038c254: mov r0, r3
0038c258: strb r5, [r2]
0038c25c: mov r1, #0x10
0038c260: str r3, [r4, #0x2a0]
0038c264: str r3, [r4, #0x2a4]
0038c268: bl #0x31167c
0038c26c: ldr r2, [r4, #0x2a0]
0038c270: add r3, r4, #0x2a8
0038c274: mov r0, r3
0038c278: strb r5, [r2]
0038c27c: mov r1, #0x10
0038c280: str r3, [r4, #0x2b8]
0038c284: str r3, [r4, #0x2bc]
0038c288: bl #0x31167c
0038c28c: ldr r2, [r4, #0x2b8]
0038c290: add r3, r4, #0x2c0
0038c294: mov r0, r3
0038c298: strb r5, [r2]
0038c29c: mov r1, #0x10
0038c2a0: str r3, [r4, #0x2d0]
0038c2a4: str r3, [r4, #0x2d4]
0038c2a8: bl #0x31167c
0038c2ac: ldr r3, [r4, #0x2d0]
0038c2b0: mov ip, #1
0038c2b4: add r6, r4, #0x304
0038c2b8: strb r5, [r3]
0038c2bc: mov r2, ip
0038c2c0: strb ip, [r4, #0x2ee]
0038c2c4: strb ip, [r4, #0x2fb]
0038c2c8: str r5, [r4, #0x2d8]
0038c2cc: str r5, [r4, #0x2dc]
0038c2d0: str r5, [r4, #0x2e0]
0038c2d4: str r5, [r4, #0x2e4]
0038c2d8: str r5, [r4, #0x2e8]
0038c2dc: strb r5, [r4, #0x2ec]
0038c2e0: strb r5, [r4, #0x2ed]
0038c2e4: strb r5, [r4, #0x2ef]
0038c2e8: strb r5, [r4, #0x2f0]
0038c2ec: str r5, [r4, #0x2f4]
0038c2f0: strb r5, [r4, #0x2f8]
0038c2f4: strb r5, [r4, #0x2f9]
0038c2f8: strb r5, [r4, #0x2fa]
0038c2fc: strb r5, [r4, #0x2fc]
0038c300: str r5, [r4, #0x300]
0038c304: mov r3, r5
0038c308: mov r1, r5
0038c30c: mov r0, r6
0038c310: str ip, [sp]
0038c314: bl #0x4a2730
0038c318: add r3, r4, #0x358
0038c31c: mov r0, r3
0038c320: str r3, [r4, #0x368]
0038c324: str r3, [r4, #0x36c]
0038c328: mov r1, #0x10
0038c32c: bl #0x31167c
0038c330: ldr r1, [r4, #0x368]
0038c334: mov r2, #0xc2000000
0038c338: mov r3, #0x42000000
0038c33c: strb r5, [r1]
0038c340: add r2, r2, #0xc80000
0038c344: add r3, r3, #0xc80000
0038c348: mov r1, #0x370
0038c34c: strh r7, [r4, r1]
0038c350: mov r0, r4
0038c354: str r2, [r4, #0x14c]
0038c358: str r3, [r4, #0x158]
0038c35c: str r2, [r4, #0x144]
0038c360: str r2, [r4, #0x148]
0038c364: str r3, [r4, #0x150]
0038c368: str r3, [r4, #0x154]
0038c36c: strb r5, [r4, #0x373]
0038c370: strb r5, [r4, #0x372]
0038c374: bl #0x38aac8
0038c378: mov r0, r6
0038c37c: mov r1, r4
0038c380: bl #0x4a191c
0038c384: mov r0, r4
0038c388: add sp, sp, #0xc
0038c38c: pop {r4, r5, r6, r7, pc}
0038c390: rsbeq r8, r0, r8, asr #18
0038c394: andeq r2, r0, r0, ror sp
# _ZN10GameObject21CheckSpawnProbabilityEv 0038bd64 248
0038bd64: push {r4, r5, lr}
0038bd68: sub sp, sp, #0x14
0038bd6c: add r4, sp, #4
0038bd70: mov r1, r0
0038bd74: mov r5, r0
0038bd78: mov r0, r4
0038bd7c: bl #0x33dd2c
0038bd80: mov r0, r4
0038bd84: bl #0x33ff54
0038bd88: ldr r4, [pc, #0xc4]
0038bd8c: subs r3, r0, #0
0038bd90: add r4, pc, r4
0038bd94: beq #0x38bdbc
0038bd98: ldr r3, [r3]
0038bd9c: mov lr, pc
0038bda0: ldr pc, [r3, #0x28]
0038bda4: cmp r0, #0
0038bda8: beq #0x38bdbc
0038bdac: mvn r0, #1
0038bdb0: str r0, [r5, #0x270]
0038bdb4: add sp, sp, #0x14
0038bdb8: pop {r4, r5, pc}
0038bdbc: ldr r0, [r5, #0x270]
0038bdc0: cmn r0, #1
0038bdc4: bne #0x38bdb4
0038bdc8: bl #0x7fd794
0038bdcc: ldrb r3, [r0, #5]
0038bdd0: cmp r3, #0
0038bdd4: beq #0x38be44
0038bdd8: ldr r3, [r5, #0x108]
0038bddc: cmn r3, #1
0038bde0: beq #0x38be44
0038bde4: ldr r0, [r5, #0xfc]
0038bde8: subs r0, r0, #0
0038bdec: movne r0, #1
0038bdf0: bl #0x38bc3c
0038bdf4: str r0, [r5, #0x270]
0038bdf8: ldr r3, [r5, #0x274]
0038bdfc: cmp r0, r3
0038be00: blt #0x38bdac
0038be04: mov r1, #0
0038be08: ldr r3, [r5]
0038be0c: mov r0, r5
0038be10: mov lr, pc
0038be14: ldr pc, [r3, #0x40]
0038be18: mov r0, r5
0038be1c: bl #0x33ddb4
0038be20: mov r3, #0
0038be24: strb r3, [r5, #0x82]
0038be28: ldr r3, [pc, #0x28]
0038be2c: mov r1, r5
0038be30: ldr r3, [r4, r3]
0038be34: ldr r0, [r3, #0x38]
0038be38: bl #0x3432f8
0038be3c: ldr r0, [r5, #0x270]
0038be40: b #0x38bdb4
0038be44: mov r0, #0
0038be48: bl #0x38bc3c
0038be4c: str r0, [r5, #0x270]
0038be50: b #0x38bdf8
0038be54: rsbeq r8, r0, r0, lsl #26
0038be58: strdeq r3, r4, [r0], -r4
# _ZN10GameObject9InitFinalEv 0038cd48 408
0038cd48: push {r4, r5, r6, r7, r8, lr}
0038cd4c: ldr r5, [pc, #0x178]
0038cd50: ldr r6, [pc, #0x178]
0038cd54: sub sp, sp, #0x28
0038cd58: add r5, pc, r5
0038cd5c: ldr r3, [r5, r6]
0038cd60: mov r4, r0
0038cd64: ldr r3, [r3]
0038cd68: str r3, [sp, #0x24]
0038cd6c: bl #0x38bd64
0038cd70: ldr r3, [r4, #0x274]
0038cd74: cmp r0, r3
0038cd78: bge #0x38ce94
0038cd7c: ldrb r3, [r4, #0x81]
0038cd80: cmp r3, #0
0038cd84: bne #0x38ce94
0038cd88: ldrb r3, [r4, #0x2ed]
0038cd8c: cmp r3, #0
0038cd90: bne #0x38ceb0
0038cd94: ldr r1, [r4, #0x144]
0038cd98: ldr r0, [r4, #0x150]
0038cd9c: bl #0x30e3ac
0038cda0: ldr r1, [r4, #0x148]
0038cda4: mov r7, r0
0038cda8: ldr r0, [r4, #0x154]
0038cdac: bl #0x30e3ac
0038cdb0: mov r8, r0
0038cdb4: mov r1, r8
0038cdb8: mov r0, r7
0038cdbc: bl #0x30e70c
0038cdc0: cmp r0, #0
0038cdc4: ldr r0, [pc, #0x108]
0038cdc8: movne r7, r8
0038cdcc: ldrb r2, [r4, #0x84]
0038cdd0: add r1, r4, #0x1c8
0038cdd4: add r3, r4, #0x160
0038cdd8: ldr r0, [r5, r0]
0038cddc: str r7, [sp]
0038cde0: str r4, [sp, #4]
0038cde4: bl #0x526b18
0038cde8: ldr r7, [r4, #0x44]
0038cdec: mov r0, r7
0038cdf0: bl #0x30de54
0038cdf4: mov r1, r7
0038cdf8: add r2, r7, r0
0038cdfc: add r0, r4, #0x254
0038ce00: bl #0x3109e0
0038ce04: ldr r0, [r4, #0x2d8]
0038ce08: cmp r0, #0
0038ce0c: beq #0x38ce8c
0038ce10: bl #0x38ba74
0038ce14: ldr r3, [pc, #0xbc]
0038ce18: add r7, sp, #0xc
0038ce1c: ldr r2, [r4, #0x288]
0038ce20: ldr r3, [r5, r3]
0038ce24: ldr r1, [r4, #0x28c]
0038ce28: mov r0, r7
0038ce2c: ldr r3, [r3, #0x10]
0038ce30: ldr r8, [r3, #0x1c]
0038ce34: str r7, [sp, #0x1c]
0038ce38: str r7, [sp, #0x20]
0038ce3c: add r8, r8, #0x294
0038ce40: bl #0x3116e8
0038ce44: mov r0, r8
0038ce48: mov r1, r7
0038ce4c: bl #0x40c3cc
0038ce50: mov r8, r0
0038ce54: mov r0, r7
0038ce58: bl #0x318254
0038ce5c: ldr r3, [r4, #0x2d8]
0038ce60: str r8, [r3, #0x40]
0038ce64: ldr r3, [r4, #0x2d8]
0038ce68: cmp r3, #0
0038ce6c: beq #0x38ce8c
0038ce70: ldr r0, [r3, #8]
0038ce74: cmp r0, #0
0038ce78: beq #0x38ce8c
0038ce7c: ldr r1, [pc, #0x58]
0038ce80: add r1, pc, r1
0038ce84: bl #0x5984f4
0038ce88: str r0, [r4, #0x180]
0038ce8c: mov r0, r4
0038ce90: bl #0x393ea0
0038ce94: ldr r3, [r5, r6]
0038ce98: ldr r2, [sp, #0x24]
0038ce9c: ldr r3, [r3]
0038cea0: cmp r2, r3
0038cea4: bne #0x38cec8
0038cea8: add sp, sp, #0x28
0038ceac: pop {r4, r5, r6, r7, r8, pc}
0038ceb0: ldr r3, [r4]
0038ceb4: mov r0, r4
0038ceb8: mov r1, #1
0038cebc: mov lr, pc
0038cec0: ldr pc, [r3, #0x40]
0038cec4: b #0x38cd94
0038cec8: bl #0x30e310
0038cecc: rsbeq r7, r0, r8, lsr sp
0038ced0: andeq r4, r0, ip, lsr #1
0038ced4: andeq r1, r0, r4, lsl #4
0038ced8: strdeq r3, r4, [r0], -r4
0038cedc: ldrsheq r5, [r3], #-0x58
# _ZN9CharacterC1EN10ObjectBase6GO_IDSE 003aa1b4 1448
003aa1b4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add ip, r0, #0x374
003aa1bc: sub sp, sp, #0x3c
003aa1c0: mov r4, r0
003aa1c4: str ip, [sp, #0xc]
003aa1c8: bl #0x38c398
003aa1cc: ldr ip, [sp, #0xc]
003aa1d0: add r5, r4, #0x4f0
003aa1d4: add r5, r5, #0xc
003aa1d8: mov r0, ip
003aa1dc: bl #0x404db8
003aa1e0: add r0, r4, #0x3b4
003aa1e4: str r0, [sp, #0x20]
003aa1e8: add r0, r4, #0x37c
003aa1ec: bl #0x3ff330
003aa1f0: add r2, r4, #0x490
003aa1f4: add r1, r4, #0x3c8
003aa1f8: add r2, r2, #0xc
003aa1fc: ldr r0, [sp, #0x20]
003aa200: str r1, [sp, #0x1c]
003aa204: str r2, [sp, #0x14]
003aa208: bl #0x3dbb0c
003aa20c: ldr r0, [sp, #0x1c]
003aa210: bl #0x3cebf0
003aa214: ldr r0, [sp, #0x14]
003aa218: bl #0x3c8ff4
003aa21c: add r3, r4, #0x560
003aa220: mov r0, r5
003aa224: str r3, [sp, #0x18]
003aa228: ldr sb, [pc, #0x50c]
003aa22c: bl #0x3c1b58
003aa230: ldr r0, [sp, #0x18]
003aa234: bl #0x3df084
003aa238: ldr lr, [pc, #0x500]
003aa23c: add sb, pc, sb
003aa240: mov r8, #0
003aa244: ldr lr, [sb, lr]
003aa248: mov fp, #1
003aa24c: mvn r6, #0
003aa250: add sl, lr, #0x324
003aa254: str sl, [sp, #0x34]
003aa258: add sl, lr, #0x180
003aa25c: str sl, [sp, #0x10]
003aa260: add sl, lr, #0x1f4
003aa264: str sl, [sp, #0x24]
003aa268: add sl, lr, #0x220
003aa26c: str sl, [sp, #0x28]
003aa270: add sl, lr, #0x230
003aa274: str sl, [sp, #0x2c]
003aa278: add r0, lr, #8
003aa27c: add r1, lr, #0x15c
003aa280: add r2, lr, #0x168
003aa284: add sl, lr, #0x304
003aa288: str sl, [sp, #0x30]
003aa28c: stm r4, {r0, r1}
003aa290: str r2, [r4, #0x24]
003aa294: ldr r0, [sp, #0x10]
003aa298: add lr, lr, #0x314
003aa29c: add r7, r4, #0x1380
003aa2a0: str r0, [r4, #0x374]
003aa2a4: ldr r1, [sp, #0x24]
003aa2a8: add r3, r7, #0x18
003aa2ac: movw sl, #0x13a8
003aa2b0: str r1, [r4, #0x37c]
003aa2b4: ldr r2, [sp, #0x28]
003aa2b8: add r7, r7, #0x30
003aa2bc: str r2, [r4, #0x3b4]
003aa2c0: ldr r0, [sp, #0x2c]
003aa2c4: str r0, [r4, #0x3c8]
003aa2c8: ldr r1, [sp, #0x30]
003aa2cc: str lr, [r4, #0x4fc]
003aa2d0: mov r0, r3
003aa2d4: str r1, [r4, #0x49c]
003aa2d8: ldr r2, [sp, #0x34]
003aa2dc: mov r1, #0x10
003aa2e0: str r2, [r4, #0x560]
003aa2e4: movw r2, #0x1394
003aa2e8: strb r8, [r4, r2]
003aa2ec: movw r2, #0x1395
003aa2f0: strb r8, [r4, r2]
003aa2f4: movw r2, #0x1396
003aa2f8: strb fp, [r4, r2]
003aa2fc: movw r2, #0x1397
003aa300: strb r6, [r4, r2]
003aa304: movw r2, #0x13ac
003aa308: str r3, [r4, r2]
003aa30c: str r3, [r4, sl]
003aa310: bl #0x31167c
003aa314: ldr r3, [r4, sl]
003aa318: mov sl, #0x13c0
003aa31c: mov r0, r7
003aa320: strb r8, [r3]
003aa324: movw r3, #0x13c4
003aa328: str r7, [r4, r3]
003aa32c: mov r1, #0x10
003aa330: str r7, [r4, sl]
003aa334: bl #0x31167c
003aa338: ldr r2, [r4, sl]
003aa33c: add r7, r4, sl
003aa340: add r3, r7, #0xc
003aa344: strb r8, [r2]
003aa348: movw r2, #0x13c8
003aa34c: strh r6, [r4, r2]
003aa350: movw r2, #0x13ca
003aa354: strh r6, [r4, r2]
003aa358: movw sl, #0x13dc
003aa35c: movw r2, #0x13e0
003aa360: str r3, [r4, r2]
003aa364: mov r0, r3
003aa368: str r3, [r4, sl]
003aa36c: mov r1, #0x10
003aa370: bl #0x31167c
003aa374: ldr r3, [r4, sl]
003aa378: add r7, r7, #0x28
003aa37c: movw sl, #0x13f8
003aa380: strb r8, [r3]
003aa384: movw r3, #0x13e4
003aa388: strb fp, [r4, r3]
003aa38c: movw r3, #0x13fc
003aa390: str r7, [r4, r3]
003aa394: mov r0, r7
003aa398: str r7, [r4, sl]
003aa39c: mov r1, #0x10
003aa3a0: bl #0x31167c
003aa3a4: ldr r3, [r4, sl]
003aa3a8: add r7, r4, #0x1400
003aa3ac: movw sl, #0x1410
003aa3b0: strb r8, [r3]
003aa3b4: movw r3, #0x1414
003aa3b8: str r7, [r4, r3]
003aa3bc: mov r0, r7
003aa3c0: str r7, [r4, sl]
003aa3c4: mov r1, #0x10
003aa3c8: bl #0x31167c
003aa3cc: ldr r3, [r4, sl]
003aa3d0: add r7, r7, #0x18
003aa3d4: movw sl, #0x1428
003aa3d8: strb r8, [r3]
003aa3dc: movw r3, #0x142c
003aa3e0: str r7, [r4, r3]
003aa3e4: mov r0, r7
003aa3e8: str r7, [r4, sl]
003aa3ec: mov r1, #0x10
003aa3f0: bl #0x31167c
003aa3f4: ldr r2, [r4, sl]
003aa3f8: mov r3, #0
003aa3fc: mov r1, #0xbf000000
003aa400: strb r8, [r2]
003aa404: movw r2, #0x14a8
003aa408: strb r6, [r4, r2]
003aa40c: movw r2, #0x1430
003aa410: strb fp, [r4, r2]
003aa414: movw r2, #0x1434
003aa418: str r8, [r4, r2]
003aa41c: movw r2, #0x1438
003aa420: str r8, [r4, r2]
003aa424: movw r2, #0x1448
003aa428: strb fp, [r4, r2]
003aa42c: movw r2, #0x1449
003aa430: strb r8, [r4, r2]
003aa434: movw r2, #0x144c
003aa438: str r8, [r4, r2]
003aa43c: movw r2, #0x1450
003aa440: str r3, [r4, r2]
003aa444: movw r2, #0x1454
003aa448: str r3, [r4, r2]
003aa44c: movw r2, #0x1458
003aa450: str r3, [r4, r2]
003aa454: movw r2, #0x145c
003aa458: str r3, [r4, r2]
003aa45c: movw r2, #0x1460
003aa460: str r3, [r4, r2]
003aa464: movw r2, #0x1464
003aa468: str r3, [r4, r2]
003aa46c: movw r2, #0x1468
003aa470: str r3, [r4, r2]
003aa474: movw r2, #0x146c
003aa478: str r3, [r4, r2]
003aa47c: movw r2, #0x1470
003aa480: str r3, [r4, r2]
003aa484: movw r2, #0x1474
003aa488: str r3, [r4, r2]
003aa48c: movw r2, #0x1478
003aa490: str r3, [r4, r2]
003aa494: movw r2, #0x147c
003aa498: str r3, [r4, r2]
003aa49c: mov r2, #0x1480
003aa4a0: strb r8, [r4, r2]
003aa4a4: movw r2, #0x1481
003aa4a8: strb r8, [r4, r2]
003aa4ac: movw r2, #0x1484
003aa4b0: str r8, [r4, r2]
003aa4b4: movw r2, #0x1488
003aa4b8: str r8, [r4, r2]
003aa4bc: movw r2, #0x148c
003aa4c0: str r8, [r4, r2]
003aa4c4: movw r2, #0x1490
003aa4c8: str r8, [r4, r2]
003aa4cc: movw r2, #0x1494
003aa4d0: str r8, [r4, r2]
003aa4d4: movw r2, #0x1498
003aa4d8: str r6, [r4, r2]
003aa4dc: movw r2, #0x149c
003aa4e0: str r8, [r4, r2]
003aa4e4: movw r2, #0x14a0
003aa4e8: str r8, [r4, r2]
003aa4ec: movw r2, #0x14a4
003aa4f0: str r8, [r4, r2]
003aa4f4: movw r2, #0x14aa
003aa4f8: strh r8, [r4, r2]
003aa4fc: movw r2, #0x14ac
003aa500: strb r8, [r4, r2]
003aa504: movw r2, #0x14d8
003aa508: str r3, [r4, r2]
003aa50c: add r1, r1, #0x800000
003aa510: movw r2, #0x14fc
003aa514: str r1, [r4, r2]
003aa518: movw r2, #0x1504
003aa51c: str r6, [r4, r2]
003aa520: movw r2, #0x14ad
003aa524: strb r8, [r4, r2]
003aa528: movw r2, #0x14b0
003aa52c: str r3, [r4, r2]
003aa530: movw r2, #0x14b4
003aa534: str r3, [r4, r2]
003aa538: movw r2, #0x14b8
003aa53c: str r3, [r4, r2]
003aa540: movw r2, #0x14bc
003aa544: str r3, [r4, r2]
003aa548: mov r2, #0x14c0
003aa54c: str r3, [r4, r2]
003aa550: movw r2, #0x14c4
003aa554: str r3, [r4, r2]
003aa558: movw r3, #0x14c8
003aa55c: strb r8, [r4, r3]
003aa560: movw r3, #0x14ca
003aa564: strh r6, [r4, r3]
003aa568: movw r3, #0x14cc
003aa56c: str r8, [r4, r3]
003aa570: movw r3, #0x14d0
003aa574: strh r8, [r4, r3]
003aa578: movw r3, #0x14d4
003aa57c: str r8, [r4, r3]
003aa580: movw r3, #0x14dc
003aa584: strb r8, [r4, r3]
003aa588: movw r3, #0x14e4
003aa58c: strb r8, [r4, r3]
003aa590: movw r3, #0x14e5
003aa594: strb r8, [r4, r3]
003aa598: movw r3, #0x14e8
003aa59c: str r8, [r4, r3]
003aa5a0: movw r3, #0x14ec
003aa5a4: str r8, [r4, r3]
003aa5a8: add r7, r4, #0x1500
003aa5ac: movw r3, #0x14f0
003aa5b0: add r0, r4, #0x1a40
003aa5b4: strb r8, [r4, r3]
003aa5b8: add r0, r0, #8
003aa5bc: mov r3, #0x1500
003aa5c0: add r7, r7, #8
003aa5c4: str r6, [r4, r3]
003aa5c8: str r0, [sp, #0x10]
003aa5cc: mov r0, r7
003aa5d0: bl #0x3a6a24
003aa5d4: ldr r0, [sp, #0x10]
003aa5d8: bl #0x3a6a24
003aa5dc: add r0, r4, #0x304
003aa5e0: mov r1, r4
003aa5e4: strb fp, [r4, #0x28]
003aa5e8: bl #0x4a191c
003aa5ec: strb fp, [r4, #0x1c4]
003aa5f0: strb fp, [r4, #0x85]
003aa5f4: mov r0, #0x10
003aa5f8: mov r1, r8
003aa5fc: bl #0x310570
003aa600: ldr r3, [pc, #0x13c]
003aa604: ldr ip, [sp, #0xc]
003aa608: mov r6, r0
003aa60c: ldr r3, [sb, r3]
003aa610: cmp ip, r8
003aa614: strb r8, [r6, #0xa]
003aa618: add r3, r3, #8
003aa61c: str r8, [r0, #0xc]
003aa620: stm r0, {r3, ip}
003aa624: strb r8, [r6, #8]
003aa628: strb r8, [r6, #9]
003aa62c: beq #0x3aa6e0
003aa630: mov r0, ip
003aa634: mov r1, r6
003aa638: bl #0x404e10
003aa63c: ldr r3, [r4, #0x378]
003aa640: ldr r0, [sp, #0x20]
003aa644: mov r1, r4
003aa648: str r4, [r3, #0xc]
003aa64c: bl #0x3db480
003aa650: ldr r0, [sp, #0x1c]
003aa654: mov r1, r4
003aa658: bl #0x3cb7c0
003aa65c: ldr r0, [sp, #0x14]
003aa660: mov r1, r4
003aa664: bl #0x3c9890
003aa668: mov r0, r5
003aa66c: mov r1, r4
003aa670: bl #0x3c1600
003aa674: ldr r0, [sp, #0x18]
003aa678: mov r1, r4
003aa67c: bl #0x3dec0c
003aa680: mov r6, #0
003aa684: str r4, [r4, #0x380]
003aa688: mov r1, r6
003aa68c: mov r0, r5
003aa690: add r6, r6, #1
003aa694: bl #0x3c7318
003aa698: cmp r6, #0x14
003aa69c: bne #0x3aa688
003aa6a0: mov r1, #0
003aa6a4: movw r2, #0x14e0
003aa6a8: str r1, [r4, r2]
003aa6ac: mvn r3, #0
003aa6b0: movw r2, #0x14f4
003aa6b4: str r3, [r4, r2]
003aa6b8: str r7, [r4, #0x100]
003aa6bc: ldr sl, [sp, #0x10]
003aa6c0: movw r2, #0x14f8
003aa6c4: mov r0, r4
003aa6c8: str sl, [r4, #0x104]
003aa6cc: str r3, [r4, r2]
003aa6d0: mov r3, #1
003aa6d4: strb r3, [r4, #0xf8]
003aa6d8: add sp, sp, #0x3c
003aa6dc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr r3, [pc, #0x60]
003aa6e4: ldr r3, [sb, r3]
003aa6e8: ldr r3, [r3]
003aa6ec: cmp r3, #2
003aa6f0: streq ip, [r4, #0x374]
003aa6f4: beq #0x3aa630
003aa6f8: cmp r3, #1
003aa6fc: bne #0x3aa630
003aa700: ldr r0, [pc, #0x44]
003aa704: ldr r1, [pc, #0x44]
003aa708: ldr r2, [pc, #0x44]
003aa70c: ldr r0, [sb, r0]
003aa710: ldr r3, [pc, #0x40]
003aa714: mov lr, #0x44
003aa718: add r1, pc, r1
003aa71c: add r0, r0, #0xa8
003aa720: add r2, pc, r2
003aa724: add r3, pc, r3
003aa728: str ip, [sp, #0xc]
003aa72c: str lr, [sp]
003aa730: bl #0x30e004
003aa734: ldr ip, [sp, #0xc]
003aa738: b #0x3aa630
003aa73c: subseq sl, lr, r4, asr r8
003aa740: andeq r2, r0, r8, lsl #28
003aa744: andeq r2, r0, r4, lsr #21
003aa748: andeq r3, r0, r0, asr #19
003aa74c: andeq r1, r0, r0, asr #19
003aa750: subseq r3, r1, r0, asr #25
003aa754: subseq r8, r1, r0, lsr #27
003aa758: subseq r8, r1, ip, lsr #27
# _ZN7COnlineC2Ev 007fd838 160
007fd838: push {r4, r5, r6, lr}
007fd83c: ldr r5, [pc, #0x84]
007fd840: ldr r2, [pc, #0x84]
007fd844: ldr r3, [pc, #0x84]
007fd848: add r5, pc, r5
007fd84c: ldr r2, [r5, r2]
007fd850: ldr r3, [r5, r3]
007fd854: mov r6, #0
007fd858: add r2, r2, #8
007fd85c: add r3, r3, #8
007fd860: mov r4, r0
007fd864: str r2, [r0]
007fd868: str r3, [r0, #8]
007fd86c: strb r6, [r0, #4]
007fd870: strb r6, [r0, #5]
007fd874: add r0, r0, #0xc
007fd878: bl #0x80e398
007fd87c: ldr r3, [pc, #0x50]
007fd880: add r2, r4, #0x10
007fd884: str r2, [r4, #0x14]
007fd888: ldr r3, [r5, r3]
007fd88c: str r6, [r4, #0x28]
007fd890: str r2, [r4, #0x10]
007fd894: add r3, r3, #8
007fd898: str r3, [r4, #8]
007fd89c: mov r3, #1
007fd8a0: strb r3, [r4, #0x30]
007fd8a4: str r6, [r4, #0x18]
007fd8a8: str r6, [r4, #0x1c]
007fd8ac: add r0, r4, #0x34
007fd8b0: bl #0x80e398
007fd8b4: add r3, r4, #0x38
007fd8b8: str r3, [r4, #0x3c]
007fd8bc: str r3, [r4, #0x38]
007fd8c0: mov r0, r4
007fd8c4: pop {r4, r5, r6, pc}
007fd8c8: andseq r7, sb, r8, asr #4
007fd8cc: andeq r2, r0, r8, lsl #23
007fd8d0: andeq r0, r0, ip, asr #20
007fd8d4: andeq r3, r0, r0, lsr #9
# _Z9GetOnlinev 007fd794 4
007fd794: b #0x7fd744
# _ZN10GameObject8InitPostEv 0038be5c 560
0038be5c: push {r4, r5, r6, r7, r8, lr}
0038be60: mov r4, r0
0038be64: bl #0x33ec0c
0038be68: mov r0, r4
0038be6c: bl #0x38bd64
0038be70: ldr r3, [r4, #0x274]
0038be74: ldr r5, [pc, #0x204]
0038be78: cmp r0, r3
0038be7c: add r5, pc, r5
0038be80: bge #0x38c02c
0038be84: ldr r0, [r4, #0x120]
0038be88: mov r3, #0
0038be8c: movw r1, #0xb717
0038be90: str r3, [r4, #0x2dc]
0038be94: movt r1, #0x38d1
0038be98: bic r0, r0, #0x80000000
0038be9c: bl #0x30e70c
0038bea0: cmp r0, #0
0038bea4: ldr r0, [r4, #0x124]
0038bea8: movne r3, #0x3f800000
0038beac: movw r1, #0xb717
0038beb0: strne r3, [r4, #0x120]
0038beb4: movt r1, #0x38d1
0038beb8: bic r0, r0, #0x80000000
0038bebc: bl #0x30e70c
0038bec0: cmp r0, #0
0038bec4: ldr r0, [r4, #0x128]
0038bec8: movne r3, #0x3f800000
0038becc: movw r1, #0xb717
0038bed0: strne r3, [r4, #0x124]
0038bed4: movt r1, #0x38d1
0038bed8: bic r0, r0, #0x80000000
0038bedc: bl #0x30e70c
0038bee0: cmp r0, #0
0038bee4: movne r3, #0x3f800000
0038bee8: movw r1, #0xfa35
0038beec: strne r3, [r4, #0x128]
0038bef0: ldr r0, [r4, #0x16c]
0038bef4: movt r1, #0x3c8e
0038bef8: bl #0x30ed6c
0038befc: movw r1, #0xfa35
0038bf00: str r0, [r4, #0x16c]
0038bf04: movt r1, #0x3c8e
0038bf08: ldr r0, [r4, #0x170]
0038bf0c: bl #0x30ed6c
0038bf10: movw r1, #0xfa35
0038bf14: str r0, [r4, #0x170]
0038bf18: movt r1, #0x3c8e
0038bf1c: ldr r0, [r4, #0x174]
0038bf20: bl #0x30ed6c
0038bf24: mov r2, #1
0038bf28: str r0, [r4, #0x178]
0038bf2c: str r0, [r4, #0x174]
0038bf30: add r1, r4, #0x160
0038bf34: mov r0, r4
0038bf38: bl #0x393db4
0038bf3c: ldr r0, [r4, #0x144]
0038bf40: ldr r1, [r4, #0x120]
0038bf44: bl #0x30ed6c
0038bf48: ldr r1, [r4, #0x124]
0038bf4c: str r0, [r4, #0x144]
0038bf50: ldr r0, [r4, #0x148]
0038bf54: bl #0x30ed6c
0038bf58: ldr r1, [r4, #0x128]
0038bf5c: str r0, [r4, #0x148]
0038bf60: ldr r0, [r4, #0x14c]
0038bf64: bl #0x30ed6c
0038bf68: ldr r1, [r4, #0x120]
0038bf6c: str r0, [r4, #0x14c]
0038bf70: ldr r0, [r4, #0x150]
0038bf74: bl #0x30ed6c
0038bf78: ldr r1, [r4, #0x124]
0038bf7c: str r0, [r4, #0x150]
0038bf80: ldr r0, [r4, #0x154]
0038bf84: bl #0x30ed6c
0038bf88: ldr r1, [r4, #0x128]
0038bf8c: str r0, [r4, #0x154]
0038bf90: ldr r0, [r4, #0x158]
0038bf94: bl #0x30ed6c
0038bf98: str r0, [r4, #0x158]
0038bf9c: mov r0, r4
0038bfa0: bl #0x38aac8
0038bfa4: ldr r0, [r4, #0x2d8]
0038bfa8: cmp r0, #0
0038bfac: beq #0x38c040
0038bfb0: bl #0x38ba74
0038bfb4: ldrb r3, [r4, #0x15c]
0038bfb8: ldr r7, [r4, #0x36c]
0038bfbc: cmp r3, #0
0038bfc0: movne r3, #0
0038bfc4: strbne r3, [r4, #0x28]
0038bfc8: ldr r3, [r4, #0x368]
0038bfcc: cmp r3, r7
0038bfd0: beq #0x38c02c
0038bfd4: ldr r3, [pc, #0xa8]
0038bfd8: ldr r3, [r5, r3]
0038bfdc: ldr r8, [r3]
0038bfe0: cmp r8, #0
0038bfe4: beq #0x38c030
0038bfe8: ldr r3, [pc, #0x98]
0038bfec: mov r6, #0
0038bff0: ldr r3, [r5, r3]
0038bff4: ldr r5, [r3]
0038bff8: b #0x38c008
0038bffc: add r6, r6, #1
0038c000: cmp r6, r8
0038c004: beq #0x38c030
0038c008: ldr r1, [r5, r6, lsl #2]
0038c00c: mov r0, r7
0038c010: bl #0x30e31c
0038c014: cmp r0, #0
0038c018: bne #0x38bffc
0038c01c: uxth r6, r6
0038c020: mov r3, #0x370
0038c024: strh r6, [r4, r3]
0038c028: pop {r4, r5, r6, r7, r8, pc}
0038c02c: pop {r4, r5, r6, r7, r8, pc}
0038c030: movw r6, #0xffff
0038c034: mov r3, #0x370
0038c038: strh r6, [r4, r3]
0038c03c: pop {r4, r5, r6, r7, r8, pc}
0038c040: bl #0x38174c
0038c044: cmp r0, #0
0038c048: bne #0x38c068
0038c04c: ldrb r3, [r4, #0x60]
0038c050: cmp r3, #0
0038c054: beq #0x38c068
0038c058: ldrb r3, [r4, #0x10c]
0038c05c: cmp r3, #0
0038c060: strne r0, [r4, #0x2d8]
0038c064: bne #0x38bfb4
0038c068: mov r0, r4
0038c06c: bl #0x394eb0
0038c070: ldr r0, [r4, #0x2d8]
0038c074: cmp r0, #0
0038c078: beq #0x38bfb4
0038c07c: b #0x38bfb0
0038c080: rsbeq r8, r0, r4, lsl ip
0038c084: andeq r3, r0, r8, lsr sp
0038c088: andeq r3, r0, r8, lsr #19
