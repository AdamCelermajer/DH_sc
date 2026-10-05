
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

00488300 <rnd::RandomGenerator::UnloadRules()>:
  488300: e92d40f0     	push	{r4, r5, r6, r7, lr}
  488304: e1a05000     	mov	r5, r0
  488308: e5900128     	ldr	r0, [r0, #0x128]
  48830c: e24dd00c     	sub	sp, sp, #12
  488310: e3500000     	cmp	r0, #0
  488314: 0a000000     	beq	0x48831c <rnd::RandomGenerator::UnloadRules()+0x1c> @ imm = #0x0
  488318: ebfa16f4     	bl	0x30def0 <.plt+0x17c>   @ imm = #-0x17a430
  48831c: e5953124     	ldr	r3, [r5, #0x124]
  488320: e3a02000     	mov	r2, #0
  488324: e5852128     	str	r2, [r5, #0x128]
  488328: e1530002     	cmp	r3, r2
  48832c: 0a000003     	beq	0x488340 <rnd::RandomGenerator::UnloadRules()+0x40> @ imm = #0xc
  488330: e1a00003     	mov	r0, r3
  488334: e5933000     	ldr	r3, [r3]
  488338: e1a0e00f     	mov	lr, pc
  48833c: e593f004     	ldr	pc, [r3, #0x4]
  488340: e3a03000     	mov	r3, #0
  488344: e5853124     	str	r3, [r5, #0x124]
  488348: e595305c     	ldr	r3, [r5, #0x5c]
  48834c: e2856054     	add	r6, r5, #84
  488350: e28d7004     	add	r7, sp, #4
  488354: e1560003     	cmp	r6, r3
  488358: e1a01007     	mov	r1, r7
  48835c: e1a00006     	mov	r0, r6
  488360: 0a000018     	beq	0x4883c8 <rnd::RandomGenerator::UnloadRules()+0xc8> @ imm = #0x60
  488364: e5934014     	ldr	r4, [r3, #0x14]
  488368: e58d3004     	str	r3, [sp, #0x4]
  48836c: ebfff35f     	bl	0x4850f0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::erase(std::priv::_Rb_tree_iterator<std::pair<char const* const, rnd::ListRule*>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>>)> @ imm = #-0x3284
  488370: e3540000     	cmp	r4, #0
  488374: e284001c     	add	r0, r4, #28
  488378: 0a00000d     	beq	0x4883b4 <rnd::RandomGenerator::UnloadRules()+0xb4> @ imm = #0x34
  48837c: ebffffbf     	bl	0x488280 <std::vector<rnd::ListElem, std::allocator<rnd::ListElem>>::~vector()> @ imm = #-0x104
  488380: e5943014     	ldr	r3, [r4, #0x14]
  488384: e1530004     	cmp	r3, r4
  488388: e1a00003     	mov	r0, r3
  48838c: 0a000006     	beq	0x4883ac <rnd::RandomGenerator::UnloadRules()+0xac> @ imm = #0x18
  488390: e3530000     	cmp	r3, #0
  488394: 0a000004     	beq	0x4883ac <rnd::RandomGenerator::UnloadRules()+0xac> @ imm = #0x10
  488398: e5941000     	ldr	r1, [r4]
  48839c: e0631001     	rsb	r1, r3, r1
  4883a0: e3510080     	cmp	r1, #128
  4883a4: 8a000020     	bhi	0x48842c <rnd::RandomGenerator::UnloadRules()+0x12c> @ imm = #0x80
  4883a8: eb0a02d4     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x280b50
  4883ac: e1a00004     	mov	r0, r4
  4883b0: ebfa2022     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x177f78
  4883b4: e595305c     	ldr	r3, [r5, #0x5c]
  4883b8: e1a01007     	mov	r1, r7
  4883bc: e1a00006     	mov	r0, r6
  4883c0: e1560003     	cmp	r6, r3
  4883c4: 1affffe6     	bne	0x488364 <rnd::RandomGenerator::UnloadRules()+0x64> @ imm = #-0x68
  4883c8: e285006c     	add	r0, r5, #108
  4883cc: eb000e4b     	bl	0x48bd00 <rnd::RootRule::Unload()> @ imm = #0x392c
  4883d0: e5953044     	ldr	r3, [r5, #0x44]
  4883d4: e285603c     	add	r6, r5, #60
  4883d8: e1a0700d     	mov	r7, sp
  4883dc: e1560003     	cmp	r6, r3
  4883e0: e1a00006     	mov	r0, r6
  4883e4: e1a0100d     	mov	r1, sp
  4883e8: 0a00000d     	beq	0x488424 <rnd::RandomGenerator::UnloadRules()+0x124> @ imm = #0x34
  4883ec: e5934014     	ldr	r4, [r3, #0x14]
  4883f0: e58d3000     	str	r3, [sp]
  4883f4: ebfff34c     	bl	0x48512c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::erase(std::priv::_Rb_tree_iterator<std::pair<char const* const, rnd::Block*>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>>)> @ imm = #-0x32d0
  4883f8: e3540000     	cmp	r4, #0
  4883fc: e1a00004     	mov	r0, r4
  488400: 0a000002     	beq	0x488410 <rnd::RandomGenerator::UnloadRules()+0x110> @ imm = #0x8
  488404: e5943000     	ldr	r3, [r4]
  488408: e1a0e00f     	mov	lr, pc
  48840c: e593f004     	ldr	pc, [r3, #0x4]
  488410: e5953044     	ldr	r3, [r5, #0x44]
  488414: e1a00006     	mov	r0, r6
  488418: e1a0100d     	mov	r1, sp
  48841c: e1560003     	cmp	r6, r3
  488420: 1afffff1     	bne	0x4883ec <rnd::RandomGenerator::UnloadRules()+0xec> @ imm = #-0x3c
  488424: e28dd00c     	add	sp, sp, #12
  488428: e8bd80f0     	pop	{r4, r5, r6, r7, pc}
  48842c: ebfa2003     	bl	0x310440 <CustomFree(void*)> @ imm = #-0x177ff4
  488430: eaffffdd     	b	0x4883ac <rnd::RandomGenerator::UnloadRules()+0xac> @ imm = #-0x8c
