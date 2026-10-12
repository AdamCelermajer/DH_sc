
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0056c740 <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const>:
  56c740: e92d40f0     	push	{r4, r5, r6, r7, lr}
  56c744: e3a0102f     	mov	r1, #47
  56c748: e24dd014     	sub	sp, sp, #20
  56c74c: e1a05000     	mov	r5, r0
  56c750: e1a00002     	mov	r0, r2
  56c754: e1a04002     	mov	r4, r2
  56c758: e1a07003     	mov	r7, r3
  56c75c: ebffffb7     	bl	0x56c640 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::rfind(char, unsigned int) const (.clone.1)> @ imm = #-0x124
  56c760: e3a0105c     	mov	r1, #92
  56c764: e1a06000     	mov	r6, r0
  56c768: e1a00004     	mov	r0, r4
  56c76c: ebffffb3     	bl	0x56c640 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::rfind(char, unsigned int) const (.clone.1)> @ imm = #-0x134
  56c770: e1500006     	cmp	r0, r6
  56c774: a1a06000     	movge	r6, r0
  56c778: b1a06006     	movlt	r6, r6
  56c77c: e3570000     	cmp	r7, #0
  56c780: 0a000010     	beq	0x56c7c8 <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const+0x88> @ imm = #0x40
  56c784: e5943014     	ldr	r3, [r4, #0x14]
  56c788: e5942010     	ldr	r2, [r4, #0x10]
  56c78c: e3a07000     	mov	r7, #0
  56c790: e0632002     	rsb	r2, r3, r2
  56c794: e1560002     	cmp	r6, r2
  56c798: 3a000017     	blo	0x56c7fc <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const+0xbc> @ imm = #0x5c
  56c79c: e3570000     	cmp	r7, #0
  56c7a0: 1a00001f     	bne	0x56c824 <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const+0xe4> @ imm = #0x7c
  56c7a4: e5855010     	str	r5, [r5, #0x10]
  56c7a8: e5855014     	str	r5, [r5, #0x14]
  56c7ac: e5942010     	ldr	r2, [r4, #0x10]
  56c7b0: e1a00005     	mov	r0, r5
  56c7b4: e5941014     	ldr	r1, [r4, #0x14]
  56c7b8: ebf6e60d     	bl	0x325ff4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_range_initialize(char const*, char const*)> @ imm = #-0x2467cc
  56c7bc: e1a00005     	mov	r0, r5
  56c7c0: e28dd014     	add	sp, sp, #20
  56c7c4: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  56c7c8: e1a00004     	mov	r0, r4
  56c7cc: e3a0102e     	mov	r1, #46
  56c7d0: ebffff9a     	bl	0x56c640 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::rfind(char, unsigned int) const (.clone.1)> @ imm = #-0x198
  56c7d4: e3700001     	cmn	r0, #1
  56c7d8: 15942010     	ldrne	r2, [r4, #0x10]
  56c7dc: 05943014     	ldreq	r3, [r4, #0x14]
  56c7e0: 05942010     	ldreq	r2, [r4, #0x10]
  56c7e4: 15943014     	ldrne	r3, [r4, #0x14]
  56c7e8: 00632002     	rsbeq	r2, r3, r2
  56c7ec: 10632002     	rsbne	r2, r3, r2
  56c7f0: 10607002     	rsbne	r7, r0, r2
  56c7f4: e1560002     	cmp	r6, r2
  56c7f8: 2affffe7     	bhs	0x56c79c <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const+0x5c> @ imm = #-0x64
  56c7fc: e1e03006     	mvn	r3, r6
  56c800: e0832002     	add	r2, r3, r2
  56c804: e0673002     	rsb	r3, r7, r2
  56c808: e28dc00c     	add	r12, sp, #12
  56c80c: e1a01004     	mov	r1, r4
  56c810: e2862001     	add	r2, r6, #1
  56c814: e1a00005     	mov	r0, r5
  56c818: e58dc000     	str	r12, [sp]
  56c81c: ebffff28     	bl	0x56c4c4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, unsigned int, unsigned int, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x360
  56c820: eaffffe5     	b	0x56c7bc <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const+0x7c> @ imm = #-0x6c
  56c824: e0673002     	rsb	r3, r7, r2
  56c828: e28dc008     	add	r12, sp, #8
  56c82c: e1a01004     	mov	r1, r4
  56c830: e1a00005     	mov	r0, r5
  56c834: e3a02000     	mov	r2, #0
  56c838: e58dc000     	str	r12, [sp]
  56c83c: ebffff20     	bl	0x56c4c4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, unsigned int, unsigned int, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x380
  56c840: eaffffdd     	b	0x56c7bc <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const+0x7c> @ imm = #-0x8c

0056c844 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)>:
  56c844: e59f310c     	ldr	r3, [pc, #0x10c]        @ 0x56c958 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0x114>
  56c848: e92d40f0     	push	{r4, r5, r6, r7, lr}
  56c84c: e59f2108     	ldr	r2, [pc, #0x108]        @ 0x56c95c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0x118>
  56c850: e08f3003     	add	r3, pc, r3
  56c854: e5901014     	ldr	r1, [r0, #0x14]
  56c858: e7934002     	ldr	r4, [r3, r2]
  56c85c: e590c010     	ldr	r12, [r0, #0x10]
  56c860: e24dd02c     	sub	sp, sp, #44
  56c864: e5940000     	ldr	r0, [r4]
  56c868: e05cc001     	subs	r12, r12, r1
  56c86c: e1a06001     	mov	r6, r1
  56c870: e58d0024     	str	r0, [sp, #0x24]
  56c874: 0a000035     	beq	0x56c950 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0x10c> @ imm = #0xd4
  56c878: e28d000c     	add	r0, sp, #12
  56c87c: e081500c     	add	r5, r1, r12
  56c880: e28d4028     	add	r4, sp, #40
  56c884: e3a0c000     	mov	r12, #0
  56c888: e5245028     	str	r5, [r4, #-0x28]!
  56c88c: e480c004     	str	r12, [r0], #4
  56c890: e480c004     	str	r12, [r0], #4
  56c894: e480c004     	str	r12, [r0], #4
  56c898: e480c004     	str	r12, [r0], #4
  56c89c: e480c004     	str	r12, [r0], #4
  56c8a0: e580c000     	str	r12, [r0]
  56c8a4: e3a00010     	mov	r0, #16
  56c8a8: e5cd000f     	strb	r0, [sp, #0xf]
  56c8ac: e1510005     	cmp	r1, r5
  56c8b0: e3e0007f     	mvn	r0, #127
  56c8b4: e58dc008     	str	r12, [sp, #0x8]
  56c8b8: e58dc004     	str	r12, [sp, #0x4]
  56c8bc: e5cd0009     	strb	r0, [sp, #0x9]
  56c8c0: 0a000014     	beq	0x56c918 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0xd4> @ imm = #0x50
  56c8c4: e555c001     	ldrb	r12, [r5, #-0x1]
  56c8c8: e28d7028     	add	r7, sp, #40
  56c8cc: e2450001     	sub	r0, r5, #1
  56c8d0: e08761ac     	add	r6, r7, r12, lsr #3
  56c8d4: e5566024     	ldrb	r6, [r6, #-0x24]
  56c8d8: e20cc007     	and	r12, r12, #7
  56c8dc: e1a0cc56     	asr	r12, r6, r12
  56c8e0: e31c0001     	tst	r12, #1
  56c8e4: 1a000017     	bne	0x56c948 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0x104> @ imm = #0x5c
  56c8e8: e1500001     	cmp	r0, r1
  56c8ec: e5840000     	str	r0, [r4]
  56c8f0: 0a000007     	beq	0x56c914 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0xd0> @ imm = #0x1c
  56c8f4: e570c001     	ldrb	r12, [r0, #-0x1]!
  56c8f8: e28d6028     	add	r6, sp, #40
  56c8fc: e08651ac     	add	r5, r6, r12, lsr #3
  56c900: e5555024     	ldrb	r5, [r5, #-0x24]
  56c904: e20cc007     	and	r12, r12, #7
  56c908: e1a0cc55     	asr	r12, r5, r12
  56c90c: e31c0001     	tst	r12, #1
  56c910: 0afffff4     	beq	0x56c8e8 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0xa4> @ imm = #-0x30
  56c914: e59d6000     	ldr	r6, [sp]
  56c918: e1510006     	cmp	r1, r6
  56c91c: 12466001     	subne	r6, r6, #1
  56c920: 10610006     	rsbne	r0, r1, r6
  56c924: 0a000009     	beq	0x56c950 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0x10c> @ imm = #0x24
  56c928: e7933002     	ldr	r3, [r3, r2]
  56c92c: e59d2024     	ldr	r2, [sp, #0x24]
  56c930: e5933000     	ldr	r3, [r3]
  56c934: e1520003     	cmp	r2, r3
  56c938: 1a000001     	bne	0x56c944 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0x100> @ imm = #0x4
  56c93c: e28dd02c     	add	sp, sp, #44
  56c940: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  56c944: ebf68671     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x25e63c
  56c948: e1a06005     	mov	r6, r5
  56c94c: eafffff1     	b	0x56c918 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0xd4> @ imm = #-0x3c
  56c950: e3e00000     	mvn	r0, #0
  56c954: eafffff3     	b	0x56c928 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)+0xe4> @ imm = #-0x34
  56c958: 40 82 42 00  	.word	0x00428240
  56c95c: ac 40 00 00  	.word	0x000040ac

0056c960 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)>:
  56c960: e92d4070     	push	{r4, r5, r6, lr}
  56c964: e5801004     	str	r1, [r0, #0x4]
  56c968: e24dd008     	sub	sp, sp, #8
  56c96c: e2805008     	add	r5, r0, #8
  56c970: e3a01000     	mov	r1, #0
  56c974: e5801000     	str	r1, [r0]
  56c978: e1a06003     	mov	r6, r3
  56c97c: e1a01002     	mov	r1, r2
  56c980: e1a04000     	mov	r4, r0
  56c984: e28d2004     	add	r2, sp, #4
  56c988: e1a00005     	mov	r0, r5
  56c98c: ebf6e5aa     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x246958
  56c990: e5c46024     	strb	r6, [r4, #0x24]
  56c994: e1a00005     	mov	r0, r5
  56c998: ebffffa9     	bl	0x56c844 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)> @ imm = #-0x15c
  56c99c: e3700001     	cmn	r0, #1
  56c9a0: 0a000007     	beq	0x56c9c4 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x64> @ imm = #0x1c
  56c9a4: e594301c     	ldr	r3, [r4, #0x1c]
  56c9a8: e5942018     	ldr	r2, [r4, #0x18]
  56c9ac: e2805001     	add	r5, r0, #1
  56c9b0: e0632002     	rsb	r2, r3, r2
  56c9b4: e1550002     	cmp	r5, r2
  56c9b8: 2a000004     	bhs	0x56c9d0 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x70> @ imm = #0x10
  56c9bc: e0835005     	add	r5, r3, r5
  56c9c0: e5845020     	str	r5, [r4, #0x20]
  56c9c4: e1a00004     	mov	r0, r4
  56c9c8: e28dd008     	add	sp, sp, #8
  56c9cc: e8bd8070     	pop	{r4, r5, r6, pc}
  56c9d0: e59f000c     	ldr	r0, [pc, #0xc]          @ 0x56c9e4 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x84>
  56c9d4: e08f0000     	add	r0, pc, r0
  56c9d8: eb067134     	bl	0x708eb0 <___ZSt24__stl_throw_out_of_rangePKc_veneer> @ imm = #0x19c4d0
  56c9dc: e594301c     	ldr	r3, [r4, #0x1c]
  56c9e0: eafffff5     	b	0x56c9bc <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x5c> @ imm = #-0x2c
  56c9e4: 84 1a 35 00  	.word	0x00351a84

0056c9e8 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)>:
  56c9e8: e92d4070     	push	{r4, r5, r6, lr}
  56c9ec: e5801004     	str	r1, [r0, #0x4]
  56c9f0: e24dd008     	sub	sp, sp, #8
  56c9f4: e2805008     	add	r5, r0, #8
  56c9f8: e3a01000     	mov	r1, #0
  56c9fc: e5801000     	str	r1, [r0]
  56ca00: e1a06003     	mov	r6, r3
  56ca04: e1a01002     	mov	r1, r2
  56ca08: e1a04000     	mov	r4, r0
  56ca0c: e28d2004     	add	r2, sp, #4
  56ca10: e1a00005     	mov	r0, r5
  56ca14: ebf6e588     	bl	0x32603c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::basic_string(char const*, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0> const&)> @ imm = #-0x2469e0
  56ca18: e5c46024     	strb	r6, [r4, #0x24]
  56ca1c: e1a00005     	mov	r0, r5
  56ca20: ebffff87     	bl	0x56c844 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::find_last_of(char const*, unsigned int, unsigned int) const (.clone.4)> @ imm = #-0x1e4
  56ca24: e3700001     	cmn	r0, #1
  56ca28: 0a000007     	beq	0x56ca4c <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x64> @ imm = #0x1c
  56ca2c: e594301c     	ldr	r3, [r4, #0x1c]
  56ca30: e5942018     	ldr	r2, [r4, #0x18]
  56ca34: e2805001     	add	r5, r0, #1
  56ca38: e0632002     	rsb	r2, r3, r2
  56ca3c: e1550002     	cmp	r5, r2
  56ca40: 2a000004     	bhs	0x56ca58 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x70> @ imm = #0x10
  56ca44: e0835005     	add	r5, r3, r5
  56ca48: e5845020     	str	r5, [r4, #0x20]
  56ca4c: e1a00004     	mov	r0, r4
  56ca50: e28dd008     	add	sp, sp, #8
  56ca54: e8bd8070     	pop	{r4, r5, r6, pc}
  56ca58: e59f000c     	ldr	r0, [pc, #0xc]          @ 0x56ca6c <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x84>
  56ca5c: e08f0000     	add	r0, pc, r0
  56ca60: eb067112     	bl	0x708eb0 <___ZSt24__stl_throw_out_of_rangePKc_veneer> @ imm = #0x19c448
  56ca64: e594301c     	ldr	r3, [r4, #0x1c]
  56ca68: eafffff5     	b	0x56ca44 <glitch::io::CFile::CFile(__sFILE*, char const*, bool)+0x5c> @ imm = #-0x2c
  56ca6c: fc 19 35 00  	.word	0x003519fc
