
# _ZNK9Character11GetCharTypeEv
003a3054: push     {r4, lr}
003a3058: bl       #0x3a3024
003a305c: ldr      r0, [r0, #0x38]
003a3060: pop      {r4, pc}

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
