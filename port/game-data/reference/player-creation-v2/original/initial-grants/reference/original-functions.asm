
# _ZN6CharAI6OnInitEv
003d12b0: push     {r4, r5, r6, r7, lr}
003d12b4: ldr      r3, [r0, #4]
003d12b8: sub      sp, sp, #0xc
003d12bc: mov      r4, r0
003d12c0: mov      r0, r3
003d12c4: ldr      r3, [r3]
003d12c8: mov      lr, pc
003d12cc: ldr      pc, [r3, #0x34]
003d12d0: ldr      r5, [pc, #0xe8]
003d12d4: cmp      r0, #0
003d12d8: add      r5, pc, r5
003d12dc: bne      #0x3d139c
003d12e0: ldr      r1, [r4, #0x10]
003d12e4: cmn      r1, #1
003d12e8: beq      #0x3d12f8
003d12ec: ldr      r0, [r4, #4]
003d12f0: add      r0, r0, #0x3b4
003d12f4: bl       #0x3db2d8
003d12f8: ldr      r6, [pc, #0xc4]
003d12fc: ldr      r1, [pc, #0xc4]
003d1300: ldr      r2, [pc, #0xc4]
003d1304: ldr      r3, [r5, r6]
003d1308: add      r1, pc, r1
003d130c: add      r2, pc, r2
003d1310: ldr      r0, [r3, #0x2c]
003d1314: ldr      r7, [r4, #4]
003d1318: bl       #0x4c4bdc
003d131c: add      r7, r7, #0x3b4
003d1320: mov      r1, r0
003d1324: mov      ip, #0
003d1328: mov      r0, r7
003d132c: mvn      r2, #0
003d1330: mov      r3, #0x33
003d1334: str      ip, [sp]
003d1338: bl       #0x3dbe24
003d133c: ldr      r1, [r4, #0x14]
003d1340: str      r0, [r4, #0x10]
003d1344: cmn      r1, #1
003d1348: beq      #0x3d1358
003d134c: ldr      r0, [r4, #4]
003d1350: add      r0, r0, #0x3b4
003d1354: bl       #0x3db2d8
003d1358: ldr      r3, [r5, r6]
003d135c: ldr      r1, [pc, #0x6c]
003d1360: ldr      r2, [pc, #0x6c]
003d1364: ldr      r0, [r3, #0x2c]
003d1368: add      r1, pc, r1
003d136c: add      r2, pc, r2
003d1370: ldr      r5, [r4, #4]
003d1374: bl       #0x4c4bdc
003d1378: add      r5, r5, #0x3b4
003d137c: mov      r1, r0
003d1380: mov      ip, #0
003d1384: mov      r0, r5
003d1388: mvn      r2, #0
003d138c: mov      r3, #0x34
003d1390: str      ip, [sp]
003d1394: bl       #0x3dbe24
003d1398: str      r0, [r4, #0x14]
003d139c: ldr      r3, [r4, #0x20]
003d13a0: cmp      r3, #0
003d13a4: beq      #0x3d13b8
003d13a8: mov      r0, r3
003d13ac: ldr      r3, [r3]
003d13b0: mov      lr, pc
003d13b4: ldr      pc, [r3, #8]
003d13b8: add      sp, sp, #0xc
003d13bc: pop      {r4, r5, r6, r7, pc}
003d13c0: ldrheq   r3, [ip], #-0x78
003d13c4: strdeq   r3, r4, [r0], -r4
003d13c8: subeq    r0, pc, r8, asr #8
003d13cc: subeq    r4, pc, ip, ror #2
003d13d0: subeq    r0, pc, r8, ror #7
003d13d4: subeq    r4, pc, r4, lsl r1

# _ZN14PlayerSavegame14SG_SetSaveDateEv
00467744: push     {r4, lr}
00467748: mov      r4, r0
0046774c: mov      r0, #0
00467750: bl       #0x30e580
00467754: str      r0, [r4, #0x38]
00467758: pop      {r4, pc}

# _ZN9Character8InitPostEv
003b4d60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr      r5, [pc, #0x828]
003b4d68: ldr      r6, [pc, #0x828]
003b4d6c: movw     r3, #0x1394
003b4d70: add      r5, pc, r5
003b4d74: ldr      r2, [r5, r6]
003b4d78: ldrb     r7, [r0, r3]
003b4d7c: sub      sp, sp, #0xfc
003b4d80: ldr      r2, [r2]
003b4d84: cmp      r7, #0
003b4d88: mov      r4, r0
003b4d8c: str      r2, [sp, #0xf4]
003b4d90: beq      #0x3b4db0
003b4d94: ldr      r3, [r5, r6]
003b4d98: ldr      r2, [sp, #0xf4]
003b4d9c: ldr      r3, [r3]
003b4da0: cmp      r2, r3
003b4da4: bne      #0x3b5590
003b4da8: add      sp, sp, #0xfc
003b4dac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov      r2, #1
003b4db4: strb     r2, [r0, r3]
003b4db8: bl       #0x38bd64
003b4dbc: ldr      r3, [r4, #0x274]
003b4dc0: cmp      r0, r3
003b4dc4: bge      #0x3b4d94
003b4dc8: ldr      fp, [pc, #0x7cc]
003b4dcc: add      r8, sp, #0xdc
003b4dd0: ldr      sl, [r5, fp]
003b4dd4: mov      r0, sl
003b4dd8: bl       #0x337888
003b4ddc: ldr      r1, [pc, #0x7bc]
003b4de0: add      r2, sp, #0x48
003b4de4: mov      r0, r8
003b4de8: add      r1, pc, r1
003b4dec: bl       #0x3140ec
003b4df0: mov      r1, r8
003b4df4: mov      r0, sl
003b4df8: bl       #0x337a88
003b4dfc: mov      r0, r8
003b4e00: bl       #0x318254
003b4e04: movw     r3, #0x13fc
003b4e08: ldr      r2, [r4, r3]
003b4e0c: movw     r3, #0x13f8
003b4e10: ldr      r3, [r4, r3]
003b4e14: cmp      r3, r2
003b4e18: beq      #0x3b4e6c
003b4e1c: ldr      r1, [pc, #0x780]
003b4e20: add      r8, sp, #0x24
003b4e24: ldr      r3, [r4, #0x64]
003b4e28: ldr      r1, [r5, r1]
003b4e2c: mov      r0, r8
003b4e30: ldr      r1, [r1, #0x38]
003b4e34: str      r7, [sp]
003b4e38: str      r7, [sp, #4]
003b4e3c: bl       #0x34aca0
003b4e40: mov      r0, r8
003b4e44: mov      r1, r7
003b4e48: bl       #0x33fdc0
003b4e4c: cmp      r0, #0
003b4e50: beq      #0x3b4e6c
003b4e54: mov      r0, r8
003b4e58: bl       #0x33fee4
003b4e5c: subs     r1, r0, #0
003b4e60: beq      #0x3b4e6c
003b4e64: ldr      r0, [r4, #0x378]
003b4e68: bl       #0x405540
003b4e6c: mov      r0, r4
003b4e70: movw     r7, #0x13c8
003b4e74: bl       #0x3b3d38
003b4e78: ldrsh    r3, [r4, r7]
003b4e7c: cmn      r3, #1
003b4e80: movweq   r1, #0xffff
003b4e84: beq      #0x3b4ec4
003b4e88: ldr      sl, [r5, fp]
003b4e8c: add      r8, sp, #0xc4
003b4e90: mov      r0, sl
003b4e94: bl       #0x337888
003b4e98: ldr      r1, [pc, #0x708]
003b4e9c: add      r2, sp, #0x44
003b4ea0: mov      r0, r8
003b4ea4: add      r1, pc, r1
003b4ea8: bl       #0x3140ec
003b4eac: mov      r1, r8
003b4eb0: mov      r0, sl
003b4eb4: bl       #0x337a88
003b4eb8: mov      r0, r8
003b4ebc: bl       #0x318254
003b4ec0: ldrh     r1, [r4, r7]
003b4ec4: add      r7, r4, #0x560
003b4ec8: sxth     r1, r1
003b4ecc: mov      r0, r7
003b4ed0: bl       #0x3df2a4
003b4ed4: mov      r0, r7
003b4ed8: mov      r1, #1
003b4edc: bl       #0x3e0810
003b4ee0: mov      r0, r4
003b4ee4: bl       #0x3a54d4
003b4ee8: subs     r8, r0, #0
003b4eec: beq      #0x3b4f04
003b4ef0: bl       #0x30de54
003b4ef4: mov      r1, r8
003b4ef8: add      r2, r8, r0
003b4efc: add      r0, r4, #0x290
003b4f00: bl       #0x3109e0
003b4f04: ldr      sl, [r5, fp]
003b4f08: add      r8, sp, #0xac
003b4f0c: mov      r0, sl
003b4f10: bl       #0x337888
003b4f14: ldr      r1, [pc, #0x690]
003b4f18: add      r2, sp, #0x40
003b4f1c: mov      r0, r8
003b4f20: add      r1, pc, r1
003b4f24: bl       #0x3140ec
003b4f28: mov      r1, r8
003b4f2c: mov      r0, sl
003b4f30: bl       #0x337a88
003b4f34: mov      r0, r8
003b4f38: bl       #0x318254
003b4f3c: ldr      r0, [r4, #0x59c]
003b4f40: bl       #0x30e964
003b4f44: movw     r1, #0x74bc
003b4f48: movt     r1, #0x3c13
003b4f4c: bl       #0x30ed6c
003b4f50: str      r0, [r4, #0x120]
003b4f54: ldr      r0, [r4, #0x5a0]
003b4f58: bl       #0x30e964
003b4f5c: movw     r1, #0x74bc
003b4f60: movt     r1, #0x3c13
003b4f64: bl       #0x30ed6c
003b4f68: str      r0, [r4, #0x124]
003b4f6c: ldr      r0, [r4, #0x5a4]
003b4f70: bl       #0x30e964
003b4f74: movw     r1, #0xd70a
003b4f78: movt     r1, #0x3c23
003b4f7c: bl       #0x30ed6c
003b4f80: str      r0, [r4, #0x128]
003b4f84: mov      r0, r4
003b4f88: bl       #0x38be5c
003b4f8c: mov      r0, r4
003b4f90: bl       #0x38ab60
003b4f94: subs     r1, r0, #0
003b4f98: beq      #0x3b5488
003b4f9c: mov      r1, #2
003b4fa0: mov      r0, r4
003b4fa4: bl       #0x3bc4d0
003b4fa8: ldr      r3, [pc, #0x600]
003b4fac: mov      r0, r4
003b4fb0: ldr      r3, [r5, r3]
003b4fb4: ldr      r8, [r3]
003b4fb8: bl       #0x3a2fec
003b4fbc: mov      r3, #0x44
003b4fc0: mla      r8, r3, r0, r8
003b4fc4: ldr      r3, [r4]
003b4fc8: mov      r0, r4
003b4fcc: mov      lr, pc
003b4fd0: ldr      pc, [r3, #0x28]
003b4fd4: cmp      r0, #0
003b4fd8: bne      #0x3b4ff4
003b4fdc: ldrb     r3, [r8, #0x10]
003b4fe0: cmp      r3, #0
003b4fe4: beq      #0x3b4ff4
003b4fe8: mov      r3, #1
003b4fec: strb     r3, [r4, #0x3ec]
003b4ff0: b        #0x3b5014
003b4ff4: ldr      r3, [r4]
003b4ff8: mov      r0, r4
003b4ffc: mov      lr, pc
003b5000: ldr      pc, [r3, #0x28]
003b5004: cmp      r0, #0
003b5008: bne      #0x3b54a4
003b500c: add      r0, r4, #0x3c8
003b5010: bl       #0x3cf1f0
003b5014: movw     r3, #0x1488
003b5018: ldr      r2, [pc, #0x594]
003b501c: ldr      r1, [r4, r3]
003b5020: ldr      r3, [r8, #0x30]
003b5024: str      r2, [sp, #0xc]
003b5028: mov      r2, r4
003b502c: add      r1, r1, r3
003b5030: ldr      r3, [sp, #0xc]
003b5034: ldr      sb, [pc, #0x57c]
003b5038: add      sl, sp, #0x94
003b503c: ldr      r0, [r5, r3]
003b5040: bl       #0x495430
003b5044: movw     r3, #0x1484
003b5048: str      r0, [r4, r3]
003b504c: mov      r0, r4
003b5050: bl       #0x3b4738
003b5054: ldr      r8, [r5, fp]
003b5058: add      sb, pc, sb
003b505c: mov      r0, r8
003b5060: bl       #0x337888
003b5064: add      r2, sp, #0x3c
003b5068: mov      r0, sl
003b506c: mov      r1, sb
003b5070: bl       #0x3140ec
003b5074: mov      r1, sl
003b5078: mov      r0, r8
003b507c: bl       #0x337a88
003b5080: mov      r0, sl
003b5084: bl       #0x318254
003b5088: add      r0, r4, #0x490
003b508c: add      r0, r0, #0xc
003b5090: bl       #0x3c9f4c
003b5094: add      sl, sp, #0x7c
003b5098: mov      r0, r8
003b509c: bl       #0x337888
003b50a0: add      r2, sp, #0x38
003b50a4: mov      r0, sl
003b50a8: mov      r1, sb
003b50ac: bl       #0x3140ec
003b50b0: mov      r1, sl
003b50b4: mov      r0, r8
003b50b8: bl       #0x337a88
003b50bc: mov      r0, sl
003b50c0: bl       #0x318254
003b50c4: mov      r0, r4
003b50c8: bl       #0x3b3b00
003b50cc: add      sl, sp, #0x64
003b50d0: mov      r0, r8
003b50d4: bl       #0x337888
003b50d8: add      r2, sp, #0x34
003b50dc: mov      r1, sb
003b50e0: mov      r0, sl
003b50e4: bl       #0x3140ec
003b50e8: mov      r1, sl
003b50ec: mov      r0, r8
003b50f0: bl       #0x337a88
003b50f4: mov      r0, sl
003b50f8: bl       #0x318254
003b50fc: ldr      r3, [r4]
003b5100: mov      r0, r4
003b5104: mov      lr, pc
003b5108: ldr      pc, [r3, #0x28]
003b510c: cmp      r0, #0
003b5110: beq      #0x3b538c
003b5114: ldr      r3, [r4, #0x2d8]
003b5118: cmp      r3, #0
003b511c: beq      #0x3b512c
003b5120: ldr      r0, [r3, #8]
003b5124: mov      r1, #0
003b5128: bl       #0x59719c
003b512c: ldr      r2, [pc, #0x470]
003b5130: mov      r0, r4
003b5134: mov      r1, #4
003b5138: str      r2, [sp, #0x10]
003b513c: bl       #0x3bc4d0
003b5140: ldr      r3, [sp, #0x10]
003b5144: ldr      r0, [r5, r3]
003b5148: bl       #0x31f594
003b514c: cmp      r0, #0
003b5150: beq      #0x3b5160
003b5154: ldr      r1, [r0, #0x118]
003b5158: mov      r0, r4
003b515c: bl       #0x3bb950
003b5160: ldr      r2, [sp, #0x10]
003b5164: mov      r1, r4
003b5168: ldr      r3, [r5, r2]
003b516c: ldr      r0, [r3, #0x40]
003b5170: bl       #0x36effc
003b5174: cmp      r0, #0
003b5178: bne      #0x3b54dc
003b517c: movw     r3, #0x13c8
003b5180: ldrsh    r1, [r4, r3]
003b5184: mov      r0, r7
003b5188: bl       #0x3df2a4
003b518c: mov      r0, r7
003b5190: bl       #0x3df480
003b5194: mov      r0, r7
003b5198: mov      r1, #1
003b519c: bl       #0x3e0810
003b51a0: ldr      r2, [sp, #0x10]
003b51a4: mov      r1, r4
003b51a8: ldr      r3, [r5, r2]
003b51ac: ldr      r0, [r3, #0x40]
003b51b0: bl       #0x36effc
003b51b4: cmp      r0, #0
003b51b8: bne      #0x3b54d0
003b51bc: movw     r3, #0xc9ff
003b51c0: movt     r3, #0x3b9a
003b51c4: str      r3, [r4, #0x3a4]
003b51c8: mov      r2, #0
003b51cc: mov      r0, r7
003b51d0: mov      r1, #0xc2
003b51d4: bl       #0x3df6e0
003b51d8: ldr      r2, [sp, #0xc]
003b51dc: bic      r0, r0, r0, asr #31
003b51e0: strb     r0, [r4, #0x3a8]
003b51e4: ldr      r3, [r5, r2]
003b51e8: ldr      r2, [r3, #0x1c]
003b51ec: ldr      r3, [r3, #0x20]
003b51f0: rsb      r3, r2, r3
003b51f4: asr      r3, r3, #3
003b51f8: add      r2, r3, r3, lsl #2
003b51fc: add      r2, r2, r2, lsl #4
003b5200: add      r2, r2, r2, lsl #8
003b5204: add      r2, r2, r2, lsl #16
003b5208: add      r3, r3, r2, lsl #1
003b520c: cmp      r3, #0
003b5210: beq      #0x3b538c
003b5214: mov      r0, #0x24
003b5218: mov      r1, #0
003b521c: bl       #0x31056c
003b5220: movw     r3, #0x1494
003b5224: str      r0, [r4, r3]
003b5228: ldr      r3, [pc, #0x38c]
003b522c: mov      sb, r0
003b5230: ldr      r3, [r5, r3]
003b5234: ldr      sl, [r3]
003b5238: cmp      sl, #0
003b523c: beq      #0x3b550c
003b5240: ldr      r3, [pc, #0x378]
003b5244: ldr      r2, [pc, #0x378]
003b5248: str      r7, [sp, #0x18]
003b524c: ldr      r3, [r5, r3]
003b5250: add      r2, pc, r2
003b5254: mov      r8, #0
003b5258: ldr      r3, [r3]
003b525c: str      r0, [sp, #0x14]
003b5260: mov      sb, r2
003b5264: mov      r7, r3
003b5268: b        #0x3b5278
003b526c: add      r8, r8, #1
003b5270: cmp      r8, sl
003b5274: beq      #0x3b5504
003b5278: mov      r0, sb
003b527c: ldr      r1, [r7, r8, lsl #2]
003b5280: bl       #0x30e31c
003b5284: cmp      r0, #0
003b5288: bne      #0x3b526c
003b528c: ldr      sb, [sp, #0x14]
003b5290: ldr      r7, [sp, #0x18]
003b5294: mov      r1, r8
003b5298: ldr      r3, [sp, #0xc]
003b529c: str      r7, [sp, #0x14]
003b52a0: str      fp, [sp, #0x18]
003b52a4: ldr      r2, [r5, r3]
003b52a8: ldr      r3, [pc, #0x318]
003b52ac: str      r6, [sp, #0x1c]
003b52b0: mov      r8, #0
003b52b4: movw     sl, #0x1494
003b52b8: mov      r7, r1
003b52bc: mov      r6, r2
003b52c0: mov      fp, r3
003b52c4: b        #0x3b52cc
003b52c8: ldr      sb, [r4, sl]
003b52cc: mov      r0, r6
003b52d0: add      r1, r7, r8
003b52d4: mov      r2, #0
003b52d8: bl       #0x495430
003b52dc: str      r0, [sb, r8, lsl #2]
003b52e0: ldr      r3, [r4, sl]
003b52e4: ldr      r3, [r3, r8, lsl #2]
003b52e8: cmp      r3, #0
003b52ec: beq      #0x3b5350
003b52f0: ldr      r2, [r5, fp]
003b52f4: mov      r0, r3
003b52f8: mov      r1, #0
003b52fc: ldr      lr, [r2]
003b5300: ldr      ip, [r2, #4]
003b5304: ldr      r2, [r2, #8]
003b5308: str      lr, [r3, #0x34]
003b530c: str      ip, [r3, #0x38]
003b5310: str      r2, [r3, #0x3c]
003b5314: bl       #0x492aa0
003b5318: ldr      r3, [r4, sl]
003b531c: mov      r1, #0
003b5320: ldr      r0, [r3, r8, lsl #2]
003b5324: bl       #0x492ef0
003b5328: ldr      r3, [r4, sl]
003b532c: ldr      r0, [r3, r8, lsl #2]
003b5330: bl       #0x49267c
003b5334: ldr      r3, [r0]
003b5338: mov      lr, pc
003b533c: ldr      pc, [r3, #0x44]
003b5340: mov      r1, #1
003b5344: ldr      r3, [r0]
003b5348: mov      lr, pc
003b534c: ldr      pc, [r3, #0x40]
003b5350: add      r8, r8, #1
003b5354: cmp      r8, #9
003b5358: bne      #0x3b52c8
003b535c: ldr      r2, [sp, #0x10]
003b5360: mov      r1, r4
003b5364: ldr      r7, [sp, #0x14]
003b5368: ldr      r3, [r5, r2]
003b536c: ldr      fp, [sp, #0x18]
003b5370: ldr      r6, [sp, #0x1c]
003b5374: ldr      r0, [r3, #0x40]
003b5378: bl       #0x36effc
003b537c: cmp      r0, #0
003b5380: bne      #0x3b5514
003b5384: mov      r0, r4
003b5388: bl       #0x3a41a0
003b538c: add      r8, r4, #0xff0
003b5390: add      r8, r8, #4
003b5394: mov      r1, r8
003b5398: mov      r2, #0xd2
003b539c: mov      r0, r7
003b53a0: bl       #0x3dedb4
003b53a4: bl       #0x30e964
003b53a8: mov      r3, #0x1440
003b53ac: str      r0, [r4, r3]
003b53b0: mov      r2, #0xd3
003b53b4: mov      r1, r8
003b53b8: mov      r0, r7
003b53bc: bl       #0x3dedb4
003b53c0: bl       #0x30e964
003b53c4: movw     r3, #0x1444
003b53c8: str      r0, [r4, r3]
003b53cc: add      r1, r4, #0x160
003b53d0: mov      r0, r4
003b53d4: bl       #0x3a58f4
003b53d8: add      r1, r4, #0x1440
003b53dc: mov      r0, r4
003b53e0: add      r1, r1, #0x10
003b53e4: mov      r2, #1
003b53e8: bl       #0x393db4
003b53ec: ldr      ip, [r4, #0x16c]
003b53f0: ldr      r0, [r4, #0x2d8]
003b53f4: ldr      r1, [r4, #0x170]
003b53f8: ldr      r2, [r4, #0x174]
003b53fc: movw     r3, #0x145c
003b5400: str      ip, [r4, r3]
003b5404: movw     r3, #0x1460
003b5408: str      r1, [r4, r3]
003b540c: cmp      r0, #0
003b5410: movw     r3, #0x1464
003b5414: str      r2, [r4, r3]
003b5418: beq      #0x3b5420
003b541c: bl       #0x470a54
003b5420: mov      r1, #0
003b5424: mov      r2, #1
003b5428: mov      r0, r4
003b542c: bl       #0x3a59ac
003b5430: mov      r0, r4
003b5434: bl       #0x3b3a70
003b5438: ldrb     r1, [r4, #0x3ec]
003b543c: cmp      r1, #0
003b5440: beq      #0x3b54c4
003b5444: mov      r0, r4
003b5448: bl       #0x3d37d0
003b544c: ldr      r7, [r5, fp]
003b5450: add      r4, sp, #0x4c
003b5454: mov      r0, r7
003b5458: bl       #0x337888
003b545c: ldr      r1, [pc, #0x168]
003b5460: add      r2, sp, #0x30
003b5464: mov      r0, r4
003b5468: add      r1, pc, r1
003b546c: bl       #0x3140ec
003b5470: mov      r0, r7
003b5474: mov      r1, r4
003b5478: bl       #0x337a88
003b547c: mov      r0, r4
003b5480: bl       #0x318254
003b5484: b        #0x3b4d94
003b5488: mov      r0, r4
003b548c: ldr      r3, [r4]
003b5490: mov      lr, pc
003b5494: ldr      pc, [r3, #0x40]
003b5498: mov      r0, r4
003b549c: bl       #0x33ddb4
003b54a0: b        #0x3b4d94
003b54a4: ldr      r3, [pc, #0xf8]
003b54a8: mov      r1, r4
003b54ac: ldr      r3, [r5, r3]
003b54b0: ldr      r0, [r3, #0x40]
003b54b4: bl       #0x36effc
003b54b8: cmp      r0, #0
003b54bc: bne      #0x3b500c
003b54c0: b        #0x3b4fe8
003b54c4: add      r0, r4, #0x3c8
003b54c8: bl       #0x3ce7c0
003b54cc: b        #0x3b5444
003b54d0: mov      r0, r4
003b54d4: bl       #0x3b3a90
003b54d8: b        #0x3b51bc
003b54dc: mov      r0, r4
003b54e0: bl       #0x3b395c
003b54e4: mov      r0, r7
003b54e8: bl       #0x3defac
003b54ec: movw     r3, #0x14e8
003b54f0: ldr      r0, [r4, r3]
003b54f4: cmp      r0, #0
003b54f8: beq      #0x3b517c
003b54fc: bl       #0x4679e8
003b5500: b        #0x3b517c
003b5504: ldr      sb, [sp, #0x14]
003b5508: ldr      r7, [sp, #0x18]
003b550c: mvn      r1, #0
003b5510: b        #0x3b5298
003b5514: ldr      r3, [sp, #0xc]
003b5518: mov      r2, #0
003b551c: movw     r8, #0x149c
003b5520: ldr      r0, [r5, r3]
003b5524: ldr      r3, [pc, #0xa4]
003b5528: ldr      r3, [r5, r3]
003b552c: ldr      r3, [r3]
003b5530: ldr      r1, [r3, #0x88]
003b5534: bl       #0x495430
003b5538: cmp      r0, #0
003b553c: str      r0, [r4, r8]
003b5540: beq      #0x3b5384
003b5544: mov      r2, #0
003b5548: str      r2, [r0, #0x3c]
003b554c: str      r2, [r0, #0x34]
003b5550: str      r2, [r0, #0x38]
003b5554: mov      r1, #0
003b5558: bl       #0x492aa0
003b555c: mov      r1, #0
003b5560: ldr      r0, [r4, r8]
003b5564: bl       #0x492ef0
003b5568: ldr      r0, [r4, r8]
003b556c: bl       #0x49267c
003b5570: ldr      r3, [r0]
003b5574: mov      lr, pc
003b5578: ldr      pc, [r3, #0x44]
003b557c: mov      r1, #1
003b5580: ldr      r3, [r0]
003b5584: mov      lr, pc
003b5588: ldr      pc, [r3, #0x40]
003b558c: b        #0x3b5384
003b5590: bl       #0x30e310
003b5594: subseq   pc, sp, r0, lsr #26
003b5598: andeq    r4, r0, ip, lsr #1
003b559c: andeq    r0, r0, r4, lsl #17
003b55a0: subseq   pc, r0, r8, lsr #32
003b55a4: strdeq   r3, r4, [r0], -r4
003b55a8: subseq   lr, r0, ip, ror #30
003b55ac: ldrsheq  lr, [r0], #-0xe0
003b55b0: andeq    r0, r0, r8, asr r7
003b55b4: andeq    r1, r0, r8, lsl #22
003b55b8: ldrheq   lr, [r0], #-0xd8
003b55bc: andeq    r0, r0, r4, asr #13
003b55c0: muleq    r0, r4, r2
003b55c4: subseq   lr, r0, r0, ror #24
003b55c8: andeq    r3, r0, ip, lsr #30
003b55cc: subseq   lr, r0, r8, lsr #19
003b55d0: andeq    r3, r0, r8, asr #5

# _ZN9Character7SG_LoadEi
003bc4d0: movw     r3, #0x14e8
003bc4d4: ldr      r0, [r0, r3]
003bc4d8: cmp      r0, #0
003bc4dc: bxeq     lr
003bc4e0: b        #0x465430

# _ZN9Character8IncSkillEib
003bcc58: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bcc5c: ldr      r4, [pc, #0x2e8]
003bcc60: ldr      r6, [pc, #0x2e8]
003bcc64: mov      r5, r0
003bcc68: add      r4, pc, r4
003bcc6c: ldr      r0, [r4, r6]
003bcc70: movw     r3, #0x14e8
003bcc74: ldr      r3, [r5, r3]
003bcc78: ldr      r0, [r0]
003bcc7c: sub      sp, sp, #0x64
003bcc80: cmp      r3, #0
003bcc84: mov      r7, r1
003bcc88: str      r0, [sp, #0x5c]
003bcc8c: mov      sb, r2
003bcc90: beq      #0x3bce18
003bcc94: ldr      r3, [r3, #0x80]
003bcc98: cmp      r3, #0
003bcc9c: beq      #0x3bce18
003bcca0: add      r8, r5, #0x560
003bcca4: mov      r0, r8
003bcca8: mov      r1, #0x9d
003bccac: mov      r2, #0
003bccb0: bl       #0x3df6e0
003bccb4: cmp      r0, #0
003bccb8: ble      #0x3bcda8
003bccbc: mov      r0, r5
003bccc0: mov      r1, r7
003bccc4: bl       #0x3bca50
003bccc8: subs     sl, r0, #0
003bcccc: bne      #0x3bcd34
003bccd0: ldr      r3, [pc, #0x27c]
003bccd4: add      r5, sp, #0x2c
003bccd8: ldr      r7, [r4, r3]
003bccdc: mov      r0, r7
003bcce0: bl       #0x337888
003bcce4: ldr      r1, [pc, #0x26c]
003bcce8: mov      r0, r5
003bccec: str      r5, [sp, #0x3c]
003bccf0: add      r1, pc, r1
003bccf4: add      r1, r1, #0x13
003bccf8: str      r5, [sp, #0x40]
003bccfc: bl       #0x3bcc08
003bcd00: mov      r1, r5
003bcd04: mov      r0, r7
003bcd08: bl       #0x337a88
003bcd0c: mov      r0, r5
003bcd10: bl       #0x3139ac
003bcd14: mov      r0, sl
003bcd18: ldr      r3, [r4, r6]
003bcd1c: ldr      r2, [sp, #0x5c]
003bcd20: ldr      r3, [r3]
003bcd24: cmp      r2, r3
003bcd28: bne      #0x3bcf48
003bcd2c: add      sp, sp, #0x64
003bcd30: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bcd34: ldr      r3, [pc, #0x220]
003bcd38: ldr      sl, [pc, #0x220]
003bcd3c: ldr      r2, [pc, #0x220]
003bcd40: ldr      fp, [r4, r3]
003bcd44: add      sl, pc, sl
003bcd48: add      r2, pc, r2
003bcd4c: mov      r1, sl
003bcd50: ldr      r0, [fp, #0x2c]
003bcd54: bl       #0x4c4bdc
003bcd58: str      r0, [sp, #0xc]
003bcd5c: mov      r0, r5
003bcd60: bl       #0x3bb918
003bcd64: cmp      r0, #1
003bcd68: beq      #0x3bce70
003bcd6c: mov      r0, r5
003bcd70: bl       #0x3bb918
003bcd74: cmp      r0, #2
003bcd78: beq      #0x3bcf2c
003bcd7c: movw     sl, #0x14e8
003bcd80: ldr      r3, [r5, sl]
003bcd84: lsl      fp, r7, #3
003bcd88: ldr      r2, [sp, #0xc]
003bcd8c: ldr      r3, [r3, #0x80]
003bcd90: add      r3, r3, fp
003bcd94: ldrh     r3, [r3, #4]
003bcd98: cmp      r2, r3
003bcd9c: bgt      #0x3bcdf4
003bcda0: mov      r0, #0
003bcda4: b        #0x3bcd18
003bcda8: ldr      r3, [pc, #0x1a4]
003bcdac: add      r5, sp, #0x44
003bcdb0: ldr      r7, [r4, r3]
003bcdb4: mov      r0, r7
003bcdb8: bl       #0x337888
003bcdbc: ldr      r1, [pc, #0x1a4]
003bcdc0: mov      r0, r5
003bcdc4: str      r5, [sp, #0x54]
003bcdc8: add      r1, pc, r1
003bcdcc: add      r1, r1, #0x13
003bcdd0: str      r5, [sp, #0x58]
003bcdd4: bl       #0x3bcc08
003bcdd8: mov      r1, r5
003bcddc: mov      r0, r7
003bcde0: bl       #0x337a88
003bcde4: mov      r0, r5
003bcde8: bl       #0x3139ac
003bcdec: mov      r0, #0
003bcdf0: b        #0x3bcd18
003bcdf4: mov      r1, r7
003bcdf8: mov      r0, r5
003bcdfc: bl       #0x3bc9ec
003bce00: cmp      r0, #0
003bce04: beq      #0x3bcda0
003bce08: cmp      sb, #0
003bce0c: beq      #0x3bce8c
003bce10: mov      r0, #1
003bce14: b        #0x3bcd18
003bce18: ldr      r3, [pc, #0x14c]
003bce1c: ldr      r3, [r4, r3]
003bce20: ldr      r3, [r3]
003bce24: cmp      r3, #2
003bce28: moveq    r3, #0
003bce2c: streq    r3, [r3]
003bce30: beq      #0x3bcca0
003bce34: cmp      r3, #1
003bce38: bne      #0x3bcca0
003bce3c: ldr      r0, [pc, #0x12c]
003bce40: ldr      r1, [pc, #0x12c]
003bce44: ldr      r2, [pc, #0x12c]
003bce48: ldr      r0, [r4, r0]
003bce4c: ldr      r3, [pc, #0x128]
003bce50: mov      ip, #0x86
003bce54: add      r1, pc, r1
003bce58: add      r2, pc, r2
003bce5c: add      r3, pc, r3
003bce60: add      r0, r0, #0xa8
003bce64: str      ip, [sp]
003bce68: bl       #0x30e004
003bce6c: b        #0x3bcca0
003bce70: ldr      r2, [pc, #0x108]
003bce74: ldr      r0, [fp, #0x2c]
003bce78: mov      r1, sl
003bce7c: add      r2, pc, r2
003bce80: bl       #0x4c4bdc
003bce84: str      r0, [sp, #0xc]
003bce88: b        #0x3bcd7c
003bce8c: mvn      r2, #0
003bce90: mov      r1, #0x9d
003bce94: mov      r0, r8
003bce98: bl       #0x3e0798
003bce9c: ldr      r3, [r5, sl]
003bcea0: add      r0, r5, #0x3c8
003bcea4: add      r7, sp, #0x14
003bcea8: ldr      r3, [r3, #0x80]
003bceac: add      fp, r3, fp
003bceb0: ldrh     r3, [fp, #4]
003bceb4: add      r3, r3, #1
003bceb8: strh     r3, [fp, #4]
003bcebc: bl       #0x3d8894
003bcec0: mov      r0, r8
003bcec4: mov      r1, #1
003bcec8: bl       #0x3e0810
003bcecc: mov      r2, sb
003bced0: mov      r1, #0xc2
003bced4: mov      r0, r8
003bced8: bl       #0x3df6e0
003bcedc: ldr      r3, [pc, #0x70]
003bcee0: bic      r0, r0, r0, asr #31
003bcee4: strb     r0, [r5, #0x3a8]
003bcee8: ldr      r5, [r4, r3]
003bceec: mov      r0, r5
003bcef0: bl       #0x337888
003bcef4: ldr      r1, [pc, #0x88]
003bcef8: mov      r0, r7
003bcefc: str      r7, [sp, #0x24]
003bcf00: add      r1, pc, r1
003bcf04: add      r1, r1, #0x13
003bcf08: str      r7, [sp, #0x28]
003bcf0c: bl       #0x3bcc08
003bcf10: mov      r1, r7
003bcf14: mov      r0, r5
003bcf18: bl       #0x337a88
003bcf1c: mov      r0, r7
003bcf20: bl       #0x3139ac
003bcf24: mov      r0, #1
003bcf28: b        #0x3bcd18
003bcf2c: ldr      r2, [pc, #0x54]
003bcf30: ldr      r0, [fp, #0x2c]
003bcf34: mov      r1, sl
003bcf38: add      r2, pc, r2
003bcf3c: bl       #0x4c4bdc
003bcf40: str      r0, [sp, #0xc]
003bcf44: b        #0x3bcd7c
003bcf48: bl       #0x30e310
003bcf4c: subseq   r7, sp, r8, lsr #28
003bcf50: andeq    r4, r0, ip, lsr #1
003bcf54: andeq    r0, r0, r4, lsl #17
003bcf58: subseq   r7, r0, r0, lsr #23
003bcf5c: strdeq   r3, r4, [r0], -r4
003bcf60: subseq   r4, r0, ip, lsl #20
003bcf64: subseq   r7, r0, r0, ror #22
003bcf68: subseq   r7, r0, r8, asr #21
003bcf6c: andeq    r3, r0, r0, asr #19
003bcf70: andeq    r1, r0, r0, asr #19
003bcf74: subseq   r1, r0, r4, lsl #11
003bcf78: subseq   r7, r0, r0, lsl #20
003bcf7c: subseq   r7, r0, r4, lsr #19
003bcf80: subseq   r7, r0, r4, asr #20

# _ZN9Character17EquipAllSlotsAutoEv
003a4c14: push     {r4, r5, r6, r7, r8, lr}
003a4c18: add      r7, r0, #0x37c
003a4c1c: mov      r5, r0
003a4c20: mov      r0, r7
003a4c24: bl       #0x3ffd20
003a4c28: subs     r4, r0, #0
003a4c2c: ble      #0x3a4c54
003a4c30: mov      r6, #0
003a4c34: mov      r1, r6
003a4c38: ldr      r3, [r5]
003a4c3c: add      r6, r6, #1
003a4c40: mov      r0, r5
003a4c44: mov      lr, pc
003a4c48: ldr      pc, [r3, #0x144]
003a4c4c: cmp      r4, r6
003a4c50: bne      #0x3a4c34
003a4c54: subs     r4, r4, #1
003a4c58: bmi      #0x3a4cbc
003a4c5c: mov      r6, r4
003a4c60: mov      r1, r6
003a4c64: ldr      r3, [r5]
003a4c68: sub      r6, r6, #1
003a4c6c: mov      r0, r5
003a4c70: mov      lr, pc
003a4c74: ldr      pc, [r3, #0x140]
003a4c78: cmn      r6, #1
003a4c7c: bne      #0x3a4c60
003a4c80: mov      r1, r4
003a4c84: mov      r0, r7
003a4c88: bl       #0x4002d8
003a4c8c: cmp      r0, #0
003a4c90: bne      #0x3a4cb4
003a4c94: cmp      r4, #1
003a4c98: cmpne    r4, #2
003a4c9c: mov      r1, r4
003a4ca0: mov      r0, r5
003a4ca4: beq      #0x3a4cb4
003a4ca8: ldr      r3, [r5]
003a4cac: mov      lr, pc
003a4cb0: ldr      pc, [r3, #0x140]
003a4cb4: subs     r4, r4, #1
003a4cb8: bhs      #0x3a4c80
003a4cbc: pop      {r4, r5, r6, r7, r8, pc}

# _ZN9Character14_InitEquipmentEv
003b395c: push     {r4, r5, r6, r7, lr}
003b3960: sub      sp, sp, #0xc
003b3964: mov      r4, r0
003b3968: bl       #0x7fd794
003b396c: ldrb     r2, [r0, #5]
003b3970: ldr      r3, [pc, #0xf0]
003b3974: cmp      r2, #0
003b3978: add      r3, pc, r3
003b397c: bne      #0x3b3a40
003b3980: add      r5, r4, #0x37c
003b3984: mov      r0, r5
003b3988: bl       #0x3fc608
003b398c: cmp      r0, #0
003b3990: bne      #0x3b3a30
003b3994: ldr      r6, [r4, #0x39c]
003b3998: cmp      r6, #0
003b399c: bne      #0x3b3a30
003b39a0: add      r1, r4, #0xff0
003b39a4: add      r1, r1, #4
003b39a8: mov      r2, #9
003b39ac: add      r0, r4, #0x560
003b39b0: bl       #0x3dedb4
003b39b4: mvn      ip, #0
003b39b8: mov      r1, r0
003b39bc: mov      r2, r6
003b39c0: mov      r3, r6
003b39c4: mov      r0, r5
003b39c8: str      ip, [sp]
003b39cc: str      r6, [sp, #4]
003b39d0: bl       #0x40407c
003b39d4: mov      r0, r5
003b39d8: bl       #0x3fc608
003b39dc: subs     r7, r0, #0
003b39e0: bne      #0x3b39f4
003b39e4: b        #0x3b3a28
003b39e8: add      r6, r6, #1
003b39ec: cmp      r6, r7
003b39f0: beq      #0x3b3a28
003b39f4: mov      r1, r6
003b39f8: mov      r0, r5
003b39fc: bl       #0x3fdeec
003b3a00: cmp      r0, #0
003b3a04: beq      #0x3b39e8
003b3a08: mov      r1, r6
003b3a0c: ldr      r3, [r4]
003b3a10: mov      r0, r4
003b3a14: add      r6, r6, #1
003b3a18: mov      lr, pc
003b3a1c: ldr      pc, [r3, #0x130]
003b3a20: cmp      r6, r7
003b3a24: bne      #0x3b39f4
003b3a28: add      sp, sp, #0xc
003b3a2c: pop      {r4, r5, r6, r7, pc}
003b3a30: mov      r0, r4
003b3a34: add      sp, sp, #0xc
003b3a38: pop      {r4, r5, r6, r7, lr}
003b3a3c: b        #0x3a999c
003b3a40: ldr      r0, [pc, #0x24]
003b3a44: mov      r1, r4
003b3a48: mov      r2, #0
003b3a4c: ldr      r3, [r3, r0]
003b3a50: ldr      r0, [r3, #0x40]
003b3a54: bl       #0x36eea8
003b3a58: ldrb     r3, [r0, #0x66c]
003b3a5c: cmp      r3, #1
003b3a60: bne      #0x3b3a28
003b3a64: b        #0x3b3980
003b3a68: subseq   r1, lr, r8, lsl r1
003b3a6c: strdeq   r3, r4, [r0], -r4

# _ZN9Character13SetFaeryStateEji
003ae7d4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ae7d8: sub      sp, sp, #0xc
003ae7dc: mov      r5, r1
003ae7e0: mov      r6, r0
003ae7e4: mov      r7, r2
003ae7e8: bl       #0x3bb8e4
003ae7ec: mov      r8, r0
003ae7f0: mov      r1, r8
003ae7f4: mov      r0, r6
003ae7f8: bl       #0x3bba20
003ae7fc: ldr      r4, [pc, #0x124]
003ae800: cmp      r0, r5
003ae804: add      r4, pc, r4
003ae808: bhi      #0x3ae830
003ae80c: ldr      r3, [pc, #0x118]
003ae810: ldr      r3, [r4, r3]
003ae814: ldr      r3, [r3]
003ae818: cmp      r3, #2
003ae81c: moveq    r3, #0
003ae820: streq    r3, [r3]
003ae824: beq      #0x3ae830
003ae828: cmp      r3, #1
003ae82c: beq      #0x3ae8f4
003ae830: cmp      r7, #1
003ae834: movne    r3, #0
003ae838: moveq    r3, #1
003ae83c: cmp      r5, #0
003ae840: moveq    r3, #0
003ae844: cmp      r3, #0
003ae848: bne      #0x3ae868
003ae84c: mov      r0, r6
003ae850: mov      r1, r5
003ae854: mov      r2, r7
003ae858: mov      r3, r8
003ae85c: add      sp, sp, #0xc
003ae860: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ae864: b        #0x3bbb48
003ae868: bl       #0x7fd794
003ae86c: ldrb     r3, [r0, #5]
003ae870: cmp      r3, #0
003ae874: bne      #0x3ae84c
003ae878: mov      r0, r6
003ae87c: bl       #0x3bb8e4
003ae880: subs     sb, r0, #0
003ae884: bne      #0x3ae84c
003ae888: ldr      sl, [pc, #0xa0]
003ae88c: ldr      r3, [r4, sl]
003ae890: ldr      r3, [r3, #0x4c]
003ae894: ldrb     r3, [r3, #0x2b]
003ae898: cmp      r3, #0
003ae89c: beq      #0x3ae84c
003ae8a0: ldr      r3, [pc, #0x8c]
003ae8a4: ldr      r1, [pc, #0x8c]
003ae8a8: mov      r2, #1
003ae8ac: ldr      fp, [r4, r3]
003ae8b0: add      r1, pc, r1
003ae8b4: mov      r0, fp
003ae8b8: bl       #0x4591f0
003ae8bc: cmn      r0, #1
003ae8c0: mov      r1, r0
003ae8c4: beq      #0x3ae8d8
003ae8c8: mov      r0, fp
003ae8cc: mov      r3, sb
003ae8d0: mvn      r2, #0
003ae8d4: bl       #0x4605c0
003ae8d8: ldr      r3, [r4, sl]
003ae8dc: mov      r1, #0
003ae8e0: ldr      r2, [r3, #0x4c]
003ae8e4: strb     r1, [r2, #0x2b]
003ae8e8: ldr      r0, [r3, #0x4c]
003ae8ec: bl       #0x46cb34
003ae8f0: b        #0x3ae84c
003ae8f4: ldr      r0, [pc, #0x40]
003ae8f8: ldr      r1, [pc, #0x40]
003ae8fc: ldr      r2, [pc, #0x40]
003ae900: ldr      r0, [r4, r0]
003ae904: ldr      r3, [pc, #0x3c]
003ae908: mov      ip, #0x87
003ae90c: add      r1, pc, r1
003ae910: add      r2, pc, r2
003ae914: add      r3, pc, r3
003ae918: add      r0, r0, #0xa8
003ae91c: str      ip, [sp]
003ae920: bl       #0x30e004
003ae924: b        #0x3ae830
003ae928: subseq   r6, lr, ip, lsl #5
003ae92c: andeq    r3, r0, r0, asr #19
003ae930: strdeq   r3, r4, [r0], -r4
003ae934: andeq    r1, r0, r0, lsr #20
003ae938: ldrheq   r4, [r1], #-0xf0
003ae93c: andeq    r1, r0, r0, asr #19
003ae940: subseq   pc, r0, ip, asr #21
003ae944: subseq   r4, r1, r0, asr #29
003ae948: subseq   r4, r1, r4, ror #29

# _ZN9Character19SG_SetCurrentFaerieEji
003bb9d8: movw     r3, #0x14e8
003bb9dc: ldr      r0, [r0, r3]
003bb9e0: ldr      r3, [pc, #0x30]
003bb9e4: cmp      r0, #0
003bb9e8: add      r3, pc, r3
003bb9ec: bxeq     lr
003bb9f0: cmn      r2, #1
003bb9f4: addne    r0, r0, r2, lsl #2
003bb9f8: strne    r1, [r0, #0xac]
003bb9fc: bxne     lr
003bba00: ldr      r2, [pc, #0x14]
003bba04: ldr      r3, [r3, r2]
003bba08: ldr      r3, [r3]
003bba0c: add      r0, r0, r3, lsl #2
003bba10: str      r1, [r0, #0xac]
003bba14: bx       lr
003bba18: subseq   sb, sp, r8, lsr #1
003bba1c: muleq    r0, ip, sl
