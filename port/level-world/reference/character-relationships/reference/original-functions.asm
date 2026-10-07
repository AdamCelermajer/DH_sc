
# _ZNK6CharAI11AI_IsFriendEPK10GameObject
003d511c: push     {r4, r5, r6, r7, lr}
003d5120: ldr      r4, [pc, #0x280]
003d5124: cmp      r1, #0
003d5128: sub      sp, sp, #0x1c
003d512c: mov      r5, r0
003d5130: add      r4, pc, r4
003d5134: beq      #0x3d5330
003d5138: add      r6, sp, #0xc
003d513c: mov      r0, r6
003d5140: bl       #0x33dd70
003d5144: mov      r0, r6
003d5148: mov      r1, #0
003d514c: bl       #0x33ff8c
003d5150: subs     r6, r0, #0
003d5154: bne      #0x3d5164
003d5158: mov      r0, #0
003d515c: add      sp, sp, #0x1c
003d5160: pop      {r4, r5, r6, r7, pc}
003d5164: ldr      r7, [r6, #0xf4]
003d5168: cmp      r7, #0
003d516c: bne      #0x3d5158
003d5170: bl       #0x3a3180
003d5174: cmp      r0, #0
003d5178: blt      #0x3d52dc
003d517c: mov      r0, r6
003d5180: bl       #0x3a3180
003d5184: ldr      r7, [pc, #0x220]
003d5188: ldr      r3, [r4, r7]
003d518c: ldr      r3, [r3]
003d5190: cmp      r0, r3
003d5194: blt      #0x3d51bc
003d5198: ldr      r3, [pc, #0x210]
003d519c: ldr      r3, [r4, r3]
003d51a0: ldr      r3, [r3]
003d51a4: cmp      r3, #2
003d51a8: moveq    r3, #0
003d51ac: streq    r3, [r3]
003d51b0: beq      #0x3d51bc
003d51b4: cmp      r3, #1
003d51b8: beq      #0x3d5374
003d51bc: ldr      r0, [r5, #4]
003d51c0: bl       #0x3a3180
003d51c4: cmp      r0, #0
003d51c8: blt      #0x3d5284
003d51cc: ldr      r0, [r5, #4]
003d51d0: bl       #0x3a3180
003d51d4: ldr      r3, [r4, r7]
003d51d8: ldr      r3, [r3]
003d51dc: cmp      r0, r3
003d51e0: blt      #0x3d5208
003d51e4: ldr      r3, [pc, #0x1c4]
003d51e8: ldr      r3, [r4, r3]
003d51ec: ldr      r3, [r3]
003d51f0: cmp      r3, #2
003d51f4: moveq    r3, #0
003d51f8: streq    r3, [r3]
003d51fc: beq      #0x3d5208
003d5200: cmp      r3, #1
003d5204: beq      #0x3d5340
003d5208: ldr      r3, [pc, #0x1a4]
003d520c: ldr      r0, [r5, #4]
003d5210: ldr      r3, [r4, r3]
003d5214: ldr      r4, [r3]
003d5218: bl       #0x3a3180
003d521c: mov      r3, #0xc
003d5220: mla      r4, r3, r0, r4
003d5224: mov      r0, r6
003d5228: bl       #0x3a3180
003d522c: ldr      ip, [r4, #4]
003d5230: cmp      ip, #0
003d5234: beq      #0x3d5158
003d5238: ldr      r2, [r4, #8]
003d523c: ldr      r3, [r2, #4]
003d5240: cmp      r0, r3
003d5244: movne    r3, #0
003d5248: bne      #0x3d525c
003d524c: b        #0x3d5270
003d5250: ldr      r1, [r2, #4]
003d5254: cmp      r0, r1
003d5258: beq      #0x3d5270
003d525c: add      r3, r3, #1
003d5260: cmp      r3, ip
003d5264: add      r2, r2, #0xc
003d5268: bne      #0x3d5250
003d526c: b        #0x3d5158
003d5270: ldr      r0, [r2, #8]
003d5274: cmp      r0, #0
003d5278: movle    r0, #0
003d527c: movgt    r0, #1
003d5280: b        #0x3d515c
003d5284: ldr      r3, [pc, #0x124]
003d5288: ldr      r3, [r4, r3]
003d528c: ldr      r3, [r3]
003d5290: cmp      r3, #2
003d5294: moveq    r3, #0
003d5298: streq    r3, [r3]
003d529c: beq      #0x3d51cc
003d52a0: cmp      r3, #1
003d52a4: bne      #0x3d51cc
003d52a8: ldr      r0, [pc, #0x108]
003d52ac: ldr      r1, [pc, #0x108]
003d52b0: ldr      r2, [pc, #0x108]
003d52b4: ldr      r0, [r4, r0]
003d52b8: ldr      r3, [pc, #0x104]
003d52bc: mov      ip, #0xc6
003d52c0: add      r1, pc, r1
003d52c4: add      r2, pc, r2
003d52c8: add      r3, pc, r3
003d52cc: add      r0, r0, #0xa8
003d52d0: str      ip, [sp]
003d52d4: bl       #0x30e004
003d52d8: b        #0x3d51cc
003d52dc: ldr      r3, [pc, #0xcc]
003d52e0: ldr      r3, [r4, r3]
003d52e4: ldr      r3, [r3]
003d52e8: cmp      r3, #2
003d52ec: streq    r7, [r7]
003d52f0: beq      #0x3d517c
003d52f4: cmp      r3, #1
003d52f8: bne      #0x3d517c
003d52fc: ldr      r0, [pc, #0xb4]
003d5300: ldr      r1, [pc, #0xc0]
003d5304: ldr      r2, [pc, #0xc0]
003d5308: ldr      r0, [r4, r0]
003d530c: ldr      r3, [pc, #0xbc]
003d5310: mov      ip, #0xc4
003d5314: add      r1, pc, r1
003d5318: add      r2, pc, r2
003d531c: add      r3, pc, r3
003d5320: add      r0, r0, #0xa8
003d5324: str      ip, [sp]
003d5328: bl       #0x30e004
003d532c: b        #0x3d517c
003d5330: ldr      r1, [r0, #0x40]
003d5334: cmp      r1, #0
003d5338: beq      #0x3d5158
003d533c: b        #0x3d5138
003d5340: ldr      r0, [pc, #0x70]
003d5344: ldr      r1, [pc, #0x88]
003d5348: ldr      r2, [pc, #0x88]
003d534c: ldr      r0, [r4, r0]
003d5350: ldr      r3, [pc, #0x84]
003d5354: mov      ip, #0xc7
003d5358: add      r1, pc, r1
003d535c: add      r2, pc, r2
003d5360: add      r3, pc, r3
003d5364: add      r0, r0, #0xa8
003d5368: str      ip, [sp]
003d536c: bl       #0x30e004
003d5370: b        #0x3d5208
003d5374: ldr      r0, [pc, #0x3c]
003d5378: ldr      r1, [pc, #0x60]
003d537c: ldr      r2, [pc, #0x60]
003d5380: ldr      r0, [r4, r0]
003d5384: ldr      r3, [pc, #0x5c]
003d5388: mov      ip, #0xc5
003d538c: add      r1, pc, r1
003d5390: add      r2, pc, r2
003d5394: add      r3, pc, r3
003d5398: add      r0, r0, #0xa8
003d539c: str      ip, [sp]
003d53a0: bl       #0x30e004
003d53a4: b        #0x3d51bc
003d53a8: subseq   pc, fp, r0, ror #18
003d53ac: andeq    r2, r0, r4, asr #4
003d53b0: andeq    r3, r0, r0, asr #19
003d53b4: andeq    r4, r0, ip, lsr #12
003d53b8: andeq    r1, r0, r0, asr #19
003d53bc: subeq    sb, lr, r8, lsl r1
003d53c0: strheq   r0, [pc], #-0x34
003d53c4: strdeq   r0, r1, [pc], #-0x28
003d53c8: subeq    sb, lr, r4, asr #1
003d53cc: subeq    r0, pc, r0, lsl #6
003d53d0: subeq    r0, pc, r4, lsr #5
003d53d4: subeq    sb, lr, r0, lsl #1
003d53d8: subeq    r0, pc, ip, lsr r3
003d53dc: subeq    r0, pc, r0, ror #4
003d53e0: subeq    sb, lr, ip, asr #32
003d53e4: subeq    r0, pc, r8, lsr #5
003d53e8: subeq    r0, pc, ip, lsr #4

# _ZNK6CharAI10AI_IsEnemyEPK10GameObject
003d574c: push     {r4, r5, r6, r7, r8, lr}
003d5750: ldr      r4, [pc, #0x2fc]
003d5754: subs     r7, r1, #0
003d5758: sub      sp, sp, #0x18
003d575c: mov      r5, r0
003d5760: add      r4, pc, r4
003d5764: beq      #0x3d59d0
003d5768: add      r6, sp, #0xc
003d576c: mov      r0, r6
003d5770: mov      r1, r7
003d5774: bl       #0x33dd70
003d5778: mov      r0, r6
003d577c: mov      r1, #0
003d5780: bl       #0x33ff8c
003d5784: subs     r6, r0, #0
003d5788: bne      #0x3d57bc
003d578c: cmp      r7, #0
003d5790: beq      #0x3d57b0
003d5794: ldr      r3, [r7]
003d5798: mov      r0, r7
003d579c: ldr      r1, [r5, #4]
003d57a0: mov      lr, pc
003d57a4: ldr      pc, [r3, #0x88]
003d57a8: cmp      r0, #0
003d57ac: bne      #0x3d58e4
003d57b0: mov      r0, #0
003d57b4: add      sp, sp, #0x18
003d57b8: pop      {r4, r5, r6, r7, r8, pc}
003d57bc: ldr      r8, [r6, #0xf4]
003d57c0: cmp      r8, #0
003d57c4: bne      #0x3d578c
003d57c8: bl       #0x3a3180
003d57cc: cmp      r0, #0
003d57d0: blt      #0x3d597c
003d57d4: mov      r0, r6
003d57d8: bl       #0x3a3180
003d57dc: ldr      r7, [pc, #0x274]
003d57e0: ldr      r3, [r4, r7]
003d57e4: ldr      r3, [r3]
003d57e8: cmp      r0, r3
003d57ec: blt      #0x3d5814
003d57f0: ldr      r3, [pc, #0x264]
003d57f4: ldr      r3, [r4, r3]
003d57f8: ldr      r3, [r3]
003d57fc: cmp      r3, #2
003d5800: moveq    r3, #0
003d5804: streq    r3, [r3]
003d5808: beq      #0x3d5814
003d580c: cmp      r3, #1
003d5810: beq      #0x3d5a20
003d5814: ldr      r0, [r5, #4]
003d5818: bl       #0x3a3180
003d581c: cmp      r0, #0
003d5820: blt      #0x3d5924
003d5824: ldr      r0, [r5, #4]
003d5828: bl       #0x3a3180
003d582c: ldr      r3, [r4, r7]
003d5830: ldr      r3, [r3]
003d5834: cmp      r0, r3
003d5838: blt      #0x3d5860
003d583c: ldr      r3, [pc, #0x218]
003d5840: ldr      r3, [r4, r3]
003d5844: ldr      r3, [r3]
003d5848: cmp      r3, #2
003d584c: moveq    r3, #0
003d5850: streq    r3, [r3]
003d5854: beq      #0x3d5860
003d5858: cmp      r3, #1
003d585c: beq      #0x3d59ec
003d5860: ldr      r3, [r5, #4]
003d5864: mov      r0, r3
003d5868: ldr      r3, [r3]
003d586c: mov      lr, pc
003d5870: ldr      pc, [r3, #0x28]
003d5874: cmp      r0, #0
003d5878: bne      #0x3d5908
003d587c: ldr      r3, [pc, #0x1dc]
003d5880: ldr      r0, [r5, #4]
003d5884: ldr      r3, [r4, r3]
003d5888: ldr      r4, [r3]
003d588c: bl       #0x3a3180
003d5890: mov      r3, #0xc
003d5894: mla      r4, r3, r0, r4
003d5898: mov      r0, r6
003d589c: bl       #0x3a3180
003d58a0: ldr      ip, [r4, #4]
003d58a4: cmp      ip, #0
003d58a8: beq      #0x3d57b0
003d58ac: ldr      r2, [r4, #8]
003d58b0: ldr      r3, [r2, #4]
003d58b4: cmp      r0, r3
003d58b8: movne    r3, #0
003d58bc: bne      #0x3d58d0
003d58c0: b        #0x3d59e0
003d58c4: ldr      r1, [r2, #4]
003d58c8: cmp      r0, r1
003d58cc: beq      #0x3d59e0
003d58d0: add      r3, r3, #1
003d58d4: cmp      r3, ip
003d58d8: add      r2, r2, #0xc
003d58dc: bne      #0x3d58c4
003d58e0: b        #0x3d57b0
003d58e4: mov      r0, r7
003d58e8: ldr      r1, [r5, #4]
003d58ec: ldr      r3, [r7]
003d58f0: mov      lr, pc
003d58f4: ldr      pc, [r3, #0x90]
003d58f8: cmp      r0, #8
003d58fc: movne    r0, #0
003d5900: moveq    r0, #1
003d5904: b        #0x3d57b4
003d5908: ldr      r3, [r6]
003d590c: mov      r0, r6
003d5910: mov      lr, pc
003d5914: ldr      pc, [r3, #0x28]
003d5918: cmp      r0, #0
003d591c: bne      #0x3d57b0
003d5920: b        #0x3d587c
003d5924: ldr      r3, [pc, #0x130]
003d5928: ldr      r3, [r4, r3]
003d592c: ldr      r3, [r3]
003d5930: cmp      r3, #2
003d5934: moveq    r3, #0
003d5938: streq    r3, [r3]
003d593c: beq      #0x3d5824
003d5940: cmp      r3, #1
003d5944: bne      #0x3d5824
003d5948: ldr      r0, [pc, #0x114]
003d594c: ldr      r1, [pc, #0x114]
003d5950: ldr      r2, [pc, #0x114]
003d5954: ldr      r0, [r4, r0]
003d5958: ldr      r3, [pc, #0x110]
003d595c: movw     ip, #0x109
003d5960: add      r1, pc, r1
003d5964: add      r2, pc, r2
003d5968: add      r3, pc, r3
003d596c: add      r0, r0, #0xa8
003d5970: str      ip, [sp]
003d5974: bl       #0x30e004
003d5978: b        #0x3d5824
003d597c: ldr      r3, [pc, #0xd8]
003d5980: ldr      r3, [r4, r3]
003d5984: ldr      r3, [r3]
003d5988: cmp      r3, #2
003d598c: streq    r8, [r8]
003d5990: beq      #0x3d57d4
003d5994: cmp      r3, #1
003d5998: bne      #0x3d57d4
003d599c: ldr      r0, [pc, #0xc0]
003d59a0: ldr      r1, [pc, #0xcc]
003d59a4: ldr      r2, [pc, #0xcc]
003d59a8: ldr      r0, [r4, r0]
003d59ac: ldr      r3, [pc, #0xc8]
003d59b0: movw     ip, #0x107
003d59b4: add      r1, pc, r1
003d59b8: add      r2, pc, r2
003d59bc: add      r3, pc, r3
003d59c0: add      r0, r0, #0xa8
003d59c4: str      ip, [sp]
003d59c8: bl       #0x30e004
003d59cc: b        #0x3d57d4
003d59d0: ldr      r7, [r0, #0x40]
003d59d4: cmp      r7, #0
003d59d8: beq      #0x3d57b0
003d59dc: b        #0x3d5768
003d59e0: ldr      r0, [r2, #8]
003d59e4: lsr      r0, r0, #0x1f
003d59e8: b        #0x3d57b4
003d59ec: ldr      r0, [pc, #0x70]
003d59f0: ldr      r1, [pc, #0x88]
003d59f4: ldr      r2, [pc, #0x88]
003d59f8: ldr      r0, [r4, r0]
003d59fc: ldr      r3, [pc, #0x84]
003d5a00: movw     ip, #0x10a
003d5a04: add      r1, pc, r1
003d5a08: add      r2, pc, r2
003d5a0c: add      r3, pc, r3
003d5a10: add      r0, r0, #0xa8
003d5a14: str      ip, [sp]
003d5a18: bl       #0x30e004
003d5a1c: b        #0x3d5860
003d5a20: ldr      r0, [pc, #0x3c]
003d5a24: ldr      r1, [pc, #0x60]
003d5a28: ldr      r2, [pc, #0x60]
003d5a2c: ldr      r0, [r4, r0]
003d5a30: ldr      r3, [pc, #0x5c]
003d5a34: mov      ip, #0x108
003d5a38: add      r1, pc, r1
003d5a3c: add      r2, pc, r2
003d5a40: add      r3, pc, r3
003d5a44: add      r0, r0, #0xa8
003d5a48: str      ip, [sp]
003d5a4c: bl       #0x30e004
003d5a50: b        #0x3d5814
003d5a54: subseq   pc, fp, r0, lsr r3
003d5a58: andeq    r2, r0, r4, asr #4
003d5a5c: andeq    r3, r0, r0, asr #19
003d5a60: andeq    r4, r0, ip, lsr #12
003d5a64: andeq    r1, r0, r0, asr #19
003d5a68: subeq    r8, lr, r8, ror sl
003d5a6c: subeq    pc, lr, r4, lsl sp
003d5a70: subeq    pc, lr, r8, asr ip
003d5a74: subeq    r8, lr, r4, lsr #20
003d5a78: subeq    pc, lr, r0, ror #24
003d5a7c: subeq    pc, lr, r4, lsl #24
003d5a80: ldrdeq   r8, sb, [lr], #-0x94
003d5a84: umaaleq  pc, lr, r0, ip
003d5a88: strheq   pc, [lr], #-0xb4
003d5a8c: subeq    r8, lr, r0, lsr #19
