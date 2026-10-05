
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f01c8 <Level::_LoadFromXML(TiXmlElement*)>:
  3f01c8: e92d4070     	push	{r4, r5, r6, lr}
  3f01cc: e59f40b4     	ldr	r4, [pc, #0xb4]         @ 0x3f0288 <Level::_LoadFromXML(TiXmlElement*)+0xc0>
  3f01d0: e2515000     	subs	r5, r1, #0
  3f01d4: e24dd008     	sub	sp, sp, #8
  3f01d8: e1a06000     	mov	r6, r0
  3f01dc: e08f4004     	add	r4, pc, r4
  3f01e0: 0a000013     	beq	0x3f0234 <Level::_LoadFromXML(TiXmlElement*)+0x6c> @ imm = #0x4c
  3f01e4: e59f10a0     	ldr	r1, [pc, #0xa0]         @ 0x3f028c <Level::_LoadFromXML(TiXmlElement*)+0xc4>
  3f01e8: e1a00005     	mov	r0, r5
  3f01ec: e08f1001     	add	r1, pc, r1
  3f01f0: eb04929e     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x124a78
  3f01f4: e59f1094     	ldr	r1, [pc, #0x94]         @ 0x3f0290 <Level::_LoadFromXML(TiXmlElement*)+0xc8>
  3f01f8: e08f1001     	add	r1, pc, r1
  3f01fc: ebfc7846     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe1ee8
  3f0200: e3500000     	cmp	r0, #0
  3f0204: 0a000008     	beq	0x3f022c <Level::_LoadFromXML(TiXmlElement*)+0x64> @ imm = #0x20
  3f0208: e59f3084     	ldr	r3, [pc, #0x84]         @ 0x3f0294 <Level::_LoadFromXML(TiXmlElement*)+0xcc>
  3f020c: e596c18c     	ldr	r12, [r6, #0x18c]
  3f0210: e1a01005     	mov	r1, r5
  3f0214: e7940003     	ldr	r0, [r4, r3]
  3f0218: e3a02000     	mov	r2, #0
  3f021c: e2863e16     	add	r3, r6, #352
  3f0220: e5900038     	ldr	r0, [r0, #0x38]
  3f0224: e58dc000     	str	r12, [sp]
  3f0228: ebfd6d8e     	bl	0x34b868 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)> @ imm = #-0xa49c8
  3f022c: e28dd008     	add	sp, sp, #8
  3f0230: e8bd8070     	pop	{r4, r5, r6, pc}
  3f0234: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x3f0298 <Level::_LoadFromXML(TiXmlElement*)+0xd0>
  3f0238: e7943003     	ldr	r3, [r4, r3]
  3f023c: e5933000     	ldr	r3, [r3]
  3f0240: e3530002     	cmp	r3, #2
  3f0244: 05855000     	streq	r5, [r5]
  3f0248: 0affffe5     	beq	0x3f01e4 <Level::_LoadFromXML(TiXmlElement*)+0x1c> @ imm = #-0x6c
  3f024c: e3530001     	cmp	r3, #1
  3f0250: 1affffe3     	bne	0x3f01e4 <Level::_LoadFromXML(TiXmlElement*)+0x1c> @ imm = #-0x74
  3f0254: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x3f029c <Level::_LoadFromXML(TiXmlElement*)+0xd4>
  3f0258: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x3f02a0 <Level::_LoadFromXML(TiXmlElement*)+0xd8>
  3f025c: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x3f02a4 <Level::_LoadFromXML(TiXmlElement*)+0xdc>
  3f0260: e7940000     	ldr	r0, [r4, r0]
  3f0264: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x3f02a8 <Level::_LoadFromXML(TiXmlElement*)+0xe0>
  3f0268: e300c8de     	movw	r12, #0x8de
  3f026c: e08f1001     	add	r1, pc, r1
  3f0270: e08f2002     	add	r2, pc, r2
  3f0274: e08f3003     	add	r3, pc, r3
  3f0278: e28000a8     	add	r0, r0, #168
  3f027c: e58dc000     	str	r12, [sp]
  3f0280: ebfc775f     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe2284
  3f0284: eaffffd6     	b	0x3f01e4 <Level::_LoadFromXML(TiXmlElement*)+0x1c> @ imm = #-0xa8
  3f0288: b4 48 5a 00  	.word	0x005a48b4
  3f028c: 8c ff 4c 00  	.word	0x004cff8c
  3f0290: d0 01 4d 00  	.word	0x004d01d0
  3f0294: f4 37 00 00  	.word	0x000037f4
  3f0298: c0 39 00 00  	.word	0x000039c0
  3f029c: c0 19 00 00  	.word	0x000019c0
  3f02a0: 6c e1 4c 00  	.word	0x004ce16c
  3f02a4: c8 01 4d 00  	.word	0x004d01c8
  3f02a8: 9c 62 4d 00  	.word	0x004d629c
