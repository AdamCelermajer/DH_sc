
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048a5d0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)>:
  48a5d0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  48a5d4: e59fa318     	ldr	r10, [pc, #0x318]       @ 0x48a8f4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x324>
  48a5d8: e59fc318     	ldr	r12, [pc, #0x318]       @ 0x48a8f8 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x328>
  48a5dc: e24dd09c     	sub	sp, sp, #156
  48a5e0: e08fa00a     	add	r10, pc, r10
  48a5e4: e58dc014     	str	r12, [sp, #0x14]
  48a5e8: e79ac00c     	ldr	r12, [r10, r12]
  48a5ec: e58d2020     	str	r2, [sp, #0x20]
  48a5f0: e58d0018     	str	r0, [sp, #0x18]
  48a5f4: e59c2000     	ldr	r2, [r12]
  48a5f8: e1a00001     	mov	r0, r1
  48a5fc: e58d3024     	str	r3, [sp, #0x24]
  48a600: e58d2094     	str	r2, [sp, #0x94]
  48a604: ebffe50f     	bl	0x483a48 <TiXmlHandle::ToElement() const> @ imm = #-0x6bc4
  48a608: e59f12ec     	ldr	r1, [pc, #0x2ec]        @ 0x48a8fc <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x32c>
  48a60c: e58d001c     	str	r0, [sp, #0x1c]
  48a610: e28d407c     	add	r4, sp, #124
  48a614: e08f1001     	add	r1, pc, r1
  48a618: eb022994     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8a650
  48a61c: e3a01010     	mov	r1, #16
  48a620: e1a05000     	mov	r5, r0
  48a624: e1a00004     	mov	r0, r4
  48a628: e58d408c     	str	r4, [sp, #0x8c]
  48a62c: e58d4090     	str	r4, [sp, #0x90]
  48a630: ebfa1c11     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x178fbc
  48a634: e59d208c     	ldr	r2, [sp, #0x8c]
  48a638: e3a03000     	mov	r3, #0
  48a63c: e3550000     	cmp	r5, #0
  48a640: e5c23000     	strb	r3, [r2]
  48a644: 028d8038     	addeq	r8, sp, #56
  48a648: e58d3040     	str	r3, [sp, #0x40]
  48a64c: e58d3038     	str	r3, [sp, #0x38]
  48a650: e58d303c     	str	r3, [sp, #0x3c]
  48a654: 0a000056     	beq	0x48a7b4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x1e4> @ imm = #0x158
  48a658: e1a00005     	mov	r0, r5
  48a65c: ebfa0dfc     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x17c810
  48a660: e1a01005     	mov	r1, r5
  48a664: e0852000     	add	r2, r5, r0
  48a668: e1a00004     	mov	r0, r4
  48a66c: ebfa18db     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x179c94
  48a670: e59d0090     	ldr	r0, [sp, #0x90]
  48a674: e59d508c     	ldr	r5, [sp, #0x8c]
  48a678: e3a01020     	mov	r1, #32
  48a67c: e0602005     	rsb	r2, r0, r5
  48a680: ebfa0fcd     	bl	0x30e5bc <.plt+0x848>   @ imm = #-0x17c0cc
  48a684: e3500000     	cmp	r0, #0
  48a688: 0a00000c     	beq	0x48a6c0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0xf0> @ imm = #0x30
  48a68c: e1550000     	cmp	r5, r0
  48a690: 0a00000a     	beq	0x48a6c0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0xf0> @ imm = #0x28
  48a694: e2803001     	add	r3, r0, #1
  48a698: e1550003     	cmp	r5, r3
  48a69c: 0a000007     	beq	0x48a6c0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0xf0> @ imm = #0x1c
  48a6a0: e2455001     	sub	r5, r5, #1
  48a6a4: e1a03000     	mov	r3, r0
  48a6a8: e5d32001     	ldrb	r2, [r3, #0x1]
  48a6ac: e2833001     	add	r3, r3, #1
  48a6b0: e3520020     	cmp	r2, #32
  48a6b4: 14c02001     	strbne	r2, [r0], #1
  48a6b8: e1530005     	cmp	r3, r5
  48a6bc: 1afffff9     	bne	0x48a6a8 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0xd8> @ imm = #-0x1c
  48a6c0: e1a00004     	mov	r0, r4
  48a6c4: ebfffe12     	bl	0x489f14 <std::string::find(char const*, unsigned int, unsigned int) const (.clone.1)> @ imm = #-0x7b8
  48a6c8: e3700001     	cmn	r0, #1
  48a6cc: e1a05000     	mov	r5, r0
  48a6d0: 028d8038     	addeq	r8, sp, #56
  48a6d4: 0a000033     	beq	0x48a7a8 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x1d8> @ imm = #0xcc
  48a6d8: e28d6064     	add	r6, sp, #100
  48a6dc: e28d9048     	add	r9, sp, #72
  48a6e0: e28d8038     	add	r8, sp, #56
  48a6e4: e28d704c     	add	r7, sp, #76
  48a6e8: e28db044     	add	r11, sp, #68
  48a6ec: ea000019     	b	0x48a758 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x188> @ imm = #0x64
  48a6f0: eb09fa02     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27e808
  48a6f4: e2852001     	add	r2, r5, #1
  48a6f8: e1a01004     	mov	r1, r4
  48a6fc: e3e03000     	mvn	r3, #0
  48a700: e1a00007     	mov	r0, r7
  48a704: e58db000     	str	r11, [sp]
  48a708: ebfe2d72     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x74a38
  48a70c: e1a00004     	mov	r0, r4
  48a710: e59d1060     	ldr	r1, [sp, #0x60]
  48a714: e59d205c     	ldr	r2, [sp, #0x5c]
  48a718: ebfa18b0     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x179d40
  48a71c: e59d0060     	ldr	r0, [sp, #0x60]
  48a720: e1500007     	cmp	r0, r7
  48a724: 0a000006     	beq	0x48a744 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x174> @ imm = #0x18
  48a728: e3500000     	cmp	r0, #0
  48a72c: 0a000004     	beq	0x48a744 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x174> @ imm = #0x10
  48a730: e59d104c     	ldr	r1, [sp, #0x4c]
  48a734: e0601001     	rsb	r1, r0, r1
  48a738: e3510080     	cmp	r1, #128
  48a73c: 8a000065     	bhi	0x48a8d8 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x308> @ imm = #0x194
  48a740: eb09f9ee     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27e7b8
  48a744: e1a00004     	mov	r0, r4
  48a748: ebfffdf1     	bl	0x489f14 <std::string::find(char const*, unsigned int, unsigned int) const (.clone.1)> @ imm = #-0x83c
  48a74c: e3700001     	cmn	r0, #1
  48a750: e1a05000     	mov	r5, r0
  48a754: 0a000013     	beq	0x48a7a8 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x1d8> @ imm = #0x4c
  48a758: e3a02000     	mov	r2, #0
  48a75c: e1a03005     	mov	r3, r5
  48a760: e1a01004     	mov	r1, r4
  48a764: e1a00006     	mov	r0, r6
  48a768: e58d9000     	str	r9, [sp]
  48a76c: ebfe2d59     	bl	0x415cd8 <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(std::string const&, unsigned int, unsigned int, std::allocator<char> const&)> @ imm = #-0x74a9c
  48a770: e1a00008     	mov	r0, r8
  48a774: e1a01006     	mov	r1, r6
  48a778: ebfa84d6     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x15eca8
  48a77c: e59d0078     	ldr	r0, [sp, #0x78]
  48a780: e1500006     	cmp	r0, r6
  48a784: 0affffda     	beq	0x48a6f4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x124> @ imm = #-0x98
  48a788: e3500000     	cmp	r0, #0
  48a78c: 0affffd8     	beq	0x48a6f4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x124> @ imm = #-0xa0
  48a790: e59d1064     	ldr	r1, [sp, #0x64]
  48a794: e0601001     	rsb	r1, r0, r1
  48a798: e3510080     	cmp	r1, #128
  48a79c: 9affffd3     	bls	0x48a6f0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x120> @ imm = #-0xb4
  48a7a0: ebfa1726     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17a368
  48a7a4: eaffffd2     	b	0x48a6f4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x124> @ imm = #-0xb8
  48a7a8: e1a00008     	mov	r0, r8
  48a7ac: e1a01004     	mov	r1, r4
  48a7b0: ebfa84c8     	bl	0x32bad8 <std::vector<std::string, std::allocator<std::string>>::push_back(std::string const&)> @ imm = #-0x15ece0
  48a7b4: e59f1144     	ldr	r1, [pc, #0x144]        @ 0x48a900 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x330>
  48a7b8: e59d001c     	ldr	r0, [sp, #0x1c]
  48a7bc: e08f1001     	add	r1, pc, r1
  48a7c0: eb02292a     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8a4a8
  48a7c4: e2507000     	subs	r7, r0, #0
  48a7c8: 0a000044     	beq	0x48a8e0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x310> @ imm = #0x110
  48a7cc: e59d0090     	ldr	r0, [sp, #0x90]
  48a7d0: ebfffcb3     	bl	0x489aa4 <rnd::TryIsZero(char const*)> @ imm = #-0xd34
  48a7d4: e2506000     	subs	r6, r0, #0
  48a7d8: 1a000040     	bne	0x48a8e0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x310> @ imm = #0x100
  48a7dc: e59fb120     	ldr	r11, [pc, #0x120]       @ 0x48a904 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x334>
  48a7e0: e79a900b     	ldr	r9, [r10, r11]
  48a7e4: e1a05206     	lsl	r5, r6, #4
  48a7e8: e0893005     	add	r3, r9, r5
  48a7ec: e5931004     	ldr	r1, [r3, #0x4]
  48a7f0: e1a00007     	mov	r0, r7
  48a7f4: ebfa0fbb     	bl	0x30e6e8 <.plt+0x974>   @ imm = #-0x17c114
  48a7f8: e3500000     	cmp	r0, #0
  48a7fc: 0a000003     	beq	0x48a810 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x240> @ imm = #0xc
  48a800: e2866001     	add	r6, r6, #1
  48a804: e3560004     	cmp	r6, #4
  48a808: 1afffff5     	bne	0x48a7e4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x214> @ imm = #-0x2c
  48a80c: e3a05040     	mov	r5, #64
  48a810: e59f10f0     	ldr	r1, [pc, #0xf0]         @ 0x48a908 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x338>
  48a814: e3a03000     	mov	r3, #0
  48a818: e28d602c     	add	r6, sp, #44
  48a81c: e08f1001     	add	r1, pc, r1
  48a820: e59d001c     	ldr	r0, [sp, #0x1c]
  48a824: e58d3034     	str	r3, [sp, #0x34]
  48a828: e58d302c     	str	r3, [sp, #0x2c]
  48a82c: e58d3030     	str	r3, [sp, #0x30]
  48a830: eb02290e     	bl	0x514c70 <TiXmlElement::Attribute(char const*) const> @ imm = #0x8a438
  48a834: e1a01006     	mov	r1, r6
  48a838: ebfa1267     	bl	0x30f1dc <StrToObj(char const*, Point3D<float>&)> @ imm = #-0x17b664
  48a83c: e59d0018     	ldr	r0, [sp, #0x18]
  48a840: e1a01006     	mov	r1, r6
  48a844: e59d2020     	ldr	r2, [sp, #0x20]
  48a848: ebfffd63     	bl	0x489ddc <rnd::Exit::GetBlockUnitPosition(Point3D<float>&, rnd::Block*)> @ imm = #-0xa74
  48a84c: e59d0018     	ldr	r0, [sp, #0x18]
  48a850: e79a100b     	ldr	r1, [r10, r11]
  48a854: e59d2020     	ldr	r2, [sp, #0x20]
  48a858: e590c00c     	ldr	r12, [r0, #0xc]
  48a85c: e5903008     	ldr	r3, [r0, #0x8]
  48a860: e0811005     	add	r1, r1, r5
  48a864: e58dc000     	str	r12, [sp]
  48a868: e59dc034     	ldr	r12, [sp, #0x34]
  48a86c: e58d800c     	str	r8, [sp, #0xc]
  48a870: e3a05001     	mov	r5, #1
  48a874: e58dc004     	str	r12, [sp, #0x4]
  48a878: e59dc024     	ldr	r12, [sp, #0x24]
  48a87c: e58dc008     	str	r12, [sp, #0x8]
  48a880: ebfffebe     	bl	0x48a380 <rnd::Exit::Exit(rnd::Direction const&, rnd::Block*, int, int, float, int, std::vector<std::string, std::allocator<std::string>>&)> @ imm = #-0x508
  48a884: e1a00008     	mov	r0, r8
  48a888: ebfa25a8     	bl	0x313f30 <std::vector<std::string, std::allocator<std::string>>::~vector()> @ imm = #-0x176960
  48a88c: e59d0090     	ldr	r0, [sp, #0x90]
  48a890: e1500004     	cmp	r0, r4
  48a894: 0a000006     	beq	0x48a8b4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x2e4> @ imm = #0x18
  48a898: e3500000     	cmp	r0, #0
  48a89c: 0a000004     	beq	0x48a8b4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x2e4> @ imm = #0x10
  48a8a0: e59d107c     	ldr	r1, [sp, #0x7c]
  48a8a4: e0601001     	rsb	r1, r0, r1
  48a8a8: e3510080     	cmp	r1, #128
  48a8ac: 8a00000d     	bhi	0x48a8e8 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x318> @ imm = #0x34
  48a8b0: eb09f992     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27e648
  48a8b4: e59d0014     	ldr	r0, [sp, #0x14]
  48a8b8: e59d2094     	ldr	r2, [sp, #0x94]
  48a8bc: e79a3000     	ldr	r3, [r10, r0]
  48a8c0: e1a00005     	mov	r0, r5
  48a8c4: e5933000     	ldr	r3, [r3]
  48a8c8: e1520003     	cmp	r2, r3
  48a8cc: 1a000007     	bne	0x48a8f0 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x320> @ imm = #0x1c
  48a8d0: e28dd09c     	add	sp, sp, #156
  48a8d4: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  48a8d8: ebfa16d8     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17a4a0
  48a8dc: eaffff98     	b	0x48a744 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x174> @ imm = #-0x1a0
  48a8e0: e3a05000     	mov	r5, #0
  48a8e4: eaffffe6     	b	0x48a884 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x2b4> @ imm = #-0x68
  48a8e8: ebfa16d4     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17a4b0
  48a8ec: eafffff0     	b	0x48a8b4 <rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)+0x2e4> @ imm = #-0x40
  48a8f0: ebfa0e86     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17c5e8
  48a8f4: b0 a4 50 00  	.word	0x0050a4b0
  48a8f8: ac 40 00 00  	.word	0x000040ac
  48a8fc: e4 a6 44 00  	.word	0x0044a6e4
  48a900: 4c a5 44 00  	.word	0x0044a54c
  48a904: fc 43 00 00  	.word	0x000043fc
  48a908: 0c 7a 43 00  	.word	0x00437a0c
