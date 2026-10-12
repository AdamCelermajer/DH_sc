
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0034b724 <ObjectManager::Spawn(char const*, char const*, bool, bool)>:
  34b724: e92d41f0     	push	{r4, r5, r6, r7, r8, lr}
  34b728: e24dd008     	sub	sp, sp, #8
  34b72c: e5ddc024     	ldrb	r12, [sp, #0x24]
  34b730: e3e0e000     	mvn	lr, #0
  34b734: e1a04000     	mov	r4, r0
  34b738: e58de000     	str	lr, [sp]
  34b73c: e58dc004     	str	r12, [sp, #0x4]
  34b740: e1a06001     	mov	r6, r1
  34b744: e1a05002     	mov	r5, r2
  34b748: e1a07003     	mov	r7, r3
  34b74c: e5dd8020     	ldrb	r8, [sp, #0x20]
  34b750: ebffff72     	bl	0x34b520 <ObjectManager::GetNewObject(char const*, char const*, int, bool)> @ imm = #-0x238
  34b754: e1a00004     	mov	r0, r4
  34b758: e3a01000     	mov	r1, #0
  34b75c: ebffd197     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb9a4
  34b760: e3500000     	cmp	r0, #0
  34b764: 0a000022     	beq	0x34b7f4 <ObjectManager::Spawn(char const*, char const*, bool, bool)+0xd0> @ imm = #0x88
  34b768: e3a01001     	mov	r1, #1
  34b76c: e1a00004     	mov	r0, r4
  34b770: ebffd192     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb9b8
  34b774: e2800004     	add	r0, r0, #4
  34b778: eb07217e     	bl	0x513d78 <PropertyMap::InitProperties()> @ imm = #0x1c85f8
  34b77c: e3a01001     	mov	r1, #1
  34b780: e1a00004     	mov	r0, r4
  34b784: ebffd18d     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb9cc
  34b788: e2800004     	add	r0, r0, #4
  34b78c: eb071fd6     	bl	0x5136ec <PropertyMap::LoadDefaultProperties()> @ imm = #0x1c7f58
  34b790: e3a01001     	mov	r1, #1
  34b794: e1a00004     	mov	r0, r4
  34b798: ebffd188     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb9e0
  34b79c: e1a01007     	mov	r1, r7
  34b7a0: ebfffd1c     	bl	0x34ac18 <ObjectBase::SetName(char const*)> @ imm = #-0xb90
  34b7a4: e3a01001     	mov	r1, #1
  34b7a8: e1a00004     	mov	r0, r4
  34b7ac: ebffd183     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xb9f4
  34b7b0: e1a07000     	mov	r7, r0
  34b7b4: e1a00005     	mov	r0, r5
  34b7b8: ebff09a5     	bl	0x30de54 <.plt+0xe0>    @ imm = #-0x3d96c
  34b7bc: e1a01005     	mov	r1, r5
  34b7c0: e0852000     	add	r2, r5, r0
  34b7c4: e2870048     	add	r0, r7, #72
  34b7c8: ebff1484     	bl	0x3109e0 <std::string::_M_assign(char const*, char const*)> @ imm = #-0x3adf0
  34b7cc: e3580000     	cmp	r8, #0
  34b7d0: 0a000018     	beq	0x34b838 <ObjectManager::Spawn(char const*, char const*, bool, bool)+0x114> @ imm = #0x60
  34b7d4: e3a01001     	mov	r1, #1
  34b7d8: e1a00004     	mov	r0, r4
  34b7dc: ebffd177     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xba24
  34b7e0: e5903000     	ldr	r3, [r0]
  34b7e4: e1a0e00f     	mov	lr, pc
  34b7e8: e593f038     	ldr	pc, [r3, #0x38]
  34b7ec: e3500000     	cmp	r0, #0
  34b7f0: 1a000002     	bne	0x34b800 <ObjectManager::Spawn(char const*, char const*, bool, bool)+0xdc> @ imm = #0x8
  34b7f4: e1a00004     	mov	r0, r4
  34b7f8: e28dd008     	add	sp, sp, #8
  34b7fc: e8bd81f0     	pop	{r4, r5, r6, r7, r8, pc}
  34b800: e3a01000     	mov	r1, #0
  34b804: e1a00004     	mov	r0, r4
  34b808: ebffd16c     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xba50
  34b80c: e2865034     	add	r5, r6, #52
  34b810: e1a07000     	mov	r7, r0
  34b814: e1a00005     	mov	r0, r5
  34b818: ebffde52     	bl	0x343168 <std::allocator<std::priv::_List_node<ObjectBase*>>::allocate(unsigned int, void const*) (.clone.20)> @ imm = #-0x86b8
  34b81c: e5807008     	str	r7, [r0, #0x8]
  34b820: e5963038     	ldr	r3, [r6, #0x38]
  34b824: e5805000     	str	r5, [r0]
  34b828: e5803004     	str	r3, [r0, #0x4]
  34b82c: e5830000     	str	r0, [r3]
  34b830: e5860038     	str	r0, [r6, #0x38]
  34b834: eaffffee     	b	0x34b7f4 <ObjectManager::Spawn(char const*, char const*, bool, bool)+0xd0> @ imm = #-0x48
  34b838: e3a01001     	mov	r1, #1
  34b83c: e1a00004     	mov	r0, r4
  34b840: ebffd15e     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xba88
  34b844: e5903000     	ldr	r3, [r0]
  34b848: e1a0e00f     	mov	lr, pc
  34b84c: e593f01c     	ldr	pc, [r3, #0x1c]
  34b850: e1a00004     	mov	r0, r4
  34b854: e3a01001     	mov	r1, #1
  34b858: ebffd158     	bl	0x33fdc0 <ObjectHandle::GetObject(bool)> @ imm = #-0xbaa0
  34b85c: e3a01001     	mov	r1, #1
  34b860: ebffcb9b     	bl	0x33e6d4 <ObjectBase::TestEnableCondition(bool)> @ imm = #-0xd194
  34b864: eaffffda     	b	0x34b7d4 <ObjectManager::Spawn(char const*, char const*, bool, bool)+0xb0> @ imm = #-0x98
