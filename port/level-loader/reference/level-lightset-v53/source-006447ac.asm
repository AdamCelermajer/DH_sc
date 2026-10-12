
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006447ac <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)>:
  6447ac: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  6447b0: e1a05002     	mov	r5, r2
  6447b4: e1a04001     	mov	r4, r1
  6447b8: e3a02001     	mov	r2, #1
  6447bc: e2811004     	add	r1, r1, #4
  6447c0: e1a06000     	mov	r6, r0
  6447c4: e1a07003     	mov	r7, r3
  6447c8: ebfcfe9a     	bl	0x584238 <glitch::scene::CLightSceneNode::CLightSceneNode(bool)> @ imm = #-0xc0598
  6447cc: e5952000     	ldr	r2, [r5]
  6447d0: e59f325c     	ldr	r3, [pc, #0x25c]        @ 0x644a34 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x288>
  6447d4: e5862158     	str	r2, [r6, #0x158]
  6447d8: e5951004     	ldr	r1, [r5, #0x4]
  6447dc: e3520000     	cmp	r2, #0
  6447e0: e08f3003     	add	r3, pc, r3
  6447e4: e586115c     	str	r1, [r6, #0x15c]
  6447e8: 0a000003     	beq	0x6447fc <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x50> @ imm = #0xc
  6447ec: e5921004     	ldr	r1, [r2, #0x4]
  6447f0: e3510000     	cmp	r1, #0
  6447f4: 12811001     	addne	r1, r1, #1
  6447f8: 15821004     	strne	r1, [r2, #0x4]
  6447fc: e59f2234     	ldr	r2, [pc, #0x234]        @ 0x644a38 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x28c>
  644800: e3a01443     	mov	r1, #1124073472
  644804: e281187f     	add	r1, r1, #8323072
  644808: e7932002     	ldr	r2, [r3, r2]
  64480c: e2822004     	add	r2, r2, #4
  644810: e5862154     	str	r2, [r6, #0x154]
  644814: e5943000     	ldr	r3, [r4]
  644818: e5863000     	str	r3, [r6]
  64481c: e594201c     	ldr	r2, [r4, #0x1c]
  644820: e513301c     	ldr	r3, [r3, #-0x1c]
  644824: e7862003     	str	r2, [r6, r3]
  644828: e5963000     	ldr	r3, [r6]
  64482c: e5942020     	ldr	r2, [r4, #0x20]
  644830: e513300c     	ldr	r3, [r3, #-0xc]
  644834: e7862003     	str	r2, [r6, r3]
  644838: e5867160     	str	r7, [r6, #0x160]
  64483c: e5970010     	ldr	r0, [r7, #0x10]
  644840: ebf32913     	bl	0x30ec94 <.plt+0xf20>   @ imm = #-0x335bb4
  644844: e1a04000     	mov	r4, r0
  644848: e5d7000c     	ldrb	r0, [r7, #0xc]
  64484c: ebf32844     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335ef0
  644850: e1a01004     	mov	r1, r4
  644854: ebf32944     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335af0
  644858: e1a0a000     	mov	r10, r0
  64485c: e5d7000d     	ldrb	r0, [r7, #0xd]
  644860: ebf3283f     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335f04
  644864: e1a01004     	mov	r1, r4
  644868: ebf3293f     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335b04
  64486c: e1a05000     	mov	r5, r0
  644870: e5d7000e     	ldrb	r0, [r7, #0xe]
  644874: ebf3283a     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335f18
  644878: e1a01004     	mov	r1, r4
  64487c: ebf3293a     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335b18
  644880: e1a08000     	mov	r8, r0
  644884: e5d7000f     	ldrb	r0, [r7, #0xf]
  644888: ebf32835     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0x335f2c
  64488c: e1a01004     	mov	r1, r4
  644890: ebf32935     	bl	0x30ed6c <.plt+0xff8>   @ imm = #-0x335b2c
  644894: e5963134     	ldr	r3, [r6, #0x134]
  644898: e583a024     	str	r10, [r3, #0x24]
  64489c: e5830030     	str	r0, [r3, #0x30]
  6448a0: e583802c     	str	r8, [r3, #0x2c]
  6448a4: e5835028     	str	r5, [r3, #0x28]
  6448a8: e5973008     	ldr	r3, [r7, #0x8]
  6448ac: e3530003     	cmp	r3, #3
  6448b0: 908ff103     	addls	pc, pc, r3, lsl #2
  6448b4: ea000016     	b	0x644914 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x168> @ imm = #0x58
  6448b8: ea000002     	b	0x6448c8 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x11c> @ imm = #0x8
  6448bc: ea000044     	b	0x6449d4 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x228> @ imm = #0x110
  6448c0: ea000023     	b	0x644954 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x1a8> @ imm = #0x8c
  6448c4: ea000016     	b	0x644924 <glitch::collada::CLightSceneNode::CLightSceneNode(glitch::collada::CColladaDatabase const&, glitch::collada::SLight&)+0x178> @ imm = #0x58
  6448c8: e5962134     	ldr	r2, [r6, #0x134]
  6448cc: e3a01003     	mov	r1, #3
  6448d0: e3a03000     	mov	r3, #0
  6448d4: e1c215b8     	strh	r1, [r2, #88]
  6448d8: e5962134     	ldr	r2, [r6, #0x134]
  6448dc: e582a004     	str	r10, [r2, #0x4]
  6448e0: e5820010     	str	r0, [r2, #0x10]
  6448e4: e582800c     	str	r8, [r2, #0xc]
  6448e8: e5825008     	str	r5, [r2, #0x8]
  6448ec: e5962134     	ldr	r2, [r6, #0x134]
  6448f0: e5823014     	str	r3, [r2, #0x14]
  6448f4: e5823020     	str	r3, [r2, #0x20]
  6448f8: e582301c     	str	r3, [r2, #0x1c]
  6448fc: e5823018     	str	r3, [r2, #0x18]
  644900: e5962134     	ldr	r2, [r6, #0x134]
  644904: e5823024     	str	r3, [r2, #0x24]
  644908: e5823030     	str	r3, [r2, #0x30]
  64490c: e582302c     	str	r3, [r2, #0x2c]
  644910: e5823028     	str	r3, [r2, #0x28]
  644914: e1a00006     	mov	r0, r6
  644918: ebfcfcbe     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0d08
  64491c: e1a00006     	mov	r0, r6
  644920: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  644924: e5963134     	ldr	r3, [r6, #0x134]
  644928: e3a01002     	mov	r1, #2
  64492c: e1c315b8     	strh	r1, [r3, #88]
  644930: e5963134     	ldr	r3, [r6, #0x134]
  644934: e5830020     	str	r0, [r3, #0x20]
  644938: e583a014     	str	r10, [r3, #0x14]
  64493c: e1a00006     	mov	r0, r6
  644940: e583801c     	str	r8, [r3, #0x1c]
  644944: e5835018     	str	r5, [r3, #0x18]
  644948: ebfcfcb2     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0d38
  64494c: e1a00006     	mov	r0, r6
  644950: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  644954: e5963134     	ldr	r3, [r6, #0x134]
  644958: e3a02001     	mov	r2, #1
  64495c: e1c325b8     	strh	r2, [r3, #88]
  644960: e5963134     	ldr	r3, [r6, #0x134]
  644964: e583a014     	str	r10, [r3, #0x14]
  644968: e5830020     	str	r0, [r3, #0x20]
  64496c: e583801c     	str	r8, [r3, #0x1c]
  644970: e5835018     	str	r5, [r3, #0x18]
  644974: e5972014     	ldr	r2, [r7, #0x14]
  644978: e5963134     	ldr	r3, [r6, #0x134]
  64497c: e1a00006     	mov	r0, r6
  644980: e5922000     	ldr	r2, [r2]
  644984: e5832034     	str	r2, [r3, #0x34]
  644988: e5972014     	ldr	r2, [r7, #0x14]
  64498c: e5963134     	ldr	r3, [r6, #0x134]
  644990: e5922004     	ldr	r2, [r2, #0x4]
  644994: e5832038     	str	r2, [r3, #0x38]
  644998: e5972014     	ldr	r2, [r7, #0x14]
  64499c: e5963134     	ldr	r3, [r6, #0x134]
  6449a0: e5922008     	ldr	r2, [r2, #0x8]
  6449a4: e583203c     	str	r2, [r3, #0x3c]
  6449a8: e5972014     	ldr	r2, [r7, #0x14]
  6449ac: e5963134     	ldr	r3, [r6, #0x134]
  6449b0: e592200c     	ldr	r2, [r2, #0xc]
  6449b4: e5832048     	str	r2, [r3, #0x48]
  6449b8: e5972014     	ldr	r2, [r7, #0x14]
  6449bc: e5963134     	ldr	r3, [r6, #0x134]
  6449c0: e5922010     	ldr	r2, [r2, #0x10]
  6449c4: e583204c     	str	r2, [r3, #0x4c]
  6449c8: ebfcfc92     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0db8
  6449cc: e1a00006     	mov	r0, r6
  6449d0: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  6449d4: e5963134     	ldr	r3, [r6, #0x134]
  6449d8: e3a01000     	mov	r1, #0
  6449dc: e1c315b8     	strh	r1, [r3, #88]
  6449e0: e5963134     	ldr	r3, [r6, #0x134]
  6449e4: e583a014     	str	r10, [r3, #0x14]
  6449e8: e5830020     	str	r0, [r3, #0x20]
  6449ec: e583801c     	str	r8, [r3, #0x1c]
  6449f0: e5835018     	str	r5, [r3, #0x18]
  6449f4: e5972014     	ldr	r2, [r7, #0x14]
  6449f8: e5963134     	ldr	r3, [r6, #0x134]
  6449fc: e1a00006     	mov	r0, r6
  644a00: e5922000     	ldr	r2, [r2]
  644a04: e5832034     	str	r2, [r3, #0x34]
  644a08: e5972014     	ldr	r2, [r7, #0x14]
  644a0c: e5963134     	ldr	r3, [r6, #0x134]
  644a10: e5922004     	ldr	r2, [r2, #0x4]
  644a14: e5832038     	str	r2, [r3, #0x38]
  644a18: e5972014     	ldr	r2, [r7, #0x14]
  644a1c: e5963134     	ldr	r3, [r6, #0x134]
  644a20: e5922008     	ldr	r2, [r2, #0x8]
  644a24: e583203c     	str	r2, [r3, #0x3c]
  644a28: ebfcfc7a     	bl	0x583c18 <glitch::scene::CLightSceneNode::doLightRecalc()> @ imm = #-0xc0e18
  644a2c: e1a00006     	mov	r0, r6
  644a30: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  644a34: b0 02 35 00  	.word	0x003502b0
  644a38: b4 17 00 00  	.word	0x000017b4
