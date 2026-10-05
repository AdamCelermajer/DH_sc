
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004850f0 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::erase(std::priv::_Rb_tree_iterator<std::pair<char const* const, rnd::ListRule*>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>>)>:
  4850f0: e92d4010     	push	{r4, lr}
  4850f4: e1a04000     	mov	r4, r0
  4850f8: e2842008     	add	r2, r4, #8
  4850fc: e5910000     	ldr	r0, [r1]
  485100: e284300c     	add	r3, r4, #12
  485104: e2841004     	add	r1, r4, #4
  485108: ebfac3bd     	bl	0x336004 <std::priv::_Rb_global<bool>::_Rebalance_for_erase(std::priv::_Rb_tree_node_base*, std::priv::_Rb_tree_node_base*&, std::priv::_Rb_tree_node_base*&, std::priv::_Rb_tree_node_base*&)> @ imm = #-0x14f10c
  48510c: e3500000     	cmp	r0, #0
  485110: 0a000001     	beq	0x48511c <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::ListRule*>, std::priv::_Select1st<std::pair<char const* const, rnd::ListRule*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>, std::allocator<std::pair<char const* const, rnd::ListRule*>>>::erase(std::priv::_Rb_tree_iterator<std::pair<char const* const, rnd::ListRule*>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::ListRule*>>>)+0x2c> @ imm = #0x4
  485114: e3a01018     	mov	r1, #24
  485118: eb0a0f78     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x283de0
  48511c: e5943010     	ldr	r3, [r4, #0x10]
  485120: e2433001     	sub	r3, r3, #1
  485124: e5843010     	str	r3, [r4, #0x10]
  485128: e8bd8010     	pop	{r4, pc}
