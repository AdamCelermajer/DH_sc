
# _ZNK9Character11GetCharTypeEv
003a3054: push     {r4, lr}
003a3058: bl       #0x3a3024
003a305c: ldr      r0, [r0, #0x38]
003a3060: pop      {r4, pc}

# _ZNK9Character8IsFaerieEv
003a3094: push     {r4, lr}
003a3098: bl       #0x3a3054
003a309c: cmp      r0, #3
003a30a0: movne    r0, #0
003a30a4: moveq    r0, #1
003a30a8: pop      {r4, pc}

# _ZNK9Character16GetCharAnimTableEv
003a3264: ldr      r3, [pc, #0x20]
003a3268: ldr      r2, [pc, #0x20]
003a326c: push     {r4, lr}
003a3270: add      r3, pc, r3
003a3274: ldr      r2, [r3, r2]
003a3278: ldr      r4, [r2]
003a327c: bl       #0x3a3228
003a3280: mov      r3, #0xa0
003a3284: mla      r0, r3, r0, r4
003a3288: pop      {r4, pc}
003a328c: subseq   r1, pc, r0, lsr #16
003a3290: andeq    r4, r0, r4, asr #16

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi9AnimationENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EEN6glitch4core10SAllocatorIS6_LNSB_6memory13E_MEMORY_HINTE0EEEE13insert_uniqueERKS6_
00365340: push     {r4, r5, r6, lr}
00365344: ldr      ip, [r1, #4]
00365348: sub      sp, sp, #0x10
0036534c: mov      r4, r0
00365350: cmp      ip, #0
00365354: mov      r3, r2
00365358: moveq    ip, r1
0036535c: beq      #0x3653b8
00365360: ldr      r6, [r2]
00365364: b        #0x36536c
00365368: mov      ip, r2
0036536c: ldr      r0, [ip, #0x10]
00365370: mov      r5, #1
00365374: cmp      r0, r6
00365378: ldrgt    r2, [ip, #8]
0036537c: ldrle    r2, [ip, #0xc]
00365380: movle    r5, #0
00365384: cmp      r2, #0
00365388: bne      #0x365368
0036538c: cmp      r5, #0
00365390: moveq    r5, ip
00365394: bne      #0x3653b8
00365398: cmp      r6, r0
0036539c: movle    r3, #0
003653a0: strle    r5, [r4]
003653a4: strble   r3, [r4, #4]
003653a8: bgt      #0x365420
003653ac: mov      r0, r4
003653b0: add      sp, sp, #0x10
003653b4: pop      {r4, r5, r6, pc}
003653b8: ldr      r2, [r1, #8]
003653bc: cmp      ip, r2
003653c0: beq      #0x3654a0
003653c4: ldrb     r2, [ip]
003653c8: cmp      r2, #0
003653cc: bne      #0x3653e0
003653d0: ldr      r2, [ip, #4]
003653d4: ldr      r2, [r2, #4]
003653d8: cmp      ip, r2
003653dc: beq      #0x36548c
003653e0: ldr      r0, [ip, #8]
003653e4: cmp      r0, #0
003653e8: bne      #0x3653f4
003653ec: b        #0x36544c
003653f0: mov      r0, r2
003653f4: ldr      r2, [r0, #0xc]
003653f8: cmp      r2, #0
003653fc: bne      #0x3653f0
00365400: ldr      r6, [r3]
00365404: mov      r5, r0
00365408: ldr      r0, [r0, #0x10]
0036540c: cmp      r6, r0
00365410: movle    r3, #0
00365414: strle    r5, [r4]
00365418: strble   r3, [r4, #4]
0036541c: ble      #0x3653ac
00365420: mov      r2, ip
00365424: add      r0, sp, #8
00365428: mov      ip, #0
0036542c: str      ip, [sp, #4]
00365430: str      ip, [sp]
00365434: bl       #0x36517c
00365438: ldr      r3, [sp, #8]
0036543c: mov      r2, #1
00365440: strb     r2, [r4, #4]
00365444: str      r3, [r4]
00365448: b        #0x3653ac
0036544c: ldr      r2, [ip, #4]
00365450: ldr      r0, [r2, #8]
00365454: cmp      ip, r0
00365458: movne    r5, r2
0036545c: ldrne    r6, [r3]
00365460: ldrne    r0, [r2, #0x10]
00365464: beq      #0x365470
00365468: b        #0x365398
0036546c: mov      r2, r5
00365470: ldr      r5, [r2, #4]
00365474: ldr      r0, [r5, #8]
00365478: cmp      r0, r2
0036547c: beq      #0x36546c
00365480: ldr      r6, [r3]
00365484: ldr      r0, [r5, #0x10]
00365488: b        #0x365398
0036548c: ldr      r2, [ip, #0xc]
00365490: ldr      r6, [r3]
00365494: mov      r5, r2
00365498: ldr      r0, [r2, #0x10]
0036549c: b        #0x365398
003654a0: mov      r2, ip
003654a4: mov      lr, #0
003654a8: add      r0, sp, #0xc
003654ac: stm      sp, {ip, lr}
003654b0: bl       #0x36517c
003654b4: ldr      r3, [sp, #0xc]
003654b8: mov      r2, #1
003654bc: strb     r2, [r4, #4]
003654c0: str      r3, [r4]
003654c4: b        #0x3653ac

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

# _ZNK6glitch7collada13CAnimationSet17getAnimationCountEv
0065f044: ldr      r3, [r0, #0x24]
0065f048: ldr      r0, [r0, #0x28]
0065f04c: rsb      r0, r3, r0
0065f050: asr      r0, r0, #3
0065f054: bx       lr

# _ZNK9Character16GetCharSkillListEv
003bc5fc: ldr      r3, [pc, #0x20]
003bc600: ldr      r2, [pc, #0x20]
003bc604: push     {r4, lr}
003bc608: add      r3, pc, r3
003bc60c: ldr      r2, [r3, r2]
003bc610: ldr      r4, [r2]
003bc614: bl       #0x3bc5c0
003bc618: mov      r3, #0xc
003bc61c: mla      r0, r3, r0, r4
003bc620: pop      {r4, pc}
003bc624: subseq   r8, sp, r8, lsl #9
003bc628: andeq    r1, r0, r8, asr #3

# _ZNK9Character16GetCharModelNameEv
003a54d4: push     {r4, r5, r6, lr}
003a54d8: sub      sp, sp, #8
003a54dc: mov      r6, r0
003a54e0: bl       #0x3a31e8
003a54e4: ldr      r4, [pc, #0x194]
003a54e8: cmn      r0, #1
003a54ec: mov      r5, r0
003a54f0: add      r4, pc, r4
003a54f4: beq      #0x3a5534
003a54f8: mov      r0, r6
003a54fc: bl       #0x3a3094
003a5500: cmp      r0, #0
003a5504: bne      #0x3a5540
003a5508: ldr      r3, [r6]
003a550c: mov      r0, r6
003a5510: mov      lr, pc
003a5514: ldr      pc, [r3, #0x28]
003a5518: cmp      r0, #0
003a551c: beq      #0x3a552c
003a5520: bl       #0x38174c
003a5524: cmp      r0, #0
003a5528: beq      #0x3a559c
003a552c: cmp      r5, #0
003a5530: bge      #0x3a556c
003a5534: mov      r0, #0
003a5538: add      sp, sp, #8
003a553c: pop      {r4, r5, r6, pc}
003a5540: ldr      r6, [r6, #0x418]
003a5544: cmp      r6, #0
003a5548: beq      #0x3a552c
003a554c: mvn      r1, #0
003a5550: mov      r0, r6
003a5554: bl       #0x3bb98c
003a5558: mov      r1, r0
003a555c: mov      r0, r6
003a5560: bl       #0x3aeac0
003a5564: ldr      r5, [r0, #0xc]
003a5568: b        #0x3a552c
003a556c: ldr      r3, [pc, #0x110]
003a5570: ldr      r3, [r4, r3]
003a5574: ldr      r3, [r3]
003a5578: cmp      r5, r3
003a557c: bge      #0x3a5534
003a5580: ldr      r3, [pc, #0x100]
003a5584: mov      r2, #0xc
003a5588: ldr      r3, [r4, r3]
003a558c: ldr      r3, [r3]
003a5590: mla      r5, r2, r5, r3
003a5594: ldr      r0, [r5, #8]
003a5598: b        #0x3a5538
003a559c: ldr      r3, [pc, #0xe8]
003a55a0: mov      r1, r6
003a55a4: ldr      r3, [r4, r3]
003a55a8: ldr      r0, [r3, #0x40]
003a55ac: bl       #0x36effc
003a55b0: cmp      r0, #0
003a55b4: bne      #0x3a552c
003a55b8: movw     r3, #0x13c8
003a55bc: ldrsh    r2, [r6, r3]
003a55c0: sub      r3, r2, #0x120
003a55c4: sub      r3, r3, #2
003a55c8: cmp      r3, #2
003a55cc: bls      #0x3a5604
003a55d0: sub      r3, r2, #0x144
003a55d4: sub      r3, r3, #1
003a55d8: cmp      r3, #2
003a55dc: bls      #0x3a5618
003a55e0: sub      r2, r2, #0x104
003a55e4: sub      r2, r2, #3
003a55e8: cmp      r2, #2
003a55ec: bhi      #0x3a562c
003a55f0: ldr      r3, [pc, #0x90]
003a55f4: ldr      r3, [r4, r3]
003a55f8: ldr      r3, [r3]
003a55fc: ldr      r0, [r3, #0x3a4]
003a5600: b        #0x3a5538
003a5604: ldr      r3, [pc, #0x7c]
003a5608: ldr      r3, [r4, r3]
003a560c: ldr      r3, [r3]
003a5610: ldr      r0, [r3, #0x38c]
003a5614: b        #0x3a5538
003a5618: ldr      r3, [pc, #0x68]
003a561c: ldr      r3, [r4, r3]
003a5620: ldr      r3, [r3]
003a5624: ldr      r0, [r3, #0x398]
003a5628: b        #0x3a5538
003a562c: ldr      r3, [pc, #0x5c]
003a5630: ldr      r3, [r4, r3]
003a5634: ldr      r3, [r3]
003a5638: cmp      r3, #2
003a563c: streq    r0, [r0]
003a5640: beq      #0x3a552c
003a5644: cmp      r3, #1
003a5648: bne      #0x3a552c
003a564c: ldr      r0, [pc, #0x40]
003a5650: ldr      r1, [pc, #0x40]
003a5654: ldr      r2, [pc, #0x40]
003a5658: ldr      r0, [r4, r0]
003a565c: ldr      r3, [pc, #0x3c]
003a5660: movw     ip, #0x4a7
003a5664: add      r1, pc, r1
003a5668: add      r2, pc, r2
003a566c: add      r3, pc, r3
003a5670: add      r0, r0, #0xa8
003a5674: str      ip, [sp]
003a5678: bl       #0x30e004
003a567c: b        #0x3a552c
003a5680: subseq   pc, lr, r0, lsr #11
003a5684: andeq    r3, r0, ip, lsl #24
003a5688: andeq    r4, r0, r4, asr #6
003a568c: strdeq   r3, r4, [r0], -r4
003a5690: andeq    r3, r0, r0, asr #19
003a5694: andeq    r1, r0, r0, asr #19
003a5698: subseq   r8, r1, r4, ror sp
003a569c: subseq   r8, r1, r0, lsl #30
003a56a0: subseq   sp, r1, r4, lsr fp

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

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi9AnimationENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EEN6glitch4core10SAllocatorIS6_LNSB_6memory13E_MEMORY_HINTE0EEEE9_M_insertEPNS_18_Rb_tree_node_baseERKS6_SJ_SJ_
0036517c: cmp      r1, r2
00365180: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00365184: mov      r4, r1
00365188: mov      r5, r2
0036518c: mov      r6, r0
00365190: mov      r7, r3
00365194: beq      #0x36530c
00365198: ldr      r3, [sp, #0x24]
0036519c: cmp      r3, #0
003651a0: beq      #0x365264
003651a4: mov      r1, #0
003651a8: mov      r0, #0x44
003651ac: bl       #0x310568
003651b0: ldr      r3, [r7]
003651b4: mov      r8, r0
003651b8: add      r0, r0, #0x14
003651bc: str      r3, [r8, #0x10]
003651c0: str      r0, [r8, #0x24]
003651c4: str      r0, [r8, #0x28]
003651c8: ldr      r2, [r7, #0x14]
003651cc: ldr      r1, [r7, #0x18]
003651d0: bl       #0x3116e8
003651d4: ldr      r3, [r7, #0x1c]
003651d8: str      r3, [r8, #0x2c]
003651dc: ldr      r2, [r7, #0x20]
003651e0: cmp      r3, #0
003651e4: str      r2, [r8, #0x30]
003651e8: beq      #0x3651fc
003651ec: ldr      r2, [r3, #4]
003651f0: cmp      r2, #0
003651f4: addne    r2, r2, #1
003651f8: strne    r2, [r3, #4]
003651fc: ldr      r2, [r7, #0x24]
00365200: mov      r3, #0
00365204: mov      sl, r8
00365208: str      r2, [r8, #0x34]
0036520c: ldr      r2, [r7, #0x28]
00365210: str      r2, [r8, #0x38]
00365214: ldr      r2, [r7, #0x2c]
00365218: str      r2, [r8, #0x3c]
0036521c: ldr      r2, [r7, #0x30]
00365220: str      r3, [r8, #0xc]
00365224: str      r3, [r8, #8]
00365228: str      r2, [r8, #0x40]
0036522c: str      r8, [r5, #0xc]
00365230: ldr      r3, [r4, #0xc]
00365234: cmp      r5, r3
00365238: streq    r8, [r4, #0xc]
0036523c: mov      r0, sl
00365240: str      r5, [sl, #4]
00365244: add      r1, r4, #4
00365248: bl       #0x313760
0036524c: ldr      r3, [r4, #0x10]
00365250: mov      r0, r6
00365254: add      r3, r3, #1
00365258: str      r3, [r4, #0x10]
0036525c: str      sl, [r6]
00365260: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00365264: ldr      r3, [sp, #0x20]
00365268: cmp      r3, #0
0036526c: beq      #0x36532c
00365270: mov      r1, #0
00365274: mov      r0, #0x44
00365278: bl       #0x310568
0036527c: ldr      r3, [r7]
00365280: mov      r8, r0
00365284: add      r0, r0, #0x14
00365288: str      r3, [r8, #0x10]
0036528c: str      r0, [r8, #0x24]
00365290: str      r0, [r8, #0x28]
00365294: ldr      r2, [r7, #0x14]
00365298: ldr      r1, [r7, #0x18]
0036529c: bl       #0x3116e8
003652a0: ldr      r2, [r7, #0x1c]
003652a4: str      r2, [r8, #0x2c]
003652a8: ldr      r3, [r7, #0x20]
003652ac: cmp      r2, #0
003652b0: str      r3, [r8, #0x30]
003652b4: beq      #0x3652c8
003652b8: ldr      r3, [r2, #4]
003652bc: cmp      r3, #0
003652c0: addne    r3, r3, #1
003652c4: strne    r3, [r2, #4]
003652c8: ldr      r2, [r7, #0x24]
003652cc: mov      r3, #0
003652d0: mov      sl, r8
003652d4: str      r2, [r8, #0x34]
003652d8: ldr      r2, [r7, #0x28]
003652dc: str      r2, [r8, #0x38]
003652e0: ldr      r2, [r7, #0x2c]
003652e4: str      r2, [r8, #0x3c]
003652e8: ldr      r2, [r7, #0x30]
003652ec: str      r3, [r8, #0xc]
003652f0: str      r3, [r8, #8]
003652f4: str      r2, [r8, #0x40]
003652f8: str      r8, [r5, #8]
003652fc: ldr      r3, [r4, #8]
00365300: cmp      r5, r3
00365304: streq    r8, [r4, #8]
00365308: b        #0x36523c
0036530c: mov      r1, r3
00365310: mov      r0, r4
00365314: bl       #0x364df4
00365318: mov      sl, r0
0036531c: str      r0, [r4, #8]
00365320: str      r0, [r4, #4]
00365324: str      r0, [r4, #0xc]
00365328: b        #0x36523c
0036532c: ldr      r2, [r7]
00365330: ldr      r3, [r5, #0x10]
00365334: cmp      r2, r3
00365338: bge      #0x3651a4
0036533c: b        #0x365270

# _ZN12AnimationSet23_UpdateAnimationIndicesEv
00364af4: push     {r4, r5, r6, lr}
00364af8: ldr      r4, [r0, #0x10]
00364afc: mov      r5, r0
00364b00: add      r6, r0, #8
00364b04: cmp      r6, r4
00364b08: beq      #0x364b48
00364b0c: ldr      r0, [r5, #0x20]
00364b10: add      r1, r4, #0x2c
00364b14: bl       #0x62dbb8
00364b18: ldr      r3, [r4, #0xc]
00364b1c: str      r0, [r4, #0x34]
00364b20: cmp      r3, #0
00364b24: bne      #0x364b30
00364b28: b        #0x364b4c
00364b2c: mov      r3, r2
00364b30: ldr      r2, [r3, #8]
00364b34: cmp      r2, #0
00364b38: bne      #0x364b2c
00364b3c: mov      r4, r3
00364b40: cmp      r6, r4
00364b44: bne      #0x364b0c
00364b48: pop      {r4, r5, r6, pc}
00364b4c: ldr      r2, [r4, #4]
00364b50: ldr      r1, [r2, #0xc]
00364b54: cmp      r4, r1
00364b58: bne      #0x364b74
00364b5c: mov      r4, r2
00364b60: ldr      r2, [r2, #4]
00364b64: ldr      r3, [r2, #0xc]
00364b68: cmp      r3, r4
00364b6c: beq      #0x364b5c
00364b70: ldr      r3, [r4, #0xc]
00364b74: cmp      r3, r2
00364b78: movne    r4, r2
00364b7c: b        #0x364b04

# _ZNSt6vectorIN6glitch7collada16CColladaDatabaseENS0_4core10SAllocatorIS2_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS2_
0062ecac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0062ecb0: ldmib    r0, {r3, r6}
0062ecb4: mov      r4, r0
0062ecb8: mov      r5, r1
0062ecbc: cmp      r3, r6
0062ecc0: beq      #0x62ecfc
0062ecc4: ldr      r2, [r1]
0062ecc8: str      r2, [r3]
0062eccc: ldr      r1, [r1, #4]
0062ecd0: cmp      r2, #0
0062ecd4: str      r1, [r3, #4]
0062ecd8: beq      #0x62ecec
0062ecdc: ldr      r3, [r2, #4]
0062ece0: cmp      r3, #0
0062ece4: addne    r3, r3, #1
0062ece8: strne    r3, [r2, #4]
0062ecec: ldr      r3, [r4, #4]
0062ecf0: add      r3, r3, #8
0062ecf4: str      r3, [r4, #4]
0062ecf8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0062ecfc: ldr      r3, [r0]
0062ed00: rsb      r3, r3, r6
0062ed04: asr      r3, r3, #3
0062ed08: cmp      r3, #1
0062ed0c: addhs    r7, r3, r3
0062ed10: addlo    r7, r3, #1
0062ed14: cmn      r7, #0xe0000001
0062ed18: bls      #0x62edfc
0062ed1c: mvn      r7, #7
0062ed20: mov      r0, r7
0062ed24: mov      r1, #0
0062ed28: bl       #0x310568
0062ed2c: ldr      lr, [r4]
0062ed30: mov      r8, r0
0062ed34: rsb      sl, lr, r6
0062ed38: asr      sl, sl, #3
0062ed3c: cmp      sl, #0
0062ed40: movle    sl, r0
0062ed44: ble      #0x62ed90
0062ed48: mov      r0, sl
0062ed4c: mov      ip, #0
0062ed50: mov      r1, lr
0062ed54: ldr      r3, [r1, ip]!
0062ed58: mov      r2, r8
0062ed5c: str      r3, [r2, ip]!
0062ed60: ldr      r1, [r1, #4]
0062ed64: cmp      r3, #0
0062ed68: add      ip, ip, #8
0062ed6c: str      r1, [r2, #4]
0062ed70: beq      #0x62ed84
0062ed74: ldr      r2, [r3, #4]
0062ed78: cmp      r2, #0
0062ed7c: addne    r2, r2, #1
0062ed80: strne    r2, [r3, #4]
0062ed84: subs     r0, r0, #1
0062ed88: bne      #0x62ed50
0062ed8c: add      sl, r8, sl, lsl #3
0062ed90: ldr      r3, [r5]
0062ed94: str      r3, [sl]
0062ed98: ldr      r2, [r5, #4]
0062ed9c: cmp      r3, #0
0062eda0: str      r2, [sl, #4]
0062eda4: beq      #0x62edb8
0062eda8: ldr      r2, [r3, #4]
0062edac: cmp      r2, #0
0062edb0: addne    r2, r2, #1
0062edb4: strne    r2, [r3, #4]
0062edb8: ldr      r5, [r4, #4]
0062edbc: ldr      r6, [r4]
0062edc0: add      sl, sl, #8
0062edc4: cmp      r5, r6
0062edc8: beq      #0x62ede4
0062edcc: sub      r5, r5, #8
0062edd0: mov      r0, r5
0062edd4: bl       #0x619474
0062edd8: cmp      r6, r5
0062eddc: bne      #0x62edcc
0062ede0: ldr      r6, [r4]
0062ede4: mov      r0, r6
0062ede8: add      r7, r8, r7
0062edec: bl       #0x310450
0062edf0: str      r7, [r4, #8]
0062edf4: stm      r4, {r8, sl}
0062edf8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0062edfc: cmp      r3, r7
0062ee00: lslls    r7, r7, #3
0062ee04: bls      #0x62ed20
0062ee08: b        #0x62ed1c

# _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp

# _ZN14AnimSetManager7AddAnimEii
0047653c: push     {r4, r5, r6, r7, r8, sl, lr}
00476540: ldr      r4, [pc, #0x124]
00476544: ldr      r5, [pc, #0x124]
00476548: sub      sp, sp, #0x2c
0047654c: add      r4, pc, r4
00476550: ldr      r3, [r4, r5]
00476554: mov      r7, r0
00476558: str      r1, [sp, #4]
0047655c: ldr      r3, [r3]
00476560: mov      r8, r2
00476564: str      r3, [sp, #0x24]
00476568: bl       #0x475404
0047656c: cmp      r0, #0
00476570: addne    r7, r7, #4
00476574: addne    r6, sp, #4
00476578: bne      #0x4765bc
0047657c: ldr      r3, [sp, #4]
00476580: cmp      r3, #0
00476584: blt      #0x4765d4
00476588: add      r7, r7, #4
0047658c: add      r6, sp, #4
00476590: mov      r1, r6
00476594: mov      r0, r7
00476598: bl       #0x476058
0047659c: mov      sl, r0
004765a0: bl       #0x364ca0
004765a4: ldr      r3, [pc, #0xc8]
004765a8: ldr      r3, [r4, r3]
004765ac: ldrb     r3, [r3]
004765b0: cmp      r3, #0
004765b4: movne    r3, #1
004765b8: strbne   r3, [sl, #0x3c]
004765bc: mov      r0, r7
004765c0: mov      r1, r6
004765c4: bl       #0x476058
004765c8: ldrb     r3, [r0, #0x3c]
004765cc: cmp      r3, #0
004765d0: beq      #0x4765f0
004765d4: ldr      r3, [r4, r5]
004765d8: ldr      r2, [sp, #0x24]
004765dc: ldr      r3, [r3]
004765e0: cmp      r2, r3
004765e4: bne      #0x476668
004765e8: add      sp, sp, #0x2c
004765ec: pop      {r4, r5, r6, r7, r8, sl, pc}
004765f0: mov      r1, r8
004765f4: bl       #0x3659ec
004765f8: ldr      r3, [pc, #0x78]
004765fc: add      r6, sp, #0xc
00476600: ldr      r7, [r4, r3]
00476604: mov      r0, r7
00476608: bl       #0x337888
0047660c: ldr      r1, [pc, #0x68]
00476610: mov      r0, r6
00476614: str      r6, [sp, #0x1c]
00476618: add      r1, pc, r1
0047661c: add      r2, r1, #0x17
00476620: str      r6, [sp, #0x20]
00476624: bl       #0x3116e8
00476628: mov      r0, r7
0047662c: mov      r1, r6
00476630: bl       #0x337a88
00476634: ldr      r0, [sp, #0x20]
00476638: cmp      r0, r6
0047663c: beq      #0x4765d4
00476640: cmp      r0, #0
00476644: beq      #0x4765d4
00476648: ldr      r1, [sp, #0xc]
0047664c: rsb      r1, r0, r1
00476650: cmp      r1, #0x80
00476654: bhi      #0x476660
00476658: bl       #0x708f00
0047665c: b        #0x4765d4
00476660: bl       #0x310440
00476664: b        #0x4765d4
00476668: bl       #0x30e310
0047666c: subseq   lr, r1, r4, asr #10
00476670: andeq    r4, r0, ip, lsr #1
00476674: andeq    r4, r0, r8, lsr #9
00476678: andeq    r0, r0, r4, lsl #17
0047667c: subeq    r7, r5, r0, lsr r2

# _ZN6CSIdle6OnBlurEiP9CharacterP16CharStateMachinei
003c2d3c: ldr      r3, [pc, #0x7c]
003c2d40: ldr      r1, [pc, #0x7c]
003c2d44: push     {r4, r5, r6, r7, lr}
003c2d48: add      r3, pc, r3
003c2d4c: ldr      r5, [r3, r1]
003c2d50: ldr      r1, [pc, #0x70]
003c2d54: mov      r7, r2
003c2d58: ldr      r2, [r5]
003c2d5c: ldr      r6, [r3, r1]
003c2d60: sub      sp, sp, #0x24
003c2d64: str      r2, [sp, #0x1c]
003c2d68: mov      r0, r6
003c2d6c: bl       #0x337888
003c2d70: ldr      r1, [pc, #0x54]
003c2d74: add      r4, sp, #4
003c2d78: mov      r2, sp
003c2d7c: add      r1, pc, r1
003c2d80: mov      r0, r4
003c2d84: bl       #0x3140ec
003c2d88: mov      r1, r4
003c2d8c: mov      r0, r6
003c2d90: bl       #0x337a88
003c2d94: mov      r0, r4
003c2d98: bl       #0x318254
003c2d9c: mov      r3, #0
003c2da0: strb     r3, [r7, #0x538]
003c2da4: ldr      r2, [sp, #0x1c]
003c2da8: ldr      r3, [r5]
003c2dac: cmp      r2, r3
003c2db0: bne      #0x3c2dbc
003c2db4: add      sp, sp, #0x24
003c2db8: pop      {r4, r5, r6, r7, pc}
003c2dbc: bl       #0x30e310
003c2dc0: subseq   r1, sp, r8, asr #26
003c2dc4: andeq    r4, r0, ip, lsr #1
003c2dc8: andeq    r0, r0, r4, lsl #17
003c2dcc: ldrsbeq  r2, [r0], #-4

# _ZN12AnimationSet13CreateAnimSetEv
00364ca0: push     {r4, r5, r6, lr}
00364ca4: mov      r4, r0
00364ca8: ldr      r0, [r0, #0x20]
00364cac: cmp      r0, #0
00364cb0: beq      #0x364cc0
00364cb4: bl       #0x31d584
00364cb8: mov      r3, #0
00364cbc: str      r3, [r4, #0x20]
00364cc0: mov      r6, #1
00364cc4: mov      r1, #0
00364cc8: str      r6, [r4, #0x2c]
00364ccc: mov      r0, #0x80
00364cd0: bl       #0x310570
00364cd4: mov      r5, r0
00364cd8: bl       #0x3648c4
00364cdc: str      r5, [r4, #0x20]
00364ce0: mov      r0, r5
00364ce4: mov      r1, r6
00364ce8: ldr      r3, [r5]
00364cec: mov      lr, pc
00364cf0: ldr      pc, [r3, #0x1c]
00364cf4: pop      {r4, r5, r6, pc}

# _ZNK9Character12GetCharSkillEi
003bc784: push     {r4, r5, r6, lr}
003bc788: sub      sp, sp, #8
003bc78c: mov      r5, r1
003bc790: bl       #0x3bc5c0
003bc794: ldr      r4, [pc, #0xa4]
003bc798: ldr      r3, [pc, #0xa4]
003bc79c: mov      r6, #0xc
003bc7a0: add      r4, pc, r4
003bc7a4: ldr      r3, [r4, r3]
003bc7a8: cmp      r5, #0
003bc7ac: ldr      r3, [r3]
003bc7b0: mla      r6, r6, r0, r3
003bc7b4: blt      #0x3bc7c4
003bc7b8: ldr      r3, [r6, #4]
003bc7bc: cmp      r5, r3
003bc7c0: blt      #0x3bc7e8
003bc7c4: ldr      r3, [pc, #0x7c]
003bc7c8: ldr      r3, [r4, r3]
003bc7cc: ldr      r3, [r3]
003bc7d0: cmp      r3, #2
003bc7d4: moveq    r3, #0
003bc7d8: streq    r3, [r3]
003bc7dc: beq      #0x3bc7e8
003bc7e0: cmp      r3, #1
003bc7e4: beq      #0x3bc80c
003bc7e8: ldr      r3, [pc, #0x5c]
003bc7ec: ldr      r2, [r6, #8]
003bc7f0: mov      r0, #0x4c
003bc7f4: ldr      r3, [r4, r3]
003bc7f8: ldr      r2, [r2, r5, lsl #2]
003bc7fc: ldr      r3, [r3]
003bc800: mla      r0, r0, r2, r3
003bc804: add      sp, sp, #8
003bc808: pop      {r4, r5, r6, pc}
003bc80c: ldr      r0, [pc, #0x3c]
003bc810: ldr      r1, [pc, #0x3c]
003bc814: ldr      r2, [pc, #0x3c]
003bc818: ldr      r0, [r4, r0]
003bc81c: ldr      r3, [pc, #0x38]
003bc820: mov      ip, #0x3d
003bc824: add      r1, pc, r1
003bc828: add      r2, pc, r2
003bc82c: add      r3, pc, r3
003bc830: add      r0, r0, #0xa8
003bc834: str      ip, [sp]
003bc838: bl       #0x30e004
003bc83c: b        #0x3bc7e8
003bc840: ldrsheq  r8, [sp], #-0x20
003bc844: andeq    r1, r0, r8, asr #3
003bc848: andeq    r3, r0, r0, asr #19
003bc84c: andeq    r4, r0, ip, lsl r4
003bc850: andeq    r1, r0, r0, asr #19
003bc854: ldrheq   r1, [r0], #-0xb4
003bc858: subseq   r7, r0, r0, lsr #31
003bc85c: ldrsbeq  r7, [r0], #-0xf4

# _ZN6glitch7collada20CDynamicAnimationSetC1Ev
003648c4: ldr      r1, [pc, #0xa4]
003648c8: ldr      r2, [pc, #0xa4]
003648cc: ldr      r3, [pc, #0xa4]
003648d0: add      r1, pc, r1
003648d4: ldr      r2, [r1, r2]
003648d8: push     {r4, r5}
003648dc: ldr      r4, [r1, r3]
003648e0: add      r5, r2, #8
003648e4: mov      ip, #1
003648e8: mov      r2, #0
003648ec: str      r2, [r0, #0x7c]
003648f0: str      r5, [r0]
003648f4: str      r4, [r0, #0x6c]
003648f8: strb     ip, [r0, #0x70]
003648fc: str      ip, [r0, #4]
00364900: str      r2, [r0, #8]
00364904: str      r2, [r0, #0xc]
00364908: str      r2, [r0, #0x10]
0036490c: str      r2, [r0, #0x14]
00364910: str      r2, [r0, #0x18]
00364914: str      r2, [r0, #0x1c]
00364918: str      r2, [r0, #0x20]
0036491c: str      r2, [r0, #0x24]
00364920: str      r2, [r0, #0x28]
00364924: str      r2, [r0, #0x2c]
00364928: str      r2, [r0, #0x30]
0036492c: str      r2, [r0, #0x34]
00364930: str      r2, [r0, #0x38]
00364934: str      r2, [r0, #0x40]
00364938: str      r2, [r0, #0x44]
0036493c: str      r2, [r0, #0x48]
00364940: str      r2, [r0, #0x4c]
00364944: str      r2, [r0, #0x50]
00364948: str      r2, [r0, #0x54]
0036494c: str      r2, [r0, #0x58]
00364950: str      r2, [r0, #0x5c]
00364954: str      r2, [r0, #0x60]
00364958: str      r2, [r0, #0x64]
0036495c: str      r2, [r0, #0x68]
00364960: str      r2, [r0, #0x74]
00364964: str      r2, [r0, #0x78]
00364968: pop      {r4, r5}
0036496c: bx       lr
00364970: rsbeq    r0, r3, r0, asr #3
00364974: andeq    r3, r0, r4, asr #15
00364978: andeq    r4, r0, r0, lsl r7

# _ZN7Structs5Skill4readEP11IStreamBase
004ebeb0: push     {r4, r5, r6, r7, r8, lr}
004ebeb4: mov      r4, r0
004ebeb8: sub      sp, sp, #8
004ebebc: mov      r0, r1
004ebec0: mov      r5, r1
004ebec4: add      r1, r4, #4
004ebec8: bl       #0x459090
004ebecc: mov      r3, #1
004ebed0: cmp      r3, #0
004ebed4: str      r3, [sp, #4]
004ebed8: bne      #0x4ebf1c
004ebedc: add      r3, r4, #5
004ebee0: add      r2, r4, #6
004ebee4: ldrb     r0, [r2, #1]
004ebee8: ldrb     r1, [r3, #-1]
004ebeec: cmp      r3, r2
004ebef0: eor      r1, r0, r1
004ebef4: strb     r1, [r3, #-1]
004ebef8: ldrb     r0, [r2, #1]
004ebefc: eor      r1, r1, r0
004ebf00: strb     r1, [r2, #1]
004ebf04: ldrb     r0, [r3, #-1]
004ebf08: sub      r2, r2, #1
004ebf0c: eor      r1, r1, r0
004ebf10: strb     r1, [r3, #-1]
004ebf14: add      r3, r3, #1
004ebf18: blo      #0x4ebee4
004ebf1c: add      r1, r4, #8
004ebf20: mov      r0, r5
004ebf24: bl       #0x4db89c
004ebf28: mov      r0, r5
004ebf2c: add      r1, r4, #0xc
004ebf30: bl       #0x3df1a0
004ebf34: mov      r3, #1
004ebf38: cmp      r3, #0
004ebf3c: str      r3, [sp, #4]
004ebf40: bne      #0x4ebf84
004ebf44: add      r3, r4, #0xd
004ebf48: add      r2, r4, #0xe
004ebf4c: ldrb     r0, [r2, #1]
004ebf50: ldrb     r1, [r3, #-1]
004ebf54: cmp      r3, r2
004ebf58: eor      r1, r0, r1
004ebf5c: strb     r1, [r3, #-1]
004ebf60: ldrb     r0, [r2, #1]
004ebf64: eor      r1, r1, r0
004ebf68: strb     r1, [r2, #1]
004ebf6c: ldrb     r0, [r3, #-1]
004ebf70: sub      r2, r2, #1
004ebf74: eor      r1, r1, r0
004ebf78: strb     r1, [r3, #-1]
004ebf7c: add      r3, r3, #1
004ebf80: blo      #0x4ebf4c
004ebf84: ldr      r0, [r4, #0x10]
004ebf88: cmp      r0, #0
004ebf8c: beq      #0x4ebf94
004ebf90: bl       #0x310440
004ebf94: ldr      r0, [r4, #0xc]
004ebf98: mov      r1, #1
004ebf9c: lsl      r0, r0, #2
004ebfa0: bl       #0x31056c
004ebfa4: ldr      r3, [r4, #0xc]
004ebfa8: str      r0, [r4, #0x10]
004ebfac: cmp      r3, #0
004ebfb0: beq      #0x4ec034
004ebfb4: mov      r6, #0
004ebfb8: mov      r8, #1
004ebfbc: lsl      r7, r6, #2
004ebfc0: add      r1, r0, r7
004ebfc4: mov      r0, r5
004ebfc8: bl       #0x459090
004ebfcc: str      r8, [sp, #4]
004ebfd0: cmp      r8, #0
004ebfd4: ldr      r3, [r4, #0x10]
004ebfd8: bne      #0x4ec020
004ebfdc: add      r7, r3, r7
004ebfe0: add      r3, r7, #2
004ebfe4: add      r7, r7, #1
004ebfe8: ldrb     r1, [r3, #1]
004ebfec: ldrb     r2, [r7, #-1]
004ebff0: cmp      r7, r3
004ebff4: eor      r2, r1, r2
004ebff8: strb     r2, [r7, #-1]
004ebffc: ldrb     r1, [r3, #1]
004ec000: eor      r2, r2, r1
004ec004: strb     r2, [r3, #1]
004ec008: ldrb     r1, [r7, #-1]
004ec00c: sub      r3, r3, #1
004ec010: eor      r2, r2, r1
004ec014: strb     r2, [r7, #-1]
004ec018: add      r7, r7, #1
004ec01c: blo      #0x4ebfe8
004ec020: ldr      r3, [r4, #0xc]
004ec024: add      r6, r6, #1
004ec028: cmp      r3, r6
004ec02c: ldrhi    r0, [r4, #0x10]
004ec030: bhi      #0x4ebfbc
004ec034: mov      r0, r5
004ec038: add      r1, r4, #0x14
004ec03c: bl       #0x459090
004ec040: mov      r3, #1
004ec044: cmp      r3, #0
004ec048: str      r3, [sp, #4]
004ec04c: bne      #0x4ec090
004ec050: add      r3, r4, #0x15
004ec054: add      r2, r4, #0x16
004ec058: ldrb     r0, [r2, #1]
004ec05c: ldrb     r1, [r3, #-1]
004ec060: cmp      r3, r2
004ec064: eor      r1, r0, r1
004ec068: strb     r1, [r3, #-1]
004ec06c: ldrb     r0, [r2, #1]
004ec070: eor      r1, r1, r0
004ec074: strb     r1, [r2, #1]
004ec078: ldrb     r0, [r3, #-1]
004ec07c: sub      r2, r2, #1
004ec080: eor      r1, r1, r0
004ec084: strb     r1, [r3, #-1]
004ec088: add      r3, r3, #1
004ec08c: blo      #0x4ec058
004ec090: add      r1, r4, #0x18
004ec094: mov      r0, r5
004ec098: bl       #0x4db89c
004ec09c: mov      r0, r5
004ec0a0: add      r1, r4, #0x1c
004ec0a4: bl       #0x459090
004ec0a8: mov      r3, #1
004ec0ac: cmp      r3, #0
004ec0b0: str      r3, [sp, #4]
004ec0b4: bne      #0x4ec0f8
004ec0b8: add      r3, r4, #0x1d
004ec0bc: add      r2, r4, #0x1e
004ec0c0: ldrb     r0, [r2, #1]
004ec0c4: ldrb     r1, [r3, #-1]
004ec0c8: cmp      r3, r2
004ec0cc: eor      r1, r0, r1
004ec0d0: strb     r1, [r3, #-1]
004ec0d4: ldrb     r0, [r2, #1]
004ec0d8: eor      r1, r1, r0
004ec0dc: strb     r1, [r2, #1]
004ec0e0: ldrb     r0, [r3, #-1]
004ec0e4: sub      r2, r2, #1
004ec0e8: eor      r1, r1, r0
004ec0ec: strb     r1, [r3, #-1]
004ec0f0: add      r3, r3, #1
004ec0f4: blo      #0x4ec0c0
004ec0f8: mov      r0, r5
004ec0fc: add      r1, r4, #0x20
004ec100: bl       #0x459090
004ec104: mov      r3, #1
004ec108: cmp      r3, #0
004ec10c: str      r3, [sp, #4]
004ec110: bne      #0x4ec154
004ec114: add      r3, r4, #0x21
004ec118: add      r2, r4, #0x22
004ec11c: ldrb     r0, [r2, #1]
004ec120: ldrb     r1, [r3, #-1]
004ec124: cmp      r3, r2
004ec128: eor      r1, r0, r1
004ec12c: strb     r1, [r3, #-1]
004ec130: ldrb     r0, [r2, #1]
004ec134: eor      r1, r1, r0
004ec138: strb     r1, [r2, #1]
004ec13c: ldrb     r0, [r3, #-1]
004ec140: sub      r2, r2, #1
004ec144: eor      r1, r1, r0
004ec148: strb     r1, [r3, #-1]
004ec14c: add      r3, r3, #1
004ec150: blo      #0x4ec11c
004ec154: mov      r0, r5
004ec158: add      r1, r4, #0x24
004ec15c: bl       #0x3df1a0
004ec160: mov      r3, #1
004ec164: cmp      r3, #0
004ec168: str      r3, [sp, #4]
004ec16c: bne      #0x4ec1b0
004ec170: add      r3, r4, #0x25
004ec174: add      r2, r4, #0x26
004ec178: ldrb     r0, [r2, #1]
004ec17c: ldrb     r1, [r3, #-1]
004ec180: cmp      r3, r2
004ec184: eor      r1, r0, r1
004ec188: strb     r1, [r3, #-1]
004ec18c: ldrb     r0, [r2, #1]
004ec190: eor      r1, r1, r0
004ec194: strb     r1, [r2, #1]
004ec198: ldrb     r0, [r3, #-1]
004ec19c: sub      r2, r2, #1
004ec1a0: eor      r1, r1, r0
004ec1a4: strb     r1, [r3, #-1]
004ec1a8: add      r3, r3, #1
004ec1ac: blo      #0x4ec178
004ec1b0: ldr      r0, [r4, #0x28]
004ec1b4: cmp      r0, #0
004ec1b8: beq      #0x4ec1c0
004ec1bc: bl       #0x310440
004ec1c0: ldr      r0, [r4, #0x24]
004ec1c4: mov      r1, #1
004ec1c8: mov      r6, #0
004ec1cc: add      r0, r0, r1
004ec1d0: bl       #0x31056c
004ec1d4: ldr      r2, [r4, #0x24]
004ec1d8: mov      r1, r0
004ec1dc: str      r0, [r4, #0x28]
004ec1e0: mov      r3, r6
004ec1e4: mov      r0, r5
004ec1e8: bl       #0x317454
004ec1ec: ldr      r3, [r4, #0x24]
004ec1f0: ldr      r2, [r4, #0x28]
004ec1f4: add      r1, r4, #0x2c
004ec1f8: mov      r0, r5
004ec1fc: strb     r6, [r2, r3]
004ec200: bl       #0x4db89c
004ec204: mov      r0, r5
004ec208: add      r1, r4, #0x30
004ec20c: bl       #0x459090
004ec210: mov      r3, #1
004ec214: cmp      r3, r6
004ec218: str      r3, [sp, #4]
004ec21c: bne      #0x4ec260
004ec220: add      r3, r4, #0x31
004ec224: add      r2, r4, #0x32
004ec228: ldrb     r0, [r2, #1]
004ec22c: ldrb     r1, [r3, #-1]
004ec230: cmp      r3, r2
004ec234: eor      r1, r0, r1
004ec238: strb     r1, [r3, #-1]
004ec23c: ldrb     r0, [r2, #1]
004ec240: eor      r1, r1, r0
004ec244: strb     r1, [r2, #1]
004ec248: ldrb     r0, [r3, #-1]
004ec24c: sub      r2, r2, #1
004ec250: eor      r1, r1, r0
004ec254: strb     r1, [r3, #-1]
004ec258: add      r3, r3, #1
004ec25c: blo      #0x4ec228
004ec260: mov      r0, r5
004ec264: add      r1, r4, #0x34
004ec268: bl       #0x459090
004ec26c: mov      r3, #1
004ec270: cmp      r3, #0
004ec274: str      r3, [sp, #4]
004ec278: bne      #0x4ec2bc
004ec27c: add      r3, r4, #0x35
004ec280: add      r2, r4, #0x36
004ec284: ldrb     r0, [r2, #1]
004ec288: ldrb     r1, [r3, #-1]
004ec28c: cmp      r3, r2
004ec290: eor      r1, r0, r1
004ec294: strb     r1, [r3, #-1]
004ec298: ldrb     r0, [r2, #1]
004ec29c: eor      r1, r1, r0
004ec2a0: strb     r1, [r2, #1]
004ec2a4: ldrb     r0, [r3, #-1]
004ec2a8: sub      r2, r2, #1
004ec2ac: eor      r1, r1, r0
004ec2b0: strb     r1, [r3, #-1]
004ec2b4: add      r3, r3, #1
004ec2b8: blo      #0x4ec284
004ec2bc: mov      r0, r5
004ec2c0: add      r1, r4, #0x38
004ec2c4: bl       #0x3df1a0
004ec2c8: mov      r3, #1
004ec2cc: cmp      r3, #0
004ec2d0: str      r3, [sp, #4]
004ec2d4: bne      #0x4ec318
004ec2d8: add      r3, r4, #0x39
004ec2dc: add      r2, r4, #0x3a
004ec2e0: ldrb     r0, [r2, #1]
004ec2e4: ldrb     r1, [r3, #-1]
004ec2e8: cmp      r3, r2
004ec2ec: eor      r1, r0, r1
004ec2f0: strb     r1, [r3, #-1]
004ec2f4: ldrb     r0, [r2, #1]
004ec2f8: eor      r1, r1, r0
004ec2fc: strb     r1, [r2, #1]
004ec300: ldrb     r0, [r3, #-1]
004ec304: sub      r2, r2, #1
004ec308: eor      r1, r1, r0
004ec30c: strb     r1, [r3, #-1]
004ec310: add      r3, r3, #1
004ec314: blo      #0x4ec2e0
004ec318: ldr      r0, [r4, #0x3c]
004ec31c: cmp      r0, #0
004ec320: beq      #0x4ec328
004ec324: bl       #0x310440
004ec328: ldr      r0, [r4, #0x38]
004ec32c: mov      r1, #1
004ec330: mov      r6, #0
004ec334: add      r0, r0, r1
004ec338: bl       #0x31056c
004ec33c: ldr      r2, [r4, #0x38]
004ec340: mov      r1, r0
004ec344: str      r0, [r4, #0x3c]
004ec348: mov      r3, r6
004ec34c: mov      r0, r5
004ec350: bl       #0x317454
004ec354: ldr      r3, [r4, #0x38]
004ec358: ldr      r2, [r4, #0x3c]
004ec35c: mov      r0, r5
004ec360: add      r1, r4, #0x40
004ec364: strb     r6, [r2, r3]
004ec368: bl       #0x459090
004ec36c: mov      r3, #1
004ec370: cmp      r3, r6
004ec374: str      r3, [sp, #4]
004ec378: bne      #0x4ec3bc
004ec37c: add      r3, r4, #0x41
004ec380: add      r2, r4, #0x42
004ec384: ldrb     r0, [r2, #1]
004ec388: ldrb     r1, [r3, #-1]
004ec38c: cmp      r3, r2
004ec390: eor      r1, r0, r1
004ec394: strb     r1, [r3, #-1]
004ec398: ldrb     r0, [r2, #1]
004ec39c: eor      r1, r1, r0
004ec3a0: strb     r1, [r2, #1]
004ec3a4: ldrb     r0, [r3, #-1]
004ec3a8: sub      r2, r2, #1
004ec3ac: eor      r1, r1, r0
004ec3b0: strb     r1, [r3, #-1]
004ec3b4: add      r3, r3, #1
004ec3b8: blo      #0x4ec384
004ec3bc: mov      r0, r5
004ec3c0: add      r1, r4, #0x44
004ec3c4: bl       #0x459090
004ec3c8: mov      r3, #1
004ec3cc: cmp      r3, #0
004ec3d0: str      r3, [sp, #4]
004ec3d4: bne      #0x4ec418
004ec3d8: add      r3, r4, #0x45
004ec3dc: add      r2, r4, #0x46
004ec3e0: ldrb     r0, [r2, #1]
004ec3e4: ldrb     r1, [r3, #-1]
004ec3e8: cmp      r3, r2
004ec3ec: eor      r1, r0, r1
004ec3f0: strb     r1, [r3, #-1]
004ec3f4: ldrb     r0, [r2, #1]
004ec3f8: eor      r1, r1, r0
004ec3fc: strb     r1, [r2, #1]
004ec400: ldrb     r0, [r3, #-1]
004ec404: sub      r2, r2, #1
004ec408: eor      r1, r1, r0
004ec40c: strb     r1, [r3, #-1]
004ec410: add      r3, r3, #1
004ec414: blo      #0x4ec3e0
004ec418: mov      r0, r5
004ec41c: add      r1, r4, #0x48
004ec420: bl       #0x459090
004ec424: mov      r3, #1
004ec428: cmp      r3, #0
004ec42c: str      r3, [sp, #4]
004ec430: bne      #0x4ec474
004ec434: add      r3, r4, #0x4a
004ec438: add      r4, r4, #0x49
004ec43c: ldrb     r1, [r3, #1]
004ec440: ldrb     r2, [r4, #-1]
004ec444: cmp      r4, r3
004ec448: eor      r2, r1, r2
004ec44c: strb     r2, [r4, #-1]
004ec450: ldrb     r1, [r3, #1]
004ec454: eor      r2, r2, r1
004ec458: strb     r2, [r3, #1]
004ec45c: ldrb     r1, [r4, #-1]
004ec460: sub      r3, r3, #1
004ec464: eor      r2, r2, r1
004ec468: strb     r2, [r4, #-1]
004ec46c: add      r4, r4, #1
004ec470: blo      #0x4ec43c
004ec474: add      sp, sp, #8
004ec478: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12CharAnimator8ANIM_SetEi
003cacb0: ldrb     r2, [r0, #0x49]
003cacb4: cmp      r2, #0
003cacb8: strne    r1, [r0, #0x50]
003cacbc: bxne     lr
003cacc0: mov      ip, #0x3f800000
003cacc4: str      ip, [r0, #0x40]
003cacc8: b        #0x3cab38

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

# _ZN6glitch7collada20CDynamicAnimationSet27addAnimationLibraryBindingsERKNS0_16CColladaDatabaseE
0062ee9c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062eea0: mov      r4, r0
0062eea4: mov      sl, r1
0062eea8: sub      sp, sp, #0x1c
0062eeac: add      r0, r0, #0x24
0062eeb0: bl       #0x62ecac
0062eeb4: ldr      r3, [sl]
0062eeb8: ldr      r1, [r4, #0x44]
0062eebc: ldr      r2, [r4, #0x48]
0062eec0: ldr      r3, [r3, #0x24]
0062eec4: cmp      r1, r2
0062eec8: ldr      r3, [r3, #0x20]
0062eecc: ldr      r3, [r3, #0x1c]
0062eed0: str      r3, [sp, #0x14]
0062eed4: beq      #0x62f09c
0062eed8: str      r3, [r1]
0062eedc: ldr      r3, [r4, #0x44]
0062eee0: add      r3, r3, #4
0062eee4: str      r3, [r4, #0x44]
0062eee8: ldr      r3, [sl]
0062eeec: ldr      r1, [r4, #0x50]
0062eef0: ldr      r2, [r4, #0x54]
0062eef4: ldr      r3, [r3, #0x24]
0062eef8: cmp      r1, r2
0062eefc: ldr      r3, [r3, #0x20]
0062ef00: ldr      r3, [r3, #0x20]
0062ef04: str      r3, [sp, #0x10]
0062ef08: beq      #0x62f0ac
0062ef0c: str      r3, [r1]
0062ef10: ldr      r3, [r4, #0x50]
0062ef14: add      r3, r3, #4
0062ef18: str      r3, [r4, #0x50]
0062ef1c: ldr      r3, [sl]
0062ef20: ldr      r2, [r4, #0x60]
0062ef24: ldr      r1, [r4, #0x5c]
0062ef28: ldr      r3, [r3, #0x24]
0062ef2c: cmp      r1, r2
0062ef30: ldr      r3, [r3, #0x20]
0062ef34: ldr      r2, [r3, #0x1c]
0062ef38: ldr      r3, [r3, #0x20]
0062ef3c: rsb      r3, r2, r3
0062ef40: str      r3, [sp, #0xc]
0062ef44: beq      #0x62f0bc
0062ef48: str      r3, [r1]
0062ef4c: ldr      r3, [r4, #0x5c]
0062ef50: add      r3, r3, #4
0062ef54: str      r3, [r4, #0x5c]
0062ef58: ldr      r5, [r4, #0x34]
0062ef5c: ldr      r3, [r4, #0x30]
0062ef60: ldr      r7, [r4, #0x3c]
0062ef64: add      r8, r4, #0x30
0062ef68: rsb      r3, r3, r5
0062ef6c: asr      r3, r3, #2
0062ef70: mov      r0, r8
0062ef74: add      r5, r3, r3, lsl #2
0062ef78: mov      r6, #0
0062ef7c: add      r5, r5, r5, lsl #4
0062ef80: add      r5, r5, r5, lsl #8
0062ef84: add      r5, r5, r5, lsl #16
0062ef88: add      r5, r3, r5, lsl #1
0062ef8c: add      r7, r5, r7
0062ef90: mov      r1, r7
0062ef94: bl       #0x62e5e0
0062ef98: mov      r0, r8
0062ef9c: mov      r1, r7
0062efa0: mov      r2, sp
0062efa4: str      r6, [sp]
0062efa8: str      r6, [sp, #4]
0062efac: str      r6, [sp, #8]
0062efb0: bl       #0x62ec50
0062efb4: ldr      r3, [r4, #0x3c]
0062efb8: cmp      r3, r6
0062efbc: beq      #0x62f094
0062efc0: mov      r3, #0xc
0062efc4: mul      r5, r3, r5
0062efc8: add      fp, r4, #0x68
0062efcc: mov      sb, #2
0062efd0: b        #0x62f004
0062efd4: ldr      r2, [r4, #0x30]
0062efd8: ldr      r1, [r4, #0x74]
0062efdc: add      r2, r2, r5
0062efe0: add      r1, r1, r7
0062efe4: add      r2, r2, #4
0062efe8: bl       #0x61c6bc
0062efec: cmp      r0, #0
0062eff0: beq      #0x62f074
0062eff4: ldr      r3, [r4, #0x3c]
0062eff8: add      r5, r5, #0xc
0062effc: cmp      r3, r6
0062f000: bls      #0x62f094
0062f004: ldr      r1, [r4, #0x74]
0062f008: lsl      r7, r6, #4
0062f00c: mov      r0, sl
0062f010: add      r1, r1, r7
0062f014: bl       #0x61c1e0
0062f018: ldr      r2, [r4, #0x30]
0062f01c: ldr      r1, [r4, #0x74]
0062f020: mov      r8, r0
0062f024: add      r2, r2, r5
0062f028: add      r2, r2, #4
0062f02c: mov      r0, sl
0062f030: add      r1, r1, r7
0062f034: bl       #0x61c6bc
0062f038: ldr      r3, [r4, #0x30]
0062f03c: cmp      r8, #0
0062f040: moveq    r2, #1
0062f044: streq    r2, [r3, r5]
0062f048: strne    sb, [r3, r5]
0062f04c: ldr      r3, [r4, #0x30]
0062f050: cmp      r0, #0
0062f054: add      r6, r6, #1
0062f058: add      r3, r3, r5
0062f05c: str      r8, [r3, #8]
0062f060: bne      #0x62eff4
0062f064: ldr      r3, [r4, #0x68]
0062f068: mov      r0, fp
0062f06c: cmp      r3, #0
0062f070: bne      #0x62efd4
0062f074: ldr      r3, [r4, #0x30]
0062f078: mov      r2, #0
0062f07c: add      r3, r3, r5
0062f080: str      r2, [r3, #4]
0062f084: ldr      r3, [r4, #0x3c]
0062f088: add      r5, r5, #0xc
0062f08c: cmp      r3, r6
0062f090: bhi      #0x62f004
0062f094: add      sp, sp, #0x1c
0062f098: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062f09c: add      r0, r4, #0x40
0062f0a0: add      r2, sp, #0x14
0062f0a4: bl       #0x62ee0c
0062f0a8: b        #0x62eee8
0062f0ac: add      r0, r4, #0x4c
0062f0b0: add      r2, sp, #0x10
0062f0b4: bl       #0x62ee0c
0062f0b8: b        #0x62ef1c
0062f0bc: add      r0, r4, #0x58
0062f0c0: add      r2, sp, #0xc
0062f0c4: bl       #0x62ee0c
0062f0c8: b        #0x62ef58

# _ZNSt4priv10_Rb_globalIbE10_RebalanceEPNS_18_Rb_tree_node_baseERS3_
00313760: mov      r3, #0
00313764: push     {r4, r5, r6}
00313768: strb     r3, [r0]
0031376c: mov      r5, #1
00313770: ldr      ip, [r1]
00313774: cmp      ip, r0
00313778: beq      #0x31378c
0031377c: ldr      r2, [r0, #4]
00313780: ldrb     r4, [r2]
00313784: cmp      r4, #0
00313788: beq      #0x31379c
0031378c: mov      r3, #1
00313790: strb     r3, [ip]
00313794: pop      {r4, r5, r6}
00313798: bx       lr
0031379c: ldr      r6, [r2, #4]
003137a0: ldr      ip, [r6, #8]
003137a4: cmp      r2, ip
003137a8: beq      #0x313864
003137ac: cmp      ip, #0
003137b0: beq      #0x3137e4
003137b4: ldrb     r4, [ip]
003137b8: cmp      r4, #0
003137bc: bne      #0x3137e4
003137c0: strb     r5, [r2]
003137c4: strb     r5, [ip]
003137c8: ldr      r2, [r0, #4]
003137cc: ldr      r2, [r2, #4]
003137d0: strb     r4, [r2]
003137d4: ldr      r2, [r0, #4]
003137d8: ldr      r2, [r2, #4]
003137dc: mov      r0, r2
003137e0: b        #0x313770
003137e4: ldr      ip, [r2, #8]
003137e8: cmp      ip, r0
003137ec: movne    ip, r2
003137f0: movne    r2, r0
003137f4: beq      #0x3138fc
003137f8: strb     r5, [ip]
003137fc: ldr      r0, [r2, #4]
00313800: ldr      r0, [r0, #4]
00313804: strb     r3, [r0]
00313808: ldr      r0, [r2, #4]
0031380c: ldr      r0, [r0, #4]
00313810: ldr      ip, [r0, #0xc]
00313814: ldr      r4, [ip, #8]
00313818: str      r4, [r0, #0xc]
0031381c: ldr      r4, [ip, #8]
00313820: cmp      r4, #0
00313824: strne    r0, [r4, #4]
00313828: ldr      r4, [r0, #4]
0031382c: str      r4, [ip, #4]
00313830: ldr      r4, [r1]
00313834: cmp      r0, r4
00313838: streq    ip, [r1]
0031383c: beq      #0x313854
00313840: ldr      r4, [r0, #4]
00313844: ldr      r6, [r4, #8]
00313848: cmp      r0, r6
0031384c: streq    ip, [r4, #8]
00313850: strne    ip, [r4, #0xc]
00313854: str      r0, [ip, #8]
00313858: str      ip, [r0, #4]
0031385c: mov      r0, r2
00313860: b        #0x313770
00313864: ldr      ip, [r6, #0xc]
00313868: cmp      ip, #0
0031386c: beq      #0x31387c
00313870: ldrb     r4, [ip]
00313874: cmp      r4, #0
00313878: beq      #0x3137c0
0031387c: ldr      ip, [r2, #0xc]
00313880: cmp      ip, r0
00313884: movne    ip, r2
00313888: movne    r2, r0
0031388c: beq      #0x313948
00313890: strb     r5, [ip]
00313894: ldr      r0, [r2, #4]
00313898: ldr      r0, [r0, #4]
0031389c: strb     r3, [r0]
003138a0: ldr      r0, [r2, #4]
003138a4: ldr      r0, [r0, #4]
003138a8: ldr      ip, [r0, #8]
003138ac: ldr      r4, [ip, #0xc]
003138b0: str      r4, [r0, #8]
003138b4: ldr      r4, [ip, #0xc]
003138b8: cmp      r4, #0
003138bc: strne    r0, [r4, #4]
003138c0: ldr      r4, [r0, #4]
003138c4: str      r4, [ip, #4]
003138c8: ldr      r4, [r1]
003138cc: cmp      r0, r4
003138d0: streq    ip, [r1]
003138d4: beq      #0x3138ec
003138d8: ldr      r4, [r0, #4]
003138dc: ldr      r6, [r4, #0xc]
003138e0: cmp      r0, r6
003138e4: streq    ip, [r4, #0xc]
003138e8: strne    ip, [r4, #8]
003138ec: str      r0, [ip, #0xc]
003138f0: str      ip, [r0, #4]
003138f4: mov      r0, r2
003138f8: b        #0x313770
003138fc: ldr      r4, [r0, #0xc]
00313900: str      r4, [r2, #8]
00313904: ldr      r0, [r0, #0xc]
00313908: cmp      r0, #0
0031390c: strne    r2, [r0, #4]
00313910: ldrne    r6, [r2, #4]
00313914: str      r6, [ip, #4]
00313918: ldr      r0, [r1]
0031391c: cmp      r2, r0
00313920: streq    ip, [r1]
00313924: beq      #0x31393c
00313928: ldr      r0, [r2, #4]
0031392c: ldr      r4, [r0, #0xc]
00313930: cmp      r2, r4
00313934: streq    ip, [r0, #0xc]
00313938: strne    ip, [r0, #8]
0031393c: str      r2, [ip, #0xc]
00313940: str      ip, [r2, #4]
00313944: b        #0x3137f8
00313948: ldr      r4, [r0, #8]
0031394c: str      r4, [r2, #0xc]
00313950: ldr      r0, [r0, #8]
00313954: cmp      r0, #0
00313958: strne    r2, [r0, #4]
0031395c: ldrne    r6, [r2, #4]
00313960: str      r6, [ip, #4]
00313964: ldr      r0, [r1]
00313968: cmp      r2, r0
0031396c: streq    ip, [r1]
00313970: beq      #0x313988
00313974: ldr      r0, [r2, #4]
00313978: ldr      r4, [r0, #8]
0031397c: cmp      r2, r4
00313980: streq    ip, [r0, #8]
00313984: strne    ip, [r0, #0xc]
00313988: str      r2, [ip, #8]
0031398c: str      ip, [r2, #4]
00313990: b        #0x313890

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi9AnimationENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EEN6glitch4core10SAllocatorIS6_LNSB_6memory13E_MEMORY_HINTE0EEEE14_M_create_nodeERKS6_
00364df4: push     {r4, r5, r6, lr}
00364df8: mov      r0, #0x44
00364dfc: mov      r5, r1
00364e00: mov      r1, #0
00364e04: bl       #0x310568
00364e08: ldr      r3, [r5]
00364e0c: mov      r4, r0
00364e10: add      r0, r0, #0x14
00364e14: str      r3, [r4, #0x10]
00364e18: str      r0, [r4, #0x24]
00364e1c: str      r0, [r4, #0x28]
00364e20: ldr      r2, [r5, #0x14]
00364e24: ldr      r1, [r5, #0x18]
00364e28: bl       #0x3116e8
00364e2c: ldr      r3, [r5, #0x1c]
00364e30: str      r3, [r4, #0x2c]
00364e34: ldr      r2, [r5, #0x20]
00364e38: cmp      r3, #0
00364e3c: str      r2, [r4, #0x30]
00364e40: beq      #0x364e54
00364e44: ldr      r2, [r3, #4]
00364e48: cmp      r2, #0
00364e4c: addne    r2, r2, #1
00364e50: strne    r2, [r3, #4]
00364e54: ldr      r2, [r5, #0x24]
00364e58: mov      r3, #0
00364e5c: mov      r0, r4
00364e60: str      r2, [r4, #0x34]
00364e64: ldr      r2, [r5, #0x28]
00364e68: str      r2, [r4, #0x38]
00364e6c: ldr      r2, [r5, #0x2c]
00364e70: str      r2, [r4, #0x3c]
00364e74: ldr      r2, [r5, #0x30]
00364e78: str      r3, [r4, #0xc]
00364e7c: str      r3, [r4, #8]
00364e80: str      r2, [r4, #0x40]
00364e84: pop      {r4, r5, r6, pc}

# _ZNK9Character8IsPlayerEv
003a49f0: push     {r4, r5, r6, lr}
003a49f4: mov      r5, r0
003a49f8: bl       #0x3a3054
003a49fc: cmp      r0, #0
003a4a00: beq      #0x3a4a14
003a4a04: cmp      r0, #1
003a4a08: movne    r0, #0
003a4a0c: moveq    r0, #1
003a4a10: pop      {r4, r5, r6, pc}
003a4a14: ldr      r4, [r5, #0x44]
003a4a18: ldr      r1, [pc, #0x18]
003a4a1c: mov      r0, r4
003a4a20: add      r1, pc, r1
003a4a24: bl       #0x30ebd4
003a4a28: cmp      r4, r0
003a4a2c: movne    r0, #0
003a4a30: moveq    r0, #1
003a4a34: pop      {r4, r5, r6, pc}
003a4a38: subseq   lr, r1, r8, asr #14

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

# _ZN7Structs8CharAnim4readEP11IStreamBase
004ef64c: push     {r4, r5, r6, r7, r8, lr}
004ef650: mov      r4, r0
004ef654: sub      sp, sp, #8
004ef658: mov      r0, r1
004ef65c: mov      r5, r1
004ef660: add      r1, r4, #4
004ef664: bl       #0x459090
004ef668: mov      r3, #1
004ef66c: cmp      r3, #0
004ef670: str      r3, [sp, #4]
004ef674: bne      #0x4ef6b8
004ef678: add      r3, r4, #5
004ef67c: add      r2, r4, #6
004ef680: ldrb     r0, [r2, #1]
004ef684: ldrb     r1, [r3, #-1]
004ef688: cmp      r3, r2
004ef68c: eor      r1, r0, r1
004ef690: strb     r1, [r3, #-1]
004ef694: ldrb     r0, [r2, #1]
004ef698: eor      r1, r1, r0
004ef69c: strb     r1, [r2, #1]
004ef6a0: ldrb     r0, [r3, #-1]
004ef6a4: sub      r2, r2, #1
004ef6a8: eor      r1, r1, r0
004ef6ac: strb     r1, [r3, #-1]
004ef6b0: add      r3, r3, #1
004ef6b4: blo      #0x4ef680
004ef6b8: mov      r0, r5
004ef6bc: add      r1, r4, #8
004ef6c0: bl       #0x459090
004ef6c4: mov      r3, #1
004ef6c8: cmp      r3, #0
004ef6cc: str      r3, [sp, #4]
004ef6d0: bne      #0x4ef714
004ef6d4: add      r3, r4, #9
004ef6d8: add      r2, r4, #0xa
004ef6dc: ldrb     r0, [r2, #1]
004ef6e0: ldrb     r1, [r3, #-1]
004ef6e4: cmp      r3, r2
004ef6e8: eor      r1, r0, r1
004ef6ec: strb     r1, [r3, #-1]
004ef6f0: ldrb     r0, [r2, #1]
004ef6f4: eor      r1, r1, r0
004ef6f8: strb     r1, [r2, #1]
004ef6fc: ldrb     r0, [r3, #-1]
004ef700: sub      r2, r2, #1
004ef704: eor      r1, r1, r0
004ef708: strb     r1, [r3, #-1]
004ef70c: add      r3, r3, #1
004ef710: blo      #0x4ef6dc
004ef714: mov      r0, r5
004ef718: add      r1, r4, #0xc
004ef71c: bl       #0x459090
004ef720: mov      r3, #1
004ef724: cmp      r3, #0
004ef728: str      r3, [sp, #4]
004ef72c: bne      #0x4ef770
004ef730: add      r3, r4, #0xd
004ef734: add      r2, r4, #0xe
004ef738: ldrb     r0, [r2, #1]
004ef73c: ldrb     r1, [r3, #-1]
004ef740: cmp      r3, r2
004ef744: eor      r1, r0, r1
004ef748: strb     r1, [r3, #-1]
004ef74c: ldrb     r0, [r2, #1]
004ef750: eor      r1, r1, r0
004ef754: strb     r1, [r2, #1]
004ef758: ldrb     r0, [r3, #-1]
004ef75c: sub      r2, r2, #1
004ef760: eor      r1, r1, r0
004ef764: strb     r1, [r3, #-1]
004ef768: add      r3, r3, #1
004ef76c: blo      #0x4ef738
004ef770: mov      r0, r5
004ef774: add      r1, r4, #0x10
004ef778: bl       #0x459090
004ef77c: mov      r3, #1
004ef780: cmp      r3, #0
004ef784: str      r3, [sp, #4]
004ef788: bne      #0x4ef7cc
004ef78c: add      r3, r4, #0x11
004ef790: add      r2, r4, #0x12
004ef794: ldrb     r0, [r2, #1]
004ef798: ldrb     r1, [r3, #-1]
004ef79c: cmp      r3, r2
004ef7a0: eor      r1, r0, r1
004ef7a4: strb     r1, [r3, #-1]
004ef7a8: ldrb     r0, [r2, #1]
004ef7ac: eor      r1, r1, r0
004ef7b0: strb     r1, [r2, #1]
004ef7b4: ldrb     r0, [r3, #-1]
004ef7b8: sub      r2, r2, #1
004ef7bc: eor      r1, r1, r0
004ef7c0: strb     r1, [r3, #-1]
004ef7c4: add      r3, r3, #1
004ef7c8: blo      #0x4ef794
004ef7cc: mov      r0, r5
004ef7d0: add      r1, r4, #0x14
004ef7d4: bl       #0x459090
004ef7d8: mov      r3, #1
004ef7dc: cmp      r3, #0
004ef7e0: str      r3, [sp, #4]
004ef7e4: bne      #0x4ef828
004ef7e8: add      r3, r4, #0x15
004ef7ec: add      r2, r4, #0x16
004ef7f0: ldrb     r0, [r2, #1]
004ef7f4: ldrb     r1, [r3, #-1]
004ef7f8: cmp      r3, r2
004ef7fc: eor      r1, r0, r1
004ef800: strb     r1, [r3, #-1]
004ef804: ldrb     r0, [r2, #1]
004ef808: eor      r1, r1, r0
004ef80c: strb     r1, [r2, #1]
004ef810: ldrb     r0, [r3, #-1]
004ef814: sub      r2, r2, #1
004ef818: eor      r1, r1, r0
004ef81c: strb     r1, [r3, #-1]
004ef820: add      r3, r3, #1
004ef824: blo      #0x4ef7f0
004ef828: mov      r0, r5
004ef82c: add      r1, r4, #0x18
004ef830: bl       #0x459090
004ef834: mov      r3, #1
004ef838: cmp      r3, #0
004ef83c: str      r3, [sp, #4]
004ef840: bne      #0x4ef884
004ef844: add      r3, r4, #0x19
004ef848: add      r2, r4, #0x1a
004ef84c: ldrb     r0, [r2, #1]
004ef850: ldrb     r1, [r3, #-1]
004ef854: cmp      r3, r2
004ef858: eor      r1, r0, r1
004ef85c: strb     r1, [r3, #-1]
004ef860: ldrb     r0, [r2, #1]
004ef864: eor      r1, r1, r0
004ef868: strb     r1, [r2, #1]
004ef86c: ldrb     r0, [r3, #-1]
004ef870: sub      r2, r2, #1
004ef874: eor      r1, r1, r0
004ef878: strb     r1, [r3, #-1]
004ef87c: add      r3, r3, #1
004ef880: blo      #0x4ef84c
004ef884: mov      r0, r5
004ef888: add      r1, r4, #0x1c
004ef88c: bl       #0x459090
004ef890: mov      r3, #1
004ef894: cmp      r3, #0
004ef898: str      r3, [sp, #4]
004ef89c: bne      #0x4ef8e0
004ef8a0: add      r3, r4, #0x1d
004ef8a4: add      r2, r4, #0x1e
004ef8a8: ldrb     r0, [r2, #1]
004ef8ac: ldrb     r1, [r3, #-1]
004ef8b0: cmp      r3, r2
004ef8b4: eor      r1, r0, r1
004ef8b8: strb     r1, [r3, #-1]
004ef8bc: ldrb     r0, [r2, #1]
004ef8c0: eor      r1, r1, r0
004ef8c4: strb     r1, [r2, #1]
004ef8c8: ldrb     r0, [r3, #-1]
004ef8cc: sub      r2, r2, #1
004ef8d0: eor      r1, r1, r0
004ef8d4: strb     r1, [r3, #-1]
004ef8d8: add      r3, r3, #1
004ef8dc: blo      #0x4ef8a8
004ef8e0: mov      r0, r5
004ef8e4: add      r1, r4, #0x20
004ef8e8: bl       #0x459090
004ef8ec: mov      r3, #1
004ef8f0: cmp      r3, #0
004ef8f4: str      r3, [sp, #4]
004ef8f8: bne      #0x4ef93c
004ef8fc: add      r3, r4, #0x21
004ef900: add      r2, r4, #0x22
004ef904: ldrb     r0, [r2, #1]
004ef908: ldrb     r1, [r3, #-1]
004ef90c: cmp      r3, r2
004ef910: eor      r1, r0, r1
004ef914: strb     r1, [r3, #-1]
004ef918: ldrb     r0, [r2, #1]
004ef91c: eor      r1, r1, r0
004ef920: strb     r1, [r2, #1]
004ef924: ldrb     r0, [r3, #-1]
004ef928: sub      r2, r2, #1
004ef92c: eor      r1, r1, r0
004ef930: strb     r1, [r3, #-1]
004ef934: add      r3, r3, #1
004ef938: blo      #0x4ef904
004ef93c: mov      r0, r5
004ef940: add      r1, r4, #0x24
004ef944: bl       #0x459090
004ef948: mov      r3, #1
004ef94c: cmp      r3, #0
004ef950: str      r3, [sp, #4]
004ef954: bne      #0x4ef998
004ef958: add      r3, r4, #0x25
004ef95c: add      r2, r4, #0x26
004ef960: ldrb     r0, [r2, #1]
004ef964: ldrb     r1, [r3, #-1]
004ef968: cmp      r3, r2
004ef96c: eor      r1, r0, r1
004ef970: strb     r1, [r3, #-1]
004ef974: ldrb     r0, [r2, #1]
004ef978: eor      r1, r1, r0
004ef97c: strb     r1, [r2, #1]
004ef980: ldrb     r0, [r3, #-1]
004ef984: sub      r2, r2, #1
004ef988: eor      r1, r1, r0
004ef98c: strb     r1, [r3, #-1]
004ef990: add      r3, r3, #1
004ef994: blo      #0x4ef960
004ef998: mov      r0, r5
004ef99c: add      r1, r4, #0x28
004ef9a0: bl       #0x459090
004ef9a4: mov      r3, #1
004ef9a8: cmp      r3, #0
004ef9ac: str      r3, [sp, #4]
004ef9b0: bne      #0x4ef9f4
004ef9b4: add      r3, r4, #0x29
004ef9b8: add      r2, r4, #0x2a
004ef9bc: ldrb     r0, [r2, #1]
004ef9c0: ldrb     r1, [r3, #-1]
004ef9c4: cmp      r3, r2
004ef9c8: eor      r1, r0, r1
004ef9cc: strb     r1, [r3, #-1]
004ef9d0: ldrb     r0, [r2, #1]
004ef9d4: eor      r1, r1, r0
004ef9d8: strb     r1, [r2, #1]
004ef9dc: ldrb     r0, [r3, #-1]
004ef9e0: sub      r2, r2, #1
004ef9e4: eor      r1, r1, r0
004ef9e8: strb     r1, [r3, #-1]
004ef9ec: add      r3, r3, #1
004ef9f0: blo      #0x4ef9bc
004ef9f4: mov      r0, r5
004ef9f8: add      r1, r4, #0x2c
004ef9fc: bl       #0x459090
004efa00: mov      r3, #1
004efa04: cmp      r3, #0
004efa08: str      r3, [sp, #4]
004efa0c: bne      #0x4efa50
004efa10: add      r3, r4, #0x2d
004efa14: add      r2, r4, #0x2e
004efa18: ldrb     r0, [r2, #1]
004efa1c: ldrb     r1, [r3, #-1]
004efa20: cmp      r3, r2
004efa24: eor      r1, r0, r1
004efa28: strb     r1, [r3, #-1]
004efa2c: ldrb     r0, [r2, #1]
004efa30: eor      r1, r1, r0
004efa34: strb     r1, [r2, #1]
004efa38: ldrb     r0, [r3, #-1]
004efa3c: sub      r2, r2, #1
004efa40: eor      r1, r1, r0
004efa44: strb     r1, [r3, #-1]
004efa48: add      r3, r3, #1
004efa4c: blo      #0x4efa18
004efa50: mov      r0, r5
004efa54: add      r1, r4, #0x30
004efa58: bl       #0x459090
004efa5c: mov      r3, #1
004efa60: cmp      r3, #0
004efa64: str      r3, [sp, #4]
004efa68: bne      #0x4efaac
004efa6c: add      r3, r4, #0x31
004efa70: add      r2, r4, #0x32
004efa74: ldrb     r0, [r2, #1]
004efa78: ldrb     r1, [r3, #-1]
004efa7c: cmp      r3, r2
004efa80: eor      r1, r0, r1
004efa84: strb     r1, [r3, #-1]
004efa88: ldrb     r0, [r2, #1]
004efa8c: eor      r1, r1, r0
004efa90: strb     r1, [r2, #1]
004efa94: ldrb     r0, [r3, #-1]
004efa98: sub      r2, r2, #1
004efa9c: eor      r1, r1, r0
004efaa0: strb     r1, [r3, #-1]
004efaa4: add      r3, r3, #1
004efaa8: blo      #0x4efa74
004efaac: mov      r0, r5
004efab0: add      r1, r4, #0x34
004efab4: bl       #0x459090
004efab8: mov      r3, #1
004efabc: cmp      r3, #0
004efac0: str      r3, [sp, #4]
004efac4: bne      #0x4efb08
004efac8: add      r3, r4, #0x35
004efacc: add      r2, r4, #0x36
004efad0: ldrb     r0, [r2, #1]
004efad4: ldrb     r1, [r3, #-1]
004efad8: cmp      r3, r2
004efadc: eor      r1, r0, r1
004efae0: strb     r1, [r3, #-1]
004efae4: ldrb     r0, [r2, #1]
004efae8: eor      r1, r1, r0
004efaec: strb     r1, [r2, #1]
004efaf0: ldrb     r0, [r3, #-1]
004efaf4: sub      r2, r2, #1
004efaf8: eor      r1, r1, r0
004efafc: strb     r1, [r3, #-1]
004efb00: add      r3, r3, #1
004efb04: blo      #0x4efad0
004efb08: mov      r0, r5
004efb0c: add      r1, r4, #0x38
004efb10: bl       #0x459090
004efb14: mov      r3, #1
004efb18: cmp      r3, #0
004efb1c: str      r3, [sp, #4]
004efb20: bne      #0x4efb64
004efb24: add      r3, r4, #0x39
004efb28: add      r2, r4, #0x3a
004efb2c: ldrb     r0, [r2, #1]
004efb30: ldrb     r1, [r3, #-1]
004efb34: cmp      r3, r2
004efb38: eor      r1, r0, r1
004efb3c: strb     r1, [r3, #-1]
004efb40: ldrb     r0, [r2, #1]
004efb44: eor      r1, r1, r0
004efb48: strb     r1, [r2, #1]
004efb4c: ldrb     r0, [r3, #-1]
004efb50: sub      r2, r2, #1
004efb54: eor      r1, r1, r0
004efb58: strb     r1, [r3, #-1]
004efb5c: add      r3, r3, #1
004efb60: blo      #0x4efb2c
004efb64: mov      r0, r5
004efb68: add      r1, r4, #0x3c
004efb6c: bl       #0x459090
004efb70: mov      r3, #1
004efb74: cmp      r3, #0
004efb78: str      r3, [sp, #4]
004efb7c: bne      #0x4efbc0
004efb80: add      r3, r4, #0x3d
004efb84: add      r2, r4, #0x3e
004efb88: ldrb     r0, [r2, #1]
004efb8c: ldrb     r1, [r3, #-1]
004efb90: cmp      r3, r2
004efb94: eor      r1, r0, r1
004efb98: strb     r1, [r3, #-1]
004efb9c: ldrb     r0, [r2, #1]
004efba0: eor      r1, r1, r0
004efba4: strb     r1, [r2, #1]
004efba8: ldrb     r0, [r3, #-1]
004efbac: sub      r2, r2, #1
004efbb0: eor      r1, r1, r0
004efbb4: strb     r1, [r3, #-1]
004efbb8: add      r3, r3, #1
004efbbc: blo      #0x4efb88
004efbc0: mov      r0, r5
004efbc4: add      r1, r4, #0x40
004efbc8: bl       #0x3df1a0
004efbcc: mov      r3, #1
004efbd0: cmp      r3, #0
004efbd4: str      r3, [sp, #4]
004efbd8: bne      #0x4efc1c
004efbdc: add      r3, r4, #0x41
004efbe0: add      r2, r4, #0x42
004efbe4: ldrb     r0, [r2, #1]
004efbe8: ldrb     r1, [r3, #-1]
004efbec: cmp      r3, r2
004efbf0: eor      r1, r0, r1
004efbf4: strb     r1, [r3, #-1]
004efbf8: ldrb     r0, [r2, #1]
004efbfc: eor      r1, r1, r0
004efc00: strb     r1, [r2, #1]
004efc04: ldrb     r0, [r3, #-1]
004efc08: sub      r2, r2, #1
004efc0c: eor      r1, r1, r0
004efc10: strb     r1, [r3, #-1]
004efc14: add      r3, r3, #1
004efc18: blo      #0x4efbe4
004efc1c: ldr      r0, [r4, #0x44]
004efc20: cmp      r0, #0
004efc24: beq      #0x4efc2c
004efc28: bl       #0x310440
004efc2c: ldr      r0, [r4, #0x40]
004efc30: mov      r1, #1
004efc34: lsl      r0, r0, #2
004efc38: bl       #0x31056c
004efc3c: ldr      r3, [r4, #0x40]
004efc40: str      r0, [r4, #0x44]
004efc44: cmp      r3, #0
004efc48: beq      #0x4efccc
004efc4c: mov      r6, #0
004efc50: mov      r8, #1
004efc54: lsl      r7, r6, #2
004efc58: add      r1, r0, r7
004efc5c: mov      r0, r5
004efc60: bl       #0x459090
004efc64: str      r8, [sp, #4]
004efc68: cmp      r8, #0
004efc6c: ldr      r3, [r4, #0x44]
004efc70: bne      #0x4efcb8
004efc74: add      r7, r3, r7
004efc78: add      r3, r7, #2
004efc7c: add      r7, r7, #1
004efc80: ldrb     r1, [r3, #1]
004efc84: ldrb     r2, [r7, #-1]
004efc88: cmp      r7, r3
004efc8c: eor      r2, r1, r2
004efc90: strb     r2, [r7, #-1]
004efc94: ldrb     r1, [r3, #1]
004efc98: eor      r2, r2, r1
004efc9c: strb     r2, [r3, #1]
004efca0: ldrb     r1, [r7, #-1]
004efca4: sub      r3, r3, #1
004efca8: eor      r2, r2, r1
004efcac: strb     r2, [r7, #-1]
004efcb0: add      r7, r7, #1
004efcb4: blo      #0x4efc80
004efcb8: ldr      r3, [r4, #0x40]
004efcbc: add      r6, r6, #1
004efcc0: cmp      r3, r6
004efcc4: ldrhi    r0, [r4, #0x44]
004efcc8: bhi      #0x4efc54
004efccc: mov      r0, r5
004efcd0: add      r1, r4, #0x48
004efcd4: bl       #0x459090
004efcd8: mov      r3, #1
004efcdc: cmp      r3, #0
004efce0: str      r3, [sp, #4]
004efce4: bne      #0x4efd28
004efce8: add      r3, r4, #0x49
004efcec: add      r2, r4, #0x4a
004efcf0: ldrb     r0, [r2, #1]
004efcf4: ldrb     r1, [r3, #-1]
004efcf8: cmp      r3, r2
004efcfc: eor      r1, r0, r1
004efd00: strb     r1, [r3, #-1]
004efd04: ldrb     r0, [r2, #1]
004efd08: eor      r1, r1, r0
004efd0c: strb     r1, [r2, #1]
004efd10: ldrb     r0, [r3, #-1]
004efd14: sub      r2, r2, #1
004efd18: eor      r1, r1, r0
004efd1c: strb     r1, [r3, #-1]
004efd20: add      r3, r3, #1
004efd24: blo      #0x4efcf0
004efd28: mov      r0, r5
004efd2c: add      r1, r4, #0x4c
004efd30: bl       #0x459090
004efd34: mov      r3, #1
004efd38: cmp      r3, #0
004efd3c: str      r3, [sp, #4]
004efd40: bne      #0x4efd84
004efd44: add      r3, r4, #0x4d
004efd48: add      r2, r4, #0x4e
004efd4c: ldrb     r0, [r2, #1]
004efd50: ldrb     r1, [r3, #-1]
004efd54: cmp      r3, r2
004efd58: eor      r1, r0, r1
004efd5c: strb     r1, [r3, #-1]
004efd60: ldrb     r0, [r2, #1]
004efd64: eor      r1, r1, r0
004efd68: strb     r1, [r2, #1]
004efd6c: ldrb     r0, [r3, #-1]
004efd70: sub      r2, r2, #1
004efd74: eor      r1, r1, r0
004efd78: strb     r1, [r3, #-1]
004efd7c: add      r3, r3, #1
004efd80: blo      #0x4efd4c
004efd84: mov      r0, r5
004efd88: add      r1, r4, #0x50
004efd8c: bl       #0x459090
004efd90: mov      r3, #1
004efd94: cmp      r3, #0
004efd98: str      r3, [sp, #4]
004efd9c: bne      #0x4efde0
004efda0: add      r3, r4, #0x51
004efda4: add      r2, r4, #0x52
004efda8: ldrb     r0, [r2, #1]
004efdac: ldrb     r1, [r3, #-1]
004efdb0: cmp      r3, r2
004efdb4: eor      r1, r0, r1
004efdb8: strb     r1, [r3, #-1]
004efdbc: ldrb     r0, [r2, #1]
004efdc0: eor      r1, r1, r0
004efdc4: strb     r1, [r2, #1]
004efdc8: ldrb     r0, [r3, #-1]
004efdcc: sub      r2, r2, #1
004efdd0: eor      r1, r1, r0
004efdd4: strb     r1, [r3, #-1]
004efdd8: add      r3, r3, #1
004efddc: blo      #0x4efda8
004efde0: mov      r0, r5
004efde4: add      r1, r4, #0x54
004efde8: bl       #0x459090
004efdec: mov      r3, #1
004efdf0: cmp      r3, #0
004efdf4: str      r3, [sp, #4]
004efdf8: bne      #0x4efe3c
004efdfc: add      r3, r4, #0x55
004efe00: add      r2, r4, #0x56
004efe04: ldrb     r0, [r2, #1]
004efe08: ldrb     r1, [r3, #-1]
004efe0c: cmp      r3, r2
004efe10: eor      r1, r0, r1
004efe14: strb     r1, [r3, #-1]
004efe18: ldrb     r0, [r2, #1]
004efe1c: eor      r1, r1, r0
004efe20: strb     r1, [r2, #1]
004efe24: ldrb     r0, [r3, #-1]
004efe28: sub      r2, r2, #1
004efe2c: eor      r1, r1, r0
004efe30: strb     r1, [r3, #-1]
004efe34: add      r3, r3, #1
004efe38: blo      #0x4efe04
004efe3c: mov      r0, r5
004efe40: add      r1, r4, #0x58
004efe44: bl       #0x459090
004efe48: mov      r3, #1
004efe4c: cmp      r3, #0
004efe50: str      r3, [sp, #4]
004efe54: bne      #0x4efe98
004efe58: add      r3, r4, #0x59
004efe5c: add      r2, r4, #0x5a
004efe60: ldrb     r0, [r2, #1]
004efe64: ldrb     r1, [r3, #-1]
004efe68: cmp      r3, r2
004efe6c: eor      r1, r0, r1
004efe70: strb     r1, [r3, #-1]
004efe74: ldrb     r0, [r2, #1]
004efe78: eor      r1, r1, r0
004efe7c: strb     r1, [r2, #1]
004efe80: ldrb     r0, [r3, #-1]
004efe84: sub      r2, r2, #1
004efe88: eor      r1, r1, r0
004efe8c: strb     r1, [r3, #-1]
004efe90: add      r3, r3, #1
004efe94: blo      #0x4efe60
004efe98: mov      r0, r5
004efe9c: add      r1, r4, #0x5c
004efea0: bl       #0x459090
004efea4: mov      r3, #1
004efea8: cmp      r3, #0
004efeac: str      r3, [sp, #4]
004efeb0: bne      #0x4efef4
004efeb4: add      r3, r4, #0x5d
004efeb8: add      r2, r4, #0x5e
004efebc: ldrb     r0, [r2, #1]
004efec0: ldrb     r1, [r3, #-1]
004efec4: cmp      r3, r2
004efec8: eor      r1, r0, r1
004efecc: strb     r1, [r3, #-1]
004efed0: ldrb     r0, [r2, #1]
004efed4: eor      r1, r1, r0
004efed8: strb     r1, [r2, #1]
004efedc: ldrb     r0, [r3, #-1]
004efee0: sub      r2, r2, #1
004efee4: eor      r1, r1, r0
004efee8: strb     r1, [r3, #-1]
004efeec: add      r3, r3, #1
004efef0: blo      #0x4efebc
004efef4: mov      r0, r5
004efef8: add      r1, r4, #0x60
004efefc: bl       #0x459090
004eff00: mov      r3, #1
004eff04: cmp      r3, #0
004eff08: str      r3, [sp, #4]
004eff0c: bne      #0x4eff50
004eff10: add      r3, r4, #0x61
004eff14: add      r2, r4, #0x62
004eff18: ldrb     r0, [r2, #1]
004eff1c: ldrb     r1, [r3, #-1]
004eff20: cmp      r3, r2
004eff24: eor      r1, r0, r1
004eff28: strb     r1, [r3, #-1]
004eff2c: ldrb     r0, [r2, #1]
004eff30: eor      r1, r1, r0
004eff34: strb     r1, [r2, #1]
004eff38: ldrb     r0, [r3, #-1]
004eff3c: sub      r2, r2, #1
004eff40: eor      r1, r1, r0
004eff44: strb     r1, [r3, #-1]
004eff48: add      r3, r3, #1
004eff4c: blo      #0x4eff18
004eff50: mov      r0, r5
004eff54: add      r1, r4, #0x64
004eff58: bl       #0x459090
004eff5c: mov      r3, #1
004eff60: cmp      r3, #0
004eff64: str      r3, [sp, #4]
004eff68: bne      #0x4effac
004eff6c: add      r3, r4, #0x65
004eff70: add      r2, r4, #0x66
004eff74: ldrb     r0, [r2, #1]
004eff78: ldrb     r1, [r3, #-1]
004eff7c: cmp      r3, r2
004eff80: eor      r1, r0, r1
004eff84: strb     r1, [r3, #-1]
004eff88: ldrb     r0, [r2, #1]
004eff8c: eor      r1, r1, r0
004eff90: strb     r1, [r2, #1]
004eff94: ldrb     r0, [r3, #-1]
004eff98: sub      r2, r2, #1
004eff9c: eor      r1, r1, r0
004effa0: strb     r1, [r3, #-1]
004effa4: add      r3, r3, #1
004effa8: blo      #0x4eff74
004effac: mov      r0, r5
004effb0: add      r1, r4, #0x68
004effb4: bl       #0x459090
004effb8: mov      r3, #1
004effbc: cmp      r3, #0
004effc0: str      r3, [sp, #4]
004effc4: bne      #0x4f0008
004effc8: add      r3, r4, #0x69
004effcc: add      r2, r4, #0x6a
004effd0: ldrb     r0, [r2, #1]
004effd4: ldrb     r1, [r3, #-1]
004effd8: cmp      r3, r2
004effdc: eor      r1, r0, r1
004effe0: strb     r1, [r3, #-1]
004effe4: ldrb     r0, [r2, #1]
004effe8: eor      r1, r1, r0
004effec: strb     r1, [r2, #1]
004efff0: ldrb     r0, [r3, #-1]
004efff4: sub      r2, r2, #1
004efff8: eor      r1, r1, r0
004efffc: strb     r1, [r3, #-1]
004f0000: add      r3, r3, #1
004f0004: blo      #0x4effd0
004f0008: mov      r0, r5
004f000c: add      r1, r4, #0x6c
004f0010: bl       #0x459090
004f0014: mov      r3, #1
004f0018: cmp      r3, #0
004f001c: str      r3, [sp, #4]
004f0020: bne      #0x4f0064
004f0024: add      r3, r4, #0x6d
004f0028: add      r2, r4, #0x6e
004f002c: ldrb     r0, [r2, #1]
004f0030: ldrb     r1, [r3, #-1]
004f0034: cmp      r3, r2
004f0038: eor      r1, r0, r1
004f003c: strb     r1, [r3, #-1]
004f0040: ldrb     r0, [r2, #1]
004f0044: eor      r1, r1, r0
004f0048: strb     r1, [r2, #1]
004f004c: ldrb     r0, [r3, #-1]
004f0050: sub      r2, r2, #1
004f0054: eor      r1, r1, r0
004f0058: strb     r1, [r3, #-1]
004f005c: add      r3, r3, #1
004f0060: blo      #0x4f002c
004f0064: mov      r0, r5
004f0068: add      r1, r4, #0x70
004f006c: bl       #0x459090
004f0070: mov      r3, #1
004f0074: cmp      r3, #0
004f0078: str      r3, [sp, #4]
004f007c: bne      #0x4f00c0
004f0080: add      r3, r4, #0x71
004f0084: add      r2, r4, #0x72
004f0088: ldrb     r0, [r2, #1]
004f008c: ldrb     r1, [r3, #-1]
004f0090: cmp      r3, r2
004f0094: eor      r1, r0, r1
004f0098: strb     r1, [r3, #-1]
004f009c: ldrb     r0, [r2, #1]
004f00a0: eor      r1, r1, r0
004f00a4: strb     r1, [r2, #1]
004f00a8: ldrb     r0, [r3, #-1]
004f00ac: sub      r2, r2, #1
004f00b0: eor      r1, r1, r0
004f00b4: strb     r1, [r3, #-1]
004f00b8: add      r3, r3, #1
004f00bc: blo      #0x4f0088
004f00c0: mov      r0, r5
004f00c4: add      r1, r4, #0x74
004f00c8: bl       #0x459090
004f00cc: mov      r3, #1
004f00d0: cmp      r3, #0
004f00d4: str      r3, [sp, #4]
004f00d8: bne      #0x4f011c
004f00dc: add      r3, r4, #0x75
004f00e0: add      r2, r4, #0x76
004f00e4: ldrb     r0, [r2, #1]
004f00e8: ldrb     r1, [r3, #-1]
004f00ec: cmp      r3, r2
004f00f0: eor      r1, r0, r1
004f00f4: strb     r1, [r3, #-1]
004f00f8: ldrb     r0, [r2, #1]
004f00fc: eor      r1, r1, r0
004f0100: strb     r1, [r2, #1]
004f0104: ldrb     r0, [r3, #-1]
004f0108: sub      r2, r2, #1
004f010c: eor      r1, r1, r0
004f0110: strb     r1, [r3, #-1]
004f0114: add      r3, r3, #1
004f0118: blo      #0x4f00e4
004f011c: mov      r0, r5
004f0120: add      r1, r4, #0x78
004f0124: bl       #0x459090
004f0128: mov      r3, #1
004f012c: cmp      r3, #0
004f0130: str      r3, [sp, #4]
004f0134: bne      #0x4f0178
004f0138: add      r3, r4, #0x79
004f013c: add      r2, r4, #0x7a
004f0140: ldrb     r0, [r2, #1]
004f0144: ldrb     r1, [r3, #-1]
004f0148: cmp      r3, r2
004f014c: eor      r1, r0, r1
004f0150: strb     r1, [r3, #-1]
004f0154: ldrb     r0, [r2, #1]
004f0158: eor      r1, r1, r0
004f015c: strb     r1, [r2, #1]
004f0160: ldrb     r0, [r3, #-1]
004f0164: sub      r2, r2, #1
004f0168: eor      r1, r1, r0
004f016c: strb     r1, [r3, #-1]
004f0170: add      r3, r3, #1
004f0174: blo      #0x4f0140
004f0178: mov      r0, r5
004f017c: add      r1, r4, #0x7c
004f0180: bl       #0x459090
004f0184: mov      r3, #1
004f0188: cmp      r3, #0
004f018c: str      r3, [sp, #4]
004f0190: bne      #0x4f01d4
004f0194: add      r3, r4, #0x7d
004f0198: add      r2, r4, #0x7e
004f019c: ldrb     r0, [r2, #1]
004f01a0: ldrb     r1, [r3, #-1]
004f01a4: cmp      r3, r2
004f01a8: eor      r1, r0, r1
004f01ac: strb     r1, [r3, #-1]
004f01b0: ldrb     r0, [r2, #1]
004f01b4: eor      r1, r1, r0
004f01b8: strb     r1, [r2, #1]
004f01bc: ldrb     r0, [r3, #-1]
004f01c0: sub      r2, r2, #1
004f01c4: eor      r1, r1, r0
004f01c8: strb     r1, [r3, #-1]
004f01cc: add      r3, r3, #1
004f01d0: blo      #0x4f019c
004f01d4: mov      r0, r5
004f01d8: add      r1, r4, #0x80
004f01dc: bl       #0x459090
004f01e0: mov      r3, #1
004f01e4: cmp      r3, #0
004f01e8: str      r3, [sp, #4]
004f01ec: bne      #0x4f0230
004f01f0: add      r3, r4, #0x81
004f01f4: add      r2, r4, #0x82
004f01f8: ldrb     r0, [r2, #1]
004f01fc: ldrb     r1, [r3, #-1]
004f0200: cmp      r3, r2
004f0204: eor      r1, r0, r1
004f0208: strb     r1, [r3, #-1]
004f020c: ldrb     r0, [r2, #1]
004f0210: eor      r1, r1, r0
004f0214: strb     r1, [r2, #1]
004f0218: ldrb     r0, [r3, #-1]
004f021c: sub      r2, r2, #1
004f0220: eor      r1, r1, r0
004f0224: strb     r1, [r3, #-1]
004f0228: add      r3, r3, #1
004f022c: blo      #0x4f01f8
004f0230: mov      r0, r5
004f0234: add      r1, r4, #0x84
004f0238: bl       #0x3df1a0
004f023c: mov      r3, #1
004f0240: cmp      r3, #0
004f0244: str      r3, [sp, #4]
004f0248: bne      #0x4f028c
004f024c: add      r3, r4, #0x85
004f0250: add      r2, r4, #0x86
004f0254: ldrb     r0, [r2, #1]
004f0258: ldrb     r1, [r3, #-1]
004f025c: cmp      r2, r3
004f0260: eor      r1, r0, r1
004f0264: strb     r1, [r3, #-1]
004f0268: ldrb     r0, [r2, #1]
004f026c: eor      r1, r1, r0
004f0270: strb     r1, [r2, #1]
004f0274: ldrb     r0, [r3, #-1]
004f0278: sub      r2, r2, #1
004f027c: eor      r1, r1, r0
004f0280: strb     r1, [r3, #-1]
004f0284: add      r3, r3, #1
004f0288: bhi      #0x4f0254
004f028c: ldr      r0, [r4, #0x88]
004f0290: cmp      r0, #0
004f0294: beq      #0x4f029c
004f0298: bl       #0x310440
004f029c: ldr      r0, [r4, #0x84]
004f02a0: mov      r1, #1
004f02a4: lsl      r0, r0, #2
004f02a8: bl       #0x31056c
004f02ac: ldr      r3, [r4, #0x84]
004f02b0: str      r0, [r4, #0x88]
004f02b4: cmp      r3, #0
004f02b8: beq      #0x4f033c
004f02bc: mov      r6, #0
004f02c0: mov      r8, #1
004f02c4: lsl      r7, r6, #2
004f02c8: add      r1, r0, r7
004f02cc: mov      r0, r5
004f02d0: bl       #0x459090
004f02d4: str      r8, [sp, #4]
004f02d8: cmp      r8, #0
004f02dc: ldr      r3, [r4, #0x88]
004f02e0: bne      #0x4f0328
004f02e4: add      r7, r3, r7
004f02e8: add      r3, r7, #2
004f02ec: add      r7, r7, #1
004f02f0: ldrb     r1, [r3, #1]
004f02f4: ldrb     r2, [r7, #-1]
004f02f8: cmp      r7, r3
004f02fc: eor      r2, r1, r2
004f0300: strb     r2, [r7, #-1]
004f0304: ldrb     r1, [r3, #1]
004f0308: eor      r2, r2, r1
004f030c: strb     r2, [r3, #1]
004f0310: ldrb     r1, [r7, #-1]
004f0314: sub      r3, r3, #1
004f0318: eor      r2, r2, r1
004f031c: strb     r2, [r7, #-1]
004f0320: add      r7, r7, #1
004f0324: blo      #0x4f02f0
004f0328: ldr      r3, [r4, #0x84]
004f032c: add      r6, r6, #1
004f0330: cmp      r3, r6
004f0334: ldrhi    r0, [r4, #0x88]
004f0338: bhi      #0x4f02c4
004f033c: mov      r0, r5
004f0340: add      r1, r4, #0x8c
004f0344: bl       #0x459090
004f0348: mov      r3, #1
004f034c: cmp      r3, #0
004f0350: str      r3, [sp, #4]
004f0354: bne      #0x4f0398
004f0358: add      r3, r4, #0x8d
004f035c: add      r2, r4, #0x8e
004f0360: ldrb     r0, [r2, #1]
004f0364: ldrb     r1, [r3, #-1]
004f0368: cmp      r2, r3
004f036c: eor      r1, r0, r1
004f0370: strb     r1, [r3, #-1]
004f0374: ldrb     r0, [r2, #1]
004f0378: eor      r1, r1, r0
004f037c: strb     r1, [r2, #1]
004f0380: ldrb     r0, [r3, #-1]
004f0384: sub      r2, r2, #1
004f0388: eor      r1, r1, r0
004f038c: strb     r1, [r3, #-1]
004f0390: add      r3, r3, #1
004f0394: bhi      #0x4f0360
004f0398: mov      r0, r5
004f039c: add      r1, r4, #0x90
004f03a0: bl       #0x459090
004f03a4: mov      r3, #1
004f03a8: cmp      r3, #0
004f03ac: str      r3, [sp, #4]
004f03b0: bne      #0x4f03f4
004f03b4: add      r3, r4, #0x91
004f03b8: add      r2, r4, #0x92
004f03bc: ldrb     r0, [r2, #1]
004f03c0: ldrb     r1, [r3, #-1]
004f03c4: cmp      r2, r3
004f03c8: eor      r1, r0, r1
004f03cc: strb     r1, [r3, #-1]
004f03d0: ldrb     r0, [r2, #1]
004f03d4: eor      r1, r1, r0
004f03d8: strb     r1, [r2, #1]
004f03dc: ldrb     r0, [r3, #-1]
004f03e0: sub      r2, r2, #1
004f03e4: eor      r1, r1, r0
004f03e8: strb     r1, [r3, #-1]
004f03ec: add      r3, r3, #1
004f03f0: bhi      #0x4f03bc
004f03f4: mov      r0, r5
004f03f8: add      r1, r4, #0x94
004f03fc: bl       #0x459090
004f0400: mov      r3, #1
004f0404: cmp      r3, #0
004f0408: str      r3, [sp, #4]
004f040c: bne      #0x4f0450
004f0410: add      r3, r4, #0x95
004f0414: add      r2, r4, #0x96
004f0418: ldrb     r0, [r2, #1]
004f041c: ldrb     r1, [r3, #-1]
004f0420: cmp      r3, r2
004f0424: eor      r1, r0, r1
004f0428: strb     r1, [r3, #-1]
004f042c: ldrb     r0, [r2, #1]
004f0430: eor      r1, r1, r0
004f0434: strb     r1, [r2, #1]
004f0438: ldrb     r0, [r3, #-1]
004f043c: sub      r2, r2, #1
004f0440: eor      r1, r1, r0
004f0444: strb     r1, [r3, #-1]
004f0448: add      r3, r3, #1
004f044c: blo      #0x4f0418
004f0450: mov      r0, r5
004f0454: add      r1, r4, #0x98
004f0458: bl       #0x459090
004f045c: mov      r3, #1
004f0460: cmp      r3, #0
004f0464: str      r3, [sp, #4]
004f0468: bne      #0x4f04ac
004f046c: add      r3, r4, #0x99
004f0470: add      r2, r4, #0x9a
004f0474: ldrb     r0, [r2, #1]
004f0478: ldrb     r1, [r3, #-1]
004f047c: cmp      r3, r2
004f0480: eor      r1, r0, r1
004f0484: strb     r1, [r3, #-1]
004f0488: ldrb     r0, [r2, #1]
004f048c: eor      r1, r1, r0
004f0490: strb     r1, [r2, #1]
004f0494: ldrb     r0, [r3, #-1]
004f0498: sub      r2, r2, #1
004f049c: eor      r1, r1, r0
004f04a0: strb     r1, [r3, #-1]
004f04a4: add      r3, r3, #1
004f04a8: blo      #0x4f0474
004f04ac: mov      r0, r5
004f04b0: add      r1, r4, #0x9c
004f04b4: bl       #0x459090
004f04b8: mov      r3, #1
004f04bc: cmp      r3, #0
004f04c0: str      r3, [sp, #4]
004f04c4: bne      #0x4f0508
004f04c8: add      r3, r4, #0x9e
004f04cc: add      r4, r4, #0x9d
004f04d0: ldrb     r1, [r3, #1]
004f04d4: ldrb     r2, [r4, #-1]
004f04d8: cmp      r3, r4
004f04dc: eor      r2, r1, r2
004f04e0: strb     r2, [r4, #-1]
004f04e4: ldrb     r1, [r3, #1]
004f04e8: eor      r2, r2, r1
004f04ec: strb     r2, [r3, #1]
004f04f0: ldrb     r1, [r4, #-1]
004f04f4: sub      r3, r3, #1
004f04f8: eor      r2, r2, r1
004f04fc: strb     r2, [r4, #-1]
004f0500: add      r4, r4, #1
004f0504: bhi      #0x4f04d0
004f0508: add      sp, sp, #8
004f050c: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404

# _ZNK9Character22GetCharUniqueAnimSetIdEv
003a54b4: push     {r4, r5, r6, lr}
003a54b8: mov      r5, r0
003a54bc: bl       #0x3a3228
003a54c0: mov      r4, r0
003a54c4: mov      r0, r5
003a54c8: bl       #0x3bc5c0
003a54cc: orr      r0, r0, r4, lsl #8
003a54d0: pop      {r4, r5, r6, pc}

# _ZN12AnimationSet13LoadAnimationEi
003659ec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003659f0: ldr      r4, [pc, #0x2fc]
003659f4: ldr      r5, [pc, #0x2fc]
003659f8: mov      r7, r0
003659fc: add      r4, pc, r4
00365a00: ldr      r3, [r4, r5]
00365a04: ldr      r0, [pc, #0x2f0]
00365a08: sub      sp, sp, #0xc4
00365a0c: ldr      r3, [r3]
00365a10: add      r0, pc, r0
00365a14: str      r1, [sp, #4]
00365a18: str      r3, [sp, #0xbc]
00365a1c: bl       #0x3136b4
00365a20: ldr      r3, [sp, #4]
00365a24: cmp      r3, #0
00365a28: blt      #0x365a40
00365a2c: ldr      r2, [pc, #0x2cc]
00365a30: ldr      r2, [r4, r2]
00365a34: ldr      r2, [r2]
00365a38: cmp      r3, r2
00365a3c: blt      #0x365a74
00365a40: ldr      r3, [pc, #0x2bc]
00365a44: ldr      r7, [r4, r3]
00365a48: ldr      r0, [pc, #0x2b8]
00365a4c: add      r0, pc, r0
00365a50: bl       #0x3136b8
00365a54: ldr      r3, [r4, r5]
00365a58: ldr      r2, [sp, #0xbc]
00365a5c: mov      r0, r7
00365a60: ldr      r3, [r3]
00365a64: cmp      r2, r3
00365a68: bne      #0x365cf0
00365a6c: add      sp, sp, #0xc4
00365a70: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00365a74: ldr      r2, [pc, #0x290]
00365a78: ldr      r1, [pc, #0x290]
00365a7c: add      r6, sp, #0x8c
00365a80: ldr      r2, [r4, r2]
00365a84: add      r1, pc, r1
00365a88: mov      r0, r6
00365a8c: ldr      ip, [r2]
00365a90: mov      sl, #0xc
00365a94: add      r2, r1, #7
00365a98: mla      sl, sl, r3, ip
00365a9c: str      r6, [sp, #0x9c]
00365aa0: str      r6, [sp, #0xa0]
00365aa4: bl       #0x3116e8
00365aa8: ldr      r2, [pc, #0x264]
00365aac: mvn      r3, #0
00365ab0: mov      r8, #0
00365ab4: ldr      r2, [r4, r2]
00365ab8: str      r3, [sp, #0xb0]
00365abc: str      r3, [sp, #0xac]
00365ac0: str      r2, [sp, #0xa8]
00365ac4: str      r8, [sp, #0xa4]
00365ac8: str      r8, [sp, #0xb4]
00365acc: str      r8, [sp, #0xb8]
00365ad0: ldr      sb, [sl, #8]
00365ad4: add      sl, sp, #0x1c
00365ad8: mov      r0, sb
00365adc: bl       #0x30de54
00365ae0: mov      r1, sb
00365ae4: add      r2, sb, r0
00365ae8: mov      r0, r6
00365aec: bl       #0x3109e0
00365af0: mov      r2, r8
00365af4: mov      r0, sl
00365af8: ldr      r1, [sp, #0xa0]
00365afc: bl       #0x60f25c
00365b00: ldr      r3, [sp, #0x1c]
00365b04: ldr      r2, [sp, #0x20]
00365b08: cmp      r3, r8
00365b0c: str      r2, [sp, #0x18]
00365b10: str      r3, [sp, #0x14]
00365b14: beq      #0x365b2c
00365b18: ldr      r2, [r3, #4]
00365b1c: cmp      r2, r8
00365b20: addne    r2, r2, #1
00365b24: strne    r2, [r3, #4]
00365b28: ldrne    r3, [sp, #0x14]
00365b2c: ldr      r2, [sp, #0x18]
00365b30: ldr      r1, [sp, #0xa4]
00365b34: str      r3, [sp, #0xa4]
00365b38: ldr      r3, [sp, #0xa8]
00365b3c: add      r0, sp, #0x14
00365b40: str      r1, [sp, #0x14]
00365b44: str      r3, [sp, #0x18]
00365b48: str      r2, [sp, #0xa8]
00365b4c: mov      r8, #0
00365b50: bl       #0x619474
00365b54: mov      r0, sl
00365b58: bl       #0x619474
00365b5c: str      r8, [sp, #0xb0]
00365b60: bl       #0x60b0cc
00365b64: ldr      r3, [r7, #0x20]
00365b68: str      r0, [sp, #0xb4]
00365b6c: str      r8, [sp, #0xb8]
00365b70: cmp      r3, r8
00365b74: beq      #0x365b9c
00365b78: ldrb     r2, [r3, #0x70]
00365b7c: cmp      r2, r8
00365b80: beq      #0x365cd4
00365b84: mov      r0, r3
00365b88: add      r1, r6, #0x18
00365b8c: ldr      r3, [r3]
00365b90: mov      lr, pc
00365b94: ldr      pc, [r3, #0x2c]
00365b98: str      r0, [sp, #0xac]
00365b9c: ldr      r3, [sp, #4]
00365ba0: add      sl, sp, #0xc0
00365ba4: str      r3, [sl, #-0x68]!
00365ba8: add      r3, sl, #4
00365bac: mov      r0, r3
00365bb0: ldr      r2, [sp, #0x9c]
00365bb4: ldr      r1, [sp, #0xa0]
00365bb8: str      r3, [sp, #0x6c]
00365bbc: str      r3, [sp, #0x70]
00365bc0: bl       #0x3116e8
00365bc4: ldr      r3, [sp, #0xa4]
00365bc8: ldr      r2, [sp, #0xa8]
00365bcc: cmp      r3, #0
00365bd0: str      r2, [sp, #0x78]
00365bd4: str      r3, [sp, #0x74]
00365bd8: beq      #0x365bec
00365bdc: ldr      r2, [r3, #4]
00365be0: cmp      r2, #0
00365be4: addne    r2, r2, #1
00365be8: strne    r2, [r3, #4]
00365bec: ldr      r3, [sp, #0x58]
00365bf0: add      r8, sp, #0xc0
00365bf4: ldr      ip, [sp, #0xac]
00365bf8: ldr      lr, [sp, #0xb0]
00365bfc: ldr      sb, [sp, #0xb4]
00365c00: ldr      fp, [sp, #0xb8]
00365c04: str      r3, [r8, #-0x9c]!
00365c08: add      r3, r8, #4
00365c0c: mov      r0, r3
00365c10: ldr      r2, [sp, #0x6c]
00365c14: ldr      r1, [sp, #0x70]
00365c18: str      r3, [sp, #0x38]
00365c1c: str      r3, [sp, #0x3c]
00365c20: str      ip, [sp, #0x7c]
00365c24: str      lr, [sp, #0x80]
00365c28: str      sb, [sp, #0x84]
00365c2c: str      fp, [sp, #0x88]
00365c30: bl       #0x3116e8
00365c34: ldr      r3, [sp, #0x74]
00365c38: ldr      r2, [sp, #0x78]
00365c3c: cmp      r3, #0
00365c40: str      r2, [sp, #0x44]
00365c44: str      r3, [sp, #0x40]
00365c48: beq      #0x365c5c
00365c4c: ldr      r2, [r3, #4]
00365c50: cmp      r2, #0
00365c54: addne    r2, r2, #1
00365c58: strne    r2, [r3, #4]
00365c5c: ldr      r3, [sp, #0x7c]
00365c60: add      r7, r7, #8
00365c64: mov      r2, r8
00365c68: str      r3, [sp, #0x48]
00365c6c: ldr      r3, [sp, #0x80]
00365c70: mov      r1, r7
00365c74: add      r0, sp, #0xc
00365c78: str      r3, [sp, #0x4c]
00365c7c: ldr      r3, [sp, #0x84]
00365c80: str      r3, [sp, #0x50]
00365c84: ldr      r3, [sp, #0x88]
00365c88: str      r3, [sp, #0x54]
00365c8c: bl       #0x365340
00365c90: add      r0, r8, #0x1c
00365c94: bl       #0x619474
00365c98: add      r0, r8, #4
00365c9c: bl       #0x3139ac
00365ca0: add      r0, sl, #0x1c
00365ca4: bl       #0x619474
00365ca8: add      r0, sl, #4
00365cac: bl       #0x3139ac
00365cb0: add      r1, sp, #4
00365cb4: mov      r0, r7
00365cb8: bl       #0x36583c
00365cbc: mov      r7, r0
00365cc0: add      r0, r6, #0x18
00365cc4: bl       #0x619474
00365cc8: mov      r0, r6
00365ccc: bl       #0x3139ac
00365cd0: b        #0x365a48
00365cd4: mov      r0, r3
00365cd8: bl       #0x65f044
00365cdc: str      r0, [sp, #0xac]
00365ce0: add      r1, r6, #0x18
00365ce4: ldr      r0, [r7, #0x20]
00365ce8: bl       #0x62ee9c
00365cec: b        #0x365b9c
00365cf0: bl       #0x30e310
00365cf4: mlseq    r2, r4, r0, pc
00365cf8: andeq    r4, r0, ip, lsr #1
00365cfc: ldrheq   fp, [r5], #-0x30
00365d00: andeq    r2, r0, r8, lsr r2
00365d04: strheq   r4, [r0], -ip
00365d08: subseq   fp, r5, r4, ror r3
00365d0c: andeq    r0, r0, r0, ror lr
00365d10: subseq   r4, r7, ip, lsl ip
00365d14: andeq    r4, r0, r0, lsl r7

# _ZN6glitch7collada20CDynamicAnimationSet16getDatabaseIndexERNS0_16CColladaDatabaseE
0062dbb8: ldr      r2, [r0, #0x28]
0062dbbc: ldr      r3, [r0, #0x24]
0062dbc0: rsb      r2, r3, r2
0062dbc4: asrs     r2, r2, #3
0062dbc8: beq      #0x62dc00
0062dbcc: ldr      ip, [r1]
0062dbd0: ldr      r1, [r3]
0062dbd4: cmp      r1, ip
0062dbd8: moveq    r0, #0
0062dbdc: bxeq     lr
0062dbe0: mov      r0, #0
0062dbe4: b        #0x62dbf4
0062dbe8: ldr      r1, [r3, r0, lsl #3]
0062dbec: cmp      r1, ip
0062dbf0: beq      #0x62dc08
0062dbf4: add      r0, r0, #1
0062dbf8: cmp      r0, r2
0062dbfc: bne      #0x62dbe8
0062dc00: mvn      r0, #0
0062dc04: bx       lr
0062dc08: bx       lr

# _ZNK9Character18GetCharAnimTableIdEv
003a3228: mov      r3, #0x1000
003a322c: ldr      r0, [r0, r3]
003a3230: ldr      r3, [pc, #0x24]
003a3234: cmp      r0, #0
003a3238: add      r3, pc, r3
003a323c: blt      #0x3a3254
003a3240: ldr      r2, [pc, #0x18]
003a3244: ldr      r3, [r3, r2]
003a3248: ldr      r3, [r3]
003a324c: cmp      r0, r3
003a3250: bxlt     lr
003a3254: mov      r0, #0x11
003a3258: bx       lr
003a325c: subseq   r1, pc, r8, asr r8
003a3260: andeq    r2, r0, r0, asr #17

# _ZN12CharAnimator12_SetAnimStepEj
003ca79c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ca7a0: ldr      r3, [r0, #0x2c]
003ca7a4: mov      r2, #0xc
003ca7a8: ldr      r5, [pc, #0x374]
003ca7ac: mla      r3, r2, r3, r0
003ca7b0: ldr      r2, [pc, #0x370]
003ca7b4: add      r5, pc, r5
003ca7b8: mov      r4, r0
003ca7bc: ldr      r2, [r5, r2]
003ca7c0: ldr      r0, [r3, #8]
003ca7c4: mov      ip, #0x14
003ca7c8: ldr      r2, [r2]
003ca7cc: sub      sp, sp, #0x24
003ca7d0: mla      r2, ip, r0, r2
003ca7d4: ldr      r0, [r2, #8]
003ca7d8: cmp      r0, r1
003ca7dc: bhi      #0x3ca7e8
003ca7e0: add      sp, sp, #0x24
003ca7e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ca7e8: ldr      r2, [r2, #0xc]
003ca7ec: mov      r6, #0x38
003ca7f0: str      r1, [r3, #0x10]
003ca7f4: mla      r6, r6, r1, r2
003ca7f8: ldr      r0, [r4, #4]
003ca7fc: mov      r1, #0x26
003ca800: mov      r2, #0
003ca804: bl       #0x3a4d5c
003ca808: ldr      r3, [r6, #0x28]
003ca80c: cmp      r3, #1
003ca810: beq      #0x3caad8
003ca814: ldr      r3, [r6, #0x10]
003ca818: cmn      r3, #1
003ca81c: beq      #0x3ca9d8
003ca820: ldr      r3, [pc, #0x304]
003ca824: ldr      r0, [r5, r3]
003ca828: bl       #0x31f594
003ca82c: cmp      r0, #0
003ca830: beq      #0x3ca870
003ca834: ldr      r7, [r0, #0x128]
003ca838: cmp      r7, #0
003ca83c: beq      #0x3ca870
003ca840: mov      r0, r7
003ca844: ldr      r1, [r4, #4]
003ca848: bl       #0x40f980
003ca84c: cmp      r0, #0
003ca850: beq      #0x3ca870
003ca854: ldr      r1, [r6, #0x10]
003ca858: cmn      r1, #1
003ca85c: beq      #0x3ca9f4
003ca860: mov      r0, r7
003ca864: mov      r2, #0
003ca868: mov      r3, #1
003ca86c: bl       #0x40f904
003ca870: ldrb     r3, [r6, #0x34]
003ca874: strb     r3, [r4, #0x30]
003ca878: ldrb     r3, [r6, #0x34]
003ca87c: cmp      r3, #0
003ca880: moveq    r7, #1
003ca884: bne      #0x3caa10
003ca888: ldr      r3, [pc, #0x2a0]
003ca88c: ldr      r0, [r4, #4]
003ca890: ldr      sb, [r6, #0x2c]
003ca894: ldr      r3, [r5, r3]
003ca898: ldr      fp, [r3]
003ca89c: bl       #0x3935dc
003ca8a0: ldr      lr, [r0]
003ca8a4: ldr      r8, [r0, #4]
003ca8a8: ldr      sl, [r0, #8]
003ca8ac: mov      ip, #0xbf000000
003ca8b0: add      ip, ip, #0x800000
003ca8b4: str      lr, [sp, #0x14]
003ca8b8: mov      r0, fp
003ca8bc: mov      lr, #1
003ca8c0: mov      r1, sb
003ca8c4: add      r2, sp, #0x14
003ca8c8: mov      r3, #0
003ca8cc: str      r8, [sp, #0x18]
003ca8d0: str      sl, [sp, #0x1c]
003ca8d4: str      lr, [sp]
003ca8d8: str      ip, [sp, #8]
003ca8dc: str      ip, [sp, #4]
003ca8e0: bl       #0x36b5d8
003ca8e4: cmp      r7, #0
003ca8e8: beq      #0x3ca91c
003ca8ec: ldr      r8, [r6, #0x18]
003ca8f0: cmn      r8, #1
003ca8f4: beq      #0x3ca91c
003ca8f8: ldrb     r7, [r6, #4]
003ca8fc: cmp      r7, #0
003ca900: beq      #0x3caa94
003ca904: ldr      r3, [pc, #0x228]
003ca908: mov      r1, r8
003ca90c: ldr      r2, [r4, #4]
003ca910: ldr      r0, [r5, r3]
003ca914: mov      r3, #0
003ca918: bl       #0x495f04
003ca91c: ldr      r2, [r6, #0x30]
003ca920: ldr      r3, [r4, #4]
003ca924: str      r2, [r4, #0x34]
003ca928: ldr      r5, [r3, #0x2d8]
003ca92c: cmp      r5, #0
003ca930: beq      #0x3ca9e8
003ca934: ldr      r3, [r6, #8]
003ca938: cmn      r3, #1
003ca93c: beq      #0x3ca9e8
003ca940: mov      r3, #0
003ca944: strb     r3, [r4, #0x48]
003ca948: mov      r0, r4
003ca94c: bl       #0x3c9b7c
003ca950: ldrb     r3, [r4, #0x54]
003ca954: cmp      r3, #0
003ca958: beq      #0x3caa80
003ca95c: ldrb     r1, [r4, #0x49]
003ca960: ldr      r3, [r5, #0x38]
003ca964: ldrb     r2, [r6, #0x1c]
003ca968: cmp      r1, #0
003ca96c: beq      #0x3caac4
003ca970: mov      r1, #0
003ca974: str      r1, [r3, #0x14]
003ca978: mov      r1, #0
003ca97c: str      r1, [r3, #0xc]
003ca980: strb     r2, [r3, #0x10]
003ca984: ldr      r2, [r5, #0x38]
003ca988: ldr      r1, [r6, #8]
003ca98c: mov      r6, #0
003ca990: ldr      r3, [r4, #0x44]
003ca994: ldr      ip, [r2]
003ca998: mov      r0, r2
003ca99c: str      r6, [sp]
003ca9a0: mov      r2, r6
003ca9a4: mov      lr, pc
003ca9a8: ldr      pc, [ip, #0x1c]
003ca9ac: ldr      r1, [r4, #0x34]
003ca9b0: ldr      r0, [r4, #0x40]
003ca9b4: bl       #0x30ed6c
003ca9b8: ldr      r5, [r5, #0x38]
003ca9bc: mov      r1, r0
003ca9c0: mov      r2, r6
003ca9c4: mov      r0, r5
003ca9c8: ldr      r3, [r5]
003ca9cc: mov      lr, pc
003ca9d0: ldr      pc, [r3, #0x28]
003ca9d4: b        #0x3ca7e0
003ca9d8: ldr      r3, [r6, #0x20]
003ca9dc: cmp      r3, #0
003ca9e0: beq      #0x3ca870
003ca9e4: b        #0x3ca820
003ca9e8: mov      r0, r4
003ca9ec: bl       #0x3c9924
003ca9f0: b        #0x3ca7e0
003ca9f4: ldr      r0, [r6, #0x20]
003ca9f8: cmp      r0, #0
003ca9fc: beq      #0x3ca870
003caa00: ldr      r8, [r6, #0x24]
003caa04: bl       #0x3ca708
003caa08: ldr      r1, [r8, r0, lsl #2]
003caa0c: b        #0x3ca860
003caa10: ldr      r0, [r4, #4]
003caa14: mov      r1, #1
003caa18: add      r0, r0, #0x37c
003caa1c: bl       #0x3ffe3c
003caa20: mov      r7, r0
003caa24: ldr      r0, [r4, #4]
003caa28: mov      r1, #2
003caa2c: add      r0, r0, #0x37c
003caa30: bl       #0x3ffe3c
003caa34: mov      r1, r7
003caa38: mov      sl, r0
003caa3c: mov      r0, r4
003caa40: bl       #0x3c94b8
003caa44: mov      r1, r7
003caa48: eor      r8, r0, #1
003caa4c: ldrb     r2, [r6, #4]
003caa50: mov      r0, r4
003caa54: bl       #0x3c955c
003caa58: uxtb     r8, r8
003caa5c: eor      r0, r0, #1
003caa60: cmp      r8, #0
003caa64: uxtb     r7, r0
003caa68: bne      #0x3caaf0
003caa6c: cmp      r7, #0
003caa70: bne      #0x3cab08
003caa74: cmp      r8, #0
003caa78: beq      #0x3ca8e4
003caa7c: b        #0x3ca888
003caa80: ldr      r2, [r5, #0x38]
003caa84: ldrb     r1, [r6, #0x1c]
003caa88: str      r3, [r2, #0xc]
003caa8c: strb     r1, [r2, #0x10]
003caa90: b        #0x3ca984
003caa94: ldr      r0, [r4, #4]
003caa98: bl       #0x3935dc
003caa9c: ldr      r3, [r4, #4]
003caaa0: mov      r2, r0
003caaa4: ldr      r0, [pc, #0x88]
003caaa8: mov      r1, r8
003caaac: add      r3, r3, #0x16c
003caab0: ldr      r0, [r5, r0]
003caab4: str      r7, [sp, #4]
003caab8: str      r7, [sp]
003caabc: bl       #0x495888
003caac0: b        #0x3ca91c
003caac4: ldrb     r1, [r4, #0x4a]
003caac8: cmp      r1, #0
003caacc: ldreq    r1, [r6, #0xc]
003caad0: beq      #0x3ca974
003caad4: b        #0x3ca970
003caad8: ldr      r2, [r4, #0x2c]
003caadc: mov      r0, r4
003caae0: ldr      r1, [r6, #8]
003caae4: add      r2, r2, #1
003caae8: bl       #0x3cab38
003caaec: b        #0x3ca7e0
003caaf0: mov      r0, r4
003caaf4: mov      r1, sl
003caaf8: bl       #0x3c94b8
003caafc: eor      r0, r0, #1
003cab00: uxtb     r8, r0
003cab04: b        #0x3caa6c
003cab08: mov      r1, sl
003cab0c: mov      r0, r4
003cab10: ldrb     r2, [r6, #4]
003cab14: bl       #0x3c955c
003cab18: eor      r0, r0, #1
003cab1c: uxtb     r8, r0
003cab20: b        #0x3caa74
003cab24: ldrsbeq  sl, [ip], #-0x2c
003cab28: andeq    r3, r0, ip, ror ip
003cab2c: strdeq   r3, r4, [r0], -r4
003cab30: andeq    r0, r0, r4, lsr #27
003cab34: andeq    r1, r0, r8, lsl #22

# _ZN6CSIdle7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3020: push     {r4, r5, r6, r7, r8, lr}
003c3024: ldr      r4, [pc, #0xf4]
003c3028: ldr      r7, [pc, #0xf4]
003c302c: ldr      r1, [pc, #0xf4]
003c3030: add      r4, pc, r4
003c3034: ldr      r3, [r4, r7]
003c3038: ldr      r8, [r4, r1]
003c303c: sub      sp, sp, #0x20
003c3040: ldr      r3, [r3]
003c3044: mov      r0, r8
003c3048: mov      r6, r2
003c304c: str      r3, [sp, #0x1c]
003c3050: bl       #0x337888
003c3054: ldr      r1, [pc, #0xd0]
003c3058: add      r5, sp, #4
003c305c: mov      r2, sp
003c3060: add      r1, pc, r1
003c3064: mov      r0, r5
003c3068: bl       #0x3140ec
003c306c: mov      r1, r5
003c3070: mov      r0, r8
003c3074: bl       #0x337a88
003c3078: mov      r0, r5
003c307c: bl       #0x318254
003c3080: ldrb     r3, [r6, #0x538]
003c3084: cmp      r3, #0
003c3088: beq      #0x3c30a8
003c308c: ldr      r3, [r4, r7]
003c3090: ldr      r2, [sp, #0x1c]
003c3094: ldr      r3, [r3]
003c3098: cmp      r2, r3
003c309c: bne      #0x3c311c
003c30a0: add      sp, sp, #0x20
003c30a4: pop      {r4, r5, r6, r7, r8, pc}
003c30a8: mov      r3, #0x2380
003c30ac: str      r3, [r6, #0x520]
003c30b0: ldr      r3, [pc, #0x78]
003c30b4: mov      r0, r6
003c30b8: add      r5, r6, #0x490
003c30bc: ldr      r3, [r4, r3]
003c30c0: add      r5, r5, #0xc
003c30c4: ldr      r8, [r3]
003c30c8: bl       #0x3a3228
003c30cc: ldr      r3, [pc, #0x60]
003c30d0: ldr      r1, [pc, #0x60]
003c30d4: ldr      r2, [r4, r3]
003c30d8: mov      r3, #0xa0
003c30dc: mla      r3, r3, r0, r8
003c30e0: ldr      r0, [r2, #0x2c]
003c30e4: ldr      r2, [pc, #0x50]
003c30e8: add      r1, pc, r1
003c30ec: ldr      r8, [r3, #0x28]
003c30f0: add      r2, pc, r2
003c30f4: bl       #0x4c4bdc
003c30f8: ands     r0, r0, #2
003c30fc: bne      #0x3c3110
003c3100: add      r1, r0, r8
003c3104: mov      r0, r5
003c3108: bl       #0x3cacb0
003c310c: b        #0x3c308c
003c3110: mov      r0, r6
003c3114: bl       #0x3a53e0
003c3118: b        #0x3c3100
003c311c: bl       #0x30e310
003c3120: subseq   r1, sp, r0, ror #20
003c3124: andeq    r4, r0, ip, lsr #1
003c3128: andeq    r0, r0, r4, lsl #17
003c312c: ldrsheq  r1, [r0], #-0xd0
003c3130: andeq    r4, r0, r4, asr #16
003c3134: strdeq   r3, r4, [r0], -r4
003c3138: ldrsbeq  r1, [r0], #-0xa0
003c313c: ldrsbeq  r1, [r0], #-0xa8

# _ZN6glitch7collada13CAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
006601d4: push     {r4, lr}
006601d8: mov      r4, r0
006601dc: add      r0, r0, #0x24
006601e0: bl       #0x62ecac
006601e4: ldr      r3, [r4, #0x24]
006601e8: ldr      r0, [r4, #0x28]
006601ec: rsb      r0, r3, r0
006601f0: asr      r0, r0, #3
006601f4: sub      r0, r0, #1
006601f8: pop      {r4, pc}

# _ZNSt3mapIi9AnimationSt4lessIiEN6glitch4core10SAllocatorISt4pairIKiS0_ELNS3_6memory13E_MEMORY_HINTE0EEEEixIiEERS0_RKT_
0036583c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00365840: ldr      r5, [pc, #0x194]
00365844: ldr      r6, [pc, #0x194]
00365848: ldr      r4, [r0, #4]
0036584c: add      r5, pc, r5
00365850: ldr      r3, [r5, r6]
00365854: sub      sp, sp, #0x70
00365858: cmp      r4, #0
0036585c: ldr      r3, [r3]
00365860: mov      sl, r0
00365864: mov      r7, r1
00365868: str      r3, [sp, #0x6c]
0036586c: moveq    r4, r0
00365870: beq      #0x3658a4
00365874: ldr      r1, [r1]
00365878: mov      r2, r0
0036587c: b        #0x365888
00365880: mov      r2, r4
00365884: mov      r4, r3
00365888: ldr      r3, [r4, #0x10]
0036588c: cmp      r3, r1
00365890: ldrlt    r3, [r4, #0xc]
00365894: ldrge    r3, [r4, #8]
00365898: movlt    r4, r2
0036589c: cmp      r3, #0
003658a0: bne      #0x365880
003658a4: cmp      sl, r4
003658a8: beq      #0x3658c0
003658ac: ldr      r2, [r7]
003658b0: ldr      r3, [r4, #0x10]
003658b4: mov      r0, r4
003658b8: cmp      r2, r3
003658bc: bge      #0x3659b8
003658c0: ldr      r1, [pc, #0x11c]
003658c4: add      r8, sp, #0x3c
003658c8: mov      r0, r8
003658cc: add      r1, pc, r1
003658d0: add      r2, r1, #7
003658d4: str      r8, [sp, #0x4c]
003658d8: str      r8, [sp, #0x50]
003658dc: bl       #0x3116e8
003658e0: ldr      r3, [r7]
003658e4: add      r7, sp, #0x70
003658e8: mov      ip, #0
003658ec: str      r3, [r7, #-0x68]!
003658f0: ldr      r3, [pc, #0xf0]
003658f4: mvn      lr, #0
003658f8: ldr      r2, [sp, #0x4c]
003658fc: ldr      sb, [r5, r3]
00365900: add      r3, r7, #4
00365904: mov      r0, r3
00365908: ldr      r1, [sp, #0x50]
0036590c: str      r3, [sp, #0x1c]
00365910: str      r3, [sp, #0x20]
00365914: str      sb, [sp, #0x58]
00365918: str      lr, [sp, #0x60]
0036591c: str      ip, [sp, #0x68]
00365920: str      ip, [sp, #0x54]
00365924: str      lr, [sp, #0x5c]
00365928: str      ip, [sp, #0x64]
0036592c: bl       #0x3116e8
00365930: ldr      r3, [sp, #0x54]
00365934: ldr      r2, [sp, #0x58]
00365938: cmp      r3, #0
0036593c: str      r2, [sp, #0x28]
00365940: str      r3, [sp, #0x24]
00365944: beq      #0x365958
00365948: ldr      r2, [r3, #4]
0036594c: cmp      r2, #0
00365950: addne    r2, r2, #1
00365954: strne    r2, [r3, #4]
00365958: ldr      ip, [sp, #0x5c]
0036595c: mov      r1, sl
00365960: mov      r2, sp
00365964: str      ip, [sp, #0x2c]
00365968: ldr      ip, [sp, #0x60]
0036596c: mov      r3, r7
00365970: add      r0, sp, #4
00365974: str      ip, [sp, #0x30]
00365978: ldr      ip, [sp, #0x64]
0036597c: str      r4, [sp]
00365980: str      ip, [sp, #0x34]
00365984: ldr      ip, [sp, #0x68]
00365988: str      ip, [sp, #0x38]
0036598c: bl       #0x3654c8
00365990: add      r0, r7, #0x1c
00365994: ldr      r4, [sp, #4]
00365998: bl       #0x619474
0036599c: add      r0, r7, #4
003659a0: bl       #0x3139ac
003659a4: add      r0, r8, #0x18
003659a8: bl       #0x619474
003659ac: mov      r0, r8
003659b0: bl       #0x3139ac
003659b4: mov      r0, r4
003659b8: ldr      r3, [r5, r6]
003659bc: ldr      r2, [sp, #0x6c]
003659c0: add      r0, r0, #0x14
003659c4: ldr      r3, [r3]
003659c8: cmp      r2, r3
003659cc: bne      #0x3659d8
003659d0: add      sp, sp, #0x70
003659d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003659d8: bl       #0x30e310
003659dc: rsbeq    pc, r2, r4, asr #4
003659e0: andeq    r4, r0, ip, lsr #1
003659e4: ldrsbeq  r4, [r7], #-0xd4
003659e8: andeq    r4, r0, r0, lsl r7

# _ZNK9Character14GetCharModelIdEv
003a31e8: movw     r3, #0x1004
003a31ec: ldr      r0, [r0, r3]
003a31f0: ldr      r3, [pc, #0x28]
003a31f4: cmp      r0, #0
003a31f8: add      r3, pc, r3
003a31fc: bge      #0x3a3208
003a3200: mvn      r0, #0
003a3204: bx       lr
003a3208: ldr      r2, [pc, #0x14]
003a320c: ldr      r3, [r3, r2]
003a3210: ldr      r3, [r3]
003a3214: cmp      r0, r3
003a3218: bxlt     lr
003a321c: b        #0x3a3200

# _ZN6glitch7collada20CDynamicAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
0062e8a8: push     {r4, lr}
0062e8ac: mov      r4, r0
0062e8b0: bl       #0x6601d4
0062e8b4: mov      r3, #1
0062e8b8: strb     r3, [r4, #0x70]
0062e8bc: pop      {r4, pc}
