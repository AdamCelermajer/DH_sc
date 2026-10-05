
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048e184 <rnd::RandomGenerator::GetBlock(char const*) const>:
  48e184: e92d40f0     	push	{r4, r5, r6, r7, lr}
  48e188: e59f40d8     	ldr	r4, [pc, #0xd8]         @ 0x48e268 <rnd::RandomGenerator::GetBlock(char const*) const+0xe4>
  48e18c: e59f50d8     	ldr	r5, [pc, #0xd8]         @ 0x48e26c <rnd::RandomGenerator::GetBlock(char const*) const+0xe8>
  48e190: e24dd02c     	sub	sp, sp, #44
  48e194: e08f4004     	add	r4, pc, r4
  48e198: e7943005     	ldr	r3, [r4, r5]
  48e19c: e28d600c     	add	r6, sp, #12
  48e1a0: e28d2008     	add	r2, sp, #8
  48e1a4: e5933000     	ldr	r3, [r3]
  48e1a8: e1a07000     	mov	r7, r0
  48e1ac: e1a00006     	mov	r0, r6
  48e1b0: e58d3024     	str	r3, [sp, #0x24]
  48e1b4: ebfa17cc     	bl	0x3140ec <std::basic_string<char, std::char_traits<char>, std::allocator<char>>::basic_string(char const*, std::allocator<char> const&)> @ imm = #-0x17a0d0
  48e1b8: e59d2020     	ldr	r2, [sp, #0x20]
  48e1bc: e59d001c     	ldr	r0, [sp, #0x1c]
  48e1c0: e1520000     	cmp	r2, r0
  48e1c4: 0a00000a     	beq	0x48e1f4 <rnd::RandomGenerator::GetBlock(char const*) const+0x70> @ imm = #0x28
  48e1c8: e59fc0a0     	ldr	r12, [pc, #0xa0]        @ 0x48e270 <rnd::RandomGenerator::GetBlock(char const*) const+0xec>
  48e1cc: e1d230d0     	ldrsb	r3, [r2]
  48e1d0: e35300ff     	cmp	r3, #255
  48e1d4: 9794100c     	ldrls	r1, [r4, r12]
  48e1d8: 95911000     	ldrls	r1, [r1]
  48e1dc: 90813083     	addls	r3, r1, r3, lsl #1
  48e1e0: 91d330f2     	ldrshls	r3, [r3, #2]
  48e1e4: e4c23001     	strb	r3, [r2], #1
  48e1e8: e1520000     	cmp	r2, r0
  48e1ec: 1afffff6     	bne	0x48e1cc <rnd::RandomGenerator::GetBlock(char const*) const+0x48> @ imm = #-0x28
  48e1f0: e59d0020     	ldr	r0, [sp, #0x20]
  48e1f4: e287703c     	add	r7, r7, #60
  48e1f8: e28d1028     	add	r1, sp, #40
  48e1fc: e5210024     	str	r0, [r1, #-0x24]!
  48e200: e1a00007     	mov	r0, r7
  48e204: ebfff883     	bl	0x48c418 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_find<char const*>(char const* const&) const> @ imm = #-0x1df4
  48e208: e1500007     	cmp	r0, r7
  48e20c: 15907014     	ldrne	r7, [r0, #0x14]
  48e210: e59d0020     	ldr	r0, [sp, #0x20]
  48e214: 03a07000     	moveq	r7, #0
  48e218: e1500006     	cmp	r0, r6
  48e21c: 0a000006     	beq	0x48e23c <rnd::RandomGenerator::GetBlock(char const*) const+0xb8> @ imm = #0x18
  48e220: e3500000     	cmp	r0, #0
  48e224: 0a000004     	beq	0x48e23c <rnd::RandomGenerator::GetBlock(char const*) const+0xb8> @ imm = #0x10
  48e228: e59d100c     	ldr	r1, [sp, #0xc]
  48e22c: e0601001     	rsb	r1, r0, r1
  48e230: e3510080     	cmp	r1, #128
  48e234: 8a000008     	bhi	0x48e25c <rnd::RandomGenerator::GetBlock(char const*) const+0xd8> @ imm = #0x20
  48e238: eb09eb30     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x27acc0
  48e23c: e7943005     	ldr	r3, [r4, r5]
  48e240: e59d2024     	ldr	r2, [sp, #0x24]
  48e244: e1a00007     	mov	r0, r7
  48e248: e5933000     	ldr	r3, [r3]
  48e24c: e1520003     	cmp	r2, r3
  48e250: 1a000003     	bne	0x48e264 <rnd::RandomGenerator::GetBlock(char const*) const+0xe0> @ imm = #0xc
  48e254: e28dd02c     	add	sp, sp, #44
  48e258: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  48e25c: ebfa0877     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17de24
  48e260: eafffff5     	b	0x48e23c <rnd::RandomGenerator::GetBlock(char const*) const+0xb8> @ imm = #-0x2c
  48e264: ebfa0029     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x17ff5c
  48e268: fc 68 50 00  	.word	0x005068fc
  48e26c: ac 40 00 00  	.word	0x000040ac
  48e270: e0 36 00 00  	.word	0x000036e0
