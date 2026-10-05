
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a90c <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)>:
  48a90c: e92d47f0     	push	{r4, r5, r6, r7, r8, r9, r10, lr}
  48a910: e1a04000     	mov	r4, r0
  48a914: e24dd010     	sub	sp, sp, #16
  48a918: e1a00001     	mov	r0, r1
  48a91c: e1a07001     	mov	r7, r1
  48a920: ebffe448     	bl	0x483a48 <TiXmlHandle::ToElement() const> @ imm = #-0x6ee0
  48a924: e2505000     	subs	r5, r0, #0
  48a928: 01a00005     	moveq	r0, r5
  48a92c: 0a000047     	beq	0x48aa50 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x144> @ imm = #0x11c
  48a930: e59f1130     	ldr	r1, [pc, #0x130]        @ 0x48aa68 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x15c>
  48a934: e1a0200d     	mov	r2, sp
  48a938: e08f1001     	add	r1, pc, r1
  48a93c: eb022b8a     	bl	0x51576c <TiXmlElement::QueryDoubleAttribute(char const*, double*) const> @ imm = #0x8ae28
  48a940: e3500000     	cmp	r0, #0
  48a944: 1a000002     	bne	0x48a954 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x48> @ imm = #0x8
  48a948: e1cd00d0     	ldrd	r0, r1, [sp]
  48a94c: ebfa0f53     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0x17c2b4
  48a950: e584004c     	str	r0, [r4, #0x4c]
  48a954: e59f1110     	ldr	r1, [pc, #0x110]        @ 0x48aa6c <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x160>
  48a958: e1a0200d     	mov	r2, sp
  48a95c: e1a00005     	mov	r0, r5
  48a960: e08f1001     	add	r1, pc, r1
  48a964: eb022b80     	bl	0x51576c <TiXmlElement::QueryDoubleAttribute(char const*, double*) const> @ imm = #0x8ae00
  48a968: e3500000     	cmp	r0, #0
  48a96c: 0a000039     	beq	0x48aa58 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x14c> @ imm = #0xe4
  48a970: e59f10f8     	ldr	r1, [pc, #0xf8]         @ 0x48aa70 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x164>
  48a974: e2842054     	add	r2, r4, #84
  48a978: e1a00005     	mov	r0, r5
  48a97c: e08f1001     	add	r1, pc, r1
  48a980: eb022b9a     	bl	0x5157f0 <TiXmlElement::QueryIntAttribute(char const*, int*) const> @ imm = #0x8ae68
  48a984: e59f10e8     	ldr	r1, [pc, #0xe8]         @ 0x48aa74 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x168>
  48a988: e2842058     	add	r2, r4, #88
  48a98c: e1a00005     	mov	r0, r5
  48a990: e08f1001     	add	r1, pc, r1
  48a994: eb022b95     	bl	0x5157f0 <TiXmlElement::QueryIntAttribute(char const*, int*) const> @ imm = #0x8ae54
  48a998: e59f20d8     	ldr	r2, [pc, #0xd8]         @ 0x48aa78 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x16c>
  48a99c: e28d600c     	add	r6, sp, #12
  48a9a0: e1a01007     	mov	r1, r7
  48a9a4: e08f2002     	add	r2, pc, r2
  48a9a8: e1a00006     	mov	r0, r6
  48a9ac: eb022926     	bl	0x514e4c <TiXmlHandle::FirstChild(char const*) const> @ imm = #0x8a498
  48a9b0: e59d300c     	ldr	r3, [sp, #0xc]
  48a9b4: e3530000     	cmp	r3, #0
  48a9b8: 0a000023     	beq	0x48aa4c <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x140> @ imm = #0x8c
  48a9bc: e59f70b8     	ldr	r7, [pc, #0xb8]         @ 0x48aa7c <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x170>
  48a9c0: e59f80b8     	ldr	r8, [pc, #0xb8]         @ 0x48aa80 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x174>
  48a9c4: e3a05000     	mov	r5, #0
  48a9c8: e08f7007     	add	r7, pc, r7
  48a9cc: e08f8008     	add	r8, pc, r8
  48a9d0: e3a09f4b     	mov	r9, #300
  48a9d4: e28da008     	add	r10, sp, #8
  48a9d8: ea000000     	b	0x48a9e0 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0xd4> @ imm = #0x0
  48a9dc: e58d300c     	str	r3, [sp, #0xc]
  48a9e0: e1a00006     	mov	r0, r6
  48a9e4: ebffe417     	bl	0x483a48 <TiXmlHandle::ToElement() const> @ imm = #-0x6fa4
  48a9e8: e1a01007     	mov	r1, r7
  48a9ec: eb02289f     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8a27c
  48a9f0: e1a01008     	mov	r1, r8
  48a9f4: ebfa0f3b     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17c314
  48a9f8: e3500000     	cmp	r0, #0
  48a9fc: e1a03005     	mov	r3, r5
  48aa00: e1a0100a     	mov	r1, r10
  48aa04: e1a02004     	mov	r2, r4
  48aa08: 1a00000b     	bne	0x48aa3c <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x130> @ imm = #0x2c
  48aa0c: e594005c     	ldr	r0, [r4, #0x5c]
  48aa10: e59dc00c     	ldr	r12, [sp, #0xc]
  48aa14: e0000099     	mul	r0, r9, r0
  48aa18: e58dc008     	str	r12, [sp, #0x8]
  48aa1c: e2800060     	add	r0, r0, #96
  48aa20: e0840000     	add	r0, r4, r0
  48aa24: ebfffee9     	bl	0x48a5d0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)> @ imm = #-0x45c
  48aa28: e3500000     	cmp	r0, #0
  48aa2c: 1594305c     	ldrne	r3, [r4, #0x5c]
  48aa30: 12855001     	addne	r5, r5, #1
  48aa34: 12833001     	addne	r3, r3, #1
  48aa38: 1584305c     	strne	r3, [r4, #0x5c]
  48aa3c: e59d300c     	ldr	r3, [sp, #0xc]
  48aa40: e593303c     	ldr	r3, [r3, #0x3c]
  48aa44: e3530000     	cmp	r3, #0
  48aa48: 1affffe3     	bne	0x48a9dc <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0xd0> @ imm = #-0x74
  48aa4c: e3a00001     	mov	r0, #1
  48aa50: e28dd010     	add	sp, sp, #16
  48aa54: e8bd87f0     	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
  48aa58: e1cd00d0     	ldrd	r0, r1, [sp]
  48aa5c: ebfa0f0f     	bl	0x30e6a0 <.plt+0x92c>   @ imm = #-0x17c3c4
  48aa60: e5840050     	str	r0, [r4, #0x50]
  48aa64: eaffffc1     	b	0x48a970 <rnd::MgxBlock::LoadFromXml(TiXmlHandle&)+0x64> @ imm = #-0xfc
  48aa68: e0 a3 44 00  	.word	0x0044a3e0
  48aa6c: c8 a3 44 00  	.word	0x0044a3c8
  48aa70: bc a3 44 00  	.word	0x0044a3bc
  48aa74: b8 a3 44 00  	.word	0x0044a3b8
  48aa78: c4 58 43 00  	.word	0x004358c4
  48aa7c: b0 57 43 00  	.word	0x004357b0
  48aa80: 8c a3 44 00  	.word	0x0044a38c
