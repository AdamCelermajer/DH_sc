
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

# _ZNK6Module10LoadModuleEv
0038a88c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038a890: ldr      sb, [pc, #0x1fc]
0038a894: ldr      r3, [pc, #0x1fc]
0038a898: ldr      fp, [pc, #0x1fc]
0038a89c: add      sb, pc, sb
0038a8a0: ldr      r2, [sb, r3]
0038a8a4: ldr      r3, [sb, fp]
0038a8a8: sub      sp, sp, #0x84
0038a8ac: ldr      r4, [r2]
0038a8b0: ldr      r3, [r3]
0038a8b4: mov      r5, r0
0038a8b8: cmp      r4, #0
0038a8bc: str      r3, [sp, #0x7c]
0038a8c0: beq      #0x38aa3c
0038a8c4: mov      r0, r4
0038a8c8: ldr      r1, [r5, #0x40c]
0038a8cc: bl       #0x3ef278
0038a8d0: ldr      r3, [r5, #0x160]
0038a8d4: add      r6, sp, #0x64
0038a8d8: mov      r0, r6
0038a8dc: str      r3, [r4, #0x160]
0038a8e0: ldr      r3, [r5, #0x164]
0038a8e4: mov      r1, #0x10
0038a8e8: add      r7, sp, #0x4c
0038a8ec: str      r3, [r4, #0x164]
0038a8f0: ldr      r3, [r5, #0x168]
0038a8f4: mov      r8, #0
0038a8f8: str      r3, [r4, #0x168]
0038a8fc: str      r6, [sp, #0x74]
0038a900: str      r6, [sp, #0x78]
0038a904: bl       #0x31167c
0038a908: ldr      r3, [sp, #0x74]
0038a90c: mov      r0, r7
0038a910: mov      r1, #0x10
0038a914: strb     r8, [r3]
0038a918: str      r7, [sp, #0x5c]
0038a91c: str      r7, [sp, #0x60]
0038a920: bl       #0x31167c
0038a924: ldr      r3, [sp, #0x5c]
0038a928: mov      r2, r7
0038a92c: mov      r0, r5
0038a930: strb     r8, [r3]
0038a934: mov      r1, r6
0038a938: bl       #0x38a38c
0038a93c: ldr      r3, [sp, #0x74]
0038a940: ldr      r2, [sp, #0x78]
0038a944: cmp      r2, r3
0038a948: beq      #0x38a998
0038a94c: ldr      r8, [pc, #0x14c]
0038a950: add      r5, sp, #0x34
0038a954: add      sl, sp, #0x18
0038a958: add      r8, pc, r8
0038a95c: mov      r1, r8
0038a960: mov      r2, sl
0038a964: mov      r0, r5
0038a968: bl       #0x3140ec
0038a96c: mov      r1, r6
0038a970: mov      r2, r5
0038a974: mov      r0, r4
0038a978: bl       #0x3f3b40
0038a97c: mov      r3, r0
0038a980: mov      r0, r5
0038a984: str      r3, [sp, #0xc]
0038a988: bl       #0x3139ac
0038a98c: ldr      r3, [sp, #0xc]
0038a990: cmp      r3, #0
0038a994: beq      #0x38a95c
0038a998: ldr      r3, [sp, #0x5c]
0038a99c: ldr      r2, [sp, #0x60]
0038a9a0: cmp      r2, r3
0038a9a4: beq      #0x38a9f4
0038a9a8: ldr      r8, [pc, #0xf4]
0038a9ac: add      r5, sp, #0x1c
0038a9b0: add      sl, sp, #0x14
0038a9b4: add      r8, pc, r8
0038a9b8: mov      r1, r8
0038a9bc: mov      r2, sl
0038a9c0: mov      r0, r5
0038a9c4: bl       #0x3140ec
0038a9c8: mov      r1, r7
0038a9cc: mov      r2, r5
0038a9d0: mov      r0, r4
0038a9d4: bl       #0x3f3b40
0038a9d8: mov      r3, r0
0038a9dc: mov      r0, r5
0038a9e0: str      r3, [sp, #0xc]
0038a9e4: bl       #0x3139ac
0038a9e8: ldr      r3, [sp, #0xc]
0038a9ec: cmp      r3, #0
0038a9f0: beq      #0x38a9b8
0038a9f4: mov      r3, #0
0038a9f8: str      r3, [r4, #0x168]
0038a9fc: str      r3, [r4, #0x160]
0038aa00: str      r3, [r4, #0x164]
0038aa04: mvn      r1, #0
0038aa08: mov      r0, r4
0038aa0c: bl       #0x3ef278
0038aa10: mov      r0, r7
0038aa14: bl       #0x3139ac
0038aa18: mov      r0, r6
0038aa1c: bl       #0x3139ac
0038aa20: ldr      r3, [sb, fp]
0038aa24: ldr      r2, [sp, #0x7c]
0038aa28: ldr      r3, [r3]
0038aa2c: cmp      r2, r3
0038aa30: bne      #0x38aa90
0038aa34: add      sp, sp, #0x84
0038aa38: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038aa3c: ldr      r3, [pc, #0x64]
0038aa40: ldr      r3, [sb, r3]
0038aa44: ldr      r3, [r3]
0038aa48: cmp      r3, #2
0038aa4c: streq    r4, [r4]
0038aa50: beq      #0x38a8c4
0038aa54: cmp      r3, #1
0038aa58: bne      #0x38a8c4
0038aa5c: ldr      r0, [pc, #0x48]
0038aa60: ldr      r1, [pc, #0x48]
0038aa64: ldr      r2, [pc, #0x48]
0038aa68: ldr      r0, [sb, r0]
0038aa6c: ldr      r3, [pc, #0x44]
0038aa70: mov      ip, #0x54
0038aa74: add      r1, pc, r1
0038aa78: add      r2, pc, r2
0038aa7c: add      r3, pc, r3
0038aa80: add      r0, r0, #0xa8
0038aa84: str      ip, [sp]
0038aa88: bl       #0x30e004
0038aa8c: b        #0x38a8c4
0038aa90: bl       #0x30e310

# _ZN7PFFloor14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEE
0051b96c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051b970: ldr      r5, [r1]
0051b974: sub      sp, sp, #0x30
0051b978: mov      r6, r1
0051b97c: mov      r4, r0
0051b980: mov      r1, r5
0051b984: ldr      r0, [r0, #0x44]
0051b988: mov      r7, r2
0051b98c: mov      sl, r3
0051b990: bl       #0x30e9ac
0051b994: ldr      r8, [pc, #0x138]
0051b998: cmp      r0, #0
0051b99c: add      r8, pc, r8
0051b9a0: beq      #0x51b9b8
0051b9a4: mov      r0, r5
0051b9a8: ldr      r1, [r4, #0x50]
0051b9ac: bl       #0x30e9ac
0051b9b0: cmp      r0, #0
0051b9b4: bne      #0x51b9c4
0051b9b8: mov      r0, #0
0051b9bc: add      sp, sp, #0x30
0051b9c0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051b9c4: ldr      sb, [r6, #4]
0051b9c8: ldr      r0, [r4, #0x48]
0051b9cc: mov      r1, sb
0051b9d0: bl       #0x30e9ac
0051b9d4: cmp      r0, #0
0051b9d8: beq      #0x51b9b8
0051b9dc: mov      r0, sb
0051b9e0: ldr      r1, [r4, #0x54]
0051b9e4: bl       #0x30e9ac
0051b9e8: cmp      r0, #0
0051b9ec: beq      #0x51b9b8
0051b9f0: ldr      r6, [r6, #8]
0051b9f4: ldr      r0, [r4, #0x4c]
0051b9f8: mov      r1, r6
0051b9fc: bl       #0x30e9ac
0051ba00: cmp      r0, #0
0051ba04: beq      #0x51b9b8
0051ba08: mov      r0, r6
0051ba0c: ldr      r1, [r4, #0x58]
0051ba10: bl       #0x30e9ac
0051ba14: cmp      r0, #0
0051ba18: beq      #0x51b9b8
0051ba1c: ldr      r2, [pc, #0xb4]
0051ba20: mov      r1, #0x44000000
0051ba24: mov      r3, #0
0051ba28: ldr      r2, [r8, r2]
0051ba2c: add      r1, r1, #0x7a0000
0051ba30: mov      r0, r6
0051ba34: ldr      r2, [r2, #0x10]
0051ba38: ldr      r8, [r2, #0x1c]
0051ba3c: str      r3, [sp, #0x2c]
0051ba40: str      r3, [sp, #0x24]
0051ba44: str      r3, [sp, #0x28]
0051ba48: str      r5, [sp, #0x18]
0051ba4c: str      r5, [sp, #0xc]
0051ba50: str      sb, [sp, #0x1c]
0051ba54: str      sb, [sp, #0x10]
0051ba58: bl       #0x30eba4
0051ba5c: mov      r1, #0x44000000
0051ba60: add      r1, r1, #0x7a0000
0051ba64: str      r0, [sp, #0x14]
0051ba68: mov      r0, r6
0051ba6c: bl       #0x30e3ac
0051ba70: str      r0, [sp, #0x20]
0051ba74: ldr      r5, [r8, #0x2c]
0051ba78: ldr      r3, [r4, #0x40]
0051ba7c: ldr      r2, [r5]
0051ba80: mov      r0, r3
0051ba84: ldr      r3, [r3]
0051ba88: ldr      r4, [r2, #0xc]
0051ba8c: mov      lr, pc
0051ba90: ldr      pc, [r3, #0xb0]
0051ba94: str      sl, [sp]
0051ba98: mov      r2, r0
0051ba9c: add      r1, sp, #0xc
0051baa0: mov      r0, r5
0051baa4: add      r3, sp, #0x24
0051baa8: blx      r4
0051baac: cmp      r0, #0
0051bab0: beq      #0x51b9b8
0051bab4: ldr      r2, [sp, #0x28]
0051bab8: ldr      r3, [sp, #0x2c]
0051babc: ldr      r1, [sp, #0x24]
0051bac0: mov      r0, #1
0051bac4: str      r2, [r7, #4]
0051bac8: str      r1, [r7]
0051bacc: str      r3, [r7, #8]
0051bad0: b        #0x51b9bc
0051bad4: strdeq   sb, sl, [r7], #-4
0051bad8: strdeq   r3, r4, [r0], -r4

# _ZN7PFWorld16GetFloorHeightAtERK7Point3DIfEPfPS1_PP6PFRoomPP7PFFloorb
00525508: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052550c: ldr      r4, [r1]
00525510: sub      sp, sp, #0xc
00525514: mov      r6, r1
00525518: mov      r5, r0
0052551c: mov      r1, r4
00525520: ldr      r0, [r0, #0x14]
00525524: mov      sl, r2
00525528: mov      r8, r3
0052552c: bl       #0x30e9ac
00525530: cmp      r0, #0
00525534: ldr      fp, [sp, #0x30]
00525538: ldr      sb, [sp, #0x34]
0052553c: ldrb     r7, [sp, #0x38]
00525540: beq      #0x525558
00525544: mov      r0, r4
00525548: ldr      r1, [r5, #0x20]
0052554c: bl       #0x30e9ac
00525550: cmp      r0, #0
00525554: bne      #0x525564
00525558: mov      r0, #0
0052555c: add      sp, sp, #0xc
00525560: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00525564: ldr      r4, [r6, #4]
00525568: ldr      r0, [r5, #0x18]
0052556c: mov      r1, r4
00525570: bl       #0x30e9ac
00525574: cmp      r0, #0
00525578: beq      #0x525558
0052557c: mov      r0, r4
00525580: ldr      r1, [r5, #0x24]
00525584: bl       #0x30e9ac
00525588: cmp      r0, #0
0052558c: beq      #0x525558
00525590: ldr      r4, [r6, #8]
00525594: ldr      r0, [r5, #0x1c]
00525598: mov      r1, r4
0052559c: bl       #0x30e9ac
005255a0: cmp      r0, #0
005255a4: beq      #0x525558
005255a8: mov      r0, r4
005255ac: ldr      r1, [r5, #0x28]
005255b0: bl       #0x30e9ac
005255b4: cmp      r0, #0
005255b8: beq      #0x525558
005255bc: ldr      r3, [r5, #8]
005255c0: ldr      r2, [r5, #0xc]
005255c4: rsb      r2, r3, r2
005255c8: lsrs     r2, r2, #2
005255cc: beq      #0x525558
005255d0: mov      r4, #0
005255d4: b        #0x5255ec
005255d8: ldr      r3, [r5, #8]
005255dc: ldr      r2, [r5, #0xc]
005255e0: rsb      r2, r3, r2
005255e4: cmp      r4, r2, asr #2
005255e8: bhs      #0x525558
005255ec: ldr      r0, [r3, r4, lsl #2]
005255f0: mov      r1, r6
005255f4: mov      r3, r8
005255f8: mov      r2, sl
005255fc: str      sb, [sp]
00525600: str      r7, [sp, #4]
00525604: bl       #0x520f98
00525608: cmp      r0, #0
0052560c: lsl      r3, r4, #2
00525610: add      r4, r4, #1
00525614: beq      #0x5255d8
00525618: cmp      fp, #0
0052561c: ldrne    r2, [r5, #8]
00525620: moveq    r0, #1
00525624: movne    r0, #1
00525628: ldrne    r3, [r2, r3]
0052562c: strne    r3, [fp]
00525630: b        #0x52555c

# _ZN6PFRoom10_LoadFloorEPN6glitch5scene14IMeshSceneNodeEPKc
00521eb8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00521ebc: ldr      r6, [pc, #0x360]
00521ec0: ldr      r7, [pc, #0x360]
00521ec4: sub      sp, sp, #0x30
00521ec8: add      r6, pc, r6
00521ecc: ldr      r3, [r6, r7]
00521ed0: subs     sb, r1, #0
00521ed4: mov      r4, r0
00521ed8: ldr      r3, [r3]
00521edc: mov      r8, r2
00521ee0: str      r3, [sp, #0x2c]
00521ee4: beq      #0x522120
00521ee8: mov      r1, #0
00521eec: mov      r0, #0xcc
00521ef0: bl       #0x310570
00521ef4: ldr      ip, [r4, #0x2c]
00521ef8: ldr      r3, [r4, #0x28]
00521efc: mov      r1, r8
00521f00: str      ip, [sp]
00521f04: mov      r2, r4
00521f08: mov      ip, #1
00521f0c: mov      r5, r0
00521f10: str      ip, [sp, #4]
00521f14: bl       #0x51d1b4
00521f18: ldr      r8, [r4, #0x34]
00521f1c: ldr      r3, [r4, #0x38]
00521f20: cmp      r8, r3
00521f24: beq      #0x522174
00521f28: str      r5, [r8]
00521f2c: ldr      r3, [r4, #0x34]
00521f30: add      r3, r3, #4
00521f34: str      r3, [r4, #0x34]
00521f38: ldr      r3, [pc, #0x2ec]
00521f3c: add      r8, sp, #0x14
00521f40: ldr      sl, [r6, r3]
00521f44: mov      r0, sl
00521f48: bl       #0x337888
00521f4c: ldr      r1, [pc, #0x2dc]
00521f50: add      r2, sp, #0x10
00521f54: mov      r0, r8
00521f58: add      r1, pc, r1
00521f5c: bl       #0x3140ec
00521f60: mov      r0, sl
00521f64: mov      r1, r8
00521f68: bl       #0x337a88
00521f6c: mov      sl, r0
00521f70: ldr      r0, [sp, #0x28]
00521f74: cmp      r0, r8
00521f78: beq      #0x521f98
00521f7c: cmp      r0, #0
00521f80: beq      #0x521f98
00521f84: ldr      r1, [sp, #0x14]
00521f88: rsb      r1, r0, r1
00521f8c: cmp      r1, #0x80
00521f90: bhi      #0x522118
00521f94: bl       #0x708f00
00521f98: cmp      sl, #0
00521f9c: beq      #0x5220c0
00521fa0: bl       #0x60b0cc
00521fa4: mov      r0, r5
00521fa8: mov      r1, sb
00521fac: bl       #0x520b40
00521fb0: bl       #0x60b0cc
00521fb4: ldr      r2, [r4, #0x34]
00521fb8: ldr      r3, [r4, #0x30]
00521fbc: rsb      r3, r3, r2
00521fc0: asr      r3, r3, #2
00521fc4: cmp      r3, #1
00521fc8: beq      #0x5220e4
00521fcc: ldr      r8, [r5, #0x44]
00521fd0: ldr      sl, [r4, #0x3c]
00521fd4: mov      r0, r8
00521fd8: mov      r1, sl
00521fdc: bl       #0x30e70c
00521fe0: cmp      r0, #0
00521fe4: moveq    r8, sl
00521fe8: str      r8, [r4, #0x3c]
00521fec: ldr      r8, [r5, #0x48]
00521ff0: ldr      sl, [r4, #0x40]
00521ff4: mov      r0, r8
00521ff8: mov      r1, sl
00521ffc: bl       #0x30e70c
00522000: cmp      r0, #0
00522004: moveq    r8, sl
00522008: str      r8, [r4, #0x40]
0052200c: ldr      r8, [r5, #0x4c]
00522010: ldr      sl, [r4, #0x44]
00522014: mov      r0, r8
00522018: mov      r1, sl
0052201c: bl       #0x30e70c
00522020: cmp      r0, #0
00522024: moveq    r8, sl
00522028: str      r8, [r4, #0x44]
0052202c: ldr      r8, [r5, #0x50]
00522030: ldr      sl, [r4, #0x48]
00522034: mov      r1, r8
00522038: mov      r0, sl
0052203c: bl       #0x30e70c
00522040: cmp      r0, #0
00522044: moveq    r8, sl
00522048: str      r8, [r4, #0x48]
0052204c: ldr      r8, [r5, #0x54]
00522050: ldr      sl, [r4, #0x4c]
00522054: mov      r1, r8
00522058: mov      r0, sl
0052205c: bl       #0x30e70c
00522060: cmp      r0, #0
00522064: moveq    r8, sl
00522068: str      r8, [r4, #0x4c]
0052206c: ldr      sl, [r4, #0x50]
00522070: ldr      r8, [r5, #0x58]
00522074: mov      r0, sl
00522078: mov      r1, r8
0052207c: bl       #0x30e70c
00522080: cmp      r0, #0
00522084: moveq    r8, sl
00522088: str      r8, [r4, #0x50]
0052208c: ldr      r3, [pc, #0x1a0]
00522090: ldr      r1, [r5, #0x40]
00522094: ldr      r3, [r6, r3]
00522098: ldr      r3, [r3, #0x10]
0052209c: ldr      r0, [r3, #0x1c]
005220a0: bl       #0x3524a0
005220a4: ldr      r3, [r6, r7]
005220a8: ldr      r2, [sp, #0x2c]
005220ac: ldr      r3, [r3]
005220b0: cmp      r2, r3
005220b4: bne      #0x522220
005220b8: add      sp, sp, #0x30
005220bc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
005220c0: mov      r1, sb
005220c4: mov      r0, r5
005220c8: bl       #0x520b40
005220cc: ldr      r2, [r4, #0x34]
005220d0: ldr      r3, [r4, #0x30]
005220d4: rsb      r3, r3, r2
005220d8: asr      r3, r3, #2
005220dc: cmp      r3, #1
005220e0: bne      #0x521fcc
005220e4: ldr      r3, [r5, #0x44]
005220e8: str      r3, [r4, #0x3c]
005220ec: ldr      r3, [r5, #0x48]
005220f0: str      r3, [r4, #0x40]
005220f4: ldr      r3, [r5, #0x4c]
005220f8: str      r3, [r4, #0x44]
005220fc: ldr      r3, [r5, #0x50]
00522100: str      r3, [r4, #0x48]
00522104: ldr      r3, [r5, #0x54]
00522108: str      r3, [r4, #0x4c]
0052210c: ldr      r3, [r5, #0x58]
00522110: str      r3, [r4, #0x50]
00522114: b        #0x52208c
00522118: bl       #0x310440
0052211c: b        #0x521f98
00522120: ldr      r3, [pc, #0x110]
00522124: ldr      r3, [r6, r3]
00522128: ldr      r3, [r3]
0052212c: cmp      r3, #2
00522130: streq    sb, [sb]
00522134: beq      #0x521ee8
00522138: cmp      r3, #1
0052213c: bne      #0x521ee8
00522140: ldr      r0, [pc, #0xf4]
00522144: ldr      r1, [pc, #0xf4]
00522148: ldr      r2, [pc, #0xf4]
0052214c: ldr      r0, [r6, r0]
00522150: ldr      r3, [pc, #0xf0]
00522154: mov      ip, #0x35
00522158: add      r1, pc, r1
0052215c: add      r2, pc, r2
00522160: add      r3, pc, r3
00522164: add      r0, r0, #0xa8
00522168: str      ip, [sp]
0052216c: bl       #0x30e004
00522170: b        #0x521ee8
00522174: ldr      r3, [r4, #0x30]
00522178: rsb      r3, r3, r8
0052217c: asr      r3, r3, #2
00522180: cmp      r3, #1
00522184: addhs    r1, r3, r3
00522188: addlo    r1, r3, #1
0052218c: cmn      r1, #0xc0000001
00522190: bls      #0x5221fc
00522194: mvn      r1, #0xc0000000
00522198: add      r2, sp, #0x30
0052219c: str      r1, [r2, #-0x24]!
005221a0: add      r0, r4, #0x38
005221a4: bl       #0x521d40
005221a8: ldr      r1, [r4, #0x30]
005221ac: mov      sl, r0
005221b0: subs     r8, r8, r1
005221b4: moveq    r8, r0
005221b8: bne      #0x522210
005221bc: str      r5, [r8], #4
005221c0: ldr      r0, [r4, #0x30]
005221c4: ldr      r1, [r4, #0x38]
005221c8: cmp      r0, #0
005221cc: beq      #0x5221e4
005221d0: rsb      r1, r0, r1
005221d4: bic      r1, r1, #3
005221d8: cmp      r1, #0x80
005221dc: bhi      #0x522208
005221e0: bl       #0x708f00
005221e4: ldr      r3, [sp, #0xc]
005221e8: str      sl, [r4, #0x30]
005221ec: str      r8, [r4, #0x34]
005221f0: add      sl, sl, r3, lsl #2
005221f4: str      sl, [r4, #0x38]
005221f8: b        #0x521f38
005221fc: cmp      r3, r1
00522200: bls      #0x522198
00522204: b        #0x522194
00522208: bl       #0x310440
0052220c: b        #0x5221e4
00522210: mov      r2, r8
00522214: bl       #0x30df38
00522218: add      r8, r0, r8
0052221c: b        #0x5221bc
00522220: bl       #0x30e310
00522224: subeq    r2, r7, r8, asr #23
00522228: andeq    r4, r0, ip, lsr #1
0052222c: andeq    r0, r0, r4, lsl #17
00522230: mlaseq   fp, r8, sl, sl
00522234: strdeq   r3, r4, [r0], -r4
00522238: andeq    r3, r0, r0, asr #19
0052223c: andeq    r1, r0, r0, asr #19
00522240: eorseq   ip, sb, r0, lsl #5
00522244: eorseq   sl, fp, ip, lsl #17
00522248: eorseq   sl, fp, r0, lsr #16

# _ZN7PFWorld8LoadRoomEPN6glitch5scene10ISceneNodeEjPKc
00523c14: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00523c18: mov      r4, r0
00523c1c: sub      sp, sp, #0x2c
00523c20: mov      r7, r3
00523c24: mov      r6, r1
00523c28: mov      r8, r2
00523c2c: bl       #0x522844
00523c30: ldr      r3, [r4, #4]
00523c34: ldr      r0, [pc, #0x540]
00523c38: cmp      r3, #1
00523c3c: add      r0, pc, r0
00523c40: str      r0, [sp, #8]
00523c44: beq      #0x523c6c
00523c48: ldr      r3, [pc, #0x530]
00523c4c: ldr      r3, [r0, r3]
00523c50: ldr      r3, [r3]
00523c54: cmp      r3, #2
00523c58: moveq    r3, #0
00523c5c: streq    r3, [r3]
00523c60: beq      #0x523c6c
00523c64: cmp      r3, #1
00523c68: beq      #0x524010
00523c6c: cmp      r6, #0
00523c70: beq      #0x524048
00523c74: mov      r1, #0
00523c78: mov      r0, #0x54
00523c7c: bl       #0x310570
00523c80: ldr      lr, [r4, #0x44]
00523c84: ldr      ip, [r4, #0x48]
00523c88: mov      r1, r7
00523c8c: mov      r3, r4
00523c90: mov      r2, r8
00523c94: mov      r5, r0
00523c98: str      lr, [sp]
00523c9c: str      ip, [sp, #4]
00523ca0: bl       #0x521540
00523ca4: ldr      r7, [r4, #0xc]
00523ca8: ldr      r3, [r4, #0x10]
00523cac: cmp      r7, r3
00523cb0: beq      #0x5240d0
00523cb4: str      r5, [r7]
00523cb8: ldr      r3, [r4, #0xc]
00523cbc: add      r3, r3, #4
00523cc0: str      r3, [r4, #0xc]
00523cc4: ldr      ip, [pc, #0x4b8]
00523cc8: ldr      r0, [sp, #8]
00523ccc: movw     r3, #0x6164
00523cd0: str      ip, [sp, #0x14]
00523cd4: ldr      r2, [r0, ip]
00523cd8: mov      sl, #0
00523cdc: mov      r1, r6
00523ce0: ldr      r0, [r2, #0x10]
00523ce4: movt     r3, #0x6d65
00523ce8: add      r2, sp, #0x18
00523cec: ldr      r0, [r0, #0x1c]
00523cf0: str      sl, [sp, #0x18]
00523cf4: str      sl, [sp, #0x1c]
00523cf8: str      sl, [sp, #0x20]
00523cfc: bl       #0x350e5c
00523d00: ldr      r6, [sp, #0x18]
00523d04: ldr      r7, [sp, #0x1c]
00523d08: cmp      r6, r7
00523d0c: beq      #0x5240a4
00523d10: ldr      r3, [pc, #0x470]
00523d14: ldr      r8, [pc, r3]
00523d18: b        #0x523d3c
00523d1c: mov      r0, sb
00523d20: mov      r1, r8
00523d24: bl       #0x30ebd4
00523d28: cmp      r0, #0
00523d2c: ldrne    sl, [r6]
00523d30: add      r6, r6, #4
00523d34: cmp      r6, r7
00523d38: beq      #0x523da8
00523d3c: ldr      r3, [r6]
00523d40: mov      r0, r3
00523d44: ldr      r3, [r3]
00523d48: mov      lr, pc
00523d4c: ldr      pc, [r3, #0x24]
00523d50: ldrb     r3, [r0]
00523d54: mov      sb, r0
00523d58: cmp      r3, #0
00523d5c: bne      #0x523d1c
00523d60: ldr      r0, [r6]
00523d64: bl       #0x597290
00523d68: cmp      r0, #0
00523d6c: beq      #0x523d1c
00523d70: ldr      r0, [r6]
00523d74: bl       #0x597290
00523d78: ldr      r3, [r0]
00523d7c: mov      lr, pc
00523d80: ldr      pc, [r3, #0x24]
00523d84: mov      sb, r0
00523d88: mov      r0, sb
00523d8c: mov      r1, r8
00523d90: bl       #0x30ebd4
00523d94: cmp      r0, #0
00523d98: ldrne    sl, [r6]
00523d9c: add      r6, r6, #4
00523da0: cmp      r6, r7
00523da4: bne      #0x523d3c
00523da8: ldr      r7, [sp, #0x18]
00523dac: ldr      fp, [sp, #0x1c]
00523db0: cmp      r7, fp
00523db4: beq      #0x5240a4
00523db8: ldr      r3, [pc, #0x3cc]
00523dbc: mov      sb, #0
00523dc0: mov      r8, r4
00523dc4: add      r3, pc, r3
00523dc8: ldr      r1, [r3, #8]
00523dcc: ldr      r3, [r3, #4]
00523dd0: str      r1, [sp, #0xc]
00523dd4: str      r3, [sp, #0x10]
00523dd8: b        #0x523e58
00523ddc: ldr      r1, [sp, #0x10]
00523de0: mov      r0, r4
00523de4: bl       #0x30ebd4
00523de8: cmp      r0, #0
00523dec: mov      r2, r4
00523df0: mov      r0, r5
00523df4: beq      #0x523e28
00523df8: ldr      r1, [r7]
00523dfc: bl       #0x521eb8
00523e00: cmp      sl, #0
00523e04: add      sb, sb, #1
00523e08: beq      #0x523eb4
00523e0c: ldr      ip, [sp, #8]
00523e10: ldr      r2, [sp, #0x14]
00523e14: mov      r1, sl
00523e18: ldr      r3, [ip, r2]
00523e1c: ldr      r3, [r3, #0x10]
00523e20: ldr      r0, [r3, #0x1c]
00523e24: bl       #0x3524a0
00523e28: ldr      r1, [sp, #0xc]
00523e2c: mov      r0, r4
00523e30: bl       #0x30ebd4
00523e34: cmp      r0, #0
00523e38: add      r7, r7, #4
00523e3c: mov      r1, r6
00523e40: mov      r2, r4
00523e44: mov      r0, r8
00523e48: beq      #0x523e50
00523e4c: bl       #0x5239e8
00523e50: cmp      r7, fp
00523e54: beq      #0x523edc
00523e58: ldr      r6, [r7]
00523e5c: ldr      r3, [r6]
00523e60: mov      r0, r6
00523e64: mov      lr, pc
00523e68: ldr      pc, [r3, #0x24]
00523e6c: ldrb     r3, [r0]
00523e70: mov      r4, r0
00523e74: cmp      r3, #0
00523e78: bne      #0x523ddc
00523e7c: ldr      r0, [r7]
00523e80: bl       #0x597290
00523e84: cmp      r0, #0
00523e88: beq      #0x523ddc
00523e8c: ldr      r0, [r7]
00523e90: bl       #0x597290
00523e94: ldr      r3, [r0]
00523e98: mov      lr, pc
00523e9c: ldr      pc, [r3, #0x24]
00523ea0: mov      r4, r0
00523ea4: mov      r0, r6
00523ea8: bl       #0x597290
00523eac: mov      r6, r0
00523eb0: b        #0x523ddc
00523eb4: ldr      r1, [sp, #8]
00523eb8: ldr      r0, [sp, #0x14]
00523ebc: ldr      r3, [r5, #0x34]
00523ec0: ldr      r2, [r1, r0]
00523ec4: ldr      r3, [r3, #-4]
00523ec8: ldr      r2, [r2, #0x10]
00523ecc: ldr      r1, [r3, #0x40]
00523ed0: ldr      r0, [r2, #0x1c]
00523ed4: bl       #0x3524a0
00523ed8: b        #0x523e28
00523edc: cmp      sb, #0
00523ee0: mov      r4, r8
00523ee4: beq      #0x5240a4
00523ee8: ldr      r2, [r8, #0xc]
00523eec: ldr      r3, [r8, #8]
00523ef0: rsb      r3, r3, r2
00523ef4: asr      r3, r3, #2
00523ef8: cmp      r3, #1
00523efc: beq      #0x523fdc
00523f00: ldr      r6, [r5, #0x3c]
00523f04: ldr      r7, [r8, #0x14]
00523f08: mov      r0, r6
00523f0c: mov      r1, r7
00523f10: bl       #0x30e70c
00523f14: cmp      r0, #0
00523f18: moveq    r6, r7
00523f1c: str      r6, [r8, #0x14]
00523f20: ldr      r6, [r5, #0x40]
00523f24: ldr      r7, [r8, #0x18]
00523f28: mov      r0, r6
00523f2c: mov      r1, r7
00523f30: bl       #0x30e70c
00523f34: cmp      r0, #0
00523f38: moveq    r6, r7
00523f3c: str      r6, [r8, #0x18]
00523f40: ldr      r6, [r5, #0x44]
00523f44: ldr      r7, [r8, #0x1c]
00523f48: mov      r0, r6
00523f4c: mov      r1, r7
00523f50: bl       #0x30e70c
00523f54: cmp      r0, #0
00523f58: moveq    r6, r7
00523f5c: str      r6, [r8, #0x1c]
00523f60: ldr      r6, [r5, #0x48]
00523f64: ldr      r7, [r8, #0x20]
00523f68: mov      r1, r6
00523f6c: mov      r0, r7
00523f70: bl       #0x30e70c
00523f74: cmp      r0, #0
00523f78: moveq    r6, r7
00523f7c: str      r6, [r8, #0x20]
00523f80: ldr      r6, [r5, #0x4c]
00523f84: ldr      r7, [r8, #0x24]
00523f88: mov      r1, r6
00523f8c: mov      r0, r7
00523f90: bl       #0x30e70c
00523f94: cmp      r0, #0
00523f98: moveq    r6, r7
00523f9c: str      r6, [r8, #0x24]
00523fa0: ldr      r7, [r8, #0x28]
00523fa4: ldr      r6, [r5, #0x50]
00523fa8: mov      r0, r7
00523fac: mov      r1, r6
00523fb0: bl       #0x30e70c
00523fb4: cmp      r0, #0
00523fb8: moveq    r6, r7
00523fbc: str      r6, [r8, #0x28]
00523fc0: ldr      r0, [sp, #0x18]
00523fc4: cmp      r0, #0
00523fc8: beq      #0x523fd0
00523fcc: bl       #0x310450
00523fd0: mov      r0, r5
00523fd4: add      sp, sp, #0x2c
00523fd8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00523fdc: ldr      r3, [r5, #0x3c]
00523fe0: str      r3, [r8, #0x14]
00523fe4: ldr      r3, [r5, #0x40]
00523fe8: str      r3, [r8, #0x18]
00523fec: ldr      r3, [r5, #0x44]
00523ff0: str      r3, [r8, #0x1c]
00523ff4: ldr      r3, [r5, #0x48]
00523ff8: str      r3, [r8, #0x20]
00523ffc: ldr      r3, [r5, #0x4c]
00524000: str      r3, [r8, #0x24]
00524004: ldr      r3, [r5, #0x50]
00524008: str      r3, [r8, #0x28]
0052400c: b        #0x523fc0
00524010: ldr      r1, [sp, #8]
00524014: ldr      r0, [pc, #0x174]
00524018: ldr      r2, [pc, #0x174]
0052401c: ldr      r3, [pc, #0x174]
00524020: ldr      r0, [r1, r0]
00524024: ldr      r1, [pc, #0x170]
00524028: mov      ip, #0x5c
0052402c: add      r2, pc, r2
00524030: add      r1, pc, r1
00524034: add      r3, pc, r3
00524038: add      r0, r0, #0xa8
0052403c: str      ip, [sp]
00524040: bl       #0x30e004
00524044: b        #0x523c6c
00524048: ldr      r2, [sp, #8]
0052404c: ldr      r3, [pc, #0x12c]
00524050: ldr      r3, [r2, r3]
00524054: ldr      r3, [r3]
00524058: cmp      r3, #2
0052405c: streq    r6, [r6]
00524060: beq      #0x523c74
00524064: cmp      r3, #1
00524068: bne      #0x523c74
0052406c: ldr      r3, [sp, #8]
00524070: ldr      r0, [pc, #0x118]
00524074: ldr      r1, [pc, #0x124]
00524078: ldr      r2, [pc, #0x124]
0052407c: ldr      r0, [r3, r0]
00524080: ldr      r3, [pc, #0x120]
00524084: mov      ip, #0x62
00524088: add      r1, pc, r1
0052408c: add      r2, pc, r2
00524090: add      r3, pc, r3
00524094: add      r0, r0, #0xa8
00524098: str      ip, [sp]
0052409c: bl       #0x30e004
005240a0: b        #0x523c74
005240a4: ldr      r3, [r4, #0xc]
005240a8: cmp      r5, #0
005240ac: sub      r3, r3, #4
005240b0: str      r3, [r4, #0xc]
005240b4: beq      #0x523fc0
005240b8: mov      r0, r5
005240bc: ldr      r3, [r5]
005240c0: mov      lr, pc
005240c4: ldr      pc, [r3, #4]
005240c8: mov      r5, #0
005240cc: b        #0x523fc0
005240d0: ldr      r3, [r4, #8]
005240d4: rsb      r3, r3, r7
005240d8: asr      r3, r3, #2
005240dc: cmp      r3, #1
005240e0: addhs    r1, r3, r3
005240e4: addlo    r1, r3, #1
005240e8: cmn      r1, #0xc0000001
005240ec: bls      #0x524158
005240f0: mvn      r1, #0xc0000000
005240f4: add      r2, sp, #0x28
005240f8: str      r1, [r2, #-4]!
005240fc: add      r0, r4, #0x10
00524100: bl       #0x522c04
00524104: ldr      r1, [r4, #8]
00524108: mov      r8, r0
0052410c: subs     r7, r7, r1
00524110: moveq    r7, r0
00524114: bne      #0x52416c
00524118: str      r5, [r7], #4
0052411c: ldr      r0, [r4, #8]
00524120: ldr      r3, [r4, #0x10]
00524124: cmp      r0, #0
00524128: beq      #0x524140
0052412c: rsb      r3, r0, r3
00524130: bic      r1, r3, #3
00524134: cmp      r1, #0x80
00524138: bhi      #0x524164
0052413c: bl       #0x708f00
00524140: ldr      r3, [sp, #0x24]
00524144: str      r8, [r4, #8]
00524148: str      r7, [r4, #0xc]
0052414c: add      r8, r8, r3, lsl #2
00524150: str      r8, [r4, #0x10]
00524154: b        #0x523cc4
00524158: cmp      r3, r1
0052415c: bls      #0x5240f4
00524160: b        #0x5240f0
00524164: bl       #0x310440
00524168: b        #0x524140
0052416c: mov      r2, r7
00524170: bl       #0x30df38
00524174: add      r7, r0, r7
00524178: b        #0x524118
0052417c: subeq    r0, r7, r4, asr lr
00524180: andeq    r3, r0, r0, asr #19
00524184: strdeq   r3, r4, [r0], -r4
00524188: subeq    r2, r3, r8, lsl lr
0052418c: subeq    r2, r3, r8, ror #26
00524190: andeq    r1, r0, r0, asr #19
00524194: eorseq   r8, fp, ip, ror #20
00524198: ldrsbteq r8, [fp], -ip
0052419c: eorseq   sl, sb, r8, lsr #7
005241a0: eorseq   sl, sb, r0, asr r3
005241a4: eorseq   r8, fp, ip, asr sb
005241a8: eorseq   r8, fp, r0, lsl #19
