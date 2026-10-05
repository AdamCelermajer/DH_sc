
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488704 <rnd::RandomGenerator::~RandomGenerator()>:
  488704: e92d4070     	push	{r4, r5, r6, lr}
  488708: e1a04000     	mov	r4, r0
  48870c: ebffef23     	bl	0x4843a0 <rnd::RandomGenerator::UnloadTiles()> @ imm = #-0x4374
  488710: e1a00004     	mov	r0, r4
  488714: ebfffef9     	bl	0x488300 <rnd::RandomGenerator::UnloadRules()> @ imm = #-0x41c
  488718: e1a00004     	mov	r0, r4
  48871c: ebfff4c4     	bl	0x485a34 <rnd::RandomGenerator::UnloadRoomPools()> @ imm = #-0x2cf0
  488720: e594318c     	ldr	r3, [r4, #0x18c]
  488724: e3530000     	cmp	r3, #0
  488728: 0a000005     	beq	0x488744 <rnd::RandomGenerator::~RandomGenerator()+0x40> @ imm = #0x14
  48872c: e1a00003     	mov	r0, r3
  488730: e5933000     	ldr	r3, [r3]
  488734: e1a0e00f     	mov	lr, pc
  488738: e593f004     	ldr	pc, [r3, #0x4]
  48873c: e3a03000     	mov	r3, #0
  488740: e584318c     	str	r3, [r4, #0x18c]
  488744: e2843f5d     	add	r3, r4, #372
  488748: e5930014     	ldr	r0, [r3, #0x14]
  48874c: e1500003     	cmp	r0, r3
  488750: 0a000006     	beq	0x488770 <rnd::RandomGenerator::~RandomGenerator()+0x6c> @ imm = #0x18
  488754: e3500000     	cmp	r0, #0
  488758: 0a000004     	beq	0x488770 <rnd::RandomGenerator::~RandomGenerator()+0x6c> @ imm = #0x10
  48875c: e5941174     	ldr	r1, [r4, #0x174]
  488760: e0601001     	rsb	r1, r0, r1
  488764: e3510080     	cmp	r1, #128
  488768: 8a000063     	bhi	0x4888fc <rnd::RandomGenerator::~RandomGenerator()+0x1f8> @ imm = #0x18c
  48876c: eb0a01e3     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x28078c
  488770: e2843f57     	add	r3, r4, #348
  488774: e5930014     	ldr	r0, [r3, #0x14]
  488778: e1500003     	cmp	r0, r3
  48877c: 0a000006     	beq	0x48879c <rnd::RandomGenerator::~RandomGenerator()+0x98> @ imm = #0x18
  488780: e3500000     	cmp	r0, #0
  488784: 0a000004     	beq	0x48879c <rnd::RandomGenerator::~RandomGenerator()+0x98> @ imm = #0x10
  488788: e594115c     	ldr	r1, [r4, #0x15c]
  48878c: e0601001     	rsb	r1, r0, r1
  488790: e3510080     	cmp	r1, #128
  488794: 8a000050     	bhi	0x4888dc <rnd::RandomGenerator::~RandomGenerator()+0x1d8> @ imm = #0x140
  488798: eb0a01d8     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280760
  48879c: e2843f51     	add	r3, r4, #324
  4887a0: e5930014     	ldr	r0, [r3, #0x14]
  4887a4: e1500003     	cmp	r0, r3
  4887a8: 0a000006     	beq	0x4887c8 <rnd::RandomGenerator::~RandomGenerator()+0xc4> @ imm = #0x18
  4887ac: e3500000     	cmp	r0, #0
  4887b0: 0a000004     	beq	0x4887c8 <rnd::RandomGenerator::~RandomGenerator()+0xc4> @ imm = #0x10
  4887b4: e5941144     	ldr	r1, [r4, #0x144]
  4887b8: e0601001     	rsb	r1, r0, r1
  4887bc: e3510080     	cmp	r1, #128
  4887c0: 8a000047     	bhi	0x4888e4 <rnd::RandomGenerator::~RandomGenerator()+0x1e0> @ imm = #0x11c
  4887c4: eb0a01cd     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280734
  4887c8: e2843f4b     	add	r3, r4, #300
  4887cc: e5930014     	ldr	r0, [r3, #0x14]
  4887d0: e1500003     	cmp	r0, r3
  4887d4: 0a000006     	beq	0x4887f4 <rnd::RandomGenerator::~RandomGenerator()+0xf0> @ imm = #0x18
  4887d8: e3500000     	cmp	r0, #0
  4887dc: 0a000004     	beq	0x4887f4 <rnd::RandomGenerator::~RandomGenerator()+0xf0> @ imm = #0x10
  4887e0: e594112c     	ldr	r1, [r4, #0x12c]
  4887e4: e0601001     	rsb	r1, r0, r1
  4887e8: e3510080     	cmp	r1, #128
  4887ec: 8a00003e     	bhi	0x4888ec <rnd::RandomGenerator::~RandomGenerator()+0x1e8> @ imm = #0xf8
  4887f0: eb0a01c2     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280708
  4887f4: e5940118     	ldr	r0, [r4, #0x118]
  4887f8: e2843f46     	add	r3, r4, #280
  4887fc: e3500000     	cmp	r0, #0
  488800: 0a000005     	beq	0x48881c <rnd::RandomGenerator::~RandomGenerator()+0x118> @ imm = #0x14
  488804: e5931008     	ldr	r1, [r3, #0x8]
  488808: e0601001     	rsb	r1, r0, r1
  48880c: e3c11003     	bic	r1, r1, #3
  488810: e3510080     	cmp	r1, #128
  488814: 8a00002e     	bhi	0x4888d4 <rnd::RandomGenerator::~RandomGenerator()+0x1d0> @ imm = #0xb8
  488818: eb0a01b8     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2806e0
  48881c: e28430fc     	add	r3, r4, #252
  488820: e5930014     	ldr	r0, [r3, #0x14]
  488824: e1500003     	cmp	r0, r3
  488828: 0a000006     	beq	0x488848 <rnd::RandomGenerator::~RandomGenerator()+0x144> @ imm = #0x18
  48882c: e3500000     	cmp	r0, #0
  488830: 0a000004     	beq	0x488848 <rnd::RandomGenerator::~RandomGenerator()+0x144> @ imm = #0x10
  488834: e59410fc     	ldr	r1, [r4, #0xfc]
  488838: e0601001     	rsb	r1, r0, r1
  48883c: e3510080     	cmp	r1, #128
  488840: 8a00002b     	bhi	0x4888f4 <rnd::RandomGenerator::~RandomGenerator()+0x1f0> @ imm = #0xac
  488844: eb0a01ad     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2806b4
  488848: e284006c     	add	r0, r4, #108
  48884c: eb001393     	bl	0x48d6a0 <rnd::RootRule::~RootRule()> @ imm = #0x4e4c
  488850: e5943064     	ldr	r3, [r4, #0x64]
  488854: e3530000     	cmp	r3, #0
  488858: 1a000013     	bne	0x4888ac <rnd::RandomGenerator::~RandomGenerator()+0x1a8> @ imm = #0x4c
  48885c: e594304c     	ldr	r3, [r4, #0x4c]
  488860: e3530000     	cmp	r3, #0
  488864: 1a000003     	bne	0x488878 <rnd::RandomGenerator::~RandomGenerator()+0x174> @ imm = #0xc
  488868: e2840014     	add	r0, r4, #20
  48886c: ebfff3f5     	bl	0x485848 <std::deque<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>, std::allocator<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>>::~deque()> @ imm = #-0x302c
  488870: e1a00004     	mov	r0, r4
  488874: e8bd8070     	pop	{r4, r5, r6, pc}
  488878: e284503c     	add	r5, r4, #60
  48887c: e1a00005     	mov	r0, r5
  488880: e5941040     	ldr	r1, [r4, #0x40]
  488884: ebfff20b     	bl	0x4850b8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x37d4
  488888: e3a03000     	mov	r3, #0
  48888c: e5845048     	str	r5, [r4, #0x48]
  488890: e584304c     	str	r3, [r4, #0x4c]
  488894: e5845044     	str	r5, [r4, #0x44]
  488898: e5843040     	str	r3, [r4, #0x40]
  48889c: e2840014     	add	r0, r4, #20
  4888a0: ebfff3e8     	bl	0x485848 <std::deque<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>, std::allocator<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>>::~deque()> @ imm = #-0x3060
  4888a4: e1a00004     	mov	r0, r4
  4888a8: e8bd8070     	pop	{r4, r5, r6, pc}
  4888ac: e2845054     	add	r5, r4, #84
  4888b0: e1a00005     	mov	r0, r5
  4888b4: e5941058     	ldr	r1, [r4, #0x58]
  4888b8: ebfff12a     	bl	0x484d68 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3b58
  4888bc: e3a03000     	mov	r3, #0
  4888c0: e5845060     	str	r5, [r4, #0x60]
  4888c4: e5843064     	str	r3, [r4, #0x64]
  4888c8: e584505c     	str	r5, [r4, #0x5c]
  4888cc: e5843058     	str	r3, [r4, #0x58]
  4888d0: eaffffe1     	b	0x48885c <rnd::RandomGenerator::~RandomGenerator()+0x158> @ imm = #-0x7c
  4888d4: ebfa1ed9     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17849c
  4888d8: eaffffcf     	b	0x48881c <rnd::RandomGenerator::~RandomGenerator()+0x118> @ imm = #-0xc4
  4888dc: ebfa1ed7     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1784a4
  4888e0: eaffffad     	b	0x48879c <rnd::RandomGenerator::~RandomGenerator()+0x98> @ imm = #-0x14c
  4888e4: ebfa1ed5     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1784ac
  4888e8: eaffffb6     	b	0x4887c8 <rnd::RandomGenerator::~RandomGenerator()+0xc4> @ imm = #-0x128
  4888ec: ebfa1ed3     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1784b4
  4888f0: eaffffbf     	b	0x4887f4 <rnd::RandomGenerator::~RandomGenerator()+0xf0> @ imm = #-0x104
  4888f4: ebfa1ed1     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1784bc
  4888f8: eaffffd2     	b	0x488848 <rnd::RandomGenerator::~RandomGenerator()+0x144> @ imm = #-0xb8
  4888fc: ebfa1ecf     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1784c4
  488900: eaffff9a     	b	0x488770 <rnd::RandomGenerator::~RandomGenerator()+0x6c> @ imm = #-0x198
