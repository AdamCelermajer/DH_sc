
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0038a100 <Module::Module(ObjectBase::GO_IDS)>:
  38a100: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  38a104: e59f613c     	ldr	r6, [pc, #0x13c]        @ 0x38a248 <Module::Module(ObjectBase::GO_IDS)+0x148>
  38a108: e1a04000     	mov	r4, r0
  38a10c: eb0008a1     	bl	0x38c398 <GameObject::GameObject(ObjectBase::GO_IDS)> @ imm = #0x2284
  38a110: e59f3134     	ldr	r3, [pc, #0x134]        @ 0x38a24c <Module::Module(ObjectBase::GO_IDS)+0x14c>
  38a114: e08f6006     	add	r6, pc, r6
  38a118: e3a07001     	mov	r7, #1
  38a11c: e7963003     	ldr	r3, [r6, r3]
  38a120: e2842fde     	add	r2, r4, #888
  38a124: e1a00002     	mov	r0, r2
  38a128: e283c008     	add	r12, r3, #8
  38a12c: e28310e4     	add	r1, r3, #228
  38a130: e28330d8     	add	r3, r3, #216
  38a134: e584c000     	str	r12, [r4]
  38a138: e5843004     	str	r3, [r4, #0x4]
  38a13c: e5841024     	str	r1, [r4, #0x24]
  38a140: e5842388     	str	r2, [r4, #0x388]
  38a144: e584238c     	str	r2, [r4, #0x38c]
  38a148: e5c47375     	strb	r7, [r4, #0x375]
  38a14c: e5c47376     	strb	r7, [r4, #0x376]
  38a150: e5c47084     	strb	r7, [r4, #0x84]
  38a154: e3a01010     	mov	r1, #16
  38a158: ebfe1d47     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78ae4
  38a15c: e5942388     	ldr	r2, [r4, #0x388]
  38a160: e3a05000     	mov	r5, #0
  38a164: e2843e39     	add	r3, r4, #912
  38a168: e5c25000     	strb	r5, [r2]
  38a16c: e1a00003     	mov	r0, r3
  38a170: e58433a0     	str	r3, [r4, #0x3a0]
  38a174: e58433a4     	str	r3, [r4, #0x3a4]
  38a178: e3a01010     	mov	r1, #16
  38a17c: ebfe1d3e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78b08
  38a180: e59423a0     	ldr	r2, [r4, #0x3a0]
  38a184: e2843fea     	add	r3, r4, #936
  38a188: e1a00003     	mov	r0, r3
  38a18c: e5c25000     	strb	r5, [r2]
  38a190: e3a01010     	mov	r1, #16
  38a194: e58433b8     	str	r3, [r4, #0x3b8]
  38a198: e58433bc     	str	r3, [r4, #0x3bc]
  38a19c: ebfe1d36     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78b28
  38a1a0: e59423b8     	ldr	r2, [r4, #0x3b8]
  38a1a4: e2843d0f     	add	r3, r4, #960
  38a1a8: e1a00003     	mov	r0, r3
  38a1ac: e5c25000     	strb	r5, [r2]
  38a1b0: e3a01010     	mov	r1, #16
  38a1b4: e58433d0     	str	r3, [r4, #0x3d0]
  38a1b8: e58433d4     	str	r3, [r4, #0x3d4]
  38a1bc: ebfe1d2e     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78b48
  38a1c0: e59423d0     	ldr	r2, [r4, #0x3d0]
  38a1c4: e2843ff6     	add	r3, r4, #984
  38a1c8: e1a00003     	mov	r0, r3
  38a1cc: e5c25000     	strb	r5, [r2]
  38a1d0: e3a01010     	mov	r1, #16
  38a1d4: e58433e8     	str	r3, [r4, #0x3e8]
  38a1d8: e58433ec     	str	r3, [r4, #0x3ec]
  38a1dc: ebfe1d26     	bl	0x31167c <std::priv::_String_base<char, std::allocator<char>>::_M_allocate_block(unsigned int)> @ imm = #-0x78b68
  38a1e0: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x38a250 <Module::Module(ObjectBase::GO_IDS)+0x150>
  38a1e4: e59423e8     	ldr	r2, [r4, #0x3e8]
  38a1e8: e2840b01     	add	r0, r4, #1024
  38a1ec: e7963003     	ldr	r3, [r6, r3]
  38a1f0: e5c25000     	strb	r5, [r2]
  38a1f4: e5932000     	ldr	r2, [r3]
  38a1f8: e58423f0     	str	r2, [r4, #0x3f0]
  38a1fc: e5932004     	ldr	r2, [r3, #0x4]
  38a200: e58423f4     	str	r2, [r4, #0x3f4]
  38a204: e5933008     	ldr	r3, [r3, #0x8]
  38a208: e5c453fc     	strb	r5, [r4, #0x3fc]
  38a20c: e58433f8     	str	r3, [r4, #0x3f8]
  38a210: ebfed4bd     	bl	0x33f50c <ObjectHandle::ObjectHandle()> @ imm = #-0x4ad0c
  38a214: e59f3038     	ldr	r3, [pc, #0x38]         @ 0x38a254 <Module::Module(ObjectBase::GO_IDS)+0x154>
  38a218: e5845418     	str	r5, [r4, #0x418]
  38a21c: e5c47084     	strb	r7, [r4, #0x84]
  38a220: e7963003     	ldr	r3, [r6, r3]
  38a224: e5845410     	str	r5, [r4, #0x410]
  38a228: e5845414     	str	r5, [r4, #0x414]
  38a22c: e5c47028     	strb	r7, [r4, #0x28]
  38a230: e5932000     	ldr	r2, [r3]
  38a234: e1a00004     	mov	r0, r4
  38a238: e0821007     	add	r1, r2, r7
  38a23c: e584240c     	str	r2, [r4, #0x40c]
  38a240: e5831000     	str	r1, [r3]
  38a244: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  38a248: 7c a9 60 00  	.word	0x0060a97c
  38a24c: 90 41 00 00  	.word	0x00004190
  38a250: 98 24 00 00  	.word	0x00002498
  38a254: 2c 28 00 00  	.word	0x0000282c
