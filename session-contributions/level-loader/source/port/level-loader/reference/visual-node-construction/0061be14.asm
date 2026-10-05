
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0061be14 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*) const>:
  61be14: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  61be18: e2526000     	subs	r6, r2, #0
  61be1c: e1a05000     	mov	r5, r0
  61be20: e1a07001     	mov	r7, r1
  61be24: 0a000019     	beq	0x61be90 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*) const+0x7c> @ imm = #0x64
  61be28: e5903004     	ldr	r3, [r0, #0x4]
  61be2c: e1a01000     	mov	r1, r0
  61be30: e1a00003     	mov	r0, r3
  61be34: e5933000     	ldr	r3, [r3]
  61be38: e1a0e00f     	mov	lr, pc
  61be3c: e593f054     	ldr	pc, [r3, #0x54]
  61be40: e1a04000     	mov	r4, r0
  61be44: e1a02006     	mov	r2, r6
  61be48: e1a01007     	mov	r1, r7
  61be4c: e1a03004     	mov	r3, r4
  61be50: e1a00005     	mov	r0, r5
  61be54: ebfffd26     	bl	0x61b2f4 <glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const> @ imm = #-0xb68
  61be58: e5943000     	ldr	r3, [r4]
  61be5c: e1a01000     	mov	r1, r0
  61be60: e1a05000     	mov	r5, r0
  61be64: e1a00004     	mov	r0, r4
  61be68: e1a0e00f     	mov	lr, pc
  61be6c: e593f05c     	ldr	pc, [r3, #0x5c]
  61be70: e1a00004     	mov	r0, r4
  61be74: eb00fd58     	bl	0x65b3dc <glitch::collada::CRootSceneNode::onPostLoad()> @ imm = #0x3f560
  61be78: e5953000     	ldr	r3, [r5]
  61be7c: e513000c     	ldr	r0, [r3, #-0xc]
  61be80: e0850000     	add	r0, r5, r0
  61be84: ebf405be     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2fe908
  61be88: e1a00004     	mov	r0, r4
  61be8c: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  61be90: e1a00006     	mov	r0, r6
  61be94: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
