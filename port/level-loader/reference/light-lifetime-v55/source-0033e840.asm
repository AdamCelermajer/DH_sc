
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033e840 <ObjectBase::~ObjectBase()>:
  33e840: e92d4030     	push	{r4, r5, lr}
  33e844: e59f5104     	ldr	r5, [pc, #0x104]        @ 0x33e950 <ObjectBase::~ObjectBase()+0x110>
  33e848: e59f3104     	ldr	r3, [pc, #0x104]        @ 0x33e954 <ObjectBase::~ObjectBase()+0x114>
  33e84c: e5d01029     	ldrb	r1, [r0, #0x29]
  33e850: e08f5005     	add	r5, pc, r5
  33e854: e7953003     	ldr	r3, [r5, r3]
  33e858: e3510000     	cmp	r1, #0
  33e85c: e24dd00c     	sub	sp, sp, #12
  33e860: e2832074     	add	r2, r3, #116
  33e864: e2831008     	add	r1, r3, #8
  33e868: e2833068     	add	r3, r3, #104
  33e86c: e1a04000     	mov	r4, r0
  33e870: e880000a     	stm	r0, {r1, r3}
  33e874: e5802024     	str	r2, [r0, #0x24]
  33e878: 0a000008     	beq	0x33e8a0 <ObjectBase::~ObjectBase()+0x60> @ imm = #0x20
  33e87c: e59f30d4     	ldr	r3, [pc, #0xd4]         @ 0x33e958 <ObjectBase::~ObjectBase()+0x118>
  33e880: e7953003     	ldr	r3, [r5, r3]
  33e884: e5933000     	ldr	r3, [r3]
  33e888: e3530002     	cmp	r3, #2
  33e88c: 03a03000     	moveq	r3, #0
  33e890: 05833000     	streq	r3, [r3]
  33e894: 0a000001     	beq	0x33e8a0 <ObjectBase::~ObjectBase()+0x60> @ imm = #0x4
  33e898: e3530001     	cmp	r3, #1
  33e89c: 0a00001e     	beq	0x33e91c <ObjectBase::~ObjectBase()+0xdc> @ imm = #0x78
  33e8a0: e594002c     	ldr	r0, [r4, #0x2c]
  33e8a4: e3500000     	cmp	r0, #0
  33e8a8: 0a000002     	beq	0x33e8b8 <ObjectBase::~ObjectBase()+0x78> @ imm = #0x8
  33e8ac: ebff46e3     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x2e474
  33e8b0: e3a03000     	mov	r3, #0
  33e8b4: e584302c     	str	r3, [r4, #0x2c]
  33e8b8: e28400d4     	add	r0, r4, #212
  33e8bc: ebff543a     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2af18
  33e8c0: e28400b0     	add	r0, r4, #176
  33e8c4: ebffffcb     	bl	0x33e7f8 <ConditionData::~ConditionData()> @ imm = #-0xd4
  33e8c8: e284008c     	add	r0, r4, #140
  33e8cc: ebffffc9     	bl	0x33e7f8 <ConditionData::~ConditionData()> @ imm = #-0xdc
  33e8d0: e2840068     	add	r0, r4, #104
  33e8d4: ebff5434     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2af30
  33e8d8: e2840048     	add	r0, r4, #72
  33e8dc: ebff5432     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2af38
  33e8e0: e2840030     	add	r0, r4, #48
  33e8e4: ebff5430     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2af40
  33e8e8: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x33e95c <ObjectBase::~ObjectBase()+0x11c>
  33e8ec: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x33e960 <ObjectBase::~ObjectBase()+0x120>
  33e8f0: e2840008     	add	r0, r4, #8
  33e8f4: e7952002     	ldr	r2, [r5, r2]
  33e8f8: e7953003     	ldr	r3, [r5, r3]
  33e8fc: e2822008     	add	r2, r2, #8
  33e900: e2833008     	add	r3, r3, #8
  33e904: e5842024     	str	r2, [r4, #0x24]
  33e908: e5843004     	str	r3, [r4, #0x4]
  33e90c: ebff5426     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2af68
  33e910: e1a00004     	mov	r0, r4
  33e914: e28dd00c     	add	sp, sp, #12
  33e918: e8bd8030     	pop	{r4, r5, pc}
  33e91c: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x33e964 <ObjectBase::~ObjectBase()+0x124>
  33e920: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x33e968 <ObjectBase::~ObjectBase()+0x128>
  33e924: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x33e96c <ObjectBase::~ObjectBase()+0x12c>
  33e928: e7950000     	ldr	r0, [r5, r0]
  33e92c: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x33e970 <ObjectBase::~ObjectBase()+0x130>
  33e930: e3a0c071     	mov	r12, #113
  33e934: e08f1001     	add	r1, pc, r1
  33e938: e08f2002     	add	r2, pc, r2
  33e93c: e08f3003     	add	r3, pc, r3
  33e940: e28000a8     	add	r0, r0, #168
  33e944: e58dc000     	str	r12, [sp]
  33e948: ebff3dad     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x3094c
  33e94c: eaffffd3     	b	0x33e8a0 <ObjectBase::~ObjectBase()+0x60> @ imm = #-0xb4
  33e950: 40 62 65 00  	.word	0x00656240
  33e954: 84 3b 00 00  	.word	0x00003b84
  33e958: c0 39 00 00  	.word	0x000039c0
  33e95c: 40 14 00 00  	.word	0x00001440
  33e960: dc 3b 00 00  	.word	0x00003bdc
  33e964: c0 19 00 00  	.word	0x000019c0
  33e968: a4 fa 57 00  	.word	0x0057faa4
  33e96c: d0 17 58 00  	.word	0x005817d0
  33e970: dc 17 58 00  	.word	0x005817dc
