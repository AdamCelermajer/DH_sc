
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056dc40 <glitch::io::CFileSystem::open(char const*, char const*)>:
  56dc40: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  56dc44: e59f42dc     	ldr	r4, [pc, #0x2dc]        @ 0x56df28 <glitch::io::CFileSystem::open(char const*, char const*)+0x2e8>
  56dc48: e59f62dc     	ldr	r6, [pc, #0x2dc]        @ 0x56df2c <glitch::io::CFileSystem::open(char const*, char const*)+0x2ec>
  56dc4c: e59f72dc     	ldr	r7, [pc, #0x2dc]        @ 0x56df30 <glitch::io::CFileSystem::open(char const*, char const*)+0x2f0>
  56dc50: e08f4004     	add	r4, pc, r4
  56dc54: e794c006     	ldr	r12, [r4, r6]
  56dc58: e7943007     	ldr	r3, [r4, r7]
  56dc5c: e24dd084     	sub	sp, sp, #132
  56dc60: e59cb010     	ldr	r11, [r12, #0x10]
  56dc64: e5933000     	ldr	r3, [r3]
  56dc68: e1a08000     	mov	r8, r0
  56dc6c: e35b0000     	cmp	r11, #0
  56dc70: e1a05001     	mov	r5, r1
  56dc74: e58d307c     	str	r3, [sp, #0x7c]
  56dc78: e1a09002     	mov	r9, r2
  56dc7c: 0a000035     	beq	0x56dd58 <glitch::io::CFileSystem::open(char const*, char const*)+0x118> @ imm = #0xd4
  56dc80: e1d130d0     	ldrsb	r3, [r1]
  56dc84: e353002e     	cmp	r3, #46
  56dc88: 0a000070     	beq	0x56de50 <glitch::io::CFileSystem::open(char const*, char const*)+0x210> @ imm = #0x1c0
  56dc8c: e1a0b005     	mov	r11, r5
  56dc90: e59f329c     	ldr	r3, [pc, #0x29c]        @ 0x56df34 <glitch::io::CFileSystem::open(char const*, char const*)+0x2f4>
  56dc94: e794a003     	ldr	r10, [r4, r3]
  56dc98: e1a0000a     	mov	r0, r10
  56dc9c: ebf6806c     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x25fe50
  56dca0: e2503000     	subs	r3, r0, #0
  56dca4: 0a00000e     	beq	0x56dce4 <glitch::io::CFileSystem::open(char const*, char const*)+0xa4> @ imm = #0x38
  56dca8: e1a0000b     	mov	r0, r11
  56dcac: e1a0100a     	mov	r1, r10
  56dcb0: e58d3004     	str	r3, [sp, #0x4]
  56dcb4: ebf683c6     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x25f0e8
  56dcb8: e3500000     	cmp	r0, #0
  56dcbc: e59d3004     	ldr	r3, [sp, #0x4]
  56dcc0: 0a000007     	beq	0x56dce4 <glitch::io::CFileSystem::open(char const*, char const*)+0xa4> @ imm = #0x1c
  56dcc4: e08aa003     	add	r10, r10, r3
  56dcc8: e15a20d1     	ldrsb	r2, [r10, #-1]
  56dccc: e352005c     	cmp	r2, #92
  56dcd0: 1352002f     	cmpne	r2, #47
  56dcd4: 03a02000     	moveq	r2, #0
  56dcd8: 13a02001     	movne	r2, #1
  56dcdc: e0823003     	add	r3, r2, r3
  56dce0: e08bb003     	add	r11, r11, r3
  56dce4: e28da064     	add	r10, sp, #100
  56dce8: e1a0100b     	mov	r1, r11
  56dcec: e1a0000a     	mov	r0, r10
  56dcf0: e28d2018     	add	r2, sp, #24
  56dcf4: ebf6e0d0     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x247cc0
  56dcf8: e59d3078     	ldr	r3, [sp, #0x78]
  56dcfc: e59d1074     	ldr	r1, [sp, #0x74]
  56dd00: e1530001     	cmp	r3, r1
  56dd04: 0a000006     	beq	0x56dd24 <glitch::io::CFileSystem::open(char const*, char const*)+0xe4> @ imm = #0x18
  56dd08: e3a0002f     	mov	r0, #47
  56dd0c: e1d320d0     	ldrsb	r2, [r3]
  56dd10: e352005c     	cmp	r2, #92
  56dd14: 05c30000     	strbeq	r0, [r3]
  56dd18: e2833001     	add	r3, r3, #1
  56dd1c: e1530001     	cmp	r3, r1
  56dd20: 1afffff9     	bne	0x56dd0c <glitch::io::CFileSystem::open(char const*, char const*)+0xcc> @ imm = #-0x1c
  56dd24: e1a0000a     	mov	r0, r10
  56dd28: ebffff86     	bl	0x56db48 <std::priv::_Rb_tree_node_base* std::priv::_Rb_tree<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>, std::less<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, std::priv::_Select1st<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>, std::priv::_MapTraitsT<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>>, glitch::core::SAllocator<std::pair<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>, (glitch::memory::E_MEMORY_HINT)0>>::_M_find<std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const (.clone.13)> @ imm = #-0x1e8
  56dd2c: e7943006     	ldr	r3, [r4, r6]
  56dd30: e1500003     	cmp	r0, r3
  56dd34: 1590503c     	ldrne	r5, [r0, #0x3c]
  56dd38: e59d0078     	ldr	r0, [sp, #0x78]
  56dd3c: 03a0b000     	moveq	r11, #0
  56dd40: 13a0b001     	movne	r11, #1
  56dd44: e150000a     	cmp	r0, r10
  56dd48: 0a000002     	beq	0x56dd58 <glitch::io::CFileSystem::open(char const*, char const*)+0x118> @ imm = #0x8
  56dd4c: e3500000     	cmp	r0, #0
  56dd50: 0a000000     	beq	0x56dd58 <glitch::io::CFileSystem::open(char const*, char const*)+0x118> @ imm = #0x0
  56dd54: ebf689bd     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x25d90c
  56dd58: e28d604c     	add	r6, sp, #76
  56dd5c: e1a01005     	mov	r1, r5
  56dd60: e1a00006     	mov	r0, r6
  56dd64: e28d2014     	add	r2, sp, #20
  56dd68: ebf6e0b3     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x247d34
  56dd6c: e59d105c     	ldr	r1, [sp, #0x5c]
  56dd70: e59d0060     	ldr	r0, [sp, #0x60]
  56dd74: e1510000     	cmp	r1, r0
  56dd78: 0a00002d     	beq	0x56de34 <glitch::io::CFileSystem::open(char const*, char const*)+0x1f4> @ imm = #0xb4
  56dd7c: e28d2080     	add	r2, sp, #128
  56dd80: e3a0303a     	mov	r3, #58
  56dd84: e5623078     	strb	r3, [r2, #-0x78]!
  56dd88: e28d300c     	add	r3, sp, #12
  56dd8c: ebf7839c     	bl	0x34ec04 <char const* std::priv::__find_if<char const*, std::priv::_Eq_char_bound<std::char_traits<char>>>(char const*, char const*, std::priv::_Eq_char_bound<std::char_traits<char>>, std::random_access_iterator_tag const&)> @ imm = #-0x21f190
  56dd90: e59d205c     	ldr	r2, [sp, #0x5c]
  56dd94: e1a03000     	mov	r3, r0
  56dd98: e1500002     	cmp	r0, r2
  56dd9c: 0a000024     	beq	0x56de34 <glitch::io::CFileSystem::open(char const*, char const*)+0x1f4> @ imm = #0x90
  56dda0: e59d0060     	ldr	r0, [sp, #0x60]
  56dda4: e0603003     	rsb	r3, r0, r3
  56dda8: e3730001     	cmn	r3, #1
  56ddac: 0a000020     	beq	0x56de34 <glitch::io::CFileSystem::open(char const*, char const*)+0x1f4> @ imm = #0x80
  56ddb0: e1a01009     	mov	r1, r9
  56ddb4: ebf681d3     	bl	0x30e508 <.plt+0x794>   @ imm = #-0x25f8b4
  56ddb8: e250a000     	subs	r10, r0, #0
  56ddbc: 0588a000     	streq	r10, [r8]
  56ddc0: 0a00000d     	beq	0x56ddfc <glitch::io::CFileSystem::open(char const*, char const*)+0x1bc> @ imm = #0x34
  56ddc4: e3a01000     	mov	r1, #0
  56ddc8: e3a00028     	mov	r0, #40
  56ddcc: e59d9060     	ldr	r9, [sp, #0x60]
  56ddd0: ebff18f5     	bl	0x5341ac <operator new(unsigned int, glitch::memory::E_MEMORY_HINT)> @ imm = #-0x39c2c
  56ddd4: e1a0300b     	mov	r3, r11
  56ddd8: e1a0100a     	mov	r1, r10
  56dddc: e1a02009     	mov	r2, r9
  56dde0: e1a05000     	mov	r5, r0
  56dde4: ebfffaff     	bl	0x56c9e8 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)> @ imm = #-0x1404
  56dde8: e3550000     	cmp	r5, #0
  56ddec: e5885000     	str	r5, [r8]
  56ddf0: 15953000     	ldrne	r3, [r5]
  56ddf4: 12833001     	addne	r3, r3, #1
  56ddf8: 15853000     	strne	r3, [r5]
  56ddfc: e59d0060     	ldr	r0, [sp, #0x60]
  56de00: e1500006     	cmp	r0, r6
  56de04: 0a000002     	beq	0x56de14 <glitch::io::CFileSystem::open(char const*, char const*)+0x1d4> @ imm = #0x8
  56de08: e3500000     	cmp	r0, #0
  56de0c: 0a000000     	beq	0x56de14 <glitch::io::CFileSystem::open(char const*, char const*)+0x1d4> @ imm = #0x0
  56de10: ebf6898e     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x25d9c8
  56de14: e7943007     	ldr	r3, [r4, r7]
  56de18: e59d207c     	ldr	r2, [sp, #0x7c]
  56de1c: e1a00008     	mov	r0, r8
  56de20: e5933000     	ldr	r3, [r3]
  56de24: e1520003     	cmp	r2, r3
  56de28: 1a00003d     	bne	0x56df24 <glitch::io::CFileSystem::open(char const*, char const*)+0x2e4> @ imm = #0xf4
  56de2c: e28dd084     	add	sp, sp, #132
  56de30: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  56de34: e59f30f8     	ldr	r3, [pc, #0xf8]         @ 0x56df34 <glitch::io::CFileSystem::open(char const*, char const*)+0x2f4>
  56de38: e7941003     	ldr	r1, [r4, r3]
  56de3c: e1d130d0     	ldrsb	r3, [r1]
  56de40: e3530000     	cmp	r3, #0
  56de44: 1a000007     	bne	0x56de68 <glitch::io::CFileSystem::open(char const*, char const*)+0x228> @ imm = #0x1c
  56de48: e59d0060     	ldr	r0, [sp, #0x60]
  56de4c: eaffffd7     	b	0x56ddb0 <glitch::io::CFileSystem::open(char const*, char const*)+0x170> @ imm = #-0xa4
  56de50: e1d130d1     	ldrsb	r3, [r1, #1]
  56de54: e353002f     	cmp	r3, #47
  56de58: 1353005c     	cmpne	r3, #92
  56de5c: 0281b002     	addeq	r11, r1, #2
  56de60: 0affff8a     	beq	0x56dc90 <glitch::io::CFileSystem::open(char const*, char const*)+0x50> @ imm = #-0x1d8
  56de64: eaffff88     	b	0x56dc8c <glitch::io::CFileSystem::open(char const*, char const*)+0x4c> @ imm = #-0x1e0
  56de68: e28da034     	add	r10, sp, #52
  56de6c: e28d2010     	add	r2, sp, #16
  56de70: e1a0000a     	mov	r0, r10
  56de74: ebf6e070     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x247e40
  56de78: e59d2044     	ldr	r2, [sp, #0x44]
  56de7c: e59d1048     	ldr	r1, [sp, #0x48]
  56de80: e15230d1     	ldrsb	r3, [r2, #-1]
  56de84: e353005c     	cmp	r3, #92
  56de88: 1353002f     	cmpne	r3, #47
  56de8c: 1a00000e     	bne	0x56decc <glitch::io::CFileSystem::open(char const*, char const*)+0x28c> @ imm = #0x38
  56de90: e28d501c     	add	r5, sp, #28
  56de94: e1a0100a     	mov	r1, r10
  56de98: e1a02006     	mov	r2, r6
  56de9c: e1a00005     	mov	r0, r5
  56dea0: ebf7811f     	bl	0x34e324 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&)> @ imm = #-0x21fb84
  56dea4: e59d1030     	ldr	r1, [sp, #0x30]
  56dea8: e59d202c     	ldr	r2, [sp, #0x2c]
  56deac: e1a00006     	mov	r0, r6
  56deb0: ebf6cb34     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x24d330
  56deb4: e1a00005     	mov	r0, r5
  56deb8: ebfff8ff     	bl	0x56c2bc <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_deallocate_block()> @ imm = #-0x1c04
  56debc: e1a0000a     	mov	r0, r10
  56dec0: ebfff8fd     	bl	0x56c2bc <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_deallocate_block()> @ imm = #-0x1c0c
  56dec4: e59d0060     	ldr	r0, [sp, #0x60]
  56dec8: eaffffb8     	b	0x56ddb0 <glitch::io::CFileSystem::open(char const*, char const*)+0x170> @ imm = #-0x120
  56decc: e151000a     	cmp	r1, r10
  56ded0: 159d1034     	ldrne	r1, [sp, #0x34]
  56ded4: 028a1010     	addeq	r1, r10, #16
  56ded8: e0621001     	rsb	r1, r2, r1
  56dedc: e3510001     	cmp	r1, #1
  56dee0: 0a000008     	beq	0x56df08 <glitch::io::CFileSystem::open(char const*, char const*)+0x2c8> @ imm = #0x20
  56dee4: e3a03000     	mov	r3, #0
  56dee8: e5c23001     	strb	r3, [r2, #0x1]
  56deec: e59d3044     	ldr	r3, [sp, #0x44]
  56def0: e3a0202f     	mov	r2, #47
  56def4: e5c32000     	strb	r2, [r3]
  56def8: e59d3044     	ldr	r3, [sp, #0x44]
  56defc: e2833001     	add	r3, r3, #1
  56df00: e58d3044     	str	r3, [sp, #0x44]
  56df04: eaffffe1     	b	0x56de90 <glitch::io::CFileSystem::open(char const*, char const*)+0x250> @ imm = #-0x7c
  56df08: e1a0000a     	mov	r0, r10
  56df0c: ebf6c911     	bl	0x320358 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_compute_next_size(unsigned int)> @ imm = #-0x24dbbc
  56df10: e1a01000     	mov	r1, r0
  56df14: e1a0000a     	mov	r0, r10
  56df18: ebfb2001     	bl	0x435f24 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_reserve(unsigned int)> @ imm = #-0x137ffc
  56df1c: e59d2044     	ldr	r2, [sp, #0x44]
  56df20: eaffffef     	b	0x56dee4 <glitch::io::CFileSystem::open(char const*, char const*)+0x2a4> @ imm = #-0x44
  56df24: ebf680f9     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x25fc1c
  56df28: 40 6e 42 00  	.word	0x00426e40
  56df2c: b4 47 00 00  	.word	0x000047b4
  56df30: ac 40 00 00  	.word	0x000040ac
  56df34: 68 0e 00 00  	.word	0x00000e68
