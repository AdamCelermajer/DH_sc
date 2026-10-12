
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056e6dc <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)>:
  56e6dc: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  56e6e0: e59fc438     	ldr	r12, [pc, #0x438]       @ 0x56eb20 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x444>
  56e6e4: e59fe438     	ldr	lr, [pc, #0x438]        @ 0x56eb24 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x448>
  56e6e8: e24dd0ec     	sub	sp, sp, #236
  56e6ec: e08fc00c     	add	r12, pc, r12
  56e6f0: e58d3018     	str	r3, [sp, #0x18]
  56e6f4: e79c300e     	ldr	r3, [r12, lr]
  56e6f8: e58dc00c     	str	r12, [sp, #0xc]
  56e6fc: e58de030     	str	lr, [sp, #0x30]
  56e700: e5933000     	ldr	r3, [r3]
  56e704: e58d2008     	str	r2, [sp, #0x8]
  56e708: e58d30e4     	str	r3, [sp, #0xe4]
  56e70c: e5903000     	ldr	r3, [r0]
  56e710: e1a0e00f     	mov	lr, pc
  56e714: e593f00c     	ldr	pc, [r3, #0xc]
  56e718: e5903000     	ldr	r3, [r0]
  56e71c: e1a05000     	mov	r5, r0
  56e720: e1a0e00f     	mov	lr, pc
  56e724: e593f020     	ldr	pc, [r3, #0x20]
  56e728: e3a01000     	mov	r1, #0
  56e72c: e1a04000     	mov	r4, r0
  56e730: e2800001     	add	r0, r0, #1
  56e734: ebff169b     	bl	0x5341a8 <operator new[](unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0x3a594
  56e738: e5953000     	ldr	r3, [r5]
  56e73c: e1a01000     	mov	r1, r0
  56e740: e1a07000     	mov	r7, r0
  56e744: e1a02004     	mov	r2, r4
  56e748: e1a00005     	mov	r0, r5
  56e74c: e1a0e00f     	mov	lr, pc
  56e750: e593f00c     	ldr	pc, [r3, #0xc]
  56e754: e1a00005     	mov	r0, r5
  56e758: ebf6bb89     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x2511dc
  56e75c: e3a05000     	mov	r5, #0
  56e760: e3540000     	cmp	r4, #0
  56e764: e7c75004     	strb	r5, [r7, r4]
  56e768: 0a000033     	beq	0x56e83c <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x160> @ imm = #0xcc
  56e76c: e30abaab     	movw	r11, #0xaaab
  56e770: e3059556     	movw	r9, #0x5556
  56e774: e5cd5048     	strb	r5, [sp, #0x48]
  56e778: e5cd5049     	strb	r5, [sp, #0x49]
  56e77c: e5cd504a     	strb	r5, [sp, #0x4a]
  56e780: e34abaaa     	movt	r11, #0xaaaa
  56e784: e3459555     	movt	r9, #0x5555
  56e788: e59d8008     	ldr	r8, [sp, #0x8]
  56e78c: ea000000     	b	0x56e794 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0xb8> @ imm = #0x0
  56e790: e0838008     	add	r8, r3, r8
  56e794: e197a0d5     	ldrsb	r10, [r7, r5]
  56e798: e086059b     	umull	r0, r6, r11, r5
  56e79c: e05aa008     	subs	r10, r10, r8
  56e7a0: 43a03c01     	movmi	r3, #256
  56e7a4: 53a03000     	movpl	r3, #0
  56e7a8: e083a00a     	add	r10, r3, r10
  56e7ac: e1a060a6     	lsr	r6, r6, #1
  56e7b0: e08a2faa     	add	r2, r10, r10, lsr #31
  56e7b4: e2020001     	and	r0, r2, #1
  56e7b8: e1a03faa     	lsr	r3, r10, #31
  56e7bc: e0866086     	add	r6, r6, r6, lsl #1
  56e7c0: e0666005     	rsb	r6, r6, r5
  56e7c4: e1a020c2     	asr	r2, r2, #1
  56e7c8: e0633000     	rsb	r3, r3, r0
  56e7cc: e28d10e8     	add	r1, sp, #232
  56e7d0: e0000392     	mul	r0, r2, r3
  56e7d4: e6efa07a     	uxtb	r10, r10
  56e7d8: e0813006     	add	r3, r1, r6
  56e7dc: e543a0a0     	strb	r10, [r3, #-0xa0]
  56e7e0: e59d1008     	ldr	r1, [sp, #0x8]
  56e7e4: ebf68046     	bl	0x30e904 <.plt+0xb90>   @ imm = #-0x25fee8
  56e7e8: e1dd04d9     	ldrsb	r0, [sp, #73]
  56e7ec: e1dd24d8     	ldrsb	r2, [sp, #72]
  56e7f0: e1dd34da     	ldrsb	r3, [sp, #74]
  56e7f4: e2466001     	sub	r6, r6, #1
  56e7f8: e0802002     	add	r2, r0, r2
  56e7fc: e0823003     	add	r3, r2, r3
  56e800: e0c2c399     	smull	r12, r2, r9, r3
  56e804: e7c7a005     	strb	r10, [r7, r5]
  56e808: e0423fc3     	sub	r3, r2, r3, asr #31
  56e80c: e0263196     	mla	r6, r6, r1, r3
  56e810: e2855001     	add	r5, r5, #1
  56e814: e0868008     	add	r8, r6, r8
  56e818: e1a00fc8     	asr	r0, r8, #31
  56e81c: e1a00c20     	lsr	r0, r0, #24
  56e820: e0888000     	add	r8, r8, r0
  56e824: e20880ff     	and	r8, r8, #255
  56e828: e0588000     	subs	r8, r8, r0
  56e82c: 43a03c01     	movmi	r3, #256
  56e830: 53a03000     	movpl	r3, #0
  56e834: e1540005     	cmp	r4, r5
  56e838: 1affffd4     	bne	0x56e790 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0xb4> @ imm = #-0xb0
  56e83c: e59f12e4     	ldr	r1, [pc, #0x2e4]        @ 0x56eb28 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x44c>
  56e840: e2444006     	sub	r4, r4, #6
  56e844: e0874004     	add	r4, r7, r4
  56e848: e08f1001     	add	r1, pc, r1
  56e84c: e1a00004     	mov	r0, r4
  56e850: ebf67eb1     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x26053c
  56e854: e3500000     	cmp	r0, #0
  56e858: 1a0000ab     	bne	0x56eb0c <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x430> @ imm = #0x2ac
  56e85c: e1570004     	cmp	r7, r4
  56e860: 2a00009b     	bhs	0x56ead4 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x3f8> @ imm = #0x26c
  56e864: e59fe2c0     	ldr	lr, [pc, #0x2c0]        @ 0x56eb2c <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x450>
  56e868: e28d0054     	add	r0, sp, #84
  56e86c: e58d0008     	str	r0, [sp, #0x8]
  56e870: e58de014     	str	lr, [sp, #0x14]
  56e874: e28d1044     	add	r1, sp, #68
  56e878: e28d209c     	add	r2, sp, #156
  56e87c: e28d304c     	add	r3, sp, #76
  56e880: e28dc040     	add	r12, sp, #64
  56e884: e28de03c     	add	lr, sp, #60
  56e888: e2800018     	add	r0, r0, #24
  56e88c: e1a05007     	mov	r5, r7
  56e890: e58d101c     	str	r1, [sp, #0x1c]
  56e894: e58d2010     	str	r2, [sp, #0x10]
  56e898: e58d3020     	str	r3, [sp, #0x20]
  56e89c: e28d8084     	add	r8, sp, #132
  56e8a0: e58dc024     	str	r12, [sp, #0x24]
  56e8a4: e58de028     	str	lr, [sp, #0x28]
  56e8a8: e58d002c     	str	r0, [sp, #0x2c]
  56e8ac: e58d7034     	str	r7, [sp, #0x34]
  56e8b0: ea00001a     	b	0x56e920 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x244> @ imm = #0x68
  56e8b4: e159000b     	cmp	r9, r11
  56e8b8: ba00004b     	blt	0x56e9ec <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x310> @ imm = #0x12c
  56e8bc: e1a0000a     	mov	r0, r10
  56e8c0: e58d3004     	str	r3, [sp, #0x4]
  56e8c4: ebfff67c     	bl	0x56c2bc <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_deallocate_block()> @ imm = #-0x2610
  56e8c8: e59d3004     	ldr	r3, [sp, #0x4]
  56e8cc: e28d60cc     	add	r6, sp, #204
  56e8d0: e283a028     	add	r10, r3, #40
  56e8d4: e1a02005     	mov	r2, r5
  56e8d8: e1a00006     	mov	r0, r6
  56e8dc: e59d1018     	ldr	r1, [sp, #0x18]
  56e8e0: ebfffbf2     	bl	0x56d8b0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, char const*)> @ imm = #-0x1038
  56e8e4: e15a0006     	cmp	r10, r6
  56e8e8: 0a000003     	beq	0x56e8fc <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x220> @ imm = #0xc
  56e8ec: e1a0000a     	mov	r0, r10
  56e8f0: e59d10e0     	ldr	r1, [sp, #0xe0]
  56e8f4: e59d20dc     	ldr	r2, [sp, #0xdc]
  56e8f8: ebf6c8a2     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x24dd78
  56e8fc: e59d00e0     	ldr	r0, [sp, #0xe0]
  56e900: e1500006     	cmp	r0, r6
  56e904: 0a000002     	beq	0x56e914 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x238> @ imm = #0x8
  56e908: e3500000     	cmp	r0, #0
  56e90c: 0a000000     	beq	0x56e914 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x238> @ imm = #0x0
  56e910: ebf686ce     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x25e4c8
  56e914: e2875001     	add	r5, r7, #1
  56e918: e1540005     	cmp	r4, r5
  56e91c: 9a000078     	bls	0x56eb04 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x428> @ imm = #0x1e0
  56e920: e1a00005     	mov	r0, r5
  56e924: e3a0103a     	mov	r1, #58
  56e928: e0652004     	rsb	r2, r5, r4
  56e92c: ebf67f22     	bl	0x30e5bc <.plt+0x848>   @ imm = #-0x260378
  56e930: e3500000     	cmp	r0, #0
  56e934: 0a000064     	beq	0x56eacc <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x3f0> @ imm = #0x190
  56e938: e1540000     	cmp	r4, r0
  56e93c: e58d0044     	str	r0, [sp, #0x44]
  56e940: 0a00006f     	beq	0x56eb04 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x428> @ imm = #0x1bc
  56e944: e3a06000     	mov	r6, #0
  56e948: e5c06000     	strb	r6, [r0]
  56e94c: e59d3044     	ldr	r3, [sp, #0x44]
  56e950: e3a0100a     	mov	r1, #10
  56e954: e2833001     	add	r3, r3, #1
  56e958: e0632004     	rsb	r2, r3, r4
  56e95c: e1a00003     	mov	r0, r3
  56e960: e58d3044     	str	r3, [sp, #0x44]
  56e964: ebf67f14     	bl	0x30e5bc <.plt+0x848>   @ imm = #-0x2603b0
  56e968: e59d1014     	ldr	r1, [sp, #0x14]
  56e96c: e59d200c     	ldr	r2, [sp, #0xc]
  56e970: e1500006     	cmp	r0, r6
  56e974: 11a07000     	movne	r7, r0
  56e978: e792a001     	ldr	r10, [r2, r1]
  56e97c: 01a07004     	moveq	r7, r4
  56e980: e5c76000     	strb	r6, [r7]
  56e984: e1a0000a     	mov	r0, r10
  56e988: e59d101c     	ldr	r1, [sp, #0x1c]
  56e98c: ebfffa90     	bl	0x56d3d4 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>, std::less<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::priv::_Select1st<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>, std::priv::_MapTraitsT<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>, glitch::core::SAllocator<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, (glitch::memory::E_MEMORY_HINT)0>>::_M_lower_bound<char*>(char* const&) const> @ imm = #-0x15c0
  56e990: e150000a     	cmp	r0, r10
  56e994: e1a06000     	mov	r6, r0
  56e998: 0a000015     	beq	0x56e9f4 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x318> @ imm = #0x54
  56e99c: e28da0b4     	add	r10, sp, #180
  56e9a0: e59d1044     	ldr	r1, [sp, #0x44]
  56e9a4: e28d2050     	add	r2, sp, #80
  56e9a8: e1a0000a     	mov	r0, r10
  56e9ac: ebf6dda2     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x248978
  56e9b0: e59d30c8     	ldr	r3, [sp, #0xc8]
  56e9b4: e5961024     	ldr	r1, [r6, #0x24]
  56e9b8: e596b020     	ldr	r11, [r6, #0x20]
  56e9bc: e59d90c4     	ldr	r9, [sp, #0xc4]
  56e9c0: e1a00003     	mov	r0, r3
  56e9c4: e061b00b     	rsb	r11, r1, r11
  56e9c8: e0639009     	rsb	r9, r3, r9
  56e9cc: e15b0009     	cmp	r11, r9
  56e9d0: b1a0200b     	movlt	r2, r11
  56e9d4: a1a02009     	movge	r2, r9
  56e9d8: ebf67f00     	bl	0x30e5e0 <.plt+0x86c>   @ imm = #-0x260400
  56e9dc: e3500000     	cmp	r0, #0
  56e9e0: e1a03006     	mov	r3, r6
  56e9e4: 0affffb2     	beq	0x56e8b4 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x1d8> @ imm = #-0x138
  56e9e8: aaffffb3     	bge	0x56e8bc <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x1e0> @ imm = #-0x134
  56e9ec: e1a0000a     	mov	r0, r10
  56e9f0: ebfff631     	bl	0x56c2bc <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_deallocate_block()> @ imm = #-0x273c
  56e9f4: e59d2020     	ldr	r2, [sp, #0x20]
  56e9f8: e59d1044     	ldr	r1, [sp, #0x44]
  56e9fc: e59d0010     	ldr	r0, [sp, #0x10]
  56ea00: ebf6dd8d     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x2489cc
  56ea04: e1a00008     	mov	r0, r8
  56ea08: e3a01010     	mov	r1, #16
  56ea0c: e58d8094     	str	r8, [sp, #0x94]
  56ea10: e58d8098     	str	r8, [sp, #0x98]
  56ea14: ebf6c7e3     	bl	0x3209a8 <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_allocate_block(unsigned int)> @ imm = #-0x24e074
  56ea18: e59d3094     	ldr	r3, [sp, #0x94]
  56ea1c: e3a02000     	mov	r2, #0
  56ea20: e59d1010     	ldr	r1, [sp, #0x10]
  56ea24: e5c32000     	strb	r2, [r3]
  56ea28: e59d0008     	ldr	r0, [sp, #0x8]
  56ea2c: e1a02008     	mov	r2, r8
  56ea30: ebfff64d     	bl	0x56c36c <std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>::pair(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&)> @ imm = #-0x26cc
  56ea34: e59d3014     	ldr	r3, [sp, #0x14]
  56ea38: e59dc00c     	ldr	r12, [sp, #0xc]
  56ea3c: e59d0024     	ldr	r0, [sp, #0x24]
  56ea40: e59d2028     	ldr	r2, [sp, #0x28]
  56ea44: e79c1003     	ldr	r1, [r12, r3]
  56ea48: e59d3008     	ldr	r3, [sp, #0x8]
  56ea4c: e58d603c     	str	r6, [sp, #0x3c]
  56ea50: ebfffde0     	bl	0x56e1d8 <std::priv::_Rb_tree<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>, std::less<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::priv::_Select1st<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>, std::priv::_MapTraitsT<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>, glitch::core::SAllocator<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, (glitch::memory::E_MEMORY_HINT)0>>::insert_unique(std::priv::_Rb_tree_iterator<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::priv::_MapTraitsT<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>>, std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>> const&)> @ imm = #-0x880
  56ea54: e59d0080     	ldr	r0, [sp, #0x80]
  56ea58: e59de02c     	ldr	lr, [sp, #0x2c]
  56ea5c: e59da040     	ldr	r10, [sp, #0x40]
  56ea60: e150000e     	cmp	r0, lr
  56ea64: 0a000002     	beq	0x56ea74 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x398> @ imm = #0x8
  56ea68: e3500000     	cmp	r0, #0
  56ea6c: 0a000000     	beq	0x56ea74 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x398> @ imm = #0x0
  56ea70: ebf68676     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x25e628
  56ea74: e59d0068     	ldr	r0, [sp, #0x68]
  56ea78: e59d1008     	ldr	r1, [sp, #0x8]
  56ea7c: e1500001     	cmp	r0, r1
  56ea80: 0a000002     	beq	0x56ea90 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x3b4> @ imm = #0x8
  56ea84: e3500000     	cmp	r0, #0
  56ea88: 0a000000     	beq	0x56ea90 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x3b4> @ imm = #0x0
  56ea8c: ebf6866f     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x25e644
  56ea90: e59d0098     	ldr	r0, [sp, #0x98]
  56ea94: e1500008     	cmp	r0, r8
  56ea98: 0a000002     	beq	0x56eaa8 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x3cc> @ imm = #0x8
  56ea9c: e3500000     	cmp	r0, #0
  56eaa0: 0a000000     	beq	0x56eaa8 <glitch::io::CFileSystem::addObfuscationFileMap(char const*, unsigned char, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>)+0x3cc> @ imm = #0x0
  56eaa4: ebf68669     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x25e65c
  56eaa8: e59d00b0     	ldr	r0, [sp, #0xb0]
  56eaac: e59d2010     	ldr	r2, [sp, #0x10]
