
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034e688 <FileSystemBase::createAndOpenFile(char const*)>:
  34e688: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34e68c: e59f42b0     	ldr	r4, [pc, #0x2b0]        @ 0x34e944 <FileSystemBase::createAndOpenFile(char const*)+0x2bc>
  34e690: e59f92b0     	ldr	r9, [pc, #0x2b0]        @ 0x34e948 <FileSystemBase::createAndOpenFile(char const*)+0x2c0>
  34e694: e24dd0a4     	sub	sp, sp, #164
  34e698: e08f4004     	add	r4, pc, r4
  34e69c: e7943009     	ldr	r3, [r4, r9]
  34e6a0: e1a05000     	mov	r5, r0
  34e6a4: e28da084     	add	r10, sp, #132
  34e6a8: e5933000     	ldr	r3, [r3]
  34e6ac: e1a02001     	mov	r2, r1
  34e6b0: e28d606c     	add	r6, sp, #108
  34e6b4: e58d309c     	str	r3, [sp, #0x9c]
  34e6b8: e5953000     	ldr	r3, [r5]
  34e6bc: e1a0000a     	mov	r0, r10
  34e6c0: e1a01005     	mov	r1, r5
  34e6c4: e1a0e00f     	mov	lr, pc
  34e6c8: e593f0b8     	ldr	pc, [r3, #0xb8]
  34e6cc: e1a00006     	mov	r0, r6
  34e6d0: e3a01010     	mov	r1, #16
  34e6d4: e58d607c     	str	r6, [sp, #0x7c]
  34e6d8: e58d6080     	str	r6, [sp, #0x80]
  34e6dc: ebff48b1     	bl	0x3209a8 <std::priv::_String_base<char, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_allocate_block(unsigned int)> @ imm = #-0x2dd3c
  34e6e0: e59d307c     	ldr	r3, [sp, #0x7c]
  34e6e4: e3a02000     	mov	r2, #0
  34e6e8: e28d7054     	add	r7, sp, #84
  34e6ec: e5c32000     	strb	r2, [r3]
  34e6f0: e59d3098     	ldr	r3, [sp, #0x98]
  34e6f4: e59d2094     	ldr	r2, [sp, #0x94]
  34e6f8: e1a00007     	mov	r0, r7
  34e6fc: e1a01003     	mov	r1, r3
  34e700: e0633002     	rsb	r3, r3, r2
  34e704: e3530008     	cmp	r3, #8
  34e708: 90812003     	addls	r2, r1, r3
  34e70c: 82812008     	addhi	r2, r1, #8
  34e710: e58d7064     	str	r7, [sp, #0x64]
  34e714: e58d7068     	str	r7, [sp, #0x68]
  34e718: ebff5e35     	bl	0x325ff4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_range_initialize(char const*, char const*)> @ imm = #-0x2872c
  34e71c: e59d8098     	ldr	r8, [sp, #0x98]
  34e720: e59f1224     	ldr	r1, [pc, #0x224]        @ 0x34e94c <FileSystemBase::createAndOpenFile(char const*)+0x2c4>
  34e724: e1a00008     	mov	r0, r8
  34e728: e08f1001     	add	r1, pc, r1
  34e72c: ebff0128     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x3fb60
  34e730: e3500000     	cmp	r0, #0
  34e734: 0a000059     	beq	0x34e8a0 <FileSystemBase::createAndOpenFile(char const*)+0x218> @ imm = #0x164
  34e738: e1a01008     	mov	r1, r8
  34e73c: e1a00006     	mov	r0, r6
  34e740: e59d2094     	ldr	r2, [sp, #0x94]
  34e744: ebff490f     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x2dbc4
  34e748: e28d8024     	add	r8, sp, #36
  34e74c: e1a00008     	mov	r0, r8
  34e750: e59d1080     	ldr	r1, [sp, #0x80]
  34e754: e59d207c     	ldr	r2, [sp, #0x7c]
  34e758: e58d8034     	str	r8, [sp, #0x34]
  34e75c: e58d8038     	str	r8, [sp, #0x38]
  34e760: ebff5e23     	bl	0x325ff4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_range_initialize(char const*, char const*)> @ imm = #-0x28774
  34e764: e59f31e4     	ldr	r3, [pc, #0x1e4]        @ 0x34e950 <FileSystemBase::createAndOpenFile(char const*)+0x2c8>
  34e768: e59d1038     	ldr	r1, [sp, #0x38]
  34e76c: e7940003     	ldr	r0, [r4, r3]
  34e770: ebff47c0     	bl	0x320678 <Application::IsUsingUncompiledData(char const*) const> @ imm = #-0x2e100
  34e774: e3500000     	cmp	r0, #0
  34e778: 0a000022     	beq	0x34e808 <FileSystemBase::createAndOpenFile(char const*)+0x180> @ imm = #0x88
  34e77c: e5953000     	ldr	r3, [r5]
  34e780: e1a00005     	mov	r0, r5
  34e784: e1a0e00f     	mov	lr, pc
  34e788: e593f02c     	ldr	pc, [r3, #0x2c]
  34e78c: e28db00c     	add	r11, sp, #12
  34e790: e58db01c     	str	r11, [sp, #0x1c]
  34e794: e58db020     	str	r11, [sp, #0x20]
  34e798: e58d0000     	str	r0, [sp]
  34e79c: ebfefdac     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x40950
  34e7a0: e59d1000     	ldr	r1, [sp]
  34e7a4: e0812000     	add	r2, r1, r0
  34e7a8: e1a0000b     	mov	r0, r11
  34e7ac: ebff0bcd     	bl	0x3116e8 <std::string::_M_range_initialize(char const*, char const*)> @ imm = #-0x3d0cc
  34e7b0: e59f119c     	ldr	r1, [pc, #0x19c]        @ 0x34e954 <FileSystemBase::createAndOpenFile(char const*)+0x2cc>
  34e7b4: e5953000     	ldr	r3, [r5]
  34e7b8: e1a00005     	mov	r0, r5
  34e7bc: e08f1001     	add	r1, pc, r1
  34e7c0: e1a0e00f     	mov	lr, pc
  34e7c4: e593f030     	ldr	pc, [r3, #0x30]
  34e7c8: e1a00005     	mov	r0, r5
  34e7cc: e59d1038     	ldr	r1, [sp, #0x38]
  34e7d0: eb087b42     	bl	0x56d4e0 <glitch::io::CFileSystem::createAndOpenFile(char const*)> @ imm = #0x21ed08
  34e7d4: e3500000     	cmp	r0, #0
  34e7d8: e58d0004     	str	r0, [sp, #0x4]
  34e7dc: 0a000002     	beq	0x34e7ec <FileSystemBase::createAndOpenFile(char const*)+0x164> @ imm = #0x8
  34e7e0: e1a0000b     	mov	r0, r11
  34e7e4: ebff1470     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x3ae40
  34e7e8: ea00000c     	b	0x34e820 <FileSystemBase::createAndOpenFile(char const*)+0x198> @ imm = #0x30
  34e7ec: e5953000     	ldr	r3, [r5]
  34e7f0: e1a00005     	mov	r0, r5
  34e7f4: e59d1020     	ldr	r1, [sp, #0x20]
  34e7f8: e1a0e00f     	mov	lr, pc
  34e7fc: e593f030     	ldr	pc, [r3, #0x30]
  34e800: e1a0000b     	mov	r0, r11
  34e804: ebff1468     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x3ae60
  34e808: e1a00005     	mov	r0, r5
  34e80c: e59d1038     	ldr	r1, [sp, #0x38]
  34e810: eb087b32     	bl	0x56d4e0 <glitch::io::CFileSystem::createAndOpenFile(char const*)> @ imm = #0x21ecc8
  34e814: e3500000     	cmp	r0, #0
  34e818: e58d0004     	str	r0, [sp, #0x4]
  34e81c: 0a00003d     	beq	0x34e918 <FileSystemBase::createAndOpenFile(char const*)+0x290> @ imm = #0xf4
  34e820: e59d0038     	ldr	r0, [sp, #0x38]
  34e824: e1500008     	cmp	r0, r8
  34e828: 0a000002     	beq	0x34e838 <FileSystemBase::createAndOpenFile(char const*)+0x1b0> @ imm = #0x8
  34e82c: e3500000     	cmp	r0, #0
  34e830: 0a000000     	beq	0x34e838 <FileSystemBase::createAndOpenFile(char const*)+0x1b0> @ imm = #0x0
  34e834: ebff0705     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e3ec
  34e838: e59d0068     	ldr	r0, [sp, #0x68]
  34e83c: e1500007     	cmp	r0, r7
  34e840: 0a000002     	beq	0x34e850 <FileSystemBase::createAndOpenFile(char const*)+0x1c8> @ imm = #0x8
  34e844: e3500000     	cmp	r0, #0
  34e848: 0a000000     	beq	0x34e850 <FileSystemBase::createAndOpenFile(char const*)+0x1c8> @ imm = #0x0
  34e84c: ebff06ff     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e404
  34e850: e59d0080     	ldr	r0, [sp, #0x80]
  34e854: e1500006     	cmp	r0, r6
  34e858: 0a000002     	beq	0x34e868 <FileSystemBase::createAndOpenFile(char const*)+0x1e0> @ imm = #0x8
  34e85c: e3500000     	cmp	r0, #0
  34e860: 0a000000     	beq	0x34e868 <FileSystemBase::createAndOpenFile(char const*)+0x1e0> @ imm = #0x0
  34e864: ebff06f9     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e41c
  34e868: e59d0098     	ldr	r0, [sp, #0x98]
  34e86c: e150000a     	cmp	r0, r10
  34e870: 0a000002     	beq	0x34e880 <FileSystemBase::createAndOpenFile(char const*)+0x1f8> @ imm = #0x8
  34e874: e3500000     	cmp	r0, #0
  34e878: 0a000000     	beq	0x34e880 <FileSystemBase::createAndOpenFile(char const*)+0x1f8> @ imm = #0x0
  34e87c: ebff06f3     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e434
  34e880: e7943009     	ldr	r3, [r4, r9]
  34e884: e59d209c     	ldr	r2, [sp, #0x9c]
  34e888: e59d0004     	ldr	r0, [sp, #0x4]
  34e88c: e5933000     	ldr	r3, [r3]
  34e890: e1520003     	cmp	r2, r3
  34e894: 1a000029     	bne	0x34e940 <FileSystemBase::createAndOpenFile(char const*)+0x2b8> @ imm = #0xa4
  34e898: e28dd0a4     	add	sp, sp, #164
  34e89c: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34e8a0: e59f10b0     	ldr	r1, [pc, #0xb0]         @ 0x34e958 <FileSystemBase::createAndOpenFile(char const*)+0x2d0>
  34e8a4: e1a00008     	mov	r0, r8
  34e8a8: e08f1001     	add	r1, pc, r1
  34e8ac: ebff00c8     	bl	0x30ebd4 <.plt+0xe60>   @ imm = #-0x3fce0
  34e8b0: e3500000     	cmp	r0, #0
  34e8b4: 1affff9f     	bne	0x34e738 <FileSystemBase::createAndOpenFile(char const*)+0xb0> @ imm = #-0x184
  34e8b8: e59f109c     	ldr	r1, [pc, #0x9c]         @ 0x34e95c <FileSystemBase::createAndOpenFile(char const*)+0x2d4>
  34e8bc: e59d0068     	ldr	r0, [sp, #0x68]
  34e8c0: e08f1001     	add	r1, pc, r1
  34e8c4: ebfefe94     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0x405b0
  34e8c8: e3500000     	cmp	r0, #0
  34e8cc: 0affff99     	beq	0x34e738 <FileSystemBase::createAndOpenFile(char const*)+0xb0> @ imm = #-0x19c
  34e8d0: e59f3088     	ldr	r3, [pc, #0x88]         @ 0x34e960 <FileSystemBase::createAndOpenFile(char const*)+0x2d8>
  34e8d4: e28d803c     	add	r8, sp, #60
  34e8d8: e1a0200a     	mov	r2, r10
  34e8dc: e7943003     	ldr	r3, [r4, r3]
  34e8e0: e1a00008     	mov	r0, r8
  34e8e4: e5931000     	ldr	r1, [r3]
  34e8e8: ebfffea9     	bl	0x34e394 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> std::operator+<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>(char const*, std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&)> @ imm = #-0x55c
  34e8ec: e1a00006     	mov	r0, r6
  34e8f0: e59d1050     	ldr	r1, [sp, #0x50]
  34e8f4: e59d204c     	ldr	r2, [sp, #0x4c]
  34e8f8: ebff48a2     	bl	0x320b88 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_assign(char const*, char const*)> @ imm = #-0x2dd78
  34e8fc: e59d0050     	ldr	r0, [sp, #0x50]
  34e900: e1500008     	cmp	r0, r8
  34e904: 0affff8f     	beq	0x34e748 <FileSystemBase::createAndOpenFile(char const*)+0xc0> @ imm = #-0x1c4
  34e908: e3500000     	cmp	r0, #0
  34e90c: 0affff8d     	beq	0x34e748 <FileSystemBase::createAndOpenFile(char const*)+0xc0> @ imm = #-0x1cc
  34e910: ebff06ce     	bl	0x310450 <GlitchFree(void*)> @ imm = #-0x3e4c8
  34e914: eaffff8b     	b	0x34e748 <FileSystemBase::createAndOpenFile(char const*)+0xc0> @ imm = #-0x1d4
  34e918: e59f0044     	ldr	r0, [pc, #0x44]         @ 0x34e964 <FileSystemBase::createAndOpenFile(char const*)+0x2dc>
  34e91c: e59d1038     	ldr	r1, [sp, #0x38]
  34e920: e08f0000     	add	r0, pc, r0
  34e924: ebff55fa     	bl	0x324114 <_DEBUG_OUT(char const*, ...)> @ imm = #-0x2a818
  34e928: e59f0038     	ldr	r0, [pc, #0x38]         @ 0x34e968 <FileSystemBase::createAndOpenFile(char const*)+0x2e0>
  34e92c: e59d1038     	ldr	r1, [sp, #0x38]
  34e930: e3a02003     	mov	r2, #3
  34e934: e08f0000     	add	r0, pc, r0
  34e938: eb0af0ea     	bl	0x60ace8 <glitch::os::Printer::log(char const*, char const*, glitch::ELOG_LEVEL)> @ imm = #0x2bc3a8
  34e93c: eaffffb7     	b	0x34e820 <FileSystemBase::createAndOpenFile(char const*)+0x198> @ imm = #-0x124
  34e940: ebfefe72     	bl	0x30e310 <.plt+0x59c>   @ imm = #-0x40638
  34e944: f8 63 64 00  	.word	0x006463f8
  34e948: ac 40 00 00  	.word	0x000040ac
  34e94c: b0 1f 57 00  	.word	0x00571fb0
  34e950: f4 37 00 00  	.word	0x000037f4
  34e954: 3c 1f 57 00  	.word	0x00571f3c
  34e958: 38 1e 57 00  	.word	0x00571e38
  34e95c: 28 1e 57 00  	.word	0x00571e28
  34e960: 00 06 00 00  	.word	0x00000600
  34e964: e0 1d 57 00  	.word	0x00571de0
  34e968: ec 1d 57 00  	.word	0x00571dec
