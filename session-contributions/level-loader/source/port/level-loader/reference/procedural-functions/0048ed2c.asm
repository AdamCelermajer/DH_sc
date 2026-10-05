
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048ed2c <rnd::ListRule::LoadFromXml(TiXmlNode*)>:
  48ed2c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48ed30: e59fa1c0     	ldr	r10, [pc, #0x1c0]       @ 0x48eef8 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1cc>
  48ed34: e59fb1c0     	ldr	r11, [pc, #0x1c0]       @ 0x48eefc <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1d0>
  48ed38: e24dd074     	sub	sp, sp, #116
  48ed3c: e08fa00a     	add	r10, pc, r10
  48ed40: e79a200b     	ldr	r2, [r10, r11]
  48ed44: e5913000     	ldr	r3, [r1]
  48ed48: e1a06000     	mov	r6, r0
  48ed4c: e5922000     	ldr	r2, [r2]
  48ed50: e1a00001     	mov	r0, r1
  48ed54: e1a04001     	mov	r4, r1
  48ed58: e58d206c     	str	r2, [sp, #0x6c]
  48ed5c: e1a0e00f     	mov	lr, pc
  48ed60: e593f02c     	ldr	pc, [r3, #0x2c]
  48ed64: e2507000     	subs	r7, r0, #0
  48ed68: 0a00001b     	beq	0x48eddc <rnd::ListRule::LoadFromXml(TiXmlNode*)+0xb0> @ imm = #0x6c
  48ed6c: e59f118c     	ldr	r1, [pc, #0x18c]        @ 0x48ef00 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1d4>
  48ed70: e08f1001     	add	r1, pc, r1
  48ed74: eb0217bd     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x85ef4
  48ed78: e3a01000     	mov	r1, #0
  48ed7c: e1a05000     	mov	r5, r0
  48ed80: e3e02000     	mvn	r2, #0
  48ed84: ebfafda2     	bl	0x34e414 <ToLowerCase(char*, int, int)> @ imm = #-0x140978
  48ed88: e3550000     	cmp	r5, #0
  48ed8c: 0a000054     	beq	0x48eee4 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1b8> @ imm = #0x150
  48ed90: e1a00005     	mov	r0, r5
  48ed94: ebf9fc2e     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x180f48
  48ed98: e0852000     	add	r2, r5, r0
  48ed9c: e1a01005     	mov	r1, r5
  48eda0: e1a00006     	mov	r0, r6
  48eda4: ebfa070d     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x17e3cc
  48eda8: e59f1154     	ldr	r1, [pc, #0x154]        @ 0x48ef04 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1d8>
  48edac: e1a00007     	mov	r0, r7
  48edb0: e08f1001     	add	r1, pc, r1
  48edb4: eb0217ad     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x85eb4
  48edb8: e3500000     	cmp	r0, #0
  48edbc: 03a00001     	moveq	r0, #1
  48edc0: 0a000004     	beq	0x48edd8 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0xac> @ imm = #0x10
  48edc4: e59f113c     	ldr	r1, [pc, #0x13c]        @ 0x48ef08 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1dc>
  48edc8: e08f1001     	add	r1, pc, r1
  48edcc: ebf9fd52     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x180ab8
  48edd0: e2700001     	rsbs	r0, r0, #1
  48edd4: 33a00000     	movlo	r0, #0
  48edd8: e5c60018     	strb	r0, [r6, #0x18]
  48eddc: e59f7128     	ldr	r7, [pc, #0x128]        @ 0x48ef0c <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1e0>
  48ede0: e1a00004     	mov	r0, r4
  48ede4: e08f7007     	add	r7, pc, r7
  48ede8: e1a01007     	mov	r1, r7
  48edec: eb0217e9     	bl	0x514d98 <TiXmlNode::FirstChild(char const*) const> @ imm = #0x85fa4
  48edf0: e2505000     	subs	r5, r0, #0
  48edf4: 0a000029     	beq	0x48eea0 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x174> @ imm = #0xa4
  48edf8: e59f3110     	ldr	r3, [pc, #0x110]        @ 0x48ef10 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1e4>
  48edfc: e59f9110     	ldr	r9, [pc, #0x110]        @ 0x48ef14 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1e8>
  48ee00: e286801c     	add	r8, r6, #28
  48ee04: e58d3008     	str	r3, [sp, #0x8]
  48ee08: e59f3108     	ldr	r3, [pc, #0x108]        @ 0x48ef18 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1ec>
  48ee0c: e28d401c     	add	r4, sp, #28
  48ee10: e08f3003     	add	r3, pc, r3
  48ee14: e58d300c     	str	r3, [sp, #0xc]
  48ee18: e59f30fc     	ldr	r3, [pc, #0xfc]         @ 0x48ef1c <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1f0>
  48ee1c: e08f3003     	add	r3, pc, r3
  48ee20: e58d3010     	str	r3, [sp, #0x10]
  48ee24: e59f30f4     	ldr	r3, [pc, #0xf4]         @ 0x48ef20 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1f4>
  48ee28: e08f3003     	add	r3, pc, r3
  48ee2c: e58d3014     	str	r3, [sp, #0x14]
  48ee30: e1a01006     	mov	r1, r6
  48ee34: e1a00004     	mov	r0, r4
  48ee38: ebfffcba     	bl	0x48e128 <rnd::ListElem::ListElem(rnd::ListRule*)> @ imm = #-0xd18
  48ee3c: e1a00004     	mov	r0, r4
  48ee40: e1a01005     	mov	r1, r5
  48ee44: ebfff5b4     	bl	0x48c51c <rnd::ListElem::LoadFromXml(TiXmlNode*)> @ imm = #-0x2930
  48ee48: e5960028     	ldr	r0, [r6, #0x28]
  48ee4c: e59d1034     	ldr	r1, [sp, #0x34]
  48ee50: ebffd5de     	bl	0x4845d0 <rnd::RandomGenerator::ValidBlock(char const*)> @ imm = #-0xa888
  48ee54: e3500000     	cmp	r0, #0
  48ee58: 1a000006     	bne	0x48ee78 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x14c> @ imm = #0x18
  48ee5c: e79a3009     	ldr	r3, [r10, r9]
  48ee60: e5933000     	ldr	r3, [r3]
  48ee64: e3530002     	cmp	r3, #2
  48ee68: 05800000     	streq	r0, [r0]
  48ee6c: 0a000001     	beq	0x48ee78 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x14c> @ imm = #0x4
  48ee70: e3530001     	cmp	r3, #1
  48ee74: 0a000010     	beq	0x48eebc <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x190> @ imm = #0x40
  48ee78: e1a01004     	mov	r1, r4
  48ee7c: e1a00008     	mov	r0, r8
  48ee80: ebffff32     	bl	0x48eb50 <std::vector<rnd::ListElem, std::allocator<rnd::ListElem>>::push_back(rnd::ListElem const&)> @ imm = #-0x338
  48ee84: e1a00004     	mov	r0, r4
  48ee88: ebffe4d0     	bl	0x4881d0 <rnd::ListElem::~ListElem()> @ imm = #-0x6cc0
  48ee8c: e1a00005     	mov	r0, r5
  48ee90: e1a01007     	mov	r1, r7
  48ee94: eb02178b     	bl	0x514cc8 <TiXmlNode::NextSibling(char const*) const> @ imm = #0x85e2c
  48ee98: e2505000     	subs	r5, r0, #0
  48ee9c: 1affffe3     	bne	0x48ee30 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x104> @ imm = #-0x74
  48eea0: e79a300b     	ldr	r3, [r10, r11]
  48eea4: e59d206c     	ldr	r2, [sp, #0x6c]
  48eea8: e5933000     	ldr	r3, [r3]
  48eeac: e1520003     	cmp	r2, r3
  48eeb0: 1a00000f     	bne	0x48eef4 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1c8> @ imm = #0x3c
  48eeb4: e28dd074     	add	sp, sp, #116
  48eeb8: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48eebc: e59d3008     	ldr	r3, [sp, #0x8]
  48eec0: e3a0c059     	mov	r12, #89
  48eec4: e59d100c     	ldr	r1, [sp, #0xc]
  48eec8: e79a0003     	ldr	r0, [r10, r3]
  48eecc: e59d2010     	ldr	r2, [sp, #0x10]
  48eed0: e59d3014     	ldr	r3, [sp, #0x14]
  48eed4: e28000a8     	add	r0, r0, #168
  48eed8: e58dc000     	str	r12, [sp]
  48eedc: ebf9fc48     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x180ee0
  48eee0: eaffffe4     	b	0x48ee78 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x14c> @ imm = #-0x70
  48eee4: e1a00005     	mov	r0, r5
  48eee8: e59f5034     	ldr	r5, [pc, #0x34]         @ 0x48ef24 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x1f8>
  48eeec: e08f5005     	add	r5, pc, r5
  48eef0: eaffffa8     	b	0x48ed98 <rnd::ListRule::LoadFromXml(TiXmlNode*)+0x6c> @ imm = #-0x160
  48eef4: ebf9fd05     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x180bec
  48eef8: 54 5d 50 00  	.word	0x00505d54
  48eefc: ac 40 00 00  	.word	0x000040ac
  48ef00: 78 23 45 00  	.word	0x00452378
  48ef04: f8 60 44 00  	.word	0x004460f8
  48ef08: 28 fb 42 00  	.word	0x0042fb28
  48ef0c: 2c 60 44 00  	.word	0x0044602c
  48ef10: c0 19 00 00  	.word	0x000019c0
  48ef14: c0 39 00 00  	.word	0x000039c0
  48ef18: c8 f5 42 00  	.word	0x0042f5c8
  48ef1c: 9c 60 44 00  	.word	0x0044609c
  48ef20: 00 60 44 00  	.word	0x00446000
  48ef24: 1c c9 43 00  	.word	0x0043c91c
