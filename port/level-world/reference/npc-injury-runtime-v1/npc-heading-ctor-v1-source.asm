# _ZN13AnchorForwardC1EP10GameObjectfffN10AnchorBase10AnchorTypeE 477a80 596
00477a80: push     {r4, r5, r6, r7, r8, sl, lr}
00477a84: sub      sp, sp, #0x1c
00477a88: mov      r6, r2
00477a8c: ldr      r5, [pc, #0x20c]
00477a90: ldr      r2, [sp, #0x3c]
00477a94: mov      r4, r0
00477a98: mov      r8, r3
00477a9c: mov      r7, r1
00477aa0: bl       #0x4771a0 ; _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE
00477aa4: ldr      r3, [pc, #0x1f8]
00477aa8: add      r5, pc, r5
00477aac: mov      r1, #0
00477ab0: ldr      r3, [r5, r3]
00477ab4: mov      sl, #0
00477ab8: mov      r0, r6
00477abc: add      r3, r3, #8
00477ac0: str      r3, [r4]
00477ac4: ldr      r3, [sp, #0x38]
00477ac8: str      r6, [r4, #0x1c]
00477acc: str      r8, [r4, #0x20]
00477ad0: str      r3, [r4, #0x24]
00477ad4: mov      r3, #5
00477ad8: str      r3, [r4, #0x3c]
00477adc: str      sl, [r4, #0x28]
00477ae0: str      r1, [r4, #0x2c]
00477ae4: str      r1, [r4, #0x30]
00477ae8: str      r1, [r4, #0x34]
00477aec: strb     sl, [r4, #0x38]
00477af0: str      r1, [r4, #0x40]
00477af4: str      r1, [r4, #0x44]
00477af8: str      r1, [r4, #0x48]
00477afc: strb     sl, [r4, #0x4c]
00477b00: str      sl, [r4, #0x50]
00477b04: str      r1, [r4, #0x54]
00477b08: str      r1, [r4, #0x58]
00477b0c: str      r1, [r4, #0x5c]
00477b10: str      r1, [r4, #0x60]
00477b14: bl       #0x30e4b4 ; 
00477b18: cmp      r0, sl
00477b1c: bne      #0x477b40
00477b20: ldr      r3, [pc, #0x180]
00477b24: ldr      r3, [r5, r3]
00477b28: ldr      r3, [r3]
00477b2c: cmp      r3, #2
00477b30: streq    sl, [sl]
00477b34: beq      #0x477b40
00477b38: cmp      r3, #1
00477b3c: beq      #0x477c34
00477b40: mov      r0, r8
00477b44: mov      r1, #0
00477b48: bl       #0x30e4b4 ; 
00477b4c: cmp      r0, #0
00477b50: bne      #0x477b78
00477b54: ldr      r3, [pc, #0x14c]
00477b58: ldr      r3, [r5, r3]
00477b5c: ldr      r3, [r3]
00477b60: cmp      r3, #2
00477b64: moveq    r3, #0
00477b68: streq    r3, [r3]
00477b6c: beq      #0x477b78
00477b70: cmp      r3, #1
00477b74: beq      #0x477c6c
00477b78: ldr      r6, [r4, #0x24]
00477b7c: mov      r1, #0
00477b80: mov      r0, r6
00477b84: bl       #0x30e4b4 ; 
00477b88: cmp      r0, #0
00477b8c: beq      #0x477bdc
00477b90: mov      r0, r6
00477b94: mov      r1, #0x3f800000
00477b98: bl       #0x30e9ac ; 
00477b9c: cmp      r0, #0
00477ba0: beq      #0x477bdc
00477ba4: cmp      r7, #0
00477ba8: beq      #0x477bc8
00477bac: add      r5, sp, #0xc
00477bb0: mov      r1, r7
00477bb4: mov      r0, r5
00477bb8: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00477bbc: mov      r0, r5
00477bc0: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00477bc4: str      r0, [r4, #0x28]
00477bc8: mov      r0, r4
00477bcc: bl       #0x4779dc ; _ZN13AnchorForward5ResetEv
00477bd0: mov      r0, r4
00477bd4: add      sp, sp, #0x1c
00477bd8: pop      {r4, r5, r6, r7, r8, sl, pc}
00477bdc: ldr      r3, [pc, #0xc4]
00477be0: ldr      r3, [r5, r3]
00477be4: ldr      r3, [r3]
00477be8: cmp      r3, #2
00477bec: moveq    r3, #0
00477bf0: streq    r3, [r3]
00477bf4: beq      #0x477ba4
00477bf8: cmp      r3, #1
00477bfc: bne      #0x477ba4
00477c00: ldr      r0, [pc, #0xa4]
00477c04: ldr      r1, [pc, #0xa4]
00477c08: ldr      r2, [pc, #0xa4]
00477c0c: ldr      r0, [r5, r0]
00477c10: ldr      r3, [pc, #0xa0]
00477c14: mov      ip, #0x32
00477c18: add      r1, pc, r1
00477c1c: add      r2, pc, r2
00477c20: add      r3, pc, r3
00477c24: add      r0, r0, #0xa8
00477c28: str      ip, [sp]
00477c2c: bl       #0x30e004 ; 
00477c30: b        #0x477ba4
00477c34: ldr      r0, [pc, #0x70]
00477c38: ldr      r1, [pc, #0x7c]
00477c3c: ldr      r2, [pc, #0x7c]
00477c40: ldr      r0, [r5, r0]
00477c44: ldr      r3, [pc, #0x78]
00477c48: mov      ip, #0x30
00477c4c: add      r1, pc, r1
00477c50: add      r0, r0, #0xa8
00477c54: add      r2, pc, r2
00477c58: add      r3, pc, r3
00477c5c: str      ip, [sp]
00477c60: bl       #0x30e004 ; 
00477c64: ldr      r8, [r4, #0x20]
00477c68: b        #0x477b40
00477c6c: ldr      r0, [pc, #0x38]
00477c70: ldr      r1, [pc, #0x50]
00477c74: ldr      r2, [pc, #0x50]
00477c78: ldr      r0, [r5, r0]
00477c7c: ldr      r3, [pc, #0x4c]
00477c80: mov      ip, #0x31
00477c84: add      r1, pc, r1
00477c88: add      r2, pc, r2
00477c8c: add      r3, pc, r3
00477c90: add      r0, r0, #0xa8
00477c94: str      ip, [sp]
00477c98: bl       #0x30e004 ; 
00477c9c: b        #0x477b78
00477ca0: subseq   ip, r1, r8, ror #31
00477ca4: muleq    r0, r8, r3
00477ca8: andeq    r3, r0, r0, asr #19
00477cac: andeq    r1, r0, r0, asr #19
00477cb0: subeq    r6, r4, r0, asr #15
00477cb4: subeq    r5, r5, r4, lsr #27
00477cb8: subeq    r5, r5, r0, lsr sp
00477cbc: subeq    r6, r4, ip, lsl #15
00477cc0: subeq    r5, r5, r4, ror #25
00477cc4: strdeq   r5, r6, [r5], #-0xc8
00477cc8: subeq    r6, r4, r4, asr r7
00477ccc: subeq    r5, r5, r0, lsr #26
00477cd0: subeq    r5, r5, r4, asr #25
# _ZN10GameObject11LookTowardsERK7Point3DIfE 393b1c 204
00393b1c: push     {r4, r5, r6, lr}
00393b20: ldr      r5, [r1, #4]
00393b24: mov      r4, r1
00393b28: mov      r6, r0
00393b2c: mov      r1, #0
00393b30: mov      r0, r5
00393b34: bl       #0x30df8c ; 
00393b38: cmp      r0, #0
00393b3c: beq      #0x393b7c
00393b40: ldr      r4, [r4]
00393b44: mov      r1, #0
00393b48: mov      r0, r4
00393b4c: bl       #0x30e2f8 ; 
00393b50: cmp      r0, #0
00393b54: bne      #0x393bd8
00393b58: mov      r0, r4
00393b5c: mov      r1, #0
00393b60: bl       #0x30e70c ; 
00393b64: cmp      r0, #0
00393b68: beq      #0x393bd4
00393b6c: movw     r3, #0xcbe4
00393b70: movt     r3, #0x4096
00393b74: str      r3, [r6, #0x178]
00393b78: pop      {r4, r5, r6, pc}
00393b7c: add      r1, r5, #0x80000000
00393b80: ldr      r0, [r4]
00393b84: bl       #0x30ec94 ; 
00393b88: bl       #0x30e79c ; 
00393b8c: str      r0, [r6, #0x178]
00393b90: mov      r5, r0
00393b94: mov      r1, #0
00393b98: ldr      r0, [r4, #4]
00393b9c: bl       #0x30e2f8 ; 
00393ba0: cmp      r0, #0
00393ba4: beq      #0x393bd4
00393ba8: ldr      r0, [r4]
00393bac: mov      r1, #0
00393bb0: bl       #0x30e2f8 ; 
00393bb4: cmp      r0, #0
00393bb8: movweq   r0, #0xfdb
00393bbc: movwne   r0, #0xfdb
00393bc0: movteq   r0, #0xc049
00393bc4: movtne   r0, #0x4049
00393bc8: mov      r1, r5
00393bcc: bl       #0x30eba4 ; 
00393bd0: str      r0, [r6, #0x178]
00393bd4: pop      {r4, r5, r6, pc}
00393bd8: movw     r3, #0xfdb
00393bdc: movt     r3, #0x3fc9
00393be0: str      r3, [r6, #0x178]
00393be4: pop      {r4, r5, r6, pc}
# _ZN13AnchorForwardC2EP10GameObjectfffN10AnchorBase10AnchorTypeE 477cd4 596
00477cd4: push     {r4, r5, r6, r7, r8, sl, lr}
00477cd8: sub      sp, sp, #0x1c
00477cdc: mov      r6, r2
00477ce0: ldr      r5, [pc, #0x20c]
00477ce4: ldr      r2, [sp, #0x3c]
00477ce8: mov      r4, r0
00477cec: mov      r8, r3
00477cf0: mov      r7, r1
00477cf4: bl       #0x4771a0 ; _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE
00477cf8: ldr      r3, [pc, #0x1f8]
00477cfc: add      r5, pc, r5
00477d00: mov      r1, #0
00477d04: ldr      r3, [r5, r3]
00477d08: mov      sl, #0
00477d0c: mov      r0, r6
00477d10: add      r3, r3, #8
00477d14: str      r3, [r4]
00477d18: ldr      r3, [sp, #0x38]
00477d1c: str      r6, [r4, #0x1c]
00477d20: str      r8, [r4, #0x20]
00477d24: str      r3, [r4, #0x24]
00477d28: mov      r3, #5
00477d2c: str      r3, [r4, #0x3c]
00477d30: str      sl, [r4, #0x28]
00477d34: str      r1, [r4, #0x2c]
00477d38: str      r1, [r4, #0x30]
00477d3c: str      r1, [r4, #0x34]
00477d40: strb     sl, [r4, #0x38]
00477d44: str      r1, [r4, #0x40]
00477d48: str      r1, [r4, #0x44]
00477d4c: str      r1, [r4, #0x48]
00477d50: strb     sl, [r4, #0x4c]
00477d54: str      sl, [r4, #0x50]
00477d58: str      r1, [r4, #0x54]
00477d5c: str      r1, [r4, #0x58]
00477d60: str      r1, [r4, #0x5c]
00477d64: str      r1, [r4, #0x60]
00477d68: bl       #0x30e4b4 ; 
00477d6c: cmp      r0, sl
00477d70: bne      #0x477d94
00477d74: ldr      r3, [pc, #0x180]
00477d78: ldr      r3, [r5, r3]
00477d7c: ldr      r3, [r3]
00477d80: cmp      r3, #2
00477d84: streq    sl, [sl]
00477d88: beq      #0x477d94
00477d8c: cmp      r3, #1
00477d90: beq      #0x477e88
00477d94: mov      r0, r8
00477d98: mov      r1, #0
00477d9c: bl       #0x30e4b4 ; 
00477da0: cmp      r0, #0
00477da4: bne      #0x477dcc
00477da8: ldr      r3, [pc, #0x14c]
00477dac: ldr      r3, [r5, r3]
00477db0: ldr      r3, [r3]
00477db4: cmp      r3, #2
00477db8: moveq    r3, #0
00477dbc: streq    r3, [r3]
00477dc0: beq      #0x477dcc
00477dc4: cmp      r3, #1
00477dc8: beq      #0x477ec0
00477dcc: ldr      r6, [r4, #0x24]
00477dd0: mov      r1, #0
00477dd4: mov      r0, r6
00477dd8: bl       #0x30e4b4 ; 
00477ddc: cmp      r0, #0
00477de0: beq      #0x477e30
00477de4: mov      r0, r6
00477de8: mov      r1, #0x3f800000
00477dec: bl       #0x30e9ac ; 
00477df0: cmp      r0, #0
00477df4: beq      #0x477e30
00477df8: cmp      r7, #0
00477dfc: beq      #0x477e1c
00477e00: add      r5, sp, #0xc
00477e04: mov      r1, r7
00477e08: mov      r0, r5
00477e0c: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00477e10: mov      r0, r5
00477e14: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00477e18: str      r0, [r4, #0x28]
00477e1c: mov      r0, r4
00477e20: bl       #0x4779dc ; _ZN13AnchorForward5ResetEv
00477e24: mov      r0, r4
00477e28: add      sp, sp, #0x1c
00477e2c: pop      {r4, r5, r6, r7, r8, sl, pc}
00477e30: ldr      r3, [pc, #0xc4]
00477e34: ldr      r3, [r5, r3]
00477e38: ldr      r3, [r3]
00477e3c: cmp      r3, #2
00477e40: moveq    r3, #0
00477e44: streq    r3, [r3]
00477e48: beq      #0x477df8
00477e4c: cmp      r3, #1
00477e50: bne      #0x477df8
00477e54: ldr      r0, [pc, #0xa4]
00477e58: ldr      r1, [pc, #0xa4]
00477e5c: ldr      r2, [pc, #0xa4]
00477e60: ldr      r0, [r5, r0]
00477e64: ldr      r3, [pc, #0xa0]
00477e68: mov      ip, #0x32
00477e6c: add      r1, pc, r1
00477e70: add      r2, pc, r2
00477e74: add      r3, pc, r3
00477e78: add      r0, r0, #0xa8
00477e7c: str      ip, [sp]
00477e80: bl       #0x30e004 ; 
00477e84: b        #0x477df8
00477e88: ldr      r0, [pc, #0x70]
00477e8c: ldr      r1, [pc, #0x7c]
00477e90: ldr      r2, [pc, #0x7c]
00477e94: ldr      r0, [r5, r0]
00477e98: ldr      r3, [pc, #0x78]
00477e9c: mov      ip, #0x30
00477ea0: add      r1, pc, r1
00477ea4: add      r0, r0, #0xa8
00477ea8: add      r2, pc, r2
00477eac: add      r3, pc, r3
00477eb0: str      ip, [sp]
00477eb4: bl       #0x30e004 ; 
00477eb8: ldr      r8, [r4, #0x20]
00477ebc: b        #0x477d94
00477ec0: ldr      r0, [pc, #0x38]
00477ec4: ldr      r1, [pc, #0x50]
00477ec8: ldr      r2, [pc, #0x50]
00477ecc: ldr      r0, [r5, r0]
00477ed0: ldr      r3, [pc, #0x4c]
00477ed4: mov      ip, #0x31
00477ed8: add      r1, pc, r1
00477edc: add      r2, pc, r2
00477ee0: add      r3, pc, r3
00477ee4: add      r0, r0, #0xa8
00477ee8: str      ip, [sp]
00477eec: bl       #0x30e004 ; 
00477ef0: b        #0x477dcc
# _ZN14PhysicalObjectC1EP13PhysicalWorldP10GameObjectbbbbstti 46ef68 904
0046ef68: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ef6c: ldr      r5, [pc, #0x360]
0046ef70: ldr      r4, [pc, #0x360]
0046ef74: sub      sp, sp, #0xb4
0046ef78: add      r5, pc, r5
0046ef7c: str      r4, [sp, #0x10]
0046ef80: ldr      lr, [r5, r4]
0046ef84: ldr      ip, [pc, #0x350]
0046ef88: ldr      r4, [pc, #0x350]
0046ef8c: ldrb     sb, [sp, #0xd8]
0046ef90: ldr      ip, [r5, ip]
0046ef94: ldr      r4, [r5, r4]
0046ef98: ldr      lr, [lr]
0046ef9c: mov      r7, #0
0046efa0: str      r4, [sp, #0xc]
0046efa4: add      ip, ip, #8
0046efa8: mov      r4, r0
0046efac: mov      sl, #0
0046efb0: str      r1, [r0, #4]
0046efb4: str      ip, [r0]
0046efb8: str      r2, [r4, #8]
0046efbc: str      sl, [r0, #0xc]
0046efc0: strb     sb, [r0, #0x10]
0046efc4: str      r7, [r0, #0x14]
0046efc8: str      r7, [r0, #0x18]
0046efcc: str      r7, [r0, #0x1c]
0046efd0: strb     r7, [r0, #0x26]
0046efd4: strb     r7, [r0, #0x27]
0046efd8: ldrb     ip, [sp, #0xdc]
0046efdc: mov      r6, r2
0046efe0: str      lr, [sp, #0xac]
0046efe4: ldrh     r2, [sp, #0xe8]
0046efe8: ldrb     lr, [sp, #0xe0]
0046efec: str      r3, [sp, #0x1c]
0046eff0: ldrh     r3, [sp, #0xec]
0046eff4: ldr      r0, [sp, #0xc]
0046eff8: str      ip, [sp, #0x20]
0046effc: str      lr, [sp, #0x24]
0046f000: str      r3, [sp, #0x18]
0046f004: ldrsh    r8, [sp, #0xe4]
0046f008: str      r2, [sp, #0x14]
0046f00c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0046f010: ldr      r1, [pc, #0x2cc]
0046f014: add      fp, sp, #0x94
0046f018: add      r2, sp, #0x90
0046f01c: add      r1, pc, r1
0046f020: mov      r0, fp
0046f024: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0046f028: mov      r1, fp
0046f02c: ldr      r0, [sp, #0xc]
0046f030: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0046f034: mov      r3, r0
0046f038: mov      r0, fp
0046f03c: str      r3, [sp, #8]
0046f040: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0046f044: ldr      r3, [sp, #8]
0046f048: mvn      r2, #0x298
0046f04c: sub      r2, r2, #1
0046f050: cmp      r3, r7
0046f054: movne    r8, r2
0046f058: cmp      r6, r7
0046f05c: beq      #0x46f1d0
0046f060: cmp      sb, r7
0046f064: bne      #0x46f1f4
0046f068: ldr      r3, [pc, #0x278]
0046f06c: mov      r2, #1
0046f070: ldr      r1, [r6, #0x12c]
0046f074: ldr      r3, [r5, r3]
0046f078: ldr      r0, [r6, #0x138]
0046f07c: str      r2, [sp, #0x30]
0046f080: add      ip, r3, #8
0046f084: movw     r3, #0xcccd
0046f088: movt     r3, #0x3e4c
0046f08c: strh     r2, [sp, #0x46]
0046f090: mvn      r2, #0
0046f094: str      ip, [sp, #0x2c]
0046f098: strh     r2, [sp, #0x48]
0046f09c: str      r3, [sp, #0x38]
0046f0a0: str      sl, [sp, #0x40]
0046f0a4: str      sb, [sp, #0x8c]
0046f0a8: str      sb, [sp, #0x34]
0046f0ac: str      sl, [sp, #0x3c]
0046f0b0: strh     sb, [sp, #0x4a]
0046f0b4: strb     sb, [sp, #0x44]
0046f0b8: bl       #0x30e3ac ; 
0046f0bc: movw     r1, #0xd70a
0046f0c0: movt     r1, #0x3c23
0046f0c4: bl       #0x30ed6c ; 
0046f0c8: ldr      r1, [r6, #0x130]
0046f0cc: mov      sb, r0
0046f0d0: ldr      r0, [r6, #0x13c]
0046f0d4: bl       #0x30e3ac ; 
0046f0d8: movw     r1, #0xd70a
0046f0dc: movt     r1, #0x3c23
0046f0e0: bl       #0x30ed6c ; 
0046f0e4: movw     r1, #0xd70a
0046f0e8: mov      r7, r0
0046f0ec: movt     r1, #0x3c23
0046f0f0: ldr      r0, [r6, #0x160]
0046f0f4: bl       #0x30ed6c ; 
0046f0f8: movw     r1, #0xd70a
0046f0fc: movt     r1, #0x3c23
0046f100: mov      sl, r0
0046f104: ldr      r0, [r6, #0x164]
0046f108: bl       #0x30ed6c ; 
0046f10c: mov      r1, #0x3f000000
0046f110: mov      r6, r0
0046f114: mov      r0, sb
0046f118: bl       #0x30ed6c ; 
0046f11c: mov      r1, #0x3f000000
0046f120: mov      r3, r0
0046f124: mov      r0, r7
0046f128: str      r3, [sp, #8]
0046f12c: bl       #0x30ed6c ; 
0046f130: ldr      r3, [sp, #8]
0046f134: add      fp, sp, #0x2c
0046f138: mov      r2, r0
0046f13c: mov      r1, r3
0046f140: mov      r0, fp
0046f144: bl       #0x7e44b8 ; _ZN12b2PolygonDef8SetAsBoxEff
0046f148: mov      r1, r7
0046f14c: mov      r0, sb
0046f150: bl       #0x30e70c ; 
0046f154: cmp      r0, #0
0046f158: moveq    r7, sb
0046f15c: mov      r0, r7
0046f160: mov      r1, #0x3f000000
0046f164: bl       #0x30ed6c ; 
0046f168: ldr      r3, [pc, #0x17c]
0046f16c: str      r0, [r4, #0xc]
0046f170: mov      ip, fp
0046f174: ldr      r3, [r5, r3]
0046f178: add      r3, r3, #8
0046f17c: str      r3, [sp, #0x2c]
0046f180: strh     r8, [r4, #0x24]
0046f184: ldr      lr, [sp, #0x14]
0046f188: mov      r1, ip
0046f18c: mov      r3, r6
0046f190: strh     lr, [r4, #0x20]
0046f194: ldr      r2, [sp, #0x18]
0046f198: mov      r0, r4
0046f19c: strh     r2, [r4, #0x22]
0046f1a0: ldr      lr, [sp, #0x20]
0046f1a4: strh     r8, [ip, #0x1e]
0046f1a8: mov      r2, sl
0046f1ac: strb     lr, [ip, #0x18]
0046f1b0: ldr      lr, [sp, #0x14]
0046f1b4: strh     lr, [ip, #0x1a]
0046f1b8: ldr      lr, [sp, #0x18]
0046f1bc: strh     lr, [ip, #0x1c]
0046f1c0: ldr      ip, [sp, #0x1c]
0046f1c4: ldr      lr, [sp, #0x24]
0046f1c8: stm      sp, {ip, lr}
0046f1cc: bl       #0x46edf0 ; _ZN14PhysicalObject5_initEP10b2ShapeDefffbb
0046f1d0: ldr      ip, [sp, #0x10]
0046f1d4: ldr      r2, [sp, #0xac]
0046f1d8: mov      r0, r4
0046f1dc: ldr      r3, [r5, ip]
0046f1e0: ldr      r3, [r3]
0046f1e4: cmp      r2, r3
0046f1e8: bne      #0x46f2d0
0046f1ec: add      sp, sp, #0xb4
0046f1f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f1f4: movw     r3, #0xcccd
0046f1f8: ldr      r1, [r6, #0x12c]
0046f1fc: ldr      r0, [r6, #0x138]
0046f200: movt     r3, #0x3e4c
0046f204: mov      ip, #1
0046f208: mvn      lr, #0
0046f20c: str      r3, [sp, #0x38]
0046f210: strh     ip, [sp, #0x46]
0046f214: strh     lr, [sp, #0x48]
0046f218: strb     r7, [sp, #0x44]
0046f21c: str      r7, [sp, #0x30]
0046f220: str      sl, [sp, #0x50]
0046f224: str      r7, [sp, #0x34]
0046f228: str      sl, [sp, #0x3c]
0046f22c: str      sl, [sp, #0x40]
0046f230: strh     r7, [sp, #0x4a]
0046f234: str      sl, [sp, #0x4c]
0046f238: bl       #0x30e3ac ; 
0046f23c: movw     r1, #0xd70a
0046f240: movt     r1, #0x3c23
0046f244: bl       #0x30ed6c ; 
0046f248: ldr      r1, [r6, #0x130]
0046f24c: mov      sb, r0
0046f250: ldr      r0, [r6, #0x13c]
0046f254: bl       #0x30e3ac ; 
0046f258: movw     r1, #0xd70a
0046f25c: movt     r1, #0x3c23
0046f260: bl       #0x30ed6c ; 
0046f264: movw     r1, #0xd70a
0046f268: mov      r7, r0
0046f26c: movt     r1, #0x3c23
0046f270: ldr      r0, [r6, #0x160]
0046f274: bl       #0x30ed6c ; 
0046f278: movw     r1, #0xd70a
0046f27c: movt     r1, #0x3c23
0046f280: mov      sl, r0
0046f284: ldr      r0, [r6, #0x164]
0046f288: bl       #0x30ed6c ; 
0046f28c: mov      r1, r7
0046f290: mov      r6, r0
0046f294: mov      r0, sb
0046f298: bl       #0x30e70c ; 
0046f29c: cmp      r0, #0
0046f2a0: moveq    r7, sb
0046f2a4: mov      r0, r7
0046f2a8: mov      r1, #0x3f000000
0046f2ac: bl       #0x30ed6c ; 
0046f2b0: ldr      r3, [pc, #0x34]
0046f2b4: add      ip, sp, #0xb0
0046f2b8: str      r0, [r4, #0xc]
0046f2bc: ldr      r3, [r5, r3]
0046f2c0: str      r0, [sp, #0x54]
0046f2c4: add      r3, r3, #8
0046f2c8: str      r3, [ip, #-0x84]!
0046f2cc: b        #0x46f180
0046f2d0: bl       #0x30e310 ; 
0046f2d4: subseq   r5, r2, r8, lsl fp
0046f2d8: andeq    r4, r0, ip, lsr #1
0046f2dc: andeq    r2, r0, r4, lsl #9
0046f2e0: andeq    r0, r0, r4, lsl #17
0046f2e4: subeq    lr, r5, ip, asr #11
0046f2e8: ldrdeq   r4, r5, [r0], -ip
0046f2ec: andeq    r3, r0, r8, lsl r8
# _ZN12VisualObjectC2EP10GameObjectRKSsS3_ 472c5c 592
00472c5c: push     {r4, r5, r6, r7, r8, sl, lr}
00472c60: ldr      r6, [pc, #0x234]
00472c64: ldr      r5, [pc, #0x234]
00472c68: mov      ip, #0xbf000000
00472c6c: add      r6, pc, r6
00472c70: ldr      r5, [r6, r5]
00472c74: mov      lr, #0
00472c78: add      ip, ip, #0x800000
00472c7c: mov      r7, r1
00472c80: add      r1, r5, #8
00472c84: mov      r5, #0
00472c88: mov      r8, r3
00472c8c: sub      sp, sp, #0xc
00472c90: str      r1, [r0]
00472c94: str      lr, [r0, #0x24]
00472c98: str      ip, [r0, #0x74]
00472c9c: str      lr, [r0, #0x10]
00472ca0: str      lr, [r0, #0x14]
00472ca4: str      lr, [r0, #0x18]
00472ca8: str      lr, [r0, #0x1c]
00472cac: str      lr, [r0, #0x20]
00472cb0: str      ip, [r0, #0x58]
00472cb4: str      ip, [r0, #0x5c]
00472cb8: str      ip, [r0, #0x60]
00472cbc: str      ip, [r0, #0x64]
00472cc0: str      ip, [r0, #0x68]
00472cc4: str      r7, [r0, #4]
00472cc8: str      r5, [r0, #8]
00472ccc: str      r5, [r0, #0xc]
00472cd0: strb     r5, [r0, #0x28]
00472cd4: str      r5, [r0, #0x2c]
00472cd8: str      r5, [r0, #0x30]
00472cdc: str      r5, [r0, #0x34]
00472ce0: str      r5, [r0, #0x38]
00472ce4: strb     r5, [r0, #0x3c]
00472ce8: str      r5, [r0, #0x40]
00472cec: str      r5, [r0, #0x44]
00472cf0: str      r5, [r0, #0x48]
00472cf4: str      r5, [r0, #0x4c]
00472cf8: str      r5, [r0, #0x50]
00472cfc: str      r5, [r0, #0x54]
00472d00: strb     r5, [r0, #0x6c]
00472d04: strb     r5, [r0, #0x7c]
00472d08: strb     r5, [r0, #0x7d]
00472d0c: strb     r5, [r0, #0x7e]
00472d10: strb     r5, [r0, #0x7f]
00472d14: str      r5, [r0, #0x80]
00472d18: str      r5, [r0, #0x84]
00472d1c: str      r5, [r0, #0x88]
00472d20: str      r5, [r0, #0x8c]
00472d24: str      r5, [r0, #0x90]
00472d28: str      r5, [r0, #0x94]
00472d2c: str      r5, [r0, #0x9c]
00472d30: str      r5, [r0, #0xa0]
00472d34: str      r5, [r0, #0xa4]
00472d38: strb     r5, [r0, #0xa9]
00472d3c: mov      sl, r2
00472d40: mov      r4, r0
00472d44: bl       #0x50a564 ; _ZN12AssetManager15GetAssetManagerEv
00472d48: ldr      ip, [r8, #0x10]
00472d4c: ldr      r2, [r8, #0x14]
00472d50: ldr      r1, [sl, #0x14]
00472d54: mov      r3, r5
00472d58: cmp      ip, r2
00472d5c: moveq    r2, r5
00472d60: mvn      ip, #0x80000000
00472d64: str      ip, [sp]
00472d68: bl       #0x50a504 ; _ZN12AssetManager13loadSceneNodeEPKcS1_bi
00472d6c: cmp      r0, r5
00472d70: str      r0, [r4, #8]
00472d74: beq      #0x472e90
00472d78: mov      r1, r7
00472d7c: mov      r0, r4
00472d80: bl       #0x47295c ; _ZN12VisualObject9SetParentEP10GameObject
00472d84: ldr      r0, [r4, #8]
00472d88: bl       #0x35c854 ; _ZN13RootSceneNode18RefreshBoundingBoxEv
00472d8c: mov      r0, r4
00472d90: bl       #0x4718f0 ; _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
00472d94: ldr      r3, [pc, #0x108]
00472d98: ldr      r1, [r4, #8]
00472d9c: ldr      r5, [r6, r3]
00472da0: ldr      r3, [r5, #0x10]
00472da4: ldr      r3, [r3, #0x1c]
00472da8: ldr      r3, [r3, #4]
00472dac: mov      r0, r3
00472db0: ldr      r3, [r3]
00472db4: mov      lr, pc
00472db8: ldr      pc, [r3, #0x5c]
00472dbc: ldr      r3, [r5, #0x10]
00472dc0: ldr      r0, [r3, #0x1c]
00472dc4: bl       #0x350ee0 ; _ZN12SceneManager13ForceRegisterEv
00472dc8: ldr      r3, [r5, #0x10]
00472dcc: ldr      r2, [pc, #0xd4]
00472dd0: ldr      r1, [r4, #8]
00472dd4: ldr      r0, [r3, #0x1c]
00472dd8: add      r2, pc, r2
00472ddc: mov      r3, #1
00472de0: bl       #0x35a0e4 ; _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
00472de4: subs     r2, r0, #0
00472de8: beq      #0x472e58
00472dec: mov      r3, #1
00472df0: strb     r3, [r4, #0x28]
00472df4: ldr      r3, [r5, #0x10]
00472df8: movw     r1, #0x6164
00472dfc: movt     r1, #0x6d65
00472e00: ldr      r3, [r3, #0x1c]
00472e04: mov      r0, r3
00472e08: ldr      r3, [r3]
00472e0c: mov      lr, pc
00472e10: ldr      pc, [r3, #0x1c]
00472e14: cmp      r0, #0
00472e18: str      r0, [r4, #0xc]
00472e1c: beq      #0x472e3c
00472e20: ldr      r3, [r0]
00472e24: ldr      r3, [r3, #-0xc]
00472e28: add      r0, r0, r3
00472e2c: ldr      r3, [r0, #4]
00472e30: add      r3, r3, #1
00472e34: str      r3, [r0, #4]
00472e38: ldr      r0, [r4, #0xc]
00472e3c: mov      r1, #0
00472e40: strb     r1, [r0, #0x138]
00472e44: ldr      r3, [r4, #0xc]
00472e48: mov      r0, r3
00472e4c: ldr      r3, [r3]
00472e50: mov      lr, pc
00472e54: ldr      pc, [r3, #0x48]
00472e58: mov      r0, r4
00472e5c: bl       #0x47211c ; _ZN12VisualObject11CalcMeshBoxEv
00472e60: mov      r0, r4
00472e64: bl       #0x470a54 ; _ZN12VisualObject12ApplyMeshBoxEv
00472e68: mov      r1, #0
00472e6c: mov      r0, #8
00472e70: bl       #0x310570 ; _Znwj15MemoryHintState
00472e74: ldr      r1, [r4, #8]
00472e78: mov      r5, r0
00472e7c: mov      r2, #0
00472e80: bl       #0x474d30 ; _ZN14AnimControllerC1EP13RootSceneNodeb
00472e84: mov      r0, r4
00472e88: mov      r1, r5
00472e8c: bl       #0x470a84 ; _ZN12VisualObject17SetAnimControllerEP14AnimController
00472e90: mov      r0, r4
00472e94: add      sp, sp, #0xc
00472e98: pop      {r4, r5, r6, r7, r8, sl, pc}
00472e9c: subseq   r1, r2, r4, lsr #28
00472ea0: muleq    r0, r4, lr
00472ea4: strdeq   r3, r4, [r0], -r4
00472ea8: ldrdeq   sl, fp, [r5], #-0x80
# _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE 4771a0 208
004771a0: ldr      r3, [pc, #0xac]
004771a4: push     {r4, lr}
004771a8: ldr      lr, [pc, #0xa8]
004771ac: add      r3, pc, r3
004771b0: mov      ip, #0
004771b4: ldr      lr, [r3, lr]
004771b8: str      r2, [r0, #4]
004771bc: cmp      r1, #0
004771c0: add      lr, lr, #8
004771c4: mov      r2, #1
004771c8: sub      sp, sp, #8
004771cc: mov      r4, r0
004771d0: str      lr, [r0]
004771d4: str      ip, [r0, #0x14]
004771d8: strb     r2, [r0, #0x18]
004771dc: str      r1, [r0, #8]
004771e0: str      ip, [r0, #0xc]
004771e4: str      ip, [r0, #0x10]
004771e8: beq      #0x477200
004771ec: mov      r0, r4
004771f0: bl       #0x477074 ; _ZN10AnchorBase5ResetEv
004771f4: mov      r0, r4
004771f8: add      sp, sp, #8
004771fc: pop      {r4, pc}
00477200: ldr      r2, [pc, #0x54]
00477204: ldr      r2, [r3, r2]
00477208: ldr      r2, [r2]
0047720c: cmp      r2, #2
00477210: streq    r1, [r1]
00477214: beq      #0x4771ec
00477218: cmp      r2, #1
0047721c: bne      #0x4771ec
00477220: ldr      r0, [pc, #0x38]
00477224: ldr      r1, [pc, #0x38]
00477228: ldr      r2, [pc, #0x38]
0047722c: ldr      r0, [r3, r0]
00477230: ldr      r3, [pc, #0x34]
00477234: mov      ip, #0x18
00477238: add      r1, pc, r1
0047723c: add      r2, pc, r2
00477240: add      r3, pc, r3
00477244: add      r0, r0, #0xa8
00477248: str      ip, [sp]
0047724c: bl       #0x30e004 ; 
00477250: b        #0x4771ec
00477254: subseq   sp, r1, r4, ror #17
00477258: strheq   r1, [r0], -ip
0047725c: andeq    r3, r0, r0, asr #19
00477260: andeq    r1, r0, r0, asr #19
00477264: subeq    r7, r4, r0, lsr #3
00477268: subeq    fp, r4, ip, asr r1
0047726c: subeq    r6, r5, r8, lsr #13
# _ZN11AnchorGroupC1EP10GameObjectN10AnchorBase10AnchorTypeE 478578 60
00478578: push     {r4, r5, r6, lr}
0047857c: ldr      r4, [pc, #0x28]
00478580: mov      r5, r0
00478584: bl       #0x4771a0 ; _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE
00478588: ldr      r3, [pc, #0x20]
0047858c: add      r4, pc, r4
00478590: mov      r0, r5
00478594: ldr      r3, [r4, r3]
00478598: add      r3, r3, #8
0047859c: str      r3, [r5]
004785a0: bl       #0x478520 ; _ZN11AnchorGroup10_AddAnchorEPS_
004785a4: mov      r0, r5
004785a8: pop      {r4, r5, r6, pc}
004785ac: subseq   ip, r1, r4, lsl #10
004785b0: andeq    r4, r0, r0, lsl #5
# _ZN10GameObject11SetRotationERK7Point3DIfE 3938a0 88
003938a0: ldr      r3, [r1]
003938a4: push     {r4, lr}
003938a8: str      r3, [r0, #0x16c]
003938ac: ldr      r3, [r1, #4]
003938b0: ldr      r2, [r0, #0x2d8]
003938b4: mov      r4, r0
003938b8: str      r3, [r0, #0x170]
003938bc: ldr      r3, [r1, #8]
003938c0: cmp      r2, #0
003938c4: str      r3, [r0, #0x174]
003938c8: ldr      r3, [r1, #8]
003938cc: str      r3, [r0, #0x178]
003938d0: beq      #0x3938f4
003938d4: ldr      r3, [r0]
003938d8: mov      lr, pc
003938dc: ldr      pc, [r3, #0x70]
003938e0: cmp      r0, #0
003938e4: beq      #0x3938f4
003938e8: ldr      r0, [r4, #0x2d8]
003938ec: pop      {r4, lr}
003938f0: b        #0x472948
003938f4: pop      {r4, pc}
# _ZN12VisualObjectC1EP10GameObjectRKSsS3_ 472a0c 592
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
00472af4: bl       #0x50a564 ; _ZN12AssetManager15GetAssetManagerEv
00472af8: ldr      ip, [r8, #0x10]
00472afc: ldr      r2, [r8, #0x14]
00472b00: ldr      r1, [sl, #0x14]
00472b04: mov      r3, r5
00472b08: cmp      ip, r2
00472b0c: moveq    r2, r5
00472b10: mvn      ip, #0x80000000
00472b14: str      ip, [sp]
00472b18: bl       #0x50a504 ; _ZN12AssetManager13loadSceneNodeEPKcS1_bi
00472b1c: cmp      r0, r5
00472b20: str      r0, [r4, #8]
00472b24: beq      #0x472c40
00472b28: mov      r1, r7
00472b2c: mov      r0, r4
00472b30: bl       #0x47295c ; _ZN12VisualObject9SetParentEP10GameObject
00472b34: ldr      r0, [r4, #8]
00472b38: bl       #0x35c854 ; _ZN13RootSceneNode18RefreshBoundingBoxEv
00472b3c: mov      r0, r4
00472b40: bl       #0x4718f0 ; _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
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
00472b74: bl       #0x350ee0 ; _ZN12SceneManager13ForceRegisterEv
00472b78: ldr      r3, [r5, #0x10]
00472b7c: ldr      r2, [pc, #0xd4]
00472b80: ldr      r1, [r4, #8]
00472b84: ldr      r0, [r3, #0x1c]
00472b88: add      r2, pc, r2
00472b8c: mov      r3, #1
00472b90: bl       #0x35a0e4 ; _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
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
00472c0c: bl       #0x47211c ; _ZN12VisualObject11CalcMeshBoxEv
00472c10: mov      r0, r4
00472c14: bl       #0x470a54 ; _ZN12VisualObject12ApplyMeshBoxEv
00472c18: mov      r1, #0
00472c1c: mov      r0, #8
00472c20: bl       #0x310570 ; _Znwj15MemoryHintState
00472c24: ldr      r1, [r4, #8]
00472c28: mov      r5, r0
00472c2c: mov      r2, #0
00472c30: bl       #0x474d30 ; _ZN14AnimControllerC1EP13RootSceneNodeb
00472c34: mov      r0, r4
00472c38: mov      r1, r5
00472c3c: bl       #0x470a84 ; _ZN12VisualObject17SetAnimControllerEP14AnimController
00472c40: mov      r0, r4
00472c44: add      sp, sp, #0xc
00472c48: pop      {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq   r2, r2, r4, ror r0
00472c50: muleq    r0, r4, lr
00472c54: strdeq   r3, r4, [r0], -r4
00472c58: subeq    sl, r5, r0, lsr #22
# _ZN10GameObjectC2EN10ObjectBase6GO_IDSE 38c398 616
0038c398: push     {r4, r5, r6, r7, lr}
0038c39c: ldr      r6, [pc, #0x254]
0038c3a0: sub      sp, sp, #0xc
0038c3a4: mov      r4, r0
0038c3a8: bl       #0x33f310 ; _ZN10ObjectBaseC2ENS_6GO_IDSE
0038c3ac: ldr      r2, [pc, #0x248]
0038c3b0: add      r6, pc, r6
0038c3b4: mov      r3, #0
0038c3b8: ldr      r2, [r6, r2]
0038c3bc: mov      r5, #0
0038c3c0: str      r3, [r4, #0x120]
0038c3c4: add      r1, r2, #0xe4
0038c3c8: add      r0, r2, #8
0038c3cc: add      r2, r2, #0xd8
0038c3d0: str      r2, [r4, #4]
0038c3d4: str      r1, [r4, #0x24]
0038c3d8: str      r0, [r4]
0038c3dc: str      r3, [r4, #0x124]
0038c3e0: str      r3, [r4, #0x128]
0038c3e4: str      r3, [r4, #0x12c]
0038c3e8: str      r3, [r4, #0x130]
0038c3ec: str      r3, [r4, #0x134]
0038c3f0: str      r3, [r4, #0x138]
0038c3f4: str      r3, [r4, #0x13c]
0038c3f8: str      r3, [r4, #0x140]
0038c3fc: str      r3, [r4, #0x144]
0038c400: str      r3, [r4, #0x148]
0038c404: str      r3, [r4, #0x14c]
0038c408: str      r3, [r4, #0x150]
0038c40c: str      r3, [r4, #0x154]
0038c410: str      r3, [r4, #0x158]
0038c414: str      r3, [r4, #0x160]
0038c418: str      r3, [r4, #0x164]
0038c41c: str      r3, [r4, #0x168]
0038c420: str      r3, [r4, #0x16c]
0038c424: str      r3, [r4, #0x170]
0038c428: str      r3, [r4, #0x174]
0038c42c: str      r3, [r4, #0x178]
0038c430: str      r3, [r4, #0x184]
0038c434: str      r3, [r4, #0x188]
0038c438: str      r3, [r4, #0x18c]
0038c43c: str      r3, [r4, #0x190]
0038c440: str      r3, [r4, #0x194]
0038c444: strb     r5, [r4, #0x15c]
0038c448: str      r5, [r4, #0x180]
0038c44c: add      r0, r4, #0x1c8
0038c450: str      r3, [r4, #0x198]
0038c454: str      r3, [r4, #0x1c0]
0038c458: str      r3, [r4, #0x19c]
0038c45c: str      r3, [r4, #0x1a0]
0038c460: str      r3, [r4, #0x1a4]
0038c464: str      r3, [r4, #0x1a8]
0038c468: str      r3, [r4, #0x1ac]
0038c46c: str      r3, [r4, #0x1b0]
0038c470: strb     r5, [r4, #0x1b4]
0038c474: strb     r5, [r4, #0x1b5]
0038c478: str      r3, [r4, #0x1b8]
0038c47c: str      r3, [r4, #0x1bc]
0038c480: strb     r5, [r4, #0x1c4]
0038c484: bl       #0x524644 ; _ZN8PFObjectC1Ev
0038c488: add      r3, r4, #0x278
0038c48c: mvn      r7, #0
0038c490: mov      r2, #0x64
0038c494: str      r2, [r4, #0x274]
0038c498: mov      r0, r3
0038c49c: str      r3, [r4, #0x288]
0038c4a0: str      r3, [r4, #0x28c]
0038c4a4: str      r5, [r4, #0x26c]
0038c4a8: str      r7, [r4, #0x270]
0038c4ac: mov      r1, #0x10
0038c4b0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c4b4: ldr      r2, [r4, #0x288]
0038c4b8: add      r3, r4, #0x290
0038c4bc: mov      r0, r3
0038c4c0: strb     r5, [r2]
0038c4c4: mov      r1, #0x10
0038c4c8: str      r3, [r4, #0x2a0]
0038c4cc: str      r3, [r4, #0x2a4]
0038c4d0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c4d4: ldr      r2, [r4, #0x2a0]
0038c4d8: add      r3, r4, #0x2a8
0038c4dc: mov      r0, r3
0038c4e0: strb     r5, [r2]
0038c4e4: mov      r1, #0x10
0038c4e8: str      r3, [r4, #0x2b8]
0038c4ec: str      r3, [r4, #0x2bc]
0038c4f0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c4f4: ldr      r2, [r4, #0x2b8]
0038c4f8: add      r3, r4, #0x2c0
0038c4fc: mov      r0, r3
0038c500: strb     r5, [r2]
0038c504: mov      r1, #0x10
0038c508: str      r3, [r4, #0x2d0]
0038c50c: str      r3, [r4, #0x2d4]
0038c510: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c514: ldr      r3, [r4, #0x2d0]
0038c518: mov      ip, #1
0038c51c: add      r6, r4, #0x304
0038c520: strb     r5, [r3]
0038c524: mov      r2, ip
0038c528: strb     ip, [r4, #0x2ee]
0038c52c: strb     ip, [r4, #0x2fb]
0038c530: str      r5, [r4, #0x2d8]
0038c534: str      r5, [r4, #0x2dc]
0038c538: str      r5, [r4, #0x2e0]
0038c53c: str      r5, [r4, #0x2e4]
0038c540: str      r5, [r4, #0x2e8]
0038c544: strb     r5, [r4, #0x2ec]
0038c548: strb     r5, [r4, #0x2ed]
0038c54c: strb     r5, [r4, #0x2ef]
0038c550: strb     r5, [r4, #0x2f0]
0038c554: str      r5, [r4, #0x2f4]
0038c558: strb     r5, [r4, #0x2f8]
0038c55c: strb     r5, [r4, #0x2f9]
0038c560: strb     r5, [r4, #0x2fa]
0038c564: strb     r5, [r4, #0x2fc]
0038c568: str      r5, [r4, #0x300]
0038c56c: mov      r3, r5
0038c570: mov      r1, r5
0038c574: mov      r0, r6
0038c578: str      ip, [sp]
0038c57c: bl       #0x4a2730 ; _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
0038c580: add      r3, r4, #0x358
0038c584: mov      r0, r3
0038c588: str      r3, [r4, #0x368]
0038c58c: str      r3, [r4, #0x36c]
0038c590: mov      r1, #0x10
0038c594: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c598: ldr      r1, [r4, #0x368]
0038c59c: mov      r2, #0xc2000000
0038c5a0: mov      r3, #0x42000000
0038c5a4: strb     r5, [r1]
0038c5a8: add      r2, r2, #0xc80000
0038c5ac: add      r3, r3, #0xc80000
0038c5b0: mov      r1, #0x370
0038c5b4: strh     r7, [r4, r1]
0038c5b8: mov      r0, r4
0038c5bc: str      r2, [r4, #0x14c]
0038c5c0: str      r3, [r4, #0x158]
0038c5c4: str      r2, [r4, #0x144]
0038c5c8: str      r2, [r4, #0x148]
0038c5cc: str      r3, [r4, #0x150]
0038c5d0: str      r3, [r4, #0x154]
0038c5d4: strb     r5, [r4, #0x373]
0038c5d8: strb     r5, [r4, #0x372]
0038c5dc: bl       #0x38aac8 ; _ZN10GameObject18UpdateAbsoluteAABBEv
0038c5e0: mov      r0, r6
0038c5e4: mov      r1, r4
0038c5e8: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
0038c5ec: mov      r0, r4
0038c5f0: add      sp, sp, #0xc
0038c5f4: pop      {r4, r5, r6, r7, pc}
0038c5f8: rsbeq    r8, r0, r0, ror #13
0038c5fc: andeq    r2, r0, r0, ror sp
# _ZNSt6vectorIP10GameObjectSaIS1_EEC1ERKS3_ 38e680 128
0038e680: push     {r4, r5, lr}
0038e684: mov      r5, r1
0038e688: ldr      r3, [r5]
0038e68c: ldr      r1, [r1, #4]
0038e690: sub      sp, sp, #0xc
0038e694: mov      r4, r0
0038e698: rsb      r1, r3, r1
0038e69c: mov      ip, #0
0038e6a0: asr      r1, r1, #2
0038e6a4: add      r2, sp, #8
0038e6a8: str      r1, [r2, #-4]!
0038e6ac: str      ip, [r4]
0038e6b0: str      ip, [r4, #4]
0038e6b4: str      ip, [r0, #8]!
0038e6b8: bl       #0x38e610 ; _ZNSaIP10GameObjectE11_M_allocateEjRj
0038e6bc: ldr      r2, [sp, #4]
0038e6c0: str      r0, [r4]
0038e6c4: str      r0, [r4, #4]
0038e6c8: add      r2, r0, r2, lsl #2
0038e6cc: str      r2, [r4, #8]
0038e6d0: ldm      r5, {r1, r2}
0038e6d4: mov      r3, r0
0038e6d8: cmp      r1, r2
0038e6dc: beq      #0x38e6f0
0038e6e0: rsb      r5, r1, r2
0038e6e4: mov      r2, r5
0038e6e8: bl       #0x30e868 ; 
0038e6ec: add      r3, r0, r5
0038e6f0: str      r3, [r4, #4]
0038e6f4: mov      r0, r4
0038e6f8: add      sp, sp, #0xc
0038e6fc: pop      {r4, r5, pc}
# _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti 46f2f0 904
0046f2f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046f2f4: ldr      r5, [pc, #0x360]
0046f2f8: ldr      r4, [pc, #0x360]
0046f2fc: sub      sp, sp, #0xb4
0046f300: add      r5, pc, r5
0046f304: str      r4, [sp, #0x10]
0046f308: ldr      lr, [r5, r4]
0046f30c: ldr      ip, [pc, #0x350]
0046f310: ldr      r4, [pc, #0x350]
0046f314: ldrb     sb, [sp, #0xd8]
0046f318: ldr      ip, [r5, ip]
0046f31c: ldr      r4, [r5, r4]
0046f320: ldr      lr, [lr]
0046f324: mov      r7, #0
0046f328: str      r4, [sp, #0xc]
0046f32c: add      ip, ip, #8
0046f330: mov      r4, r0
0046f334: mov      sl, #0
0046f338: str      r1, [r0, #4]
0046f33c: str      ip, [r0]
0046f340: str      r2, [r4, #8]
0046f344: str      sl, [r0, #0xc]
0046f348: strb     sb, [r0, #0x10]
0046f34c: str      r7, [r0, #0x14]
0046f350: str      r7, [r0, #0x18]
0046f354: str      r7, [r0, #0x1c]
0046f358: strb     r7, [r0, #0x26]
0046f35c: strb     r7, [r0, #0x27]
0046f360: ldrb     ip, [sp, #0xdc]
0046f364: mov      r6, r2
0046f368: str      lr, [sp, #0xac]
0046f36c: ldrh     r2, [sp, #0xe8]
0046f370: ldrb     lr, [sp, #0xe0]
0046f374: str      r3, [sp, #0x1c]
0046f378: ldrh     r3, [sp, #0xec]
0046f37c: ldr      r0, [sp, #0xc]
0046f380: str      ip, [sp, #0x20]
0046f384: str      lr, [sp, #0x24]
0046f388: str      r3, [sp, #0x18]
0046f38c: ldrsh    r8, [sp, #0xe4]
0046f390: str      r2, [sp, #0x14]
0046f394: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0046f398: ldr      r1, [pc, #0x2cc]
0046f39c: add      fp, sp, #0x94
0046f3a0: add      r2, sp, #0x90
0046f3a4: add      r1, pc, r1
0046f3a8: mov      r0, fp
0046f3ac: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0046f3b0: mov      r1, fp
0046f3b4: ldr      r0, [sp, #0xc]
0046f3b8: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0046f3bc: mov      r3, r0
0046f3c0: mov      r0, fp
0046f3c4: str      r3, [sp, #8]
0046f3c8: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0046f3cc: ldr      r3, [sp, #8]
0046f3d0: mvn      r2, #0x298
0046f3d4: sub      r2, r2, #1
0046f3d8: cmp      r3, r7
0046f3dc: movne    r8, r2
0046f3e0: cmp      r6, r7
0046f3e4: beq      #0x46f558
0046f3e8: cmp      sb, r7
0046f3ec: bne      #0x46f57c
0046f3f0: ldr      r3, [pc, #0x278]
0046f3f4: mov      r2, #1
0046f3f8: ldr      r1, [r6, #0x12c]
0046f3fc: ldr      r3, [r5, r3]
0046f400: ldr      r0, [r6, #0x138]
0046f404: str      r2, [sp, #0x30]
0046f408: add      ip, r3, #8
0046f40c: movw     r3, #0xcccd
0046f410: movt     r3, #0x3e4c
0046f414: strh     r2, [sp, #0x46]
0046f418: mvn      r2, #0
0046f41c: str      ip, [sp, #0x2c]
0046f420: strh     r2, [sp, #0x48]
0046f424: str      r3, [sp, #0x38]
0046f428: str      sl, [sp, #0x40]
0046f42c: str      sb, [sp, #0x8c]
0046f430: str      sb, [sp, #0x34]
0046f434: str      sl, [sp, #0x3c]
0046f438: strh     sb, [sp, #0x4a]
0046f43c: strb     sb, [sp, #0x44]
0046f440: bl       #0x30e3ac ; 
0046f444: movw     r1, #0xd70a
0046f448: movt     r1, #0x3c23
0046f44c: bl       #0x30ed6c ; 
0046f450: ldr      r1, [r6, #0x130]
0046f454: mov      sb, r0
0046f458: ldr      r0, [r6, #0x13c]
0046f45c: bl       #0x30e3ac ; 
0046f460: movw     r1, #0xd70a
0046f464: movt     r1, #0x3c23
0046f468: bl       #0x30ed6c ; 
0046f46c: movw     r1, #0xd70a
0046f470: mov      r7, r0
0046f474: movt     r1, #0x3c23
0046f478: ldr      r0, [r6, #0x160]
0046f47c: bl       #0x30ed6c ; 
0046f480: movw     r1, #0xd70a
0046f484: movt     r1, #0x3c23
0046f488: mov      sl, r0
0046f48c: ldr      r0, [r6, #0x164]
0046f490: bl       #0x30ed6c ; 
0046f494: mov      r1, #0x3f000000
0046f498: mov      r6, r0
0046f49c: mov      r0, sb
0046f4a0: bl       #0x30ed6c ; 
0046f4a4: mov      r1, #0x3f000000
0046f4a8: mov      r3, r0
0046f4ac: mov      r0, r7
0046f4b0: str      r3, [sp, #8]
0046f4b4: bl       #0x30ed6c ; 
0046f4b8: ldr      r3, [sp, #8]
0046f4bc: add      fp, sp, #0x2c
0046f4c0: mov      r2, r0
0046f4c4: mov      r1, r3
0046f4c8: mov      r0, fp
0046f4cc: bl       #0x7e44b8 ; _ZN12b2PolygonDef8SetAsBoxEff
0046f4d0: mov      r1, r7
0046f4d4: mov      r0, sb
0046f4d8: bl       #0x30e70c ; 
0046f4dc: cmp      r0, #0
0046f4e0: moveq    r7, sb
0046f4e4: mov      r0, r7
0046f4e8: mov      r1, #0x3f000000
0046f4ec: bl       #0x30ed6c ; 
0046f4f0: ldr      r3, [pc, #0x17c]
0046f4f4: str      r0, [r4, #0xc]
0046f4f8: mov      ip, fp
0046f4fc: ldr      r3, [r5, r3]
0046f500: add      r3, r3, #8
0046f504: str      r3, [sp, #0x2c]
0046f508: strh     r8, [r4, #0x24]
0046f50c: ldr      lr, [sp, #0x14]
0046f510: mov      r1, ip
0046f514: mov      r3, r6
0046f518: strh     lr, [r4, #0x20]
0046f51c: ldr      r2, [sp, #0x18]
0046f520: mov      r0, r4
0046f524: strh     r2, [r4, #0x22]
0046f528: ldr      lr, [sp, #0x20]
0046f52c: strh     r8, [ip, #0x1e]
0046f530: mov      r2, sl
0046f534: strb     lr, [ip, #0x18]
0046f538: ldr      lr, [sp, #0x14]
0046f53c: strh     lr, [ip, #0x1a]
0046f540: ldr      lr, [sp, #0x18]
0046f544: strh     lr, [ip, #0x1c]
0046f548: ldr      ip, [sp, #0x1c]
0046f54c: ldr      lr, [sp, #0x24]
0046f550: stm      sp, {ip, lr}
0046f554: bl       #0x46edf0 ; _ZN14PhysicalObject5_initEP10b2ShapeDefffbb
0046f558: ldr      ip, [sp, #0x10]
0046f55c: ldr      r2, [sp, #0xac]
0046f560: mov      r0, r4
0046f564: ldr      r3, [r5, ip]
0046f568: ldr      r3, [r3]
0046f56c: cmp      r2, r3
0046f570: bne      #0x46f658
0046f574: add      sp, sp, #0xb4
0046f578: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f57c: movw     r3, #0xcccd
0046f580: ldr      r1, [r6, #0x12c]
0046f584: ldr      r0, [r6, #0x138]
0046f588: movt     r3, #0x3e4c
0046f58c: mov      ip, #1
0046f590: mvn      lr, #0
0046f594: str      r3, [sp, #0x38]
0046f598: strh     ip, [sp, #0x46]
0046f59c: strh     lr, [sp, #0x48]
0046f5a0: strb     r7, [sp, #0x44]
0046f5a4: str      r7, [sp, #0x30]
0046f5a8: str      sl, [sp, #0x50]
0046f5ac: str      r7, [sp, #0x34]
0046f5b0: str      sl, [sp, #0x3c]
0046f5b4: str      sl, [sp, #0x40]
0046f5b8: strh     r7, [sp, #0x4a]
0046f5bc: str      sl, [sp, #0x4c]
0046f5c0: bl       #0x30e3ac ; 
0046f5c4: movw     r1, #0xd70a
0046f5c8: movt     r1, #0x3c23
0046f5cc: bl       #0x30ed6c ; 
0046f5d0: ldr      r1, [r6, #0x130]
0046f5d4: mov      sb, r0
0046f5d8: ldr      r0, [r6, #0x13c]
0046f5dc: bl       #0x30e3ac ; 
0046f5e0: movw     r1, #0xd70a
0046f5e4: movt     r1, #0x3c23
0046f5e8: bl       #0x30ed6c ; 
0046f5ec: movw     r1, #0xd70a
0046f5f0: mov      r7, r0
0046f5f4: movt     r1, #0x3c23
0046f5f8: ldr      r0, [r6, #0x160]
0046f5fc: bl       #0x30ed6c ; 
0046f600: movw     r1, #0xd70a
0046f604: movt     r1, #0x3c23
0046f608: mov      sl, r0
0046f60c: ldr      r0, [r6, #0x164]
0046f610: bl       #0x30ed6c ; 
0046f614: mov      r1, r7
0046f618: mov      r6, r0
0046f61c: mov      r0, sb
0046f620: bl       #0x30e70c ; 
0046f624: cmp      r0, #0
0046f628: moveq    r7, sb
0046f62c: mov      r0, r7
0046f630: mov      r1, #0x3f000000
0046f634: bl       #0x30ed6c ; 
0046f638: ldr      r3, [pc, #0x34]
0046f63c: add      ip, sp, #0xb0
0046f640: str      r0, [r4, #0xc]
0046f644: ldr      r3, [r5, r3]
0046f648: str      r0, [sp, #0x54]
0046f64c: add      r3, r3, #8
0046f650: str      r3, [ip, #-0x84]!
0046f654: b        #0x46f508
0046f658: bl       #0x30e310 ; 
# _ZN10GameObjectC1EN10ObjectBase6GO_IDSE 38c130 616
0038c130: push     {r4, r5, r6, r7, lr}
0038c134: ldr      r6, [pc, #0x254]
0038c138: sub      sp, sp, #0xc
0038c13c: mov      r4, r0
0038c140: bl       #0x33f310 ; _ZN10ObjectBaseC2ENS_6GO_IDSE
0038c144: ldr      r2, [pc, #0x248]
0038c148: add      r6, pc, r6
0038c14c: mov      r3, #0
0038c150: ldr      r2, [r6, r2]
0038c154: mov      r5, #0
0038c158: str      r3, [r4, #0x120]
0038c15c: add      r1, r2, #0xe4
0038c160: add      r0, r2, #8
0038c164: add      r2, r2, #0xd8
0038c168: str      r2, [r4, #4]
0038c16c: str      r1, [r4, #0x24]
0038c170: str      r0, [r4]
0038c174: str      r3, [r4, #0x124]
0038c178: str      r3, [r4, #0x128]
0038c17c: str      r3, [r4, #0x12c]
0038c180: str      r3, [r4, #0x130]
0038c184: str      r3, [r4, #0x134]
0038c188: str      r3, [r4, #0x138]
0038c18c: str      r3, [r4, #0x13c]
0038c190: str      r3, [r4, #0x140]
0038c194: str      r3, [r4, #0x144]
0038c198: str      r3, [r4, #0x148]
0038c19c: str      r3, [r4, #0x14c]
0038c1a0: str      r3, [r4, #0x150]
0038c1a4: str      r3, [r4, #0x154]
0038c1a8: str      r3, [r4, #0x158]
0038c1ac: str      r3, [r4, #0x160]
0038c1b0: str      r3, [r4, #0x164]
0038c1b4: str      r3, [r4, #0x168]
0038c1b8: str      r3, [r4, #0x16c]
0038c1bc: str      r3, [r4, #0x170]
0038c1c0: str      r3, [r4, #0x174]
0038c1c4: str      r3, [r4, #0x178]
0038c1c8: str      r3, [r4, #0x184]
0038c1cc: str      r3, [r4, #0x188]
0038c1d0: str      r3, [r4, #0x18c]
0038c1d4: str      r3, [r4, #0x190]
0038c1d8: str      r3, [r4, #0x194]
0038c1dc: strb     r5, [r4, #0x15c]
0038c1e0: str      r5, [r4, #0x180]
0038c1e4: add      r0, r4, #0x1c8
0038c1e8: str      r3, [r4, #0x198]
0038c1ec: str      r3, [r4, #0x1c0]
0038c1f0: str      r3, [r4, #0x19c]
0038c1f4: str      r3, [r4, #0x1a0]
0038c1f8: str      r3, [r4, #0x1a4]
0038c1fc: str      r3, [r4, #0x1a8]
0038c200: str      r3, [r4, #0x1ac]
0038c204: str      r3, [r4, #0x1b0]
0038c208: strb     r5, [r4, #0x1b4]
0038c20c: strb     r5, [r4, #0x1b5]
0038c210: str      r3, [r4, #0x1b8]
0038c214: str      r3, [r4, #0x1bc]
0038c218: strb     r5, [r4, #0x1c4]
0038c21c: bl       #0x524644 ; _ZN8PFObjectC1Ev
0038c220: add      r3, r4, #0x278
0038c224: mvn      r7, #0
0038c228: mov      r2, #0x64
0038c22c: str      r2, [r4, #0x274]
0038c230: mov      r0, r3
0038c234: str      r3, [r4, #0x288]
0038c238: str      r3, [r4, #0x28c]
0038c23c: str      r5, [r4, #0x26c]
0038c240: str      r7, [r4, #0x270]
0038c244: mov      r1, #0x10
0038c248: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c24c: ldr      r2, [r4, #0x288]
0038c250: add      r3, r4, #0x290
0038c254: mov      r0, r3
0038c258: strb     r5, [r2]
0038c25c: mov      r1, #0x10
0038c260: str      r3, [r4, #0x2a0]
0038c264: str      r3, [r4, #0x2a4]
0038c268: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c26c: ldr      r2, [r4, #0x2a0]
0038c270: add      r3, r4, #0x2a8
0038c274: mov      r0, r3
0038c278: strb     r5, [r2]
0038c27c: mov      r1, #0x10
0038c280: str      r3, [r4, #0x2b8]
0038c284: str      r3, [r4, #0x2bc]
0038c288: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c28c: ldr      r2, [r4, #0x2b8]
0038c290: add      r3, r4, #0x2c0
0038c294: mov      r0, r3
0038c298: strb     r5, [r2]
0038c29c: mov      r1, #0x10
0038c2a0: str      r3, [r4, #0x2d0]
0038c2a4: str      r3, [r4, #0x2d4]
0038c2a8: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c2ac: ldr      r3, [r4, #0x2d0]
0038c2b0: mov      ip, #1
0038c2b4: add      r6, r4, #0x304
0038c2b8: strb     r5, [r3]
0038c2bc: mov      r2, ip
0038c2c0: strb     ip, [r4, #0x2ee]
0038c2c4: strb     ip, [r4, #0x2fb]
0038c2c8: str      r5, [r4, #0x2d8]
0038c2cc: str      r5, [r4, #0x2dc]
0038c2d0: str      r5, [r4, #0x2e0]
0038c2d4: str      r5, [r4, #0x2e4]
0038c2d8: str      r5, [r4, #0x2e8]
0038c2dc: strb     r5, [r4, #0x2ec]
0038c2e0: strb     r5, [r4, #0x2ed]
0038c2e4: strb     r5, [r4, #0x2ef]
0038c2e8: strb     r5, [r4, #0x2f0]
0038c2ec: str      r5, [r4, #0x2f4]
0038c2f0: strb     r5, [r4, #0x2f8]
0038c2f4: strb     r5, [r4, #0x2f9]
0038c2f8: strb     r5, [r4, #0x2fa]
0038c2fc: strb     r5, [r4, #0x2fc]
0038c300: str      r5, [r4, #0x300]
0038c304: mov      r3, r5
0038c308: mov      r1, r5
0038c30c: mov      r0, r6
0038c310: str      ip, [sp]
0038c314: bl       #0x4a2730 ; _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
0038c318: add      r3, r4, #0x358
0038c31c: mov      r0, r3
0038c320: str      r3, [r4, #0x368]
0038c324: str      r3, [r4, #0x36c]
0038c328: mov      r1, #0x10
0038c32c: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c330: ldr      r1, [r4, #0x368]
0038c334: mov      r2, #0xc2000000
0038c338: mov      r3, #0x42000000
0038c33c: strb     r5, [r1]
0038c340: add      r2, r2, #0xc80000
0038c344: add      r3, r3, #0xc80000
0038c348: mov      r1, #0x370
0038c34c: strh     r7, [r4, r1]
0038c350: mov      r0, r4
0038c354: str      r2, [r4, #0x14c]
0038c358: str      r3, [r4, #0x158]
0038c35c: str      r2, [r4, #0x144]
0038c360: str      r2, [r4, #0x148]
0038c364: str      r3, [r4, #0x150]
0038c368: str      r3, [r4, #0x154]
0038c36c: strb     r5, [r4, #0x373]
0038c370: strb     r5, [r4, #0x372]
0038c374: bl       #0x38aac8 ; _ZN10GameObject18UpdateAbsoluteAABBEv
0038c378: mov      r0, r6
0038c37c: mov      r1, r4
0038c380: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
0038c384: mov      r0, r4
0038c388: add      sp, sp, #0xc
0038c38c: pop      {r4, r5, r6, r7, pc}
0038c390: rsbeq    r8, r0, r8, asr #18
0038c394: andeq    r2, r0, r0, ror sp
# _ZN10AnchorBaseC1EP10GameObjectNS_10AnchorTypeE 4770d0 208
004770d0: ldr      r3, [pc, #0xac]
004770d4: push     {r4, lr}
004770d8: ldr      lr, [pc, #0xa8]
004770dc: add      r3, pc, r3
004770e0: mov      ip, #0
004770e4: ldr      lr, [r3, lr]
004770e8: str      r2, [r0, #4]
004770ec: cmp      r1, #0
004770f0: add      lr, lr, #8
004770f4: mov      r2, #1
004770f8: sub      sp, sp, #8
004770fc: mov      r4, r0
00477100: str      lr, [r0]
00477104: str      ip, [r0, #0x14]
00477108: strb     r2, [r0, #0x18]
0047710c: str      r1, [r0, #8]
00477110: str      ip, [r0, #0xc]
00477114: str      ip, [r0, #0x10]
00477118: beq      #0x477130
0047711c: mov      r0, r4
00477120: bl       #0x477074 ; _ZN10AnchorBase5ResetEv
00477124: mov      r0, r4
00477128: add      sp, sp, #8
0047712c: pop      {r4, pc}
00477130: ldr      r2, [pc, #0x54]
00477134: ldr      r2, [r3, r2]
00477138: ldr      r2, [r2]
0047713c: cmp      r2, #2
00477140: streq    r1, [r1]
00477144: beq      #0x47711c
00477148: cmp      r2, #1
0047714c: bne      #0x47711c
00477150: ldr      r0, [pc, #0x38]
00477154: ldr      r1, [pc, #0x38]
00477158: ldr      r2, [pc, #0x38]
0047715c: ldr      r0, [r3, r0]
00477160: ldr      r3, [pc, #0x34]
00477164: mov      ip, #0x18
00477168: add      r1, pc, r1
0047716c: add      r2, pc, r2
00477170: add      r3, pc, r3
00477174: add      r0, r0, #0xa8
00477178: str      ip, [sp]
0047717c: bl       #0x30e004 ; 
00477180: b        #0x47711c
00477184: ldrheq   sp, [r1], #-0x94
00477188: strheq   r1, [r0], -ip
0047718c: andeq    r3, r0, r0, asr #19
00477190: andeq    r1, r0, r0, asr #19
00477194: subeq    r7, r4, r0, ror r2
00477198: subeq    fp, r4, ip, lsr #4
0047719c: subeq    r6, r5, r8, ror r7
# _ZN14ObjectSearcher10TargetListC2EP10GameObjectiii 4a348c 296
004a348c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a3490: ldr      r6, [pc, #0x10c]
004a3494: ldr      r7, [pc, #0x10c]
004a3498: mov      r5, #0
004a349c: add      r6, pc, r6
004a34a0: str      r5, [r0]
004a34a4: str      r5, [r0, #4]
004a34a8: str      r5, [r0, #8]
004a34ac: str      r5, [r0, #0xc]
004a34b0: str      r5, [r0, #0x10]
004a34b4: str      r5, [r0, #0x14]
004a34b8: str      r5, [r0, #0x18]
004a34bc: str      r5, [r0, #0x1c]
004a34c0: str      r5, [r0, #0x20]
004a34c4: str      r5, [r0, #0x24]
004a34c8: mov      r4, r0
004a34cc: mov      r8, r2
004a34d0: mov      sl, r3
004a34d4: mov      fp, r1
004a34d8: ldr      sb, [sp, #0x28]
004a34dc: bl       #0x4a2240 ; 
004a34e0: ldr      r2, [r6, r7]
004a34e4: mov      r3, r4
004a34e8: str      r8, [r4, #0x34]
004a34ec: str      r2, [r4, #0x28]
004a34f0: str      sl, [r4, #0x38]
004a34f4: str      r5, [r4, #0x2c]
004a34f8: str      r5, [r4, #0x30]
004a34fc: str      r5, [r4, #0x40]
004a3500: strb     r5, [r3, #0x3c]!
004a3504: ldr      r1, [r4, #0x10]
004a3508: ldr      r2, [r4]
004a350c: str      r3, [r4, #0x48]
004a3510: str      r5, [r4, #0x4c]
004a3514: cmp      r1, r2
004a3518: str      r3, [r4, #0x44]
004a351c: beq      #0x4a3538
004a3520: mov      r0, r4
004a3524: bl       #0x38fb18 ; _ZNSt14priority_queueIN14ObjectSearcher10TargetInfoESt5dequeIS1_SaIS1_EENS0_12TargetSorterEE3popEv
004a3528: ldr      r2, [r4, #0x10]
004a352c: ldr      r3, [r4]
004a3530: cmp      r2, r3
004a3534: bne      #0x4a3520
004a3538: cmp      sb, #1
004a353c: beq      #0x4a3564
004a3540: cmp      sb, #2
004a3544: beq      #0x4a3584
004a3548: ldr      r3, [r6, r7]
004a354c: mov      r0, r4
004a3550: mov      r1, fp
004a3554: str      r3, [r4, #0x28]
004a3558: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a355c: mov      r0, r4
004a3560: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a3564: ldr      r3, [pc, #0x40]
004a3568: mov      r0, r4
004a356c: mov      r1, fp
004a3570: ldr      r3, [r6, r3]
004a3574: str      r3, [r4, #0x28]
004a3578: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a357c: mov      r0, r4
004a3580: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a3584: ldr      r3, [pc, #0x24]
004a3588: mov      r0, r4
004a358c: mov      r1, fp
004a3590: ldr      r3, [r6, r3]
004a3594: str      r3, [r4, #0x28]
004a3598: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a359c: mov      r0, r4
004a35a0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a35a4: strdeq   r1, r2, [pc], #-0x54
004a35a8: andeq    r2, r0, r4, ror ip
004a35ac: andeq    r4, r0, r4, asr #21
004a35b0: ldrdeq   r1, r2, [r0], -r4
# _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii 4a2730 296
004a2730: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a2734: ldr      r6, [pc, #0x10c]
004a2738: ldr      r7, [pc, #0x10c]
004a273c: mov      r5, #0
004a2740: add      r6, pc, r6
004a2744: str      r5, [r0]
004a2748: str      r5, [r0, #4]
004a274c: str      r5, [r0, #8]
004a2750: str      r5, [r0, #0xc]
004a2754: str      r5, [r0, #0x10]
004a2758: str      r5, [r0, #0x14]
004a275c: str      r5, [r0, #0x18]
004a2760: str      r5, [r0, #0x1c]
004a2764: str      r5, [r0, #0x20]
004a2768: str      r5, [r0, #0x24]
004a276c: mov      r4, r0
004a2770: mov      r8, r2
004a2774: mov      sl, r3
004a2778: mov      fp, r1
004a277c: ldr      sb, [sp, #0x28]
004a2780: bl       #0x4a2240 ; 
004a2784: ldr      r2, [r6, r7]
004a2788: mov      r3, r4
004a278c: str      r8, [r4, #0x34]
004a2790: str      r2, [r4, #0x28]
004a2794: str      sl, [r4, #0x38]
004a2798: str      r5, [r4, #0x2c]
004a279c: str      r5, [r4, #0x30]
004a27a0: str      r5, [r4, #0x40]
004a27a4: strb     r5, [r3, #0x3c]!
004a27a8: ldr      r1, [r4, #0x10]
004a27ac: ldr      r2, [r4]
004a27b0: str      r3, [r4, #0x48]
004a27b4: str      r5, [r4, #0x4c]
004a27b8: cmp      r1, r2
004a27bc: str      r3, [r4, #0x44]
004a27c0: beq      #0x4a27dc
004a27c4: mov      r0, r4
004a27c8: bl       #0x38fb18 ; _ZNSt14priority_queueIN14ObjectSearcher10TargetInfoESt5dequeIS1_SaIS1_EENS0_12TargetSorterEE3popEv
004a27cc: ldr      r2, [r4, #0x10]
004a27d0: ldr      r3, [r4]
004a27d4: cmp      r2, r3
004a27d8: bne      #0x4a27c4
004a27dc: cmp      sb, #1
004a27e0: beq      #0x4a2808
004a27e4: cmp      sb, #2
004a27e8: beq      #0x4a2828
004a27ec: ldr      r3, [r6, r7]
004a27f0: mov      r0, r4
004a27f4: mov      r1, fp
004a27f8: str      r3, [r4, #0x28]
004a27fc: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a2800: mov      r0, r4
004a2804: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2808: ldr      r3, [pc, #0x40]
004a280c: mov      r0, r4
004a2810: mov      r1, fp
004a2814: ldr      r3, [r6, r3]
004a2818: str      r3, [r4, #0x28]
004a281c: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a2820: mov      r0, r4
004a2824: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2828: ldr      r3, [pc, #0x24]
004a282c: mov      r0, r4
004a2830: mov      r1, fp
004a2834: ldr      r3, [r6, r3]
004a2838: str      r3, [r4, #0x28]
004a283c: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a2840: mov      r0, r4
004a2844: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2848: subeq    r2, pc, r0, asr r3
004a284c: andeq    r2, r0, r4, ror ip
004a2850: andeq    r4, r0, r4, asr #21
004a2854: ldrdeq   r1, r2, [r0], -r4
# _ZN11AnchorGroupC2EP10GameObjectN10AnchorBase10AnchorTypeE 4785b4 60
004785b4: push     {r4, r5, r6, lr}
004785b8: ldr      r4, [pc, #0x28]
004785bc: mov      r5, r0
004785c0: bl       #0x4771a0 ; _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE
004785c4: ldr      r3, [pc, #0x20]
004785c8: add      r4, pc, r4
004785cc: mov      r0, r5
004785d0: ldr      r3, [r4, r3]
004785d4: add      r3, r3, #8
004785d8: str      r3, [r5]
004785dc: bl       #0x478520 ; _ZN11AnchorGroup10_AddAnchorEPS_
004785e0: mov      r0, r5
004785e4: pop      {r4, r5, r6, pc}
004785e8: subseq   ip, r1, r8, asr #9
004785ec: andeq    r4, r0, r0, lsl #5