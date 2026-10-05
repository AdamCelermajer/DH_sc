
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048c51c <rnd::ListElem::LoadFromXml(TiXmlNode*)>:
  48c51c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  48c520: e1a04000     	mov	r4, r0
  48c524: e5913000     	ldr	r3, [r1]
  48c528: e1a00001     	mov	r0, r1
  48c52c: e1a0e00f     	mov	lr, pc
  48c530: e593f02c     	ldr	pc, [r3, #0x2c]
  48c534: e2505000     	subs	r5, r0, #0
  48c538: 0a00002a     	beq	0x48c5e8 <rnd::ListElem::LoadFromXml(TiXmlNode*)+0xcc> @ imm = #0xa8
  48c53c: e59f10d8     	ldr	r1, [pc, #0xd8]         @ 0x48c61c <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x100>
  48c540: e2846004     	add	r6, r4, #4
  48c544: e08f1001     	add	r1, pc, r1
  48c548: eb0221c8     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x88720
  48c54c: e2507000     	subs	r7, r0, #0
  48c550: 0a000025     	beq	0x48c5ec <rnd::ListElem::LoadFromXml(TiXmlNode*)+0xd0> @ imm = #0x94
  48c554: ebfa063e     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17e708
  48c558: e0872000     	add	r2, r7, r0
  48c55c: e1a01007     	mov	r1, r7
  48c560: e1a00006     	mov	r0, r6
  48c564: ebfa111d     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17bb8c
  48c568: e59f10b0     	ldr	r1, [pc, #0xb0]         @ 0x48c620 <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x104>
  48c56c: e1a00005     	mov	r0, r5
  48c570: e284601c     	add	r6, r4, #28
  48c574: e08f1001     	add	r1, pc, r1
  48c578: eb0221bc     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x886f0
  48c57c: e2507000     	subs	r7, r0, #0
  48c580: 0a00001d     	beq	0x48c5fc <rnd::ListElem::LoadFromXml(TiXmlNode*)+0xe0> @ imm = #0x74
  48c584: ebfa0632     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17e738
  48c588: e0872000     	add	r2, r7, r0
  48c58c: e1a01007     	mov	r1, r7
  48c590: e1a00006     	mov	r0, r6
  48c594: ebfa1111     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17bbbc
  48c598: e59f1084     	ldr	r1, [pc, #0x84]         @ 0x48c624 <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x108>
  48c59c: e1a00005     	mov	r0, r5
  48c5a0: e2846034     	add	r6, r4, #52
  48c5a4: e08f1001     	add	r1, pc, r1
  48c5a8: eb0221b0     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x886c0
  48c5ac: e2507000     	subs	r7, r0, #0
  48c5b0: 0a000015     	beq	0x48c60c <rnd::ListElem::LoadFromXml(TiXmlNode*)+0xf0> @ imm = #0x54
  48c5b4: ebfa0626     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17e768
  48c5b8: e0872000     	add	r2, r7, r0
  48c5bc: e1a01007     	mov	r1, r7
  48c5c0: e1a00006     	mov	r0, r6
  48c5c4: ebfa1105     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17bbec
  48c5c8: e59f1058     	ldr	r1, [pc, #0x58]         @ 0x48c628 <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x10c>
  48c5cc: e1a00005     	mov	r0, r5
  48c5d0: e284204c     	add	r2, r4, #76
  48c5d4: e08f1001     	add	r1, pc, r1
  48c5d8: eb022484     	bl	0x5157f0 <TiXmlElement::QueryIntAttribute(char const*, int*) const> @ imm = #0x89210
  48c5dc: e3500000     	cmp	r0, #0
  48c5e0: 13a03064     	movne	r3, #100
  48c5e4: 1584304c     	strne	r3, [r4, #0x4c]
  48c5e8: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  48c5ec: e59f2038     	ldr	r2, [pc, #0x38]         @ 0x48c62c <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x110>
  48c5f0: e08f2002     	add	r2, pc, r2
  48c5f4: e1a07002     	mov	r7, r2
  48c5f8: eaffffd7     	b	0x48c55c <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x40> @ imm = #-0xa4
  48c5fc: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x48c630 <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x114>
  48c600: e08f2002     	add	r2, pc, r2
  48c604: e1a07002     	mov	r7, r2
  48c608: eaffffdf     	b	0x48c58c <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x70> @ imm = #-0x84
  48c60c: e59f2020     	ldr	r2, [pc, #0x20]         @ 0x48c634 <rnd::ListElem::LoadFromXml(TiXmlNode*)+0x118>
  48c610: e08f2002     	add	r2, pc, r2
  48c614: e1a07002     	mov	r7, r2
  48c618: eaffffe7     	b	0x48c5bc <rnd::ListElem::LoadFromXml(TiXmlNode*)+0xa0> @ imm = #-0x64
  48c61c: a4 4b 45 00  	.word	0x00454ba4
  48c620: 7c 88 44 00  	.word	0x0044887c
  48c624: 5c 88 44 00  	.word	0x0044885c
  48c628: 34 88 44 00  	.word	0x00448834
  48c62c: 18 f2 43 00  	.word	0x0043f218
  48c630: 08 f2 43 00  	.word	0x0043f208
  48c634: f8 f1 43 00  	.word	0x0043f1f8
