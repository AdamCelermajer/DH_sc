
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

006594a0 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)>:
  6594a0: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  6594a4: e59f4248     	ldr	r4, [pc, #0x248]        @ 0x6596f4 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x254>
  6594a8: e59f8248     	ldr	r8, [pc, #0x248]        @ 0x6596f8 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x258>
  6594ac: e24dd0c4     	sub	sp, sp, #196
  6594b0: e08f4004     	add	r4, pc, r4
  6594b4: e7941008     	ldr	r1, [r4, r8]
  6594b8: e58d200c     	str	r2, [sp, #0xc]
  6594bc: e59f2238     	ldr	r2, [pc, #0x238]        @ 0x6596fc <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x25c>
  6594c0: e591c000     	ldr	r12, [r1]
  6594c4: e59da0ec     	ldr	r10, [sp, #0xec]
  6594c8: e59db0f0     	ldr	r11, [sp, #0xf0]
  6594cc: e28d60a4     	add	r6, sp, #164
  6594d0: e59d90e8     	ldr	r9, [sp, #0xe8]
  6594d4: e1a05000     	mov	r5, r0
  6594d8: e08f2002     	add	r2, pc, r2
  6594dc: e1a00006     	mov	r0, r6
  6594e0: e59d100c     	ldr	r1, [sp, #0xc]
  6594e4: e28d708c     	add	r7, sp, #140
  6594e8: e58dc0bc     	str	r12, [sp, #0xbc]
  6594ec: e58d3010     	str	r3, [sp, #0x10]
  6594f0: ebfc50ee     	bl	0x56d8b0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, char const*)> @ imm = #-0xebc48
  6594f4: e1a0200a     	mov	r2, r10
  6594f8: e1a00007     	mov	r0, r7
  6594fc: e1a01006     	mov	r1, r6
  659500: ebfc50ea     	bl	0x56d8b0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, char const*)> @ imm = #-0xebc58
  659504: e1a00005     	mov	r0, r5
  659508: e1a01009     	mov	r1, r9
  65950c: e59d20a0     	ldr	r2, [sp, #0xa0]
  659510: e1a0300b     	mov	r3, r11
  659514: ebfe4f3d     	bl	0x5ed210 <glitch::video::CTextureManager::getTexture(char const*, char const*)> @ imm = #-0x6c30c
  659518: e59d00a0     	ldr	r0, [sp, #0xa0]
  65951c: e1500007     	cmp	r0, r7
  659520: 0a000002     	beq	0x659530 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x90> @ imm = #0x8
  659524: e3500000     	cmp	r0, #0
  659528: 0a000000     	beq	0x659530 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x90> @ imm = #0x0
  65952c: ebf2dbc7     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3490e4
  659530: e59d00b8     	ldr	r0, [sp, #0xb8]
  659534: e1500006     	cmp	r0, r6
  659538: 0a000002     	beq	0x659548 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xa8> @ imm = #0x8
  65953c: e3500000     	cmp	r0, #0
  659540: 0a000000     	beq	0x659548 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xa8> @ imm = #0x0
  659544: ebf2dbc1     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3490fc
  659548: e5953000     	ldr	r3, [r5]
  65954c: e3530000     	cmp	r3, #0
  659550: 0a000007     	beq	0x659574 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xd4> @ imm = #0x1c
  659554: e7943008     	ldr	r3, [r4, r8]
  659558: e59d20bc     	ldr	r2, [sp, #0xbc]
  65955c: e1a00005     	mov	r0, r5
  659560: e5933000     	ldr	r3, [r3]
  659564: e1520003     	cmp	r2, r3
  659568: 1a000060     	bne	0x6596f0 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x250> @ imm = #0x180
  65956c: e28dd0c4     	add	sp, sp, #196
  659570: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  659574: e1a0200a     	mov	r2, r10
  659578: e28d0020     	add	r0, sp, #32
  65957c: e1a0300b     	mov	r3, r11
  659580: e1a01009     	mov	r1, r9
  659584: ebfe4f21     	bl	0x5ed210 <glitch::video::CTextureManager::getTexture(char const*, char const*)> @ imm = #-0x6c37c
  659588: e59d3020     	ldr	r3, [sp, #0x20]
  65958c: e3530000     	cmp	r3, #0
  659590: 15932004     	ldrne	r2, [r3, #0x4]
  659594: 12822001     	addne	r2, r2, #1
  659598: 15832004     	strne	r2, [r3, #0x4]
  65959c: e5950000     	ldr	r0, [r5]
  6595a0: e5853000     	str	r3, [r5]
  6595a4: e3500000     	cmp	r0, #0
  6595a8: 0a000000     	beq	0x6595b0 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x110> @ imm = #0x0
  6595ac: ebf30ff4     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x33c030
  6595b0: e59d0020     	ldr	r0, [sp, #0x20]
  6595b4: e3500000     	cmp	r0, #0
  6595b8: 0a000000     	beq	0x6595c0 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x120> @ imm = #0x0
  6595bc: ebf30ff0     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x33c040
  6595c0: e5952000     	ldr	r2, [r5]
  6595c4: e3520000     	cmp	r2, #0
  6595c8: e58d2014     	str	r2, [sp, #0x14]
  6595cc: 1affffe0     	bne	0x659554 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xb4> @ imm = #-0x80
  6595d0: e59d3010     	ldr	r3, [sp, #0x10]
  6595d4: e3530000     	cmp	r3, #0
  6595d8: 0affffdd     	beq	0x659554 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xb4> @ imm = #-0x8c
  6595dc: e1a00003     	mov	r0, r3
  6595e0: e1a0100a     	mov	r1, r10
  6595e4: e5933000     	ldr	r3, [r3]
  6595e8: e28d201c     	add	r2, sp, #28
  6595ec: e1a0e00f     	mov	lr, pc
  6595f0: e593f034     	ldr	pc, [r3, #0x34]
  6595f4: e3500000     	cmp	r0, #0
  6595f8: e58d0010     	str	r0, [sp, #0x10]
  6595fc: 0affffd4     	beq	0x659554 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xb4> @ imm = #-0xb0
  659600: e59f20f8     	ldr	r2, [pc, #0xf8]         @ 0x659700 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x260>
  659604: e59d301c     	ldr	r3, [sp, #0x1c]
  659608: e28d7074     	add	r7, sp, #116
  65960c: e28dc05c     	add	r12, sp, #92
  659610: e59d100c     	ldr	r1, [sp, #0xc]
  659614: e08f2002     	add	r2, pc, r2
  659618: e1a00007     	mov	r0, r7
  65961c: e58dc00c     	str	r12, [sp, #0xc]
  659620: e58d3008     	str	r3, [sp, #0x8]
  659624: ebfc50a1     	bl	0x56d8b0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, char const*)> @ imm = #-0xebd7c
  659628: e1a0200a     	mov	r2, r10
  65962c: e59d000c     	ldr	r0, [sp, #0xc]
  659630: e1a01007     	mov	r1, r7
  659634: ebfc509d     	bl	0x56d8b0 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, char const*)> @ imm = #-0xebd8c
  659638: e59d3008     	ldr	r3, [sp, #0x8]
  65963c: e59dc014     	ldr	r12, [sp, #0x14]
  659640: e28d6024     	add	r6, sp, #36
  659644: e1a02003     	mov	r2, r3
  659648: e59d1010     	ldr	r1, [sp, #0x10]
  65964c: e59d3070     	ldr	r3, [sp, #0x70]
  659650: e1a00006     	mov	r0, r6
  659654: e58dc000     	str	r12, [sp]
  659658: ebfc5723     	bl	0x56f2ec <glitch::io::CMemoryReadFile::CMemoryReadFile(void*, long, char const*, bool)> @ imm = #-0xea374
  65965c: e59d0070     	ldr	r0, [sp, #0x70]
  659660: e59d200c     	ldr	r2, [sp, #0xc]
  659664: e1500002     	cmp	r0, r2
  659668: 0a000002     	beq	0x659678 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x1d8> @ imm = #0x8
  65966c: e3500000     	cmp	r0, #0
  659670: 0a000000     	beq	0x659678 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x1d8> @ imm = #0x0
  659674: ebf2db75     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x34922c
  659678: e59d0088     	ldr	r0, [sp, #0x88]
  65967c: e1500007     	cmp	r0, r7
  659680: 0a000002     	beq	0x659690 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x1f0> @ imm = #0x8
  659684: e3500000     	cmp	r0, #0
  659688: 0a000000     	beq	0x659690 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x1f0> @ imm = #0x0
  65968c: ebf2db6f     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x349244
  659690: e1a02006     	mov	r2, r6
  659694: e1a0300b     	mov	r3, r11
  659698: e28d0018     	add	r0, sp, #24
  65969c: e3a0c000     	mov	r12, #0
  6596a0: e1a01009     	mov	r1, r9
  6596a4: e58dc000     	str	r12, [sp]
  6596a8: ebfe4e85     	bl	0x5ed0c4 <glitch::video::CTextureManager::getTexture(glitch::io::IReadFile*, char const*, bool)> @ imm = #-0x6c5ec
  6596ac: e59d3018     	ldr	r3, [sp, #0x18]
  6596b0: e3530000     	cmp	r3, #0
  6596b4: 15932004     	ldrne	r2, [r3, #0x4]
  6596b8: 12822001     	addne	r2, r2, #1
  6596bc: 15832004     	strne	r2, [r3, #0x4]
  6596c0: e5950000     	ldr	r0, [r5]
  6596c4: e5853000     	str	r3, [r5]
  6596c8: e3500000     	cmp	r0, #0
  6596cc: 0a000000     	beq	0x6596d4 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x234> @ imm = #0x0
  6596d0: ebf30fab     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x33c154
  6596d4: e59d0018     	ldr	r0, [sp, #0x18]
  6596d8: e3500000     	cmp	r0, #0
  6596dc: 0a000000     	beq	0x6596e4 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0x244> @ imm = #0x0
  6596e0: ebf30fa7     	bl	0x31d584 <glitch::IReferenceCounted::drop() const> @ imm = #-0x33c164
  6596e4: e1a00006     	mov	r0, r6
  6596e8: ebfc56b1     	bl	0x56f1b4 <glitch::io::CMemoryReadFile::~CMemoryReadFile()> @ imm = #-0xea53c
  6596ec: eaffff98     	b	0x659554 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)+0xb4> @ imm = #-0x1a0
  6596f0: ebf2d306     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x34b3e8
  6596f4: e0 b5 33 00  	.word	0x0033b5e0
  6596f8: ac 40 00 00  	.word	0x000040ac
  6596fc: 80 77 26 00  	.word	0x00267780
  659700: 44 76 26 00  	.word	0x00267644

00659704 <glitch::collada::CResFactory::getTexture(glitch::collada::CResFile*, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, glitch::collada::SImage*)>:
  659704: e59f2054     	ldr	r2, [pc, #0x54]         @ 0x659760 <glitch::collada::CResFactory::getTexture(glitch::collada::CResFile*, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, glitch::collada::SImage*)+0x5c>
  659708: e92d4030     	push	{r4, r5, lr}
  65970c: e1a04000     	mov	r4, r0
  659710: e59f004c     	ldr	r0, [pc, #0x4c]         @ 0x659764 <glitch::collada::CResFactory::getTexture(glitch::collada::CResFile*, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, glitch::collada::SImage*)+0x60>
  659714: e08f2002     	add	r2, pc, r2
  659718: e24dd014     	sub	sp, sp, #20
  65971c: e7920000     	ldr	r0, [r2, r0]
  659720: e59d5024     	ldr	r5, [sp, #0x24]
  659724: e5902000     	ldr	r2, [r0]
  659728: e1a00004     	mov	r0, r4
  65972c: e5d2c029     	ldrb	r12, [r2, #0x29]
  659730: e59d2028     	ldr	r2, [sp, #0x28]
  659734: e35c0000     	cmp	r12, #0
  659738: 1592c000     	ldrne	r12, [r2]
  65973c: e592e008     	ldr	lr, [r2, #0x8]
  659740: e1a02003     	mov	r2, r3
  659744: e59d3020     	ldr	r3, [sp, #0x20]
  659748: e88d4020     	stm	sp, {r5, lr}
  65974c: e58dc008     	str	r12, [sp, #0x8]
  659750: ebffff52     	bl	0x6594a0 <glitch::collada::CResFactory::getTextureImpl(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, glitch::io::IReadFile*, glitch::video::CTextureManager*, char const*, char const*)> @ imm = #-0x2b8
  659754: e1a00004     	mov	r0, r4
  659758: e28dd014     	add	sp, sp, #20
  65975c: e8bd8030     	pop	{r4, r5, pc}
  659760: 7c b3 33 00  	.word	0x0033b37c
  659764: 48 44 00 00  	.word	0x00004448
