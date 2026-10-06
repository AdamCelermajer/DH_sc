
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006463c8 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()>:
  6463c8: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  6463cc: e5903134     	ldr	r3, [r0, #0x134]
  6463d0: e24dd024     	sub	sp, sp, #36
  6463d4: e1a05000     	mov	r5, r0
  6463d8: e3530000     	cmp	r3, #0
  6463dc: 0a000060     	beq	0x646564 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x19c> @ imm = #0x180
  6463e0: e5902110     	ldr	r2, [r0, #0x110]
  6463e4: e5929014     	ldr	r9, [r2, #0x14]
  6463e8: e3590000     	cmp	r9, #0
  6463ec: 0a00005c     	beq	0x646564 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x19c> @ imm = #0x170
  6463f0: e1a00003     	mov	r0, r3
  6463f4: e5933000     	ldr	r3, [r3]
  6463f8: e1a0e00f     	mov	lr, pc
  6463fc: e593f010     	ldr	pc, [r3, #0x10]
  646400: e2507000     	subs	r7, r0, #0
  646404: 0a000056     	beq	0x646564 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x19c> @ imm = #0x158
  646408: e3a04001     	mov	r4, #1
  64640c: e28da01c     	add	r10, sp, #28
  646410: e28d6018     	add	r6, sp, #24
  646414: e1a08007     	mov	r8, r7
  646418: ea000006     	b	0x646438 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x70> @ imm = #0x18
  64641c: e3500005     	cmp	r0, #5
  646420: 0a000052     	beq	0x646570 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x1a8> @ imm = #0x148
  646424: e1a00006     	mov	r0, r6
  646428: ebf329ee     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x335848
  64642c: e1580004     	cmp	r8, r4
  646430: e2844001     	add	r4, r4, #1
  646434: 9a00004a     	bls	0x646564 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x19c> @ imm = #0x128
  646438: e5953134     	ldr	r3, [r5, #0x134]
  64643c: e2447001     	sub	r7, r4, #1
  646440: e1a0000a     	mov	r0, r10
  646444: e1a01003     	mov	r1, r3
  646448: e1a02007     	mov	r2, r7
  64644c: e5933000     	ldr	r3, [r3]
  646450: e1a0e00f     	mov	lr, pc
  646454: e593f014     	ldr	pc, [r3, #0x14]
  646458: e59d301c     	ldr	r3, [sp, #0x1c]
  64645c: e2530000     	subs	r0, r3, #0
  646460: 0afffff1     	beq	0x64642c <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x64> @ imm = #-0x3c
  646464: ebf35c46     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x328ee8
  646468: e5953134     	ldr	r3, [r5, #0x134]
  64646c: e1a00006     	mov	r0, r6
  646470: e1a02007     	mov	r2, r7
  646474: e1a01003     	mov	r1, r3
  646478: e5933000     	ldr	r3, [r3]
  64647c: e1a0e00f     	mov	lr, pc
  646480: e593f018     	ldr	pc, [r3, #0x18]
  646484: e595c134     	ldr	r12, [r5, #0x134]
  646488: e1a03007     	mov	r3, r7
  64648c: e1a02009     	mov	r2, r9
  646490: e1a0000c     	mov	r0, r12
  646494: e3a01000     	mov	r1, #0
  646498: e59cc000     	ldr	r12, [r12]
  64649c: e1a0e00f     	mov	lr, pc
  6464a0: e59cf038     	ldr	pc, [r12, #0x38]
  6464a4: e3500004     	cmp	r0, #4
  6464a8: 13500010     	cmpne	r0, #16
  6464ac: 1affffda     	bne	0x64641c <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x54> @ imm = #-0x98
  6464b0: e5957110     	ldr	r7, [r5, #0x110]
  6464b4: e59d3018     	ldr	r3, [sp, #0x18]
  6464b8: e5972000     	ldr	r2, [r7]
  6464bc: e1a00003     	mov	r0, r3
  6464c0: e592b024     	ldr	r11, [r2, #0x24]
  6464c4: e58d3014     	str	r3, [sp, #0x14]
  6464c8: ebfdfe19     	bl	0x5c5d34 <glitch::video::CMaterial::getTechnique() const> @ imm = #-0x8079c
  6464cc: e59d3014     	ldr	r3, [sp, #0x14]
  6464d0: e3a0e00c     	mov	lr, #12
  6464d4: e1a01005     	mov	r1, r5
  6464d8: e5932004     	ldr	r2, [r3, #0x4]
  6464dc: e1a03004     	mov	r3, r4
  6464e0: e592c018     	ldr	r12, [r2, #0x18]
  6464e4: e1a02006     	mov	r2, r6
  6464e8: e02cc09e     	mla	r12, lr, r0, r12
  6464ec: e3a0e000     	mov	lr, #0
  6464f0: e59cc008     	ldr	r12, [r12, #0x8]
  6464f4: e1a00007     	mov	r0, r7
  6464f8: e59cc004     	ldr	r12, [r12, #0x4]
  6464fc: e58de004     	str	lr, [sp, #0x4]
  646500: e3e0e102     	mvn	lr, #-2147483648
  646504: e31c0801     	tst	r12, #65536
  646508: 13a0c008     	movne	r12, #8
  64650c: 03a0c004     	moveq	r12, #4
  646510: e58de008     	str	lr, [sp, #0x8]
  646514: e58dc000     	str	r12, [sp]
  646518: e12fff3b     	blx	r11
  64651c: e595311c     	ldr	r3, [r5, #0x11c]
  646520: e3130b02     	tst	r3, #2048
  646524: 0affffbe     	beq	0x646424 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x5c> @ imm = #-0x108
  646528: e5953110     	ldr	r3, [r5, #0x110]
  64652c: e3a0e000     	mov	lr, #0
  646530: e1a01005     	mov	r1, r5
  646534: e593c000     	ldr	r12, [r3]
  646538: e1a00003     	mov	r0, r3
  64653c: e3a03007     	mov	r3, #7
  646540: e58d3000     	str	r3, [sp]
  646544: e3e03102     	mvn	r3, #-2147483648
  646548: e58d3008     	str	r3, [sp, #0x8]
  64654c: e1a02006     	mov	r2, r6
  646550: e58de004     	str	lr, [sp, #0x4]
  646554: e1a03004     	mov	r3, r4
  646558: e1a0e00f     	mov	lr, pc
  64655c: e59cf024     	ldr	pc, [r12, #0x24]
  646560: eaffffaf     	b	0x646424 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x5c> @ imm = #-0x144
  646564: e3a00001     	mov	r0, #1
  646568: e28dd024     	add	sp, sp, #36
  64656c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  646570: e5953134     	ldr	r3, [r5, #0x134]
  646574: e1a00003     	mov	r0, r3
  646578: e5933000     	ldr	r3, [r3]
  64657c: e1a0e00f     	mov	lr, pc
  646580: e593f024     	ldr	pc, [r3, #0x24]
  646584: eaffffa6     	b	0x646424 <glitch::collada::CMeshSceneNode::onRegisterSceneNode()+0x5c> @ imm = #-0x168
