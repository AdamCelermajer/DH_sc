
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c48c <rnd::RPElem::LoadFromXml(TiXmlNode*)>:
  48c48c: e92d4070     	push	{r4, r5, r6, lr}
  48c490: e1a04000     	mov	r4, r0
  48c494: e5913000     	ldr	r3, [r1]
  48c498: e1a00001     	mov	r0, r1
  48c49c: e1a0e00f     	mov	lr, pc
  48c4a0: e593f02c     	ldr	pc, [r3, #0x2c]
  48c4a4: e2505000     	subs	r5, r0, #0
  48c4a8: 0a000017     	beq	0x48c50c <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x80> @ imm = #0x5c
  48c4ac: e5943000     	ldr	r3, [r4]
  48c4b0: e3530000     	cmp	r3, #0
  48c4b4: 0a000014     	beq	0x48c50c <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x80> @ imm = #0x50
  48c4b8: e59f1050     	ldr	r1, [pc, #0x50]         @ 0x48c510 <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x84>
  48c4bc: e08f1001     	add	r1, pc, r1
  48c4c0: eb0221ea     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x887a8
  48c4c4: e2501000     	subs	r1, r0, #0
  48c4c8: 0a000003     	beq	0x48c4dc <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x50> @ imm = #0xc
  48c4cc: e5943000     	ldr	r3, [r4]
  48c4d0: e5930010     	ldr	r0, [r3, #0x10]
  48c4d4: ebffdd8e     	bl	0x483b14 <rnd::RandomGenerator::Hash(unsigned char*)> @ imm = #-0x89c8
  48c4d8: e5840004     	str	r0, [r4, #0x4]
  48c4dc: e59f1030     	ldr	r1, [pc, #0x30]         @ 0x48c514 <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x88>
  48c4e0: e1a00005     	mov	r0, r5
  48c4e4: e08f1001     	add	r1, pc, r1
  48c4e8: eb0221e0     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x88780
  48c4ec: e2503000     	subs	r3, r0, #0
  48c4f0: 0a000004     	beq	0x48c508 <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x7c> @ imm = #0x10
  48c4f4: e59f101c     	ldr	r1, [pc, #0x1c]         @ 0x48c518 <rnd::RPElem::LoadFromXml(TiXmlNode*)+0x8c>
  48c4f8: e08f1001     	add	r1, pc, r1
  48c4fc: ebfa0786     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x17e1e8
  48c500: e2703001     	rsbs	r3, r0, #1
  48c504: 33a03000     	movlo	r3, #0
  48c508: e5c43008     	strb	r3, [r4, #0x8]
  48c50c: e8bd8070     	pop	{r4, r5, r6, pc}
  48c510: d4 f1 47 00  	.word	0x0047f1d4
  48c514: fc 88 44 00  	.word	0x004488fc
  48c518: f8 23 43 00  	.word	0x004323f8
