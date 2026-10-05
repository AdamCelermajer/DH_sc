
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048cac0 <rnd::RoomPool::LoadFromXml(TiXmlNode*)>:
  48cac0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48cac4: e1a04000     	mov	r4, r0
  48cac8: e24dd044     	sub	sp, sp, #68
  48cacc: e5913000     	ldr	r3, [r1]
  48cad0: e1a00001     	mov	r0, r1
  48cad4: e1a05001     	mov	r5, r1
  48cad8: e1a0e00f     	mov	lr, pc
  48cadc: e593f02c     	ldr	pc, [r3, #0x2c]
  48cae0: e3500000     	cmp	r0, #0
  48cae4: 0a000004     	beq	0x48cafc <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x3c> @ imm = #0x10
  48cae8: e59f128c     	ldr	r1, [pc, #0x28c]        @ 0x48cd7c <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x2bc>
  48caec: e08f1001     	add	r1, pc, r1
  48caf0: eb02205e     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x88178
  48caf4: ebfa0566     	bl	0x30e094 <.plt+0x320>   @ imm = #-0x17ea68
  48caf8: e5840000     	str	r0, [r4]
  48cafc: e59fb27c     	ldr	r11, [pc, #0x27c]       @ 0x48cd80 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x2c0>
  48cb00: e1a00005     	mov	r0, r5
  48cb04: e08fb00b     	add	r11, pc, r11
  48cb08: e1a0100b     	mov	r1, r11
  48cb0c: eb0220a1     	bl	0x514d98 <TiXmlNode::FirstChild(char const*) const> @ imm = #0x88284
  48cb10: e2505000     	subs	r5, r0, #0
  48cb14: 0a000023     	beq	0x48cba8 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0xe8> @ imm = #0x8c
  48cb18: e30a1aaa     	movw	r1, #0xaaaa
  48cb1c: e1811601     	orr	r1, r1, r1, lsl #12
  48cb20: e284200c     	add	r2, r4, #12
  48cb24: e28d303c     	add	r3, sp, #60
  48cb28: e58d1004     	str	r1, [sp, #0x4]
  48cb2c: e58d2010     	str	r2, [sp, #0x10]
  48cb30: e28da024     	add	r10, sp, #36
  48cb34: e3a08000     	mov	r8, #0
  48cb38: e3e06000     	mvn	r6, #0
  48cb3c: e58d3014     	str	r3, [sp, #0x14]
  48cb40: e1a0000a     	mov	r0, r10
  48cb44: e1a01005     	mov	r1, r5
  48cb48: e58d4024     	str	r4, [sp, #0x24]
  48cb4c: e58d8028     	str	r8, [sp, #0x28]
  48cb50: e5cd802c     	strb	r8, [sp, #0x2c]
  48cb54: e58d6030     	str	r6, [sp, #0x30]
  48cb58: e58d6034     	str	r6, [sp, #0x34]
  48cb5c: e58d6038     	str	r6, [sp, #0x38]
  48cb60: ebfffe49     	bl	0x48c48c <rnd::RPElem::LoadFromXml(TiXmlNode*)> @ imm = #-0x6dc
  48cb64: e594c008     	ldr	r12, [r4, #0x8]
  48cb68: e594700c     	ldr	r7, [r4, #0xc]
  48cb6c: e15c0007     	cmp	r12, r7
  48cb70: 0a00000e     	beq	0x48cbb0 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0xf0> @ imm = #0x38
  48cb74: e1a0e00a     	mov	lr, r10
  48cb78: e8be000f     	ldm	lr!, {r0, r1, r2, r3}
  48cb7c: e8ac000f     	stm	r12!, {r0, r1, r2, r3}
  48cb80: e89e0003     	ldm	lr, {r0, r1}
  48cb84: e88c0003     	stm	r12, {r0, r1}
  48cb88: e5943008     	ldr	r3, [r4, #0x8]
  48cb8c: e2833018     	add	r3, r3, #24
  48cb90: e5843008     	str	r3, [r4, #0x8]
  48cb94: e1a00005     	mov	r0, r5
  48cb98: e1a0100b     	mov	r1, r11
  48cb9c: eb022049     	bl	0x514cc8 <TiXmlNode::NextSibling(char const*) const> @ imm = #0x88124
  48cba0: e2505000     	subs	r5, r0, #0
  48cba4: 1affffe5     	bne	0x48cb40 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x80> @ imm = #-0x6c
  48cba8: e28dd044     	add	sp, sp, #68
  48cbac: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48cbb0: e5943004     	ldr	r3, [r4, #0x4]
  48cbb4: e59d1004     	ldr	r1, [sp, #0x4]
  48cbb8: e0633007     	rsb	r3, r3, r7
  48cbbc: e1a031c3     	asr	r3, r3, #3
  48cbc0: e0832103     	add	r2, r3, r3, lsl #2
  48cbc4: e0822202     	add	r2, r2, r2, lsl #4
  48cbc8: e0822402     	add	r2, r2, r2, lsl #8
  48cbcc: e0822802     	add	r2, r2, r2, lsl #16
  48cbd0: e0832082     	add	r2, r3, r2, lsl #1
  48cbd4: e3520001     	cmp	r2, #1
  48cbd8: 20823002     	addhs	r3, r2, r2
  48cbdc: 32823001     	addlo	r3, r2, #1
  48cbe0: e1530001     	cmp	r3, r1
  48cbe4: 9a000058     	bls	0x48cd4c <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x28c> @ imm = #0x160
  48cbe8: e30a3aaa     	movw	r3, #0xaaaa
  48cbec: e1833603     	orr	r3, r3, r3, lsl #12
  48cbf0: e1a01003     	mov	r1, r3
  48cbf4: e59d0010     	ldr	r0, [sp, #0x10]
  48cbf8: e59d2014     	ldr	r2, [sp, #0x14]
  48cbfc: e58d303c     	str	r3, [sp, #0x3c]
  48cc00: ebffff7c     	bl	0x48c9f8 <std::allocator<rnd::RPElem>::_M_allocate(unsigned int, unsigned int&)> @ imm = #-0x210
  48cc04: e58d0008     	str	r0, [sp, #0x8]
  48cc08: e5943004     	ldr	r3, [r4, #0x4]
  48cc0c: e0637007     	rsb	r7, r3, r7
  48cc10: e1a071c7     	asr	r7, r7, #3
  48cc14: e087e107     	add	lr, r7, r7, lsl #2
  48cc18: e08ee20e     	add	lr, lr, lr, lsl #4
  48cc1c: e08ee40e     	add	lr, lr, lr, lsl #8
  48cc20: e08ee80e     	add	lr, lr, lr, lsl #16
  48cc24: e087e08e     	add	lr, r7, lr, lsl #1
  48cc28: e35e0000     	cmp	lr, #0
  48cc2c: e58de00c     	str	lr, [sp, #0xc]
  48cc30: d1a0e000     	movle	lr, r0
  48cc34: da000014     	ble	0x48cc8c <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x1cc> @ imm = #0x50
  48cc38: e58d5018     	str	r5, [sp, #0x18]
  48cc3c: e59d900c     	ldr	r9, [sp, #0xc]
  48cc40: e59d5008     	ldr	r5, [sp, #0x8]
  48cc44: e58d401c     	str	r4, [sp, #0x1c]
  48cc48: e3a07000     	mov	r7, #0
  48cc4c: e1a04003     	mov	r4, r3
  48cc50: e085c007     	add	r12, r5, r7
  48cc54: e084e007     	add	lr, r4, r7
  48cc58: e8be000f     	ldm	lr!, {r0, r1, r2, r3}
  48cc5c: e8ac000f     	stm	r12!, {r0, r1, r2, r3}
  48cc60: e89e0003     	ldm	lr, {r0, r1}
  48cc64: e2599001     	subs	r9, r9, #1
  48cc68: e88c0003     	stm	r12, {r0, r1}
  48cc6c: e2877018     	add	r7, r7, #24
  48cc70: 1afffff6     	bne	0x48cc50 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x190> @ imm = #-0x28
  48cc74: e59d200c     	ldr	r2, [sp, #0xc]
  48cc78: e59d7008     	ldr	r7, [sp, #0x8]
  48cc7c: e3a03018     	mov	r3, #24
  48cc80: e59d5018     	ldr	r5, [sp, #0x18]
  48cc84: e59d401c     	ldr	r4, [sp, #0x1c]
  48cc88: e02e7293     	mla	lr, r3, r2, r7
  48cc8c: e1a0c00a     	mov	r12, r10
  48cc90: e1a0700e     	mov	r7, lr
  48cc94: e8bc000f     	ldm	r12!, {r0, r1, r2, r3}
  48cc98: e8a7000f     	stm	r7!, {r0, r1, r2, r3}
  48cc9c: e89c0003     	ldm	r12, {r0, r1}
  48cca0: e1a03007     	mov	r3, r7
  48cca4: e8830003     	stm	r3, {r0, r1}
  48cca8: e9940009     	ldmib	r4, {r0, r3}
  48ccac: e28e7018     	add	r7, lr, #24
  48ccb0: e1530000     	cmp	r3, r0
  48ccb4: 0a00000d     	beq	0x48ccf0 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x230> @ imm = #0x34
  48ccb8: e2432018     	sub	r2, r3, #24
  48ccbc: e0602002     	rsb	r2, r0, r2
  48ccc0: e1a021a2     	lsr	r2, r2, #3
  48ccc4: e0821102     	add	r1, r2, r2, lsl #2
  48ccc8: e0821101     	add	r1, r2, r1, lsl #2
  48cccc: e0811301     	add	r1, r1, r1, lsl #6
  48ccd0: e0821101     	add	r1, r2, r1, lsl #2
  48ccd4: e0811701     	add	r1, r1, r1, lsl #14
  48ccd8: e0822081     	add	r2, r2, r1, lsl #1
  48ccdc: e3c2220e     	bic	r2, r2, #-536870912
  48cce0: e3e01017     	mvn	r1, #23
  48cce4: e0020291     	mul	r2, r1, r2
  48cce8: e0822001     	add	r2, r2, r1
  48ccec: e0833002     	add	r3, r3, r2
  48ccf0: e3530000     	cmp	r3, #0
  48ccf4: e594200c     	ldr	r2, [r4, #0xc]
  48ccf8: 0a00000b     	beq	0x48cd2c <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x26c> @ imm = #0x2c
  48ccfc: e0633002     	rsb	r3, r3, r2
  48cd00: e1a031c3     	asr	r3, r3, #3
  48cd04: e3a02018     	mov	r2, #24
  48cd08: e0831103     	add	r1, r3, r3, lsl #2
  48cd0c: e0811201     	add	r1, r1, r1, lsl #4
  48cd10: e0811401     	add	r1, r1, r1, lsl #8
  48cd14: e0811801     	add	r1, r1, r1, lsl #16
  48cd18: e0831081     	add	r1, r3, r1, lsl #1
  48cd1c: e0010192     	mul	r1, r2, r1
  48cd20: e3510080     	cmp	r1, #128
  48cd24: 8a00000b     	bhi	0x48cd58 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x298> @ imm = #0x2c
  48cd28: eb09f074     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27c1d0
  48cd2c: e59d303c     	ldr	r3, [sp, #0x3c]
  48cd30: e59d1008     	ldr	r1, [sp, #0x8]
  48cd34: e3a02018     	mov	r2, #24
  48cd38: e5847008     	str	r7, [r4, #0x8]
  48cd3c: e0231392     	mla	r3, r2, r3, r1
  48cd40: e5841004     	str	r1, [r4, #0x4]
  48cd44: e584300c     	str	r3, [r4, #0xc]
  48cd48: eaffff91     	b	0x48cb94 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0xd4> @ imm = #-0x1bc
  48cd4c: e1520003     	cmp	r2, r3
  48cd50: 9affffa6     	bls	0x48cbf0 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x130> @ imm = #-0x168
  48cd54: eaffffa3     	b	0x48cbe8 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0x128> @ imm = #-0x174
  48cd58: ebfa0db8     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17c920
  48cd5c: e59d303c     	ldr	r3, [sp, #0x3c]
  48cd60: e59d1008     	ldr	r1, [sp, #0x8]
  48cd64: e3a02018     	mov	r2, #24
  48cd68: e5847008     	str	r7, [r4, #0x8]
  48cd6c: e0231392     	mla	r3, r2, r3, r1
  48cd70: e5841004     	str	r1, [r4, #0x4]
  48cd74: e584300c     	str	r3, [r4, #0xc]
  48cd78: eaffff85     	b	0x48cb94 <rnd::RoomPool::LoadFromXml(TiXmlNode*)+0xd4> @ imm = #-0x1ec
  48cd7c: e4 8b 43 00  	.word	0x00438be4
  48cd80: 0c 83 44 00  	.word	0x0044830c
