
.local-inputs/libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0032d79c <Application::Application()>:
  32d79c: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  32d7a0: e59f61bc     	ldr	r6, [pc, #0x1bc]        @ 0x32d964 <Application::Application()+0x1c8>
  32d7a4: e59f31bc     	ldr	r3, [pc, #0x1bc]        @ 0x32d968 <Application::Application()+0x1cc>
  32d7a8: e1a02000     	mov	r2, r0
  32d7ac: e08f6006     	add	r6, pc, r6
  32d7b0: e7963003     	ldr	r3, [r6, r3]
  32d7b4: e3a0101e     	mov	r1, #30
  32d7b8: e1a04000     	mov	r4, r0
  32d7bc: e2833008     	add	r3, r3, #8
  32d7c0: e4823008     	str	r3, [r2], #8
  32d7c4: e580106c     	str	r1, [r0, #0x6c]
  32d7c8: e3a015fe     	mov	r1, #1065353216
  32d7cc: e3a05000     	mov	r5, #0
  32d7d0: e580109c     	str	r1, [r0, #0x9c]
  32d7d4: e3a07001     	mov	r7, #1
  32d7d8: e28030bc     	add	r3, r0, #188
  32d7dc: e3e01000     	mvn	r1, #0
  32d7e0: e580200c     	str	r2, [r0, #0xc]
  32d7e4: e5802008     	str	r2, [r0, #0x8]
  32d7e8: e58010a0     	str	r1, [r0, #0xa0]
  32d7ec: e1a00003     	mov	r0, r3
  32d7f0: e5845010     	str	r5, [r4, #0x10]
  32d7f4: e5845014     	str	r5, [r4, #0x14]
  32d7f8: e5845018     	str	r5, [r4, #0x18]
  32d7fc: e584501c     	str	r5, [r4, #0x1c]
  32d800: e5845020     	str	r5, [r4, #0x20]
  32d804: e5845024     	str	r5, [r4, #0x24]
  32d808: e5845028     	str	r5, [r4, #0x28]
  32d80c: e584502c     	str	r5, [r4, #0x2c]
  32d810: e5845030     	str	r5, [r4, #0x30]
  32d814: e5845034     	str	r5, [r4, #0x34]
  32d818: e5845038     	str	r5, [r4, #0x38]
  32d81c: e584503c     	str	r5, [r4, #0x3c]
  32d820: e5845040     	str	r5, [r4, #0x40]
  32d824: e5845044     	str	r5, [r4, #0x44]
  32d828: e5845048     	str	r5, [r4, #0x48]
  32d82c: e584504c     	str	r5, [r4, #0x4c]
  32d830: e5845050     	str	r5, [r4, #0x50]
  32d834: e5845074     	str	r5, [r4, #0x74]
  32d838: e5c45078     	strb	r5, [r4, #0x78]
  32d83c: e5c45079     	strb	r5, [r4, #0x79]
  32d840: e5c4507a     	strb	r5, [r4, #0x7a]
  32d844: e5845088     	str	r5, [r4, #0x88]
  32d848: e5c450a4     	strb	r5, [r4, #0xa4]
  32d84c: e5c450a5     	strb	r5, [r4, #0xa5]
  32d850: e5c450a6     	strb	r5, [r4, #0xa6]
  32d854: e3a01010     	mov	r1, #16
  32d858: e5c450a7     	strb	r5, [r4, #0xa7]
  32d85c: e5c450a8     	strb	r5, [r4, #0xa8]
  32d860: e5c450a9     	strb	r5, [r4, #0xa9]
  32d864: e5c450aa     	strb	r5, [r4, #0xaa]
  32d868: e5c450ad     	strb	r5, [r4, #0xad]
  32d86c: e58430cc     	str	r3, [r4, #0xcc]
  32d870: e58430d0     	str	r3, [r4, #0xd0]
  32d874: e5c470ab     	strb	r7, [r4, #0xab]
  32d878: e5c470b4     	strb	r7, [r4, #0xb4]
  32d87c: ebff8f7e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1c208
  32d880: e59420cc     	ldr	r2, [r4, #0xcc]
  32d884: e28430d4     	add	r3, r4, #212
  32d888: e1a00003     	mov	r0, r3
  32d88c: e5c25000     	strb	r5, [r2]
  32d890: e3a01010     	mov	r1, #16
  32d894: e58430e4     	str	r3, [r4, #0xe4]
  32d898: e58430e8     	str	r3, [r4, #0xe8]
  32d89c: ebff8f76     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x1c228
  32d8a0: e59f30c4     	ldr	r3, [pc, #0xc4]         @ 0x32d96c <Application::Application()+0x1d0>
  32d8a4: e59420e4     	ldr	r2, [r4, #0xe4]
  32d8a8: e3a00010     	mov	r0, #16
  32d8ac: e7963003     	ldr	r3, [r6, r3]
  32d8b0: e5c25000     	strb	r5, [r2]
  32d8b4: e5c450ee     	strb	r5, [r4, #0xee]
  32d8b8: e5c450ed     	strb	r5, [r4, #0xed]
  32d8bc: e5c4510e     	strb	r5, [r4, #0x10e]
  32d8c0: e5c4710f     	strb	r7, [r4, #0x10f]
  32d8c4: e5c37000     	strb	r7, [r3]
  32d8c8: ebff8ae1     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x1d47c
  32d8cc: e1a01007     	mov	r1, r7
  32d8d0: e1a05000     	mov	r5, r0
  32d8d4: ebffa95a     	bl	0x317e44 <updateJob_thread::updateJob_thread(int)> @ imm = #-0x15a98
  32d8d8: e5845004     	str	r5, [r4, #0x4]
  32d8dc: e3a00010     	mov	r0, #16
  32d8e0: ebff8adb     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x1d494
  32d8e4: e3a01002     	mov	r1, #2
  32d8e8: e1a05000     	mov	r5, r0
  32d8ec: ebffa954     	bl	0x317e44 <updateJob_thread::updateJob_thread(int)> @ imm = #-0x15ab0
  32d8f0: e59f3078     	ldr	r3, [pc, #0x78]         @ 0x32d970 <Application::Application()+0x1d4>
  32d8f4: e3a00010     	mov	r0, #16
  32d8f8: e7963003     	ldr	r3, [r6, r3]
  32d8fc: e5835000     	str	r5, [r3]
  32d900: ebff8ad3     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x1d4b4
  32d904: e3a01003     	mov	r1, #3
  32d908: e1a05000     	mov	r5, r0
  32d90c: ebffa94c     	bl	0x317e44 <updateJob_thread::updateJob_thread(int)> @ imm = #-0x15ad0
  32d910: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x32d974 <Application::Application()+0x1d8>
  32d914: e3a00010     	mov	r0, #16
  32d918: e7963003     	ldr	r3, [r6, r3]
  32d91c: e5835000     	str	r5, [r3]
  32d920: ebff8acb     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x1d4d4
  32d924: e3a01004     	mov	r1, #4
  32d928: e1a05000     	mov	r5, r0
  32d92c: ebffa944     	bl	0x317e44 <updateJob_thread::updateJob_thread(int)> @ imm = #-0x15af0
  32d930: e59f3040     	ldr	r3, [pc, #0x40]         @ 0x32d978 <Application::Application()+0x1dc>
  32d934: e3a00010     	mov	r0, #16
  32d938: e7963003     	ldr	r3, [r6, r3]
  32d93c: e5835000     	str	r5, [r3]
  32d940: ebff8ac3     	bl	0x310454 <CustomAlloc(unsigned int)> @ imm = #-0x1d4f4
  32d944: e3a01005     	mov	r1, #5
  32d948: e1a05000     	mov	r5, r0
  32d94c: ebffa93c     	bl	0x317e44 <updateJob_thread::updateJob_thread(int)> @ imm = #-0x15b10
  32d950: e59f3024     	ldr	r3, [pc, #0x24]         @ 0x32d97c <Application::Application()+0x1e0>
  32d954: e1a00004     	mov	r0, r4
  32d958: e7963003     	ldr	r3, [r6, r3]
  32d95c: e5835000     	str	r5, [r3]
  32d960: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  32d964: e4 72 66 00  	.word	0x006672e4
  32d968: cc 0d 00 00  	.word	0x00000dcc
  32d96c: 58 3d 00 00  	.word	0x00003d58
  32d970: 40 48 00 00  	.word	0x00004840
  32d974: bc 0c 00 00  	.word	0x00000cbc
  32d978: 98 15 00 00  	.word	0x00001598
  32d97c: 40 1d 00 00  	.word	0x00001d40
