
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034f8a4 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const>:
  34f8a4: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  34f8a8: e1a04000     	mov	r4, r0
  34f8ac: e5840010     	str	r0, [r4, #0x10]
  34f8b0: e5840014     	str	r0, [r4, #0x14]
  34f8b4: e1a03002     	mov	r3, r2
  34f8b8: e24dd02c     	sub	sp, sp, #44
  34f8bc: e5931014     	ldr	r1, [r3, #0x14]
  34f8c0: e5922010     	ldr	r2, [r2, #0x10]
  34f8c4: ebff59ca     	bl	0x325ff4 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_range_initialize(char const*, char const*)> @ imm = #-0x298d8
  34f8c8: e59f328c     	ldr	r3, [pc, #0x28c]        @ 0x34fb5c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2b8>
  34f8cc: e59f128c     	ldr	r1, [pc, #0x28c]        @ 0x34fb60 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2bc>
  34f8d0: e59f228c     	ldr	r2, [pc, #0x28c]        @ 0x34fb64 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2c0>
  34f8d4: e08f3003     	add	r3, pc, r3
  34f8d8: e08f1001     	add	r1, pc, r1
  34f8dc: e58d1010     	str	r1, [sp, #0x10]
  34f8e0: e2833001     	add	r3, r3, #1
  34f8e4: e59fb27c     	ldr	r11, [pc, #0x27c]       @ 0x34fb68 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2c4>
  34f8e8: e58d301c     	str	r3, [sp, #0x1c]
  34f8ec: e59d3010     	ldr	r3, [sp, #0x10]
  34f8f0: e08f2002     	add	r2, pc, r2
  34f8f4: e2822001     	add	r2, r2, #1
  34f8f8: e08fb00b     	add	r11, pc, r11
  34f8fc: e58d200c     	str	r2, [sp, #0xc]
  34f900: e2833001     	add	r3, r3, #1
  34f904: e28b2001     	add	r2, r11, #1
  34f908: e5941010     	ldr	r1, [r4, #0x10]
  34f90c: e5940014     	ldr	r0, [r4, #0x14]
  34f910: e3a07000     	mov	r7, #0
  34f914: e58d2014     	str	r2, [sp, #0x14]
  34f918: e58d3018     	str	r3, [sp, #0x18]
  34f91c: e0603001     	rsb	r3, r0, r1
  34f920: e1570003     	cmp	r7, r3
  34f924: 3a000002     	blo	0x34f934 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x90> @ imm = #0x8
  34f928: e1a00004     	mov	r0, r4
  34f92c: e28dd02c     	add	sp, sp, #44
  34f930: e8bd8ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, pc}
  34f934: e28d2028     	add	r2, sp, #40
  34f938: e3a0302f     	mov	r3, #47
  34f93c: e5623004     	strb	r3, [r2, #-0x4]!
  34f940: e0800007     	add	r0, r0, r7
  34f944: e28d3020     	add	r3, sp, #32
  34f948: ebfffcad     	bl	0x34ec04 <char const* std::priv::__find_if<char const*, std::priv::_Eq_char_bound<std::char_traits<char>>>(char const*, char const*, std::priv::_Eq_char_bound<std::char_traits<char>>, std::random_access_iterator_tag const&)> @ imm = #-0xd4c
  34f94c: e5942010     	ldr	r2, [r4, #0x10]
  34f950: e1500002     	cmp	r0, r2
  34f954: 0afffff3     	beq	0x34f928 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x84> @ imm = #-0x34
  34f958: e5945014     	ldr	r5, [r4, #0x14]
  34f95c: e0653000     	rsb	r3, r5, r0
  34f960: e3730001     	cmn	r3, #1
  34f964: 0affffef     	beq	0x34f928 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x84> @ imm = #-0x44
  34f968: e0655002     	rsb	r5, r5, r2
  34f96c: e1530005     	cmp	r3, r5
  34f970: e2837001     	add	r7, r3, #1
  34f974: 8a000059     	bhi	0x34fae0 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x23c> @ imm = #0x164
  34f978: e30f2ffe     	movw	r2, #0xfffe
  34f97c: e34f2fff     	movt	r2, #0xffff
  34f980: e0638005     	rsb	r8, r3, r5
  34f984: e3580001     	cmp	r8, #1
  34f988: 23a08001     	movhs	r8, #1
  34f98c: e0652002     	rsb	r2, r5, r2
  34f990: e0822008     	add	r2, r2, r8
  34f994: e3520000     	cmp	r2, #0
  34f998: 0a00004a     	beq	0x34fac8 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x224> @ imm = #0x128
  34f99c: e59f21c8     	ldr	r2, [pc, #0x1c8]        @ 0x34fb6c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2c8>
  34f9a0: e5945014     	ldr	r5, [r4, #0x14]
  34f9a4: e0888003     	add	r8, r8, r3
  34f9a8: e08f2002     	add	r2, pc, r2
  34f9ac: e1550002     	cmp	r5, r2
  34f9b0: e0858008     	add	r8, r5, r8
  34f9b4: e0856003     	add	r6, r5, r3
  34f9b8: 83a09000     	movhi	r9, #0
  34f9bc: 8a000003     	bhi	0x34f9d0 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x12c> @ imm = #0xc
  34f9c0: e5949010     	ldr	r9, [r4, #0x10]
  34f9c4: e1590002     	cmp	r9, r2
  34f9c8: 93a09000     	movls	r9, #0
  34f9cc: 83a09001     	movhi	r9, #1
  34f9d0: e066a008     	rsb	r10, r6, r8
  34f9d4: e35a0000     	cmp	r10, #0
  34f9d8: da000011     	ble	0x34fa24 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x180> @ imm = #0x44
  34f9dc: e2866001     	add	r6, r6, #1
  34f9e0: e3a0205c     	mov	r2, #92
  34f9e4: e1580006     	cmp	r8, r6
  34f9e8: e7c52003     	strb	r2, [r5, r3]
  34f9ec: 0a000022     	beq	0x34fa7c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x1d8> @ imm = #0x88
  34f9f0: e5943010     	ldr	r3, [r4, #0x10]
  34f9f4: e2832001     	add	r2, r3, #1
  34f9f8: e0522008     	subs	r2, r2, r8
  34f9fc: 0a000003     	beq	0x34fa10 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x16c> @ imm = #0xc
  34fa00: e1a00006     	mov	r0, r6
  34fa04: e1a01008     	mov	r1, r8
  34fa08: ebfef94a     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x41ad8
  34fa0c: e5943010     	ldr	r3, [r4, #0x10]
  34fa10: e0686006     	rsb	r6, r8, r6
  34fa14: e0831006     	add	r1, r3, r6
  34fa18: e5841010     	str	r1, [r4, #0x10]
  34fa1c: e5940014     	ldr	r0, [r4, #0x14]
  34fa20: eaffffbd     	b	0x34f91c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x78> @ imm = #-0x10c
  34fa24: e3590000     	cmp	r9, #0
  34fa28: 0a000016     	beq	0x34fa88 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x1e4> @ imm = #0x58
  34fa2c: e59d2014     	ldr	r2, [sp, #0x14]
  34fa30: e1560002     	cmp	r6, r2
  34fa34: 33a03000     	movlo	r3, #0
  34fa38: 23a03001     	movhs	r3, #1
  34fa3c: e158000b     	cmp	r8, r11
  34fa40: 93833001     	orrls	r3, r3, #1
  34fa44: e3530000     	cmp	r3, #0
  34fa48: 1a00000e     	bne	0x34fa88 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x1e4> @ imm = #0x38
  34fa4c: e156000b     	cmp	r6, r11
  34fa50: 8a000028     	bhi	0x34faf8 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x254> @ imm = #0xa0
  34fa54: e35a0000     	cmp	r10, #0
  34fa58: e08a500b     	add	r5, r10, r11
  34fa5c: 1a000039     	bne	0x34fb48 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2a4> @ imm = #0xe4
  34fa60: e3a0c001     	mov	r12, #1
  34fa64: e1a01008     	mov	r1, r8
  34fa68: e1a02005     	mov	r2, r5
  34fa6c: e1a00004     	mov	r0, r4
  34fa70: e59d301c     	ldr	r3, [sp, #0x1c]
  34fa74: e58dc000     	str	r12, [sp]
  34fa78: ebfffe52     	bl	0x34f3c8 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert(char*, char const*, char const*, bool)> @ imm = #-0x6b8
  34fa7c: e5941010     	ldr	r1, [r4, #0x10]
  34fa80: e5940014     	ldr	r0, [r4, #0x14]
  34fa84: eaffffa4     	b	0x34f91c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x78> @ imm = #-0x170
  34fa88: e59f10e0     	ldr	r1, [pc, #0xe0]         @ 0x34fb70 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2cc>
  34fa8c: e08f1001     	add	r1, pc, r1
  34fa90: e08aa001     	add	r10, r10, r1
  34fa94: e05a200b     	subs	r2, r10, r11
  34fa98: 0a000001     	beq	0x34faa4 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x200> @ imm = #0x4
  34fa9c: e1a00006     	mov	r0, r6
  34faa0: ebfefb70     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x41240
  34faa4: e1a01008     	mov	r1, r8
  34faa8: e1a00004     	mov	r0, r4
  34faac: e1a0200a     	mov	r2, r10
  34fab0: e59d300c     	ldr	r3, [sp, #0xc]
  34fab4: e58d9000     	str	r9, [sp]
  34fab8: ebfffe42     	bl	0x34f3c8 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert(char*, char const*, char const*, bool)> @ imm = #-0x6f8
  34fabc: e5941010     	ldr	r1, [r4, #0x10]
  34fac0: e5940014     	ldr	r0, [r4, #0x14]
  34fac4: eaffff94     	b	0x34f91c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x78> @ imm = #-0x1b0
  34fac8: e59f00a4     	ldr	r0, [pc, #0xa4]         @ 0x34fb74 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2d0>
  34facc: e58d3008     	str	r3, [sp, #0x8]
  34fad0: e08f0000     	add	r0, pc, r0
  34fad4: eb0ee4d9     	bl	0x708e40 <___ZSt24__stl_throw_length_errorPKc_veneer> @ imm = #0x3b9364
  34fad8: e59d3008     	ldr	r3, [sp, #0x8]
  34fadc: eaffffae     	b	0x34f99c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0xf8> @ imm = #-0x148
  34fae0: e59f0090     	ldr	r0, [pc, #0x90]         @ 0x34fb78 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x2d4>
  34fae4: e58d3008     	str	r3, [sp, #0x8]
  34fae8: e08f0000     	add	r0, pc, r0
  34faec: eb0ee4ef     	bl	0x708eb0 <___ZSt24__stl_throw_out_of_rangePKc_veneer> @ imm = #0x3b93bc
  34faf0: e59d3008     	ldr	r3, [sp, #0x8]
  34faf4: eaffff9f     	b	0x34f978 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0xd4> @ imm = #-0x184
  34faf8: e1a01008     	mov	r1, r8
  34fafc: e1a00004     	mov	r0, r4
  34fb00: e3a0c001     	mov	r12, #1
  34fb04: e08a200b     	add	r2, r10, r11
  34fb08: e59d3014     	ldr	r3, [sp, #0x14]
  34fb0c: e58dc000     	str	r12, [sp]
  34fb10: ebfffe2c     	bl	0x34f3c8 <std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>>::_M_insert(char*, char const*, char const*, bool)> @ imm = #-0x750
  34fb14: e35a0000     	cmp	r10, #0
  34fb18: e5940014     	ldr	r0, [r4, #0x14]
  34fb1c: 05941010     	ldreq	r1, [r4, #0x10]
  34fb20: 0affff7d     	beq	0x34f91c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x78> @ imm = #-0x20c
  34fb24: e0656006     	rsb	r6, r5, r6
  34fb28: e065100b     	rsb	r1, r5, r11
  34fb2c: e0801001     	add	r1, r0, r1
  34fb30: e1a0200a     	mov	r2, r10
  34fb34: e0800006     	add	r0, r0, r6
  34fb38: ebfef8fe     	bl	0x30df38 <.plt+0x1c4>   @ imm = #-0x41c08
  34fb3c: e5941010     	ldr	r1, [r4, #0x10]
  34fb40: e5940014     	ldr	r0, [r4, #0x14]
  34fb44: eaffff74     	b	0x34f91c <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x78> @ imm = #-0x230
  34fb48: e1a00006     	mov	r0, r6
  34fb4c: e1a0200a     	mov	r2, r10
  34fb50: e1a0100b     	mov	r1, r11
  34fb54: ebfefb43     	bl	0x30e868 <.plt+0xaf4>   @ imm = #-0x412f4
  34fb58: eaffffc0     	b	0x34fa60 <FileSystemWin32::formatFilePath(std::basic_string<char, std::char_traits<char>, glitch::core::SAllocator<char, (glitch::memory::E_MEMORY_HINT)0>> const&) const+0x1bc> @ imm = #-0x100
  34fb5c: a4 0e 57 00  	.word	0x00570ea4
  34fb60: a0 0e 57 00  	.word	0x00570ea0
  34fb64: 88 0e 57 00  	.word	0x00570e88
  34fb68: 80 0e 57 00  	.word	0x00570e80
  34fb6c: d0 0d 57 00  	.word	0x00570dd0
  34fb70: ec 0c 57 00  	.word	0x00570cec
  34fb74: 88 e9 56 00  	.word	0x0056e988
  34fb78: 70 e9 56 00  	.word	0x0056e970

0034fb7c <FileSystemWin32::_FileHandle::~_FileHandle()>:
  34fb7c: e92d4010     	push	{r4, lr}
  34fb80: e59f302c     	ldr	r3, [pc, #0x2c]         @ 0x34fbb4 <FileSystemWin32::_FileHandle::~_FileHandle()+0x38>
  34fb84: e59f202c     	ldr	r2, [pc, #0x2c]         @ 0x34fbb8 <FileSystemWin32::_FileHandle::~_FileHandle()+0x3c>
  34fb88: e1a04000     	mov	r4, r0
  34fb8c: e08f3003     	add	r3, pc, r3
  34fb90: e5900008     	ldr	r0, [r0, #0x8]
  34fb94: e7932002     	ldr	r2, [r3, r2]
  34fb98: e3500000     	cmp	r0, #0
  34fb9c: e2822008     	add	r2, r2, #8
  34fba0: e5842000     	str	r2, [r4]
  34fba4: 0a000000     	beq	0x34fbac <FileSystemWin32::_FileHandle::~_FileHandle()+0x30> @ imm = #0x0
  34fba8: ebfffd22     	bl	0x34f038 <glitch::ISharedObject<glitch::io::CFile>::drop() const> @ imm = #-0xb78
  34fbac: e1a00004     	mov	r0, r4
  34fbb0: e8bd8010     	pop	{r4, pc}
  34fbb4: 04 4f 64 00  	.word	0x00644f04
  34fbb8: 00 1c 00 00  	.word	0x00001c00
