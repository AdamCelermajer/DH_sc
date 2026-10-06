
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0066391c <glitch::collada::CSkinnedMesh::setMaterial(unsigned int, boost::intrusive_ptr<glitch::video::CMaterial> const&, boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap> const&)>:
  66391c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  663920: e1a07002     	mov	r7, r2
  663924: e5922000     	ldr	r2, [r2]
  663928: e24dd008     	sub	sp, sp, #8
  66392c: e3a05014     	mov	r5, #20
  663930: e3520000     	cmp	r2, #0
  663934: e0050195     	mul	r5, r5, r1
  663938: e1a08001     	mov	r8, r1
  66393c: e590105c     	ldr	r1, [r0, #0x5c]
  663940: e58d2004     	str	r2, [sp, #0x4]
  663944: e1a06003     	mov	r6, r3
  663948: 15923000     	ldrne	r3, [r2]
  66394c: e0811005     	add	r1, r1, r5
  663950: e1a04000     	mov	r4, r0
  663954: 12833001     	addne	r3, r3, #1
  663958: 15823000     	strne	r3, [r2]
  66395c: e5913004     	ldr	r3, [r1, #0x4]
  663960: 159d2004     	ldrne	r2, [sp, #0x4]
  663964: e28d0008     	add	r0, sp, #8
  663968: e5203004     	str	r3, [r0, #-0x4]!
  66396c: e5812004     	str	r2, [r1, #0x4]
  663970: ebf2b49c     	bl	0x310be8 <boost::intrusive_ptr<glitch::video::CMaterial>::~intrusive_ptr()> @ imm = #-0x352d90
  663974: e5963000     	ldr	r3, [r6]
  663978: e594205c     	ldr	r2, [r4, #0x5c]
  66397c: e28d0008     	add	r0, sp, #8
  663980: e58d3000     	str	r3, [sp]
  663984: e3530000     	cmp	r3, #0
  663988: 15931000     	ldrne	r1, [r3]
  66398c: e0822005     	add	r2, r2, r5
  663990: 12811001     	addne	r1, r1, #1
  663994: 15831000     	strne	r1, [r3]
  663998: 159d3000     	ldrne	r3, [sp]
  66399c: e5921008     	ldr	r1, [r2, #0x8]
  6639a0: e5201008     	str	r1, [r0, #-0x8]!
  6639a4: e5823008     	str	r3, [r2, #0x8]
  6639a8: e1a0000d     	mov	r0, sp
  6639ac: ebfc5a2e     	bl	0x57a26c <boost::intrusive_ptr<glitch::video::CMaterialVertexAttributeMap>::~intrusive_ptr()> @ imm = #-0xe9748
  6639b0: e594305c     	ldr	r3, [r4, #0x5c]
  6639b4: e3e00000     	mvn	r0, #0
  6639b8: e1a01008     	mov	r1, r8
  6639bc: e0833005     	add	r3, r3, r5
  6639c0: e5c30010     	strb	r0, [r3, #0x10]
  6639c4: e594c05c     	ldr	r12, [r4, #0x5c]
  6639c8: e1a02007     	mov	r2, r7
  6639cc: e1a03006     	mov	r3, r6
  6639d0: e08c5005     	add	r5, r12, r5
  6639d4: e5c50011     	strb	r0, [r5, #0x11]
  6639d8: e594c068     	ldr	r12, [r4, #0x68]
  6639dc: e1a0000c     	mov	r0, r12
  6639e0: e59cc000     	ldr	r12, [r12]
  6639e4: e1a0e00f     	mov	lr, pc
  6639e8: e59cf020     	ldr	pc, [r12, #0x20]
  6639ec: e28dd008     	add	sp, sp, #8
  6639f0: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
