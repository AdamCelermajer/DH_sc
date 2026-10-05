
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488504 <rnd::RandomGenerator::~RandomGenerator()>:
  488504: e92d4070     	push	{r4, r5, r6, lr}
  488508: e1a04000     	mov	r4, r0
  48850c: ebffefa3     	bl	0x4843a0 <rnd::RandomGenerator::UnloadTiles()> @ imm = #-0x4174
  488510: e1a00004     	mov	r0, r4
  488514: ebffff79     	bl	0x488300 <rnd::RandomGenerator::UnloadRules()> @ imm = #-0x21c
  488518: e1a00004     	mov	r0, r4
  48851c: ebfff544     	bl	0x485a34 <rnd::RandomGenerator::UnloadRoomPools()> @ imm = #-0x2af0
  488520: e594318c     	ldr	r3, [r4, #0x18c]
  488524: e3530000     	cmp	r3, #0
  488528: 0a000005     	beq	0x488544 <rnd::RandomGenerator::~RandomGenerator()+0x40> @ imm = #0x14
  48852c: e1a00003     	mov	r0, r3
  488530: e5933000     	ldr	r3, [r3]
  488534: e1a0e00f     	mov	lr, pc
  488538: e593f004     	ldr	pc, [r3, #0x4]
  48853c: e3a03000     	mov	r3, #0
  488540: e584318c     	str	r3, [r4, #0x18c]
  488544: e2843f5d     	add	r3, r4, #372
  488548: e5930014     	ldr	r0, [r3, #0x14]
  48854c: e1500003     	cmp	r0, r3
  488550: 0a000006     	beq	0x488570 <rnd::RandomGenerator::~RandomGenerator()+0x6c> @ imm = #0x18
  488554: e3500000     	cmp	r0, #0
  488558: 0a000004     	beq	0x488570 <rnd::RandomGenerator::~RandomGenerator()+0x6c> @ imm = #0x10
  48855c: e5941174     	ldr	r1, [r4, #0x174]
  488560: e0601001     	rsb	r1, r0, r1
  488564: e3510080     	cmp	r1, #128
  488568: 8a000063     	bhi	0x4886fc <rnd::RandomGenerator::~RandomGenerator()+0x1f8> @ imm = #0x18c
  48856c: eb0a0263     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x28098c
  488570: e2843f57     	add	r3, r4, #348
  488574: e5930014     	ldr	r0, [r3, #0x14]
  488578: e1500003     	cmp	r0, r3
  48857c: 0a000006     	beq	0x48859c <rnd::RandomGenerator::~RandomGenerator()+0x98> @ imm = #0x18
  488580: e3500000     	cmp	r0, #0
  488584: 0a000004     	beq	0x48859c <rnd::RandomGenerator::~RandomGenerator()+0x98> @ imm = #0x10
  488588: e594115c     	ldr	r1, [r4, #0x15c]
  48858c: e0601001     	rsb	r1, r0, r1
  488590: e3510080     	cmp	r1, #128
  488594: 8a000050     	bhi	0x4886dc <rnd::RandomGenerator::~RandomGenerator()+0x1d8> @ imm = #0x140
  488598: eb0a0258     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280960
  48859c: e2843f51     	add	r3, r4, #324
  4885a0: e5930014     	ldr	r0, [r3, #0x14]
  4885a4: e1500003     	cmp	r0, r3
  4885a8: 0a000006     	beq	0x4885c8 <rnd::RandomGenerator::~RandomGenerator()+0xc4> @ imm = #0x18
  4885ac: e3500000     	cmp	r0, #0
  4885b0: 0a000004     	beq	0x4885c8 <rnd::RandomGenerator::~RandomGenerator()+0xc4> @ imm = #0x10
  4885b4: e5941144     	ldr	r1, [r4, #0x144]
  4885b8: e0601001     	rsb	r1, r0, r1
  4885bc: e3510080     	cmp	r1, #128
  4885c0: 8a000047     	bhi	0x4886e4 <rnd::RandomGenerator::~RandomGenerator()+0x1e0> @ imm = #0x11c
  4885c4: eb0a024d     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280934
  4885c8: e2843f4b     	add	r3, r4, #300
  4885cc: e5930014     	ldr	r0, [r3, #0x14]
  4885d0: e1500003     	cmp	r0, r3
  4885d4: 0a000006     	beq	0x4885f4 <rnd::RandomGenerator::~RandomGenerator()+0xf0> @ imm = #0x18
  4885d8: e3500000     	cmp	r0, #0
  4885dc: 0a000004     	beq	0x4885f4 <rnd::RandomGenerator::~RandomGenerator()+0xf0> @ imm = #0x10
  4885e0: e594112c     	ldr	r1, [r4, #0x12c]
  4885e4: e0601001     	rsb	r1, r0, r1
  4885e8: e3510080     	cmp	r1, #128
  4885ec: 8a00003e     	bhi	0x4886ec <rnd::RandomGenerator::~RandomGenerator()+0x1e8> @ imm = #0xf8
  4885f0: eb0a0242     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280908
  4885f4: e5940118     	ldr	r0, [r4, #0x118]
  4885f8: e2843f46     	add	r3, r4, #280
  4885fc: e3500000     	cmp	r0, #0
  488600: 0a000005     	beq	0x48861c <rnd::RandomGenerator::~RandomGenerator()+0x118> @ imm = #0x14
  488604: e5931008     	ldr	r1, [r3, #0x8]
  488608: e0601001     	rsb	r1, r0, r1
  48860c: e3c11003     	bic	r1, r1, #3
  488610: e3510080     	cmp	r1, #128
  488614: 8a00002e     	bhi	0x4886d4 <rnd::RandomGenerator::~RandomGenerator()+0x1d0> @ imm = #0xb8
  488618: eb0a0238     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2808e0
  48861c: e28430fc     	add	r3, r4, #252
  488620: e5930014     	ldr	r0, [r3, #0x14]
  488624: e1500003     	cmp	r0, r3
  488628: 0a000006     	beq	0x488648 <rnd::RandomGenerator::~RandomGenerator()+0x144> @ imm = #0x18
  48862c: e3500000     	cmp	r0, #0
  488630: 0a000004     	beq	0x488648 <rnd::RandomGenerator::~RandomGenerator()+0x144> @ imm = #0x10
  488634: e59410fc     	ldr	r1, [r4, #0xfc]
  488638: e0601001     	rsb	r1, r0, r1
  48863c: e3510080     	cmp	r1, #128
  488640: 8a00002b     	bhi	0x4886f4 <rnd::RandomGenerator::~RandomGenerator()+0x1f0> @ imm = #0xac
  488644: eb0a022d     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x2808b4
  488648: e284006c     	add	r0, r4, #108
  48864c: eb001413     	bl	0x48d6a0 <rnd::RootRule::~RootRule()> @ imm = #0x504c
  488650: e5943064     	ldr	r3, [r4, #0x64]
  488654: e3530000     	cmp	r3, #0
  488658: 1a000013     	bne	0x4886ac <rnd::RandomGenerator::~RandomGenerator()+0x1a8> @ imm = #0x4c
  48865c: e594304c     	ldr	r3, [r4, #0x4c]
  488660: e3530000     	cmp	r3, #0
  488664: 1a000003     	bne	0x488678 <rnd::RandomGenerator::~RandomGenerator()+0x174> @ imm = #0xc
  488668: e2840014     	add	r0, r4, #20
  48866c: ebfff475     	bl	0x485848 <std::deque<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>, std::allocator<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>>::~deque()> @ imm = #-0x2e2c
  488670: e1a00004     	mov	r0, r4
  488674: e8bd8070     	pop	{r4, r5, r6, pc}
  488678: e284503c     	add	r5, r4, #60
  48867c: e1a00005     	mov	r0, r5
  488680: e5941040     	ldr	r1, [r4, #0x40]
  488684: ebfff28b     	bl	0x4850b8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x35d4
  488688: e3a03000     	mov	r3, #0
  48868c: e5845048     	str	r5, [r4, #0x48]
  488690: e584304c     	str	r3, [r4, #0x4c]
  488694: e5845044     	str	r5, [r4, #0x44]
  488698: e5843040     	str	r3, [r4, #0x40]
  48869c: e2840014     	add	r0, r4, #20
  4886a0: ebfff468     	bl	0x485848 <std::deque<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>, std::allocator<std::deque<rnd::Tile*, std::allocator<rnd::Tile*>>>>::~deque()> @ imm = #-0x2e60
  4886a4: e1a00004     	mov	r0, r4
  4886a8: e8bd8070     	pop	{r4, r5, r6, pc}
  4886ac: e2845054     	add	r5, r4, #84
  4886b0: e1a00005     	mov	r0, r5
  4886b4: e5941058     	ldr	r1, [r4, #0x58]
  4886b8: ebfff1aa     	bl	0x484d68 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x3958
  4886bc: e3a03000     	mov	r3, #0
  4886c0: e5845060     	str	r5, [r4, #0x60]
  4886c4: e5843064     	str	r3, [r4, #0x64]
  4886c8: e584505c     	str	r5, [r4, #0x5c]
  4886cc: e5843058     	str	r3, [r4, #0x58]
  4886d0: eaffffe1     	b	0x48865c <rnd::RandomGenerator::~RandomGenerator()+0x158> @ imm = #-0x7c
  4886d4: ebfa1f59     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x17829c
  4886d8: eaffffcf     	b	0x48861c <rnd::RandomGenerator::~RandomGenerator()+0x118> @ imm = #-0xc4
  4886dc: ebfa1f57     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1782a4
  4886e0: eaffffad     	b	0x48859c <rnd::RandomGenerator::~RandomGenerator()+0x98> @ imm = #-0x14c
  4886e4: ebfa1f55     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1782ac
  4886e8: eaffffb6     	b	0x4885c8 <rnd::RandomGenerator::~RandomGenerator()+0xc4> @ imm = #-0x128
  4886ec: ebfa1f53     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1782b4
  4886f0: eaffffbf     	b	0x4885f4 <rnd::RandomGenerator::~RandomGenerator()+0xf0> @ imm = #-0x104
  4886f4: ebfa1f51     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1782bc
  4886f8: eaffffd2     	b	0x488648 <rnd::RandomGenerator::~RandomGenerator()+0x144> @ imm = #-0xb8
  4886fc: ebfa1f4f     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x1782c4
  488700: eaffff9a     	b	0x488570 <rnd::RandomGenerator::~RandomGenerator()+0x6c> @ imm = #-0x198
