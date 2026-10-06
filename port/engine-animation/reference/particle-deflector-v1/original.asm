# 0x634518 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_10PDeflectorEE5applyEPS2_S5_PNS0_16IParticleContextIS2_EE
00634518: add r0, r0, #0xc
0063451c: b #0x6338e0

# 0x630574 _ZTv0_n20_N6glitch7collada15particle_system24CDeflectorForceSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
00630574: ldr r3, [r0]
00630578: ldr r3, [r3, #-0x14]
0063057c: add r0, r0, r3
00630580: b #0x630480

# 0x632b60 _ZTv0_n12_N6glitch7collada15particle_system24CDeflectorForceSceneNodeD0Ev
00632b60: ldr r3, [r0]
00632b64: ldr r3, [r3, #-0xc]
00632b68: add r0, r0, r3
00632b6c: b #0x632afc

# 0x62ff58 _ZN6glitch2ps11PForceProxyINS0_12GNPSParticleENS0_10PDeflectorEED1Ev
0062ff58: bx lr

# 0x634a98 _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE9bindForceINS0_10PDeflectorEEEiRNT_15parameters_typeE
00634a98: push {r4, r5, r6, r7, r8, lr}
00634a9c: ldr r2, [r0]
00634aa0: mov r3, r0
00634aa4: mov r6, r1
00634aa8: ldr r8, [r2, #-0xc]
00634aac: mov r1, #0
00634ab0: mov r0, #0x54
00634ab4: ldr r2, [r3, r8]
00634ab8: add r8, r3, r8
00634abc: ldr r5, [pc, #0x50]
00634ac0: ldr r7, [r2, #0x58]
00634ac4: bl #0x5341ac
00634ac8: ldr r3, [pc, #0x48]
00634acc: add r5, pc, r5
00634ad0: mov r2, #1
00634ad4: ldr r3, [r5, r3]
00634ad8: mov r4, r0
00634adc: str r2, [r0, #8]
00634ae0: add r3, r3, #8
00634ae4: mov r1, r6
00634ae8: stm r0, {r3, r6}
00634aec: add r0, r0, #0xc
00634af0: bl #0x6349a0
00634af4: ldr r3, [pc, #0x20]
00634af8: mov r0, r8
00634afc: mov r1, r4
00634b00: ldr r3, [r5, r3]
00634b04: add r3, r3, #8
00634b08: str r3, [r4]
00634b0c: blx r7
00634b10: pop {r4, r5, r6, r7, r8, pc}
00634b14: eorseq pc, r5, r4, asr #31
00634b18: andeq r0, r0, ip, ror r7
00634b1c: andeq r0, r0, r0, lsr #24

# 0x634a00 _ZN6glitch2ps15IParticleSystemINS0_12GNPSParticleEE9bindForceINS0_10PDeflectorEEEiRNT_15parameters_typeE
00634a00: push {r4, r5, r6, r7, r8, lr}
00634a04: ldr r2, [r0]
00634a08: mov r3, r0
00634a0c: mov r6, r1
00634a10: ldr r8, [r2, #-0xc]
00634a14: mov r1, #0
00634a18: mov r0, #0x54
00634a1c: ldr r2, [r3, r8]
00634a20: add r8, r3, r8
00634a24: ldr r5, [pc, #0x50]
00634a28: ldr r7, [r2, #0x58]
00634a2c: bl #0x5341ac
00634a30: ldr r3, [pc, #0x48]
00634a34: add r5, pc, r5
00634a38: mov r2, #1
00634a3c: ldr r3, [r5, r3]
00634a40: mov r4, r0
00634a44: str r2, [r0, #8]
00634a48: add r3, r3, #8
00634a4c: mov r1, r6
00634a50: stm r0, {r3, r6}
00634a54: add r0, r0, #0xc
00634a58: bl #0x6349a0
00634a5c: ldr r3, [pc, #0x20]
00634a60: mov r0, r8
00634a64: mov r1, r4
00634a68: ldr r3, [r5, r3]
00634a6c: add r3, r3, #8
00634a70: str r3, [r4]
00634a74: blx r7
00634a78: pop {r4, r5, r6, r7, r8, pc}
00634a7c: eorseq r0, r6, ip, asr r0
00634a80: andeq r3, r0, ip, lsl #14
00634a84: strdeq r2, r3, [r0], -r4

# 0x6306ac _ZN6glitch2ps11PForceProxyINS0_12GNPSParticleENS0_10PDeflectorEED0Ev
006306ac: push {r4, lr}
006306b0: mov r4, r0
006306b4: bl #0x30e2b0
006306b8: mov r0, r4
006306bc: pop {r4, pc}

# 0x630308 _ZTv0_n16_NK6glitch7collada15particle_system24CDeflectorForceSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
00630308: ldr r3, [r0]
0063030c: ldr r3, [r3, #-0x10]
00630310: add r0, r0, r3
00630314: b #0x6301f8

# 0x634b20 _ZN6glitch7collada15particle_system24CDeflectorForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
00634b20: add r3, r0, #0x140
00634b24: ldr r0, [r1, #0x178]
00634b28: mov r1, r3
00634b2c: b #0x634a98

# 0x8e4e4c _ZZN6glitch2ps10PDeflector22GetFrictionCoefficientEffffE12angleCoefBig
008e4e4c: stclo p12, c5, [pc, #0xa4]

# 0x8e4e50 _ZZN6glitch2ps10PDeflector22GetFrictionCoefficientEffffE14angleCoefSmall
008e4e50: stclo p12, c5, [pc, #-0xa4]

# 0x6338e0 _ZN6glitch2ps10PDeflector5applyINS0_9SParticleEEEvNS0_16IParticleContextIT_E11ParticleIttES7_PS6_
006338e0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006338e4: sub sp, sp, #0xfc
006338e8: str r0, [sp, #0x18]
006338ec: ldr r4, [r0]
006338f0: mov r5, #0
006338f4: add r0, sp, #0xec
006338f8: ldr r6, [r4]
006338fc: str r6, [sp, #0x1c]
00633900: ldr r7, [r4, #0x14]
00633904: ldr lr, [sp, #0x1c]
00633908: str r7, [sp, #0x60]
0063390c: ldr ip, [r4, #4]
00633910: ldr r7, [sp, #0x1c]
00633914: str ip, [sp, #0x58]
00633918: ldr r6, [r6, #0x10]
0063391c: ldr ip, [lr, #0x24]
00633920: ldr r8, [lr, #0x14]
00633924: add r6, r6, #0x80000000
00633928: ldr sl, [lr, #0x18]
0063392c: ldr sb, [r7, #0x20]
00633930: ldr lr, [lr, #0x28]
00633934: str r6, [sp, #0x40]
00633938: ldr r6, [r4, #0xc]
0063393c: ldr r7, [r4, #8]
00633940: add r8, r8, #0x80000000
00633944: str r6, [sp, #0x68]
00633948: ldr r4, [r4, #0x10]
0063394c: add sl, sl, #0x80000000
00633950: str r4, [sp, #0x6c]
00633954: ldr r4, [sp, #0x1c]
00633958: strb r5, [r4, #0x40]
0063395c: ldr r6, [sp, #0x1c]
00633960: str r2, [sp, #0x20]
00633964: str r3, [sp, #0x54]
00633968: str r8, [sp, #0x3c]
0063396c: str sl, [sp, #0x38]
00633970: ldr r6, [r6]
00633974: mov r4, r1
00633978: ldr r1, [sp, #0x1c]
0063397c: str r6, [sp, #0x4c]
00633980: ldr r1, [r1, #4]
00633984: str r1, [sp, #0x48]
00633988: ldr r2, [sp, #0x1c]
0063398c: ldr r3, [sp, #0x1c]
00633990: ldr r6, [sp, #0x1c]
00633994: ldr r2, [r2, #8]
00633998: ldr r1, [sp, #0x1c]
0063399c: str r2, [sp, #0x44]
006339a0: ldr r3, [r3, #0x30]
006339a4: str r3, [sp, #0x2c]
006339a8: ldr r6, [r6, #0x34]
006339ac: str r6, [sp, #0x30]
006339b0: ldr r1, [r1, #0x38]
006339b4: str lr, [sp, #0xf4]
006339b8: str ip, [sp, #0xf0]
006339bc: str r1, [sp, #0x34]
006339c0: str sb, [sp, #0xec]
006339c4: bl #0x35e8e0
006339c8: ldr r0, [sp, #0x40]
006339cc: mov r1, r0
006339d0: bl #0x30ed6c
006339d4: mov r6, r0
006339d8: ldr r0, [sp, #0x3c]
006339dc: mov r1, r0
006339e0: bl #0x30ed6c
006339e4: mov r1, r0
006339e8: mov r0, r6
006339ec: bl #0x30eba4
006339f0: mov r6, r0
006339f4: ldr r0, [sp, #0x38]
006339f8: mov r1, r0
006339fc: bl #0x30ed6c
00633a00: mov r1, r0
00633a04: mov r0, r6
00633a08: bl #0x30eba4
00633a0c: bl #0x30e8a4
00633a10: bl #0x30e1c0
00633a14: ldr r2, [sp, #0x18]
00633a18: ldr r6, [r2]
00633a1c: bl #0x30e6a0
00633a20: ldr r1, [r6, #0x1c]
00633a24: bl #0x30ed6c
00633a28: mov r1, #0x3f000000
00633a2c: bl #0x30ed6c
00633a30: str r0, [sp, #0x50]
00633a34: ldr r0, [sp, #0x4c]
00633a38: mov r1, r0
00633a3c: bl #0x30ed6c
00633a40: mov r6, r0
00633a44: ldr r0, [sp, #0x48]
00633a48: mov r1, r0
00633a4c: bl #0x30ed6c
00633a50: mov r1, r0
00633a54: mov r0, r6
00633a58: bl #0x30eba4
00633a5c: mov r6, r0
00633a60: ldr r0, [sp, #0x44]
00633a64: mov r1, r0
00633a68: bl #0x30ed6c
00633a6c: mov r1, r0
00633a70: mov r0, r6
00633a74: bl #0x30eba4
00633a78: bl #0x30e8a4
00633a7c: bl #0x30e1c0
00633a80: ldr r3, [sp, #0x18]
00633a84: ldr r6, [r3]
00633a88: bl #0x30e6a0
00633a8c: ldr r1, [r6, #0x18]
00633a90: bl #0x30ed6c
00633a94: mov r1, #0x3f000000
00633a98: bl #0x30ed6c
00633a9c: ldr r6, [sp, #0x18]
00633aa0: str r0, [sp, #0x5c]
00633aa4: mov r1, r7
00633aa8: ldr r3, [r6, #0x14]
00633aac: ldr r2, [r6, #0x18]
00633ab0: ldr ip, [r6, #0x1c]
00633ab4: strb r5, [r6, #0x44]
00633ab8: add r3, r3, #0x80000000
00633abc: add r2, r2, #0x80000000
00633ac0: add ip, ip, #0x80000000
00633ac4: ldr r0, [sp, #0x58]
00633ac8: str r3, [sp, #0x90]
00633acc: str r2, [sp, #0x8c]
00633ad0: str ip, [sp, #0x88]
00633ad4: bl #0x30ed6c
00633ad8: str r0, [sp, #0x64]
00633adc: ldr ip, [r6, #0x34]
00633ae0: mov r3, #0
00633ae4: str r3, [sp, #0xe0]
00633ae8: str ip, [sp, #0x70]
00633aec: str r3, [sp, #0xe4]
00633af0: str r3, [sp, #0xe8]
00633af4: ldr lr, [r6, #0x38]
00633af8: ldr r7, [sp, #0x20]
00633afc: str lr, [sp, #0x74]
00633b00: ldr r0, [r6, #0x3c]
00633b04: cmp r4, r7
00633b08: ldr r7, [sp, #0x54]
00633b0c: str r0, [sp, #0x78]
00633b10: ldr r1, [r6, #4]
00633b14: str r1, [sp, #0x84]
00633b18: ldr r2, [r6, #8]
00633b1c: str r2, [sp, #0x80]
00633b20: ldr r6, [r6, #0xc]
00633b24: str r6, [sp, #0x7c]
00633b28: ldr r5, [r7, #0x50]
00633b2c: beq #0x633da4
00633b30: add ip, sp, #0xe0
00633b34: add lr, sp, #0xd4
00633b38: add r0, sp, #0xc8
00633b3c: add r1, sp, #0xbc
00633b40: str ip, [sp, #0x94]
00633b44: str lr, [sp, #0x98]
00633b48: str r0, [sp, #0x9c]
00633b4c: str r1, [sp, #0xa0]
00633b50: ldr r3, [r4, #0xc]
00633b54: ldr r7, [r4, #0x10]
00633b58: ldr r6, [r4, #0x14]
00633b5c: mov r1, r3
00633b60: mov r0, r5
00633b64: str r3, [sp, #0xe0]
00633b68: str r7, [sp, #0xe4]
00633b6c: str r6, [sp, #0xe8]
00633b70: bl #0x30ed6c
00633b74: mov r1, r7
00633b78: mov fp, r0
00633b7c: mov r0, r5
00633b80: bl #0x30ed6c
00633b84: mov r1, r6
00633b88: mov sb, r0
00633b8c: mov r0, r5
00633b90: bl #0x30ed6c
00633b94: ldr r8, [sp, #0xec]
00633b98: mov sl, r0
00633b9c: mov r0, fp
00633ba0: mov r1, r8
00633ba4: bl #0x30ed6c
00633ba8: ldr r7, [sp, #0xf0]
00633bac: mov r6, r0
00633bb0: mov r0, sb
00633bb4: mov r1, r7
00633bb8: bl #0x30ed6c
00633bbc: mov r1, r0
00633bc0: mov r0, r6
00633bc4: bl #0x30eba4
00633bc8: ldr r6, [sp, #0xf4]
00633bcc: mov r3, r0
00633bd0: mov r0, sl
00633bd4: mov r1, r6
00633bd8: str r3, [sp, #8]
00633bdc: bl #0x30ed6c
00633be0: ldr r3, [sp, #8]
00633be4: mov r1, r0
00633be8: mov r0, r3
00633bec: bl #0x30eba4
00633bf0: mov r1, #0
00633bf4: str r0, [sp, #0x14]
00633bf8: bl #0x30df8c
00633bfc: cmp r0, #0
00633c00: bne #0x633d94
00633c04: ldr r2, [r4]
00633c08: ldr r0, [sp, #0x2c]
00633c0c: str r2, [sp, #0x24]
00633c10: ldr r3, [r4, #4]
00633c14: mov r1, r2
00633c18: str r3, [sp, #0x28]
00633c1c: bl #0x30e3ac
00633c20: mov r1, r0
00633c24: mov r0, r8
00633c28: bl #0x30ed6c
00633c2c: ldr r1, [sp, #0x28]
00633c30: mov r8, r0
00633c34: ldr r0, [sp, #0x30]
00633c38: bl #0x30e3ac
00633c3c: mov r1, r0
00633c40: mov r0, r7
00633c44: bl #0x30ed6c
00633c48: mov r1, r0
00633c4c: mov r0, r8
00633c50: bl #0x30eba4
00633c54: ldr r7, [r4, #8]
00633c58: mov r8, r0
00633c5c: ldr r0, [sp, #0x34]
00633c60: mov r1, r7
00633c64: bl #0x30e3ac
00633c68: mov r1, r0
00633c6c: mov r0, r6
00633c70: bl #0x30ed6c
00633c74: mov r1, r0
00633c78: mov r0, r8
00633c7c: bl #0x30eba4
00633c80: ldr r1, [sp, #0x14]
00633c84: bl #0x30ec94
00633c88: mov r1, #0
00633c8c: mov r6, r0
00633c90: bl #0x30e9ac
00633c94: cmp r0, #0
00633c98: bne #0x633d94
00633c9c: mov r0, r6
00633ca0: mov r1, #0x3f800000
00633ca4: bl #0x30e2f8
00633ca8: cmp r0, #0
00633cac: bne #0x633d94
00633cb0: mov r1, fp
00633cb4: mov r0, r6
00633cb8: bl #0x30ed6c
00633cbc: mov r1, r0
00633cc0: ldr r0, [sp, #0x24]
00633cc4: bl #0x30eba4
00633cc8: ldr r1, [sp, #0x2c]
00633ccc: bl #0x30e3ac
00633cd0: mov r1, sb
00633cd4: mov fp, r0
00633cd8: mov r0, r6
00633cdc: bl #0x30ed6c
00633ce0: mov r1, r0
00633ce4: ldr r0, [sp, #0x28]
00633ce8: bl #0x30eba4
00633cec: ldr r1, [sp, #0x30]
00633cf0: bl #0x30e3ac
00633cf4: mov r1, sl
00633cf8: mov r8, r0
00633cfc: mov r0, r6
00633d00: bl #0x30ed6c
00633d04: mov r1, r0
00633d08: mov r0, r7
00633d0c: bl #0x30eba4
00633d10: ldr r1, [sp, #0x34]
00633d14: bl #0x30e3ac
00633d18: mov r1, fp
00633d1c: mov r7, r0
00633d20: ldr r0, [sp, #0x40]
00633d24: bl #0x30ed6c
00633d28: mov r1, r8
00633d2c: mov sl, r0
00633d30: ldr r0, [sp, #0x3c]
00633d34: bl #0x30ed6c
00633d38: mov r1, r0
00633d3c: mov r0, sl
00633d40: bl #0x30eba4
00633d44: mov r1, r7
00633d48: mov sl, r0
00633d4c: ldr r0, [sp, #0x38]
00633d50: bl #0x30ed6c
00633d54: mov r1, r0
00633d58: mov r0, sl
00633d5c: bl #0x30eba4
00633d60: ldr r1, [sp, #0x50]
00633d64: bl #0x30ec94
00633d68: mov r1, #0x3f800000
00633d6c: mov sl, r0
00633d70: bl #0x30e2f8
00633d74: cmp r0, #0
00633d78: bne #0x633d94
00633d7c: mov r1, #0xbf000000
00633d80: mov r0, sl
00633d84: add r1, r1, #0x800000
00633d88: bl #0x30e70c
00633d8c: cmp r0, #0
00633d90: beq #0x633dc0
00633d94: ldr r3, [sp, #0x20]
00633d98: add r4, r4, #0x64
00633d9c: cmp r3, r4
00633da0: bne #0x633b50
00633da4: ldr r4, [sp, #0x18]
00633da8: ldr r1, [sp, #0x1c]
00633dac: mov r2, #0x41
00633db0: add r0, r4, #4
00633db4: bl #0x30e868
00633db8: add sp, sp, #0xfc
00633dbc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00633dc0: mov r1, fp
00633dc4: ldr r0, [sp, #0x4c]
00633dc8: bl #0x30ed6c
00633dcc: mov r1, r8
00633dd0: mov sb, r0
00633dd4: ldr r0, [sp, #0x48]
00633dd8: bl #0x30ed6c
00633ddc: mov r1, r0
00633de0: mov r0, sb
00633de4: bl #0x30eba4
00633de8: mov r1, r7
00633dec: mov sb, r0
00633df0: ldr r0, [sp, #0x44]
00633df4: bl #0x30ed6c
00633df8: mov r1, r0
00633dfc: mov r0, sb
00633e00: bl #0x30eba4
00633e04: ldr r1, [sp, #0x5c]
00633e08: bl #0x30ec94
00633e0c: mov r1, #0x3f800000
00633e10: mov sb, r0
00633e14: bl #0x30e2f8
00633e18: cmp r0, #0
00633e1c: bne #0x633d94
00633e20: mov r1, #0xbf000000
00633e24: mov r0, sb
00633e28: add r1, r1, #0x800000
00633e2c: bl #0x30e70c
00633e30: cmp r0, #0
00633e34: bne #0x633d94
00633e38: ldr ip, [sp, #0x54]
00633e3c: ldr r3, [ip]
00633e40: mov r0, ip
00633e44: mov lr, pc
00633e48: ldr pc, [r3, #0x18]
00633e4c: mov r1, #0
00633e50: str r0, [sp, #0xb0]
00633e54: ldr r0, [sp, #0x64]
00633e58: bl #0x30df8c
00633e5c: cmp r0, #0
00633e60: movne lr, #0
00633e64: strne lr, [sp, #0xb4]
00633e68: beq #0x6344d4
00633e6c: ldr r2, [sp, #0xe0]
00633e70: ldr ip, [sp, #0xec]
00633e74: ldr r0, [sp, #0xf0]
00633e78: mov r1, r2
00633e7c: str r2, [sp, #0x10]
00633e80: str r0, [sp, #0x24]
00633e84: mov r0, ip
00633e88: str ip, [sp, #0xc]
00633e8c: bl #0x30ed6c
00633e90: ldr r1, [sp, #0xe4]
00633e94: mov r3, r0
00633e98: ldr r0, [sp, #0x24]
00633e9c: str r3, [sp, #8]
00633ea0: bl #0x30ed6c
00633ea4: ldr r3, [sp, #8]
00633ea8: mov r1, r0
00633eac: mov r0, r3
00633eb0: ldr r3, [sp, #0xf4]
00633eb4: str r3, [sp, #0x28]
00633eb8: bl #0x30eba4
00633ebc: ldr r1, [sp, #0xe8]
00633ec0: mov r3, r0
00633ec4: ldr r0, [sp, #0x28]
00633ec8: str r3, [sp, #8]
00633ecc: bl #0x30ed6c
00633ed0: ldr r3, [sp, #8]
00633ed4: mov r1, r0
00633ed8: mov r0, r3
00633edc: bl #0x30eba4
00633ee0: ldr ip, [sp, #0xc]
00633ee4: add r0, r0, #0x80000000
00633ee8: str r0, [sp, #0x14]
00633eec: mov r1, ip
00633ef0: ldr r0, [sp, #0x14]
00633ef4: bl #0x30ed6c
00633ef8: ldr r2, [sp, #0x10]
00633efc: mov r1, r0
00633f00: mov r0, r2
00633f04: bl #0x30eba4
00633f08: ldr r1, [sp, #0x24]
00633f0c: str r0, [sp, #0xac]
00633f10: ldr r0, [sp, #0x14]
00633f14: bl #0x30ed6c
00633f18: mov r1, r0
00633f1c: ldr r0, [sp, #0xe4]
00633f20: bl #0x30eba4
00633f24: ldr r1, [sp, #0x28]
00633f28: str r0, [sp, #0xa4]
00633f2c: ldr r0, [sp, #0x14]
00633f30: bl #0x30ed6c
00633f34: mov r1, r0
00633f38: ldr r0, [sp, #0xe8]
00633f3c: bl #0x30eba4
00633f40: ldr r1, [sp, #0x58]
00633f44: str r0, [sp, #0xa8]
00633f48: ldr r0, [sp, #0xb4]
00633f4c: bl #0x30eba4
00633f50: mov r1, r0
00633f54: ldr r0, [sp, #0x14]
00633f58: bl #0x30ed6c
00633f5c: str r0, [sp, #0x14]
00633f60: ldr r0, [sp, #0xac]
00633f64: mov r1, r0
00633f68: bl #0x30ed6c
00633f6c: mov r3, r0
00633f70: ldr r0, [sp, #0xa4]
00633f74: str r3, [sp, #8]
00633f78: mov r1, r0
00633f7c: bl #0x30ed6c
00633f80: ldr r3, [sp, #8]
00633f84: mov r1, r0
00633f88: mov r0, r3
00633f8c: bl #0x30eba4
00633f90: mov r3, r0
00633f94: ldr r0, [sp, #0xa8]
00633f98: str r3, [sp, #8]
00633f9c: mov r1, r0
00633fa0: bl #0x30ed6c
00633fa4: ldr r3, [sp, #8]
00633fa8: mov r1, r0
00633fac: mov r0, r3
00633fb0: bl #0x30eba4
00633fb4: bl #0x30e8a4
00633fb8: bl #0x30e1c0
00633fbc: bl #0x30e6a0
00633fc0: mov r1, r6
00633fc4: mov r3, r0
00633fc8: mov r0, #0x3f800000
00633fcc: str r3, [sp, #8]
00633fd0: bl #0x30e3ac
00633fd4: mov r1, r5
00633fd8: bl #0x30ed6c
00633fdc: ldr r2, [sp, #0x14]
00633fe0: ldr r3, [sp, #8]
00633fe4: str r0, [sp]
00633fe8: ldr r1, [sp, #0x6c]
00633fec: ldr r0, [sp, #0x18]
00633ff0: bl #0x630abc
00633ff4: ldr ip, [sp, #0xc]
00633ff8: mov r6, r0
00633ffc: ldr r0, [sp, #0x14]
00634000: mov r1, ip
00634004: bl #0x30ed6c
00634008: ldr r1, [sp, #0xac]
0063400c: mov r3, r0
00634010: mov r0, r6
00634014: str r3, [sp, #8]
00634018: bl #0x30ed6c
0063401c: ldr r3, [sp, #8]
00634020: mov r1, r0
00634024: mov r0, r3
00634028: bl #0x30eba4
0063402c: ldr r1, [sp, #0x24]
00634030: str r0, [sp, #0xe0]
00634034: ldr r0, [sp, #0x14]
00634038: bl #0x30ed6c
0063403c: ldr r1, [sp, #0xa4]
00634040: mov r3, r0
00634044: mov r0, r6
00634048: str r3, [sp, #8]
0063404c: bl #0x30ed6c
00634050: ldr r3, [sp, #8]
00634054: mov r1, r0
00634058: mov r0, r3
0063405c: bl #0x30eba4
00634060: ldr r1, [sp, #0x28]
00634064: str r0, [sp, #0xe4]
00634068: ldr r0, [sp, #0x14]
0063406c: bl #0x30ed6c
00634070: ldr r1, [sp, #0xa8]
00634074: mov r3, r0
00634078: mov r0, r6
0063407c: str r3, [sp, #8]
00634080: bl #0x30ed6c
00634084: ldr r3, [sp, #8]
00634088: mov r1, r0
0063408c: mov r0, r3
00634090: bl #0x30eba4
00634094: mov r1, #0
00634098: str r0, [sp, #0xe8]
0063409c: ldr r0, [sp, #0x68]
006340a0: bl #0x30e2f8
006340a4: cmp r0, #0
006340a8: bne #0x6342a8
006340ac: mov r1, fp
006340b0: ldr r0, [sp, #0x2c]
006340b4: bl #0x30eba4
006340b8: mov r1, r8
006340bc: mov fp, r0
006340c0: ldr r0, [sp, #0x30]
006340c4: bl #0x30eba4
006340c8: mov r1, r7
006340cc: mov r6, r0
006340d0: ldr r0, [sp, #0x34]
006340d4: bl #0x30eba4
006340d8: mov r1, #0
006340dc: mov r7, r0
006340e0: ldr r0, [sp, #0x60]
006340e4: bl #0x30e2f8
006340e8: cmp r0, #0
006340ec: beq #0x634224
006340f0: ldr r1, [sp, #0x50]
006340f4: mov r0, sl
006340f8: bl #0x30ed6c
006340fc: ldr r1, [sp, #0x5c]
00634100: mov r8, r0
00634104: mov r0, sb
00634108: bl #0x30ed6c
0063410c: mov r1, r8
00634110: mov sl, r0
00634114: ldr r0, [sp, #0x90]
00634118: bl #0x30ed6c
0063411c: mov r1, r0
00634120: ldr r0, [sp, #0x70]
00634124: bl #0x30eba4
00634128: mov r1, sl
0063412c: mov sb, r0
00634130: ldr r0, [sp, #0x84]
00634134: bl #0x30ed6c
00634138: mov r1, r0
0063413c: mov r0, sb
00634140: bl #0x30eba4
00634144: mov r1, r0
00634148: mov r0, fp
0063414c: bl #0x30e3ac
00634150: mov r1, r0
00634154: ldr r0, [sp, #0x60]
00634158: bl #0x30ed6c
0063415c: mov r1, r0
00634160: ldr r0, [sp, #0xe0]
00634164: bl #0x30eba4
00634168: mov r1, r8
0063416c: str r0, [sp, #0xe0]
00634170: ldr r0, [sp, #0x8c]
00634174: bl #0x30ed6c
00634178: mov r1, r0
0063417c: ldr r0, [sp, #0x74]
00634180: bl #0x30eba4
00634184: mov r1, sl
00634188: mov sb, r0
0063418c: ldr r0, [sp, #0x80]
00634190: bl #0x30ed6c
00634194: mov r1, r0
00634198: mov r0, sb
0063419c: bl #0x30eba4
006341a0: mov r1, r0
006341a4: mov r0, r6
006341a8: bl #0x30e3ac
006341ac: mov r1, r0
006341b0: ldr r0, [sp, #0x60]
006341b4: bl #0x30ed6c
006341b8: mov r1, r0
006341bc: ldr r0, [sp, #0xe4]
006341c0: bl #0x30eba4
006341c4: mov r1, r8
006341c8: str r0, [sp, #0xe4]
006341cc: ldr r0, [sp, #0x88]
006341d0: bl #0x30ed6c
006341d4: mov r1, r0
006341d8: ldr r0, [sp, #0x78]
006341dc: bl #0x30eba4
006341e0: mov r1, sl
006341e4: mov r8, r0
006341e8: ldr r0, [sp, #0x7c]
006341ec: bl #0x30ed6c
006341f0: mov r1, r0
006341f4: mov r0, r8
006341f8: bl #0x30eba4
006341fc: mov r1, r0
00634200: mov r0, r7
00634204: bl #0x30e3ac
00634208: mov r1, r0
0063420c: ldr r0, [sp, #0x60]
00634210: bl #0x30ed6c
00634214: mov r1, r0
00634218: ldr r0, [sp, #0xe8]
0063421c: bl #0x30eba4
00634220: str r0, [sp, #0xe8]
00634224: movw r1, #0x999a
00634228: ldr r0, [sp, #0xf0]
0063422c: movt r1, #0x3e99
00634230: bl #0x30ed6c
00634234: mov r1, r0
00634238: mov r0, r6
0063423c: bl #0x30eba4
00634240: movw r1, #0x999a
00634244: mov r6, r0
00634248: movt r1, #0x3e99
0063424c: ldr r0, [sp, #0xf4]
00634250: bl #0x30ed6c
00634254: mov r1, r0
00634258: mov r0, r7
0063425c: bl #0x30eba4
00634260: movw r1, #0x999a
00634264: mov r7, r0
00634268: movt r1, #0x3e99
0063426c: ldr r0, [sp, #0xec]
00634270: bl #0x30ed6c
00634274: mov r1, r0
00634278: mov r0, fp
0063427c: bl #0x30eba4
00634280: ldr r1, [sp, #0xe0]
00634284: ldr r2, [sp, #0xe4]
00634288: ldr r3, [sp, #0xe8]
0063428c: str r0, [r4]
00634290: str r6, [r4, #4]
00634294: str r7, [r4, #8]
00634298: str r1, [r4, #0xc]
0063429c: str r2, [r4, #0x10]
006342a0: str r3, [r4, #0x14]
006342a4: b #0x633d94
006342a8: mov r1, #0x43000000
006342ac: add r1, r1, #0x340000
006342b0: ldr r0, [sp, #0x68]
006342b4: bl #0x30ed6c
006342b8: mov r6, r0
006342bc: ldr r0, [sp, #0xb0]
006342c0: bl #0x62fe78
006342c4: mov r2, r0
006342c8: mov r3, r1
006342cc: mov r0, r6
006342d0: mov r1, #0xbf000000
006342d4: str r2, [sp, #0x10]
006342d8: str r3, [sp, #8]
006342dc: bl #0x30ed6c
006342e0: ldr r2, [sp, #0x10]
006342e4: ldr r3, [sp, #8]
006342e8: str r0, [sp, #0x14]
006342ec: mov r0, r2
006342f0: mov r1, r3
006342f4: bl #0x30e6a0
006342f8: mov r1, r0
006342fc: mov r0, r6
00634300: bl #0x30ed6c
00634304: ldr r1, [sp, #0x14]
00634308: bl #0x30eba4
0063430c: bl #0x30e8a4
00634310: ldr lr, [sp, #0x98]
00634314: mov ip, #0
00634318: mov r2, r0
0063431c: mov r3, r1
00634320: ldr r0, [sp, #0x94]
00634324: str lr, [sp]
00634328: str ip, [sp, #0xd4]
0063432c: str ip, [sp, #0xd8]
00634330: str ip, [sp, #0xdc]
00634334: bl #0x630bfc
00634338: ldr r0, [sp, #0xb0]
0063433c: bl #0x62fe78
00634340: bl #0x30e6a0
00634344: mov r1, r0
00634348: mov r0, r6
0063434c: bl #0x30ed6c
00634350: mov r1, r0
00634354: ldr r0, [sp, #0x14]
00634358: bl #0x30eba4
0063435c: bl #0x30e8a4
00634360: ldr ip, [sp, #0x9c]
00634364: mov r2, r0
00634368: mov r3, r1
0063436c: ldr r0, [sp, #0x94]
00634370: mov r1, #0
00634374: str ip, [sp]
00634378: str r1, [sp, #0xc8]
0063437c: str r1, [sp, #0xcc]
00634380: str r1, [sp, #0xd0]
00634384: bl #0x630cf8
00634388: ldr r0, [sp, #0xb0]
0063438c: bl #0x62fe78
00634390: bl #0x30e6a0
00634394: mov r1, r0
00634398: mov r0, r6
0063439c: bl #0x30ed6c
006343a0: mov r1, r0
006343a4: ldr r0, [sp, #0x14]
006343a8: bl #0x30eba4
006343ac: bl #0x30e8a4
006343b0: mov r3, r1
006343b4: ldr r1, [sp, #0xa0]
006343b8: mov lr, #0
006343bc: mov r2, r0
006343c0: ldr r0, [sp, #0x94]
006343c4: str lr, [sp, #0xbc]
006343c8: str lr, [sp, #0xc0]
006343cc: str lr, [sp, #0xc4]
006343d0: str r1, [sp]
006343d4: bl #0x630de8
006343d8: ldr r2, [sp, #0xe0]
006343dc: ldr r6, [sp, #0xec]
006343e0: ldr ip, [sp, #0xe4]
006343e4: ldr r3, [sp, #0xf0]
006343e8: mov r1, r2
006343ec: mov r0, r6
006343f0: str ip, [sp, #0x28]
006343f4: str r2, [sp, #0x14]
006343f8: str r3, [sp, #0x24]
006343fc: bl #0x30ed6c
00634400: ldr r1, [sp, #0x28]
00634404: mov r3, r0
00634408: ldr r0, [sp, #0x24]
0063440c: str r3, [sp, #8]
00634410: bl #0x30ed6c
00634414: ldr r3, [sp, #8]
00634418: ldr lr, [sp, #0xf4]
0063441c: ldr r2, [sp, #0xe8]
00634420: mov r1, r0
00634424: mov r0, r3
00634428: str lr, [sp, #0xac]
0063442c: str r2, [sp, #0xa4]
00634430: bl #0x30eba4
00634434: ldr r1, [sp, #0xa4]
00634438: mov r3, r0
0063443c: ldr r0, [sp, #0xac]
00634440: str r3, [sp, #8]
00634444: bl #0x30ed6c
00634448: ldr r3, [sp, #8]
0063444c: mov r1, r0
00634450: mov r0, r3
00634454: bl #0x30eba4
00634458: mov r1, #0
0063445c: str r0, [sp, #8]
00634460: bl #0x30e70c
00634464: cmp r0, #0
00634468: ldr r3, [sp, #8]
0063446c: beq #0x6340ac
00634470: mov r0, r3
00634474: mov r1, #0xc0000000
00634478: bl #0x30ed6c
0063447c: mov r1, r6
00634480: str r0, [sp, #0xa8]
00634484: bl #0x30ed6c
00634488: mov r1, r0
0063448c: ldr r0, [sp, #0x14]
00634490: bl #0x30eba4
00634494: ldr r1, [sp, #0x24]
00634498: str r0, [sp, #0xe0]
0063449c: ldr r0, [sp, #0xa8]
006344a0: bl #0x30ed6c
006344a4: mov r1, r0
006344a8: ldr r0, [sp, #0x28]
006344ac: bl #0x30eba4
006344b0: ldr r1, [sp, #0xac]
006344b4: str r0, [sp, #0xe4]
006344b8: ldr r0, [sp, #0xa8]
006344bc: bl #0x30ed6c
006344c0: mov r1, r0
006344c4: ldr r0, [sp, #0xa4]
006344c8: bl #0x30eba4
006344cc: str r0, [sp, #0xe8]
006344d0: b #0x6340ac
006344d4: ldr r0, [sp, #0xb0]
006344d8: bl #0x62fe78
006344dc: bl #0x30e6a0
006344e0: mov r1, r0
006344e4: ldr r0, [sp, #0x64]
006344e8: bl #0x30ed6c
006344ec: mov r1, #0xbf000000
006344f0: mov r3, r0
006344f4: ldr r0, [sp, #0x64]
006344f8: str r3, [sp, #8]
006344fc: bl #0x30ed6c
00634500: ldr r3, [sp, #8]
00634504: mov r1, r0
00634508: mov r0, r3
0063450c: bl #0x30eba4
00634510: str r0, [sp, #0xb4]
00634514: b #0x633e6c

# 0x630480 _ZN6glitch7collada15particle_system24CDeflectorForceSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
00630480: push {r4, r5, r6, lr}
00630484: mov r4, r1
00630488: mov r5, r0
0063048c: bl #0x598058
00630490: ldr r1, [pc, #0xc0]
00630494: ldr r3, [r4]
00630498: mov r0, r4
0063049c: add r1, pc, r1
006304a0: mov lr, pc
006304a4: ldr pc, [r3, #0x70]
006304a8: ldr r1, [pc, #0xac]
006304ac: str r0, [r5, #0x144]
006304b0: ldr r3, [r4]
006304b4: add r1, pc, r1
006304b8: mov r0, r4
006304bc: mov lr, pc
006304c0: ldr pc, [r3, #0x70]
006304c4: ldr r1, [pc, #0x94]
006304c8: str r0, [r5, #0x148]
006304cc: ldr r3, [r4]
006304d0: add r1, pc, r1
006304d4: mov r0, r4
006304d8: mov lr, pc
006304dc: ldr pc, [r3, #0x70]
006304e0: ldr r1, [pc, #0x7c]
006304e4: str r0, [r5, #0x14c]
006304e8: ldr r3, [r4]
006304ec: add r1, pc, r1
006304f0: mov r0, r4
006304f4: mov lr, pc
006304f8: ldr pc, [r3, #0x70]
006304fc: ldr r1, [pc, #0x64]
00630500: str r0, [r5, #0x150]
00630504: ldr r3, [r4]
00630508: add r1, pc, r1
0063050c: mov r0, r4
00630510: mov lr, pc
00630514: ldr pc, [r3, #0x70]
00630518: ldr r1, [pc, #0x4c]
0063051c: str r0, [r5, #0x154]
00630520: ldr r3, [r4]
00630524: add r1, pc, r1
00630528: mov r0, r4
0063052c: mov lr, pc
00630530: ldr pc, [r3, #0x70]
00630534: ldr r1, [pc, #0x34]
00630538: str r0, [r5, #0x158]
0063053c: ldr r3, [r4]
00630540: mov r0, r4
00630544: add r1, pc, r1
00630548: mov lr, pc
0063054c: ldr pc, [r3, #0x70]
00630550: str r0, [r5, #0x15c]
00630554: pop {r4, r5, r6, pc}
00630558: eoreq r4, fp, r4, asr sl
0063055c: eoreq r4, fp, r4, asr #20
00630560: eoreq r4, fp, r8, lsr sl
00630564: eoreq r4, fp, r4, lsr #20
00630568: eoreq r4, fp, r8, lsl sl
0063056c: eoreq lr, sl, r4, lsl #13
00630570: mlaeq fp, r4, sb, sb

# 0x6306d4 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_10PDeflectorEED0Ev
006306d4: push {r4, lr}
006306d8: mov r4, r0
006306dc: bl #0x30e2b0
006306e0: mov r0, r4
006306e4: pop {r4, pc}

# 0x632b50 _ZTv0_n24_N6glitch7collada15particle_system24CDeflectorForceSceneNodeD0Ev
00632b50: ldr r3, [r0]
00632b54: ldr r3, [r3, #-0x18]
00632b58: add r0, r0, r3
00632b5c: b #0x632afc

# 0x631138 _ZN6glitch7collada15CColladaFactory34createParticleSystemDeflectorForceERKNS0_16CColladaDatabaseEPNS0_6SForceE
00631138: push {r4, r5, r6, lr}
0063113c: mov r0, #0x168
00631140: mov r5, r1
00631144: mov r1, #0
00631148: mov r6, r2
0063114c: bl #0x5341ac
00631150: mov r1, r5
00631154: mov r4, r0
00631158: mov r2, r6
0063115c: bl #0x631050
00631160: mov r0, r4
00631164: pop {r4, r5, r6, pc}

# 0x6308c4 _ZTv0_n12_N6glitch7collada15particle_system24CDeflectorForceSceneNodeD1Ev
006308c4: ldr r3, [r0]
006308c8: ldr r3, [r3, #-0xc]
006308cc: add r0, r0, r3
006308d0: b #0x630868

# 0x630abc _ZN6glitch2ps10PDeflector22GetFrictionCoefficientEffff
00630abc: push {r4, r5, r6, r7, r8, lr}
00630ac0: mov r0, r1
00630ac4: mov r4, r1
00630ac8: mov r1, #0
00630acc: mov r5, r2
00630ad0: mov r6, r3
00630ad4: bl #0x30df8c
00630ad8: cmp r0, #0
00630adc: ldr r7, [sp, #0x18]
00630ae0: bne #0x630bc0
00630ae4: mov r0, r4
00630ae8: mov r1, #0x3f800000
00630aec: bl #0x30df8c
00630af0: cmp r0, #0
00630af4: bne #0x630bb8
00630af8: movw r1, #0x5c29
00630afc: movt r1, #0x3d8f
00630b00: mov r0, r6
00630b04: bl #0x30ed6c
00630b08: mov r1, r5
00630b0c: bl #0x30e9ac
00630b10: cmp r0, #0
00630b14: bne #0x630bec
00630b18: movw r1, #0x5c29
00630b1c: movt r1, #0x3d0f
00630b20: mov r0, r6
00630b24: bl #0x30ed6c
00630b28: mov r1, r5
00630b2c: bl #0x30e2f8
00630b30: cmp r0, #0
00630b34: bne #0x630bc8
00630b38: mov r1, r6
00630b3c: mov r0, r5
00630b40: bl #0x30ec94
00630b44: movw r1, #0x5c29
00630b48: movt r1, #0x3d0f
00630b4c: bl #0x30ec94
00630b50: mov r1, #0x3f800000
00630b54: bl #0x30e3ac
00630b58: mov r1, r4
00630b5c: mov r5, r0
00630b60: mov r0, #0x3f800000
00630b64: bl #0x30e3ac
00630b68: mov r4, r0
00630b6c: bl #0x30deb4
00630b70: mov r1, r0
00630b74: mov r0, r7
00630b78: bl #0x30ed6c
00630b7c: bl #0x30ec64
00630b80: mov r1, r5
00630b84: mov r6, r0
00630b88: mov r0, r4
00630b8c: bl #0x30ed6c
00630b90: mov r1, r5
00630b94: mov r4, r0
00630b98: mov r0, #0x3f800000
00630b9c: bl #0x30e3ac
00630ba0: mov r1, r6
00630ba4: bl #0x30ed6c
00630ba8: mov r1, r0
00630bac: mov r0, r4
00630bb0: bl #0x30eba4
00630bb4: pop {r4, r5, r6, r7, r8, pc}
00630bb8: mov r0, #0
00630bbc: pop {r4, r5, r6, r7, r8, pc}
00630bc0: mov r0, #0x3f800000
00630bc4: pop {r4, r5, r6, r7, r8, pc}
00630bc8: mov r1, r4
00630bcc: mov r0, #0x3f800000
00630bd0: bl #0x30e3ac
00630bd4: bl #0x30deb4
00630bd8: mov r1, r0
00630bdc: mov r0, r7
00630be0: bl #0x30ed6c
00630be4: pop {r4, r5, r6, r7, r8, lr}
00630be8: b #0x30ec64
00630bec: mov r1, r4
00630bf0: mov r0, #0x3f800000
00630bf4: bl #0x30e3ac
00630bf8: pop {r4, r5, r6, r7, r8, pc}

# 0x62ff5c _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_10PDeflectorEED1Ev
0062ff5c: bx lr

# 0x632afc _ZN6glitch7collada15particle_system24CDeflectorForceSceneNodeD0Ev
00632afc: ldr r3, [pc, #0x40]
00632b00: ldr r2, [pc, #0x40]
00632b04: ldr r1, [pc, #0x40]
00632b08: add r3, pc, r3
00632b0c: ldr r2, [r3, r2]
00632b10: ldr r1, [r3, r1]
00632b14: push {r4, lr}
00632b18: add ip, r2, #0x128
00632b1c: add r2, r2, #0x1c
00632b20: mov r4, r0
00632b24: str r2, [r0]
00632b28: str ip, [r0, #0x160]
00632b2c: add r1, r1, #4
00632b30: bl #0x63081c
00632b34: mov r0, r4
00632b38: bl #0x30e2b0
00632b3c: mov r0, r4
00632b40: pop {r4, pc}
00632b44: eorseq r1, r6, r8, lsl #31
00632b48: andeq r1, r0, r0, ror r2
00632b4c: andeq r0, r0, r0, lsr fp

# 0x631050 _ZN6glitch7collada15particle_system24CDeflectorForceSceneNodeC1ERKNS0_16CColladaDatabaseERKNS0_6SForceE
00631050: push {r4, r5, r6, r7, r8, lr}
00631054: ldr r5, [pc, #0xcc]
00631058: ldr ip, [pc, #0xcc]
0063105c: ldr r3, [pc, #0xcc]
00631060: add r5, pc, r5
00631064: ldr ip, [r5, ip]
00631068: ldr r3, [r5, r3]
0063106c: mov r6, #1
00631070: ldr lr, [ip, #0x24]
00631074: add r3, r3, #8
00631078: str r3, [r0, #0x160]
0063107c: str lr, [r0]
00631080: str r6, [r0, #0x164]
00631084: ldr r6, [lr, #-0xc]
00631088: ldr r7, [ip, #0x28]
0063108c: mov lr, r1
00631090: mov r3, r2
00631094: add r1, ip, #4
00631098: mov r2, lr
0063109c: str r7, [r0, r6]
006310a0: mov r4, r0
006310a4: bl #0x630f6c
006310a8: ldr r2, [pc, #0x84]
006310ac: ldr r3, [r4, #0x13c]
006310b0: add r1, r4, #0x24
006310b4: ldr r2, [r5, r2]
006310b8: str r1, [r4, #0x140]
006310bc: mov r0, r4
006310c0: add r1, r2, #0x128
006310c4: add r2, r2, #0x1c
006310c8: str r2, [r4]
006310cc: str r1, [r4, #0x160]
006310d0: ldr r2, [r3, #0xc]
006310d4: ldr r2, [r2]
006310d8: str r2, [r4, #0x144]
006310dc: ldr r2, [r3, #0xc]
006310e0: ldr r2, [r2, #4]
006310e4: str r2, [r4, #0x148]
006310e8: ldr r2, [r3, #0xc]
006310ec: ldr r2, [r2, #8]
006310f0: str r2, [r4, #0x14c]
006310f4: ldr r2, [r3, #0xc]
006310f8: ldr r2, [r2, #0xc]
006310fc: str r2, [r4, #0x150]
00631100: ldr r2, [r3, #0xc]
00631104: ldr r2, [r2, #0x10]
00631108: str r2, [r4, #0x154]
0063110c: ldr r2, [r3, #0xc]
00631110: ldr r2, [r2, #0x14]
00631114: str r2, [r4, #0x158]
00631118: ldr r3, [r3, #0xc]
0063111c: ldr r3, [r3, #0x18]
00631120: str r3, [r4, #0x15c]
00631124: pop {r4, r5, r6, r7, r8, pc}
00631128: eorseq r3, r6, r0, lsr sl
0063112c: andeq r0, r0, r0, lsr fp
00631130: andeq r2, r0, r4, asr #22
00631134: andeq r1, r0, r0, ror r2

# 0x630868 _ZN6glitch7collada15particle_system24CDeflectorForceSceneNodeD1Ev
00630868: ldr r3, [pc, #0x38]
0063086c: ldr r2, [pc, #0x38]
00630870: ldr r1, [pc, #0x38]
00630874: add r3, pc, r3
00630878: ldr r2, [r3, r2]
0063087c: ldr r1, [r3, r1]
00630880: push {r4, lr}
00630884: add ip, r2, #0x128
00630888: add r2, r2, #0x1c
0063088c: mov r4, r0
00630890: str r2, [r0]
00630894: str ip, [r0, #0x160]
00630898: add r1, r1, #4
0063089c: bl #0x63081c
006308a0: mov r0, r4
006308a4: pop {r4, pc}
006308a8: eorseq r4, r6, ip, lsl r2
006308ac: andeq r1, r0, r0, ror r2
006308b0: andeq r0, r0, r0, lsr fp

# 0x637a60 _ZN6glitch2ps11PForceProxyINS0_12GNPSParticleENS0_10PDeflectorEE5applyEPS2_S5_PNS0_16IParticleContextIS2_EE
00637a60: add r0, r0, #0xc
00637a64: b #0x636e28

# 0x634a88 _ZN6glitch7collada15particle_system24CDeflectorForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
00634a88: add r3, r0, #0x140
00634a8c: ldr r0, [r1, #0x178]
00634a90: mov r1, r3
00634a94: b #0x634a00

# 0x6308b4 _ZTv0_n24_N6glitch7collada15particle_system24CDeflectorForceSceneNodeD1Ev
006308b4: ldr r3, [r0]
006308b8: ldr r3, [r3, #-0x18]
006308bc: add r0, r0, r3
006308c0: b #0x630868

# 0x636e28 _ZN6glitch2ps10PDeflector5applyINS0_12GNPSParticleEEEvNS0_16IParticleContextIT_E11ParticleIttES7_PS6_
00636e28: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00636e2c: sub sp, sp, #0xfc
00636e30: str r0, [sp, #0x18]
00636e34: ldr r4, [r0]
00636e38: mov r5, #0
00636e3c: add r0, sp, #0xec
00636e40: ldr r6, [r4]
00636e44: str r6, [sp, #0x1c]
00636e48: ldr r7, [r4, #0x14]
00636e4c: ldr lr, [sp, #0x1c]
00636e50: str r7, [sp, #0x60]
00636e54: ldr ip, [r4, #4]
00636e58: ldr r7, [sp, #0x1c]
00636e5c: str ip, [sp, #0x58]
00636e60: ldr r6, [r6, #0x10]
00636e64: ldr ip, [lr, #0x24]
00636e68: ldr r8, [lr, #0x14]
00636e6c: add r6, r6, #0x80000000
00636e70: ldr sl, [lr, #0x18]
00636e74: ldr sb, [r7, #0x20]
00636e78: ldr lr, [lr, #0x28]
00636e7c: str r6, [sp, #0x40]
00636e80: ldr r6, [r4, #0xc]
00636e84: ldr r7, [r4, #8]
00636e88: add r8, r8, #0x80000000
00636e8c: str r6, [sp, #0x68]
00636e90: ldr r4, [r4, #0x10]
00636e94: add sl, sl, #0x80000000
00636e98: str r4, [sp, #0x6c]
00636e9c: ldr r4, [sp, #0x1c]
00636ea0: strb r5, [r4, #0x40]
00636ea4: ldr r6, [sp, #0x1c]
00636ea8: str r2, [sp, #0x20]
00636eac: str r3, [sp, #0x54]
00636eb0: str r8, [sp, #0x3c]
00636eb4: str sl, [sp, #0x38]
00636eb8: ldr r6, [r6]
00636ebc: mov r4, r1
00636ec0: ldr r1, [sp, #0x1c]
00636ec4: str r6, [sp, #0x4c]
00636ec8: ldr r1, [r1, #4]
00636ecc: str r1, [sp, #0x48]
00636ed0: ldr r2, [sp, #0x1c]
00636ed4: ldr r3, [sp, #0x1c]
00636ed8: ldr r6, [sp, #0x1c]
00636edc: ldr r2, [r2, #8]
00636ee0: ldr r1, [sp, #0x1c]
00636ee4: str r2, [sp, #0x44]
00636ee8: ldr r3, [r3, #0x30]
00636eec: str r3, [sp, #0x2c]
00636ef0: ldr r6, [r6, #0x34]
00636ef4: str r6, [sp, #0x30]
00636ef8: ldr r1, [r1, #0x38]
00636efc: str lr, [sp, #0xf4]
00636f00: str ip, [sp, #0xf0]
00636f04: str r1, [sp, #0x34]
00636f08: str sb, [sp, #0xec]
00636f0c: bl #0x35e8e0
00636f10: ldr r0, [sp, #0x40]
00636f14: mov r1, r0
00636f18: bl #0x30ed6c
00636f1c: mov r6, r0
00636f20: ldr r0, [sp, #0x3c]
00636f24: mov r1, r0
00636f28: bl #0x30ed6c
00636f2c: mov r1, r0
00636f30: mov r0, r6
00636f34: bl #0x30eba4
00636f38: mov r6, r0
00636f3c: ldr r0, [sp, #0x38]
00636f40: mov r1, r0
00636f44: bl #0x30ed6c
00636f48: mov r1, r0
00636f4c: mov r0, r6
00636f50: bl #0x30eba4
00636f54: bl #0x30e8a4
00636f58: bl #0x30e1c0
00636f5c: ldr r2, [sp, #0x18]
00636f60: ldr r6, [r2]
00636f64: bl #0x30e6a0
00636f68: ldr r1, [r6, #0x1c]
00636f6c: bl #0x30ed6c
00636f70: mov r1, #0x3f000000
00636f74: bl #0x30ed6c
00636f78: str r0, [sp, #0x50]
00636f7c: ldr r0, [sp, #0x4c]
00636f80: mov r1, r0
00636f84: bl #0x30ed6c
00636f88: mov r6, r0
00636f8c: ldr r0, [sp, #0x48]
00636f90: mov r1, r0
00636f94: bl #0x30ed6c
00636f98: mov r1, r0
00636f9c: mov r0, r6
00636fa0: bl #0x30eba4
00636fa4: mov r6, r0
00636fa8: ldr r0, [sp, #0x44]
00636fac: mov r1, r0
00636fb0: bl #0x30ed6c
00636fb4: mov r1, r0
00636fb8: mov r0, r6
00636fbc: bl #0x30eba4
00636fc0: bl #0x30e8a4
00636fc4: bl #0x30e1c0
00636fc8: ldr r3, [sp, #0x18]
00636fcc: ldr r6, [r3]
00636fd0: bl #0x30e6a0
00636fd4: ldr r1, [r6, #0x18]
00636fd8: bl #0x30ed6c
00636fdc: mov r1, #0x3f000000
00636fe0: bl #0x30ed6c
00636fe4: ldr r6, [sp, #0x18]
00636fe8: str r0, [sp, #0x5c]
00636fec: mov r1, r7
00636ff0: ldr r3, [r6, #0x14]
00636ff4: ldr r2, [r6, #0x18]
00636ff8: ldr ip, [r6, #0x1c]
00636ffc: strb r5, [r6, #0x44]
00637000: add r3, r3, #0x80000000
00637004: add r2, r2, #0x80000000
00637008: add ip, ip, #0x80000000
0063700c: ldr r0, [sp, #0x58]
00637010: str r3, [sp, #0x90]
00637014: str r2, [sp, #0x8c]
00637018: str ip, [sp, #0x88]
0063701c: bl #0x30ed6c
00637020: str r0, [sp, #0x64]
00637024: ldr ip, [r6, #0x34]
00637028: mov r3, #0
0063702c: str r3, [sp, #0xe0]
00637030: str ip, [sp, #0x70]
00637034: str r3, [sp, #0xe4]
00637038: str r3, [sp, #0xe8]
0063703c: ldr lr, [r6, #0x38]
00637040: ldr r7, [sp, #0x20]
00637044: str lr, [sp, #0x74]
00637048: ldr r0, [r6, #0x3c]
0063704c: cmp r4, r7
00637050: ldr r7, [sp, #0x54]
00637054: str r0, [sp, #0x78]
00637058: ldr r1, [r6, #4]
0063705c: str r1, [sp, #0x84]
00637060: ldr r2, [r6, #8]
00637064: str r2, [sp, #0x80]
00637068: ldr r6, [r6, #0xc]
0063706c: str r6, [sp, #0x7c]
00637070: ldr r5, [r7, #0x50]
00637074: beq #0x6372ec
00637078: add ip, sp, #0xe0
0063707c: add lr, sp, #0xd4
00637080: add r0, sp, #0xc8
00637084: add r1, sp, #0xbc
00637088: str ip, [sp, #0x94]
0063708c: str lr, [sp, #0x98]
00637090: str r0, [sp, #0x9c]
00637094: str r1, [sp, #0xa0]
00637098: ldr r3, [r4, #0xc]
0063709c: ldr r7, [r4, #0x10]
006370a0: ldr r6, [r4, #0x14]
006370a4: mov r1, r3
006370a8: mov r0, r5
006370ac: str r3, [sp, #0xe0]
006370b0: str r7, [sp, #0xe4]
006370b4: str r6, [sp, #0xe8]
006370b8: bl #0x30ed6c
006370bc: mov r1, r7
006370c0: mov fp, r0
006370c4: mov r0, r5
006370c8: bl #0x30ed6c
006370cc: mov r1, r6
006370d0: mov sb, r0
006370d4: mov r0, r5
006370d8: bl #0x30ed6c
006370dc: ldr r8, [sp, #0xec]
006370e0: mov sl, r0
006370e4: mov r0, fp
006370e8: mov r1, r8
006370ec: bl #0x30ed6c
006370f0: ldr r7, [sp, #0xf0]
006370f4: mov r6, r0
006370f8: mov r0, sb
006370fc: mov r1, r7
00637100: bl #0x30ed6c
00637104: mov r1, r0
00637108: mov r0, r6
0063710c: bl #0x30eba4
00637110: ldr r6, [sp, #0xf4]
00637114: mov r3, r0
00637118: mov r0, sl
0063711c: mov r1, r6
00637120: str r3, [sp, #8]
00637124: bl #0x30ed6c
00637128: ldr r3, [sp, #8]
0063712c: mov r1, r0
00637130: mov r0, r3
00637134: bl #0x30eba4
00637138: mov r1, #0
0063713c: str r0, [sp, #0x14]
00637140: bl #0x30df8c
00637144: cmp r0, #0
00637148: bne #0x6372dc
0063714c: ldr r2, [r4]
00637150: ldr r0, [sp, #0x2c]
00637154: str r2, [sp, #0x24]
00637158: ldr r3, [r4, #4]
0063715c: mov r1, r2
00637160: str r3, [sp, #0x28]
00637164: bl #0x30e3ac
00637168: mov r1, r0
0063716c: mov r0, r8
00637170: bl #0x30ed6c
00637174: ldr r1, [sp, #0x28]
00637178: mov r8, r0
0063717c: ldr r0, [sp, #0x30]
00637180: bl #0x30e3ac
00637184: mov r1, r0
00637188: mov r0, r7
0063718c: bl #0x30ed6c
00637190: mov r1, r0
00637194: mov r0, r8
00637198: bl #0x30eba4
0063719c: ldr r7, [r4, #8]
006371a0: mov r8, r0
006371a4: ldr r0, [sp, #0x34]
006371a8: mov r1, r7
006371ac: bl #0x30e3ac
006371b0: mov r1, r0
006371b4: mov r0, r6
006371b8: bl #0x30ed6c
006371bc: mov r1, r0
006371c0: mov r0, r8
006371c4: bl #0x30eba4
006371c8: ldr r1, [sp, #0x14]
006371cc: bl #0x30ec94
006371d0: mov r1, #0
006371d4: mov r6, r0
006371d8: bl #0x30e9ac
006371dc: cmp r0, #0
006371e0: bne #0x6372dc
006371e4: mov r0, r6
006371e8: mov r1, #0x3f800000
006371ec: bl #0x30e2f8
006371f0: cmp r0, #0
006371f4: bne #0x6372dc
006371f8: mov r1, fp
006371fc: mov r0, r6
00637200: bl #0x30ed6c
00637204: mov r1, r0
00637208: ldr r0, [sp, #0x24]
0063720c: bl #0x30eba4
00637210: ldr r1, [sp, #0x2c]
00637214: bl #0x30e3ac
00637218: mov r1, sb
0063721c: mov fp, r0
00637220: mov r0, r6
00637224: bl #0x30ed6c
00637228: mov r1, r0
0063722c: ldr r0, [sp, #0x28]
00637230: bl #0x30eba4
00637234: ldr r1, [sp, #0x30]
00637238: bl #0x30e3ac
0063723c: mov r1, sl
00637240: mov r8, r0
00637244: mov r0, r6
00637248: bl #0x30ed6c
0063724c: mov r1, r0
00637250: mov r0, r7
00637254: bl #0x30eba4
00637258: ldr r1, [sp, #0x34]
0063725c: bl #0x30e3ac
00637260: mov r1, fp
00637264: mov r7, r0
00637268: ldr r0, [sp, #0x40]
0063726c: bl #0x30ed6c
00637270: mov r1, r8
00637274: mov sl, r0
00637278: ldr r0, [sp, #0x3c]
0063727c: bl #0x30ed6c
00637280: mov r1, r0
00637284: mov r0, sl
00637288: bl #0x30eba4
0063728c: mov r1, r7
00637290: mov sl, r0
00637294: ldr r0, [sp, #0x38]
00637298: bl #0x30ed6c
0063729c: mov r1, r0
006372a0: mov r0, sl
006372a4: bl #0x30eba4
006372a8: ldr r1, [sp, #0x50]
006372ac: bl #0x30ec94
006372b0: mov r1, #0x3f800000
006372b4: mov sl, r0
006372b8: bl #0x30e2f8
006372bc: cmp r0, #0
006372c0: bne #0x6372dc
006372c4: mov r1, #0xbf000000
006372c8: mov r0, sl
006372cc: add r1, r1, #0x800000
006372d0: bl #0x30e70c
006372d4: cmp r0, #0
006372d8: beq #0x637308
006372dc: ldr r3, [sp, #0x20]
006372e0: add r4, r4, #0x9c
006372e4: cmp r3, r4
006372e8: bne #0x637098
006372ec: ldr r4, [sp, #0x18]
006372f0: ldr r1, [sp, #0x1c]
006372f4: mov r2, #0x41
006372f8: add r0, r4, #4
006372fc: bl #0x30e868
00637300: add sp, sp, #0xfc
00637304: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00637308: mov r1, fp
0063730c: ldr r0, [sp, #0x4c]
00637310: bl #0x30ed6c
00637314: mov r1, r8
00637318: mov sb, r0
0063731c: ldr r0, [sp, #0x48]
00637320: bl #0x30ed6c
00637324: mov r1, r0
00637328: mov r0, sb
0063732c: bl #0x30eba4
00637330: mov r1, r7
00637334: mov sb, r0
00637338: ldr r0, [sp, #0x44]
0063733c: bl #0x30ed6c
00637340: mov r1, r0
00637344: mov r0, sb
00637348: bl #0x30eba4
0063734c: ldr r1, [sp, #0x5c]
00637350: bl #0x30ec94
00637354: mov r1, #0x3f800000
00637358: mov sb, r0
0063735c: bl #0x30e2f8
00637360: cmp r0, #0
00637364: bne #0x6372dc
00637368: mov r1, #0xbf000000
0063736c: mov r0, sb
00637370: add r1, r1, #0x800000
00637374: bl #0x30e70c
00637378: cmp r0, #0
0063737c: bne #0x6372dc
00637380: ldr ip, [sp, #0x54]
00637384: ldr r3, [ip]
00637388: mov r0, ip
0063738c: mov lr, pc
00637390: ldr pc, [r3, #0x18]
00637394: mov r1, #0
00637398: str r0, [sp, #0xb0]
0063739c: ldr r0, [sp, #0x64]
006373a0: bl #0x30df8c
006373a4: cmp r0, #0
006373a8: movne lr, #0
006373ac: strne lr, [sp, #0xb4]
006373b0: beq #0x637a1c
006373b4: ldr r2, [sp, #0xe0]
006373b8: ldr ip, [sp, #0xec]
006373bc: ldr r0, [sp, #0xf0]
006373c0: mov r1, r2
006373c4: str r2, [sp, #0x10]
006373c8: str r0, [sp, #0x24]
006373cc: mov r0, ip
006373d0: str ip, [sp, #0xc]
006373d4: bl #0x30ed6c
006373d8: ldr r1, [sp, #0xe4]
006373dc: mov r3, r0
006373e0: ldr r0, [sp, #0x24]
006373e4: str r3, [sp, #8]
006373e8: bl #0x30ed6c
006373ec: ldr r3, [sp, #8]
006373f0: mov r1, r0
006373f4: mov r0, r3
006373f8: ldr r3, [sp, #0xf4]
006373fc: str r3, [sp, #0x28]
00637400: bl #0x30eba4
00637404: ldr r1, [sp, #0xe8]
00637408: mov r3, r0
0063740c: ldr r0, [sp, #0x28]
00637410: str r3, [sp, #8]
00637414: bl #0x30ed6c
00637418: ldr r3, [sp, #8]
0063741c: mov r1, r0
00637420: mov r0, r3
00637424: bl #0x30eba4
00637428: ldr ip, [sp, #0xc]
0063742c: add r0, r0, #0x80000000
00637430: str r0, [sp, #0x14]
00637434: mov r1, ip
00637438: ldr r0, [sp, #0x14]
0063743c: bl #0x30ed6c
00637440: ldr r2, [sp, #0x10]
00637444: mov r1, r0
00637448: mov r0, r2
0063744c: bl #0x30eba4
00637450: ldr r1, [sp, #0x24]
00637454: str r0, [sp, #0xac]
00637458: ldr r0, [sp, #0x14]
0063745c: bl #0x30ed6c
00637460: mov r1, r0
00637464: ldr r0, [sp, #0xe4]
00637468: bl #0x30eba4
0063746c: ldr r1, [sp, #0x28]
00637470: str r0, [sp, #0xa4]
00637474: ldr r0, [sp, #0x14]
00637478: bl #0x30ed6c
0063747c: mov r1, r0
00637480: ldr r0, [sp, #0xe8]
00637484: bl #0x30eba4
00637488: ldr r1, [sp, #0x58]
0063748c: str r0, [sp, #0xa8]
00637490: ldr r0, [sp, #0xb4]
00637494: bl #0x30eba4
00637498: mov r1, r0
0063749c: ldr r0, [sp, #0x14]
006374a0: bl #0x30ed6c
006374a4: str r0, [sp, #0x14]
006374a8: ldr r0, [sp, #0xac]
006374ac: mov r1, r0
006374b0: bl #0x30ed6c
006374b4: mov r3, r0
006374b8: ldr r0, [sp, #0xa4]
006374bc: str r3, [sp, #8]
006374c0: mov r1, r0
006374c4: bl #0x30ed6c
006374c8: ldr r3, [sp, #8]
006374cc: mov r1, r0
006374d0: mov r0, r3
006374d4: bl #0x30eba4
006374d8: mov r3, r0
006374dc: ldr r0, [sp, #0xa8]
006374e0: str r3, [sp, #8]
006374e4: mov r1, r0
006374e8: bl #0x30ed6c
006374ec: ldr r3, [sp, #8]
006374f0: mov r1, r0
006374f4: mov r0, r3
006374f8: bl #0x30eba4
006374fc: bl #0x30e8a4
00637500: bl #0x30e1c0
00637504: bl #0x30e6a0
00637508: mov r1, r6
0063750c: mov r3, r0
00637510: mov r0, #0x3f800000
00637514: str r3, [sp, #8]
00637518: bl #0x30e3ac
0063751c: mov r1, r5
00637520: bl #0x30ed6c
00637524: ldr r2, [sp, #0x14]
00637528: ldr r3, [sp, #8]
0063752c: str r0, [sp]
00637530: ldr r1, [sp, #0x6c]
00637534: ldr r0, [sp, #0x18]
00637538: bl #0x630abc
0063753c: ldr ip, [sp, #0xc]
00637540: mov r6, r0
00637544: ldr r0, [sp, #0x14]
00637548: mov r1, ip
0063754c: bl #0x30ed6c
00637550: ldr r1, [sp, #0xac]
00637554: mov r3, r0
00637558: mov r0, r6
0063755c: str r3, [sp, #8]
00637560: bl #0x30ed6c
00637564: ldr r3, [sp, #8]
00637568: mov r1, r0
0063756c: mov r0, r3
00637570: bl #0x30eba4
00637574: ldr r1, [sp, #0x24]
00637578: str r0, [sp, #0xe0]
0063757c: ldr r0, [sp, #0x14]
00637580: bl #0x30ed6c
00637584: ldr r1, [sp, #0xa4]
00637588: mov r3, r0
0063758c: mov r0, r6
00637590: str r3, [sp, #8]
00637594: bl #0x30ed6c
00637598: ldr r3, [sp, #8]
0063759c: mov r1, r0
006375a0: mov r0, r3
006375a4: bl #0x30eba4
006375a8: ldr r1, [sp, #0x28]
006375ac: str r0, [sp, #0xe4]
006375b0: ldr r0, [sp, #0x14]
006375b4: bl #0x30ed6c
006375b8: ldr r1, [sp, #0xa8]
006375bc: mov r3, r0
006375c0: mov r0, r6
006375c4: str r3, [sp, #8]
006375c8: bl #0x30ed6c
006375cc: ldr r3, [sp, #8]
006375d0: mov r1, r0
006375d4: mov r0, r3
006375d8: bl #0x30eba4
006375dc: mov r1, #0
006375e0: str r0, [sp, #0xe8]
006375e4: ldr r0, [sp, #0x68]
006375e8: bl #0x30e2f8
006375ec: cmp r0, #0
006375f0: bne #0x6377f0
006375f4: mov r1, fp
006375f8: ldr r0, [sp, #0x2c]
006375fc: bl #0x30eba4
00637600: mov r1, r8
00637604: mov fp, r0
00637608: ldr r0, [sp, #0x30]
0063760c: bl #0x30eba4
00637610: mov r1, r7
00637614: mov r6, r0
00637618: ldr r0, [sp, #0x34]
0063761c: bl #0x30eba4
00637620: mov r1, #0
00637624: mov r7, r0
00637628: ldr r0, [sp, #0x60]
0063762c: bl #0x30e2f8
00637630: cmp r0, #0
00637634: beq #0x63776c
00637638: ldr r1, [sp, #0x50]
0063763c: mov r0, sl
00637640: bl #0x30ed6c
00637644: ldr r1, [sp, #0x5c]
00637648: mov r8, r0
0063764c: mov r0, sb
00637650: bl #0x30ed6c
00637654: mov r1, r8
00637658: mov sl, r0
0063765c: ldr r0, [sp, #0x90]
00637660: bl #0x30ed6c
00637664: mov r1, r0
00637668: ldr r0, [sp, #0x70]
0063766c: bl #0x30eba4
00637670: mov r1, sl
00637674: mov sb, r0
00637678: ldr r0, [sp, #0x84]
0063767c: bl #0x30ed6c
00637680: mov r1, r0
00637684: mov r0, sb
00637688: bl #0x30eba4
0063768c: mov r1, r0
00637690: mov r0, fp
00637694: bl #0x30e3ac
00637698: mov r1, r0
0063769c: ldr r0, [sp, #0x60]
006376a0: bl #0x30ed6c
006376a4: mov r1, r0
006376a8: ldr r0, [sp, #0xe0]
006376ac: bl #0x30eba4
006376b0: mov r1, r8
006376b4: str r0, [sp, #0xe0]
006376b8: ldr r0, [sp, #0x8c]
006376bc: bl #0x30ed6c
006376c0: mov r1, r0
006376c4: ldr r0, [sp, #0x74]
006376c8: bl #0x30eba4
006376cc: mov r1, sl
006376d0: mov sb, r0
006376d4: ldr r0, [sp, #0x80]
006376d8: bl #0x30ed6c
006376dc: mov r1, r0
006376e0: mov r0, sb
006376e4: bl #0x30eba4
006376e8: mov r1, r0
006376ec: mov r0, r6
006376f0: bl #0x30e3ac
006376f4: mov r1, r0
006376f8: ldr r0, [sp, #0x60]
006376fc: bl #0x30ed6c
00637700: mov r1, r0
00637704: ldr r0, [sp, #0xe4]
00637708: bl #0x30eba4
0063770c: mov r1, r8
00637710: str r0, [sp, #0xe4]
00637714: ldr r0, [sp, #0x88]
00637718: bl #0x30ed6c
0063771c: mov r1, r0
00637720: ldr r0, [sp, #0x78]
00637724: bl #0x30eba4
00637728: mov r1, sl
0063772c: mov r8, r0
00637730: ldr r0, [sp, #0x7c]
00637734: bl #0x30ed6c
00637738: mov r1, r0
0063773c: mov r0, r8
00637740: bl #0x30eba4
00637744: mov r1, r0
00637748: mov r0, r7
0063774c: bl #0x30e3ac
00637750: mov r1, r0
00637754: ldr r0, [sp, #0x60]
00637758: bl #0x30ed6c
0063775c: mov r1, r0
00637760: ldr r0, [sp, #0xe8]
00637764: bl #0x30eba4
00637768: str r0, [sp, #0xe8]
0063776c: movw r1, #0x999a
00637770: ldr r0, [sp, #0xf0]
00637774: movt r1, #0x3e99
00637778: bl #0x30ed6c
0063777c: mov r1, r0
00637780: mov r0, r6
00637784: bl #0x30eba4
00637788: movw r1, #0x999a
0063778c: mov r6, r0
00637790: movt r1, #0x3e99
00637794: ldr r0, [sp, #0xf4]
00637798: bl #0x30ed6c
0063779c: mov r1, r0
006377a0: mov r0, r7
006377a4: bl #0x30eba4
006377a8: movw r1, #0x999a
006377ac: mov r7, r0
006377b0: movt r1, #0x3e99
006377b4: ldr r0, [sp, #0xec]
006377b8: bl #0x30ed6c
006377bc: mov r1, r0
006377c0: mov r0, fp
006377c4: bl #0x30eba4
006377c8: ldr r1, [sp, #0xe0]
006377cc: ldr r2, [sp, #0xe4]
006377d0: ldr r3, [sp, #0xe8]
006377d4: str r0, [r4]
006377d8: str r6, [r4, #4]
006377dc: str r7, [r4, #8]
006377e0: str r1, [r4, #0xc]
006377e4: str r2, [r4, #0x10]
006377e8: str r3, [r4, #0x14]
006377ec: b #0x6372dc
006377f0: mov r1, #0x43000000
006377f4: add r1, r1, #0x340000
006377f8: ldr r0, [sp, #0x68]
006377fc: bl #0x30ed6c
00637800: mov r6, r0
00637804: ldr r0, [sp, #0xb0]
00637808: bl #0x62fe78
0063780c: mov r2, r0
00637810: mov r3, r1
00637814: mov r0, r6
00637818: mov r1, #0xbf000000
0063781c: str r2, [sp, #0x10]
00637820: str r3, [sp, #8]
00637824: bl #0x30ed6c
00637828: ldr r2, [sp, #0x10]
0063782c: ldr r3, [sp, #8]
00637830: str r0, [sp, #0x14]
00637834: mov r0, r2
00637838: mov r1, r3
0063783c: bl #0x30e6a0
00637840: mov r1, r0
00637844: mov r0, r6
00637848: bl #0x30ed6c
0063784c: ldr r1, [sp, #0x14]
00637850: bl #0x30eba4
00637854: bl #0x30e8a4
00637858: ldr lr, [sp, #0x98]
0063785c: mov ip, #0
00637860: mov r2, r0
00637864: mov r3, r1
00637868: ldr r0, [sp, #0x94]
0063786c: str lr, [sp]
00637870: str ip, [sp, #0xd4]
00637874: str ip, [sp, #0xd8]
00637878: str ip, [sp, #0xdc]
0063787c: bl #0x630bfc
00637880: ldr r0, [sp, #0xb0]
00637884: bl #0x62fe78
00637888: bl #0x30e6a0
0063788c: mov r1, r0
00637890: mov r0, r6
00637894: bl #0x30ed6c
00637898: mov r1, r0
0063789c: ldr r0, [sp, #0x14]
006378a0: bl #0x30eba4
006378a4: bl #0x30e8a4
006378a8: ldr ip, [sp, #0x9c]
006378ac: mov r2, r0
006378b0: mov r3, r1
006378b4: ldr r0, [sp, #0x94]
006378b8: mov r1, #0
006378bc: str ip, [sp]
006378c0: str r1, [sp, #0xc8]
006378c4: str r1, [sp, #0xcc]
006378c8: str r1, [sp, #0xd0]
006378cc: bl #0x630cf8
006378d0: ldr r0, [sp, #0xb0]
006378d4: bl #0x62fe78
006378d8: bl #0x30e6a0
006378dc: mov r1, r0
006378e0: mov r0, r6
006378e4: bl #0x30ed6c
006378e8: mov r1, r0
006378ec: ldr r0, [sp, #0x14]
006378f0: bl #0x30eba4
006378f4: bl #0x30e8a4
006378f8: mov r3, r1
006378fc: ldr r1, [sp, #0xa0]
00637900: mov lr, #0
00637904: mov r2, r0
00637908: ldr r0, [sp, #0x94]
0063790c: str lr, [sp, #0xbc]
00637910: str lr, [sp, #0xc0]
00637914: str lr, [sp, #0xc4]
00637918: str r1, [sp]
0063791c: bl #0x630de8
00637920: ldr r2, [sp, #0xe0]
00637924: ldr r6, [sp, #0xec]
00637928: ldr ip, [sp, #0xe4]
0063792c: ldr r3, [sp, #0xf0]
00637930: mov r1, r2
00637934: mov r0, r6
00637938: str ip, [sp, #0x28]
0063793c: str r2, [sp, #0x14]
00637940: str r3, [sp, #0x24]
00637944: bl #0x30ed6c
00637948: ldr r1, [sp, #0x28]
0063794c: mov r3, r0
00637950: ldr r0, [sp, #0x24]
00637954: str r3, [sp, #8]
00637958: bl #0x30ed6c
0063795c: ldr r3, [sp, #8]
00637960: ldr lr, [sp, #0xf4]
00637964: ldr r2, [sp, #0xe8]
00637968: mov r1, r0
0063796c: mov r0, r3
00637970: str lr, [sp, #0xac]
00637974: str r2, [sp, #0xa4]
00637978: bl #0x30eba4
0063797c: ldr r1, [sp, #0xa4]
00637980: mov r3, r0
00637984: ldr r0, [sp, #0xac]
00637988: str r3, [sp, #8]
0063798c: bl #0x30ed6c
00637990: ldr r3, [sp, #8]
00637994: mov r1, r0
00637998: mov r0, r3
0063799c: bl #0x30eba4
006379a0: mov r1, #0
006379a4: str r0, [sp, #8]
006379a8: bl #0x30e70c
006379ac: cmp r0, #0
006379b0: ldr r3, [sp, #8]
006379b4: beq #0x6375f4
006379b8: mov r0, r3
006379bc: mov r1, #0xc0000000
006379c0: bl #0x30ed6c
006379c4: mov r1, r6
006379c8: str r0, [sp, #0xa8]
006379cc: bl #0x30ed6c
006379d0: mov r1, r0
006379d4: ldr r0, [sp, #0x14]
006379d8: bl #0x30eba4
006379dc: ldr r1, [sp, #0x24]
006379e0: str r0, [sp, #0xe0]
006379e4: ldr r0, [sp, #0xa8]
006379e8: bl #0x30ed6c
006379ec: mov r1, r0
006379f0: ldr r0, [sp, #0x28]
006379f4: bl #0x30eba4
006379f8: ldr r1, [sp, #0xac]
006379fc: str r0, [sp, #0xe4]
00637a00: ldr r0, [sp, #0xa8]
00637a04: bl #0x30ed6c
00637a08: mov r1, r0
00637a0c: ldr r0, [sp, #0xa4]
00637a10: bl #0x30eba4
00637a14: str r0, [sp, #0xe8]
00637a18: b #0x6375f4
00637a1c: ldr r0, [sp, #0xb0]
00637a20: bl #0x62fe78
00637a24: bl #0x30e6a0
00637a28: mov r1, r0
00637a2c: ldr r0, [sp, #0x64]
00637a30: bl #0x30ed6c
00637a34: mov r1, #0xbf000000
00637a38: mov r3, r0
00637a3c: ldr r0, [sp, #0x64]
00637a40: str r3, [sp, #8]
00637a44: bl #0x30ed6c
00637a48: ldr r3, [sp, #8]
00637a4c: mov r1, r0
00637a50: mov r0, r3
00637a54: bl #0x30eba4
00637a58: str r0, [sp, #0xb4]
00637a5c: b #0x6373b4

# 0x6349a0 _ZN6glitch2ps10PDeflectorC2ERNS1_10ParametersE
006349a0: push {r4, r5, r6, lr}
006349a4: mov r3, #0
006349a8: add r5, r0, #4
006349ac: mov r4, r0
006349b0: str r1, [r0]
006349b4: strb r3, [r0, #0x44]
006349b8: mov r1, r3
006349bc: mov r2, #0x40
006349c0: mov r0, r5
006349c4: bl #0x30e460
006349c8: ldr r3, [r4]
006349cc: mov r2, #0x3f800000
006349d0: mov r1, #1
006349d4: str r2, [r4, #0x40]
006349d8: str r2, [r4, #4]
006349dc: str r2, [r4, #0x18]
006349e0: str r2, [r4, #0x2c]
006349e4: strb r1, [r4, #0x44]
006349e8: mov r0, r5
006349ec: ldr r1, [r3]
006349f0: mov r2, #0x41
006349f4: bl #0x30e868
006349f8: mov r0, r4
006349fc: pop {r4, r5, r6, pc}

# 0x6301f8 _ZNK6glitch7collada15particle_system24CDeflectorForceSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
006301f8: push {r4, r5, r6, lr}
006301fc: mov r4, r1
00630200: mov r5, r0
00630204: bl #0x5972a4
00630208: ldr r1, [pc, #0xdc]
0063020c: mov r0, r4
00630210: ldr r2, [r5, #0x144]
00630214: ldr ip, [r4]
00630218: add r1, pc, r1
0063021c: mov r3, #0
00630220: mov lr, pc
00630224: ldr pc, [ip, #0x64]
00630228: ldr r1, [pc, #0xc0]
0063022c: mov r0, r4
00630230: ldr r2, [r5, #0x148]
00630234: ldr ip, [r4]
00630238: add r1, pc, r1
0063023c: mov r3, #0
00630240: mov lr, pc
00630244: ldr pc, [ip, #0x64]
00630248: ldr r1, [pc, #0xa4]
0063024c: mov r0, r4
00630250: ldr r2, [r5, #0x14c]
00630254: ldr ip, [r4]
00630258: add r1, pc, r1
0063025c: mov r3, #0
00630260: mov lr, pc
00630264: ldr pc, [ip, #0x64]
00630268: ldr r1, [pc, #0x88]
0063026c: mov r0, r4
00630270: ldr r2, [r5, #0x150]
00630274: ldr ip, [r4]
00630278: add r1, pc, r1
0063027c: mov r3, #0
00630280: mov lr, pc
00630284: ldr pc, [ip, #0x64]
00630288: ldr r1, [pc, #0x6c]
0063028c: mov r0, r4
00630290: ldr r2, [r5, #0x154]
00630294: ldr ip, [r4]
00630298: add r1, pc, r1
0063029c: mov r3, #0
006302a0: mov lr, pc
006302a4: ldr pc, [ip, #0x64]
006302a8: ldr r1, [pc, #0x50]
006302ac: mov r0, r4
006302b0: ldr r2, [r5, #0x158]
006302b4: ldr ip, [r4]
006302b8: add r1, pc, r1
006302bc: mov r3, #0
006302c0: mov lr, pc
006302c4: ldr pc, [ip, #0x64]
006302c8: ldr r1, [pc, #0x34]
006302cc: mov r0, r4
006302d0: ldr r2, [r5, #0x15c]
006302d4: add r1, pc, r1
006302d8: ldr ip, [r4]
006302dc: mov r3, #0
006302e0: mov lr, pc
006302e4: ldr pc, [ip, #0x64]
006302e8: pop {r4, r5, r6, pc}
