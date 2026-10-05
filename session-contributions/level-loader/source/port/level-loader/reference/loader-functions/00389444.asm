
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00389444 <Module::~Module()>:
  389444: e92d4070     	push	{r4, r5, r6, lr}
  389448: e59f50c8     	ldr	r5, [pc, #0xc8]         @ 0x389518 <Module::~Module()+0xd4>
  38944c: e59f30c8     	ldr	r3, [pc, #0xc8]         @ 0x38951c <Module::~Module()+0xd8>
  389450: e1a04000     	mov	r4, r0
  389454: e08f5005     	add	r5, pc, r5
  389458: e7953003     	ldr	r3, [r5, r3]
  38945c: e2800b01     	add	r0, r0, #1024
  389460: e3a01000     	mov	r1, #0
  389464: e28320e4     	add	r2, r3, #228
  389468: e283c008     	add	r12, r3, #8
  38946c: e28330d8     	add	r3, r3, #216
  389470: e584c000     	str	r12, [r4]
  389474: e5843004     	str	r3, [r4, #0x4]
  389478: e5842024     	str	r2, [r4, #0x24]
  38947c: ebfeda4f     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0x496c4
  389480: e3500000     	cmp	r0, #0
  389484: 0a000003     	beq	0x389498 <Module::~Module()+0x54> @ imm = #0xc
  389488: e59030f4     	ldr	r3, [r0, #0xf4]
  38948c: e353000b     	cmp	r3, #11
  389490: 03a03000     	moveq	r3, #0
  389494: 0580338c     	streq	r3, [r0, #0x38c]
  389498: e5940410     	ldr	r0, [r4, #0x410]
  38949c: e3500000     	cmp	r0, #0
  3894a0: 0a000005     	beq	0x3894bc <Module::~Module()+0x78> @ imm = #0x14
  3894a4: e5941418     	ldr	r1, [r4, #0x418]
  3894a8: e0601001     	rsb	r1, r0, r1
  3894ac: e3c11003     	bic	r1, r1, #3
  3894b0: e3510080     	cmp	r1, #128
  3894b4: 8a000015     	bhi	0x389510 <Module::~Module()+0xcc> @ imm = #0x54
  3894b8: eb0dfe90     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x37fa40
  3894bc: e2840ff6     	add	r0, r4, #984
  3894c0: ebfe2939     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75b1c
  3894c4: e2840d0f     	add	r0, r4, #960
  3894c8: ebfe2937     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75b24
  3894cc: e2840fea     	add	r0, r4, #936
  3894d0: ebfe2935     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75b2c
  3894d4: e2840e39     	add	r0, r4, #912
  3894d8: ebfe2933     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75b34
  3894dc: e2840fde     	add	r0, r4, #888
  3894e0: ebfe2931     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x75b3c
  3894e4: e59f3034     	ldr	r3, [pc, #0x34]         @ 0x389520 <Module::~Module()+0xdc>
  3894e8: e1a00004     	mov	r0, r4
  3894ec: e7953003     	ldr	r3, [r5, r3]
  3894f0: e28320e4     	add	r2, r3, #228
  3894f4: e2831008     	add	r1, r3, #8
  3894f8: e28330d8     	add	r3, r3, #216
  3894fc: e884000a     	stm	r4, {r1, r3}
  389500: e5842024     	str	r2, [r4, #0x24]
  389504: eb000f9b     	bl	0x38d378 <GameObject::~GameObject()> @ imm = #0x3e6c
  389508: e1a00004     	mov	r0, r4
  38950c: e8bd8070     	pop	{r4, r5, r6, pc}
  389510: ebfe1bca     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x790d8
  389514: eaffffe8     	b	0x3894bc <Module::~Module()+0x78> @ imm = #-0x60
  389518: 3c b6 60 00  	.word	0x0060b63c
  38951c: 90 41 00 00  	.word	0x00004190
  389520: 0c 2b 00 00  	.word	0x00002b0c
