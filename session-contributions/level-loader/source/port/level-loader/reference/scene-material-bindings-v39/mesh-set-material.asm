
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00644bdc <glitch::collada::CMesh::setMaterial(unsigned int, boost::intrusive_ptr<glitch::video::CMaterial> const&, boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap> const&)>:
  644bdc: e92d4030     	push	{r4, r5, lr}
  644be0: e590c01c     	ldr	r12, [r0, #0x1c]
  644be4: e5900018     	ldr	r0, [r0, #0x18]
  644be8: e1a05003     	mov	r5, r3
  644bec: e24dd00c     	sub	sp, sp, #12
  644bf0: e060c00c     	rsb	r12, r0, r12
  644bf4: e1a0c14c     	asr	r12, r12, #2
  644bf8: e08c310c     	add	r3, r12, r12, lsl #2
  644bfc: e0833203     	add	r3, r3, r3, lsl #4
  644c00: e0833403     	add	r3, r3, r3, lsl #8
  644c04: e0833803     	add	r3, r3, r3, lsl #16
  644c08: e08cc083     	add	r12, r12, r3, lsl #1
  644c0c: e151000c     	cmp	r1, r12
  644c10: 2a00001a     	bhs	0x644c80 <glitch::collada::CMesh::setMaterial(unsigned int, boost::intrusive_ptr<glitch::video::CMaterial> const&, boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap> const&)+0xa4> @ imm = #0x68
  644c14: e5923000     	ldr	r3, [r2]
  644c18: e3a0400c     	mov	r4, #12
  644c1c: e0240194     	mla	r4, r4, r1, r0
  644c20: e58d3004     	str	r3, [sp, #0x4]
  644c24: e3530000     	cmp	r3, #0
  644c28: 15932000     	ldrne	r2, [r3]
  644c2c: e28d0008     	add	r0, sp, #8
  644c30: 12822001     	addne	r2, r2, #1
  644c34: 15832000     	strne	r2, [r3]
  644c38: e5942004     	ldr	r2, [r4, #0x4]
  644c3c: 159d3004     	ldrne	r3, [sp, #0x4]
  644c40: e5202004     	str	r2, [r0, #-0x4]!
  644c44: e5843004     	str	r3, [r4, #0x4]
  644c48: ebf32fe6     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x334068
  644c4c: e5953000     	ldr	r3, [r5]
  644c50: e28d0008     	add	r0, sp, #8
  644c54: e58d3000     	str	r3, [sp]
  644c58: e3530000     	cmp	r3, #0
  644c5c: 15932000     	ldrne	r2, [r3]
  644c60: 12822001     	addne	r2, r2, #1
  644c64: 15832000     	strne	r2, [r3]
  644c68: 159d3000     	ldrne	r3, [sp]
  644c6c: e5942008     	ldr	r2, [r4, #0x8]
  644c70: e5202008     	str	r2, [r0, #-0x8]!
  644c74: e5843008     	str	r3, [r4, #0x8]
  644c78: e1a0000d     	mov	r0, sp
  644c7c: ebfcd57a     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xcaa18
  644c80: e28dd00c     	add	sp, sp, #12
  644c84: e8bd8030     	pop	{r4, r5, pc}
