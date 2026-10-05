
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488b84 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)>:
  488b84: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  488b88: e59f528c     	ldr	r5, [pc, #0x28c]        @ 0x488e1c <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x298>
  488b8c: e59f228c     	ldr	r2, [pc, #0x28c]        @ 0x488e20 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x29c>
  488b90: e24ddf57     	sub	sp, sp, #348
  488b94: e08f5005     	add	r5, pc, r5
  488b98: e7953002     	ldr	r3, [r5, r2]
  488b9c: e28d4090     	add	r4, sp, #144
  488ba0: e1a07000     	mov	r7, r0
  488ba4: e5933000     	ldr	r3, [r3]
  488ba8: e1a00004     	mov	r0, r4
  488bac: e58d200c     	str	r2, [sp, #0xc]
  488bb0: e58d3154     	str	r3, [sp, #0x154]
  488bb4: e1a09001     	mov	r9, r1
  488bb8: eb0238c1     	bl	0x516ec4 <TiXmlDocument::TiXmlDocument()> @ imm = #0x8e304
  488bbc: e3a01000     	mov	r1, #0
  488bc0: e3a00088     	mov	r0, #136
  488bc4: ebfa1e69     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x17865c
  488bc8: e59f1254     	ldr	r1, [pc, #0x254]        @ 0x488e24 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2a0>
  488bcc: e59f2254     	ldr	r2, [pc, #0x254]        @ 0x488e28 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2a4>
  488bd0: e59f3254     	ldr	r3, [pc, #0x254]        @ 0x488e2c <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2a8>
  488bd4: e1a08000     	mov	r8, r0
  488bd8: e08f2002     	add	r2, pc, r2
  488bdc: e08f3003     	add	r3, pc, r3
  488be0: e08f1001     	add	r1, pc, r1
  488be4: eb023aa9     	bl	0x517690 <TiXmlDeclaration::TiXmlDeclaration(char const*, char const*, char const*)> @ imm = #0x8eaa4
  488be8: e1a01008     	mov	r1, r8
  488bec: e1a00004     	mov	r0, r4
  488bf0: eb02335b     	bl	0x515964 <TiXmlNode::LinkEndChild(TiXmlNode*)> @ imm = #0x8cd6c
  488bf4: e3a01000     	mov	r1, #0
  488bf8: e3a00040     	mov	r0, #64
  488bfc: ebfa1e5b     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x178694
  488c00: e59fa228     	ldr	r10, [pc, #0x228]       @ 0x488e30 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2ac>
  488c04: e3a01002     	mov	r1, #2
  488c08: e1a0b000     	mov	r11, r0
  488c0c: eb023481     	bl	0x515e18 <TiXmlNode::TiXmlNode(TiXmlNode::NodeType)> @ imm = #0x8d204
  488c10: e795a00a     	ldr	r10, [r5, r10]
  488c14: e59f1218     	ldr	r1, [pc, #0x218]        @ 0x488e34 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2b0>
  488c18: e1a0000b     	mov	r0, r11
  488c1c: e28aa008     	add	r10, r10, #8
  488c20: e08f1001     	add	r1, pc, r1
  488c24: e2812040     	add	r2, r1, #64
  488c28: e480a020     	str	r10, [r0], #32
  488c2c: ebfa1f6b     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178254
  488c30: e3a01000     	mov	r1, #0
  488c34: e3a00040     	mov	r0, #64
  488c38: ebfa1e4c     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x1786d0
  488c3c: e3a01002     	mov	r1, #2
  488c40: e58d0008     	str	r0, [sp, #0x8]
  488c44: eb023473     	bl	0x515e18 <TiXmlNode::TiXmlNode(TiXmlNode::NodeType)> @ imm = #0x8d1cc
  488c48: e59f11e8     	ldr	r1, [pc, #0x1e8]        @ 0x488e38 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2b4>
  488c4c: e59d0008     	ldr	r0, [sp, #0x8]
  488c50: e28d6010     	add	r6, sp, #16
  488c54: e08f1001     	add	r1, pc, r1
  488c58: e2812017     	add	r2, r1, #23
  488c5c: e480a020     	str	r10, [r0], #32
  488c60: ebfa1f5e     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178288
  488c64: e59f11d0     	ldr	r1, [pc, #0x1d0]        @ 0x488e3c <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2b8>
  488c68: e5972188     	ldr	r2, [r7, #0x188]
  488c6c: e1a00006     	mov	r0, r6
  488c70: e08f1001     	add	r1, pc, r1
  488c74: ebfa179a     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x17a198
  488c78: e3a01000     	mov	r1, #0
  488c7c: e3a00040     	mov	r0, #64
  488c80: ebfa1e3a     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x178718
  488c84: e3a01002     	mov	r1, #2
  488c88: e1a08000     	mov	r8, r0
  488c8c: e58d0004     	str	r0, [sp, #0x4]
  488c90: eb023460     	bl	0x515e18 <TiXmlNode::TiXmlNode(TiXmlNode::NodeType)> @ imm = #0x8d180
  488c94: e488a020     	str	r10, [r8], #32
  488c98: e1a00006     	mov	r0, r6
  488c9c: ebfa146c     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17ae50
  488ca0: e1a01006     	mov	r1, r6
  488ca4: e0862000     	add	r2, r6, r0
  488ca8: e1a00008     	mov	r0, r8
  488cac: ebfa1f4b     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x1782d4
  488cb0: e1a0100b     	mov	r1, r11
  488cb4: e1a00004     	mov	r0, r4
  488cb8: eb023329     	bl	0x515964 <TiXmlNode::LinkEndChild(TiXmlNode*)> @ imm = #0x8cca4
  488cbc: e59d1008     	ldr	r1, [sp, #0x8]
  488cc0: e1a00004     	mov	r0, r4
  488cc4: eb023326     	bl	0x515964 <TiXmlNode::LinkEndChild(TiXmlNode*)> @ imm = #0x8cc98
  488cc8: e59d3004     	ldr	r3, [sp, #0x4]
  488ccc: e1a00004     	mov	r0, r4
  488cd0: e1a01003     	mov	r1, r3
  488cd4: eb023322     	bl	0x515964 <TiXmlNode::LinkEndChild(TiXmlNode*)> @ imm = #0x8cc88
  488cd8: e3a01000     	mov	r1, #0
  488cdc: e3a0008c     	mov	r0, #140
  488ce0: ebfa1e22     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0x178778
  488ce4: e59f1154     	ldr	r1, [pc, #0x154]        @ 0x488e40 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2bc>
  488ce8: e1a06000     	mov	r6, r0
  488cec: e08f1001     	add	r1, pc, r1
  488cf0: eb0239d9     	bl	0x51745c <TiXmlElement::TiXmlElement(char const*)> @ imm = #0x8e764
  488cf4: e1a01006     	mov	r1, r6
  488cf8: e1a00004     	mov	r0, r4
  488cfc: eb023318     	bl	0x515964 <TiXmlNode::LinkEndChild(TiXmlNode*)> @ imm = #0x8cc60
  488d00: e597018c     	ldr	r0, [r7, #0x18c]
  488d04: e1a01006     	mov	r1, r6
  488d08: e3a02000     	mov	r2, #0
  488d0c: e2800004     	add	r0, r0, #4
  488d10: eb0229f6     	bl	0x5134f0 <PropertyMap::SavePropertiesToXML(TiXmlElement*, char const*)> @ imm = #0x8a7d8
  488d14: e5970114     	ldr	r0, [r7, #0x114]
  488d18: e3500000     	cmp	r0, #0
  488d1c: 0a000002     	beq	0x488d2c <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x1a8> @ imm = #0x8
  488d20: e1a01006     	mov	r1, r6
  488d24: e3a02000     	mov	r2, #0
  488d28: eb002418     	bl	0x491d90 <rnd::Tile::SaveAsModuleXML(TiXmlElement*, int)> @ imm = #0x9060
  488d2c: e28d6c01     	add	r6, sp, #256
  488d30: e59f710c     	ldr	r7, [pc, #0x10c]        @ 0x488e44 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2c0>
  488d34: e1a00006     	mov	r0, r6
  488d38: ebfff358     	bl	0x485aa0 <TiXmlPrinter::TiXmlPrinter()> @ imm = #-0x32a0
  488d3c: e59f1104     	ldr	r1, [pc, #0x104]        @ 0x488e48 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2c4>
  488d40: e7957007     	ldr	r7, [r5, r7]
  488d44: e286003c     	add	r0, r6, #60
  488d48: e08f1001     	add	r1, pc, r1
  488d4c: e2812002     	add	r2, r1, #2
  488d50: e2877008     	add	r7, r7, #8
  488d54: e58d7100     	str	r7, [sp, #0x100]
  488d58: ebfa1f20     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x178380
  488d5c: e1a01006     	mov	r1, r6
  488d60: e1a00004     	mov	r0, r4
  488d64: eb022e17     	bl	0x5145c8 <TiXmlDocument::Accept(TiXmlVisitor*) const> @ imm = #0x8b85c
  488d68: e59d111c     	ldr	r1, [sp, #0x11c]
  488d6c: e59d2120     	ldr	r2, [sp, #0x120]
  488d70: e1a00009     	mov	r0, r9
  488d74: e3a03000     	mov	r3, #0
  488d78: e0622001     	rsb	r2, r2, r1
  488d7c: ebfa38ff     	bl	0x317180 <StreamBuffer::expand(unsigned long long)> @ imm = #-0x171c04
  488d80: e59d1120     	ldr	r1, [sp, #0x120]
  488d84: e59d211c     	ldr	r2, [sp, #0x11c]
  488d88: e3a03000     	mov	r3, #0
  488d8c: e599c000     	ldr	r12, [r9]
  488d90: e0612002     	rsb	r2, r1, r2
  488d94: e1a00009     	mov	r0, r9
  488d98: e1a0e00f     	mov	lr, pc
  488d9c: e59cf01c     	ldr	pc, [r12, #0x1c]
  488da0: e1a00006     	mov	r0, r6
  488da4: e58d7100     	str	r7, [sp, #0x100]
  488da8: ebfffda1     	bl	0x488434 <TiXmlPrinter::~TiXmlPrinter()> @ imm = #-0x97c
  488dac: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x488e4c <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x2c8>
  488db0: e59d00ec     	ldr	r0, [sp, #0xec]
  488db4: e2842048     	add	r2, r4, #72
  488db8: e7953003     	ldr	r3, [r5, r3]
  488dbc: e1500002     	cmp	r0, r2
  488dc0: e2833008     	add	r3, r3, #8
  488dc4: e58d3090     	str	r3, [sp, #0x90]
  488dc8: 0a000006     	beq	0x488de8 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x264> @ imm = #0x18
  488dcc: e3500000     	cmp	r0, #0
  488dd0: 0a000004     	beq	0x488de8 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x264> @ imm = #0x10
  488dd4: e59d10d8     	ldr	r1, [sp, #0xd8]
  488dd8: e0601001     	rsb	r1, r0, r1
  488ddc: e3510080     	cmp	r1, #128
  488de0: 8a00000a     	bhi	0x488e10 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x28c> @ imm = #0x28
  488de4: eb0a0045     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280114
  488de8: e1a00004     	mov	r0, r4
  488dec: eb022f30     	bl	0x514ab4 <TiXmlNode::~TiXmlNode()> @ imm = #0x8bcc0
  488df0: e59d200c     	ldr	r2, [sp, #0xc]
  488df4: e7953002     	ldr	r3, [r5, r2]
  488df8: e59d2154     	ldr	r2, [sp, #0x154]
  488dfc: e5933000     	ldr	r3, [r3]
  488e00: e1520003     	cmp	r2, r3
  488e04: 1a000003     	bne	0x488e18 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x294> @ imm = #0xc
  488e08: e28ddf57     	add	sp, sp, #348
  488e0c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  488e10: ebfa1d8a     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1789d8
  488e14: eafffff3     	b	0x488de8 <rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)+0x264> @ imm = #-0x34
  488e18: ebfa153c     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17ab10
  488e1c: fc be 50 00  	.word	0x0050befc
  488e20: ac 40 00 00  	.word	0x000040ac
  488e24: 08 c0 44 00  	.word	0x0044c008
  488e28: 18 c0 44 00  	.word	0x0044c018
  488e2c: 2c 2c 44 00  	.word	0x00442c2c
  488e30: 84 0e 00 00  	.word	0x00000e84
  488e34: d8 bf 44 00  	.word	0x0044bfd8
  488e38: ec bf 44 00  	.word	0x0044bfec
  488e3c: e8 bf 44 00  	.word	0x0044bfe8
  488e40: c4 da 43 00  	.word	0x0043dac4
  488e44: ec 42 00 00  	.word	0x000042ec
  488e48: b8 72 43 00  	.word	0x004372b8
  488e4c: 30 09 00 00  	.word	0x00000930
