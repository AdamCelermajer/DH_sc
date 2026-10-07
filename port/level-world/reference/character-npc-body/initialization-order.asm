
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

# _ZN12CharAnimator15SetAnimationSetEv
003c9f4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c9f50: mov      r4, r0
003c9f54: sub      sp, sp, #0x14
003c9f58: ldr      r0, [r0, #4]
003c9f5c: bl       #0x3a54b4
003c9f60: ldr      r5, [pc, #0x5a8]
003c9f64: ldr      r3, [pc, #0x5a8]
003c9f68: mov      r1, r0
003c9f6c: add      r5, pc, r5
003c9f70: str      r0, [r4, #0x3c]
003c9f74: ldr      r0, [r5, r3]
003c9f78: bl       #0x475404
003c9f7c: cmp      r0, #0
003c9f80: beq      #0x3c9f8c
003c9f84: add      sp, sp, #0x14
003c9f88: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c9f8c: ldr      r3, [r4, #4]
003c9f90: mov      r0, r3
003c9f94: ldr      r3, [r3]
003c9f98: mov      lr, pc
003c9f9c: ldr      pc, [r3, #0x28]
003c9fa0: cmp      r0, #0
003c9fa4: bne      #0x3ca4d8
003c9fa8: ldr      r6, [pc, #0x568]
003c9fac: mov      r3, #1
003c9fb0: str      r3, [sp, #0xc]
003c9fb4: ldr      r3, [r5, r6]
003c9fb8: ldr      r1, [pc, #0x55c]
003c9fbc: ldr      r2, [pc, #0x55c]
003c9fc0: ldr      r0, [r3, #0x2c]
003c9fc4: add      r1, pc, r1
003c9fc8: add      r2, pc, r2
003c9fcc: bl       #0x4c4bdc
003c9fd0: mov      r7, r0
003c9fd4: ldr      r0, [r4, #4]
003c9fd8: bl       #0x3a3264
003c9fdc: mov      r5, r0
003c9fe0: ldr      r0, [r4, #4]
003c9fe4: bl       #0x3bc5fc
003c9fe8: ldr      r2, [r5, #0x90]
003c9fec: mov      sl, r0
003c9ff0: cmn      r2, #1
003c9ff4: beq      #0x3ca500
003c9ff8: mov      r6, #0
003c9ffc: mov      r0, r4
003ca000: ldr      r1, [r4, #0x3c]
003ca004: mov      r3, #0
003ca008: bl       #0x3c9c7c
003ca00c: ldr      r2, [r5, #0x58]
003ca010: ldr      r1, [r4, #0x3c]
003ca014: mov      r0, r4
003ca018: mov      r3, r6
003ca01c: str      r6, [sp]
003ca020: str      r6, [sp, #4]
003ca024: bl       #0x3c9d80
003ca028: ldr      r2, [r5, #0x64]
003ca02c: ldr      r1, [r4, #0x3c]
003ca030: mov      r0, r4
003ca034: mov      r3, r6
003ca038: str      r6, [sp]
003ca03c: str      r6, [sp, #4]
003ca040: bl       #0x3c9d80
003ca044: ldr      r2, [r5, #0x80]
003ca048: ldr      r1, [r4, #0x3c]
003ca04c: mov      r0, r4
003ca050: mov      r3, r6
003ca054: str      r6, [sp]
003ca058: str      r6, [sp, #4]
003ca05c: bl       #0x3c9d80
003ca060: ldr      r2, [r5, #0x5c]
003ca064: ldr      r1, [r4, #0x3c]
003ca068: mov      r0, r4
003ca06c: mov      r3, r6
003ca070: str      r6, [sp]
003ca074: str      r6, [sp, #4]
003ca078: bl       #0x3c9d80
003ca07c: ldr      ip, [sp, #0xc]
003ca080: cmp      ip, r6
003ca084: beq      #0x3c9f84
003ca088: mov      sb, #0x800000
003ca08c: ldr      r2, [r5, #0x28]
003ca090: ldr      r1, [r4, #0x3c]
003ca094: mov      ip, #2
003ca098: mov      r0, r4
003ca09c: mov      r3, r6
003ca0a0: str      ip, [sp]
003ca0a4: str      r7, [sp, #4]
003ca0a8: bl       #0x3c9d80
003ca0ac: ldr      r2, [r5, #0x94]
003ca0b0: ldr      r1, [r4, #0x3c]
003ca0b4: mov      ip, #0x10
003ca0b8: mov      r0, r4
003ca0bc: mov      r3, r6
003ca0c0: str      ip, [sp]
003ca0c4: str      r7, [sp, #4]
003ca0c8: bl       #0x3c9d80
003ca0cc: ldr      r2, [r5, #0x70]
003ca0d0: ldr      r1, [r4, #0x3c]
003ca0d4: mov      ip, #0x20
003ca0d8: mov      r0, r4
003ca0dc: mov      r3, r6
003ca0e0: str      ip, [sp]
003ca0e4: str      r7, [sp, #4]
003ca0e8: bl       #0x3c9d80
003ca0ec: ldr      r2, [r5, #4]
003ca0f0: ldr      r1, [r4, #0x3c]
003ca0f4: mov      ip, #0x40
003ca0f8: mov      r0, r4
003ca0fc: mov      r3, r6
003ca100: str      ip, [sp]
003ca104: str      r7, [sp, #4]
003ca108: bl       #0x3c9d80
003ca10c: ldr      r2, [r5, #8]
003ca110: ldr      r1, [r4, #0x3c]
003ca114: mov      ip, #0x80
003ca118: mov      r0, r4
003ca11c: mov      r3, r6
003ca120: str      ip, [sp]
003ca124: str      r7, [sp, #4]
003ca128: bl       #0x3c9d80
003ca12c: ldr      r2, [r5, #0x7c]
003ca130: ldr      r1, [r4, #0x3c]
003ca134: mov      ip, #0x100
003ca138: mov      r0, r4
003ca13c: mov      r3, r6
003ca140: str      ip, [sp]
003ca144: str      r7, [sp, #4]
003ca148: bl       #0x3c9d80
003ca14c: ldr      r2, [r5, #0x8c]
003ca150: ldr      r1, [r4, #0x3c]
003ca154: mov      ip, #0x200
003ca158: mov      r0, r4
003ca15c: mov      r3, r6
003ca160: str      ip, [sp]
003ca164: str      r7, [sp, #4]
003ca168: bl       #0x3c9d80
003ca16c: ldr      r2, [r5, #0x48]
003ca170: ldr      r1, [r4, #0x3c]
003ca174: mov      ip, #0x400
003ca178: mov      r0, r4
003ca17c: mov      r3, r6
003ca180: str      ip, [sp]
003ca184: str      r7, [sp, #4]
003ca188: bl       #0x3c9d80
003ca18c: ldr      r2, [r5, #0x24]
003ca190: ldr      r1, [r4, #0x3c]
003ca194: mov      ip, #0x800
003ca198: mov      r0, r4
003ca19c: mov      r3, r6
003ca1a0: str      ip, [sp]
003ca1a4: str      r7, [sp, #4]
003ca1a8: bl       #0x3c9d80
003ca1ac: ldr      r2, [r5, #0x3c]
003ca1b0: ldr      r1, [r4, #0x3c]
003ca1b4: mov      ip, #0x1000
003ca1b8: mov      r0, r4
003ca1bc: mov      r3, r6
003ca1c0: str      ip, [sp]
003ca1c4: str      r7, [sp, #4]
003ca1c8: bl       #0x3c9d80
003ca1cc: ldr      r2, [r5, #0xc]
003ca1d0: ldr      r1, [r4, #0x3c]
003ca1d4: mov      ip, #0x2000
003ca1d8: mov      r0, r4
003ca1dc: mov      r3, r6
003ca1e0: str      ip, [sp]
003ca1e4: str      r7, [sp, #4]
003ca1e8: bl       #0x3c9d80
003ca1ec: ldr      r2, [r5, #0x20]
003ca1f0: ldr      r1, [r4, #0x3c]
003ca1f4: mov      ip, #0x4000
003ca1f8: mov      r0, r4
003ca1fc: mov      r3, r6
003ca200: str      ip, [sp]
003ca204: str      r7, [sp, #4]
003ca208: bl       #0x3c9d80
003ca20c: ldr      r2, [r5, #0x1c]
003ca210: ldr      r1, [r4, #0x3c]
003ca214: mov      ip, #0x8000
003ca218: mov      r0, r4
003ca21c: mov      r3, r6
003ca220: str      ip, [sp]
003ca224: str      r7, [sp, #4]
003ca228: bl       #0x3c9d80
003ca22c: ldr      r2, [r5, #0x14]
003ca230: ldr      r1, [r4, #0x3c]
003ca234: mov      ip, #0x10000
003ca238: mov      r0, r4
003ca23c: mov      r3, r6
003ca240: str      ip, [sp]
003ca244: str      r7, [sp, #4]
003ca248: bl       #0x3c9d80
003ca24c: ldr      r2, [r5, #0x10]
003ca250: ldr      r1, [r4, #0x3c]
003ca254: mov      ip, #0x20000
003ca258: mov      r0, r4
003ca25c: mov      r3, r6
003ca260: str      ip, [sp]
003ca264: str      r7, [sp, #4]
003ca268: bl       #0x3c9d80
003ca26c: ldr      r2, [r5, #0x18]
003ca270: ldr      r1, [r4, #0x3c]
003ca274: mov      ip, #0x40000
003ca278: mov      r0, r4
003ca27c: mov      r3, r6
003ca280: str      ip, [sp]
003ca284: str      r7, [sp, #4]
003ca288: bl       #0x3c9d80
003ca28c: ldr      r2, [r5, #0x68]
003ca290: ldr      r1, [r4, #0x3c]
003ca294: mov      ip, #0x100000
003ca298: mov      r0, r4
003ca29c: mov      r3, r6
003ca2a0: str      ip, [sp]
003ca2a4: str      r7, [sp, #4]
003ca2a8: bl       #0x3c9d80
003ca2ac: mov      ip, #0x80000
003ca2b0: ldr      r2, [r5, #0x6c]
003ca2b4: ldr      r1, [r4, #0x3c]
003ca2b8: mov      r0, r4
003ca2bc: mov      r3, r6
003ca2c0: mov      r8, #0
003ca2c4: str      ip, [sp]
003ca2c8: str      r7, [sp, #4]
003ca2cc: bl       #0x3c9d80
003ca2d0: ldr      r2, [r5, #0x98]
003ca2d4: ldr      r1, [r4, #0x3c]
003ca2d8: mov      r0, r4
003ca2dc: mov      r3, r6
003ca2e0: str      r8, [sp]
003ca2e4: str      r7, [sp, #4]
003ca2e8: bl       #0x3c9d80
003ca2ec: ldr      r2, [r5, #0x74]
003ca2f0: ldr      r1, [r4, #0x3c]
003ca2f4: mov      r0, r4
003ca2f8: mov      r3, r6
003ca2fc: str      r8, [sp]
003ca300: str      r7, [sp, #4]
003ca304: bl       #0x3c9d80
003ca308: mov      ip, #4
003ca30c: ldr      r2, [r5, #0x30]
003ca310: ldr      r1, [r4, #0x3c]
003ca314: mov      r0, r4
003ca318: mov      r3, r6
003ca31c: str      ip, [sp]
003ca320: mov      fp, #8
003ca324: str      r7, [sp, #4]
003ca328: bl       #0x3c9d80
003ca32c: ldr      r2, [r5, #0x38]
003ca330: ldr      r1, [r4, #0x3c]
003ca334: mov      r0, r4
003ca338: mov      r3, r6
003ca33c: str      fp, [sp]
003ca340: str      r7, [sp, #4]
003ca344: bl       #0x3c9d80
003ca348: ldr      r2, [r5, #0x2c]
003ca34c: ldr      r1, [r4, #0x3c]
003ca350: mov      r0, r4
003ca354: mov      r3, r6
003ca358: str      fp, [sp]
003ca35c: str      r7, [sp, #4]
003ca360: bl       #0x3c9d80
003ca364: ldr      r2, [r5, #0x34]
003ca368: ldr      r1, [r4, #0x3c]
003ca36c: mov      r0, r4
003ca370: mov      r3, r6
003ca374: str      r8, [sp]
003ca378: str      r7, [sp, #4]
003ca37c: bl       #0x3c9d80
003ca380: ldr      r2, [r5, #0x9c]
003ca384: ldr      r1, [r4, #0x3c]
003ca388: mov      r0, r4
003ca38c: mov      r3, r6
003ca390: str      r8, [sp]
003ca394: str      r7, [sp, #4]
003ca398: bl       #0x3c9d80
003ca39c: ldr      r2, [r5, #0x78]
003ca3a0: ldr      r1, [r4, #0x3c]
003ca3a4: mov      r0, r4
003ca3a8: mov      r3, r6
003ca3ac: mov      fp, #0x1000000
003ca3b0: str      r8, [sp]
003ca3b4: str      r7, [sp, #4]
003ca3b8: bl       #0x3c9d80
003ca3bc: ldr      r2, [r5, #0x50]
003ca3c0: ldr      r1, [r4, #0x3c]
003ca3c4: mov      r0, r4
003ca3c8: mov      r3, r6
003ca3cc: str      fp, [sp]
003ca3d0: str      r7, [sp, #4]
003ca3d4: bl       #0x3c9d80
003ca3d8: ldr      r2, [r5, #0x54]
003ca3dc: ldr      r1, [r4, #0x3c]
003ca3e0: mov      r3, r6
003ca3e4: mov      r0, r4
003ca3e8: str      fp, [sp]
003ca3ec: str      r7, [sp, #4]
003ca3f0: bl       #0x3c9d80
003ca3f4: ldr      r3, [r5, #0x40]
003ca3f8: cmp      r3, r8
003ca3fc: beq      #0x3ca430
003ca400: ldr      r3, [r5, #0x44]
003ca404: ldr      r1, [r4, #0x3c]
003ca408: mov      r0, r4
003ca40c: ldr      r2, [r3, r8, lsl #2]
003ca410: mov      r3, r6
003ca414: str      sb, [sp]
003ca418: str      r7, [sp, #4]
003ca41c: bl       #0x3c9d80
003ca420: ldr      r3, [r5, #0x40]
003ca424: add      r8, r8, #1
003ca428: cmp      r3, r8
003ca42c: bhi      #0x3ca400
003ca430: ldr      r3, [r5, #0x84]
003ca434: cmp      r3, #0
003ca438: beq      #0x3ca474
003ca43c: mov      r8, #0
003ca440: ldr      r3, [r5, #0x88]
003ca444: ldr      r1, [r4, #0x3c]
003ca448: mov      ip, #0x400000
003ca44c: ldr      r2, [r3, r8, lsl #2]
003ca450: mov      r0, r4
003ca454: mov      r3, r6
003ca458: str      ip, [sp]
003ca45c: str      r7, [sp, #4]
003ca460: bl       #0x3c9d80
003ca464: ldr      r3, [r5, #0x84]
003ca468: add      r8, r8, #1
003ca46c: cmp      r3, r8
003ca470: bhi      #0x3ca440
003ca474: ldr      r3, [sl, #4]
003ca478: cmp      r3, #0
003ca47c: beq      #0x3ca4c4
003ca480: mov      r8, #0
003ca484: mov      r1, r8
003ca488: ldr      r0, [r4, #4]
003ca48c: ldr      fp, [r4, #0x3c]
003ca490: bl       #0x3bc784
003ca494: mov      r3, r6
003ca498: ldr      r2, [r0, #4]
003ca49c: mov      ip, #0x200000
003ca4a0: mov      r1, fp
003ca4a4: mov      r0, r4
003ca4a8: str      ip, [sp]
003ca4ac: str      r7, [sp, #4]
003ca4b0: bl       #0x3c9d80
003ca4b4: ldr      r3, [sl, #4]
003ca4b8: add      r8, r8, #1
003ca4bc: cmp      r3, r8
003ca4c0: bhi      #0x3ca484
003ca4c4: ldr      r3, [sp, #0xc]
003ca4c8: add      r6, r6, #1
003ca4cc: cmp      r6, r3
003ca4d0: bne      #0x3ca08c
003ca4d4: b        #0x3c9f84
003ca4d8: ldr      r6, [pc, #0x38]
003ca4dc: ldr      r1, [pc, #0x40]
003ca4e0: ldr      r2, [pc, #0x40]
003ca4e4: ldr      r3, [r5, r6]
003ca4e8: add      r1, pc, r1
003ca4ec: add      r2, pc, r2
003ca4f0: ldr      r0, [r3, #0x2c]
003ca4f4: bl       #0x4c4bdc
003ca4f8: str      r0, [sp, #0xc]
003ca4fc: b        #0x3c9fb4
003ca500: ldr      r0, [r4, #4]
003ca504: bl       #0x3a3228
003ca508: ldr      r2, [r5, #0x90]
003ca50c: b        #0x3c9ff8
003ca510: subseq   sl, ip, r4, lsr #22
003ca514: andeq    r4, r0, r8, lsr r8
003ca518: strdeq   r3, r4, [r0], -r4
003ca51c: strdeq   sl, fp, [pc], #-0xb4
003ca520: subeq    sl, pc, r0, lsl #24
003ca524: subeq    r8, pc, r8, lsr #27
003ca528: strheq   r8, [pc], #-0xd4

# _ZN12CharAnimator21_AddTemplateAnimTableEiij
003c9c7c: push     {r4, r5, r6, lr}
003c9c80: ldr      r4, [pc, #0xd4]
003c9c84: cmp      r2, #0
003c9c88: sub      sp, sp, #8
003c9c8c: mov      r6, r1
003c9c90: add      r4, pc, r4
003c9c94: blt      #0x3c9d04
003c9c98: add      r2, r3, r2
003c9c9c: ldr      r3, [pc, #0xbc]
003c9ca0: ldr      r3, [r4, r3]
003c9ca4: ldr      r3, [r3]
003c9ca8: cmp      r2, r3
003c9cac: bge      #0x3c9d04
003c9cb0: ldr      r3, [pc, #0xac]
003c9cb4: mov      r5, #0x14
003c9cb8: ldr      r3, [r4, r3]
003c9cbc: ldr      r3, [r3]
003c9cc0: mla      r5, r5, r2, r3
003c9cc4: ldr      r3, [r5, #8]
003c9cc8: cmp      r3, #1
003c9ccc: beq      #0x3c9cf4
003c9cd0: ldr      r3, [pc, #0x90]
003c9cd4: ldr      r3, [r4, r3]
003c9cd8: ldr      r3, [r3]
003c9cdc: cmp      r3, #2
003c9ce0: moveq    r3, #0
003c9ce4: streq    r3, [r3]
003c9ce8: beq      #0x3c9cf4
003c9cec: cmp      r3, #1
003c9cf0: beq      #0x3c9d28
003c9cf4: ldr      r3, [r5, #0xc]
003c9cf8: ldr      r2, [r3, #0x28]
003c9cfc: cmp      r2, #0
003c9d00: beq      #0x3c9d0c
003c9d04: add      sp, sp, #8
003c9d08: pop      {r4, r5, r6, pc}
003c9d0c: ldr      r2, [r3, #8]
003c9d10: ldr      r3, [pc, #0x54]
003c9d14: mov      r1, r6
003c9d18: ldr      r0, [r4, r3]
003c9d1c: add      sp, sp, #8
003c9d20: pop      {r4, r5, r6, lr}
003c9d24: b        #0x476398
003c9d28: ldr      r0, [pc, #0x40]
003c9d2c: ldr      r1, [pc, #0x40]
003c9d30: ldr      r2, [pc, #0x40]
003c9d34: ldr      r0, [r4, r0]
003c9d38: ldr      r3, [pc, #0x3c]
003c9d3c: mov      ip, #0xc8
003c9d40: add      r1, pc, r1
003c9d44: add      r2, pc, r2
003c9d48: add      r3, pc, r3
003c9d4c: add      r0, r0, #0xa8
003c9d50: str      ip, [sp]
003c9d54: bl       #0x30e004
003c9d58: b        #0x3c9cf4
003c9d5c: subseq   sl, ip, r0, lsl #28
003c9d60: andeq    r2, r0, r8, asr #20
003c9d64: andeq    r3, r0, ip, ror ip
003c9d68: andeq    r3, r0, r0, asr #19
003c9d6c: andeq    r4, r0, r8, lsr r8
003c9d70: andeq    r1, r0, r0, asr #19
003c9d74: umaaleq  r4, pc, r8, r6
003c9d78: strdeq   fp, ip, [pc], #-0x24
003c9d7c: subeq    fp, pc, r8, lsr r2

# _ZN12CharAnimator13_AddAnimTableEiijjj
003c9d80: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c9d84: ldr      r4, [pc, #0x19c]
003c9d88: ldr      sl, [pc, #0x19c]
003c9d8c: mov      sb, r1
003c9d90: add      r4, pc, r4
003c9d94: ldr      ip, [r4, sl]
003c9d98: sub      sp, sp, #0x3c
003c9d9c: cmp      r2, #0
003c9da0: ldr      r1, [ip]
003c9da4: mov      fp, r0
003c9da8: ldr      r0, [sp, #0x60]
003c9dac: str      r1, [sp, #0x34]
003c9db0: blt      #0x3c9de8
003c9db4: ldr      r1, [pc, #0x174]
003c9db8: add      r2, r3, r2
003c9dbc: ldr      r1, [r4, r1]
003c9dc0: ldr      r1, [r1]
003c9dc4: cmp      r2, r1
003c9dc8: bge      #0x3c9de8
003c9dcc: ldr      r8, [sp, #0x64]
003c9dd0: and      r8, r8, r0
003c9dd4: cmp      r8, r0
003c9dd8: cmpne    r3, #0
003c9ddc: moveq    r8, #0
003c9de0: movne    r8, #1
003c9de4: beq      #0x3c9e04
003c9de8: ldr      r3, [r4, sl]
003c9dec: ldr      r2, [sp, #0x34]
003c9df0: ldr      r3, [r3]
003c9df4: cmp      r2, r3
003c9df8: bne      #0x3c9f24
003c9dfc: add      sp, sp, #0x3c
003c9e00: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c9e04: ldr      r3, [pc, #0x128]
003c9e08: ldr      r1, [pc, #0x128]
003c9e0c: mov      r7, #0x14
003c9e10: ldr      r3, [r4, r3]
003c9e14: ldr      r6, [r4, r1]
003c9e18: add      r5, sp, #0x1c
003c9e1c: ldr      r3, [r3]
003c9e20: mov      r0, r6
003c9e24: mla      r7, r7, r2, r3
003c9e28: bl       #0x337888
003c9e2c: ldr      r1, [pc, #0x108]
003c9e30: add      r2, sp, #0x18
003c9e34: mov      r0, r5
003c9e38: add      r1, pc, r1
003c9e3c: bl       #0x3140ec
003c9e40: mov      r1, r5
003c9e44: mov      r0, r6
003c9e48: bl       #0x337a88
003c9e4c: mov      r0, r5
003c9e50: bl       #0x3139ac
003c9e54: ldr      r3, [r7, #8]
003c9e58: cmp      r3, #0
003c9e5c: beq      #0x3c9de8
003c9e60: ldr      r2, [pc, #0xd8]
003c9e64: ldr      r3, [pc, #0xd8]
003c9e68: mov      r5, r8
003c9e6c: str      r2, [sp, #0xc]
003c9e70: ldr      r2, [pc, #0xd0]
003c9e74: str      r3, [sp, #0x10]
003c9e78: mov      r6, r8
003c9e7c: str      r2, [sp, #0x14]
003c9e80: b        #0x3c9ef0
003c9e84: ldr      r2, [r3, #8]
003c9e88: ldr      r3, [sp, #0xc]
003c9e8c: mov      r1, sb
003c9e90: ldr      r0, [r4, r3]
003c9e94: bl       #0x47653c
003c9e98: ldr      r2, [sp, #0x10]
003c9e9c: ldr      r3, [r4, r2]
003c9ea0: ldr      r0, [r3]
003c9ea4: cmp      r0, #0
003c9ea8: beq      #0x3c9ebc
003c9eac: ldr      r3, [r7, #0xc]
003c9eb0: add      r3, r3, r5
003c9eb4: ldr      r1, [r3, #0x2c]
003c9eb8: bl       #0x3699fc
003c9ebc: ldr      r3, [r7, #0xc]
003c9ec0: add      r3, r3, r5
003c9ec4: ldr      r1, [r3, #0x18]
003c9ec8: cmp      r1, #0
003c9ecc: blt      #0x3c9edc
003c9ed0: ldr      r3, [sp, #0x14]
003c9ed4: ldr      r0, [r4, r3]
003c9ed8: bl       #0x4967e8
003c9edc: ldr      r3, [r7, #8]
003c9ee0: add      r6, r6, #1
003c9ee4: add      r5, r5, #0x38
003c9ee8: cmp      r3, r6
003c9eec: bls      #0x3c9de8
003c9ef0: ldr      r3, [r7, #0xc]
003c9ef4: add      r3, r3, r5
003c9ef8: ldr      r2, [r3, #0x28]
003c9efc: cmp      r2, #0
003c9f00: beq      #0x3c9e84
003c9f04: ldr      r2, [r3, #8]
003c9f08: mov      r0, fp
003c9f0c: mov      r1, sb
003c9f10: mov      r3, r8
003c9f14: str      r8, [sp]
003c9f18: str      r8, [sp, #4]
003c9f1c: bl       #0x3c9d80
003c9f20: b        #0x3c9edc
003c9f24: bl       #0x30e310
003c9f28: subseq   sl, ip, r0, lsl #26
003c9f2c: andeq    r4, r0, ip, lsr #1
003c9f30: andeq    r2, r0, r8, asr #20
003c9f34: andeq    r3, r0, ip, ror ip
003c9f38: andeq    r0, r0, r4, lsl #17
003c9f3c: ldrdeq   sb, sl, [pc], #-0xf8
003c9f40: andeq    r4, r0, r8, lsr r8
003c9f44: andeq    r0, r0, r4, lsr #27
003c9f48: andeq    r1, r0, r8, lsl #22

# _ZN14AnimSetManager15AddTemplateAnimEii
00476398: push     {r4, r5, r6, r7, r8, lr}
0047639c: sub      sp, sp, #0x10
004763a0: mov      r5, r0
004763a4: str      r1, [sp, #4]
004763a8: mov      r7, r2
004763ac: bl       #0x475404
004763b0: ldr      r6, [pc, #0xa0]
004763b4: cmp      r0, #0
004763b8: addne    r5, r5, #4
004763bc: add      r6, pc, r6
004763c0: addne    r4, sp, #4
004763c4: bne      #0x476408
004763c8: ldr      r3, [sp, #4]
004763cc: cmp      r3, #0
004763d0: blt      #0x476450
004763d4: add      r5, r5, #4
004763d8: add      r4, sp, #4
004763dc: mov      r1, r4
004763e0: mov      r0, r5
004763e4: bl       #0x476058
004763e8: mov      r8, r0
004763ec: bl       #0x364ca0
004763f0: ldr      r3, [pc, #0x64]
004763f4: ldr      r3, [r6, r3]
004763f8: ldrb     r3, [r3]
004763fc: cmp      r3, #0
00476400: movne    r3, #1
00476404: strbne   r3, [r8, #0x3c]
00476408: mov      r1, r4
0047640c: mov      r0, r5
00476410: bl       #0x476058
00476414: mov      r1, r7
00476418: mov      r5, r0
0047641c: bl       #0x3659ec
00476420: ldr      r3, [pc, #0x38]
00476424: ldr      r5, [r5, #0x20]
00476428: add      r4, sp, #8
0047642c: ldr      r1, [r0, #0x14]
00476430: ldr      r2, [r6, r3]
00476434: mov      r0, r4
00476438: bl       #0x60f25c
0047643c: mov      r0, r5
00476440: mov      r1, r4
00476444: bl       #0x62fc90
00476448: mov      r0, r4
0047644c: bl       #0x619474
00476450: add      sp, sp, #0x10
00476454: pop      {r4, r5, r6, r7, r8, pc}
00476458: ldrsbeq  lr, [r1], #-0x64
0047645c: andeq    r4, r0, r8, lsr #9
00476460: andeq    r4, r0, r0, lsl r7

# _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryERKNS0_16CColladaDatabaseE
0062fc90: push     {r4, lr}
0062fc94: ldr      r3, [r1]
0062fc98: ldr      r2, [r1, #4]
0062fc9c: sub      sp, sp, #8
0062fca0: cmp      r3, #0
0062fca4: mov      r4, r0
0062fca8: str      r2, [sp, #4]
0062fcac: str      r3, [sp]
0062fcb0: beq      #0x62fcc8
0062fcb4: ldr      r2, [r3, #4]
0062fcb8: cmp      r2, #0
0062fcbc: addne    r2, r2, #1
0062fcc0: strne    r2, [r3, #4]
0062fcc4: ldrne    r3, [sp]
0062fcc8: ldr      r0, [sp, #4]
0062fccc: ldr      r1, [r4, #0x68]
0062fcd0: ldr      r2, [r4, #0x6c]
0062fcd4: str      r3, [r4, #0x68]
0062fcd8: str      r0, [r4, #0x6c]
0062fcdc: mov      r0, sp
0062fce0: stm      sp, {r1, r2}
0062fce4: bl       #0x619474
0062fce8: mov      r3, #1
0062fcec: strb     r3, [r4, #0x70]
0062fcf0: add      sp, sp, #8
0062fcf4: pop      {r4, pc}

# _ZN12VisualObject12ApplyMeshBoxEv
00470a54: push     {r4, lr}
00470a58: ldr      r3, [r0, #4]
00470a5c: mov      r1, r0
00470a60: cmp      r3, #0
00470a64: beq      #0x470a80
00470a68: mov      r0, r3
00470a6c: ldrb     r2, [r1, #0x28]
00470a70: ldr      r3, [r3]
00470a74: add      r1, r1, #0x10
00470a78: mov      lr, pc
00470a7c: ldr      pc, [r3, #0x9c]
00470a80: pop      {r4, pc}

# _ZN9Character6ReviveEP10GameObjectb
003a59ac: push     {r4, r5, r6, r7, lr}
003a59b0: movw     r3, #0x1449
003a59b4: ldrb     r3, [r0, r3]
003a59b8: ldr      r5, [pc, #0x11c]
003a59bc: sub      sp, sp, #0x24
003a59c0: cmp      r3, #0
003a59c4: mov      r4, r0
003a59c8: mov      r6, r2
003a59cc: add      r5, pc, r5
003a59d0: bne      #0x3a5a44
003a59d4: mov      r2, #1
003a59d8: movw     r3, #0x1448
003a59dc: strb     r2, [r4, r3]
003a59e0: mov      r7, #0
003a59e4: movw     r3, #0x1449
003a59e8: strb     r7, [r4, r3]
003a59ec: mov      r0, r4
003a59f0: bl       #0x3b3a70
003a59f4: strb     r7, [r4, #0x118]
003a59f8: bl       #0x7fd794
003a59fc: ldrb     r3, [r0, #5]
003a5a00: cmp      r3, r7
003a5a04: mvnne    r3, #0
003a5a08: strne    r3, [r4, #0x110]
003a5a0c: movne    r3, #0
003a5a10: strne    r3, [r4, #0x114]
003a5a14: cmp      r6, #0
003a5a18: bne      #0x3a5ad0
003a5a1c: ldr      r3, [r4]
003a5a20: mov      r0, r4
003a5a24: mov      lr, pc
003a5a28: ldr      pc, [r3, #0x28]
003a5a2c: cmp      r0, #0
003a5a30: bne      #0x3a5a54
003a5a34: add      r0, r4, #0x3c8
003a5a38: bl       #0x3d8894
003a5a3c: add      sp, sp, #0x24
003a5a40: pop      {r4, r5, r6, r7, pc}
003a5a44: mov      r1, #3
003a5a48: mov      r2, #0
003a5a4c: bl       #0x3a4d5c
003a5a50: b        #0x3a59d4
003a5a54: bl       #0x7fd794
003a5a58: ldrb     ip, [r0, #5]
003a5a5c: cmp      ip, #0
003a5a60: bne      #0x3a5a34
003a5a64: ldr      r3, [pc, #0x74]
003a5a68: add      r2, sp, #0x1c
003a5a6c: ldr      r0, [r5, r3]
003a5a70: movw     r3, #0x147c
003a5a74: ldr      lr, [r4, r3]
003a5a78: movw     r3, #0x1474
003a5a7c: ldr      r7, [r4, r3]
003a5a80: movw     r3, #0x1478
003a5a84: ldr      r6, [r4, r3]
003a5a88: add      r5, sp, #0x10
003a5a8c: mov      r3, ip
003a5a90: mov      r1, r5
003a5a94: str      r7, [sp, #0x10]
003a5a98: str      r6, [sp, #0x14]
003a5a9c: str      lr, [sp, #0x1c]
003a5aa0: str      lr, [sp, #0x18]
003a5aa4: str      ip, [sp]
003a5aa8: str      ip, [sp, #4]
003a5aac: str      ip, [sp, #8]
003a5ab0: bl       #0x525508
003a5ab4: ldr      r3, [sp, #0x1c]
003a5ab8: mov      r1, r5
003a5abc: mov      r0, r4
003a5ac0: mov      r2, #1
003a5ac4: str      r3, [sp, #0x18]
003a5ac8: bl       #0x393db4
003a5acc: b        #0x3a5a34
003a5ad0: mov      r0, r4
003a5ad4: bl       #0x3b4088
003a5ad8: b        #0x3a5a1c
003a5adc: subseq   pc, lr, r4, asr #1
003a5ae0: andeq    r1, r0, r4, lsl #4

# _ZN12VisualObjectC1EP10GameObjectRKSsS3_
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
00472af4: bl       #0x50a564
00472af8: ldr      ip, [r8, #0x10]
00472afc: ldr      r2, [r8, #0x14]
00472b00: ldr      r1, [sl, #0x14]
00472b04: mov      r3, r5
00472b08: cmp      ip, r2
00472b0c: moveq    r2, r5
00472b10: mvn      ip, #0x80000000
00472b14: str      ip, [sp]
00472b18: bl       #0x50a504
00472b1c: cmp      r0, r5
00472b20: str      r0, [r4, #8]
00472b24: beq      #0x472c40
00472b28: mov      r1, r7
00472b2c: mov      r0, r4
00472b30: bl       #0x47295c
00472b34: ldr      r0, [r4, #8]
00472b38: bl       #0x35c854
00472b3c: mov      r0, r4
00472b40: bl       #0x4718f0
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
00472b74: bl       #0x350ee0
00472b78: ldr      r3, [r5, #0x10]
00472b7c: ldr      r2, [pc, #0xd4]
00472b80: ldr      r1, [r4, #8]
00472b84: ldr      r0, [r3, #0x1c]
00472b88: add      r2, pc, r2
00472b8c: mov      r3, #1
00472b90: bl       #0x35a0e4
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
00472c0c: bl       #0x47211c
00472c10: mov      r0, r4
00472c14: bl       #0x470a54
00472c18: mov      r1, #0
00472c1c: mov      r0, #8
00472c20: bl       #0x310570
00472c24: ldr      r1, [r4, #8]
00472c28: mov      r5, r0
00472c2c: mov      r2, #0
00472c30: bl       #0x474d30
00472c34: mov      r0, r4
00472c38: mov      r1, r5
00472c3c: bl       #0x470a84
00472c40: mov      r0, r4
00472c44: add      sp, sp, #0xc
00472c48: pop      {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq   r2, r2, r4, ror r0
00472c50: muleq    r0, r4, lr
00472c54: strdeq   r3, r4, [r0], -r4
00472c58: subeq    sl, r5, r0, lsr #22
