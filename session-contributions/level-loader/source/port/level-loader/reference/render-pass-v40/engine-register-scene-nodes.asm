
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0058b88c <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)>:
  58b88c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  58b890: e2515000     	subs	r5, r1, #0
  58b894: e1a08000     	mov	r8, r0
  58b898: 0a000035     	beq	0x58b974 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xe8> @ imm = #0xd4
  58b89c: e1a00005     	mov	r0, r5
  58b8a0: eb002e7a     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0xb9e8
  58b8a4: e5956004     	ldr	r6, [r5, #0x4]
  58b8a8: e1a07000     	mov	r7, r0
  58b8ac: e2855004     	add	r5, r5, #4
  58b8b0: e1a04000     	mov	r4, r0
  58b8b4: e3550000     	cmp	r5, #0
  58b8b8: 01a03005     	moveq	r3, r5
  58b8bc: 12453004     	subne	r3, r5, #4
  58b8c0: e593311c     	ldr	r3, [r3, #0x11c]
  58b8c4: e3130001     	tst	r3, #1
  58b8c8: 0a000019     	beq	0x58b934 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xa8> @ imm = #0x64
  58b8cc: e3550000     	cmp	r5, #0
  58b8d0: 01a01005     	moveq	r1, r5
  58b8d4: 12451004     	subne	r1, r5, #4
  58b8d8: e1a00008     	mov	r0, r8
  58b8dc: ebfffc91     	bl	0x58ab28 <glitch::scene::CSceneManager::isCulled(glitch::scene::ISceneNode const*) const> @ imm = #-0xdbc
  58b8e0: e3500000     	cmp	r0, #0
  58b8e4: 1a000012     	bne	0x58b934 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xa8> @ imm = #0x48
  58b8e8: e3550000     	cmp	r5, #0
  58b8ec: 01a03005     	moveq	r3, r5
  58b8f0: 12453004     	subne	r3, r5, #4
  58b8f4: e1a00003     	mov	r0, r3
  58b8f8: e5933000     	ldr	r3, [r3]
  58b8fc: e1a0e00f     	mov	lr, pc
  58b900: e593f010     	ldr	pc, [r3, #0x10]
  58b904: e3500000     	cmp	r0, #0
  58b908: 0a000009     	beq	0x58b934 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xa8> @ imm = #0x24
  58b90c: e3550000     	cmp	r5, #0
  58b910: 01a04005     	moveq	r4, r5
  58b914: 12454004     	subne	r4, r5, #4
  58b918: e59450f4     	ldr	r5, [r4, #0xf4]
  58b91c: e28460f4     	add	r6, r4, #244
  58b920: e1560005     	cmp	r6, r5
  58b924: 0a000005     	beq	0x58b940 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xb4> @ imm = #0x14
  58b928: e1540007     	cmp	r4, r7
  58b92c: 1affffe0     	bne	0x58b8b4 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x28> @ imm = #-0x80
  58b930: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  58b934: e5955000     	ldr	r5, [r5]
  58b938: e1560005     	cmp	r6, r5
  58b93c: 1afffff9     	bne	0x58b928 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x9c> @ imm = #-0x1c
  58b940: e1570004     	cmp	r7, r4
  58b944: 0a000026     	beq	0x58b9e4 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x158> @ imm = #0x98
  58b948: e1a00004     	mov	r0, r4
  58b94c: eb002e4f     	bl	0x597290 <glitch::scene::ISceneNode::getParent() const> @ imm = #0xb93c
  58b950: e5945004     	ldr	r5, [r4, #0x4]
  58b954: e28060f4     	add	r6, r0, #244
  58b958: e1560005     	cmp	r6, r5
  58b95c: 11a04000     	movne	r4, r0
  58b960: 1afffff0     	bne	0x58b928 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x9c> @ imm = #-0x40
  58b964: e1570000     	cmp	r7, r0
  58b968: e1a04000     	mov	r4, r0
  58b96c: 1afffff5     	bne	0x58b948 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xbc> @ imm = #-0x2c
  58b970: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  58b974: e5d03288     	ldrb	r3, [r0, #0x288]
  58b978: e3530000     	cmp	r3, #0
  58b97c: 1a000019     	bne	0x58b9e8 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x15c> @ imm = #0x64
  58b980: e5984270     	ldr	r4, [r8, #0x270]
  58b984: e5985274     	ldr	r5, [r8, #0x274]
  58b988: e1540005     	cmp	r4, r5
  58b98c: 1a000003     	bne	0x58b9a0 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x114> @ imm = #0xc
  58b990: eafffff6     	b	0x58b970 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xe4> @ imm = #-0x28
  58b994: e2844004     	add	r4, r4, #4
  58b998: e1540005     	cmp	r4, r5
  58b99c: 0a00000f     	beq	0x58b9e0 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x154> @ imm = #0x3c
  58b9a0: e5941000     	ldr	r1, [r4]
  58b9a4: e591311c     	ldr	r3, [r1, #0x11c]
  58b9a8: e3130001     	tst	r3, #1
  58b9ac: 0afffff8     	beq	0x58b994 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x108> @ imm = #-0x20
  58b9b0: e1a00008     	mov	r0, r8
  58b9b4: ebfffc5b     	bl	0x58ab28 <glitch::scene::CSceneManager::isCulled(glitch::scene::ISceneNode const*) const> @ imm = #-0xe94
  58b9b8: e3500000     	cmp	r0, #0
  58b9bc: 1afffff4     	bne	0x58b994 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x108> @ imm = #-0x30
  58b9c0: e5943000     	ldr	r3, [r4]
  58b9c4: e2844004     	add	r4, r4, #4
  58b9c8: e1a00003     	mov	r0, r3
  58b9cc: e5933000     	ldr	r3, [r3]
  58b9d0: e1a0e00f     	mov	lr, pc
  58b9d4: e593f010     	ldr	pc, [r3, #0x10]
  58b9d8: e1540005     	cmp	r4, r5
  58b9dc: 1affffef     	bne	0x58b9a0 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0x114> @ imm = #-0x44
  58b9e0: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  58b9e4: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  58b9e8: ebffff6f     	bl	0x58b7ac <glitch::scene::CSceneManager::collectAllNodes()> @ imm = #-0x244
  58b9ec: eaffffe3     	b	0x58b980 <glitch::scene::CSceneManager::registerSceneNodes(glitch::scene::ISceneNode*)+0xf4> @ imm = #-0x74
