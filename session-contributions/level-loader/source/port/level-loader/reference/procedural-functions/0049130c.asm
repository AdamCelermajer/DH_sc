
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0049130c <rnd::Path::LoadFromXml(TiXmlNode*)>:
  49130c: e92d4070     	push	{r4, r5, r6, lr}
  491310: e2514000     	subs	r4, r1, #0
  491314: e1a05000     	mov	r5, r0
  491318: e24dd008     	sub	sp, sp, #8
  49131c: 01a00004     	moveq	r0, r4
  491320: 0a000020     	beq	0x4913a8 <rnd::Path::LoadFromXml(TiXmlNode*)+0x9c> @ imm = #0x80
  491324: e5943000     	ldr	r3, [r4]
  491328: e1a00004     	mov	r0, r4
  49132c: e1a0e00f     	mov	lr, pc
  491330: e593f02c     	ldr	pc, [r3, #0x2c]
  491334: e59f1088     	ldr	r1, [pc, #0x88]         @ 0x4913c4 <rnd::Path::LoadFromXml(TiXmlNode*)+0xb8>
  491338: e28d2004     	add	r2, sp, #4
  49133c: e08f1001     	add	r1, pc, r1
  491340: eb02112a     	bl	0x5157f0 <TiXmlElement::QueryIntAttribute(char const*, int*) const> @ imm = #0x844a8
  491344: e3500000     	cmp	r0, #0
  491348: 0a000018     	beq	0x4913b0 <rnd::Path::LoadFromXml(TiXmlNode*)+0xa4> @ imm = #0x60
  49134c: e5943000     	ldr	r3, [r4]
  491350: e1a00004     	mov	r0, r4
  491354: e1a0e00f     	mov	lr, pc
  491358: e593f02c     	ldr	pc, [r3, #0x2c]
  49135c: e59f1064     	ldr	r1, [pc, #0x64]         @ 0x4913c8 <rnd::Path::LoadFromXml(TiXmlNode*)+0xbc>
  491360: e08f1001     	add	r1, pc, r1
  491364: eb020e41     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x83904
  491368: e2506000     	subs	r6, r0, #0
  49136c: 0a00000a     	beq	0x49139c <rnd::Path::LoadFromXml(TiXmlNode*)+0x90> @ imm = #0x28
  491370: ebf9f347     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x1832e4
  491374: e3a0102c     	mov	r1, #44
  491378: e5850084     	str	r0, [r5, #0x84]
  49137c: e5850080     	str	r0, [r5, #0x80]
  491380: e1a00006     	mov	r0, r6
  491384: ebf9f627     	bl	0x30ec28 <.plt+0xeb4>   @ imm = #-0x182764
  491388: e3500000     	cmp	r0, #0
  49138c: 0a000002     	beq	0x49139c <rnd::Path::LoadFromXml(TiXmlNode*)+0x90> @ imm = #0x8
  491390: e2800001     	add	r0, r0, #1
  491394: ebf9f33e     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x183308
  491398: e5850084     	str	r0, [r5, #0x84]
  49139c: e1a00005     	mov	r0, r5
  4913a0: e1a01004     	mov	r1, r4
  4913a4: ebfffe0c     	bl	0x490bdc <rnd::Rule::LoadFromXml(TiXmlNode*)> @ imm = #-0x7d0
  4913a8: e28dd008     	add	sp, sp, #8
  4913ac: e8bd8070     	pop	{r4, r5, r6, pc}
  4913b0: e59d3004     	ldr	r3, [sp, #0x4]
  4913b4: e2533000     	subs	r3, r3, #0
  4913b8: 13a03001     	movne	r3, #1
  4913bc: e5c5308c     	strb	r3, [r5, #0x8c]
  4913c0: eaffffe1     	b	0x49134c <rnd::Path::LoadFromXml(TiXmlNode*)+0x40> @ imm = #-0x7c
  4913c4: 94 3c 44 00  	.word	0x00443c94
  4913c8: 78 8b 45 00  	.word	0x00458b78
