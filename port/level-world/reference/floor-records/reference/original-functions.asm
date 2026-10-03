
# _Z17CopyMeshSceneNodePN6glitch5scene14IMeshSceneNodeE
0050f89c: push     {r4, r5, r6, r7, r8, lr}
0050f8a0: sub      sp, sp, #0x18
0050f8a4: mov      r4, r0
0050f8a8: add      r6, sp, #0x10
0050f8ac: mov      r0, r6
0050f8b0: mov      r1, r4
0050f8b4: ldr      r3, [r4]
0050f8b8: ldr      r5, [pc, #0x110]
0050f8bc: mov      lr, pc
0050f8c0: ldr      pc, [r3, #0xf8]
0050f8c4: ldr      r3, [pc, #0x108]
0050f8c8: add      r5, pc, r5
0050f8cc: add      r0, sp, #0x14
0050f8d0: ldr      r3, [r5, r3]
0050f8d4: mov      ip, #0
0050f8d8: mov      r1, r6
0050f8dc: ldr      r2, [r3, #0x10]
0050f8e0: mvn      r3, #0
0050f8e4: ldr      r2, [r2, #0x10]
0050f8e8: str      ip, [sp]
0050f8ec: bl       #0x59b2a4
0050f8f0: ldr      r0, [sp, #0x10]
0050f8f4: cmp      r0, #0
0050f8f8: beq      #0x50f900
0050f8fc: bl       #0x31d584
0050f900: ldr      r3, [sp, #0x14]
0050f904: mov      r0, r4
0050f908: cmp      r3, #0
0050f90c: str      r3, [sp, #0xc]
0050f910: ldrne    r2, [r3, #4]
0050f914: addne    r2, r2, #1
0050f918: strne    r2, [r3, #4]
0050f91c: ldr      r3, [r4]
0050f920: mov      lr, pc
0050f924: ldr      pc, [r3, #0xa0]
0050f928: ldr      r3, [r4]
0050f92c: mov      r8, r0
0050f930: mov      r0, r4
0050f934: mov      lr, pc
0050f938: ldr      pc, [r3, #0x98]
0050f93c: ldr      r3, [r4]
0050f940: mov      r7, r0
0050f944: mov      r0, r4
0050f948: mov      lr, pc
0050f94c: ldr      pc, [r3, #0x90]
0050f950: mov      r1, #0
0050f954: mov      r6, r0
0050f958: mov      r0, #0x140
0050f95c: bl       #0x5341ac
0050f960: mov      r3, r8
0050f964: add      r1, sp, #0xc
0050f968: mvn      r2, #0
0050f96c: mov      r5, r0
0050f970: str      r7, [sp]
0050f974: str      r6, [sp, #4]
0050f978: bl       #0x585118
0050f97c: ldr      r0, [sp, #0xc]
0050f980: cmp      r0, #0
0050f984: beq      #0x50f98c
0050f988: bl       #0x31d584
0050f98c: ldr      r3, [r5]
0050f990: mov      r0, r4
0050f994: ldr      r4, [r3, #0x28]
0050f998: bl       #0x597290
0050f99c: ldr      r3, [r0]
0050f9a0: mov      lr, pc
0050f9a4: ldr      pc, [r3, #0x24]
0050f9a8: mov      r1, r0
0050f9ac: mov      r0, r5
0050f9b0: blx      r4
0050f9b4: ldr      r0, [sp, #0x14]
0050f9b8: cmp      r0, #0
0050f9bc: beq      #0x50f9c4
0050f9c0: bl       #0x31d584
0050f9c4: mov      r0, r5
0050f9c8: add      sp, sp, #0x18
0050f9cc: pop      {r4, r5, r6, r7, r8, pc}
0050f9d0: subeq    r5, r8, r8, asr #3
0050f9d4: strdeq   r3, r4, [r0], -r4

# _ZNK6glitch5scene10ISceneNode8getScaleEv
005970bc: add      r0, r0, #0xc8
005970c0: bx       lr

# _ZN17BaseMeshSceneNodeIN6glitch7collada14CMeshSceneNodeEEC2ERKN5boost13intrusive_ptrINS1_5IMeshEEE
0035b19c: push     {r4, r5, lr}
0035b1a0: mvn      r4, #0
0035b1a4: sub      sp, sp, #0x3c
0035b1a8: str      r4, [sp]
0035b1ac: add      r4, sp, #0x2c
0035b1b0: str      r4, [sp, #4]
0035b1b4: add      r4, sp, #0x10
0035b1b8: mov      ip, #0
0035b1bc: mov      lr, #0x3f800000
0035b1c0: mov      r5, r1
0035b1c4: str      r4, [sp, #8]
0035b1c8: add      r1, r1, #4
0035b1cc: add      r4, sp, #0x20
0035b1d0: mov      r3, #0
0035b1d4: str      r4, [sp, #0xc]
0035b1d8: str      ip, [sp, #0x18]
0035b1dc: mov      r4, r0
0035b1e0: str      lr, [sp, #0x28]
0035b1e4: str      ip, [sp, #0x2c]
0035b1e8: str      ip, [sp, #0x30]
0035b1ec: str      ip, [sp, #0x34]
0035b1f0: str      ip, [sp, #0x10]
0035b1f4: str      ip, [sp, #0x14]
0035b1f8: str      lr, [sp, #0x1c]
0035b1fc: str      lr, [sp, #0x20]
0035b200: str      lr, [sp, #0x24]
0035b204: bl       #0x646678
0035b208: ldr      r3, [r5]
0035b20c: mov      r0, r4
0035b210: str      r3, [r4]
0035b214: ldr      r3, [r3, #-0x1c]
0035b218: ldr      r2, [r5, #0x28]
0035b21c: str      r2, [r4, r3]
0035b220: ldr      r3, [r4]
0035b224: ldr      r2, [r5, #0x2c]
0035b228: ldr      r3, [r3, #-0xc]
0035b22c: str      r2, [r4, r3]
0035b230: mov      r3, #1
0035b234: strb     r3, [r4, #0x138]
0035b238: add      sp, sp, #0x3c
0035b23c: pop      {r4, r5, pc}

# _ZNK6glitch5scene10ISceneNode11getPositionEv
00597124: add      r0, r0, #0xac
00597128: bx       lr

# _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0059712c: ldr      r3, [r1]
00597130: ldr      r2, [r0, #0x11c]
00597134: str      r3, [r0, #0xac]
00597138: ldr      r3, [r1, #4]
0059713c: orr      r2, r2, #8
00597140: str      r3, [r0, #0xb0]
00597144: ldr      r3, [r1, #8]
00597148: str      r2, [r0, #0x11c]
0059714c: str      r3, [r0, #0xb4]
00597150: bx       lr

# _ZN7PFFloor12_LoadNavMeshEPN6glitch5scene14IMeshSceneNodeE
00520b40: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00520b44: mov      r4, r0
00520b48: sub      sp, sp, #0x38
00520b4c: mov      r0, r1
00520b50: mov      r5, r1
00520b54: bl       #0x597290
00520b58: ldr      r3, [r0]
00520b5c: mov      lr, pc
00520b60: ldr      pc, [r3, #0xac]
00520b64: ldr      sl, [pc, #0x400]
00520b68: add      r8, sp, #8
00520b6c: mov      r1, r0
00520b70: add      r7, sp, #0x38
00520b74: add      sl, pc, sl
00520b78: mov      r0, r8
00520b7c: bl       #0x319158
00520b80: add      sb, r8, #4
00520b84: str      sl, [r7, #-8]!
00520b88: mov      r0, sb
00520b8c: mov      r1, r7
00520b90: bl       #0x51cfb8
00520b94: ldr      r6, [pc, #0x3d4]
00520b98: cmp      sb, r0
00520b9c: add      r6, pc, r6
00520ba0: beq      #0x520bd4
00520ba4: mov      r1, r7
00520ba8: mov      r0, sb
00520bac: str      sl, [sp, #0x30]
00520bb0: bl       #0x51cfb8
00520bb4: ldr      sb, [r0, #0x3c]
00520bb8: add      sl, r4, #0x28
00520bbc: mov      r0, sb
00520bc0: bl       #0x30de54
00520bc4: mov      r1, sb
00520bc8: add      r2, sb, r0
00520bcc: mov      r0, sl
00520bd0: bl       #0x3109e0
00520bd4: ldr      sb, [r4, #0x3c]
00520bd8: ldr      r1, [pc, #0x394]
00520bdc: mov      r0, sb
00520be0: add      r1, pc, r1
00520be4: bl       #0x30ebd4
00520be8: ldr      sl, [r4, #0x24]
00520bec: ldr      r1, [pc, #0x384]
00520bf0: cmp      r0, #0
00520bf4: orrne    sl, sl, #0x1000000
00520bf8: strne    sl, [r4, #0x24]
00520bfc: add      r1, pc, r1
00520c00: mov      r0, sb
00520c04: bl       #0x30ebd4
00520c08: ldr      r1, [pc, #0x36c]
00520c0c: cmp      r0, #0
00520c10: orrne    sl, sl, #0x2000000
00520c14: strne    sl, [r4, #0x24]
00520c18: add      r1, pc, r1
00520c1c: mov      r0, sb
00520c20: bl       #0x30ebd4
00520c24: ldr      r1, [pc, #0x354]
00520c28: cmp      r0, #0
00520c2c: orrne    sl, sl, #1
00520c30: strne    sl, [r4, #0x24]
00520c34: add      r1, pc, r1
00520c38: mov      r0, sb
00520c3c: bl       #0x30ebd4
00520c40: cmp      r0, #0
00520c44: orrne    sl, sl, #2
00520c48: strne    sl, [r4, #0x24]
00520c4c: tst      sl, #0x3000000
00520c50: ldrne    r3, [r4, #0x20]
00520c54: mov      r0, r5
00520c58: orrne    r3, r3, #0x7000000
00520c5c: strne    r3, [r4, #0x20]
00520c60: bl       #0x597290
00520c64: cmp      r0, #0
00520c68: beq      #0x520f18
00520c6c: mov      r0, r5
00520c70: bl       #0x597290
00520c74: cmp      r0, #0
00520c78: beq      #0x520ca0
00520c7c: ldr      r3, [r5]
00520c80: add      r6, sp, #0x24
00520c84: mov      r0, r6
00520c88: mov      r1, r5
00520c8c: ldr      sl, [r3, #0xa4]
00520c90: bl       #0x597180
00520c94: mov      r0, r5
00520c98: mov      r1, r6
00520c9c: blx      sl
00520ca0: mov      r0, r5
00520ca4: bl       #0x50f89c
00520ca8: str      r0, [r4, #0x40]
00520cac: mov      r1, #0
00520cb0: mov      r0, r5
00520cb4: ldr      r3, [r5]
00520cb8: mov      lr, pc
00520cbc: ldr      pc, [r3, #0x48]
00520cc0: mov      r0, r5
00520cc4: ldr      r3, [r5]
00520cc8: mov      lr, pc
00520ccc: ldr      pc, [r3, #0x68]
00520cd0: ldr      r3, [r4, #0x40]
00520cd4: mov      r0, r3
00520cd8: ldr      r3, [r3]
00520cdc: mov      lr, pc
00520ce0: ldr      pc, [r3, #0xa0]
00520ce4: ldr      r1, [r0]
00520ce8: mov      r3, r0
00520cec: ldr      r2, [r4, #0x40]
00520cf0: str      r1, [r4, #0x5c]
00520cf4: ldr      r1, [r0, #4]
00520cf8: mov      r0, r2
00520cfc: str      r1, [r4, #0x60]
00520d00: ldr      r3, [r3, #8]
00520d04: str      r3, [r4, #0x64]
00520d08: ldr      r3, [r2]
00520d0c: mov      lr, pc
00520d10: ldr      pc, [r3, #0x34]
00520d14: ldr      r3, [r0]
00520d18: mov      r1, #0x44000000
00520d1c: add      r1, r1, #0x7a0000
00520d20: str      r3, [r4, #0x44]
00520d24: ldr      r3, [r0, #4]
00520d28: str      r3, [r4, #0x48]
00520d2c: ldr      r5, [r0, #8]
00520d30: str      r5, [r4, #0x4c]
00520d34: ldr      r3, [r0, #0xc]
00520d38: str      r3, [r4, #0x50]
00520d3c: ldr      r3, [r0, #0x10]
00520d40: str      r3, [r4, #0x54]
00520d44: ldr      r0, [r0, #0x14]
00520d48: bl       #0x30eba4
00520d4c: mov      r1, #0x44000000
00520d50: str      r0, [r4, #0x58]
00520d54: add      r1, r1, #0x7a0000
00520d58: mov      r0, r5
00520d5c: bl       #0x30e3ac
00520d60: ldr      r3, [r4, #0x40]
00520d64: str      r0, [r4, #0x4c]
00520d68: add      r0, sp, #0x34
00520d6c: mov      r1, r3
00520d70: ldr      r3, [r3]
00520d74: mov      lr, pc
00520d78: ldr      pc, [r3, #0xf8]
00520d7c: mov      r1, #0
00520d80: mov      r0, #0xb8
00520d84: ldr      r5, [sp, #0x34]
00520d88: bl       #0x5341ac
00520d8c: ldr      r2, [r4, #0x40]
00520d90: mov      ip, #1
00520d94: mov      r1, r5
00520d98: mov      r3, #0xf
00520d9c: mov      r6, r0
00520da0: str      ip, [sp]
00520da4: bl       #0x588654
00520da8: ldr      r0, [sp, #0x34]
00520dac: cmp      r0, #0
00520db0: beq      #0x520db8
00520db4: bl       #0x31d584
00520db8: ldr      r3, [r4, #0x40]
00520dbc: mov      r1, r6
00520dc0: mov      r0, r3
00520dc4: ldr      r3, [r3]
00520dc8: mov      lr, pc
00520dcc: ldr      pc, [r3, #0xb4]
00520dd0: mov      r0, r6
00520dd4: bl       #0x31d584
00520dd8: ldr      r3, [r6]
00520ddc: mov      r0, r6
00520de0: mov      lr, pc
00520de4: ldr      pc, [r3, #0xc]
00520de8: cmp      r0, #0
00520dec: mov      sl, r0
00520df0: str      r0, [sp, #0x30]
00520df4: movle    r5, #0
00520df8: ble      #0x520e7c
00520dfc: mov      r0, #0x24
00520e00: mov      r1, #0
00520e04: mul      r0, r0, sl
00520e08: bl       #0x31056c
00520e0c: mov      r2, #0
00520e10: mov      r5, r0
00520e14: mov      r3, r0
00520e18: mov      r1, #0
00520e1c: b        #0x520e24
00520e20: add      r3, r3, #0x24
00520e24: add      r1, r1, #1
00520e28: cmp      sl, r1
00520e2c: str      r2, [r3]
00520e30: str      r2, [r3, #4]
00520e34: str      r2, [r3, #8]
00520e38: str      r2, [r3, #0xc]
00520e3c: str      r2, [r3, #0x10]
00520e40: str      r2, [r3, #0x14]
00520e44: str      r2, [r3, #0x18]
00520e48: str      r2, [r3, #0x1c]
00520e4c: str      r2, [r3, #0x20]
00520e50: bne      #0x520e20
00520e54: mov      r1, #0
00520e58: ldr      ip, [r6]
00520e5c: mov      r0, r6
00520e60: str      r1, [sp]
00520e64: ldr      r2, [sp, #0x30]
00520e68: mov      r3, r7
00520e6c: mov      r1, r5
00520e70: mov      lr, pc
00520e74: ldr      pc, [ip, #0x10]
00520e78: ldr      sl, [sp, #0x30]
00520e7c: mov      r2, sl
00520e80: mov      r0, r4
00520e84: mov      r1, r5
00520e88: bl       #0x520588
00520e8c: ldr      r3, [sp, #0x30]
00520e90: str      r5, [r4, #0x68]
00520e94: cmp      r3, #0
00520e98: str      r3, [r4, #0x6c]
00520e9c: beq      #0x520f08
00520ea0: mov      r6, #0
00520ea4: mov      r7, r6
00520ea8: b        #0x520eb0
00520eac: ldr      r5, [r4, #0x68]
00520eb0: add      r5, r5, r6
00520eb4: ldr      r0, [r5, #8]
00520eb8: mov      r1, #0x3f800000
00520ebc: bl       #0x30eba4
00520ec0: str      r0, [r5, #8]
00520ec4: ldr      r5, [r4, #0x68]
00520ec8: mov      r1, #0x3f800000
00520ecc: add      r7, r7, #1
00520ed0: add      r5, r5, r6
00520ed4: ldr      r0, [r5, #0x14]
00520ed8: bl       #0x30eba4
00520edc: str      r0, [r5, #0x14]
00520ee0: ldr      r5, [r4, #0x68]
00520ee4: mov      r1, #0x3f800000
00520ee8: add      r5, r5, r6
00520eec: ldr      r0, [r5, #0x20]
00520ef0: bl       #0x30eba4
00520ef4: str      r0, [r5, #0x20]
00520ef8: ldr      r3, [r4, #0x6c]
00520efc: add      r6, r6, #0x24
00520f00: cmp      r3, r7
00520f04: bhi      #0x520eac
00520f08: mov      r0, r8
00520f0c: bl       #0x318178
00520f10: add      sp, sp, #0x38
00520f14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00520f18: ldr      r3, [pc, #0x64]
00520f1c: ldr      r3, [r6, r3]
00520f20: ldr      r3, [r3]
00520f24: cmp      r3, #2
00520f28: streq    r0, [r0]
00520f2c: beq      #0x520c6c
00520f30: cmp      r3, #1
00520f34: bne      #0x520c6c
00520f38: ldr      r0, [pc, #0x48]
00520f3c: ldr      r1, [pc, #0x48]
00520f40: ldr      r2, [pc, #0x48]
00520f44: ldr      r0, [r6, r0]
00520f48: ldr      r3, [pc, #0x44]
00520f4c: mov      ip, #0x8a
00520f50: add      r1, pc, r1
00520f54: add      r2, pc, r2
00520f58: add      r3, pc, r3
00520f5c: add      r0, r0, #0xa8
00520f60: str      ip, [sp]
00520f64: bl       #0x30e004
00520f68: b        #0x520c6c
00520f6c: eorseq   fp, fp, r4, asr #27
00520f70: strdeq   r3, r4, [r7], #-0xe4
00520f74: eorseq   sl, lr, r0, lsl fp
00520f78: eorseq   fp, fp, ip, asr #26
00520f7c: eorseq   fp, fp, r8, lsr sp
00520f80: eorseq   fp, fp, r4, lsr #26
00520f84: andeq    r3, r0, r0, asr #19
00520f88: andeq    r1, r0, r0, asr #19
00520f8c: eorseq   sp, sb, r8, lsl #9
00520f90: eorseq   fp, fp, ip, lsl #20
00520f94: eorseq   fp, fp, r8, ror sb

# _ZNK6glitch5scene10ISceneNode11getRotationEv
005970ec: add      r0, r0, #0xb8
005970f0: bx       lr

# _ZNK6glitch5scene10ISceneNode25getRelativeTransformationEv
00598908: push     {r4, r5, r6, lr}
0059890c: ldr      r3, [r0, #0x11c]
00598910: sub      sp, sp, #0x48
00598914: mov      r4, r0
00598918: tst      r3, #0xe
0059891c: addeq    r5, r0, #0x68
00598920: beq      #0x598958
00598924: ands     r2, r3, #6
00598928: bne      #0x598964
0059892c: ldr      ip, [r0, #0xac]
00598930: ldr      r1, [r4, #0xb4]
00598934: ldr      r0, [r0, #0xb0]
00598938: add      r5, r4, #0x68
0059893c: strb     r2, [r4, #0xa8]
00598940: str      ip, [r4, #0x98]
00598944: str      r0, [r4, #0x9c]
00598948: str      r1, [r4, #0xa0]
0059894c: bic      r3, r3, #0xe
00598950: orr      r3, r3, #0x10
00598954: str      r3, [r4, #0x11c]
00598958: mov      r0, r5
0059895c: add      sp, sp, #0x48
00598960: pop      {r4, r5, r6, pc}
00598964: add      r6, sp, #4
00598968: mov      r3, #0
0059896c: add      r5, r0, #0x68
00598970: mov      r1, r6
00598974: add      r0, r0, #0xb8
00598978: strb     r3, [sp, #0x44]
0059897c: bl       #0x5602d0
00598980: mov      r1, r6
00598984: mov      r2, #0x41
00598988: mov      r0, r5
0059898c: bl       #0x30e868
00598990: ldr      r0, [r4, #0xc8]
00598994: mov      r1, #0x3f800000
00598998: bl       #0x30df8c
0059899c: cmp      r0, #0
005989a0: beq      #0x5989b8
005989a4: ldr      r0, [r4, #0xcc]
005989a8: mov      r1, #0x3f800000
005989ac: bl       #0x30df8c
005989b0: cmp      r0, #0
005989b4: bne      #0x5989ec
005989b8: mov      r0, r5
005989bc: add      r1, r4, #0xc8
005989c0: bl       #0x597788
005989c4: ldr      r3, [r4, #0xb4]
005989c8: ldr      r1, [r4, #0xac]
005989cc: ldr      r2, [r4, #0xb0]
005989d0: mov      r0, #0
005989d4: str      r3, [r4, #0xa0]
005989d8: strb     r0, [r4, #0xa8]
005989dc: str      r1, [r4, #0x98]
005989e0: str      r2, [r4, #0x9c]
005989e4: ldr      r3, [r4, #0x11c]
005989e8: b        #0x59894c
005989ec: ldr      r0, [r4, #0xd0]
005989f0: mov      r1, #0x3f800000
005989f4: bl       #0x30df8c
005989f8: cmp      r0, #0
005989fc: bne      #0x5989c4
00598a00: b        #0x5989b8

# _ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb
00597c60: push     {r4, r5, r6, lr}
00597c64: ldr      r3, [r0, #0xec]
00597c68: mov      r4, r0
00597c6c: mov      r5, r1
00597c70: cmp      r3, #0
00597c74: beq      #0x597d18
00597c78: ldr      r2, [r3, #0x11c]
00597c7c: tst      r2, #0x20
00597c80: bne      #0x597cd0
00597c84: ldr      r2, [r0, #0x11c]
00597c88: tst      r2, #0x5e
00597c8c: bne      #0x597cd0
00597c90: cmp      r5, #0
00597c94: ldrne    r5, [r4, #0xf4]!
00597c98: bne      #0x597cc4
00597c9c: b        #0x597ccc
00597ca0: cmp      r5, #0
00597ca4: moveq    r3, r5
00597ca8: subne    r3, r5, #4
00597cac: mov      r0, r3
00597cb0: mov      r1, #1
00597cb4: ldr      r3, [r3]
00597cb8: mov      lr, pc
00597cbc: ldr      pc, [r3, #0xb8]
00597cc0: ldr      r5, [r5]
00597cc4: cmp      r4, r5
00597cc8: bne      #0x597ca0
00597ccc: pop      {r4, r5, r6, pc}
00597cd0: mov      r0, r3
00597cd4: ldr      r3, [r3]
00597cd8: mov      lr, pc
00597cdc: ldr      pc, [r3, #0x38]
00597ce0: ldr      r3, [r4]
00597ce4: mov      r6, r0
00597ce8: mov      r0, r4
00597cec: mov      lr, pc
00597cf0: ldr      pc, [r3, #0x40]
00597cf4: add      r2, r4, #0x24
00597cf8: mov      r1, r0
00597cfc: mov      r0, r6
00597d00: bl       #0x597884
00597d04: ldr      r3, [r4, #0x11c]
00597d08: orr      r3, r3, #0x120
00597d0c: bic      r3, r3, #0x50
00597d10: str      r3, [r4, #0x11c]
00597d14: b        #0x597c90
00597d18: ldr      r3, [r0, #0x11c]
00597d1c: tst      r3, #0x5e
00597d20: beq      #0x597c90
00597d24: mov      r6, r0
00597d28: ldr      r3, [r6], #0x24
00597d2c: mov      lr, pc
00597d30: ldr      pc, [r3, #0x40]
00597d34: mov      r2, #0x41
00597d38: mov      r1, r0
00597d3c: mov      r0, r6
00597d40: bl       #0x30e868
00597d44: ldr      r3, [r4, #0x11c]
00597d48: orr      r3, r3, #0x120
00597d4c: bic      r3, r3, #0x50
00597d50: str      r3, [r4, #0x11c]
00597d54: b        #0x597c90

# _ZNK6glitch5scene10ISceneNode7getNameEv
00596e2c: ldr      r0, [r0, #0x20]
00596e30: bx       lr

# _ZNK6glitch7collada10CSceneNode18getUserPropertyStrEv
0065cd80: ldr      r3, [r0, #0x154]
0065cd84: cmp      r3, #0
0065cd88: bne      #0x65cd94
0065cd8c: mov      r0, #0
0065cd90: bx       lr
0065cd94: ldr      r3, [r3, #0x48]
0065cd98: cmp      r3, #0
0065cd9c: ldrne    r0, [r3]
0065cda0: bxne     lr
0065cda4: b        #0x65cd8c

# _ZN7PFFloorC1EPKcP6PFRoomP13PFGOuterGraphP13PFGInnerGraphj
0051d1b4: push     {r4, r5, r6, r7, r8, lr}
0051d1b8: ldr      r5, [pc, #0x218]
0051d1bc: ldr      ip, [pc, #0x218]
0051d1c0: sub      sp, sp, #0x10
0051d1c4: add      r5, pc, r5
0051d1c8: ldr      ip, [r5, ip]
0051d1cc: mov      r4, r0
0051d1d0: mov      r6, r2
0051d1d4: add      ip, ip, #8
0051d1d8: add      r2, sp, #0xc
0051d1dc: str      ip, [r0], #4
0051d1e0: mov      r8, r3
0051d1e4: bl       #0x3140ec
0051d1e8: str      r6, [r4, #0x1c]
0051d1ec: ldr      r3, [sp, #0x2c]
0051d1f0: mov      r7, #0
0051d1f4: add      r0, r4, #0x28
0051d1f8: str      r3, [r4, #0x20]
0051d1fc: str      r0, [r4, #0x38]
0051d200: str      r0, [r4, #0x3c]
0051d204: mov      r1, #0x10
0051d208: str      r7, [r4, #0x24]
0051d20c: bl       #0x31167c
0051d210: ldr      r0, [r4, #0x38]
0051d214: mov      r3, #0
0051d218: mov      r1, r4
0051d21c: strb     r7, [r0]
0051d220: str      r3, [r4, #0x64]
0051d224: ldr      r0, [sp, #0x28]
0051d228: mov      r2, r4
0051d22c: str      r3, [r4, #0x44]
0051d230: str      r3, [r4, #0x48]
0051d234: str      r3, [r4, #0x4c]
0051d238: str      r3, [r4, #0x50]
0051d23c: str      r3, [r4, #0x54]
0051d240: str      r3, [r4, #0x58]
0051d244: str      r3, [r4, #0x5c]
0051d248: str      r3, [r4, #0x60]
0051d24c: str      r0, [r4, #0x74]
0051d250: str      r7, [r4, #0x40]
0051d254: str      r7, [r4, #0x68]
0051d258: str      r7, [r4, #0x6c]
0051d25c: str      r8, [r4, #0x70]
0051d260: str      r7, [r4, #0x7c]
0051d264: strb     r7, [r1, #0x78]!
0051d268: str      r1, [r4, #0x84]
0051d26c: str      r1, [r4, #0x80]
0051d270: str      r7, [r4, #0x88]
0051d274: str      r7, [r4, #0x94]
0051d278: strb     r7, [r2, #0x90]!
0051d27c: str      r2, [r4, #0x9c]
0051d280: str      r2, [r4, #0x98]
0051d284: str      r7, [r4, #0xa0]
0051d288: str      r7, [r4, #0xa8]
0051d28c: str      r7, [r4, #0xac]
0051d290: str      r7, [r4, #0xb0]
0051d294: str      r7, [r4, #0xb4]
0051d298: str      r7, [r4, #0xb8]
0051d29c: str      r7, [r4, #0xbc]
0051d2a0: str      r7, [r4, #0xc0]
0051d2a4: ldr      r3, [r4, #0x1c]
0051d2a8: str      r7, [r4, #0xc4]
0051d2ac: str      r7, [r4, #0xc8]
0051d2b0: cmp      r3, r7
0051d2b4: beq      #0x51d32c
0051d2b8: cmp      r8, #0
0051d2bc: beq      #0x51d384
0051d2c0: ldr      r3, [r4, #0x74]
0051d2c4: cmp      r3, #0
0051d2c8: beq      #0x51d2d8
0051d2cc: mov      r0, r4
0051d2d0: add      sp, sp, #0x10
0051d2d4: pop      {r4, r5, r6, r7, r8, pc}
0051d2d8: ldr      r2, [pc, #0x100]
0051d2dc: ldr      r2, [r5, r2]
0051d2e0: ldr      r2, [r2]
0051d2e4: cmp      r2, #2
0051d2e8: streq    r3, [r3]
0051d2ec: beq      #0x51d2cc
0051d2f0: cmp      r2, #1
0051d2f4: bne      #0x51d2cc
0051d2f8: ldr      r0, [pc, #0xe4]
0051d2fc: ldr      r1, [pc, #0xe4]
0051d300: ldr      r2, [pc, #0xe4]
0051d304: ldr      r0, [r5, r0]
0051d308: ldr      r3, [pc, #0xe0]
0051d30c: mov      ip, #0x24
0051d310: add      r1, pc, r1
0051d314: add      r2, pc, r2
0051d318: add      r3, pc, r3
0051d31c: add      r0, r0, #0xa8
0051d320: str      ip, [sp]
0051d324: bl       #0x30e004
0051d328: b        #0x51d2cc
0051d32c: ldr      r2, [pc, #0xac]
0051d330: ldr      r2, [r5, r2]
0051d334: ldr      r2, [r2]
0051d338: cmp      r2, #2
0051d33c: streq    r3, [r3]
0051d340: beq      #0x51d2b8
0051d344: cmp      r2, #1
0051d348: bne      #0x51d2b8
0051d34c: ldr      r0, [pc, #0x90]
0051d350: ldr      r1, [pc, #0x9c]
0051d354: ldr      r2, [pc, #0x9c]
0051d358: ldr      r0, [r5, r0]
0051d35c: ldr      r3, [pc, #0x98]
0051d360: mov      ip, #0x22
0051d364: add      r1, pc, r1
0051d368: add      r0, r0, #0xa8
0051d36c: add      r2, pc, r2
0051d370: add      r3, pc, r3
0051d374: str      ip, [sp]
0051d378: bl       #0x30e004
0051d37c: ldr      r8, [r4, #0x70]
0051d380: b        #0x51d2b8
0051d384: ldr      r3, [pc, #0x54]
0051d388: ldr      r3, [r5, r3]
0051d38c: ldr      r3, [r3]
0051d390: cmp      r3, #2
0051d394: streq    r8, [r8]
0051d398: beq      #0x51d2c0
0051d39c: cmp      r3, #1
0051d3a0: bne      #0x51d2c0
0051d3a4: ldr      r0, [pc, #0x38]
0051d3a8: ldr      r1, [pc, #0x50]
0051d3ac: ldr      r2, [pc, #0x50]
0051d3b0: ldr      r0, [r5, r0]
0051d3b4: ldr      r3, [pc, #0x4c]
0051d3b8: mov      ip, #0x23
0051d3bc: add      r1, pc, r1
0051d3c0: add      r2, pc, r2
0051d3c4: add      r3, pc, r3
0051d3c8: add      r0, r0, #0xa8
0051d3cc: str      ip, [sp]
0051d3d0: bl       #0x30e004
0051d3d4: b        #0x51d2c0
0051d3d8: subeq    r7, r7, ip, asr #17
0051d3dc: andeq    r4, r0, r0, lsr #7
0051d3e0: andeq    r3, r0, r0, asr #19
0051d3e4: andeq    r1, r0, r0, asr #19
0051d3e8: eorseq   r1, sl, r8, asr #1
0051d3ec: eorseq   pc, fp, r4, lsl r6
0051d3f0: ldrhteq  pc, [fp], -r8
0051d3f4: eorseq   r1, sl, r4, ror r0
0051d3f8: eorseq   pc, fp, ip, asr r5
0051d3fc: eorseq   pc, fp, r0, ror #10
0051d400: eorseq   r1, sl, ip, lsl r0
0051d404: eorseq   pc, fp, r8, asr r5
0051d408: eorseq   pc, fp, ip, lsl #10

# _ZNK6glitch5scene10ISceneNode9getParentEv
00597290: ldr      r0, [r0, #0xec]
00597294: bx       lr

# _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
00597180: ldr      ip, [r1, #0x54]
00597184: ldr      r2, [r1, #0x58]
00597188: ldr      r1, [r1, #0x5c]
0059718c: str      ip, [r0]
00597190: str      r2, [r0, #4]
00597194: str      r1, [r0, #8]
00597198: bx       lr

# _ZN6glitch5scene14CMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS8_10quaternionESC_
00585118: push     {r4, r5, r6, r7, lr}
0058511c: ldr      r6, [pc, #0xe8]
00585120: ldr      lr, [pc, #0xe8]
00585124: ldr      ip, [pc, #0xe8]
00585128: add      r6, pc, r6
0058512c: ldr      r5, [r6, lr]
00585130: ldr      ip, [r6, ip]
00585134: mov      r7, #1
00585138: ldr      lr, [r5, #0x24]
0058513c: add      ip, ip, #8
00585140: str      r7, [r0, #0x13c]
00585144: str      lr, [r0]
00585148: str      ip, [r0, #0x138]
0058514c: ldr      ip, [lr, #-0xc]
00585150: ldr      lr, [r5, #0x28]
00585154: sub      sp, sp, #0xc
00585158: mov      r7, r1
0058515c: str      lr, [r0, ip]
00585160: ldr      ip, [sp, #0x20]
00585164: add      r1, r5, #8
00585168: mov      r4, r0
0058516c: str      ip, [sp]
00585170: ldr      ip, [sp, #0x24]
00585174: str      ip, [sp, #4]
00585178: bl       #0x5990c0
0058517c: ldr      r2, [r5, #4]
00585180: ldr      r1, [r5, #0x14]
00585184: ldr      r3, [pc, #0x8c]
00585188: str      r2, [r4]
0058518c: ldr      r2, [r2, #-0x1c]
00585190: ldr      r3, [r6, r3]
00585194: ldr      ip, [r5, #0x18]
00585198: str      r1, [r4, r2]
0058519c: ldr      r0, [r4]
005851a0: mov      r2, #0
005851a4: add      r1, r3, #0x128
005851a8: ldr      r0, [r0, #-0xc]
005851ac: add      r3, r3, #0x1c
005851b0: str      ip, [r4, r0]
005851b4: str      r2, [r4, #0x130]
005851b8: str      r3, [r4]
005851bc: str      r1, [r4, #0x138]
005851c0: str      r2, [r4, #0x134]
005851c4: ldr      r3, [r7]
005851c8: cmp      r3, r2
005851cc: streq    r3, [r4, #0x130]
005851d0: beq      #0x5851f4
005851d4: ldr      r2, [r3, #4]
005851d8: add      r2, r2, #1
005851dc: str      r2, [r3, #4]
005851e0: ldr      r0, [r4, #0x130]
005851e4: str      r3, [r4, #0x130]
005851e8: cmp      r0, #0
005851ec: beq      #0x5851f4
005851f0: bl       #0x31d584
005851f4: mov      r0, r4
005851f8: mov      r1, #2
005851fc: bl       #0x59719c
00585200: mov      r0, r4
00585204: add      sp, sp, #0xc
00585208: pop      {r4, r5, r6, r7, pc}
0058520c: subeq    pc, r0, r8, ror #18
00585210: andeq    r0, r0, r0, lsr sl
00585214: andeq    r2, r0, r4, asr #22
00585218: andeq    r3, r0, r8, lsr #23

# _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeEPNS0_14CRootSceneNodeE
0061b2f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061b2f8: subs     r4, r2, #0
0061b2fc: sub      sp, sp, #0x44
0061b300: mov      r8, r0
0061b304: mov      fp, r1
0061b308: mov      sl, r3
0061b30c: moveq    r6, r4
0061b310: beq      #0x61b55c
0061b314: ldr      r3, [r4, #0x4c]
0061b318: cmp      r3, #0
0061b31c: beq      #0x61b874
0061b320: ldr      r3, [r0, #4]
0061b324: mov      r1, r0
0061b328: mov      r0, r3
0061b32c: ldr      r3, [r3]
0061b330: mov      lr, pc
0061b334: ldr      pc, [r3, #0x40]
0061b338: mov      r6, r0
0061b33c: ldr      r1, [r4, #0x40]
0061b340: cmp      r1, #0
0061b344: ble      #0x61b434
0061b348: add      r3, sp, #0x38
0061b34c: add      ip, sp, #0x3c
0061b350: mov      r5, #0
0061b354: str      r3, [sp, #8]
0061b358: str      ip, [sp, #0xc]
0061b35c: mov      r7, r6
0061b360: ldr      r2, [r4, #0x44]
0061b364: lsl      r6, r5, #3
0061b368: ldr      r3, [r2, r5, lsl #3]
0061b36c: add      r2, r2, r6
0061b370: sub      r3, r3, #1
0061b374: cmp      r3, #0xc
0061b378: addls    pc, pc, r3, lsl #2
0061b37c: b        #0x61b424
0061b380: b        #0x61b84c
0061b384: b        #0x61b720
0061b388: b        #0x61b68c
0061b38c: b        #0x61b664
0061b390: b        #0x61b424
0061b394: b        #0x61b424
0061b398: b        #0x61b424
0061b39c: b        #0x61b424
0061b3a0: b        #0x61b804
0061b3a4: b        #0x61b3b4
0061b3a8: b        #0x61b828
0061b3ac: b        #0x61b614
0061b3b0: b        #0x61b568
0061b3b4: ldr      r1, [r2, #4]
0061b3b8: mov      r0, r8
0061b3bc: mov      r2, fp
0061b3c0: mov      r3, sl
0061b3c4: bl       #0x61a608
0061b3c8: subs     sb, r0, #0
0061b3cc: beq      #0x61b600
0061b3d0: ldr      r2, [r4, #0x44]
0061b3d4: ldr      r3, [sb]
0061b3d8: add      r6, r2, r6
0061b3dc: ldr      r2, [r6, #4]
0061b3e0: ldr      r1, [r2, #0x14]
0061b3e4: mov      lr, pc
0061b3e8: ldr      pc, [r3, #0xd4]
0061b3ec: mov      r0, sb
0061b3f0: ldr      r3, [sb]
0061b3f4: mov      lr, pc
0061b3f8: ldr      pc, [r3, #0x104]
0061b3fc: mov      r0, r7
0061b400: ldr      r3, [r7]
0061b404: mov      r1, sb
0061b408: mov      lr, pc
0061b40c: ldr      pc, [r3, #0x5c]
0061b410: ldr      r3, [sb]
0061b414: ldr      r0, [r3, #-0xc]
0061b418: add      r0, sb, r0
0061b41c: bl       #0x31d584
0061b420: ldr      r1, [r4, #0x40]
0061b424: add      r5, r5, #1
0061b428: cmp      r5, r1
0061b42c: blt      #0x61b360
0061b430: mov      r6, r7
0061b434: mov      r0, r6
0061b438: ldr      r1, [r4, #4]
0061b43c: ldr      r3, [r6]
0061b440: mov      lr, pc
0061b444: ldr      pc, [r3, #0x28]
0061b448: ldr      r3, [r6]
0061b44c: ldr      r2, [r4, #0xc]
0061b450: mov      r0, r6
0061b454: ldr      r3, [r3, #0xa4]
0061b458: str      r2, [sp, #0x2c]
0061b45c: ldr      r2, [r4, #0x10]
0061b460: add      r1, sp, #0x2c
0061b464: str      r2, [sp, #0x30]
0061b468: ldr      r2, [r4, #0x14]
0061b46c: str      r2, [sp, #0x34]
0061b470: blx      r3
0061b474: ldr      r3, [r6]
0061b478: ldr      r2, [r4, #0x18]
0061b47c: mov      r0, r6
0061b480: ldr      r3, [r3, #0x9c]
0061b484: str      r2, [sp, #0x10]
0061b488: ldr      r2, [r4, #0x1c]
0061b48c: add      r1, sp, #0x10
0061b490: str      r2, [sp, #0x14]
0061b494: ldr      r2, [r4, #0x20]
0061b498: str      r2, [sp, #0x18]
0061b49c: ldr      r2, [r4, #0x24]
0061b4a0: str      r2, [sp, #0x1c]
0061b4a4: blx      r3
0061b4a8: ldr      r3, [r6]
0061b4ac: ldr      r2, [r4, #0x28]
0061b4b0: mov      r0, r6
0061b4b4: ldr      r3, [r3, #0x94]
0061b4b8: str      r2, [sp, #0x20]
0061b4bc: ldr      r2, [r4, #0x2c]
0061b4c0: add      r1, sp, #0x20
0061b4c4: str      r2, [sp, #0x24]
0061b4c8: ldr      r2, [r4, #0x30]
0061b4cc: str      r2, [sp, #0x28]
0061b4d0: blx      r3
0061b4d4: ldr      r1, [r4, #0x34]
0061b4d8: ldr      r3, [r6]
0061b4dc: mov      r0, r6
0061b4e0: subs     r1, r1, #0
0061b4e4: movne    r1, #1
0061b4e8: mov      lr, pc
0061b4ec: ldr      pc, [r3, #0x48]
0061b4f0: ldr      r3, [r4, #0x38]
0061b4f4: cmp      r3, #0
0061b4f8: ble      #0x61b55c
0061b4fc: mov      r5, #0
0061b500: mov      r7, r5
0061b504: mov      sb, r8
0061b508: ldr      r2, [r4, #0x3c]
0061b50c: mov      r3, sl
0061b510: mov      r1, fp
0061b514: add      r2, r2, r5
0061b518: mov      r0, sb
0061b51c: bl       #0x61b2f4
0061b520: ldr      r3, [r6]
0061b524: mov      r8, r0
0061b528: mov      r1, r0
0061b52c: mov      r0, r6
0061b530: mov      lr, pc
0061b534: ldr      pc, [r3, #0x5c]
0061b538: ldr      r3, [r8]
0061b53c: add      r7, r7, #1
0061b540: add      r5, r5, #0x50
0061b544: ldr      r0, [r3, #-0xc]
0061b548: add      r0, r8, r0
0061b54c: bl       #0x31d584
0061b550: ldr      r3, [r4, #0x38]
0061b554: cmp      r7, r3
0061b558: blt      #0x61b508
0061b55c: mov      r0, r6
0061b560: add      sp, sp, #0x44
0061b564: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061b568: ldr      r2, [r2, #4]
0061b56c: ldr      r0, [sp, #8]
0061b570: mov      r1, r8
0061b574: mov      r3, sl
0061b578: bl       #0x60e6f0
0061b57c: ldr      r3, [r8, #4]
0061b580: mov      r1, r8
0061b584: ldr      r2, [sp, #8]
0061b588: mov      r0, r3
0061b58c: ldr      ip, [r3]
0061b590: ldr      r3, [r4, #0x48]
0061b594: mov      lr, pc
0061b598: ldr      pc, [ip, #0x50]
0061b59c: subs     sb, r0, #0
0061b5a0: beq      #0x61b5f0
0061b5a4: ldr      r2, [r4, #0x44]
0061b5a8: ldr      r3, [sb]
0061b5ac: add      r6, r2, r6
0061b5b0: ldr      r2, [r6, #4]
0061b5b4: ldr      r1, [r2, #0x14]
0061b5b8: mov      lr, pc
0061b5bc: ldr      pc, [r3, #0xd4]
0061b5c0: mov      r0, sb
0061b5c4: mov      r1, #2
0061b5c8: bl       #0x59719c
0061b5cc: mov      r0, r7
0061b5d0: ldr      r3, [r7]
0061b5d4: mov      r1, sb
0061b5d8: mov      lr, pc
0061b5dc: ldr      pc, [r3, #0x5c]
0061b5e0: ldr      r3, [sb]
0061b5e4: ldr      r0, [r3, #-0xc]
0061b5e8: add      r0, sb, r0
0061b5ec: bl       #0x31d584
0061b5f0: ldr      r0, [sp, #0x38]
0061b5f4: cmp      r0, #0
0061b5f8: beq      #0x61b600
0061b5fc: bl       #0x31d584
0061b600: ldr      r1, [r4, #0x40]
0061b604: add      r5, r5, #1
0061b608: cmp      r5, r1
0061b60c: blt      #0x61b360
0061b610: b        #0x61b430
0061b614: ldr      r1, [r2, #4]
0061b618: mov      r0, r8
0061b61c: mov      r2, sl
0061b620: bl       #0x61a994
0061b624: subs     r6, r0, #0
0061b628: beq      #0x61b600
0061b62c: mov      r1, r6
0061b630: mov      r0, r7
0061b634: ldr      r3, [r7]
0061b638: mov      lr, pc
0061b63c: ldr      pc, [r3, #0x5c]
0061b640: ldr      r3, [r6]
0061b644: add      r5, r5, #1
0061b648: ldr      r0, [r3, #-0xc]
0061b64c: add      r0, r6, r0
0061b650: bl       #0x31d584
0061b654: ldr      r1, [r4, #0x40]
0061b658: cmp      r5, r1
0061b65c: blt      #0x61b360
0061b660: b        #0x61b430
0061b664: ldr      r3, [r2, #4]
0061b668: mov      r0, r8
0061b66c: mov      r2, sl
0061b670: ldr      r1, [r3, #4]
0061b674: add      r1, r1, #1
0061b678: bl       #0x61b24c
0061b67c: subs     r6, r0, #0
0061b680: bne      #0x61b62c
0061b684: ldr      r1, [r4, #0x40]
0061b688: b        #0x61b604
0061b68c: ldr      r3, [r2, #4]
0061b690: ldr      r0, [sp, #0xc]
0061b694: mov      r1, r8
0061b698: mov      r2, fp
0061b69c: str      sl, [sp]
0061b6a0: bl       #0x61aeb8
0061b6a4: ldr      r0, [sp, #0x3c]
0061b6a8: cmp      r0, #0
0061b6ac: str      r0, [sp, #0x38]
0061b6b0: ldrne    r3, [r0, #4]
0061b6b4: addne    r3, r3, #1
0061b6b8: strne    r3, [r0, #4]
0061b6bc: ldrne    r0, [sp, #0x3c]
0061b6c0: cmp      r0, #0
0061b6c4: beq      #0x61b6cc
0061b6c8: bl       #0x31d584
0061b6cc: ldr      r3, [sp, #0x38]
0061b6d0: cmp      r3, #0
0061b6d4: beq      #0x61b600
0061b6d8: ldr      r3, [r8, #4]
0061b6dc: mov      r1, r8
0061b6e0: ldr      r2, [sp, #8]
0061b6e4: mov      r0, r3
0061b6e8: ldr      ip, [r3]
0061b6ec: ldr      r3, [r4, #0x48]
0061b6f0: mov      lr, pc
0061b6f4: ldr      pc, [ip, #0x48]
0061b6f8: subs     sb, r0, #0
0061b6fc: beq      #0x61b7f0
0061b700: ldr      r2, [r4, #0x44]
0061b704: ldr      r3, [sb]
0061b708: add      r6, r2, r6
0061b70c: ldr      r2, [r6, #4]
0061b710: ldr      r1, [r2, #0x14]
0061b714: mov      lr, pc
0061b718: ldr      pc, [r3, #0xd4]
0061b71c: b        #0x61b7cc
0061b720: ldr      r3, [r2, #4]
0061b724: mov      ip, #1
0061b728: ldr      r0, [sp, #8]
0061b72c: mov      r1, r8
0061b730: mov      r2, fp
0061b734: stm      sp, {sl, ip}
0061b738: bl       #0x61ace8
0061b73c: ldr      r3, [sp, #0x38]
0061b740: mov      r0, r3
0061b744: ldr      r3, [r3]
0061b748: mov      lr, pc
0061b74c: ldr      pc, [r3, #0x30]
0061b750: cmp      r0, #2
0061b754: beq      #0x61b894
0061b758: ldr      r3, [sp, #0x38]
0061b75c: mov      r0, r3
0061b760: ldr      r3, [r3]
0061b764: mov      lr, pc
0061b768: ldr      pc, [r3, #0x30]
0061b76c: cmp      r0, #3
0061b770: beq      #0x61b894
0061b774: ldr      r3, [r8, #4]
0061b778: mov      r1, r8
0061b77c: ldr      r2, [sp, #8]
0061b780: mov      r0, r3
0061b784: ldr      ip, [r3]
0061b788: ldr      r3, [r4, #0x48]
0061b78c: mov      lr, pc
0061b790: ldr      pc, [ip, #0x48]
0061b794: mov      sb, r0
0061b798: cmp      sb, #0
0061b79c: beq      #0x61b7f0
0061b7a0: ldr      r2, [r4, #0x44]
0061b7a4: mov      r0, sb
0061b7a8: ldr      r3, [sb]
0061b7ac: add      r6, r2, r6
0061b7b0: ldr      r2, [r6, #4]
0061b7b4: ldr      r1, [r2, #0x14]
0061b7b8: mov      lr, pc
0061b7bc: ldr      pc, [r3, #0xd4]
0061b7c0: mov      r0, sb
0061b7c4: mov      r1, #2
0061b7c8: bl       #0x59719c
0061b7cc: mov      r0, r7
0061b7d0: ldr      r3, [r7]
0061b7d4: mov      r1, sb
0061b7d8: mov      lr, pc
0061b7dc: ldr      pc, [r3, #0x5c]
0061b7e0: ldr      r3, [sb]
0061b7e4: ldr      r0, [r3, #-0xc]
0061b7e8: add      r0, sb, r0
0061b7ec: bl       #0x31d584
0061b7f0: ldr      r0, [sp, #0x38]
0061b7f4: cmp      r0, #0
0061b7f8: bne      #0x61b41c
0061b7fc: ldr      r1, [r4, #0x40]
0061b800: b        #0x61b604
0061b804: ldr      r1, [r2, #4]
0061b808: mov      r0, r8
0061b80c: mov      r2, fp
0061b810: mov      r3, sl
0061b814: bl       #0x61a888
0061b818: subs     sb, r0, #0
0061b81c: bne      #0x61b3d0
0061b820: ldr      r1, [r4, #0x40]
0061b824: b        #0x61b604
0061b828: ldr      r1, [r2, #4]
0061b82c: mov      r0, r8
0061b830: mov      r2, fp
0061b834: mov      r3, sl
0061b838: bl       #0x61a77c
0061b83c: subs     r6, r0, #0
0061b840: bne      #0x61b62c
0061b844: ldr      r1, [r4, #0x40]
0061b848: b        #0x61b604
0061b84c: ldr      r3, [r2, #4]
0061b850: mov      r0, r8
0061b854: mov      r2, sl
0061b858: ldr      r1, [r3, #4]
0061b85c: add      r1, r1, #1
0061b860: bl       #0x61b2d0
0061b864: subs     r6, r0, #0
0061b868: bne      #0x61b62c
0061b86c: ldr      r1, [r4, #0x40]
0061b870: b        #0x61b604
0061b874: ldr      r3, [r0, #4]
0061b878: mov      r1, r0
0061b87c: mov      r0, r3
0061b880: ldr      r3, [r3]
0061b884: mov      lr, pc
0061b888: ldr      pc, [r3, #0x3c]
0061b88c: mov      r6, r0
0061b890: b        #0x61b33c
0061b894: ldr      r3, [r8, #4]
0061b898: mov      r1, r8
0061b89c: ldr      r2, [sp, #8]
0061b8a0: mov      r0, r3
0061b8a4: ldr      ip, [r3]
0061b8a8: ldr      r3, [r4, #0x48]
0061b8ac: mov      lr, pc
0061b8b0: ldr      pc, [ip, #0x4c]
0061b8b4: mov      sb, r0
0061b8b8: b        #0x61b798

# _ZN6glitch4core8CMatrix4IfE9postScaleERKNS0_8vector3dIfEE
00597788: push     {r4, r5, r6, lr}
0059778c: ldrb     r3, [r0, #0x40]
00597790: mov      r4, r0
00597794: mov      r5, r1
00597798: cmp      r3, #0
0059779c: bne      #0x59783c
005977a0: strb     r3, [r0, #0x40]
005977a4: ldr      r1, [r1]
005977a8: ldr      r0, [r0]
005977ac: bl       #0x30ed6c
005977b0: str      r0, [r4]
005977b4: ldr      r1, [r5]
005977b8: ldr      r0, [r4, #4]
005977bc: bl       #0x30ed6c
005977c0: str      r0, [r4, #4]
005977c4: ldr      r1, [r5]
005977c8: ldr      r0, [r4, #8]
005977cc: bl       #0x30ed6c
005977d0: str      r0, [r4, #8]
005977d4: ldr      r1, [r5, #4]
005977d8: ldr      r0, [r4, #0x10]
005977dc: bl       #0x30ed6c
005977e0: str      r0, [r4, #0x10]
005977e4: ldr      r1, [r5, #4]
005977e8: ldr      r0, [r4, #0x14]
005977ec: bl       #0x30ed6c
005977f0: str      r0, [r4, #0x14]
005977f4: ldr      r1, [r5, #4]
005977f8: ldr      r0, [r4, #0x18]
005977fc: bl       #0x30ed6c
00597800: str      r0, [r4, #0x18]
00597804: ldr      r1, [r5, #8]
00597808: ldr      r0, [r4, #0x20]
0059780c: bl       #0x30ed6c
00597810: str      r0, [r4, #0x20]
00597814: ldr      r1, [r5, #8]
00597818: ldr      r0, [r4, #0x24]
0059781c: bl       #0x30ed6c
00597820: str      r0, [r4, #0x24]
00597824: ldr      r1, [r5, #8]
00597828: ldr      r0, [r4, #0x28]
0059782c: bl       #0x30ed6c
00597830: str      r0, [r4, #0x28]
00597834: mov      r0, r4
00597838: pop      {r4, r5, r6, pc}
0059783c: mov      r3, #0
00597840: strb     r3, [r0, #0x40]
00597844: ldr      r3, [r1]
00597848: str      r3, [r0]
0059784c: ldr      r3, [r1, #4]
00597850: str      r3, [r0, #0x14]
00597854: ldr      r3, [r1, #8]
00597858: str      r3, [r0, #0x28]
0059785c: mov      r0, r4
00597860: pop      {r4, r5, r6, pc}
