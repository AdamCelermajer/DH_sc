
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

004850b8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_erase(std::priv::_Rb_tree_node_base*)>:
  4850b8: e92d4070     	push	{r4, r5, r6, lr}
  4850bc: e2514000     	subs	r4, r1, #0
  4850c0: e1a06000     	mov	r6, r0
  4850c4: 0a000008     	beq	0x4850ec <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_erase(std::priv::_Rb_tree_node_base*)+0x34> @ imm = #0x20
  4850c8: e594100c     	ldr	r1, [r4, #0xc]
  4850cc: e1a00006     	mov	r0, r6
  4850d0: ebfffff8     	bl	0x4850b8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_erase(std::priv::_Rb_tree_node_base*)> @ imm = #-0x20
  4850d4: e5945008     	ldr	r5, [r4, #0x8]
  4850d8: e1a00004     	mov	r0, r4
  4850dc: e3a01018     	mov	r1, #24
  4850e0: eb0a0f86     	bl	0x708f00 <___ZNSt12__node_alloc13_M_deallocateEPvj_veneer> @ imm = #0x283e18
  4850e4: e2554000     	subs	r4, r5, #0
  4850e8: 1afffff6     	bne	0x4850c8 <std::priv::_Rb_tree<char const*, lstr, std::pair<char const* const, rnd::Block*>, std::priv::_Select1st<std::pair<char const* const, rnd::Block*>>, std::priv::_MapTraitsT<std::pair<char const* const, rnd::Block*>>, std::allocator<std::pair<char const* const, rnd::Block*>>>::_M_erase(std::priv::_Rb_tree_node_base*)+0x10> @ imm = #-0x28
  4850ec: e8bd8070     	pop	{r4, r5, r6, pc}
