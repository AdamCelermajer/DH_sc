
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e96c <FileSystemBase::ApplyFilenameHacks(char const*) const>:
  34e96c: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34e970: e59f51d4     	ldr	r5, [pc, #0x1d4]        @ 0x34eb4c <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1e0>
  34e974: e59f81d4     	ldr	r8, [pc, #0x1d4]        @ 0x34eb50 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1e4>
  34e978: e1a04000     	mov	r4, r0
  34e97c: e08f5005     	add	r5, pc, r5
  34e980: e7953008     	ldr	r3, [r5, r8]
  34e984: e24dd054     	sub	sp, sp, #84
  34e988: e1a0a001     	mov	r10, r1
  34e98c: e5933000     	ldr	r3, [r3]
  34e990: e3a01010     	mov	r1, #16
  34e994: e5840010     	str	r0, [r4, #0x10]
  34e998: e5840014     	str	r0, [r4, #0x14]
  34e99c: e1a06002     	mov	r6, r2
  34e9a0: e58d304c     	str	r3, [sp, #0x4c]
  34e9a4: ebff47ff     	bl	0x3209a8 <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_allocate_block(unsigned int)> @ imm = #-0x2e004
  34e9a8: e5943010     	ldr	r3, [r4, #0x10]
  34e9ac: e3a02000     	mov	r2, #0
  34e9b0: e1a0000a     	mov	r0, r10
  34e9b4: e5c32000     	strb	r2, [r3]
  34e9b8: e59a3000     	ldr	r3, [r10]
  34e9bc: e1a0e00f     	mov	lr, pc
  34e9c0: e593f02c     	ldr	pc, [r3, #0x2c]
  34e9c4: e1a07000     	mov	r7, r0
  34e9c8: ebfefd21     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x40b7c
  34e9cc: e1a01007     	mov	r1, r7
  34e9d0: e1a09000     	mov	r9, r0
  34e9d4: e1a00006     	mov	r0, r6
  34e9d8: ebff007d     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x3fe0c
  34e9dc: e2507000     	subs	r7, r0, #0
  34e9e0: 0a000051     	beq	0x34eb2c <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1c0> @ imm = #0x144
  34e9e4: e289b001     	add	r11, r9, #1
  34e9e8: e087b00b     	add	r11, r7, r11
  34e9ec: e1a0000b     	mov	r0, r11
  34e9f0: ebfefd17     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x40ba4
  34e9f4: e1a0100b     	mov	r1, r11
  34e9f8: e08b2000     	add	r2, r11, r0
  34e9fc: e1a00004     	mov	r0, r4
  34ea00: ebff4860     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x2de80
  34ea04: e59f1148     	ldr	r1, [pc, #0x148]        @ 0x34eb54 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1e8>
  34ea08: e5940014     	ldr	r0, [r4, #0x14]
  34ea0c: e08f1001     	add	r1, pc, r1
  34ea10: ebff006f     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x3fe44
  34ea14: e3500000     	cmp	r0, #0
  34ea18: 0a000014     	beq	0x34ea70 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x104> @ imm = #0x50
  34ea1c: e28db034     	add	r11, sp, #52
  34ea20: e3a03001     	mov	r3, #1
  34ea24: e1a0100a     	mov	r1, r10
  34ea28: e1a0000b     	mov	r0, r11
  34ea2c: e1a02004     	mov	r2, r4
  34ea30: eb087742     	bl	0x56c740 <glitch::io::CFileSystem::getFileBasename(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, bool) const> @ imm = #0x21dd08
  34ea34: e59f111c     	ldr	r1, [pc, #0x11c]        @ 0x34eb58 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1ec>
  34ea38: e1a00004     	mov	r0, r4
  34ea3c: e08f1001     	add	r1, pc, r1
  34ea40: e2812010     	add	r2, r1, #16
  34ea44: ebff484f     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x2dec4
  34ea48: e1a00004     	mov	r0, r4
  34ea4c: e59d1048     	ldr	r1, [sp, #0x48]
  34ea50: e59d2044     	ldr	r2, [sp, #0x44]
  34ea54: ebff47fc     	bl	0x320a4c <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_append(char const*, char const*)> @ imm = #-0x2e010
  34ea58: e59d0048     	ldr	r0, [sp, #0x48]
  34ea5c: e150000b     	cmp	r0, r11
  34ea60: 0a000002     	beq	0x34ea70 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x104> @ imm = #0x8
  34ea64: e3500000     	cmp	r0, #0
  34ea68: 0a000000     	beq	0x34ea70 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x104> @ imm = #0x0
  34ea6c: ebff0677     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e624
  34ea70: e1a00004     	mov	r0, r4
  34ea74: e3a01000     	mov	r1, #0
  34ea78: e3e02000     	mvn	r2, #0
  34ea7c: ebfffd83     	bl	0x34e090 <ToLowerCase(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>&, int, int)> @ imm = #-0x9f4
  34ea80: e3570000     	cmp	r7, #0
  34ea84: 0a000020     	beq	0x34eb0c <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1a0> @ imm = #0x80
  34ea88: e2663001     	rsb	r3, r6, #1
  34ea8c: e0839009     	add	r9, r3, r9
  34ea90: e1a01006     	mov	r1, r6
  34ea94: e0877009     	add	r7, r7, r9
  34ea98: e28d601c     	add	r6, sp, #28
  34ea9c: e0812007     	add	r2, r1, r7
  34eaa0: e1a00006     	mov	r0, r6
  34eaa4: e28d7004     	add	r7, sp, #4
  34eaa8: e58d602c     	str	r6, [sp, #0x2c]
  34eaac: e58d6030     	str	r6, [sp, #0x30]
  34eab0: ebff5d4f     	bl	0x325ff4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_range_initialize(char const*, char const*)> @ imm = #-0x28ac4
  34eab4: e1a00007     	mov	r0, r7
  34eab8: e1a01006     	mov	r1, r6
  34eabc: e1a02004     	mov	r2, r4
  34eac0: ebfffe17     	bl	0x34e324 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&)> @ imm = #-0x7a4
  34eac4: e1540007     	cmp	r4, r7
  34eac8: 0a000003     	beq	0x34eadc <FileSystemBase::ApplyFilenameHacks(char const*) const+0x170> @ imm = #0xc
  34eacc: e1a00004     	mov	r0, r4
  34ead0: e59d1018     	ldr	r1, [sp, #0x18]
  34ead4: e59d2014     	ldr	r2, [sp, #0x14]
  34ead8: ebff482a     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x2df58
  34eadc: e59d0018     	ldr	r0, [sp, #0x18]
  34eae0: e1500007     	cmp	r0, r7
  34eae4: 0a000002     	beq	0x34eaf4 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x188> @ imm = #0x8
  34eae8: e3500000     	cmp	r0, #0
  34eaec: 0a000000     	beq	0x34eaf4 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x188> @ imm = #0x0
  34eaf0: ebff0656     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e6a8
  34eaf4: e59d0030     	ldr	r0, [sp, #0x30]
  34eaf8: e1500006     	cmp	r0, r6
  34eafc: 0a000002     	beq	0x34eb0c <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1a0> @ imm = #0x8
  34eb00: e3500000     	cmp	r0, #0
  34eb04: 0a000000     	beq	0x34eb0c <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1a0> @ imm = #0x0
  34eb08: ebff0650     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e6c0
  34eb0c: e7953008     	ldr	r3, [r5, r8]
  34eb10: e59d204c     	ldr	r2, [sp, #0x4c]
  34eb14: e1a00004     	mov	r0, r4
  34eb18: e5933000     	ldr	r3, [r3]
  34eb1c: e1520003     	cmp	r2, r3
  34eb20: 1a000008     	bne	0x34eb48 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x1dc> @ imm = #0x20
  34eb24: e28dd054     	add	sp, sp, #84
  34eb28: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34eb2c: e1a00006     	mov	r0, r6
  34eb30: ebfefcc7     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x40ce4
  34eb34: e1a01006     	mov	r1, r6
  34eb38: e0862000     	add	r2, r6, r0
  34eb3c: e1a00004     	mov	r0, r4
  34eb40: ebff4810     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x2dfc0
  34eb44: eaffffae     	b	0x34ea04 <FileSystemBase::ApplyFilenameHacks(char const*) const+0x98> @ imm = #-0x148
  34eb48: ebfefdf0     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x40840
  34eb4c: 14 61 64 00  	.word	0x00646114
  34eb50: ac 40 00 00  	.word	0x000040ac
  34eb54: 2c 97 57 00  	.word	0x0057972c
  34eb58: fc 1c 57 00  	.word	0x00571cfc

0034eb5c <IFileStream::~IFileStream()>:
  34eb5c: e12fff1e     	bx	lr

0034eb60 <IFileStream::skip(unsigned long long)>:
  34eb60: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  34eb64: e5901000     	ldr	r1, [r0]
  34eb68: e1a04000     	mov	r4, r0
  34eb6c: e1a06002     	mov	r6, r2
  34eb70: e1a07003     	mov	r7, r3
  34eb74: e5915020     	ldr	r5, [r1, #0x20]
  34eb78: e1a0e00f     	mov	lr, pc
  34eb7c: e591f024     	ldr	pc, [r1, #0x24]
  34eb80: e0902006     	adds	r2, r0, r6
  34eb84: e0a13007     	adc	r3, r1, r7
  34eb88: e1a00004     	mov	r0, r4
  34eb8c: e12fff35     	blx	r5
  34eb90: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}

0034eb94 <FileSystemWin32::_FileHandle::canRead() const>:
  34eb94: e5d0000c     	ldrb	r0, [r0, #0xc]
  34eb98: e12fff1e     	bx	lr

0034eb9c <FileSystemWin32::_FileHandle::canWrite() const>:
  34eb9c: e5d0000d     	ldrb	r0, [r0, #0xd]
  34eba0: e12fff1e     	bx	lr

0034eba4 <FileSystemWin32::getRootPath() const>:
  34eba4: e280002c     	add	r0, r0, #44
  34eba8: e12fff1e     	bx	lr

0034ebac <FileSystemWin32::getResourcesPath() const>:
  34ebac: e2800f8d     	add	r0, r0, #564
  34ebb0: e12fff1e     	bx	lr

0034ebb4 <FileSystemWin32::getSavefilesPath() const>:
  34ebb4: e2800e43     	add	r0, r0, #1072
  34ebb8: e280000c     	add	r0, r0, #12
