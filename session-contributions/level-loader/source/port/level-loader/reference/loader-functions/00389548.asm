
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389548 <Module::~Module()>:
  389548: e92d4070     	push	{r4, r5, r6, lr}
  38954c: e59f50c8     	ldr	r5, [pc, #0xc8]         @ 0x38961c <Module::~Module()+0xd4>
  389550: e59f30c8     	ldr	r3, [pc, #0xc8]         @ 0x389620 <Module::~Module()+0xd8>
  389554: e1a04000     	mov	r4, r0
  389558: e08f5005     	add	r5, pc, r5
  38955c: e7953003     	ldr	r3, [r5, r3]
  389560: e2800b01     	add	r0, r0, #1024
  389564: e3a01000     	mov	r1, #0
  389568: e28320e4     	add	r2, r3, #228
  38956c: e283c008     	add	r12, r3, #8
  389570: e28330d8     	add	r3, r3, #216
  389574: e584c000     	str	r12, [r4]
  389578: e5843004     	str	r3, [r4, #0x4]
  38957c: e5842024     	str	r2, [r4, #0x24]
  389580: ebfeda0e     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x497c8
  389584: e3500000     	cmp	r0, #0
  389588: 0a000003     	beq	0x38959c <Module::~Module()+0x54> @ imm = #0xc
  38958c: e59030f4     	ldr	r3, [r0, #0xf4]
  389590: e353000b     	cmp	r3, #11
  389594: 03a03000     	moveq	r3, #0
  389598: 0580338c     	streq	r3, [r0, #0x38c]
  38959c: e5940410     	ldr	r0, [r4, #0x410]
  3895a0: e3500000     	cmp	r0, #0
  3895a4: 0a000005     	beq	0x3895c0 <Module::~Module()+0x78> @ imm = #0x14
  3895a8: e5941418     	ldr	r1, [r4, #0x418]
  3895ac: e0601001     	rsb	r1, r0, r1
  3895b0: e3c11003     	bic	r1, r1, #3
  3895b4: e3510080     	cmp	r1, #128
  3895b8: 8a000015     	bhi	0x389614 <Module::~Module()+0xcc> @ imm = #0x54
  3895bc: eb0dfe4f     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x37f93c
  3895c0: e2840ff6     	add	r0, r4, #984
  3895c4: ebfe28f8     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75c20
  3895c8: e2840d0f     	add	r0, r4, #960
  3895cc: ebfe28f6     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75c28
  3895d0: e2840fea     	add	r0, r4, #936
  3895d4: ebfe28f4     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75c30
  3895d8: e2840e39     	add	r0, r4, #912
  3895dc: ebfe28f2     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75c38
  3895e0: e2840fde     	add	r0, r4, #888
  3895e4: ebfe28f0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75c40
  3895e8: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x389624 <Module::~Module()+0xdc>
  3895ec: e1a00004     	mov	r0, r4
  3895f0: e7953003     	ldr	r3, [r5, r3]
  3895f4: e28320e4     	add	r2, r3, #228
  3895f8: e2831008     	add	r1, r3, #8
  3895fc: e28330d8     	add	r3, r3, #216
  389600: e884000a     	stm	r4, {r1, r3}
  389604: e5842024     	str	r2, [r4, #0x24]
  389608: eb000f5a     	bl	0x38d378 <GameObject::~GameObject()> @ imm = #0x3d68
  38960c: e1a00004     	mov	r0, r4
  389610: e8bd8070     	pop	{r4, r5, r6, pc}
  389614: ebfe1b89     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x791dc
  389618: eaffffe8     	b	0x3895c0 <Module::~Module()+0x78> @ imm = #-0x60
  38961c: 38 b5 60 00  	.word	0x0060b538
  389620: 90 41 00 00  	.word	0x00004190
  389624: 0c 2b 00 00  	.word	0x00002b0c
