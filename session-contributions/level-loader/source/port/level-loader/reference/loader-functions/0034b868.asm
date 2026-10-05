
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034b868 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)>:
  34b868: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34b86c: e59f4338     	ldr	r4, [pc, #0x338]        @ 0x34bbac <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x344>
  34b870: e59f5338     	ldr	r5, [pc, #0x338]        @ 0x34bbb0 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x348>
  34b874: e2517000     	subs	r7, r1, #0
  34b878: e08f4004     	add	r4, pc, r4
  34b87c: e7941005     	ldr	r1, [r4, r5]
  34b880: e1a0a002     	mov	r10, r2
  34b884: e24ddf97     	sub	sp, sp, #604
  34b888: e5912000     	ldr	r2, [r1]
  34b88c: e1a0b000     	mov	r11, r0
  34b890: e58d300c     	str	r3, [sp, #0xc]
  34b894: e58d2254     	str	r2, [sp, #0x254]
  34b898: 0a0000a4     	beq	0x34bb30 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x2c8> @ imm = #0x290
  34b89c: e59f1310     	ldr	r1, [pc, #0x310]        @ 0x34bbb4 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x34c>
  34b8a0: e1a00007     	mov	r0, r7
  34b8a4: e08f1001     	add	r1, pc, r1
  34b8a8: eb0724f0     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x1c93c0
  34b8ac: e59f1304     	ldr	r1, [pc, #0x304]        @ 0x34bbb8 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x350>
  34b8b0: e1a08000     	mov	r8, r0
  34b8b4: e1a00007     	mov	r0, r7
  34b8b8: e08f1001     	add	r1, pc, r1
  34b8bc: eb0724eb     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x1c93ac
  34b8c0: e2509000     	subs	r9, r0, #0
  34b8c4: 0a000084     	beq	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #0x210
  34b8c8: e3580000     	cmp	r8, #0
  34b8cc: 0a000082     	beq	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #0x208
  34b8d0: e28d602c     	add	r6, sp, #44
  34b8d4: e1a00006     	mov	r0, r6
  34b8d8: ebffcf0b     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0xc3d4
  34b8dc: e35a0000     	cmp	r10, #0
  34b8e0: 0a000004     	beq	0x34b8f8 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x90> @ imm = #0x10
  34b8e4: e1a0000a     	mov	r0, r10
  34b8e8: e1a01008     	mov	r1, r8
  34b8ec: ebff0a8a     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x3d5d8
  34b8f0: e3500000     	cmp	r0, #0
  34b8f4: 1a000078     	bne	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #0x1e0
  34b8f8: e59f12bc     	ldr	r1, [pc, #0x2bc]        @ 0x34bbbc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x354>
  34b8fc: e1a00009     	mov	r0, r9
  34b900: e08f1001     	add	r1, pc, r1
  34b904: ebff0a84     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x3d5f0
  34b908: e3500000     	cmp	r0, #0
  34b90c: 0a000079     	beq	0x34baf8 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x290> @ imm = #0x1e4
  34b910: e28daf4f     	add	r10, sp, #316
  34b914: e1a01009     	mov	r1, r9
  34b918: e1a0000a     	mov	r0, r10
  34b91c: ebff0c70     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x3ce40
  34b920: e59f1298     	ldr	r1, [pc, #0x298]        @ 0x34bbc0 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x358>
  34b924: e1a0200a     	mov	r2, r10
  34b928: e28d003c     	add	r0, sp, #60
  34b92c: e08f1001     	add	r1, pc, r1
  34b930: e3a03053     	mov	r3, #83
  34b934: ebff0c6a     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x3ce58
  34b938: e59dc280     	ldr	r12, [sp, #0x280]
  34b93c: e28da010     	add	r10, sp, #16
  34b940: e1a03009     	mov	r3, r9
  34b944: e1a0100b     	mov	r1, r11
  34b948: e1a0000a     	mov	r0, r10
  34b94c: e1a02008     	mov	r2, r8
  34b950: e3a09001     	mov	r9, #1
  34b954: e58dc000     	str	r12, [sp]
  34b958: e58d9004     	str	r9, [sp, #0x4]
  34b95c: ebfffeef     	bl	0x34b520 <ObjectManager::GetNewObject(char const*, char const*, int, bool)> @ imm = #-0x444
  34b960: e59d1014     	ldr	r1, [sp, #0x14]
  34b964: e2863004     	add	r3, r6, #4
  34b968: e59d2010     	ldr	r2, [sp, #0x10]
  34b96c: e4831004     	str	r1, [r3], #4
  34b970: e59ac008     	ldr	r12, [r10, #0x8]
  34b974: e28d602c     	add	r6, sp, #44
  34b978: e1a00006     	mov	r0, r6
  34b97c: e3a01000     	mov	r1, #0
  34b980: e583c000     	str	r12, [r3]
  34b984: e58d202c     	str	r2, [sp, #0x2c]
  34b988: ebffd10c     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbbd0
  34b98c: e3500000     	cmp	r0, #0
  34b990: 0a000051     	beq	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #0x144
  34b994: e1a01009     	mov	r1, r9
  34b998: e1a00006     	mov	r0, r6
  34b99c: ebffd107     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbbe4
  34b9a0: e2800004     	add	r0, r0, #4
  34b9a4: eb0720f3     	bl	0x513d78 <PropertyMap::InitProperties()> @ imm = #0x1c83cc
  34b9a8: e59f1214     	ldr	r1, [pc, #0x214]        @ 0x34bbc4 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x35c>
  34b9ac: e1a00007     	mov	r0, r7
  34b9b0: e08f1001     	add	r1, pc, r1
  34b9b4: eb0724ad     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x1c92b4
  34b9b8: e250b000     	subs	r11, r0, #0
  34b9bc: 0a000015     	beq	0x34ba18 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x1b0> @ imm = #0x54
  34b9c0: e1a01009     	mov	r1, r9
  34b9c4: e1a00006     	mov	r0, r6
  34b9c8: ebffd0fc     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbc10
  34b9cc: e28d9f8f     	add	r9, sp, #572
  34b9d0: e280a004     	add	r10, r0, #4
  34b9d4: e1a0100b     	mov	r1, r11
  34b9d8: e28d2038     	add	r2, sp, #56
  34b9dc: e1a00009     	mov	r0, r9
  34b9e0: ebff21c1     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x378fc
  34b9e4: e1a0000a     	mov	r0, r10
  34b9e8: e1a01009     	mov	r1, r9
  34b9ec: eb07217e     	bl	0x513fec <PropertyMap::SetTemplate(std::string const&)> @ imm = #0x1c85f8
  34b9f0: e59d0250     	ldr	r0, [sp, #0x250]
  34b9f4: e1500009     	cmp	r0, r9
  34b9f8: 0a000006     	beq	0x34ba18 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x1b0> @ imm = #0x18
  34b9fc: e3500000     	cmp	r0, #0
  34ba00: 0a000004     	beq	0x34ba18 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x1b0> @ imm = #0x10
  34ba04: e59d123c     	ldr	r1, [sp, #0x23c]
  34ba08: e0601001     	rsb	r1, r0, r1
  34ba0c: e3510080     	cmp	r1, #128
  34ba10: 8a000062     	bhi	0x34bba0 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x338> @ imm = #0x188
  34ba14: eb0ef539     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x3bd4e4
  34ba18: e3a01001     	mov	r1, #1
  34ba1c: e1a00006     	mov	r0, r6
  34ba20: ebffd0e6     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbc68
  34ba24: e2800004     	add	r0, r0, #4
  34ba28: eb071f2f     	bl	0x5136ec <PropertyMap::LoadDefaultProperties()> @ imm = #0x1c7cbc
  34ba2c: e3a01001     	mov	r1, #1
  34ba30: e1a00006     	mov	r0, r6
  34ba34: ebffd0e1     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbc7c
  34ba38: e1a01007     	mov	r1, r7
  34ba3c: e2800004     	add	r0, r0, #4
  34ba40: eb071fee     	bl	0x513a00 <PropertyMap::LoadOverridesFromXML(TiXmlElement*)> @ imm = #0x1c7fb8
  34ba44: e59f117c     	ldr	r1, [pc, #0x17c]        @ 0x34bbc8 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x360>
  34ba48: e1a00008     	mov	r0, r8
  34ba4c: e08f1001     	add	r1, pc, r1
  34ba50: ebff0a31     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x3d73c
  34ba54: e3500000     	cmp	r0, #0
  34ba58: 0a000049     	beq	0x34bb84 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x31c> @ imm = #0x124
  34ba5c: e3a01001     	mov	r1, #1
  34ba60: e1a00006     	mov	r0, r6
  34ba64: ebffd0d5     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbcac
  34ba68: e5903000     	ldr	r3, [r0]
  34ba6c: e1a0e00f     	mov	lr, pc
  34ba70: e593f020     	ldr	pc, [r3, #0x20]
  34ba74: e3500000     	cmp	r0, #0
  34ba78: 0a000017     	beq	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #0x5c
  34ba7c: e1a00006     	mov	r0, r6
  34ba80: ebffd117     	bl	0x33fee4 <ObjectHandle::operator GameObject*()> @ imm = #-0xbba4
  34ba84: e59d300c     	ldr	r3, [sp, #0xc]
  34ba88: e1a08000     	mov	r8, r0
  34ba8c: e5900164     	ldr	r0, [r0, #0x164]
  34ba90: e5931004     	ldr	r1, [r3, #0x4]
  34ba94: ebff0c42     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x3cef8
  34ba98: e59dc00c     	ldr	r12, [sp, #0xc]
  34ba9c: e1a07000     	mov	r7, r0
  34baa0: e5980168     	ldr	r0, [r8, #0x168]
  34baa4: e59c1008     	ldr	r1, [r12, #0x8]
  34baa8: ebff0c3d     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x3cf0c
  34baac: e59d300c     	ldr	r3, [sp, #0xc]
  34bab0: e1a06000     	mov	r6, r0
  34bab4: e5980160     	ldr	r0, [r8, #0x160]
  34bab8: e5931000     	ldr	r1, [r3]
  34babc: ebff0c38     	bl	0x30eba4 <.plt+0xe30>   @ imm = #-0x3cf20
  34bac0: e28d1020     	add	r1, sp, #32
  34bac4: e58d0020     	str	r0, [sp, #0x20]
  34bac8: e3a02001     	mov	r2, #1
  34bacc: e1a00008     	mov	r0, r8
  34bad0: e58d7024     	str	r7, [sp, #0x24]
  34bad4: e58d6028     	str	r6, [sp, #0x28]
  34bad8: eb0120b5     	bl	0x393db4 <GameObject::SetPosition(Point3D<float> const&, bool)> @ imm = #0x482d4
  34badc: e7943005     	ldr	r3, [r4, r5]
  34bae0: e59d2254     	ldr	r2, [sp, #0x254]
  34bae4: e5933000     	ldr	r3, [r3]
  34bae8: e1520003     	cmp	r2, r3
  34baec: 1a00002d     	bne	0x34bba8 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x340> @ imm = #0xb4
  34baf0: e28ddf97     	add	sp, sp, #604
  34baf4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34baf8: e28daf4f     	add	r10, sp, #316
  34bafc: e1a01009     	mov	r1, r9
  34bb00: e1a0000a     	mov	r0, r10
  34bb04: ebff0bf6     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x3d028
  34bb08: e59f10bc     	ldr	r1, [pc, #0xbc]         @ 0x34bbcc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x364>
  34bb0c: e28d903c     	add	r9, sp, #60
  34bb10: e1a0200a     	mov	r2, r10
  34bb14: e08f1001     	add	r1, pc, r1
  34bb18: e1a00009     	mov	r0, r9
  34bb1c: e3a03053     	mov	r3, #83
  34bb20: ebff0bef     	bl	0x30eae4 <.plt+0xd70>   @ imm = #-0x3d044
  34bb24: e3e0c000     	mvn	r12, #0
  34bb28: e58dc280     	str	r12, [sp, #0x280]
  34bb2c: eaffff81     	b	0x34b938 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0xd0> @ imm = #-0x1fc
  34bb30: e59f3098     	ldr	r3, [pc, #0x98]         @ 0x34bbd0 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x368>
  34bb34: e7943003     	ldr	r3, [r4, r3]
  34bb38: e5933000     	ldr	r3, [r3]
  34bb3c: e3530002     	cmp	r3, #2
  34bb40: 05877000     	streq	r7, [r7]
  34bb44: 0affffe4     	beq	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #-0x70
  34bb48: e3530001     	cmp	r3, #1
  34bb4c: 1affffe2     	bne	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #-0x78
  34bb50: e59f007c     	ldr	r0, [pc, #0x7c]         @ 0x34bbd4 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x36c>
  34bb54: e59f107c     	ldr	r1, [pc, #0x7c]         @ 0x34bbd8 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x370>
  34bb58: e59f207c     	ldr	r2, [pc, #0x7c]         @ 0x34bbdc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x374>
  34bb5c: e7940000     	ldr	r0, [r4, r0]
  34bb60: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x34bbe0 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x378>
  34bb64: e3a0cf83     	mov	r12, #524
  34bb68: e08f1001     	add	r1, pc, r1
  34bb6c: e08f2002     	add	r2, pc, r2
  34bb70: e08f3003     	add	r3, pc, r3
  34bb74: e28000a8     	add	r0, r0, #168
  34bb78: e58dc000     	str	r12, [sp]
  34bb7c: ebff0920     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3db80
  34bb80: eaffffd5     	b	0x34badc <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x274> @ imm = #-0xac
  34bb84: e1a00006     	mov	r0, r6
  34bb88: e3a01001     	mov	r1, #1
  34bb8c: ebffd08b     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbdd4
  34bb90: e5903000     	ldr	r3, [r0]
  34bb94: e1a0e00f     	mov	lr, pc
  34bb98: e593f01c     	ldr	pc, [r3, #0x1c]
  34bb9c: eaffffae     	b	0x34ba5c <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x1f4> @ imm = #-0x148
  34bba0: ebff1226     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x3b768
  34bba4: eaffff9b     	b	0x34ba18 <ObjectManager::LoadFromXML(TiXmlElement*, char const*, Point3D<float> const&, int)+0x1b0> @ imm = #-0x194
  34bba8: ebff09d8     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x3d8a0
  34bbac: 18 92 64 00  	.word	0x00649218
  34bbb0: ac 40 00 00  	.word	0x000040ac
  34bbb4: d4 48 57 00  	.word	0x005748d4
  34bbb8: 30 58 59 00  	.word	0x00595830
  34bbbc: 40 4b 57 00  	.word	0x00574b40
  34bbc0: 2c 4b 57 00  	.word	0x00574b2c
  34bbc4: b0 4a 57 00  	.word	0x00574ab0
  34bbc8: 24 4a 57 00  	.word	0x00574a24
  34bbcc: 44 49 57 00  	.word	0x00574944
  34bbd0: c0 39 00 00  	.word	0x000039c0
  34bbd4: c0 19 00 00  	.word	0x000019c0
  34bbd8: 70 28 57 00  	.word	0x00572870
  34bbdc: cc 48 57 00  	.word	0x005748cc
  34bbe0: 28 47 57 00  	.word	0x00574728
