
# _ZN24BlendedAnimSetControllerC1EP13RootSceneNodei
00476ddc: push     {r4, r5, r6, r7, r8, sl, lr}
00476de0: mov      r5, r2
00476de4: sub      sp, sp, #0x14
00476de8: mov      r2, #1
00476dec: ldr      r7, [pc, #0x258]
00476df0: mov      r4, r0
00476df4: bl       #0x474e44
00476df8: ldr      r3, [pc, #0x250]
00476dfc: add      r7, pc, r7
00476e00: ldr      r2, [pc, #0x24c]
00476e04: ldr      r3, [r7, r3]
00476e08: mov      r8, #0
00476e0c: ldr      r6, [r7, r2]
00476e10: add      r3, r3, #8
00476e14: str      r3, [r4]
00476e18: mov      r3, #1
00476e1c: strb     r3, [r4, #0x10]
00476e20: mov      r1, r5
00476e24: str      r5, [r4, #8]
00476e28: mov      r0, r6
00476e2c: str      r8, [r4, #0xc]
00476e30: str      r8, [r4, #0x14]
00476e34: bl       #0x4762c4
00476e38: mov      r5, r0
00476e3c: ldr      r1, [r4, #8]
00476e40: mov      r0, r6
00476e44: bl       #0x4762c4
00476e48: subs     r3, r5, r8
00476e4c: movne    r3, #1
00476e50: subs     sl, r0, r8
00476e54: movne    sl, #1
00476e58: tst      sl, r3
00476e5c: mov      r6, r0
00476e60: bne      #0x476ea0
00476e64: cmp      r3, #0
00476e68: beq      #0x476e7c
00476e6c: ldr      r3, [r5]
00476e70: ldr      r0, [r3, #-0xc]
00476e74: add      r0, r5, r0
00476e78: bl       #0x31d584
00476e7c: cmp      sl, #0
00476e80: beq      #0x476e94
00476e84: ldr      r3, [r6]
00476e88: ldr      r0, [r3, #-0xc]
00476e8c: add      r0, r6, r0
00476e90: bl       #0x31d584
00476e94: mov      r0, r4
00476e98: add      sp, sp, #0x14
00476e9c: pop      {r4, r5, r6, r7, r8, sl, pc}
00476ea0: mov      r0, r5
00476ea4: bl       #0x65f11c
00476ea8: cmp      r0, r8
00476eac: ble      #0x476fd8
00476eb0: mov      r1, #0
00476eb4: mov      r0, #0xd0
00476eb8: bl       #0x5341ac
00476ebc: mov      r7, r0
00476ec0: bl       #0x367018
00476ec4: mov      r3, #1
00476ec8: str      r5, [sp, #0xc]
00476ecc: strb     r3, [r7, #0x24]
00476ed0: ldr      r3, [sp, #0xc]
00476ed4: add      r8, r7, #0x28
00476ed8: ldr      r2, [r3]
00476edc: ldr      r2, [r2, #-0xc]
00476ee0: add      r3, r3, r2
00476ee4: ldr      r2, [r3, #4]
00476ee8: add      r2, r2, #1
00476eec: str      r2, [r3, #4]
00476ef0: ldr      r1, [r7, #0x2c]
00476ef4: ldr      r3, [r7, #0x30]
00476ef8: cmp      r1, r3
00476efc: beq      #0x47702c
00476f00: ldr      r3, [sp, #0xc]
00476f04: str      r3, [r1]
00476f08: ldr      r3, [r7, #0x2c]
00476f0c: add      r3, r3, #4
00476f10: str      r3, [r7, #0x2c]
00476f14: mov      r3, #1
00476f18: str      r6, [sp, #0xc]
00476f1c: strb     r3, [r7, #0x24]
00476f20: ldr      r3, [sp, #0xc]
00476f24: ldr      r2, [r3]
00476f28: ldr      r2, [r2, #-0xc]
00476f2c: add      r3, r3, r2
00476f30: ldr      r2, [r3, #4]
00476f34: add      r2, r2, #1
00476f38: str      r2, [r3, #4]
00476f3c: ldr      r1, [r7, #0x2c]
00476f40: ldr      r3, [r7, #0x30]
00476f44: cmp      r1, r3
00476f48: beq      #0x47703c
00476f4c: ldr      r3, [sp, #0xc]
00476f50: str      r3, [r1]
00476f54: ldr      r3, [r7, #0x2c]
00476f58: add      r3, r3, #4
00476f5c: str      r3, [r7, #0x2c]
00476f60: mov      r0, r7
00476f64: ldr      r3, [r7]
00476f68: mov      r1, #0
00476f6c: mov      lr, pc
00476f70: ldr      pc, [r3, #0x88]
00476f74: ldr      r3, [r7, #0x34]
00476f78: mov      r2, #0x3f800000
00476f7c: mov      r1, r7
00476f80: str      r2, [r3]
00476f84: ldr      r3, [r7, #0x34]
00476f88: mov      r2, #0
00476f8c: str      r2, [r3, #4]
00476f90: ldr      r3, [r4, #4]
00476f94: mov      r0, r3
00476f98: ldr      r3, [r3]
00476f9c: mov      lr, pc
00476fa0: ldr      pc, [r3, #0x6c]
00476fa4: ldr      r3, [r5]
00476fa8: ldr      r0, [r3, #-0xc]
00476fac: add      r0, r5, r0
00476fb0: bl       #0x31d584
00476fb4: ldr      r3, [r6]
00476fb8: ldr      r0, [r3, #-0xc]
00476fbc: add      r0, r6, r0
00476fc0: bl       #0x31d584
00476fc4: ldr      r3, [r7]
00476fc8: ldr      r0, [r3, #-0xc]
00476fcc: add      r0, r7, r0
00476fd0: bl       #0x31d584
00476fd4: b        #0x476e94
00476fd8: ldr      r3, [pc, #0x78]
00476fdc: ldr      r3, [r7, r3]
00476fe0: ldr      r3, [r3]
00476fe4: cmp      r3, #2
00476fe8: streq    r8, [r8]
00476fec: beq      #0x476eb0
00476ff0: cmp      r3, #1
00476ff4: bne      #0x476eb0
00476ff8: ldr      r0, [pc, #0x5c]
00476ffc: ldr      r1, [pc, #0x5c]
00477000: ldr      r2, [pc, #0x5c]
00477004: ldr      r0, [r7, r0]
00477008: ldr      r3, [pc, #0x58]
0047700c: mov      ip, #0x45
00477010: add      r1, pc, r1
00477014: add      r2, pc, r2
00477018: add      r3, pc, r3
0047701c: add      r0, r0, #0xa8
00477020: str      ip, [sp]
00477024: bl       #0x30e004
00477028: b        #0x476eb0
0047702c: mov      r0, r8
00477030: add      r2, sp, #0xc
00477034: bl       #0x476a9c
00477038: b        #0x476f14
0047703c: mov      r0, r8
00477040: add      r2, sp, #0xc
00477044: bl       #0x476a9c
00477048: b        #0x476f60

# _ZN24BlendedAnimSetControllerC2EP13RootSceneNodei
00476b4c: push     {r4, r5, r6, r7, r8, sl, lr}
00476b50: mov      r5, r2
00476b54: sub      sp, sp, #0x14
00476b58: mov      r2, #1
00476b5c: ldr      r7, [pc, #0x258]
00476b60: mov      r4, r0
00476b64: bl       #0x474e44
00476b68: ldr      r3, [pc, #0x250]
00476b6c: add      r7, pc, r7
00476b70: ldr      r2, [pc, #0x24c]
00476b74: ldr      r3, [r7, r3]
00476b78: mov      r8, #0
00476b7c: ldr      r6, [r7, r2]
00476b80: add      r3, r3, #8
00476b84: str      r3, [r4]
00476b88: mov      r3, #1
00476b8c: strb     r3, [r4, #0x10]
00476b90: mov      r1, r5
00476b94: str      r5, [r4, #8]
00476b98: mov      r0, r6
00476b9c: str      r8, [r4, #0xc]
00476ba0: str      r8, [r4, #0x14]
00476ba4: bl       #0x4762c4
00476ba8: mov      r5, r0
00476bac: ldr      r1, [r4, #8]
00476bb0: mov      r0, r6
00476bb4: bl       #0x4762c4
00476bb8: subs     r3, r5, r8
00476bbc: movne    r3, #1
00476bc0: subs     sl, r0, r8
00476bc4: movne    sl, #1
00476bc8: tst      sl, r3
00476bcc: mov      r6, r0
00476bd0: bne      #0x476c10
00476bd4: cmp      r3, #0
00476bd8: beq      #0x476bec
00476bdc: ldr      r3, [r5]
00476be0: ldr      r0, [r3, #-0xc]
00476be4: add      r0, r5, r0
00476be8: bl       #0x31d584
00476bec: cmp      sl, #0
00476bf0: beq      #0x476c04
00476bf4: ldr      r3, [r6]
00476bf8: ldr      r0, [r3, #-0xc]
00476bfc: add      r0, r6, r0
00476c00: bl       #0x31d584
00476c04: mov      r0, r4
00476c08: add      sp, sp, #0x14
00476c0c: pop      {r4, r5, r6, r7, r8, sl, pc}
00476c10: mov      r0, r5
00476c14: bl       #0x65f11c
00476c18: cmp      r0, r8
00476c1c: ble      #0x476d48
00476c20: mov      r1, #0
00476c24: mov      r0, #0xd0
00476c28: bl       #0x5341ac
00476c2c: mov      r7, r0
00476c30: bl       #0x367018
00476c34: mov      r3, #1
00476c38: str      r5, [sp, #0xc]
00476c3c: strb     r3, [r7, #0x24]
00476c40: ldr      r3, [sp, #0xc]
00476c44: add      r8, r7, #0x28
00476c48: ldr      r2, [r3]
00476c4c: ldr      r2, [r2, #-0xc]
00476c50: add      r3, r3, r2
00476c54: ldr      r2, [r3, #4]
00476c58: add      r2, r2, #1
00476c5c: str      r2, [r3, #4]
00476c60: ldr      r1, [r7, #0x2c]
00476c64: ldr      r3, [r7, #0x30]
00476c68: cmp      r1, r3
00476c6c: beq      #0x476d9c
00476c70: ldr      r3, [sp, #0xc]
00476c74: str      r3, [r1]
00476c78: ldr      r3, [r7, #0x2c]
00476c7c: add      r3, r3, #4
00476c80: str      r3, [r7, #0x2c]
00476c84: mov      r3, #1
00476c88: str      r6, [sp, #0xc]
00476c8c: strb     r3, [r7, #0x24]
00476c90: ldr      r3, [sp, #0xc]
00476c94: ldr      r2, [r3]
00476c98: ldr      r2, [r2, #-0xc]
00476c9c: add      r3, r3, r2
00476ca0: ldr      r2, [r3, #4]
00476ca4: add      r2, r2, #1
00476ca8: str      r2, [r3, #4]
00476cac: ldr      r1, [r7, #0x2c]
00476cb0: ldr      r3, [r7, #0x30]
00476cb4: cmp      r1, r3
00476cb8: beq      #0x476dac
00476cbc: ldr      r3, [sp, #0xc]
00476cc0: str      r3, [r1]
00476cc4: ldr      r3, [r7, #0x2c]
00476cc8: add      r3, r3, #4
00476ccc: str      r3, [r7, #0x2c]
00476cd0: mov      r0, r7
00476cd4: ldr      r3, [r7]
00476cd8: mov      r1, #0
00476cdc: mov      lr, pc
00476ce0: ldr      pc, [r3, #0x88]
00476ce4: ldr      r3, [r7, #0x34]
00476ce8: mov      r2, #0x3f800000
00476cec: mov      r1, r7
00476cf0: str      r2, [r3]
00476cf4: ldr      r3, [r7, #0x34]
00476cf8: mov      r2, #0
00476cfc: str      r2, [r3, #4]
00476d00: ldr      r3, [r4, #4]
00476d04: mov      r0, r3
00476d08: ldr      r3, [r3]
00476d0c: mov      lr, pc
00476d10: ldr      pc, [r3, #0x6c]
00476d14: ldr      r3, [r5]
00476d18: ldr      r0, [r3, #-0xc]
00476d1c: add      r0, r5, r0
00476d20: bl       #0x31d584
00476d24: ldr      r3, [r6]
00476d28: ldr      r0, [r3, #-0xc]
00476d2c: add      r0, r6, r0
00476d30: bl       #0x31d584
00476d34: ldr      r3, [r7]
00476d38: ldr      r0, [r3, #-0xc]
00476d3c: add      r0, r7, r0
00476d40: bl       #0x31d584
00476d44: b        #0x476c04
00476d48: ldr      r3, [pc, #0x78]
00476d4c: ldr      r3, [r7, r3]
00476d50: ldr      r3, [r3]
00476d54: cmp      r3, #2
00476d58: streq    r8, [r8]
00476d5c: beq      #0x476c20
00476d60: cmp      r3, #1
00476d64: bne      #0x476c20
00476d68: ldr      r0, [pc, #0x5c]
00476d6c: ldr      r1, [pc, #0x5c]
00476d70: ldr      r2, [pc, #0x5c]
00476d74: ldr      r0, [r7, r0]
00476d78: ldr      r3, [pc, #0x58]
00476d7c: mov      ip, #0x45
00476d80: add      r1, pc, r1
00476d84: add      r2, pc, r2
00476d88: add      r3, pc, r3
00476d8c: add      r0, r0, #0xa8
00476d90: str      ip, [sp]
00476d94: bl       #0x30e004
00476d98: b        #0x476c20
00476d9c: mov      r0, r8
00476da0: add      r2, sp, #0xc
00476da4: bl       #0x476a9c
00476da8: b        #0x476c84
00476dac: mov      r0, r8
00476db0: add      r2, sp, #0xc
00476db4: bl       #0x476a9c
00476db8: b        #0x476cd0
00476dbc: subseq   sp, r1, r4, lsr #30
00476dc0: andeq    r2, r0, r8, lsr #7
00476dc4: andeq    r4, r0, r8, lsr r8
00476dc8: andeq    r3, r0, r0, asr #19
00476dcc: andeq    r1, r0, r0, asr #19
00476dd0: subeq    r7, r4, r8, asr r6
00476dd4: ldrdeq   r6, r7, [r5], #-0xac
00476dd8: strdeq   r6, r7, [r5], #-0xa8

# _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv
00667f18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00667f1c: ldr      r6, [r0, #0x10]
00667f20: ldr      sl, [pc, #0xeec]
00667f24: sub      sp, sp, #0x24
00667f28: cmp      r6, #0
00667f2c: mov      r4, r0
00667f30: add      sl, pc, sl
00667f34: beq      #0x6681e4
00667f38: ldr      r3, [r0]
00667f3c: mov      lr, pc
00667f40: ldr      pc, [r3, #0x70]
00667f44: subs     r7, r0, #0
00667f48: ble      #0x6681e4
00667f4c: ldr      r3, [pc, #0xec4]
00667f50: ldr      fp, [pc, #0xec4]
00667f54: mov      r5, #0
00667f58: add      r3, pc, r3
00667f5c: str      r3, [sp]
00667f60: ldr      r3, [pc, #0xeb8]
00667f64: add      r3, pc, r3
00667f68: str      r3, [sp, #4]
00667f6c: ldr      r3, [pc, #0xeb0]
00667f70: add      r3, pc, r3
00667f74: str      r3, [sp, #8]
00667f78: ldr      r3, [pc, #0xea8]
00667f7c: add      r3, pc, r3
00667f80: str      r3, [sp, #0xc]
00667f84: mov      r1, r5
00667f88: ldr      r3, [r4]
00667f8c: mov      r0, r4
00667f90: mov      lr, pc
00667f94: ldr      pc, [r3, #0x6c]
00667f98: ldr      r3, [r4]
00667f9c: mov      sb, r0
00667fa0: mov      r1, r5
00667fa4: mov      r0, r4
00667fa8: mov      lr, pc
00667fac: ldr      pc, [r3, #0x54]
00667fb0: ldr      r3, [r0, #8]
00667fb4: sub      r3, r3, #1
00667fb8: cmp      r3, #0x5a
00667fbc: addls    pc, pc, r3, lsl #2
00667fc0: b        #0x6681ec
00667fc4: b        #0x66820c
00667fc8: b        #0x66820c
00667fcc: b        #0x66820c
00667fd0: b        #0x66820c
00667fd4: b        #0x66820c
00667fd8: b        #0x6681ec
00667fdc: b        #0x6681ec
00667fe0: b        #0x6681ec
00667fe4: b        #0x66820c
00667fe8: b        #0x66820c
00667fec: b        #0x66820c
00667ff0: b        #0x66820c
00667ff4: b        #0x66820c
00667ff8: b        #0x668258
00667ffc: b        #0x6682ac
00668000: b        #0x6682cc
00668004: b        #0x6681ec
00668008: b        #0x6681ec
0066800c: b        #0x6681ec
00668010: b        #0x66820c
00668014: b        #0x6681ec
00668018: b        #0x6681ec
0066801c: b        #0x6681ec
00668020: b        #0x6681ec
00668024: b        #0x6681ec
00668028: b        #0x66830c
0066802c: b        #0x6681ec
00668030: b        #0x668354
00668034: b        #0x66839c
00668038: b        #0x6683e4
0066803c: b        #0x66842c
00668040: b        #0x668474
00668044: b        #0x6684bc
00668048: b        #0x668504
0066804c: b        #0x66854c
00668050: b        #0x668594
00668054: b        #0x6685dc
00668058: b        #0x668624
0066805c: b        #0x66866c
00668060: b        #0x6686b4
00668064: b        #0x6686fc
00668068: b        #0x668744
0066806c: b        #0x66878c
00668070: b        #0x6687d4
00668074: b        #0x66881c
00668078: b        #0x668864
0066807c: b        #0x6688ac
00668080: b        #0x6688f4
00668084: b        #0x66893c
00668088: b        #0x668984
0066808c: b        #0x6689cc
00668090: b        #0x668a14
00668094: b        #0x668a5c
00668098: b        #0x668aac
0066809c: b        #0x668af4
006680a0: b        #0x668b3c
006680a4: b        #0x668b8c
006680a8: b        #0x668bd4
006680ac: b        #0x668c1c
006680b0: b        #0x668c64
006680b4: b        #0x668cac
006680b8: b        #0x668cf4
006680bc: b        #0x668d3c
006680c0: b        #0x668d84
006680c4: b        #0x668dcc
006680c8: b        #0x668ecc
006680cc: b        #0x668f1c
006680d0: b        #0x668f64
006680d4: b        #0x668fac
006680d8: b        #0x668ff0
006680dc: b        #0x6681ec
006680e0: b        #0x6681ec
006680e4: b        #0x6681ec
006680e8: b        #0x6681ec
006680ec: b        #0x6681ec
006680f0: b        #0x6681ec
006680f4: b        #0x6681ec
006680f8: b        #0x6681ec
006680fc: b        #0x6681ec
00668100: b        #0x6681ec
00668104: b        #0x6681ec
00668108: b        #0x6681ec
0066810c: b        #0x6681ec
00668110: b        #0x6681ec
00668114: b        #0x6681ec
00668118: b        #0x668130
0066811c: b        #0x668130
00668120: b        #0x668130
00668124: b        #0x668130
00668128: b        #0x668130
0066812c: b        #0x668130
00668130: add      r8, sp, #0x1c
00668134: mov      r2, sb
00668138: mov      r0, r8
0066813c: mov      r1, r6
00668140: mov      r3, #0
00668144: bl       #0x65ca30
00668148: ldr      r2, [sp, #0x1c]
0066814c: cmp      r2, #0
00668150: beq      #0x6694a4
00668154: mov      r1, r5
00668158: ldr      r3, [r4]
0066815c: mov      r0, r4
00668160: mov      lr, pc
00668164: ldr      pc, [r3, #0x54]
00668168: ldr      r3, [pc, #0xcbc]
0066816c: mov      r2, #0
00668170: mvn      r1, #0
00668174: ldr      r3, [sl, r3]
00668178: str      r2, [sp, #0x14]
0066817c: str      r1, [sp, #0x18]
00668180: add      r3, r3, #8
00668184: str      r3, [sp, #0x10]
00668188: ldr      r3, [r0, #0xc]
0066818c: str      r3, [sp, #0x14]
00668190: ldr      r3, [sp, #0x1c]
00668194: ldr      r1, [r0, #0xc]
00668198: ldr      r0, [r3, #4]
0066819c: bl       #0x5d308c
006681a0: str      r0, [sp, #0x18]
006681a4: add      r3, sp, #0x10
006681a8: mov      r0, r4
006681ac: ldr      ip, [r4]
006681b0: mov      r1, r5
006681b4: ldr      r2, [sp, #0x1c]
006681b8: mov      lr, pc
006681bc: ldr      pc, [ip, #0x68]
006681c0: ldr      r3, [pc, #0xc68]
006681c4: mov      r0, r8
006681c8: ldr      r3, [sl, r3]
006681cc: add      r3, r3, #8
006681d0: str      r3, [sp, #0x10]
006681d4: bl       #0x310be8
006681d8: add      r5, r5, #1
006681dc: cmp      r5, r7
006681e0: bne      #0x667f84
006681e4: add      sp, sp, #0x24
006681e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006681ec: mov      r2, #0
006681f0: ldr      ip, [r4]
006681f4: mov      r0, r4
006681f8: mov      r1, r5
006681fc: mov      r3, r2
00668200: mov      lr, pc
00668204: ldr      pc, [ip, #0x68]
00668208: b        #0x6681d8
0066820c: mov      r1, sb
00668210: mov      r0, r6
00668214: bl       #0x5985e4
00668218: mov      r8, r0
0066821c: ldr      ip, [r4]
00668220: mov      r0, r4
00668224: mov      r1, r5
00668228: mov      r2, r8
0066822c: mov      r3, #0
00668230: mov      lr, pc
00668234: ldr      pc, [ip, #0x68]
00668238: cmp      r8, #0
0066823c: beq      #0x6681d8
00668240: mov      r0, r8
00668244: ldr      r3, [r8]
00668248: mov      r1, r4
0066824c: mov      lr, pc
00668250: ldr      pc, [r3, #0x78]
00668254: b        #0x6681d8
00668258: mov      r1, sb
0066825c: mov      r0, r6
00668260: bl       #0x65b5b0
00668264: subs     r8, r0, #0
00668268: beq      #0x669484
0066826c: mov      r1, r5
00668270: ldr      r3, [r4]
00668274: mov      r0, r4
00668278: mov      lr, pc
0066827c: ldr      pc, [r3, #0x54]
00668280: ldr      r3, [r8, #0x24]
00668284: ldrb     r2, [r0, #0xc]
00668288: ldr      ip, [r4]
0066828c: mov      r0, r4
00668290: add      r2, r3, r2, lsl #3
00668294: add      r2, r2, #0xc
00668298: mov      r1, r5
0066829c: mov      r3, #0
006682a0: mov      lr, pc
006682a4: ldr      pc, [ip, #0x68]
006682a8: b        #0x6681d8
006682ac: ldr      ip, [r4]
006682b0: mov      r0, r4
006682b4: mov      r1, r5
006682b8: mov      r2, r6
006682bc: mov      r3, #0
006682c0: mov      lr, pc
006682c4: ldr      pc, [ip, #0x68]
006682c8: b        #0x6681d8
006682cc: mov      r1, sb
006682d0: mov      r0, r6
006682d4: bl       #0x65b5f4
006682d8: subs     r2, r0, #0
006682dc: ldreq    ip, [r4]
006682e0: moveq    r0, r4
006682e4: moveq    r1, r5
006682e8: moveq    r3, r2
006682ec: ldrne    r2, [r2, #0x134]
006682f0: ldrne    ip, [r4]
006682f4: movne    r0, r4
006682f8: movne    r1, r5
006682fc: movne    r3, #0
00668300: mov      lr, pc
00668304: ldr      pc, [ip, #0x68]
00668308: b        #0x6681d8
0066830c: add      r8, sp, #0x1c
00668310: mov      r3, #0
00668314: mov      r2, sb
00668318: mov      r0, r8
0066831c: mov      r1, r6
00668320: bl       #0x65ca30
00668324: ldr      r2, [sp, #0x1c]
00668328: mov      r0, r4
0066832c: ldr      ip, [r4]
00668330: cmp      r2, #0
00668334: mov      r1, r5
00668338: moveq    r3, r2
0066833c: movne    r3, #0
00668340: mov      lr, pc
00668344: ldr      pc, [ip, #0x68]
00668348: mov      r0, r8
0066834c: bl       #0x310be8
00668350: b        #0x6681d8
00668354: mov      r1, sb
00668358: mov      r0, r6
0066835c: bl       #0x65b4e4
00668360: subs     r2, r0, #0
00668364: beq      #0x669468
00668368: ldr      r1, [pc, #0xac4]
0066836c: ldr      ip, [r4]
00668370: ldr      r3, [r2]
00668374: add      r1, pc, r1
00668378: ldr      r8, [ip, #0x68]
0066837c: mov      lr, pc
00668380: ldr      pc, [r3, #0xfc]
00668384: mov      r1, r5
00668388: mov      r2, r0
0066838c: mov      r3, #0
00668390: mov      r0, r4
00668394: blx      r8
00668398: b        #0x6681d8
0066839c: mov      r1, sb
006683a0: mov      r0, r6
006683a4: bl       #0x65b4e4
006683a8: subs     r2, r0, #0
006683ac: beq      #0x66944c
006683b0: ldr      r1, [pc, #0xa80]
006683b4: ldr      ip, [r4]
006683b8: ldr      r3, [r2]
006683bc: add      r1, pc, r1
006683c0: ldr      r8, [ip, #0x68]
006683c4: mov      lr, pc
006683c8: ldr      pc, [r3, #0xfc]
006683cc: mov      r1, r5
006683d0: mov      r2, r0
006683d4: mov      r3, #0
006683d8: mov      r0, r4
006683dc: blx      r8
006683e0: b        #0x6681d8
006683e4: mov      r1, sb
006683e8: mov      r0, r6
006683ec: bl       #0x65b4e4
006683f0: subs     r2, r0, #0
006683f4: beq      #0x669430
006683f8: ldr      r1, [pc, #0xa3c]
006683fc: ldr      ip, [r4]
00668400: ldr      r3, [r2]
00668404: add      r1, pc, r1
00668408: ldr      r8, [ip, #0x68]
0066840c: mov      lr, pc
00668410: ldr      pc, [r3, #0xfc]
00668414: mov      r1, r5
00668418: mov      r2, r0
0066841c: mov      r3, #0
00668420: mov      r0, r4
00668424: blx      r8
00668428: b        #0x6681d8
0066842c: mov      r1, sb
00668430: mov      r0, r6
00668434: bl       #0x65b4e4
00668438: subs     r2, r0, #0
0066843c: beq      #0x669414
00668440: ldr      r1, [pc, #0x9f8]
00668444: ldr      ip, [r4]
00668448: ldr      r3, [r2]
0066844c: add      r1, pc, r1
00668450: ldr      r8, [ip, #0x68]
00668454: mov      lr, pc
00668458: ldr      pc, [r3, #0xfc]
0066845c: mov      r1, r5
00668460: mov      r2, r0
00668464: mov      r3, #0
00668468: mov      r0, r4
0066846c: blx      r8
00668470: b        #0x6681d8
00668474: mov      r1, sb
00668478: mov      r0, r6
0066847c: bl       #0x65b4e4
00668480: subs     r2, r0, #0
00668484: beq      #0x6693f8
00668488: ldr      r1, [pc, #0x9b4]
0066848c: ldr      ip, [r4]
00668490: ldr      r3, [r2]
00668494: add      r1, pc, r1
00668498: ldr      r8, [ip, #0x68]
0066849c: mov      lr, pc
006684a0: ldr      pc, [r3, #0xfc]
006684a4: mov      r1, r5
006684a8: mov      r2, r0
006684ac: mov      r3, #0
006684b0: mov      r0, r4
006684b4: blx      r8
006684b8: b        #0x6681d8
006684bc: mov      r1, sb
006684c0: mov      r0, r6
006684c4: bl       #0x65b4e4
006684c8: subs     r2, r0, #0
006684cc: beq      #0x6693dc
006684d0: ldr      r1, [pc, #0x970]
006684d4: ldr      ip, [r4]
006684d8: ldr      r3, [r2]
006684dc: add      r1, pc, r1
006684e0: ldr      r8, [ip, #0x68]
006684e4: mov      lr, pc
006684e8: ldr      pc, [r3, #0xfc]
006684ec: mov      r1, r5
006684f0: mov      r2, r0
006684f4: mov      r3, #0
006684f8: mov      r0, r4
006684fc: blx      r8
00668500: b        #0x6681d8
00668504: mov      r1, sb
00668508: mov      r0, r6
0066850c: bl       #0x65b4e4
00668510: subs     r2, r0, #0
00668514: beq      #0x6693c0
00668518: ldr      r1, [pc, #0x92c]
0066851c: ldr      ip, [r4]
00668520: ldr      r3, [r2]
00668524: add      r1, pc, r1
00668528: ldr      r8, [ip, #0x68]
0066852c: mov      lr, pc
00668530: ldr      pc, [r3, #0xfc]
00668534: mov      r1, r5
00668538: mov      r2, r0
0066853c: mov      r3, #0
00668540: mov      r0, r4
00668544: blx      r8
00668548: b        #0x6681d8
0066854c: mov      r1, sb
00668550: mov      r0, r6
00668554: bl       #0x65b4e4
00668558: subs     r2, r0, #0
0066855c: beq      #0x6693a4
00668560: ldr      r1, [pc, #0x8e8]
00668564: ldr      ip, [r4]
00668568: ldr      r3, [r2]
0066856c: add      r1, pc, r1
00668570: ldr      r8, [ip, #0x68]
00668574: mov      lr, pc
00668578: ldr      pc, [r3, #0xfc]
0066857c: mov      r1, r5
00668580: mov      r2, r0
00668584: mov      r3, #0
00668588: mov      r0, r4
0066858c: blx      r8
00668590: b        #0x6681d8
00668594: mov      r1, sb
00668598: mov      r0, r6
0066859c: bl       #0x65b4e4
006685a0: subs     r2, r0, #0
006685a4: beq      #0x669388
006685a8: ldr      r1, [pc, #0x8a4]
006685ac: ldr      ip, [r4]
006685b0: ldr      r3, [r2]
006685b4: add      r1, pc, r1
006685b8: ldr      r8, [ip, #0x68]
006685bc: mov      lr, pc
006685c0: ldr      pc, [r3, #0xfc]
006685c4: mov      r1, r5
006685c8: mov      r2, r0
006685cc: mov      r3, #0
006685d0: mov      r0, r4
006685d4: blx      r8
006685d8: b        #0x6681d8
006685dc: mov      r1, sb
006685e0: mov      r0, r6
006685e4: bl       #0x65b4e4
006685e8: subs     r2, r0, #0
006685ec: beq      #0x66936c
006685f0: ldr      r1, [pc, #0x860]
006685f4: ldr      ip, [r4]
006685f8: ldr      r3, [r2]
006685fc: add      r1, pc, r1
00668600: ldr      r8, [ip, #0x68]
00668604: mov      lr, pc
00668608: ldr      pc, [r3, #0xfc]
0066860c: mov      r1, r5
00668610: mov      r2, r0
00668614: mov      r3, #0
00668618: mov      r0, r4
0066861c: blx      r8
00668620: b        #0x6681d8
00668624: mov      r1, sb
00668628: mov      r0, r6
0066862c: bl       #0x65b4e4
00668630: subs     r2, r0, #0
00668634: beq      #0x669350
00668638: ldr      r1, [pc, #0x81c]
0066863c: ldr      ip, [r4]
00668640: ldr      r3, [r2]
00668644: add      r1, pc, r1
00668648: ldr      r8, [ip, #0x68]
0066864c: mov      lr, pc
00668650: ldr      pc, [r3, #0xfc]
00668654: mov      r1, r5
00668658: mov      r2, r0
0066865c: mov      r3, #0
00668660: mov      r0, r4
00668664: blx      r8
00668668: b        #0x6681d8
0066866c: mov      r1, sb
00668670: mov      r0, r6
00668674: bl       #0x65b4e4
00668678: subs     r2, r0, #0
0066867c: beq      #0x669334
00668680: ldr      r1, [pc, #0x7d8]
00668684: ldr      ip, [r4]
00668688: ldr      r3, [r2]
0066868c: add      r1, pc, r1
00668690: ldr      r8, [ip, #0x68]
00668694: mov      lr, pc
00668698: ldr      pc, [r3, #0xfc]
0066869c: mov      r1, r5
006686a0: mov      r2, r0
006686a4: mov      r3, #0
006686a8: mov      r0, r4
006686ac: blx      r8
006686b0: b        #0x6681d8
006686b4: mov      r1, sb
006686b8: mov      r0, r6
006686bc: bl       #0x65b4e4
006686c0: subs     r2, r0, #0
006686c4: beq      #0x669318
006686c8: ldr      r1, [pc, #0x794]
006686cc: ldr      ip, [r4]
006686d0: ldr      r3, [r2]
006686d4: add      r1, pc, r1
006686d8: ldr      r8, [ip, #0x68]
006686dc: mov      lr, pc
006686e0: ldr      pc, [r3, #0xfc]
006686e4: mov      r1, r5
006686e8: mov      r2, r0
006686ec: mov      r3, #0
006686f0: mov      r0, r4
006686f4: blx      r8
006686f8: b        #0x6681d8
006686fc: mov      r1, sb
00668700: mov      r0, r6
00668704: bl       #0x65b4e4
00668708: subs     r2, r0, #0
0066870c: beq      #0x6692fc
00668710: ldr      r1, [pc, #0x750]
00668714: ldr      ip, [r4]
00668718: ldr      r3, [r2]
0066871c: add      r1, pc, r1
00668720: ldr      r8, [ip, #0x68]
00668724: mov      lr, pc
00668728: ldr      pc, [r3, #0xfc]
0066872c: mov      r1, r5
00668730: mov      r2, r0
00668734: mov      r3, #0
00668738: mov      r0, r4
0066873c: blx      r8
00668740: b        #0x6681d8
00668744: mov      r1, sb
00668748: mov      r0, r6
0066874c: bl       #0x65b4e4
00668750: subs     r2, r0, #0
00668754: beq      #0x6692e0
00668758: ldr      r1, [pc, #0x70c]
0066875c: ldr      ip, [r4]
00668760: ldr      r3, [r2]
00668764: add      r1, pc, r1
00668768: ldr      r8, [ip, #0x68]
0066876c: mov      lr, pc
00668770: ldr      pc, [r3, #0xfc]
00668774: mov      r1, r5
00668778: mov      r2, r0
0066877c: mov      r3, #0
00668780: mov      r0, r4
00668784: blx      r8
00668788: b        #0x6681d8
0066878c: mov      r1, sb
00668790: mov      r0, r6
00668794: bl       #0x65b4e4
00668798: subs     r2, r0, #0
0066879c: beq      #0x6692c4
006687a0: ldr      r1, [pc, #0x6c8]
006687a4: ldr      ip, [r4]
006687a8: ldr      r3, [r2]
006687ac: add      r1, pc, r1
006687b0: ldr      r8, [ip, #0x68]
006687b4: mov      lr, pc
006687b8: ldr      pc, [r3, #0xfc]
006687bc: mov      r1, r5
006687c0: mov      r2, r0
006687c4: mov      r3, #0
006687c8: mov      r0, r4
006687cc: blx      r8
006687d0: b        #0x6681d8
006687d4: mov      r1, sb
006687d8: mov      r0, r6
006687dc: bl       #0x65b4e4
006687e0: subs     r2, r0, #0
006687e4: beq      #0x6692a8
006687e8: ldr      r1, [pc, #0x684]
006687ec: ldr      ip, [r4]
006687f0: ldr      r3, [r2]
006687f4: add      r1, pc, r1
006687f8: ldr      r8, [ip, #0x68]
006687fc: mov      lr, pc
00668800: ldr      pc, [r3, #0xfc]
00668804: mov      r1, r5
00668808: mov      r2, r0
0066880c: mov      r3, #0
00668810: mov      r0, r4
00668814: blx      r8
00668818: b        #0x6681d8
0066881c: mov      r1, sb
00668820: mov      r0, r6
00668824: bl       #0x65b4e4
00668828: subs     r2, r0, #0
0066882c: beq      #0x66928c
00668830: ldr      r1, [pc, #0x640]
00668834: ldr      ip, [r4]
00668838: ldr      r3, [r2]
0066883c: add      r1, pc, r1
00668840: ldr      r8, [ip, #0x68]
00668844: mov      lr, pc
00668848: ldr      pc, [r3, #0xfc]
0066884c: mov      r1, r5
00668850: mov      r2, r0
00668854: mov      r3, #0
00668858: mov      r0, r4
0066885c: blx      r8
00668860: b        #0x6681d8
00668864: mov      r1, sb
00668868: mov      r0, r6
0066886c: bl       #0x65b4e4
00668870: subs     r2, r0, #0
00668874: beq      #0x669270
00668878: ldr      r1, [pc, #0x5fc]
0066887c: ldr      ip, [r4]
00668880: ldr      r3, [r2]
00668884: add      r1, pc, r1
00668888: ldr      r8, [ip, #0x68]
0066888c: mov      lr, pc
00668890: ldr      pc, [r3, #0xfc]
00668894: mov      r1, r5
00668898: mov      r2, r0
0066889c: mov      r3, #0
006688a0: mov      r0, r4
006688a4: blx      r8
006688a8: b        #0x6681d8
006688ac: mov      r1, sb
006688b0: mov      r0, r6
006688b4: bl       #0x65b4e4
006688b8: subs     r2, r0, #0
006688bc: beq      #0x669254
006688c0: ldr      r1, [pc, #0x5b8]
006688c4: ldr      ip, [r4]
006688c8: ldr      r3, [r2]
006688cc: add      r1, pc, r1
006688d0: ldr      r8, [ip, #0x68]
006688d4: mov      lr, pc
006688d8: ldr      pc, [r3, #0xfc]
006688dc: mov      r1, r5
006688e0: mov      r2, r0
006688e4: mov      r3, #0
006688e8: mov      r0, r4
006688ec: blx      r8
006688f0: b        #0x6681d8
006688f4: mov      r1, sb
006688f8: mov      r0, r6
006688fc: bl       #0x65b4e4
00668900: subs     r2, r0, #0
00668904: beq      #0x669238
00668908: ldr      r1, [pc, #0x574]
0066890c: ldr      ip, [r4]
00668910: ldr      r3, [r2]
00668914: add      r1, pc, r1
00668918: ldr      r8, [ip, #0x68]
0066891c: mov      lr, pc
00668920: ldr      pc, [r3, #0xfc]
00668924: mov      r1, r5
00668928: mov      r2, r0
0066892c: mov      r3, #0
00668930: mov      r0, r4
00668934: blx      r8
00668938: b        #0x6681d8
0066893c: mov      r1, sb
00668940: mov      r0, r6
00668944: bl       #0x65b4e4
00668948: subs     r2, r0, #0
0066894c: beq      #0x66921c
00668950: ldr      r1, [pc, #0x530]
00668954: ldr      ip, [r4]
00668958: ldr      r3, [r2]
0066895c: add      r1, pc, r1
00668960: ldr      r8, [ip, #0x68]
00668964: mov      lr, pc
00668968: ldr      pc, [r3, #0xfc]
0066896c: mov      r1, r5
00668970: mov      r2, r0
00668974: mov      r3, #0
00668978: mov      r0, r4
0066897c: blx      r8
00668980: b        #0x6681d8
00668984: mov      r1, sb
00668988: mov      r0, r6
0066898c: bl       #0x65b4e4
00668990: subs     r2, r0, #0
00668994: beq      #0x669200
00668998: ldr      r1, [pc, #0x4ec]
0066899c: ldr      ip, [r4]
006689a0: ldr      r3, [r2]
006689a4: add      r1, pc, r1
006689a8: ldr      r8, [ip, #0x68]
006689ac: mov      lr, pc
006689b0: ldr      pc, [r3, #0xfc]
006689b4: mov      r1, r5
006689b8: mov      r2, r0
006689bc: mov      r3, #0
006689c0: mov      r0, r4
006689c4: blx      r8
006689c8: b        #0x6681d8
006689cc: mov      r1, sb
006689d0: mov      r0, r6
006689d4: bl       #0x65b4e4
006689d8: subs     r2, r0, #0
006689dc: beq      #0x6691e4
006689e0: ldr      r1, [pc, #0x4a8]
006689e4: ldr      ip, [r4]
006689e8: ldr      r3, [r2]
006689ec: add      r1, pc, r1
006689f0: ldr      r8, [ip, #0x68]
006689f4: mov      lr, pc
006689f8: ldr      pc, [r3, #0xfc]
006689fc: mov      r1, r5
00668a00: mov      r2, r0
00668a04: mov      r3, #0
00668a08: mov      r0, r4
00668a0c: blx      r8
00668a10: b        #0x6681d8
00668a14: mov      r1, sb
00668a18: mov      r0, r6
00668a1c: bl       #0x65b4e4
00668a20: subs     r2, r0, #0
00668a24: beq      #0x6691c8
00668a28: ldr      r1, [pc, #0x464]
00668a2c: ldr      ip, [r4]
00668a30: ldr      r3, [r2]
00668a34: add      r1, pc, r1
00668a38: ldr      r8, [ip, #0x68]
00668a3c: mov      lr, pc
00668a40: ldr      pc, [r3, #0xfc]
00668a44: mov      r1, r5
00668a48: mov      r2, r0
00668a4c: mov      r3, #0
00668a50: mov      r0, r4
00668a54: blx      r8
00668a58: b        #0x6681d8
00668a5c: mov      r1, sb
00668a60: mov      r0, r6
00668a64: bl       #0x65b4e4
00668a68: subs     r2, r0, #0
00668a6c: beq      #0x66951c
00668a70: ldrb     r8, [r2, #0x13c]
00668a74: cmp      r8, #0
00668a78: bne      #0x6681d8
00668a7c: ldr      ip, [r4]
00668a80: ldr      r3, [r2]
00668a84: ldr      r1, [sp, #0xc]
00668a88: ldr      sb, [ip, #0x68]
00668a8c: mov      lr, pc
00668a90: ldr      pc, [r3, #0xfc]
00668a94: mov      r1, r5
00668a98: mov      r2, r0
00668a9c: mov      r3, r8
00668aa0: mov      r0, r4
00668aa4: blx      sb
00668aa8: b        #0x6681d8
00668aac: mov      r1, sb
00668ab0: mov      r0, r6
00668ab4: bl       #0x65b4e4
00668ab8: subs     r2, r0, #0
00668abc: beq      #0x669040
00668ac0: ldr      r1, [pc, #0x3d0]
00668ac4: ldr      ip, [r4]
00668ac8: ldr      r3, [r2]
00668acc: add      r1, pc, r1
00668ad0: ldr      r8, [ip, #0x68]
00668ad4: mov      lr, pc
00668ad8: ldr      pc, [r3, #0xfc]
00668adc: mov      r1, r5
00668ae0: mov      r2, r0
00668ae4: mov      r3, #0
00668ae8: mov      r0, r4
00668aec: blx      r8
00668af0: b        #0x6681d8
00668af4: mov      r1, sb
00668af8: mov      r0, r6
00668afc: bl       #0x65b4e4
00668b00: subs     r2, r0, #0
00668b04: beq      #0x6691ac
00668b08: ldr      r1, [pc, #0x38c]
00668b0c: ldr      ip, [r4]
00668b10: ldr      r3, [r2]
00668b14: add      r1, pc, r1
00668b18: ldr      r8, [ip, #0x68]
00668b1c: mov      lr, pc
00668b20: ldr      pc, [r3, #0xfc]
00668b24: mov      r1, r5
00668b28: mov      r2, r0
00668b2c: mov      r3, #0
00668b30: mov      r0, r4
00668b34: blx      r8
00668b38: b        #0x6681d8
00668b3c: mov      r1, sb
00668b40: mov      r0, r6
00668b44: bl       #0x65b4e4
00668b48: subs     r2, r0, #0
00668b4c: beq      #0x669500
00668b50: ldrb     r8, [r2, #0x13d]
00668b54: cmp      r8, #0
00668b58: bne      #0x6681d8
00668b5c: ldr      ip, [r4]
00668b60: ldr      r3, [r2]
00668b64: ldr      r1, [sp, #8]
00668b68: ldr      sb, [ip, #0x68]
00668b6c: mov      lr, pc
00668b70: ldr      pc, [r3, #0xfc]
00668b74: mov      r1, r5
00668b78: mov      r2, r0
00668b7c: mov      r3, r8
00668b80: mov      r0, r4
00668b84: blx      sb
00668b88: b        #0x6681d8
00668b8c: mov      r1, sb
00668b90: mov      r0, r6
00668b94: bl       #0x65b4e4
00668b98: subs     r2, r0, #0
00668b9c: beq      #0x6690b0
00668ba0: ldr      r1, [pc, #0x2f8]
00668ba4: ldr      ip, [r4]
00668ba8: ldr      r3, [r2]
00668bac: add      r1, pc, r1
00668bb0: ldr      r8, [ip, #0x68]
00668bb4: mov      lr, pc
00668bb8: ldr      pc, [r3, #0xfc]
00668bbc: mov      r1, r5
00668bc0: mov      r2, r0
00668bc4: mov      r3, #0
00668bc8: mov      r0, r4
00668bcc: blx      r8
00668bd0: b        #0x6681d8
00668bd4: mov      r1, sb
00668bd8: mov      r0, r6
00668bdc: bl       #0x65b4e4
00668be0: subs     r2, r0, #0
00668be4: beq      #0x6690e8
00668be8: ldr      r1, [pc, #0x2b4]
00668bec: ldr      ip, [r4]
00668bf0: ldr      r3, [r2]
00668bf4: add      r1, pc, r1
00668bf8: ldr      r8, [ip, #0x68]
00668bfc: mov      lr, pc
00668c00: ldr      pc, [r3, #0xfc]
00668c04: mov      r1, r5
00668c08: mov      r2, r0
00668c0c: mov      r3, #0
00668c10: mov      r0, r4
00668c14: blx      r8
00668c18: b        #0x6681d8
00668c1c: mov      r1, sb
00668c20: mov      r0, r6
00668c24: bl       #0x65b4e4
00668c28: subs     r2, r0, #0
00668c2c: beq      #0x6690cc
00668c30: ldr      r1, [pc, #0x270]
00668c34: ldr      ip, [r4]
00668c38: ldr      r3, [r2]
00668c3c: add      r1, pc, r1
00668c40: ldr      r8, [ip, #0x68]
00668c44: mov      lr, pc
00668c48: ldr      pc, [r3, #0xfc]
00668c4c: mov      r1, r5
00668c50: mov      r2, r0
00668c54: mov      r3, #0
00668c58: mov      r0, r4
00668c5c: blx      r8
00668c60: b        #0x6681d8
00668c64: mov      r1, sb
00668c68: mov      r0, r6
00668c6c: bl       #0x65b4e4
00668c70: subs     r2, r0, #0
00668c74: beq      #0x669158
00668c78: ldr      r1, [pc, #0x22c]
00668c7c: ldr      ip, [r4]
00668c80: ldr      r3, [r2]
00668c84: add      r1, pc, r1
00668c88: ldr      r8, [ip, #0x68]
00668c8c: mov      lr, pc
00668c90: ldr      pc, [r3, #0xfc]
00668c94: mov      r1, r5
00668c98: mov      r2, r0
00668c9c: mov      r3, #0
00668ca0: mov      r0, r4
00668ca4: blx      r8
00668ca8: b        #0x6681d8
00668cac: mov      r1, sb
00668cb0: mov      r0, r6
00668cb4: bl       #0x65b4e4
00668cb8: subs     r2, r0, #0
00668cbc: beq      #0x66913c
00668cc0: ldr      r1, [pc, #0x1e8]
00668cc4: ldr      ip, [r4]
00668cc8: ldr      r3, [r2]
00668ccc: add      r1, pc, r1
00668cd0: ldr      r8, [ip, #0x68]
00668cd4: mov      lr, pc
00668cd8: ldr      pc, [r3, #0xfc]
00668cdc: mov      r1, r5
00668ce0: mov      r2, r0
00668ce4: mov      r3, #0
00668ce8: mov      r0, r4
00668cec: blx      r8
00668cf0: b        #0x6681d8
00668cf4: mov      r1, sb
00668cf8: mov      r0, r6
00668cfc: bl       #0x65b4e4
00668d00: subs     r2, r0, #0
00668d04: beq      #0x669120
00668d08: ldr      r1, [pc, #0x1a4]
00668d0c: ldr      ip, [r4]
00668d10: ldr      r3, [r2]
00668d14: add      r1, pc, r1
00668d18: ldr      r8, [ip, #0x68]
00668d1c: mov      lr, pc
00668d20: ldr      pc, [r3, #0xfc]
00668d24: mov      r1, r5
00668d28: mov      r2, r0
00668d2c: mov      r3, #0
00668d30: mov      r0, r4
00668d34: blx      r8
00668d38: b        #0x6681d8
00668d3c: mov      r1, sb
00668d40: mov      r0, r6
00668d44: bl       #0x65b4e4
00668d48: subs     r2, r0, #0
00668d4c: beq      #0x669104
00668d50: ldr      r1, [pc, #0x160]
00668d54: ldr      ip, [r4]
00668d58: ldr      r3, [r2]
00668d5c: add      r1, pc, r1
00668d60: ldr      r8, [ip, #0x68]
00668d64: mov      lr, pc
00668d68: ldr      pc, [r3, #0xfc]
00668d6c: mov      r1, r5
00668d70: mov      r2, r0
00668d74: mov      r3, #0
00668d78: mov      r0, r4
00668d7c: blx      r8
00668d80: b        #0x6681d8
00668d84: mov      r1, sb
00668d88: mov      r0, r6
00668d8c: bl       #0x65b4e4
00668d90: subs     r2, r0, #0
00668d94: beq      #0x669190
00668d98: ldr      r1, [pc, #0x11c]
00668d9c: ldr      ip, [r4]
00668da0: ldr      r3, [r2]
00668da4: add      r1, pc, r1
00668da8: ldr      r8, [ip, #0x68]
00668dac: mov      lr, pc
00668db0: ldr      pc, [r3, #0xfc]
00668db4: mov      r1, r5
00668db8: mov      r2, r0
00668dbc: mov      r3, #0
00668dc0: mov      r0, r4
00668dc4: blx      r8
00668dc8: b        #0x6681d8
00668dcc: mov      r1, sb
00668dd0: mov      r0, r6
00668dd4: bl       #0x65b4e4
00668dd8: subs     r2, r0, #0
00668ddc: beq      #0x669174
00668de0: ldr      r1, [pc, #0xd8]
00668de4: ldr      ip, [r4]
00668de8: ldr      r3, [r2]
00668dec: add      r1, pc, r1
00668df0: ldr      r8, [ip, #0x68]
00668df4: mov      lr, pc
00668df8: ldr      pc, [r3, #0xfc]
00668dfc: mov      r1, r5
00668e00: mov      r2, r0
00668e04: mov      r3, #0
00668e08: mov      r0, r4
00668e0c: blx      r8
00668e10: b        #0x6681d8
00668e14: eorseq   ip, r2, r0, ror #22
00668e18: eoreq    sp, r7, r8, ror r2
00668e1c: mlaeq    r7, r8, r2, ip
00668e20: eoreq    sp, r7, r4, asr #4
00668e24: eoreq    sp, r7, r8, lsl r2
00668e28: eoreq    sp, r7, r4, ror #3
00668e2c: andeq    r0, r0, r0, lsl #20
00668e30: andeq    r1, r0, ip, asr r2
00668e34: eoreq    sp, r7, r4, asr #32
00668e38: eoreq    sp, r7, ip, lsr r0
00668e3c: eoreq    sp, r7, r4
00668e40: eoreq    r0, r7, r4, lsr #31
00668e44: eoreq    ip, r7, ip, lsl lr
00668e48: eoreq    ip, r7, ip, asr #26
00668e4c: eoreq    ip, r7, r4, lsl sp
00668e50: eoreq    ip, r7, r4, asr sp
00668e54: eoreq    ip, r7, ip, lsl sp
00668e58: eoreq    ip, r7, r4, ror #25
00668e5c: eoreq    ip, r7, ip, lsr #25
00668e60: eoreq    ip, r7, ip, ror ip
00668e64: eoreq    ip, r7, r4, asr #24
00668e68: eoreq    ip, r7, ip, lsr #26
