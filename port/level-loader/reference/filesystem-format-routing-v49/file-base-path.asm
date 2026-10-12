
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e5f4 <FileSystemBase::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const>:
  34e5f4: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  34e5f8: e59f4080     	ldr	r4, [pc, #0x80]         @ 0x34e680 <FileSystemBase::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x8c>
  34e5fc: e59f7080     	ldr	r7, [pc, #0x80]         @ 0x34e684 <FileSystemBase::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x90>
  34e600: e24dd020     	sub	sp, sp, #32
  34e604: e08f4004     	add	r4, pc, r4
  34e608: e7943007     	ldr	r3, [r4, r7]
  34e60c: e28d5004     	add	r5, sp, #4
  34e610: e1a08001     	mov	r8, r1
  34e614: e593c000     	ldr	r12, [r3]
  34e618: e5913000     	ldr	r3, [r1]
  34e61c: e1a06000     	mov	r6, r0
  34e620: e58dc01c     	str	r12, [sp, #0x1c]
  34e624: e1a00005     	mov	r0, r5
  34e628: e5922014     	ldr	r2, [r2, #0x14]
  34e62c: e1a0e00f     	mov	lr, pc
  34e630: e593f0b8     	ldr	pc, [r3, #0xb8]
  34e634: e1a00006     	mov	r0, r6
  34e638: e1a01008     	mov	r1, r8
  34e63c: e1a02005     	mov	r2, r5
  34e640: eb087740     	bl	0x56c348 <glitch::io::CFileSystem::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const> @ imm = #0x21dd00
  34e644: e59d0018     	ldr	r0, [sp, #0x18]
  34e648: e1500005     	cmp	r0, r5
  34e64c: 0a000002     	beq	0x34e65c <FileSystemBase::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x68> @ imm = #0x8
  34e650: e3500000     	cmp	r0, #0
  34e654: 0a000000     	beq	0x34e65c <FileSystemBase::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x68> @ imm = #0x0
  34e658: ebff077c     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e210
  34e65c: e7943007     	ldr	r3, [r4, r7]
  34e660: e59d201c     	ldr	r2, [sp, #0x1c]
  34e664: e1a00006     	mov	r0, r6
  34e668: e5933000     	ldr	r3, [r3]
  34e66c: e1520003     	cmp	r2, r3
  34e670: 1a000001     	bne	0x34e67c <FileSystemBase::getAbsolutePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x88> @ imm = #0x4
  34e674: e28dd020     	add	sp, sp, #32
  34e678: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  34e67c: ebfeff23     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x40374
  34e680: 8c 64 64 00  	.word	0x0064648c
  34e684: ac 40 00 00  	.word	0x000040ac
