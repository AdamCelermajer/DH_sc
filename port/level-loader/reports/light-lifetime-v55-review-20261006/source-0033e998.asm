
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0033e998 <ObjectBase::~ObjectBase()>:
  33e998: e92d4030     	push	{r4, r5, lr}
  33e99c: e59f5104     	ldr	r5, [pc, #0x104]        @ 0x33eaa8 <ObjectBase::~ObjectBase()+0x110>
  33e9a0: e59f3104     	ldr	r3, [pc, #0x104]        @ 0x33eaac <ObjectBase::~ObjectBase()+0x114>
  33e9a4: e5d01029     	ldrb	r1, [r0, #0x29]
  33e9a8: e08f5005     	add	r5, pc, r5
  33e9ac: e7953003     	ldr	r3, [r5, r3]
  33e9b0: e3510000     	cmp	r1, #0
  33e9b4: e24dd00c     	sub	sp, sp, #12
  33e9b8: e2832074     	add	r2, r3, #116
  33e9bc: e2831008     	add	r1, r3, #8
  33e9c0: e2833068     	add	r3, r3, #104
  33e9c4: e1a04000     	mov	r4, r0
  33e9c8: e880000a     	stm	r0, {r1, r3}
  33e9cc: e5802024     	str	r2, [r0, #0x24]
  33e9d0: 0a000008     	beq	0x33e9f8 <ObjectBase::~ObjectBase()+0x60> @ imm = #0x20
  33e9d4: e59f30d4     	ldr	r3, [pc, #0xd4]         @ 0x33eab0 <ObjectBase::~ObjectBase()+0x118>
  33e9d8: e7953003     	ldr	r3, [r5, r3]
  33e9dc: e5933000     	ldr	r3, [r3]
  33e9e0: e3530002     	cmp	r3, #2
  33e9e4: 03a03000     	moveq	r3, #0
  33e9e8: 05833000     	streq	r3, [r3]
  33e9ec: 0a000001     	beq	0x33e9f8 <ObjectBase::~ObjectBase()+0x60> @ imm = #0x4
  33e9f0: e3530001     	cmp	r3, #1
  33e9f4: 0a00001e     	beq	0x33ea74 <ObjectBase::~ObjectBase()+0xdc> @ imm = #0x78
  33e9f8: e594002c     	ldr	r0, [r4, #0x2c]
  33e9fc: e3500000     	cmp	r0, #0
  33ea00: 0a000002     	beq	0x33ea10 <ObjectBase::~ObjectBase()+0x78> @ imm = #0x8
  33ea04: ebff468d     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x2e5cc
  33ea08: e3a03000     	mov	r3, #0
  33ea0c: e584302c     	str	r3, [r4, #0x2c]
  33ea10: e28400d4     	add	r0, r4, #212
  33ea14: ebff53e4     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2b070
  33ea18: e28400b0     	add	r0, r4, #176
  33ea1c: ebffff75     	bl	0x33e7f8 <ConditionData::~ConditionData()> @ imm = #-0x22c
  33ea20: e284008c     	add	r0, r4, #140
  33ea24: ebffff73     	bl	0x33e7f8 <ConditionData::~ConditionData()> @ imm = #-0x234
  33ea28: e2840068     	add	r0, r4, #104
  33ea2c: ebff53de     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2b088
  33ea30: e2840048     	add	r0, r4, #72
  33ea34: ebff53dc     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2b090
  33ea38: e2840030     	add	r0, r4, #48
  33ea3c: ebff53da     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2b098
  33ea40: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x33eab4 <ObjectBase::~ObjectBase()+0x11c>
  33ea44: e59f306c     	ldr	r3, [pc, #0x6c]         @ 0x33eab8 <ObjectBase::~ObjectBase()+0x120>
  33ea48: e2840008     	add	r0, r4, #8
  33ea4c: e7952002     	ldr	r2, [r5, r2]
  33ea50: e7953003     	ldr	r3, [r5, r3]
  33ea54: e2822008     	add	r2, r2, #8
  33ea58: e2833008     	add	r3, r3, #8
  33ea5c: e5842024     	str	r2, [r4, #0x24]
  33ea60: e5843004     	str	r3, [r4, #0x4]
  33ea64: ebff53d0     	bl	0x3139ac <std::priv::_String_base<char, std::allocator<char>>::_M_deallocate_block()> @ imm = #-0x2b0c0
  33ea68: e1a00004     	mov	r0, r4
  33ea6c: e28dd00c     	add	sp, sp, #12
  33ea70: e8bd8030     	pop	{r4, r5, pc}
  33ea74: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x33eabc <ObjectBase::~ObjectBase()+0x124>
  33ea78: e59f1040     	ldr	r1, [pc, #0x40]         @ 0x33eac0 <ObjectBase::~ObjectBase()+0x128>
  33ea7c: e59f2040     	ldr	r2, [pc, #0x40]         @ 0x33eac4 <ObjectBase::~ObjectBase()+0x12c>
  33ea80: e7950000     	ldr	r0, [r5, r0]
  33ea84: e59f303c     	ldr	r3, [pc, #0x3c]         @ 0x33eac8 <ObjectBase::~ObjectBase()+0x130>
  33ea88: e3a0c071     	mov	r12, #113
  33ea8c: e08f1001     	add	r1, pc, r1
  33ea90: e08f2002     	add	r2, pc, r2
  33ea94: e08f3003     	add	r3, pc, r3
  33ea98: e28000a8     	add	r0, r0, #168
  33ea9c: e58dc000     	str	r12, [sp]
  33eaa0: ebff3d57     	bl	0x30e004 <.plt+0x290>   @ imm = #-0x30aa4
  33eaa4: eaffffd3     	b	0x33e9f8 <ObjectBase::~ObjectBase()+0x60> @ imm = #-0xb4
  33eaa8: e8 60 65 00  	.word	0x006560e8
  33eaac: 84 3b 00 00  	.word	0x00003b84
  33eab0: c0 39 00 00  	.word	0x000039c0
  33eab4: 40 14 00 00  	.word	0x00001440
  33eab8: dc 3b 00 00  	.word	0x00003bdc
  33eabc: c0 19 00 00  	.word	0x000019c0
  33eac0: 4c f9 57 00  	.word	0x0057f94c
  33eac4: 78 16 58 00  	.word	0x00581678
  33eac8: 84 16 58 00  	.word	0x00581684
