
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389fa8 <Module::Module(ObjectBase::GO_IDS)>:
  389fa8: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  389fac: e59f613c     	ldr	r6, [pc, #0x13c]        @ 0x38a0f0 <Module::Module(ObjectBase::GO_IDS)+0x148>
  389fb0: e1a04000     	mov	r4, r0
  389fb4: eb0008f7     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #0x23dc
  389fb8: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x38a0f4 <Module::Module(ObjectBase::GO_IDS)+0x14c>
  389fbc: e08f6006     	add	r6, pc, r6
  389fc0: e3a07001     	mov	r7, #1
  389fc4: e7963003     	ldr	r3, [r6, r3]
  389fc8: e2842fde     	add	r2, r4, #888
  389fcc: e1a00002     	mov	r0, r2
  389fd0: e283c008     	add	r12, r3, #8
  389fd4: e28310e4     	add	r1, r3, #228
  389fd8: e28330d8     	add	r3, r3, #216
  389fdc: e584c000     	str	r12, [r4]
  389fe0: e5843004     	str	r3, [r4, #0x4]
  389fe4: e5841024     	str	r1, [r4, #0x24]
  389fe8: e5842388     	str	r2, [r4, #0x388]
  389fec: e584238c     	str	r2, [r4, #0x38c]
  389ff0: e5c47375     	strb	r7, [r4, #0x375]
  389ff4: e5c47376     	strb	r7, [r4, #0x376]
  389ff8: e5c47084     	strb	r7, [r4, #0x84]
  389ffc: e3a01010     	mov	r1, #16
  38a000: ebfe1d9d     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x7898c
  38a004: e5942388     	ldr	r2, [r4, #0x388]
  38a008: e3a05000     	mov	r5, #0
  38a00c: e2843e39     	add	r3, r4, #912
  38a010: e5c25000     	strb	r5, [r2]
  38a014: e1a00003     	mov	r0, r3
  38a018: e58433a0     	str	r3, [r4, #0x3a0]
  38a01c: e58433a4     	str	r3, [r4, #0x3a4]
  38a020: e3a01010     	mov	r1, #16
  38a024: ebfe1d94     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x789b0
  38a028: e59423a0     	ldr	r2, [r4, #0x3a0]
  38a02c: e2843fea     	add	r3, r4, #936
  38a030: e1a00003     	mov	r0, r3
  38a034: e5c25000     	strb	r5, [r2]
  38a038: e3a01010     	mov	r1, #16
  38a03c: e58433b8     	str	r3, [r4, #0x3b8]
  38a040: e58433bc     	str	r3, [r4, #0x3bc]
  38a044: ebfe1d8c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x789d0
  38a048: e59423b8     	ldr	r2, [r4, #0x3b8]
  38a04c: e2843d0f     	add	r3, r4, #960
  38a050: e1a00003     	mov	r0, r3
  38a054: e5c25000     	strb	r5, [r2]
  38a058: e3a01010     	mov	r1, #16
  38a05c: e58433d0     	str	r3, [r4, #0x3d0]
  38a060: e58433d4     	str	r3, [r4, #0x3d4]
  38a064: ebfe1d84     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x789f0
  38a068: e59423d0     	ldr	r2, [r4, #0x3d0]
  38a06c: e2843ff6     	add	r3, r4, #984
  38a070: e1a00003     	mov	r0, r3
  38a074: e5c25000     	strb	r5, [r2]
  38a078: e3a01010     	mov	r1, #16
  38a07c: e58433e8     	str	r3, [r4, #0x3e8]
  38a080: e58433ec     	str	r3, [r4, #0x3ec]
  38a084: ebfe1d7c     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78a10
  38a088: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x38a0f8 <Module::Module(ObjectBase::GO_IDS)+0x150>
  38a08c: e59423e8     	ldr	r2, [r4, #0x3e8]
  38a090: e2840b01     	add	r0, r4, #1024
  38a094: e7963003     	ldr	r3, [r6, r3]
  38a098: e5c25000     	strb	r5, [r2]
  38a09c: e5932000     	ldr	r2, [r3]
  38a0a0: e58423f0     	str	r2, [r4, #0x3f0]
  38a0a4: e5932004     	ldr	r2, [r3, #0x4]
  38a0a8: e58423f4     	str	r2, [r4, #0x3f4]
  38a0ac: e5933008     	ldr	r3, [r3, #0x8]
  38a0b0: e5c453fc     	strb	r5, [r4, #0x3fc]
  38a0b4: e58433f8     	str	r3, [r4, #0x3f8]
  38a0b8: ebfed513     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0x4abb4
  38a0bc: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x38a0fc <Module::Module(ObjectBase::GO_IDS)+0x154>
  38a0c0: e5845418     	str	r5, [r4, #0x418]
  38a0c4: e5c47084     	strb	r7, [r4, #0x84]
  38a0c8: e7963003     	ldr	r3, [r6, r3]
  38a0cc: e5845410     	str	r5, [r4, #0x410]
  38a0d0: e5845414     	str	r5, [r4, #0x414]
  38a0d4: e5c47028     	strb	r7, [r4, #0x28]
  38a0d8: e5932000     	ldr	r2, [r3]
  38a0dc: e1a00004     	mov	r0, r4
  38a0e0: e0821007     	add	r1, r2, r7
  38a0e4: e584240c     	str	r2, [r4, #0x40c]
  38a0e8: e5831000     	str	r1, [r3]
  38a0ec: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  38a0f0: d4 aa 60 00  	.word	0x0060aad4
  38a0f4: 90 41 00 00  	.word	0x00004190
  38a0f8: 98 24 00 00  	.word	0x00002498
  38a0fc: 2c 28 00 00  	.word	0x0000282c
