
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061b2f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const>:
  61b2f4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  61b2f8: e2524000     	subs	r4, r2, #0
  61b2fc: e24dd044     	sub	sp, sp, #68
  61b300: e1a08000     	mov	r8, r0
  61b304: e1a0b001     	mov	r11, r1
  61b308: e1a0a003     	mov	r10, r3
  61b30c: 01a06004     	moveq	r6, r4
  61b310: 0a000091     	beq	0x61b55c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x268> @ imm = #0x244
  61b314: e594304c     	ldr	r3, [r4, #0x4c]
  61b318: e3530000     	cmp	r3, #0
  61b31c: 0a000154     	beq	0x61b874 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x580> @ imm = #0x550
  61b320: e5903004     	ldr	r3, [r0, #0x4]
  61b324: e1a01000     	mov	r1, r0
  61b328: e1a00003     	mov	r0, r3
  61b32c: e5933000     	ldr	r3, [r3]
  61b330: e1a0e00f     	mov	lr, pc
  61b334: e593f040     	ldr	pc, [r3, #0x40]
  61b338: e1a06000     	mov	r6, r0
  61b33c: e5941040     	ldr	r1, [r4, #0x40]
  61b340: e3510000     	cmp	r1, #0
  61b344: da00003a     	ble	0x61b434 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x140> @ imm = #0xe8
  61b348: e28d3038     	add	r3, sp, #56
  61b34c: e28dc03c     	add	r12, sp, #60
  61b350: e3a05000     	mov	r5, #0
  61b354: e58d3008     	str	r3, [sp, #0x8]
  61b358: e58dc00c     	str	r12, [sp, #0xc]
  61b35c: e1a07006     	mov	r7, r6
  61b360: e5942044     	ldr	r2, [r4, #0x44]
  61b364: e1a06185     	lsl	r6, r5, #3
  61b368: e7923185     	ldr	r3, [r2, r5, lsl #3]
  61b36c: e0822006     	add	r2, r2, r6
  61b370: e2433001     	sub	r3, r3, #1
  61b374: e353000c     	cmp	r3, #12
  61b378: 908ff103     	addls	pc, pc, r3, lsl #2
  61b37c: ea000028     	b	0x61b424 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x130> @ imm = #0xa0
  61b380: ea000131     	b	0x61b84c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x558> @ imm = #0x4c4
  61b384: ea0000e5     	b	0x61b720 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x42c> @ imm = #0x394
  61b388: ea0000bf     	b	0x61b68c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x398> @ imm = #0x2fc
  61b38c: ea0000b4     	b	0x61b664 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x370> @ imm = #0x2d0
  61b390: ea000023     	b	0x61b424 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x130> @ imm = #0x8c
  61b394: ea000022     	b	0x61b424 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x130> @ imm = #0x88
  61b398: ea000021     	b	0x61b424 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x130> @ imm = #0x84
  61b39c: ea000020     	b	0x61b424 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x130> @ imm = #0x80
  61b3a0: ea000117     	b	0x61b804 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x510> @ imm = #0x45c
  61b3a4: ea000002     	b	0x61b3b4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0xc0> @ imm = #0x8
  61b3a8: ea00011e     	b	0x61b828 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x534> @ imm = #0x478
  61b3ac: ea000098     	b	0x61b614 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x320> @ imm = #0x260
  61b3b0: ea00006c     	b	0x61b568 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x274> @ imm = #0x1b0
  61b3b4: e5921004     	ldr	r1, [r2, #0x4]
  61b3b8: e1a00008     	mov	r0, r8
  61b3bc: e1a0200b     	mov	r2, r11
  61b3c0: e1a0300a     	mov	r3, r10
  61b3c4: ebfffc8f     	bl	0x61a608 <glitch::collada::CColladaDatabase::constructGNPSEmitter(glitch::collada::SInstanceGNPSEmitter*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const> @ imm = #-0xdc4
  61b3c8: e2509000     	subs	r9, r0, #0
  61b3cc: 0a00008b     	beq	0x61b600 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x30c> @ imm = #0x22c
  61b3d0: e5942044     	ldr	r2, [r4, #0x44]
  61b3d4: e5993000     	ldr	r3, [r9]
  61b3d8: e0826006     	add	r6, r2, r6
  61b3dc: e5962004     	ldr	r2, [r6, #0x4]
  61b3e0: e5921014     	ldr	r1, [r2, #0x14]
  61b3e4: e1a0e00f     	mov	lr, pc
  61b3e8: e593f0d4     	ldr	pc, [r3, #0xd4]
  61b3ec: e1a00009     	mov	r0, r9
  61b3f0: e5993000     	ldr	r3, [r9]
  61b3f4: e1a0e00f     	mov	lr, pc
  61b3f8: e593f104     	ldr	pc, [r3, #0x104]
  61b3fc: e1a00007     	mov	r0, r7
  61b400: e5973000     	ldr	r3, [r7]
  61b404: e1a01009     	mov	r1, r9
  61b408: e1a0e00f     	mov	lr, pc
  61b40c: e593f05c     	ldr	pc, [r3, #0x5c]
  61b410: e5993000     	ldr	r3, [r9]
  61b414: e513000c     	ldr	r0, [r3, #-0xc]
  61b418: e0890000     	add	r0, r9, r0
  61b41c: ebf40858     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fdea0
  61b420: e5941040     	ldr	r1, [r4, #0x40]
  61b424: e2855001     	add	r5, r5, #1
  61b428: e1550001     	cmp	r5, r1
  61b42c: baffffcb     	blt	0x61b360 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x6c> @ imm = #-0xd4
  61b430: e1a06007     	mov	r6, r7
  61b434: e1a00006     	mov	r0, r6
  61b438: e5941004     	ldr	r1, [r4, #0x4]
  61b43c: e5963000     	ldr	r3, [r6]
  61b440: e1a0e00f     	mov	lr, pc
  61b444: e593f028     	ldr	pc, [r3, #0x28]
  61b448: e5963000     	ldr	r3, [r6]
  61b44c: e594200c     	ldr	r2, [r4, #0xc]
  61b450: e1a00006     	mov	r0, r6
  61b454: e59330a4     	ldr	r3, [r3, #0xa4]
  61b458: e58d202c     	str	r2, [sp, #0x2c]
  61b45c: e5942010     	ldr	r2, [r4, #0x10]
  61b460: e28d102c     	add	r1, sp, #44
  61b464: e58d2030     	str	r2, [sp, #0x30]
  61b468: e5942014     	ldr	r2, [r4, #0x14]
  61b46c: e58d2034     	str	r2, [sp, #0x34]
  61b470: e12fff33     	blx	r3
  61b474: e5963000     	ldr	r3, [r6]
  61b478: e5942018     	ldr	r2, [r4, #0x18]
  61b47c: e1a00006     	mov	r0, r6
  61b480: e593309c     	ldr	r3, [r3, #0x9c]
  61b484: e58d2010     	str	r2, [sp, #0x10]
  61b488: e594201c     	ldr	r2, [r4, #0x1c]
  61b48c: e28d1010     	add	r1, sp, #16
  61b490: e58d2014     	str	r2, [sp, #0x14]
  61b494: e5942020     	ldr	r2, [r4, #0x20]
  61b498: e58d2018     	str	r2, [sp, #0x18]
  61b49c: e5942024     	ldr	r2, [r4, #0x24]
  61b4a0: e58d201c     	str	r2, [sp, #0x1c]
  61b4a4: e12fff33     	blx	r3
  61b4a8: e5963000     	ldr	r3, [r6]
  61b4ac: e5942028     	ldr	r2, [r4, #0x28]
  61b4b0: e1a00006     	mov	r0, r6
  61b4b4: e5933094     	ldr	r3, [r3, #0x94]
  61b4b8: e58d2020     	str	r2, [sp, #0x20]
  61b4bc: e594202c     	ldr	r2, [r4, #0x2c]
  61b4c0: e28d1020     	add	r1, sp, #32
  61b4c4: e58d2024     	str	r2, [sp, #0x24]
  61b4c8: e5942030     	ldr	r2, [r4, #0x30]
  61b4cc: e58d2028     	str	r2, [sp, #0x28]
  61b4d0: e12fff33     	blx	r3
  61b4d4: e5941034     	ldr	r1, [r4, #0x34]
  61b4d8: e5963000     	ldr	r3, [r6]
  61b4dc: e1a00006     	mov	r0, r6
  61b4e0: e2511000     	subs	r1, r1, #0
  61b4e4: 13a01001     	movne	r1, #1
  61b4e8: e1a0e00f     	mov	lr, pc
  61b4ec: e593f048     	ldr	pc, [r3, #0x48]
  61b4f0: e5943038     	ldr	r3, [r4, #0x38]
  61b4f4: e3530000     	cmp	r3, #0
  61b4f8: da000017     	ble	0x61b55c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x268> @ imm = #0x5c
  61b4fc: e3a05000     	mov	r5, #0
  61b500: e1a07005     	mov	r7, r5
  61b504: e1a09008     	mov	r9, r8
  61b508: e594203c     	ldr	r2, [r4, #0x3c]
  61b50c: e1a0300a     	mov	r3, r10
  61b510: e1a0100b     	mov	r1, r11
  61b514: e0822005     	add	r2, r2, r5
  61b518: e1a00009     	mov	r0, r9
  61b51c: ebffff74     	bl	0x61b2f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x230
  61b520: e5963000     	ldr	r3, [r6]
  61b524: e1a08000     	mov	r8, r0
  61b528: e1a01000     	mov	r1, r0
  61b52c: e1a00006     	mov	r0, r6
  61b530: e1a0e00f     	mov	lr, pc
  61b534: e593f05c     	ldr	pc, [r3, #0x5c]
  61b538: e5983000     	ldr	r3, [r8]
  61b53c: e2877001     	add	r7, r7, #1
  61b540: e2855050     	add	r5, r5, #80
  61b544: e513000c     	ldr	r0, [r3, #-0xc]
  61b548: e0880000     	add	r0, r8, r0
  61b54c: ebf4080c     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fdfd0
  61b550: e5943038     	ldr	r3, [r4, #0x38]
  61b554: e1570003     	cmp	r7, r3
  61b558: baffffea     	blt	0x61b508 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x214> @ imm = #-0x58
  61b55c: e1a00006     	mov	r0, r6
  61b560: e28dd044     	add	sp, sp, #68
  61b564: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  61b568: e5922004     	ldr	r2, [r2, #0x4]
  61b56c: e59d0008     	ldr	r0, [sp, #0x8]
  61b570: e1a01008     	mov	r1, r8
  61b574: e1a0300a     	mov	r3, r10
  61b578: ebffcc5c     	bl	0x60e6f0 <glitch::collada::CColladaDatabase::constructModularSkin(glitch::collada::SInstanceModularSkin*, glitch::collada::CRootSceneNode*) const> @ imm = #-0xce90
  61b57c: e5983004     	ldr	r3, [r8, #0x4]
  61b580: e1a01008     	mov	r1, r8
  61b584: e59d2008     	ldr	r2, [sp, #0x8]
  61b588: e1a00003     	mov	r0, r3
  61b58c: e593c000     	ldr	r12, [r3]
  61b590: e5943048     	ldr	r3, [r4, #0x48]
  61b594: e1a0e00f     	mov	lr, pc
  61b598: e59cf050     	ldr	pc, [r12, #0x50]
  61b59c: e2509000     	subs	r9, r0, #0
  61b5a0: 0a000012     	beq	0x61b5f0 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x2fc> @ imm = #0x48
  61b5a4: e5942044     	ldr	r2, [r4, #0x44]
  61b5a8: e5993000     	ldr	r3, [r9]
  61b5ac: e0826006     	add	r6, r2, r6
  61b5b0: e5962004     	ldr	r2, [r6, #0x4]
  61b5b4: e5921014     	ldr	r1, [r2, #0x14]
  61b5b8: e1a0e00f     	mov	lr, pc
  61b5bc: e593f0d4     	ldr	pc, [r3, #0xd4]
  61b5c0: e1a00009     	mov	r0, r9
  61b5c4: e3a01002     	mov	r1, #2
  61b5c8: ebfdeef3     	bl	0x59719c <glitch::scene::ISceneNode::setAutomaticCulling(glitch::scene::E_CULLING_TYPE)> @ imm = #-0x84434
  61b5cc: e1a00007     	mov	r0, r7
  61b5d0: e5973000     	ldr	r3, [r7]
  61b5d4: e1a01009     	mov	r1, r9
  61b5d8: e1a0e00f     	mov	lr, pc
  61b5dc: e593f05c     	ldr	pc, [r3, #0x5c]
  61b5e0: e5993000     	ldr	r3, [r9]
  61b5e4: e513000c     	ldr	r0, [r3, #-0xc]
  61b5e8: e0890000     	add	r0, r9, r0
  61b5ec: ebf407e4     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fe070
  61b5f0: e59d0038     	ldr	r0, [sp, #0x38]
  61b5f4: e3500000     	cmp	r0, #0
  61b5f8: 0a000000     	beq	0x61b600 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x30c> @ imm = #0x0
  61b5fc: ebf407e0     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fe080
  61b600: e5941040     	ldr	r1, [r4, #0x40]
  61b604: e2855001     	add	r5, r5, #1
  61b608: e1550001     	cmp	r5, r1
  61b60c: baffff53     	blt	0x61b360 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x6c> @ imm = #-0x2b4
  61b610: eaffff86     	b	0x61b430 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x13c> @ imm = #-0x1e8
  61b614: e5921004     	ldr	r1, [r2, #0x4]
  61b618: e1a00008     	mov	r0, r8
  61b61c: e1a0200a     	mov	r2, r10
  61b620: ebfffcdb     	bl	0x61a994 <glitch::collada::CColladaDatabase::constructForce(glitch::collada::SInstanceForce*, glitch::collada::CRootSceneNode*) const> @ imm = #-0xc94
  61b624: e2506000     	subs	r6, r0, #0
  61b628: 0afffff4     	beq	0x61b600 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x30c> @ imm = #-0x30
  61b62c: e1a01006     	mov	r1, r6
  61b630: e1a00007     	mov	r0, r7
  61b634: e5973000     	ldr	r3, [r7]
  61b638: e1a0e00f     	mov	lr, pc
  61b63c: e593f05c     	ldr	pc, [r3, #0x5c]
  61b640: e5963000     	ldr	r3, [r6]
  61b644: e2855001     	add	r5, r5, #1
  61b648: e513000c     	ldr	r0, [r3, #-0xc]
  61b64c: e0860000     	add	r0, r6, r0
  61b650: ebf407cb     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fe0d4
  61b654: e5941040     	ldr	r1, [r4, #0x40]
  61b658: e1550001     	cmp	r5, r1
  61b65c: baffff3f     	blt	0x61b360 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x6c> @ imm = #-0x304
  61b660: eaffff72     	b	0x61b430 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x13c> @ imm = #-0x238
  61b664: e5923004     	ldr	r3, [r2, #0x4]
  61b668: e1a00008     	mov	r0, r8
  61b66c: e1a0200a     	mov	r2, r10
  61b670: e5931004     	ldr	r1, [r3, #0x4]
  61b674: e2811001     	add	r1, r1, #1
  61b678: ebfffef3     	bl	0x61b24c <glitch::collada::CColladaDatabase::constructLight(char const*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x434
  61b67c: e2506000     	subs	r6, r0, #0
  61b680: 1affffe9     	bne	0x61b62c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x338> @ imm = #-0x5c
  61b684: e5941040     	ldr	r1, [r4, #0x40]
  61b688: eaffffdd     	b	0x61b604 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x310> @ imm = #-0x8c
  61b68c: e5923004     	ldr	r3, [r2, #0x4]
  61b690: e59d000c     	ldr	r0, [sp, #0xc]
  61b694: e1a01008     	mov	r1, r8
  61b698: e1a0200b     	mov	r2, r11
  61b69c: e58da000     	str	r10, [sp]
  61b6a0: ebfffe04     	bl	0x61aeb8 <glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x7f0
  61b6a4: e59d003c     	ldr	r0, [sp, #0x3c]
  61b6a8: e3500000     	cmp	r0, #0
  61b6ac: e58d0038     	str	r0, [sp, #0x38]
  61b6b0: 15903004     	ldrne	r3, [r0, #0x4]
  61b6b4: 12833001     	addne	r3, r3, #1
  61b6b8: 15803004     	strne	r3, [r0, #0x4]
  61b6bc: 159d003c     	ldrne	r0, [sp, #0x3c]
  61b6c0: e3500000     	cmp	r0, #0
  61b6c4: 0a000000     	beq	0x61b6cc <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x3d8> @ imm = #0x0
  61b6c8: ebf407ad     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fe14c
  61b6cc: e59d3038     	ldr	r3, [sp, #0x38]
  61b6d0: e3530000     	cmp	r3, #0
  61b6d4: 0affffc9     	beq	0x61b600 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x30c> @ imm = #-0xdc
  61b6d8: e5983004     	ldr	r3, [r8, #0x4]
  61b6dc: e1a01008     	mov	r1, r8
  61b6e0: e59d2008     	ldr	r2, [sp, #0x8]
  61b6e4: e1a00003     	mov	r0, r3
  61b6e8: e593c000     	ldr	r12, [r3]
  61b6ec: e5943048     	ldr	r3, [r4, #0x48]
  61b6f0: e1a0e00f     	mov	lr, pc
  61b6f4: e59cf048     	ldr	pc, [r12, #0x48]
  61b6f8: e2509000     	subs	r9, r0, #0
  61b6fc: 0a00003b     	beq	0x61b7f0 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x4fc> @ imm = #0xec
  61b700: e5942044     	ldr	r2, [r4, #0x44]
  61b704: e5993000     	ldr	r3, [r9]
  61b708: e0826006     	add	r6, r2, r6
  61b70c: e5962004     	ldr	r2, [r6, #0x4]
  61b710: e5921014     	ldr	r1, [r2, #0x14]
  61b714: e1a0e00f     	mov	lr, pc
  61b718: e593f0d4     	ldr	pc, [r3, #0xd4]
  61b71c: ea00002a     	b	0x61b7cc <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x4d8> @ imm = #0xa8
  61b720: e5923004     	ldr	r3, [r2, #0x4]
  61b724: e3a0c001     	mov	r12, #1
  61b728: e59d0008     	ldr	r0, [sp, #0x8]
  61b72c: e1a01008     	mov	r1, r8
  61b730: e1a0200b     	mov	r2, r11
  61b734: e88d1400     	stm	sp, {r10, r12}
  61b738: ebfffd6a     	bl	0x61ace8 <glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const> @ imm = #-0xa58
  61b73c: e59d3038     	ldr	r3, [sp, #0x38]
  61b740: e1a00003     	mov	r0, r3
  61b744: e5933000     	ldr	r3, [r3]
  61b748: e1a0e00f     	mov	lr, pc
  61b74c: e593f030     	ldr	pc, [r3, #0x30]
  61b750: e3500002     	cmp	r0, #2
  61b754: 0a00004e     	beq	0x61b894 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x5a0> @ imm = #0x138
  61b758: e59d3038     	ldr	r3, [sp, #0x38]
  61b75c: e1a00003     	mov	r0, r3
  61b760: e5933000     	ldr	r3, [r3]
  61b764: e1a0e00f     	mov	lr, pc
  61b768: e593f030     	ldr	pc, [r3, #0x30]
  61b76c: e3500003     	cmp	r0, #3
  61b770: 0a000047     	beq	0x61b894 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x5a0> @ imm = #0x11c
  61b774: e5983004     	ldr	r3, [r8, #0x4]
  61b778: e1a01008     	mov	r1, r8
  61b77c: e59d2008     	ldr	r2, [sp, #0x8]
  61b780: e1a00003     	mov	r0, r3
  61b784: e593c000     	ldr	r12, [r3]
  61b788: e5943048     	ldr	r3, [r4, #0x48]
  61b78c: e1a0e00f     	mov	lr, pc
  61b790: e59cf048     	ldr	pc, [r12, #0x48]
  61b794: e1a09000     	mov	r9, r0
  61b798: e3590000     	cmp	r9, #0
  61b79c: 0a000013     	beq	0x61b7f0 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x4fc> @ imm = #0x4c
  61b7a0: e5942044     	ldr	r2, [r4, #0x44]
  61b7a4: e1a00009     	mov	r0, r9
  61b7a8: e5993000     	ldr	r3, [r9]
  61b7ac: e0826006     	add	r6, r2, r6
  61b7b0: e5962004     	ldr	r2, [r6, #0x4]
  61b7b4: e5921014     	ldr	r1, [r2, #0x14]
  61b7b8: e1a0e00f     	mov	lr, pc
  61b7bc: e593f0d4     	ldr	pc, [r3, #0xd4]
  61b7c0: e1a00009     	mov	r0, r9
  61b7c4: e3a01002     	mov	r1, #2
  61b7c8: ebfdee73     	bl	0x59719c <glitch::scene::ISceneNode::setAutomaticCulling(glitch::scene::E_CULLING_TYPE)> @ imm = #-0x84634
  61b7cc: e1a00007     	mov	r0, r7
  61b7d0: e5973000     	ldr	r3, [r7]
  61b7d4: e1a01009     	mov	r1, r9
  61b7d8: e1a0e00f     	mov	lr, pc
  61b7dc: e593f05c     	ldr	pc, [r3, #0x5c]
  61b7e0: e5993000     	ldr	r3, [r9]
  61b7e4: e513000c     	ldr	r0, [r3, #-0xc]
  61b7e8: e0890000     	add	r0, r9, r0
  61b7ec: ebf40764     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fe270
  61b7f0: e59d0038     	ldr	r0, [sp, #0x38]
  61b7f4: e3500000     	cmp	r0, #0
  61b7f8: 1affff07     	bne	0x61b41c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x128> @ imm = #-0x3e4
  61b7fc: e5941040     	ldr	r1, [r4, #0x40]
  61b800: eaffff7f     	b	0x61b604 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x310> @ imm = #-0x204
  61b804: e5921004     	ldr	r1, [r2, #0x4]
  61b808: e1a00008     	mov	r0, r8
  61b80c: e1a0200b     	mov	r2, r11
  61b810: e1a0300a     	mov	r3, r10
  61b814: ebfffc1b     	bl	0x61a888 <glitch::collada::CColladaDatabase::constructEmitter(glitch::collada::SInstanceEmitter*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const> @ imm = #-0xf94
  61b818: e2509000     	subs	r9, r0, #0
  61b81c: 1afffeeb     	bne	0x61b3d0 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0xdc> @ imm = #-0x454
  61b820: e5941040     	ldr	r1, [r4, #0x40]
  61b824: eaffff76     	b	0x61b604 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x310> @ imm = #-0x228
  61b828: e5921004     	ldr	r1, [r2, #0x4]
  61b82c: e1a00008     	mov	r0, r8
  61b830: e1a0200b     	mov	r2, r11
  61b834: e1a0300a     	mov	r3, r10
  61b838: ebfffbcf     	bl	0x61a77c <glitch::collada::CColladaDatabase::constructCoronas(glitch::collada::SInstanceCoronas*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x10c4
  61b83c: e2506000     	subs	r6, r0, #0
  61b840: 1affff79     	bne	0x61b62c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x338> @ imm = #-0x21c
  61b844: e5941040     	ldr	r1, [r4, #0x40]
  61b848: eaffff6d     	b	0x61b604 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x310> @ imm = #-0x24c
  61b84c: e5923004     	ldr	r3, [r2, #0x4]
  61b850: e1a00008     	mov	r0, r8
  61b854: e1a0200a     	mov	r2, r10
  61b858: e5931004     	ldr	r1, [r3, #0x4]
  61b85c: e2811001     	add	r1, r1, #1
  61b860: ebfffe9a     	bl	0x61b2d0 <glitch::collada::CColladaDatabase::constructCamera(char const*, glitch::collada::CRootSceneNode*) const> @ imm = #-0x598
  61b864: e2506000     	subs	r6, r0, #0
  61b868: 1affff6f     	bne	0x61b62c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x338> @ imm = #-0x244
  61b86c: e5941040     	ldr	r1, [r4, #0x40]
  61b870: eaffff63     	b	0x61b604 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x310> @ imm = #-0x274
  61b874: e5903004     	ldr	r3, [r0, #0x4]
  61b878: e1a01000     	mov	r1, r0
  61b87c: e1a00003     	mov	r0, r3
  61b880: e5933000     	ldr	r3, [r3]
  61b884: e1a0e00f     	mov	lr, pc
  61b888: e593f03c     	ldr	pc, [r3, #0x3c]
  61b88c: e1a06000     	mov	r6, r0
  61b890: eafffea9     	b	0x61b33c <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x48> @ imm = #-0x55c
  61b894: e5983004     	ldr	r3, [r8, #0x4]
  61b898: e1a01008     	mov	r1, r8
  61b89c: e59d2008     	ldr	r2, [sp, #0x8]
  61b8a0: e1a00003     	mov	r0, r3
  61b8a4: e593c000     	ldr	r12, [r3]
  61b8a8: e5943048     	ldr	r3, [r4, #0x48]
  61b8ac: e1a0e00f     	mov	lr, pc
  61b8b0: e59cf04c     	ldr	pc, [r12, #0x4c]
  61b8b4: e1a09000     	mov	r9, r0
  61b8b8: eaffffb6     	b	0x61b798 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const+0x4a4> @ imm = #-0x128
