
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

0048512c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::erase(std::priv::_Rb_tree_iterator<std::pair<char const* const, rnd::Block*>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>>)>:
  48512c: e92d4010     	push	{r4, lr}
  485130: e1a04000     	mov	r4, r0
  485134: e2842008     	add	r2, r4, #8
  485138: e5910000     	ldr	r0, [r1]
  48513c: e284300c     	add	r3, r4, #12
  485140: e2841004     	add	r1, r4, #4
  485144: ebfac3ae     	bl	0x336004 <std::priv::_Rb_global<bool>::_Rebalance_for_erase(std::priv::_Rb_tree_node_base*, std::priv::_Rb_tree_node_base*&, std::priv::_Rb_tree_node_base*&, std::priv::_Rb_tree_node_base*&)> @ imm = #-0x14f148
  485148: e3500000     	cmp	r0, #0
  48514c: 0a000001     	beq	0x485158 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::erase(std::priv::_Rb_tree_iterator<std::pair<char const* const, rnd::Block*>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>>)+0x2c> @ imm = #0x4
  485150: e3a01018     	mov	r1, #24
  485154: eb0a0f69     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x283da4
  485158: e5943010     	ldr	r3, [r4, #0x10]
  48515c: e2433001     	sub	r3, r3, #1
  485160: e5843010     	str	r3, [r4, #0x10]
  485164: e8bd8010     	pop	{r4, pc}
